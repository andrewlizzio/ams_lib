/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_DLL_H
#define AMS_DLL_H

#include "ams_ext.h"

class AMS::DLL {
public:
    // Ruby Functions
    static VALUE rbf_load_library(VALUE self, VALUE v_path);
    static VALUE rbf_free_library(VALUE self, VALUE v_hmodule);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_DLL_H */
