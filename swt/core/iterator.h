/*
 * iterator.h
 *
 *  Created on: 24 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CORE_ITERATOR_H_
#define SWT_CORE_ITERATOR_H_
#include "defs.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_basic_iterator {
	void *v_table; //used in c++ as virtual table
	struct _w_iterator_class *clazz;
} w_basic_iterator;
typedef struct w_iterator {
	w_basic_iterator base;
	void *reserved[0x10];
} w_iterator;
typedef struct _w_iterator_class {
	wresult (*close)(w_iterator *it);
	wresult (*next)(w_iterator *it, void *obj);
	wresult (*reset)(w_iterator *it);
	wresult (*remove)(w_iterator *it);
	size_t (*get_count)(w_iterator *it);
} _w_iterator_class;
SWT_PUBLIC extern struct _w_iterator_class _w_iterator_class_empty;
SWT_PUBLIC void w_iterator_init(w_iterator *it);
SWT_PUBLIC wresult w_iterator_next(w_iterator *it, void *obj);
SWT_PUBLIC wresult w_iterator_reset(w_iterator *it);
SWT_PUBLIC wresult w_iterator_close(w_iterator *it);
SWT_PUBLIC wresult w_iterator_remove(w_iterator *it);
SWT_PUBLIC size_t w_iterator_get_count(w_iterator *it);
typedef wresult (*w_iterator_array_filter)(void *userdata, void *obj);
typedef struct w_iterator_array {
	w_basic_iterator base;
	void *array;
	w_iterator_array_filter filter;
	void *user_data;
	wuint i;
	wuint type_size;
	wuint length;
	wuint count;
} w_iterator_array;

SWT_PUBLIC wresult w_iterator_array_create(w_iterator *it, void *array,
		size_t length, size_t type_size, w_iterator_array_filter filter,
		void *user_data);
#ifdef __cplusplus
}
#endif
#endif /* SWT_CORE_ITERATOR_H_ */
