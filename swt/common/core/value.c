/*
 * value.c
 *
 *  Created on: 20 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>
wresult w_value_unknown_free(w_value *value) {
	return W_FALSE;
}
wresult w_value_unknown_convert(w_value *value, w_value *v2) {
	return W_FALSE;
}
w_value_class w_value_unknown_class = { //
		W_VALUE_POINTER, //
				0, //
				w_value_unknown_free, //
				w_value_unknown_convert, //
		};
/*
 * pointer type
 */
wresult w_value_pointer_free(w_value *value) {
	if (value->flags & W_VALUE_FREE_MEMORY) {
		free(value->pointer);
	}
	return W_TRUE;
}
wresult w_value_pointer_convert(w_value *value, w_value *v2) {
	switch (v2->clazz->type) {
	case W_VALUE_POINTER:
		value->pointer = v2->pointer;
		break;
	case W_VALUE_UTF8:
		value->pointer = v2->string;
		break;
	default:
		value->pointer = 0;
		return W_FALSE;
	}
	return W_TRUE;
}
w_value_class w_value_pointer_class = { //
		W_VALUE_POINTER, //
				0, //
				w_value_pointer_free, //
				w_value_pointer_convert, //
		};
/*
 * Int type
 */
wresult w_value_int_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_int_convert(w_value *value, w_value *v2) {
	if (v2->clazz != 0) {
		switch (v2->clazz->type) {
		case W_VALUE_POINTER:
			v2->pointer = 0;
			break;
		case W_VALUE_BOOL:
		case W_VALUE_CHAR:
		case W_VLAUE_SHORT:
		case W_VALUE_INT:
		case W_VALUE_INT64:
			v2->INT64 = value->INT64;
			break;
		case W_VALUE_FLOAT:
			v2->FLOAT = value->INT64;
			break;
		case W_VALUE_DOUBLE:
			v2->DOUBLE = value->INT64;
			break;
		case W_VALUE_UTF8: {
			char *s = w_value_string_create(v2, 20);
			if (s != 0) {
				sprintf(s, "%ld", value->INT64);
			}
		}
			break;
		default:
			return W_FALSE;
		}
		return W_TRUE;
	}
	return W_FALSE;
}
/*
 * Int type
 */
wresult w_value_bool_convert(w_value *value, w_value *v2) {
	if (v2->clazz != 0) {
		if (v2->clazz->type == W_VALUE_UTF8) {
			if (value->INT64) {
				w_value_set_string(v2, "true", -1, 0);
			} else {
				w_value_set_string(v2, "false", -1, 0);
			}
			return W_TRUE;
		}
	}
	return w_value_int_convert(value, v2);
}
w_value_class w_value_bool_class = { //
		W_VALUE_BOOL, //
				0, //
				w_value_int_free, //
				w_value_bool_convert, //
		};
w_value_class w_value_char_class = { //
		W_VALUE_CHAR, //
				0, //
				w_value_int_free, //
				w_value_int_convert, //
		};
w_value_class w_value_short_class = { //
		W_VLAUE_SHORT, //
				0, //
				w_value_int_free, //
				w_value_int_convert, //
		};
w_value_class w_value_int_class = { //
		W_VALUE_INT, //
				0, //
				w_value_int_free, //
				w_value_int_convert, //
		};
w_value_class w_value_int64_class = { //
		W_VALUE_INT64, //
				0, //
				w_value_int_free, //
				w_value_int_convert, //
		};
/*
 * float type
 */
wresult w_value_float_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_float_convert(w_value *value, w_value *v2) {
	if (v2->clazz != 0) {
		switch (v2->clazz->type) {
		case W_VALUE_POINTER:
			v2->pointer = 0;
			break;
		case W_VALUE_BOOL:
		case W_VALUE_CHAR:
		case W_VLAUE_SHORT:
		case W_VALUE_INT:
		case W_VALUE_INT64:
			v2->INT64 = value->INT64;
			break;
		case W_VALUE_FLOAT:
			v2->FLOAT = value->INT64;
			break;
		case W_VALUE_DOUBLE:
			v2->DOUBLE = value->INT64;
			break;
		case W_VALUE_UTF8:
			v2->string = malloc(20);
			if (v2->string != 0) {
				sprintf(v2->string, "%ld", value->INT64);
			}
			break;
		default:
			return W_FALSE;
		}
		return W_TRUE;
	}
	return W_FALSE;
}
w_value_class w_value_float_class = { //
		W_VALUE_FLOAT, //
				0, //
				w_value_float_free, //
				w_value_float_convert, //
		};
/*
 * double type
 */
wresult w_value_double_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_double_convert(w_value *value, w_value *v2) {
	if (v2->clazz != 0) {
		switch (v2->clazz->type) {
		case W_VALUE_POINTER:
			v2->pointer = 0;
			break;
		case W_VALUE_BOOL:
		case W_VALUE_CHAR:
		case W_VLAUE_SHORT:
		case W_VALUE_INT:
		case W_VALUE_INT64:
			v2->INT64 = value->INT64;
			break;
		case W_VALUE_FLOAT:
			v2->FLOAT = value->INT64;
			break;
		case W_VALUE_DOUBLE:
			v2->DOUBLE = value->INT64;
			break;
		case W_VALUE_UTF8:
			v2->string = malloc(20);
			if (v2->string != 0) {
				sprintf(v2->string, "%ld", value->INT64);
			}
			break;
		default:
			return W_FALSE;
		}
		return W_TRUE;
	}
	return W_FALSE;
}
w_value_class w_value_double_class = { //
		W_VALUE_DOUBLE, //
				0, //
				w_value_double_free, //
				w_value_double_convert, //
		};
/*
 * float type
 */
