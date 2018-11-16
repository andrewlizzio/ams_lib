/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_GROUP_H
#define AMS_GROUP_H

#include "ams.h"

class AMS::Group {
private:
    // Helper Functions
    static void c_get_bounding_box_from_edges(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_bb);
    static void c_get_bounding_box_from_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_bb);
    static void c_get_edges(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_edges);
    static void c_get_vertices_from_edges(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_vertices);
    static void c_get_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_faces);
    static void c_get_vertices_from_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_vertices);
    static void c_get_vertices_from_faces2(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_collections);
    static void c_get_construction(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_points);
    static void c_get_polygons_from_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, bool transformation_flipped, VALUE& v_triplets);
    static void c_get_triangular_mesh(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, bool transformation_flipped, VALUE& v_mesh);
    static void c_get_triangular_meshes(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, bool transformation_flipped, VALUE& v_meshes);
    static void c_calc_centre_of_mass(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, Geom::Vector3d& magnified_centre, double& total_darea);
    static bool c_exclude_sub(VALUE self, VALUE v_entity, bool recurse);
    static VALUE c_split(VALUE self, VALUE v_entity, const Geom::Vector3d& point, const Geom::Vector3d& normal, VALUE v_context, bool recurse);

public:
    // Ruby Functions
    static VALUE rbf_get_definition(VALUE self, VALUE v_entity);
    static VALUE rbf_get_entities(VALUE self, VALUE v_entity);
    static VALUE rbf_get_bounding_box_from_edges(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_bounding_box_from_faces(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_edges(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_vertices_from_edges(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_faces(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_vertices_from_faces(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_vertices_from_faces2(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_construction(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_polygons_from_faces(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_triangular_mesh(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_triangular_meshes(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_calc_centre_of_mass(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_copy(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_split(int argc, VALUE* argv, VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_GROUP_H */
