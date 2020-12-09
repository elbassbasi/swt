/*
 * glcanvas.c
 *
 *  Created on: 1 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "glcanvas.h"
#include "toolkit.h"

#define USE_TEXTURE 0

wresult _w_glcanvas_init_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	if (data == 0)
		return W_ERROR_NULL_ARGUMENT;

	return W_TRUE;
}
wresult _w_glcanvas_get_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	return W_TRUE;
}
wresult _w_glcanvas_is_current(w_glcanvas *glcanvas) {
}
wresult _w_glcanvas_set_current(w_glcanvas *glcanvas) {
	return W_TRUE;
}

wresult _w_glcanvas_swap_buffers(w_glcanvas *glcanvas) {
	return W_TRUE;
}
void _w_glcanvas_resize_handle(w_control *control, w_size *size,
		struct _w_control_reserved *reserved) {
}
/*
 * signals
 */
void _w_glcanvas_class_init(struct _w_glcanvas_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GLCANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_glcanvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_glcanvas);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_glcanvas);
	/*
	 *
	 */
	clazz->init_gldata = _w_glcanvas_init_gldata;
	clazz->get_gldata = _w_glcanvas_get_gldata;
	clazz->is_current = _w_glcanvas_is_current;
	clazz->set_current = _w_glcanvas_set_current;
	clazz->swap_buffers = _w_glcanvas_swap_buffers;
	/*
	 *
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
#endif
