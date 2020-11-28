/*
 * link.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_link_create(w_link *link, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(link), toolkit, W_WIDGET(parent), style,
			_W_CLASS_LINK, post_event);
}
w_link* w_link_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_LINK(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_LINK,post_event));
}
wresult w_link_get_text(w_link *link,w_alloc alloc,void* user_data){
	wresult res = W_WIDGET_CHECK0(link);
	if (res > 0) {
		return W_LINK_GET_CLASS(link)->get_text(link, alloc,user_data);
	} else
		return res;
}
wresult w_link_set_text(w_link *link, const char *string) {
	wresult res = W_WIDGET_CHECK0(link);
	if (res > 0) {
		return W_LINK_GET_CLASS(link)->set_text(link, string);
	} else
		return res;
}
