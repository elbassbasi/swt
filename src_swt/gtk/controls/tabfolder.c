/*
 * tabview.c
 *
 *  Created on: 4 mai 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "tabfolder.h"
#include "../widgets/toolkit.h"
w_imagelist* _w_tabfolder_get_image_list(w_widget *widget) {
	return _W_TABFOLDER(widget)->imagelist;
}
/*
 * tabitem
 */
void* _w_tabitem_get_data(w_item *item);
wresult _w_tabfolder_get_selection(w_tabfolder *tabfolder, w_tabitem *item);
GtkWidget* _w_tabfolder_parenting_handle(w_widget *composite,
		struct _w_widget_reserved *reserved) {
	return gtk_widget_get_parent(_W_WIDGET(composite)->handle);
}
GtkWidget* _w_tabfolder_clientHandle(w_widget *composite,
		struct _w_widget_reserved *reserved) {
	int index = gtk_notebook_get_current_page(
			GTK_NOTEBOOK(_W_WIDGET(composite)->handle));
	if (index != -1) {
		return gtk_notebook_get_nth_page(
				GTK_NOTEBOOK(_W_WIDGET(composite)->handle), index);
	}
	return _W_WIDGET(composite)->handle;
}
wresult _w_tabfolder_set_bounds_0(w_control *control, w_point *location,
		w_size *size,_w_control_reserved *reserved) {
	wresult result = _w_control_set_bounds_0(control,location,size, reserved);
	if (result >= 2) {
		w_tabitem item;
		_w_tabfolder_get_selection(W_TABFOLDER(control), &item);
		w_control *c = 0;
		if (W_WIDGETDATA(&item)->clazz != 0) {
			c = w_tabitem_get_control(W_TABITEM(&item));
		}
		if (c != 0) {
			w_event e;
			e.type = W_EVENT_RESIZE;
			e.platform_event = 0;
			e.data = 0;
			e.widget = W_WIDGET(c);
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(control), &r);
			w_control_set_bounds(c, &r.pt,&r.sz);
			//_w_widget_send_event(W_WIDGET(c), &e);
		}
	}
	return result;
}
void _w_tabitem_copy(w_widgetdata *from, w_widgetdata *to) {
}
wbool _w_tabitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
}
w_widget* _w_tabitem_get_parent_widget(w_item *item) {
	return W_WIDGET(_W_ITEM(item)->parent);
}
wresult _w_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item);
void* _w_tabitem_get_data(w_item *item) {
	GtkWidget *pageHandle = gtk_notebook_get_nth_page(
			GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(item)->parent)->handle),
			_W_ITEM(item)->index);
	if (pageHandle != 0) {
		return g_object_get_qdata(G_OBJECT(pageHandle), gtk_toolkit->quark[1]);
	} else
		return 0;
}
int _w_tabitem_get_index(w_item *item) {
	return _W_ITEM(item)->index;
}
wresult _w_tabitem_get_text(w_item *item, w_alloc *string) {
}
wresult _w_tabitem_set_data(w_item *item, void *data) {
	GtkWidget *pageHandle = gtk_notebook_get_nth_page(
			GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(item)->parent)->handle),
			_W_ITEM(item)->index);
	if (pageHandle != 0) {
		g_object_set_qdata(G_OBJECT(pageHandle), gtk_toolkit->quark[1], data);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_tabitem_set_text(w_item *item, const char *string) {
}
wresult _w_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
}
int _w_tabitem_get_image(w_tabitem *item) {
}
void _w_tabitem_find_image_0(GtkWidget *widget, gpointer data) {
	if (_W_IS_IMAGE_WIDGET(widget)) {
		*((_w_image_widget**) data) = (_w_image_widget*) widget;
	}
}
wresult _w_tabitem_set_image(w_tabitem *item, int index) {
	GtkWidget *pageHandle = gtk_notebook_get_nth_page(
			GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(item)->parent)->handle),
			_W_ITEM(item)->index);
	if (pageHandle != 0) {
		GtkWidget *tab = gtk_notebook_get_tab_label(
				GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(item)->parent)->handle),
				pageHandle);
		if (tab != 0) {
			_w_image_widget *image = 0;
			gtk_container_forall(GTK_CONTAINER(tab), _w_tabitem_find_image_0,
					&image);
			if (image != 0) {
				image->index = index;
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}
wresult _w_tabitem_remove(w_tabitem *item, wbool destroy_Control) {
	gtk_notebook_remove_page(
			GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(item)->parent)->handle),
			_W_ITEM(item)->index);
	return W_TRUE;
}
wresult _w_tabitem_pack(w_tabitem *item, w_control *control) {
	if (w_widget_is_ok(W_WIDGET(control))) {
		GtkWidget *pageHandle = gtk_notebook_get_nth_page(
				GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(item)->parent)->handle),
				_W_ITEM(item)->index);
		if (pageHandle != 0) {
			struct _w_widget_reserved *reserved = _w_widget_get_reserved(
					W_WIDGET(control));
			GtkWidget *topHandle = reserved->topHandle(W_WIDGET(control),
					reserved);
			if (gtk_widget_get_parent(topHandle) != pageHandle) {
				gtk_widget_reparent(topHandle, pageHandle);
			}
		}
		int index = gtk_notebook_get_current_page(
				GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(item)->parent)->handle));
		if (index == _W_ITEM(item)->index) {
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(_W_ITEM(item)->parent),
					&r);
			w_control_set_visible(control, W_TRUE);
			w_control_set_bounds(control, &r.pt,&r.sz);
		} else {
			w_control_set_visible(control, W_FALSE);
		}
	}
	return W_TRUE;
}
/*
 * tabfolder
 */

