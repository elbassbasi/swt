/*
 * tooltip.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_tooltip_create(w_tooltip *tooltip, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tooltip), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TOOLTIP, post_event);
}
w_tooltip* w_tooltip_new(struct w_toolkit *toolkit, w_composite *parent,
                         wuint64 style, w_widget_post_event_proc post_event) {
	return W_TOOLTIP(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TOOLTIP,post_event));
}
