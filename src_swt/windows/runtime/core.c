/*
 * win32.c
 *
 *  Created on: 30 mai 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "core.h"
#include "../widgets/toolkit.h"
void unicode_set(struct UnicodeString *string, const char *str, int length) {
	if (str != 0) {
		int l = w_utf8_to_utf16(str, length, string->temp,
				sizeof(string->temp) / sizeof(string->temp[0]));
		if ((l + 1) > sizeof(string->temp) / sizeof(string->temp[0])) {
			unicode_alloc(string, l + 1);
			string->length = w_utf8_to_utf16(str, length, string->str, l + 1);
		} else {
			string->str = string->temp;
			string->length = l;
		}
	} else {
		unicode_init(string);
	}
}
void unicode_init(struct UnicodeString *string) {
	string->str = 0;
	string->length = 0;
}
void unicode_free(struct UnicodeString *string) {
	if (string->str != 0 && string->str != string->temp)
		free(string->str);
}
void unicode_alloc_tmp(struct UnicodeString *string) {
	string->str = string->temp;
	string->length = sizeof(string->temp) / sizeof(string->temp[0]);
}
void unicode_alloc(struct UnicodeString *string, size_t size) {
	if (size < (sizeof(string->temp) / sizeof(string->temp[0]))) {
		string->str = string->temp;
	} else {
		string->str = malloc(size * sizeof(WCHAR));
	}
	string->length = size;
}
w_string_ref* unicode_to_ref(WCHAR *text, int length) {
	w_string_ref *ref = 0;
	int l = w_utf8_from_utf16(text, length, 0, 0);
	ref = w_string_new(l);
	if (ref != 0) {
		w_utf8_from_utf16(text, length, ref->data, l);
	}
	return ref;
}
WCHAR* _win_text_fix(const char *text, size_t text_length, size_t *newlength,
		int enc) {
	WCHAR *s;
	if ((enc & 0xFF) == W_ENCODING_UNICODE) {
		if (text_length == -1) {
			s = (WCHAR*) text;
			*newlength = lstrlenW(s);
		} else {
			size_t total;
			s = _w_toolkit_malloc((text_length + 1) * sizeof(WCHAR));
			if (s != 0) {
				memcpy(s, text, text_length * sizeof(WCHAR));
				s[text_length] = 0;
				*newlength = text_length + 1;
			} else
				*newlength = 0;
		}
	} else {
		size_t total;
		s = (WCHAR*) _w_toolkit_malloc_all(&total);
		size_t l = w_utf8_to_utf16(text, text_length, s, total / sizeof(WCHAR));
		if (((l + 1) * sizeof(WCHAR)) > total) {
			win_toolkit->tmp_length -= total;
			s = _w_toolkit_malloc((l + 1) * sizeof(WCHAR));
			if (s != 0) {
				w_utf8_to_utf16(text, text_length, s, l + 1);
				*newlength = l + 1;
			} else
				*newlength = 0;
		} else {
			win_toolkit->tmp_length += (l + 1) * sizeof(WCHAR) - total;
			*newlength = l + 1;
		}
	}
	return s;
}
void _win_text_free(const char *text, WCHAR *alloc, int length) {
	if ((WCHAR*) text == alloc)
		return;
	_w_toolkit_free(alloc, length * sizeof(WCHAR));
}
wresult _win_text_set(WCHAR *text, int length, w_alloc alloc, void *user_data,
		int enc) {

}
HINSTANCE hinst;
int APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpvReserved) {
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		hinst = hInstance;
		break;

	case DLL_PROCESS_DETACH:
		hinst = 0;
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
#endif
