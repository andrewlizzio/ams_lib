/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_REGISTRY_H
#define AMS_REGISTRY_H

#include "ams_ext.h"

class AMS::Registry {
public:
    // Structures
    struct RegData {
        HKEY base;
        wchar_t* path;
        wchar_t* name;
    };

private:
    // Helper Functions
    static bool c_format_path(VALUE v_full_path, bool rel_to_su_reg_path, bool read_value, RegData& reg_data);

public:
    // Ruby Functions
    static VALUE rbf_read(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_write(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_erase(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_keys(int argc, VALUE* argv, VALUE self);
    static VALUE rbf_get_values(int argc, VALUE* argv, VALUE self);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_REGISTRY_H */