w_imagelist* _w_tabfolder_get_imagelist(w_tabfolder *tabfolder) {

}
wresult _w_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item) {
	int count = gtk_notebook_get_n_pages(
			GTK_NOTEBOOK(_W_WIDGET(tabfolder)->handle));
	if (index < count && index >= 0) {
		if (item != 0) {
			_W_ITEM(item)->clazz = W_WIDGETDATA_CLASS(
					W_TABFOLDER_GET_CLASS(tabfolder)->class_tabfolderitem);
			_W_ITEM(item)->parent = W_WIDGET(tabfolder);
			_W_ITEM(item)->index = index;
		}
		return W_TRUE;
	} else {
		_W_ITEM(item)->clazz = 0;
		return W_FALSE;
	}
}
wresult _w_tabfolder_get_item_p(w_tabfolder *tabfolder, w_point *point,
		w_tabitem *item) {
}
int _w_tabfolder_get_item_count(w_tabfolder *tabfolder) {
	return gtk_notebook_get_n_pages(GTK_NOTEBOOK(_W_WIDGET(tabfolder)->handle));
}
void _w_tabfolder_get_items(w_tabfolder *tabfolder, w_iterator *items) {
}
wresult _w_tabfolder_get_selection(w_tabfolder *tabfolder, w_tabitem *item) {
	int index = gtk_notebook_get_current_page(
			GTK_NOTEBOOK(_W_WIDGET(tabfolder)->handle));
	return _w_tabfolder_get_item(tabfolder, index, item);
}
wresult _w_tabfolder_insert_item(w_tabfolder *tabfolder, w_tabitem *item,
		const char *text, int index) {
	if (text == 0)
		text = "";
	struct _w_tabfolder_reserved *reserved = _W_TABFOLDER_GET_RESERVED(
			tabfolder);
	GtkWidget *boxHandle, *labelHandle = 0, *pageHandle = 0, *closeHandle;
	_w_image_widget *imageHandle = 0;
	int i;
	boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	if (boxHandle == 0)
		goto _err;
	labelHandle = gtk_label_new_with_mnemonic(text);
	if (labelHandle == 0)
		goto _err;
	imageHandle = _w_image_widget_new();
	if (imageHandle == 0)
		goto _err;
	imageHandle->get_image_list = _w_tabfolder_get_image_list;
	imageHandle->parent = W_WIDGET(tabfolder);
	imageHandle->index = -1;

	gtk_container_add(GTK_CONTAINER(boxHandle), (GtkWidget*) imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_box_set_child_packing(GTK_BOX(boxHandle), labelHandle, TRUE, TRUE, 0,
			GTK_PACK_START);
	gtk_box_set_child_packing(GTK_BOX(boxHandle), (GtkWidget*) imageHandle,
	TRUE, TRUE, 0, GTK_PACK_START);
	pageHandle = _w_fixed_new();
	if (pageHandle == 0)
		goto _err;
	g_object_set_qdata(G_OBJECT(pageHandle), gtk_toolkit->quark[0], tabfolder);
	if (_W_WIDGET(tabfolder)->style & W_CLOSE) {
		closeHandle = gtk_button_new_from_icon_name("gtk-close",
				GTK_ICON_SIZE_MENU);
		if (closeHandle == 0)
			goto _err;
		if (reserved->signal_selection_id == 0) {
			reserved->signal_selection_id = g_signal_lookup("clicked",
					gtk_button_get_type());
		}
		g_object_set_qdata(G_OBJECT(closeHandle), gtk_toolkit->quark[0],
				tabfolder);
		g_object_set_qdata(G_OBJECT(closeHandle), gtk_toolkit->quark[1],
				pageHandle);
		_w_widget_connect(closeHandle, SIGNAL_CLICKED,
				reserved->signal_selection_id,
				FALSE);
		gtk_button_set_relief(GTK_BUTTON(closeHandle), GTK_RELIEF_NONE);
		gtk_container_add(GTK_CONTAINER(boxHandle), closeHandle);
		gtk_box_set_child_packing(GTK_BOX(boxHandle), (GtkWidget*) imageHandle,
		TRUE, TRUE, 0, GTK_PACK_START);
	}
	//_w_fixed_resize(pageHandle, -2, -2);
	//g_signal_handlers_block_matched (handle, G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, SWITCH_PAGE);
	/*if (index < 0)
	 index = gtk_notebook_get_n_pages(
	 GTK_NOTEBOOK(parent->GetHandle<Tab_t>().handle));*/
	g_signal_handlers_block_matched(GTK_NOTEBOOK(_W_WIDGET(tabfolder)->handle),
			G_SIGNAL_MATCH_DATA, reserved->signal_switch_page_id, 0, 0, 0, 0);
	i = gtk_notebook_insert_page(GTK_NOTEBOOK(_W_WIDGET(tabfolder)->handle),
			pageHandle, boxHandle, index);
	g_signal_handlers_unblock_matched(
			GTK_NOTEBOOK(_W_WIDGET(tabfolder)->handle), G_SIGNAL_MATCH_DATA,
			reserved->signal_switch_page_id, 0, 0, 0, 0);
//g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, SWITCH_PAGE);
	if (i < 0)
		goto _err;

	gtk_widget_show_all(boxHandle);
	gtk_widget_show_all(pageHandle);
	//gtk_widget_hide(imageHandle);
	if (item != 0) {
		_W_ITEM(item)->clazz = W_WIDGETDATA_CLASS(
				W_TABFOLDER_GET_CLASS(tabfolder)->class_tabfolderitem);
		_W_ITEM(item)->parent = W_WIDGET(tabfolder);
		_W_ITEM(item)->index = i;
	}

	/*if (itemCount == 1) {
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, SWITCH_PAGE);
	 OS.gtk_notebook_set_current_page (handle, 0);
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, SWITCH_PAGE);
	 Event event = new Event();
	 event.item = items[0];
	 sendSelectionEvent (SWT.Selection, event, false);
	 }*/
	return 1;
	_err: if (labelHandle)
		gtk_widget_destroy(labelHandle);
	if (pageHandle)
		gtk_widget_destroy(pageHandle);
	if (item != 0) {
		_W_ITEM(item)->clazz = 0;
	}
	return W_ERROR_NO_HANDLES;
}
wresult _w_tabfolder_set_imagelist(w_tabfolder *tabfolder,
		w_imagelist *imagelist) {
	wresult ret = W_TRUE;
	if (imagelist == 0) {
		_W_TABFOLDER(tabfolder)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_TABFOLDER(tabfolder)->imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_TABFOLDER(tabfolder)->imagelist = imagelist;
		}
	}
	//_w_control_update(W_CONTROL(tree));
	return ret;
}

