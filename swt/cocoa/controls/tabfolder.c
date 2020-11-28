/*
 * tabview.c
 *
 *  Created on: 4 mai 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "tabfolder.h"
#include "../widgets/toolkit.h"
/*
 * tabitem
 */
void* _w_tabitem_get_data(w_item *item);
wresult _w_tabfolder_get_selection(w_tabfolder *tabfolder, w_tabitem *item);
void _w_tabitem_copy(w_widgetdata *from, w_widgetdata *to) {
}
wbool _w_tabitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
}
w_widget* _w_tabitem_get_parent_widget(w_item *item) {
}
wresult _w_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item);
void* _w_tabitem_get_data(w_item *item) {
}
int _w_tabitem_get_index(w_item *item) {
}
w_string_ref* _w_tabitem_get_text(w_item *item) {
}
wresult _w_tabitem_set_data(w_item *item, void *data) {
}
wresult _w_tabitem_set_text(w_item *item, const char *string) {
}
wresult _w_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
}
int _w_tabitem_get_image(w_tabitem *item) {
}
wresult _w_tabitem_set_image(w_tabitem *item, int index) {
}
wresult _w_tabitem_pack(w_tabitem *item, w_control *control) {
}
/*
 * tabfolder
 */

w_imagelist* _w_tabfolder_get_imagelist(w_tabfolder *tabfolder) {

}
wresult _w_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item) {
}
wresult _w_tabfolder_get_item_p(w_tabfolder *tabfolder, w_point *point,
		w_tabitem *item) {
}
int _w_tabfolder_get_item_count(w_tabfolder *tabfolder) {
}
void _w_tabfolder_get_items(w_tabfolder *tabfolder, w_iterator *items) {
}
wresult _w_tabfolder_get_selection(w_tabfolder *tabfolder, w_tabitem *item) {
    
}
wresult _w_tabfolder_insert_item(w_tabfolder *tabfolder, w_tabitem *item,
		const char *text, int index) {
	if (text == 0)
		text = "";
	struct _w_tabfolder_reserved *reserved = _W_TABFOLDER_GET_RESERVED(
			tabfolder);
	return W_ERROR_NO_HANDLES;
}
wresult _w_tabfolder_set_imagelist(w_tabfolder *tabfolder,
		w_imagelist *imagelist) {
}

wresult _w_tabfolder_set_selection(w_tabfolder *tabfolder, int index) {
}
wresult _w_tabfolder_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
}
/**
 * signals
 */
int _w_tabfolder_get_client_area(w_widget *widget,
		struct w_event_client_area *e, struct _w_widget_reserved *reserved) {
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
	item->item.widgetdata.toolkit = (w_toolkit*) mac_toolkit;
	item->get_bounds = _w_tabitem_get_bounds;
	item->get_image = _w_tabitem_get_image;
	item->set_image = _w_tabitem_set_image;
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
	_W_WIDGET_RESERVED(reserved)->get_client_area =
			_w_tabfolder_get_client_area;

}
#endif
