/*
 * menu.h
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_MENU_H_
#define SRC_WIN32_WIDGETS_MENU_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "widget.h"
typedef struct _w_menu {
	struct _w_widget widget;
	w_control* parent;
	HMENU handle;
	int x;
	int y;
} _w_menu;
typedef struct _w_menuitem {
	w_widgetdata widgetdata;
	w_menu* menu;
	HMENU parentItem;
	int index;
} _w_menuitem;

struct _w_menu_reserved {
	struct _w_widget_reserved widget;

};
#define _W_MENU(x) ((_w_menu*)x)
#define _W_MENU_RESERVED(x) ((struct _w_menu_reserved*)x)
#define _W_MENUITEM(x) ((_w_menuitem*)x)
w_menu* _w_menu_get_top(HMENU menu);
void _w_menu_class_init(struct _w_menu_class *clazz);
#endif
#endif /* SRC_WIN32_WIDGETS_MENU_H_ */
