/*
 * progressbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "progressbar.h"
#include "../widgets/toolkit.h"
int _w_progressbar_get_maximum(w_progressbar *progressbar) {
}
int _w_progressbar_get_selection(w_progressbar *progressbar) {
}
int _w_progressbar_get_state(w_progressbar *progressbar) {
	return W_NORMAL;
}
wresult _w_progressbar_set_maximum(w_progressbar *progressbar, int value) {
}
wresult _w_progressbar_set_selection(w_progressbar *progressbar, int value) {
}
wresult _w_progressbar_set_state(w_progressbar *progressbar, int state) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_progressbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
}
int _w_progressbar_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
}
void _w_progressbar_class_init(struct _w_progressbar_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_PROGRESSBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_progressbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_progressbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_progressbar);
	/*
	 * public function
	 */
	clazz->get_maximum = _w_progressbar_get_maximum;
	clazz->get_selection = _w_progressbar_get_selection;
	clazz->get_state = _w_progressbar_get_state;
	clazz->set_maximum = _w_progressbar_set_maximum;
	clazz->set_selection = _w_progressbar_set_selection;
	clazz->set_state = _w_progressbar_set_state;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_progressbar_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
    reserved->widget.compute_size = _w_progressbar_compute_size;
	//signal
}
#endif
