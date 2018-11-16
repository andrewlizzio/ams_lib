/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_system.h"

/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::System::rbf_get_windows_version(VALUE self) {
    OSVERSIONINFO osvi;
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    double version = osvi.dwMajorVersion + 0.1 * osvi.dwMinorVersion;
    return RU::to_value(version);
}

VALUE AMS::System::rbf_get_metrics(VALUE self, VALUE v_index) {
    return RU::to_value( GetSystemMetrics(RU::value_to_int(v_index)) );
}

VALUE AMS::System::rbf_get_mac_addresses(VALUE self) {
    // Declare variables
    DWORD adapter_size = 0;
    UINT i;
    IP_ADAPTER_INFO* adapters;
    IP_ADAPTER_INFO* cur_adapter;
    char buffer[MAX_ADAPTER_ADDRESS_LENGTH * 3]; // also includes nullterminating
    VALUE v_addreses = rb_hash_new();
    VALUE v_name;
    VALUE v_address;

    // First, call the function to obtain adapter size
    // (The returned value must be ERROR_BUFFER_OVERFLOW)
    if (GetAdaptersInfo(NULL, &adapter_size) != ERROR_BUFFER_OVERFLOW)
        return v_addreses;

    // Now, allocate memory for adapters
    adapters = (IP_ADAPTER_INFO*)malloc(adapter_size);

    // Now obtain all adapters and append their names and MAC addresses to our hash.
    if (GetAdaptersInfo(adapters, &adapter_size) == ERROR_SUCCESS) {
        cur_adapter = adapters;
        while (cur_adapter) {
            // Convert name to Ruby string
            v_name = rb_str_new2(cur_adapter->Description);
            #ifdef HAVE_RUBY_ENCODING_H
                rb_enc_associate_index(v_name, rb_utf8_encindex());
            #endif
            // Convert MAC address to string
            buffer[0] = '\0'; // terminate in case no address
            for (i = 0; i < cur_adapter->AddressLength - 1; ++i) {
                sprintf(buffer + i * 3, "%02X:", cur_adapter->Address[i]);
            }
            if (i < cur_adapter->AddressLength)
                sprintf(buffer + i * 3, "%02X", cur_adapter->Address[i]);
            v_address = rb_str_new2(buffer);
            #ifdef HAVE_RUBY_ENCODING_H
                rb_enc_associate_index(v_address, rb_utf8_encindex());
            #endif
            // Append to hash
            rb_hash_aset(v_addreses, v_name, v_address);
            // Move to next adapter
            cur_adapter = cur_adapter->Next;
        }
    }

    // Free heap
    free(adapters);

    // Return our addresses
    return v_addreses;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::System::init_ruby(VALUE mAMS) {
    VALUE mSystem = rb_define_module_under(mAMS, "System");

    rb_define_module_function(mSystem, "get_windows_version", VALUEFUNC(AMS::System::rbf_get_windows_version), 0);
    rb_define_module_function(mSystem, "get_metrics", VALUEFUNC(AMS::System::rbf_get_metrics), 1);
    rb_define_module_function(mSystem, "get_mac_addresses", VALUEFUNC(AMS::System::rbf_get_mac_addresses), 0);
}
