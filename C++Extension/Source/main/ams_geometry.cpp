/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_geometry.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Geometry::rbf_calc_cubic_bezier_point(VALUE self, VALUE v_t, VALUE v_p0, VALUE v_p1, VALUE v_p2, VALUE v_p3) {
    double t = RU::value_to_double(v_t);
    Geom::Vector3d p0, p1, p2, p3;
    RU::value_to_vector(v_p0, p0);
    RU::value_to_vector(v_p1, p1);
    RU::value_to_vector(v_p2, p2);
    RU::value_to_vector(v_p3, p3);
    double u = 1.0 - t;
    double uu = u * u;
    double tt = t * t;
    double m0 = uu * u;
    double m1 = 3.0 * t * uu;
    double m2 = 3.0 * tt * u;
    double m3 = tt * t;
    Geom::Vector3d point(
        m0 * p0.m_x + m1 * p1.m_x + m2 * p2.m_x + m3 * p3.m_x,
        m0 * p0.m_y + m1 * p1.m_y + m2 * p2.m_y + m3 * p3.m_y,
        m0 * p0.m_z + m1 * p1.m_z + m2 * p2.m_z + m3 * p3.m_z);
    return RU::point_to_value(point);
}

VALUE AMS::Geometry::rbf_calc_cubic_bezier_slope(VALUE self, VALUE v_t, VALUE v_p0, VALUE v_p1, VALUE v_p2, VALUE v_p3) {
    double t = RU::value_to_double(v_t);
    Geom::Vector3d p0, p1, p2, p3;
    RU::value_to_vector(v_p0, p0);
    RU::value_to_vector(v_p1, p1);
    RU::value_to_vector(v_p2, p2);
    RU::value_to_vector(v_p3, p3);
    double tt = t * t;
    double m0 = -3.0 * tt + 6.0 * t - 3.0;
    double m1 = 9.0 * tt - 12.0 * t + 3.0;
    double m2 = -9.0 * tt + 6.0 * t;
    double m3 = 3.0 * tt;
    Geom::Vector3d point(
        m0 * p0.m_x + m1 * p1.m_x + m2 * p2.m_x + m3 * p3.m_x,
        m0 * p0.m_y + m1 * p1.m_y + m2 * p2.m_y + m3 * p3.m_y,
        m0 * p0.m_z + m1 * p1.m_z + m2 * p2.m_z + m3 * p3.m_z);
    return RU::vector_to_value(point);
}

VALUE AMS::Geometry::rbf_scale_point(VALUE self, VALUE v_point, VALUE v_scale) {
    Geom::Vector3d point;
    RU::value_to_vector(v_point, point);
    double scale = RU::value_to_double(v_scale);
    return RU::point_to_value2(point, scale);
}

VALUE AMS::Geometry::rbf_scale_vector(VALUE self, VALUE v_vector, VALUE v_scale) {
    Geom::Vector3d vector;
    RU::value_to_vector(v_vector, vector);
    double scale = RU::value_to_double(v_scale);
    return RU::vector_to_value2(vector, scale);
}

VALUE AMS::Geometry::rbf_scale_matrix(VALUE self, VALUE v_matrix, VALUE v_scale) {
    Geom::Transformation tra;
    Geom::Vector3d scale;
    RU::value_to_transformation(v_matrix, tra);
    RU::value_to_vector(v_scale, scale);
    tra.scale_axes_self(scale);
    return RU::transformation_to_value(tra);
}

VALUE AMS::Geometry::rbf_product_vectors(VALUE self, VALUE v_vector, VALUE v_other_vector) {
    Geom::Vector3d vector, other_vector;
    RU::value_to_vector(v_vector, vector);
    RU::value_to_vector(v_other_vector, other_vector);
    return RU::vector_to_value(vector.product(other_vector));
}

VALUE AMS::Geometry::rbf_rotate_vector(VALUE self, VALUE v_vector, VALUE v_normal, VALUE v_angle) {
    Geom::Vector3d vector, normal;
    RU::value_to_vector(v_vector, vector);
    RU::value_to_vector(v_normal, normal);
    double angle = RU::value_to_double(v_angle);
    return RU::vector_to_value(vector.rotate(normal, angle));
}

VALUE AMS::Geometry::rbf_angle_between_vectors(int argc, VALUE* argv, VALUE self) {
    if (argc == 3) {
        Geom::Vector3d vector1, vector2, normal;
        RU::value_to_vector(argv[0], vector1);
        RU::value_to_vector(argv[1], vector2);
        RU::value_to_vector(argv[2], normal);
        return RU::to_value(vector1.angle_between(vector2, normal));
    }
    else if (argc == 2) {
        Geom::Vector3d vector1, vector2;
        RU::value_to_vector(argv[0], vector1);
        RU::value_to_vector(argv[1], vector2);
        return RU::to_value(vector1.angle_between(vector2));
    }
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");
}

VALUE AMS::Geometry::rbf_get_unique_points(VALUE self, VALUE v_points) {
    unsigned int i, size;
    if (TYPE(v_points) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array!");
    size = (unsigned int)RARRAY_LEN(v_points);
    // A fast way to get unique points is by creating a PolygonMesh and adding points to it
    VALUE v_mesh = rb_class_new_instance(0, nullptr, RU::SU_POLYGON_MESH);
    for (i = 0; i < size; ++i)
        rb_funcall(v_mesh, RU::INTERN_ADD_POINT, 1, rb_ary_entry(v_points, i));
    return rb_funcall(v_mesh, RU::INTERN_POINTS, 0);
}

VALUE AMS::Geometry::rbf_points_collinear(VALUE self, VALUE v_points) {
    // Declare variables
    unsigned int i, size;
    VALUE v_mesh, v_pt1, v_pt2, v_pt3;
    Geom::Vector3d pt1, pt2, pt3, v1, v1r, v2;
    // Validate
    if (TYPE(v_points) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array!");
    size = (unsigned int)RARRAY_LEN(v_points);
    // Add points to a PolygonMesh for obtaining unique points
    v_mesh = rb_class_new_instance(0, nullptr, RU::SU_POLYGON_MESH);
    for (i = 0; i < size; ++i)
        rb_funcall(v_mesh, RU::INTERN_ADD_POINT, 1, rb_ary_entry(v_points, i));
    size = RU::value_to_uint(rb_funcall(v_mesh, RU::INTERN_COUNT_POINTS, 0));
    // Any two or less points are collinear
    if (size < 3) return Qtrue;
    // Create a vector from the first two points
    v_pt1 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(1));
    v_pt2 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(2));
    RU::value_to_vector(v_pt1, pt1);
    RU::value_to_vector(v_pt2, pt2);
    v1 = pt2 - pt1;
    v1.normalize_self();
    v1r = v1.reverse();
    // Find a third point that is not collinear
    for (i = 3; i <= size; ++i) {
        v_pt3 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(i));
        RU::value_to_vector(v_pt3, pt3);
        v2 = pt3 - pt1;
        v2.normalize_self();
        // Return false if other vector is not parallel to the first vector
        if (v2 != v1 && v2 != v1r)
            return Qfalse;
    }
    // Otherwise, return true
    return Qtrue;
}

