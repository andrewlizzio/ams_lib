/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef GEOM_BOUNDING_BOX_H
#define GEOM_BOUNDING_BOX_H

#include "geom.h"
#include "geom_vector3d.h"

class Geom::BoundingBox
{
public:
    // Constants
    static const treal MIN_VALUE;
    static const treal MAX_VALUE;

    // Variables
    Vector3d m_min, m_max;

    // Constructors
    BoundingBox();
    BoundingBox(const BoundingBox& other);
    BoundingBox(const Geom::Vector3d& min, const Geom::Vector3d& max);

    // Operators
    BoundingBox& operator=(const BoundingBox& other);

    // Functions
    BoundingBox& add(const Vector3d& point);
    BoundingBox& add(const BoundingBox& other);
    BoundingBox& add(const Geom::Vector3d& min, const Geom::Vector3d& max);
    BoundingBox& scale_self(const treal& scale);
    BoundingBox scale(const treal& scale) const;
    BoundingBox& product_self(const Geom::Vector3d& scale);
    BoundingBox product(const Geom::Vector3d& scale) const;
    void intersect(const BoundingBox& other, Geom::BoundingBox& bb_out) const;
    bool is_point_inside(const Vector3d& point) const;
    bool overlaps_with(const BoundingBox& other) const;
    bool is_within(const BoundingBox& other) const;
    bool intersects_ray(const Geom::Vector3d& ray_point, const Geom::Vector3d& ray_vector) const;
    bool intersects_ray(const Geom::Vector3d& ray_point, const Geom::Vector3d& ray_vector, treal cone_angle) const; // Assuming ray_vector is normal
    treal get_width() const;
    treal get_height() const;
    treal get_depth() const;
    treal get_diagonal() const;
    treal get_min_max_difference_at(unsigned int axis) const;
    treal get_min_max_sum_at(unsigned int axis) const;
    treal get_center_at(unsigned int axis) const;
    void get_center(Geom::Vector3d& center_out) const;
    void get_corner(unsigned int i, Geom::Vector3d& corner_out) const;
    void clear();
    void pad_out(treal value);
    bool is_valid() const;
    bool is_invalid() const;
};


// Define inline functions

inline treal Geom::BoundingBox::get_width() const {
    return m_max.m_x - m_min.m_x;
}

inline treal Geom::BoundingBox::get_height() const {
    return m_max.m_y - m_min.m_y;
}

inline treal Geom::BoundingBox::get_depth() const {
    return m_max.m_z - m_min.m_z;
}

inline treal Geom::BoundingBox::get_diagonal() const {
    return (m_max - m_min).get_length();
}

inline treal Geom::BoundingBox::get_min_max_difference_at(unsigned int axis) const {
    return m_max[axis] - m_min[axis];
}

inline treal Geom::BoundingBox::get_min_max_sum_at(unsigned int axis) const {
    return m_min[axis] + m_max[axis];
}

inline treal Geom::BoundingBox::get_center_at(unsigned int axis) const {
    return (m_min[axis] + m_max[axis]) * (treal)(0.5);
}

inline void Geom::BoundingBox::get_center(Geom::Vector3d& center_out) const {
    center_out = (m_min + m_max).scale(0.5);
}

inline bool Geom::BoundingBox::is_valid() const {
    return m_max.m_x >= m_min.m_x && m_max.m_y >= m_min.m_y && m_max.m_z >= m_min.m_z;
}

inline bool Geom::BoundingBox::is_invalid() const {
    return m_max.m_x < m_min.m_x || m_max.m_y < m_min.m_y || m_max.m_z < m_min.m_z;
}


#endif /* GEOM_BOUNDING_BOX_H */
