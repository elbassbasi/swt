/*
 * controlcanvas.c
 *
 *  Created on: 21 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "ccanvas.h"
#include "toolkit.h"
wresult _w_ccanvas_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		return W_ERROR_NO_HANDLES;
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = fixedHandle;
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_w_control_hook_events(widget,
			_W_CONTROL_RESERVED(_w_widget_get_reserved(widget)));
	gtk_widget_show_all(fixedHandle);
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
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_h;
	reserved->fixedHandle = _w_widget_h;
	/*
	 * signals
	 */
}
#endif
