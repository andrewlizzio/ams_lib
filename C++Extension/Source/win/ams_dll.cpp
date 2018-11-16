/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_dll.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::DLL::rbf_load_library(VALUE self, VALUE v_path) {
    wchar_t* path = RU::value_to_wc_str(v_path);
    HMODULE handle = LoadLibrary(path);
    delete[] path;
    return RU::to_value(handle);
}

VALUE AMS::DLL::rbf_free_library(VALUE self, VALUE v_hmodule) {
    HMODULE hmodule = RU::value_to_hmodule(v_hmodule);
    return RU::to_value(FreeLibrary(hmodule) == TRUE);
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::DLL::init_ruby(VALUE mAMS) {
    VALUE mDLL = rb_define_module_under(mAMS, "DLL");

    rb_define_module_function(mDLL, "load_library", VALUEFUNC(AMS::DLL::rbf_load_library), 1);
    rb_define_module_function(mDLL, "free_library", VALUEFUNC(AMS::DLL::rbf_free_library), 1);
}
