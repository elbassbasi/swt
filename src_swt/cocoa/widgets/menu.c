/*
 * menu.c
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "menu.h"
#include "shell.h"
#include "toolkit.h"
/*
 * private
 */
struct _w_menuitem_iterator {
	struct _w_iterator_class *clazz;
};
#define _W_MENUITEM_ITERATOR(x) ((struct _w_menuitem_iterator*)x)
wresult _w_menuitem_iterator_close(w_iterator *it) {
	return W_TRUE;
}
int _w_menuitem_iterator_reset(w_iterator *it) {
	return W_TRUE;
}
wresult _w_menuitem_iterator_next(w_iterator *it, void *obj) {
}
size_t _w_menuitem_iterator_get_count(w_iterator *it) {
}
int _w_menuitem_iterator_remove(w_iterator *it) {

}
_w_iterator_class _w_menuitem_iterator_class = { _w_menuitem_iterator_close, //
		_w_menuitem_iterator_next, //
		_w_menuitem_iterator_reset, //
		_w_menuitem_iterator_remove, //
		_w_menuitem_iterator_get_count //
		};

/*
 * menuitem
 */
void _w_menuitem_copy(w_widgetdata *from, w_widgetdata *to) {
}
wresult _w_menuitem_insert(w_menuitem *parent, w_menuitem *item,
		const char *text, int style, int index) {
}
int _w_menuitem_get_accelerator(w_menuitem *item) {
}
void* _w_menuitem_get_data(w_item *item) {
}
wresult _w_menuitem_get_enabled(w_menuitem *item) {
}
wresult _w_menuitem_get_item(w_menuitem *item, unsigned int index,
		w_menuitem *sub_item) {
}
int _w_menuitem_get_item_count(w_menuitem *item) {
}
void _w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
}
unsigned short _w_menuitem_get_id(w_menuitem *item) {
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
}
w_widget* _w_menuitem_get_menu(w_item *item) {
}
wresult _w_menuitem_get_parent(w_menuitem *item, w_menuitem *parent) {
}
wresult _w_menuitem_get_selection(w_menuitem *item) {
}
int _w_menuitem_get_style(w_menuitem *item) {
}
w_string_ref* _w_menuitem_get_text(w_item *item) {
}
wresult _w_menuitem_is_enabled(w_menuitem *item) {
}
wresult _w_menuitem_remove(w_menuitem *item) {
}
wresult _w_menuitem_remove_item(w_menuitem *item, int index) {
}
wresult _w_menuitem_set_accelerator(w_menuitem *item, int accelerator) {
	return W_TRUE;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	return W_TRUE;
}
wresult _w_menuitem_set_enabled(w_menuitem *item, int enabled) {
	return W_TRUE;
}
wresult _w_menuitem_set_id(w_menuitem *item, unsigned short id) {
	return W_TRUE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
}
wresult _w_menuitem_set_selection(w_menuitem *item, int selected) {

	return W_FALSE;
}
wresult _w_menuitem_set_text(w_item *item, const char *string) {
}
/*
 * menu
 */
wresult _w_menu_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
}

void _w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
}
wresult _w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	return W_TRUE;
}
void _w_menu_dispose(w_widget *widget) {
}
int _w_menu_get_orientation(w_menu *menu) {
}
w_control* _w_menu_get_parent(w_menu *menu) {
	return _W_MENU(menu)->parent;
}
wresult _w_menu_get_visible(w_menu *menu) {
}
wresult _w_menu_is_visible(w_menu *menu) {
}
void _w_menu_set_location(w_menu *menu, w_point *location) {
}
void _w_menu_set_orientation(w_menu *menu, int orientation) {
}
wresult _w_menu_set_visible(w_menu *menu, int visible) {
	return W_TRUE;
}
/*
 * post event
 */
int _w_menu_post_event(w_widget *widget, w_event *e) {
	if (e->type == W_EVENT_PLATFORM) {
	} else
		return W_TRUE;
}
void _w_menu_class_init(struct _w_menu_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_MENU;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_menu_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_menu);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_menu);
	//clazz
	W_WIDGET_CLASS(clazz)->create = _w_menu_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_menu_post_event;
	W_WIDGET_CLASS(clazz)->dispose = _w_menu_dispose;
	clazz->get_bounds = _w_menu_get_bounds;
	clazz->get_root = _w_menu_get_root;
	clazz->get_orientation = _w_menu_get_orientation;
	clazz->get_parent = _w_menu_get_parent;
	clazz->get_visible = _w_menu_get_visible;
	clazz->is_visible = _w_menu_is_visible;
	clazz->set_location = _w_menu_set_location;
	clazz->set_orientation = _w_menu_set_orientation;
	clazz->set_visible = _w_menu_set_visible;
	//item
	struct _w_menuitem_class *item = clazz->class_menuitem;
	item->item.widgetdata.toolkit = mac_toolkit;
	item->item.get_parent_widget = _w_menuitem_get_menu;
	item->item.get_data = _w_menuitem_get_data;
	item->item.get_text = _w_menuitem_get_text;
	item->item.set_data = _w_menuitem_set_data;
	item->item.set_text = _w_menuitem_set_text;
	item->insert = _w_menuitem_insert;
	item->get_accelerator = _w_menuitem_get_accelerator;
	item->get_enabled = _w_menuitem_get_enabled;
	item->get_item = _w_menuitem_get_item;
	item->get_item_count = _w_menuitem_get_item_count;
	item->get_items = _w_menuitem_get_items;
	item->get_id = _w_menuitem_get_id;
	item->get_image = _w_menuitem_get_image;
	item->get_parent = _w_menuitem_get_parent;
	item->get_selection = _w_menuitem_get_selection;
	item->get_style = _w_menuitem_get_style;
	item->is_enabled = _w_menuitem_is_enabled;
	item->remove = _w_menuitem_remove;
	item->remove_item = _w_menuitem_remove_item;
	item->set_accelerator = _w_menuitem_set_accelerator;
	item->set_enabled = _w_menuitem_set_enabled;
	item->set_id = _w_menuitem_set_id;
	item->set_image = _w_menuitem_set_image;
	item->set_selection = _w_menuitem_set_selection;
	item->item.widgetdata.close = _w_widgetdata_close;
	item->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	item->item.widgetdata.copy = _w_menuitem_copy;
}
#endif
