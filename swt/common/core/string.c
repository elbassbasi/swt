/*
 * string.c
 *
 *  Created on: 1 mars 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>
#include <wchar.h>
struct w_string_ref_ {
	w_string_ref s;
	char r[1];
};

struct w_string_ref_ empty_string_ = { .s = { 0, 0, -1 }, .r = { 0 } };
w_string_ref *empty_string = (w_string_ref*) &empty_string_;
w_string_ref* w_string_new(size_t length) {
	if (length == 0)
		return empty_string;
	w_string_ref *ref = malloc(sizeof(w_string_ref) + length + 1);
	if (ref != 0) {
		ref->sign = 0;
		ref->ref = 0;
		ref->length = length;
		ref->data[length] = 0;
	}
	return ref;
}
const char* w_string_get_chars(struct w_string_ref *str) {
	if (str == 0)
		return 0;
	else if (str->sign == 0) {
		return str->data;
	} else
		return (const char*) str;
}
const char* w_string_get_chars_not_null(struct w_string_ref *str) {
	if (str == 0)
		return "";
	else if (str->sign == 0) {
		return str->data;
	} else
		return (const char*) str;
}
size_t w_string_get_length(struct w_string_ref *str) {
	if (str == 0)
		return 0;
	else if (str->sign == 0)
		return str->length;
	else
		return strlen((const char*) str);
}
void w_string_init_from_chars(struct w_string_ref **str, const char *chars,
		wbool copy) {
	if (chars == 0) {
		*str = 0;
		return;
	} else if (chars[0] == 0) {
		*str = empty_string;
		return;
	} else {
		if (copy) {
			int length = strlen(chars);
			w_string_ref *ref = w_string_new(length);
			if (ref != 0) {
				memcpy(ref->data, chars, length);
			}
			*str = ref;
		} else
			*str = (struct w_string_ref*) chars;
	}
}
void w_string_set_chars(struct w_string_ref **str, const char *chars,
		wbool copy) {
	w_string_ref_dec(*str);
	w_string_init_from_chars(str, chars, copy);
}
void w_string_init_from_wchars(struct w_string_ref **str,
		const wchar_t *wchars) {
	if (wchars == 0) {
		*str = 0;
	} else if (wchars[0] == 0) {
		*str = empty_string;
	} else {
		int l = wcslen(wchars);
		int length = w_utf8_from_utf16(wchars, l, 0, 0);
		w_string_ref *ref = w_string_new(length);
		if (ref != 0) {
			w_utf8_from_utf16(wchars, l, ref->data, length);
		}
		*str = ref;
	}
}
void w_string_set_wchars(struct w_string_ref **str, const wchar_t *wchars) {
	w_string_ref_dec(*str);
	w_string_init_from_wchars(str, wchars);
}
void w_string_init_from_ref(w_string_ref **str, w_string_ref *s, wbool clone) {
	*str = 0;
	w_string_move(str, s, clone);
}
void w_string_move(w_string_ref **str, w_string_ref *s, wbool clone) {
	w_string_ref *last = *str;
	if (s == 0) {
		*str = s;
	} else if (s->sign == 0) {
		if (s->ref >= 0)
			s->ref++;
		*str = s;
	} else {
		w_string_set_chars(str, (const char*) s, clone);
	}
	w_string_ref_dec(last);
}
void w_string_copy(w_string_ref **str, w_string_ref *s) {
	w_string_ref_dec(*str);
	if (s == 0) {
		*str = s;
	} else if (s->sign == 0) {
		w_string_ref *ref = w_string_new(s->length);
		if (ref != 0) {
			memcpy(ref->data, s->data, s->length);
		}
		*str = ref;
	} else {
		w_string_set_chars(str, (const char*) s, W_TRUE);
	}
}
void w_string_copy_chars(w_string_ref **ref, const char *str, size_t size) {
	w_string_ref_dec(*ref);
	*ref = w_string_new(size);
	if (*ref != 0) {
		memcpy((*ref)->data, str, size);
	}
}
void w_string_ref_inc(w_string_ref *str) {
	if (str != 0 && str->sign == 0 && str->ref >= 0) {
		str->ref++;
	}
}
w_string_ref* w_string_ref_dec(w_string_ref *str) {
	if (str != 0 && str->sign == 0 && str->ref >= 0) {
		str->ref--;
		if (str->ref <= 0) {
			free(str);
			return 0;
		}
	}
	return str;
}
void w_string_sprintf_0(w_string_ref **str, const char *format, va_list args) {
	w_string_ref_dec(*str);
	va_list cp;
	va_copy(cp, args);
	int count = vsnprintf(0, 0, format, cp);
	va_end(cp);
	w_string_ref *ref = w_string_new(count);
	*str = ref;
	if (ref == 0)
		return;
	vsnprintf(ref->data, count, format, args);
	va_end(args);
}

void w_string_sprintf(w_string_ref **str, const char *format, ...) {
	va_list args;
	va_start(args, format);
	w_string_sprintf_0(str, format, args);
	va_end(args);
}
void w_string_concat(w_string_ref **str, w_string_ref *s1, w_string_ref *s2) {
	w_string_ref_dec(*str);
	int l1, l2;
	l1 = w_string_get_length(s1);
	l2 = w_string_get_length(s2);
	w_string_ref *ref = w_string_new(l1 + l2);
	*str = ref;
	if (ref != 0) {
		memcpy(ref->data, w_string_get_chars(s1), l1);
		memcpy(&ref->data[l1], w_string_get_chars(s2), l2);
		ref->data[l1 + l2] = 0;
	}
}

size_t w_alloc_string_ref(void *user_data, size_t size, void **string) {
	w_string_ref *ref = w_string_new(size);
	*((w_string_ref**) user_data) = ref;
	if (ref != 0) {
		*string = ref->data;
		return size;
	} else {
		*string = 0;
		return 0;
	}
}
