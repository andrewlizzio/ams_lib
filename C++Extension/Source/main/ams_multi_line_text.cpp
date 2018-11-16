/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams_multi_line_text.h"


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Constants
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

const char* AMS::MultiLineText::HORIZONTAL_LINE = "--------------------------------------------------";
const unsigned int AMS::MultiLineText::MIN_LINE_LIMIT = 1;
const unsigned int AMS::MultiLineText::MAX_LINE_LIMIT = 1000;
const unsigned int AMS::MultiLineText::DEFAULT_LINE_LIMIT = 20;
const bool AMS::MultiLineText::DEFAULT_SHOW_LINE_NUMBERS = true;


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

// Triggered one time per object, before initialization of an object.
VALUE AMS::MultiLineText::c_class_allocate(VALUE klass) {
    MultiLineTextData* mlt_data = ALLOC(MultiLineTextData);
    return Data_Wrap_Struct(klass, c_class_mark, c_class_deallocate, mlt_data);
}


// Triggered for every object, each time right before there is a garbage collection.
void AMS::MultiLineText::c_class_mark(void* data) {
    MultiLineTextData* mlt_data = (MultiLineTextData*)data;
    rb_gc_mark(mlt_data->v_text_ary);
    rb_gc_mark(mlt_data->v_intro);
    rb_gc_mark(mlt_data->v_entity);
}

// Triggered one time per object, when an object is to be freed.
void AMS::MultiLineText::c_class_deallocate(void* data) {
    MultiLineTextData* mlt_data = (MultiLineTextData*)data;
    if (mlt_data->v_entity != Qnil && rb_funcall(mlt_data->v_entity, RU::INTERN_TVALID, 0) == Qtrue) {
        rb_funcall(mlt_data->v_entity, RU::INTERN_SMATERIAL, 1, Qnil);
        rb_funcall(mlt_data->v_entity, RU::INTERN_EERASE, 0);
        mlt_data->v_entity = Qnil;
    }
    ruby_xfree(mlt_data);
}

void AMS::MultiLineText::c_update(MultiLineTextData* mlt_data) {
    // Create the text entity
    if (mlt_data->v_entity == Qnil || rb_funcall(mlt_data->v_entity, RU::INTERN_TVALID, 0) == Qfalse) {
        VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
        mlt_data->v_entity = rb_funcall(
            v_model,
            RU::INTERN_ADD_NOTE,
            3,
            rb_str_new2(""),
            RU::to_value(mlt_data->xpos_ratio),
            RU::to_value(mlt_data->ypos_ratio));
    }
    // Keep text array clamped to the desired limit.
    unsigned int text_ary_size = (unsigned int)RARRAY_LEN(mlt_data->v_text_ary);
    if (text_ary_size > mlt_data->limit) {
        unsigned int diff = text_ary_size - mlt_data->limit;
        for (unsigned int i = 0; i < diff; ++i)
            rb_ary_shift(mlt_data->v_text_ary);
        text_ary_size = mlt_data->limit;
    }
    // Create a string for outing text
    VALUE v_text;
    // Add intro
    if (RSTRING_LEN(mlt_data->v_intro) != 0) {
        v_text = rb_str_new2(HORIZONTAL_LINE);
        rb_str_cat2(v_text, "\n\n");
        rb_str_concat(v_text, mlt_data->v_intro);
        rb_str_cat2(v_text, "\n\n");
        rb_str_cat2(v_text, HORIZONTAL_LINE);
        rb_str_cat2(v_text, "\n\n");
    }
    else
        v_text = rb_str_new2("");
    // Add text lines
    for (unsigned int i = 0; i < text_ary_size; ++i) {
        // Append line number
        if (mlt_data->show_line_numbers) {
            char line_number_text[32];
            sprintf(line_number_text, "[%03i]   ", mlt_data->count - text_ary_size + i + 1);
            rb_str_cat2(v_text, line_number_text);
        }
        // Append line text
        rb_str_concat(v_text, rb_ary_entry(mlt_data->v_text_ary, i));
        rb_str_cat2(v_text, "\n");
    }
    // Set entity text
    rb_funcall(mlt_data->v_entity, RU::INTERN_SET_TEXT, 1, v_text);
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::MultiLineText::rbf_initialize(VALUE self, VALUE v_x, VALUE v_y) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);

    VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_view = rb_funcall(v_model, RU::INTERN_ACTIVE_VIEW, 0);
    int vpwidth = RU::value_to_int(rb_funcall(v_view, RU::INTERN_VPWIDTH, 0));
    int vpheight = RU::value_to_int(rb_funcall(v_view, RU::INTERN_VPWIDTH, 0));
    if (vpwidth < 1) vpwidth = 800;
    if (vpheight < 1) vpheight = 600;

    mlt_data->v_text_ary = rb_ary_new();
    mlt_data->v_intro = rb_str_new2("");
    mlt_data->v_entity = Qnil;
    mlt_data->limit = DEFAULT_LINE_LIMIT;
    mlt_data->count = 0;
    mlt_data->xpos_ratio = RU::value_to_double(v_x) / vpwidth;
    mlt_data->ypos_ratio = RU::value_to_double(v_y) / vpheight;
    mlt_data->show_line_numbers = DEFAULT_SHOW_LINE_NUMBERS;

    return self;
}

