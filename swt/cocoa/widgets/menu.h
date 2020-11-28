/*
 * menu.h
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_WIDGETS_MENU_H_
#define SRC_COCOA_WIDGETS_MENU_H_

#ifdef __APPLE__
#include "widget.h"
typedef struct _w_menu {
	_w_widget widget;
	w_control* parent;
	int x;
	int y;
} _w_menu;
typedef struct _w_menuitem {
	struct w_widgetdata widgetdata;
	w_menu* menu;
} _w_menuitem;

struct _w_menu_reserved {
	struct _w_widget_reserved widget;

};
#define _W_MENU(x) ((_w_menu*)x)
#define _W_MENU_RESERVED(x) ((struct _w_menu_reserved*)x)
#define _W_MENUITEM(x) ((_w_menuitem*)x)
wresult _w_menuitem_remove(w_menuitem *item);
void _w_menu_class_init(struct _w_menu_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_MENU_H_ */