wresult w_value_utf8_free(w_value *value) {
	return W_TRUE;
}
wresult w_value_utf8_convert(w_value *value, w_value *v2) {
	if (v2->clazz != 0) {
		switch (v2->clazz->type) {
		case W_VALUE_POINTER:
			value->INT64 = ((wint64*) v2->pointer)[0];
			break;
		case W_VALUE_BOOL:
		case W_VALUE_CHAR:
		case W_VLAUE_SHORT:
		case W_VALUE_INT:
		case W_VALUE_INT64:
			value->INT64 = v2->INT64;
			break;
		case W_VALUE_FLOAT:
			value->INT64 = v2->FLOAT;
			break;
		case W_VALUE_DOUBLE:
			value->INT64 = v2->DOUBLE;
			break;
		case W_VALUE_UTF8:
			value->INT64 = atoll(v2->string);
			break;
		default:
			return W_FALSE;
		}
		return W_TRUE;
	}
	return W_FALSE;
}
w_value_class w_value_utf8_class = { //
		W_VALUE_UTF8, //
				0, //
				w_value_utf8_free, //
				w_value_utf8_convert, //
		};
wushort id = 0x100;
void w_value_registre_class(w_value_class *clazz) {
	clazz->type = id;
	id++;
}
void w_value_init_pointer(w_value *value, void *pointer, size_t length,
		int flags) {
	value->clazz = &w_value_pointer_class;
	value->flags = flags;
	value->size = length;
	value->pointer = pointer;
}
void w_value_init_bool(w_value *value, int _int) {
	value->clazz = &w_value_bool_class;
	value->INT64 = _int;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_char(w_value *value, char _char) {
	value->clazz = &w_value_char_class;
	value->INT64 = _char;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_short(w_value *value, short _short) {
	value->clazz = &w_value_short_class;
	value->INT64 = _short;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_int(w_value *value, int _int) {
	value->clazz = &w_value_int_class;
	value->INT64 = _int;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_int64(w_value *value, wint64 _int64) {
	value->clazz = &w_value_int64_class;
	value->INT64 = _int64;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_double(w_value *value, double _double) {
	value->clazz = &w_value_double_class;
	value->DOUBLE = _double;
	value->flags = 0;
	value->size = 0;
}
void w_value_init_float(w_value *value, float _float) {
	value->clazz = &w_value_float_class;
	value->FLOAT = _float;
	value->flags = 0;
	value->size = 0;
}
wbool w_value_init_string(w_value *value, const char *string, size_t length,
		int flags) {
	value->clazz = &w_value_utf8_class;
	value->flags = flags;
	value->size = length;
	if (flags & W_VALUE_FREE_MEMORY) {
		value->string = strdup(string);
	} else {
		value->string = (char*) string;
	}
	return W_TRUE;
}
void w_value_set_int(w_value *value, int _int) {
	w_value_free(value);
	w_value_init_int(value, _int);
}
void w_value_set_float(w_value *value, float _float) {
	w_value_free(value);
	w_value_init_float(value, _float);
}
wbool w_value_set_string(w_value *value, const char *string, size_t length,
		int flags) {
	w_value_free(value);
	return w_value_init_string(value, string, length, flags);
}
void w_value_set_pointer(w_value *value, void *pointer, size_t length,
		int flags) {
	w_value_free(value);
	return w_value_init_pointer(value, pointer, length, flags);
}

void w_value_init(w_value *value) {
	value->clazz = &w_value_unknown_class;
	value->pointer = 0;
	value->flags = 0;
	value->size = 0;
}
void w_value_free(w_value *value) {
	value->clazz->free(value);
	value->clazz = &w_value_unknown_class;
}

wint64 w_value_get_int(w_value *value) {
	w_value v2;
	w_value_init_int64(&v2, 0);
	value->clazz->convert_to(value, &v2);
	return v2.INT64;
}
float w_value_get_float(w_value *value) {
	w_value v2;
	w_value_init_float(&v2, 0);
	value->clazz->convert_to(value, &v2);
	return v2.FLOAT;
}
double w_value_get_double(w_value *value) {
	w_value v2;
	w_value_init_double(&v2, 0);
	value->clazz->convert_to(value, &v2);
	return v2.DOUBLE;
}
const char* w_value_get_string(w_value *value) {
	w_value v2;
	w_value_init_string(&v2, 0, 0, 0);
	value->clazz->convert_to(value, &v2);
	w_value_free(&v2);
	return v2.string;
}
void w_value_string_copy(w_value *value, char *text, size_t length) {
}
char* w_value_string_copy_is_needed(w_value *value, char *text, size_t length) {
	w_value v2;
	w_value_init_string(&v2, text, length, 0);
	value->clazz->convert_to(value, &v2);
	w_value_free(&v2);
	return v2.string;
}
char* w_value_string_create(w_value *value, size_t length) {
	if (value->clazz == &w_value_utf8_class
			&& (value->flags & W_VALUE_USER_MEMORY) && value->string != 0
			&& value->size >= length) {
		return value->string;
	} else {
		w_value_free(value);
		value->clazz = &w_value_utf8_class;
		value->string = (char*) malloc(length);
		value->flags = W_VALUE_FREE_MEMORY;
		value->size = length;
		return value->string;
	}
}
int w_value_vprint(w_value *value, const char *Template, va_list args) {
	size_t sz = vsnprintf(0, value->size, Template, args);
	char *s = w_value_string_create(value, sz + 1);
	if (s != 0) {
		size_t sz = vsnprintf(s, sz, Template, args);
	}
	return sz;
}
int w_value_print(w_value *value, const char *Template, ...) {
	va_list args;
	va_start(args, Template);
	int ret = w_value_vprint(value, Template, args);
	va_end(args);
	return ret;
}