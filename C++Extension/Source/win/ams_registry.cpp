/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_registry.h"
#include "ams_sketchup.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

bool AMS::Registry::c_format_path(VALUE v_full_path, bool rel_to_su_reg_path, bool read_value, RegData& reg_data) {
    wchar_t* full_path = RU::value_to_wc_str(v_full_path);
    wchar_t* full_path2;
    if (rel_to_su_reg_path) {
        wchar_t* reg_path = new wchar_t[70];
        if (RU::su_version == 5)
            reg_path = L"HKEY_CURRENT_USER\\Software\\Google\\Google SketchUp5\\";
        else if (RU::su_version >= 6 && RU::su_version <= 8)
            wsprintf(reg_path, L"HKEY_CURRENT_USER\\Software\\Google\\SketchUp%d\\", RU::su_version);
        else
            wsprintf(reg_path, L"HKEY_CURRENT_USER\\Software\\SketchUp\\SketchUp 20%d\\", RU::su_version);
        full_path2 = new wchar_t[71 + wcslen(full_path)];
        wcscpy(full_path2, reg_path);
        wcscat(full_path2, full_path);
        delete[] reg_path;
    }
    else {
        full_path2 = new wchar_t[wcslen(full_path) + 1];
        wcscpy(full_path2, full_path);
    }
    delete[] full_path;
    long len = (long)wcslen(full_path2);
    if (len == 0) {
        delete[] full_path2;
        return false;
    }
    long last_index = -1;
    long data_size = 0;
    for (long i = 0; i < len; ++i) {
        if (full_path2[i] == '/')
            full_path2[i] = '\\';
        if (full_path2[i] == '\\') {
            if (i - last_index > 1)
                last_index = i;
            else {
                delete[] full_path2;
                return false;
            }
        }
        if (full_path2[i] == '\\' || i == len - 1)
            ++data_size;
    }
    if (full_path2[len-1] == '\\') {
        full_path2[len-1] = '\0';
        --len;
    }
    wchar_t hkey[40];
    for (int i = 0; i < 40; ++i) {
        if (full_path2[i] == '\\' || i == 39) {
            hkey[i] = '\0';
            break;
        }
        else
            hkey[i] = full_path2[i];
    }
    if (StrCmpI(hkey, L"hkey_classes_root") == 0)
        reg_data.base = HKEY_CLASSES_ROOT;
    else if (StrCmpI(hkey, L"hkey_current_user") == 0)
        reg_data.base = HKEY_CURRENT_USER;
    else if (StrCmpI(hkey, L"hkey_local_machine") == 0)
        reg_data.base = HKEY_LOCAL_MACHINE;
    else if (StrCmpI(hkey, L"hkey_users") == 0)
        reg_data.base = HKEY_USERS;
    else if (StrCmpI(hkey, L"hkey_performance_data") == 0)
        reg_data.base = HKEY_PERFORMANCE_DATA;
    else if (StrCmpI(hkey, L"hkey_performance_text") == 0)
        reg_data.base = HKEY_PERFORMANCE_TEXT;
    else if (StrCmpI(hkey, L"hkey_performance_nlstext") == 0)
        reg_data.base = HKEY_PERFORMANCE_NLSTEXT;
    else if (StrCmpI(hkey, L"hkey_current_config") == 0)
        reg_data.base = HKEY_CURRENT_CONFIG;
    else if (StrCmpI(hkey, L"hkey_dyn_data") == 0)
        reg_data.base = HKEY_DYN_DATA;
    else if (StrCmpI(hkey, L"hkey_current_user_local_settings") == 0)
        reg_data.base = HKEY_CURRENT_USER_LOCAL_SETTINGS;
    else {
        delete[] full_path2;
        return false;
    }
    size_t hkey_len = wcslen(hkey);
    if (data_size == 1) {
        reg_data.path = new wchar_t[1];
        reg_data.path[0] = '\0';
        reg_data.name = new wchar_t[1];
        reg_data.name[0] = '\0';
    }
    else if (data_size == 2) {
        if (read_value) {
            reg_data.path = new wchar_t[1];
            reg_data.path[0] = '\0';
        }
        else {
            reg_data.path = new wchar_t[len - hkey_len];
            wcsncpy(reg_data.path, full_path2 + hkey_len + 1, len - hkey_len - 1);
            reg_data.path[len - hkey_len - 1] = '\0';
        }
        reg_data.name = new wchar_t[len - hkey_len];
        wcsncpy(reg_data.name, full_path2 + hkey_len + 1, len - hkey_len - 1);
        reg_data.name[len - hkey_len - 1] = '\0';
    }
    else {
        std::vector<long> indeces;
        for (long i = 0; i < len; ++i)
            if (full_path2[i] == '\\')
                indeces.push_back(i);
        int i1 = indeces[0];
        int i2 = indeces[indeces.size() - 1];
        if (read_value) {
            reg_data.path = new wchar_t[i2 - i1];
            wcsncpy(reg_data.path, full_path2 + i1 + 1, i2 - i1 - 1);
            reg_data.path[i2 - i1 - 1] = '\0';
        }
        else {
            reg_data.path = new wchar_t[len - i1];
            wcsncpy(reg_data.path, full_path2 + i1 + 1, len - i1 - 1);
            reg_data.path[len - i1 - 1] = '\0';
        }
        reg_data.name = new wchar_t[len - i2];
        wcsncpy(reg_data.name, full_path2 + i2 + 1, len - i2 - 1);
        reg_data.name[len - i2 - 1] = '\0';
    }
    delete[] full_path2;
    return true;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::Registry::rbf_read(int argc, VALUE* argv, VALUE self) {
    bool rel_to_su_reg_path = true;
    if (argc == 2)
        rel_to_su_reg_path = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");

    RegData reg_data;
    if (!c_format_path(argv[0], rel_to_su_reg_path, true, reg_data))
        return Qnil;

    HKEY hkey;
    LONG res = RegOpenKey(reg_data.base, reg_data.path, &hkey);
    if (res != ERROR_SUCCESS) {
        delete[] reg_data.path;
        delete[] reg_data.name;
        return Qnil;
    }

    DWORD type;
    BYTE* data = nullptr;
    DWORD cbdata = 0;
    res = RegQueryValueEx(hkey, reg_data.name, NULL, &type, NULL, &cbdata);
    if (res == ERROR_SUCCESS) {
        data = new BYTE[cbdata];
        res = RegQueryValueEx(hkey, reg_data.name, NULL, &type, data, &cbdata);
    }
    else {
        res = RegQueryValue(hkey, reg_data.name, NULL, (PLONG)&cbdata);
        if (res == ERROR_SUCCESS) {
            data = new BYTE[cbdata];
            res = RegQueryValue(hkey, reg_data.name, (LPTSTR)data, (PLONG)&cbdata);
        }
        type = REG_SZ;
    }

    RegCloseKey(hkey);
    delete[] reg_data.path;
    delete[] reg_data.name;
    if (res != ERROR_SUCCESS) {
        delete[] data;
        return Qnil;
    }

    VALUE v_result;

    if (type == REG_SZ || type == REG_MULTI_SZ || type == REG_LINK || type == REG_BINARY)
        v_result = RU::win_wc_str_to_value(reinterpret_cast<wchar_t*>(data), cbdata - 1);
    else if (type == REG_EXPAND_SZ) {
        wchar_t buffer[4096];
        int s = ExpandEnvironmentStrings((LPTSTR)data, buffer, 4096);
        v_result = RU::win_wc_str_to_value(buffer, s - 1);
    }
    else if (type == REG_DWORD) {
        DWORD value = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
        v_result = RU::to_value(value);
    }
    else if (type == REG_QWORD) {
        long long value = (
            (((long long)data[7] & 0xff) << 56) |
            (((long long)data[6] & 0xff) << 48) |
            (((long long)data[5] & 0xff) << 40) |
            (((long long)data[4] & 0xff) << 32) |
            ((long long)data[3] << 24) |
            ((long long)data[2] << 16) |
            ((long long)data[1] << 8) |
            (long long)data[0]);
        v_result = RU::to_value(value);
    }
    else
        v_result = Qnil;

    delete[] data;
    return v_result;
}

VALUE AMS::Registry::rbf_write(int argc, VALUE* argv, VALUE self) {
    bool rel_to_su_reg_path = true;
    if (argc == 3)
        rel_to_su_reg_path = RU::value_to_bool(argv[2]);
    else if (argc != 2)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 2..3 arguments.");

    RegData reg_data;
    if (!c_format_path(argv[0], rel_to_su_reg_path, true, reg_data))
        return Qnil;

    HKEY hkey;
    LONG res = RegCreateKey(reg_data.base, reg_data.path, &hkey);
    if (res != ERROR_SUCCESS) {
        delete[] reg_data.path;
        delete[] reg_data.name;
        return Qnil;
    }

    int type = TYPE(argv[1]);
    if (type == T_FIXNUM) {
        DWORD data = RU::value_to_ulong(argv[1]);
        res = RegSetValueEx(hkey, reg_data.name, 0, REG_DWORD, reinterpret_cast<BYTE*>(&data), sizeof(data));
    }
    else if (type == T_BIGNUM) {
        long long data = RU::value_to_ll(argv[1]);
        res = RegSetValueEx(hkey, reg_data.name, 0, REG_QWORD, reinterpret_cast<BYTE*>(&data), sizeof(data));
    }
    else if (type == T_NIL)
        res = RegSetValueEx(hkey, reg_data.name, 0, REG_NONE, NULL, 0);
    else {
        wchar_t* data = RU::value_to_wc_str(argv[1]);
        unsigned int data_size = (unsigned int)wcslen(data) * 2;
        res = RegSetValueEx(hkey, reg_data.name, 0, REG_SZ, reinterpret_cast<BYTE*>(data), data_size);
        delete[] data;
    }

    RegCloseKey(hkey);
    delete[] reg_data.path;
    delete[] reg_data.name;

    return RU::to_value(res == ERROR_SUCCESS);
}

VALUE AMS::Registry::rbf_erase(int argc, VALUE* argv, VALUE self) {
    bool rel_to_su_reg_path = true;
    if (argc == 2)
        rel_to_su_reg_path = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");

    RegData reg_data;
    if (!c_format_path(argv[0], rel_to_su_reg_path, true, reg_data))
        return Qnil;

    HKEY hkey;
    LONG res = RegOpenKey(reg_data.base, reg_data.path, &hkey);
    if (res != ERROR_SUCCESS) {
        delete[] reg_data.path;
        delete[] reg_data.name;
        return Qnil;
    }

    res = RegDeleteValue(hkey, reg_data.name);
    if (res != ERROR_SUCCESS)
        RegDeleteKey(hkey, reg_data.name);

    RegCloseKey(hkey);
    delete[] reg_data.path;
    delete[] reg_data.name;

    return RU::to_value(res == ERROR_SUCCESS);
}

VALUE AMS::Registry::rbf_get_keys(int argc, VALUE* argv, VALUE self) {
    bool rel_to_su_reg_path = true;
    if (argc == 2)
        rel_to_su_reg_path = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");

    RegData reg_data;
    if (!c_format_path(argv[0], rel_to_su_reg_path, false, reg_data))
        return Qnil;

    VALUE v_keys = rb_ary_new();
    HKEY hkey;
    LONG res = RegOpenKey(reg_data.base, reg_data.path, &hkey);
    if (res != ERROR_SUCCESS) {
        delete[] reg_data.path;
        delete[] reg_data.name;
        return v_keys;
    }
    DWORD index = 0;
    wchar_t buffer[256];
    while((res = RegEnumKey(hkey, index, buffer, 256)) != ERROR_NO_MORE_ITEMS) {
        if (res != ERROR_MORE_DATA)
            rb_ary_push(v_keys, RU::to_value(buffer));
        ++index;
    }
    RegCloseKey(hkey);
    delete[] reg_data.path;
    delete[] reg_data.name;
    return v_keys;
}

VALUE AMS::Registry::rbf_get_values(int argc, VALUE* argv, VALUE self) {
    bool rel_to_su_reg_path = true;
    if (argc == 2)
        rel_to_su_reg_path = RU::value_to_bool(argv[1]);
    else if (argc != 1)
        rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");

    RegData reg_data;
    if (!c_format_path(argv[0], rel_to_su_reg_path, false, reg_data))
        return Qnil;

    VALUE v_values = rb_hash_new();
    HKEY hkey;
    LONG res = RegOpenKey(reg_data.base, reg_data.path, &hkey);
    if (res != ERROR_SUCCESS) {
        delete[] reg_data.path;
        delete[] reg_data.name;
        return v_values;
    }
    DWORD index = 0;
    wchar_t value[256];
    DWORD value_size = 256;
    DWORD type = 0;
    BYTE data[4096];
    DWORD data_size = 4096;
    while((res = RegEnumValue(hkey, index, value, &value_size, NULL, &type, data, &data_size)) != ERROR_NO_MORE_ITEMS) {
        if (res != ERROR_MORE_DATA) {
            VALUE v_value = RU::win_wc_str_to_value(value, value_size);
            if (type == REG_SZ || type == REG_MULTI_SZ || type == REG_LINK || type == REG_BINARY)
                rb_hash_aset(v_values, v_value, RU::win_wc_str_to_value(reinterpret_cast<wchar_t*>(data), data_size - 1));
            else if (type == REG_EXPAND_SZ) {
                wchar_t buffer[4096];
                int s = ExpandEnvironmentStrings((LPTSTR)data, buffer, 4096);
                rb_hash_aset(v_values, v_value, RU::win_wc_str_to_value(buffer, s - 1));
            }
            else if (type == REG_DWORD) {
                int number = (data[3] << 24) | (data[2] << 16) | (data[1] << 8) | data[0];
                rb_hash_aset(v_values, v_value, RU::to_value(number));
            }
            else if (type == REG_QWORD) {
                long long number = (
                    (((long long)data[7] & 0xff) << 56) |
                    (((long long)data[6] & 0xff) << 48) |
                    (((long long)data[5] & 0xff) << 40) |
                    (((long long)data[4] & 0xff) << 32) |
                    ((long long)data[3] << 24) |
                    ((long long)data[2] << 16) |
                    ((long long)data[1] << 8) |
                    (long long)data[0]);
                rb_hash_aset(v_values, v_value, RU::to_value(number));
            }
        }
        value_size = 256;
        data_size = 4096;
        ++index;
    }
    RegCloseKey(hkey);
    delete[] reg_data.path;
    delete[] reg_data.name;
    return v_values;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::Registry::init_ruby(VALUE mAMS) {
    VALUE mRegistry = rb_define_module_under(mAMS, "Registry");

    rb_define_module_function(mRegistry, "read", VALUEFUNC(AMS::Registry::rbf_read), -1);
    rb_define_module_function(mRegistry, "get", VALUEFUNC(AMS::Registry::rbf_read), -1);

    rb_define_module_function(mRegistry, "write", VALUEFUNC(AMS::Registry::rbf_write), -1);
    rb_define_module_function(mRegistry, "set", VALUEFUNC(AMS::Registry::rbf_write), -1);

    rb_define_module_function(mRegistry, "delete", VALUEFUNC(AMS::Registry::rbf_erase), -1);
    rb_define_module_function(mRegistry, "remove", VALUEFUNC(AMS::Registry::rbf_erase), -1);

    rb_define_module_function(mRegistry, "get_keys", VALUEFUNC(AMS::Registry::rbf_get_keys), -1);
    rb_define_module_function(mRegistry, "get_values", VALUEFUNC(AMS::Registry::rbf_get_values), -1);
}
