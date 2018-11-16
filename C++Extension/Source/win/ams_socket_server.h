/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_SOCKET_SERVER_H
#define AMS_SOCKET_SERVER_H

#include "ams_socket.h"
#include <tchar.h>
#include <strsafe.h>

class AMS::SocketServer {
private:
    // Variables
    static WSADATA* wsa_data;

    // Helper Functions
    DWORD WINAPI c_listener_thread_function(LPVOID lpParam);
    void c_error_handler(LPTSTR lpszFunction);

public:
    // Ruby Functions

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_SOCKET_SERVER_H */
