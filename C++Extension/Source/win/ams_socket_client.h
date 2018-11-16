/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_SOCKET_CLIENT_H
#define AMS_SOCKET_CLIENT_H

#include "ams_socket.h"

class AMS::SocketClient {
private:
    // Variables
    static WSADATA* wsa_data;

    // Helper Functions

public:
    // Ruby Functions

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_SOCKET_CLIENT_H */
