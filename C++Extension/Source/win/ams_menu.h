/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_MENU_H
#define AMS_MENU_H

#include "ams_ext.h"

class AMS::Menu {
public:
    // Ruby Functions
    static VALUE rbf_is_valid(VALUE self, VALUE v_handle);
    static VALUE rbf_end(VALUE self);
    static VALUE rbf_get_item_count(VALUE self, VALUE v_handle);
    static VALUE rbf_get_item_id(VALUE self, VALUE v_handle, VALUE v_index);
    static VALUE rbf_get_sub_menu(VALUE self, VALUE v_handle, VALUE v_index);
    static VALUE rbf_get_item_string_by_pos(VALUE self, VALUE v_handle, VALUE v_index);
    static VALUE rbf_get_item_string_by_id(VALUE self, VALUE v_handle, VALUE v_id);
    static VALUE rbf_set_item_string_by_pos(VALUE self, VALUE v_handle, VALUE v_index, VALUE v_string);
    static VALUE rbf_set_item_string_by_id(VALUE self, VALUE v_handle, VALUE v_id, VALUE v_string);
    static VALUE rbf_get_commands(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_item_checked_by_pos(VALUE self, VALUE v_handle, VALUE v_index);
    static VALUE rbf_is_item_checked_by_id(VALUE self, VALUE v_handle, VALUE v_id);
    static VALUE rbf_is_item_grayed_by_pos(VALUE self, VALUE v_handle, VALUE v_index);
    static VALUE rbf_is_item_grayed_by_id(VALUE self, VALUE v_handle, VALUE v_id);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_MENU_H */
