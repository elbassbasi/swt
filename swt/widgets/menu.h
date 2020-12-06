/*
 * menu.h
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_WIDGETS_MENU_H_
#define SWT_WIDGETS_MENU_H_
#include "widget.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_menu {
	w_widget widget;
	void *handle[0x10];
} w_menu;
typedef struct w_menuitem {
	w_widgetdata widgetdata;
	void *handle[6];
} w_menuitem;
struct _w_menuitem_class {
	struct _w_item_class item;
	wresult (*insert)(w_menuitem *parent, w_menuitem *item, const char *text,
			int style, int index);
	int (*get_accelerator)(w_menuitem *item);
	wresult (*get_enabled)(w_menuitem *item);
	wresult (*get_item)(w_menuitem *item, unsigned int index,
			w_menuitem *sub_item);
	int (*get_item_count)(w_menuitem *item);
	void (*get_items)(w_menuitem *item, w_iterator *items);
	wushort (*get_id)(w_menuitem *item);
	wresult (*get_image)(w_menuitem *item, w_image *image);
	wresult (*get_parent)(w_menuitem *item, w_menuitem *parent);
	wresult (*get_selection)(w_menuitem *item);
	int (*get_style)(w_menuitem *item);
	wresult (*is_enabled)(w_menuitem *item);
	wresult (*remove)(w_menuitem *item);
	wresult (*remove_item)(w_menuitem *item, int index);
	wresult (*set_accelerator)(w_menuitem *item, int accelerator);
	wresult (*set_enabled)(w_menuitem *item, int enabled);
	wresult (*set_id)(w_menuitem *item, wushort id);
	wresult (*set_image)(w_menuitem *item, w_image *image);
	wresult (*set_selection)(w_menuitem *item, int selected);
};
struct _w_menu_class {
	struct _w_widget_class widget;
	struct _w_menuitem_class *class_menuitem;
	void (*get_bounds)(w_menu *menu, w_rect *bounds);
	wresult (*get_root)(w_menu *menu, w_menuitem *rootitem);
	int (*get_orientation)(w_menu *menu);
	struct w_control* (*get_parent)(w_menu *menu);
	wresult (*get_visible)(w_menu *menu);
	wresult (*is_visible)(w_menu *menu);
	void (*set_location)(w_menu *menu, w_point *location);
	void (*set_orientation)(w_menu *menu, int orientation);
	wresult (*set_visible)(w_menu *menu, int visible);
};
/*
 * menuitem
 */
SWT_PUBLIC wresult w_menuitem_insert(w_menuitem *parent, w_menuitem *item,
		const char *text, int style, int index);
SWT_PUBLIC int w_menuitem_get_accelerator(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_get_enabled(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_get_item(w_menuitem *item, unsigned int index,
		w_menuitem *sub_item);
SWT_PUBLIC int w_menuitem_get_item_count(w_menuitem *item);
SWT_PUBLIC void w_menuitem_get_items(w_menuitem *item, w_iterator *items);
SWT_PUBLIC wushort w_menuitem_get_id(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_get_image(w_menuitem *item, w_image *image);
SWT_PUBLIC wresult w_menuitem_get_parent(w_menuitem *item, w_menuitem *parent);
SWT_PUBLIC wresult w_menuitem_get_selection(w_menuitem *item);
SWT_PUBLIC int w_menuitem_get_style(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_is_enabled(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_remove(w_menuitem *item);
SWT_PUBLIC wresult w_menuitem_remove_item(w_menuitem *item, int index);
SWT_PUBLIC wresult w_menuitem_set_accelerator(w_menuitem *item, int accelerator);
SWT_PUBLIC wresult w_menuitem_set_enabled(w_menuitem *item, int enabled);
SWT_PUBLIC wresult w_menuitem_set_id(w_menuitem *item, wushort id);
SWT_PUBLIC wresult w_menuitem_set_image(w_menuitem *item, w_image *image);
SWT_PUBLIC wresult w_menuitem_set_selection(w_menuitem *item, int selected);
/*
 * menu
 */
SWT_PUBLIC wresult w_menu_create(w_menu *menu, struct w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_menu* w_menu_new(struct w_control *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC void w_menu_get_bounds(w_menu *menu, w_rect *bounds);
SWT_PUBLIC wresult w_menu_get_root(w_menu *menu, w_menuitem *rootitem);
SWT_PUBLIC int w_menu_get_orientation(w_menu *menu);
SWT_PUBLIC struct w_control* w_menu_get_parent(w_menu *menu);
SWT_PUBLIC wresult w_menu_get_visible(w_menu *menu);
SWT_PUBLIC wresult w_menu_is_visible(w_menu *menu);
SWT_PUBLIC void w_menu_set_location(w_menu *menu, w_point *location);
SWT_PUBLIC void w_menu_set_orientation(w_menu *menu, int orientation);
SWT_PUBLIC wresult w_menu_set_visible(w_menu *menu, int visible);

#define W_MENU(x) ((w_menu*)x)
#define W_MENU_CLASS(x) ((struct _w_menu_class*)x)
#define W_MENU_GET_CLASS(x) ((struct _w_menu_class*)W_WIDGET_GET_CLASS(x))

#define W_MENUITEM(x) ((w_menuitem*)x)
#define W_MENUITEM_CLASS(x) ((struct _w_menuitem_class*)x)
#define W_MENUITEM_GET_CLASS(x) ((struct _w_menuitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif

#endif /* SWT_WIDGETS_MENU_H_ */
