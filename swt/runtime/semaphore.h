/*
 * semaphore.h
 *
 *  Created on: 21 avr. 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_RUNTIME_SEMAPHORE_H_
#define SWT_RUNTIME_SEMAPHORE_H_
#include "mutex.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct w_semaphore {
	union {
		void *handle[14];
	};
} w_semaphore;
SWT_PUBLIC wresult w_semaphore_init(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_create(w_semaphore *semaphore, int initialcount,
		int maxcount);
SWT_PUBLIC void w_semaphore_dispose(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_wait(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_try_wait(w_semaphore *semaphore);
SWT_PUBLIC wresult w_semaphore_wait_timeout(w_semaphore *semaphore,
		size_t milliseconds);
SWT_PUBLIC wresult w_semaphore_post(w_semaphore *semaphore);

#ifdef __cplusplus
}
#endif
#endif /* SWT_RUNTIME_SEMAPHORE_H_ */
