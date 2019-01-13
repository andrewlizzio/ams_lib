/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "geom_bounding_box.h"
#include "geom_vector3d.h"
#include "geom_transformation.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Constants
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

const treal Geom::BoundingBox::MIN_VALUE = (treal)(-1.0e15);
const treal Geom::BoundingBox::MAX_VALUE = (treal)(1.0e15);


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Constructors
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

Geom::BoundingBox::BoundingBox() :
    m_min(MAX_VALUE),
    m_max(MIN_VALUE)
{
}

Geom::BoundingBox::BoundingBox(const BoundingBox& other) :
    m_min(other.m_min),
    m_max(other.m_max)
{
}

Geom::BoundingBox::BoundingBox(const Geom::Vector3d& min, const Geom::Vector3d& max) :
    m_min(min),
    m_max(max)
{
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Operators
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

Geom::BoundingBox& Geom::BoundingBox::operator=(const BoundingBox& other) {
    if (this != &other) {
        m_min = other.m_min;
        m_max = other.m_max;
    }
    return *this;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

Geom::BoundingBox& Geom::BoundingBox::add(const Vector3d& point) {
    Geom::min_treal2(m_min.m_x, point.m_x);
    Geom::min_treal2(m_min.m_y, point.m_y);
    Geom::min_treal2(m_min.m_z, point.m_z);
    Geom::max_treal2(m_max.m_x, point.m_x);
    Geom::max_treal2(m_max.m_y, point.m_y);
    Geom::max_treal2(m_max.m_z, point.m_z);
    return *this;
}

Geom::BoundingBox& Geom::BoundingBox::add(const BoundingBox& other) {
    Geom::min_treal2(m_min.m_x, other.m_min.m_x);
    Geom::min_treal2(m_min.m_y, other.m_min.m_y);
    Geom::min_treal2(m_min.m_z, other.m_min.m_z);
    Geom::max_treal2(m_max.m_x, other.m_max.m_x);
    Geom::max_treal2(m_max.m_y, other.m_max.m_y);
    Geom::max_treal2(m_max.m_z, other.m_max.m_z);
    return *this;
}

Geom::BoundingBox& Geom::BoundingBox::add(const Geom::Vector3d& min, const Geom::Vector3d& max) {
    Geom::min_treal2(m_min.m_x, min.m_x);
    Geom::min_treal2(m_min.m_y, min.m_y);
    Geom::min_treal2(m_min.m_z, min.m_z);
    Geom::max_treal2(m_max.m_x, max.m_x);
    Geom::max_treal2(m_max.m_y, max.m_y);
    Geom::max_treal2(m_max.m_z, max.m_z);
    return *this;
}

Geom::BoundingBox& Geom::BoundingBox::scale_self(const treal& scale) {
    m_min.scale_self(scale);
    m_max.scale_self(scale);
    return *this;
}

Geom::BoundingBox Geom::BoundingBox::scale(const treal& scale) const {
    Geom::BoundingBox bb(*this);
    bb.scale_self(scale);
    return bb;
}

Geom::BoundingBox& Geom::BoundingBox::product_self(const Geom::Vector3d& scale) {
    m_min.product_self(scale);
    m_max.product_self(scale);
    return *this;
}

Geom::BoundingBox Geom::BoundingBox::product(const Geom::Vector3d& scale) const {
    Geom::BoundingBox bb(*this);
    bb.product_self(scale);
    return bb;
}

void Geom::BoundingBox::intersect(const BoundingBox& other, Geom::BoundingBox& bb_out) const {
    bb_out.m_min.m_x = Geom::max_treal(m_min.m_x, other.m_min.m_x);
    bb_out.m_min.m_y = Geom::max_treal(m_min.m_y, other.m_min.m_y);
    bb_out.m_min.m_z = Geom::max_treal(m_min.m_z, other.m_min.m_z);
    bb_out.m_max.m_x = Geom::min_treal(m_max.m_x, other.m_max.m_x);
    bb_out.m_max.m_y = Geom::min_treal(m_max.m_y, other.m_max.m_y);
    bb_out.m_max.m_z = Geom::min_treal(m_max.m_z, other.m_max.m_z);
}

bool Geom::BoundingBox::is_point_inside(const Vector3d& point) const {
    return (point.m_x > m_min.m_x && point.m_x < m_max.m_x &&
            point.m_y > m_min.m_y && point.m_y < m_max.m_y &&
            point.m_z > m_min.m_z && point.m_z < m_max.m_z);
}

bool Geom::BoundingBox::overlaps_with(const BoundingBox& other) const {
    return (other.m_max.m_x > m_min.m_x && other.m_min.m_x < m_max.m_x &&
            other.m_max.m_y > m_min.m_y && other.m_min.m_y < m_max.m_y &&
            other.m_max.m_z > m_min.m_z && other.m_min.m_z < m_max.m_z);
}

bool Geom::BoundingBox::is_within(const BoundingBox& other) const {
    return (m_min.m_x > other.m_min.m_x && m_max.m_x < other.m_max.m_x &&
            m_min.m_y > other.m_min.m_y && m_max.m_y < other.m_max.m_y &&
            m_min.m_z > other.m_min.m_z && m_max.m_z < other.m_max.m_z);
}

bool Geom::BoundingBox::intersects_ray(const Geom::Vector3d& ray_point, const Geom::Vector3d& ray_vector) const {
    if (is_point_inside(ray_point)) return true;

    Geom::Vector3d v1, v2, pt;
    treal si;

    v1 = m_min - ray_point;
    v2 = m_max - ray_point;

    // X-axis normal
    if (fabs(ray_vector.m_x) > M_EPSILON) {
        si = v1.m_x / ray_vector.m_x;
        if (si > (treal)(0.0)) {
            pt = ray_point + ray_vector.scale(si);
            if (pt.m_y > m_min.m_y && pt.m_y < m_max.m_y &&
                pt.m_z > m_min.m_z && pt.m_z < m_max.m_z)
                return true;
        }
        si = v2.m_x / ray_vector.m_x;
        if (si > (treal)(0.0)) {
            pt = ray_point + ray_vector.scale(si);
            if (pt.m_y > m_min.m_y && pt.m_y < m_max.m_y &&
                pt.m_z > m_min.m_z && pt.m_z < m_max.m_z)
                return true;
        }
    }

    // Y-axis normal
    if (fabs(ray_vector.m_y) > M_EPSILON) {
        si = v1.m_y / ray_vector.m_y;
        if (si > (treal)(0.0)) {
            pt = ray_point + ray_vector.scale(si);
            if (pt.m_x > m_min.m_x && pt.m_x < m_max.m_x &&
                pt.m_z > m_min.m_z && pt.m_z < m_max.m_z)
                return true;
        }
        si = v2.m_y / ray_vector.m_y;
        if (si > (treal)(0.0)) {
            pt = ray_point + ray_vector.scale(si);
            if (pt.m_x > m_min.m_x && pt.m_x < m_max.m_x &&
                pt.m_z > m_min.m_z && pt.m_z < m_max.m_z)
                return true;
        }
    }

    // Z-axis normal
    if (fabs(ray_vector.m_z) > M_EPSILON) {
        si = v1.m_z / ray_vector.m_z;
        if (si > (treal)(0.0)) {
            pt = ray_point + ray_vector.scale(si);
            if (pt.m_x > m_min.m_x && pt.m_x < m_max.m_x &&
                pt.m_y > m_min.m_y && pt.m_y < m_max.m_y)
                return true;
        }
        si = v2.m_z / ray_vector.m_z;
        if (si > (treal)(0.0)) {
            pt = ray_point + ray_vector.scale(si);
            if (pt.m_x > m_min.m_x && pt.m_x < m_max.m_x &&
                pt.m_y > m_min.m_y && pt.m_y < m_max.m_y)
                return true;
        }
    }

    return false;
}

bool Geom::BoundingBox::intersects_ray(const Geom::Vector3d& ray_point, const Geom::Vector3d& ray_vector, treal cone_angle) const {
    Geom::Vector3d center((m_min + m_max).scale(0.5));
    Geom::Vector3d dir(center - ray_point);

    treal dg_sq = dir.get_length_squared();
    if (dg_sq < M_EPSILON_SQ)
        return intersects_ray(ray_point, ray_vector);

    Geom::Vector3d n(ray_vector.cross(dir));
    treal ng_sq = n.get_length_squared();
    if (ng_sq < M_EPSILON_SQ)
        return intersects_ray(ray_point, ray_vector);

    treal ng = sqrt(ng_sq);
    treal ng_inv = (treal)(1.0) / ng;
    n.scale_self(ng_inv);

    treal dg = sqrt(dg_sq);
    treal dg_inv = (treal)(1.0) / dg;

    treal cosa = dir.dot(ray_vector) * dg_inv;
    treal ang;
    if (cosa < 0.0)
        ang = M_SPI;
    else if (cosa > 1.0)
        ang = 0.0;
    else
        ang = acos(cosa);

    treal ang2 = Geom::min_treal(ang, cone_angle);

    Geom::Transformation rot(Geom::Transformation::rotate(n, ang2));

    Geom::Vector3d ray_vector2(rot.rotate_vector(ray_vector));

    return intersects_ray(ray_point, ray_vector2);
}

void Geom::BoundingBox::get_corner(unsigned int i, Geom::Vector3d& corner_out) const {
    switch (i) {
        case 0: // [0,0,0]
            corner_out.m_x = m_min.m_x;
            corner_out.m_y = m_min.m_y;
            corner_out.m_z = m_min.m_z;
            break;
        case 1: // [1,0,0]
            corner_out.m_x = m_max.m_x;
            corner_out.m_y = m_min.m_y;
            corner_out.m_z = m_min.m_z;
            break;
        case 2: // [0,1,0]
            corner_out.m_x = m_min.m_x;
            corner_out.m_y = m_max.m_y;
            corner_out.m_z = m_min.m_z;
            break;
        case 3: // [1,1,0]
            corner_out.m_x = m_max.m_x;
            corner_out.m_y = m_max.m_y;
            corner_out.m_z = m_min.m_z;
            break;
        case 4: // [0,0,1]
            corner_out.m_x = m_min.m_x;
            corner_out.m_y = m_min.m_y;
            corner_out.m_z = m_max.m_z;
            break;
        case 5: // [1,0,1]
            corner_out.m_x = m_max.m_x;
            corner_out.m_y = m_min.m_y;
            corner_out.m_z = m_max.m_z;
            break;
        case 6: // [0,1,1]
            corner_out.m_x = m_min.m_x;
            corner_out.m_y = m_max.m_y;
            corner_out.m_z = m_max.m_z;
            break;
        case 7 : // [1,1,1]
            corner_out.m_x = m_max.m_x;
            corner_out.m_y = m_max.m_y;
            corner_out.m_z = m_max.m_z;
            break;
    }
}

void Geom::BoundingBox::clear() {
    m_min.set_all(MAX_VALUE);
    m_max.set_all(MIN_VALUE);
}

void Geom::BoundingBox::pad_out(treal value) {
    m_min.m_x -= value;
    m_min.m_y -= value;
    m_min.m_z -= value;
    m_max.m_x += value;
    m_max.m_y += value;
    m_max.m_z += value;
}
