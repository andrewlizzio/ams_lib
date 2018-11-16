#include "xallocator.h"
#include "allocator.h"
#include "fault.h"

#if WIN32
	#include "windows.h"
#endif

#ifndef CHAR_BIT
    #define CHAR_BIT 8
#endif

#define MAX_ALLOCATORS 32

static CRITICAL_SECTION _critical_section;
static bool _xalloc_initialized = false;
static Allocator* _allocators[MAX_ALLOCATORS];

// For C++ applications, must define AUTOMATIC_XALLOCATOR_INIT_DESTROY to
// correctly ensure allocators are initialized before any static user C++
// construtor/destructor executes which might call into the xallocator API.
// This feature costs 1-byte of RAM per C++ translation unit. This feature
// can be disabled only under the following circumstances:
//
// 1) The xallocator is only used within C files.
// 2) STATIC_POOLS is undefined and the application never exits main (e.g.
// an embedded system).
//
// In either of the two cases above, call xalloc_init() in main at startup,
// and xalloc_destroy() before main exits. In all other situations
// XallocInitDestroy must be used to call xalloc_init() and xalloc_destroy().
#ifdef AUTOMATIC_XALLOCATOR_INIT_DESTROY
int XallocInitDestroy::ref_count = 0;
XallocInitDestroy::XallocInitDestroy()
{
    // Track how many static instances of XallocInitDestroy are created
    if (ref_count++ == 0)
        xalloc_init();
}

XallocInitDestroy::~XallocInitDestroy()
{
    // Last static instance to have destructor called?
    if (--ref_count == 0)
        xalloc_destroy();
}
#endif  // AUTOMATIC_XALLOCATOR_INIT_DESTROY

/// Returns the next higher powers of two. For instance, pass in 12 and
/// the value returned would be 16.
/// @param[in] k - numeric value to compute the next higher power of two.
/// @return The next higher power of two based on the input k.
template <class T>
T nexthigher(T k)
{
    k--;
    for (size_t i = 1; i < sizeof(T) * CHAR_BIT; i <<= 1)
        k |= (k >> i);
    return k + 1;
}

/// Create the xallocator lock. Call only one time at startup.
static void lock_init()
{
    BOOL success = InitializeCriticalSectionAndSpinCount(&_critical_section, 0x00000400);
    ASSERT_TRUE(success != 0);
    _xalloc_initialized = true;
}

/// Destroy the xallocator lock.
static void lock_destroy()
{
    DeleteCriticalSection(&_critical_section);
    _xalloc_initialized = false;
}

/// Lock the shared resource.
static inline void lock_get()
{
    if (_xalloc_initialized == false)
        return;

    EnterCriticalSection(&_critical_section);
}

/// Unlock the shared resource.
static inline void lock_release()
{
    if (_xalloc_initialized == false)
        return;

    LeaveCriticalSection(&_critical_section);
}

/// Stored a pointer to the allocator instance within the block region.
/// a pointer to the client's area within the block.
/// @param[in] block - a pointer to the raw memory block.
/// @param[in] size - the client requested size of the memory block.
/// @return A pointer to the client's address within the raw memory block.
static inline void *set_block_allocator(void* block, Allocator* allocator)
{
    // Cast the raw block memory to a Allocator pointer
    Allocator** pAllocatorInBlock = static_cast<Allocator**>(block);

    // Write the size into the memory block
    *pAllocatorInBlock = allocator;

    // Advance the pointer past the Allocator* block size and return a pointer to
    // the client's memory region
    return ++pAllocatorInBlock;
}

/// Gets the size of the memory block stored within the block.
/// @param[in] block - a pointer to the client's memory block.
/// @return The original allocator instance stored in the memory block.
static inline Allocator* get_block_allocator(void* block)
{
    // Cast the client memory to a Allocator pointer
    Allocator** pAllocatorInBlock = static_cast<Allocator**>(block);

    // Back up one Allocator* position to get the stored allocator instance
    pAllocatorInBlock--;

    // Return the allocator instance stored within the memory block
    return *pAllocatorInBlock;
}

/// Returns the raw memory block pointer given a client memory pointer.
/// @param[in] block - a pointer to the client memory block.
/// @return A pointer to the original raw memory block address.
static inline void *get_block_ptr(void* block)
{
    // Cast the client memory to a Allocator* pointer
    Allocator** pAllocatorInBlock = static_cast<Allocator**>(block);

    // Back up one Allocator* position and return the original raw memory block pointer
    return --pAllocatorInBlock;
}

/// Returns an allocator instance matching the size provided
/// @param[in] size - allocator block size
/// @return Allocator instance handling requested block size or nullptr
/// if no allocator exists.
static inline Allocator* find_allocator(size_t size)
{
    for (unsigned int i = 0; i < MAX_ALLOCATORS; ++i)
    {
        if (_allocators[i] == nullptr)
            break;
        if (_allocators[i]->get_block_size() == size)
            return _allocators[i];
    }

    return nullptr;
}

