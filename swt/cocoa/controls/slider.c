/*
 * slider.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "slider.h"
#include "../widgets/toolkit.h"
int _w_slider_check_style(int style) {
	return _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
}
wresult _w_slider_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;

}
int _w_slider_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);

}
void _w_slider_class_init (struct _w_slider_class* clazz){
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SLIDER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_slider_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_slider);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_slider);
	/*
	 * public function
	 */
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_slider_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_slider_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
#endif
