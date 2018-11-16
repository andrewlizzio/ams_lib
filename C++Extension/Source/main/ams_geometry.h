/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_GEOMETRY_H
#define AMS_GEOMETRY_H

#include "ams.h"

class AMS::Geometry {
public:
    // Ruby Functions
    static VALUE rbf_calc_cubic_bezier_point(VALUE self, VALUE v_t, VALUE v_p0, VALUE v_p1, VALUE v_p2, VALUE v_p3);
    static VALUE rbf_calc_cubic_bezier_slope(VALUE self, VALUE v_t, VALUE v_p0, VALUE v_p1, VALUE v_p2, VALUE v_p3);
    static VALUE rbf_scale_point(VALUE self, VALUE v_point, VALUE v_scale);
    static VALUE rbf_scale_vector(VALUE self, VALUE v_vector, VALUE v_scale);
    static VALUE rbf_scale_matrix(VALUE self, VALUE v_matrix, VALUE v_scale);
    static VALUE rbf_product_vectors(VALUE self, VALUE v_vector, VALUE v_other_vector);
    static VALUE rbf_rotate_vector(VALUE self, VALUE v_vector, VALUE v_normal, VALUE v_angle);
    static VALUE rbf_angle_between_vectors(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_unique_points(VALUE self, VALUE v_points);
    static VALUE rbf_points_collinear(VALUE self, VALUE v_points);
    static VALUE rbf_get_noncollinear_points(VALUE self, VALUE v_points);
    static VALUE rbf_get_plane_normal(VALUE self, VALUE v_plane);
    static VALUE rbf_points_coplanar(VALUE self, VALUE v_points);
    static VALUE rbf_sort_polygon_points(VALUE self, VALUE v_points);
    static VALUE rbf_calc_edge_centre(VALUE self, VALUE v_edge);
    static VALUE rbf_calc_face_centre(VALUE self, VALUE v_face);
    static VALUE rbf_is_point_on_edge(VALUE self, VALUE v_point, VALUE v_edge);
    static VALUE rbf_is_point_on_face(VALUE self, VALUE v_point, VALUE v_face);
    static VALUE rbf_is_point_on_triangle(VALUE self, VALUE v_point, VALUE v_pt1, VALUE v_pt2, VALUE v_pt3);
    static VALUE rbf_intersect_ray_triangle(VALUE self, VALUE v_origin, VALUE v_dir, VALUE v_pt1, VALUE v_pt2, VALUE v_pt3);
    static VALUE rbf_get_matrix_scale(VALUE self, VALUE v_matrix);
    static VALUE rbf_set_matrix_scale(VALUE self, VALUE v_matrix, VALUE v_scale);
    static VALUE rbf_extract_matrix_scale(VALUE self, VALUE v_matrix);
    static VALUE rbf_extract_matrix_w_factor(VALUE self, VALUE v_matrix);
    static VALUE rbf_is_matrix_flipped(VALUE self, VALUE v_matrix);
    static VALUE rbf_is_matrix_uniform(VALUE self, VALUE v_matrix);
    static VALUE rbf_rotate_matrix_xaxis_to_dir(VALUE self, VALUE v_matrix, VALUE v_dir);
    static VALUE rbf_rotate_matrix_yaxis_to_dir(VALUE self, VALUE v_matrix, VALUE v_dir);
    static VALUE rbf_rotate_matrix_zaxis_to_dir(VALUE self, VALUE v_matrix, VALUE v_dir);
    static VALUE rbf_transition_camera(VALUE self, VALUE v_camera1, VALUE v_camera2, VALUE v_ratio);
    static VALUE rbf_transition_color(VALUE self, VALUE v_color1, VALUE v_color2, VALUE v_ratio);
    static VALUE rbf_transition_point(VALUE self, VALUE v_point1, VALUE v_point2, VALUE v_ratio);
    static VALUE rbf_transition_vector(VALUE self, VALUE v_vector1, VALUE v_vector2, VALUE v_ratio);
    static VALUE rbf_transition_transformation(VALUE self, VALUE v_transformation1, VALUE v_transformation2, VALUE v_ratio);
    static VALUE rbf_transition_number(VALUE self, VALUE v_number1, VALUE v_number2, VALUE v_ratio);
    static VALUE rbf_blend_colors(VALUE self, VALUE v_colors, VALUE v_ratio);
    static VALUE rbf_get_points_on_circle2d(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_points_on_circle3d(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_raytest1(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_raytest2(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_raytest3(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_deepray1(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_deepray2(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_deepray3(int argc, VALUE* argv, VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_GEOMETRY_H */
