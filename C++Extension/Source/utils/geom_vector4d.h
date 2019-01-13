/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef GEOM_VECTOR4D_H
#define GEOM_VECTOR4D_H

#include "geom_vector3d.h"

bool operator == (const Geom::Vector4d& lhs, const Geom::Vector4d& rhs);
bool operator != (const Geom::Vector4d& lhs, const Geom::Vector4d& rhs);

Geom::Vector4d operator * (Geom::Vector4d v, treal scalar);
Geom::Vector4d operator / (Geom::Vector4d v, treal scalar);
Geom::Vector4d operator * (treal scalar, Geom::Vector4d v);
Geom::Vector4d operator + (Geom::Vector4d lhs, const Geom::Vector4d& rhs);
Geom::Vector4d operator - (Geom::Vector4d lhs, const Geom::Vector4d& rhs);

class Geom::Vector4d : public Vector3d
{
public:
    // Variables
    treal m_w;

    // Constructors
    Vector4d();
    Vector4d(treal value);
    Vector4d(const Vector4d& other);
    Vector4d(treal x, treal y, treal z, treal w);
    Vector4d(const treal* values);

    // Operators
    friend bool(::operator ==) (const Geom::Vector4d& lhs, const Geom::Vector4d& rhs);
    friend bool(::operator !=) (const Geom::Vector4d& lhs, const Geom::Vector4d& rhs);

    friend Geom::Vector4d(::operator *) (Geom::Vector4d v, treal scalar);
    friend Geom::Vector4d(::operator /) (Geom::Vector4d v, treal scalar);
    friend Geom::Vector4d(::operator *) (treal scalar, Geom::Vector4d v);
    friend Geom::Vector4d(::operator +) (Geom::Vector4d lhs, const Geom::Vector4d& rhs);
    friend Geom::Vector4d(::operator -) (Geom::Vector4d lhs, const Geom::Vector4d& rhs);

    Vector4d& operator=(const Vector4d& other);
    Vector4d& operator=(const Vector3d& other);

    Vector4d& operator *= (treal scalar);
    Vector4d& operator /= (treal scalar);
    Vector4d& operator *= (const Vector4d& other);
    Vector4d& operator /= (const Vector4d& other);
    Vector4d& operator += (const Vector4d& other);
    Vector4d& operator -= (const Vector4d& other);
};


// Define inline functions

inline bool operator == (const Geom::Vector4d& lhs, const Geom::Vector4d& rhs) {
    return (fabs(lhs.m_x - rhs.m_x) < M_EPSILON &&
        fabs(lhs.m_y - rhs.m_y) < M_EPSILON &&
        fabs(lhs.m_z - rhs.m_z) < M_EPSILON &&
        fabs(lhs.m_w - rhs.m_w) < M_EPSILON);
}

inline bool operator != (const Geom::Vector4d& lhs, const Geom::Vector4d& rhs) {
    return !(lhs == rhs);
}

#endif /* GEOM_VECTOR4D_H */
