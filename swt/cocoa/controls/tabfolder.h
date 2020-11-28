/*
 * tabview.h
 *
 *  Created on: 4 mai 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_TABFOLDER_H_
#define SRC_COCOA_CONTROLS_TABFOLDER_H_

#ifdef __APPLE__
#include "../widgets/composite.h"
typedef struct _w_tabitem {
	struct w_widgetdata widgetdata;
	w_tabfolder* tab;
	int index;
} _w_tabitem;
#define _W_TABITEM(x) ((_w_tabitem*)x)
typedef struct _w_tabfolder {
	struct _w_composite composite;
	//GtkWidget* children;
	w_imagelist* imagelist;
} _w_tabfolder;
#define _W_TABFOLDER(x) ((_w_tabfolder*)x)
struct _w_tabfolder_reserved {
	struct _w_composite_reserved composite;
	int signal_switch_page_id;

};
#define _W_TABFOLDER_RESERVED(x) ((struct _w_tabfolder_reserved*)x)
#define _W_TABFOLDER_GET_RESERVED(x) ((struct _w_tabfolder_reserved*)_w_widget_get_reserved(W_WIDGET(x)))
void _w_tabfolder_class_init(struct _w_tabfolder_class *clazz);
#endif
#endif /* SRC_GTK_CONTROLS_TABFOLDER_H_ */
