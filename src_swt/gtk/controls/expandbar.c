/*
 * expandbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "expandbar.h"
#include "../widgets/toolkit.h"
void GtkCallback_clientHandle(GtkWidget *widget, gpointer data) {
	*((GtkWidget**) data) = widget;
}
GtkWidget* _w_expanditem_clientHandle(w_expanditem *expanditem) {
	GtkWidget *clientHandle = 0;
	gtk_container_forall(GTK_CONTAINER(_W_EXPANDITEM(expanditem)->handle),
			GtkCallback_clientHandle, &clientHandle);
	return clientHandle;
}
w_control* _w_expanditem_get_control(w_expanditem *expanditem) {

}
wresult _w_expanditem_get_expanded(w_expanditem *expanditem) {
}
int _w_expanditem_get_header_height(w_expanditem *expanditem) {
}
int _w_expanditem_get_height(w_expanditem *expanditem) {
}

wresult _w_expanditem_set_control(w_expanditem *expanditem,
		w_control *control) {

}
wresult _w_expanditem_set_expanded(w_expanditem *expanditem, int expanded) {
}
wresult _w_expanditem_set_height(w_expanditem *expanditem, int height) {
	if (height < 0)
		return FALSE;
	GtkWidget *clientHandle = _w_expanditem_clientHandle(expanditem);
	gtk_widget_set_size_request(clientHandle, -1, height);
	//parent.layoutItems (0, false);
	return TRUE;
}
wresult _w_expanditem_set_image(w_expanditem *expanditem, int image) {
}
/*
 *
 */
wresult _w_expandbar_get_item(w_expandbar *expandbar, int index,
		w_expanditem *item) {
}
int _w_expandbar_get_item_count(w_expandbar *expandbar) {
}
void _w_expandbar_get_items(w_expandbar *expandbar, w_iterator *items) {
}
int _w_expandbar_get_spacing(w_expandbar *expandbar) {
}

wresult _w_expandbar_insert_item(w_expandbar *expandbar, w_expanditem *item,
		const char *text, int index) {
	GtkWidget *handle = 0, *clientHandle = 0, *boxHandle = 0, *labelHandle = 0,
			*imageHandle = 0;
	handle = gtk_expander_new(text);
	if (handle == 0)
		goto _err;
	clientHandle = _w_fixed_new();
	if (clientHandle == 0)
		goto _err;
#if GTK3
	boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
	if (boxHandle == 0)
		goto _err;
	gtk_box_set_homogeneous(GTK_BOX(boxHandle), FALSE);
#else
	vboxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE, 4);
	if (vboxHandle == 0)
		goto _err;
#endif
	if (boxHandle == 0)
		goto _err;
	labelHandle = gtk_label_new(NULL);
	if (labelHandle == 0)
		goto _err;
	imageHandle = gtk_image_new();
	if (imageHandle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(handle), clientHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_expander_set_label_widget(GTK_EXPANDER(handle), boxHandle);
	gtk_widget_set_can_focus(handle, TRUE);
	gtk_widget_show_all(handle);
	gtk_container_add(GTK_CONTAINER(_W_WIDGET(expandbar)->handle), handle);
	if (item != 0) {
		_W_EXPANDITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_EXPANDBAR_GET_CLASS(expandbar)->class_expandbaritem);
		_W_EXPANDITEM(item)->parent = expandbar;
		_W_EXPANDITEM(item)->handle = handle;
	}
	return TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
wresult _w_expandbar_set_spacing(w_expandbar *expandbar, int spacing) {
}

wresult _w_expandbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle, *handle = 0, *scrolledHandle, *viewport;
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
#if GTK3
	handle = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	if (handle == 0)
		goto _err;
	gtk_box_set_homogeneous(GTK_BOX(handle), FALSE);
#else
	handle = gtk_box_new(GTK_ORIENTATION_VERTICAL, FALSE, 0);
	if (handle == 0)
		goto _err;
#endif
	if ((style & W_VSCROLL) != 0) {
		scrolledHandle = gtk_scrolled_window_new(0, 0);
		if (scrolledHandle == 0)
			goto _err;
		g_object_set_qdata(G_OBJECT(scrolledHandle), gtk_toolkit->quark[0],
				widget);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle),
				GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		gtk_container_add(GTK_CONTAINER(fixedHandle), scrolledHandle);
		if (GTK_VERSION < VERSION(3, 8, 0)) {
			gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
		} else {
			gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
		}
		viewport = gtk_bin_get_child(GTK_BIN(scrolledHandle));
		gtk_viewport_set_shadow_type(GTK_VIEWPORT(viewport), GTK_SHADOW_NONE);
	} else {
		gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	}
	gtk_container_set_border_width(GTK_CONTAINER(handle), 0);
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
	_W_WIDGET(widget)->handle = handle;
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	g_object_set_qdata(G_OBJECT(_W_WIDGET(widget)->handle),
			gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
GtkWidget* _w_expandbar_scrolledHandle(w_widget *control,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(control)->style & W_VSCROLL) != 0) {
		return gtk_widget_get_parent(_W_WIDGET(control)->handle);
	} else
		return 0;
}
GtkWidget* _w_expandbar_fixedHandle(w_widget *control,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(control)->style & W_VSCROLL) != 0) {
		return _w_widget_hpp(control, reserved);
	} else
		return _w_widget_hp(control, reserved);
}
void _w_expandbar_class_init(struct _w_expandbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_EXPANDBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_expandbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_expandbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_expandbar);
	/*
	 * public function
	 */
	clazz->get_item = _w_expandbar_get_item;
	clazz->get_item_count = _w_expandbar_get_item_count;
	clazz->get_items = _w_expandbar_get_items;
	clazz->get_spacing = _w_expandbar_get_spacing;
	clazz->insert_item = _w_expandbar_insert_item;
	clazz->set_spacing = _w_expandbar_set_spacing;
	/*
	 * expanditem
	 */
	struct _w_expanditem_class *itemclazz = clazz->class_expandbaritem;
	itemclazz->item.widgetdata.toolkit = W_TOOLKIT(gtk_toolkit);
	itemclazz->item.widgetdata.close = _w_widgetdata_close;
	itemclazz->get_control = _w_expanditem_get_control;
	itemclazz->get_expanded = _w_expanditem_get_expanded;
	itemclazz->get_header_height = _w_expanditem_get_header_height;
	itemclazz->get_height = _w_expanditem_get_height;
	itemclazz->set_control = _w_expanditem_set_control;
	itemclazz->set_expanded = _w_expanditem_set_expanded;
	itemclazz->set_height = _w_expanditem_set_height;
	itemclazz->set_image = _w_expanditem_set_image;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_expandbar_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_expandbar_fixedHandle;
	reserved->fixedHandle = _w_expandbar_fixedHandle;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle =
			_w_expandbar_scrolledHandle;
	_W_COMPOSITE_RESERVED(reserved)->parenting_handle =
			_w_expandbar_fixedHandle;
}
#endif
