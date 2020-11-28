/*
 * canvas.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_WIDGETS_CANVAS_H_
#define SRC_COCOA_WIDGETS_CANVAS_H_

#ifdef __APPLE__
#include "composite.h"
#define STATE_CANVAS_END (STATE_COMPOSITE_END + 0)
typedef struct _w_canvas {
	struct _w_composite composite;
}_w_canvas;

struct _w_canvas_reserved {
	struct _w_composite_reserved composite;

};
#define _W_CANVAS(x) ((_w_canvas*)x)
#define _W_CANVAS_RESERVED(x) ((struct _w_canvas_reserved*)x)
/*
 * function
 */
w_caret* _w_canvas_get_caret(w_canvas *canvas);
wresult _w_canvas_set_caret(w_canvas *canvas, w_caret *caret);
/*
 * signals
 */
void _w_canvas_class_init (struct _w_canvas_class* clazz);
#endif
#endif /* SRC_GTK_WIDGETS_CANVAS_H_ */
