/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "geom.h"
#include "geom_vector3d.h"
#include "geom_transformation.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

double Geom::inv_sqrt(double x) {
    double xhalf = 0.5 * x;
    long long i = *(long long*)(&x);
    i = 0x5fe6ec85e7de30daLL - (i >> 1);
    x = *(double*)(&i);
    x = x * (1.5 - xhalf * x * x);
    return x;
}

float Geom::inv_sqrt(float x) {
    float xhalf = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
}

int Geom::intersect_segment_plane(const Geom::Vector3d& s1, const Geom::Vector3d& s2, const Geom::Vector3d& pl_point, const Geom::Vector3d& pl_normal, Geom::Vector3d& point_out) {
    Geom::Vector3d u(s2 - s1);
    Geom::Vector3d w(s1 - pl_point);
    treal d = u.dot(pl_normal);
    treal n = -w.dot(pl_normal);
    if (fabs(d) < M_EPSILON) { // segment is parallel to plane
        if (n == 0)
            return 2; // segment lies in plane
        else
            return 0; // no intersection
    }
    else {
        treal si = n / d;
        if (si < (treal)(0.0) || si > (treal)(1.0))
            return 0;
        else {
            point_out = s1 + u.scale(si);
            return 1;
        }
    }
}

int Geom::intersect_ray_plane(const Geom::Vector3d& ray_point, const Geom::Vector3d& ray_vector, const Geom::Vector3d& plane_point, const Geom::Vector3d& plane_normal, Geom::Vector3d& point_out) {
    treal d = ray_vector.dot(plane_normal);
    if (fabs(d) > M_EPSILON) {
        treal n = (plane_point - ray_point).dot(plane_normal);
        treal si = n / d;

        if (si < (treal)(0.0))
            return 0;
        else {
            point_out = ray_point + ray_vector.scale(si);
            return 1;
        }

    }
    else {
        return 0;
    }
}

void Geom::triplet_convert_point_to_uvwh(const Geom::Vector3d& p, const Geom::Vector3d& p0, const Geom::Vector3d& p1, const Geom::Vector3d& p2, Geom::Vector4d& uvwh_out) {
    Geom::Vector3d rp(p - p0);
    Geom::Vector3d e0(p1 - p0);
    Geom::Vector3d e1(p2 - p0);
    treal dot00 = e0.get_length_squared();
    treal dot01 = e0.dot(e1);
    treal dot11 = e1.get_length_squared();
    treal f = dot00 * dot11 - dot01 * dot01;
    treal ti;
    if (fabs(f) > M_EPSILON)
        ti = (treal)(1.0) / f;
    else
        ti = (treal)(0.0);
    Geom::Vector3d n(e0.cross(e1).normalize());
    
    uvwh_out.m_w = rp.dot(n);
    Geom::Vector3d e2(rp - n.scale(uvwh_out.m_w));
    treal dot02 = e0.dot(e2);
    treal dot12 = e1.dot(e2);

    uvwh_out.m_y = (dot02 * dot11 - dot01 * dot12) * ti;
    uvwh_out.m_z = (dot00 * dot12 - dot02 * dot01) * ti;
    uvwh_out.m_x = (treal)(1.0) - uvwh_out.m_y - uvwh_out.m_z;
}

bool Geom::is_point_inside_triplet(const Geom::Vector3d& p, const Geom::Vector3d& p0, const Geom::Vector3d& p1, const Geom::Vector3d& p2) {
    Geom::Vector4d uvwh;
    triplet_convert_point_to_uvwh(p, p0, p1, p2, uvwh);
    if (uvwh.m_x > (treal)(0.0) && uvwh.m_y > (treal)(0.0) && uvwh.m_z > (treal)(0.0))
        return true;
    else
        return false;
}

void Geom::cpa_line_line(const Geom::Vector3d& pA, const Geom::Vector3d& vA, const Geom::Vector3d& pB, const Geom::Vector3d& vB, Geom::Vector3d& p1_out, Geom::Vector3d& p2_out) {
    Geom::Vector3d w(pA - pB);
    treal a = vA.get_length_squared();
    treal b = vA.dot(vB);
    treal c = vB.get_length_squared();
    treal d = vA.dot(w);
    treal e = vB.dot(w);
    treal f = a * c - b * b;
    treal sc, tc;
    if (f < M_EPSILON) {
        sc = 0.0;
        if (b > c)
            tc = d / b;
        else
            tc = e / c;
    }
    else {
        sc = (b * e - c * d) / f;
        tc = (a * e - b * d) / f;
    }

    p1_out = pA + vA.scale(sc);
    p2_out = pB + vB.scale(tc);
}

// cf. Appendix B of [Meyer et al 2002]
// http://www.geometry.caltech.edu/pubs/DMSB_III.pdf
treal Geom::cotan(const Geom::Vector3d& u, const Geom::Vector3d& v) {
    treal d = u.dot(v);

    treal sf = u.get_length_squared() * v.get_length_squared() - d * d;

    if (sf < M_EPSILON_SQ) {
        return (treal)(0.0);
    }
    else {
        return d / sqrt(sf);
    }
}

Geom::Vector3d* Geom::points_on_circle(unsigned int num_segs, treal radius, treal rot_ang) {
    Geom::Vector3d* pts = (Geom::Vector3d*)malloc(sizeof(Geom::Vector3d) * num_segs);

    unsigned int i = 0;
    treal delta = M_SPI2 / static_cast<treal>(num_segs);
    treal ang = rot_ang;
    for (; i < num_segs; ++i) {
        pts[i].m_x = cos(ang) * radius;
        pts[i].m_y = sin(ang) * radius;
        pts[i].m_z = 0.0;
        ang += delta;
    }

    return pts;
}

Geom::Vector3d* Geom::points_on_arc(unsigned int num_segs, treal radius, treal start_ang, treal end_ang) {
    unsigned int num_pts = num_segs + 1;
    Geom::Vector3d* pts = (Geom::Vector3d*)malloc(sizeof(Geom::Vector3d) * num_pts);

    unsigned int i = 0;
    treal delta = (end_ang - start_ang) / static_cast<treal>(num_segs);
    treal ang = start_ang;
    for (; i < num_pts; ++i) {
        pts[i].m_x = cos(ang) * radius;
        pts[i].m_y = sin(ang) * radius;
        pts[i].m_z = 0.0;
        ang += delta;
    }

    return pts;
}
