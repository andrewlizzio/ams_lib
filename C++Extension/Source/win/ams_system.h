/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_SYSTEM_H
#define AMS_SYSTEM_H

#include "ams_ext.h"

class AMS::System {
public:
    // Ruby Functions
    static VALUE rbf_get_windows_version(VALUE self);
    static VALUE rbf_get_metrics(VALUE self, VALUE v_index);
    static VALUE rbf_get_mac_addresses(VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_SYSTEM_H */
