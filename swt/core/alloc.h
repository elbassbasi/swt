/*
 * alloc.h
 *
 *  Created on: 2 oct. 2020
 *      Author: azeddine
 */

#ifndef SWT_CORE_ALLOC_H_
#define SWT_CORE_ALLOC_H_
#include "defs.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef size_t (*w_alloc)(void *user_data, size_t size, void **buf);
SWT_PUBLIC wresult w_alloc_set_text(w_alloc alloc, void *user_data,
		const char *text, size_t length);

typedef struct w_alloc_buffer {
	void *buffer;
	size_t size;
	size_t total_size;
} w_alloc_buffer;
size_t w_alloc_buffer_new(void *user_data, size_t size, void **buffer);
size_t w_alloc_buffer_copy(void *user_data, size_t size, void **buffer);

#ifdef __cplusplus
}
#endif
#endif /* SWT_CORE_ALLOC_H_ */
