/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_sketchup.h"
#include "../main/ams.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Constants
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

const int AMS::Sketchup::ACCELERATOR_TIMER_ID = 5000;
const int AMS::Sketchup::CONTAINER_TIMER_ID = 5100;
const unsigned int AMS::Sketchup::POST_EVENT_DELAY = USER_TIMER_MINIMUM;
const unsigned int AMS::Sketchup::UMESSAGE_BASE = 0x5000;
const unsigned int AMS::Sketchup::UMESSAGE_MIN = 0x0000;
const unsigned int AMS::Sketchup::UMESSAGE_MAX = 0x0FFF;
const unsigned int AMS::Sketchup::UMESSAGE_DATA_ID = 0x5B7;


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Variables
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

ID AMS::Sketchup::ID_SWO_ACTIVATE;
ID AMS::Sketchup::ID_SWO_DEACTIVATE;
ID AMS::Sketchup::ID_SWO_ERROR;

ID AMS::Sketchup::ID_SWO_ON_QUIT;

ID AMS::Sketchup::ID_SWO_ON_RESTORE;
ID AMS::Sketchup::ID_SWO_ON_POST_RESTORE;
ID AMS::Sketchup::ID_SWO_ON_MINIMIZE;
ID AMS::Sketchup::ID_SWO_ON_POST_MINIMIZE;
ID AMS::Sketchup::ID_SWO_ON_MAXIMIZE;
ID AMS::Sketchup::ID_SWO_ON_POST_MAXIMIZE;
ID AMS::Sketchup::ID_SWO_ON_SWITCH_FULL_SCREEN;
ID AMS::Sketchup::ID_SWO_ON_POST_SWITCH_FULL_SCREEN;

ID AMS::Sketchup::ID_SWO_ON_MENU_BAR_CHANGED;
ID AMS::Sketchup::ID_SWO_ON_POST_MENU_BAR_CHANGED;

ID AMS::Sketchup::ID_SWO_ON_ENTER_MENU;
ID AMS::Sketchup::ID_SWO_ON_POST_ENTER_MENU;
ID AMS::Sketchup::ID_SWO_ON_EXIT_MENU;
ID AMS::Sketchup::ID_SWO_ON_POST_EXIT_MENU;

ID AMS::Sketchup::ID_SWO_ON_CAPTION_CHANGED;
ID AMS::Sketchup::ID_SWO_ON_POST_CAPTION_CHANGED;

ID AMS::Sketchup::ID_SWO_ON_FOCUS;
ID AMS::Sketchup::ID_SWO_ON_POST_FOCUS;
ID AMS::Sketchup::ID_SWO_ON_BLUR;
ID AMS::Sketchup::ID_SWO_ON_POST_BLUR;

ID AMS::Sketchup::ID_SWO_ON_ENTER_SIZE_MOVE;
ID AMS::Sketchup::ID_SWO_ON_POST_ENTER_SIZE_MOVE;
ID AMS::Sketchup::ID_SWO_ON_EXIT_SIZE_MOVE;
ID AMS::Sketchup::ID_SWO_ON_POST_EXIT_SIZE_MOVE;
ID AMS::Sketchup::ID_SWO_ON_SIZE_MOVE;
ID AMS::Sketchup::ID_SWO_ON_POST_SIZE_MOVE;

ID AMS::Sketchup::ID_SWO_ON_USER_MESSAGE;

ID AMS::Sketchup::ID_SWP_ON_COMMAND;

ID AMS::Sketchup::ID_SWO_ON_VIEWPORT_BORDER_CHANGED;
ID AMS::Sketchup::ID_SWO_ON_POST_VIEWPORT_BORDER_CHANGED;

ID AMS::Sketchup::ID_SWO_ON_VIEWPORT_SIZE;
ID AMS::Sketchup::ID_SWO_ON_POST_VIEWPORT_SIZE;

ID AMS::Sketchup::ID_SWP_ON_LBUTTON_DOWN;
ID AMS::Sketchup::ID_SWP_ON_LBUTTON_UP;
ID AMS::Sketchup::ID_SWP_ON_LBUTTON_DOUBLE_CLICK;
ID AMS::Sketchup::ID_SWP_ON_RBUTTON_DOWN;
ID AMS::Sketchup::ID_SWP_ON_RBUTTON_UP;
ID AMS::Sketchup::ID_SWP_ON_RBUTTON_DOUBLE_CLICK;
ID AMS::Sketchup::ID_SWP_ON_MBUTTON_DOWN;
ID AMS::Sketchup::ID_SWP_ON_MBUTTON_UP;
ID AMS::Sketchup::ID_SWP_ON_MBUTTON_DOUBLE_CLICK;
ID AMS::Sketchup::ID_SWP_ON_XBUTTON1_DOWN;
ID AMS::Sketchup::ID_SWP_ON_XBUTTON1_UP;
ID AMS::Sketchup::ID_SWP_ON_XBUTTON1_DOUBLE_CLICK;
ID AMS::Sketchup::ID_SWP_ON_XBUTTON2_DOWN;
ID AMS::Sketchup::ID_SWP_ON_XBUTTON2_UP;
ID AMS::Sketchup::ID_SWP_ON_XBUTTON2_DOUBLE_CLICK;

ID AMS::Sketchup::ID_SWP_ON_MOUSE_WHEEL_ROTATE;
ID AMS::Sketchup::ID_SWP_ON_MOUSE_WHEEL_TILT;

ID AMS::Sketchup::ID_SWP_ON_MOUSE_ENTER;
ID AMS::Sketchup::ID_SWP_ON_MOUSE_LEAVE;
ID AMS::Sketchup::ID_SWP_ON_MOUSE_MOVE;

ID AMS::Sketchup::ID_SWO_ON_SCENES_BAR_FILLED;
ID AMS::Sketchup::ID_SWO_ON_POST_SCENES_BAR_FILLED;
ID AMS::Sketchup::ID_SWO_ON_SCENES_BAR_EMPTIED;
ID AMS::Sketchup::ID_SWO_ON_POST_SCENES_BAR_EMPTIED;

ID AMS::Sketchup::ID_SWO_ON_TOOLBAR_CONTAINER_FILLED;
ID AMS::Sketchup::ID_SWO_ON_POST_TOOLBAR_CONTAINER_FILLED;
ID AMS::Sketchup::ID_SWO_ON_TOOLBAR_CONTAINER_EMPTIED;
ID AMS::Sketchup::ID_SWO_ON_POST_TOOLBAR_CONTAINER_EMPTIED;

ID AMS::Sketchup::ID_SWO_ON_PAGE_SELECTED;
ID AMS::Sketchup::ID_SWP_ON_PAGE_SELECTED;

ID AMS::Sketchup::ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED;
ID AMS::Sketchup::ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED;

ID AMS::Sketchup::ID_SWO_ON_SCENES_BAR_VISIBILITY_CHANGED;
ID AMS::Sketchup::ID_SWO_ON_POST_SCENES_BAR_VISIBILITY_CHANGED;

ID AMS::Sketchup::ID_SWO_ON_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED;
ID AMS::Sketchup::ID_SWO_ON_POST_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED;

ID AMS::Sketchup::ID_SWP_ON_KEY_DOWN;
ID AMS::Sketchup::ID_SWP_ON_KEY_UP;
ID AMS::Sketchup::ID_SWP_ON_KEY_EXTENDED;

DWORD AMS::Sketchup::su_process_id;
DWORD AMS::Sketchup::su_thread_id;
HWND AMS::Sketchup::su_main_window;
HWND AMS::Sketchup::su_viewport;
HMODULE AMS::Sketchup::su_module_handle;
HMENU AMS::Sketchup::su_menu_bar;
HWND AMS::Sketchup::su_status_bar;
HWND AMS::Sketchup::su_scenes_bar;
HWND AMS::Sketchup::su_control_bars[4];
HWND AMS::Sketchup::su_tray_dock_panes[4];
unsigned int AMS::Sketchup::su_dock_pane_count;
HWND AMS::Sketchup::su_tab_control;
std::map<int, const wchar_t*> AMS::Sketchup::su_virtual_key_names;
std::map<const wchar_t*, int, AMS::Sketchup::CompareCString2> AMS::Sketchup::su_virtual_key_codes;
std::map<const wchar_t*, int, AMS::Sketchup::CompareCString2> AMS::Sketchup::su_virtual_key_codes2;
VALUE AMS::Sketchup::su_messagebox_procedures;

CRITICAL_SECTION AMS::Sketchup::su_mb_critical_section;
std::set<HWND> AMS::Sketchup::su_active_dialogs;
std::set<HWND> AMS::Sketchup::su_active_toolbars;
std::set<HWND> AMS::Sketchup::su_dialogs_to_ignore;
std::set<HWND> AMS::Sketchup::su_toolbars_to_ignore;
std::vector<const wchar_t*> AMS::Sketchup::su_dialog_en_names;
std::vector<const wchar_t*> AMS::Sketchup::su_controlled_dialogs;
WNDPROC AMS::Sketchup::su_original_main_window_proc;
WNDPROC AMS::Sketchup::su_original_viewport_window_proc;
WNDPROC AMS::Sketchup::su_original_tab_proc;
WNDPROC AMS::Sketchup::su_original_statusbar_proc;
WNDPROC AMS::Sketchup::su_original_scenesbar_proc;
WNDPROC AMS::Sketchup::su_original_controlbar_proc[4];
VALUE AMS::Sketchup::su_observers;
unsigned int AMS::Sketchup::su_observers_size;
LONG_PTR AMS::Sketchup::su_mw_style;
LONG_PTR AMS::Sketchup::su_mw_style_ex;
WINDOWPLACEMENT AMS::Sketchup::su_mw_placement;
bool AMS::Sketchup::su_menu_bar_set;
int AMS::Sketchup::su_mw_show_state;
std::map<AMS::Sketchup::KeyboardShortcut*, unsigned long long, AMS::Sketchup::CompareKeyboardShortcuts> AMS::Sketchup::su_accelerators;
unsigned long long AMS::Sketchup::su_expected_command;
unsigned long long AMS::Sketchup::su_triggered_command;
int AMS::Sketchup::su_vp_cursor_pos[2];
bool AMS::Sketchup::su_vp_mouse_leave;
bool AMS::Sketchup::su_tbc_visible[4];
bool AMS::Sketchup::su_tbc_filled[4];
HHOOK AMS::Sketchup::su_hooks[2];
short AMS::Sketchup::su_f1_state;
short AMS::Sketchup::su_lwin_state;
short AMS::Sketchup::su_rwin_state;
std::set<HWND> AMS::Sketchup::su_active_trays;
int AMS::Sketchup::su_tray_tab_size[4];
std::set<HWND> AMS::Sketchup::su_active_sliders;
std::map<UINT_PTR, AMS::Sketchup::PostEventData*> AMS::Sketchup::su_post_event_data;
bool AMS::Sketchup::su_process_mw_messages;
bool AMS::Sketchup::su_process_kb_messages;
bool AMS::Sketchup::su_process_sb_page_selection;

/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Sketchup::c_update_viewport_handle() {
    if (IsWindow(su_viewport)) return;
    TCHAR* cname;
    if (RU::su_version == 6)
        cname = TEXT("AfxFrameOrView70u");
    else if (RU::su_version <= 8)
        cname = TEXT("AfxFrameOrView80u");
    else if (RU::su_version <= 16)
        cname = TEXT("AfxFrameOrView100u");
    else
        cname = TEXT("AfxFrameOrView140u");
    su_viewport = FindWindowEx(su_main_window, NULL, cname, NULL);
}

void AMS::Sketchup::c_get_viewport_rect(RECT& rect) {
    c_update_viewport_handle();

    LONG_PTR style = GetWindowLongPtr(su_viewport, GWL_STYLE);
    LONG_PTR ex_style = GetWindowLongPtr(su_viewport, GWL_EXSTYLE);

    RECT wrect;
    RECT crect;
    RECT arect;

    GetWindowRect(su_viewport, &wrect);
    GetClientRect(su_viewport, &crect);
    GetClientRect(su_viewport, &arect);
    AdjustWindowRectEx(&arect, (DWORD)style, 0, (DWORD)ex_style);

    rect.left = wrect.left - arect.left;
    rect.top = wrect.top - arect.top;
    rect.right = rect.left + crect.right;
    rect.bottom = rect.top + crect.bottom;
}

int AMS::Sketchup::c_vk_name_to_code(const wchar_t* vk_name) {
    std::map<const wchar_t*, int, AMS::Sketchup::CompareCString2>::iterator it;
    it = su_virtual_key_codes.find(vk_name);
    if (it != su_virtual_key_codes.end()) return it->second;
    it = su_virtual_key_codes2.find(vk_name);
    if (it != su_virtual_key_codes2.end()) return it->second;
    return 0;
}

void AMS::Sketchup::c_get_menu_commands(HMENU handle, const wchar_t* cur_path, int cur_path_len, std::map<const wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2>& commands) {
    if (IsMenu(handle) == FALSE) return;
    int count = GetMenuItemCount(handle);
    for (int i = 0; i < count; ++i) {
        MENUITEMINFO mii;
        wchar_t sz_string[256];
        mii.cbSize = sizeof(mii);
        mii.fMask = MIIM_TYPE;
        mii.fType = MFT_STRING;
        mii.cch = 256;
        mii.dwTypeData = sz_string;
        if (GetMenuItemInfo(handle, i, 1, &mii) == FALSE || mii.cch == 0) continue;
        wchar_t* path;
        int index;
        int item_len = 1;
        for (unsigned int x = 0; x < mii.cch; ++x) {
            if (mii.dwTypeData[x] == '\t')
                break;
            else if (mii.dwTypeData[x] != '&')
                ++item_len;
        }
        if (cur_path_len == 0) {
            path = new wchar_t[item_len];
            index = 0;
        }
        else {
            path = new wchar_t[cur_path_len + 1 + item_len];
            wmemcpy(path, cur_path, cur_path_len);
            path[cur_path_len] = '/';
            index = cur_path_len + 1;
        }
        for (unsigned int x = 0; x < mii.cch; ++x) {
            if (mii.dwTypeData[x] == '\t')
                break;
            else if (mii.dwTypeData[x] != '&') {
                path[index] = mii.dwTypeData[x];
                ++index;
            }
        }
        path[index] = '\0';
        unsigned int id = GetMenuItemID(handle, i);
        if (id == -1) {
            HMENU sub_menu = GetSubMenu(handle, i);
            if (IsMenu(sub_menu) == TRUE) {
                commands[path] = reinterpret_cast<unsigned long long>(sub_menu);
                c_get_menu_commands(sub_menu, path, index, commands);
                /*std::map<wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2> sub_commands;
                c_get_menu_commands(sub_menu, path, index, sub_commands);
                for (std::map<wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2>::iterator it = sub_commands.begin(); it != sub_commands.end(); ++it) {
                    commands[it->first] = it->second;
                }
                sub_commands.clear();*/
            }
        }
        else
            commands[path] = id;
    }
}

VALUE AMS::Sketchup::c_mbp_call_proc(VALUE v_args) {
    MessageboxData* data = reinterpret_cast<MessageboxData*>(v_args);
    rb_eval_cmd(data->proc, rb_ary_new3(1, RU::to_value(data->result)), 0);
    return Qnil;
}

VALUE AMS::Sketchup::c_mbp_rescue_proc(VALUE v_args, VALUE v_exception) {
    VALUE v_message = rb_funcall(v_exception, RU::INTERN_MESSAGE, 0);
    rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, v_message);
    VALUE v_backtrace = rb_funcall(v_exception, RU::INTERN_BACKTRACE, 0);
    rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, rb_ary_entry(v_backtrace, 0));
    return Qnil;
}

VALUE AMS::Sketchup::c_mbp_trigger_proc(void *arg) {
    MessageboxData* data = reinterpret_cast<MessageboxData*>(arg);
    VALUE v_data = reinterpret_cast<VALUE>(data);
    rb_rescue2(RUBY_METHOD_FUNC(c_mbp_call_proc), v_data, RUBY_METHOD_FUNC(c_mbp_rescue_proc), Qnil, rb_eException, VALUE(0));
    EnterCriticalSection(&su_mb_critical_section);
    RU::array_delete_first(su_messagebox_procedures, data->proc);
    delete data;
    LeaveCriticalSection(&su_mb_critical_section);
    return Qnil;
}

bool AMS::Sketchup::c_is_main_window_full_screen() {
    LONG_PTR style = GetWindowLongPtr(su_main_window, GWL_STYLE);
    return (style | WS_CAPTION) != style;
}

bool AMS::Sketchup::c_is_main_window_maximized() {
    if (c_is_main_window_full_screen())
        return false;
    else
        return IsZoomed(su_main_window) == TRUE;
}

bool AMS::Sketchup::c_is_main_window_minimized() {
    return IsIconic(su_main_window) == TRUE;
}

bool AMS::Sketchup::c_is_main_window_restored() {
    return !(c_is_main_window_full_screen() || IsZoomed(su_main_window) == TRUE || IsIconic(su_main_window) == TRUE);
}

