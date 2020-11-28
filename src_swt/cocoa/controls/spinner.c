/*
 * spinner.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "spinner.h"
#include "../widgets/toolkit.h"
int _w_spinner_check_style(int style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
wresult _w_spinner_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
}
void _w_spinner_class_init(struct _w_spinner_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SPINNER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_spinner_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_spinner);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_spinner);
	/*
	 * public function
	 */
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_spinner_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
#endif