wresult _w_tabfolder_set_selection(w_tabfolder *tabfolder, int index) {
	gtk_notebook_set_current_page(GTK_NOTEBOOK(_W_WIDGET(tabfolder)->handle),
			index);
	return W_TRUE;
}
wresult _w_tabfolder_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle, *handle = 0;
	GtkPositionType pos;
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	handle = gtk_notebook_new();
	if (handle == 0)
		goto _err;
	gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	gtk_notebook_set_show_tabs(GTK_NOTEBOOK(handle), TRUE);
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(handle), TRUE);
	pos = GTK_POS_TOP;
	if (style & W_BOTTOM) {
		pos = GTK_POS_BOTTOM;
	}
	if (style & W_LEFT) {
		pos = GTK_POS_LEFT;
	}
	if (style & W_RIGHT) {
		pos = GTK_POS_RIGHT;
	}
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(handle), pos);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	//g_object_set_qdata(G_OBJECT(children), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	struct _w_tabfolder_reserved *reserved = _W_TABFOLDER_GET_RESERVED(widget);
	_w_composite_hook_events(widget, _W_WIDGET_RESERVED(reserved));
	if (reserved->signal_switch_page_id == 0) {
		reserved->signal_switch_page_id = g_signal_lookup("switch-page",
				gtk_notebook_get_type());
	}
	_w_widget_connect(handle, SIGNAL_SWITCH_PAGE,
			reserved->signal_switch_page_id,
			FALSE);

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
/**
 * signals
 */
