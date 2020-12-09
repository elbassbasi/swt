/*
 * expandbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "expandbar.h"
#include "../widgets/toolkit.h"
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
}
wresult _w_expandbar_set_spacing(w_expandbar *expandbar, int spacing) {
}

wresult _w_expandbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
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
	itemclazz->item.widgetdata.toolkit = W_TOOLKIT(mac_toolkit);
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
}
#endif
