/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_SKETCHUP_H
#define AMS_SKETCHUP_H

#include "ams_ext.h"

class AMS::Sketchup {
public:
    // Structures
    struct KeyboardShortcut {
        bool control_down;
        bool menu_down;
        bool shift_down;
        int virtual_key;

        KeyboardShortcut() :
            control_down(false),
            menu_down(false),
            shift_down(false),
            virtual_key(0)
        {
        }
    };

    struct EventData {
        VALUE observer;
        ID id;
        VALUE* params;
        unsigned int size;
    };

    struct PostEventData {
        ID id;
        VALUE* params;
        unsigned int size;
    };

    struct ExceptionData {
        VALUE observer;
        VALUE exception;
    };

    struct CompareCString {
        bool operator()(const char* a, const char* b) const {
            return std::strcmp(a, b) < 0;
        }
    };

    struct CompareCString2 {
        bool operator()(const wchar_t* a, const wchar_t *b) const {
            return std::wcscmp(a, b) < 0;
        }
    };

    struct CompareKeyboardShortcuts {
        bool operator() (const KeyboardShortcut* ksa, const KeyboardShortcut* ksb) const {
            return std::tie(ksa->control_down, ksa->menu_down, ksa->shift_down, ksa->virtual_key) < std::tie(ksb->control_down, ksb->menu_down, ksb->shift_down, ksb->virtual_key);
        }
    };

    struct SearchInfo {
        bool full_match;
        bool case_sensitive;
        int mode;
        const wchar_t* text;
        HWND found_handle;
    };

    struct ChildSearchInfo {
        bool full_match;
        bool case_sensitive;
        bool include_sub_childs;
        int mode;
        const wchar_t* text;
        HWND parent_handle;
        HWND found_handle;
    };

    struct MessageboxData {
        HWND handle;
        wchar_t* text;
        wchar_t* caption;
        unsigned int type;
        int result;
        VALUE proc;

        MessageboxData(HWND p_handle, wchar_t* p_text, wchar_t* p_caption, unsigned int p_type, VALUE v_proc) :
            handle(p_handle),
            text(p_text),
            caption(p_caption),
            type(p_type),
            result(0),
            proc(v_proc)
        {
        }

        ~MessageboxData() {
            if (text != nullptr)
                delete[] text;
            if (caption != nullptr)
                delete[] caption;
        }
    };

private:
    // Constants
    static const int ACCELERATOR_TIMER_ID;
    static const int CONTAINER_TIMER_ID;
    static const unsigned int POST_EVENT_DELAY;
    static const unsigned int UMESSAGE_BASE;
    static const unsigned int UMESSAGE_MIN;
    static const unsigned int UMESSAGE_MAX;
    static const unsigned int UMESSAGE_DATA_ID;

    // Variables
    static ID ID_SWO_ACTIVATE;
    static ID ID_SWO_DEACTIVATE;
    static ID ID_SWO_ERROR;

    static ID ID_SWO_ON_QUIT;

    static ID ID_SWO_ON_RESTORE;
    static ID ID_SWO_ON_POST_RESTORE;
    static ID ID_SWO_ON_MINIMIZE;
    static ID ID_SWO_ON_POST_MINIMIZE;
    static ID ID_SWO_ON_MAXIMIZE;
    static ID ID_SWO_ON_POST_MAXIMIZE;
    static ID ID_SWO_ON_SWITCH_FULL_SCREEN;
    static ID ID_SWO_ON_POST_SWITCH_FULL_SCREEN;

    static ID ID_SWO_ON_MENU_BAR_CHANGED;
    static ID ID_SWO_ON_POST_MENU_BAR_CHANGED;

    static ID ID_SWO_ON_ENTER_MENU;
    static ID ID_SWO_ON_POST_ENTER_MENU;
    static ID ID_SWO_ON_EXIT_MENU;
    static ID ID_SWO_ON_POST_EXIT_MENU;

    static ID ID_SWO_ON_CAPTION_CHANGED;
    static ID ID_SWO_ON_POST_CAPTION_CHANGED;

    static ID ID_SWO_ON_FOCUS;
    static ID ID_SWO_ON_POST_FOCUS;
    static ID ID_SWO_ON_BLUR;
    static ID ID_SWO_ON_POST_BLUR;

    static ID ID_SWO_ON_ENTER_SIZE_MOVE;
    static ID ID_SWO_ON_POST_ENTER_SIZE_MOVE;
    static ID ID_SWO_ON_EXIT_SIZE_MOVE;
    static ID ID_SWO_ON_POST_EXIT_SIZE_MOVE;
    static ID ID_SWO_ON_SIZE_MOVE;
    static ID ID_SWO_ON_POST_SIZE_MOVE;

    static ID ID_SWO_ON_USER_MESSAGE;

    static ID ID_SWP_ON_COMMAND;

