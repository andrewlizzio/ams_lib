/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_cursor.h"
#include "ams_sketchup.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Cursor::rbf_show(VALUE self, VALUE v_state) {
    CURSORINFO info;
    info.cbSize = sizeof(info);
    GetCursorInfo(&info);
    bool state = RU::value_to_bool(v_state);
    if ((info.flags == 1) == state) return Qfalse;
    if (state)
        while(ShowCursor(1) < 0);
    else
        while(ShowCursor(0) > -1);
    return Qtrue;
}

VALUE AMS::Cursor::rbf_is_visible(VALUE self) {
    CURSORINFO info;
    info.cbSize = sizeof(info);
    GetCursorInfo(&info);
    return info.flags == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Cursor::rbf_get_pos(int argc, VALUE* argv, VALUE self) {
    int mode;
    if (argc == 0)
        mode = 1;
    else if (argc == 1)
        mode = RU::value_to_int(argv[0]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 0..1 arguments.");
    POINT pt;
    GetCursorPos(&pt);
    if (mode == 2) {
        RECT rect;
        AMS::Sketchup::c_get_viewport_rect(rect);
        pt.x -= rect.left;
        pt.y -= rect.top;
    }
    return rb_ary_new3(2, RU::to_value(pt.x), RU::to_value(pt.y));
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
        RECT rect;
        AMS::Sketchup::c_get_viewport_rect(rect);
        x += rect.left;
        y += rect.top;
    }
    return SetCursorPos(x, y) ? Qtrue : Qfalse;
}

VALUE AMS::Cursor::rbf_get_clip_rect(int argc, VALUE* argv, VALUE self) {
    int mode;
    if (argc == 0)
        mode = 1;
    else if (argc == 1)
        mode = RU::value_to_int(argv[0]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 0..1 arguments.");
    RECT rect;
    GetClipCursor(&rect);
    if (mode == 2) {
        RECT vrect;
        AMS::Sketchup::c_get_viewport_rect(vrect);
        rect.left -= vrect.left;
        rect.top -= vrect.top;
        rect.right -= vrect.right;
        rect.bottom -= vrect.bottom;
    }
    return rb_ary_new3(
        4,
        RU::to_value(rect.left),
        RU::to_value(rect.top),
        RU::to_value(rect.right),
        RU::to_value(rect.bottom));
}

VALUE AMS::Cursor::rbf_set_clip_rect(int argc, VALUE* argv, VALUE self) {
    int mode;
    if (argc == 4)
        mode = 1;
    else if (argc == 5)
        mode = RU::value_to_int(argv[4]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 4..5 arguments.");
    RECT rect;
    rect.left = RU::value_to_int(argv[0]);
    rect.top = RU::value_to_int(argv[1]);
    rect.right = RU::value_to_int(argv[2]);
    rect.bottom = RU::value_to_int(argv[3]);
    if (mode == 2) {
        RECT vrect;
        AMS::Sketchup::c_get_viewport_rect(vrect);
        rect.left += vrect.left;
        rect.top += vrect.top;
        rect.right += vrect.right;
        rect.bottom += vrect.bottom;
    }
    return ClipCursor(&rect) ? Qtrue : Qfalse;
}

VALUE AMS::Cursor::rbf_clip_to_main_window(VALUE self) {
    RECT rect;
    GetWindowRect(AMS::Sketchup::su_main_window, &rect);
    return ClipCursor(&rect) ? Qtrue : Qfalse;
}

VALUE AMS::Cursor::rbf_clip_to_viewport(VALUE self) {
    RECT rect;
    AMS::Sketchup::c_get_viewport_rect(rect);
    return ClipCursor(&rect) ? Qtrue : Qfalse;
}

VALUE AMS::Cursor::rbf_clear_clip(VALUE self) {
    return ClipCursor(NULL) ? Qtrue : Qfalse;
}

VALUE AMS::Cursor::rbf_is_main_window_target(VALUE self) {
    POINT pt;
    GetCursorPos(&pt);
    return WindowFromPoint(pt) == AMS::Sketchup::su_main_window ? Qtrue : Qfalse;
}

VALUE AMS::Cursor::rbf_is_viewport_target(VALUE self) {
    RECT rect;
    AMS::Sketchup::c_get_viewport_rect(rect);
    POINT pt;
    GetCursorPos(&pt);
    return pt.x >= rect.left && pt.x <= rect.right && pt.y >= rect.top && pt.y <= rect.bottom ? Qtrue : Qfalse;
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
    rb_define_module_function(mCursor, "get_clip_rect", VALUEFUNC(AMS::Cursor::rbf_get_clip_rect), -1);
    rb_define_module_function(mCursor, "set_clip_rect", VALUEFUNC(AMS::Cursor::rbf_set_clip_rect), -1);
    rb_define_module_function(mCursor, "clip_to_main_window", VALUEFUNC(AMS::Cursor::rbf_clip_to_main_window), 0);
    rb_define_module_function(mCursor, "clip_to_viewport", VALUEFUNC(AMS::Cursor::rbf_clip_to_viewport), 0);
    rb_define_module_function(mCursor, "clear_clip", VALUEFUNC(AMS::Cursor::rbf_clear_clip), 0);
    rb_define_module_function(mCursor, "is_main_window_target?", VALUEFUNC(AMS::Cursor::rbf_is_main_window_target), 0);
    rb_define_module_function(mCursor, "is_viewport_target?", VALUEFUNC(AMS::Cursor::rbf_is_viewport_target), 0);
}
