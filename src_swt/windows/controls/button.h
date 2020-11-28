/*
 * button.h
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_BUTTON_H_
#define SRC_WIN32_CONTROLS_BUTTON_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/control.h"
typedef struct _w_button {
	struct _w_control control;
}_w_button;

struct _w_button_reserved {
	struct _w_control_reserved control;

};
#define _W_BUTTON(x) ((_w_button*)x)
#define _W_BUTTON_RESERVED(x) ((struct _w_button_reserved*)x)
void _w_button_class_init (struct _w_button_class* clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_BUTTON_H_ */
