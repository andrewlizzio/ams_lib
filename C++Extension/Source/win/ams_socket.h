/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_SOCKET_H
#define AMS_SOCKET_H

#include "ams_ext.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "AdvApi32.lib")

class AMS::Socket {
private:
    // Variables
    static WSADATA* wsa_data;

    // Helper Functions
    static bool c_init_socket();
    static bool c_uninit_socket();

public:
    // Ruby Functions

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_SOCKET_H */
