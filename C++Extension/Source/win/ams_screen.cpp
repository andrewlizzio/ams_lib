/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_screen.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Screen::rbf_resolution(VALUE self) {
    VALUE v_w = RU::to_value(GetSystemMetrics(SM_CXSCREEN));
    VALUE v_h = RU::to_value(GetSystemMetrics(SM_CYSCREEN));
    return rb_ary_new3(2, v_w, v_h);
}

VALUE AMS::Screen::rbf_virtual_resolution(VALUE self) {
    VALUE v_w = RU::to_value(GetSystemMetrics(SM_CXVIRTUALSCREEN));
    VALUE v_h = RU::to_value(GetSystemMetrics(SM_CYVIRTUALSCREEN));
    return rb_ary_new3(2, v_w, v_h);
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Screen::init_ruby(VALUE mAMS) {
    VALUE mScreen = rb_define_module_under(mAMS, "Screen");

    rb_define_module_function(mScreen, "resolution", VALUEFUNC(AMS::Screen::rbf_resolution), 0);
    rb_define_module_function(mScreen, "virtual_resolution", VALUEFUNC(AMS::Screen::rbf_virtual_resolution), 0);
}
