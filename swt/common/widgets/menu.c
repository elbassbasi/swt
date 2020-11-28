/*
 * menu.c
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>
/*
 * menuitem
 */
wresult w_menuitem_insert(w_menuitem *parent, w_menuitem *item,
		const char *text, int style, int index) {
	w_widgetdata_close(W_WIDGETDATA(item));
	if (W_WIDGETDATA_CHECK(parent)) {
		return W_MENUITEM_GET_CLASS(parent)->insert(parent, item, text, style,
				index);
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
int w_menuitem_get_accelerator(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_accelerator(item);
	} else
		return 0;
}
wbool w_menuitem_get_enabled(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_enabled(item);
	} else
		return W_FALSE;
}
wresult w_menuitem_get_item(w_menuitem *item, unsigned int index, w_menuitem *sub_item) {
	w_widgetdata_close(W_WIDGETDATA(sub_item));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_item(item, index, sub_item);
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
int w_menuitem_get_item_count(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_item_count(item);
	} else
		return 0;
}
void w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	w_iterator_close(items);
	if (W_WIDGETDATA_CHECK(item)) {
		W_MENUITEM_GET_CLASS(item)->get_items(item, items);
	}
}
unsigned short w_menuitem_get_id(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_id(item);
	} else
		return 0;
}
wresult w_menuitem_get_image(w_menuitem *item, w_image *image) {
	w_image_dispose(image);
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_image(item, image);
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
wbool w_menuitem_get_parent(w_menuitem *item, w_menuitem *parent) {
	w_widgetdata_close(W_WIDGETDATA(parent));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_parent(item,parent);
	} else
		return W_FALSE;
}
wbool w_menuitem_get_selection(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_selection(item);
	} else
		return W_FALSE;
}
int w_menuitem_get_style(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->get_style(item);
	} else
		return 0;
}
wbool w_menuitem_is_enabled(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->is_enabled(item);
	} else
		return W_FALSE;
}
wresult w_menuitem_remove(w_menuitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->remove(item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_menuitem_remove_item(w_menuitem *item, int index) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->remove_item(item, index);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_menuitem_set_accelerator(w_menuitem *item, int accelerator) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->set_accelerator(item, accelerator);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_menuitem_set_enabled(w_menuitem *item, wbool enabled) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->set_enabled(item, enabled);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_menuitem_set_id(w_menuitem *item, unsigned short id) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->set_id(item, id);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_menuitem_set_image(w_menuitem *item, w_image *image) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_menuitem_set_selection(w_menuitem *item, wbool selected) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_MENUITEM_GET_CLASS(item)->set_selection(item, selected);
	} else
		return W_ERROR_NO_HANDLES;
}
/*
 * menu
 */
wresult w_menu_create(w_menu *menu, w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(menu), 0, W_WIDGET(parent), style,
			_W_CLASS_MENU, post_event);
}
w_menu* w_menu_new(w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return (w_menu*)_w_widget_new(0, W_WIDGET(parent), style, _W_CLASS_MENU, post_event);
}
void w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
	if (W_WIDGET_CHECK(menu)) {
		W_MENU_GET_CLASS(menu)->get_bounds(menu, bounds);
	} else {
		memset(bounds, 0, sizeof(w_rect));
	}
}
wresult w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	w_widgetdata_close(W_WIDGETDATA(rootitem));
	if (W_WIDGET_CHECK(menu)) {
		return W_MENU_GET_CLASS(menu)->get_root(menu, rootitem);
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
int w_menu_get_orientation(w_menu *menu) {
	if (W_WIDGET_CHECK(menu)) {
		return W_MENU_GET_CLASS(menu)->get_orientation(menu);
	} else {
		return 0;
	}
}
struct w_control* w_menu_get_parent(w_menu *menu) {
	if (W_WIDGET_CHECK(menu)) {
		return W_MENU_GET_CLASS(menu)->get_parent(menu);
	} else {
		return 0;
	}
}
wbool w_menu_get_visible(w_menu *menu) {
	if (W_WIDGET_CHECK(menu)) {
		return W_MENU_GET_CLASS(menu)->get_visible(menu);
	} else {
		return W_FALSE;
	}
}
wbool w_menu_is_visible(w_menu *menu) {
	if (W_WIDGET_CHECK(menu)) {
		return W_MENU_GET_CLASS(menu)->is_visible(menu);
	} else {
		return W_FALSE;
	}
}
void w_menu_set_location(w_menu *menu, w_point *location) {
	if (W_WIDGET_CHECK(menu)) {
		W_MENU_GET_CLASS(menu)->set_location(menu, location);
	}
}
void w_menu_set_orientation(w_menu *menu, int orientation) {
	if (W_WIDGET_CHECK(menu)) {
		W_MENU_GET_CLASS(menu)->set_orientation(menu, orientation);
	}
}
wresult w_menu_set_visible(w_menu *menu, wbool visible) {
	if (W_WIDGET_CHECK(menu)) {
		return W_MENU_GET_CLASS(menu)->set_visible(menu, visible);
	} else {
		return W_ERROR_NO_HANDLES;
	}
}
