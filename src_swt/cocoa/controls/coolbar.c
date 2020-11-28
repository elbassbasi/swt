/*
 * coolbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "coolbar.h"
#include "../widgets/toolkit.h"
#define MARGIN_WIDTH 4
#define GRABBER_WIDTH 2
#define MINIMUM_WIDTH ((2 * MARGIN_WIDTH) + GRABBER_WIDTH)

#define CHEVRON_HORIZONTAL_TRIM -1			//platform dependent values
#define CHEVRON_VERTICAL_TRIM -1
#define CHEVRON_LEFT_MARGIN 2
#define CHEVRON_IMAGE_WIDTH 8	//Width to draw the double arrow
#define ROW_SPACING 2
#define CLICK_DISTANCE 3
#define DEFAULT_COOLBAR_WIDTH 0
#define DEFAULT_COOLBAR_HEIGHT 0
int _w_coolbar_check_style(int style) {
	style |= W_NO_FOCUS;
	return (style | W_NO_REDRAW_RESIZE) & ~(W_VSCROLL | W_HSCROLL);
}
wresult _w_coolbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
}
void _w_coolbar_fix_point(w_coolbar *coolbar, w_point *result, int x, int y) {
}
void _w_coolbar_fix_rectangle(w_coolbar *coolbar, w_rect *result, int x, int y,
		int width, int height) {
}
int _w_coolbar_get_width(w_coolbar *coolbar) {
}
int _w_coolitem_internal_get_minimum_width (w_coolbar *coolbar,__w_coolitem* _item) {
}
void _w_coolitem_set_bounds (w_coolbar *coolbar,__w_coolitem* _item,w_rect* bounds) {
}
void _w_coolbar_internal_redraw (w_coolbar *coolbar,int x, int y, int width, int height) {
}
void _w_coolbar_wrap_items(w_coolbar *coolbar, int maxWidth) {
}
/**
 * Return the height of the bar after it has
 * been properly laid out for the given width.
 */
int _w_coolbar_layout_items(w_coolbar *coolbar) {
}
/*
 *
 */
wresult _w_coolitem_compute_size(w_coolitem *coolitem, w_size *result,
		int wHint, int hHint) {
}
wresult _w_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds) {
}
w_control* _w_coolitem_get_control(w_coolitem *coolitem) {

}
wresult _w_coolitem_get_minimum_size(w_coolitem *coolitem, w_size *size) {
}
int _w_coolitem_get_order(w_coolitem *coolitem) {
}
wresult _w_coolitem_get_preferred_size(w_coolitem *coolitem, w_size *size) {
}
wresult _w_coolitem_get_size(w_coolitem *coolitem, w_size *size) {
}
int _w_coolitem_get_wrap_indice(w_coolitem *coolitem) {
}
wresult _w_coolitem_set_control(w_coolitem *coolitem, w_control *control) {
	w_coolbar *coolbar = _W_COOLITEM(coolitem)->coolbar;
}
wresult _w_coolitem_set_layout(w_coolitem *coolitem, int order,
		int wrapindices, w_size *sizes) {
}
wresult _w_coolitem_set_minimum_size(w_coolitem *coolitem, w_size *size) {
}
wresult _w_coolitem_set_order(w_coolitem *coolitem, int order) {
}
wresult _w_coolitem_set_preferred_size(w_coolitem *coolitem, w_size *size) {
}
wresult _w_coolitem_set_size(w_coolitem *coolitem, w_size *size) {
}
wresult _w_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
}

wresult _w_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
}
int _w_coolbar_get_item_count(w_coolbar *coolbar) {
}
void _w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {

}
wbool _w_coolbar_get_locked(w_coolbar *coolbar) {
}
wresult _w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
}
wresult _w_coolbar_set_locked(w_coolbar *coolbar, wbool locked) {
}
int _w_coolbar_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
}
void _w_coolbar_class_init(struct _w_coolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_coolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_coolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_coolbar);
	/*
	 * public function
	 */
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_coolbar_create;
	clazz->get_item = _w_coolbar_get_item;
	clazz->get_item_count = _w_coolbar_get_item_count;
	clazz->get_items = _w_coolbar_get_items;
	clazz->get_locked = _w_coolbar_get_locked;
	clazz->insert_item = _w_coolbar_insert_item;
	clazz->set_locked = _w_coolbar_set_locked;

	struct _w_coolitem_class *class_item = clazz->class_coolbaritem;
	class_item->item.widgetdata.toolkit =W_TOOLKIT(mac_toolkit);
	class_item->item.widgetdata.close = _w_widgetdata_close;
	class_item->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	class_item->compute_size = _w_coolitem_compute_size;
	class_item->get_bounds = _w_coolitem_get_bounds;
	class_item->get_control = _w_coolitem_get_control;
	class_item->get_minimum_size = _w_coolitem_get_minimum_size;
	class_item->get_order = _w_coolitem_get_order;
	class_item->get_preferred_size = _w_coolitem_get_preferred_size;
	class_item->get_size = _w_coolitem_get_size;
	class_item->get_wrap_indice = _w_coolitem_get_wrap_indice;
	class_item->set_control = _w_coolitem_set_control;
	class_item->set_layout = _w_coolitem_set_layout;
	class_item->set_minimum_size = _w_coolitem_set_minimum_size;
	class_item->set_order = _w_coolitem_set_order;
	class_item->set_preferred_size = _w_coolitem_set_preferred_size;
	class_item->set_size = _w_coolitem_set_size;
	class_item->set_wrap_indice = _w_coolitem_set_wrap_indice;
//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->widget.compute_size = _w_coolbar_compute_size;
}
#endif
