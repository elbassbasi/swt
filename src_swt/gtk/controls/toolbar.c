/*
 * toolbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "toolbar.h"
#include "../widgets/toolkit.h"
/**
 * private
 */
w_imagelist* _w_toolbar_get_image_list(w_widget *widget) {
	return _W_TOOLBAR(widget)->imagelist;
}
void _w_toolbar_relayout(w_toolbar *toolbar) {
	int length = gtk_toolbar_get_n_items(
			GTK_TOOLBAR(_W_WIDGET(toolbar)->handle));
	gboolean hasText = FALSE, hasImage = FALSE;
	for (int i = 0; i < length; i++) {
		GtkToolItem *item = gtk_toolbar_get_nth_item(
				GTK_TOOLBAR(_W_WIDGET(toolbar)->handle), i);
		if (item != 0) {
			//item.resizeControl ();
			if (GTK_IS_TOOL_BUTTON(item)) {
				GtkWidget *label = gtk_tool_button_get_label_widget(
						GTK_TOOL_BUTTON(item));
				const char *text = gtk_label_get_text(GTK_LABEL(label));
				if (text != 0 && text[0] != 0) {
					hasText = TRUE;
				}
				_w_image_widget *imageHandle =
						(_w_image_widget*) gtk_tool_button_get_icon_widget(
								GTK_TOOL_BUTTON(item));
				if (imageHandle->index >= 0)
					hasImage = TRUE;
			}
		}
	}
	GtkToolbarStyle type = GTK_TOOLBAR_ICONS;
	if (hasText && hasImage) {
		if ((_W_WIDGET(toolbar)->style & W_RIGHT) != 0) {
			type = GTK_TOOLBAR_BOTH_HORIZ;
		} else {
			type = GTK_TOOLBAR_BOTH;
		}
	} else if (hasText) {
		type = GTK_TOOLBAR_TEXT;
	} else if (hasImage) {
		type = GTK_TOOLBAR_ICONS;
	}
	gtk_toolbar_set_style(GTK_TOOLBAR(_W_WIDGET(toolbar)->handle), type);
}
/**
 *
 *
 */

w_imagelist* _w_toolbar_get_imagelist(w_toolbar *toolbar) {
	return _W_TOOLBAR(toolbar)->imagelist;
}
wresult _w_toolbar_get_item(w_toolbar *toolbar, int index, w_toolitem *item) {
}
wresult _w_toolbar_get_item_from_point(w_toolbar *toolbar, w_point *point,
		w_toolitem *item) {
}
int _w_toolbar_get_item_count(w_toolbar *toolbar) {
}
void _w_toolbar_get_items(w_toolbar *toolbar, w_iterator *items) {
}
int _w_toolbar_get_row_count(w_toolbar *toolbar) {
}
wresult _w_toolbar_insert(w_toolbar *toolbar, w_toolitem *item,
		const char *text, int style, int index) {
	GtkWidget *labelHandle = 0;
	GtkToolItem* handle = 0;
	_w_image_widget *imageHandle = 0;
	int bits = W_SEPARATOR | W_RADIO | W_CHECK | W_PUSH | W_DROP_DOWN;
	if ((style & W_SEPARATOR) == 0) {
		labelHandle = gtk_label_new_with_mnemonic(text);
		if (labelHandle == 0)
			goto _err;
		imageHandle = _w_image_widget_new();
		if (imageHandle == 0)
			goto _err;
		imageHandle->get_image_list = _w_toolbar_get_image_list;
		imageHandle->parent = W_WIDGET(toolbar);
		imageHandle->index = -1;
	}
	switch (style & bits) {
	case W_SEPARATOR:
		handle = gtk_separator_tool_item_new();
		if (handle == 0)
			goto _err;
		gtk_separator_tool_item_set_draw(GTK_SEPARATOR_TOOL_ITEM(handle), TRUE);
		break;
	case W_DROP_DOWN: {
		handle = gtk_menu_tool_button_new(0, 0);
		if (handle == 0)
			goto _err;
		/*
		 * Feature in GTK. The arrow button of DropDown tool-item is
		 * disabled when it does not contain menu. The fix is to
		 * find the arrow button handle and enable it.
		 */
		/*GtkWidget* child = gtk_bin_get_child(GTK_BIN(handle));
		 long list = gtk_container_get_children(GTK_CONTAINER(child));
		 arrowHandle = g_list_nth_data(list, 1);
		 if (arrowHandle != 0) {
		 GTK.gtk_widget_set_sensitive(arrowHandle, true);
		 if (!GTK.GTK3) {
		 GTK.gtk_widget_set_size_request(
		 GTK.gtk_bin_get_child(arrowHandle), 8, 6);
		 }
		 }*/
	}
		break;
	case W_RADIO:
		/*
		 * Because GTK enforces radio behavior in a button group
		 * a radio group is not created for each set of contiguous
		 * buttons, each radio button will not draw unpressed.
		 * The fix is to use toggle buttons instead.
		 */
	case W_CHECK:
		handle = gtk_toggle_tool_button_new();
		if (handle == 0)
			goto _err;
		break;
	case W_PUSH:
	default:
		handle = gtk_tool_button_new(0, 0);
		if (handle == 0)
			goto _err;
		break;
	}
	if (labelHandle != 0) {
		gtk_tool_button_set_label_widget(GTK_TOOL_BUTTON(handle), labelHandle);
		gtk_widget_show(labelHandle);
	}
	if (imageHandle != 0) {
		gtk_tool_button_set_icon_widget(GTK_TOOL_BUTTON(handle),
				(GtkWidget*) imageHandle);
	}
	/*if ((_W_WIDGET(toolbar)->state & STATE_FOREGROUND) != 0) {
	 setForegroundColor(parent.getForegroundGdkColor());
	 }
	 if ((_W_WIDGET(toolbar)->state & STATE_FONT) != 0) {
	 setFontDescription(parent.getFontDescription());
	 }*/
	/*
	 * Feature in GTK. GtkToolButton class uses this property to
	 * determine whether to show or hide its label when the toolbar
	 * style is GTK_TOOLBAR_BOTH_HORIZ (or W_RIGHT).
	 */
	if ((_W_WIDGET(toolbar)->style & W_RIGHT) != 0)
		gtk_tool_item_set_is_important(GTK_TOOL_ITEM(handle), TRUE);
	if ((style & W_SEPARATOR) == 0)
		gtk_tool_button_set_use_underline(GTK_TOOL_BUTTON(handle), TRUE);
	gtk_widget_show_all(GTK_WIDGET(handle));
	int _index;
	if (index < 0) {
		_index = gtk_toolbar_get_n_items(
				GTK_TOOLBAR(_W_WIDGET(toolbar)->handle));
	} else
		_index = index;
	gtk_toolbar_insert(GTK_TOOLBAR(_W_WIDGET(toolbar)->handle),
			GTK_TOOL_ITEM(handle), index);
	_w_toolbar_relayout(toolbar);
	if (item != 0) {
		_W_TOOLITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TOOLBAR_GET_CLASS(toolbar)->class_toolbaritem);
		_W_TOOLITEM(item)->toolbar = toolbar;
		_W_TOOLITEM(item)->index = _index;
	}
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}

