/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_window.h"
#include "ams_sketchup.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Callback Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

BOOL CALLBACK AMS::Window::GetWindowsProc(HWND handle, LPARAM lParam) {
    WindowsInfo* info = reinterpret_cast<WindowsInfo*>(lParam);
    DWORD pid;
    DWORD tid = GetWindowThreadProcessId(handle, &pid);
    bool store = false;
    if (info->mode == 0)
        store = true;
    else if (info->mode == 1 && pid == info->id)
        store = true;
    else if (info->mode == 2 && tid == info->id)
        store = true;
    if (store)
        info->handles.push_back(handle);
    return TRUE;
}

BOOL CALLBACK AMS::Window::GetChildWindowsProc(HWND handle, LPARAM lParam) {
    ChildWindowsInfo* info = reinterpret_cast<ChildWindowsInfo*>(lParam);
    if (info->include_sub_childs || GetAncestor(handle, GA_PARENT) == info->parent_handle)
        info->handles.push_back(handle);
    return TRUE;
}

BOOL CALLBACK AMS::Window::FindWindowProc(HWND handle, LPARAM lParam) {
    AMS::Sketchup::SearchInfo* info = reinterpret_cast<AMS::Sketchup::SearchInfo*>(lParam);
    wchar_t* text;
    if (info->mode == 0) {
        int len = GetWindowTextLength(handle) + 1;
        text = new wchar_t[len];
        GetWindowText(handle, text, len);
    }
    else {
        int len = 256;
        text = new wchar_t[len];
        GetClassName(handle, text, len);
    }
    bool found;
    if (info->full_match) {
        if (info->case_sensitive)
            found = StrCmp(info->text, text) == 0;
        else
            found = StrCmpI(info->text, text) == 0;
    }
    else {
        if (info->case_sensitive)
            found = StrStr(info->text, text) != NULL;
        else
            found = StrStrI(info->text, text) != NULL;
    }
    delete[] text;
    if (found) {
        info->found_handle = handle;
        return FALSE;
    }
    else
        return TRUE;
}

