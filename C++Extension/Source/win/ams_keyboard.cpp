/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_keyboard.h"
#include "ams_sketchup.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

int AMS::Keyboard::c_value_to_vk_code(VALUE v_vk) {
    int type = TYPE(v_vk);
    if (type == T_FLOAT || type == T_BIGNUM || type == T_FIXNUM)
        return RU::value_to_int(v_vk);
    else {
        wchar_t* vk = RU::value_to_wc_str(v_vk);
        _wcslwr(vk);
        int code = AMS::Sketchup::c_vk_name_to_code(vk);
        delete[] vk;
        return code;
    }
}

short AMS::Keyboard::c_get_key_state(VALUE v_vk) {
    return (GetKeyState(c_value_to_vk_code(v_vk)) >> 15) == 0 ? 0 : 1;
}

short AMS::Keyboard::c_get_key_state2(int vk) {
    return (GetKeyState(vk) >> 15) == 0 ? 0 : 1;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Keyboard::rbf_get_key_state(VALUE self, VALUE v_vk) {
    return RU::to_value(c_get_key_state(v_vk));
}

VALUE AMS::Keyboard::rbf_get_keyboard_state(VALUE self) {
    BYTE keys[256];
    VALUE v_keys = rb_ary_new2(256);
    GetKeyboardState(keys);
    for (int i = 0; i < 256; ++i)
        rb_ary_store(v_keys, i, RU::to_value(keys[i] >> 7));
    return v_keys;
}

VALUE AMS::Keyboard::rbf_get_key_code(VALUE self, VALUE v_vk_name) {
    return RU::to_value(c_value_to_vk_code(v_vk_name));
}

VALUE AMS::Keyboard::rbf_get_key_name(VALUE self, VALUE v_vk_code) {
    int vk_code = RU::value_to_int(v_vk_code);
    std::map<int, const wchar_t*>::iterator it = AMS::Sketchup::su_virtual_key_names.find(vk_code);
    if (it != AMS::Sketchup::su_virtual_key_names.end())
        return RU::to_value(it->second);
    else
        return Qnil;
}

VALUE AMS::Keyboard::rbf_get_virtual_key_names(VALUE self) {
    VALUE v_vk_codes = rb_hash_new();
    for (std::map<int, const wchar_t*>::iterator it = AMS::Sketchup::su_virtual_key_names.begin(); it != AMS::Sketchup::su_virtual_key_names.end(); ++it)
        rb_hash_aset(v_vk_codes, RU::to_value(it->first), RU::to_value(it->second));
    return v_vk_codes;
}

VALUE AMS::Keyboard::rbf_get_virtual_key_codes(VALUE self) {
    VALUE v_vk_codes = rb_hash_new();
    for (std::map<const wchar_t*, int, AMS::Sketchup::CompareCString2>::iterator it = AMS::Sketchup::su_virtual_key_codes.begin(); it != AMS::Sketchup::su_virtual_key_codes.end(); ++it)
        rb_hash_aset(v_vk_codes, RU::to_value(it->first), RU::to_value(it->second));
    return v_vk_codes;
}

VALUE AMS::Keyboard::rbf_get_virtual_key_codes2(VALUE self) {
    VALUE v_vk_codes = rb_hash_new();
    for (std::map<const wchar_t*, int, AMS::Sketchup::CompareCString2>::iterator it = AMS::Sketchup::su_virtual_key_codes2.begin(); it != AMS::Sketchup::su_virtual_key_codes2.end(); ++it)
        rb_hash_aset(v_vk_codes, RU::to_value(it->first), RU::to_value(it->second));
    return v_vk_codes;
}

VALUE AMS::Keyboard::rbf_is_key_toggled(VALUE self, VALUE v_vk) {
    return (GetKeyState(c_value_to_vk_code(v_vk)) & 1) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_key_down(VALUE self, VALUE v_vk) {
    return c_get_key_state(v_vk) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_are_keys_down(int argc, VALUE* argv, VALUE self) {
    for (int i = 0; i < argc; ++i)
        if (c_get_key_state(argv[i]) == 0) return Qfalse;
    return Qtrue;
}

VALUE AMS::Keyboard::rbf_is_key_up(VALUE self, VALUE v_vk) {
    return c_get_key_state(v_vk) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_are_keys_up(int argc, VALUE* argv, VALUE self) {
    for (int i = 0; i < argc; ++i)
        if (c_get_key_state(argv[i]) == 1) return Qfalse;
    return Qtrue;
}

VALUE AMS::Keyboard::rbf_is_menu_down(VALUE self) {
    return c_get_key_state2(0x12) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_menu_up(VALUE self) {
    return c_get_key_state2(0x12) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_control_down(VALUE self) {
    return c_get_key_state2(0x11) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_control_up(VALUE self) {
    return c_get_key_state2(0x11) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_shift_down(VALUE self) {
    return c_get_key_state2(0x10) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_shift_up(VALUE self) {
    return c_get_key_state2(0x10) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lbutton_down(VALUE self) {
    return c_get_key_state2(0x01) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lbutton_up(VALUE self) {
    return c_get_key_state2(0x01) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rbutton_down(VALUE self) {
    return c_get_key_state2(0x02) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rbutton_up(VALUE self) {
    return c_get_key_state2(0x02) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_mbutton_down(VALUE self) {
    return c_get_key_state2(0x04) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_mbutton_up(VALUE self) {
    return c_get_key_state2(0x04) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_xbutton1_down(VALUE self) {
    return c_get_key_state2(0x05) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_xbutton1_up(VALUE self) {
    return c_get_key_state2(0x05) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_xbutton2_down(VALUE self) {
    return c_get_key_state2(0x06) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_xbutton2_up(VALUE self) {
    return c_get_key_state2(0x06) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lshift_down(VALUE self) {
    return c_get_key_state2(0xA0) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lshift_up(VALUE self) {
    return c_get_key_state2(0xA0) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rshift_down(VALUE self) {
    return c_get_key_state2(0xA1) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rshift_up(VALUE self) {
    return c_get_key_state2(0xA1) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lcontrol_down(VALUE self) {
    return c_get_key_state2(0xA2) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lcontrol_up(VALUE self) {
    return c_get_key_state2(0xA2) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rcontrol_down(VALUE self) {
    return c_get_key_state2(0xA3) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rcontrol_up(VALUE self) {
    return c_get_key_state2(0xA3) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lmenu_down(VALUE self) {
    return c_get_key_state2(0xA4) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lmenu_up(VALUE self) {
    return c_get_key_state2(0xA4) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rmenu_down(VALUE self) {
    return c_get_key_state2(0xA5) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rmenu_up(VALUE self) {
    return c_get_key_state2(0xA5) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_send_mouse_input(VALUE self, VALUE v_dx, VALUE v_dy, VALUE v_mouse_data, VALUE v_flags, VALUE v_time) {
    MOUSEINPUT mi;
    mi.dx = RU::value_to_long(v_dx);
    mi.dy = RU::value_to_long(v_dy);
    mi.mouseData = RU::value_to_ulong(v_mouse_data);
    mi.dwFlags = RU::value_to_ulong(v_flags);
    mi.time = RU::value_to_ulong(v_time);
    mi.dwExtraInfo = 0;

    INPUT input;
    input.type = 0;
    input.mi = mi;

    return RU::to_value(SendInput(1, &input, sizeof(INPUT)));
}

VALUE AMS::Keyboard::rbf_send_keyboard_input(VALUE self, VALUE v_vk, VALUE v_scan, VALUE v_flags, VALUE v_time) {
    KEYBDINPUT ki;
    ki.wVk = RU::value_to_ushort(v_vk);
    ki.wScan = RU::value_to_ushort(v_scan);
    ki.dwFlags = RU::value_to_ulong(v_flags);
    ki.time = RU::value_to_ulong(v_time);
    ki.dwExtraInfo = 0;

    INPUT input;
    input.type = 1;
    input.ki = ki;

    return RU::to_value(SendInput(1, &input, sizeof(INPUT)));
}

VALUE AMS::Keyboard::rbf_send_hardware_input(VALUE self, VALUE v_msg, VALUE v_wParamL, VALUE v_wParamH) {
    HARDWAREINPUT hi;
    hi.uMsg = RU::value_to_ulong(v_msg);
    hi.wParamL = RU::value_to_ushort(v_wParamL);
    hi.wParamH = RU::value_to_ushort(v_wParamH);

    INPUT input;
    input.type = 2;
    input.hi = hi;

    return RU::to_value(SendInput(1, &input, sizeof(INPUT)));
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Keyboard::init_ruby(VALUE mAMS) {
    VALUE mKeyboard = rb_define_module_under(mAMS, "Keyboard");

    rb_define_module_function(mKeyboard, "get_key_state", VALUEFUNC(AMS::Keyboard::rbf_get_key_state), 1);
    rb_define_module_function(mKeyboard, "key", VALUEFUNC(AMS::Keyboard::rbf_get_key_state), 1);

    rb_define_module_function(mKeyboard, "get_keyboard_state", VALUEFUNC(AMS::Keyboard::rbf_get_keyboard_state), 0);

    rb_define_module_function(mKeyboard, "get_key_code", VALUEFUNC(AMS::Keyboard::rbf_get_key_code), 1);
    rb_define_module_function(mKeyboard, "get_key_name", VALUEFUNC(AMS::Keyboard::rbf_get_key_name), 1);

    rb_define_module_function(mKeyboard, "get_virtual_key_names", VALUEFUNC(AMS::Keyboard::rbf_get_virtual_key_names), 0);
    rb_define_module_function(mKeyboard, "get_virtual_key_codes", VALUEFUNC(AMS::Keyboard::rbf_get_virtual_key_codes), 0);
    rb_define_module_function(mKeyboard, "get_virtual_key_codes2", VALUEFUNC(AMS::Keyboard::rbf_get_virtual_key_codes2), 0);

    rb_define_module_function(mKeyboard, "key_toggled?", VALUEFUNC(AMS::Keyboard::rbf_is_key_toggled), 1);
    rb_define_module_function(mKeyboard, "key_down?", VALUEFUNC(AMS::Keyboard::rbf_is_key_down), 1);
    rb_define_module_function(mKeyboard, "keys_down?", VALUEFUNC(AMS::Keyboard::rbf_are_keys_down), -1);
    rb_define_module_function(mKeyboard, "key_up?", VALUEFUNC(AMS::Keyboard::rbf_is_key_up), 1);
    rb_define_module_function(mKeyboard, "keys_up?", VALUEFUNC(AMS::Keyboard::rbf_are_keys_up), -1);

    rb_define_module_function(mKeyboard, "menu_down?", VALUEFUNC(AMS::Keyboard::rbf_is_menu_down), 0);
    rb_define_module_function(mKeyboard, "alt_down?", VALUEFUNC(AMS::Keyboard::rbf_is_menu_down), 0);
    rb_define_module_function(mKeyboard, "menu_up?", VALUEFUNC(AMS::Keyboard::rbf_is_menu_up), 0);
    rb_define_module_function(mKeyboard, "alt_up?", VALUEFUNC(AMS::Keyboard::rbf_is_menu_up), 0);

    rb_define_module_function(mKeyboard, "control_down?", VALUEFUNC(AMS::Keyboard::rbf_is_control_down), 0);
    rb_define_module_function(mKeyboard, "ctrl_down?", VALUEFUNC(AMS::Keyboard::rbf_is_control_down), 0);
    rb_define_module_function(mKeyboard, "control_up?", VALUEFUNC(AMS::Keyboard::rbf_is_control_up), 0);
    rb_define_module_function(mKeyboard, "ctrl_up?", VALUEFUNC(AMS::Keyboard::rbf_is_control_up), 0);

    rb_define_module_function(mKeyboard, "shift_down?", VALUEFUNC(AMS::Keyboard::rbf_is_shift_down), 0);
    rb_define_module_function(mKeyboard, "shift_up?", VALUEFUNC(AMS::Keyboard::rbf_is_shift_up), 0);

    rb_define_module_function(mKeyboard, "lbutton_down?", VALUEFUNC(AMS::Keyboard::rbf_is_lbutton_down), 0);
    rb_define_module_function(mKeyboard, "lbutton_up?", VALUEFUNC(AMS::Keyboard::rbf_is_lbutton_up), 0);

    rb_define_module_function(mKeyboard, "rbutton_down?", VALUEFUNC(AMS::Keyboard::rbf_is_rbutton_down), 0);
    rb_define_module_function(mKeyboard, "rbutton_up?", VALUEFUNC(AMS::Keyboard::rbf_is_rbutton_up), 0);

    rb_define_module_function(mKeyboard, "mbutton_down?", VALUEFUNC(AMS::Keyboard::rbf_is_mbutton_down), 0);
    rb_define_module_function(mKeyboard, "mbutton_up?", VALUEFUNC(AMS::Keyboard::rbf_is_mbutton_up), 0);

    rb_define_module_function(mKeyboard, "xbutton1_down?", VALUEFUNC(AMS::Keyboard::rbf_is_xbutton1_down), 0);
    rb_define_module_function(mKeyboard, "xbutton1_up?", VALUEFUNC(AMS::Keyboard::rbf_is_xbutton1_up), 0);

    rb_define_module_function(mKeyboard, "xbutton2_down?", VALUEFUNC(AMS::Keyboard::rbf_is_xbutton2_down), 0);
    rb_define_module_function(mKeyboard, "xbutton2_up?", VALUEFUNC(AMS::Keyboard::rbf_is_xbutton2_up), 0);

    rb_define_module_function(mKeyboard, "lshift_down?", VALUEFUNC(AMS::Keyboard::rbf_is_lshift_down), 0);
    rb_define_module_function(mKeyboard, "lshift_up?", VALUEFUNC(AMS::Keyboard::rbf_is_lshift_up), 0);

    rb_define_module_function(mKeyboard, "rshift_down?", VALUEFUNC(AMS::Keyboard::rbf_is_rshift_down), 0);
    rb_define_module_function(mKeyboard, "rshift_up?", VALUEFUNC(AMS::Keyboard::rbf_is_rshift_up), 0);

    rb_define_module_function(mKeyboard, "lcontrol_down?", VALUEFUNC(AMS::Keyboard::rbf_is_lcontrol_down), 0);
    rb_define_module_function(mKeyboard, "lctrl_down?", VALUEFUNC(AMS::Keyboard::rbf_is_lcontrol_down), 0);
    rb_define_module_function(mKeyboard, "lcontrol_up?", VALUEFUNC(AMS::Keyboard::rbf_is_lcontrol_up), 0);
    rb_define_module_function(mKeyboard, "lctrl_up?", VALUEFUNC(AMS::Keyboard::rbf_is_lcontrol_up), 0);

    rb_define_module_function(mKeyboard, "rcontrol_down?", VALUEFUNC(AMS::Keyboard::rbf_is_rcontrol_down), 0);
    rb_define_module_function(mKeyboard, "rctrl_down?", VALUEFUNC(AMS::Keyboard::rbf_is_rcontrol_down), 0);
    rb_define_module_function(mKeyboard, "rcontrol_up?", VALUEFUNC(AMS::Keyboard::rbf_is_rcontrol_up), 0);
    rb_define_module_function(mKeyboard, "rctrl_up?", VALUEFUNC(AMS::Keyboard::rbf_is_rcontrol_up), 0);

    rb_define_module_function(mKeyboard, "lmenu_down?", VALUEFUNC(AMS::Keyboard::rbf_is_lmenu_down), 0);
    rb_define_module_function(mKeyboard, "lalt_down?", VALUEFUNC(AMS::Keyboard::rbf_is_lmenu_down), 0);
    rb_define_module_function(mKeyboard, "lmenu_up?", VALUEFUNC(AMS::Keyboard::rbf_is_lmenu_up), 0);
    rb_define_module_function(mKeyboard, "lalt_up?", VALUEFUNC(AMS::Keyboard::rbf_is_lmenu_up), 0);

    rb_define_module_function(mKeyboard, "rmenu_down?", VALUEFUNC(AMS::Keyboard::rbf_is_rmenu_down), 0);
    rb_define_module_function(mKeyboard, "ralt_down?", VALUEFUNC(AMS::Keyboard::rbf_is_rmenu_down), 0);
    rb_define_module_function(mKeyboard, "rmenu_up?", VALUEFUNC(AMS::Keyboard::rbf_is_rmenu_up), 0);
    rb_define_module_function(mKeyboard, "ralt_up?", VALUEFUNC(AMS::Keyboard::rbf_is_rmenu_up), 0);

    rb_define_module_function(mKeyboard, "send_mouse_input", VALUEFUNC(AMS::Keyboard::rbf_send_mouse_input), 5);
    rb_define_module_function(mKeyboard, "send_keyboard_input", VALUEFUNC(AMS::Keyboard::rbf_send_keyboard_input), 4);
    rb_define_module_function(mKeyboard, "send_hardware_input", VALUEFUNC(AMS::Keyboard::rbf_send_hardware_input), 3);
}
