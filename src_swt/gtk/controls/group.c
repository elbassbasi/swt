/*
 * group.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
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
	GtkWidget *fixedHandle = 0, *handle = 0, *labelHandle = 0, *clientHandle;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);

	handle = gtk_frame_new(NULL);
	if (handle == 0)
		goto _err;

	labelHandle = gtk_label_new(NULL);
	if (labelHandle == 0)
		goto _err;
	g_object_ref(labelHandle);
	g_object_ref_sink(labelHandle);

	clientHandle = _w_fixed_new();
	if (clientHandle == 0)
		goto _err;
	/*
	 * Bug 453827 - clientHandle now has it's own window so that
	 * it can listen to events (clicking/tooltip etc.) and so that
	 * background can be drawn on it.
	 */
	gtk_widget_set_has_window(clientHandle, TRUE);

	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	gtk_container_add(GTK_CONTAINER(handle), clientHandle);
	gtk_frame_set_label_widget(GTK_FRAME(handle), labelHandle);
	if ((style & W_SHADOW_IN) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_IN);
	}
	if ((style & W_SHADOW_OUT) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_OUT);
	}
	if ((style & W_SHADOW_ETCHED_IN) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_ETCHED_IN);
	}
	if ((style & W_SHADOW_ETCHED_OUT) != 0) {
		gtk_frame_set_shadow_type(GTK_FRAME(handle), GTK_SHADOW_ETCHED_OUT);
	}
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription (defaultFont ().handle);
#endif
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(clientHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(labelHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = clientHandle;
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);

	_W_WIDGET(widget)->style = _w_group_check_style(style);
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	gtk_widget_show_all(fixedHandle);
	gtk_widget_hide(labelHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_group_get_text(w_group *group, w_alloc *text) {

}
wresult _w_group_set_text(w_group *group, const char *string) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	struct _mnemonics mn;
	_mnemonics_fix(&mn, string, -1, W_TRUE);
	GtkWidget *handle = gtk_widget_get_parent(_W_WIDGET(group)->handle);
	GtkWidget *label = gtk_frame_get_label_widget(GTK_FRAME(handle));
	if (mn.length != 0) {
		gtk_label_set_text_with_mnemonic(GTK_LABEL(label), mn.str);
		gtk_widget_show(label);
	} else {
		gtk_widget_hide(label);
	}
	// Set the foreground now that the text has been set
	/*if (GTK_VERSION >= VERSION (3, 16, 0) && foreground != null) {
	 setForegroundGdkRGBA (labelHandle, foreground);
	 }*/
	_mnemonics_free(&mn, string);
	return W_TRUE;
}
int _w_group_get_client_area(w_widget *widget, struct w_event_client_area *e,
		struct _w_widget_reserved *reserved) {
	_w_composite_get_client_area(widget, e, reserved);
	/*
	 * Bug 453827 Child position fix.
	 * SWT's calls to gtk_widget_size_allocate and gtk_widget_set_allocation
	 * causes GTK+ to move the clientHandle's SwtFixed down by the size of the label.
	 * These calls can come up from 'shell' and group has no control over these calls.
	 *
	 * This is an undesired side-effect. Client handle's x & y positions should never
	 * be incremented as this is an internal sub-container.
	 *
	 * Note: 0 by 0 was chosen as 1 by 1 shifts controls beyond their original pos.
	 * The long term fix would be to not use widget_*_allocation from higher containers
	 * like shell and to not use	gtkframe in non-group widgets (e.g used in label atm).
	 */
	e->rect->x = 0;
	e->rect->y = 0;
	return W_TRUE;
}
int _w_group_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	w_size sz;
	_w_composite_compute_size(widget, e, reserved);
	e->wHint = W_DEFAULT;
	e->hHint = W_DEFAULT;
	sz.width = e->size->width;
	sz.height = e->size->height;
	GtkWidget *handle = gtk_widget_get_parent(_W_WIDGET(widget)->handle);
	_w_control_compute_native_size(widget,handle, e,
			reserved);
	e->wHint = wHint;
	e->hHint = hHint;
	e->size->width = w_max(e->size->width, sz.width);
	e->size->height = sz.height;
	return TRUE;
}
int _w_group_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved) {
	_W_CONTROL_RESERVED(reserved)->force_resize(W_CONTROL(widget), _W_CONTROL_RESERVED(reserved));
	GtkAllocation allocation;
	GtkWidget *clientHandle = _W_CONTROL_RESERVED(reserved)->clientHandle(
			widget, reserved);
	gtk_widget_get_allocation(clientHandle, &allocation);
	int clientX = allocation.x;
	int clientY = allocation.y;
	e->result->x = e->rect->x - clientX;
	e->result->y = e->rect->y - clientX;
	e->result->width = e->rect->width + clientX + clientX;
	e->result->height = e->rect->height + clientX + clientY;
	return TRUE;
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
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hpp;
	reserved->fixedHandle = _w_widget_hpp;
	reserved->clientHandle = _w_widget_h;
	_W_COMPOSITE_RESERVED(reserved)->parenting_handle = _w_widget_h;
	_W_WIDGET_RESERVED(reserved)->get_client_area = _w_group_get_client_area;
	_W_WIDGET_RESERVED(reserved)->compute_trim = _w_group_compute_trim;
	_W_WIDGET_RESERVED(reserved)->compute_size = _w_group_compute_size;
}
#endif
