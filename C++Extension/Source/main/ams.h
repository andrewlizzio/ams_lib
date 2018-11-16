/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_H
#define AMS_H

#include "../utils/ruby_util.h"

#include <vector>
#include <set>
#include <map>
#include <string>

namespace AMS {
    // Classes
    class Geometry;
    class Group;
    class MultiLineText;
    class Translate;

    // Variables
    extern double E_VALUES[];

    // Helper Functions
    VALUE c_inspect_element(VALUE v_item);
    void c_inspect_element_dest(VALUE v_item, VALUE v_dest_str);

    // Ruby Functions
    VALUE rbf_inspect_element(VALUE self, VALUE v_item);
    VALUE rbf_validate_type(int argc, VALUE* argv, VALUE self);
    VALUE rbf_clamp_real(VALUE self, VALUE v_value, VALUE v_min, VALUE v_max);
    VALUE rbf_min_real(VALUE self, VALUE v_a, VALUE v_b);
    VALUE rbf_max_real(VALUE self, VALUE v_a, VALUE v_b);
    VALUE rbf_sign_real(VALUE self, VALUE v_value);
    VALUE rbf_clamp_int(VALUE self, VALUE v_value, VALUE v_min, VALUE v_max);
    VALUE rbf_min_int(VALUE self, VALUE v_a, VALUE v_b);
    VALUE rbf_max_int(VALUE self, VALUE v_a, VALUE v_b);
    VALUE rbf_sign_int(VALUE self, VALUE v_value);
    VALUE rbf_round(int argc, VALUE* argv, VALUE self);
    VALUE rbf_is_boolean(VALUE self, VALUE v_object);
    VALUE rbf_get_entity_by_id(VALUE self, VALUE v_id);
    VALUE rbf_get_top_entity_by_id(VALUE self, VALUE v_id);

    // Main
    void init_ruby(VALUE mAMS);
};

#endif  /* AMS_H */
