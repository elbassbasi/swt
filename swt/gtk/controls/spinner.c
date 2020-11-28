/*
 * spinner.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
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
	GtkWidget *fixedHandle, *handle = 0;
	GtkAdjustment *adjustment;
	style = _w_spinner_check_style(style);
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_MENU;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	adjustment = gtk_adjustment_new(0, 0, 100, 1, 10, 0);
	if (adjustment == 0)
		goto _err;
	_W_SPINNER(widget)->climbRate = 1;
	handle = gtk_spin_button_new(adjustment, _W_SPINNER(widget)->climbRate, 0);
	if (handle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	gtk_editable_set_editable(GTK_EDITABLE(handle), (style & W_READ_ONLY) == 0);
	if (GTK_VERSION <= VERSION(3, 20, 0)) {
		gtk_entry_set_has_frame(GTK_ENTRY(handle), (style & W_BORDER) != 0);
	}
	gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(handle), (style & W_WRAP) != 0);
#if GTK3
	//imContext = OS.imContextLast();
#endif
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
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
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	reserved->fixedHandle = _w_widget_hp;
}
#endif
