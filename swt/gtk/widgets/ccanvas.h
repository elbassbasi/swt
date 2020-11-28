/*
 * controlcanvas.h
 *
 *  Created on: 21 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_CCANVAS_H_
#define SRC_GTK_WIDGETS_CCANVAS_H_
#ifdef __linux
#include "control.h"
typedef struct _w_ccanvas {
	struct _w_control control;
} _w_ccanvas;

struct _w_ccanvas_reserved {
	struct _w_control_reserved control;
};
#define _W_CCANVAS(x) ((_w_ccanvas*)x)
#define _W_CCANVAS_RESERVED(x) ((struct _w_ccanvas_reserved*)x)
#define _W_CCANVAS_GET_RESERVED(x) ((struct _w_ccanvas_reserved*)_w_widget_reserved(W_WIDGET(x)))
void _w_ccanvas_class_init(struct _w_control_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_CCANVAS_H_ */
