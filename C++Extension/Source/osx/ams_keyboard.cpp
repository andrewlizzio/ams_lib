/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_keyboard.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Variables
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

std::map<short, const wchar_t*> AMS::Keyboard::virtual_key_names;
std::map<const wchar_t*, short, AMS::Keyboard::compare_c_str2> AMS::Keyboard::virtual_key_codes;
std::map<const wchar_t*, short, AMS::Keyboard::compare_c_str2> AMS::Keyboard::virtual_key_codes2;


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

short AMS::Keyboard::c_vk_name_to_code(const wchar_t* vk_name) {
    std::map<const wchar_t*, short, compare_c_str2>::iterator it;
    it = virtual_key_codes.find(vk_name);
    if (it != virtual_key_codes.end()) return it->second;
    it = virtual_key_codes2.find(vk_name);
    if (it != virtual_key_codes2.end()) return it->second;
    return -1;
}

short AMS::Keyboard::c_value_to_vk_code(VALUE v_vk) {
    int type = TYPE(v_vk);
    if (type == T_FLOAT || type == T_BIGNUM || type == T_FIXNUM)
        return RU::value_to_int(v_vk);
    else {
        wchar_t* vk = RU::value_to_wc_str(v_vk);
        for (int i = 0; i < wcslen(vk); ++i)
            vk[i] = tolower(vk[i]);
        short code = c_vk_name_to_code(vk);
        delete[] vk;
        return code;
    }
}

short AMS::Keyboard::c_get_key_state(VALUE v_vk) {
    short vk = c_value_to_vk_code(v_vk);
    return test_for_key_down(vk);
}

short AMS::Keyboard::c_get_key_state2(short vk) {
    return test_for_key_down(vk);
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
    return Qnil;
}

VALUE AMS::Keyboard::rbf_get_key_code(VALUE self, VALUE v_vk_name) {
    return RU::to_value(c_value_to_vk_code(v_vk_name));
}

VALUE AMS::Keyboard::rbf_get_key_name(VALUE self, VALUE v_vk_code) {
    short vk_code = RU::value_to_short(v_vk_code);
    std::map<short, const wchar_t*>::iterator it = virtual_key_names.find(vk_code);
    if (it != virtual_key_names.end())
        return RU::to_value(it->second);
    else
        return Qnil;
}

VALUE AMS::Keyboard::rbf_get_virtual_key_names(VALUE self) {
    VALUE v_vk_codes = rb_hash_new();
    for (std::map<short, const wchar_t*>::iterator it = virtual_key_names.begin(); it != virtual_key_names.end(); ++it)
        rb_hash_aset(v_vk_codes, RU::to_value(it->first), RU::to_value(it->second));
    return v_vk_codes;
}

VALUE AMS::Keyboard::rbf_get_virtual_key_codes(VALUE self) {
    VALUE v_vk_codes = rb_hash_new();
    for (std::map<const wchar_t*, short, compare_c_str2>::iterator it = virtual_key_codes.begin(); it != virtual_key_codes.end(); ++it)
        rb_hash_aset(v_vk_codes, RU::to_value(it->first), RU::to_value(it->second));
    return v_vk_codes;
}

VALUE AMS::Keyboard::rbf_get_virtual_key_codes2(VALUE self) {
    VALUE v_vk_codes = rb_hash_new();
    for (std::map<const wchar_t*, short, compare_c_str2>::iterator it = virtual_key_codes2.begin(); it != virtual_key_codes2.end(); ++it)
        rb_hash_aset(v_vk_codes, RU::to_value(it->first), RU::to_value(it->second));
    return v_vk_codes;
}