bool AMS::Sketchup::c_set_main_window_full_screen(bool state, int reset_mode, int set_mode) {
    // Return false if already set to the requested state.
    if (c_is_main_window_full_screen() == state)
        return false;
    // Restore if minimized.
    if (IsIconic(su_main_window) == TRUE)
        ShowWindow(su_main_window, SW_SHOWNA);
    // Make sure it's active.
    BringWindowToTop(su_main_window);
    // Switch full screen.
    LONG_PTR style_ex = 0;
    LONG_PTR style = 0;
    UINT flags = 0;
    if (state) {
        // Save original style and placement.
        style = GetWindowLongPtr(su_main_window, GWL_STYLE);
        su_mw_style = 0x14cfc000 | (style & WS_MAXIMIZE);
        su_mw_placement.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(su_main_window, &su_mw_placement);
        // Set full screen styles and placement.
        style_ex = 0;
        style = WS_CLIPSIBLINGS | WS_MAXIMIZE | WS_TABSTOP | WS_GROUP | SS_WORDELLIPSIS;
        flags = SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOCOPYBITS;
    }
    else {
        // Set original styles and placement.
        style_ex = su_mw_style_ex;
        style = su_mw_style;
        if (reset_mode == 0) {
            style = style & ~ WS_MAXIMIZE;
            su_mw_placement.flags = 0;
            su_mw_placement.showCmd = SW_SHOWNORMAL;
        }
        else if (reset_mode == 1) {
            style = style | WS_MAXIMIZE;
            su_mw_placement.flags = WPF_RESTORETOMAXIMIZED;
            su_mw_placement.showCmd = SW_MAXIMIZE;
        }
        flags = SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW | SWP_NOCOPYBITS;
    }
    // Paint screen.
    RECT rect;
    GetClientRect(su_main_window, &rect);
    HDC hdc = GetDC(su_main_window);
    HBRUSH brush = CreateSolidBrush(RGB(16, 16, 16));
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
    /*SetBkColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    HBRUSH hatch_brush = CreateHatchBrush(HS_CROSS, RGB(0, 0, 0));
    FillRect(hdc, &rect, hatch_brush);
    DeleteObject(hatch_brush);*/
    ReleaseDC(su_main_window, hdc);
    // Set style.
    LockWindowUpdate(su_main_window);
    SetWindowLongPtr(su_main_window, GWL_EXSTYLE, (LONG_PTR)style_ex);
    SetWindowLongPtr(su_main_window, GWL_STYLE, (LONG_PTR)style);
    LockWindowUpdate(NULL);
    // Set placement.
    if (!state) SetWindowPlacement(su_main_window, &su_mw_placement);
    // Set position.
    int x, y, sx, sy;
    if (set_mode == 2) {
        rect.left = 0;
        rect.top = 0;
        rect.bottom = 0;
        rect.right = 0;
        EnumDisplayMonitors(NULL, NULL, GetVirtualScreenRectProc, reinterpret_cast<LPARAM>(&rect));
        x = rect.left;
        y = rect.top;
        sx = rect.right - rect.left;
        sy = rect.bottom - rect.top;
    }
    else if (set_mode == 1) {
        HMONITOR monitor = MonitorFromWindow(su_main_window, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info;
        info.cbSize = sizeof(MONITORINFO);
        GetMonitorInfo(monitor, &info);
        x = info.rcMonitor.left;
        y = info.rcMonitor.top;
        sx = info.rcMonitor.right - info.rcMonitor.left;
        sy = info.rcMonitor.bottom - info.rcMonitor.top;
    }
    else {
        x = 0;
        y = 0;
        sx = GetSystemMetrics(SM_CXSCREEN);
        sy = GetSystemMetrics(SM_CYSCREEN);
    }
    SetWindowPos(su_main_window, NULL, x, y, sx, sy, flags);
    // Return success.
    return true;
}

bool AMS::Sketchup::c_is_main_window_menu_bar_set() {
    return GetMenu(su_main_window) == su_menu_bar;
}

bool AMS::Sketchup::c_is_viewport_border_set() {
    c_update_viewport_handle();
    LONG_PTR style_ex = GetWindowLongPtr(su_viewport, GWL_EXSTYLE);
    return (style_ex | WS_EX_CLIENTEDGE) == style_ex;
}

bool AMS::Sketchup::c_show_toolbar_container(int bar, bool state, bool refresh) {
    bool cur_state = IsWindowVisible(su_control_bars[bar]) == TRUE;
    if (cur_state == state) {
        RECT rect;
        GetWindowRect(su_control_bars[bar], &rect);
        int w = rect.right - rect.left;
        int h = rect.bottom - rect.top;
        c_adjust_container_rect(bar, state, false);
        GetWindowRect(su_control_bars[bar], &rect);
        if (rect.right - rect.left == w && rect.bottom - rect.top == h)
            return false;
        else if (refresh)
            c_recalc_layout();
        return true;
    }
    else {
        ShowWindow(su_control_bars[bar], state ? SW_SHOWNOACTIVATE : SW_HIDE);
        c_adjust_container_rect(bar, state, refresh);
        return true;
    }
}

void AMS::Sketchup::c_adjust_container_rect(int bar, bool state, bool refresh) {
    if (RU::su_version < 13) {
        if (refresh) c_recalc_layout();
        return;
    }
    RECT crect;
    RECT arect;
    RECT wrect;
    RECT main_rect;
    RECT cont_rect;
    LONG_PTR style;
    LONG_PTR ex_style;
    HWND handle = su_control_bars[bar];
    // Get main window client rect in screen coordinates.
    GetClientRect(su_main_window, &crect);
    GetClientRect(su_main_window, &arect);
    GetWindowRect(su_main_window, &wrect);
    style = GetWindowLongPtr(su_main_window, GWL_STYLE);
    ex_style = GetWindowLongPtr(su_main_window, GWL_EXSTYLE);
    AdjustWindowRectEx(&arect, (DWORD)style, GetMenu(su_main_window) ? 1 : 0, (DWORD)ex_style);
    main_rect.left = wrect.left - arect.left;
    main_rect.top = wrect.top - arect.top;
    main_rect.right = main_rect.left + crect.right;
    main_rect.bottom = main_rect.top + crect.bottom;
    // Get container window rect in screen coordinates.
    GetWindowRect(handle, &cont_rect);
    // Adjust
    if (state) { // Adjust container
        RECT container_rect;
        container_rect.left = cont_rect.right;
        container_rect.top = cont_rect.bottom;
        container_rect.right = cont_rect.left;
        container_rect.bottom = cont_rect.top;
        EnumChildWindows(handle, ComputeContainerRectProc, reinterpret_cast<LPARAM>(&container_rect));
        if (bar == 0)
            cont_rect.bottom = container_rect.bottom;
        else if (bar == 1)
            cont_rect.top = cont_rect.bottom - (container_rect.bottom - container_rect.top);
        else if (bar == 2)
            cont_rect.right = container_rect.right;
        else if (bar == 3)
            cont_rect.left = cont_rect.right - (container_rect.right - container_rect.left);
    }
    else { // Collapse container
        if (bar == 0)
            cont_rect.bottom = cont_rect.top;
        else if (bar == 1)
            cont_rect.top = cont_rect.bottom;
        else if (bar == 2)
            cont_rect.right = cont_rect.left;
        else if (bar == 3)
            cont_rect.left = cont_rect.right;
    }
    cont_rect.left -= main_rect.left;
    cont_rect.top -= main_rect.top;
    cont_rect.right -= main_rect.left;
    cont_rect.bottom -= main_rect.top;
    MoveWindow(handle, cont_rect.left, cont_rect.top, cont_rect.right - cont_rect.left, cont_rect.bottom - cont_rect.top, 1);
    if (refresh) c_recalc_layout();
}

bool AMS::Sketchup::c_is_toolbar_container_filled(int bar) {
    bool container_filled = false;
    EnumChildWindows(su_control_bars[bar], IsContainerFilledProc, reinterpret_cast<LPARAM>(&container_filled));
    return container_filled;
}

bool AMS::Sketchup::c_is_toolbar_container_empty(int bar) {
    bool container_filled = false;
    EnumChildWindows(su_control_bars[bar], IsContainerFilledProc, reinterpret_cast<LPARAM>(&container_filled));
    return !container_filled;
}

bool AMS::Sketchup::c_is_toolbar_container_visible(int bar) {
    return IsWindowVisible(su_control_bars[bar]) == TRUE;
}

void AMS::Sketchup::c_get_dialogs(std::set<HWND>& items) {
    EnumWindows(GetDialogsProc, reinterpret_cast<LPARAM>(&items));
}

void AMS::Sketchup::c_get_visible_dialogs(std::set<HWND>& items) {
    EnumWindows(GetVisibleDialogsProc, reinterpret_cast<LPARAM>(&items));
}

void AMS::Sketchup::c_get_toolbars(std::set<HWND>& items) {
    EnumWindows(GetToolbarsProc, reinterpret_cast<LPARAM>(&items));
}

void AMS::Sketchup::c_get_visible_toolbars(std::set<HWND>& items) {
    EnumWindows(GetVisibleToolbarsProc, reinterpret_cast<LPARAM>(&items));
}

void AMS::Sketchup::c_get_other_main_windows(std::set<HWND>& items) {
    EnumWindows(GetOtherMainWindowsProc, reinterpret_cast<LPARAM>(&items));
}

bool AMS::Sketchup::c_observers_contain(ID method_id) {
    for (unsigned int i = 0; i < su_observers_size; ++i)
        if (rb_respond_to(rb_ary_entry(su_observers, i), method_id) != 0)
            return true;
    return false;
}

void AMS::Sketchup::c_observers_update_process_info() {
    unsigned int i;

    // Update viewport handle
    c_update_viewport_handle();

    // Reset process variables
    su_process_mw_messages = false;
    su_process_kb_messages = false;

    // Reset window procedures
    if (su_original_viewport_window_proc) {
        if (IsWindow(su_viewport) == TRUE)
            SetWindowLongPtr(su_viewport, GWLP_WNDPROC, (LONG_PTR)su_original_viewport_window_proc);
        su_original_viewport_window_proc = nullptr;
    }
    if (su_original_tab_proc) {
        SetWindowLongPtr(su_tab_control, GWLP_WNDPROC, (LONG_PTR)su_original_tab_proc);
        su_original_tab_proc = nullptr;
    }
    if (su_original_statusbar_proc) {
        SetWindowLongPtr(su_status_bar, GWLP_WNDPROC, (LONG_PTR)su_original_statusbar_proc);
        su_original_statusbar_proc = nullptr;
    }
    if (su_original_scenesbar_proc) {
        SetWindowLongPtr(su_scenes_bar, GWLP_WNDPROC, (LONG_PTR)su_original_scenesbar_proc);
        su_original_scenesbar_proc = nullptr;
    }
    for (i = 0; i < 4; ++i) {
        if (su_original_controlbar_proc[i]) {
            SetWindowLongPtr(su_control_bars[i], GWLP_WNDPROC, (LONG_PTR)su_original_controlbar_proc[i]);
            su_original_controlbar_proc[i] = nullptr;
        }
    }

    // Update process variables and turn on particular procedures
    for (i = 0; i < su_observers_size; ++i) {
        VALUE v_observer = rb_ary_entry(su_observers, i);
        if (su_process_mw_messages == false && (
            rb_respond_to(v_observer, ID_SWP_ON_COMMAND) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_MENU_BAR_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_MENU_BAR_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_RESTORE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_RESTORE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_MINIMIZE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_MINIMIZE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_MAXIMIZE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_MAXIMIZE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_SWITCH_FULL_SCREEN) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_SWITCH_FULL_SCREEN) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_ENTER_MENU) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_ENTER_MENU) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_EXIT_MENU) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_EXIT_MENU) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_CAPTION_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_CAPTION_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_FOCUS) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_FOCUS) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_BLUR) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_BLUR) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_ENTER_SIZE_MOVE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_ENTER_SIZE_MOVE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_EXIT_SIZE_MOVE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_EXIT_SIZE_MOVE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_SIZE_MOVE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_SIZE_MOVE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_USER_MESSAGE) != 0))
        {
            su_process_mw_messages = true;
        }
        if (su_process_kb_messages == false && (
            rb_respond_to(v_observer, ID_SWP_ON_KEY_DOWN) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_KEY_EXTENDED) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_KEY_UP) != 0))
        {
            su_process_kb_messages = true;
        }
        if (su_original_viewport_window_proc == nullptr &&
            IsWindow(su_viewport) == TRUE && (
            rb_respond_to(v_observer, ID_SWP_ON_LBUTTON_DOWN) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_LBUTTON_UP) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_LBUTTON_DOUBLE_CLICK) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_RBUTTON_DOWN) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_RBUTTON_UP) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_RBUTTON_DOUBLE_CLICK) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MBUTTON_DOWN) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MBUTTON_UP) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MBUTTON_DOUBLE_CLICK) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_XBUTTON1_DOWN) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_XBUTTON1_UP) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_XBUTTON1_DOUBLE_CLICK) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_XBUTTON2_DOWN) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_XBUTTON2_UP) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_XBUTTON2_DOUBLE_CLICK) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MOUSE_WHEEL_ROTATE) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MOUSE_WHEEL_TILT) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MOUSE_ENTER) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MOUSE_LEAVE) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_MOUSE_MOVE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_VIEWPORT_BORDER_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_VIEWPORT_BORDER_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_VIEWPORT_SIZE) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_VIEWPORT_SIZE) != 0))
        {
            su_original_viewport_window_proc = (WNDPROC)GetWindowLongPtr(su_viewport, GWLP_WNDPROC);
            SetWindowLongPtr(su_viewport, GWLP_WNDPROC, (LONG_PTR)CustomViewportWindowProc);
        }
        if (su_original_tab_proc == nullptr && (
            rb_respond_to(v_observer, ID_SWO_ON_SCENES_BAR_FILLED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_SCENES_BAR_FILLED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_SCENES_BAR_EMPTIED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_SCENES_BAR_EMPTIED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_PAGE_SELECTED) != 0 ||
            rb_respond_to(v_observer, ID_SWP_ON_PAGE_SELECTED) != 0))
        {
            su_original_tab_proc = (WNDPROC)GetWindowLongPtr(su_tab_control, GWLP_WNDPROC);
            SetWindowLongPtr(su_tab_control, GWLP_WNDPROC, (LONG_PTR)CustomTabProc);
        }
        if (su_original_statusbar_proc == nullptr && (
            rb_respond_to(v_observer, ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED) != 0))
        {
            su_original_statusbar_proc = (WNDPROC)GetWindowLongPtr(su_status_bar, GWLP_WNDPROC);
            SetWindowLongPtr(su_status_bar, GWLP_WNDPROC, (LONG_PTR)CustomStatusBarProc);
        }
        if (su_original_scenesbar_proc == nullptr && (
            rb_respond_to(v_observer, ID_SWO_ON_SCENES_BAR_VISIBILITY_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_SCENES_BAR_VISIBILITY_CHANGED) != 0))
        {
            su_original_scenesbar_proc = (WNDPROC)GetWindowLongPtr(su_scenes_bar, GWLP_WNDPROC);
            SetWindowLongPtr(su_scenes_bar, GWLP_WNDPROC, (LONG_PTR)CustomScenesBarProc);
        }
        if (su_original_controlbar_proc[0] == nullptr && (
            rb_respond_to(v_observer, ID_SWO_ON_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_TOOLBAR_CONTAINER_FILLED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_TOOLBAR_CONTAINER_FILLED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_TOOLBAR_CONTAINER_EMPTIED) != 0 ||
            rb_respond_to(v_observer, ID_SWO_ON_POST_TOOLBAR_CONTAINER_EMPTIED) != 0))
        {
            for (unsigned int j = 0; j < 4; ++j) {
                su_original_controlbar_proc[j] = (WNDPROC)GetWindowLongPtr(su_control_bars[j], GWLP_WNDPROC);
                SetWindowLongPtr(su_control_bars[j], GWLP_WNDPROC, (LONG_PTR)CustomControlBarProc);
                su_tbc_filled[j] = c_is_toolbar_container_filled(j);
                su_tbc_visible[j] = c_is_toolbar_container_visible(j);
            }
        }
    }
}

VALUE AMS::Sketchup::c_observer_call_proc(VALUE v_event_data) {
    EventData* event_data = reinterpret_cast<EventData*>(v_event_data);
    VALUE v_res = rb_funcall2(event_data->observer, event_data->id, event_data->size, event_data->params);
    return v_res == RU::to_value(1) ? Qfalse : Qtrue;
}

VALUE AMS::Sketchup::c_observer_rescue_proc(VALUE v_event_data, VALUE v_exception) {
    EventData* event_data = reinterpret_cast<EventData*>(v_event_data);
    if (event_data->id != ID_SWO_ERROR && rb_respond_to(event_data->observer, ID_SWO_ERROR) == 1)
        c_call_exception_event(event_data->observer, v_exception);
    else {
        VALUE v_message = rb_funcall(v_exception, RU::INTERN_MESSAGE, 0);
        rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, v_message);
        VALUE v_backtrace = rb_funcall(v_exception, RU::INTERN_BACKTRACE, 0);
        rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, rb_ary_entry(v_backtrace, 0));
    }
    return Qtrue;
}

bool AMS::Sketchup::c_call_observer_event(ID method_id, int param_count, VALUE* params, bool block_allowed) {
    if (!c_observers_contain(method_id)) return true;
    EventData event_data;
    event_data.id = method_id;
    event_data.params = new VALUE[param_count];
    for (int x = 0; x < param_count; ++x)
        event_data.params[x] = TYPE(params[x]) == T_STRING ? rb_str_dup(params[x]) : params[x];
    event_data.size = param_count;
    VALUE t_su_observers = rb_ary_dup(su_observers);
    unsigned int t_su_observers_size = su_observers_size;
    bool process_message = true;
    for (unsigned int i = 0; i < t_su_observers_size; ++i) {
        VALUE v_observer = rb_ary_entry(t_su_observers, i);
        if (rb_ary_includes(su_observers, v_observer) == Qfalse || rb_respond_to(v_observer, method_id) == 0) continue;
        event_data.observer = v_observer;
        for (int x = 0; x < param_count; ++x)
            if (TYPE(params[x]) == T_STRING)
                event_data.params[x] = rb_str_dup(params[x]);
        VALUE v_res = rb_rescue2(RUBY_METHOD_FUNC(c_observer_call_proc), reinterpret_cast<VALUE>(&event_data), RUBY_METHOD_FUNC(c_observer_rescue_proc), reinterpret_cast<VALUE>(&event_data), rb_eException, 0);
        if (v_res == Qfalse && block_allowed == true) process_message = false;
    }
    rb_ary_clear(t_su_observers);
    delete[] event_data.params;
    return process_message;
}

bool AMS::Sketchup::c_call_single_observer_event(VALUE v_observer, ID method_id, int param_count, VALUE* params, bool block_allowed) {
    if (rb_respond_to(v_observer, method_id) == 0) return true;
    EventData event_data;
    event_data.observer = v_observer;
    event_data.id = method_id;
    event_data.params = new VALUE[param_count];
    for (int x = 0; x < param_count; ++x)
        event_data.params[x] = TYPE(params[x]) == T_STRING ? rb_str_dup(params[x]) : params[x];
    event_data.size = param_count;
    VALUE v_res = rb_rescue2(RUBY_METHOD_FUNC(c_observer_call_proc), reinterpret_cast<VALUE>(&event_data), RUBY_METHOD_FUNC(c_observer_rescue_proc), reinterpret_cast<VALUE>(&event_data), rb_eException, 0);
    bool process_message = (v_res == Qfalse && block_allowed == true) ? true : false;
    delete[] event_data.params;
    return process_message;
}

VALUE AMS::Sketchup::c_exception_call_proc(VALUE v_exception_data) {
    ExceptionData* exception_data = reinterpret_cast<ExceptionData*>(v_exception_data);
    rb_funcall(exception_data->observer, ID_SWO_ERROR, 1, exception_data->exception);
    return Qtrue;
}

VALUE AMS::Sketchup::c_exception_rescue_proc(VALUE v_observer, VALUE v_exception) {
    VALUE v_message = rb_funcall(v_exception, RU::INTERN_MESSAGE, 0);
    rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, v_message);
    VALUE v_backtrace = rb_funcall(v_exception, RU::INTERN_BACKTRACE, 0);
    rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, rb_ary_entry(v_backtrace, 0));
    return Qtrue;
}

bool AMS::Sketchup::c_call_exception_event(VALUE v_observer, VALUE v_exception) {
    ExceptionData exception_data;
    exception_data.observer = v_observer;
    exception_data.exception = v_exception;
    rb_rescue2(RUBY_METHOD_FUNC(c_exception_call_proc), (VALUE)&exception_data, RUBY_METHOD_FUNC(c_exception_rescue_proc), (VALUE)&exception_data, rb_eException, (VALUE)0);
    return true;
}

void AMS::Sketchup::c_preset_status_bar_visible()
{
    if (RU::su_version > 8) {
        HKEY hkey;
        wchar_t path[256];
        DWORD value = 1;
        wsprintf(path, L"Software\\SketchUp\\SketchUp 20%d\\Workspace\\BasePane-59393", RU::su_version);
        RegCreateKey(HKEY_CURRENT_USER, path, &hkey);
        RegSetValueEx(hkey, L"IsVisible", 0, REG_DWORD, reinterpret_cast<BYTE*>(&value), sizeof(value));
        RegCloseKey(hkey);
    }
    else {
        int count = 0;
        while (count < 100) {
            HKEY hkey;
            TCHAR path[256];
            wsprintf(path, L"Software\\Google\\SketchUp%d\\ToolbarsUser-Bar%d", RU::su_version, count);
            if (RegOpenKey(HKEY_CURRENT_USER, path, &hkey) == ERROR_SUCCESS) {
                DWORD type = REG_DWORD;
                DWORD id;
                DWORD size = sizeof(id);
                if (RegQueryValueEx(hkey, L"BarID", NULL, (LPDWORD)&type, (LPBYTE)&id, (LPDWORD)&size) == ERROR_SUCCESS && id == 59393) {
                    DWORD value = 1;
                    RegSetValueEx(hkey, L"Visible", 0, REG_DWORD, reinterpret_cast<BYTE*>(&value), sizeof(value));
                    RegCloseKey(hkey);
                    break;
                }
                RegCloseKey(hkey);
            }
            ++count;
        }
    }
}

void AMS::Sketchup::c_find_accelerators() {
    for (std::map<KeyboardShortcut*, unsigned long long, CompareKeyboardShortcuts>::iterator it = su_accelerators.begin(); it != su_accelerators.end(); ++it) {
        KeyboardShortcut* shortcut = it->first;
        delete shortcut;
    }
    su_accelerators.clear();
    VALUE v_args[1] = { rb_str_new2("Localizable.strings") };
    VALUE v_lang_handler_inst = rb_class_new_instance(1, v_args, RU::SU_LANGUAGE_HANDLER);
    VALUE v_locale_strings = rb_ivar_get(v_lang_handler_inst, rb_intern("@strings"));
    std::map<const wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2> commands;
    c_get_menu_commands(su_menu_bar, nullptr, 0, commands);
    VALUE v_shortcuts = rb_funcall(RU::SU_SKETCHUP, rb_intern("get_shortcuts"), 0);
    int len = RARRAY_LEN(v_shortcuts);
    for (int i = 0; i < len; ++i) {
        VALUE v_data = rb_ary_entry(v_shortcuts, i);
        const wchar_t* data = RU::value_to_wc_str(v_data);
        // Split data into accelerator and menu path.
        unsigned int data_len = (unsigned int)wcslen(data);
        bool found = false;
        unsigned int x = 0;
        for (x = 0; x < data_len; ++x)
            if (data[x] == L'\t') {
                found = true;
                break;
            }
        if (!found) {
            delete[] data;
            continue;
        }
        wchar_t* shortcut = new wchar_t[x+1];
        wchar_t* menu_path = new wchar_t[data_len-x];
        wmemcpy(shortcut, data, x);
        wmemcpy(menu_path, data+x+1, data_len-x-1);
        delete[] data;
        shortcut[x] = L'\0';
        menu_path[data_len-x-1] = L'\0';
        // Get menu ID associated with menu path.
        std::map<const wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2>::iterator it = commands.find(menu_path);
        delete[] menu_path;
        if (it == commands.end()) {
            delete[] shortcut;
            continue;
        }
        unsigned long long id = it->second;
        // Get shortcut keys and translate them to English.
        KeyboardShortcut* accelerator = new KeyboardShortcut();
        unsigned int shortcut_len = x;
        unsigned int last_index = 0;
        for (x = 0; x < shortcut_len; ++x) {
            if (shortcut[x] == L'+' || x == shortcut_len - 1) {
                unsigned int count = x - last_index;
                if (x == shortcut_len - 1) ++count;
                VALUE v_vk_name = RU::win_wc_str_to_value(shortcut + last_index, count);
                ID func = RU::su_version > 13 ? RU::INTERN_KEY : RU::INTERN_INDEX;
                VALUE v_vk_name_en = rb_funcall(v_locale_strings, func, 1, v_vk_name);
                if (v_vk_name_en != Qnil) v_vk_name = v_vk_name_en;
                wchar_t* vk_name = RU::value_to_wc_str(v_vk_name);
                _wcslwr(vk_name);
                if (wcscmp(vk_name, L"ctrl") == 0)
                    accelerator->control_down = true;
                else if (wcscmp(vk_name, L"alt") == 0)
                    accelerator->menu_down = true;
                else if (wcscmp(vk_name, L"shift") == 0)
                    accelerator->shift_down = true;
                else
                    accelerator->virtual_key = c_vk_name_to_code(vk_name);
                delete[] vk_name;
                last_index = x + 1;
            }
        }
        delete[] shortcut;
        su_accelerators[accelerator] = id;
    }
    for (std::map<const wchar_t*, unsigned long long, AMS::Sketchup::CompareCString2>::iterator it = commands.begin(); it != commands.end(); ++it)
        delete[] it->first;
    commands.clear();
}

bool AMS::Sketchup::c_call_accelerator(KeyboardShortcut* accelerator) {
    std::map<KeyboardShortcut*, unsigned long long, CompareKeyboardShortcuts>::iterator it = su_accelerators.find(accelerator);
    if (it == su_accelerators.end())
        return false;
    su_expected_command = it->second;
    su_triggered_command = 0;
    //SetTimer(su_main_window, ACCELERATOR_TIMER_ID, USER_TIMER_MINIMUM, AcceleratorTimerProc);
    SetTimer(NULL, 0, USER_TIMER_MINIMUM, AcceleratorTimerProc);
    return true;
}

void AMS::Sketchup::c_verify_accelerator(WORD id) {
    if (su_triggered_command == 0) su_triggered_command = id;
}

bool AMS::Sketchup::c_is_main_window_style_maximized() {
    LONG_PTR style = GetWindowLongPtr(su_main_window, GWL_STYLE);
    return (style | WS_MAXIMIZE) == style;
}

unsigned int AMS::Sketchup::c_show_trays(bool state) {
    if (RU::su_version < 16)
        return 0;

    unsigned int fcount = 0;
    unsigned int dcount = 0;
    unsigned int ecount = 0;

    if (state) {
        for (std::set<HWND>::iterator it = su_active_trays.begin(); it != su_active_trays.end(); ++it) {
            if (IsWindow(*it) == FALSE || IsWindowVisible(*it) == TRUE) continue;
            ShowWindow(*it, SW_SHOWNA);
            HWND parent = GetAncestor(*it, GA_PARENT);
            if (parent == su_main_window)
                ++dcount;
            else {
                ShowWindow(parent, SW_SHOWNA);
                ++fcount;
            }
        }
        su_active_trays.clear();

        for (int i = 0; i < 4; ++i) {
            if (su_tray_tab_size[i] == 0) continue;
            HWND handle = su_tray_dock_panes[i];
            if (GetWindow(handle, GW_CHILD) == NULL) continue;
            RECT rect;
            GetWindowRect(handle, &rect);
            int x = rect.left;
            int y = rect.top;
            int w = rect.right - rect.left;
            int h = rect.bottom - rect.top;
            if (i == 0)
                h = su_tray_tab_size[i];
            else if (i == 1) {
                h = su_tray_tab_size[i];
                y = rect.bottom - h;
            }
            else if (i == 2)
                w = su_tray_tab_size[i];
            else if (i == 3) {
                w = su_tray_tab_size[i];
                x = rect.right - w;
            }
            RECT crect;
            GetClientRect(su_main_window, &crect);
            ClientToScreen(su_main_window, reinterpret_cast<LPPOINT>(&crect));
            x -= crect.left;
            y -= crect.top;
            SetWindowPos(handle, NULL, x, y, w, h, SWP_DRAWFRAME | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOSENDCHANGING);
            ++ecount;
        }

        for (std::set<HWND>::iterator it = su_active_sliders.begin(); it != su_active_sliders.end(); ++it) {
            ShowWindow(*it, SW_SHOWNA);
            ++ecount;
        }
        su_active_sliders.clear();
    }
    else {
        std::set<HWND> visible_trays;
        EnumWindows(GetVisibleFloatingTraysProc, reinterpret_cast<LPARAM>(&visible_trays));
        EnumChildWindows(su_main_window, GetVisibleDockedTraysProc, reinterpret_cast<LPARAM>(&visible_trays));

        for (std::set<HWND>::iterator it = visible_trays.begin(); it != visible_trays.end(); ++it) {
            ShowWindow(*it, SW_HIDE);
            HWND parent = GetAncestor(*it, GA_PARENT);
            if (parent == su_main_window)
                ++dcount;
            else {
                ShowWindow(parent, SW_HIDE);
                ++fcount;
            }
            su_active_trays.insert(*it);
        }
        visible_trays.clear();

        for (int i = 0; i < 4; ++i) {
            HWND handle = su_tray_dock_panes[i];
            if (GetWindow(handle, GW_CHILD) == NULL) continue;
            RECT rect;
            GetWindowRect(handle, &rect);
            int x = rect.left;
            int y = rect.top;
            int w = rect.right - rect.left;
            int h = rect.bottom - rect.top;
            if (w == 0 || h == 0) continue;
            if (i == 0) {
                su_tray_tab_size[i] = h;
                h = 0;
            }
            else if (i == 1) {
                su_tray_tab_size[i] = h;
                y += h;
                h = 0;
            }
            else if (i == 2) {
                su_tray_tab_size[i] = w;
                w = 0;
            }
            else if (i == 3) {
                su_tray_tab_size[i] = w;
                x += w;
                w = 0;
            }
            RECT crect;
            GetClientRect(su_main_window, &crect);
            ClientToScreen(su_main_window, (LPPOINT)&crect);
            x -= crect.left;
            y -= crect.top;
            SetWindowPos(handle, NULL, x, y, w, h, SWP_DRAWFRAME | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOSENDCHANGING);
            ++ecount;
        }

        std::set<HWND> visible_sliders;
        EnumChildWindows(su_main_window, GetVisibleTraySlidersProc, reinterpret_cast<LPARAM>(&visible_sliders));
        for (std::set<HWND>::iterator it = visible_sliders.begin(); it != visible_sliders.end(); ++it) {
            ShowWindow(*it, SW_HIDE);
            su_active_sliders.insert(*it);
            ++ecount;
        }
    }

    if (dcount + ecount > 0) c_recalc_layout();

    return fcount + dcount;
}

