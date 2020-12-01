/*
 * mnemonics.c
 *
 *  Created on: 23 juil. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "graphics.h"
#include "../widgets/toolkit.h"
int _mnemonics_fix_0(struct _mnemonics *mnemonics, const char *text,
		size_t text_length, int replace, int *process) {
	char ch, _t[2], _l;
	size_t length = 0;
	for (size_t i = 0; (i < text_length && (ch = text[i]) != 0); i++) {
		_l = 1;
		if (ch == '&') {
			if (i + 1 < text_length && text[i + 1] == '&') {
				_t[0] = '&';
				i++;
			} else {
				if (replace) {
					_t[0] = '_';
				}
			}
			*process = 1;
		} else if (ch == '_') {
			if (replace) {
				_t[0] = '_';
			} else {
				_t[0] = '_';
				_t[1] = ch;
				_l = 2;
			}
			*process = 1;
		} else if (ch == '\t') {
			_t[0] = ' ';
		} else {
			_t[0] = ch;
		}
		if (length < mnemonics->length) {
			memcpy(&mnemonics->str[length], _t, _l);
		}
		length += _l;
	}
	return length;
}
void _mnemonics_fix(_mnemonics *mnemonics, const char *text, size_t text_length,
		int replace) {
	int process = 0, length;
	if (gtk_toolkit->mem_tmp_allocated == 0) {
		mnemonics->length = gtk_toolkit->mem_tmp_total_size - 1;
		mnemonics->str = gtk_toolkit->mem_tmp;
	} else {
		mnemonics->length = 0;
		mnemonics->str = 0;
	}
	length = _mnemonics_fix_0(mnemonics, text, text_length, replace, &process);
	if (process == 0) {
		mnemonics->str = (char*) text;
		mnemonics->length = length;
	} else {
		if (length > mnemonics->length) {
			mnemonics->str = malloc(length + 1);
			if (mnemonics->str == 0)
				return;
			mnemonics->length = length + 1;
			length = _mnemonics_fix_0(mnemonics, text, text_length, replace,
					&process);
		}
		mnemonics->str[length] = 0;
	}
}
void _mnemonics_free(_mnemonics *mnemonics, const char *text) {
	if (mnemonics->str != gtk_toolkit->mem_tmp && mnemonics->str != text) {
		free(mnemonics->str);
	}
}
int _mnemonics_from_gtk_0(const char *text, size_t text_length, char *out,
		size_t out_length, int *process) {
	int length = 0;
	*process = 0;
	char ch, _t[2], _l;
	for (wuint i = 0; (i < text_length && (ch = text[i]) != 0); i++) {
		_l = 1;
		if (ch == '_') {
			if ((i + 1) < text_length && text[i + 1] == '_') {
				_t[0] = '_';
				i++;
			} else {
				_t[0] = '&';
			}
			*process = 1;
		} else if (ch == '&') {
			_t[0] = '&';
			_t[1] = '&';
			_l = 2;
			*process = 1;
		} else {
			_t[0] = ch;
		}
		if (length < out_length) {
			memcpy(&out[length], _t, _l);
		}
		length += _l;
	}
	return length;
}
char* _gtk_text_fix(const char *text, size_t text_length, size_t *newlength,
		int enc) {
	if ((enc & 0xFF) == W_ENCODING_UNICODE) {
		char *s = 0;
		size_t l = w_utf8_from_utf16((const wchar_t*) text, text_length, 0, 0);
		if ((enc & _GTK_TEXT_FIX_COPY) == 0
				&& l < gtk_toolkit->mem_tmp_total_size) {
			if (gtk_toolkit->mem_tmp_allocated == 0) {
				s = (char*) gtk_toolkit->mem_tmp;
			}
		}
		if (s == 0)
			s = malloc(l + 1);
		if (s == 0)
			return s;
		w_utf8_from_utf16((const wchar_t*) text, text_length, s, l + 1);
		if (newlength != 0) {
			*newlength = l;
		}
		return s;
	} else {
		// W_ENCODING_UTF8
		if ((enc & _GTK_TEXT_FIX_COPY) == 0
				&& (text_length == -1
						|| (enc & _GTK_TEXT_FIX_REQUIRED_NULL) == 0)) {
			if (newlength != 0) {
				if ((enc & _GTK_TEXT_FIX_CALCUL_LENGTH) != 0
						&& text_length == -1) {
					*newlength = strlen(text);
				} else {
					*newlength = text_length;
				}
			}
			return (char*) text;
		} else {
			char *s;
			if ((enc & _GTK_TEXT_FIX_COPY) == 0
					&& text_length < gtk_toolkit->mem_tmp_total_size) {
				if (gtk_toolkit->mem_tmp_allocated == 0) {
					s = (char*) gtk_toolkit->mem_tmp;
				}
			}
			if (s == 0)
				s = malloc(text_length + 1);
			if (s == 0)
				return s;
			if (newlength != 0) {
				*newlength = text_length;
			}
			memcpy(s, text, text_length);
			s[text_length] = 0;
			return s;
		}
	}
}
void _gtk_text_free(const char *text, char *alloc) {
	if (text == alloc)
		return;
	if (text == gtk_toolkit->mem_tmp) {
		gtk_toolkit->mem_tmp_allocated = 0;
		return;
	}
	free(alloc);
}
wresult _gtk_alloc_set_text(w_alloc string, void *user_data, const char *text,
		size_t text_length, int enc) {
	if (text == 0)
		return W_FALSE;
	int process;
	int length = _mnemonics_from_gtk_0(text, text_length, 0, 0, &process);
	char *t = 0;
	int ll = string(user_data, length + 1, (void**) &t);
	if (t != 0 && ll > 0) {
		_mnemonics_from_gtk_0(text, text_length, t, ll, &process);
		t[ll - 1] = 0;
		return W_TRUE;
	}
	return W_FALSE;
}
#endif
