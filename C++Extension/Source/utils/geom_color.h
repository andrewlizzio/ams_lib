/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef GEOM_COLOR_H
#define GEOM_COLOR_H

#include "geom.h"

bool operator == (const Geom::Color& lhs, const Geom::Color& rhs);
bool operator != (const Geom::Color& lhs, const Geom::Color& rhs);

class Geom::Color
{
public:
    // Variables
    unsigned char m_r, m_g, m_b, m_a;

    // Constructors
    Color();
    Color(const Color& other);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

    // Operators
    Color& operator=(const Color& other);

    friend bool(::operator ==) (const Geom::Color& lhs, const Geom::Color& rhs);
    friend bool(::operator !=) (const Geom::Color& lhs, const Geom::Color& rhs);

    // Functions
    Color transition_to(const Color& other, treal ratio);
};


// Define inline functions

inline bool operator == (const Geom::Color& lhs, const Geom::Color& rhs) {
    return (lhs.m_r == rhs.m_r && lhs.m_g == rhs.m_g && lhs.m_b == rhs.m_b && lhs.m_a == rhs.m_a);
}

inline bool operator != (const Geom::Color& lhs, const Geom::Color& rhs) {
    return !(lhs == rhs);
}

#endif /* GEOM_COLOR_H */