int _w_tabfolder_get_client_area(w_widget *widget,
		struct w_event_client_area *e, struct _w_widget_reserved *reserved) {
	int ret = _w_composite_get_client_area(widget, e, reserved);
	e->rect->x = 0;
	e->rect->y = 0;
	return ret;
}

gboolean _gtk_tabfolder_focus(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	return FALSE;
}

gboolean _gtk_tabfolder_switch_page(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	w_event_item _e;
	w_tabitem item;
	_w_tabfolder_get_selection(W_TABFOLDER(widget), &item);
	w_control *c = 0;
	if (W_WIDGETDATA(&item)->clazz != 0) {
		c = w_tabitem_get_control(&item);
	}
	if (c != 0 && w_widget_is_ok(W_WIDGET(c))) {
		w_control_set_visible(c, W_FALSE);
		GtkWidget *pageHandle = gtk_notebook_get_nth_page(
				GTK_NOTEBOOK(_W_WIDGET(_W_ITEM(&item)->parent)->handle),
				_W_ITEM(&item)->index);
		if (pageHandle != 0) {
			struct _w_widget_reserved *reserved = _w_widget_get_reserved(
					W_WIDGET(c));
			GtkWidget *topHandle = reserved->topHandle(W_WIDGET(c), reserved);
			if (gtk_widget_get_parent(topHandle) != pageHandle) {
				//gtk_widget_reparent(topHandle, pageHandle);
				gtk_widget_unparent(pageHandle);
				gtk_widget_set_parent(topHandle, pageHandle);
			}
		}
	}
	_w_tabfolder_get_item(W_TABFOLDER(widget), (intptr_t) e->args[1], &item);
	c = w_tabitem_get_control(&item);
	if (c != 0) {
		if (w_widget_is_ok(W_WIDGET(c))) {
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(widget), &r);
			w_control_set_visible(c, W_TRUE);
			w_control_set_bounds(c, &r.pt,&r.sz);
		}
	}
	_e.event.type = W_EVENT_ITEM_SELECTION;
	_e.event.widget = widget;
	_e.event.platform_event = (struct w_event_platform*) e;
	_e.event.data = 0;
	_e.item = (w_item*) &item;
	_w_widget_send_event(widget, (w_event*) &_e);
	return FALSE;
}
gboolean _gtk_tabfolder_clicked(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	w_event_item _e;
	w_tabitem item;
	GtkWidget *pageHandle = (GtkWidget*) g_object_get_qdata(G_OBJECT(e->widget),
			gtk_toolkit->quark[1]);
	GtkNotebook *notebook = GTK_NOTEBOOK(_W_WIDGET(widget)->handle);
	wuint l = gtk_notebook_get_n_pages(notebook);
	int index = -1;
	for (wuint i = 0; i < l; i++) {
		if (gtk_notebook_get_nth_page(notebook, i) == pageHandle) {
			index = i;
			break;
		}
	}
	if(index == -1) return FALSE;
	_w_tabfolder_get_item(W_TABFOLDER(widget), index, &item);
	_e.event.type = W_EVENT_ITEM_CLOSE;
	_e.event.widget = widget;
	_e.event.platform_event = (struct w_event_platform*) e;
	_e.event.data = 0;
	_e.item = (w_item*) &item;
	_w_widget_send_event(widget, (w_event*) &_e);
	return FALSE;
}
void _w_tabfolder_class_init(struct _w_tabfolder_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	/*
	 * class
	 */
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TABFOLDER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tabfolder_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tabfolder);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tabfolder);
	W_WIDGET_CLASS(clazz)->create = _w_tabfolder_create;

	clazz->get_imagelist = _w_tabfolder_get_imagelist;
	clazz->get_item = _w_tabfolder_get_item;
	clazz->get_item_p = _w_tabfolder_get_item_p;
	clazz->get_item_count = _w_tabfolder_get_item_count;
	clazz->get_items = _w_tabfolder_get_items;
	clazz->get_selection = _w_tabfolder_get_selection;
	clazz->insert_item = _w_tabfolder_insert_item;
	clazz->set_imagelist = _w_tabfolder_set_imagelist;
	clazz->set_selection = _w_tabfolder_set_selection;
	/*
	 * class item
	 */
	struct _w_tabitem_class *item = clazz->class_tabfolderitem;
	item->item.widgetdata.toolkit = (w_toolkit*) gtk_toolkit;
	item->get_bounds = _w_tabitem_get_bounds;
	item->get_image = _w_tabitem_get_image;
	item->set_image = _w_tabitem_set_image;
	item->remove = _w_tabitem_remove;
	item->pack = _w_tabitem_pack;
	item->item.get_parent_widget = _w_tabitem_get_parent_widget;
	item->item.get_data = _w_tabitem_get_data;
	item->item.get_index = _w_tabitem_get_index;
	item->item.get_text = _w_tabitem_get_text;
	item->item.set_data = _w_tabitem_set_data;
	item->item.set_text = _w_tabitem_set_text;
	item->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	item->item.widgetdata.close = _w_widgetdata_close;
	item->item.widgetdata.copy = _w_tabitem_copy;
	item->item.widgetdata.equals = _w_tabitem_equals;
	/*
	 * reserved
	 */
	struct _w_tabfolder_reserved *reserved = _W_TABFOLDER_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	_W_CONTROL_RESERVED(reserved)->fixedHandle = _w_widget_hp;
	_W_CONTROL_RESERVED(reserved)->clientHandle = _w_tabfolder_clientHandle;
	_W_CONTROL_RESERVED(reserved)->eventHandle = _w_widget_h;
	_W_CONTROL_RESERVED(reserved)->set_bounds = _w_tabfolder_set_bounds_0;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle = _w_widget_h0;
	_W_COMPOSITE_RESERVED(reserved)->parenting_handle =
			_w_tabfolder_parenting_handle;
	_W_WIDGET_RESERVED(reserved)->get_client_area =
			_w_tabfolder_get_client_area;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_RESERVED(reserved)->signals;
	signals[SIGNAL_FOCUS] = _gtk_tabfolder_focus;
	signals[SIGNAL_SWITCH_PAGE] = _gtk_tabfolder_switch_page;
	signals[SIGNAL_CLICKED] = _gtk_tabfolder_clicked;

}
#endif
