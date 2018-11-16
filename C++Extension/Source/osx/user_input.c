/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "user_input.h"

short test_for_key_down(short vk) {
    if (vk >= 0 || vk <= 127) {
        unsigned char * key_bytes;
        KeyMap key_states;
        GetKeys(key_states);
        key_bytes = (unsigned char *)key_states;
        return ((key_bytes[vk>>3] & (1 << (vk&7))) != 0) ? true : false;
    }
    else
        return 0;
}

bool is_cursor_visible(void) {
    return CGCursorIsVisible() ? true : false;
}

void show_cursor(bool state) {
    /*if (CGCursorIsVisible() == state)
        return;
    if (state) {
        while(!CGCursorIsVisible()) CGDisplayShowCursor(kCGNullDirectDisplay);
    }
    else {
        while(CGCursorIsVisible()) CGDisplayHideCursor(kCGNullDirectDisplay);
    }*/
}

void get_cursor_position(int *x, int *y) {
    /*CGEventRef ourEvent = CGEventCreate(NULL);
    CGPoint point = CGEventGetLocation(ourEvent);
    *x = point.x;
    *y = point.y;
    CFRelease(ourEvent);*/
    *x = 0;
    *y = 0;
}

bool set_cursor_position(int x, int y) {
    //return CGWarpMouseCursorPosition(CGPointMake(x, y)) == 0;
    return true;
}
