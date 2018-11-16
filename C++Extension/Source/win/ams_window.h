/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_WINDOW_H
#define AMS_WINDOW_H

#include "ams_ext.h"

class AMS::Window {
public:
    // Structures
    struct WindowsInfo {
        DWORD id;
        int mode;
        bool include_hidden;
        std::vector<HWND> handles;
    };

    struct ChildWindowsInfo {
        HWND parent_handle;
        bool include_sub_childs;
        bool include_hidden;
        std::vector<HWND> handles;
    };

private:
    // Callback Functions
    static BOOL CALLBACK GetWindowsProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetChildWindowsProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK FindWindowProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK FindChildWindowProc(HWND handle, LPARAM lParam);

public:
    // Ruby Functions
    static VALUE rbf_is_valid(VALUE self, VALUE v_handle);
    static VALUE rbf_is_active(VALUE self, VALUE v_handle);
    static VALUE rbf_is_visible(VALUE self, VALUE v_handle);
    static VALUE rbf_is_unicode(VALUE self, VALUE v_handle);
    static VALUE rbf_is_maximized(VALUE self, VALUE v_handle);
    static VALUE rbf_is_minimized(VALUE self, VALUE v_handle);
    static VALUE rbf_is_restored(VALUE self, VALUE v_handle);
    static VALUE rbf_is_child(VALUE self, VALUE v_parent_handle, VALUE v_handle);
    static VALUE rbf_show(VALUE self, VALUE v_handle, VALUE v_state);
    static VALUE rbf_get_active(VALUE self);
    static VALUE rbf_set_active(VALUE self, VALUE v_handle);
    static VALUE rbf_get_keyboard_focus(VALUE self);
    static VALUE rbf_set_keyboard_focus(VALUE self, VALUE v_handle);
    static VALUE rbf_get_parent(VALUE self, VALUE v_handle);
    static VALUE rbf_set_parent(VALUE self, VALUE v_handle, VALUE v_parent_handle);
    static VALUE rbf_get_ancestor(VALUE self, VALUE v_handle, VALUE v_flag);
    static VALUE rbf_bring_to_top(VALUE self, VALUE v_handle);
    static VALUE rbf_get_class_name(VALUE self, VALUE v_handle);
    static VALUE rbf_get_related(VALUE self, VALUE v_handle, VALUE v_command);
    static VALUE rbf_get_thread_id(VALUE self, VALUE v_handle);
    static VALUE rbf_get_process_id(VALUE self, VALUE v_handle);
    static VALUE rbf_get_long(VALUE self, VALUE v_handle, VALUE v_index);
    static VALUE rbf_set_long(VALUE self, VALUE v_handle, VALUE v_index, VALUE v_long);
    static VALUE rbf_get_class_long(VALUE self, VALUE v_handle, VALUE v_index);
    static VALUE rbf_set_class_long(VALUE self, VALUE v_handle, VALUE v_index, VALUE v_long);
    static VALUE rbf_get_menu(VALUE self, VALUE v_handle);
    static VALUE rbf_set_menu(VALUE self, VALUE v_handle, VALUE v_menu_handle);
    static VALUE rbf_get_caption(VALUE self, VALUE v_handle);
    static VALUE rbf_set_caption(VALUE self, VALUE v_handle, VALUE v_caption);
    static VALUE rbf_get_rect(VALUE self, VALUE v_handle);
    static VALUE rbf_set_rect(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_client_rect(VALUE self, VALUE v_handle);
    static VALUE rbf_adjust_rect(VALUE self, VALUE v_rect, VALUE v_style, VALUE v_style_ex, VALUE v_bmenu);
    static VALUE rbf_get_placement(VALUE self, VALUE v_handle);
    static VALUE rbf_set_placement(VALUE self, VALUE v_handle, VALUE v_flags, VALUE v_show_cmd, VALUE v_min_pt, VALUE v_max_pt, VALUE v_rect);
    static VALUE rbf_lock_update(VALUE self, VALUE v_handle);
    static VALUE rbf_set_pos(VALUE self, VALUE v_handle, VALUE v_handle_insert_after, VALUE v_x, VALUE v_y, VALUE v_cx, VALUE v_cy, VALUE v_flags);
    static VALUE rbf_move(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_refresh(VALUE self, VALUE v_handle);
    static VALUE rbf_close(VALUE self, VALUE v_handle);
    static VALUE rbf_get_origin(VALUE self, VALUE v_handle);
    static VALUE rbf_set_origin(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_size(VALUE self, VALUE v_handle);
    static VALUE rbf_set_size(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_resizeable(VALUE self, VALUE v_handle);
    static VALUE rbf_set_resizeable(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_input_enabled(VALUE self, VALUE v_handle);
    static VALUE rbf_enable_input(VALUE self, VALUE v_handle, VALUE v_state);
    static VALUE rbf_send_message(VALUE self, VALUE v_handle, VALUE v_message, VALUE v_wParam, VALUE v_lParam);
    static VALUE rbf_post_message(VALUE self, VALUE v_handle, VALUE v_message, VALUE v_wParam, VALUE v_lParam);
    static VALUE rbf_peek_message(VALUE self, VALUE v_handle, VALUE v_msg_filter_min, VALUE v_msg_filter_max, VALUE v_remove_flag);
    static VALUE rbf_peek_message2(VALUE self, VALUE v_handle, VALUE v_msg_filter_min, VALUE v_msg_filter_max, VALUE v_remove_flag);
    static VALUE rbf_set_icon(VALUE self, VALUE v_handle, VALUE v_path);
    static VALUE rbf_draw_menu_bar(VALUE self, VALUE v_handle);
    static VALUE rbf_get_layered_attributes(VALUE self, VALUE v_handle);
    static VALUE rbf_set_layered_attributes(VALUE self, VALUE v_handle, VALUE v_color, VALUE v_opacity, VALUE v_flags);
    static VALUE rbf_get_windows(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_process_windows(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_thread_windows(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_child_windows(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_find_window_by_caption(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_find_child_window_by_caption(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_find_window_by_class_name(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_find_child_window_by_class_name(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_client_to_screen(VALUE self, VALUE v_handle, VALUE v_x, VALUE v_y);
    static VALUE rbf_screen_to_client(VALUE self, VALUE v_handle, VALUE v_x, VALUE v_y);
    static VALUE rbf_map_point(VALUE self, VALUE v_handle_from, VALUE v_handle_to, VALUE v_x, VALUE v_y);
    static VALUE rbf_get_module_handle(VALUE self, VALUE v_handle);
    static VALUE rbf_get_executable_path(VALUE self, VALUE v_handle);
    static VALUE rbf_get_executable_name(VALUE self, VALUE v_handle);
    static VALUE rbf_get_system_color(VALUE self, VALUE v_index);
    static VALUE rbf_set_system_color(VALUE self, VALUE v_index, VALUE v_color);
    static VALUE rbf_set_background_color(int argc, VALUE* argv, VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_WINDOW_H */
