/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"

template <class T>
class Buffer {
public:
    T* m_data;
    unsigned int m_capacity;

    Buffer();
    Buffer(unsigned int init_capacity);
    Buffer(const Buffer<T>& other);
    Buffer<T>& operator=(const Buffer<T>& other);
    ~Buffer();

    void ensure_capacity(unsigned int s);

    T* pat(unsigned int index) const;
    T& operator[](unsigned int index); // does not perform boundary checks
    const T& operator[](unsigned int index) const; // does not perform boundary checks
};


template <class T>
Buffer<T>::Buffer()
    : m_capacity(1)
{
    m_data = reinterpret_cast<T*>(malloc(sizeof(T) * m_capacity));
}

template <class T>
Buffer<T>::Buffer(unsigned int init_capacity)
    : m_capacity(init_capacity)
{
    if (m_capacity < 1)
        m_capacity = 1;
    m_data = reinterpret_cast<T*>(malloc(sizeof(T) * m_capacity));
}

template <class T>
Buffer<T>::Buffer(const Buffer<T>& other)
    : m_capacity(other.m_capacity)
{
    // Allocate and copy
    m_data = reinterpret_cast<T*>(malloc(sizeof(T) * m_capacity));
    memcpy(m_data, other.m_data, sizeof(T) * m_capacity);
}

template <class T>
Buffer<T>& Buffer<T>::operator=(const Buffer<T>& other) {
    if (&other != this) {
        // Deallocate
        free(m_data);

        // Assign new info
        m_capacity = other.m_capacity;

        // Allocate and copy
        m_data = reinterpret_cast<T*>(malloc(sizeof(T) * m_capacity));
        memcpy(m_data, other.m_data, sizeof(T) * m_capacity);

    }
    return *this;
}

template <class T>
Buffer<T>::~Buffer() {
    // Deallocate
    free(m_data);
}

template <class T>
void Buffer<T>::ensure_capacity(unsigned int s) {
    if (m_capacity < s) {
        T* orig_data = m_data;

        m_data = reinterpret_cast<T*>(malloc(sizeof(T) * s));
        memcpy(m_data, orig_data, sizeof(T) * m_capacity);

        free(orig_data);
        m_capacity = s;
    }
}

template <class T>
T* Buffer<T>::pat(unsigned int index) const {
    return m_data + index;
}

template <class T>
T& Buffer<T>::operator[](unsigned int index) {
    return m_data[index];
}

template <class T>
const T& Buffer<T>::operator[](unsigned int index) const {
    return m_data[index];
}

#endif  /* BUFFER_H */
