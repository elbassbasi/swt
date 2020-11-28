/*
 * toolbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_TOOLBAR_H_
#define SRC_COCOA_CONTROLS_TOOLBAR_H_
#ifdef __APPLE__
#include "../widgets/composite.h"
typedef struct _w_toolitem {
	struct w_widgetdata widgetdata;
	w_toolbar *toolbar;
	int index;
} _w_toolitem;
#define _W_TOOLITEM(x) ((_w_toolitem*)x)
typedef struct _w_toolbar {
	struct _w_composite composite;
	w_imagelist *imagelist;
} _w_toolbar;

struct _w_toolbar_reserved {
	struct _w_composite_reserved composite;

};
#define _W_TOOLBAR(x) ((_w_toolbar*)x)
#define _W_TOOLBAR_RESERVED(x) ((struct _w_toolbar_reserved*)x)
void _w_toolbar_class_init(struct _w_toolbar_class *clazz);

#endif
#endif /* SRC_GTK_CONTROLS_TOOLBAR_H_ */
