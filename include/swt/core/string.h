/*
 * string.h
 *
 *  Created on: 7 mars 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CORE_STRING_H_
#define SWT_CORE_STRING_H_
#include "utf8.h"
#ifdef __cplusplus
extern "C" {
#endif


typedef struct w_string_ref {
	unsigned sign :8; //must be 0
	unsigned length :24;
	unsigned int ref;
	char data[0];
} w_string_ref;

SWT_PUBLIC extern struct w_string_ref *empty_string;
SWT_PUBLIC const char* w_string_get_chars(w_string_ref *ref);
SWT_PUBLIC const char* w_string_get_chars_not_null(struct w_string_ref *ref);
SWT_PUBLIC size_t w_string_get_length(w_string_ref *ref);
SWT_PUBLIC w_string_ref* w_string_new(size_t size);
SWT_PUBLIC void w_string_init_from_chars(struct w_string_ref **ref,
		const char *chars, wbool copy);
SWT_PUBLIC void w_string_set_chars(w_string_ref **ref, const char *chars,
		wbool copy);
SWT_PUBLIC void w_string_init_from_wchars(struct w_string_ref **ref,
		const wchar_t *wchars);
SWT_PUBLIC void w_string_set_wchars(w_string_ref **ref, const wchar_t *chars);
SWT_PUBLIC void w_string_init_from_ref(w_string_ref **ref, w_string_ref *s,
		wbool clone);
SWT_PUBLIC void w_string_move(w_string_ref **ref, w_string_ref *s,
		wbool clone);
SWT_PUBLIC void w_string_copy(w_string_ref **ref, w_string_ref *s);
SWT_PUBLIC void w_string_copy_chars(w_string_ref **ref, const char *str,
		size_t size);
SWT_PUBLIC void w_string_ref_inc(w_string_ref *ref);
SWT_PUBLIC w_string_ref* w_string_ref_dec(w_string_ref *ref);
SWT_PUBLIC void w_string_sprintf_0(w_string_ref **ref, const char *format,
		va_list args);
SWT_PUBLIC void w_string_sprintf(w_string_ref **ref, const char *format, ...);
SWT_PUBLIC void w_string_concat(w_string_ref **ref, w_string_ref *s1,
		w_string_ref *s2);
/*
 *	w_alloc_string_ref
 */
SWT_PUBLIC size_t w_alloc_string_ref(void* user_data,
		size_t size, void **string);


#ifdef __cplusplus
}
#endif
#endif /* SWT_CORE_STRING_H_ */