VALUE AMS::Geometry::rbf_get_noncollinear_points(VALUE self, VALUE v_points) {
    // Declare variables
    unsigned int i, size;
    VALUE v_mesh, v_pt1, v_pt2, v_pt3;
    Geom::Vector3d pt1, pt2, pt3, v1, v1r, v2;
    // Validate
    if (TYPE(v_points) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array!");
    size = (unsigned int)RARRAY_LEN(v_points);
    // Add points to a PolygonMesh for obtaining unique points
    v_mesh = rb_class_new_instance(0, nullptr, RU::SU_POLYGON_MESH);
    for (i = 0; i < size; ++i)
        rb_funcall(v_mesh, RU::INTERN_ADD_POINT, 1, rb_ary_entry(v_points, i));
    size = RU::value_to_uint(rb_funcall(v_mesh, RU::INTERN_COUNT_POINTS, 0));
    // Any two or less points are collinear
    if (size < 3) return Qnil;
    // Create a vector from the first two points
    v_pt1 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(1));
    v_pt2 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(2));
    RU::value_to_vector(v_pt1, pt1);
    RU::value_to_vector(v_pt2, pt2);
    v1 = pt2 - pt1;
    v1.normalize_self();
    v1r = v1.reverse();
    // Find a third point that is not collinear
    for (i = 3; i <= size; ++i) {
        v_pt3 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(i));
        RU::value_to_vector(v_pt3, pt3);
        v2 = pt3 - pt1;
        v2.normalize_self();
        // Return all three points if other vector is not parallel to the first vector
        if (v2 != v1 && v2 != v1r)
            return rb_ary_new3(3, v_pt1, v_pt2, v_pt3);
    }
    // Otherwise, return nil
    return Qnil;
}

VALUE AMS::Geometry::rbf_get_plane_normal(VALUE self, VALUE v_plane) {
    // Declare variables
    Geom::Vector3d pt1, pt2, pt3, normal;
    // Validate
    if (TYPE(v_plane) != T_ARRAY || RARRAY_LEN(v_plane) < 3)
        rb_raise(rb_eTypeError, "Expected an array of at least three points!");
    // Obtain three points
    RU::value_to_vector(rb_ary_entry(v_plane, 0), pt1);
    RU::value_to_vector(rb_ary_entry(v_plane, 1), pt2);
    RU::value_to_vector(rb_ary_entry(v_plane, 2), pt3);
    // Compute normal
    normal = (pt2 - pt1).cross(pt3 - pt1);
    normal.normalize_self();
    // Return
    return RU::vector_to_value(normal);
}

VALUE AMS::Geometry::rbf_points_coplanar(VALUE self, VALUE v_points) {
    // Declare variables
    unsigned int i, size;
    VALUE v_mesh, v_pt1, v_pt2, v_pt3, v_pt4;
    Geom::Vector3d pt1, pt2, pt3, pt4, v1, v1r, v2, normal;
    // Validate
    if (TYPE(v_points) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array!");
    size = (unsigned int)RARRAY_LEN(v_points);
    // Add points to a PolygonMesh for obtaining unique points
    v_mesh = rb_class_new_instance(0, nullptr, RU::SU_POLYGON_MESH);
    for (i = 0; i < size; ++i)
        rb_funcall(v_mesh, RU::INTERN_ADD_POINT, 1, rb_ary_entry(v_points, i));
    size = RU::value_to_uint(rb_funcall(v_mesh, RU::INTERN_COUNT_POINTS, 0));
    // Any three or less points are coplanar
    if (size < 4) return Qtrue;
    // Step 1: Obtain three noncollinear points
    v_pt1 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(1));
    v_pt2 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(2));
    // Create a vector from the first two points
    RU::value_to_vector(v_pt1, pt1);
    RU::value_to_vector(v_pt2, pt2);
    v1 = pt2 - pt1;
    v1.normalize_self();
    v1r = v1.reverse();
    // Find a third point that is not collinear
    for (i = 3; i <= size; ++i) {
        v_pt3 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(i));
        RU::value_to_vector(v_pt3, pt3);
        v2 = pt3 - pt1;
        v2.normalize_self();
        // Break out if other vector is not parallel to the first vector
        if (v2 != v1 && v2 != v1r)
            break;
    }
    // All collinear point are also coplanar
    if (i - 1 == size)
        return Qtrue;
    // Step 2: Compute triplet normal
    normal = v1.cross(v2);
    normal.normalize_self();
    // Step 3: Check if all other points lie on the same plane
    for (; i <= size; ++i) {
        v_pt4 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(i));
        RU::value_to_vector(v_pt4, pt4);
        // Return false if point is a certain threshold above the plane
        if (fabs((pt4 - pt1).dot(normal)) > M_EPSILON)
            return Qfalse;
    }
    // Otherwise, return true
    return Qtrue;
}

