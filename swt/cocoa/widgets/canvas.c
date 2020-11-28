/*
 * canvas.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "canvas.h"
/*
 * function
 */
w_caret* _w_canvas_get_caret(w_canvas *canvas) {

}
wresult _w_canvas_set_caret(w_canvas *canvas, w_caret *caret) {

}
/*
 * signals
 */
void _w_canvas_class_init(struct _w_canvas_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_canvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_canvas);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_canvas);
	/**
	 *
	 */
	clazz->get_caret = _w_canvas_get_caret;
	clazz->set_caret = _w_canvas_set_caret;
	/*
	 * reserved
	 */
	struct _w_canvas_reserved *reserved = _W_CANVAS_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	/*
	 * signals
	 */
}
#endif

