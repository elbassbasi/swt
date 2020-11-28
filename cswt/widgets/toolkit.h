/*
 * toolkit.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef SRC_CUSTOM_WIDGETS_TOOLKIT_H_
#define SRC_CUSTOM_WIDGETS_TOOLKIT_H_
#include "ccanvas.h"
typedef struct c_toolkit {
	w_toolkit toolkit;
	struct _w_toolkit_class class_toolkit;
	w_toolkit *system_toolkit;
	w_theme *theme;
	struct _w_widget_class *classes[_W_CLASS_LAST];
    struct _w_shell_class class_shell;
    struct _w_composite_class class_composite;
	struct _w_canvas_class class_canvas;
	struct _w_ccanvas_class class_ccanvas;
    struct _w_button_class class_button;
    struct _w_tabfolder_class class_tabfolder;
    struct _w_tabitem_class class_tabitem;
    struct _w_tree_class class_tree;
    struct _w_treecolumn_class class_treecolumn;
    struct _w_treeitem_class class_treeitem;
} c_toolkit;
typedef w_theme* (*themes_creates_function)();
extern c_toolkit *ctoolkit;
CSWT_PUBLIC w_toolkit* w_toolkit_get_custom();
wresult w_toolkit_custom_set_theme(w_toolkit *custom,
		w_theme *theme);
CSWT_PUBLIC w_theme* w_toolkit_custom_create_theme(const char* name);
CSWT_PUBLIC themes_creates_function w_toolkit_custom_find_theme(const char* name);
void w_toolkit_custom_init(c_toolkit *toolkit);
void w_toolkit_custom_init_clazz(c_toolkit *toolkit);

#endif /* SRC_CUSTOM_WIDGETS_TOOLKIT_H_ */
