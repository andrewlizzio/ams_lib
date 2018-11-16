/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_group.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Group::c_get_bounding_box_from_edges(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_bb) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_EDGE) == Qtrue) {
            VALUE v_position1 = rb_funcall(rb_funcall(v_sub_entity, RU::INTERN_START, 0), RU::INTERN_POSITION, 0);
            VALUE v_position2 = rb_funcall(rb_funcall(v_sub_entity, RU::INTERN_END, 0), RU::INTERN_POSITION, 0);
            if (v_transformation != Qnil) {
                rb_funcall(v_position1, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_funcall(v_position2, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
            }
            rb_funcall(v_bb, RU::INTERN_ADD, 1, v_position1);
            rb_funcall(v_bb, RU::INTERN_ADD, 1, v_position2);
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_bounding_box_from_edges(self, v_sub_entity, true, v_new_transformation, v_bb);
            }
        }
    }
}

void AMS::Group::c_get_bounding_box_from_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_bb) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            VALUE v_sub_entity_vertices = rb_funcall(v_sub_entity, RU::INTERN_VERTICES, 0);
            unsigned int sub_entity_vertices_size = (unsigned int)RARRAY_LEN(v_sub_entity_vertices);
            for (unsigned int j = 0; j < sub_entity_vertices_size; ++j) {
                VALUE v_position = rb_funcall(rb_ary_entry(v_sub_entity_vertices, j), RU::INTERN_POSITION, 0);
                if (v_transformation != Qnil)
                    rb_funcall(v_position, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_funcall(v_bb, RU::INTERN_ADD, 1, v_position);
            }
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_bounding_box_from_faces(self, v_sub_entity, true, v_new_transformation, v_bb);
            }
        }
    }
}

void AMS::Group::c_get_edges(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_edges) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_EDGE) == Qtrue) {
            VALUE v_position1 = rb_funcall(rb_funcall(v_sub_entity, RU::INTERN_START, 0), RU::INTERN_POSITION, 0);
            VALUE v_position2 = rb_funcall(rb_funcall(v_sub_entity, RU::INTERN_END, 0), RU::INTERN_POSITION, 0);
            if (v_transformation != Qnil) {
                rb_funcall(v_position1, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_funcall(v_position2, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
            }
            rb_ary_push(v_edges, rb_ary_new3(2, v_position1, v_position2));
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_edges(self, v_sub_entity, true, v_new_transformation, v_edges);
            }
        }
    }
}

void AMS::Group::c_get_vertices_from_edges(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_vertices) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_EDGE) == Qtrue) {
            VALUE v_position1 = rb_funcall(rb_funcall(v_sub_entity, RU::INTERN_START, 0), RU::INTERN_POSITION, 0);
            VALUE v_position2 = rb_funcall(rb_funcall(v_sub_entity, RU::INTERN_END, 0), RU::INTERN_POSITION, 0);
            if (v_transformation != Qnil) {
                rb_funcall(v_position1, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_funcall(v_position2, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
            }
            rb_ary_push(v_vertices, v_position1);
            rb_ary_push(v_vertices, v_position2);
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_vertices_from_edges(self, v_sub_entity, true, v_new_transformation, v_vertices);
            }
        }
    }
}

void AMS::Group::c_get_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_faces) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            VALUE v_sub_entity_vertices = rb_funcall(v_sub_entity, RU::INTERN_VERTICES, 0);
            unsigned int sub_entity_vertices_size = (unsigned int)RARRAY_LEN(v_sub_entity_vertices);
            VALUE v_face = rb_ary_new2(sub_entity_vertices_size);
            for (unsigned int j = 0; j < sub_entity_vertices_size; ++j) {
                VALUE v_position = rb_funcall(rb_ary_entry(v_sub_entity_vertices, j), RU::INTERN_POSITION, 0);
                if (v_transformation != Qnil)
                    rb_funcall(v_position, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_ary_store(v_face, j, v_position);
            }
            rb_ary_push(v_faces, v_face);
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_faces(self, v_sub_entity, true, v_new_transformation, v_faces);
            }
        }
    }
}

void AMS::Group::c_get_vertices_from_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_vertices) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            VALUE v_sub_entity_vertices = rb_funcall(v_sub_entity, RU::INTERN_VERTICES, 0);
            unsigned int sub_entity_vertices_size = (unsigned int)RARRAY_LEN(v_sub_entity_vertices);
            for (unsigned int j = 0; j < sub_entity_vertices_size; ++j) {
                VALUE v_position = rb_funcall(rb_ary_entry(v_sub_entity_vertices, j), RU::INTERN_POSITION, 0);
                if (v_transformation != Qnil)
                    rb_funcall(v_position, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_ary_push(v_vertices, v_position);
            }
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_vertices_from_faces(self, v_sub_entity, true, v_new_transformation, v_vertices);
            }
        }
    }
}

