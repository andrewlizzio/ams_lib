/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_SCREEN_H
#define AMS_SCREEN_H

#include "ams_ext.h"

class AMS::Screen {
public:
    // Ruby Functions
    static VALUE rbf_resolution(VALUE self);
    static VALUE rbf_virtual_resolution(VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_SCREEN_H */
