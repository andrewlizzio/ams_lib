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
#include <string.h>
#include <wctype.h>

extern "C" {
    #include "user_input.h"
}

class AMS::Keyboard {
private:
    // Structures
    struct compare_c_str {
        bool operator()(const char* a, const char* b) const {
            return std::strcmp(a, b) < 0;
        }
    };

    struct compare_c_str2 {
        bool operator()(const wchar_t* a, const wchar_t *b) const {
            return std::wcscmp(a, b) < 0;
        }
    };

public:
    // Variables
    static std::map<short, const wchar_t*> virtual_key_names;
    static std::map<const wchar_t*, short, compare_c_str2> virtual_key_codes;
    static std::map<const wchar_t*, short, compare_c_str2> virtual_key_codes2;

    // Helper Functions
    static short c_vk_name_to_code(const wchar_t* vk_name);
    static short c_value_to_vk_code(VALUE v_vk);
    static short c_get_key_state(VALUE v_vk);
    static short c_get_key_state2(short vk);

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

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_KEYBOARD_H */
