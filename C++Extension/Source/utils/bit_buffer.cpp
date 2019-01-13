/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "bit_buffer.h"

BitBuffer::BitBuffer() :
    m_cap(1)
{
    m_dat = (char*)malloc(sizeof(char) * m_cap);
    memset(m_dat, 0, sizeof(char) * m_cap);
}

BitBuffer::BitBuffer(size_t init_cap) :
    m_cap(init_cap)
{
    if (m_cap == 0) m_cap = 1;
    m_dat = (char*)malloc(sizeof(char) * m_cap);
    memset(m_dat, 0, sizeof(char) * m_cap);
}

BitBuffer::BitBuffer(const BitBuffer& other) :
    m_cap(other.m_cap)
{
    m_dat = (char*)malloc(sizeof(char) * m_cap);
    memcpy(m_dat, other.m_dat, sizeof(char) * m_cap);
}

BitBuffer& BitBuffer::operator=(const BitBuffer& other) {
    if (&other != this) {
        free(m_dat);
        m_cap = other.m_cap;
        m_dat = (char*)malloc(sizeof(char) * m_cap);
        memcpy(m_dat, other.m_dat, sizeof(char) * m_cap);
    }
    return *this;
}

BitBuffer::~BitBuffer() {
    free(m_dat);
}

void BitBuffer::ensure_capacity(size_t bc) {
    size_t c = (bc >> 3) + 1;
    if (m_cap < c) {
        char* orig_data = m_dat;

        m_dat = reinterpret_cast<char*>(malloc(sizeof(char) * c));
        memcpy(m_dat, orig_data, sizeof(char) * m_cap);
        memset(m_dat + m_cap, 0, sizeof(char) * (c - m_cap));

        free(orig_data);
        m_cap = c;
    }
}

void BitBuffer::ensure_capacity_pow2(size_t bc) {
    size_t c = (bc >> 3) + 1;
    if (m_cap < c) {
        char* orig_data = m_dat;
        size_t orig_cap = m_cap;

        m_cap = 1;
        while (m_cap < c) m_cap <<= 1;

        m_dat = reinterpret_cast<char*>(malloc(sizeof(char) * m_cap));
        memcpy(m_dat, orig_data, sizeof(char) * orig_cap);
        memset(m_dat + orig_cap, 0, sizeof(char) * (m_cap - orig_cap));

        free(orig_data);
        m_cap = c;
    }
}

void BitBuffer::clear() {
    memset(m_dat, 0, sizeof(char) * m_cap);
}

void BitBuffer::partial_clear(size_t n) {
    // clear bytes
    size_t j = n >> 3;
    memset(m_dat, 0, sizeof(char) * j);
    // clear leftover, individual bits
    size_t r = n - (j << 3);
    size_t k;
    for (k = 0; k < r; ++k) {
        m_dat[j] &= ~(1 << k);
    }
}

void BitBuffer::reset() {
    m_cap = 1;
    free(m_dat);
    m_dat = (char*)malloc(sizeof(char) * m_cap);
    memset(m_dat, 0, sizeof(char) * m_cap);
}
