/*
 * slider.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "slider.h"
#include "../widgets/toolkit.h"
int _w_slider_check_style(int style) {
	return _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
}
wresult _w_slider_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget* fixedHandle,*handle=0;
	GtkAdjustment *hAdjustment = 0;
	style = _w_slider_check_style(style);
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window (fixedHandle, TRUE);
	 hAdjustment = gtk_adjustment_new (0, 0, 100, 1, 10, 10);
	if (hAdjustment == 0) goto _err;
	if ((style & W_HORIZONTAL) != 0) {
		handle = gtk_scrollbar_new (GTK_ORIENTATION_HORIZONTAL, hAdjustment);
	} else {
		handle = gtk_scrollbar_new (GTK_ORIENTATION_VERTICAL, hAdjustment);
	}
	if (handle == 0) goto _err;
	/*
	* Bug in GTK. In GTK 2.10, the buttons on either end of
	* a horizontal slider are created taller then the slider bar
	* when the GTK_CAN_FOCUS flag is set. The fix is not to set
	* the flag for horizontal bars in all versions of 2.10. Note
	* that a bug has been logged with GTK about this issue.
	* (http://bugzilla.gnome.org/show_bug.cgi?id=475909)
	*/
	if ((style & W_VERTICAL) != 0) {
		gtk_widget_set_can_focus (handle, TRUE);
	}
	gtk_container_add (GTK_CONTAINER(fixedHandle), handle);
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;

}
int _w_slider_post_event(w_widget *widget, w_event *e) {
	return _w_widget_post_event(widget, e);

}
void _w_slider_class_init (struct _w_slider_class* clazz){
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SLIDER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_slider_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_slider);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_slider);
	/*
	 * public function
	 */
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_slider_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_slider_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	reserved->fixedHandle = _w_widget_hp;
}
#endif
