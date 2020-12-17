/*
 * controlcanvas.c
 *
 *  Created on: 21 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "ccanvas.h"
#include "toolkit.h"
wresult _w_ccanvas_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved){
	GtkWidget *fixedHandle;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		return W_ERROR_NO_HANDLES;
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = fixedHandle;
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
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_h;
	_W_WIDGET_RESERVED(reserved)->create_handle = _w_ccanvas_create_handle;
	reserved->fixedHandle = _w_widget_h;

	/*
	 * signals
	 */
}
#endif
