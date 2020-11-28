/*
 * menu.h
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_MENU_H_
#define SRC_GTK_WIDGETS_MENU_H_

#ifdef __linux
#include "widget.h"
typedef struct _w_menu {
	struct _w_widget widget;
	w_control* parent;
	int x;
	int y;
} _w_menu;
typedef struct _w_menuitem {
	struct w_widgetdata widgetdata;
	w_menu* menu;
	GtkWidget* widget;
} _w_menuitem;

struct _w_menu_reserved {
	struct _w_widget_reserved widget;
	int signal_activate_id;
	int signal_select_id;

};
#define _W_MENU(x) ((_w_menu*)x)
#define _W_MENU_RESERVED(x) ((struct _w_menu_reserved*)x)
#define _W_MENUITEM(x) ((_w_menuitem*)x)
wresult _w_menuitem_set_text_0(GtkWidget *widget, const char *text);
wresult _w_menuitem_remove(w_menuitem *item);
void _w_menu_set_orientation_0(w_menu *menu, int orientation);
void _w_menu_class_init(struct _w_menu_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_MENU_H_ */
