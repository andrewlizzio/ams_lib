/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef GEOM_H
#define GEOM_H

#include <cmath>

#ifdef _MSC_VER
    #include <intrin.h>
    #include <emmintrin.h>
#else
    #include <pmmintrin.h>
    #include <emmintrin.h>
    #include <mmintrin.h>
#endif

#define M_GEOM_USE_DOUBLE

#ifdef M_GEOM_USE_DOUBLE
    typedef double treal;
#else
    typedef float treal;
#endif


#define M_SPI               (treal)(3.141592653589793)
#define M_SPI2              (treal)(6.283185307179586)
#define M_EPSILON           (treal)(1.0e-6)
#define M_EPSILON_SQ        (treal)(1.0e-12)
#define M_EPSILON2          (treal)(1.0e-3)
#define M_INCH_TO_METER     (treal)(0.0254)
#define M_INCH2_TO_METER2   (treal)(0.0254 * 0.0254)
#define M_INCH3_TO_METER3   (treal)(0.0254 * 0.0254 * 0.0254)
#define M_INCH4_TO_METER4   (treal)(0.0254 * 0.0254 * 0.0254 * 0.0254)
#define M_METER_TO_INCH     (treal)(1.0 / 0.0254)
#define M_METER2_TO_INCH2   (treal)(1.0 / (0.0254 * 0.0254))
#define M_METER3_TO_INCH3   (treal)(1.0 / (0.0254 * 0.0254 * 0.0254))
#define M_DEG_TO_RAD        (treal)(3.141592653589793 / 180.0)
#define M_RAD_TO_DEG        (treal)(180.0 / 3.141592653589793)

namespace Geom {
    // Classes
    class Color;
    class Vector3d;
    class Vector4d;
    class Transformation;
    class Quaternion;
    class BoundingBox;

    template <class T>
    class BoxSpace;

    // Structures
    struct Pair {
        unsigned int m_a;
        unsigned int m_b;

        Pair()
            : m_a(0), m_b(0)
        {}

        Pair(unsigned int a, unsigned int b)
            : m_a(a), m_b(b)
        {}
    };

    // Functions
    template<typename T>
    T clamp(const T& value, const T& min, const T& max) {
        if (value < min)
            return min;
        else if (value > max)
            return max;
        else
            return value;
    }

    template<typename T>
    int sign(T val) {
        return (T(0) < val) - (val < T(0));
    }

    template<typename T>
    int rsign(T val) {
        return (val < T(0)) - (T(0) < val);
    }

    int min_int(int a, int b);
    int max_int(int a, int b);
    int clamp_int(int val, int min_val, int max_val);

    long long min_ll(long long a, long long b);
    long long max_ll(long long a, long long b);
    long long clamp_ll(long long val, long long min_val, long long max_val);

    float min_float(float a, float b);
    float max_float(float a, float b);
    float clamp_float(float val, float min_val, float max_val);

    double min_double(double a, double b);
    double max_double(double a, double b);
    double clamp_double(double val, double min_val, double max_val);

    treal min_treal(treal a, treal b);
    treal max_treal(treal a, treal b);
    treal clamp_treal(treal val, treal min_val, treal max_val);

    void min_float2(float& a, float b);
    void max_float2(float& a, float b);
    void clamp_float2(float& val, float min_val, float max_val);

    void min_double2(double& a, double b);
    void max_double2(double& a, double b);
    void clamp_double2(double& val, double min_val, double max_val);

    void min_treal2(treal& a, treal b);
    void max_treal2(treal& a, treal b);
    void clamp_treal2(treal& val, treal min_val, treal max_val);

    double inv_sqrt(double x);
    float inv_sqrt(float x);

    unsigned int log2uint(unsigned int x);
    unsigned int pow2uint(unsigned int x);

    // Rounds up x to nearest multiple m, such that x' >= m
    size_t round_up(size_t m, size_t x);

    // Returns
    // - 0 if no intersection
    // - 1 if intersection exists
    // - 2 if segment lies in plane
    int intersect_segment_plane(const Geom::Vector3d& s1, const Geom::Vector3d& s2, const Geom::Vector3d& pl_point, const Geom::Vector3d& pl_normal, Geom::Vector3d& point_out);

    // Returns
    // - 0 if no intersection
    // - 1 if intersection exists
    int intersect_ray_plane(const Geom::Vector3d& ray_point, const Geom::Vector3d& ray_vector, const Geom::Vector3d& plane_point, const Geom::Vector3d& plane_normal, Geom::Vector3d& point_out);

    void triplet_convert_point_to_uvwh(const Geom::Vector3d& p, const Geom::Vector3d& p0, const Geom::Vector3d& p1, const Geom::Vector3d& p2, Geom::Vector4d& uvwh_out);
    bool is_point_inside_triplet(const Geom::Vector3d& p, const Geom::Vector3d& p0, const Geom::Vector3d& p1, const Geom::Vector3d& p2);

    // http://geomalgorithms.com/a07-_distance.html
    void cpa_line_line(const Geom::Vector3d& pA, const Geom::Vector3d& vA, const Geom::Vector3d& pB, const Geom::Vector3d& vB, Geom::Vector3d& p1_out, Geom::Vector3d& p2_out);

    treal cotan(const Geom::Vector3d& u, const Geom::Vector3d& v);

    // Returns num_segs points
    Geom::Vector3d* points_on_circle(unsigned int num_segs, treal radius, treal rot_ang);

    // Returns num_segs + 1 points
    Geom::Vector3d* points_on_arc(unsigned int num_segs, treal radius, treal start_ang, treal end_ang);
};


// Define inline functions