void AMS::Group::c_get_vertices_from_faces2(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_collections) {
    VALUE v_collection = rb_ary_new();
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            VALUE v_sub_entity_vertices = rb_funcall(v_sub_entity, RU::INTERN_VERTICES, 0);
            unsigned int sub_entity_vertices_size = (unsigned int)RARRAY_LEN(v_sub_entity_vertices);
            for (unsigned int j = 0; j < sub_entity_vertices_size; ++j) {
                VALUE v_position = rb_funcall(rb_ary_entry(v_sub_entity_vertices, j), RU::INTERN_POSITION, 0);
                if (v_transformation != Qnil)
                    rb_funcall(v_position, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_ary_push(v_collection, v_position);
            }
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_vertices_from_faces2(self, v_sub_entity, true, v_new_transformation, v_collections);
            }
        }
    }
    rb_ary_push(v_collections, v_collection);
}

void AMS::Group::c_get_construction(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, VALUE& v_points) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_CONSTRUCTION_POINT) == Qtrue) {
            VALUE v_position = rb_funcall(v_sub_entity, RU::INTERN_POSITION, 0);
            if (v_transformation != Qnil)
                rb_funcall(v_position, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
            rb_ary_push(v_points, v_position);
        }
        else if (rb_obj_is_kind_of(v_sub_entity, RU::SU_CONSTRUCTION_LINE) == Qtrue) {
            VALUE v_vertex1 = rb_funcall(v_sub_entity, RU::INTERN_START, 0);
            VALUE v_vertex2 = rb_funcall(v_sub_entity, RU::INTERN_END, 0);
            if (v_vertex1 != Qnil) {
                VALUE v_position1 = rb_funcall(v_vertex1, RU::INTERN_POSITION, 0);
                if (v_transformation != Qnil)
                    rb_funcall(v_position1, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_ary_push(v_points, v_position1);
            }
            if (v_vertex2 != Qnil) {
                VALUE v_position2 = rb_funcall(v_vertex2, RU::INTERN_POSITION, 0);
                if (v_transformation != Qnil)
                    rb_funcall(v_position2, RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
                rb_ary_push(v_points, v_position2);
            }
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                c_get_construction(self, v_sub_entity, true, v_new_transformation, v_points);
            }
        }
    }
}

void AMS::Group::c_get_polygons_from_faces(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, bool transformation_flipped, VALUE& v_triplets) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            VALUE v_face_mesh = rb_funcall(v_sub_entity, RU::INTERN_MESH, 0);
            if (v_transformation != Qnil)
                rb_funcall(v_face_mesh,  RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
            unsigned int number_of_polygons = RU::value_to_uint(rb_funcall(v_face_mesh, RU::INTERN_COUNT_POLYGONS, 0));
            for (unsigned int j = 0; j < number_of_polygons; ++j) {
                VALUE v_triplet = rb_funcall(v_face_mesh, RU::INTERN_POLYGON_POINTS_AT, 1, RU::to_value(j+1));
                if (transformation_flipped)
                    rb_ary_reverse(v_triplet);
                rb_ary_push(v_triplets, v_triplet);
            }
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                Geom::Transformation new_parent_tra;
                RU::value_to_transformation(v_new_transformation, new_parent_tra);
                c_get_polygons_from_faces(self, v_sub_entity, true, v_new_transformation, new_parent_tra.is_flipped(), v_triplets);
            }
        }
    }
}

void AMS::Group::c_get_triangular_mesh(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, bool transformation_flipped, VALUE& v_mesh) {
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            VALUE v_face_mesh = rb_funcall(v_sub_entity, RU::INTERN_MESH, 0);
            if (v_transformation != Qnil)
                rb_funcall(v_face_mesh,  RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
            unsigned int number_of_polygons = RU::value_to_uint(rb_funcall(v_face_mesh, RU::INTERN_COUNT_POLYGONS, 0));
            for (unsigned int j = 0; j < number_of_polygons; ++j) {
                VALUE v_triplet = rb_funcall(v_face_mesh, RU::INTERN_POLYGON_POINTS_AT, 1, RU::to_value(j+1));
                if (transformation_flipped)
                    rb_ary_reverse(v_triplet);
                rb_funcall(v_mesh, RU::INTERN_ADD_POLYGON, 1, v_triplet);
            }
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                Geom::Transformation new_parent_tra;
                RU::value_to_transformation(v_new_transformation, new_parent_tra);
                c_get_triangular_mesh(self, v_sub_entity, true, v_new_transformation, new_parent_tra.is_flipped(), v_mesh);
            }
        }
    }
}