    static ID ID_SWO_ON_VIEWPORT_BORDER_CHANGED;
    static ID ID_SWO_ON_POST_VIEWPORT_BORDER_CHANGED;

    static ID ID_SWO_ON_VIEWPORT_SIZE;
    static ID ID_SWO_ON_POST_VIEWPORT_SIZE;

    static ID ID_SWP_ON_LBUTTON_DOWN;
    static ID ID_SWP_ON_LBUTTON_UP;
    static ID ID_SWP_ON_LBUTTON_DOUBLE_CLICK;
    static ID ID_SWP_ON_RBUTTON_DOWN;
    static ID ID_SWP_ON_RBUTTON_UP;
    static ID ID_SWP_ON_RBUTTON_DOUBLE_CLICK;
    static ID ID_SWP_ON_MBUTTON_DOWN;
    static ID ID_SWP_ON_MBUTTON_UP;
    static ID ID_SWP_ON_MBUTTON_DOUBLE_CLICK;
    static ID ID_SWP_ON_XBUTTON1_DOWN;
    static ID ID_SWP_ON_XBUTTON1_UP;
    static ID ID_SWP_ON_XBUTTON1_DOUBLE_CLICK;
    static ID ID_SWP_ON_XBUTTON2_DOWN;
    static ID ID_SWP_ON_XBUTTON2_UP;
    static ID ID_SWP_ON_XBUTTON2_DOUBLE_CLICK;

    static ID ID_SWP_ON_MOUSE_WHEEL_ROTATE;
    static ID ID_SWP_ON_MOUSE_WHEEL_TILT;

    static ID ID_SWP_ON_MOUSE_ENTER;
    static ID ID_SWP_ON_MOUSE_LEAVE;
    static ID ID_SWP_ON_MOUSE_MOVE;

    static ID ID_SWO_ON_SCENES_BAR_FILLED;
    static ID ID_SWO_ON_POST_SCENES_BAR_FILLED;
    static ID ID_SWO_ON_SCENES_BAR_EMPTIED;
    static ID ID_SWO_ON_POST_SCENES_BAR_EMPTIED;

    static ID ID_SWO_ON_TOOLBAR_CONTAINER_FILLED;
    static ID ID_SWO_ON_POST_TOOLBAR_CONTAINER_FILLED;
    static ID ID_SWO_ON_TOOLBAR_CONTAINER_EMPTIED;
    static ID ID_SWO_ON_POST_TOOLBAR_CONTAINER_EMPTIED;

    static ID ID_SWO_ON_PAGE_SELECTED;
    static ID ID_SWP_ON_PAGE_SELECTED;

    static ID ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED;
    static ID ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED;

    static ID ID_SWO_ON_SCENES_BAR_VISIBILITY_CHANGED;
    static ID ID_SWO_ON_POST_SCENES_BAR_VISIBILITY_CHANGED;

    static ID ID_SWO_ON_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED;
    static ID ID_SWO_ON_POST_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED;

    static ID ID_SWP_ON_KEY_DOWN;
    static ID ID_SWP_ON_KEY_UP;
    static ID ID_SWP_ON_KEY_EXTENDED;

public:
    static DWORD su_process_id;
    static DWORD su_thread_id;
    static HWND su_main_window;
    static HWND su_viewport;
    static HMODULE su_module_handle;
    static HMENU su_menu_bar;
    static HWND su_status_bar;
    static HWND su_scenes_bar;
    static HWND su_control_bars[4];
    static HWND su_tray_dock_panes[4];
    static unsigned int su_dock_pane_count;
    static HWND su_tab_control;
    static std::map<int, const wchar_t*> su_virtual_key_names;
    static std::map<const wchar_t*, int, CompareCString2> su_virtual_key_codes;
    static std::map<const wchar_t*, int, CompareCString2> su_virtual_key_codes2;
    static VALUE su_messagebox_procedures;

private:
    static CRITICAL_SECTION su_mb_critical_section;
    static std::set<HWND> su_active_dialogs;
    static std::set<HWND> su_active_toolbars;
    static std::set<HWND> su_dialogs_to_ignore;
    static std::set<HWND> su_toolbars_to_ignore;
    static std::vector<const wchar_t*> su_dialog_en_names;
    static std::vector<const wchar_t*> su_controlled_dialogs;
    static WNDPROC su_original_main_window_proc;
    static WNDPROC su_original_viewport_window_proc;
    static WNDPROC su_original_tab_proc;
    static WNDPROC su_original_statusbar_proc;
    static WNDPROC su_original_scenesbar_proc;
    static WNDPROC su_original_controlbar_proc[4];
    static VALUE su_observers;
    static unsigned int su_observers_size;
    static LONG_PTR su_mw_style;
    static LONG_PTR su_mw_style_ex;
    static WINDOWPLACEMENT su_mw_placement;
    static bool su_menu_bar_set;
    static int su_mw_show_state;
    static std::map<KeyboardShortcut*, unsigned long long, CompareKeyboardShortcuts> su_accelerators;
    static unsigned long long su_expected_command;
    static unsigned long long su_triggered_command;
    static int su_vp_cursor_pos[2];
    static bool su_vp_mouse_leave;
    static bool su_tbc_visible[4];
    static bool su_tbc_filled[4];
    static HHOOK su_hooks[2];
    static short su_f1_state;
    static short su_lwin_state;
    static short su_rwin_state;
    static std::set<HWND> su_active_trays;
    static int su_tray_tab_size[4];
    static std::set<HWND> su_active_sliders;
    static std::map<UINT_PTR, PostEventData*> su_post_event_data;
    static bool su_process_mw_messages;
    static bool su_process_kb_messages;
    static bool su_process_sb_page_selection;

