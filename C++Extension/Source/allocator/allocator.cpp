#include "allocator.h"

size_t Allocator::get_min_object_size() {
    return sizeof(Block);
}

Allocator::Allocator(size_t block_size)
    : m_num_object_in_use(0), m_num_objects_total(0)
{
    if (block_size < sizeof(long*))
        m_block_size = sizeof(long*);
    else
        m_block_size = block_size;
    m_top = nullptr;
}

Allocator::~Allocator() {
    Block* next_block;
    while (m_top) {
        next_block = m_top->m_next;
        delete[] reinterpret_cast<char*>(m_top);
        m_top = next_block;
    }
}

void* Allocator::allocate() {
    ++m_num_object_in_use;
    if (m_top) {
        Block* block = m_top;
        m_top = m_top->m_next;
        return reinterpret_cast<void*>(block);
    }
    else {
        ++m_num_objects_total;
        return reinterpret_cast<void*>( new char[m_block_size] );
    }
}

void Allocator::deallocate(void* ptr) {
    Block* last_top = m_top;
    m_top = reinterpret_cast<Block*>(ptr);
    m_top->m_next = last_top;
    --m_num_object_in_use;
}

void Allocator::clean() {
    Block* next_block;
    while (m_top) {
        next_block = m_top->m_next;
        delete[] reinterpret_cast<char*>(m_top);
        m_top = next_block;
        --m_num_objects_total;
    }
}

size_t Allocator::get_num_blocks_in_use() const {
    return m_num_object_in_use;
}

size_t Allocator::get_num_blocks_total() const {
    return m_num_objects_total;
}

size_t Allocator::get_num_blocks_idle() const {
    return m_num_objects_total - m_num_object_in_use;
}

size_t Allocator::get_block_size() const {
    return m_block_size;
}