unsigned int AMS::Sketchup::c_close_trays() {
    if (RU::su_version < 16)
        return 0;

    unsigned int fcount = 0;
    unsigned int dcount = 0;
    unsigned int ecount = 0;
    std::set<HWND> trays;

    EnumWindows(GetVisibleFloatingTraysProc, reinterpret_cast<LPARAM>(&trays));
    EnumChildWindows(su_main_window, GetVisibleDockedTraysProc, reinterpret_cast<LPARAM>(&trays));

    for (std::set<HWND>::iterator it = su_active_trays.begin(); it != su_active_trays.end(); ++it) {
        if (IsWindow(*it) == TRUE)
            trays.insert(*it);
    }
    su_active_trays.clear();

    for (std::set<HWND>::iterator it = trays.begin(); it != trays.end(); ++it) {
        HWND parent = GetAncestor(*it, GA_PARENT);
        if (parent == su_main_window) {
            ShowWindow(*it, SW_HIDE);
            ++dcount;
        }
        else {
            SendMessage(parent, WM_SYSCOMMAND, SC_CLOSE, 0);
            ++fcount;
        }
    }

    for (int i = 0; i < 4; ++i) {
        HWND handle = su_tray_dock_panes[i];
        if (GetWindow(handle, GW_CHILD) == NULL) continue;
        RECT rect;
        GetWindowRect(handle, &rect);
        int x = rect.left;
        int y = rect.top;
        int w = rect.right - rect.left;
        int h = rect.bottom - rect.top;
        if (w == 0 || h == 0) continue;
        if (i == 0) {
            su_tray_tab_size[i] = h;
            h = 0;
        }
        else if (i == 1) {
            su_tray_tab_size[i] = h;
            y += h;
            h = 0;
        }
        else if (i == 2) {
            su_tray_tab_size[i] = w;
            w = 0;
        }
        else if (i == 3) {
            su_tray_tab_size[i] = w;
            x += w;
            w = 0;
        }
        RECT crect;
        GetClientRect(su_main_window, &crect);
        ClientToScreen(su_main_window, reinterpret_cast<LPPOINT>(&crect));
        x -= crect.left;
        y -= crect.top;
        SetWindowPos(handle, NULL, x, y, w, h, SWP_DRAWFRAME | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_HIDEWINDOW | SWP_NOACTIVATE);
        ++ecount;
    }

    std::set<HWND> visible_sliders;
    EnumChildWindows(su_main_window, GetVisibleTraySlidersProc, reinterpret_cast<LPARAM>(&visible_sliders));
    for (std::set<HWND>::iterator it = visible_sliders.begin(); it != visible_sliders.end(); ++it) {
        ShowWindow(*it, SW_HIDE);
        ++ecount;
    }
    su_active_sliders.clear();

    if (dcount + ecount > 0)
        c_recalc_layout();

    return fcount + dcount;
}

void AMS::Sketchup::c_get_hidden_tabbed_trays(std::set<HWND>& items) {
    std::set<HWND> trays;
    EnumChildWindows(su_main_window, GetDockedTraysProc, reinterpret_cast<LPARAM>(&trays));
    for (std::set<HWND>::iterator it = trays.begin(); it != trays.end(); ++it) {
        if (IsWindowVisible(*it) == TRUE) continue;
        RECT drect;
        GetWindowRect(*it, &drect);
        for (int i = 0; i < 4; ++i) {
            HWND handle = su_tray_dock_panes[i];
            if (GetWindow(handle, GW_CHILD) == NULL) continue;
            RECT trect;
            GetWindowRect(handle, &trect);
            if (trect.right - trect.left == 0 || trect.bottom - trect.top == 0) continue;
            if (i == 0) {
                if (drect.bottom == trect.bottom && drect.left == trect.left) { //&& drect.right - drect.left == trect.right - trect.left) {
                    items.insert(*it);
                    break;
                }
            }
            else if (i == 1) {
                if (drect.top == trect.top && drect.left == trect.left) { //&& drect.right - drect.left == trect.right - trect.left) {
                    items.insert(*it);
                    break;
                }
            }
            else if (i == 2) {
                if (drect.right == trect.right && drect.top == trect.top) { //&& drect.bottom - drect.top == trect.bottom - trect.top) {
                    items.insert(*it);
                    break;
                }
            }
            else if (i == 3) {
                if (drect.left == trect.left && drect.top == trect.top) { //&& drect.bottom - drect.top == trect.bottom - trect.top) {
                    items.insert(*it);
                    break;
                }
            }
        }
    }
}

bool AMS::Sketchup::c_recalc_layout() {
    BOOL original_state = IsWindowEnabled(su_main_window);
    EnableWindow(su_main_window, FALSE);
    bool res = SendMessage(su_main_window, WM_SIZE, 0, 0) == 0;
    EnableWindow(su_main_window, original_state);
    RedrawWindow(su_main_window, NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
    return res;
}

void AMS::Sketchup::c_init() {
    unsigned int i;

    // Preset global variables
    su_menu_bar_set = true;
    if (c_is_main_window_full_screen())
        su_mw_show_state = 3;
    else if (IsZoomed(su_main_window) == TRUE)
        su_mw_show_state = 2;
    else if (IsIconic(su_main_window) == TRUE)
        su_mw_show_state = 1;
    else
        su_mw_show_state = 0;

    su_expected_command = 0;
    su_triggered_command = 0;
    su_vp_mouse_leave = true;
    su_vp_cursor_pos[0] = 0;
    su_vp_cursor_pos[1] = 0;
    su_f1_state = -1;
    su_lwin_state = -1;
    su_rwin_state = -1;
    su_mw_style = 0x14cfc000;
    su_mw_style_ex = WS_EX_WINDOWEDGE;

    su_process_mw_messages = false;
    su_process_kb_messages = false;
    su_process_sb_page_selection = true;

    for (i = 0; i < 4; ++i)
        su_tray_tab_size[i] = 0;

    if (RU::su_version < 16) {
        // Store all SketchUp application controlled dialogs
        su_dialog_en_names.push_back(L"Model Info");
        su_dialog_en_names.push_back(L"Entity Info");
        su_dialog_en_names.push_back(L"Materials");
        su_dialog_en_names.push_back(L"Components");
        su_dialog_en_names.push_back(L"Styles");
        su_dialog_en_names.push_back(L"Layers");
        su_dialog_en_names.push_back(L"Outliner");
        su_dialog_en_names.push_back(L"Scenes");
        su_dialog_en_names.push_back(L"Shadow Settings");
        su_dialog_en_names.push_back(L"Fog");
        su_dialog_en_names.push_back(L"Match Photo");
        su_dialog_en_names.push_back(L"Soften Edges");
        su_dialog_en_names.push_back(L"Instructor");
        // Convert SU controlled dialogs to the currently used language
        VALUE v_args[1] = { rb_str_new2("Localizable.strings") };
        VALUE v_lang_handler_inst = rb_class_new_instance(1, v_args, RU::SU_LANGUAGE_HANDLER);
        VALUE v_locale_strings = rb_ivar_get(v_lang_handler_inst, rb_intern("@strings"));
        for (std::vector<const wchar_t*>::iterator it = su_dialog_en_names.begin(); it != su_dialog_en_names.end(); ++it) {
            const wchar_t* lname = *it;
            VALUE v_lname = rb_hash_aref(v_locale_strings, RU::to_value(lname));
            if (v_lname == Qnil) {
                wchar_t* lname2 = new wchar_t[wcslen(lname) + 1];
                wcscpy(lname2, lname);
                su_controlled_dialogs.push_back(lname2);
            }
            else {
                wchar_t* lname2 = RU::value_to_wc_str(v_lname);
                su_controlled_dialogs.push_back(lname2);
            }
        }
    }

    // Update viewport handle
    c_update_viewport_handle();

    // Store reference to original main window procedure
    su_original_main_window_proc = (WNDPROC)GetWindowLongPtr(su_main_window, GWLP_WNDPROC);

    // All others references are null until they are activated
    su_original_viewport_window_proc = nullptr;
    su_original_tab_proc = nullptr;
    su_original_statusbar_proc = nullptr;
    su_original_scenesbar_proc = nullptr;
    for (i = 0; i < 4; ++i)
        su_original_controlbar_proc[i] = nullptr;

    // Activate new main window procedure
    SetWindowLongPtr(su_main_window, GWLP_WNDPROC, (LONG_PTR)CustomMainWindowProc);

    /*su_original_tab_proc = (WNDPROC)GetWindowLongPtr(su_tab_control, GWLP_WNDPROC);
    SetWindowLongPtr(su_tab_control, GWLP_WNDPROC, (LONG_PTR)CustomTabProc);

    su_original_statusbar_proc = (WNDPROC)GetWindowLongPtr(su_status_bar, GWLP_WNDPROC);
    SetWindowLongPtr(su_status_bar, GWLP_WNDPROC, (LONG_PTR)CustomStatusBarProc);

    su_original_scenesbar_proc = (WNDPROC)GetWindowLongPtr(su_scenes_bar, GWLP_WNDPROC);
    SetWindowLongPtr(su_scenes_bar, GWLP_WNDPROC, (LONG_PTR)CustomScenesBarProc);

    for (unsigned int j = 0; j < 4; ++j) {
        su_original_controlbar_proc[j] = (WNDPROC)GetWindowLongPtr(su_control_bars[j], GWLP_WNDPROC);
        SetWindowLongPtr(su_control_bars[j], GWLP_WNDPROC, (LONG_PTR)CustomControlBarProc);
        su_tbc_filled[j] = c_is_toolbar_container_filled(j);
        su_tbc_visible[j] = c_is_toolbar_container_visible(j);
    }*/

    // Activate window hooks
    su_hooks[0] = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, su_module_handle, 0);
    su_hooks[1] = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, su_thread_id);

    // Instantiate observers
    su_observers = rb_ary_new();
    su_observers_size = 0;
    rb_gc_register_address(&su_observers);

    su_messagebox_procedures = rb_ary_new();
    rb_gc_register_address(&su_messagebox_procedures);

    InitializeCriticalSection(&su_mb_critical_section);
}

void AMS::Sketchup::c_uninit() {
    unsigned int i;

    DeleteCriticalSection(&su_mb_critical_section);

    // Call closing event
    c_call_observer_event(ID_SWO_ON_QUIT, 0, nullptr);

    // Clear observer data
    rb_ary_clear(su_observers);
    rb_gc_unregister_address(&su_observers);

    rb_ary_clear(su_messagebox_procedures);
    rb_gc_unregister_address(&su_messagebox_procedures);

    // Deactivate new window procedures
    if (su_original_main_window_proc) {
        SetWindowLongPtr(su_main_window, GWLP_WNDPROC, (LONG_PTR)su_original_main_window_proc);
        su_original_main_window_proc = nullptr;
    }
    if (su_original_viewport_window_proc) {
        if (IsWindow(su_viewport) == TRUE)
            SetWindowLongPtr(su_viewport, GWLP_WNDPROC, (LONG_PTR)su_original_viewport_window_proc);
        su_original_viewport_window_proc = nullptr;
    }
    if (su_original_tab_proc) {
        SetWindowLongPtr(su_tab_control, GWLP_WNDPROC, (LONG_PTR)su_original_tab_proc);
        su_original_tab_proc = nullptr;
    }
    if (su_original_statusbar_proc) {
        SetWindowLongPtr(su_status_bar, GWLP_WNDPROC, (LONG_PTR)su_original_statusbar_proc);
        su_original_statusbar_proc = nullptr;
    }
    if (su_original_scenesbar_proc) {
        SetWindowLongPtr(su_scenes_bar, GWLP_WNDPROC, (LONG_PTR)su_original_scenesbar_proc);
        su_original_scenesbar_proc = nullptr;
    }
    for (i = 0; i < 4; ++i) {
        if (su_original_controlbar_proc[i]) {
            SetWindowLongPtr(su_control_bars[i], GWLP_WNDPROC, (LONG_PTR)su_original_controlbar_proc[i]);
            su_original_controlbar_proc[i] = nullptr;
        }
    }

    // Deactivate window hooks
    if (su_hooks[0]) {
        UnhookWindowsHookEx(su_hooks[0]);
        su_hooks[0] = nullptr;
    }
    if (su_hooks[1]) {
        UnhookWindowsHookEx(su_hooks[1]);
        su_hooks[1] = nullptr;
    }

    // Free su_controlled_dialogs
    for (std::vector<const wchar_t*>::iterator it = su_dialog_en_names.begin(); it != su_dialog_en_names.end(); ++it) {
        const wchar_t* lname = *it;
        delete[] lname;
    }

    // Free accelerators
    for (std::map<KeyboardShortcut*, unsigned long long, CompareKeyboardShortcuts>::iterator it = su_accelerators.begin(); it != su_accelerators.end(); ++it) {
        KeyboardShortcut* shortcut = it->first;
        delete shortcut;
    }
    su_accelerators.clear();

    // Ensure that status bar is visible at exit, so that it remains visible when
    // SU is launched again.
    c_preset_status_bar_visible();
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Callback Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

BOOL CALLBACK AMS::Sketchup::FindMainWindowProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    DWORD tid = GetWindowThreadProcessId(handle, &pid);
    if (pid == static_cast<DWORD>(lParam)) {
        HWND owner = GetWindow(handle, GW_OWNER);
        HWND potential_main_window = (owner != NULL) ? owner : handle;
        TCHAR cname[64];
        GetClassName(potential_main_window, cname, 64);
        if (StrCmpN(cname, TEXT("Afx:"), 4) == 0 && StrStr(cname, TEXT(":b:")) != NULL) {
            su_main_window = potential_main_window;
            su_thread_id = tid;
            return FALSE;
        }
        else
            return TRUE;
    }
    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::FindChildWindowsProc(HWND handle, LPARAM lParam) {
    if (GetAncestor(handle, GA_PARENT) != su_main_window) return TRUE;
    TCHAR cname[60];
    GetClassName(handle, cname, 60);
    if (RU::su_version < 13) {
        if (su_dock_pane_count < 4 && (StrCmpN(cname, TEXT("AfxControlBar70u"), 16) == 0 || StrCmpN(cname, TEXT("AfxControlBar80u"), 16) == 0)) {
            su_control_bars[su_dock_pane_count] = handle;
            ++su_dock_pane_count;
        }
        else if (StrCmpN(cname, TEXT("msctls_statusbar32"), 18) == 0) {
            HWND tab_handle = FindWindowEx(handle, NULL, TEXT("SysTabControl32"), NULL);
            if (tab_handle) {
                su_scenes_bar = handle;
                su_tab_control = tab_handle;
            }
            else
                su_status_bar = handle;
        }
    }
    else if (StrCmpN(cname, TEXT("Afx:DockPane"), 12) == 0) {
        if (su_dock_pane_count < 4)
            su_control_bars[su_dock_pane_count] = handle;
        else if (su_dock_pane_count < 8)
            su_tray_dock_panes[su_dock_pane_count - 4] = handle;
        ++su_dock_pane_count;
    }
    else if (StrCmpN(cname, TEXT("Afx:RibbonStatusBar"), 19) == 0)
        su_status_bar = handle;
    else if (StrCmpN(cname, TEXT("msctls_statusbar32"), 18) == 0) {
        su_scenes_bar = handle;
        su_tab_control = FindWindowEx(handle, NULL, TEXT("SysTabControl32"), NULL);
    }
    return TRUE;
}

LRESULT CALLBACK AMS::Sketchup::CustomMainWindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
    bool process_message = true;

    switch(msg) {
        case WM_PARENTNOTIFY: {
            if (LOWORD(wParam) == WM_CREATE) {
                // When user changes preferences, old viewport is destroyed and new one
                // is created. The new viewport window starts off using original window
                // procedure. We implement this message to ensure that new viewport uses
                // our custom viewport window procedure.
                wchar_t cname[20];
                GetClassName((HWND)lParam, cname, 20);
                if (StrStr(cname, L"AfxFrameOrView") != NULL) {
                    su_viewport = (HWND)lParam;
                    if (su_original_viewport_window_proc) {
                        su_original_viewport_window_proc = (WNDPROC)GetWindowLongPtr(su_viewport, GWLP_WNDPROC);
                        SetWindowLongPtr(su_viewport, GWLP_WNDPROC, (LONG_PTR)CustomViewportWindowProc);
                    }
                }
            }
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) != 0x5e96) {
                // Process accelerators when menu bar is removed.
                if (GetMenu(su_main_window) != su_menu_bar)
                    c_verify_accelerator(LOWORD(wParam));
                if (su_process_mw_messages) {
                    VALUE params[1] = { RU::to_value(LOWORD(wParam)) };
                    process_message = c_call_observer_event(ID_SWP_ON_COMMAND, 1, params, true);
                }
            }
            break;
        }
        case WM_WINDOWPOSCHANGED: {
            if (GetMenu(handle) == NULL && su_menu_bar_set == true) {
                su_menu_bar_set = false;
                c_find_accelerators();
                if (su_process_mw_messages) {
                    VALUE params[1] = { Qfalse };
                    c_call_observer_event(ID_SWO_ON_MENU_BAR_CHANGED, 1, params);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_MENU_BAR_CHANGED;
                        data->size = 1;
                        data->params = new VALUE[1];
                        data->params[0] = params[0];
                        su_post_event_data[timer_id] = data;
                    }
                }
            }
            else if (GetMenu(handle) != NULL && su_menu_bar_set == false) {
                su_menu_bar_set = true;
                if (su_process_mw_messages) {
                    VALUE params[1] = { Qtrue };
                    c_call_observer_event(ID_SWO_ON_MENU_BAR_CHANGED, 1, params);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_MENU_BAR_CHANGED;
                        data->size = 1;
                        data->params = new VALUE[1];
                        data->params[0] = params[0];
                        su_post_event_data[timer_id] = data;
                    }
                }
            }
            break;
        }
        case WM_SIZE: {
            if (wParam == SIZE_RESTORED && su_mw_show_state != 0 && !c_is_main_window_full_screen() && !c_is_main_window_style_maximized()) {
                su_mw_show_state = 0;
                if (su_process_mw_messages) {
                    c_call_observer_event(ID_SWO_ON_RESTORE, 0, nullptr);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_RESTORE;
                        data->size = 0;
                        su_post_event_data[timer_id] = data;
                    }
                }
            }
            else if (wParam == SIZE_MINIMIZED && su_mw_show_state != 1) {
                su_mw_show_state = 1;
                if (su_process_mw_messages) {
                    c_call_observer_event(ID_SWO_ON_MINIMIZE, 0, nullptr);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_MINIMIZE;
                        data->size = 0;
                        su_post_event_data[timer_id] = data;
                    }
                }
            }
            else if (wParam == SIZE_MAXIMIZED && su_mw_show_state != 2 && !c_is_main_window_full_screen()) {
                su_mw_show_state = 2;
                if (su_process_mw_messages) {
                    c_call_observer_event(ID_SWO_ON_MAXIMIZE, 0, nullptr);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_MAXIMIZE;
                        data->size = 0;
                        su_post_event_data[timer_id] = data;
                    }
                }
            }
            break;
        }
        case WM_STYLECHANGED: {
            if (wParam != 20) {
                STYLESTRUCT* style = reinterpret_cast<STYLESTRUCT*>(lParam);
                if ((style->styleOld | WS_CAPTION) == style->styleOld && (style->styleNew | WS_CAPTION) != style->styleNew) {
                    su_mw_show_state = 3;
                    if (su_process_mw_messages) {
                        VALUE params[1] = { Qtrue };
                        c_call_observer_event(ID_SWO_ON_SWITCH_FULL_SCREEN, 1, params);
                        UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                        if (timer_id != 0) {
                            PostEventData* data = new PostEventData;
                            data->id = ID_SWO_ON_POST_SWITCH_FULL_SCREEN;
                            data->size = 1;
                            data->params = new VALUE[1];
                            data->params[0] = params[0];
                            su_post_event_data[timer_id] = data;
                        }
                    }
                }
                else if ((style->styleOld | WS_CAPTION) != style->styleOld && (style->styleNew | WS_CAPTION) == style->styleNew) {
                    if (su_process_mw_messages) {
                        VALUE params[1] = { Qfalse };
                        c_call_observer_event(ID_SWO_ON_SWITCH_FULL_SCREEN, 1, params);
                        UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                        if (timer_id != 0) {
                            PostEventData* data = new PostEventData;
                            data->id = ID_SWO_ON_POST_SWITCH_FULL_SCREEN;
                            data->size = 1;
                            data->params = new VALUE[1];
                            data->params[0] = params[0];
                            su_post_event_data[timer_id] = data;
                        }
                    }
                }
            }
            break;
        }
        case WM_CLOSE: {
            SetMenu(su_main_window, su_menu_bar);
            break;
        }
        case WM_DESTROY: {
            c_uninit();
            break;
        }
        case WM_ENTERMENULOOP: {
            if (su_process_mw_messages) {
                c_call_observer_event(ID_SWO_ON_ENTER_MENU, 0, nullptr);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_ENTER_MENU;
                    data->size = 0;
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_EXITMENULOOP: {
            if (su_process_mw_messages) {
                c_call_observer_event(ID_SWO_ON_EXIT_MENU, 0, nullptr);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_EXIT_MENU;
                    data->size = 0;
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_SETTEXT: {
            if (su_process_mw_messages) {
                VALUE params[1] = { RU::to_value(reinterpret_cast<wchar_t*>(lParam)) };
                c_call_observer_event(ID_SWO_ON_CAPTION_CHANGED, 1, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_CAPTION_CHANGED;
                    data->size = 1;
                    data->params = new VALUE[1];
                    data->params[0] = params[0];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_ACTIVATE: {
            if (su_process_mw_messages) {
                c_call_observer_event(wParam == WA_INACTIVE ? ID_SWO_ON_BLUR : ID_SWO_ON_FOCUS, 0, nullptr);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = wParam == WA_INACTIVE ? ID_SWO_ON_POST_BLUR : ID_SWO_ON_POST_FOCUS;
                    data->size = 0;
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_ENTERSIZEMOVE: {
            if (su_process_mw_messages) {
                VALUE params[4];
                RECT rect;
                GetWindowRect(handle, &rect);
                params[0] = RU::to_value(rect.left);
                params[1] = RU::to_value(rect.top);
                params[2] = RU::to_value(rect.right - rect.left);
                params[3] = RU::to_value(rect.bottom - rect.top);
                c_call_observer_event(ID_SWO_ON_ENTER_SIZE_MOVE, 4, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_ENTER_SIZE_MOVE;
                    data->size = 4;
                    data->params = new VALUE[data->size];
                    for (unsigned int i = 0; i < data->size; ++i)
                        data->params[i] = params[i];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_EXITSIZEMOVE: {
            if (su_process_mw_messages) {
                VALUE params[4];
                RECT rect;
                GetWindowRect(handle, &rect);
                params[0] = RU::to_value(rect.left);
                params[1] = RU::to_value(rect.top);
                params[2] = RU::to_value(rect.right - rect.left);
                params[3] = RU::to_value(rect.bottom - rect.top);
                c_call_observer_event(ID_SWO_ON_EXIT_SIZE_MOVE, 4, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_EXIT_SIZE_MOVE;
                    data->size = 4;
                    data->params = new VALUE[data->size];
                    for (unsigned int i = 0; i < data->size; ++i)
                        data->params[i] = params[i];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_MOVING:
        case WM_SIZING: {
            if (su_process_mw_messages) {
                VALUE params[4];
                RECT* rect = reinterpret_cast<RECT*>(lParam);
                params[0] = RU::to_value(rect->left);
                params[1] = RU::to_value(rect->top);
                params[2] = RU::to_value(rect->right - rect->left);
                params[3] = RU::to_value(rect->bottom - rect->top);
                c_call_observer_event(ID_SWO_ON_SIZE_MOVE, 4, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_SIZE_MOVE;
                    data->size = 4;
                    data->params = new VALUE[data->size];
                    for (unsigned int i = 0; i < data->size; ++i)
                        data->params[i] = params[i];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        default: {
            if (su_process_mw_messages == true && msg >= UMESSAGE_BASE + UMESSAGE_MIN && msg <= UMESSAGE_BASE + UMESSAGE_MAX) {
                unsigned int msg_id = msg - UMESSAGE_BASE;
                HWND sender_handle = (HWND)wParam;
                if (IsWindow(sender_handle) == TRUE) {
                    DWORD pid;
                    GetWindowThreadProcessId(sender_handle, &pid);
                    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
                    if (hProc != NULL) {
                        const char* udata_address = reinterpret_cast<char*>(lParam);
                        unsigned int detail_size = sizeof(unsigned int) * 2;
                        char* detail_buf = new char[detail_size];
                        SIZE_T num_bytes_read;
                        BOOL res = ReadProcessMemory(hProc, udata_address, detail_buf, detail_size, &num_bytes_read);
                        if (res == TRUE && (unsigned int)num_bytes_read == detail_size && reinterpret_cast<unsigned int*>(detail_buf)[0] == UMESSAGE_DATA_ID) {
                            bool data_valid = true;
                            unsigned int data_type = reinterpret_cast<unsigned int*>(detail_buf)[1];
                            VALUE params[3];
                            params[0] = RU::to_value(sender_handle);
                            params[1] = RU::to_value(msg_id);
                            if (data_type == 0) // Nil
                                params[2] = Qnil;
                            else if (data_type == 1) // True
                                params[2] = Qtrue;
                            else if (data_type == 2) // False
                                params[2] = Qfalse;
                            else if (data_type == 3) { // Fixnum
                                unsigned int data_size = sizeof(int);
                                char* data_buf = new char[data_size];
                                res = ReadProcessMemory(hProc, udata_address + detail_size, data_buf, data_size, &num_bytes_read);
                                if (res == TRUE && (unsigned int)num_bytes_read == data_size)
                                    params[2] = RU::to_value(reinterpret_cast<int*>(data_buf)[0]);
                                else
                                    data_valid = false;
                                delete[] data_buf;
                            }
                            else if (data_type == 4) { // Float
                                unsigned int data_size = sizeof(double);
                                char* data_buf = new char[data_size];
                                res = ReadProcessMemory(hProc, udata_address + detail_size, data_buf, data_size, &num_bytes_read);
                                if (res == TRUE && (unsigned int)num_bytes_read == data_size)
                                    params[2] = RU::to_value(reinterpret_cast<double*>(data_buf)[0]);
                                else
                                    data_valid = false;
                                delete[] data_buf;
                            }
                            else if (data_type > 4 && data_type < 10 ) { // Bignum, String, Symbol, Array, Hash
                                unsigned int data_size = sizeof(unsigned int);
                                char* data_buf = new char[data_size];
                                res = ReadProcessMemory(hProc, udata_address + detail_size, data_buf, data_size, &num_bytes_read);
                                unsigned int text_data_size = 0;
                                if (res == TRUE && (unsigned int)num_bytes_read == data_size)
                                    text_data_size = reinterpret_cast<unsigned int*>(data_buf)[0];
                                else
                                    data_valid = false;
                                delete[] data_buf;
                                if (data_valid) {
                                    data_buf = new char[text_data_size];
                                    res = ReadProcessMemory(hProc, udata_address + detail_size + data_size, data_buf, text_data_size, &num_bytes_read);
                                    if (res == TRUE && static_cast<unsigned int>(num_bytes_read) == text_data_size) {
                                        if (data_type == 5) { // Bignum
                                            VALUE v_data = RU::to_value(data_buf);
                                            params[2] = rb_funcall(v_data, RU::INTERN_TO_I, 0);
                                            //params[2] = rb_cstr2inum(data_buf, 10);
                                        }
                                        else if (data_type == 6) // String
                                            params[2] = RU::to_value(data_buf, text_data_size);
                                        else if (data_type == 7) // Symbol
                                            params[2] = rb_str_intern(RU::to_value(data_buf));
                                        else if (data_type == 8 || data_type == 9) { // Array or Hash
                                            int eval_res = 0;
                                            params[2] = rb_eval_string_protect(data_buf, &eval_res);
                                            if (eval_res != 0) data_valid = false;
                                        }
                                        else
                                            data_valid = false;
                                    }
                                    else
                                        data_valid = false;
                                    delete[] data_buf;
                                }
                            }
                            else
                                data_valid = false;

                            if (data_valid)
                                c_call_observer_event(ID_SWO_ON_USER_MESSAGE, 3, params);
                        }
                        if (detail_buf != nullptr)
                            delete[] detail_buf;
                        CloseHandle(hProc);
                    }
                }
            }
        }
    }

    if (process_message)
        return CallWindowProc(su_original_main_window_proc, handle, msg, wParam, lParam);
    else
        return 0L;
}

LRESULT CALLBACK AMS::Sketchup::CustomViewportWindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
    bool process_message = true;

    switch (msg) {
        case WM_STYLECHANGED: {
            if (wParam != 16) {
                STYLESTRUCT* style = reinterpret_cast<STYLESTRUCT*>(lParam);
                if ((style->styleOld | WS_EX_CLIENTEDGE) == style->styleOld && (style->styleNew | WS_EX_CLIENTEDGE) != style->styleNew) {
                    VALUE params[1] = { Qfalse };
                    c_call_observer_event(ID_SWO_ON_VIEWPORT_BORDER_CHANGED, 1, params);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_VIEWPORT_BORDER_CHANGED;
                        data->size = 1;
                        data->params = new VALUE[1];
                        data->params[0] = params[0];
                        su_post_event_data[timer_id] = data;
                    }
                }
                else if ((style->styleOld | WS_EX_CLIENTEDGE) != style->styleOld && (style->styleNew | WS_EX_CLIENTEDGE) == style->styleNew) {
                    VALUE params[1] = { Qtrue };
                    c_call_observer_event(ID_SWO_ON_VIEWPORT_BORDER_CHANGED, 1, params);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_VIEWPORT_BORDER_CHANGED;
                        data->size = 1;
                        data->params = new VALUE[1];
                        data->params[0] = params[0];
                        su_post_event_data[timer_id] = data;
                    }
                }
            }
            break;
        }
        case WM_SIZE: {
            int w = LOWORD(lParam);
            int h = HIWORD(lParam);
            if (w != 0 || h != 0) {
                VALUE params[2];
                params[0] = RU::to_value(w);
                params[1] = RU::to_value(h);
                c_call_observer_event(ID_SWO_ON_VIEWPORT_SIZE, 2, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_VIEWPORT_SIZE;
                    data->size = 2;
                    data->params = new VALUE[data->size];
                    for (unsigned int i = 0; i < data->size; ++i)
                        data->params[i] = params[i];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_MOUSEMOVE: {
            VALUE params[2];
            su_vp_cursor_pos[0] = GET_X_LPARAM(lParam);
            su_vp_cursor_pos[1] = GET_Y_LPARAM(lParam);
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            if (su_vp_mouse_leave) {
                su_vp_mouse_leave = false;
                process_message = c_call_observer_event(ID_SWP_ON_MOUSE_ENTER, 2, params, true);
            }
            else
                process_message = c_call_observer_event(ID_SWP_ON_MOUSE_MOVE, 2, params, true);
            break;
        }
        case WM_MOUSELEAVE: {
            VALUE params[2];
            params[0] = RU::to_value(su_vp_cursor_pos[0]);
            params[1] = RU::to_value(su_vp_cursor_pos[1]);
            su_vp_mouse_leave = true;
            process_message = c_call_observer_event(ID_SWP_ON_MOUSE_LEAVE, 2, params, true);
            break;
        }
        case WM_LBUTTONDOWN: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_LBUTTON_DOWN, 2, params, true);
            break;
        }
        case WM_LBUTTONUP: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_LBUTTON_UP, 2, params, true);
            break;
        }
        case WM_LBUTTONDBLCLK: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_LBUTTON_DOUBLE_CLICK, 2, params, true);
            break;
        }
        case WM_RBUTTONDOWN: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_RBUTTON_DOWN, 2, params, true);
            break;
        }
        case WM_RBUTTONUP: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_RBUTTON_UP, 2, params, true);
            break;
        }
        case WM_RBUTTONDBLCLK: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_RBUTTON_DOUBLE_CLICK, 2, params, true);
            break;
        }
        case WM_MBUTTONDOWN: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_MBUTTON_DOWN, 2, params, true);
            break;
        }
        case WM_MBUTTONUP: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_MBUTTON_UP, 2, params, true);
            break;
        }
        case WM_MBUTTONDBLCLK: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event(ID_SWP_ON_MBUTTON_DOUBLE_CLICK, 2, params, true);
            break;
        }
        case WM_XBUTTONDOWN: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event((wParam >> 16) == 1 ? ID_SWP_ON_XBUTTON1_DOWN : ID_SWP_ON_XBUTTON2_DOWN, 2, params, true);
            break;
        }
        case WM_XBUTTONUP: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event((wParam >> 16) == 1 ? ID_SWP_ON_XBUTTON1_UP : ID_SWP_ON_XBUTTON2_UP, 2, params, true);
            break;
        }
        case WM_XBUTTONDBLCLK: {
            VALUE params[2];
            params[0] = RU::to_value(GET_X_LPARAM(lParam));
            params[1] = RU::to_value(GET_Y_LPARAM(lParam));
            process_message = c_call_observer_event((wParam >> 16) == 1 ? ID_SWP_ON_XBUTTON1_DOUBLE_CLICK : ID_SWP_ON_XBUTTON2_DOUBLE_CLICK, 2, params, true);
            break;
        }
        case WM_MOUSEWHEEL: {
            VALUE params[3];
            RECT rect;
            c_get_viewport_rect(rect);
            params[0] = RU::to_value(GET_X_LPARAM(lParam) - rect.left);
            params[1] = RU::to_value(GET_Y_LPARAM(lParam) - rect.top);
            params[2] = RU::to_value(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1 : -1);
            process_message = c_call_observer_event(ID_SWP_ON_MOUSE_WHEEL_ROTATE, 3, params, true);
            break;
        }
        case WM_MOUSEHWHEEL: {
            VALUE params[3];
            RECT rect;
            c_get_viewport_rect(rect);
            params[0] = RU::to_value(GET_X_LPARAM(lParam) - rect.left);
            params[1] = RU::to_value(GET_Y_LPARAM(lParam) - rect.top);
            params[2] = RU::to_value(GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1 : -1);
            process_message = c_call_observer_event(ID_SWP_ON_MOUSE_WHEEL_TILT, 3, params, true);
            break;
        }
    }

    if (su_original_viewport_window_proc != nullptr && process_message)
        return CallWindowProc(su_original_viewport_window_proc, handle, msg, wParam, lParam);
    else
        return 0L;
}