VALUE AMS::Geometry::rbf_sort_polygon_points(VALUE self, VALUE v_points) {
    // Declare variables
    unsigned int i, size;
    VALUE v_mesh, v_pt1, v_pt2, v_pt3;
    Geom::Vector3d pt1, pt2, pt3, v1, v1r, v2, normal, xaxis, yaxis;
    Geom::Vector3d center(0.0);
    Geom::Vector3d* points;
    double scale, x, y, theta;
    std::map<double, unsigned int> sorted_points;
    VALUE v_sorted_points;
    // Validate
    if (TYPE(v_points) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array!");
    size = (unsigned int)RARRAY_LEN(v_points);
    // Add points to a PolygonMesh for obtaining unique points
    v_mesh = rb_class_new_instance(0, nullptr, RU::SU_POLYGON_MESH);
    for (i = 0; i < size; ++i)
        rb_funcall(v_mesh, RU::INTERN_ADD_POINT, 1, rb_ary_entry(v_points, i));
    size = RU::value_to_uint(rb_funcall(v_mesh, RU::INTERN_COUNT_POINTS, 0));
    // Any three or less points are considered as sorted
    if (size < 4)
        return rb_funcall(v_mesh, RU::INTERN_POINTS, 0);
    // Step 1: Obtain three noncollinear points
    v_pt1 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(1));
    v_pt2 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(2));
    // Create a vector from the first two points
    RU::value_to_vector(v_pt1, pt1);
    RU::value_to_vector(v_pt2, pt2);
    v1 = pt2 - pt1;
    v1.normalize_self();
    v1r = v1.reverse();
    // Find a third point that is not collinear
    for (i = 3; i <= size; ++i) {
        v_pt3 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(i));
        RU::value_to_vector(v_pt3, pt3);
        v2 = pt3 - pt1;
        v2.normalize_self();
        // Break out if other vector is not parallel to the first vector
        if (v2 != v1 && v2 != v1r)
            break;
    }
    // If all points are collinear, return all unique points
    if (i - 1 == size)
        return rb_funcall(v_mesh, RU::INTERN_POINTS, 0);
    // Step 2: Compute triplet normal
    normal = v1.cross(v2);
    normal.normalize_self();
    // Step 3: Convert all points to C++ and compute center
    points = new Geom::Vector3d[size];
    scale = 1.0 / static_cast<double>(size);
    for (i = 0; i < size; ++i) {
        v_pt3 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(i + 1));
        RU::value_to_vector(v_pt3, points[i]);
        center += points[i].scale(scale);
    }
    // Step 4: Compute xaxis and yaxis from normal
    if (fabs(normal.m_z) < (treal)(0.9999995))
        xaxis = Geom::Vector3d::Z_AXIS.cross(normal);
    else
        xaxis = Geom::Vector3d::Y_AXIS.cross(normal);
    xaxis.normalize_self();
    yaxis = normal.cross(xaxis);
    yaxis.normalize_self();
    // Step 5: Sort all points by angle, relative to the computed orientation
    for (i = 0; i < size; ++i) {
        v1 = points[i] - center;
        x = v1.dot(xaxis);
        y = v1.dot(yaxis);
        scale = x * x + y * y;
        if (scale > M_EPSILON_SQ) {
            theta = acos(Geom::clamp_double(x / sqrt(scale), -1.0, 1.0));
            if (y < 0.0)
                theta = -theta;
            sorted_points[theta] = i;
        }
    }
    // Discard of points
    delete[] points;
    // Step 6: Return sorted points
    v_sorted_points = rb_ary_new2(static_cast<unsigned int>(sorted_points.size()));
    i = 0;
    for (std::map<double, unsigned int>::iterator it = sorted_points.begin(); it != sorted_points.end(); ++it) {
        v_pt3 = rb_funcall(v_mesh, RU::INTERN_POINT_AT, 1, INT2FIX(it->second + 1));
        rb_ary_store(v_sorted_points, i, v_pt3);
        ++i;
    }
    return v_sorted_points;
}

VALUE AMS::Geometry::rbf_calc_edge_centre(VALUE self, VALUE v_edge) {
    return rb_funcall(rb_funcall(v_edge, RU::INTERN_BOUNDS, 0), RU::INTERN_CENTER, 0);
}

VALUE AMS::Geometry::rbf_calc_face_centre(VALUE self, VALUE v_face) {
    // Declare all variables
    unsigned int i, num_triplets;
    VALUE v_mesh, v_triplet;
    double total_darea = 0.0;
    double darea;
    Geom::Vector3d pt1, pt2, pt3, e0, e1;
    Geom::Vector3d scaled_centre(0.0);
    // To calculate face centre of mass, we obtain its triangular mesh and
    // average area-magnified triplet centroids.
    v_mesh = rb_funcall(v_face, RU::INTERN_MESH, 0);
    num_triplets = RU::value_to_uint(rb_funcall(v_mesh, RU::INTERN_COUNT_POLYGONS, 0));
    for (i = 0; i < num_triplets; ++i) {
        v_triplet = rb_funcall(v_mesh, RU::INTERN_POLYGON_POINTS_AT, 1, RU::to_value(i + 1));
        // Obtain triplet points
        RU::value_to_vector(rb_ary_entry(v_triplet, 0), pt1);
        RU::value_to_vector(rb_ary_entry(v_triplet, 1), pt2);
        RU::value_to_vector(rb_ary_entry(v_triplet, 2), pt3);
        // Computer area
        e0 = pt3 - pt1;
        e1 = pt2 - pt1;
        darea = e0.cross(e1).get_length();
        total_darea += darea;
        // Scale centroid by area and add it to centre array
        scaled_centre += (pt1 + pt2 + pt3).scale(darea);
    }
    // Compute centre
    if (total_darea > M_EPSILON)
        scaled_centre.scale_self((1.0 / 3.0) / total_darea);
    // Return
    return RU::point_to_value(scaled_centre);
}

VALUE AMS::Geometry::rbf_is_point_on_edge(VALUE self, VALUE v_point, VALUE v_edge) {
    // Declare variables
    VALUE v_v1, v_v2, v_pt1, v_pt2;
    Geom::Vector3d pt1, pt2, pt3, v1, v2, v3;
    double mag, mag_sq, mag2;
    // Obtain edge start and end positions
    v_v1 = rb_funcall(v_edge, RU::INTERN_START, 0);
    v_v2 = rb_funcall(v_edge, RU::INTERN_END, 0);
    v_pt1 = rb_funcall(v_v1, RU::INTERN_POSITION, 0);
    v_pt2 = rb_funcall(v_v2, RU::INTERN_POSITION, 0);
    RU::value_to_vector(v_pt1, pt1);
    RU::value_to_vector(v_pt2, pt2);
    RU::value_to_vector(v_point, pt3);
    // Check if pt3 is on segment
    v1 = pt2 - pt1;
    mag_sq = v1.get_length_squared();
    if (mag_sq > M_EPSILON_SQ) {
        mag = sqrt(mag_sq);
        v1.scale_self(1.0 / mag_sq);
        v2 = pt3 - pt1;
        mag2 = v2.dot(v1);
        if (mag2 > -M_EPSILON && mag2 < mag + M_EPSILON) {
            v3 = pt1 + v1.scale(mag2);
            if ((v2 - v3).get_length_squared() < M_EPSILON_SQ)
                return Qtrue;
            else
                return Qfalse;
        }
        else
            return Qfalse;
    }
    else
        return (pt3 == pt1) ? Qtrue : Qfalse;
}

VALUE AMS::Geometry::rbf_is_point_on_face(VALUE self, VALUE v_point, VALUE v_face) {
    VALUE v_result = rb_funcall(v_face, RU::INTERN_CLASSIFY_POINT, 1, v_point);
    return (v_result == RU::SU_FACE_POINT_INSIDE ||
        v_result == RU::SU_FACE_POINT_ON_VERTEX ||
        v_result == RU::SU_FACE_POINT_ON_EDGE ||
        v_result == RU::SU_FACE_POINT_ON_FACE) ? Qtrue : Qfalse;
}

