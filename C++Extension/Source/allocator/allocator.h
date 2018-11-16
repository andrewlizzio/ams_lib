#ifndef ALLOCATOR_H
#define ALLOCATOR_H

class Allocator {
private:
    // Disable copy constructor and assignment operator
    Allocator(const Allocator& other);
    Allocator& operator=(const Allocator& other);

    // Structures
    struct Block {
        Block* m_next;
    };

    // Variables
    Block* m_top;
    size_t m_block_size; // in bytes
    size_t m_num_object_in_use;
    size_t m_num_objects_total;

public:

    // Static Functions
    static size_t get_min_object_size();

    // Functions
    Allocator(size_t block_size);
    virtual ~Allocator();

    void* allocate();
    void deallocate(void* object);
    void clean();

    size_t get_num_blocks_in_use() const;
    size_t get_num_blocks_total() const;
    size_t get_num_blocks_idle() const;
    size_t get_block_size() const;
};

#endif /* ALLOCATOR_H */