LRESULT CALLBACK AMS::Sketchup::CustomTabProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case TCM_INSERTITEMA:
        case TCM_INSERTITEMW: {
            if (TabCtrl_GetItemCount(handle) == 0) {
                c_call_observer_event(ID_SWO_ON_SCENES_BAR_FILLED, 0, nullptr);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_SCENES_BAR_FILLED;
                    data->size = 0;
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case TCM_DELETEITEM: {
            if (TabCtrl_GetItemCount(handle) == 1) {
                c_call_observer_event(ID_SWO_ON_SCENES_BAR_EMPTIED, 0, nullptr);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_SCENES_BAR_EMPTIED;
                    data->size = 0;
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case TCM_DELETEALLITEMS: {
            if (TabCtrl_GetItemCount(handle) > 0) {
                c_call_observer_event(ID_SWO_ON_SCENES_BAR_EMPTIED, 0, nullptr);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_SCENES_BAR_EMPTIED;
                    data->size = 0;
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case TCM_SETCURSEL: {
            VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
            VALUE v_pages = rb_funcall(v_active_model, RU::INTERN_PAGES, 0);
            VALUE v_tab1 = RU::to_value(TabCtrl_GetCurSel(handle));
            VALUE v_tab2 = RU::to_value((int)wParam);
            VALUE v_page1 = rb_funcall(v_pages, RU::INTERN_OP_SQUARE_BRACKETS, 1, v_tab1);
            VALUE v_page2 = rb_funcall(v_pages, RU::INTERN_OP_SQUARE_BRACKETS, 1, v_tab2);
            VALUE params[4] = { v_page1, v_page2, v_tab1, v_tab2 };
            c_call_observer_event(ID_SWO_ON_PAGE_SELECTED, 4, params);
            break;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONDBLCLK: {
            su_process_sb_page_selection = true;
            TCHITTESTINFO hit;
            hit.pt.x = GET_X_LPARAM(lParam);
            hit.pt.y = GET_Y_LPARAM(lParam);
            int tab = TabCtrl_HitTest(handle, &hit);
            if (tab != -1) {
                VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
                VALUE v_pages = rb_funcall(v_active_model, RU::INTERN_PAGES, 0);
                VALUE v_tab1 = RU::to_value(TabCtrl_GetCurSel(handle));
                VALUE v_tab2 = RU::to_value(tab);
                VALUE v_page1 = rb_funcall(v_pages, RU::INTERN_OP_SQUARE_BRACKETS, 1, v_tab1);
                VALUE v_page2 = rb_funcall(v_pages, RU::INTERN_OP_SQUARE_BRACKETS, 1, v_tab2);
                VALUE params[4] = { v_page1, v_page2, v_tab1, v_tab2 };
                if (!c_call_observer_event(ID_SWP_ON_PAGE_SELECTED, 4, params, true)) {
                    su_process_sb_page_selection = false;
                    return 0L;
                }
            }
            break;
        }
        case WM_LBUTTONUP: {
            if (!su_process_sb_page_selection)
                return 0L;
            break;
        }
    }

    if (su_original_tab_proc)
        return CallWindowProc(su_original_tab_proc, handle, msg, wParam, lParam);
    else
        return 0L;
}

LRESULT CALLBACK AMS::Sketchup::CustomStatusBarProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_WINDOWPOSCHANGED: {
            WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
            if ((wp->flags | SWP_SHOWWINDOW) == wp->flags) {
                VALUE params[1] = { Qtrue };
                c_call_observer_event(ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED, 1, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED;
                    data->size = 1;
                    data->params = new VALUE[1];
                    data->params[0] = params[0];
                    su_post_event_data[timer_id] = data;
                }
            }
            else if ((wp->flags | SWP_HIDEWINDOW) == wp->flags) {
                VALUE params[1] = { Qfalse };
                c_call_observer_event(ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED, 1, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED;
                    data->size = 1;
                    data->params = new VALUE[1];
                    data->params[0] = params[0];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_STYLECHANGED: {
            if ((int)wParam == GWL_STYLE) {
                STYLESTRUCT* style = reinterpret_cast<STYLESTRUCT*>(lParam);
                if ((style->styleOld | WS_VISIBLE) != style->styleOld && (style->styleNew | WS_VISIBLE) == style->styleNew) {
                    VALUE params[1] = { Qtrue };
                    c_call_observer_event(ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED, 1, params);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED;
                        data->size = 1;
                        data->params = new VALUE[1];
                        data->params[0] = params[0];
                        su_post_event_data[timer_id] = data;
                    }
                }
                else if ((style->styleOld | WS_VISIBLE) == style->styleOld && (style->styleNew | WS_VISIBLE) != style->styleNew) {
                    VALUE params[1] = { Qfalse };
                    c_call_observer_event(ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED, 1, params);
                    UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                    if (timer_id != 0) {
                        PostEventData* data = new PostEventData;
                        data->id = ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED;
                        data->size = 1;
                        data->params = new VALUE[1];
                        data->params[0] = params[0];
                        su_post_event_data[timer_id] = data;
                    }
                }
            }
            break;
        }
    }

    if (su_original_statusbar_proc)
        return CallWindowProc(su_original_statusbar_proc, handle, msg, wParam, lParam);
    else
        return 0L;
}

LRESULT CALLBACK AMS::Sketchup::CustomScenesBarProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_WINDOWPOSCHANGED: {
            WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
            if ((wp->flags | SWP_SHOWWINDOW) == wp->flags) {
                VALUE params[1] = { Qtrue };
                c_call_observer_event(ID_SWO_ON_SCENES_BAR_VISIBILITY_CHANGED, 1, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_SCENES_BAR_VISIBILITY_CHANGED;
                    data->size = 1;
                    data->params = new VALUE[1];
                    data->params[0] = params[0];
                    su_post_event_data[timer_id] = data;
                }
            }
            else if ((wp->flags | SWP_HIDEWINDOW) == wp->flags) {
                VALUE params[1] = { Qfalse };
                c_call_observer_event(ID_SWO_ON_SCENES_BAR_VISIBILITY_CHANGED, 1, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_SCENES_BAR_VISIBILITY_CHANGED;
                    data->size = 1;
                    data->params = new VALUE[1];
                    data->params[0] = params[0];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
    }

    if (su_original_scenesbar_proc)
        return CallWindowProc(su_original_scenesbar_proc, handle, msg, wParam, lParam);
    else
        return 0L;
}

LRESULT CALLBACK AMS::Sketchup::CustomControlBarProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam) {
    int index = 0;
    for (int i = 0; i < 4; ++i) {
        if (su_control_bars[i] == handle) {
            index = i;
            break;
        }
    }

    switch (msg) {
        case WM_WINDOWPOSCHANGED: {
            WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(lParam);
            if ((wp->flags | SWP_SHOWWINDOW) == wp->flags && su_tbc_visible[index] == false) {
                VALUE params[2];
                su_tbc_visible[index] = true;
                params[0] = RU::to_value(index + 1);
                params[1] = Qtrue;
                c_call_observer_event(ID_SWO_ON_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED, 2, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED;
                    data->size = 2;
                    data->params = new VALUE[data->size];
                    for (unsigned int i = 0; i < data->size; ++i)
                        data->params[i] = params[i];
                    su_post_event_data[timer_id] = data;
                }
            }
            else if ((wp->flags | SWP_HIDEWINDOW) == wp->flags && su_tbc_visible[index] == true) {
                VALUE params[2];
                su_tbc_visible[index] = false;
                params[0] = RU::to_value(index + 1);
                params[1] = Qfalse;
                c_call_observer_event(ID_SWO_ON_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED, 2, params);
                UINT_PTR timer_id = SetTimer(NULL, 0, POST_EVENT_DELAY, PostEventTimerProc);
                if (timer_id != 0) {
                    PostEventData* data = new PostEventData;
                    data->id = ID_SWO_ON_POST_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED;
                    data->size = 2;
                    data->params = new VALUE[data->size];
                    for (unsigned int i = 0; i < data->size; ++i)
                        data->params[i] = params[i];
                    su_post_event_data[timer_id] = data;
                }
            }
            break;
        }
        case WM_SIZE: {
            int w = LOWORD(lParam);
            int h = HIWORD(lParam);
            bool filled = w > 0 && h > 0;
            if (filled != su_tbc_filled[index])
                SetTimer(su_main_window, CONTAINER_TIMER_ID + (filled ? 10 : 0) + index, USER_TIMER_MINIMUM, ContainerTimerProc);
            break;
        }
    }

    if (su_original_controlbar_proc[index])
        return CallWindowProc(su_original_controlbar_proc[index], handle, msg, wParam, lParam);
    else
        return 0L;
}

BOOL CALLBACK AMS::Sketchup::ComputeContainerRectProc(HWND handle, LPARAM lParam) {
    if (GetAncestor(GetAncestor(handle, GA_PARENT), GA_PARENT) != su_main_window) return TRUE;
    LONG_PTR style = GetWindowLongPtr(handle, GWL_STYLE);
    if ((style | WS_VISIBLE) == style) {
        RECT* container_rect = reinterpret_cast<RECT*>(lParam);
        RECT rect;
        GetWindowRect(handle, &rect);
        if (rect.left < container_rect->left)
            container_rect->left = rect.left;
        if (rect.top < container_rect->top)
            container_rect->top = rect.top;
        if (rect.bottom > container_rect->bottom)
            container_rect->bottom = rect.bottom;
        if (rect.right > container_rect->right)
            container_rect->right = rect.right;
    }
    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::IsContainerFilledProc(HWND handle, LPARAM lParam) {
    if (GetAncestor(GetAncestor(handle, GA_PARENT), GA_PARENT) != su_main_window) return TRUE;
    LONG_PTR style = GetWindowLongPtr(handle, GWL_STYLE);
    if ((style | WS_VISIBLE) == style) {
        bool* container_filled = reinterpret_cast<bool*>(lParam);
        *container_filled = true;
        return FALSE;
    }
    else
        return TRUE;
}

// Use this procedure to get all dialogs.
BOOL CALLBACK AMS::Sketchup::GetDialogsProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid == su_process_id) {
        TCHAR cname[10];
        GetClassName(handle, cname, 10);
        if (StrCmpN(cname, TEXT("#32770"), 6) == 0 || StrCmpN(cname, TEXT("QWidget"), 7) == 0 || StrCmpN(cname, TEXT("QTool"), 5) == 0) {
            if (su_dialogs_to_ignore.find(handle) == su_dialogs_to_ignore.end()) {
                std::set<HWND>* dialogs = reinterpret_cast<std::set<HWND>*>(lParam);
                dialogs->insert(handle);
            }
        }
    }
    return TRUE;
}

// Use this procedure to get all visible dialogs.
BOOL CALLBACK AMS::Sketchup::GetVisibleDialogsProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid == su_process_id && IsWindowVisible(handle) == 1) {
        TCHAR cname[10];
        GetClassName(handle, cname, 10);
        if (StrCmpN(cname, TEXT("#32770"), 6) == 0 || StrCmpN(cname, TEXT("QWidget"), 7) == 0 || StrCmpN(cname, TEXT("QTool"), 5) == 0) {
            if (su_dialogs_to_ignore.find(handle) == su_dialogs_to_ignore.end()) {
                std::set<HWND>* visible_dialogs = reinterpret_cast<std::set<HWND>*>(lParam);
                visible_dialogs->insert(handle);
            }
        }
    }
    return TRUE;
}

// Use this procedure to get all toolbars.
BOOL CALLBACK AMS::Sketchup::GetToolbarsProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid == su_process_id) {
        bool found = false;
        TCHAR cname[60];
        GetClassName(handle, cname, 60);
        if (RU::su_version < 13) {
            if (StrStr(cname, TEXT("Afx:")) != NULL && StrStr(cname, TEXT(":8:")) != NULL)
                found = true;
        }
        else if (StrCmpN(cname, TEXT("Afx:MiniFrame:"), 14) == 0) {
            if (RU::su_version > 15) {
                HWND child = GetWindow(handle, GW_CHILD);
                if (child != NULL) {
                    GetClassName(child, cname, 60);
                    if (StrCmpN(cname, TEXT("Afx:ControlBar:"), 15) != 0)
                        found = true;
                }
            }
            else
                found = true;
        }
        if (found && su_toolbars_to_ignore.find(handle) == su_toolbars_to_ignore.end()) {
            std::set<HWND>* toolbars = reinterpret_cast<std::set<HWND>*>(lParam);
            toolbars->insert(handle);
        }
    }
    return TRUE;
}

// Use this procedure to get all visible toolbars.
BOOL CALLBACK AMS::Sketchup::GetVisibleToolbarsProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid == su_process_id && IsWindowVisible(handle) == TRUE) {
        bool found = false;
        TCHAR cname[60];
        GetClassName(handle, cname, 60);
        if (RU::su_version < 13) {
            if (StrStr(cname, TEXT("Afx:")) != NULL && StrStr(cname, TEXT(":8:")) != NULL)
                found = true;
        }
        else if (StrCmpN(cname, TEXT("Afx:MiniFrame:"), 14) == 0) {
            if (RU::su_version > 15) {
                HWND child = GetWindow(handle, GW_CHILD);
                if (child != NULL) {
                    GetClassName(child, cname, 60);
                    if (StrCmpN(cname, TEXT("Afx:ControlBar:"), 15) != 0)
                        found = true;
                }
            }
            else
                found = true;
        }
        if (found && su_toolbars_to_ignore.find(handle) == su_toolbars_to_ignore.end()) {
            std::set<HWND>* visible_toolbars = reinterpret_cast<std::set<HWND>*>(lParam);
            visible_toolbars->insert(handle);
        }
    }
    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::FindWindowProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid != su_process_id) return TRUE;
    SearchInfo* info = reinterpret_cast<SearchInfo*>(lParam);
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

BOOL CALLBACK AMS::Sketchup::FindChildWindowProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid != su_process_id) return TRUE;
    ChildSearchInfo* info = reinterpret_cast<ChildSearchInfo*>(lParam);
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

VOID CALLBACK AMS::Sketchup::AcceleratorTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    KillTimer(hwnd, idEvent);
    if (su_triggered_command != su_expected_command)
        SendMessage(su_main_window, WM_COMMAND, static_cast<WPARAM>(su_expected_command), 0);
    su_triggered_command = 0;
    su_expected_command = 0;
}

VOID CALLBACK AMS::Sketchup::ContainerTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    KillTimer(hwnd, idEvent);
    bool filled = (idEvent % 100) >= 10;
    int index = idEvent % 10;
    VALUE params[1];
    params[0] = RU::to_value(index+1);
    if (c_is_toolbar_container_filled(index) == filled) {
        su_tbc_filled[index] = filled;
        c_call_observer_event(filled ? ID_SWO_ON_TOOLBAR_CONTAINER_FILLED : ID_SWO_ON_TOOLBAR_CONTAINER_EMPTIED, 1, params);
        c_call_observer_event(filled ? ID_SWO_ON_POST_TOOLBAR_CONTAINER_FILLED : ID_SWO_ON_POST_TOOLBAR_CONTAINER_EMPTIED, 1, params);
    }
}

VOID CALLBACK AMS::Sketchup::PostEventTimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    KillTimer(hwnd, idEvent);
    std::map<UINT_PTR, PostEventData*>::iterator it = su_post_event_data.find(idEvent);
    if (it == su_post_event_data.end()) return;
    PostEventData* data = it->second;
    c_call_observer_event(data->id, data->size, data->params);
    su_post_event_data.erase(it);
    if (data->size != 0 && data->params != nullptr)
        delete[] data->params;
    delete data;
}