    // Helper Functions
public:
    static void c_init();
    static void c_uninit();
    static void c_update_viewport_handle();
    static void c_get_viewport_rect(RECT& rect);
    static int c_vk_name_to_code(const wchar_t* vk_name);
    static void c_get_menu_commands(HMENU handle, const wchar_t* cur_path, int cur_path_len, std::map<const wchar_t*, unsigned long long, CompareCString2>& commands);
    static VALUE c_mbp_call_proc(VALUE v_args);
    static VALUE c_mbp_rescue_proc(VALUE v_args, VALUE v_exception);
    static VALUE c_mbp_trigger_proc(void *arg);

private:
    static bool c_is_main_window_full_screen();
    static bool c_is_main_window_maximized();
    static bool c_is_main_window_minimized();
    static bool c_is_main_window_restored();
    static bool c_set_main_window_full_screen(bool state, int reset_mode = 2, int set_mode = 1);
    static bool c_is_main_window_menu_bar_set();
    static bool c_is_viewport_border_set();
    static bool c_show_toolbar_container(int bar, bool state, bool refresh);
    static void c_adjust_container_rect(int bar, bool state, bool refresh);
    static bool c_is_toolbar_container_filled(int bar);
    static bool c_is_toolbar_container_empty(int bar);
    static bool c_is_toolbar_container_visible(int bar);
    static void c_get_dialogs(std::set<HWND>& items);
    static void c_get_visible_dialogs(std::set<HWND>& items);
    static void c_get_toolbars(std::set<HWND>& items);
    static void c_get_visible_toolbars(std::set<HWND>& items);
    static void c_get_other_main_windows(std::set<HWND>& items);
    static bool c_observers_contain(ID method_id);
    static void c_observers_update_process_info();
    static VALUE c_observer_call_proc(VALUE v_observer);
    static VALUE c_observer_rescue_proc(VALUE v_observer, VALUE v_exception);
    static bool c_call_observer_event(ID method_id, int param_count, VALUE* params, bool block_allowed = false);
    static bool c_call_single_observer_event(VALUE v_observer, ID method_id, int param_count, VALUE* params, bool block_allowed = false);
    static VALUE c_exception_call_proc(VALUE v_observer);
    static VALUE c_exception_rescue_proc(VALUE v_observer, VALUE v_exception);
    static bool c_call_exception_event(VALUE v_observer, VALUE v_exception);
    static void c_preset_status_bar_visible();
    static void c_find_accelerators();
    static bool c_call_accelerator(KeyboardShortcut* accelerator);
    static void c_verify_accelerator(WORD id);
    static bool c_is_main_window_style_maximized();
    static unsigned int c_show_trays(bool state);
    static unsigned int c_close_trays();
    static void c_get_hidden_tabbed_trays(std::set<HWND>& items);
    static bool c_recalc_layout();

