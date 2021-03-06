/*
 * label.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_LABEL_H_
#define SRC_WIN32_CONTROLS_LABEL_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/control.h"
typedef struct _w_label {
	struct _w_control control;
}_w_label;

struct _w_label_reserved {
	struct _w_control_reserved control;

};
#define _W_LABEL(x) ((_w_label*)x)
#define _W_LABEL_RESERVED(x) ((struct _w_label_reserved*)x)
void _w_label_class_init (struct _w_label_class* clazz);


#endif
#endif /* SRC_WIN32_CONTROLS_LABEL_H_ */
