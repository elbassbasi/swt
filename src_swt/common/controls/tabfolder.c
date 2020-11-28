/*
 * tabview.c
 *
 *  Created on: 12 juin 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>
/*
 * tabitem
 */
w_control* w_tabitem_get_control(w_tabitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		w_widget *widget = w_item_get_parent_widget(W_ITEM(item));
		if (widget != 0) {
			w_event_tabitem event;
			event.event.type = W_EVENT_ITEM_GET_CONTROL;
			event.event.platform_event = 0;
			event.event.data = 0;
			event.event.widget = widget;
			event.control = 0;
			event.item = item;
			w_widget_send_event(widget, (w_event*) &event);
			return event.control;
		}
	}
	return 0;
}
wresult w_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->get_bounds(item, rect);
	} else
		return W_ERROR_NO_HANDLES;
}
int w_tabitem_get_image(w_tabitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->get_image(item);
	} else
		return -1;
}
wresult w_tabitem_remove(w_tabitem *item,int destroy_Control){
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->remove(item, destroy_Control);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tabitem_set_image(w_tabitem *item, int image) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return W_ERROR_NO_HANDLES;
}

wresult w_tabitem_set_control(w_tabitem *item, w_control *control) {
	if (W_WIDGETDATA_CHECK(item)) {
		w_widget *widget = w_item_get_parent_widget(W_ITEM(item));
		if (widget != 0) {
			w_event_tabitem event;
			event.event.type = W_EVENT_ITEM_SET_CONTROL;
			event.event.platform_event = 0;
			event.event.data = 0;
			event.event.widget = widget;
			event.control = control;
			event.item = item;
			int ret = w_widget_send_event(widget, (w_event*) &event);
			W_TABITEM_GET_CLASS(item)->pack(item, control);
			return ret;
		}
		return W_FALSE;
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tabitem_pack(w_tabitem *item, w_control *control) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TABITEM_GET_CLASS(item)->pack(item, control);
	} else
		return W_ERROR_NO_HANDLES;
}
/*
 * tabfolder
 */
wresult w_tabfolder_create(w_tabfolder *tabfolder, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tabfolder), toolkit, W_WIDGET(parent),
			style, _W_CLASS_TABFOLDER, post_event);
}
w_tabfolder* w_tabfolder_new(struct w_toolkit *toolkit, w_composite *parent,
                             wuint64 style, w_widget_post_event_proc post_event) {
	return W_TABFOLDER(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TABFOLDER,post_event));
}
w_imagelist* w_tabfolder_get_imagelist(w_tabfolder *tabfolder) {
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->get_imagelist(tabfolder);
	} else
		return 0;
}
wresult w_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->get_item(tabfolder, index,
				item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tabfolder_get_item_p(w_tabfolder *tabfolder, w_point *point,
		w_tabitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->get_item_p(tabfolder, point,
				item);
	} else
		return W_ERROR_NO_HANDLES;
}
int w_tabfolder_get_item_count(w_tabfolder *tabfolder) {
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->get_item_count(tabfolder);
	} else
		return 0;
}
void w_tabfolder_get_items(w_tabfolder *tabfolder, w_iterator *items) {
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->get_items(tabfolder, items);
	}
}
wresult w_tabfolder_get_selection(w_tabfolder *tabfolder, w_tabitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->get_selection(tabfolder, item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tabfolder_insert_item(w_tabfolder *tabfolder, w_tabitem *item,
		const char *text, int index) {
	w_widgetdata_close(W_WIDGETDATA(item));
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->insert_item(tabfolder, item,
				text, index);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tabfolder_set_imagelist(w_tabfolder *tabfolder,
		w_imagelist *imagelist) {
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->set_imagelist(tabfolder,
				imagelist);
	} else
		return W_ERROR_NO_HANDLES;
}

wresult w_tabfolder_set_selection(w_tabfolder *tabfolder, int index) {
	if (W_WIDGET_CHECK(tabfolder)) {
		return W_TABFOLDER_GET_CLASS(tabfolder)->set_selection(tabfolder, index);
	} else
		return W_ERROR_NO_HANDLES;
}