VALUE AMS::Keyboard::rbf_is_key_toggled(VALUE self, VALUE v_vk) {
    return Qnil;
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
    return c_get_key_state2(kVK_Option) == 1 || c_get_key_state2(kVK_RightOption) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_menu_up(VALUE self) {
    return c_get_key_state2(kVK_Option) == 0 && c_get_key_state2(kVK_RightOption) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_control_down(VALUE self) {
    return c_get_key_state2(kVK_Control) == 1 || c_get_key_state2(kVK_RightControl) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_control_up(VALUE self) {
    return c_get_key_state2(kVK_Control) == 0 && c_get_key_state2(kVK_RightControl) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_shift_down(VALUE self) {
    return c_get_key_state2(kVK_Shift) == 1 || c_get_key_state2(kVK_RightShift) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_shift_up(VALUE self) {
    return c_get_key_state2(kVK_Shift) == 0 && c_get_key_state2(kVK_RightShift) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lbutton_down(VALUE self) {
    //int mouse_state = [NSEvent pressedMouseButtons];
    //return RU::to_value(mouse_state);
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_lbutton_up(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_rbutton_down(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_rbutton_up(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_mbutton_down(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_mbutton_up(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_xbutton1_down(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_xbutton1_up(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_xbutton2_down(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_xbutton2_up(VALUE self) {
    return Qnil;
}

VALUE AMS::Keyboard::rbf_is_lshift_down(VALUE self) {
    return c_get_key_state2(kVK_Shift) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lshift_up(VALUE self) {
    return c_get_key_state2(kVK_Shift) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rshift_down(VALUE self) {
    return c_get_key_state2(kVK_RightShift) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rshift_up(VALUE self) {
    return c_get_key_state2(kVK_RightShift) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lcontrol_down(VALUE self) {
    return c_get_key_state2(kVK_Control) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lcontrol_up(VALUE self) {
    return c_get_key_state2(kVK_Control) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rcontrol_down(VALUE self) {
    return c_get_key_state2(kVK_RightControl) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rcontrol_up(VALUE self) {
    return c_get_key_state2(kVK_RightControl) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lmenu_down(VALUE self) {
    return c_get_key_state2(kVK_Option) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_lmenu_up(VALUE self) {
    return c_get_key_state2(kVK_Option) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rmenu_down(VALUE self) {
    return c_get_key_state2(kVK_RightOption) == 1 ? Qtrue : Qfalse;
}

VALUE AMS::Keyboard::rbf_is_rmenu_up(VALUE self) {
    return c_get_key_state2(kVK_RightOption) == 0 ? Qtrue : Qfalse;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Keyboard::init_ruby(VALUE mAMS) {
    // Map virtual key codes

    // Virtual key names
    AMS::Keyboard::virtual_key_names[kVK_ANSI_A] = L"a";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_S] = L"s";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_D] = L"d";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_F] = L"f";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_H] = L"h";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_G] = L"g";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Z] = L"z";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_X] = L"x";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_C] = L"c";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_V] = L"v";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_B] = L"b";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Q] = L"q";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_W] = L"w";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_E] = L"e";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_R] = L"r";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Y] = L"y";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_T] = L"t";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_1] = L"1";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_2] = L"2";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_3] = L"3";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_4] = L"4";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_6] = L"6";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_5] = L"5";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Equal] = L"equals";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_9] = L"9";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_7] = L"7";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Minus] = L"minus";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_8] = L"8";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_0] = L"0";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_RightBracket] = L"rightbracket";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_O] = L"o";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_U] = L"u";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_LeftBracket] = L"leftbracket";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_I] = L"i";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_P] = L"p";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_L] = L"l";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_J] = L"j";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Quote] = L"quote";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_K] = L"k";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Semicolon] = L"semicolon";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Backslash] = L"backslash";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Comma] = L"comma";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Slash] = L"slash";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_N] = L"n";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_M] = L"m";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Period] = L"period";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Grave] = L"grave";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadDecimal] = L"numpaddecimal";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadMultiply] = L"numpadmultiply";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadPlus] = L"numpadplus";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadClear] = L"numpadclear";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadDivide] = L"numpaddivide";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadEnter] = L"numpadenter";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadMinus] = L"numpadminus";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_KeypadEquals] = L"numpadequals";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad0] = L"numpad0";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad1] = L"numpad1";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad2] = L"numpad2";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad3] = L"numpad3";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad4] = L"numpad4";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad5] = L"numpad5";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad6] = L"numpad6";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad7] = L"numpad7";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad8] = L"numpad8";
    AMS::Keyboard::virtual_key_names[kVK_ANSI_Keypad9] = L"numpad9";

    // Key names for keys independent of keyboard layout
    AMS::Keyboard::virtual_key_names[kVK_Return] = L"return";
    AMS::Keyboard::virtual_key_names[kVK_Tab] = L"tab";
    AMS::Keyboard::virtual_key_names[kVK_Space] = L"space";
    AMS::Keyboard::virtual_key_names[kVK_Delete] = L"backspace";
    AMS::Keyboard::virtual_key_names[kVK_Escape] = L"escape";
    AMS::Keyboard::virtual_key_names[kVK_Command] = L"command";
    AMS::Keyboard::virtual_key_names[kVK_Shift] = L"lshift";
    AMS::Keyboard::virtual_key_names[kVK_CapsLock] = L"capslock";
    AMS::Keyboard::virtual_key_names[kVK_Option] = L"lmenu";
    AMS::Keyboard::virtual_key_names[kVK_Control] = L"lcontrol";
    AMS::Keyboard::virtual_key_names[kVK_RightShift] = L"rshift";
    AMS::Keyboard::virtual_key_names[kVK_RightOption] = L"rmenu";
    AMS::Keyboard::virtual_key_names[kVK_RightControl] = L"rcontrol";
    AMS::Keyboard::virtual_key_names[kVK_Function] = L"function";
    AMS::Keyboard::virtual_key_names[kVK_F17] = L"f17";
    AMS::Keyboard::virtual_key_names[kVK_VolumeUp] = L"volumeup";
    AMS::Keyboard::virtual_key_names[kVK_VolumeDown] = L"volumedown";
    AMS::Keyboard::virtual_key_names[kVK_Mute] = L"mute";
    AMS::Keyboard::virtual_key_names[kVK_F18] = L"f18";
    AMS::Keyboard::virtual_key_names[kVK_F19] = L"f19";
    AMS::Keyboard::virtual_key_names[kVK_F20] = L"f20";
    AMS::Keyboard::virtual_key_names[kVK_F5] = L"f5";
    AMS::Keyboard::virtual_key_names[kVK_F6] = L"f6";
    AMS::Keyboard::virtual_key_names[kVK_F7] = L"f7";
    AMS::Keyboard::virtual_key_names[kVK_F3] = L"f3";
    AMS::Keyboard::virtual_key_names[kVK_F8] = L"f8";
    AMS::Keyboard::virtual_key_names[kVK_F9] = L"f9";
    AMS::Keyboard::virtual_key_names[kVK_F11] = L"f11";
    AMS::Keyboard::virtual_key_names[kVK_F13] = L"f13";
    AMS::Keyboard::virtual_key_names[kVK_F16] = L"f16";
    AMS::Keyboard::virtual_key_names[kVK_F14] = L"f14";
    AMS::Keyboard::virtual_key_names[kVK_F10] = L"f10";
    AMS::Keyboard::virtual_key_names[kVK_F12] = L"f12";
    AMS::Keyboard::virtual_key_names[kVK_F15] = L"f15";
    AMS::Keyboard::virtual_key_names[kVK_Help] = L"help";
    AMS::Keyboard::virtual_key_names[kVK_Home] = L"home";
    AMS::Keyboard::virtual_key_names[kVK_PageUp] = L"pageup";
    AMS::Keyboard::virtual_key_names[kVK_ForwardDelete] = L"delete";
    AMS::Keyboard::virtual_key_names[kVK_F4] = L"f4";
    AMS::Keyboard::virtual_key_names[kVK_End] = L"end";
    AMS::Keyboard::virtual_key_names[kVK_F2] = L"f2";
    AMS::Keyboard::virtual_key_names[kVK_PageDown] = L"pagedown";
    AMS::Keyboard::virtual_key_names[kVK_F1] = L"f1";
    AMS::Keyboard::virtual_key_names[kVK_LeftArrow] = L"left";
    AMS::Keyboard::virtual_key_names[kVK_RightArrow] = L"right";
    AMS::Keyboard::virtual_key_names[kVK_DownArrow] = L"down";
    AMS::Keyboard::virtual_key_names[kVK_UpArrow] = L"up";

    // Key names for ISO keyboards only
    AMS::Keyboard::virtual_key_names[kVK_ISO_Section] = L"section";

    // Key names for JIS keyboards only
    AMS::Keyboard::virtual_key_names[kVK_JIS_Yen] = L"yen";
    AMS::Keyboard::virtual_key_names[kVK_JIS_Underscore] = L"underscore";
    AMS::Keyboard::virtual_key_names[kVK_JIS_KeypadComma] = L"keypadcomma";
    AMS::Keyboard::virtual_key_names[kVK_JIS_Eisu] = L"eisu";
    AMS::Keyboard::virtual_key_names[kVK_JIS_Kana] = L"kana";


    // Virtual key codes
    AMS::Keyboard::virtual_key_codes[L"a"] = kVK_ANSI_A;
    AMS::Keyboard::virtual_key_codes[L"s"] = kVK_ANSI_S;
    AMS::Keyboard::virtual_key_codes[L"d"] = kVK_ANSI_D;
    AMS::Keyboard::virtual_key_codes[L"f"] = kVK_ANSI_F;
    AMS::Keyboard::virtual_key_codes[L"h"] = kVK_ANSI_H;
    AMS::Keyboard::virtual_key_codes[L"g"] = kVK_ANSI_G;
    AMS::Keyboard::virtual_key_codes[L"z"] = kVK_ANSI_Z;
    AMS::Keyboard::virtual_key_codes[L"x"] = kVK_ANSI_X;
    AMS::Keyboard::virtual_key_codes[L"c"] = kVK_ANSI_C;
    AMS::Keyboard::virtual_key_codes[L"v"] = kVK_ANSI_V;
    AMS::Keyboard::virtual_key_codes[L"b"] = kVK_ANSI_B;
    AMS::Keyboard::virtual_key_codes[L"q"] = kVK_ANSI_Q;
    AMS::Keyboard::virtual_key_codes[L"w"] = kVK_ANSI_W;
    AMS::Keyboard::virtual_key_codes[L"e"] = kVK_ANSI_E;
    AMS::Keyboard::virtual_key_codes[L"r"] = kVK_ANSI_R;
    AMS::Keyboard::virtual_key_codes[L"y"] = kVK_ANSI_Y;
    AMS::Keyboard::virtual_key_codes[L"t"] = kVK_ANSI_T;
    AMS::Keyboard::virtual_key_codes[L"1"] = kVK_ANSI_1;
    AMS::Keyboard::virtual_key_codes[L"2"] = kVK_ANSI_2;
    AMS::Keyboard::virtual_key_codes[L"3"] = kVK_ANSI_3;
    AMS::Keyboard::virtual_key_codes[L"4"] = kVK_ANSI_4;
    AMS::Keyboard::virtual_key_codes[L"6"] = kVK_ANSI_6;
    AMS::Keyboard::virtual_key_codes[L"5"] = kVK_ANSI_5;
    AMS::Keyboard::virtual_key_codes[L"equals"] = kVK_ANSI_Equal;
    AMS::Keyboard::virtual_key_codes[L"9"] = kVK_ANSI_9;
    AMS::Keyboard::virtual_key_codes[L"7"] = kVK_ANSI_7;
    AMS::Keyboard::virtual_key_codes[L"minus"] = kVK_ANSI_Minus;
    AMS::Keyboard::virtual_key_codes[L"8"] = kVK_ANSI_8;
    AMS::Keyboard::virtual_key_codes[L"0"] = kVK_ANSI_0;
    AMS::Keyboard::virtual_key_codes[L"rightbracket"] = kVK_ANSI_RightBracket;
    AMS::Keyboard::virtual_key_codes[L"o"] = kVK_ANSI_O;
    AMS::Keyboard::virtual_key_codes[L"u"] = kVK_ANSI_U;
    AMS::Keyboard::virtual_key_codes[L"leftbracket"] = kVK_ANSI_LeftBracket;
    AMS::Keyboard::virtual_key_codes[L"i"] = kVK_ANSI_I;
    AMS::Keyboard::virtual_key_codes[L"p"] = kVK_ANSI_P;
    AMS::Keyboard::virtual_key_codes[L"l"] = kVK_ANSI_L;
    AMS::Keyboard::virtual_key_codes[L"j"] = kVK_ANSI_J;
    AMS::Keyboard::virtual_key_codes[L"quote"] = kVK_ANSI_Quote;
    AMS::Keyboard::virtual_key_codes[L"k"] = kVK_ANSI_K;
    AMS::Keyboard::virtual_key_codes[L"semicolon"] = kVK_ANSI_Semicolon;
    AMS::Keyboard::virtual_key_codes[L"backslash"] = kVK_ANSI_Backslash;
    AMS::Keyboard::virtual_key_codes[L"comma"] = kVK_ANSI_Comma;
    AMS::Keyboard::virtual_key_codes[L"slash"] = kVK_ANSI_Slash;
    AMS::Keyboard::virtual_key_codes[L"n"] = kVK_ANSI_N;
    AMS::Keyboard::virtual_key_codes[L"m"] = kVK_ANSI_M;
    AMS::Keyboard::virtual_key_codes[L"period"] = kVK_ANSI_Period;
    AMS::Keyboard::virtual_key_codes[L"grave"] = kVK_ANSI_Grave;
    AMS::Keyboard::virtual_key_codes[L"numpaddecimal"] = kVK_ANSI_KeypadDecimal;
    AMS::Keyboard::virtual_key_codes[L"numpadmultiply"] = kVK_ANSI_KeypadMultiply;
    AMS::Keyboard::virtual_key_codes[L"numpadplus"] = kVK_ANSI_KeypadPlus;
    AMS::Keyboard::virtual_key_codes[L"numpadclear"] = kVK_ANSI_KeypadClear;
    AMS::Keyboard::virtual_key_codes[L"numpaddivide"] = kVK_ANSI_KeypadDivide;
    AMS::Keyboard::virtual_key_codes[L"numpadenter"] = kVK_ANSI_KeypadEnter;
    AMS::Keyboard::virtual_key_codes[L"numpadminus"] = kVK_ANSI_KeypadMinus;
    AMS::Keyboard::virtual_key_codes[L"numpadequals"] = kVK_ANSI_KeypadEquals;
    AMS::Keyboard::virtual_key_codes[L"numpad0"] = kVK_ANSI_Keypad0;
    AMS::Keyboard::virtual_key_codes[L"numpad1"] = kVK_ANSI_Keypad1;
    AMS::Keyboard::virtual_key_codes[L"numpad2"] = kVK_ANSI_Keypad2;
    AMS::Keyboard::virtual_key_codes[L"numpad3"] = kVK_ANSI_Keypad3;
    AMS::Keyboard::virtual_key_codes[L"numpad4"] = kVK_ANSI_Keypad4;
    AMS::Keyboard::virtual_key_codes[L"numpad5"] = kVK_ANSI_Keypad5;
    AMS::Keyboard::virtual_key_codes[L"numpad6"] = kVK_ANSI_Keypad6;
    AMS::Keyboard::virtual_key_codes[L"numpad7"] = kVK_ANSI_Keypad7;
    AMS::Keyboard::virtual_key_codes[L"numpad8"] = kVK_ANSI_Keypad8;
    AMS::Keyboard::virtual_key_codes[L"numpad9"] = kVK_ANSI_Keypad9;

    // Key codes for keys independent of keyboard layout
    AMS::Keyboard::virtual_key_codes[L"return"] = kVK_Return;
    AMS::Keyboard::virtual_key_codes[L"tab"] = kVK_Tab;
    AMS::Keyboard::virtual_key_codes[L"space"] = kVK_Space;
    AMS::Keyboard::virtual_key_codes[L"backspace"] = kVK_Delete;
    AMS::Keyboard::virtual_key_codes[L"escape"] = kVK_Escape;
    AMS::Keyboard::virtual_key_codes[L"command"] = kVK_Command;
    AMS::Keyboard::virtual_key_codes[L"lshift"] = kVK_Shift;
    AMS::Keyboard::virtual_key_codes[L"capslock"] = kVK_CapsLock;
    AMS::Keyboard::virtual_key_codes[L"lmenu"] = kVK_Option;
    AMS::Keyboard::virtual_key_codes[L"lcontrol"] = kVK_Control;
    AMS::Keyboard::virtual_key_codes[L"rshift"] = kVK_RightShift;
    AMS::Keyboard::virtual_key_codes[L"rmenu"] = kVK_RightOption;
    AMS::Keyboard::virtual_key_codes[L"rcontrol"] = kVK_RightControl;
    AMS::Keyboard::virtual_key_codes[L"function"] = kVK_Function;
    AMS::Keyboard::virtual_key_codes[L"f17"] = kVK_F17;
    AMS::Keyboard::virtual_key_codes[L"volumeup"] = kVK_VolumeUp;
    AMS::Keyboard::virtual_key_codes[L"volumedown"] = kVK_VolumeDown;
    AMS::Keyboard::virtual_key_codes[L"mute"] = kVK_Mute;
    AMS::Keyboard::virtual_key_codes[L"f18"] = kVK_F18;
    AMS::Keyboard::virtual_key_codes[L"f19"] = kVK_F19;
    AMS::Keyboard::virtual_key_codes[L"f20"] = kVK_F20;
    AMS::Keyboard::virtual_key_codes[L"f5"] = kVK_F5;
    AMS::Keyboard::virtual_key_codes[L"f6"] = kVK_F6;
    AMS::Keyboard::virtual_key_codes[L"f7"] = kVK_F7;
    AMS::Keyboard::virtual_key_codes[L"f3"] = kVK_F3;
    AMS::Keyboard::virtual_key_codes[L"f8"] = kVK_F8;
    AMS::Keyboard::virtual_key_codes[L"f9"] = kVK_F9;
    AMS::Keyboard::virtual_key_codes[L"f11"] = kVK_F11;
    AMS::Keyboard::virtual_key_codes[L"f13"] = kVK_F13;
    AMS::Keyboard::virtual_key_codes[L"f16"] = kVK_F16;
    AMS::Keyboard::virtual_key_codes[L"f14"] = kVK_F14;
    AMS::Keyboard::virtual_key_codes[L"f10"] = kVK_F10;
    AMS::Keyboard::virtual_key_codes[L"f12"] = kVK_F12;
    AMS::Keyboard::virtual_key_codes[L"f15"] = kVK_F15;
    AMS::Keyboard::virtual_key_codes[L"help"] = kVK_Help;
    AMS::Keyboard::virtual_key_codes[L"home"] = kVK_Home;
    AMS::Keyboard::virtual_key_codes[L"pageup"] = kVK_PageUp;
    AMS::Keyboard::virtual_key_codes[L"delete"] = kVK_ForwardDelete;
    AMS::Keyboard::virtual_key_codes[L"f4"] = kVK_F4;
    AMS::Keyboard::virtual_key_codes[L"end"] = kVK_End;
    AMS::Keyboard::virtual_key_codes[L"f2"] = kVK_F2;
    AMS::Keyboard::virtual_key_codes[L"pagedown"] = kVK_PageDown;
    AMS::Keyboard::virtual_key_codes[L"f1"] = kVK_F1;
    AMS::Keyboard::virtual_key_codes[L"left"] = kVK_LeftArrow;
    AMS::Keyboard::virtual_key_codes[L"right"] = kVK_RightArrow;
    AMS::Keyboard::virtual_key_codes[L"down"] = kVK_DownArrow;
    AMS::Keyboard::virtual_key_codes[L"up"] = kVK_UpArrow;

    // Key codes for ISO keyboards only
    AMS::Keyboard::virtual_key_codes[L"section"] = kVK_ISO_Section;

    // Key codes for JIS keyboards only
    AMS::Keyboard::virtual_key_codes[L"yen"] = kVK_JIS_Yen;
    AMS::Keyboard::virtual_key_codes[L"underscore"] = kVK_JIS_Underscore;
    AMS::Keyboard::virtual_key_codes[L"keypadcomma"] = kVK_JIS_KeypadComma;
    AMS::Keyboard::virtual_key_codes[L"eisu"] = kVK_JIS_Eisu;
    AMS::Keyboard::virtual_key_codes[L"kana"] = kVK_JIS_Kana;

    // Virtual key code by name extended
    AMS::Keyboard::virtual_key_codes2[L"back"]              = kVK_Delete;
    AMS::Keyboard::virtual_key_codes2[L"enter"]             = kVK_Return;
    AMS::Keyboard::virtual_key_codes2[L"alt"]               = kVK_Option;
    //AMS::Keyboard::virtual_key_codes2[L"break"]           = 0x13;
    AMS::Keyboard::virtual_key_codes2[L"capital"]           = kVK_CapsLock;
    AMS::Keyboard::virtual_key_codes2[L"esc"]               = kVK_Escape;
    AMS::Keyboard::virtual_key_codes2[L"spacebar"]          = kVK_Space;
    AMS::Keyboard::virtual_key_codes2[L" "]                 = kVK_Space;
    AMS::Keyboard::virtual_key_codes2[L"prior"]             = kVK_PageUp;
    AMS::Keyboard::virtual_key_codes2[L"next"]              = kVK_PageDown;
    AMS::Keyboard::virtual_key_codes2[L"←"]                 = kVK_LeftArrow;
    AMS::Keyboard::virtual_key_codes2[L"↑"]                 = kVK_UpArrow;
    AMS::Keyboard::virtual_key_codes2[L"→"]                 = kVK_RightArrow;
    AMS::Keyboard::virtual_key_codes2[L"↓"]                 = kVK_DownArrow;
    AMS::Keyboard::virtual_key_codes2[L"leftarrow"]         = kVK_LeftArrow;
    AMS::Keyboard::virtual_key_codes2[L"rightarrow"]        = kVK_RightArrow;
    AMS::Keyboard::virtual_key_codes2[L"downarrow"]         = kVK_DownArrow;
    AMS::Keyboard::virtual_key_codes2[L"uparrow"]           = kVK_UpArrow;
    //AMS::Keyboard::virtual_key_codes2[L"printscreen"]     = 0x2c;
    //AMS::Keyboard::virtual_key_codes2[L"prtscn"]          = 0x2c;
    //AMS::Keyboard::virtual_key_codes2[L"sysrq"]           = 0x2c;
    AMS::Keyboard::virtual_key_codes2[L")"]                 = kVK_ANSI_0;
    AMS::Keyboard::virtual_key_codes2[L"closeparenthese"]   = kVK_ANSI_0;
    AMS::Keyboard::virtual_key_codes2[L"rightparenthese"]   = kVK_ANSI_0;
    AMS::Keyboard::virtual_key_codes2[L"!"]                 = kVK_ANSI_1;
    AMS::Keyboard::virtual_key_codes2[L"exclamation"]       = kVK_ANSI_1;
    AMS::Keyboard::virtual_key_codes2[L"exclamationmark"]   = kVK_ANSI_1;
    AMS::Keyboard::virtual_key_codes2[L"@"]                 = kVK_ANSI_2;
    AMS::Keyboard::virtual_key_codes2[L"at"]                = kVK_ANSI_2;
    AMS::Keyboard::virtual_key_codes2[L"atsign"]            = kVK_ANSI_2;
    AMS::Keyboard::virtual_key_codes2[L"#"]                 = kVK_ANSI_3;
    AMS::Keyboard::virtual_key_codes2[L"number"]            = kVK_ANSI_3;
    AMS::Keyboard::virtual_key_codes2[L"numbersign"]        = kVK_ANSI_3;
    AMS::Keyboard::virtual_key_codes2[L"hash"]              = kVK_ANSI_3;
    AMS::Keyboard::virtual_key_codes2[L"$"]                 = kVK_ANSI_4;
    AMS::Keyboard::virtual_key_codes2[L"dollar"]            = kVK_ANSI_4;
    AMS::Keyboard::virtual_key_codes2[L"dollarsign"]        = kVK_ANSI_4;
    AMS::Keyboard::virtual_key_codes2[L"%"]                 = kVK_ANSI_5;
    AMS::Keyboard::virtual_key_codes2[L"percent"]           = kVK_ANSI_5;
    AMS::Keyboard::virtual_key_codes2[L"^"]                 = kVK_ANSI_6;
    AMS::Keyboard::virtual_key_codes2[L"caret"]             = kVK_ANSI_6;
    AMS::Keyboard::virtual_key_codes2[L"&"]                 = kVK_ANSI_7;
    AMS::Keyboard::virtual_key_codes2[L"ampersand"]         = kVK_ANSI_7;
    AMS::Keyboard::virtual_key_codes2[L"*"]                 = kVK_ANSI_8;
    AMS::Keyboard::virtual_key_codes2[L"asterisk"]          = kVK_ANSI_8;
    AMS::Keyboard::virtual_key_codes2[L"("]                 = kVK_ANSI_9;
    AMS::Keyboard::virtual_key_codes2[L"openparenthese"]    = kVK_ANSI_9;
    AMS::Keyboard::virtual_key_codes2[L"leftparenthese"]    = kVK_ANSI_9;
    AMS::Keyboard::virtual_key_codes2[L"keypad0"]           = kVK_ANSI_Keypad0;
    AMS::Keyboard::virtual_key_codes2[L"keypad1"]           = kVK_ANSI_Keypad1;
    AMS::Keyboard::virtual_key_codes2[L"keypad2"]           = kVK_ANSI_Keypad2;
    AMS::Keyboard::virtual_key_codes2[L"keypad3"]           = kVK_ANSI_Keypad3;
    AMS::Keyboard::virtual_key_codes2[L"keypad4"]           = kVK_ANSI_Keypad4;
    AMS::Keyboard::virtual_key_codes2[L"keypad5"]           = kVK_ANSI_Keypad5;
    AMS::Keyboard::virtual_key_codes2[L"keypad6"]           = kVK_ANSI_Keypad6;
    AMS::Keyboard::virtual_key_codes2[L"keypad7"]           = kVK_ANSI_Keypad7;
    AMS::Keyboard::virtual_key_codes2[L"keypad8"]           = kVK_ANSI_Keypad8;
    AMS::Keyboard::virtual_key_codes2[L"keypad9"]           = kVK_ANSI_Keypad9;
    AMS::Keyboard::virtual_key_codes2[L"keypadmultiply"]    = kVK_ANSI_KeypadMultiply;
    AMS::Keyboard::virtual_key_codes2[L"keypadplus"]        = kVK_ANSI_KeypadPlus;
    AMS::Keyboard::virtual_key_codes2[L"keypadminus"]       = kVK_ANSI_KeypadMinus;
    AMS::Keyboard::virtual_key_codes2[L"keypaddecimal"]     = kVK_ANSI_KeypadDecimal;
    AMS::Keyboard::virtual_key_codes2[L"keypaddivide"]      = kVK_ANSI_KeypadDivide;
    AMS::Keyboard::virtual_key_codes2[L"keypad*"]           = kVK_ANSI_KeypadMultiply;
    AMS::Keyboard::virtual_key_codes2[L"keypad+"]           = kVK_ANSI_KeypadPlus;
    AMS::Keyboard::virtual_key_codes2[L"keypad-"]           = kVK_ANSI_KeypadMinus;
    AMS::Keyboard::virtual_key_codes2[L"keypad."]           = kVK_ANSI_KeypadDecimal;
    AMS::Keyboard::virtual_key_codes2[L"keypad/"]           = kVK_ANSI_KeypadDivide;
    AMS::Keyboard::virtual_key_codes2[L"numpad*"]           = kVK_ANSI_KeypadMultiply;
    AMS::Keyboard::virtual_key_codes2[L"numpad+"]           = kVK_ANSI_KeypadPlus;
    AMS::Keyboard::virtual_key_codes2[L"numpad-"]           = kVK_ANSI_KeypadMinus;
    AMS::Keyboard::virtual_key_codes2[L"numpad."]           = kVK_ANSI_KeypadDecimal;
    AMS::Keyboard::virtual_key_codes2[L"numpad/"]           = kVK_ANSI_KeypadDivide;
    //AMS::Keyboard::virtual_key_codes2[L"scrolllock"]      = 0x91;
    //AMS::Keyboard::virtual_key_codes2[L"scrlk"]           = 0x91;
    AMS::Keyboard::virtual_key_codes2[L"lctrl"]             = kVK_Control;
    AMS::Keyboard::virtual_key_codes2[L"rctrl"]             = kVK_RightControl;
    AMS::Keyboard::virtual_key_codes2[L"lalt"]              = kVK_Option;
    AMS::Keyboard::virtual_key_codes2[L"ralt"]              = kVK_RightOption;
    AMS::Keyboard::virtual_key_codes2[L"loption"]           = kVK_Option;
    AMS::Keyboard::virtual_key_codes2[L"roption"]           = kVK_RightOption;
    //AMS::Keyboard::virtual_key_codes2[L"medianext"]       = 0xb0;
    //AMS::Keyboard::virtual_key_codes2[L"mediaprev"]       = 0xb1;
    //AMS::Keyboard::virtual_key_codes2[L"mediatoggleplay"] = 0xb3;
    //AMS::Keyboard::virtual_key_codes2[L"mail"]            = 0xb4;
    //AMS::Keyboard::virtual_key_codes2[L"mediaselect"]     = 0xb5;
    //AMS::Keyboard::virtual_key_codes2[L"app1"]            = 0xb6;
    //AMS::Keyboard::virtual_key_codes2[L"app2"]            = 0xb7;
    AMS::Keyboard::virtual_key_codes2[L"colon"]             = kVK_ANSI_Semicolon;
    AMS::Keyboard::virtual_key_codes2[L":"]                 = kVK_ANSI_Semicolon;
    AMS::Keyboard::virtual_key_codes2[L";"]                 = kVK_ANSI_Semicolon;
    AMS::Keyboard::virtual_key_codes2[L"plus"]              = kVK_ANSI_Equal;
    AMS::Keyboard::virtual_key_codes2[L"+"]                 = kVK_ANSI_Equal;
    AMS::Keyboard::virtual_key_codes2[L"="]                 = kVK_ANSI_Equal;
    AMS::Keyboard::virtual_key_codes2[L","]                 = kVK_ANSI_Comma;
    AMS::Keyboard::virtual_key_codes2[L"<"]                 = kVK_ANSI_Comma;
    AMS::Keyboard::virtual_key_codes2[L"dash"]              = kVK_ANSI_Minus;
    AMS::Keyboard::virtual_key_codes2[L"underscore"]        = kVK_ANSI_Minus;
    AMS::Keyboard::virtual_key_codes2[L"_"]                 = kVK_ANSI_Minus;
    AMS::Keyboard::virtual_key_codes2[L"-"]                 = kVK_ANSI_Minus;
    AMS::Keyboard::virtual_key_codes2[L"."]                 = kVK_ANSI_Period;
    AMS::Keyboard::virtual_key_codes2[L">"]                 = kVK_ANSI_Period;
    AMS::Keyboard::virtual_key_codes2[L"question"]          = kVK_ANSI_Slash;
    AMS::Keyboard::virtual_key_codes2[L"questionmark"]      = kVK_ANSI_Slash;
    AMS::Keyboard::virtual_key_codes2[L"?"]                 = kVK_ANSI_Slash;
    AMS::Keyboard::virtual_key_codes2[L"/"]                 = kVK_ANSI_Slash;
    AMS::Keyboard::virtual_key_codes2[L"tilde"]             = kVK_ANSI_Grave;
    AMS::Keyboard::virtual_key_codes2[L"~"]                 = kVK_ANSI_Grave;
    AMS::Keyboard::virtual_key_codes2[L"`"]                 = kVK_ANSI_Grave;
    AMS::Keyboard::virtual_key_codes2[L"openbrace"]         = kVK_ANSI_LeftBracket;
    AMS::Keyboard::virtual_key_codes2[L"leftbrace"]         = kVK_ANSI_LeftBracket;
    AMS::Keyboard::virtual_key_codes2[L"openbracket"]       = kVK_ANSI_LeftBracket;
    AMS::Keyboard::virtual_key_codes2[L"{"]                 = kVK_ANSI_LeftBracket;
    AMS::Keyboard::virtual_key_codes2[L"["]                 = kVK_ANSI_LeftBracket;
    AMS::Keyboard::virtual_key_codes2[L"verticalbar"]       = kVK_ANSI_Backslash;
    AMS::Keyboard::virtual_key_codes2[L"|"]                 = kVK_ANSI_Backslash;
    AMS::Keyboard::virtual_key_codes2[L"\\"]                = kVK_ANSI_Backslash;
    AMS::Keyboard::virtual_key_codes2[L"closebrace"]        = kVK_ANSI_RightBracket;
    AMS::Keyboard::virtual_key_codes2[L"rightbrace"]        = kVK_ANSI_RightBracket;
    AMS::Keyboard::virtual_key_codes2[L"closebracket"]      = kVK_ANSI_RightBracket;
    AMS::Keyboard::virtual_key_codes2[L"}"]                 = kVK_ANSI_RightBracket;
    AMS::Keyboard::virtual_key_codes2[L"]"]                 = kVK_ANSI_RightBracket;
    AMS::Keyboard::virtual_key_codes2[L"quotation"]         = kVK_ANSI_Quote;
    AMS::Keyboard::virtual_key_codes2[L"quotationmark"]     = kVK_ANSI_Quote;
    AMS::Keyboard::virtual_key_codes2[L"apostrophe"]        = kVK_ANSI_Quote;
    AMS::Keyboard::virtual_key_codes2[L"'"]                 = kVK_ANSI_Quote;
    AMS::Keyboard::virtual_key_codes2[L"\""]                = kVK_ANSI_Quote;


    // Create Ruby Functions
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
}