BOOL CALLBACK AMS::Window::FindChildWindowProc(HWND handle, LPARAM lParam) {
    AMS::Sketchup::ChildSearchInfo* info = reinterpret_cast<AMS::Sketchup::ChildSearchInfo*>(lParam);
    if (info->include_sub_childs == false && GetAncestor(handle, GA_PARENT) != info->parent_handle) return TRUE;
    wchar_t* text;
    if (info->mode == 0) {
        int len = GetWindowTextLength(handle) + 1;
        text = new wchar_t[len];
        GetWindowText(handle, text, len);
    }
    else {
        int len = 256;
        text = new wchar_t[len];
        GetClassName(handle, text, len);
    }
    bool found;
    if (info->full_match) {
        if (info->case_sensitive)
            found = StrCmp(info->text, text) == 0;
        else
            found = StrCmpI(info->text, text) == 0;
    }
    else {
        if (info->case_sensitive)
            found = StrStr(info->text, text) != NULL;
        else
            found = StrStrI(info->text, text) != NULL;
    }
    delete[] text;
    if (found) {
        info->found_handle = handle;
        return FALSE;
    }
    else
        return TRUE;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Window::rbf_is_valid(VALUE self, VALUE v_handle) {
    if (IsWindow(reinterpret_cast<HWND>(rb_num2ull(v_handle))) == TRUE)
        return Qtrue;
    else
        return Qfalse;
}

VALUE AMS::Window::rbf_is_active(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(GetActiveWindow() == handle);
}

VALUE AMS::Window::rbf_is_visible(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(IsWindowVisible(handle) == TRUE);
}

VALUE AMS::Window::rbf_is_unicode(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(IsWindowUnicode(handle) == TRUE);
}

VALUE AMS::Window::rbf_is_maximized(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(IsZoomed(handle) == TRUE);
}

VALUE AMS::Window::rbf_is_minimized(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(IsIconic(handle) == TRUE);
}

VALUE AMS::Window::rbf_is_restored(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(IsIconic(handle) == FALSE && IsZoomed(handle) == FALSE);
}

VALUE AMS::Window::rbf_is_child(VALUE self, VALUE v_handle, VALUE v_parent_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    HWND parent_handle = RU::value_to_hwnd(v_parent_handle);
    return RU::to_value(IsChild(parent_handle, handle) == TRUE);
}

VALUE AMS::Window::rbf_show(VALUE self, VALUE v_handle, VALUE v_state) {
    HWND handle = RU::value_to_hwnd(v_handle);
    int state = RU::value_to_int(v_state);
    return RU::to_value(ShowWindow(handle, state));
}

VALUE AMS::Window::rbf_get_active(VALUE self) {
    return RU::to_value(GetActiveWindow());
}

VALUE AMS::Window::rbf_set_active(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(SetActiveWindow(handle));
}

VALUE AMS::Window::rbf_get_keyboard_focus(VALUE self) {
    return RU::to_value(GetFocus());
}

VALUE AMS::Window::rbf_set_keyboard_focus(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(SetFocus(handle));
}

VALUE AMS::Window::rbf_get_parent(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(GetParent(handle));
}

VALUE AMS::Window::rbf_set_parent(VALUE self, VALUE v_handle, VALUE v_parent_handle) {
    HWND parent_handle = RU::value_to_hwnd(v_parent_handle);
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(SetParent(handle, parent_handle));
}

VALUE AMS::Window::rbf_get_ancestor(VALUE self, VALUE v_handle, VALUE v_flag) {
    HWND handle = RU::value_to_hwnd(v_handle);
    UINT flag = RU::value_to_uint(v_flag);
    return RU::to_value(GetAncestor(handle, flag));
}

VALUE AMS::Window::rbf_bring_to_top(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(BringWindowToTop(handle) == TRUE);
}

VALUE AMS::Window::rbf_get_class_name(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    wchar_t cname[256];
    GetClassName(handle, cname, 256);
    return RU::to_value(cname);
}

VALUE AMS::Window::rbf_get_related(VALUE self, VALUE v_handle, VALUE v_command) {
    HWND handle = RU::value_to_hwnd(v_handle);
    UINT command = RU::value_to_uint(v_command);
    return RU::to_value(GetWindow(handle, command));
}

VALUE AMS::Window::rbf_get_thread_id(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(GetWindowThreadProcessId(handle, NULL));
}

VALUE AMS::Window::rbf_get_process_id(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    return RU::to_value(pid);
}

VALUE AMS::Window::rbf_get_long(VALUE self, VALUE v_handle, VALUE v_index) {
    HWND handle = RU::value_to_hwnd(v_handle);
    int index = RU::value_to_int(v_index);
    return RU::to_value(GetWindowLongPtr(handle, index));
}

VALUE AMS::Window::rbf_set_long(VALUE self, VALUE v_handle, VALUE v_index, VALUE v_long) {
    HWND handle = RU::value_to_hwnd(v_handle);
    int index = RU::value_to_int(v_index);
    LONG_PTR new_long = static_cast<LONG_PTR>(RU::value_to_ll(v_long));
    return RU::to_value(SetWindowLongPtr(handle, index, new_long));
}

VALUE AMS::Window::rbf_get_class_long(VALUE self, VALUE v_handle, VALUE v_index) {
    HWND handle = RU::value_to_hwnd(v_handle);
    int index = RU::value_to_int(v_index);
    return RU::to_value(GetClassLongPtr(handle, index));
}

VALUE AMS::Window::rbf_set_class_long(VALUE self, VALUE v_handle, VALUE v_index, VALUE v_long) {
    HWND handle = RU::value_to_hwnd(v_handle);
    int index = RU::value_to_int(v_index);
    LONG_PTR new_long = static_cast<LONG_PTR>(RU::value_to_ll(v_long));
    return RU::to_value(SetClassLongPtr(handle, index, new_long));
}

VALUE AMS::Window::rbf_get_menu(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(GetMenu(handle));
}

VALUE AMS::Window::rbf_set_menu(VALUE self, VALUE v_handle, VALUE v_menu_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    HMENU menu_handle = RU::value_to_hmenu(v_menu_handle);
    return RU::to_value(SetMenu(handle, menu_handle) == TRUE);
}

VALUE AMS::Window::rbf_get_caption(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    int len = GetWindowTextLength(handle);
    wchar_t* text = new wchar_t[len + 1];
    GetWindowText(handle, text, len + 1);
    VALUE v_text = RU::to_value(text);
    delete[] text;
    return v_text;
}

VALUE AMS::Window::rbf_set_caption(VALUE self, VALUE v_handle, VALUE v_caption) {
    HWND handle = RU::value_to_hwnd(v_handle);
    wchar_t* text = RU::value_to_wc_str(v_caption);
    BOOL res = SetWindowText(handle, text);
    delete[] text;
    return res == TRUE ? Qtrue : Qfalse;
}

VALUE AMS::Window::rbf_get_rect(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    RECT rect;
    GetWindowRect(handle, &rect);
    return rb_ary_new3(
        4,
        RU::to_value(rect.left),
        RU::to_value(rect.top),
        RU::to_value(rect.right),
        RU::to_value(rect.bottom)
    );
}

VALUE AMS::Window::rbf_set_rect(int argc, VALUE* argv, VALUE self) {
    bool b_activate = true;
    if (argc == 6)
        b_activate = RU::value_to_bool(argv[4]);
    else if (argc != 5)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 5..6 arguments.");
    HWND handle = RU::value_to_hwnd(argv[0]);
    int x1 = RU::value_to_int(argv[1]);
    int y1 = RU::value_to_int(argv[2]);
    int x2 = RU::value_to_int(argv[3]);
    int y2 = RU::value_to_int(argv[4]);
    return RU::to_value(SetWindowPos(handle, NULL, x1, y1, x2 - x1, y2 - y1, 0x0260 | (b_activate ? 0 : 0x0014)) == TRUE);
}

VALUE AMS::Window::rbf_get_client_rect(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    RECT rect;
    GetClientRect(handle, &rect);
    return rb_ary_new3(
        4,
        RU::to_value(rect.left),
        RU::to_value(rect.top),
        RU::to_value(rect.right),
        RU::to_value(rect.bottom)
    );
}

VALUE AMS::Window::rbf_adjust_rect(VALUE self, VALUE v_rect, VALUE v_style, VALUE v_style_ex, VALUE v_bmenu) {
    if (TYPE(v_rect) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'rect' parameter!");
    RECT rect;
    rect.left = RU::value_to_int(rb_ary_entry(v_rect, 0));
    rect.top = RU::value_to_int(rb_ary_entry(v_rect, 1));
    rect.right = RU::value_to_int(rb_ary_entry(v_rect, 2));
    rect.bottom = RU::value_to_int(rb_ary_entry(v_rect, 3));
    DWORD style = RU::value_to_ulong(v_style);
    DWORD style_ex = RU::value_to_ulong(v_style_ex);
    BOOL bmenu = RU::value_to_bool(v_bmenu) ? 1 : 0;
    AdjustWindowRectEx(&rect, style, bmenu, style_ex);
    return rb_ary_new3(
        4,
        RU::to_value(rect.left),
        RU::to_value(rect.top),
        RU::to_value(rect.right),
        RU::to_value(rect.bottom)
    );
}

VALUE AMS::Window::rbf_get_placement(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(handle, &wp);
    return rb_ary_new3(
        6,
        RU::to_value(wp.length),
        RU::to_value(wp.flags),
        RU::to_value(wp.showCmd),
        rb_ary_new3(2, RU::to_value(wp.ptMinPosition.x), RU::to_value(wp.ptMinPosition.y)),
        rb_ary_new3(2, RU::to_value(wp.ptMaxPosition.x), RU::to_value(wp.ptMaxPosition.y)),
        rb_ary_new3(
            4,
            RU::to_value(wp.rcNormalPosition.left),
            RU::to_value(wp.rcNormalPosition.top),
            RU::to_value(wp.rcNormalPosition.right),
            RU::to_value(wp.rcNormalPosition.bottom)
        )
    );
}

VALUE AMS::Window::rbf_set_placement(VALUE self, VALUE v_handle, VALUE v_flags, VALUE v_show_cmd, VALUE v_min_pt, VALUE v_max_pt, VALUE v_rect) {
    if (TYPE(v_min_pt) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'min_pt' parameter!");
    if (TYPE(v_max_pt) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'max_pt' parameter!");
    if (TYPE(v_rect) != T_ARRAY)
        rb_raise(rb_eTypeError, "Expected an array for the 'rect' parameter!");

    HWND handle = RU::value_to_hwnd(v_handle);
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    wp.flags = RU::value_to_uint(v_flags);
    wp.showCmd = RU::value_to_uint(v_show_cmd);

    wp.ptMinPosition.x = RU::value_to_int(rb_ary_entry(v_min_pt, 0));
    wp.ptMinPosition.x = RU::value_to_int(rb_ary_entry(v_min_pt, 1));

    wp.ptMaxPosition.x = RU::value_to_int(rb_ary_entry(v_max_pt, 0));
    wp.ptMaxPosition.x = RU::value_to_int(rb_ary_entry(v_max_pt, 1));

    wp.rcNormalPosition.left = RU::value_to_int(rb_ary_entry(v_rect, 0));
    wp.rcNormalPosition.top = RU::value_to_int(rb_ary_entry(v_rect, 1));
    wp.rcNormalPosition.right = RU::value_to_int(rb_ary_entry(v_rect, 2));
    wp.rcNormalPosition.bottom = RU::value_to_int(rb_ary_entry(v_rect, 3));

    return RU::to_value(SetWindowPlacement(handle, &wp) == TRUE);
}

VALUE AMS::Window::rbf_lock_update(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(LockWindowUpdate(handle) == TRUE);
}

VALUE AMS::Window::rbf_set_pos(VALUE self, VALUE v_handle, VALUE v_handle_insert_after, VALUE v_x, VALUE v_y, VALUE v_cx, VALUE v_cy, VALUE v_flags) {
    HWND handle = RU::value_to_hwnd(v_handle);
    HWND handle_insert_after = (HWND)RU::value_to_ll(v_handle_insert_after); //RU::value_to_hwnd(v_handle_insert_after);
    int x = RU::value_to_int(v_x);
    int y = RU::value_to_int(v_y);
    int cx = RU::value_to_int(v_cx);
    int cy = RU::value_to_int(v_cy);
    unsigned int flags = RU::value_to_uint(v_flags);
    return RU::to_value(SetWindowPos(handle, handle_insert_after, x, y, cx, cy, flags) == TRUE);
}

VALUE AMS::Window::rbf_move(int argc, VALUE* argv, VALUE self) {
    bool repaint = true;
    if (argc == 6)
        repaint = RU::value_to_bool(argv[5]);
    else if (argc != 5)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 5..6 arguments.");
    HWND handle = RU::value_to_hwnd(argv[0]);
    int x = RU::value_to_int(argv[1]);
    int y = RU::value_to_int(argv[2]);
    int w = RU::value_to_int(argv[3]);
    int h = RU::value_to_int(argv[4]);
    return RU::to_value(MoveWindow(handle, x, y, w, h, repaint) == TRUE);
}

VALUE AMS::Window::rbf_refresh(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    BOOL original_state = IsWindowEnabled(handle);
    EnableWindow(handle, FALSE);
    LRESULT res = SendMessage(handle, WM_SIZE, 0, 0);
    EnableWindow(handle, original_state);
    RedrawWindow(handle, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
    return res == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Window::rbf_close(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return SendMessage(handle, WM_SYSCOMMAND, SC_CLOSE, 0) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Window::rbf_get_origin(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    RECT rect;
    GetWindowRect(handle, &rect);
    return rb_ary_new3(
        2,
        RU::to_value(rect.left),
        RU::to_value(rect.top)
    );
}

VALUE AMS::Window::rbf_set_origin(int argc, VALUE* argv, VALUE self) {
    bool b_activate = true;
    if (argc == 4)
        b_activate = RU::value_to_bool(argv[3]);
    else if (argc != 3)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 3..4 arguments.");
    HWND handle = RU::value_to_hwnd(argv[0]);
    int x = RU::value_to_int(argv[1]);
    int y = RU::value_to_int(argv[2]);
    return RU::to_value(SetWindowPos(handle, NULL, x, y, 0, 0, 0x0261 | (b_activate ? 0 : 0x0014)) == TRUE);
}

VALUE AMS::Window::rbf_get_size(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    RECT rect;
    GetWindowRect(handle, &rect);
    return rb_ary_new3(
        2,
        RU::to_value(rect.right - rect.left),
        RU::to_value(rect.bottom - rect.top)
    );
}

VALUE AMS::Window::rbf_set_size(int argc, VALUE* argv, VALUE self) {
    bool b_activate = true;
    if (argc == 4)
        b_activate = RU::value_to_bool(argv[3]);
    else if (argc != 3)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 3..4 arguments.");
    HWND handle = RU::value_to_hwnd(argv[0]);
    int w = RU::value_to_int(argv[1]);
    int h = RU::value_to_int(argv[2]);
    return RU::to_value(SetWindowPos(handle, NULL, 0, 0, w, h, 0x0262 | (b_activate ? 0 : 0x0014)) == TRUE);
}

VALUE AMS::Window::rbf_is_resizeable(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    LONG_PTR style = GetWindowLongPtr(handle, GWL_STYLE);
    return RU::to_value((style | 0x00040000) == style);
}

VALUE AMS::Window::rbf_set_resizeable(int argc, VALUE* argv, VALUE self) {
    bool b_activate = true;
    if (argc == 3)
        b_activate = RU::value_to_bool(argv[2]);
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");
    HWND handle = RU::value_to_hwnd(argv[0]);
    bool b_state = RU::value_to_bool(argv[1]);
    LONG_PTR style = GetWindowLongPtr(handle, GWL_STYLE);
    bool cur_state = (style | 0x00040000) == style;
    if (b_state == cur_state) return false;
    LONG_PTR new_style;
    if (b_state)
        new_style = style | 0x00040000;
    else
        new_style = style & ~0x00040000;
    LockWindowUpdate(handle);
    SetWindowLongPtr(handle, GWL_STYLE, new_style);
    LockWindowUpdate(NULL);
    return RU::to_value(SetWindowPos(handle, NULL, 0, 0, 0, 0, 0x0267 | (b_activate ? 0 : 0x0014)) == TRUE);
}

VALUE AMS::Window::rbf_is_input_enabled(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(IsWindowEnabled(handle) == TRUE);
}

VALUE AMS::Window::rbf_enable_input(VALUE self, VALUE v_handle, VALUE v_state) {
    HWND handle = RU::value_to_hwnd(v_handle);
    BOOL state = RU::value_to_bool(v_state) ? 1 : 0;
    return RU::to_value(EnableWindow(handle, state) == TRUE);
}

VALUE AMS::Window::rbf_send_message(VALUE self, VALUE v_handle, VALUE v_message, VALUE v_wParam, VALUE v_lParam) {
    HWND handle = RU::value_to_hwnd(v_handle);
    UINT message = RU::value_to_uint(v_message);
    WPARAM wParam = static_cast<WPARAM>(RU::value_to_ull(v_wParam));
    LPARAM lParam = static_cast<LPARAM>(RU::value_to_ull(v_lParam));
    return RU::to_value(SendMessage(handle, message, wParam, lParam));
}

VALUE AMS::Window::rbf_post_message(VALUE self, VALUE v_handle, VALUE v_message, VALUE v_wParam, VALUE v_lParam) {
    HWND handle = RU::value_to_hwnd(v_handle);
    UINT message = RU::value_to_uint(v_message);
    WPARAM wParam = static_cast<WPARAM>(RU::value_to_ull(v_wParam));
    LPARAM lParam = static_cast<LPARAM>(RU::value_to_ull(v_lParam));
    return RU::to_value(PostMessage(handle, message, wParam, lParam) == TRUE);
}

VALUE AMS::Window::rbf_peek_message(VALUE self, VALUE v_handle, VALUE v_msg_filter_min, VALUE v_msg_filter_max, VALUE v_remove_flag) {
    HWND handle = RU::value_to_hwnd(v_handle);
    UINT msg_filter_min = RU::value_to_uint(v_msg_filter_min);
    UINT msg_filter_max = RU::value_to_uint(v_msg_filter_max);
    UINT remove_flag = RU::value_to_uint(v_remove_flag);
    MSG msg;
    if (PeekMessage(&msg, handle, msg_filter_min, msg_filter_max, remove_flag) == 0) return Qnil;
    return rb_ary_new3(
        6,
        RU::to_value(msg.hwnd),
        RU::to_value(msg.message),
        RU::to_value(msg.wParam),
        RU::to_value(msg.lParam),
        RU::to_value(msg.time),
        rb_ary_new3(2, RU::to_value(msg.pt.x), RU::to_value(msg.pt.y)));
}

VALUE AMS::Window::rbf_peek_message2(VALUE self, VALUE v_handle, VALUE v_msg_filter_min, VALUE v_msg_filter_max, VALUE v_remove_flag) {
    HWND handle = RU::value_to_hwnd(v_handle);
    UINT msg_filter_min = RU::value_to_uint(v_msg_filter_min);
    UINT msg_filter_max = RU::value_to_uint(v_msg_filter_max);
    UINT remove_flag = RU::value_to_uint(v_remove_flag);
    MSG msg;
    return PeekMessage(&msg, handle, msg_filter_min, msg_filter_max, remove_flag) != 0 ? Qtrue : Qfalse;
}

VALUE AMS::Window::rbf_set_icon(VALUE self, VALUE v_handle, VALUE v_path) {
    HWND handle = RU::value_to_hwnd(v_handle);
    wchar_t* path = RU::value_to_wc_str(v_path);
    long len = (long)wcslen(path);
    for (long i = 0; i < len; ++i)
        if (path[i] == '/') path[i] = '\\';
    HICON icon = (HICON)LoadImage(NULL, path, IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
    delete[] path;
    if (icon == NULL) return Qfalse;
    SendMessage(handle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
    SendMessage(handle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
    DestroyIcon(icon);
    return Qtrue;
}

VALUE AMS::Window::rbf_draw_menu_bar(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    return RU::to_value(DrawMenuBar(handle) == TRUE);
}

VALUE AMS::Window::rbf_get_layered_attributes(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    COLORREF color;
    BYTE opacity;
    DWORD flags;
    GetLayeredWindowAttributes(handle, &color, &opacity, &flags);
    return rb_ary_new3(
        3,
        rb_ary_new3(
            3,
            RU::to_value(GetRValue(color)),
            RU::to_value(GetGValue(color)),
            RU::to_value(GetBValue(color))
        ),
        RU::to_value(opacity),
        RU::to_value(flags)
    );
}

VALUE AMS::Window::rbf_set_layered_attributes(VALUE self, VALUE v_handle, VALUE v_color, VALUE v_opacity, VALUE v_flags) {
    HWND handle = RU::value_to_hwnd(v_handle);
    COLORREF color;
    if (TYPE(v_color) == T_ARRAY)
        color = RGB(
            RU::value_to_uchar(rb_ary_entry(v_color, 0)),
            RU::value_to_uchar(rb_ary_entry(v_color, 1)),
            RU::value_to_uchar(rb_ary_entry(v_color, 2))
        );
    else
        color = RU::value_to_ulong(v_color);
    BYTE opacity = RU::value_to_uchar(v_opacity);
    DWORD flags = RU::value_to_ulong(v_flags);
    return RU::to_value(SetLayeredWindowAttributes(handle, color, opacity, flags) == TRUE);
}

VALUE AMS::Window::rbf_get_windows(int argc, VALUE* argv, VALUE self) {
    bool include_hidden = true;
    if (argc == 1)
        include_hidden = RU::value_to_bool(argv[0]);
    else if (argc != 0)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 0..1 arguments.");
    WindowsInfo info;
    info.mode = 0;
    info.include_hidden = include_hidden;
    EnumWindows(GetWindowsProc, reinterpret_cast<LPARAM>(&info));
    VALUE v_handles = rb_ary_new2((long)info.handles.size());
    int index = 0;
    for (std::vector<HWND>::iterator it = info.handles.begin(); it != info.handles.end(); ++it) {
        rb_ary_store(v_handles, index, RU::to_value(*it));
        ++index;
    }
    return v_handles;
}

VALUE AMS::Window::rbf_get_process_windows(int argc, VALUE* argv, VALUE self) {
    bool include_hidden = true;
    if (argc == 2)
        include_hidden = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
    WindowsInfo info;
    info.mode = 1;
    info.id = RU::value_to_ulong(argv[0]);
    info.include_hidden = include_hidden;
    EnumWindows(GetWindowsProc, reinterpret_cast<LPARAM>(&info));
    VALUE v_handles = rb_ary_new2((long)info.handles.size());
    int index = 0;
    for (std::vector<HWND>::iterator it = info.handles.begin(); it != info.handles.end(); ++it) {
        rb_ary_store(v_handles, index, RU::to_value(*it));
        ++index;
    }
    return v_handles;
}

VALUE AMS::Window::rbf_get_thread_windows(int argc, VALUE* argv, VALUE self) {
    bool include_hidden = true;
    if (argc == 2)
        include_hidden = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
    WindowsInfo info;
    info.mode = 2;
    info.id = RU::value_to_ulong(argv[0]);
    info.include_hidden = include_hidden;
    EnumWindows(GetWindowsProc, reinterpret_cast<LPARAM>(&info));
    VALUE v_handles = rb_ary_new2((long)info.handles.size());
    int index = 0;
    for (std::vector<HWND>::iterator it = info.handles.begin(); it != info.handles.end(); ++it) {
        rb_ary_store(v_handles, index, RU::to_value(*it));
        ++index;
    }
    return v_handles;
}

VALUE AMS::Window::rbf_get_child_windows(int argc, VALUE* argv, VALUE self) {
    bool include_sub_childs = false;
    bool include_hidden = true;
    if (argc == 3) {
        include_sub_childs = RU::value_to_bool(argv[1]);
        include_hidden = RU::value_to_bool(argv[2]);
    }
    else if (argc == 2)
        include_sub_childs = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    HWND parent_handle = RU::value_to_hwnd(argv[0]);
    ChildWindowsInfo info;
    info.parent_handle = parent_handle;
    info.include_sub_childs = include_sub_childs;
    info.include_hidden = include_hidden;
    EnumChildWindows(parent_handle, GetChildWindowsProc, reinterpret_cast<LPARAM>(&info));
    VALUE v_handles = rb_ary_new2((long)info.handles.size());
    int index = 0;
    for (std::vector<HWND>::iterator it = info.handles.begin(); it != info.handles.end(); ++it) {
        rb_ary_store(v_handles, index, RU::to_value(*it));
        ++index;
    }
    return v_handles;
}

VALUE AMS::Window::rbf_find_window_by_caption(int argc, VALUE* argv, VALUE self) {
    AMS::Sketchup::SearchInfo info;
    info.full_match = true;
    info.case_sensitive = true;
    info.mode = 0;
    if (argc == 3) {
        info.full_match = RU::value_to_bool(argv[1]);
        info.case_sensitive = RU::value_to_bool(argv[2]);
    }
    else if (argc == 2)
        info.full_match = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    info.text = RU::value_to_wc_str(argv[0]);
    info.found_handle = NULL;
    EnumWindows(FindWindowProc, reinterpret_cast<LPARAM>(&info));
    delete[] info.text;
    return RU::to_value(info.found_handle);
}

VALUE AMS::Window::rbf_find_child_window_by_caption(int argc, VALUE* argv, VALUE self) {
    AMS::Sketchup::ChildSearchInfo info;
    info.full_match = true;
    info.case_sensitive = true;
    info.include_sub_childs = false;
    info.mode = 0;
    if (argc == 5) {
        info.include_sub_childs = RU::value_to_bool(argv[2]);
        info.full_match = RU::value_to_bool(argv[3]);
        info.case_sensitive = RU::value_to_bool(argv[4]);
    }
    else if (argc == 4) {
        info.include_sub_childs = RU::value_to_bool(argv[2]);
        info.full_match = RU::value_to_bool(argv[3]);
    }
    else if (argc == 3) {
        info.include_sub_childs = RU::value_to_bool(argv[2]);
    }
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..5 arguments.");
    info.parent_handle = RU::value_to_hwnd(argv[0]);
    info.text = RU::value_to_wc_str(argv[1]);
    info.found_handle = NULL;
    EnumChildWindows(info.parent_handle, FindChildWindowProc, reinterpret_cast<LPARAM>(&info));
    delete[] info.text;
    return RU::to_value(info.found_handle);
}

VALUE AMS::Window::rbf_find_window_by_class_name(int argc, VALUE* argv, VALUE self) {
    AMS::Sketchup::SearchInfo info;
    info.full_match = true;
    info.case_sensitive = true;
    info.mode = 1;
    if (argc == 3) {
        info.full_match = RU::value_to_bool(argv[1]);
        info.case_sensitive = RU::value_to_bool(argv[2]);
    }
    else if (argc == 2)
        info.full_match = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    info.text = RU::value_to_wc_str(argv[0]);
    info.found_handle = NULL;
    EnumWindows(FindWindowProc, reinterpret_cast<LPARAM>(&info));
    delete[] info.text;
    return RU::to_value(info.found_handle);
}

VALUE AMS::Window::rbf_find_child_window_by_class_name(int argc, VALUE* argv, VALUE self) {
    AMS::Sketchup::ChildSearchInfo info;
    info.full_match = true;
    info.case_sensitive = true;
    info.include_sub_childs = false;
    info.mode = 1;
    if (argc == 5) {
        info.include_sub_childs = RU::value_to_bool(argv[2]);
        info.full_match = RU::value_to_bool(argv[3]);
        info.case_sensitive = RU::value_to_bool(argv[4]);
    }
    else if (argc == 4) {
        info.include_sub_childs = RU::value_to_bool(argv[2]);
        info.full_match = RU::value_to_bool(argv[3]);
    }
    else if (argc == 3) {
        info.include_sub_childs = RU::value_to_bool(argv[2]);
    }
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..5 arguments.");
    info.parent_handle = RU::value_to_hwnd(argv[0]);
    info.text = RU::value_to_wc_str(argv[1]);
    info.found_handle = NULL;
    EnumChildWindows(info.parent_handle, FindChildWindowProc, reinterpret_cast<LPARAM>(&info));
    delete[] info.text;
    return RU::to_value(info.found_handle);
}

VALUE AMS::Window::rbf_client_to_screen(VALUE self, VALUE v_handle, VALUE v_x, VALUE v_y) {
    HWND handle = RU::value_to_hwnd(v_handle);
    POINT pt;
    pt.x = RU::value_to_int(v_x);
    pt.y = RU::value_to_int(v_y);
    ClientToScreen(handle, &pt);
    return rb_ary_new3(2, RU::to_value(pt.x), RU::to_value(pt.y));
}

VALUE AMS::Window::rbf_screen_to_client(VALUE self, VALUE v_handle, VALUE v_x, VALUE v_y) {
    HWND handle = RU::value_to_hwnd(v_handle);
    POINT pt;
    pt.x = RU::value_to_int(v_x);
    pt.y = RU::value_to_int(v_y);
    ScreenToClient(handle, &pt);
    return rb_ary_new3(2, RU::to_value(pt.x), RU::to_value(pt.y));
}

VALUE AMS::Window::rbf_map_point(VALUE self, VALUE v_handle_from, VALUE v_handle_to, VALUE v_x, VALUE v_y) {
    HWND handle_from = RU::value_to_hwnd(v_handle_from);
    HWND handle_to = RU::value_to_hwnd(v_handle_to);
    POINT pt;
    pt.x = RU::value_to_int(v_x);
    pt.y = RU::value_to_int(v_y);
    MapWindowPoints(handle_from, handle_to, &pt, 1);
    return rb_ary_new3(2, RU::to_value(pt.x), RU::to_value(pt.y));
}

VALUE AMS::Window::rbf_get_module_handle(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    HMODULE module_handle = (HMODULE)GetWindowLongPtr(handle, GWLP_HINSTANCE);
    return RU::to_value(module_handle);
}

VALUE AMS::Window::rbf_get_executable_path(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProc == NULL) return Qnil;
    TCHAR buffer[4096];
    unsigned int len = GetModuleFileNameEx(hProc, 0, buffer, 4096);
    CloseHandle(hProc);
    for (unsigned int i = 0; i < len; ++i)
        if (buffer[i] == '\\') buffer[i] = '/';
    return RU::win_wc_str_to_value(buffer, len);
}

VALUE AMS::Window::rbf_get_executable_name(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProc == NULL) return Qnil;
    TCHAR buffer[256];
    unsigned int len = GetModuleBaseName(hProc, 0, buffer, 256);
    CloseHandle(hProc);
    return RU::win_wc_str_to_value(buffer, len);
}

VALUE AMS::Window::rbf_get_system_color(VALUE self, VALUE v_index) {
    DWORD color = GetSysColor(RU::value_to_int(v_index));
    Geom::Color c(GetRValue(color), GetGValue(color), GetBValue(color), 255);
    return RU::color_to_value(c);
}

VALUE AMS::Window::rbf_set_system_color(VALUE self, VALUE v_index, VALUE v_color) {
    int index = RU::value_to_int(v_index);
    Geom::Color color;
    RU::value_to_color(v_color, color);
    int indices[1] = { index };
    COLORREF colors[1] = { RGB(color.m_r, color.m_g, color.m_b) };
    if (SetSysColors(1, indices, colors) != 0)
        return Qtrue;
    else
        return Qfalse;
}

VALUE AMS::Window::rbf_set_background_color(int argc, VALUE* argv, VALUE self) {
    bool b_activate = true;
    HWND handle;
    HBRUSH hbrush;
    RECT rect;
    HDC hdc;
    Geom::Color color;
    if (argc == 3)
        b_activate = RU::value_to_bool(argv[2]);
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");
    handle = RU::value_to_hwnd(argv[0]);
    RU::value_to_color(argv[1], color);
    hbrush = CreateSolidBrush(RGB(color.m_r, color.m_g, color.m_b));
    SetClassLongPtr(handle, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(hbrush));
    //DeleteObject(hbrush);
    //return RU::to_value(SetWindowPos(handle, NULL, 0, 0, 0, 0, 0x0267 | (b_activate ? 0 : 0x0014)) == TRUE);
    GetClientRect(handle, &rect);
    hdc = GetDC(handle);
    FillRect(hdc, &rect, hbrush);

    return Qnil;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Window::init_ruby(VALUE mAMS) {
    VALUE mWindow = rb_define_module_under(mAMS, "Window");

    rb_define_module_function(mWindow, "is_valid?", VALUEFUNC(AMS::Window::rbf_is_valid), 1);
    rb_define_module_function(mWindow, "is_active?", VALUEFUNC(AMS::Window::rbf_is_active), 1);
    rb_define_module_function(mWindow, "is_visible?", VALUEFUNC(AMS::Window::rbf_is_visible), 1);
    rb_define_module_function(mWindow, "is_unicode?", VALUEFUNC(AMS::Window::rbf_is_unicode), 1);
    rb_define_module_function(mWindow, "is_maximized?", VALUEFUNC(AMS::Window::rbf_is_maximized), 1);
    rb_define_module_function(mWindow, "is_minimized?", VALUEFUNC(AMS::Window::rbf_is_minimized), 1);
    rb_define_module_function(mWindow, "is_restored?", VALUEFUNC(AMS::Window::rbf_is_restored), 1);
    rb_define_module_function(mWindow, "is_child?", VALUEFUNC(AMS::Window::rbf_is_child), 2);
    rb_define_module_function(mWindow, "show", VALUEFUNC(AMS::Window::rbf_show), 2);
    rb_define_module_function(mWindow, "get_active", VALUEFUNC(AMS::Window::rbf_get_active), 0);
    rb_define_module_function(mWindow, "set_active", VALUEFUNC(AMS::Window::rbf_set_active), 1);
    rb_define_module_function(mWindow, "get_keyboard_focus", VALUEFUNC(AMS::Window::rbf_get_keyboard_focus), 0);
    rb_define_module_function(mWindow, "set_keyboard_focus", VALUEFUNC(AMS::Window::rbf_set_keyboard_focus), 1);
    rb_define_module_function(mWindow, "get_parent", VALUEFUNC(AMS::Window::rbf_get_parent), 1);
    rb_define_module_function(mWindow, "set_parent", VALUEFUNC(AMS::Window::rbf_set_parent), 2);
    rb_define_module_function(mWindow, "get_ancestor", VALUEFUNC(AMS::Window::rbf_get_ancestor), 2);
    rb_define_module_function(mWindow, "bring_to_top", VALUEFUNC(AMS::Window::rbf_bring_to_top), 1);
    rb_define_module_function(mWindow, "get_class_name", VALUEFUNC(AMS::Window::rbf_get_class_name), 1);
    rb_define_module_function(mWindow, "get_related", VALUEFUNC(AMS::Window::rbf_get_related), 2);
    rb_define_module_function(mWindow, "get_thread_id", VALUEFUNC(AMS::Window::rbf_get_thread_id), 1);
    rb_define_module_function(mWindow, "get_process_id", VALUEFUNC(AMS::Window::rbf_get_process_id), 1);
    rb_define_module_function(mWindow, "get_long", VALUEFUNC(AMS::Window::rbf_get_long), 2);
    rb_define_module_function(mWindow, "set_long", VALUEFUNC(AMS::Window::rbf_set_long), 3);
    rb_define_module_function(mWindow, "get_class_long", VALUEFUNC(AMS::Window::rbf_get_class_long), 2);
    rb_define_module_function(mWindow, "set_class_long", VALUEFUNC(AMS::Window::rbf_set_class_long), 3);
    rb_define_module_function(mWindow, "get_menu", VALUEFUNC(AMS::Window::rbf_get_menu), 1);
    rb_define_module_function(mWindow, "set_menu", VALUEFUNC(AMS::Window::rbf_set_menu), 2);
    rb_define_module_function(mWindow, "get_caption", VALUEFUNC(AMS::Window::rbf_get_caption), 1);
    rb_define_module_function(mWindow, "set_caption", VALUEFUNC(AMS::Window::rbf_set_caption), 2);
    rb_define_module_function(mWindow, "get_rect", VALUEFUNC(AMS::Window::rbf_get_rect), 1);
    rb_define_module_function(mWindow, "set_rect", VALUEFUNC(AMS::Window::rbf_set_rect), -1);
    rb_define_module_function(mWindow, "get_client_rect", VALUEFUNC(AMS::Window::rbf_get_client_rect), 1);
    rb_define_module_function(mWindow, "adjust_rect", VALUEFUNC(AMS::Window::rbf_adjust_rect), 4);
    rb_define_module_function(mWindow, "get_placement", VALUEFUNC(AMS::Window::rbf_get_placement), 1);
    rb_define_module_function(mWindow, "set_placement", VALUEFUNC(AMS::Window::rbf_set_placement), 6);
    rb_define_module_function(mWindow, "lock_update", VALUEFUNC(AMS::Window::rbf_lock_update), 1);
    rb_define_module_function(mWindow, "set_pos", VALUEFUNC(AMS::Window::rbf_set_pos), 7);
    rb_define_module_function(mWindow, "move", VALUEFUNC(AMS::Window::rbf_move), -1);
    rb_define_module_function(mWindow, "refresh", VALUEFUNC(AMS::Window::rbf_refresh), 1);
    rb_define_module_function(mWindow, "close", VALUEFUNC(AMS::Window::rbf_close), 1);
    rb_define_module_function(mWindow, "get_origin", VALUEFUNC(AMS::Window::rbf_get_origin), 1);
    rb_define_module_function(mWindow, "set_origin", VALUEFUNC(AMS::Window::rbf_set_origin), -1);
    rb_define_module_function(mWindow, "get_size", VALUEFUNC(AMS::Window::rbf_get_size), 1);
    rb_define_module_function(mWindow, "set_size", VALUEFUNC(AMS::Window::rbf_set_size), -1);
    rb_define_module_function(mWindow, "is_resizeable", VALUEFUNC(AMS::Window::rbf_is_resizeable), 1);
    rb_define_module_function(mWindow, "set_resizeable", VALUEFUNC(AMS::Window::rbf_set_resizeable), -1);
    rb_define_module_function(mWindow, "is_input_enabled", VALUEFUNC(AMS::Window::rbf_is_input_enabled), 1);
    rb_define_module_function(mWindow, "enable_input", VALUEFUNC(AMS::Window::rbf_enable_input), 2);
    rb_define_module_function(mWindow, "send_message", VALUEFUNC(AMS::Window::rbf_send_message), 4);
    rb_define_module_function(mWindow, "post_message", VALUEFUNC(AMS::Window::rbf_post_message), 4);
    rb_define_module_function(mWindow, "peek_message", VALUEFUNC(AMS::Window::rbf_peek_message), 4);
    rb_define_module_function(mWindow, "peek_message2", VALUEFUNC(AMS::Window::rbf_peek_message2), 4);
    rb_define_module_function(mWindow, "set_icon", VALUEFUNC(AMS::Window::rbf_set_icon), 2);
    rb_define_module_function(mWindow, "draw_menu_bar", VALUEFUNC(AMS::Window::rbf_draw_menu_bar), 1);
    rb_define_module_function(mWindow, "get_layered_attributes", VALUEFUNC(AMS::Window::rbf_get_layered_attributes), 1);
    rb_define_module_function(mWindow, "set_layered_attributes", VALUEFUNC(AMS::Window::rbf_set_layered_attributes), 4);
    rb_define_module_function(mWindow, "get_windows", VALUEFUNC(AMS::Window::rbf_get_windows), -1);
    rb_define_module_function(mWindow, "get_process_windows", VALUEFUNC(AMS::Window::rbf_get_process_windows), -1);
    rb_define_module_function(mWindow, "get_thread_windows", VALUEFUNC(AMS::Window::rbf_get_thread_windows), -1);
    rb_define_module_function(mWindow, "get_child_windows", VALUEFUNC(AMS::Window::rbf_get_child_windows), -1);
    rb_define_module_function(mWindow, "find_window_by_caption", VALUEFUNC(AMS::Window::rbf_find_window_by_caption), -1);
    rb_define_module_function(mWindow, "find_child_window_by_caption", VALUEFUNC(AMS::Window::rbf_find_child_window_by_caption), -1);
    rb_define_module_function(mWindow, "find_window_by_class_name", VALUEFUNC(AMS::Window::rbf_find_window_by_class_name), -1);
    rb_define_module_function(mWindow, "find_child_window_by_class_name", VALUEFUNC(AMS::Window::rbf_find_child_window_by_class_name), -1);
    rb_define_module_function(mWindow, "client_to_screen", VALUEFUNC(AMS::Window::rbf_client_to_screen), 3);
    rb_define_module_function(mWindow, "screen_to_client", VALUEFUNC(AMS::Window::rbf_screen_to_client), 3);
    rb_define_module_function(mWindow, "map_point", VALUEFUNC(AMS::Window::rbf_map_point), 4);
    rb_define_module_function(mWindow, "get_module_handle", VALUEFUNC(AMS::Window::rbf_get_module_handle), 1);
    rb_define_module_function(mWindow, "get_executable_path", VALUEFUNC(AMS::Window::rbf_get_executable_path), 1);
    rb_define_module_function(mWindow, "get_executable_name", VALUEFUNC(AMS::Window::rbf_get_executable_name), 1);
    /*rb_define_module_function(mWindow, "get_system_color", VALUEFUNC(AMS::Window::rbf_get_system_color), 1);
    rb_define_module_function(mWindow, "set_system_color", VALUEFUNC(AMS::Window::rbf_set_system_color), 2);*/
    rb_define_module_function(mWindow, "set_background_color", VALUEFUNC(AMS::Window::rbf_set_background_color), -1);
}
