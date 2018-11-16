/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ruby_util_ext.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

HWND RU::value_to_hwnd(VALUE value, bool validate) {
    if (value == Qnil)
        return NULL;
    else {
        HWND handle = reinterpret_cast<HWND>(rb_num2ull(value));
        if (validate && handle != NULL && IsWindow(handle) == FALSE)
            rb_raise(rb_eTypeError, "Given handle does not reference a valid window!");
        return handle;
    }
}

HMODULE RU::value_to_hmodule(VALUE value) {
    if (value == Qnil)
        return NULL;
    else
        return reinterpret_cast<HMODULE>(rb_num2ull(value));
}

HANDLE RU::value_to_handle(VALUE value) {
    if (value == Qnil)
        return NULL;
    else
        return reinterpret_cast<HANDLE>(rb_num2ull(value));
}

HMENU RU::value_to_hmenu(VALUE value, bool validate) {
    if (value == Qnil)
        return NULL;
    else {
        HMENU handle = reinterpret_cast<HMENU>(rb_num2ull(value));
        if (validate && handle != NULL && IsMenu(handle) == FALSE)
            rb_raise(rb_eTypeError, "Given handle does not reference a valid menu!");
        return handle;
    }
}
/*
VALUE RU::to_value(const wchar_t* value) {
    unsigned int length = (unsigned int)wcslen(value);
    return to_value(value, length);
}*/

VALUE RU::win_wc_str_to_value(const wchar_t* value, unsigned int length) {
    int num_bytes = WideCharToMultiByte(CP_UTF8, 0, value, length, nullptr, 0, NULL, NULL);

    if (num_bytes == 0) return Qnil;

    char* buffer = new char[num_bytes + 1];

    int bytes_written = WideCharToMultiByte(CP_UTF8, 0, value, length, buffer, num_bytes, NULL, NULL);

    if (bytes_written == 0) {
        delete[] buffer;
        return Qnil;
    }
    else {
#ifdef HAVE_RUBY_ENCODING_H
        VALUE v_string = rb_enc_str_new(buffer, bytes_written, rb_utf8_encoding());
#else
        VALUE v_string = rb_str_new(buffer, bytes_written);
#endif
        delete[] buffer;
        return v_string;
    }
}

//#ifdef HAVE_RUBY_ENCODING_H

/*wchar_t* RU::value_to_win_wc_str(VALUE value) {
    value = StringValue(value);
    char* mb_str = RSTRING_PTR(value);
//#ifdef RUBY_VERSION18
//  unsigned int wc_str_len = RSTRING_LEN(value) + 1;
//#else
    unsigned int wc_str_len = rb_str_strlen(value) + 1;
//#endif
    wchar_t* wc_str = new wchar_t[wc_str_len];
    int characters_written = MultiByteToWideChar(CP_UTF8, 0, mb_str, -1, wc_str, wc_str_len);
    if (characters_written == 0) {
        delete[] wc_str;
        return nullptr;
    }
    else {
        return wc_str;
    }
}*/

//#else

/*VALUE RU::win_wc_str_to_value(const wchar_t* value, unsigned int length) {
    VALUE v_data(rb_ary_new2(length));
    for (unsigned int i = 0; i < length; ++i)
        rb_ary_store(v_data, i, INT2FIX(value[i]));
    return rb_funcall(v_data, INTERN_PACK, 1, rb_str_new2("U*"));
}*/

/*wchar_t* RU::value_to_wc_str(VALUE value) {
    value = StringValue(value);
    VALUE v_data = rb_funcall(value, INTERN_UNPACK, 1, rb_str_new2("U*"));
    unsigned int len = (unsigned int)RARRAY_LEN(v_data);
    wchar_t* buffer = new wchar_t[len + 1];
    for (unsigned int i = 0; i < len; ++i)
        buffer[i] = (wchar_t)NUM2INT(rb_ary_entry(v_data, i));
    buffer[len] = '\0';
    return buffer;
}*/

//#endif
