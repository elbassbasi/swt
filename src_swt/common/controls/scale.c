/*
 * scale.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_scale_create(w_scale *scale, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(scale), toolkit, W_WIDGET(parent), style,
			_W_CLASS_SCALE, post_event);
}
w_scale* w_scale_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_SCALE(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_SCALE,post_event));
}
int w_scale_get_increment(w_scale *scale) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->get_increment(scale);
	} else
		return 0;
}
int w_scale_get_maximum(w_scale *scale) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->get_maximum(scale);
	} else
		return 0;
}
int w_scale_get_minimum(w_scale *scale) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->get_minimum(scale);
	} else
		return 0;
}
int w_scale_get_page_increment(w_scale *scale) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->get_page_increment(scale);
	} else
		return 0;
}
int w_scale_get_selection(w_scale *scale) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->get_selection(scale);
	} else
		return 0;
}
wresult w_scale_set_increment(w_scale *scale, int increment) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->set_increment(scale,increment);
	} else
		return res;
}
wresult w_scale_set_maximum(w_scale *scale, int value) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->set_maximum(scale,value);
	} else
		return res;
}
wresult w_scale_set_minimum(w_scale *scale, int value) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->set_minimum(scale,value);
	} else
		return res;
}
wresult w_scale_set_page_increment(w_scale *scale, int pageIncrement) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->set_page_increment(scale,pageIncrement);
	} else
		return res;
}
wresult w_scale_set_selection(w_scale *scale, int value) {
	wresult res = W_WIDGET_CHECK0(scale);
	if (res > 0) {
		return W_SCALE_GET_CLASS(scale)->set_selection(scale,value);
	} else
		return res;
}
