/*
 * label.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_label_create(w_label *label, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(label), toolkit, W_WIDGET(parent), style,
			_W_CLASS_LABEL, post_event);
}
w_label* w_label_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_LABEL(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_LABEL,post_event));
}

wresult w_label_get_image(w_label *label, w_image *image) {
	if (W_WIDGET_CHECK(label)) {
		return W_LABEL_GET_CLASS(label)->get_image(label,image);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_label_get_text(w_label *label,w_alloc alloc,void* user_data) {
	if (W_WIDGET_CHECK(label)) {
		return W_LABEL_GET_CLASS(label)->get_text(label,alloc,user_data);
	} else
		return 0;
}
wresult w_label_set_image(w_label *label, w_image *image) {
	if (W_WIDGET_CHECK(label)) {
		return W_LABEL_GET_CLASS(label)->set_image(label,image);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_label_set_text(w_label *label, const char *string) {
	if (W_WIDGET_CHECK(label)) {
		return W_LABEL_GET_CLASS(label)->set_text(label,string);
	} else
		return W_ERROR_NO_HANDLES;
}
