/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_KEYBOARD_H
#define AMS_KEYBOARD_H

#include "ams_ext.h"

class AMS::Keyboard {
private:
    // Helper Functions
    static int c_value_to_vk_code(VALUE v_vk);
    static short c_get_key_state(VALUE v_vk);
    static short c_get_key_state2(int vk);

public:
    // Ruby Functions
    static VALUE rbf_get_key_state(VALUE self, VALUE v_vk);
    static VALUE rbf_get_keyboard_state(VALUE self);
    static VALUE rbf_get_key_code(VALUE self, VALUE v_vk_name);
    static VALUE rbf_get_key_name(VALUE self, VALUE v_vk_code);
    static VALUE rbf_get_virtual_key_names(VALUE self);
    static VALUE rbf_get_virtual_key_codes(VALUE self);
    static VALUE rbf_get_virtual_key_codes2(VALUE self);
    static VALUE rbf_is_key_toggled(VALUE self, VALUE v_vk);
    static VALUE rbf_is_key_down(VALUE self, VALUE v_vk);
    static VALUE rbf_are_keys_down(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_key_up(VALUE self, VALUE v_vk);
    static VALUE rbf_are_keys_up(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_menu_down(VALUE self);
    static VALUE rbf_is_menu_up(VALUE self);
    static VALUE rbf_is_control_down(VALUE self);
    static VALUE rbf_is_control_up(VALUE self);
    static VALUE rbf_is_shift_down(VALUE self);
    static VALUE rbf_is_shift_up(VALUE self);
    static VALUE rbf_is_lbutton_down(VALUE self);
    static VALUE rbf_is_lbutton_up(VALUE self);
    static VALUE rbf_is_rbutton_down(VALUE self);
    static VALUE rbf_is_rbutton_up(VALUE self);
    static VALUE rbf_is_mbutton_down(VALUE self);
    static VALUE rbf_is_mbutton_up(VALUE self);
    static VALUE rbf_is_xbutton1_down(VALUE self);
    static VALUE rbf_is_xbutton1_up(VALUE self);
    static VALUE rbf_is_xbutton2_down(VALUE self);
    static VALUE rbf_is_xbutton2_up(VALUE self);
    static VALUE rbf_is_lshift_down(VALUE self);
    static VALUE rbf_is_lshift_up(VALUE self);
    static VALUE rbf_is_rshift_down(VALUE self);
    static VALUE rbf_is_rshift_up(VALUE self);
    static VALUE rbf_is_lcontrol_down(VALUE self);
    static VALUE rbf_is_lcontrol_up(VALUE self);
    static VALUE rbf_is_rcontrol_down(VALUE self);
    static VALUE rbf_is_rcontrol_up(VALUE self);
    static VALUE rbf_is_lmenu_down(VALUE self);
    static VALUE rbf_is_lmenu_up(VALUE self);
    static VALUE rbf_is_rmenu_down(VALUE self);
    static VALUE rbf_is_rmenu_up(VALUE self);
    static VALUE rbf_send_mouse_input(VALUE self, VALUE v_dx, VALUE v_dy, VALUE v_mouse_data, VALUE v_flags, VALUE v_time);
    static VALUE rbf_send_keyboard_input(VALUE self, VALUE v_vk, VALUE v_scan, VALUE v_flags, VALUE v_time);
    static VALUE rbf_send_hardware_input(VALUE self, VALUE v_msg, VALUE v_wParamL, VALUE v_wParamH);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_KEYBOARD_H */