// Use this procedure to monitor and make decisions to F1 and Windows key.
// The F1 key opens the Help dialog regardless of whether it was disabled in
// standard keyboard procedure.
// The Windows key opens the start menu, which could be inconvinient when
// creating games in SU.
// Both of these system keys cannot be disabled with a standard keyboard
// procedure. That's where low level keyboard procedure comes in to play.
// There is one disadvantage of the low level keyboard procedure, it does not
// specify whether the key is extended. We will have to use variables to
// distinguish the key down and key extended events.
// This method will not process messages if active window is not SU main window.
LRESULT CALLBACK AMS::Sketchup::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* data = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
    unsigned int vk = data->vkCode;

    if (su_process_kb_messages &&
        nCode == 0 &&
        (vk == VK_F1 || vk == VK_LWIN || vk == VK_RWIN))
    {
        BYTE keys[256];
        wchar_t* key_name = new wchar_t[16];
        VALUE params[3];
        short vk_state = -1;

        GetKeyboardState(keys);
        int ures = ToUnicode(data->vkCode, data->scanCode, keys, key_name, 16, 0);

        std::map<int, const wchar_t*>::iterator it = su_virtual_key_names.find(vk);
        if (it == su_virtual_key_names.end())
            params[0] = RU::to_value(vk);
        else
            params[0] = RU::to_value(it->second);
        params[1] = RU::to_value(vk);
        params[2] = (ures > 0) ? RU::win_wc_str_to_value(key_name, ures) : Qnil;

        delete[] key_name;

        // Key states: up(-1), down(0), extended(1)
        if (vk == VK_F1) {
            su_f1_state = ((wParam == WM_KEYUP || wParam == WM_SYSKEYUP) ? -1 : (su_f1_state == -1 ? 0 : 1));
            vk_state = su_f1_state;
        }
        else if (vk == VK_LWIN) {
            su_lwin_state = ((wParam == WM_KEYUP || wParam == WM_SYSKEYUP) ? -1 : (su_lwin_state == -1 ? 0 : 1));
            vk_state = su_lwin_state;
        }
        else if (vk == VK_RWIN) {
            su_rwin_state = ((wParam == WM_KEYUP || wParam == WM_SYSKEYUP) ? -1 : (su_rwin_state == -1 ? 0 : 1));
            vk_state = su_rwin_state;
        }

        bool process_message = c_call_observer_event(vk_state == -1 ? ID_SWP_ON_KEY_UP : (vk_state == 0 ? ID_SWP_ON_KEY_DOWN : ID_SWP_ON_KEY_EXTENDED), 3, params, true);

        LRESULT res = CallNextHookEx(su_hooks[0], nCode, wParam, lParam);
        return process_message ? res : 1L;
    }
    else
        return CallNextHookEx(su_hooks[0], nCode, wParam, lParam);
}

// Use this procedure to monitor and make decisions to most virtual keys.
LRESULT CALLBACK AMS::Sketchup::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode != 0 || wParam == VK_F1 || wParam == VK_LWIN || wParam == VK_RWIN)
        return CallNextHookEx(su_hooks[1], nCode, wParam, lParam);

    if (su_process_kb_messages) {
        BYTE keys[256];
        wchar_t* key_name = new wchar_t[16];
        VALUE params[3];

        GetKeyboardState(keys);
        int ures = ToUnicode((UINT)wParam, MapVirtualKey((UINT)wParam, 0), keys, key_name, 16, 0);

        std::map<int, const wchar_t*>::iterator it = su_virtual_key_names.find((int)wParam);
        if (it == su_virtual_key_names.end())
            params[0] = RU::to_value((int)wParam);
        else
            params[0] = RU::to_value(it->second);
        params[1] = RU::to_value((int)wParam);
        params[2] = (ures > 0) ? RU::win_wc_str_to_value(key_name, ures) : Qnil;

        delete[] key_name;
        bool process_message = c_call_observer_event((HIWORD(lParam) & KF_UP) == KF_UP ? ID_SWP_ON_KEY_UP : ((lParam >> 30) == 0 ? ID_SWP_ON_KEY_DOWN : ID_SWP_ON_KEY_EXTENDED), 3, params, true);
        if (!process_message) {
            CallNextHookEx(su_hooks[1], nCode, wParam, lParam);
            return 1;
        }
    }

    // Process accelerators when menu bar is removed.
    if ((HIWORD(lParam) & KF_UP) != KF_UP && GetMenu(su_main_window) != su_menu_bar) {
        KeyboardShortcut* accelerator = new KeyboardShortcut();
        accelerator->control_down = (GetKeyState(VK_CONTROL) >> 15) != 0;
        accelerator->menu_down = (GetKeyState(VK_MENU) >> 15) != 0;
        accelerator->shift_down = (GetKeyState(VK_SHIFT) >> 15) != 0;
        accelerator->virtual_key = (int)wParam;
        c_call_accelerator(accelerator);
        delete accelerator;
    }
    return CallNextHookEx(su_hooks[1], nCode, wParam, lParam);
}

