/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_cursor.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Cursor::rbf_show(VALUE self, VALUE v_state) {
    //bool state = RU::value_to_bool(v_state);
    return Qfalse;
}

VALUE AMS::Cursor::rbf_is_visible(VALUE self) {
    return Qtrue;
}

VALUE AMS::Cursor::rbf_get_pos(int argc, VALUE* argv, VALUE self) {
    int mode;
    if (argc == 0)
        mode = 1;
    else if (argc == 1)
        mode = RU::value_to_int(argv[0]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 0..1 arguments.");
    int x;
    int y;
    get_cursor_position(&x, &y);
    if (mode == 2) {
        // Subtract the origin of viewport
    }
    return rb_ary_new3(2, RU::to_value(x), RU::to_value(y));
}

VALUE AMS::Cursor::rbf_set_pos(int argc, VALUE* argv, VALUE self) {
    int mode;
    if (argc == 2)
        mode = 1;
    else if (argc == 3)
        mode = RU::value_to_int(argv[2]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");
    int x = RU::value_to_int(argv[0]);
    int y = RU::value_to_int(argv[1]);
    if (mode == 2) {
        // Add the the origin of viewport
    }
    return RU::to_value(set_cursor_position(x, y));
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Cursor::init_ruby(VALUE mAMS) {
    VALUE mCursor = rb_define_module_under(mAMS, "Cursor");

    rb_define_module_function(mCursor, "show", VALUEFUNC(AMS::Cursor::rbf_show), 1);
    rb_define_module_function(mCursor, "is_visible?", VALUEFUNC(AMS::Cursor::rbf_is_visible), 0);
    rb_define_module_function(mCursor, "get_pos", VALUEFUNC(AMS::Cursor::rbf_get_pos), -1);
    rb_define_module_function(mCursor, "set_pos", VALUEFUNC(AMS::Cursor::rbf_set_pos), -1);
}