VALUE AMS::Geometry::rbf_is_point_on_triangle(VALUE self, VALUE v_point, VALUE v_pt1, VALUE v_pt2, VALUE v_pt3) {
    Geom::Vector3d point, pt1, pt2, pt3;
    RU::value_to_vector(v_point, point);
    RU::value_to_vector(v_pt1, pt1);
    RU::value_to_vector(v_pt2, pt2);
    RU::value_to_vector(v_pt3, pt3);
    // Use Barycentric Technique: http://blackpawn.com/texts/pointinpoly/
    // Compute vectors
    Geom::Vector3d v0(pt3 - pt1);
    Geom::Vector3d v1(pt2 - pt1);
    Geom::Vector3d v2(point - pt1);
    // Compute dot products
    double dot00 = v0.dot(v0);
    double dot01 = v0.dot(v1);
    double dot02 = v0.dot(v2);
    double dot11 = v1.dot(v1);
    double dot12 = v1.dot(v2);
    // Compute barycentric coordinates
    double inv_denom = 1.0 / (dot00 * dot11 - dot01 * dot01);
    double u = (dot11 * dot02 - dot01 * dot12) * inv_denom;
    double v = (dot00 * dot12 - dot01 * dot02) * inv_denom;
    double w = 1.0 - u - v;
    // Check if point is in triangle
    return (u > 0.0 && v > 0.0 && w > 0.0) ? Qtrue : Qfalse;
}

VALUE AMS::Geometry::rbf_intersect_ray_triangle(VALUE self, VALUE v_origin, VALUE v_dir, VALUE v_pt1, VALUE v_pt2, VALUE v_pt3) {
    Geom::Vector3d origin, dir, pt1, pt2, pt3;
    RU::value_to_vector(v_origin, origin);
    RU::value_to_vector(v_dir, dir);
    RU::value_to_vector(v_pt1, pt1);
    RU::value_to_vector(v_pt2, pt2);
    RU::value_to_vector(v_pt3, pt3);
    // Source: http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    //~ dir.normalize_self();
    // Find vectors for two edges sharing pt0.
    Geom::Vector3d edge1(pt2 - pt1);
    Geom::Vector3d edge2(pt3 - pt1);
    // Begin within calculating the determinant
    Geom::Vector3d pvec(dir.cross(edge2));
    // If the determinant is near zero, then ray lies in plane of the triangle.
    double det = edge1.dot(pvec);
    // Determine if ray intersects a two sided triangle.
    if (fabs(det) < M_EPSILON) return Qnil;
    double inv_det = 1.0 / det;
    // Calculate distance from pt1 to ray origin
    Geom::Vector3d tvec(origin - pt1);
    // Calculate the U parameter and test bounds
    double u = tvec.dot(pvec) * inv_det;
    if (u < 0.0 || u > 1.0) return Qnil;
    // Prepare to test the V parameter
    Geom::Vector3d qvec(tvec.cross(edge1));
    // Calculate the V parameter and test bounds
    double v = dir.dot(qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) return Qnil;
    // Calculate the T parameter
    double t = edge2.dot(qvec) * inv_det;
    // If t is greater than zero, then ray intersects the triangle.
    if (t <= 0.0) return Qnil;
    dir.set_length(t);
    return RU::point_to_value(origin + dir);
}

VALUE AMS::Geometry::rbf_get_matrix_scale(VALUE self, VALUE v_matrix) {
    Geom::Transformation tra;
    RU::value_to_transformation(v_matrix, tra);
    return RU::vector_to_value(tra.get_scale());
}

VALUE AMS::Geometry::rbf_set_matrix_scale(VALUE self, VALUE v_matrix, VALUE v_scale) {
    Geom::Transformation tra;
    Geom::Vector3d scale;
    RU::value_to_transformation(v_matrix, tra);
    RU::value_to_vector(v_scale, scale);
    tra.set_scale(scale);
    return RU::transformation_to_value(tra);
}

VALUE AMS::Geometry::rbf_extract_matrix_scale(VALUE self, VALUE v_matrix) {
    Geom::Transformation tra;
    RU::value_to_transformation(v_matrix, tra);
    tra.normalize_self();
    return RU::transformation_to_value(tra);
}

VALUE AMS::Geometry::rbf_extract_matrix_w_factor(VALUE self, VALUE v_matrix) {
    Geom::Transformation tra;
    RU::value_to_transformation(v_matrix, tra);
    tra.extract_w_factor();
    return RU::transformation_to_value(tra);
}

VALUE AMS::Geometry::rbf_is_matrix_flipped(VALUE self, VALUE v_matrix) {
    Geom::Transformation tra;
    RU::value_to_transformation(v_matrix, tra);
    return RU::to_value(tra.is_flipped());
}

VALUE AMS::Geometry::rbf_is_matrix_uniform(VALUE self, VALUE v_matrix) {
    Geom::Transformation tra;
    RU::value_to_transformation(v_matrix, tra);
    return RU::to_value(tra.is_uniform());
}

VALUE AMS::Geometry::rbf_rotate_matrix_xaxis_to_dir(VALUE self, VALUE v_matrix, VALUE v_dir) {
    Geom::Transformation tra;
    Geom::Vector3d dir;
    RU::value_to_transformation(v_matrix, tra);
    RU::value_to_vector(v_dir, dir);
    return RU::transformation_to_value(tra.rotate_xaxis_to(dir));
}

VALUE AMS::Geometry::rbf_rotate_matrix_yaxis_to_dir(VALUE self, VALUE v_matrix, VALUE v_dir) {
    Geom::Transformation tra;
    Geom::Vector3d dir;
    RU::value_to_transformation(v_matrix, tra);
    RU::value_to_vector(v_dir, dir);
    return RU::transformation_to_value(tra.rotate_yaxis_to(dir));
}

VALUE AMS::Geometry::rbf_rotate_matrix_zaxis_to_dir(VALUE self, VALUE v_matrix, VALUE v_dir) {
    Geom::Transformation tra;
    Geom::Vector3d dir;
    RU::value_to_transformation(v_matrix, tra);
    RU::value_to_vector(v_dir, dir);
    return RU::transformation_to_value(tra.rotate_zaxis_to(dir));
}