BOOL CALLBACK AMS::Sketchup::GetFloatingTraysProc(HWND handle, LPARAM lParam) {
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid != su_process_id) return TRUE;

    TCHAR cname[60];
    GetClassName(handle, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:MiniFrame"), 13) != 0) return TRUE;

    HWND handle2 = GetWindow(handle, GW_CHILD);
    if (handle2 == NULL) return TRUE;
    GetClassName(handle2, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:ControlBar"), 14) != 0) return TRUE;

    HWND handle3 = GetWindow(handle2, GW_CHILD);
    if (handle3 == NULL) return TRUE;
    GetClassName(handle3, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:TabWnd"), 10) == 0 || StrCmpN(cname, TEXT("#32770"), 6) == 0) {
        std::set<HWND>* trays = reinterpret_cast<std::set<HWND>*>(lParam);
        trays->insert(handle2);
    }

    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::GetVisibleFloatingTraysProc(HWND handle, LPARAM lParam) {
    if (IsWindowVisible(handle) == FALSE) return TRUE;

    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid != su_process_id) return TRUE;

    TCHAR cname[60];
    GetClassName(handle, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:MiniFrame"), 13) != 0) return TRUE;

    HWND handle2 = GetWindow(handle, GW_CHILD);
    if (handle2 == NULL) return TRUE;
    GetClassName(handle2, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:ControlBar"), 14) != 0) return TRUE;

    HWND handle3 = GetWindow(handle2, GW_CHILD);
    if (handle3 == NULL) return TRUE;
    GetClassName(handle3, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:TabWnd"), 10) == 0 || StrCmpN(cname, TEXT("#32770"), 6) == 0) {
        std::set<HWND>* trays = reinterpret_cast<std::set<HWND>*>(lParam);
        trays->insert(handle2);
    }

    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::GetDockedTraysProc(HWND handle, LPARAM lParam) {
    if (GetAncestor(handle, GA_PARENT) != su_main_window) return TRUE;
    TCHAR cname[60];
    GetClassName(handle, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:ControlBar"), 14) != 0) return TRUE;

    HWND child = GetWindow(handle, GW_CHILD);
    GetClassName(child, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:TabWnd"), 10) == 0 || StrCmpN(cname, TEXT("#32770"), 6) == 0) {
        std::set<HWND>* trays = reinterpret_cast<std::set<HWND>*>(lParam);
        trays->insert(handle);
    }

    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::GetVisibleDockedTraysProc(HWND handle, LPARAM lParam) {
    if (GetAncestor(handle, GA_PARENT) != su_main_window || IsWindowVisible(handle) == FALSE) return TRUE;
    TCHAR cname[60];
    GetClassName(handle, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:ControlBar"), 14) != 0) return TRUE;

    HWND child = GetWindow(handle, GW_CHILD);
    GetClassName(child, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:TabWnd"), 10) == 0 || StrCmpN(cname, TEXT("#32770"), 6) == 0) {
        std::set<HWND>* trays = reinterpret_cast<std::set<HWND>*>(lParam);
        trays->insert(handle);
    }

    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::GetVisibleTraySlidersProc(HWND handle, LPARAM lParam) {
    if (GetAncestor(handle, GA_PARENT) != su_main_window || IsWindowVisible(handle) == FALSE) return TRUE;
    TCHAR cname[60];
    GetClassName(handle, cname, 60);
    if (StrCmpN(cname, TEXT("Afx:Slider"), 10) == 0) {
        std::set<HWND>* sliders = reinterpret_cast<std::set<HWND>*>(lParam);
        sliders->insert(handle);
    }
    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::GetOtherMainWindowsProc(HWND handle, LPARAM lParam) {
    if (handle == su_main_window || GetWindow(handle, GW_OWNER) != NULL || IsWindowVisible(handle) == FALSE) return TRUE;
    TCHAR cname[128];
    GetClassName(handle, cname, 128);
    if (StrCmpN(cname, TEXT("Afx:"), 4) != 0 || StrStr(cname, TEXT(":b:")) == NULL) return TRUE;
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProc == NULL) return TRUE;
    TCHAR buffer[20];
    GetModuleBaseName(hProc, 0, buffer, 20);
    CloseHandle(hProc);
    if (StrCmp(buffer, TEXT("SketchUp.exe")) == 0) {
        std::set<HWND>* main_windows = reinterpret_cast<std::set<HWND>*>(lParam);
        main_windows->insert(handle);
    }
    return TRUE;
}

BOOL CALLBACK AMS::Sketchup::GetVirtualScreenRectProc(HMONITOR monitor, HDC hdc, LPRECT lpRect, LPARAM dwData) {
    // Get monitor rect.
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    // Calculate global rect.
    LPRECT glob_rect = (LPRECT)dwData;
    if (info.rcMonitor.left < glob_rect->left)
        glob_rect->left = info.rcMonitor.left;
    if (info.rcMonitor.top < glob_rect->top)
        glob_rect->top = info.rcMonitor.top;
    if (info.rcMonitor.right > glob_rect->right)
        glob_rect->right = info.rcMonitor.right;
    if (info.rcMonitor.bottom > glob_rect->bottom)
        glob_rect->bottom = info.rcMonitor.bottom;
    // Continue enumeration.
    return TRUE;
}

DWORD CALLBACK AMS::Sketchup::ThreadedMessageBox(LPVOID lpParam) {
    MessageboxData* data = reinterpret_cast<MessageboxData*>(lpParam);
    VALUE v_data = reinterpret_cast<VALUE>(data);
    data->result = MessageBox(data->handle, data->text, data->caption, data->type);
    rb_thread_create(RUBY_METHOD_FUNC(c_mbp_trigger_proc), reinterpret_cast<void*>(v_data));
    return 0;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Sketchup::rbf_is_32bit(VALUE self) {
    return RU::su_b64bit ? Qfalse : Qtrue;
}

VALUE AMS::Sketchup::rbf_is_64bit(VALUE self) {
    return RU::su_b64bit ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_get_module_handle(VALUE self) {
    return RU::to_value(su_module_handle);
}

VALUE AMS::Sketchup::rbf_get_executable_path(VALUE self) {
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, su_process_id);
    if (hProc == NULL) return Qnil;
    TCHAR buffer[4096];
    unsigned int len = GetModuleFileNameEx(hProc, 0, buffer, 4096);
    CloseHandle(hProc);
    for (unsigned int i = 0; i < len; ++i)
        if (buffer[i] == '\\') buffer[i] = '/';
    return RU::win_wc_str_to_value(buffer, len);
}

VALUE AMS::Sketchup::rbf_get_executable_name(VALUE self) {
    HANDLE hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, su_process_id);
    if (hProc == NULL) return Qnil;
    TCHAR buffer[256];
    unsigned int len = GetModuleBaseName(hProc, 0, buffer, 256);
    CloseHandle(hProc);
    return RU::win_wc_str_to_value(buffer, len);
}

VALUE AMS::Sketchup::rbf_get_registry_path(VALUE self) {
    if (RU::su_version == 5)
        return RU::to_value("HKEY_CURRENT_USER/Software/Google/Google SketchUp5/");
    else if (RU::su_version >= 6 && RU::su_version <= 8) {
        char path[60];
        int len = sprintf(path, "HKEY_CURRENT_USER/Software/Google/SketchUp%d/", RU::su_version);
        return RU::to_value(path, len);
    }
    else {
        char path[60];
        int len = sprintf(path, "HKEY_CURRENT_USER/Software/SketchUp/SketchUp 20%d/", RU::su_version);
        return RU::to_value(path, len);
    }
}

VALUE AMS::Sketchup::rbf_get_process_id(VALUE self) {
    return RU::to_value(su_process_id);
}

VALUE AMS::Sketchup::rbf_get_thread_id(VALUE self) {
    return RU::to_value(su_thread_id);
}

VALUE AMS::Sketchup::rbf_get_main_window(VALUE self) {
    return RU::to_value(su_main_window);
}

VALUE AMS::Sketchup::rbf_get_menu_bar(VALUE self) {
    return RU::to_value(su_menu_bar);
}

VALUE AMS::Sketchup::rbf_get_viewport(VALUE self) {
    c_update_viewport_handle();
    return RU::to_value(su_viewport);
}

VALUE AMS::Sketchup::rbf_get_status_bar(VALUE self) {
    return RU::to_value(su_status_bar);
}

VALUE AMS::Sketchup::rbf_get_scenes_bar(VALUE self) {
    return RU::to_value(su_scenes_bar);
}

VALUE AMS::Sketchup::rbf_get_toolbar_containers(VALUE self) {
    VALUE v_containers = rb_ary_new2(4);
    for (unsigned int i = 0; i < 4; ++i)
        rb_ary_store(v_containers, i, RU::to_value(su_control_bars[i]));
    return v_containers;
}

VALUE AMS::Sketchup::rbf_switch_full_screen(int argc, VALUE* argv, VALUE self) {
    int reset_mode = 2;
    int set_mode = 1;
    if (argc == 3) {
        reset_mode = RU::value_to_int(argv[1]);
        set_mode = RU::value_to_int(argv[2]);
    }
    else if (argc == 2)
        reset_mode = RU::value_to_int(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..3 arguments.");
    bool state = RU::value_to_bool(argv[0]);
    return RU::to_value(c_set_main_window_full_screen(state, reset_mode, set_mode));
}

VALUE AMS::Sketchup::rbf_is_full_screen(VALUE self) {
    return RU::to_value(c_is_main_window_full_screen());
}

VALUE AMS::Sketchup::rbf_maximize(VALUE self) {
    if (c_is_main_window_maximized())
        return Qfalse;
    else if (c_is_main_window_full_screen())
        c_set_main_window_full_screen(false, 1);
    else
        ShowWindow(su_main_window, SW_MAXIMIZE);
    return Qtrue;
}

VALUE AMS::Sketchup::rbf_is_maximized(VALUE self) {
    return RU::to_value(c_is_main_window_maximized());
}

VALUE AMS::Sketchup::rbf_minimize(VALUE self) {
    if (c_is_main_window_minimized())
        return Qfalse;
    else
        ShowWindow(su_main_window, SW_MINIMIZE);
    return Qtrue;
}

VALUE AMS::Sketchup::rbf_is_minimized(VALUE self) {
    return RU::to_value(c_is_main_window_minimized());
}

VALUE AMS::Sketchup::rbf_restore(VALUE self) {
    if (c_is_main_window_minimized())
        return Qfalse;
    else if (c_is_main_window_full_screen())
        c_set_main_window_full_screen(false, 0);
    else
        ShowWindow(su_main_window, SW_RESTORE);
    return Qtrue;
}

VALUE AMS::Sketchup::rbf_is_restored(VALUE self) {
    return RU::to_value(c_is_main_window_restored());
}

VALUE AMS::Sketchup::rbf_activate(VALUE self) {
    return RU::to_value(BringWindowToTop(su_main_window) == TRUE);
}

VALUE AMS::Sketchup::rbf_set_menu_bar(VALUE self, VALUE v_state) {
    bool state = RU::value_to_bool(v_state);
    if (c_is_main_window_menu_bar_set() == state)
        return Qfalse;
    else {
        //~ BOOL original_state = IsWindowEnabled(su_main_window);
        //~ EnableWindow(su_main_window, FALSE);
        BOOL res = SetMenu(su_main_window, state ? su_menu_bar : NULL);
        //~ EnableWindow(su_main_window, original_state);
        return RU::to_value(res == TRUE);
    }
}

VALUE AMS::Sketchup::rbf_is_menu_bar_set(VALUE self) {
    return RU::to_value(c_is_main_window_menu_bar_set());
}

VALUE AMS::Sketchup::rbf_set_viewport_border(VALUE self, VALUE v_state) {
    bool state = RU::value_to_bool(v_state);
    if (c_is_viewport_border_set() == state) return Qfalse;
    LockWindowUpdate(su_viewport);
    SetWindowLongPtr(su_viewport, GWL_EXSTYLE, (LONG_PTR)(state ? WS_EX_CLIENTEDGE : 0));
    LockWindowUpdate(NULL);
    SetWindowPos(su_viewport, NULL, 0,0, 0,0, SWP_NOREPOSITION | SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE | SWP_NOMOVE);
    return Qtrue;
}

VALUE AMS::Sketchup::rbf_is_viewport_border_set(VALUE self) {
    return RU::to_value(c_is_viewport_border_set());
}

VALUE AMS::Sketchup::rbf_show_status_bar(int argc, VALUE* argv, VALUE self) {
    bool refresh;
    if (argc == 1)
        refresh = true;
    else if (argc == 2)
        refresh = RU::value_to_bool(argv[1]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
    bool state = RU::value_to_bool(argv[0]);
    if ((IsWindowVisible(su_status_bar) == TRUE) == state)
        return Qfalse;
    LONG_PTR style = GetWindowLongPtr(su_status_bar, GWL_STYLE);
    if (state)
        SetWindowLongPtr(su_status_bar, GWL_STYLE, style | WS_VISIBLE);
    else
        SetWindowLongPtr(su_status_bar, GWL_STYLE, style & ~WS_VISIBLE);
    if (refresh)
        c_recalc_layout();
    return Qtrue;
}

VALUE AMS::Sketchup::rbf_is_status_bar_visible(VALUE self) {
    return IsWindowVisible(su_status_bar) ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_show_scenes_bar(int argc, VALUE* argv, VALUE self) {
    bool refresh;
    if (argc == 1)
        refresh = true;
    else if (argc == 2)
        refresh = RU::value_to_bool(argv[1]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
    bool state = RU::value_to_bool(argv[0]);
    if (state && TabCtrl_GetItemCount(su_tab_control) == 0)
        return Qfalse;
    if ((IsWindowVisible(su_scenes_bar) == TRUE) == state)
        return Qfalse;
    ShowWindow(su_scenes_bar, state ? SW_SHOWNOACTIVATE : SW_HIDE);
    if (refresh) c_recalc_layout();
    return Qtrue;
}

VALUE AMS::Sketchup::rbf_is_scenes_bar_visible(VALUE self) {
    return IsWindowVisible(su_scenes_bar) ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_is_scenes_bar_filled(VALUE self) {
    return TabCtrl_GetItemCount(su_tab_control) > 0 ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_is_scenes_bar_empty(VALUE self) {
    return TabCtrl_GetItemCount(su_tab_control) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_activate_scenes_bar_tab(VALUE self, VALUE v_tab) {
    int tab = RU::value_to_int(v_tab);
    int tabs_len = TabCtrl_GetItemCount(su_tab_control);
    if (tabs_len > 0) {
        for (int i = 0; i < tabs_len; ++i)
            TabCtrl_HighlightItem(su_tab_control, i, 0);
        TabCtrl_SetCurSel(su_tab_control, tab);
        TabCtrl_SetCurFocus(su_tab_control, tab);
        TabCtrl_HighlightItem(su_tab_control, tab, 1);
        return Qtrue;
    }
    else
        return Qfalse;
}

VALUE AMS::Sketchup::rbf_show_toolbar_container(int argc, VALUE* argv, VALUE self) {
    bool refresh;
    if (argc == 2)
        refresh = true;
    else if (argc == 3)
        refresh = RU::value_to_bool(argv[2]);
    else
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");
    int bar = Geom::clamp(RU::value_to_int(argv[0]), 1, 6);
    bool state = RU::value_to_bool(argv[1]);
    if (bar >= 1 && bar <= 4)
        return RU::to_value(c_show_toolbar_container(bar - 1, state, refresh));
    else if (bar == 5) {
        bool change = false;
        for (int i = 0; i < 4; ++i)
            if (c_is_toolbar_container_filled(i))
                if (c_show_toolbar_container(i, state, false))
                    change = true;
        if (refresh) c_recalc_layout();
        return RU::to_value(change);
    }
    else if (bar == 6) {
        bool change = false;
        for (int i = 0; i < 4; ++i)
            if (c_show_toolbar_container(i, state, false))
                change = true;
        if (refresh) c_recalc_layout();
        return RU::to_value(change);
    }
    else
        rb_raise(rb_eTypeError, "Given bar value is invalid! Expected a Fixnum value between 1 and 6.");

}

VALUE AMS::Sketchup::rbf_is_toolbar_container_visible(VALUE self, VALUE v_bar) {
    int bar = RU::value_to_int(v_bar);
    if (bar >= 1 && bar <= 4)
        return RU::to_value(c_is_toolbar_container_visible(bar - 1));
    else if (bar == 5) {
        for (int i = 0; i < 4; ++i)
            if (c_is_toolbar_container_visible(i)) return Qtrue;
        return Qfalse;
    }
    else if (bar == 6) {
        for (int i = 0; i < 4; ++i)
            if (!c_is_toolbar_container_visible(i)) return Qfalse;
        return Qtrue;
    }
    else if (bar == 7) {
        for (int i = 0; i < 4; ++i)
            if (c_is_toolbar_container_filled(i) && !c_is_toolbar_container_visible(i)) return Qfalse;
        return Qtrue;
    }
    else
        rb_raise(rb_eTypeError, "Given bar value is invalid! Expected a Fixnum value between 1 and 7.");
}

VALUE AMS::Sketchup::rbf_is_toolbar_container_filled(VALUE self, VALUE v_bar) {
    int bar = RU::value_to_int(v_bar);
    if (bar >= 1 && bar <= 4)
        return RU::to_value(c_is_toolbar_container_filled(bar - 1));
    else if (bar == 5) {
        for (int i = 0; i < 4; ++i)
            if (c_is_toolbar_container_filled(i)) return Qtrue;
        return Qfalse;
    }
    else if (bar == 6) {
        for (int i = 0; i < 4; ++i)
            if (!c_is_toolbar_container_filled(i)) return Qfalse;
        return Qtrue;
    }
    else
        rb_raise(rb_eTypeError, "Given bar value is invalid! Expected a Fixnum value between 1 and 6.");
}

VALUE AMS::Sketchup::rbf_is_toolbar_container_empty(VALUE self, VALUE v_bar) {
    int bar = RU::value_to_int(v_bar);
    if (bar >= 1 && bar <= 4)
        return RU::to_value(c_is_toolbar_container_empty(bar - 1));
    else if (bar == 5) {
        for (int i = 0; i < 4; ++i)
            if (c_is_toolbar_container_empty(i)) return Qtrue;
        return Qfalse;
    }
    else if (bar == 6) {
        for (int i = 0; i < 4; ++i)
            if (!c_is_toolbar_container_empty(i)) return Qfalse;
        return Qtrue;
    }
    else
        rb_raise(rb_eTypeError, "Given bar value is invalid! Expected a Fixnum value between 1 and 6.");
}

VALUE AMS::Sketchup::rbf_get_viewport_rect(VALUE self) {
    RECT rect;
    c_get_viewport_rect(rect);
    VALUE v_rect = rb_ary_new2(4);
    rb_ary_store(v_rect, 0, RU::to_value(rect.left));
    rb_ary_store(v_rect, 1, RU::to_value(rect.top));
    rb_ary_store(v_rect, 2, RU::to_value(rect.right));
    rb_ary_store(v_rect, 3, RU::to_value(rect.bottom));
    return v_rect;
}

VALUE AMS::Sketchup::rbf_get_viewport_origin(VALUE self) {
    RECT rect;
    c_get_viewport_rect(rect);
    VALUE v_rect = rb_ary_new2(2);
    rb_ary_store(v_rect, 0, RU::to_value(rect.left));
    rb_ary_store(v_rect, 1, RU::to_value(rect.top));
    return v_rect;
}

VALUE AMS::Sketchup::rbf_get_viewport_size(VALUE self) {
    RECT rect;
    c_get_viewport_rect(rect);
    VALUE v_rect = rb_ary_new2(2);
    rb_ary_store(v_rect, 0, RU::to_value(rect.right - rect.left));
    rb_ary_store(v_rect, 1, RU::to_value(rect.bottom - rect.top));
    return v_rect;
}

VALUE AMS::Sketchup::rbf_get_viewport_center(VALUE self) {
    RECT rect;
    c_get_viewport_rect(rect);
    VALUE v_rect = rb_ary_new2(2);
    rb_ary_store(v_rect, 0, RU::to_value((rect.right - rect.left) / 2));
    rb_ary_store(v_rect, 1, RU::to_value((rect.bottom - rect.top) / 2));
    return v_rect;
}

VALUE AMS::Sketchup::rbf_show_dialogs(VALUE self, VALUE v_state) {
    bool state = RU::value_to_bool(v_state);
    unsigned int count = 0;
    std::set<HWND> dialogs;
    if (state) {
        for (std::set<HWND>::iterator it = su_active_dialogs.begin(); it != su_active_dialogs.end(); ++it) {
            if (IsWindow(*it) == FALSE ||
                su_dialogs_to_ignore.find(*it) != su_dialogs_to_ignore.end() ||
                IsWindowVisible(*it) == TRUE)
                continue;
            bool found = false;
            if (RU::su_version < 16) {
                int len = GetWindowTextLength(*it) + 1;
                wchar_t* text = new wchar_t[len];
                GetWindowText(*it, text, len);
                for (std::vector<const wchar_t*>::iterator it2 = su_controlled_dialogs.begin(); it2 != su_controlled_dialogs.end(); ++it2) {
                    if (wcscmp(*it2, text) == 0) {
                        found = true;
                        dialogs.insert(*it);
                        break;
                    }
                }
                delete[] text;
            }
            if (!found) ShowWindow(*it, SW_SHOWNA);
            ++count;
        }
        if (!dialogs.empty()) {
            SendMessage(su_main_window, WM_COMMAND, 21926, 0);
            for (std::set<HWND>::iterator it = dialogs.begin(); it != dialogs.end(); ++it) {
                if (IsWindow(*it) == TRUE && IsWindowVisible(*it) == FALSE) {
                    SendMessage(su_main_window, WM_COMMAND, 21926, 0);
                    break;
                }
            }
        }
        su_active_dialogs.clear();
    }
    else {
        std::set<HWND> visible_dialogs;
        c_get_visible_dialogs(visible_dialogs);
        for (std::set<HWND>::iterator it = visible_dialogs.begin(); it != visible_dialogs.end(); ++it) {
            su_active_dialogs.insert(*it);
            bool found = false;
            if (RU::su_version < 16) {
                int len = GetWindowTextLength(*it) + 1;
                wchar_t* text = new wchar_t[len];
                GetWindowText(*it, text, len);
                for (std::vector<const wchar_t*>::iterator it2 = su_controlled_dialogs.begin(); it2 != su_controlled_dialogs.end(); ++it2) {
                    if (wcscmp(*it2, text) == 0) {
                        found = true;
                        dialogs.insert(*it);
                        break;
                    }
                }
                delete[] text;
            }
            if (!found) ShowWindow(*it, SW_HIDE);
            ++count;
        }
        if (!dialogs.empty()) {
            SendMessage(su_main_window, WM_COMMAND, 21926, 0);
            for (std::set<HWND>::iterator it = dialogs.begin(); it != dialogs.end(); ++it) {
                if (IsWindow(*it) == TRUE && IsWindowVisible(*it) == TRUE) {
                    SendMessage(su_main_window, WM_COMMAND, 21926, 0);
                    break;
                }
            }
        }
    }
    count += c_show_trays(state);
    return RU::to_value(count);
}

VALUE AMS::Sketchup::rbf_close_dialogs(VALUE self) {
    unsigned int count = 0;
    for (std::set<HWND>::iterator it = su_active_dialogs.begin(); it != su_active_dialogs.end(); ++it) {
        if (IsWindow(*it) == TRUE &&
            su_dialogs_to_ignore.find(*it) == su_dialogs_to_ignore.end()) {
            PostMessage(*it, WM_SYSCOMMAND, SC_CLOSE, 0);
            ++count;
        }
    }
    su_active_dialogs.clear();
    std::set<HWND> visible_dialogs;
    c_get_visible_dialogs(visible_dialogs);
    for (std::set<HWND>::iterator it = visible_dialogs.begin(); it != visible_dialogs.end(); ++it) {
        PostMessage(*it, WM_SYSCOMMAND, SC_CLOSE, 0);
        ++count;
    }
    count += c_close_trays();
    return RU::to_value(count);
}

VALUE AMS::Sketchup::rbf_include_dialog(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle, false);
    if (su_dialogs_to_ignore.find(handle) != su_dialogs_to_ignore.end()) {
        su_dialogs_to_ignore.erase(handle);
        return Qtrue;
    }
    else
        return Qfalse;
}

VALUE AMS::Sketchup::rbf_ignore_dialog(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid == su_process_id) {
        su_dialogs_to_ignore.insert(handle);
        return Qtrue;
    }
    else
        return Qfalse;
}

VALUE AMS::Sketchup::rbf_show_trays(VALUE self, VALUE v_state) {
    bool state = RU::value_to_bool(v_state);
    return RU::to_value(c_show_trays(state));
}

VALUE AMS::Sketchup::rbf_close_trays(VALUE self) {
    return RU::to_value(c_close_trays());
}

VALUE AMS::Sketchup::rbf_show_toolbars(VALUE self, VALUE v_state) {
    bool state = RU::value_to_bool(v_state);
    int count = 0;
    if (state) {
        for (std::set<HWND>::iterator it = su_active_toolbars.begin(); it != su_active_toolbars.end(); ++it) {
            if (IsWindow(*it) == TRUE &&
                su_toolbars_to_ignore.find(*it) == su_toolbars_to_ignore.end()) {
                ShowWindow(*it, SW_SHOWNA);
                ++count;
            }
        }
        su_active_toolbars.clear();
    }
    else {
        std::set<HWND> visible_toolbars;
        c_get_visible_toolbars(visible_toolbars);
        for (std::set<HWND>::iterator it = visible_toolbars.begin(); it != visible_toolbars.end(); ++it) {
            su_active_toolbars.insert(*it);
            ShowWindow(*it, SW_HIDE);
            ++count;
        }
    }
    return RU::to_value(count);
}

VALUE AMS::Sketchup::rbf_close_toolbars(VALUE self) {
    unsigned int count = 0;
    for (std::set<HWND>::iterator it = su_active_toolbars.begin(); it != su_active_toolbars.end(); ++it) {
        if (IsWindow(*it) == TRUE &&
            su_toolbars_to_ignore.find(*it) == su_toolbars_to_ignore.end()) {
            PostMessage(*it, WM_SYSCOMMAND, SC_CLOSE, 0);
            ++count;
        }
    }
    su_active_toolbars.clear();
    std::set<HWND> visible_toolbars;
    c_get_visible_toolbars(visible_toolbars);
    for (std::set<HWND>::iterator it = visible_toolbars.begin(); it != visible_toolbars.end(); ++it) {
        PostMessage(*it, WM_SYSCOMMAND, SC_CLOSE, 0);
        ++count;
    }
    return RU::to_value(count);
}

VALUE AMS::Sketchup::rbf_include_toolbar(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle, false);
    if (su_toolbars_to_ignore.find(handle) != su_toolbars_to_ignore.end()) {
        su_toolbars_to_ignore.erase(handle);
        return Qtrue;
    }
    else
        return Qfalse;
}

VALUE AMS::Sketchup::rbf_ignore_toolbar(VALUE self, VALUE v_handle) {
    HWND handle = RU::value_to_hwnd(v_handle);
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    if (pid == su_process_id) {
        su_toolbars_to_ignore.insert(handle);
        return Qtrue;
    }
    else
        return Qfalse;
}

VALUE AMS::Sketchup::rbf_is_main_window_active(VALUE self) {
    return (GetActiveWindow() == su_main_window) ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_is_active(VALUE self) {
    HWND handle = GetActiveWindow();
    DWORD pid;
    GetWindowThreadProcessId(handle, &pid);
    return (pid == su_process_id) ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_find_window_by_caption(int argc, VALUE* argv, VALUE self) {
    SearchInfo info;
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

VALUE AMS::Sketchup::rbf_find_child_window_by_caption(int argc, VALUE* argv, VALUE self) {
    ChildSearchInfo info;
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
    DWORD pid;
    GetWindowThreadProcessId(info.parent_handle, &pid);
    VALUE v_result;
    if (pid == su_process_id) {
        EnumChildWindows(info.parent_handle, FindChildWindowProc, reinterpret_cast<LPARAM>(&info));
        v_result = RU::to_value(info.found_handle);
    }
    else
        v_result = Qnil;
    delete[] info.text;
    return v_result;
}

VALUE AMS::Sketchup::rbf_find_window_by_class_name(int argc, VALUE* argv, VALUE self) {
    SearchInfo info;
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

VALUE AMS::Sketchup::rbf_find_child_window_by_class_name(int argc, VALUE* argv, VALUE self) {
    ChildSearchInfo info;
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
    DWORD pid;
    GetWindowThreadProcessId(info.parent_handle, &pid);
    VALUE v_result;
    if (pid == su_process_id) {
        EnumChildWindows(info.parent_handle, FindChildWindowProc, reinterpret_cast<LPARAM>(&info));
        v_result = RU::to_value(info.found_handle);
    }
    else
        v_result = Qnil;
    delete[] info.text;
    return v_result;
}

VALUE AMS::Sketchup::rbf_get_dialogs(VALUE self) {
    std::set<HWND> dialogs;
    c_get_dialogs(dialogs);

    std::set<HWND> trays;
    EnumWindows(GetFloatingTraysProc, reinterpret_cast<LPARAM>(&trays));
    EnumChildWindows(su_main_window, GetDockedTraysProc, reinterpret_cast<LPARAM>(&trays));

    for (std::set<HWND>::iterator it = trays.begin(); it != trays.end(); ++it) {
        HWND parent = GetAncestor(*it, GA_PARENT);
        if (parent == su_main_window)
            dialogs.insert(*it);
        else
            dialogs.insert(parent);
    }

    VALUE v_dialogs = rb_ary_new2(static_cast<unsigned int>(dialogs.size()));
    unsigned int i = 0;
    for (std::set<HWND>::iterator it = dialogs.begin(); it != dialogs.end(); ++it) {
        rb_ary_store(v_dialogs, i, RU::to_value(*it));
        ++i;
    }

    return v_dialogs;
}

VALUE AMS::Sketchup::rbf_get_active_dialogs(VALUE self) {
    std::set<HWND> dialogs;
    c_get_visible_dialogs(dialogs);
    for (std::set<HWND>::iterator it = su_active_dialogs.begin(); it != su_active_dialogs.end(); ++it) {
        if (IsWindow(*it) == TRUE &&
            su_dialogs_to_ignore.find(*it) == su_dialogs_to_ignore.end())
            dialogs.insert(*it);
    }

    std::set<HWND> trays;
    EnumWindows(GetVisibleFloatingTraysProc, reinterpret_cast<LPARAM>(&trays));
    EnumChildWindows(su_main_window, GetVisibleDockedTraysProc, reinterpret_cast<LPARAM>(&trays));
    c_get_hidden_tabbed_trays(trays);

    for (std::set<HWND>::iterator it = su_active_trays.begin(); it != su_active_trays.end(); ++it) {
        if (IsWindow(*it) == TRUE)
            trays.insert(*it);
    }

    for (std::set<HWND>::iterator it = trays.begin(); it != trays.end(); ++it) {
        HWND parent = GetAncestor(*it, GA_PARENT);
        if (parent == su_main_window)
            dialogs.insert(*it);
        else
            dialogs.insert(parent);
    }

    VALUE v_dialogs = rb_ary_new2(static_cast<unsigned int>(dialogs.size()));
    unsigned int i = 0;
    for (std::set<HWND>::iterator it = dialogs.begin(); it != dialogs.end(); ++it) {
        rb_ary_store(v_dialogs, i, RU::to_value(*it));
        ++i;
    }

    return v_dialogs;
}

VALUE AMS::Sketchup::rbf_get_visible_dialogs(VALUE self) {
    std::set<HWND> dialogs;
    c_get_visible_dialogs(dialogs);

    std::set<HWND> trays;
    EnumWindows(GetVisibleFloatingTraysProc, reinterpret_cast<LPARAM>(&trays));
    EnumChildWindows(su_main_window, GetVisibleDockedTraysProc, reinterpret_cast<LPARAM>(&trays));
    c_get_hidden_tabbed_trays(trays);

    for (std::set<HWND>::iterator it = trays.begin(); it != trays.end(); ++it) {
        HWND parent = GetAncestor(*it, GA_PARENT);
        if (parent == su_main_window)
            dialogs.insert(*it);
        else
            dialogs.insert(parent);
    }

    VALUE v_dialogs = rb_ary_new2(static_cast<unsigned int>(dialogs.size()));
    unsigned int i = 0;
    for (std::set<HWND>::iterator it = dialogs.begin(); it != dialogs.end(); ++it) {
        rb_ary_store(v_dialogs, i, RU::to_value(*it));
        ++i;
    }

    return v_dialogs;
}

VALUE AMS::Sketchup::rbf_get_toolbars(VALUE self) {
    std::set<HWND> toolbars;
    c_get_toolbars(toolbars);

    VALUE v_toolbars = rb_ary_new2(static_cast<unsigned int>(toolbars.size()));
    unsigned int i = 0;
    for (std::set<HWND>::iterator it = toolbars.begin(); it != toolbars.end(); ++it) {
        rb_ary_store(v_toolbars, i, RU::to_value(*it));
        ++i;
    }

    return v_toolbars;
}

VALUE AMS::Sketchup::rbf_get_active_toolbars(VALUE self) {
    std::set<HWND> visible_toolbars;
    c_get_visible_toolbars(visible_toolbars);

    VALUE v_active_toolbars = rb_ary_new();

    for (std::set<HWND>::iterator it = visible_toolbars.begin(); it != visible_toolbars.end(); ++it)
        rb_ary_push(v_active_toolbars, RU::to_value(*it));

    for (std::set<HWND>::iterator it = su_active_toolbars.begin(); it != su_active_toolbars.end(); ++it) {
        if (su_toolbars_to_ignore.find(*it) == su_toolbars_to_ignore.end() &&
            visible_toolbars.find(*it) == visible_toolbars.end())
            rb_ary_push(v_active_toolbars, RU::to_value(*it));
    }

    return v_active_toolbars;
}

VALUE AMS::Sketchup::rbf_get_visible_toolbars(VALUE self) {
    std::set<HWND> visible_toolbars;
    c_get_visible_toolbars(visible_toolbars);

    VALUE v_visible_toolbars = rb_ary_new2(static_cast<unsigned int>(visible_toolbars.size()));
    unsigned int i = 0;

    for (std::set<HWND>::iterator it = visible_toolbars.begin(); it != visible_toolbars.end(); ++it) {
        rb_ary_store(v_visible_toolbars, i, RU::to_value(*it));
        ++i;
    }

    return v_visible_toolbars;
}

VALUE AMS::Sketchup::rbf_get_caption(VALUE self) {
    int len = GetWindowTextLength(su_main_window);
    TCHAR* text = new TCHAR[len + 1];
    GetWindowText(su_main_window, text, len + 1);
    VALUE v_text = RU::to_value(text);
    delete[] text;
    return v_text;
}

VALUE AMS::Sketchup::rbf_set_caption(VALUE self, VALUE v_caption) {
    wchar_t* text = RU::value_to_wc_str(v_caption);
    BOOL res = SetWindowText(su_main_window, text);
    delete[] text;
    return res == TRUE ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_refresh(VALUE self) {
    return RU::to_value(c_recalc_layout());
}

VALUE AMS::Sketchup::rbf_close(VALUE self) {
    return SendMessage(su_main_window, WM_SYSCOMMAND, SC_CLOSE, 0) == 0 ? Qtrue : Qfalse;
}

VALUE AMS::Sketchup::rbf_add_observer(VALUE self, VALUE v_object) {
    if (rb_ary_includes(su_observers, v_object) == Qtrue)
        return Qfalse;
    else {
        int type = TYPE(v_object);
        if (type == T_CLASS || type == T_MODULE || type == T_OBJECT) {
            rb_ary_push(su_observers, v_object);
            su_observers_size = RARRAY_LEN(su_observers);
            //++su_observers_size;
            c_observers_update_process_info();
            c_call_single_observer_event(v_object, ID_SWO_ACTIVATE, 0, nullptr);
        }
        else
            rb_raise(rb_eTypeError, "Expected a class, a module, or a class instance!");
        return Qtrue;
    }
}

VALUE AMS::Sketchup::rbf_remove_observer(VALUE self, VALUE v_object) {
    if (rb_ary_delete(su_observers, v_object) != Qnil) {
        su_observers_size = RARRAY_LEN(su_observers);
        //--su_observers_size;
        c_observers_update_process_info();
        c_call_single_observer_event(v_object, ID_SWO_DEACTIVATE, 0, nullptr);
        return Qtrue;
    }
    else
        return Qfalse;
}

VALUE AMS::Sketchup::rbf_send_user_message(VALUE self, VALUE v_receiver, VALUE v_id, VALUE v_user_data) {
    HWND receiver = RU::value_to_hwnd(v_receiver);
    UINT id = RU::value_to_uint(v_id);
    if (id < UMESSAGE_MIN || id > UMESSAGE_MAX)
        rb_raise(rb_eRangeError, "ID must be a fixed value between %d and %d.", UMESSAGE_MIN, UMESSAGE_MAX);

    int udata_type = rb_type(v_user_data);
    char* udata2 = nullptr;
    unsigned int detail_size = sizeof(unsigned int) * 2;

    if (udata_type == T_NIL || udata_type == T_TRUE || udata_type == T_FALSE) {
        udata2 = new char[detail_size];
        reinterpret_cast<unsigned int*>(udata2)[0] = UMESSAGE_DATA_ID;
        reinterpret_cast<unsigned int*>(udata2)[1] = (udata_type == T_NIL ? 0 : (udata_type == T_TRUE ? 1 : 2));
    }
    else if (udata_type == T_FIXNUM) {
        udata2 = new char[detail_size + sizeof(int)];
        reinterpret_cast<unsigned int*>(udata2)[0] = UMESSAGE_DATA_ID;
        reinterpret_cast<unsigned int*>(udata2)[1] = 3;
        reinterpret_cast<int*>(udata2 + detail_size)[0] = RU::value_to_int(v_user_data);
    }
    else if (udata_type == T_FLOAT) {
        udata2 = new char[detail_size + sizeof(double)];
        reinterpret_cast<unsigned int*>(udata2)[0] = UMESSAGE_DATA_ID;
        reinterpret_cast<unsigned int*>(udata2)[1] = 4;
        reinterpret_cast<double*>(udata2 + detail_size)[0] = RU::value_to_double(v_user_data);
    }
    else {
        unsigned int udata_type2;
        VALUE v_user_data2;
        if (udata_type == T_BIGNUM) {
            v_user_data2 = rb_funcall(v_user_data, RU::INTERN_TO_S, 0);
            //v_user_data2 = rb_big2str(v_user_data, 10);
            udata_type2 = 5;
        }
        else if (udata_type == T_STRING) {
            v_user_data2 = v_user_data;
            udata_type2 = 6;
        }
        else if (udata_type == T_SYMBOL) {
            v_user_data2 = rb_funcall(v_user_data, RU::INTERN_TO_S, 0);
            udata_type2 = 7;
        }
        else if (udata_type == T_ARRAY) {
            v_user_data2 = AMS::c_inspect_element(v_user_data);
            udata_type2 = 8;
        }
        else if (udata_type == T_HASH) {
            v_user_data2 = AMS::c_inspect_element(v_user_data);
            udata_type2 = 9;
        }
        else {
            VALUE v_element_obj_str = rb_funcall(v_user_data, RU::INTERN_TO_S, 0);
            VALUE v_element_class_str = rb_funcall(rb_class_of(v_user_data), RU::INTERN_TO_S, 0);
            const char* element_obj_str = RU::value_to_c_str(v_element_obj_str);
            const char* element_class_str = RU::value_to_c_str(v_element_class_str);
            rb_raise(rb_eTypeError, "User data, '%s', of type '%s' is not supported! Supported types are Nil, Boolean, Fixnum, Float, Bignum, String, Symbol, Array, and Hash.", element_obj_str, element_class_str);
        }
        char* udata = RU::value_to_c_str(v_user_data2);
        unsigned int udata_size = (unsigned int)RSTRING_LEN(v_user_data2);
        udata2 = new char[detail_size + sizeof(unsigned int) + udata_size];
        reinterpret_cast<unsigned int*>(udata2)[0] = UMESSAGE_DATA_ID;
        reinterpret_cast<unsigned int*>(udata2)[1] = udata_type2;
        reinterpret_cast<unsigned int*>(udata2)[2] = udata_size;
        strcpy(udata2 + sizeof(unsigned int) + detail_size, udata);
    }

    SendMessage(receiver, UMESSAGE_BASE + id, (WPARAM)su_main_window, reinterpret_cast<LPARAM>(udata2));

    if (udata2 != nullptr) delete[] udata2;

    return Qtrue;
}

VALUE AMS::Sketchup::rbf_get_other_main_windows(VALUE self) {
    std::set<HWND> items;
    c_get_other_main_windows(items);

    VALUE v_items = rb_ary_new2(static_cast<unsigned int>(items.size()));
    unsigned int i = 0;

    for (std::set<HWND>::iterator it = items.begin(); it != items.end(); ++it) {
        rb_ary_store(v_items, i, RU::to_value(*it));
        ++i;
    }

    return v_items;
}

VALUE AMS::Sketchup::rbf_threaded_messagebox(int argc, VALUE* argv, VALUE self) {
    HWND handle = NULL;
    UINT type = 0x00002030L;
    if (argc == 4) {
        type = RU::value_to_uint(argv[2]);
        handle = RU::value_to_hwnd(argv[3]);
    }
    else if (argc == 3) {
        type = RU::value_to_uint(argv[2]);
    }
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..4 arguments.");
    if (rb_block_given_p() == 0)
        rb_raise(rb_eArgError, "Expected a block, a messagebox procedure, in addition to all the parameters!");
    wchar_t* caption = RU::value_to_wc_str(argv[0]);
    wchar_t* text = RU::value_to_wc_str(argv[1]);
    MessageboxData* data = new MessageboxData(handle, text, caption, type, rb_block_proc());
    HANDLE thandle = CreateThread(NULL, 0, &ThreadedMessageBox, data, CREATE_SUSPENDED, NULL);
    if (thandle == NULL)
        delete data;
    else {
        EnterCriticalSection(&su_mb_critical_section);
        rb_ary_push(su_messagebox_procedures, data->proc);
        LeaveCriticalSection(&su_mb_critical_section);
        ResumeThread(thandle);
    }
    return RU::to_value(thandle);
}

VALUE AMS::Sketchup::rbf_set_background_color(VALUE self, VALUE v_color) {
    Geom::Color color;
    RU::value_to_color(v_color, color);
    //HBRUSH brush = CreateSolidBrush();
    HBRUSH brush = CreateSolidBrush(RGB(color.m_r, color.m_g, color.m_b));
    SetClassLongPtr(su_main_window, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_viewport, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_control_bars[0], GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_control_bars[1], GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_control_bars[2], GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_control_bars[3], GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_status_bar, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_scenes_bar, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    SetClassLongPtr(su_tab_control, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    DeleteObject(brush);
    return Qnil;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Sketchup::init_ruby(VALUE mAMS) {
    // Initialize interns
    ID_SWO_ACTIVATE                                         = rb_intern("swo_activate");
    ID_SWO_DEACTIVATE                                       = rb_intern("swo_deactivate");
    ID_SWO_ERROR                                            = rb_intern("swo_error");

    ID_SWO_ON_QUIT                                          = rb_intern("swo_on_quit");

    ID_SWO_ON_RESTORE                                       = rb_intern("swo_on_restore");
    ID_SWO_ON_POST_RESTORE                                  = rb_intern("swo_on_post_restore");
    ID_SWO_ON_MINIMIZE                                      = rb_intern("swo_on_minimize");
    ID_SWO_ON_POST_MINIMIZE                                 = rb_intern("swo_on_post_minimize");
    ID_SWO_ON_MAXIMIZE                                      = rb_intern("swo_on_maximize");
    ID_SWO_ON_POST_MAXIMIZE                                 = rb_intern("swo_on_post_maximize");
    ID_SWO_ON_SWITCH_FULL_SCREEN                            = rb_intern("swo_on_switch_full_screen");
    ID_SWO_ON_POST_SWITCH_FULL_SCREEN                       = rb_intern("swo_on_post_switch_full_screen");

    ID_SWO_ON_MENU_BAR_CHANGED                              = rb_intern("swo_on_menu_bar_changed");
    ID_SWO_ON_POST_MENU_BAR_CHANGED                         = rb_intern("swo_on_post_menu_bar_changed");

    ID_SWO_ON_ENTER_MENU                                    = rb_intern("swo_on_enter_menu");
    ID_SWO_ON_POST_ENTER_MENU                               = rb_intern("swo_on_post_enter_menu");
    ID_SWO_ON_EXIT_MENU                                     = rb_intern("swo_on_exit_menu");
    ID_SWO_ON_POST_EXIT_MENU                                = rb_intern("swo_on_post_exit_menu");

    ID_SWO_ON_CAPTION_CHANGED                               = rb_intern("swo_on_caption_changed");
    ID_SWO_ON_POST_CAPTION_CHANGED                          = rb_intern("swo_on_post_caption_changed");

    ID_SWO_ON_FOCUS                                         = rb_intern("swo_on_focus");
    ID_SWO_ON_POST_FOCUS                                    = rb_intern("swo_on_post_focus");
    ID_SWO_ON_BLUR                                          = rb_intern("swo_on_blur");
    ID_SWO_ON_POST_BLUR                                     = rb_intern("swo_on_post_blur");

    ID_SWO_ON_ENTER_SIZE_MOVE                               = rb_intern("swo_on_enter_size_move");
    ID_SWO_ON_POST_ENTER_SIZE_MOVE                          = rb_intern("swo_on_post_enter_size_move");
    ID_SWO_ON_EXIT_SIZE_MOVE                                = rb_intern("swo_on_exit_size_move");
    ID_SWO_ON_POST_EXIT_SIZE_MOVE                           = rb_intern("swo_on_post_exit_size_move");
    ID_SWO_ON_SIZE_MOVE                                     = rb_intern("swo_on_size_move");
    ID_SWO_ON_POST_SIZE_MOVE                                = rb_intern("swo_on_post_size_move");

    ID_SWO_ON_USER_MESSAGE                                  = rb_intern("swo_on_user_message");

    ID_SWP_ON_COMMAND                                       = rb_intern("swp_on_command");

    ID_SWO_ON_VIEWPORT_BORDER_CHANGED                       = rb_intern("swo_on_viewport_border_changed");
    ID_SWO_ON_POST_VIEWPORT_BORDER_CHANGED                  = rb_intern("swo_on_post_viewport_border_changed");

    ID_SWO_ON_VIEWPORT_SIZE                                 = rb_intern("swo_on_viewport_size");
    ID_SWO_ON_POST_VIEWPORT_SIZE                            = rb_intern("swo_on_post_viewport_size");

    ID_SWP_ON_LBUTTON_DOWN                                  = rb_intern("swp_on_lbutton_down");
    ID_SWP_ON_LBUTTON_UP                                    = rb_intern("swp_on_lbutton_up");
    ID_SWP_ON_LBUTTON_DOUBLE_CLICK                          = rb_intern("swp_on_lbutton_double_click");
    ID_SWP_ON_RBUTTON_DOWN                                  = rb_intern("swp_on_rbutton_down");
    ID_SWP_ON_RBUTTON_UP                                    = rb_intern("swp_on_rbutton_up");
    ID_SWP_ON_RBUTTON_DOUBLE_CLICK                          = rb_intern("swp_on_rbutton_double_click");
    ID_SWP_ON_MBUTTON_DOWN                                  = rb_intern("swp_on_mbutton_down");
    ID_SWP_ON_MBUTTON_UP                                    = rb_intern("swp_on_mbutton_up");
    ID_SWP_ON_MBUTTON_DOUBLE_CLICK                          = rb_intern("swp_on_mbutton_double_click");
    ID_SWP_ON_XBUTTON1_DOWN                                 = rb_intern("swp_on_xbutton1_down");
    ID_SWP_ON_XBUTTON1_UP                                   = rb_intern("swp_on_xbutton1_up");
    ID_SWP_ON_XBUTTON1_DOUBLE_CLICK                         = rb_intern("swp_on_xbutton1_double_click");
    ID_SWP_ON_XBUTTON2_DOWN                                 = rb_intern("swp_on_xbutton2_down");
    ID_SWP_ON_XBUTTON2_UP                                   = rb_intern("swp_on_xbutton2_up");
    ID_SWP_ON_XBUTTON2_DOUBLE_CLICK                         = rb_intern("swp_on_xbutton2_double_click");

    ID_SWP_ON_MOUSE_WHEEL_ROTATE                            = rb_intern("swp_on_mouse_wheel_rotate");
    ID_SWP_ON_MOUSE_WHEEL_TILT                              = rb_intern("swp_on_mouse_wheel_tilt");

    ID_SWP_ON_MOUSE_ENTER                                   = rb_intern("swp_on_mouse_enter");
    ID_SWP_ON_MOUSE_LEAVE                                   = rb_intern("swp_on_mouse_leave");
    ID_SWP_ON_MOUSE_MOVE                                    = rb_intern("swp_on_mouse_move");

    ID_SWO_ON_SCENES_BAR_FILLED                             = rb_intern("swo_on_scenes_bar_filled");
    ID_SWO_ON_POST_SCENES_BAR_FILLED                        = rb_intern("swo_on_post_scenes_bar_filled");
    ID_SWO_ON_SCENES_BAR_EMPTIED                            = rb_intern("swo_on_scenes_bar_emptied");
    ID_SWO_ON_POST_SCENES_BAR_EMPTIED                       = rb_intern("swo_on_post_scenes_bar_emptied");

    ID_SWO_ON_TOOLBAR_CONTAINER_FILLED                      = rb_intern("swo_on_toolbar_container_filled");
    ID_SWO_ON_POST_TOOLBAR_CONTAINER_FILLED                 = rb_intern("swo_on_post_toolbar_container_filled");
    ID_SWO_ON_TOOLBAR_CONTAINER_EMPTIED                     = rb_intern("swo_on_toolbar_container_emptied");
    ID_SWO_ON_POST_TOOLBAR_CONTAINER_EMPTIED                = rb_intern("swo_on_post_toolbar_container_emptied");

    ID_SWO_ON_PAGE_SELECTED                                 = rb_intern("swo_on_page_selected");
    ID_SWP_ON_PAGE_SELECTED                                 = rb_intern("swp_on_page_selected");

    ID_SWO_ON_STATUS_BAR_VISIBILITY_CHANGED                 = rb_intern("swo_on_status_bar_visibility_changed");
    ID_SWO_ON_POST_STATUS_BAR_VISIBILITY_CHANGED            = rb_intern("swo_on_post_status_bar_visibility_changed");

    ID_SWO_ON_SCENES_BAR_VISIBILITY_CHANGED                 = rb_intern("swo_on_scenes_bar_visibility_changed");
    ID_SWO_ON_POST_SCENES_BAR_VISIBILITY_CHANGED            = rb_intern("swo_on_post_scenes_bar_visibility_changed");

    ID_SWO_ON_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED         = rb_intern("swo_on_toolbar_container_visibility_changed");
    ID_SWO_ON_POST_TOOLBAR_CONTAINERS_VISIBILITY_CHANGED    = rb_intern("swo_on_post_toolbar_container_visibility_changed");

    ID_SWP_ON_KEY_DOWN                                      = rb_intern("swp_on_key_down");
    ID_SWP_ON_KEY_UP                                        = rb_intern("swp_on_key_up");
    ID_SWP_ON_KEY_EXTENDED                                  = rb_intern("swp_on_key_extended");

    //su_process_id = GetCurrentProcessId(); // May be unsafe
    VALUE v_pid = rb_funcall(rb_define_module("Process"), rb_intern("pid"), 0);
    su_process_id = RU::value_to_ulong(v_pid);

    // Find handle to SU main window.
    EnumWindows(FindMainWindowProc, static_cast<LPARAM>(su_process_id));

    // Get main window module handle.
    su_module_handle = (HMODULE)GetWindowLongPtr(su_main_window, GWLP_HINSTANCE);

    // Get main window menu-bar handle.
    su_menu_bar = GetMenu(su_main_window);

    // Find handles to particular child windows of SU main window.
    su_dock_pane_count = 0;
    EnumChildWindows(su_main_window, FindChildWindowsProc, NULL);

    // Virtual key name by code.
    su_virtual_key_names[0x01] = L"lbutton";
    su_virtual_key_names[0x02] = L"rbutton";
    su_virtual_key_names[0x03] = L"cancel";
    su_virtual_key_names[0x04] = L"mbutton";
    su_virtual_key_names[0x05] = L"xbutton1";
    su_virtual_key_names[0x06] = L"xbutton2";
    su_virtual_key_names[0x08] = L"backspace";
    su_virtual_key_names[0x09] = L"tab";
    su_virtual_key_names[0x0c] = L"clear";
    su_virtual_key_names[0x0d] = L"return";
    su_virtual_key_names[0x10] = L"shift";
    su_virtual_key_names[0x11] = L"control";
    su_virtual_key_names[0x12] = L"menu";
    su_virtual_key_names[0x13] = L"pause";
    su_virtual_key_names[0x14] = L"capslock";
    su_virtual_key_names[0x15] = L"kana";
    su_virtual_key_names[0x15] = L"handuel";
    su_virtual_key_names[0x15] = L"hangul";
    su_virtual_key_names[0x17] = L"junja";
    su_virtual_key_names[0x18] = L"final";
    su_virtual_key_names[0x19] = L"hanja";
    su_virtual_key_names[0x19] = L"hanji";
    su_virtual_key_names[0x1b] = L"escape";
    su_virtual_key_names[0x1c] = L"convert";
    su_virtual_key_names[0x1d] = L"nonconvert";
    su_virtual_key_names[0x1e] = L"accept";
    su_virtual_key_names[0x1f] = L"modechange";
    su_virtual_key_names[0x20] = L"space";
    su_virtual_key_names[0x21] = L"pageup";
    su_virtual_key_names[0x22] = L"pagedown";
    su_virtual_key_names[0x23] = L"end";
    su_virtual_key_names[0x24] = L"home";
    su_virtual_key_names[0x25] = L"left";
    su_virtual_key_names[0x26] = L"up";
    su_virtual_key_names[0x27] = L"right";
    su_virtual_key_names[0x28] = L"down";
    su_virtual_key_names[0x29] = L"select";
    su_virtual_key_names[0x2a] = L"print";
    su_virtual_key_names[0x2b] = L"execute";
    su_virtual_key_names[0x2c] = L"snapshot";
    su_virtual_key_names[0x2d] = L"insert";
    su_virtual_key_names[0x2e] = L"delete";
    su_virtual_key_names[0x2f] = L"help";
    su_virtual_key_names[0x30] = L"0";
    su_virtual_key_names[0x31] = L"1";
    su_virtual_key_names[0x32] = L"2";
    su_virtual_key_names[0x33] = L"3";
    su_virtual_key_names[0x34] = L"4";
    su_virtual_key_names[0x35] = L"5";
    su_virtual_key_names[0x36] = L"6";
    su_virtual_key_names[0x37] = L"7";
    su_virtual_key_names[0x38] = L"8";
    su_virtual_key_names[0x39] = L"9";
    su_virtual_key_names[0x41] = L"a";
    su_virtual_key_names[0x42] = L"b";
    su_virtual_key_names[0x43] = L"c";
    su_virtual_key_names[0x44] = L"d";
    su_virtual_key_names[0x45] = L"e";
    su_virtual_key_names[0x46] = L"f";
    su_virtual_key_names[0x47] = L"g";
    su_virtual_key_names[0x48] = L"h";
    su_virtual_key_names[0x49] = L"i";
    su_virtual_key_names[0x4a] = L"j";
    su_virtual_key_names[0x4b] = L"k";
    su_virtual_key_names[0x4c] = L"l";
    su_virtual_key_names[0x4d] = L"m";
    su_virtual_key_names[0x4e] = L"n";
    su_virtual_key_names[0x4f] = L"o";
    su_virtual_key_names[0x50] = L"p";
    su_virtual_key_names[0x51] = L"q";
    su_virtual_key_names[0x52] = L"r";
    su_virtual_key_names[0x53] = L"s";
    su_virtual_key_names[0x54] = L"t";
    su_virtual_key_names[0x55] = L"u";
    su_virtual_key_names[0x56] = L"v";
    su_virtual_key_names[0x57] = L"w";
    su_virtual_key_names[0x58] = L"x";
    su_virtual_key_names[0x59] = L"y";
    su_virtual_key_names[0x5a] = L"z";
    su_virtual_key_names[0x5b] = L"lwin";
    su_virtual_key_names[0x5c] = L"rwin";
    su_virtual_key_names[0x5d] = L"apps";
    su_virtual_key_names[0x5f] = L"sleep";
    su_virtual_key_names[0x60] = L"numpad0";
    su_virtual_key_names[0x61] = L"numpad1";
    su_virtual_key_names[0x62] = L"numpad2";
    su_virtual_key_names[0x63] = L"numpad3";
    su_virtual_key_names[0x64] = L"numpad4";
    su_virtual_key_names[0x65] = L"numpad5";
    su_virtual_key_names[0x66] = L"numpad6";
    su_virtual_key_names[0x67] = L"numpad7";
    su_virtual_key_names[0x68] = L"numpad8";
    su_virtual_key_names[0x69] = L"numpad9";
    su_virtual_key_names[0x6a] = L"numpadmultiply";
    su_virtual_key_names[0x6b] = L"numpadplus";
    su_virtual_key_names[0x6c] = L"separator";
    su_virtual_key_names[0x6d] = L"numpadminus";
    su_virtual_key_names[0x6e] = L"numpaddecimal";
    su_virtual_key_names[0x6f] = L"numpaddivide";
    su_virtual_key_names[0x70] = L"f1";
    su_virtual_key_names[0x71] = L"f2";
    su_virtual_key_names[0x72] = L"f3";
    su_virtual_key_names[0x73] = L"f4";
    su_virtual_key_names[0x74] = L"f5";
    su_virtual_key_names[0x75] = L"f6";
    su_virtual_key_names[0x76] = L"f7";
    su_virtual_key_names[0x77] = L"f8";
    su_virtual_key_names[0x78] = L"f9";
    su_virtual_key_names[0x79] = L"f10";
    su_virtual_key_names[0x7a] = L"f11";
    su_virtual_key_names[0x7b] = L"f12";
    su_virtual_key_names[0x7c] = L"f13";
    su_virtual_key_names[0x7d] = L"f14";
    su_virtual_key_names[0x7e] = L"f15";
    su_virtual_key_names[0x7f] = L"f16";
    su_virtual_key_names[0x80] = L"f17";
    su_virtual_key_names[0x81] = L"f18";
    su_virtual_key_names[0x82] = L"f19";
    su_virtual_key_names[0x83] = L"f20";
    su_virtual_key_names[0x84] = L"f21";
    su_virtual_key_names[0x85] = L"f22";
    su_virtual_key_names[0x86] = L"f23";
    su_virtual_key_names[0x87] = L"f24";
    su_virtual_key_names[0x90] = L"numlock";
    su_virtual_key_names[0x91] = L"scroll";
    su_virtual_key_names[0xa0] = L"lshift";
    su_virtual_key_names[0xa1] = L"rshift";
    su_virtual_key_names[0xa2] = L"lcontrol";
    su_virtual_key_names[0xa3] = L"rcontrol";
    su_virtual_key_names[0xa4] = L"lmenu";
    su_virtual_key_names[0xa5] = L"rmenu";
    su_virtual_key_names[0xa6] = L"browserback";
    su_virtual_key_names[0xa7] = L"browserforward";
    su_virtual_key_names[0xa8] = L"browserrefresh";
    su_virtual_key_names[0xa9] = L"browserstop";
    su_virtual_key_names[0xaa] = L"browsersearch";
    su_virtual_key_names[0xab] = L"browserfavorites";
    su_virtual_key_names[0xac] = L"browserhome";
    su_virtual_key_names[0xad] = L"mute";
    su_virtual_key_names[0xae] = L"volumedown";
    su_virtual_key_names[0xaf] = L"volumeup";
    su_virtual_key_names[0xb0] = L"medianexttrack";
    su_virtual_key_names[0xb1] = L"mediaprevtrack";
    su_virtual_key_names[0xb2] = L"mediastop";
    su_virtual_key_names[0xb3] = L"mediaplaypause";
    su_virtual_key_names[0xb4] = L"launchmail";
    su_virtual_key_names[0xb5] = L"launchmediaselect";
    su_virtual_key_names[0xb6] = L"launchapp1";
    su_virtual_key_names[0xb7] = L"launchapp2";
    su_virtual_key_names[0xba] = L"semicolon";
    su_virtual_key_names[0xbb] = L"equals";
    su_virtual_key_names[0xbc] = L"comma";
    su_virtual_key_names[0xbd] = L"minus";
    su_virtual_key_names[0xbe] = L"period";
    su_virtual_key_names[0xbf] = L"slash";
    su_virtual_key_names[0xc0] = L"grave";
    su_virtual_key_names[0xdb] = L"leftbracket";
    su_virtual_key_names[0xdc] = L"backslash";
    su_virtual_key_names[0xdd] = L"rightbracket";
    su_virtual_key_names[0xde] = L"quote";
    su_virtual_key_names[0xdf] = L"oem8";
    su_virtual_key_names[0xf6] = L"attn";
    su_virtual_key_names[0xf7] = L"crsel";
    su_virtual_key_names[0xf8] = L"exsel";
    su_virtual_key_names[0xf9] = L"ereof";
    su_virtual_key_names[0xfa] = L"play";
    su_virtual_key_names[0xfb] = L"zoom";
    su_virtual_key_names[0xfd] = L"pa1";
    su_virtual_key_names[0xfe] = L"oemclear";

    // Virtual key code by name
    su_virtual_key_codes[L"lbutton"]            = 0x01;
    su_virtual_key_codes[L"rbutton"]            = 0x02;
    su_virtual_key_codes[L"cancel"]             = 0x03;
    su_virtual_key_codes[L"mbutton"]            = 0x04;
    su_virtual_key_codes[L"xbutton1"]           = 0x05;
    su_virtual_key_codes[L"xbutton2"]           = 0x06;
    su_virtual_key_codes[L"backspace"]          = 0x08;
    su_virtual_key_codes[L"tab"]                = 0x09;
    su_virtual_key_codes[L"clear"]              = 0x0c;
    su_virtual_key_codes[L"return"]             = 0x0d;
    su_virtual_key_codes[L"shift"]              = 0x10;
    su_virtual_key_codes[L"control"]            = 0x11;
    su_virtual_key_codes[L"menu"]               = 0x12;
    su_virtual_key_codes[L"pause"]              = 0x13;
    su_virtual_key_codes[L"capslock"]           = 0x14;
    su_virtual_key_codes[L"kana"]               = 0x15;
    su_virtual_key_codes[L"handuel"]            = 0x15;
    su_virtual_key_codes[L"hangul"]             = 0x15;
    su_virtual_key_codes[L"junja"]              = 0x17;
    su_virtual_key_codes[L"final"]              = 0x18;
    su_virtual_key_codes[L"hanja"]              = 0x19;
    su_virtual_key_codes[L"hanji"]              = 0x19;
    su_virtual_key_codes[L"escape"]             = 0x1b;
    su_virtual_key_codes[L"convert"]            = 0x1c;
    su_virtual_key_codes[L"nonconvert"]         = 0x1d;
    su_virtual_key_codes[L"accept"]             = 0x1e;
    su_virtual_key_codes[L"modechange"]         = 0x1f;
    su_virtual_key_codes[L"space"]              = 0x20;
    su_virtual_key_codes[L"pageup"]             = 0x21;
    su_virtual_key_codes[L"pagedown"]           = 0x22;
    su_virtual_key_codes[L"end"]                = 0x23;
    su_virtual_key_codes[L"home"]               = 0x24;
    su_virtual_key_codes[L"left"]               = 0x25;
    su_virtual_key_codes[L"up"]                 = 0x26;
    su_virtual_key_codes[L"right"]              = 0x27;
    su_virtual_key_codes[L"down"]               = 0x28;
    su_virtual_key_codes[L"select"]             = 0x29;
    su_virtual_key_codes[L"print"]              = 0x2a;
    su_virtual_key_codes[L"execute"]            = 0x2b;
    su_virtual_key_codes[L"snapshot"]           = 0x2c;
    su_virtual_key_codes[L"insert"]             = 0x2d;
    su_virtual_key_codes[L"delete"]             = 0x2e;
    su_virtual_key_codes[L"help"]               = 0x2f;
    su_virtual_key_codes[L"0"]                  = 0x30;
    su_virtual_key_codes[L"1"]                  = 0x31;
    su_virtual_key_codes[L"2"]                  = 0x32;
    su_virtual_key_codes[L"3"]                  = 0x33;
    su_virtual_key_codes[L"4"]                  = 0x34;
    su_virtual_key_codes[L"5"]                  = 0x35;
    su_virtual_key_codes[L"6"]                  = 0x36;
    su_virtual_key_codes[L"7"]                  = 0x37;
    su_virtual_key_codes[L"8"]                  = 0x38;
    su_virtual_key_codes[L"9"]                  = 0x39;
    su_virtual_key_codes[L"a"]                  = 0x41;
    su_virtual_key_codes[L"b"]                  = 0x42;
    su_virtual_key_codes[L"c"]                  = 0x43;
    su_virtual_key_codes[L"d"]                  = 0x44;
    su_virtual_key_codes[L"e"]                  = 0x45;
    su_virtual_key_codes[L"f"]                  = 0x46;
    su_virtual_key_codes[L"g"]                  = 0x47;
    su_virtual_key_codes[L"h"]                  = 0x48;
    su_virtual_key_codes[L"i"]                  = 0x49;
    su_virtual_key_codes[L"j"]                  = 0x4a;
    su_virtual_key_codes[L"k"]                  = 0x4b;
    su_virtual_key_codes[L"l"]                  = 0x4c;
    su_virtual_key_codes[L"m"]                  = 0x4d;
    su_virtual_key_codes[L"n"]                  = 0x4e;
    su_virtual_key_codes[L"o"]                  = 0x4f;
    su_virtual_key_codes[L"p"]                  = 0x50;
    su_virtual_key_codes[L"q"]                  = 0x51;
    su_virtual_key_codes[L"r"]                  = 0x52;
    su_virtual_key_codes[L"s"]                  = 0x53;
    su_virtual_key_codes[L"t"]                  = 0x54;
    su_virtual_key_codes[L"u"]                  = 0x55;
    su_virtual_key_codes[L"v"]                  = 0x56;
    su_virtual_key_codes[L"w"]                  = 0x57;
    su_virtual_key_codes[L"x"]                  = 0x58;
    su_virtual_key_codes[L"y"]                  = 0x59;
    su_virtual_key_codes[L"z"]                  = 0x5a;
    su_virtual_key_codes[L"lwin"]               = 0x5b;
    su_virtual_key_codes[L"rwin"]               = 0x5c;
    su_virtual_key_codes[L"apps"]               = 0x5d;
    su_virtual_key_codes[L"sleep"]              = 0x5f;
    su_virtual_key_codes[L"numpad0"]            = 0x60;
    su_virtual_key_codes[L"numpad1"]            = 0x61;
    su_virtual_key_codes[L"numpad2"]            = 0x62;
    su_virtual_key_codes[L"numpad3"]            = 0x63;
    su_virtual_key_codes[L"numpad4"]            = 0x64;
    su_virtual_key_codes[L"numpad5"]            = 0x65;
    su_virtual_key_codes[L"numpad6"]            = 0x66;
    su_virtual_key_codes[L"numpad7"]            = 0x67;
    su_virtual_key_codes[L"numpad8"]            = 0x68;
    su_virtual_key_codes[L"numpad9"]            = 0x69;
    su_virtual_key_codes[L"numpadmultiply"]     = 0x6a;
    su_virtual_key_codes[L"numpadplus"]         = 0x6b;
    su_virtual_key_codes[L"separator"]          = 0x6c;
    su_virtual_key_codes[L"numpadminus"]        = 0x6d;
    su_virtual_key_codes[L"numpaddecimal"]      = 0x6e;
    su_virtual_key_codes[L"numpaddivide"]       = 0x6f;
    su_virtual_key_codes[L"f1"]                 = 0x70;
    su_virtual_key_codes[L"f2"]                 = 0x71;
    su_virtual_key_codes[L"f3"]                 = 0x72;
    su_virtual_key_codes[L"f4"]                 = 0x73;
    su_virtual_key_codes[L"f5"]                 = 0x74;
    su_virtual_key_codes[L"f6"]                 = 0x75;
    su_virtual_key_codes[L"f7"]                 = 0x76;
    su_virtual_key_codes[L"f8"]                 = 0x77;
    su_virtual_key_codes[L"f9"]                 = 0x78;
    su_virtual_key_codes[L"f10"]                = 0x79;
    su_virtual_key_codes[L"f11"]                = 0x7a;
    su_virtual_key_codes[L"f12"]                = 0x7b;
    su_virtual_key_codes[L"f13"]                = 0x7c;
    su_virtual_key_codes[L"f14"]                = 0x7d;
    su_virtual_key_codes[L"f15"]                = 0x7e;
    su_virtual_key_codes[L"f16"]                = 0x7f;
    su_virtual_key_codes[L"f17"]                = 0x80;
    su_virtual_key_codes[L"f18"]                = 0x81;
    su_virtual_key_codes[L"f19"]                = 0x82;
    su_virtual_key_codes[L"f20"]                = 0x83;
    su_virtual_key_codes[L"f21"]                = 0x84;
    su_virtual_key_codes[L"f22"]                = 0x85;
    su_virtual_key_codes[L"f23"]                = 0x86;
    su_virtual_key_codes[L"f24"]                = 0x87;
    su_virtual_key_codes[L"numlock"]            = 0x90;
    su_virtual_key_codes[L"scroll"]             = 0x91;
    su_virtual_key_codes[L"lshift"]             = 0xa0;
    su_virtual_key_codes[L"rshift"]             = 0xa1;
    su_virtual_key_codes[L"lcontrol"]           = 0xa2;
    su_virtual_key_codes[L"rcontrol"]           = 0xa3;
    su_virtual_key_codes[L"lmenu"]              = 0xa4;
    su_virtual_key_codes[L"rmenu"]              = 0xa5;
    su_virtual_key_codes[L"browserback"]        = 0xa6;
    su_virtual_key_codes[L"browserforward"]     = 0xa7;
    su_virtual_key_codes[L"browserrefresh"]     = 0xa8;
    su_virtual_key_codes[L"browserstop"]        = 0xa9;
    su_virtual_key_codes[L"browsersearch"]      = 0xaa;
    su_virtual_key_codes[L"browserfavorites"]   = 0xab;
    su_virtual_key_codes[L"browserhome"]        = 0xac;
    su_virtual_key_codes[L"mute"]               = 0xad;
    su_virtual_key_codes[L"volumedown"]         = 0xae;
    su_virtual_key_codes[L"volumeup"]           = 0xaf;
    su_virtual_key_codes[L"medianexttrack"]     = 0xb0;
    su_virtual_key_codes[L"mediaprevtrack"]     = 0xb1;
    su_virtual_key_codes[L"mediastop"]          = 0xb2;
    su_virtual_key_codes[L"mediaplaypause"]     = 0xb3;
    su_virtual_key_codes[L"launchmail"]         = 0xb4;
    su_virtual_key_codes[L"launchmediaselect"]  = 0xb5;
    su_virtual_key_codes[L"launchapp1"]         = 0xb6;
    su_virtual_key_codes[L"launchapp2"]         = 0xb7;
    su_virtual_key_codes[L"semicolon"]          = 0xba; // US keyboard : ;
    su_virtual_key_codes[L"equals"]             = 0xbb; // US keyboard + =
    su_virtual_key_codes[L"comma"]              = 0xbc; // US keyboard < ,
    su_virtual_key_codes[L"minus"]              = 0xbd; // US keyboard _ -
    su_virtual_key_codes[L"period"]             = 0xbe; // US keyboard > .
    su_virtual_key_codes[L"slash"]              = 0xbf; // US keyboard ? /
    su_virtual_key_codes[L"grave"]              = 0xc0; // US keyboard ~ `
    su_virtual_key_codes[L"leftbracket"]        = 0xdb; // US keyboard { [
    su_virtual_key_codes[L"backslash"]          = 0xdc; // US keyboard |
    su_virtual_key_codes[L"rightbracket"]       = 0xdd; // US keyboard } ]
    su_virtual_key_codes[L"quote"]              = 0xde; // US keyboard " '
    su_virtual_key_codes[L"oem8"]               = 0xdf;
    su_virtual_key_codes[L"attn"]               = 0xf6;
    su_virtual_key_codes[L"crsel"]              = 0xf7;
    su_virtual_key_codes[L"exsel"]              = 0xf8;
    su_virtual_key_codes[L"ereof"]              = 0xf9;
    su_virtual_key_codes[L"play"]               = 0xfa;
    su_virtual_key_codes[L"zoom"]               = 0xfb;
    su_virtual_key_codes[L"pa1"]                = 0xfd;
    su_virtual_key_codes[L"oemclear"]           = 0xfe;

    // Virtual key code by name extended
    su_virtual_key_codes2[L"back"]              = 0x08;
    su_virtual_key_codes2[L"enter"]             = 0x0d;
    su_virtual_key_codes2[L"ctrl"]              = 0x11;
    su_virtual_key_codes2[L"alt"]               = 0x12;
    su_virtual_key_codes2[L"option"]            = 0x12;
    su_virtual_key_codes2[L"break"]             = 0x13;
    su_virtual_key_codes2[L"capital"]           = 0x14;
    su_virtual_key_codes2[L"esc"]               = 0x1b;
    su_virtual_key_codes2[L"spacebar"]          = 0x20;
    su_virtual_key_codes2[L" "]                 = 0x20;
    su_virtual_key_codes2[L"prior"]             = 0x21;
    su_virtual_key_codes2[L"next"]              = 0x22;
    su_virtual_key_codes2[L"←"]                 = 0x25;
    su_virtual_key_codes2[L"leftarrow"]         = 0x25;
    su_virtual_key_codes2[L"↑"]                 = 0x26;
    su_virtual_key_codes2[L"uparrow"]           = 0x26;
    su_virtual_key_codes2[L"→"]                 = 0x27;
    su_virtual_key_codes2[L"rightarrow"]        = 0x27;
    su_virtual_key_codes2[L"↓"]                 = 0x28;
    su_virtual_key_codes2[L"downarrow"]         = 0x28;
    su_virtual_key_codes2[L"printscreen"]       = 0x2c;
    su_virtual_key_codes2[L"prtscn"]            = 0x2c;
    su_virtual_key_codes2[L"sysrq"]             = 0x2c;
    su_virtual_key_codes2[L")"]                 = 0x30;
    su_virtual_key_codes2[L"closeparenthese"]   = 0x30;
    su_virtual_key_codes2[L"rightparenthese"]   = 0x30;
    su_virtual_key_codes2[L"!"]                 = 0x31;
    su_virtual_key_codes2[L"exclamation"]       = 0x31;
    su_virtual_key_codes2[L"exclamationmark"]   = 0x31;
    su_virtual_key_codes2[L"@"]                 = 0x32;
    su_virtual_key_codes2[L"at"]                = 0x32;
    su_virtual_key_codes2[L"atsign"]            = 0x32;
    su_virtual_key_codes2[L"#"]                 = 0x33;
    su_virtual_key_codes2[L"number"]            = 0x33;
    su_virtual_key_codes2[L"numbersign"]        = 0x33;
    su_virtual_key_codes2[L"hash"]              = 0x33;
    su_virtual_key_codes2[L"$"]                 = 0x34;
    su_virtual_key_codes2[L"dollar"]            = 0x34;
    su_virtual_key_codes2[L"dollarsign"]        = 0x34;
    su_virtual_key_codes2[L"%"]                 = 0x35;
    su_virtual_key_codes2[L"percent"]           = 0x35;
    su_virtual_key_codes2[L"^"]                 = 0x36;
    su_virtual_key_codes2[L"caret"]             = 0x36;
    su_virtual_key_codes2[L"&"]                 = 0x37;
    su_virtual_key_codes2[L"ampersand"]         = 0x37;
    su_virtual_key_codes2[L"*"]                 = 0x38;
    su_virtual_key_codes2[L"asterisk"]          = 0x38;
    su_virtual_key_codes2[L"("]                 = 0x39;
    su_virtual_key_codes2[L"openparenthese"]    = 0x39;
    su_virtual_key_codes2[L"leftparenthese"]    = 0x39;
    su_virtual_key_codes2[L"keypad0"]           = 0x60;
    su_virtual_key_codes2[L"keypad1"]           = 0x61;
    su_virtual_key_codes2[L"keypad2"]           = 0x62;
    su_virtual_key_codes2[L"keypad3"]           = 0x63;
    su_virtual_key_codes2[L"keypad4"]           = 0x64;
    su_virtual_key_codes2[L"keypad5"]           = 0x65;
    su_virtual_key_codes2[L"keypad6"]           = 0x66;
    su_virtual_key_codes2[L"keypad7"]           = 0x67;
    su_virtual_key_codes2[L"keypad8"]           = 0x68;
    su_virtual_key_codes2[L"keypad9"]           = 0x69;
    su_virtual_key_codes2[L"keypadmultiply"]    = 0x6a;
    su_virtual_key_codes2[L"keypadplus"]        = 0x6b;
    su_virtual_key_codes2[L"keypadminus"]       = 0x6d;
    su_virtual_key_codes2[L"keypaddecimal"]     = 0x6e;
    su_virtual_key_codes2[L"keypaddivide"]      = 0x6f;
    su_virtual_key_codes2[L"keypad*"]           = 0x6a;
    su_virtual_key_codes2[L"keypad+"]           = 0x6b;
    su_virtual_key_codes2[L"keypad-"]           = 0x6d;
    su_virtual_key_codes2[L"keypad."]           = 0x6e;
    su_virtual_key_codes2[L"keypad/"]           = 0x6f;
    su_virtual_key_codes2[L"numpad*"]           = 0x6a;
    su_virtual_key_codes2[L"numpad+"]           = 0x6b;
    su_virtual_key_codes2[L"numpad-"]           = 0x6d;
    su_virtual_key_codes2[L"numpad."]           = 0x6e;
    su_virtual_key_codes2[L"numpad/"]           = 0x6f;
    su_virtual_key_codes2[L"scrolllock"]        = 0x91;
    su_virtual_key_codes2[L"scrlk"]             = 0x91;
    su_virtual_key_codes2[L"lctrl"]             = 0xa2;
    su_virtual_key_codes2[L"rctrl"]             = 0xa3;
    su_virtual_key_codes2[L"lalt"]              = 0xa4;
    su_virtual_key_codes2[L"ralt"]              = 0xa5;
    su_virtual_key_codes2[L"loption"]           = 0xa4;
    su_virtual_key_codes2[L"roption"]           = 0xa5;
    su_virtual_key_codes2[L"medianext"]         = 0xb0;
    su_virtual_key_codes2[L"mediaprev"]         = 0xb1;
    su_virtual_key_codes2[L"mediatoggleplay"]   = 0xb3;
    su_virtual_key_codes2[L"mail"]              = 0xb4;
    su_virtual_key_codes2[L"mediaselect"]       = 0xb5;
    su_virtual_key_codes2[L"app1"]              = 0xb6;
    su_virtual_key_codes2[L"app2"]              = 0xb7;
    su_virtual_key_codes2[L"colon"]             = 0xba;
    su_virtual_key_codes2[L":"]                 = 0xba;
    su_virtual_key_codes2[L";"]                 = 0xba;
    su_virtual_key_codes2[L"plus"]              = 0xbb;
    su_virtual_key_codes2[L"+"]                 = 0xbb;
    su_virtual_key_codes2[L"="]                 = 0xbb;
    su_virtual_key_codes2[L","]                 = 0xbc;
    su_virtual_key_codes2[L"<"]                 = 0xbc;
    su_virtual_key_codes2[L"dash"]              = 0xbd;
    su_virtual_key_codes2[L"underscore"]        = 0xbd;
    su_virtual_key_codes2[L"_"]                 = 0xbd;
    su_virtual_key_codes2[L"-"]                 = 0xbd;
    su_virtual_key_codes2[L"."]                 = 0xbe;
    su_virtual_key_codes2[L">"]                 = 0xbe;
    su_virtual_key_codes2[L"question"]          = 0xbf;
    su_virtual_key_codes2[L"questionmark"]      = 0xbf;
    su_virtual_key_codes2[L"?"]                 = 0xbf;
    su_virtual_key_codes2[L"/"]                 = 0xbf;
    su_virtual_key_codes2[L"tilde"]             = 0xc0;
    su_virtual_key_codes2[L"~"]                 = 0xc0;
    su_virtual_key_codes2[L"`"]                 = 0xc0;
    su_virtual_key_codes2[L"openbrace"]         = 0xdb;
    su_virtual_key_codes2[L"leftbrace"]         = 0xdb;
    su_virtual_key_codes2[L"openbracket"]       = 0xdb;
    su_virtual_key_codes2[L"{"]                 = 0xdb;
    su_virtual_key_codes2[L"["]                 = 0xdb;
    su_virtual_key_codes2[L"verticalbar"]       = 0xdc;
    su_virtual_key_codes2[L"backslash"]         = 0xdc;
    su_virtual_key_codes2[L"|"]                 = 0xdc;
    su_virtual_key_codes2[L"\\"]                = 0xdc;
    su_virtual_key_codes2[L"closebrace"]        = 0xdd;
    su_virtual_key_codes2[L"rightbrace"]        = 0xdd;
    su_virtual_key_codes2[L"closebracket"]      = 0xdd;
    su_virtual_key_codes2[L"}"]                 = 0xdd;
    su_virtual_key_codes2[L"]"]                 = 0xdd;
    su_virtual_key_codes2[L"quotation"]         = 0xde;
    su_virtual_key_codes2[L"quotationmark"]     = 0xde;
    su_virtual_key_codes2[L"apostrophe"]        = 0xde;
    su_virtual_key_codes2[L"'"]                 = 0xde;
    su_virtual_key_codes2[L"\""]                = 0xde;

    // Initialize custom window procedure and hooks
    c_init();

    // Create functions
    VALUE mSketchup = rb_define_module_under(mAMS, "Sketchup");

    //~ rb_define_module_function(mSketchup, "is_32bit?", VALUEFUNC(AMS::Sketchup::rbf_is_32bit), 0);
    //~ rb_define_module_function(mSketchup, "is_64bit?", VALUEFUNC(AMS::Sketchup::rbf_is_64bit), 0);
    rb_define_module_function(mSketchup, "get_module_handle", VALUEFUNC(AMS::Sketchup::rbf_get_module_handle), 0);
    rb_define_module_function(mSketchup, "get_executable_path", VALUEFUNC(AMS::Sketchup::rbf_get_executable_path), 0);
    rb_define_module_function(mSketchup, "get_executable_name", VALUEFUNC(AMS::Sketchup::rbf_get_executable_name), 0);
    rb_define_module_function(mSketchup, "get_registry_path", VALUEFUNC(AMS::Sketchup::rbf_get_registry_path), 0);
    rb_define_module_function(mSketchup, "get_process_id", VALUEFUNC(AMS::Sketchup::rbf_get_process_id), 0);
    rb_define_module_function(mSketchup, "get_thread_id", VALUEFUNC(AMS::Sketchup::rbf_get_thread_id), 0);
    rb_define_module_function(mSketchup, "get_main_window", VALUEFUNC(AMS::Sketchup::rbf_get_main_window), 0);
    rb_define_module_function(mSketchup, "get_menu_bar", VALUEFUNC(AMS::Sketchup::rbf_get_menu_bar), 0);
    rb_define_module_function(mSketchup, "get_viewport", VALUEFUNC(AMS::Sketchup::rbf_get_viewport), 0);
    rb_define_module_function(mSketchup, "get_status_bar", VALUEFUNC(AMS::Sketchup::rbf_get_status_bar), 0);
    rb_define_module_function(mSketchup, "get_scenes_bar", VALUEFUNC(AMS::Sketchup::rbf_get_scenes_bar), 0);
    rb_define_module_function(mSketchup, "get_toolbar_containers", VALUEFUNC(AMS::Sketchup::rbf_get_toolbar_containers), 0);
    rb_define_module_function(mSketchup, "switch_full_screen", VALUEFUNC(AMS::Sketchup::rbf_switch_full_screen), -1);
    rb_define_module_function(mSketchup, "is_full_screen?", VALUEFUNC(AMS::Sketchup::rbf_is_full_screen), 0);
    rb_define_module_function(mSketchup, "maximize", VALUEFUNC(AMS::Sketchup::rbf_maximize), 0);
    rb_define_module_function(mSketchup, "is_maximized?", VALUEFUNC(AMS::Sketchup::rbf_is_maximized), 0);
    rb_define_module_function(mSketchup, "minimize", VALUEFUNC(AMS::Sketchup::rbf_minimize), 0);
    rb_define_module_function(mSketchup, "is_minimized?", VALUEFUNC(AMS::Sketchup::rbf_is_minimized), 0);
    rb_define_module_function(mSketchup, "restore", VALUEFUNC(AMS::Sketchup::rbf_restore), 0);
    rb_define_module_function(mSketchup, "is_restored?", VALUEFUNC(AMS::Sketchup::rbf_is_restored), 0);
    rb_define_module_function(mSketchup, "activate", VALUEFUNC(AMS::Sketchup::rbf_activate), 0);
    rb_define_module_function(mSketchup, "bring_to_top", VALUEFUNC(AMS::Sketchup::rbf_activate), 0);
    rb_define_module_function(mSketchup, "set_menu_bar", VALUEFUNC(AMS::Sketchup::rbf_set_menu_bar), 1);
    rb_define_module_function(mSketchup, "is_menu_bar_set?", VALUEFUNC(AMS::Sketchup::rbf_is_menu_bar_set), 0);
    rb_define_module_function(mSketchup, "set_viewport_border", VALUEFUNC(AMS::Sketchup::rbf_set_viewport_border), 1);
    rb_define_module_function(mSketchup, "is_viewport_border_set?", VALUEFUNC(AMS::Sketchup::rbf_is_viewport_border_set), 0);
    rb_define_module_function(mSketchup, "show_status_bar", VALUEFUNC(AMS::Sketchup::rbf_show_status_bar), -1);
    rb_define_module_function(mSketchup, "is_status_bar_visible?", VALUEFUNC(AMS::Sketchup::rbf_is_status_bar_visible), 0);
    rb_define_module_function(mSketchup, "show_scenes_bar", VALUEFUNC(AMS::Sketchup::rbf_show_scenes_bar), -1);
    rb_define_module_function(mSketchup, "is_scenes_bar_visible?", VALUEFUNC(AMS::Sketchup::rbf_is_scenes_bar_visible), 0);
    rb_define_module_function(mSketchup, "is_scenes_bar_filled?", VALUEFUNC(AMS::Sketchup::rbf_is_scenes_bar_filled), 0);
    rb_define_module_function(mSketchup, "is_scenes_bar_empty?", VALUEFUNC(AMS::Sketchup::rbf_is_scenes_bar_empty), 0);
    rb_define_module_function(mSketchup, "activate_scenes_bar_tab", VALUEFUNC(AMS::Sketchup::rbf_activate_scenes_bar_tab), 1);
    rb_define_module_function(mSketchup, "show_toolbar_container", VALUEFUNC(AMS::Sketchup::rbf_show_toolbar_container), -1);
    rb_define_module_function(mSketchup, "is_toolbar_container_visible?", VALUEFUNC(AMS::Sketchup::rbf_is_toolbar_container_visible), 1);
    rb_define_module_function(mSketchup, "is_toolbar_container_filled?", VALUEFUNC(AMS::Sketchup::rbf_is_toolbar_container_filled), 1);
    rb_define_module_function(mSketchup, "is_toolbar_container_empty?", VALUEFUNC(AMS::Sketchup::rbf_is_toolbar_container_empty), 1);
    rb_define_module_function(mSketchup, "get_viewport_rect", VALUEFUNC(AMS::Sketchup::rbf_get_viewport_rect), 0);
    rb_define_module_function(mSketchup, "get_viewport_origin", VALUEFUNC(AMS::Sketchup::rbf_get_viewport_origin), 0);
    rb_define_module_function(mSketchup, "get_viewport_size", VALUEFUNC(AMS::Sketchup::rbf_get_viewport_size), 0);
    rb_define_module_function(mSketchup, "get_viewport_center", VALUEFUNC(AMS::Sketchup::rbf_get_viewport_center), 0);
    rb_define_module_function(mSketchup, "show_dialogs", VALUEFUNC(AMS::Sketchup::rbf_show_dialogs), 1);
    rb_define_module_function(mSketchup, "close_dialogs", VALUEFUNC(AMS::Sketchup::rbf_close_dialogs), 0);
    rb_define_module_function(mSketchup, "include_dialog", VALUEFUNC(AMS::Sketchup::rbf_include_dialog), 1);
    rb_define_module_function(mSketchup, "ignore_dialog", VALUEFUNC(AMS::Sketchup::rbf_ignore_dialog), 1);
    rb_define_module_function(mSketchup, "show_trays", VALUEFUNC(AMS::Sketchup::rbf_show_trays), 1);
    rb_define_module_function(mSketchup, "close_trays", VALUEFUNC(AMS::Sketchup::rbf_close_trays), 0);
    rb_define_module_function(mSketchup, "show_toolbars", VALUEFUNC(AMS::Sketchup::rbf_show_toolbars), 1);
    rb_define_module_function(mSketchup, "close_toolbars", VALUEFUNC(AMS::Sketchup::rbf_close_toolbars), 0);
    rb_define_module_function(mSketchup, "include_toolbar", VALUEFUNC(AMS::Sketchup::rbf_include_toolbar), 1);
    rb_define_module_function(mSketchup, "ignore_toolbar", VALUEFUNC(AMS::Sketchup::rbf_ignore_toolbar), 1);
    rb_define_module_function(mSketchup, "is_main_window_active?", VALUEFUNC(AMS::Sketchup::rbf_is_main_window_active), 0);
    rb_define_module_function(mSketchup, "is_active?", VALUEFUNC(AMS::Sketchup::rbf_is_active), 0);
    rb_define_module_function(mSketchup, "find_window_by_caption", VALUEFUNC(AMS::Sketchup::rbf_find_window_by_caption), -1);
    rb_define_module_function(mSketchup, "find_child_window_by_caption", VALUEFUNC(AMS::Sketchup::rbf_find_child_window_by_caption), -1);
    rb_define_module_function(mSketchup, "find_window_by_class_name", VALUEFUNC(AMS::Sketchup::rbf_find_window_by_class_name), -1);
    rb_define_module_function(mSketchup, "find_child_window_by_class_name", VALUEFUNC(AMS::Sketchup::rbf_find_child_window_by_class_name), -1);
    rb_define_module_function(mSketchup, "get_dialogs", VALUEFUNC(AMS::Sketchup::rbf_get_dialogs), 0);
    rb_define_module_function(mSketchup, "get_active_dialogs", VALUEFUNC(AMS::Sketchup::rbf_get_active_dialogs), 0);
    rb_define_module_function(mSketchup, "get_visible_dialogs", VALUEFUNC(AMS::Sketchup::rbf_get_visible_dialogs), 0);
    rb_define_module_function(mSketchup, "get_toolbars", VALUEFUNC(AMS::Sketchup::rbf_get_toolbars), 0);
    rb_define_module_function(mSketchup, "get_active_toolbars", VALUEFUNC(AMS::Sketchup::rbf_get_active_toolbars), 0);
    rb_define_module_function(mSketchup, "get_visible_toolbars", VALUEFUNC(AMS::Sketchup::rbf_get_visible_toolbars), 0);
    rb_define_module_function(mSketchup, "get_caption", VALUEFUNC(AMS::Sketchup::rbf_get_caption), 0);
    rb_define_module_function(mSketchup, "set_caption", VALUEFUNC(AMS::Sketchup::rbf_set_caption), 1);
    rb_define_module_function(mSketchup, "refresh", VALUEFUNC(AMS::Sketchup::rbf_refresh), 0);
    rb_define_module_function(mSketchup, "close", VALUEFUNC(AMS::Sketchup::rbf_close), 0);
    rb_define_module_function(mSketchup, "add_observer", VALUEFUNC(AMS::Sketchup::rbf_add_observer), 1);
    rb_define_module_function(mSketchup, "remove_observer", VALUEFUNC(AMS::Sketchup::rbf_remove_observer), 1);
    rb_define_module_function(mSketchup, "send_user_message", VALUEFUNC(AMS::Sketchup::rbf_send_user_message), 3);
    rb_define_module_function(mSketchup, "get_other_main_windows", VALUEFUNC(AMS::Sketchup::rbf_get_other_main_windows), 0);
    rb_define_module_function(mSketchup, "threaded_messagebox", VALUEFUNC(AMS::Sketchup::rbf_threaded_messagebox), -1);
    //rb_define_module_function(mSketchup, "set_background_color", VALUEFUNC(AMS::Sketchup::rbf_set_background_color), 1);
}
