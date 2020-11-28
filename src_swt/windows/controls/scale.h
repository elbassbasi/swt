/*
 * scale.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_SCALE_H_
#define SRC_WIN32_CONTROLS_SCALE_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/control.h"
typedef struct _w_scale {
	struct _w_control control;
}_w_scale;

struct _w_scale_reserved {
	struct _w_control_reserved control;

};
#define _W_SCALE(x) ((_w_scale*)x)
#define _W_SCALE_RESERVED(x) ((struct _w_scale_reserved*)x)
void _w_scale_class_init (struct _w_scale_class* clazz);




#endif
#endif /* SRC_WIN32_CONTROLS_SCALE_H_ */
