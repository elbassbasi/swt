/*
 * link.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "link.h"
#include "../widgets/toolkit.h"
wresult _w_link_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	return W_TRUE;
}
wresult _w_link_set_text(w_link *link, const char *string) {
	return W_TRUE;
}
int _w_link_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);

}
void _w_link_class_init(struct _w_link_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LINK;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_link_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_link);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_link);
	/*
	 * public function
	 */
	clazz->set_text = _w_link_set_text;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_link_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_link_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
#endif
