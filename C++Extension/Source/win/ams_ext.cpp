/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_ext.h"
#include "ams_cursor.h"
#include "ams_dll.h"
#include "ams_keyboard.h"
#include "ams_menu.h"
#include "ams_midi.h"
#include "ams_registry.h"
#include "ams_screen.h"
#include "ams_sketchup.h"
#include "ams_socket.h"
#include "ams_socket_client.h"
#include "ams_socket_server.h"
#include "ams_system.h"
#include "ams_timer.h"
#include "ams_window.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::init_ruby_ext(VALUE mAMS) {
    AMS::init_ruby(mAMS);

    AMS::Cursor::init_ruby(mAMS);
    AMS::DLL::init_ruby(mAMS);
    AMS::Keyboard::init_ruby(mAMS);
    AMS::Menu::init_ruby(mAMS);
    AMS::MIDI::init_ruby(mAMS);
    AMS::Registry::init_ruby(mAMS);
    AMS::Screen::init_ruby(mAMS);
    AMS::Sketchup::init_ruby(mAMS);
    AMS::Socket::init_ruby(mAMS);
    AMS::SocketClient::init_ruby(mAMS);
    AMS::SocketServer::init_ruby(mAMS);
    AMS::System::init_ruby(mAMS);
    AMS::Timer::init_ruby(mAMS);
    AMS::Window::init_ruby(mAMS);
}
