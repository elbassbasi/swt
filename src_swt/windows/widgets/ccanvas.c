/*
 * controlcanvas.c
 *
 *  Created on: 21 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "ccanvas.h"
#include "toolkit.h"
wresult _w_ccanvas_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_composite_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), (w_composite*) parent,
			WindowName, dwExStyle, dwStyle);
	if (result > 0) {
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
		_W_WIDGET(widget)->state |= STATE_CANVAS;
		ShowWindow(_W_WIDGET(widget)->handle, SW_NORMAL);
	}
	return result;
}
void _w_ccanvas_class_init(struct _w_control_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CCANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_ccanvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_control);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_control);

	/*
	 * overriding function
	 */
	//W_WIDGET_CLASS(clazz)->post_event = _w_ccanvas_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_ccanvas_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
#endif