VALUE AMS::MultiLineText::rbf_initialize_copy(VALUE self, VALUE orig_self) {
    MultiLineTextData* mlt_data;
    MultiLineTextData* orig_mlt_data;

#ifndef RUBY_VERSION18
    if (!OBJ_INIT_COPY(self, orig_self)) return self;
#endif

    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    Data_Get_Struct(orig_self, MultiLineTextData, orig_mlt_data);

    mlt_data->v_text_ary = rb_ary_dup(orig_mlt_data->v_text_ary);
    mlt_data->v_intro = rb_str_dup(orig_mlt_data->v_intro);
    mlt_data->v_entity = Qnil;
    mlt_data->limit = orig_mlt_data->limit;
    mlt_data->count = orig_mlt_data->count;
    mlt_data->xpos_ratio = orig_mlt_data->xpos_ratio;
    mlt_data->ypos_ratio = orig_mlt_data->ypos_ratio;
    mlt_data->show_line_numbers = orig_mlt_data->show_line_numbers;

    return self;
}

VALUE AMS::MultiLineText::rbf_get_limit(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    return RU::to_value(mlt_data->limit);
}

VALUE AMS::MultiLineText::rbf_set_limit(VALUE self, VALUE v_limit) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    mlt_data->limit = Geom::clamp<unsigned int>(RU::value_to_uint(v_limit), MIN_LINE_LIMIT, MAX_LINE_LIMIT);
    c_update(mlt_data);
    return Qnil;
}


VALUE AMS::MultiLineText::rbf_get_intro(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    return rb_str_dup(mlt_data->v_intro);
}

VALUE AMS::MultiLineText::rbf_set_intro(VALUE self, VALUE v_intro) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    mlt_data->v_intro = rb_str_dup(StringValue(v_intro));
    c_update(mlt_data);
    return Qnil;
}

VALUE AMS::MultiLineText::rbf_line_numbers_visible(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    return RU::to_value(mlt_data->show_line_numbers);
}

VALUE AMS::MultiLineText::rbf_show_line_numbers(VALUE self, VALUE v_state) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    mlt_data->show_line_numbers = RU::value_to_bool(v_state);
    c_update(mlt_data);
    return Qnil;
}

VALUE AMS::MultiLineText::rbf_print(VALUE self, VALUE v_text) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    unsigned int text_ary_size = (unsigned int)RARRAY_LEN(mlt_data->v_text_ary);
    if (text_ary_size == 0) {
        rb_ary_push(mlt_data->v_text_ary, rb_str_dup(StringValue(v_text)));
        ++mlt_data->count;
    }
    else {
        VALUE v_string = rb_ary_entry(mlt_data->v_text_ary, text_ary_size - 1);
        rb_str_concat(v_string, StringValue(v_text));
    }
    c_update(mlt_data);
    return Qnil;
}