inline int Geom::min_int(int a, int b) {
    return b ^ ((a ^ b) & -(a < b));
}

inline int Geom::max_int(int a, int b) {
    return a ^ ((a ^ b) & -(a < b));
}

inline int Geom::clamp_int(int val, int min_val, int max_val) {
    return min_int(max_int(val, min_val), max_val);
}

inline long long Geom::min_ll(long long a, long long b) {
    return b ^ ((a ^ b) & -(a < b));
}

inline long long Geom::max_ll(long long a, long long b) {
    return a ^ ((a ^ b) & -(a < b));
}

inline long long Geom::clamp_ll(long long val, long long min_val, long long max_val) {
    return min_ll(max_ll(val, min_val), max_val);
}

inline float Geom::min_float(float a, float b) {
    _mm_store_ss(&a, _mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
    return a;
}

inline float Geom::max_float(float a, float b) {
    _mm_store_ss(&a, _mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
    return a;
}

inline float Geom::clamp_float(float val, float min_val, float max_val) {
    _mm_store_ss(&val, _mm_min_ss(_mm_max_ss(_mm_set_ss(val), _mm_set_ss(min_val)), _mm_set_ss(max_val)));
    return val;
}

inline double Geom::min_double(double a, double b) {
    _mm_store_sd(&a, _mm_min_sd(_mm_set_sd(a), _mm_set_sd(b)));
    return a;
}

inline double Geom::max_double(double a, double b) {
    _mm_store_sd(&a, _mm_max_sd(_mm_set_sd(a), _mm_set_sd(b)));
    return a;
}

inline double Geom::clamp_double(double val, double min_val, double max_val) {
    _mm_store_sd(&val, _mm_min_sd(_mm_max_sd(_mm_set_sd(val), _mm_set_sd(min_val)), _mm_set_sd(max_val)));
    return val;
}

inline treal Geom::min_treal(treal a, treal b) {
#ifdef M_GEOM_USE_DOUBLE
    _mm_store_sd(&a, _mm_min_sd(_mm_set_sd(a), _mm_set_sd(b)));
#else
    _mm_store_ss(&a, _mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
#endif
    return a;
}

inline treal Geom::max_treal(treal a, treal b) {
#ifdef M_GEOM_USE_DOUBLE
    _mm_store_sd(&a, _mm_max_sd(_mm_set_sd(a), _mm_set_sd(b)));
#else
    _mm_store_ss(&a, _mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
#endif
    return a;
}

inline treal Geom::clamp_treal(treal val, treal min_val, treal max_val) {
#ifdef M_GEOM_USE_DOUBLE
    _mm_store_sd(&val, _mm_min_sd(_mm_max_sd(_mm_set_sd(val), _mm_set_sd(min_val)), _mm_set_sd(max_val)));
#else
    _mm_store_ss(&val, _mm_min_ss(_mm_max_ss(_mm_set_ss(val), _mm_set_ss(min_val)), _mm_set_ss(max_val)));
#endif
    return val;
}

inline void Geom::min_float2(float& a, float b) {
    _mm_store_ss(&a, _mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
}

inline void Geom::max_float2(float& a, float b) {
    _mm_store_ss(&a, _mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
}

inline void Geom::clamp_float2(float& val, float min_val, float max_val) {
    _mm_store_ss(&val, _mm_min_ss(_mm_max_ss(_mm_set_ss(val), _mm_set_ss(min_val)), _mm_set_ss(max_val)));
}

inline void Geom::min_double2(double& a, double b) {
    _mm_store_sd(&a, _mm_min_sd(_mm_set_sd(a), _mm_set_sd(b)));
}

inline void Geom::max_double2(double& a, double b) {
    _mm_store_sd(&a, _mm_max_sd(_mm_set_sd(a), _mm_set_sd(b)));
}

inline void Geom::clamp_double2(double& val, double min_val, double max_val) {
    _mm_store_sd(&val, _mm_min_sd(_mm_max_sd(_mm_set_sd(val), _mm_set_sd(min_val)), _mm_set_sd(max_val)));
}

inline void Geom::min_treal2(treal& a, treal b) {
#ifdef M_GEOM_USE_DOUBLE
    _mm_store_sd(&a, _mm_min_sd(_mm_set_sd(a), _mm_set_sd(b)));
#else
    _mm_store_ss(&a, _mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
#endif
}

inline void Geom::max_treal2(treal& a, treal b) {
#ifdef M_GEOM_USE_DOUBLE
    _mm_store_sd(&a, _mm_max_sd(_mm_set_sd(a), _mm_set_sd(b)));
#else
    _mm_store_ss(&a, _mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
#endif
}

inline void Geom::clamp_treal2(treal& val, treal min_val, treal max_val) {
#ifdef M_GEOM_USE_DOUBLE
    _mm_store_sd(&val, _mm_min_sd(_mm_max_sd(_mm_set_sd(val), _mm_set_sd(min_val)), _mm_set_sd(max_val)));
#else
    _mm_store_ss(&val, _mm_min_ss(_mm_max_ss(_mm_set_ss(val), _mm_set_ss(min_val)), _mm_set_ss(max_val)));
#endif
}

inline unsigned int Geom::log2uint(unsigned int x) {
    unsigned int v = 0;
    while (x >>= 1) ++v;
    return v;
}

inline unsigned int Geom::pow2uint(unsigned int x) {
    return (unsigned int)(1) << x;
}

inline size_t Geom::round_up(size_t m, size_t x) {
    if (x < m)
        return m;
    else {
        size_t r = x % m;
        if (r != 0)
            return x + m - r;
        else
            return x;
    }
}

#endif /* GEOM_H */