wresult _w_toolbar_set_imagelist(w_toolbar *toolbar, w_imagelist *imagelist) {
	wresult ret = W_TRUE;
	if (imagelist == 0) {
		_W_TOOLBAR(toolbar)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_TOOLBAR(toolbar)->imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_TOOLBAR(toolbar)->imagelist = imagelist;
		}
	}
	//_w_control_update(W_CONTROL(tree));
	return ret;
}
wresult _w_toolitem_get_bounds(w_toolitem *item, w_rect *rect) {
}
w_control* _w_toolitem_get_control(w_toolitem *item) {
}
wresult _w_toolitem_get_enabled(w_toolitem *item) {
}
wresult _w_toolitem_get_selection(w_toolitem *item) {
}
wresult _w_toolitem_get_tooltip_text(w_toolitem *item, w_alloc *text) {
}
int _w_toolitem_get_width(w_toolitem *item) {
}
wresult _w_toolitem_is_enabled(w_toolitem *item) {
}
wresult _w_toolitem_set_control(w_toolitem *item, w_control *control) {
}
wresult _w_toolitem_set_enabled(w_toolitem *item, int enabled) {
}
wresult _w_toolitem_set_image(w_toolitem *item, int image) {
	GtkToolItem *toolitem = gtk_toolbar_get_nth_item(
			GTK_TOOLBAR(_W_WIDGET(_W_TOOLITEM(item)->toolbar)->handle),
			_W_TOOLITEM(item)->index);
	if (toolitem != 0) {
		if (GTK_IS_TOOL_BUTTON(toolitem)) {
			_w_image_widget *im =
					(_w_image_widget*) gtk_tool_button_get_icon_widget(
							GTK_TOOL_BUTTON(toolitem));
			if (im != 0) {
				im->index = image;
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}
wresult _w_toolitem_set_selection(w_toolitem *item, int selected) {
}
wresult _w_toolitem_set_tooltip_text(w_toolitem *item, const char *string) {
}
wresult _w_toolitem_set_width(w_toolitem *item, int width) {
}

wresult _w_toolbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle, *handle = 0;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	handle = gtk_toolbar_new();
	if (handle == 0)
		goto _err;
	/*
	* Ensure that either of HORIZONTAL or VERTICAL is set.
	* NOTE: HORIZONTAL and VERTICAL have the same values
	* as H_SCROLL and V_SCROLL so it is necessary to first
	* clear these bits to avoid scroll bars and then reset
	* the bits using the original style supplied by the
	* programmer.
	*/
	/*if ((style & W_VERTICAL) != 0) {
		this.style |= SWT.VERTICAL;
	} else {
		this.style |= SWT.HORIZONTAL;
	}*/
	GtkOrientation orientation = (style & W_VERTICAL) != 0 ? GTK_ORIENTATION_VERTICAL : GTK_ORIENTATION_HORIZONTAL;
	gtk_orientable_set_orientation(GTK_ORIENTABLE(handle), orientation);
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	GtkIconSize iconesize;
#if !GTK3
	if ((style & W_FLAT) != 0) {
		gtk_widget_set_name (handle, "swt-toolbar-flat");
	}
	iconesize = GTK_ICON_SIZE_LARGE_TOOLBAR;
#else
	iconesize = GTK_ICON_SIZE_SMALL_TOOLBAR;
#endif

	/*
	 * Bug in GTK.  GTK will segment fault if gtk_widget_reparent() is called
	 * on a tool bar or on a widget hierarchy containing a tool bar when the icon
	 * size is not GTK_ICON_SIZE_LARGE_TOOLBAR.  The fix is to set the icon
	 * size to GTK_ICON_SIZE_LARGE_TOOLBAR.
	 *
	 * Note that the segmentation fault does not happen on GTK 3, but the
	 * tool bar preferred size is too big with GTK_ICON_SIZE_LARGE_TOOLBAR
	 * when the tool bar item has no image or text.
	 */
	gtk_toolbar_set_icon_size(GTK_TOOLBAR(handle), iconesize);

	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
	//gtk_toolbar_set_style (GTK_TOOLBAR(handle), GTK_TOOLBAR_TEXT);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	//g_object_set_qdata(G_OBJECT(children), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	struct _w_widget_reserved *reserved = _w_widget_get_reserved(widget);
	//_w_composite_connect_signal(widget, _W_CONTROL_RESERVED(reserved));

	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	//_W_TABFOLDER(widget)->children = children;
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: if (fixedHandle)
		gtk_widget_destroy(fixedHandle);
	if (handle)
		gtk_widget_destroy(handle);
	return W_ERROR_NO_HANDLES;
}
int _w_toolbar_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	/*
	 * Feature in GTK. Size of toolbar is calculated incorrectly
	 * and appears as just the overflow arrow, if the arrow is enabled
	 * to display. The fix is to disable it before the computation of
	 * size and enable it if WRAP style is set.
	 */
	gtk_toolbar_set_show_arrow(GTK_TOOLBAR(_W_WIDGET(widget)->handle), FALSE);
	_w_control_compute_native_size(widget, _W_WIDGET(widget)->handle, e,
			reserved);
	if ((_W_WIDGET(widget)->style & W_WRAP) != 0)
		gtk_toolbar_set_show_arrow(GTK_TOOLBAR(_W_WIDGET(widget)->handle),
				TRUE);
	return W_TRUE;

}
void _w_toolbar_class_init(struct _w_toolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TOOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_toolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_toolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_toolbar);
	/*
	 * public function
	 */
	clazz->get_imagelist = _w_toolbar_get_imagelist;
	clazz->get_item = _w_toolbar_get_item;
	clazz->get_item_from_point = _w_toolbar_get_item_from_point;
	clazz->get_item_count = _w_toolbar_get_item_count;
	clazz->get_items = _w_toolbar_get_items;
	clazz->get_row_count = _w_toolbar_get_row_count;
	clazz->insert = _w_toolbar_insert;
	clazz->set_imagelist = _w_toolbar_set_imagelist;
	/*
	 *
	 */
	struct _w_toolitem_class *class_item = clazz->class_toolbaritem;
	class_item->get_bounds = _w_toolitem_get_bounds;
	class_item->get_control = _w_toolitem_get_control;
	class_item->get_enabled = _w_toolitem_get_enabled;
	class_item->get_selection = _w_toolitem_get_selection;
	class_item->get_tooltip_text = _w_toolitem_get_tooltip_text;
	class_item->get_width = _w_toolitem_get_width;
	class_item->is_enabled = _w_toolitem_is_enabled;
	class_item->set_control = _w_toolitem_set_control;
	class_item->set_enabled = _w_toolitem_set_enabled;
	class_item->set_image = _w_toolitem_set_image;
	class_item->set_selection = _w_toolitem_set_selection;
	class_item->set_tooltip_text = _w_toolitem_set_tooltip_text;
	class_item->set_width = _w_toolitem_set_width;
	class_item->item.widgetdata.toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	class_item->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	class_item->item.widgetdata.close = _w_widgetdata_close;
	//class_item->item.widgetdata.copy = _w_tabitem_copy;
	//class_item->item.widgetdata.equals = _w_tabitem_equals;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_toolbar_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	reserved->fixedHandle = _w_widget_hp;
	//reserved->clientHandle = _w_widget_h;
	_W_COMPOSITE_RESERVED(reserved)->parenting_handle = _w_widget_h;
	reserved->widget.compute_size = _w_toolbar_compute_size;
}
#endif
