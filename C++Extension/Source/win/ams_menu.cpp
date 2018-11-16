/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_menu.h"
#include "ams_sketchup.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Menu::rbf_is_valid(VALUE self, VALUE v_handle) {
    if (IsMenu(reinterpret_cast<HMENU>(rb_num2ull(v_handle))) == TRUE)
        return Qtrue;
    else
        return Qfalse;
}

VALUE AMS::Menu::rbf_end(VALUE self) {
    if (EndMenu() == TRUE)
        return Qtrue;
    else
        return Qfalse;
}

VALUE AMS::Menu::rbf_get_item_count(VALUE self, VALUE v_handle) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    return RU::to_value(GetMenuItemCount(handle));
}

VALUE AMS::Menu::rbf_get_item_id(VALUE self, VALUE v_handle, VALUE v_index) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int index = RU::value_to_int(v_index);
    int id = GetMenuItemID(handle, index);
    return RU::to_value(id);
}

VALUE AMS::Menu::rbf_get_sub_menu(VALUE self, VALUE v_handle, VALUE v_index) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int index = RU::value_to_int(v_index);
    HMENU submenu = GetSubMenu(handle, index);
    return RU::to_value(submenu);
}

VALUE AMS::Menu::rbf_get_item_string_by_pos(VALUE self, VALUE v_handle, VALUE v_index) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int index = RU::value_to_int(v_index);
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE;
    mii.fType = MFT_STRING;
    mii.cch = 0;
    mii.dwTypeData = NULL;
    if (GetMenuItemInfo(handle, index, 1, &mii) == FALSE)
        return Qnil;
    wchar_t* sz_string = new wchar_t[mii.cch+1];
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE;
    mii.fType = MFT_STRING;
    ++mii.cch;
    mii.dwTypeData = sz_string;
    BOOL res = GetMenuItemInfo(handle, index, 1, &mii);
    VALUE v_result;
    if (res == TRUE)
        v_result = RU::win_wc_str_to_value(mii.dwTypeData, mii.cch);
    else
        v_result = Qnil;
    delete[] sz_string;
    return v_result;
}

VALUE AMS::Menu::rbf_get_item_string_by_id(VALUE self, VALUE v_handle, VALUE v_id) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int id = RU::value_to_int(v_id);
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE;
    mii.fType = MFT_STRING;
    mii.cch = 0;
    mii.dwTypeData = NULL;
    if (GetMenuItemInfo(handle, id, 0, &mii) == FALSE)
        return Qnil;
    wchar_t* sz_string = new wchar_t[mii.cch+1];
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE;
    mii.fType = MFT_STRING;
    ++mii.cch;
    mii.dwTypeData = sz_string;
    BOOL res = GetMenuItemInfo(handle, id, 0, &mii);
    VALUE v_result;
    if (res == TRUE)
        v_result = RU::win_wc_str_to_value(mii.dwTypeData, mii.cch);
    else
        v_result = Qnil;
    delete[] sz_string;
    return v_result;
}

VALUE AMS::Menu::rbf_set_item_string_by_pos(VALUE self, VALUE v_handle, VALUE v_index, VALUE v_string) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int index = RU::value_to_int(v_index);
    wchar_t* sz_string = RU::value_to_wc_str(v_string);
    unsigned int sz_string_len = static_cast<unsigned int>(wcslen(sz_string));
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE;
    mii.fType = MFT_STRING;
    mii.cch = sz_string_len;
    mii.dwTypeData = sz_string;
    BOOL res = SetMenuItemInfo(handle, index, 1, &mii);
    delete[] sz_string;
    return res == TRUE ? Qtrue : Qfalse;
}

VALUE AMS::Menu::rbf_set_item_string_by_id(VALUE self, VALUE v_handle, VALUE v_id, VALUE v_string) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int id = RU::value_to_int(v_id);
    wchar_t* sz_string = RU::value_to_wc_str(v_string);
    unsigned int sz_string_len = static_cast<unsigned int>(wcslen(sz_string));
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_TYPE;
    mii.fType = MFT_STRING;
    mii.cch = sz_string_len;
    mii.dwTypeData = sz_string;
    BOOL res = SetMenuItemInfo(handle, id, 0, &mii);
    delete[] sz_string;
    return res == TRUE ? Qtrue : Qfalse;
}

