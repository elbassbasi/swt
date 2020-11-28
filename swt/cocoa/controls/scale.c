/*
 * scale.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
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

}
int _w_scale_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);

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
}
#endif
