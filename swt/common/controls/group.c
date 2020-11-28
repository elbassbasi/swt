/*
 * group.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_group_create(w_group *group, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(group), toolkit, W_WIDGET(parent), style,
			_W_CLASS_GROUP, post_event);
}
w_group* w_group_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_GROUP(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_GROUP,post_event));
}
wresult w_group_get_text(w_group *group,w_alloc alloc,void* user_data) {
	if (W_WIDGET_CHECK(group)) {
		return W_GROUP_GET_CLASS(group)->get_text(group,alloc,user_data);
	} else
		return 0;
}
wresult w_group_set_text(w_group *group, const char *string) {
	if (W_WIDGET_CHECK(group)) {
		return W_GROUP_GET_CLASS(group)->set_text(group, string);
	} else
		return W_ERROR_NO_HANDLES;
}