VALUE AMS::MultiLineText::rbf_puts(VALUE self, VALUE v_text) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    rb_ary_push(mlt_data->v_text_ary, rb_str_dup(StringValue(v_text)));
    ++mlt_data->count;
    c_update(mlt_data);
    return Qnil;
}

VALUE AMS::MultiLineText::rbf_clear(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    rb_ary_clear(mlt_data->v_text_ary);
    mlt_data->count = 0;
    c_update(mlt_data);
    return Qnil;
}

VALUE AMS::MultiLineText::rbf_remove(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    if (mlt_data->v_entity != Qnil && rb_funcall(mlt_data->v_entity, RU::INTERN_TVALID, 0) == Qtrue) {
        rb_funcall(mlt_data->v_entity, RU::INTERN_SMATERIAL, 1, Qnil);
        rb_funcall(mlt_data->v_entity, RU::INTERN_EERASE, 0);
        mlt_data->v_entity = Qnil;
    }
    rb_ary_clear(mlt_data->v_text_ary);
    mlt_data->count = 0;
    return Qnil;
}

VALUE AMS::MultiLineText::rbf_get_entity(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    if (mlt_data->v_entity != Qnil && rb_funcall(mlt_data->v_entity, RU::INTERN_TVALID, 0) == Qfalse)
        mlt_data->v_entity = Qnil;
    return mlt_data->v_entity;
}

VALUE AMS::MultiLineText::rbf_set_entity(VALUE self, VALUE v_entity) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    if (rb_obj_is_kind_of(v_entity, RU::SU_TEXT) == Qfalse)
        rb_raise(rb_eTypeError, "Expected a Sketchup::Text object!");
    mlt_data->v_entity = v_entity;
    c_update(mlt_data);
    return Qnil;
}

VALUE AMS::MultiLineText::rbf_get_count(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);
    return RU::to_value(mlt_data->count);
}

VALUE AMS::MultiLineText::rbf_get_position(VALUE self) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);

    VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_view = rb_funcall(v_model, RU::INTERN_ACTIVE_VIEW, 0);
    int vpwidth = RU::value_to_int(rb_funcall(v_view, RU::INTERN_VPWIDTH, 0));
    int vpheight = RU::value_to_int(rb_funcall(v_view, RU::INTERN_VPWIDTH, 0));

    return rb_ary_new3(
        2,
        RU::to_value(static_cast<int>(mlt_data->xpos_ratio * vpwidth)),
        RU::to_value(static_cast<int>(mlt_data->ypos_ratio * vpheight)));
}

