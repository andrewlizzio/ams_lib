/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_TIMER_H
#define AMS_TIMER_H

#include "ams_ext.h"

class AMS::Timer {
public:
    // Structures
    struct TimerData {
        VALUE proc;
        bool repeat;
        unsigned int count;
        DWORD time;
    };

private:
    // Variables
    static std::map<unsigned long long, TimerData*> su_timers;
    static VALUE su_timer_procedures;

    // Helper Functions
    static VALUE c_call_proc(VALUE v_args);
    static VALUE c_rescue_proc(VALUE v_args, VALUE v_exception);

    // Callback Functions
    static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

public:
    // Ruby Functions
    static VALUE rbf_start(VALUE self, VALUE v_time_out, VALUE v_repeat);
    static VALUE rbf_stop(VALUE self, VALUE v_id);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_TIMER_H */
