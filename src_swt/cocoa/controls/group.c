/*
 * group.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "group.h"
#include "../widgets/toolkit.h"
int _w_group_check_style(int style) {
	style |= W_NO_FOCUS;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_group_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
}
wresult _w_group_get_text(w_group *group, w_alloc alloc,void* user_data) {

}
wresult _w_group_set_text(w_group *group, const char *string) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
}
int _w_group_get_client_area(w_widget *widget, struct w_event_client_area *e,
		struct _w_widget_reserved *reserved) {
	return W_TRUE;
}
int _w_group_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	w_size sz;
}
int _w_group_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved) {
}
void _w_group_class_init(struct _w_group_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GROUP;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_group_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_group);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_group);
	/*
	 * public function
	 */
	clazz->get_text = _w_group_get_text;
	clazz->set_text = _w_group_set_text;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_group_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);	_W_WIDGET_RESERVED(reserved)->get_client_area = _w_group_get_client_area;
	_W_WIDGET_RESERVED(reserved)->compute_trim = _w_group_compute_trim;
	_W_WIDGET_RESERVED(reserved)->compute_size = _w_group_compute_size;
}
#endif
