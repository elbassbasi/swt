/*
 * toolbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "toolbar.h"
#include "../widgets/toolkit.h"
/**
 * private
 */
w_imagelist* _w_toolbar_get_image_list(w_widget *widget) {
}
void _w_toolbar_relayout(w_toolbar *toolbar) {
}
/**
 *
 *
 */

w_imagelist* _w_toolbar_get_imagelist(w_toolbar *toolbar) {
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
}

wresult _w_toolbar_set_imagelist(w_toolbar *toolbar, w_imagelist *imagelist) {
}
wresult _w_toolitem_get_bounds(w_toolitem *item, w_rect *rect) {
}
w_control* _w_toolitem_get_control(w_toolitem *item) {
}
wresult _w_toolitem_get_enabled(w_toolitem *item) {
}
wresult _w_toolitem_get_selection(w_toolitem *item) {
}
wresult _w_toolitem_get_tooltip_text(w_toolitem *item,w_alloc alloc,void* user_data) {
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
}
int _w_toolbar_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;

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
	reserved->widget.compute_size = _w_toolbar_compute_size;
}
#endif
