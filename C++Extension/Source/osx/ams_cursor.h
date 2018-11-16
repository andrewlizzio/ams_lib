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

extern "C" {
    #include "user_input.h"
}

class AMS::Cursor {
public:
    // Ruby Functions
    static VALUE rbf_show(VALUE self, VALUE v_state);
    static VALUE rbf_is_visible(VALUE self);
    static VALUE rbf_get_pos(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_set_pos(int argc, VALUE* argv, VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_CURSOR_H */