void AMS::Group::c_get_triangular_meshes(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, bool transformation_flipped, VALUE& v_meshes) {
    std::map<VALUE, bool> faces;
    VALUE v_entities = rbf_get_entities(self, v_entity);
    unsigned int entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (unsigned int i = 0; i < entities_size; ++i) {
        VALUE v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            faces[v_sub_entity] = true;
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                VALUE v_new_transformation = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_transformation = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_transformation);
                Geom::Transformation new_parent_tra;
                RU::value_to_transformation(v_new_transformation, new_parent_tra);
                c_get_triangular_meshes(self, v_sub_entity, true, v_new_transformation, new_parent_tra.is_flipped(), v_meshes);
            }
        }
    }
    while(true) {
        VALUE v_face = Qnil;
        for (std::map<VALUE, bool>::iterator it = faces.begin(); it != faces.end(); ++it) {
            if (it->second) {
                v_face = it->first;
                break;
            }
        }
        if (v_face == Qnil) break;
        VALUE v_mesh = rb_class_new_instance(0, nullptr, RU::SU_POLYGON_MESH);
        VALUE v_all_connected = rb_funcall(v_face, RU::INTERN_ALL_CONNECTED, 0);
        // all_connected also includes the face itself
        unsigned int all_connected_size = RU::value_to_uint(rb_funcall(v_all_connected, RU::INTERN_LENGTH, 0));
        for (unsigned int i = 0; i < all_connected_size; ++i) {
            VALUE v_connected_item = rb_funcall(v_all_connected, RU::INTERN_AT, 1, INT2FIX(i));
            if (rb_obj_is_kind_of(v_connected_item, RU::SU_FACE) == Qtrue) {
                std::map<VALUE, bool>::iterator it = faces.find(v_connected_item);
                if (it != faces.end()) {
                    VALUE v_face_mesh = rb_funcall(v_connected_item, RU::INTERN_MESH, 0);
                    unsigned int number_of_polygons = RU::value_to_uint(rb_funcall(v_face_mesh, RU::INTERN_COUNT_POLYGONS, 0));
                    for (unsigned int j = 0; j < number_of_polygons; ++j) {
                        VALUE v_triplet = rb_funcall(v_face_mesh, RU::INTERN_POLYGON_POINTS_AT, 1, RU::to_value(j+1));
                        if (transformation_flipped)
                            rb_ary_reverse(v_triplet);
                        rb_funcall(v_mesh, RU::INTERN_ADD_POLYGON, 1, v_triplet);
                    }
                    it->second = false;
                }
            }
        }
        if (v_transformation != Qnil)
            rb_funcall(v_mesh,  RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
        rb_ary_push(v_meshes, v_mesh);
    }
}

void AMS::Group::c_calc_centre_of_mass(VALUE self, VALUE v_entity, bool recurse, VALUE v_transformation, Geom::Vector3d& magnified_centre, double& total_darea) {
    // Declare variables
    Geom::Vector3d pt1, pt2, pt3, e0, e1;
    VALUE v_entities, v_sub_entity, v_face_mesh, v_new_tra;
    unsigned int entities_size, num_triplets, i, j;
    double darea;
    // Convert and compute

    v_entities = rbf_get_entities(self, v_entity);
    entities_size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
    for (i = 0; i < entities_size; ++i) {
        v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_FACE) == Qtrue) {
            v_face_mesh = rb_funcall(v_sub_entity, RU::INTERN_MESH, 0);
            if (v_transformation != Qnil)
                rb_funcall(v_face_mesh,  RU::INTERN_TRANSFORM_SELF, 1, v_transformation);
            num_triplets = RU::value_to_uint(rb_funcall(v_face_mesh, RU::INTERN_COUNT_POLYGONS, 0));
            for (j = 0; j < num_triplets; ++j) {
                // Use Heron's formula to compute the area of the triangle.
                VALUE v_triplet = rb_funcall(v_face_mesh, RU::INTERN_POLYGON_POINTS_AT, 1, RU::to_value(j + 1));
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
                magnified_centre += (pt1 + pt2 + pt3).scale(darea);
            }
        }
        else if (recurse == true && (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue || rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)) {
            if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true) {
                v_new_tra = rb_funcall(v_sub_entity, RU::INTERN_TRANSFORMATION, 0);
                if (v_transformation != Qnil)
                    v_new_tra = rb_funcall(v_transformation, RU::INTERN_OP_ASTERISKS, 1, v_new_tra);
                c_calc_centre_of_mass(self, v_sub_entity, true, v_new_tra, magnified_centre, total_darea);
            }
        }
    }
}

