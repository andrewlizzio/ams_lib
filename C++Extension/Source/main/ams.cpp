/*
 * ---------------------------------------------------------------------------------------------------------------------
 *
 * Copyright (C) 2018, Anton Synytsia
 *
 * ---------------------------------------------------------------------------------------------------------------------
 */

#include "ams.h"
#include "ams_geometry.h"
#include "ams_group.h"
#include "ams_multi_line_text.h"


 /*
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   Variables
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 */

double AMS::E_VALUES[] = {
	0.0000000001,
	0.000000001,
	0.00000001,
	0.0000001,
	0.000001,
	0.00001,
	0.0001,
	0.001,
	0.01,
	0.1,
	1.0,
	10.0,
	100.0,
	1000.0,
	10000.0,
	100000.0,
	1000000.0,
	10000000.0,
	100000000.0,
	1000000000.0,
	10000000000.0
};


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Helper Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::c_inspect_element(VALUE v_item) {
	VALUE v_dest_str = rb_str_new2("");
	c_inspect_element_dest(v_item, v_dest_str);
#ifdef HAVE_RUBY_ENCODING_H
	static int enc_index = rb_enc_find_index("UTF-8");
	rb_enc_associate_index(v_dest_str, enc_index);
#endif
	return v_dest_str;
}

void AMS::c_inspect_element_dest(VALUE v_item, VALUE v_dest_str) {
	int type = rb_type(v_item);
	if (type == T_NIL)
		rb_str_cat(v_dest_str, "nil", 3);
	else if (type == T_TRUE)
		rb_str_cat(v_dest_str, "true", 4);
	else if (type == T_FALSE)
		rb_str_cat(v_dest_str, "false", 5);
	else if (type == T_FIXNUM || type == T_BIGNUM) {
		VALUE v_res = rb_funcall(v_item, RU::INTERN_TO_S, 0);
		const char* item = RU::value_to_c_str(v_res);
		rb_str_cat(v_dest_str, item, (long)strlen(item));
	}
	else if (type == T_FLOAT) {
		VALUE v_res = rb_funcall(v_item, RU::INTERN_TO_S, 0);
		const char* item = RU::value_to_c_str(v_res);
		if (strncmp(item, "Infinity", 8) == 0)
			rb_str_cat(v_dest_str, "1.0/0.0", 7);
		else if (strncmp(item, "-Infinity", 9) == 0)
			rb_str_cat(v_dest_str, "-1.0/0.0", 8);
		else if (strncmp(item, "NaN", 3) == 0)
			rb_str_cat(v_dest_str, "0.0/0.0", 7);
		else
			rb_str_cat(v_dest_str, item, (long)strlen(item));
	}
	else if (type == T_STRING) {
		VALUE v_res = rb_funcall(v_item, RU::INTERN_INSPECT, 0);
		const char* item = RU::value_to_c_str(v_res);
		rb_str_cat(v_dest_str, item, (long)strlen(item));
	}
	else if (type == T_SYMBOL) {
		VALUE v_res = rb_funcall(v_item, RU::INTERN_TO_S, 0);
		const char* item = RU::value_to_c_str(v_res);
		rb_str_cat(v_dest_str, ":", 1);
		rb_str_cat(v_dest_str, item, (long)strlen(item));
	}
	else if (type == T_ARRAY) {
		rb_str_cat(v_dest_str, "[", 1);
		unsigned int len = (unsigned int)RARRAY_LEN(v_item);
		for (unsigned int i = 0; i < len; ++i) {
			VALUE v_sub_item = rb_ary_entry(v_item, i);
			c_inspect_element_dest(v_sub_item, v_dest_str);
			if (i < len - 1) rb_str_cat(v_dest_str, ", ", 2);
		}
		rb_str_cat(v_dest_str, "]", 1);
	}
	else if (type == T_HASH) {
		rb_str_cat(v_dest_str, "{", 1);
		VALUE v_keys = rb_funcall(v_item, RU::INTERN_KEYS, 0);
		unsigned int len = (unsigned int)RARRAY_LEN(v_keys);
		for (unsigned int i = 0; i < len; ++i) {
			VALUE v_key = rb_ary_entry(v_keys, i);
			VALUE v_val = rb_hash_aref(v_item, v_key);
			c_inspect_element_dest(v_key, v_dest_str);
			rb_str_cat(v_dest_str, " => ", 4);
			c_inspect_element_dest(v_val, v_dest_str);
			if (i < len - 1) rb_str_cat(v_dest_str, ", ", 2);
		}
		rb_str_cat(v_dest_str, "}", 1);
		rb_ary_clear(v_keys);
	}
	else {
		VALUE v_element_obj_str = rb_funcall(v_item, RU::INTERN_TO_S, 0);
		VALUE v_element_class_str = rb_funcall(rb_class_of(v_item), RU::INTERN_TO_S, 0);
		const char* element_obj_str = RU::value_to_c_str(v_element_obj_str);
		const char* element_class_str = RU::value_to_c_str(v_element_class_str);
		rb_raise(rb_eTypeError, "An element, %s, of type '%s' is not supported! Supported types are Nil, Boolean, Fixnum, Float, Bignum, String, Symbol, Array, and Hash.", element_obj_str, element_class_str);
	}
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Ruby Functions
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

VALUE AMS::rbf_inspect_element(VALUE self, VALUE v_item) {
	return c_inspect_element(v_item);
}

VALUE AMS::rbf_validate_type(int argc, VALUE* argv, VALUE self) {
	if (argc < 2) {
		rb_raise(rb_eArgError, "Wrong number of arguments! Expected at least 2 arguments.");
	}
	else if (TYPE(argv[1]) == T_ARRAY) {
		VALUE v_kinds = argv[1];
		unsigned int ary_len = static_cast<unsigned int>(RARRAY_LEN(argv[1]));
		if (ary_len == 0)
			rb_raise(rb_eTypeError, "Expected at least one type to check against.");
		for (unsigned int i = 0; i < ary_len; ++i)
			if (rb_obj_is_kind_of(argv[0], rb_ary_entry(v_kinds, i)) == Qtrue) return Qnil;
		std::string msg("Invalid parameter! Expected ");
		if (ary_len == 1) {
			msg.append(rb_class2name(rb_ary_entry(v_kinds, 0)));
			//rb_str_concat(v_string, rb_funcall(rb_ary_entry(v_kinds, 0), RU::INTERN_TO_S, 0));
		}
		else if (ary_len == 2) {
			msg.append(rb_class2name(rb_ary_entry(v_kinds, 0)));
			msg.append(" or ");
			msg.append(rb_class2name(rb_ary_entry(v_kinds, 1)));
			//rb_str_concat(v_string, rb_funcall(rb_ary_entry(v_kinds, 0), RU::INTERN_TO_S, 0));
			//rb_str_cat2(v_string, " or ");
			//rb_str_concat(v_string, rb_funcall(rb_ary_entry(v_kinds, 1), RU::INTERN_TO_S, 0));
		}
		else {
			unsigned int i;
			for (i = 0; i < ary_len - 1; ++i) {
				msg.append(rb_class2name(rb_ary_entry(v_kinds, i)));
				msg.append(", ");
				//rb_str_concat(v_string, rb_funcall(rb_ary_entry(v_kinds, i), RU::INTERN_TO_S, 0));
				//rb_str_cat2(v_string, ", ");
			}
			msg.append("or ");
			msg.append(rb_class2name(rb_ary_entry(v_kinds, i)));
			//rb_str_cat2(v_string, "or ");
			//rb_str_concat(v_string, rb_funcall(rb_ary_entry(v_kinds, size - 1), RU::INTERN_TO_S, 0));
		}
		VALUE obj_class = rb_class_of(argv[0]);
		msg.append(" but got ");
		msg.append(rb_class2name(obj_class));
		msg.append(".");
		rb_raise(rb_eTypeError, "%s", msg.c_str());
		//rb_str_cat2(v_string, " but got ");
		//VALUE v_object_class_str = rb_funcall(rb_class_of(argv[0]), RU::INTERN_TO_S, 0);
		//rb_str_concat(v_string, v_object_class_str);
		//rb_str_cat2(v_string, ".");
		//const char* err_info = RU::value_to_c_str(v_string);
		//rb_raise(rb_eTypeError, "%s", err_info);
	}
	else {
		int i;
		for (i = 1; i < argc; ++i)
			if (rb_obj_is_kind_of(argv[0], argv[i]) == Qtrue) return Qnil;
		//VALUE v_string = rb_str_new2("Invalid parameter! Expected ");
		std::string msg("Invalid parameter! Expected ");
		if (argc == 2) {
			msg.append(rb_class2name(argv[1]));
			//rb_str_concat(v_string, rb_funcall(argv[1], RU::INTERN_TO_S, 0));
		}
		else if (argc == 3) {
			msg.append(rb_class2name(argv[1]));
			msg.append(" or ");
			msg.append(rb_class2name(argv[2]));
			//rb_str_concat(v_string, rb_funcall(argv[1], RU::INTERN_TO_S, 0));
			//rb_str_cat2(v_string, " or ");
			//rb_str_concat(v_string, rb_funcall(argv[2], RU::INTERN_TO_S, 0));
		}
		else {
			for (i = 1; i < argc - 1; ++i) {
				msg.append(rb_class2name(argv[i]));
				msg.append(", ");
				//rb_str_concat(v_string, rb_funcall(argv[i], RU::INTERN_TO_S, 0));
				//rb_str_cat2(v_string, ", ");
			}
			msg.append("or ");
			msg.append(rb_class2name(argv[i]));
			//rb_str_cat2(v_string, "or ");
			//rb_str_concat(v_string, rb_funcall(argv[argc - 1], RU::INTERN_TO_S, 0));
		}
		VALUE obj_class = rb_class_of(argv[0]);
		msg.append(" but got ");
		msg.append(rb_class2name(obj_class));
		msg.append(".");
		rb_raise(rb_eTypeError, "%s", msg.c_str());
		//rb_str_cat2(v_string, " but got ");
		//VALUE v_object_class_str = rb_funcall(rb_class_of(argv[0]), RU::INTERN_TO_S, 0);
		//rb_str_concat(v_string, v_object_class_str);
		//rb_str_cat2(v_string, ".");
		//const char* err_info = RU::value_to_c_str(v_string);
		//rb_raise(rb_eTypeError, "%s", err_info);
	}
}

VALUE AMS::rbf_clamp_real(VALUE self, VALUE v_value, VALUE v_min, VALUE v_max) {
	return RU::to_value(Geom::clamp_double(
		RU::value_to_double(v_value),
		RU::value_to_double(v_min),
		RU::value_to_double(v_max)));
}

VALUE AMS::rbf_min_real(VALUE self, VALUE v_a, VALUE v_b) {
	return RU::to_value(Geom::min_double(
		RU::value_to_double(v_a),
		RU::value_to_double(v_b)));
}

VALUE AMS::rbf_max_real(VALUE self, VALUE v_a, VALUE v_b) {
	return RU::to_value(Geom::max_double(
		RU::value_to_double(v_a),
		RU::value_to_double(v_b)));
}

VALUE AMS::rbf_sign_real(VALUE self, VALUE v_value) {
	double value = RU::value_to_double(v_value);
	if (value < 0.0)
		return RU::to_value(-1);
	else
		return RU::to_value(1);
}

VALUE AMS::rbf_clamp_int(VALUE self, VALUE v_value, VALUE v_min, VALUE v_max) {
	return RU::to_value(Geom::clamp_ll(
		RU::value_to_ll(v_value),
		RU::value_to_ll(v_min),
		RU::value_to_ll(v_max)));
}

VALUE AMS::rbf_min_int(VALUE self, VALUE v_a, VALUE v_b) {
	return RU::to_value(Geom::min_ll(
		RU::value_to_ll(v_a),
		RU::value_to_ll(v_b)));
}

VALUE AMS::rbf_max_int(VALUE self, VALUE v_a, VALUE v_b) {
	return RU::to_value(Geom::max_ll(
		RU::value_to_ll(v_a),
		RU::value_to_ll(v_b)));
}

VALUE AMS::rbf_sign_int(VALUE self, VALUE v_value) {
	long long value = RU::value_to_ll(v_value);
	return (1 | (value >> (sizeof(long long) * 8 - 1)));
}

VALUE AMS::rbf_round(int argc, VALUE* argv, VALUE self) {
	int precision = 0;
	if (argc == 2)
		precision = Geom::clamp_int(RU::value_to_int(argv[1]), 0, 10);
	else if (argc != 1)
		rb_raise(rb_eArgError, "Wrong number of arguments! Expected 1..2 arguments.");
	double number = RU::value_to_double(argv[0]);

	number *= E_VALUES[precision + 10];
	number = floor(fabs(number) + 0.5) * Geom::sign(number);
	number *= E_VALUES[10 - precision];

	return RU::to_value(number);
}

VALUE AMS::rbf_is_boolean(VALUE self, VALUE v_value) {
	return (v_value == Qtrue || v_value == Qfalse) ? Qtrue : Qfalse;
}

VALUE AMS::rbf_get_entity_by_id(VALUE self, VALUE v_id) {
	int id = RU::value_to_int(v_id);
	VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
	// Test top level entities
	VALUE v_entities = rb_funcall(v_active_model, RU::INTERN_ENTITIES, 0);
	unsigned int size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
	for (unsigned int i = 0; i < size; ++i) {
		VALUE v_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
		int item_id = RU::value_to_int(rb_funcall(v_entity, RU::INTERN_ENTITYID, 0));
		if (item_id == id) return v_entity;
	}
	// Test definitions
	VALUE v_definitions = rb_funcall(v_active_model, RU::INTERN_DEFINITIONS, 0);
	size = RU::value_to_uint(rb_funcall(v_definitions, RU::INTERN_LENGTH, 0));
	for (unsigned int i = 0; i < size; ++i) {
		// Test definition
		VALUE v_definition = rb_funcall(v_definitions, RU::INTERN_AT, 1, INT2FIX(i));
		int item_id = RU::value_to_int(rb_funcall(v_definition, RU::INTERN_ENTITYID, 0));
		if (item_id == id) return v_definition;
		// Test definition instances
		VALUE v_instances = rb_funcall(v_definition, RU::INTERN_INSTANCES, 0);
		unsigned int sub_size = RU::value_to_uint(rb_funcall(v_instances, RU::INTERN_LENGTH, 0));
		for (unsigned int j = 0; j < sub_size; ++j) {
			VALUE v_instance = rb_funcall(v_instances, RU::INTERN_AT, 1, INT2FIX(j));
			int sub_item_id = RU::value_to_int(rb_funcall(v_instance, RU::INTERN_ENTITYID, 0));
			if (sub_item_id == id) return v_instance;
		}
		// Test definition entities
		VALUE v_sub_entities = rb_funcall(v_definition, RU::INTERN_ENTITIES, 0);
		sub_size = RU::value_to_uint(rb_funcall(v_sub_entities, RU::INTERN_LENGTH, 0));
		for (unsigned int j = 0; j < sub_size; ++j) {
			VALUE v_entity = rb_funcall(v_sub_entities, RU::INTERN_AT, 1, INT2FIX(j));
			int sub_item_id = RU::value_to_int(rb_funcall(v_entity, RU::INTERN_ENTITYID, 0));
			if (sub_item_id == id) return v_entity;
		}
	}
	// Test materials
	VALUE v_materials = rb_funcall(v_active_model, RU::INTERN_MATERIALS, 0);
	size = RU::value_to_uint(rb_funcall(v_materials, RU::INTERN_LENGTH, 0));
	for (unsigned int i = 0; i < size; ++i) {
		VALUE v_material = rb_funcall(v_materials, RU::INTERN_AT, 1, INT2FIX(i));
		int item_id = RU::value_to_int(rb_funcall(v_material, RU::INTERN_ENTITYID, 0));
		if (item_id == id) return v_material;
	}
	// Test layers
	VALUE v_layers = rb_funcall(v_active_model, RU::INTERN_LAYERS, 0);
	size = RU::value_to_uint(rb_funcall(v_layers, RU::INTERN_LENGTH, 0));
	for (unsigned int i = 0; i < size; ++i) {
		VALUE v_layer = rb_funcall(v_layers, RU::INTERN_AT, 1, INT2FIX(i));
		int item_id = RU::value_to_int(rb_funcall(v_layer, RU::INTERN_ENTITYID, 0));
		if (item_id == id) return v_layer;
	}
	// Test styles
	VALUE v_styles = rb_funcall(v_active_model, RU::INTERN_STYLES, 0);
	size = RU::value_to_uint(rb_funcall(v_styles, RU::INTERN_SIZE, 0));
	for (unsigned int i = 0; i < size; ++i) {
		VALUE v_style = rb_funcall(v_styles, RU::INTERN_OP_SQUARE_BRACKETS, 1, INT2FIX(i));
		int item_id = RU::value_to_int(rb_funcall(v_style, RU::INTERN_ENTITYID, 0));
		if (item_id == id) return v_style;
	}
	// Test pages
	VALUE v_pages = rb_funcall(v_active_model, RU::INTERN_PAGES, 0);
	size = RU::value_to_uint(rb_funcall(v_pages, RU::INTERN_SIZE, 0));
	for (unsigned int i = 0; i < size; ++i) {
		VALUE v_page = rb_funcall(v_pages, RU::INTERN_OP_SQUARE_BRACKETS, 1, INT2FIX(i));
		int item_id = RU::value_to_int(rb_funcall(v_page, RU::INTERN_ENTITYID, 0));
		if (item_id == id) return v_page;
	}
	// Return nil otherwise
	return Qnil;
}

VALUE AMS::rbf_get_top_entity_by_id(VALUE self, VALUE v_id) {
	int id = RU::value_to_int(v_id);
	VALUE v_active_model = rb_funcall(RU::SU_SKETCHUP, RU::INTERN_ACTIVE_MODEL, 0);
	// Test top level entities
	VALUE v_entities = rb_funcall(v_active_model, RU::INTERN_ENTITIES, 0);
	unsigned int size = RU::value_to_uint(rb_funcall(v_entities, RU::INTERN_LENGTH, 0));
	for (unsigned int i = 0; i < size; ++i) {
		VALUE v_entity = rb_funcall(v_entities, RU::INTERN_AT, 1, INT2FIX(i));
		int item_id = RU::value_to_int(rb_funcall(v_entity, RU::INTERN_ENTITYID, 0));
		if (item_id == id) return v_entity;
	}
	// Return nil otherwise
	return Qnil;
}


/*
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Main
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/

void AMS::init_ruby(VALUE mAMS) {
	RU::init_ruby();

	rb_define_module_function(mAMS, "inspect_element", VALUEFUNC(AMS::rbf_inspect_element), 1);
	rb_define_module_function(mAMS, "validate_type", VALUEFUNC(AMS::rbf_validate_type), -1);
	rb_define_module_function(mAMS, "clamp_real", VALUEFUNC(AMS::rbf_clamp_real), 3);
	rb_define_module_function(mAMS, "min_real", VALUEFUNC(AMS::rbf_min_real), 2);
	rb_define_module_function(mAMS, "max_real", VALUEFUNC(AMS::rbf_max_real), 2);
	rb_define_module_function(mAMS, "sign_real", VALUEFUNC(AMS::rbf_sign_real), 1);
	rb_define_module_function(mAMS, "clamp_int", VALUEFUNC(AMS::rbf_clamp_int), 3);
	rb_define_module_function(mAMS, "min_int", VALUEFUNC(AMS::rbf_min_int), 2);
	rb_define_module_function(mAMS, "max_int", VALUEFUNC(AMS::rbf_max_int), 2);
	rb_define_module_function(mAMS, "sign_int", VALUEFUNC(AMS::rbf_sign_int), 1);
	rb_define_module_function(mAMS, "round", VALUEFUNC(AMS::rbf_round), -1);
	rb_define_module_function(mAMS, "is_bool?", VALUEFUNC(AMS::rbf_is_boolean), 1);
	rb_define_module_function(mAMS, "is_boolean?", VALUEFUNC(AMS::rbf_is_boolean), 1);
	rb_define_module_function(mAMS, "get_entity_by_id", VALUEFUNC(AMS::rbf_get_entity_by_id), 1);
	rb_define_module_function(mAMS, "get_top_entity_by_id", VALUEFUNC(AMS::rbf_get_top_entity_by_id), 1);

	AMS::Geometry::init_ruby(mAMS);
	AMS::Group::init_ruby(mAMS);
	AMS::MultiLineText::init_ruby(mAMS);
}
