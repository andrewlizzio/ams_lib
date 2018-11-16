/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_EXT
#define AMS_EXT

#include "stdafx.h"
#include "../main/ams.h"
#include "ruby_util_ext.h"

namespace AMS {
    // Classes
    class Cursor;
    class DLL;
    class Keyboard;
    class Menu;
    class MIDI;
    class Registry;
    class Screen;
    class Sketchup;
    class Socket;
    class SocketClient;
    class SocketServer;
    class System;
    class Timer;
    class Window;

    // Main
    void init_ruby_ext(VALUE mAMS);
};

#endif /* AMS_EXT */
