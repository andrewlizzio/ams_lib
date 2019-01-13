/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef GEOM_TRANSFORMATION_H
#define GEOM_TRANSFORMATION_H

#include "geom_vector4d.h"

Geom::Transformation operator * (const Geom::Transformation& a, const Geom::Transformation& b);
Geom::Vector4d operator * (const Geom::Transformation& t, const Geom::Vector4d& v);

class Geom::Transformation
{
public:
    // Variables
    Vector4d m_xaxis, m_yaxis, m_zaxis, m_origin;

    // Constants
    static const Transformation IDENTITY;

    // Constructors
    Transformation();
    Transformation(const Vector3d& origin, const Vector3d& zaxis);
    Transformation(const Vector3d& origin);
    Transformation(const Vector3d& xaxis, const Vector3d& yaxis, const Vector3d& zaxis);
    Transformation(const Vector3d& xaxis, const Vector3d& yaxis, const Vector3d& zaxis, const Vector3d& origin);
    Transformation(const Vector4d& xaxis, const Vector4d& yaxis, const Vector4d& zaxis, const Vector4d& origin);

    Transformation(const Transformation& other);
    Transformation(const Quaternion& rotation, Geom::Vector3d position);
    Transformation(const treal* matrix);
    Transformation(const Geom::Vector3d& origin, const Geom::Vector3d& normal, treal angle);

    static Transformation rotate(const Geom::Vector3d& axis, treal angle);
    static Transformation rotatex(treal angle);
    static Transformation rotatey(treal angle);
    static Transformation rotatez(treal angle);

    // Operators
    friend Geom::Transformation (::operator *) (const Geom::Transformation& a, const Geom::Transformation& b);
    friend Geom::Vector4d (::operator *) (const Geom::Transformation& t, const Geom::Vector4d& v);

    Transformation& operator = (const Transformation& other);
    Vector4d& operator [] (const int index);
    const Vector4d& operator [] (const int index) const;

    // Functions
    Transformation inverse() const;
    Vector3d get_scale() const;
    Transformation& set_scale(const Vector3d& scale);
    Transformation& normalize_self();
    Transformation normalize() const;
    Transformation& scale_axes_self(const Vector3d& scale);
    Transformation scale_axes(const Vector3d& scale) const;
    Transformation rotate_xaxis_to(const Vector3d& dir) const;
    Transformation rotate_yaxis_to(const Vector3d& dir) const;
    Transformation rotate_zaxis_to(const Vector3d& dir) const;
    Transformation uniform_normal_transition_to(const Transformation& other, treal ratio) const;
    Transformation uniform_transition_to(const Transformation& other, treal ratio) const;
    Vector3d transform_vector(const Vector3d& v) const;
    Vector3d transform_vector(const Vector3d& v, treal det) const;
    Vector3d transform_vector2(const Vector3d& v) const;
    Vector3d rotate_vector(const Vector3d& v) const;

    treal get_determinant() const;
    void extract_w_factor();

    void get_normal_xaxis(Geom::Vector3d& res) const;
    void get_normal_yaxis(Geom::Vector3d& res) const;
    void get_normal_zaxis(Geom::Vector3d& res) const;
    void get_normal_origin(Geom::Vector3d& res) const;

    bool is_uniform() const;
    bool is_flipped() const;
    bool is_flat() const;

    void zero_out();
};


// Define inline functions

inline Geom::Vector4d& Geom::Transformation::operator [] (const int index) {
    return (&m_xaxis)[index];
}

inline const Geom::Vector4d& Geom::Transformation::operator [] (const int index) const {
    return (&m_xaxis)[index];
}

inline void Geom::Transformation::get_normal_xaxis(Geom::Vector3d& res) const {
    res = m_xaxis.normalize();
}

inline void Geom::Transformation::get_normal_yaxis(Geom::Vector3d& res) const {
    res = m_yaxis.normalize();
}

inline void Geom::Transformation::get_normal_zaxis(Geom::Vector3d& res) const {
    res = m_zaxis.normalize();
}

inline bool Geom::Transformation::is_uniform() const {
    return fabs(m_xaxis.dot(m_yaxis)) < M_EPSILON2 && fabs(m_xaxis.dot(m_zaxis)) < M_EPSILON2 && fabs(m_yaxis.dot(m_zaxis)) < M_EPSILON2;
}

inline bool Geom::Transformation::is_flipped() const {
    return (m_xaxis.cross(m_yaxis)).dot(m_zaxis) < (treal)(0.0);
}

inline bool Geom::Transformation::is_flat() const {
    return m_xaxis.get_length() < M_EPSILON || m_yaxis.get_length() < M_EPSILON || m_zaxis.get_length() < M_EPSILON;
}

#endif /* GEOM_TRANSFORMATION_H */
