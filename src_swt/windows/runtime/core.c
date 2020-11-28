/*
 * win32.c
 *
 *  Created on: 30 mai 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "core.h"
void unicode_set(struct UnicodeString *string, const char *str, int length) {
	if (str != 0) {
		int l = w_utf8_to_utf16(str, length, string->temp,
				sizeof(string->temp) / sizeof(string->temp[0]));
		if ((l + 1) > sizeof(string->temp) / sizeof(string->temp[0])) {
			unicode_alloc(string, l + 1);
			string->length = w_utf8_to_utf16(str, length, string->str, l + 1);
		}else{
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
