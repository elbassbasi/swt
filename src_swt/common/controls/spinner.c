/*
 * spinner.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_spinner_create(w_spinner *spinner, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(spinner), toolkit, W_WIDGET(parent), style,
			_W_CLASS_SPINNER, post_event);
}
w_spinner* w_spinner_new(struct w_toolkit *toolkit, w_composite *parent,
                         wuint64 style, w_widget_post_event_proc post_event) {
	return W_SPINNER(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_SPINNER,post_event));
}
