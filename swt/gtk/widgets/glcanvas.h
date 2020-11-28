/*
 * glcanvas.h
 *
 *  Created on: 1 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_GLCANVAS_H_
#define SRC_GTK_WIDGETS_GLCANVAS_H_
#ifdef __linux
#include "canvas.h"
typedef struct _w_glcanvas {
	struct _w_canvas canvas;
	GdkGLContext *context;
	GdkWindow *event_window;
	guint frame_buffer;
	union {
		guint texture;
		guint render_buffer;
	};
} _w_glcanvas;

struct _w_glcanvas_reserved {
	struct _w_canvas_reserved canvas;

};
#define _W_GLCANVAS(x) ((_w_glcanvas*)x)
#define _W_GLCANVAS_RESERVED(x) ((struct _w_glcanvas_reserved*)x)

void _w_glcanvas_class_init(struct _w_glcanvas_class *clazz);
#endif

#endif /* SRC_GTK_WIDGETS_GLCANVAS_H_ */
