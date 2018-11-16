/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#ifndef AMS_MULTI_LINE_TEXT_H
#define AMS_MULTI_LINE_TEXT_H

#include "ams.h"

class AMS::MultiLineText {
private:
    // Constants
    static const char* HORIZONTAL_LINE;
    static const unsigned int MIN_LINE_LIMIT;
    static const unsigned int MAX_LINE_LIMIT;
    static const unsigned int DEFAULT_LINE_LIMIT;
    static const bool DEFAULT_SHOW_LINE_NUMBERS;

    // Structures
    struct MultiLineTextData {
        VALUE v_text_ary;
        VALUE v_intro;
        VALUE v_entity;
        unsigned int limit;
        unsigned int count;
        double xpos_ratio;
        double ypos_ratio;
        bool show_line_numbers;
    };

    // Helper Functions
    static VALUE c_class_allocate(VALUE klass);
    static void c_class_mark(void* data);
    static void c_class_deallocate(void* data);
    static void c_update(MultiLineTextData* mlt_data);

public:
    // Ruby Functions
    static VALUE rbf_initialize(VALUE self, VALUE v_x, VALUE v_y);
    static VALUE rbf_initialize_copy(VALUE self, VALUE v_other);
    static VALUE rbf_get_limit(VALUE self);
    static VALUE rbf_set_limit(VALUE self, VALUE v_limit);
    static VALUE rbf_get_intro(VALUE self);
    static VALUE rbf_set_intro(VALUE self, VALUE v_intro);
    static VALUE rbf_line_numbers_visible(VALUE self);
    static VALUE rbf_show_line_numbers(VALUE self, VALUE v_state);
    static VALUE rbf_print(VALUE self, VALUE v_text);
    static VALUE rbf_puts(VALUE self, VALUE v_text);
    static VALUE rbf_clear(VALUE self);
    static VALUE rbf_remove(VALUE self);
    static VALUE rbf_get_entity(VALUE self);
    static VALUE rbf_set_entity(VALUE self, VALUE v_entity);
    static VALUE rbf_get_count(VALUE self);
    static VALUE rbf_get_position(VALUE self);
    static VALUE rbf_set_position(VALUE self, VALUE v_x, VALUE v_y);

    // Main
    static void init_ruby(VALUE mAMS);
};

#endif  /* AMS_MULTI_LINE_TEXT_H */