VALUE AMS::MultiLineText::rbf_set_position(VALUE self, VALUE v_x, VALUE v_y) {
    MultiLineTextData* mlt_data;
    Data_Get_Struct(self, MultiLineTextData, mlt_data);

    double x = RU::value_to_double(v_x);
    double y = RU::value_to_double(v_y);

    VALUE v_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
    VALUE v_view = rb_funcall(v_model, RU::INTERN_ACTIVE_VIEW, 0);
    VALUE v_entities = rb_funcall(v_model, RU::INTERN_ENTITIES, 0);
    VALUE v_active_entities = rb_funcall(v_model, RU::INTERN_ACTIVE_ENTITIES, 0);
    int vpwidth = RU::value_to_int(rb_funcall(v_view, RU::INTERN_VPWIDTH, 0));
    int vpheight = RU::value_to_int(rb_funcall(v_view, RU::INTERN_VPWIDTH, 0));
    if (vpwidth < 1) vpwidth = 800;
    if (vpheight < 1) vpheight = 600;

    // Preset position ratios for future use
    mlt_data->xpos_ratio = x / vpwidth;
    mlt_data->ypos_ratio = y / vpheight;

    // Make sure the text object is valid
    if (mlt_data->v_entity == Qnil || rb_funcall(mlt_data->v_entity, RU::INTERN_TVALID, 0) == Qfalse)
        return Qnil;

    // Make sure the text object is actually a screen-locked text
    if (rb_funcall(mlt_data->v_entity, RU::INTERN_THAS_LEADER, 0) == Qtrue)
        return Qnil;

    // Ensure the active model path is closed
    if (v_active_entities != v_entities) {
        VALUE v_state = Qtrue;
        while (v_state == Qtrue)
            v_state = rb_funcall(v_model, RU::INTERN_CLOSE_ACTIVE, 0);
    }

    // Add group with the text object
    VALUE v_group = rb_funcall(v_entities, RU::INTERN_ADD_GROUP, 1, mlt_data->v_entity);

    // Calculate new position
    VALUE v_ray = rb_funcall(v_view, RU::INTERN_PICKRAY, 2, v_x, v_y);
    VALUE v_point = rb_funcall(rb_ary_entry(v_ray, 0), RU::INTERN_OP_ADD, 1, rb_ary_entry(v_ray, 1));

    // Move group to the new position
    rb_funcall(v_group, RU::INTERN_EMOVE, 1, v_point);

    // Explode the group
    VALUE v_content = rb_funcall(v_group, RU::INTERN_EXPLODE, 0);
    // When the group is explode the text entity is recreated.
    // Find it and set it as the new entity.
    unsigned int content_size = (unsigned int)RARRAY_LEN(v_content);
    for (unsigned int i = 0; i < content_size; ++i) {
        VALUE v_entity = rb_ary_entry(v_content, i);
        if (rb_obj_is_kind_of(rb_ary_entry(v_content, i), RU::SU_TEXT) == Qtrue) {
            mlt_data->v_entity = v_entity;
            break;
        }
    }

    return Qnil;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::MultiLineText::init_ruby(VALUE mAMS) {
    VALUE cMultiLineText = rb_define_class_under(mAMS, "MultiLineText", rb_cObject);

    rb_define_alloc_func(cMultiLineText, c_class_allocate);

    rb_define_method(cMultiLineText, "initialize", VALUEFUNC(rbf_initialize), 2);
    rb_define_method(cMultiLineText, "initialize_copy", VALUEFUNC(rbf_initialize_copy), 1);
    rb_define_method(cMultiLineText, "get_limit", VALUEFUNC(rbf_get_limit), 0);
    rb_define_method(cMultiLineText, "set_limit", VALUEFUNC(rbf_set_limit), 1);
    rb_define_method(cMultiLineText, "get_intro", VALUEFUNC(rbf_get_intro), 0);
    rb_define_method(cMultiLineText, "set_intro", VALUEFUNC(rbf_set_intro), 1);
    rb_define_method(cMultiLineText, "line_numbers_visible?", VALUEFUNC(rbf_line_numbers_visible), 0);
    rb_define_method(cMultiLineText, "show_line_numbers", VALUEFUNC(rbf_show_line_numbers), 1);
    rb_define_method(cMultiLineText, "print", VALUEFUNC(rbf_print), 1);
    rb_define_method(cMultiLineText, "puts", VALUEFUNC(rbf_puts), 1);
    rb_define_method(cMultiLineText, "clear", VALUEFUNC(rbf_clear), 0);
    rb_define_method(cMultiLineText, "remove", VALUEFUNC(rbf_remove), 0);
    rb_define_method(cMultiLineText, "get_entity", VALUEFUNC(rbf_get_entity), 0);
    rb_define_method(cMultiLineText, "set_entity", VALUEFUNC(rbf_set_entity), 1);
    rb_define_method(cMultiLineText, "count", VALUEFUNC(rbf_get_count), 0);
    rb_define_method(cMultiLineText, "get_position", VALUEFUNC(rbf_get_position), 0);
    rb_define_method(cMultiLineText, "set_position", VALUEFUNC(rbf_set_position), 2);
}