VALUE AMS::Geometry::rbf_transition_camera(VALUE self, VALUE v_camera1, VALUE v_camera2, VALUE v_ratio) {
    double ratio = Geom::clamp_double(RU::value_to_double(v_ratio), 0.0, 1.0);
    Geom::Vector3d c1_xaxis, c2_xaxis, c1_direction, c2_direction, c1_up, c2_up, c1_eye, c2_eye;
    RU::value_to_vector(rb_funcall(v_camera1, RU::INTERN_XAXIS, 0), c1_xaxis);
    RU::value_to_vector(rb_funcall(v_camera2, RU::INTERN_XAXIS, 0), c2_xaxis);
    RU::value_to_vector(rb_funcall(v_camera1, RU::INTERN_DIRECTION, 0), c1_direction);
    RU::value_to_vector(rb_funcall(v_camera2, RU::INTERN_DIRECTION, 0), c2_direction);
    RU::value_to_vector(rb_funcall(v_camera1, RU::INTERN_UP, 0), c1_up);
    RU::value_to_vector(rb_funcall(v_camera2, RU::INTERN_UP, 0), c2_up);
    RU::value_to_vector(rb_funcall(v_camera1, RU::INTERN_EYE, 0), c1_eye);
    RU::value_to_vector(rb_funcall(v_camera2, RU::INTERN_EYE, 0), c2_eye);
    Geom::Transformation t1(c1_xaxis, c1_direction, c1_up, c1_eye);
    Geom::Transformation t2(c2_xaxis, c2_direction, c2_up, c2_eye);
    Geom::Transformation t3(t1.uniform_normal_transition_to(t2, ratio));
    VALUE argv[3];
    argv[0] = RU::point_to_value(t3.m_origin); // eye
    argv[1] = RU::point_to_value(t3.m_origin + t3.m_yaxis); // target
    argv[2] = RU::vector_to_value(t3.m_zaxis); // up
    VALUE v_camera3 = rb_class_new_instance(3, argv, RU::SU_CAMERA);
    double c1_aspect_ratio = RU::value_to_double(rb_funcall(v_camera1, RU::INTERN_ASPECT_RATIO, 0));
    double c2_aspect_ratio = RU::value_to_double(rb_funcall(v_camera2, RU::INTERN_ASPECT_RATIO, 0));
    double c3_aspect_ratio = c1_aspect_ratio + (c2_aspect_ratio - c1_aspect_ratio) * ratio;
    rb_funcall(v_camera3, RU::INTERN_SASPECT_RATIO, 1, RU::to_value(c3_aspect_ratio));
    bool c1_perspective = RU::value_to_bool(rb_funcall(v_camera1, RU::INTERN_TPERSPECTIVE, 0));
    bool c2_perspective = RU::value_to_bool(rb_funcall(v_camera2, RU::INTERN_TPERSPECTIVE, 0));
    if (c1_perspective != c2_perspective) return v_camera3;
    rb_funcall(v_camera3, RU::INTERN_SPERSPECTIVE, 1, RU::to_value(c1_perspective));
    if (c1_perspective) {
        double c1_focal_length = RU::value_to_double(rb_funcall(v_camera1, RU::INTERN_FOCAL_LENGTH, 0));
        double c2_focal_length = RU::value_to_double(rb_funcall(v_camera2, RU::INTERN_FOCAL_LENGTH, 0));
        double c3_focal_length = c1_focal_length + (c2_focal_length - c1_focal_length) * ratio;
        rb_funcall(v_camera3, RU::INTERN_SFOCAL_LENGTH, 1, RU::to_value(c3_focal_length));

        double c1_fov = RU::value_to_double(rb_funcall(v_camera1, RU::INTERN_FOV, 0));
        double c2_fov = RU::value_to_double(rb_funcall(v_camera2, RU::INTERN_FOV, 0));
        double c3_fov = c1_fov + (c2_fov - c1_fov) * ratio;
        rb_funcall(v_camera3, RU::INTERN_SFOV, 1, RU::to_value(c3_fov));

        double c1_image_width = RU::value_to_double(rb_funcall(v_camera1, RU::INTERN_IMAGE_WIDTH, 0));
        double c2_image_width = RU::value_to_double(rb_funcall(v_camera2, RU::INTERN_IMAGE_WIDTH, 0));
        double c3_image_width = c1_image_width + (c2_image_width - c1_image_width) * ratio;
        rb_funcall(v_camera3, RU::INTERN_SIMAGE_WIDTH, 1, RU::to_value(c3_image_width));
    }
    else {
        double c1_height = RU::value_to_double(rb_funcall(v_camera1, RU::INTERN_HEIGHT, 0));
        double c2_height = RU::value_to_double(rb_funcall(v_camera2, RU::INTERN_HEIGHT, 0));
        double c3_height = c1_height + (c2_height - c1_height) * ratio;
        rb_funcall(v_camera3, RU::INTERN_SHEIGHT, 1, RU::to_value(c3_height));
    }
    return v_camera3;
}

VALUE AMS::Geometry::rbf_transition_color(VALUE self, VALUE v_color1, VALUE v_color2, VALUE v_ratio) {
    Geom::Color color1, color2;
    RU::value_to_color(v_color1, color1);
    RU::value_to_color(v_color2, color2);
    double ratio = Geom::clamp_double(RU::value_to_double(v_ratio), 0.0, 1.0);
    return RU::color_to_value(color1.transition_to(color2, ratio));
}

VALUE AMS::Geometry::rbf_transition_point(VALUE self, VALUE v_point1, VALUE v_point2, VALUE v_ratio) {
    Geom::Vector3d point1, point2;
    RU::value_to_vector(v_point1, point1);
    RU::value_to_vector(v_point2, point2);
    double ratio = RU::value_to_double(v_ratio);
    return RU::point_to_value(point1.transition_to(point2, ratio));
}

VALUE AMS::Geometry::rbf_transition_vector(VALUE self, VALUE v_vector1, VALUE v_vector2, VALUE v_ratio) {
    Geom::Vector3d vector1, vector2;
    RU::value_to_vector(v_vector1, vector1);
    RU::value_to_vector(v_vector2, vector2);
    double ratio = RU::value_to_double(v_ratio);
    return RU::vector_to_value(vector1.rotate_and_scale_to(vector2, ratio));
}

VALUE AMS::Geometry::rbf_transition_transformation(VALUE self, VALUE v_transformation1, VALUE v_transformation2, VALUE v_ratio) {
    Geom::Transformation t1, t2;
    RU::value_to_transformation(v_transformation1, t1);
    RU::value_to_transformation(v_transformation2, t2);
    double ratio = RU::value_to_double(v_ratio);
    return RU::transformation_to_value(t1.uniform_transition_to(t2, ratio));
}