VALUE AMS::Menu::rbf_get_commands(int argc, VALUE* argv, VALUE self) {
    wchar_t* cur_path = nullptr;
    unsigned int cur_path_len = 0;
    if (argc == 2) {
        cur_path = RU::value_to_wc_str(argv[1]);
        cur_path_len = static_cast<unsigned int>(wcslen(cur_path));
    }
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
    HMENU handle = RU::value_to_hmenu(argv[0]);
    std::map<const wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2> commands;
    AMS::Sketchup::c_get_menu_commands(handle, cur_path, cur_path_len, commands);
    if (cur_path != nullptr) delete[] cur_path;
    VALUE v_commands = rb_hash_new();
    for (std::map<const wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2>::iterator it = commands.begin(); it != commands.end(); ++it) {
        VALUE v_string = RU::to_value(it->first);
        rb_hash_aset(v_commands, v_string, RU::to_value(it->second));
        delete[] it->first;
    }
    commands.clear();
    return v_commands;
}

VALUE AMS::Menu::rbf_is_item_checked_by_pos(VALUE self, VALUE v_handle, VALUE v_index) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int index = RU::value_to_int(v_index);
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STATE;
    if (GetMenuItemInfo(handle, index, 1, &mii) == 0)
        return Qfalse;
    return ((mii.fState | MFS_CHECKED) == mii.fState) ? Qtrue : Qfalse;
}

VALUE AMS::Menu::rbf_is_item_checked_by_id(VALUE self, VALUE v_handle, VALUE v_id) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int id = RU::value_to_int(v_id);
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STATE;
    if (GetMenuItemInfo(handle, id, 0, &mii) == 0)
        return Qfalse;
    return ((mii.fState | MFS_CHECKED) == mii.fState) ? Qtrue : Qfalse;
}

VALUE AMS::Menu::rbf_is_item_grayed_by_pos(VALUE self, VALUE v_handle, VALUE v_index) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int index = RU::value_to_int(v_index);
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STATE;
    if (GetMenuItemInfo(handle, index, 1, &mii) == 0)
        return Qfalse;
    return ((mii.fState | MFS_GRAYED) == mii.fState) ? Qtrue : Qfalse;
}

VALUE AMS::Menu::rbf_is_item_grayed_by_id(VALUE self, VALUE v_handle, VALUE v_id) {
    HMENU handle = RU::value_to_hmenu(v_handle);
    int id = RU::value_to_int(v_id);
    MENUITEMINFO mii;
    mii.cbSize = sizeof(mii);
    mii.fMask = MIIM_STATE;
    if (GetMenuItemInfo(handle, id, 0, &mii) == 0)
        return Qfalse;
    return ((mii.fState | MFS_GRAYED) == mii.fState) ? Qtrue : Qfalse;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Menu::init_ruby(VALUE mAMS) {
    VALUE mMenu = rb_define_module_under(mAMS, "Menu");

    rb_define_module_function(mMenu, "is_valid?", VALUEFUNC(AMS::Menu::rbf_is_valid), 1);
    rb_define_module_function(mMenu, "end", VALUEFUNC(AMS::Menu::rbf_end), 0);
    rb_define_module_function(mMenu, "get_item_count", VALUEFUNC(AMS::Menu::rbf_get_item_count), 1);
    rb_define_module_function(mMenu, "get_item_id", VALUEFUNC(AMS::Menu::rbf_get_item_id), 2);
    rb_define_module_function(mMenu, "get_sub_menu", VALUEFUNC(AMS::Menu::rbf_get_sub_menu), 2);
    rb_define_module_function(mMenu, "get_item_string_by_pos", VALUEFUNC(AMS::Menu::rbf_get_item_string_by_pos), 2);
    rb_define_module_function(mMenu, "get_item_string_by_id", VALUEFUNC(AMS::Menu::rbf_get_item_string_by_id), 2);
    rb_define_module_function(mMenu, "set_item_string_by_pos", VALUEFUNC(AMS::Menu::rbf_set_item_string_by_pos), 3);
    rb_define_module_function(mMenu, "set_item_string_by_id", VALUEFUNC(AMS::Menu::rbf_set_item_string_by_id), 3);
    rb_define_module_function(mMenu, "get_commands", VALUEFUNC(AMS::Menu::rbf_get_commands), -1);
    rb_define_module_function(mMenu, "is_item_checked_by_pos?", VALUEFUNC(AMS::Menu::rbf_is_item_checked_by_pos), 2);
    rb_define_module_function(mMenu, "is_item_checked_by_id?", VALUEFUNC(AMS::Menu::rbf_is_item_checked_by_id), 2);
    rb_define_module_function(mMenu, "is_item_grayed_by_pos?", VALUEFUNC(AMS::Menu::rbf_is_item_grayed_by_pos), 2);
    rb_define_module_function(mMenu, "is_item_grayed_by_id?", VALUEFUNC(AMS::Menu::rbf_is_item_grayed_by_id), 2);
}