/// Insert an allocator instance into the array
/// @param[in] allocator - An allocator instance
static inline void insert_allocator(Allocator* allocator)
{
    for (unsigned int i = 0; i < MAX_ALLOCATORS; ++i)
    {
        if (_allocators[i] == nullptr)
        {
            _allocators[i] = allocator;
            return;
        }
    }

    ASSERT();
}

/// This function must be called exactly one time *before* any other xallocator
/// API is called. XallocInitDestroy constructor calls this function automatically.
extern "C" void xalloc_init()
{
    lock_init();

    for (unsigned int i = 0; i < MAX_ALLOCATORS; ++i) {
        _allocators[i] = nullptr;
    }
}

/// Called one time when the application exits to cleanup any allocated memory.
/// ~XallocInitDestroy destructor calls this function automatically.
extern "C" void xalloc_destroy()
{
    lock_get();

    for (unsigned int i = 0; i < MAX_ALLOCATORS; ++i)
    {
        if (_allocators[i] == nullptr)
            break;
        delete _allocators[i];
        _allocators[i] = nullptr;
    }

    lock_release();

    lock_destroy();
}

/// Get an Allocator instance based upon the client's requested block size.
/// If a Allocator instance is not currently available to handle the size,
/// then a new Allocator instance is create.
/// @param[in] size - the client's requested block size.
/// @return An Allocator instance that handles blocks of the requested
/// size.
extern "C" Allocator* xallocator_get_allocator(size_t size)
{
    // Based on the size, find the next higher powers of two value.
    // Add sizeof(Allocator*) to the requested block size to hold the size
    // within the block memory region. Most blocks are powers of two,
    // however some common allocator block sizes can be explicitly defined
    // to minimize wasted storage. This offers application specific tuning.
    size_t block_size = nexthigher<size_t>(size + sizeof(Allocator*));

    Allocator* allocator = find_allocator(block_size);

    // If there is not an allocator already created to handle this block size
    if (allocator == nullptr)
    {
        // Create a new allocator to handle blocks of the size required
        allocator = new Allocator(block_size);

        // Insert allocator into array
        insert_allocator(allocator);
    }

    return allocator;
}

/// Allocates a memory block of the requested size. The blocks are created from
/// the fixed block allocators.
/// @param[in] size - the client requested size of the block.
/// @return A pointer to the client's memory block.
extern "C" void *xmalloc(size_t size)
{
    lock_get();

    // Allocate a raw memory block
    Allocator* allocator = xallocator_get_allocator(size);
    void* block_memory_ptr = allocator->allocate();

    lock_release();

    // Set the block Allocator* within the raw memory block region
    void* clients_memory_ptr = set_block_allocator(block_memory_ptr, allocator);
    return clients_memory_ptr;
}

/// Frees a memory block previously allocated with xalloc. The blocks are returned
/// to the fixed block allocator that originally created it.
/// @param[in] ptr - a pointer to a block created with xalloc.
extern "C" void xfree(void* ptr)
{
    if (ptr == 0)
        return;

    // Extract the original allocator instance from the caller's block pointer
    Allocator* allocator = get_block_allocator(ptr);

    // Convert the client pointer into the original raw block pointer
    void* block_ptr = get_block_ptr(ptr);

    lock_get();

    // Deallocate the block
    allocator->deallocate(block_ptr);

    lock_release();
}

/// Reallocates a memory block previously allocated with xalloc.
/// @param[in] ptr - a pointer to a block created with xalloc.
/// @param[in] size - the client requested block size to create.
extern "C" void *xrealloc(void *old_mem, size_t size)
{
    if (old_mem == 0)
        return xmalloc(size);

    if (size == 0)
    {
        xfree(old_mem);
        return 0;
    }
    else
    {
        // Create a new memory block
        void* new_mem = xmalloc(size);
        if (new_mem != 0)
        {
            // Get the original allocator instance from the old memory block
            Allocator* old_allocator = get_block_allocator(old_mem);
            size_t old_size = old_allocator->get_block_size() - sizeof(Allocator*);

            // Copy the bytes from the old memory block into the new (as much as will fit)
            memcpy(new_mem, old_mem, (old_size < size) ? old_size : size);

            // Free the old memory block
            xfree(old_mem);

            // Return the client pointer to the new memory block
            return new_mem;
        }
        return 0;
    }
}

extern "C" void xalloc_clean()
{
    for (unsigned int i = 0; i < MAX_ALLOCATORS; ++i)
    {
        if (_allocators[i] == nullptr)
            break;
        _allocators[i]->clean();
    }
}

extern "C" size_t xalloc_get_total_memory_used() {
	size_t num_bytes = 0;
	for (unsigned int i = 0; i < MAX_ALLOCATORS; ++i)
	{
		if (_allocators[i] == nullptr)
			break;
		num_bytes += _allocators[i]->get_num_blocks_total() * _allocators[i]->get_block_size();
	}
	return num_bytes;
}