VALUE AMS::Geometry::rbf_transition_number(VALUE self, VALUE v_number1, VALUE v_number2, VALUE v_ratio) {
    double number1 = RU::value_to_double(v_number1);
    double number2 = RU::value_to_double(v_number2);
    double ratio = RU::value_to_double(v_ratio);
    double number3 = number1 + (number2 - number1) * ratio;
    return RU::to_value(number3);
}

VALUE AMS::Geometry::rbf_blend_colors(VALUE self, VALUE v_colors, VALUE v_ratio) {
    unsigned int colors_size, i1, i2;
    Geom::Color color1, color2;
    double ratio, cr, lr;
    if (TYPE(v_colors) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'colors' parameter!");
    colors_size = (unsigned int)RARRAY_LEN(v_colors);
    if (colors_size == 0)
        rb_raise(rb_eTypeError, "Expected at least one color but got none!");
    else if (colors_size == 1) {
        RU::value_to_color(rb_ary_entry(v_colors, 0), color1);
        return RU::color_to_value(color1);
    }
    ratio = Geom::clamp_double(RU::value_to_double(v_ratio), 0.0, 1.0);
    cr = static_cast<double>(colors_size - 1) * ratio;
    i1 = static_cast<unsigned int>(cr);
    i2 = i1 + 1;
    if (i2 == colors_size) --i2;
    lr = cr - static_cast<double>(i1);
    RU::value_to_color(rb_ary_entry(v_colors, i1), color1);
    RU::value_to_color(rb_ary_entry(v_colors, i2), color2);
    return RU::color_to_value(color1.transition_to(color2, lr));
}

VALUE AMS::Geometry::rbf_get_points_on_circle2d(int argc, VALUE* argv, VALUE self) {
    unsigned int num_seg = 16;
    unsigned int i = 0;
    double rot_angle = 0.0;
    double radius, offset, angle;
    Geom::Vector3d origin, point;
    VALUE v_points;
    if (argc == 4) {
        num_seg = RU::value_to_uint(argv[2]);
        rot_angle = RU::value_to_double(argv[3]);
    }
    else if (argc == 3)
        num_seg = RU::value_to_uint(argv[2]);
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..4 arguments.");
    RU::value_to_vector(argv[0], origin);
    radius = RU::value_to_double(argv[1]);
    v_points = rb_ary_new2(num_seg);
    offset = M_SPI2 / num_seg;
    angle = rot_angle;
    for (; i < num_seg; ++i) {
        point.m_x = origin.m_x + cos(angle) * radius;
        point.m_y = origin.m_y + sin(angle) * radius;
        point.m_z = origin.m_z;
        rb_ary_store(v_points, i, RU::point_to_value(point));
        angle += offset;
    }
    return v_points;
}

VALUE AMS::Geometry::rbf_get_points_on_circle3d(int argc, VALUE* argv, VALUE self) {
    unsigned int num_seg = 16;
    unsigned int i = 0;
    double rot_angle = 0.0;
    double radius, offset, angle;
    Geom::Vector3d origin, normal, xaxis, yaxis, point;
    VALUE v_points;
    if (argc == 5) {
        num_seg = RU::value_to_uint(argv[3]);
        rot_angle = RU::value_to_double(argv[4]);
    }
    else if (argc == 4)
        num_seg = RU::value_to_uint(argv[3]);
    else if (argc != 3)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 3..5 arguments.");
    RU::value_to_vector(argv[0], origin);
    RU::value_to_vector(argv[1], normal);
    radius = RU::value_to_double(argv[2]);
    if (fabs(normal.m_z) < (treal)(0.9999995))
        xaxis = Geom::Vector3d::Z_AXIS.cross(normal);
    else
        xaxis = Geom::Vector3d::Y_AXIS.cross(normal);
    xaxis.normalize_self();
    yaxis = normal.cross(xaxis);
    yaxis.normalize_self();
    v_points = rb_ary_new2(num_seg);
    offset = M_SPI2 / num_seg;
    angle = rot_angle;
    for (; i < num_seg; ++i) {
        point = origin + xaxis.scale(cos(angle) * radius) + yaxis.scale(sin(angle) * radius);
        rb_ary_store(v_points, i, RU::point_to_value(point));
        angle += offset;
    }
    return v_points;
}

VALUE AMS::Geometry::rbf_raytest1(int argc, VALUE* argv, VALUE self) {
    VALUE v_nchg = Qtrue;
    if (argc == 4)
        v_nchg = (argv[3] == Qtrue ? Qfalse : Qtrue);
    else if (argc != 3)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 3..4 arguments.");
    VALUE v_ents = argv[0];
    VALUE v_point = argv[1];
    VALUE v_vector = argv[2];
    if (TYPE(v_ents) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'entities' parameter!");

    std::map<VALUE, unsigned int> ents;
    unsigned int ents_size = (unsigned int)RARRAY_LEN(v_ents);
    for (unsigned int i = 0; i < ents_size; ++i)
        ents[rb_ary_entry(v_ents, i)] = i;

    VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    while (true) {
        VALUE v_hit = rb_funcall(v_active_model, RU::INTERN_RAYTEST, 3, v_point, v_vector, v_nchg);
        if (v_hit == Qnil) break;
        VALUE v_hit_ents = rb_ary_entry(v_hit, 1);
        unsigned int hit_ents_size = (unsigned int)RARRAY_LEN(v_hit_ents);
        for (unsigned int i = 0; i < hit_ents_size; ++i)
            if (ents.find(rb_ary_entry(v_hit_ents, i)) != ents.end()) return v_hit;
        v_point = rb_ary_entry(v_hit, 0);
    }
    return Qnil;
}

VALUE AMS::Geometry::rbf_raytest2(int argc, VALUE* argv, VALUE self) {
    VALUE v_nchg = Qtrue;
    if (argc == 4)
        v_nchg = (argv[3] == Qtrue ? Qfalse : Qtrue);
    else if (argc != 3)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 3..4 arguments.");
    VALUE v_ents = argv[0];
    VALUE v_point = argv[1];
    VALUE v_vector = argv[2];
    if (TYPE(v_ents) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'entities' parameter!");

    std::map<VALUE, unsigned int> ents;
    unsigned int ents_size = (unsigned int)RARRAY_LEN(v_ents);
    for (unsigned int i = 0; i < ents_size; ++i)
        ents[rb_ary_entry(v_ents, i)] = i;

    VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    while (true) {
        VALUE v_hit = rb_funcall(v_active_model, RU::INTERN_RAYTEST, 3, v_point, v_vector, v_nchg);
        if (v_hit == Qnil) break;
        VALUE v_hit_ents = rb_ary_entry(v_hit, 1);
        unsigned int hit_ents_size = (unsigned int)RARRAY_LEN(v_hit_ents);
        bool found = false;
        for (unsigned int i = 0; i < hit_ents_size; ++i) {
            if (ents.find(rb_ary_entry(v_hit_ents, i)) != ents.end()) {
                found = true;
                break;
            }
        }
        if (!found) return v_hit;
        v_point = rb_ary_entry(v_hit, 0);
    }
    return Qnil;
}

VALUE AMS::Geometry::rbf_raytest3(int argc, VALUE* argv, VALUE self) {
    VALUE v_nchg = Qtrue;
    Geom::Vector3d vector, face_normal;
    Geom::Transformation inst_tra;
    if (argc == 3)
        v_nchg = (argv[2] == Qtrue ? Qfalse : Qtrue);
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");
    VALUE v_point = argv[0];
    VALUE v_vector = argv[1];
    RU::value_to_vector(v_vector, vector);
    VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    while (true) {
        VALUE v_hit = rb_funcall(v_active_model, RU::INTERN_RAYTEST, 3, v_point, v_vector, v_nchg);
        if (v_hit == Qnil) break;
        VALUE v_hit_ents = rb_ary_entry(v_hit, 1);
        unsigned int hit_ents_size = (unsigned int)RARRAY_LEN(v_hit_ents);
        VALUE v_face = rb_ary_entry(v_hit_ents, hit_ents_size - 1);
        if (rb_obj_is_kind_of(v_face, RU::SU_FACE) == Qfalse) return v_hit;
        VALUE v_inst_mat = Qnil;
        RU::value_to_vector(rb_funcall(v_face, RU::INTERN_NORMAL, 0), face_normal);
        for (int i = hit_ents_size - 2; i >= 0; --i) {
            VALUE v_inst = rb_ary_entry(v_hit_ents, i);
            RU::value_to_transformation(rb_funcall(v_inst, RU::INTERN_TRANSFORMATION, 0), inst_tra);
            face_normal = inst_tra.rotate_vector(face_normal);
            if (v_inst_mat == Qnil) v_inst_mat = rb_funcall(v_inst, RU::INTERN_MATERIAL, 0);
        }
        VALUE v_face_mat = rb_funcall(v_face, (vector.dot(face_normal) < 0.0) ? RU::INTERN_MATERIAL : RU::INTERN_BACK_MATERIAL, 0);
        if (v_face_mat == Qnil) v_face_mat = v_inst_mat;
        if (v_face_mat == Qnil) return v_hit;
        double mat_alpha = RU::value_to_double(rb_funcall(v_face_mat, RU::INTERN_ALPHA, 0));
        if (mat_alpha > 0.9995) return v_hit;
        v_point = rb_ary_entry(v_hit, 0);
    }
    return Qnil;
}

VALUE AMS::Geometry::rbf_deepray1(int argc, VALUE* argv, VALUE self) {
    VALUE v_nchg = Qtrue;
    if (argc == 3)
        v_nchg = (argv[2] == Qtrue ? Qfalse : Qtrue);
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");
    VALUE v_point = argv[0];
    VALUE v_vector = argv[1];
    VALUE v_hits = rb_ary_new();
    VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    while (true) {
        VALUE v_hit = rb_funcall(v_active_model, RU::INTERN_RAYTEST, 3, v_point, v_vector, v_nchg);
        if (v_hit == Qnil) break;
        rb_ary_push(v_hits, v_hit);
        v_point = rb_ary_entry(v_hit, 0);
    }
    return v_hits;
}

VALUE AMS::Geometry::rbf_deepray2(int argc, VALUE* argv, VALUE self) {
    VALUE v_nchg = Qtrue;
    if (argc == 4)
        v_nchg = (argv[3] == Qtrue ? Qfalse : Qtrue);
    else if (argc != 3)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 3..4 arguments.");
    VALUE v_ents = argv[0];
    VALUE v_point = argv[1];
    VALUE v_vector = argv[2];
    if (TYPE(v_ents) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'entities' parameter!");

    std::map<VALUE, unsigned int> ents;
    unsigned int ents_size = (unsigned int)RARRAY_LEN(v_ents);
    for (unsigned int i = 0; i < ents_size; ++i)
        ents[rb_ary_entry(v_ents, i)] = i;

    VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_hits = rb_ary_new();
    while (true) {
        VALUE v_hit = rb_funcall(v_active_model, RU::INTERN_RAYTEST, 3, v_point, v_vector, v_nchg);
        if (v_hit == Qnil) break;
        VALUE v_hit_ents = rb_ary_entry(v_hit, 1);
        unsigned int hit_ents_size = (unsigned int)RARRAY_LEN(v_hit_ents);
        for (unsigned int i = 0; i < hit_ents_size; ++i) {
            if (ents.find(rb_ary_entry(v_hit_ents, i)) != ents.end()) {
                rb_ary_push(v_hits, v_hit);
                break;
            }
        }
        v_point = rb_ary_entry(v_hit, 0);
    }
    return v_hits;
}

VALUE AMS::Geometry::rbf_deepray3(int argc, VALUE* argv, VALUE self) {
    VALUE v_nchg = Qtrue;
    if (argc == 4)
        v_nchg = (argv[3] == Qtrue ? Qfalse : Qtrue);
    else if (argc != 3)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 3..4 arguments.");
    VALUE v_ents = argv[0];
    VALUE v_point = argv[1];
    VALUE v_vector = argv[2];
    if (TYPE(v_ents) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'entities' parameter!");

    std::map<VALUE, unsigned int> ents;
    unsigned int ents_size = (unsigned int)RARRAY_LEN(v_ents);
    for (unsigned int i = 0; i < ents_size; ++i)
        ents[rb_ary_entry(v_ents, i)] = i;

    VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_hits = rb_ary_new();
    while (true) {
        VALUE v_hit = rb_funcall(v_active_model, RU::INTERN_RAYTEST, 3, v_point, v_vector, v_nchg);
        if (v_hit == Qnil) break;
        VALUE v_hit_ents = rb_ary_entry(v_hit, 1);
        unsigned int hit_ents_size = (unsigned int)RARRAY_LEN(v_hit_ents);
        bool found = false;
        for (unsigned int i = 0; i < hit_ents_size; ++i) {
            if (ents.find(rb_ary_entry(v_hit_ents, i)) != ents.end()) {
                found = true;
                break;
            }
        }
        if (!found) rb_ary_push(v_hits, v_hit);
        v_point = rb_ary_entry(v_hit, 0);
    }
    return v_hits;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Geometry::init_ruby(VALUE mAMS) {
    VALUE mGeometry = rb_define_module_under(mAMS, "Geometry");

    rb_define_module_function(mGeometry, "calc_cubic_bezier_point", VALUEFUNC(AMS::Geometry::rbf_calc_cubic_bezier_point), 5);
    rb_define_module_function(mGeometry, "calc_cubic_bezier_slope", VALUEFUNC(AMS::Geometry::rbf_calc_cubic_bezier_slope), 5);
    rb_define_module_function(mGeometry, "scale_point", VALUEFUNC(AMS::Geometry::rbf_scale_point), 2);
    rb_define_module_function(mGeometry, "scale_vector", VALUEFUNC(AMS::Geometry::rbf_scale_vector), 2);
    rb_define_module_function(mGeometry, "scale_matrix", VALUEFUNC(AMS::Geometry::rbf_scale_matrix), 2);
    rb_define_module_function(mGeometry, "product_vectors", VALUEFUNC(AMS::Geometry::rbf_product_vectors), 2);
    rb_define_module_function(mGeometry, "rotate_vector", VALUEFUNC(AMS::Geometry::rbf_rotate_vector), 3);
    rb_define_module_function(mGeometry, "angle_between_vectors", VALUEFUNC(AMS::Geometry::rbf_angle_between_vectors), -1);
    rb_define_module_function(mGeometry, "get_unique_points", VALUEFUNC(AMS::Geometry::rbf_get_unique_points), 1);
    rb_define_module_function(mGeometry, "points_collinear?", VALUEFUNC(AMS::Geometry::rbf_points_collinear), 1);
    rb_define_module_function(mGeometry, "get_noncollinear_points", VALUEFUNC(AMS::Geometry::rbf_get_noncollinear_points), 1);
    rb_define_module_function(mGeometry, "get_plane_normal", VALUEFUNC(AMS::Geometry::rbf_get_plane_normal), 1);
    rb_define_module_function(mGeometry, "points_coplanar?", VALUEFUNC(AMS::Geometry::rbf_points_coplanar), 1);
    rb_define_module_function(mGeometry, "sort_polygon_points", VALUEFUNC(AMS::Geometry::rbf_sort_polygon_points), 1);
    rb_define_module_function(mGeometry, "calc_edge_centre", VALUEFUNC(AMS::Geometry::rbf_calc_edge_centre), 1);
    rb_define_module_function(mGeometry, "calc_face_centre", VALUEFUNC(AMS::Geometry::rbf_calc_face_centre), 1);
    rb_define_module_function(mGeometry, "is_point_on_edge?", VALUEFUNC(AMS::Geometry::rbf_is_point_on_edge), 2);
    rb_define_module_function(mGeometry, "is_point_on_face?", VALUEFUNC(AMS::Geometry::rbf_is_point_on_face), 2);
    rb_define_module_function(mGeometry, "is_point_on_triangle?", VALUEFUNC(AMS::Geometry::rbf_is_point_on_triangle), 4);
    rb_define_module_function(mGeometry, "intersect_ray_triangle", VALUEFUNC(AMS::Geometry::rbf_intersect_ray_triangle), 5);
    rb_define_module_function(mGeometry, "get_matrix_scale", VALUEFUNC(AMS::Geometry::rbf_get_matrix_scale), 1);
    rb_define_module_function(mGeometry, "set_matrix_scale", VALUEFUNC(AMS::Geometry::rbf_set_matrix_scale), 2);
    rb_define_module_function(mGeometry, "extract_matrix_scale", VALUEFUNC(AMS::Geometry::rbf_extract_matrix_scale), 1);
    rb_define_module_function(mGeometry, "extract_matrix_w_factor", VALUEFUNC(AMS::Geometry::rbf_extract_matrix_w_factor), 1);
    rb_define_module_function(mGeometry, "is_matrix_flipped?", VALUEFUNC(AMS::Geometry::rbf_is_matrix_flipped), 1);
    rb_define_module_function(mGeometry, "is_matrix_uniform?", VALUEFUNC(AMS::Geometry::rbf_is_matrix_uniform), 1);
    rb_define_module_function(mGeometry, "rotate_matrix_xaxis_to_dir", VALUEFUNC(AMS::Geometry::rbf_rotate_matrix_xaxis_to_dir), 2);
    rb_define_module_function(mGeometry, "rotate_matrix_yaxis_to_dir", VALUEFUNC(AMS::Geometry::rbf_rotate_matrix_yaxis_to_dir), 2);
    rb_define_module_function(mGeometry, "rotate_matrix_zaxis_to_dir", VALUEFUNC(AMS::Geometry::rbf_rotate_matrix_zaxis_to_dir), 2);
    rb_define_module_function(mGeometry, "transition_camera", VALUEFUNC(AMS::Geometry::rbf_transition_camera), 3);
    rb_define_module_function(mGeometry, "transition_color", VALUEFUNC(AMS::Geometry::rbf_transition_color), 3);
    rb_define_module_function(mGeometry, "transition_point", VALUEFUNC(AMS::Geometry::rbf_transition_point), 3);
    rb_define_module_function(mGeometry, "transition_vector", VALUEFUNC(AMS::Geometry::rbf_transition_vector), 3);
    rb_define_module_function(mGeometry, "transition_transformation", VALUEFUNC(AMS::Geometry::rbf_transition_transformation), 3);
    rb_define_module_function(mGeometry, "transition_number", VALUEFUNC(AMS::Geometry::rbf_transition_number), 3);
    rb_define_module_function(mGeometry, "blend_colors", VALUEFUNC(AMS::Geometry::rbf_blend_colors), 2);
    rb_define_module_function(mGeometry, "get_points_on_circle2d", VALUEFUNC(AMS::Geometry::rbf_get_points_on_circle2d), -1);
    rb_define_module_function(mGeometry, "get_points_on_circle3d", VALUEFUNC(AMS::Geometry::rbf_get_points_on_circle3d), -1);
    rb_define_module_function(mGeometry, "raytest1", VALUEFUNC(AMS::Geometry::rbf_raytest1), -1);
    rb_define_module_function(mGeometry, "raytest2", VALUEFUNC(AMS::Geometry::rbf_raytest2), -1);
    rb_define_module_function(mGeometry, "raytest3", VALUEFUNC(AMS::Geometry::rbf_raytest3), -1);
    rb_define_module_function(mGeometry, "deepray1", VALUEFUNC(AMS::Geometry::rbf_deepray1), -1);
    rb_define_module_function(mGeometry, "deepray2", VALUEFUNC(AMS::Geometry::rbf_deepray2), -1);
    rb_define_module_function(mGeometry, "deepray3", VALUEFUNC(AMS::Geometry::rbf_deepray3), -1);
}
