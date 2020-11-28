/*
 * link.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "link.h"
#include "../widgets/toolkit.h"
wresult _w_link_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *handle;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	handle = _w_fixed_new();
	if (handle == 0)
		return W_ERROR_NO_HANDLES;
	gtk_widget_set_has_window(handle, TRUE);
	gtk_widget_set_can_focus(handle, TRUE);
	/*_W_LINK(widget)->layout = pango_layout_new(pango_context_new());
	 disabledColor = new Color (display, LINK_DISABLED_FOREGROUND);
	 offsets = new Point [0];
	 ids = new String [0];
	 mnemonics = new int [0];
	 selection = new Point (-1, -1);
	 focusIndex = -1;*/
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	_w_composite_add_child(W_COMPOSITE(parent), handle);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	gtk_widget_show_all(handle);
	return W_TRUE;
}
wresult _w_link_set_text(w_link *link, const char *string) {
	if (_W_LINK(link)->text != 0) {
		free(_W_LINK(link)->text);
	}
	if (string == 0) {
		_W_LINK(link)->text = 0;
		return W_TRUE;
	}
	_W_LINK(link)->text = strdup(string);
	if (_W_LINK(link)->text == 0)
		return W_FALSE;
	return W_TRUE;
}
int _w_link_post_event(w_widget *widget, w_event *e) {
	return _w_widget_post_event(widget, e);

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
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_h;
	reserved->fixedHandle = _w_widget_h;
}
#endif
