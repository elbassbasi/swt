/*
 * condition.h
 *
 *  Created on: 21 avr. 2020
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_RUNTIME_CONDITION_H_
#define SWT_RUNTIME_CONDITION_H_
#include "mutex.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_condition {
	union {
		void *handle[14];
	};
} w_condition;

SWT_PUBLIC void w_condition_init(w_condition *condition);
SWT_PUBLIC wresult w_condition_create(w_condition *condition, w_mutex *mutex);
SWT_PUBLIC void w_condition_dispose(w_condition *condition);
SWT_PUBLIC wresult w_condition_wait(w_condition *condition);
SWT_PUBLIC wresult w_condition_wait_timeout(w_condition *condition,
		size_t milliseconds);
SWT_PUBLIC wresult w_condition_signal(w_condition *condition);
SWT_PUBLIC wresult w_condition_broadcast(w_condition *condition);


#ifdef __cplusplus
}
#endif
#endif /* SWT_RUNTIME_CONDITION_H_ */
