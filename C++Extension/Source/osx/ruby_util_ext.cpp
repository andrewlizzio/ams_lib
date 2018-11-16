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

/*VALUE RU::to_value(const wchar_t* value) {
    unsigned int length = (unsigned int)wcslen(value);
    return to_value(value, length);
}

VALUE RU::to_value(const wchar_t* value, unsigned int length) {
    VALUE v_data(rb_ary_new2(length));
    for (unsigned int i = 0; i < length; ++i)
        rb_ary_store(v_data, i, INT2FIX(value[i]));
    return rb_funcall(v_data, INTERN_PACK, 1, rb_str_new2("U*"));
}

wchar_t* RU::value_to_c_str2(VALUE value) {
    value = StringValue(value);
    VALUE v_data = rb_funcall(value, INTERN_UNPACK, 1, rb_str_new2("U*"));
    unsigned int len = (unsigned int)RARRAY_LEN(v_data);
    wchar_t* buffer = new wchar_t[len+1];
    for (unsigned int i = 0; i < len; ++i)
        buffer[i] = (wchar_t)NUM2INT(rb_ary_entry(v_data, i));
    buffer[len] = '\0';
    return buffer;
}*/
