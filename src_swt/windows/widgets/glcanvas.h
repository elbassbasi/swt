/*
 * glcanvas.h
 *
 *  Created on: 1 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_GLCANVAS_H_
#define SRC_WIN32_WIDGETS_GLCANVAS_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "canvas.h"
typedef struct _w_glcanvas {
	struct _w_canvas canvas;
	HGLRC context;
	int pixelFormat;
}_w_glcanvas;

struct _w_glcanvas_reserved {
	struct _w_canvas_reserved canvas;

};
#define _W_GLCANVAS(x) ((_w_glcanvas*)x)
#define _W_GLCANVAS_RESERVED(x) ((struct _w_glcanvas_reserved*)x)

int _w_glcanvas_post_event(w_widget *widget, w_event *e);
void _w_glcanvas_class_init (struct _w_glcanvas_class* clazz);
#endif

#endif /* SRC_WIN32_WIDGETS_GLCANVAS_H_ */
