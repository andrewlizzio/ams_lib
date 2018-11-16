/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_EXT
#define AMS_EXT

#include "../main/ams.h"
#include "ruby_util_ext.h"

namespace AMS {
    // Classes
    class Cursor;
    class Keyboard;
    class MIDI;

    // Main
    void init_ruby_ext(VALUE mAMS);
};

#endif /* AMS_EXT */
