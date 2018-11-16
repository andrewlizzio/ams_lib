/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_CURSOR_H
#define AMS_CURSOR_H

#include "ams_ext.h"

class AMS::Cursor {
public:
    // Ruby Functions
    static VALUE rbf_show(VALUE self, VALUE v_state);
    static VALUE rbf_is_visible(VALUE self);
    static VALUE rbf_get_pos(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_set_pos(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_clip_rect(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_set_clip_rect(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_clip_to_main_window(VALUE self);
    static VALUE rbf_clip_to_viewport(VALUE self);
    static VALUE rbf_clear_clip(VALUE self);
    static VALUE rbf_is_main_window_target(VALUE self);
    static VALUE rbf_is_viewport_target(VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_CURSOR_H */
