/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef BIT_BUFFER_H
#define BIT_BUFFER_H

#include "common.h"

class BitBuffer {
protected:
    char* m_dat;
    size_t m_cap; // in bytes

public:
    BitBuffer();
    BitBuffer(size_t init_cap);
    BitBuffer(const BitBuffer& other);
    BitBuffer& operator=(const BitBuffer& other);
    virtual ~BitBuffer();

    size_t capacity() const; // in bits

    void ensure_capacity(size_t bc); // in bits
    void ensure_capacity_pow2(size_t bc); // in bits

    bool get_at(size_t i) const;
    void set_at(size_t i);
    void unset_at(size_t i);
    void toggle_at(size_t i);
    void clear();
    void partial_clear(size_t n); // clear first n bits
    void reset();
};

inline size_t BitBuffer::capacity() const {
    return m_cap << 3;
}

inline bool BitBuffer::get_at(size_t i) const {
    size_t j = i >> 3;
    size_t r = i - (j << 3);
    return (m_dat[j] >> r) & 1;
}

inline void BitBuffer::set_at(size_t i) {
    size_t j = i >> 3;
    size_t r = i - (j << 3);
    m_dat[j] |= 1 << r;
}

inline void BitBuffer::unset_at(size_t i) {
    size_t j = i >> 3;
    size_t r = i - (j << 3);
    m_dat[j] &= ~(1 << r);
}

inline void BitBuffer::toggle_at(size_t i) {
    size_t j = i >> 3;
    size_t r = i - (j << 3);
    m_dat[j] ^= 1 << r;
}

#endif  /* BIT_BUFFER_H */
