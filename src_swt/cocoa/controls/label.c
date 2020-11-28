/*
 * label.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "label.h"
#include "../widgets/toolkit.h"
w_image* _w_label_get_image(w_label *label) {
}
w_string_ref* _w_label_get_text(w_label *label) {
}
wresult _w_label_set_image(w_label *label, w_image *image) {
}
wresult _w_label_set_text(w_label *label, const char *text) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	return W_TRUE;
}
int _w_label_check_style(int style) {
	style |= W_NO_FOCUS;
	if ((style & W_SEPARATOR) != 0) {
		style = _w_widget_check_bits(style, W_VERTICAL, W_HORIZONTAL, 0, 0, 0,
				0);
		return _w_widget_check_bits(style, W_SHADOW_OUT, W_SHADOW_IN,
				W_SHADOW_NONE, 0, 0, 0);
	}
	return _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
}
void _w_label_set_alignment(w_label *label, int alignment) {
}

wresult _w_label_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
}
int _w_label_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint, hHint = e->hHint;
	int tmp[2];
	return W_TRUE;
}
void _w_label_class_init(struct _w_label_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LABEL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_label_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_label);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_label);
	/*
	 * public function
	 */
	clazz->get_image = _w_label_get_image;
	clazz->get_text = _w_label_get_text;
	clazz->set_image = _w_label_set_image;
	clazz->set_text = _w_label_set_text;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_label_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->widget.compute_size = _w_label_compute_size;
}
#endif
