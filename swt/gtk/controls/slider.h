/*
 * slider.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_CONTROLS_SLIDER_H_
#define SRC_GTK_CONTROLS_SLIDER_H_
#ifdef __linux
#include "../widgets/control.h"
typedef struct _w_slider {
	struct _w_control control;
}_w_slider;

struct _w_slider_reserved {
	struct _w_control_reserved control;

};
#define _W_SLIDER(x) ((_w_slider*)x)
#define _W_SLIDER_RESERVED(x) ((struct _w_slider_reserved*)x)
void _w_slider_class_init (struct _w_slider_class* clazz);

#endif
#endif /* SRC_GTK_CONTROLS_SLIDER_H_ */
