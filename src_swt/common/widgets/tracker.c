/*
 * tracker.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_tracker_create(w_tracker *tracker, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tracker), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TRACKER, post_event);
}
w_tracker* w_tracker_new(struct w_toolkit *toolkit, w_composite *parent,
                         wuint64 style, w_widget_post_event_proc post_event) {
	return W_TRACKER(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TRACKER,post_event));
}
