/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <Carbon/Carbon.h>

short test_for_key_down(short vk);
bool is_cursor_visible(void);
void show_cursor(bool state);
void get_cursor_position(int *x, int *y);
bool set_cursor_position(int x, int y);

#endif  /* USER_INPUT_H */
