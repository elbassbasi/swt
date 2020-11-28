/*
 * slider.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_slider_create(w_slider *slider, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(slider), toolkit, W_WIDGET(parent), style,
			_W_CLASS_SLIDER, post_event);
}
w_slider* w_slider_new(struct w_toolkit *toolkit, w_composite *parent,
                       wuint64 style, w_widget_post_event_proc post_event) {
	return W_SLIDER(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_SLIDER,post_event));
}