bool AMS::Group::c_exclude_sub(VALUE self, VALUE v_entity, bool recurse) {
    VALUE v_entities, v_sub_entity;
    unsigned int ary_len, i;
    std::vector<VALUE> to_erase;

    if (rb_block_given_p() == 0)
        return v_entity;

    rb_funcall(v_entity, RU::INTERN_MAKE_UNIQUE, 0);

    v_entities = rbf_get_entities(self, v_entity);
    ary_len = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));

    for (i = 0; i < ary_len; ++i) {
        v_sub_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue ||
            rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue) {
            if (RTEST(rb_yield(v_sub_entity))) {
                if (recurse && c_exclude_sub(self, v_sub_entity, true))
                    to_erase.push_back(v_sub_entity);
            }
            else
                to_erase.push_back(v_sub_entity);
        }
    }

    for (std::vector<VALUE>::iterator it = to_erase.begin(); it != to_erase.end(); ++it) {
        if (rb_funcall(*it, RU::INTERN_TVALID, 0) == Qtrue)
            rb_funcall(*it, RU::INTERN_EERASE, 0);
    }
    to_erase.clear();

    return (RU::value_to_int(rb_funcall(v_entities, RU::INTERN_COUNT, 0)) == 0);
}

VALUE AMS::Group::c_split(VALUE self, VALUE v_entity, const Geom::Vector3d& point, const Geom::Vector3d& normal, VALUE v_context, bool recurse) {
    // A fast way to split is by creating a copy of two groups
    // and then dividing it at plane
    // All objects behind plane are be deleted

    // Declare variables
    VALUE v_gents, v_gtra, v_gbb, v_plg, v_plg_ents, v_gface, v_gedges, v_edge, v_sub_entity, v_pt1, v_pt2;
    Geom::Transformation gtra, pl_tra, pl_tra_loc;
    Geom::BoundingBox gbb;
    Geom::Vector3d center, pt1, pt2, pt3, pt4, loc_point, loc_normal;
    double x;
    unsigned int i, ary_len;
    std::vector<VALUE> items;

    // Make group unique
    rb_funcall(v_entity, RU::INTERN_MAKE_UNIQUE, 0);

    // Obtain group information
    v_gents = rbf_get_entities(self, v_entity);
    v_gtra = rb_funcall(v_entity, RU::INTERN_TRANSFORMATION, 0);
    v_gbb = rb_funcall(v_entity, RU::INTERN_BOUNDS, 0);

    RU::value_to_transformation(v_gtra, gtra);
    RU::value_to_bb(v_gbb, gbb);

    // Shift plane origin so that it aligns with the group's center relative to plane normal.
    gbb.get_center(center);
    x = (point - center).dot(normal);
    pl_tra = Geom::Transformation(center + normal.scale(x), normal);

    // Compute local transformation of plane group
    pl_tra_loc = pl_tra * gtra.inverse();

    // Obtain normal point and direction
    pl_tra_loc.get_normal_origin(loc_point);
    pl_tra_loc.get_normal_zaxis(loc_normal);

    // Create a plane group for intersecting with
    v_plg = rb_funcall(v_context, RU::INTERN_ADD_GROUP, 0);
    v_plg_ents = rb_funcall(v_plg, RU::INTERN_ENTITIES, 0);
    rb_funcall(v_plg, RU::INTERN_EMOVE, 1, RU::transformation_to_value(pl_tra));

    // Compute face dimentions
    x = Geom::max_double(gbb.get_diagonal(), 1.0);

    pt1 = Geom::Vector3d(-x, -x, 0.0);
    pt2 = Geom::Vector3d( x, -x, 0.0);
    pt3 = Geom::Vector3d( x,  x, 0.0);
    pt4 = Geom::Vector3d(-x,  x, 0.0);

    // Add face to group being split
    v_gface = rb_funcall(v_gents, RU::INTERN_ADD_FACE, 4,
        RU::point_to_value(pl_tra_loc.transform_vector(pt1)),
        RU::point_to_value(pl_tra_loc.transform_vector(pt2)),
        RU::point_to_value(pl_tra_loc.transform_vector(pt3)),
        RU::point_to_value(pl_tra_loc.transform_vector(pt4)));

    // Add face to plane group
    rb_funcall(v_plg_ents, RU::INTERN_ADD_FACE, 4,
        RU::point_to_value(pt1.scale(2.0)),
        RU::point_to_value(pt2.scale(2.0)),
        RU::point_to_value(pt3.scale(2.0)),
        RU::point_to_value(pt4.scale(2.0)));

    // Obtain outer edges of the added face for erasing them in the end
    if (v_gface != Qnil)
        v_gedges = rb_funcall(v_gface, RU::INTERN_EDGES, 0);
    else
        v_gedges = Qnil;

    // Intersect group with plane
    rb_funcall(v_gents, RU::INTERN_INTERSECT_WITH, 6, Qfalse, v_gtra, v_gents, v_gtra, Qtrue, v_plg);

    // Erase the created outer edges within the group being split
    if (v_gedges) {
        ary_len = static_cast<unsigned int>(RARRAY_LEN(v_gedges));
        for (i = 0; i < ary_len; ++i) {
            v_edge = rb_ary_entry(v_gedges, i);
            if (rb_funcall(v_edge, RU::INTERN_TVALID, 0) == Qtrue)
                rb_funcall(v_edge, RU::INTERN_EERASE, 0);
        }
    }

    // Erase the plane group
    if (rb_funcall(v_plg, RU::INTERN_TVALID, 0) == Qtrue)
        rb_funcall(v_plg, RU::INTERN_EERASE, 0);

    // Delete edges behind plane
    ary_len = RU::value_to_uint(rb_funcall(v_gents, RU::INTERN_LENGTH, 0));
    for (i = 0; i < ary_len; ++i) {
        v_sub_entity = rb_funcall(v_gents, RU::INTERN_AT, 1, INT2FIX(i));
        if (rb_obj_is_kind_of(v_sub_entity, RU::SU_EDGE) == Qtrue)
            items.push_back(v_sub_entity);
    }
    for (std::vector<VALUE>::iterator it = items.begin(); it != items.end(); ++it) {
        v_edge = *it;
        if (rb_funcall(v_edge, RU::INTERN_TVALID, 0) == Qtrue) {
            v_pt1 = rb_funcall(rb_funcall(v_edge, RU::INTERN_START, 0), RU::INTERN_POSITION, 0);
            v_pt2 = rb_funcall(rb_funcall(v_edge, RU::INTERN_END, 0), RU::INTERN_POSITION, 0);
            RU::value_to_vector(v_pt1, pt1);
            RU::value_to_vector(v_pt2, pt2);
            if ((pt1 - loc_point).dot(loc_normal) > 1.0e-4 || (pt2 - loc_point).dot(loc_normal) > 1.0e-4)
                rb_funcall(v_edge, RU::INTERN_EERASE, 0);
        }
    }
    items.clear();

    // Recurse
    if (recurse) {
        ary_len = RU::value_to_uint(rb_funcall(v_gents, RU::INTERN_LENGTH, 0));
        for (i = 0; i < ary_len; ++i) {
            v_sub_entity = rb_funcall(v_gents, RU::INTERN_AT, 1, INT2FIX(i));
            if (rb_obj_is_kind_of(v_sub_entity, RU::SU_GROUP) == Qtrue ||
                rb_obj_is_kind_of(v_sub_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue) {
                if (rb_block_given_p() == 0 || RTEST(rb_yield(v_sub_entity)) == true)
                    items.push_back(v_sub_entity);
            }
        }
        for (std::vector<VALUE>::iterator it = items.begin(); it != items.end(); ++it) {
            v_sub_entity = *it;
            if (rb_funcall(v_sub_entity, RU::INTERN_TVALID, 0) == Qtrue)
                c_split(self, v_sub_entity, loc_point, loc_normal, v_gents, true);
        }
        items.clear();
    }

    // Validate and return
    if (RU::value_to_int(rb_funcall(v_gents, RU::INTERN_COUNT, 0)) == 0) {
        rb_funcall(v_entity, RU::INTERN_EERASE, 0);
        return Qnil;
    }
    else
        return v_entity;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Group::rbf_get_definition(VALUE self, VALUE v_entity) {
    if (rb_obj_is_kind_of(v_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue)
        return rb_funcall(v_entity, RU::INTERN_DEFINITION, 0);
    if (RU::su_version >= 14) {
        VALUE v_entities = rb_funcall(v_entity, RU::INTERN_ENTITIES, 0);
        return rb_funcall(v_entities, RU::INTERN_PARENT, 0);
    }
    else {
        VALUE v_model = rb_funcall(v_entity, RU::INTERN_MODEL, 0);
        VALUE v_definitions = rb_funcall(v_model, RU::INTERN_DEFINITIONS, 0);
        unsigned int definitions_size = RU::value_to_uint(rb_funcall(v_definitions, RU::INTERN_LENGTH, 0));
        for (unsigned int i = 0; i < definitions_size; ++i) {
            VALUE v_definition = rb_funcall(v_definitions, RU::INTERN_AT, 1, INT2FIX(i));
            if (rb_funcall(v_definition, RU::INTERN_TGROUP, 0) == Qfalse) continue;
            VALUE v_instances = rb_funcall(v_definition, RU::INTERN_INSTANCES, 0);
            unsigned int instances_size = RU::value_to_uint(rb_funcall(v_instances, RU::INTERN_LENGTH, 0));
            for (unsigned int j = 0; j < instances_size; ++j) {
                VALUE v_instance = rb_funcall(v_instances, RU::INTERN_AT, 1, INT2FIX(j));
                if (v_instance == v_entity) return v_definition;
            }
        }
    }
    return Qnil;
}

VALUE AMS::Group::rbf_get_entities(VALUE self, VALUE v_entity) {
    if (rb_obj_is_kind_of(v_entity, RU::SU_COMPONENT_INSTANCE) == Qtrue) {
        VALUE v_definition = rb_funcall(v_entity, RU::INTERN_DEFINITION, 0);
        return rb_funcall(v_definition, RU::INTERN_ENTITIES, 0);
    }
    else
        return rb_funcall(v_entity, RU::INTERN_ENTITIES, 0);
}

VALUE AMS::Group::rbf_get_bounding_box_from_edges(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_bb = rb_class_new_instance(0, nullptr, RU::SU_BOUNDING_BOX);
    c_get_bounding_box_from_edges(self, argv[0], RTEST(v_recurse), v_transformation, v_bb);
    return v_bb;
}

VALUE AMS::Group::rbf_get_bounding_box_from_faces(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_bb = rb_class_new_instance(0, nullptr, RU::SU_BOUNDING_BOX);
    c_get_bounding_box_from_faces(self, argv[0], RTEST(v_recurse), v_transformation, v_bb);
    return v_bb;
}

VALUE AMS::Group::rbf_get_edges(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_edges = rb_ary_new();
    c_get_edges(self, argv[0], RTEST(v_recurse), v_transformation, v_edges);
    return v_edges;
}

VALUE AMS::Group::rbf_get_vertices_from_edges(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_vertices = rb_ary_new();
    c_get_vertices_from_edges(self, argv[0], RTEST(v_recurse), v_transformation, v_vertices);
    return v_vertices;
}

VALUE AMS::Group::rbf_get_faces(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_faces = rb_ary_new();
    c_get_faces(self, argv[0], RTEST(v_recurse), v_transformation, v_faces);
    return v_faces;
}

VALUE AMS::Group::rbf_get_vertices_from_faces(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_vertices = rb_ary_new();
    c_get_vertices_from_faces(self, argv[0], RTEST(v_recurse), v_transformation, v_vertices);
    return v_vertices;
}

VALUE AMS::Group::rbf_get_vertices_from_faces2(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_collections = rb_ary_new();
    c_get_vertices_from_faces2(self, argv[0], RTEST(v_recurse), v_transformation, v_collections);
    return v_collections;
}

VALUE AMS::Group::rbf_get_construction(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    VALUE v_points = rb_ary_new();
    c_get_construction(self, argv[0], RTEST(v_recurse), v_transformation, v_points);
    return v_points;
}

VALUE AMS::Group::rbf_get_polygons_from_faces(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    bool transformation_flipped = false;
    if (v_transformation != Qnil) {
        Geom::Transformation transformation;
        RU::value_to_transformation(v_transformation, transformation);
        transformation_flipped = transformation.is_flipped();
    }
    VALUE v_triplets = rb_ary_new();
    c_get_polygons_from_faces(self, argv[0], RTEST(v_recurse), v_transformation, transformation_flipped, v_triplets);
    return v_triplets;
}

VALUE AMS::Group::rbf_get_triangular_mesh(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    bool transformation_flipped = false;
    if (v_transformation != Qnil) {
        Geom::Transformation transformation;
        RU::value_to_transformation(v_transformation, transformation);
        transformation_flipped = transformation.is_flipped();
    }
    VALUE v_mesh = rb_class_new_instance(0, nullptr, RU::SU_POLYGON_MESH);
    c_get_triangular_mesh(self, argv[0], RTEST(v_recurse), v_transformation, transformation_flipped, v_mesh);
    return v_mesh;
}

VALUE AMS::Group::rbf_get_triangular_meshes(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    bool transformation_flipped = false;
    if (v_transformation != Qnil) {
        Geom::Transformation transformation;
        RU::value_to_transformation(v_transformation, transformation);
        transformation_flipped = transformation.is_flipped();
    }
    VALUE v_meshes = rb_ary_new();
    c_get_triangular_meshes(self, argv[0], RTEST(v_recurse), v_transformation, transformation_flipped, v_meshes);
    return v_meshes;
}

VALUE AMS::Group::rbf_calc_centre_of_mass(int argc, VALUE* argv, VALUE self) {
    VALUE v_recurse = Qtrue;
    VALUE v_transformation = Qnil;
    if (argc == 3) {
        v_recurse = argv[1];
        v_transformation = argv[2];
    }
    else if (argc == 2)
        v_recurse = argv[1];
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    Geom::Vector3d magnified_centre;
    double total_darea = 0.0;
    c_calc_centre_of_mass(self, argv[0], RTEST(v_recurse), v_transformation, magnified_centre, total_darea);
    if (total_darea > M_EPSILON)
        magnified_centre.scale_self((1.0 / 3.0) / total_darea);
    return RU::point_to_value(magnified_centre);
}

VALUE AMS::Group::rbf_copy(int argc, VALUE* argv, VALUE self) {
    VALUE v_transformation = Qnil;
    VALUE v_recurse = Qtrue;
    if (argc == 4) {
        v_transformation = argv[2];
        v_recurse = argv[3];
    }
    else if (argc == 3) {
        v_transformation = argv[2];
    }
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..4 arguments.");
    VALUE v_entity = argv[0];
    VALUE v_context = argv[1];
    if (v_transformation == Qnil)
        v_transformation = rb_funcall(v_entity, RU::INTERN_TRANSFORMATION, 0);

    bool recurse = RTEST(v_recurse);

    // Create instances
    VALUE v_definition = rbf_get_definition(self, v_entity);
    VALUE v_inst = rb_funcall(v_context, RU::INTERN_ADD_INSTANCE, 2, v_definition, v_transformation);

    VALUE v_material = rb_funcall(v_entity, RU::INTERN_MATERIAL, 0);
    VALUE v_layer = rb_funcall(v_entity, RU::INTERN_LAYER, 0);
    VALUE v_casts_shadows = rb_funcall(v_entity, RU::INTERN_TCASTS_SHADOWS, 0);
    VALUE v_receives_shadows = rb_funcall(v_entity, RU::INTERN_TRECEIVES_SHADOWS, 0);
    VALUE v_visible = rb_funcall(v_entity, RU::INTERN_TVISIBLE, 0);
    VALUE v_name = rb_funcall(v_entity, RU::INTERN_NAME, 0);

    rb_funcall(v_inst, RU::INTERN_SMATERIAL, 1, v_material);
    rb_funcall(v_inst, RU::INTERN_SLAYER, 1, v_layer);
    rb_funcall(v_inst, RU::INTERN_SCASTS_SHADOWS, 1, v_casts_shadows);
    rb_funcall(v_inst, RU::INTERN_SRECEIVES_SHADOWS, 1, v_receives_shadows);
    rb_funcall(v_inst, RU::INTERN_SVISIBLE, 1, v_visible);
    rb_funcall(v_inst, RU::INTERN_SNAME, 1, v_name);

    if (c_exclude_sub(self, v_inst, recurse)) {
        if (rb_funcall(v_inst, RU::INTERN_TVALID, 0) == Qtrue)
            rb_funcall(v_inst, RU::INTERN_EERASE, 0);
        return Qnil;
    }
    else
        return v_inst;
}

VALUE AMS::Group::rbf_split(int argc, VALUE* argv, VALUE self) {
    VALUE v_transformation = Qnil;
    VALUE v_recurse = Qtrue;

    if (argc == 6) {
        v_transformation = argv[4];
        v_recurse = argv[5];
    }
    else if (argc == 5) {
        v_transformation = argv[4];
    }
    else if (argc != 4)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 4..6 arguments.");

    VALUE v_entity = argv[0];
    VALUE v_point = argv[1];
    VALUE v_normal = argv[2];
    VALUE v_context = argv[3];

    if (v_transformation == Qnil)
        v_transformation = rb_funcall(v_entity, RU::INTERN_TRANSFORMATION, 0);

    Geom::Vector3d point, normal, normal_rev;
    RU::value_to_vector(v_point, point);
    RU::value_to_vector(v_normal, normal);
    normal_rev = normal.reverse();
    bool recurse = RTEST(v_recurse);

    // Create instances
    VALUE v_definition = rbf_get_definition(self, v_entity);
    VALUE v_inst1 = rb_funcall(v_context, RU::INTERN_ADD_INSTANCE, 2, v_definition, v_transformation);
    VALUE v_inst2 = rb_funcall(v_context, RU::INTERN_ADD_INSTANCE, 2, v_definition, v_transformation);

    VALUE v_material = rb_funcall(v_entity, RU::INTERN_MATERIAL, 0);
    VALUE v_layer = rb_funcall(v_entity, RU::INTERN_LAYER, 0);
    VALUE v_casts_shadows = rb_funcall(v_entity, RU::INTERN_TCASTS_SHADOWS, 0);
    VALUE v_receives_shadows = rb_funcall(v_entity, RU::INTERN_TRECEIVES_SHADOWS, 0);
    VALUE v_visible = rb_funcall(v_entity, RU::INTERN_TVISIBLE, 0);
    VALUE v_name = rb_funcall(v_entity, RU::INTERN_NAME, 0);

    rb_funcall(v_inst1, RU::INTERN_SMATERIAL, 1, v_material);
    rb_funcall(v_inst1, RU::INTERN_SLAYER, 1, v_layer);
    rb_funcall(v_inst1, RU::INTERN_SCASTS_SHADOWS, 1, v_casts_shadows);
    rb_funcall(v_inst1, RU::INTERN_SRECEIVES_SHADOWS, 1, v_receives_shadows);
    rb_funcall(v_inst1, RU::INTERN_SVISIBLE, 1, v_visible);
    rb_funcall(v_inst1, RU::INTERN_SNAME, 1, v_name);

    rb_funcall(v_inst2, RU::INTERN_SMATERIAL, 1, v_material);
    rb_funcall(v_inst2, RU::INTERN_SLAYER, 1, v_layer);
    rb_funcall(v_inst2, RU::INTERN_SCASTS_SHADOWS, 1, v_casts_shadows);
    rb_funcall(v_inst2, RU::INTERN_SRECEIVES_SHADOWS, 1, v_receives_shadows);
    rb_funcall(v_inst2, RU::INTERN_SVISIBLE, 1, v_visible);
    rb_funcall(v_inst2, RU::INTERN_SNAME, 1, v_name);

    VALUE v_res1 = c_split(self, v_inst1, point, normal, v_context, recurse);
    VALUE v_res2 = c_split(self, v_inst2, point, normal_rev, v_context, recurse);

    return rb_ary_new3(2, v_res1, v_res2);
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Group::init_ruby(VALUE mAMS) {
    VALUE mGroup = rb_define_module_under(mAMS, "Group");

    rb_define_module_function(mGroup, "get_definition", VALUEFUNC(AMS::Group::rbf_get_definition), 1);
    rb_define_module_function(mGroup, "get_entities", VALUEFUNC(AMS::Group::rbf_get_entities), 1);
    rb_define_module_function(mGroup, "get_bounding_box_from_edges", VALUEFUNC(AMS::Group::rbf_get_bounding_box_from_edges), -1);
    rb_define_module_function(mGroup, "get_bounding_box_from_faces", VALUEFUNC(AMS::Group::rbf_get_bounding_box_from_faces), -1);
    rb_define_module_function(mGroup, "get_edges", VALUEFUNC(AMS::Group::rbf_get_edges), -1);
    rb_define_module_function(mGroup, "get_vertices_from_edges", VALUEFUNC(AMS::Group::rbf_get_vertices_from_edges), -1);
    rb_define_module_function(mGroup, "get_faces", VALUEFUNC(AMS::Group::rbf_get_faces), -1);
    rb_define_module_function(mGroup, "get_vertices_from_faces", VALUEFUNC(AMS::Group::rbf_get_vertices_from_faces), -1);
    rb_define_module_function(mGroup, "get_vertices_from_faces2", VALUEFUNC(AMS::Group::rbf_get_vertices_from_faces2), -1);
    rb_define_module_function(mGroup, "get_construction", VALUEFUNC(AMS::Group::rbf_get_construction), -1);
    rb_define_module_function(mGroup, "get_polygons_from_faces", VALUEFUNC(AMS::Group::rbf_get_polygons_from_faces), -1);
    rb_define_module_function(mGroup, "get_triangular_mesh", VALUEFUNC(AMS::Group::rbf_get_triangular_mesh), -1);
    rb_define_module_function(mGroup, "get_triangular_meshes", VALUEFUNC(AMS::Group::rbf_get_triangular_meshes), -1);
    rb_define_module_function(mGroup, "calc_centre_of_mass", VALUEFUNC(AMS::Group::rbf_calc_centre_of_mass), -1);
    rb_define_module_function(mGroup, "copy", VALUEFUNC(AMS::Group::rbf_copy), -1);
    rb_define_module_function(mGroup, "split", VALUEFUNC(AMS::Group::rbf_split), -1);
}