    // Callback Functions
    static BOOL CALLBACK FindMainWindowProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK FindChildWindowsProc(HWND handle, LPARAM lParam);
    static LRESULT CALLBACK CustomMainWindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CustomViewportWindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CustomTabProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CustomStatusBarProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CustomScenesBarProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK CustomControlBarProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
    static BOOL CALLBACK ComputeContainerRectProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK IsContainerFilledProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetDialogsProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetVisibleDialogsProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetToolbarsProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetVisibleToolbarsProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK FindWindowProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK FindChildWindowProc(HWND handle, LPARAM lParam);
    static VOID CALLBACK AcceleratorTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    static VOID CALLBACK ContainerTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    static VOID CALLBACK PostEventTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static BOOL CALLBACK GetFloatingTraysProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetVisibleFloatingTraysProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetDockedTraysProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetVisibleDockedTraysProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetVisibleTraySlidersProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetOtherMainWindowsProc(HWND handle, LPARAM lParam);
    static BOOL CALLBACK GetVirtualScreenRectProc(HMONITOR monitor, HDC hdc, LPRECT lpRect, LPARAM dwData);
    static DWORD CALLBACK ThreadedMessageBox(LPVOID lpParam);

public:
    // Ruby Functions
    static VALUE rbf_is_32bit(VALUE self);
    static VALUE rbf_is_64bit(VALUE self);
    static VALUE rbf_get_module_handle(VALUE self);
    static VALUE rbf_get_executable_path(VALUE self);
    static VALUE rbf_get_executable_name(VALUE self);
    static VALUE rbf_get_registry_path(VALUE self);
    static VALUE rbf_get_process_id(VALUE self);
    static VALUE rbf_get_thread_id(VALUE self);
    static VALUE rbf_get_main_window(VALUE self);
    static VALUE rbf_get_menu_bar(VALUE self);
    static VALUE rbf_get_viewport(VALUE self);
    static VALUE rbf_get_status_bar(VALUE self);
    static VALUE rbf_get_scenes_bar(VALUE self);
    static VALUE rbf_get_toolbar_containers(VALUE self);
    static VALUE rbf_switch_full_screen(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_full_screen(VALUE self);
    static VALUE rbf_maximize(VALUE self);
    static VALUE rbf_is_maximized(VALUE self);
    static VALUE rbf_minimize(VALUE self);
    static VALUE rbf_is_minimized(VALUE self);
    static VALUE rbf_restore(VALUE self);
    static VALUE rbf_is_restored(VALUE self);
    static VALUE rbf_activate(VALUE self);
    static VALUE rbf_set_menu_bar(VALUE self, VALUE v_state);
    static VALUE rbf_is_menu_bar_set(VALUE self);
    static VALUE rbf_set_viewport_border(VALUE self, VALUE v_state);
    static VALUE rbf_is_viewport_border_set(VALUE self);
    static VALUE rbf_show_status_bar(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_status_bar_visible(VALUE self);
    static VALUE rbf_show_scenes_bar(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_scenes_bar_visible(VALUE self);
    static VALUE rbf_is_scenes_bar_filled(VALUE self);
    static VALUE rbf_is_scenes_bar_empty(VALUE self);
    static VALUE rbf_activate_scenes_bar_tab(VALUE self, VALUE v_tab);
    static VALUE rbf_show_toolbar_container(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_is_toolbar_container_visible(VALUE self, VALUE v_bar);
    static VALUE rbf_is_toolbar_container_filled(VALUE self, VALUE v_bar);
    static VALUE rbf_is_toolbar_container_empty(VALUE self, VALUE v_bar);
    static VALUE rbf_get_viewport_rect(VALUE self);
    static VALUE rbf_get_viewport_origin(VALUE self);
    static VALUE rbf_get_viewport_size(VALUE self);
    static VALUE rbf_get_viewport_center(VALUE self);
    static VALUE rbf_show_dialogs(VALUE self, VALUE v_state);
    static VALUE rbf_close_dialogs(VALUE self);
    static VALUE rbf_include_dialog(VALUE self, VALUE v_handle);
    static VALUE rbf_ignore_dialog(VALUE self, VALUE v_handle);
    static VALUE rbf_show_trays(VALUE self, VALUE v_state);
    static VALUE rbf_close_trays(VALUE self);
    static VALUE rbf_show_toolbars(VALUE self, VALUE v_state);
    static VALUE rbf_close_toolbars(VALUE self);
    static VALUE rbf_include_toolbar(VALUE self, VALUE v_handle);
    static VALUE rbf_ignore_toolbar(VALUE self, VALUE v_handle);
    static VALUE rbf_is_main_window_active(VALUE self);
    static VALUE rbf_is_active(VALUE self);
    static VALUE rbf_find_window_by_caption(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_find_child_window_by_caption(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_find_window_by_class_name(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_find_child_window_by_class_name(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_dialogs(VALUE self);
    static VALUE rbf_get_active_dialogs(VALUE self);
    static VALUE rbf_get_visible_dialogs(VALUE self);
    static VALUE rbf_get_toolbars(VALUE self);
    static VALUE rbf_get_active_toolbars(VALUE self);
    static VALUE rbf_get_visible_toolbars(VALUE self);
    static VALUE rbf_get_caption(VALUE self);
    static VALUE rbf_set_caption(VALUE self, VALUE v_caption);
    static VALUE rbf_refresh(VALUE self);
    static VALUE rbf_close(VALUE self);
    static VALUE rbf_add_observer(VALUE self, VALUE v_object);
    static VALUE rbf_remove_observer(VALUE self, VALUE v_object);
    static VALUE rbf_send_user_message(VALUE self, VALUE v_receiver, VALUE v_id, VALUE v_data);
    static VALUE rbf_get_other_main_windows(VALUE self);
    static VALUE rbf_threaded_messagebox(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_set_background_color(VALUE self, VALUE v_color);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_SKETCHUP_H */
