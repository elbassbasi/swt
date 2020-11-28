/*
 * controlcanvas.c
 *
 *  Created on: 21 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "ccanvas.h"
#include "toolkit.h"
wresult _w_ccanvas_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	return W_TRUE;
}
void _w_ccanvas_class_init(struct _w_control_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CCANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_ccanvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_control);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_control);

	/*
	 * function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_ccanvas_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	/*
	 * signals
	 */
}
#endif
