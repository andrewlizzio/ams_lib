/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_timer.h"
#include "ams_sketchup.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Variables
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

std::map<unsigned long long, AMS::Timer::TimerData*> AMS::Timer::su_timers;
VALUE AMS::Timer::su_timer_procedures;


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Timer::c_call_proc(VALUE v_args) {
    TimerData* data = reinterpret_cast<TimerData*>(v_args);
    //rb_funcall(data->proc, RU::INTERN_CALL, 2, RU::to_value(data->count), RU::to_value(data->time));
    rb_eval_cmd(data->proc, rb_ary_new3(2, RU::to_value(data->count), RU::to_value(data->time)), 0);
    return Qnil;
}

VALUE AMS::Timer::c_rescue_proc(VALUE v_args, VALUE v_exception) {
    VALUE v_message = rb_funcall(v_exception, RU::INTERN_MESSAGE, 0);
    rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, v_message);
    VALUE v_backtrace = rb_funcall(v_exception, RU::INTERN_BACKTRACE, 0);
    rb_funcall(rb_stdout, RU::INTERN_PUTS, 1, rb_ary_entry(v_backtrace, 0));
    return Qnil;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Callback Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VOID CALLBACK AMS::Timer::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    unsigned long long id = static_cast<unsigned long long>(idEvent);
    std::map<unsigned long long, TimerData*>::iterator it = su_timers.find(id);
    if (it == su_timers.end()) {
        KillTimer(hwnd, idEvent);
        return;
    }
    TimerData* data = it->second;
    ++data->count;
    data->time = dwTime;
    if (data->repeat == false)
        KillTimer(hwnd, idEvent);
    rb_rescue2(RUBY_METHOD_FUNC(c_call_proc), reinterpret_cast<VALUE>(data), RUBY_METHOD_FUNC(c_rescue_proc), Qnil, rb_eException, VALUE(0));
    if (data->repeat == false && su_timers.find(id) != su_timers.end()) {
        RU::array_delete_first(su_timer_procedures, data->proc);
        delete data;
        su_timers.erase(it);
    }
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Timer::rbf_start(VALUE self, VALUE v_time_out, VALUE v_repeat) {
    unsigned int time_out = RU::value_to_uint(v_time_out);
    bool repeat = RU::value_to_bool(v_repeat);
    if (rb_block_given_p() == 0)
        rb_raise(rb_eArgError, "Expected a block, a timer procedure, in addition to all the parameters!");
    VALUE v_proc = rb_block_proc();
    TimerData* data = new TimerData;
    data->proc = v_proc;
    data->repeat = repeat;
    data->count = 0;
    data->time = 0;
    unsigned long long id = static_cast<unsigned long long>(SetTimer(NULL, 0, time_out, TimerProc));
    if (id == 0) {
        delete data;
        return Qnil;
    }
    else {
        rb_ary_push(su_timer_procedures, v_proc);
        su_timers[id] = data;
        return RU::to_value(id);
    }
}

VALUE AMS::Timer::rbf_stop(VALUE self, VALUE v_id) {
    unsigned long long id = RU::value_to_ull(v_id);
    std::map<unsigned long long, TimerData*>::iterator it = su_timers.find(id);
    if (it == su_timers.end())
        return Qfalse;
    else {
        KillTimer(AMS::Sketchup::su_main_window, static_cast<UINT_PTR>(id));
        RU::array_delete_first(su_timer_procedures, it->second->proc);
        delete it->second;
        su_timers.erase(it);
        return Qtrue;
    }
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Timer::init_ruby(VALUE mAMS) {
    su_timer_procedures = rb_ary_new();
    rb_gc_register_address(&su_timer_procedures);

    VALUE mTimer = rb_define_module_under(mAMS, "Timer");

    rb_define_module_function(mTimer, "start", VALUEFUNC(AMS::Timer::rbf_start), 2);
    rb_define_module_function(mTimer, "stop", VALUEFUNC(AMS::Timer::rbf_stop), 1);
}
