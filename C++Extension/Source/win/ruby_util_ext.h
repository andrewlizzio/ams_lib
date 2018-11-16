/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef RUBY_UTIL_EXT_H
#define RUBY_UTIL_EXT_H

#include "../utils/ruby_util.h"

namespace RU
{
    inline VALUE to_value(HWND value) {
        if (value)
            return rb_ull2inum(reinterpret_cast<unsigned long long>(value));
        else
            return Qnil;
    }

    inline VALUE to_value(HMENU value) {
        if (value)
            return rb_ull2inum(reinterpret_cast<unsigned long long>(value));
        else
            return Qnil;
    }

    inline VALUE to_value(HMODULE value) {
        if (value)
            return rb_ull2inum(reinterpret_cast<unsigned long long>(value));
        else
            return Qnil;
    }

    inline VALUE to_value(HMIDIOUT value) {
        if (value)
            return rb_ull2inum(reinterpret_cast<unsigned long long>(value));
        else
            return Qnil;
    }

    inline VALUE to_value(HANDLE value) {
        if (value)
            return rb_ull2inum(reinterpret_cast<unsigned long long>(value));
        else
            return Qnil;
    }

    HWND value_to_hwnd(VALUE value, bool validate = true);
    HMODULE value_to_hmodule(VALUE value);
    HANDLE value_to_handle(VALUE value);
    HMENU value_to_hmenu(VALUE value, bool validate = true);

    VALUE win_wc_str_to_value(const wchar_t* value, unsigned int length);
    //wchar_t* value_to_win_wc_str(VALUE value);
}

#endif  /* RUBY_UTIL_EXT_H */
