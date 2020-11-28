/*
 * tooltip.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#if defined( __WIN32__) || defined(__WIN64__)
#include "tooltip.h"
wresult _w_tooltip_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {

}
int _w_tooltip_post_event(w_widget *widget, w_event *e) {
	return _w_widget_post_event(widget, e);

}
void _w_tooltip_class_init (struct _w_tooltip_class* clazz){
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TOOLTIP;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tooltip_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tooltip);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tooltip);
	/*
	 * public function
	 */
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_tooltip_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_tooltip_create;
}
#endif
