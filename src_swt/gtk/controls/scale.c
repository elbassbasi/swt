/*
 * scale.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "scale.h"
#include "../widgets/toolkit.h"
int _w_scale_check_style(int style) {
	return _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
}
int _w_scale_get_increment(w_scale *scale) {
}
int _w_scale_get_maximum(w_scale *scale) {
}
int _w_scale_get_minimum(w_scale *scale) {
}
int _w_scale_get_page_increment(w_scale *scale) {
}
int _w_scale_get_selection(w_scale *scale) {
}
wresult _w_scale_set_increment(w_scale *scale, int increment) {
}
wresult _w_scale_set_maximum(w_scale *scale, int value) {
}
wresult _w_scale_set_minimum(w_scale *scale, int value) {
}
wresult _w_scale_set_page_increment(w_scale *scale, int pageIncrement) {
}
wresult _w_scale_set_selection(w_scale *scale, int value) {
}
wresult _w_scale_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle, *handle = 0;
	GtkAdjustment *hAdjustment = 0;
	style = _w_scale_check_style(style);
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	hAdjustment = gtk_adjustment_new(0, 0, 100, 1, 10, 0);
	if (hAdjustment == 0)
		goto _err;
	if ((style & W_HORIZONTAL) != 0) {
		handle = gtk_scale_new(GTK_ORIENTATION_HORIZONTAL, hAdjustment);
	} else {
		handle = gtk_scale_new(GTK_ORIENTATION_VERTICAL, hAdjustment);
	}
	if (handle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	gtk_scale_set_digits(GTK_SCALE(handle), 0);
	gtk_scale_set_draw_value(GTK_SCALE(handle), FALSE);
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;

}
int _w_scale_post_event(w_widget *widget, w_event *e) {
	return _w_widget_post_event(widget, e);

}
void _w_scale_class_init(struct _w_scale_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SCALE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_scale_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_scale);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_scale);
	/*
	 * public function
	 */
	clazz->get_increment=_w_scale_get_increment;
	clazz->get_maximum=_w_scale_get_maximum;
	clazz->get_minimum=_w_scale_get_minimum;
	clazz->get_page_increment=_w_scale_get_page_increment;
	clazz->get_selection=_w_scale_get_selection;
	clazz->set_increment=_w_scale_set_increment;
	clazz->set_maximum=_w_scale_set_maximum;
	clazz->set_minimum=_w_scale_set_minimum;
	clazz->set_page_increment=_w_scale_set_page_increment;
	clazz->set_selection=_w_scale_set_selection;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_scale_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_scale_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	reserved->fixedHandle = _w_widget_hp;
}
#endif
