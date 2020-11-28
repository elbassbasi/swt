/*
 * toolbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_TOOLBAR_H_
#define SRC_WIN32_CONTROLS_TOOLBAR_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
#define STATE_IGNORE_RESIZE (1 << (STATE_COMPOSITE_END + 0))
typedef struct _w_toolbar {
	struct _w_composite composite;
	w_imagelist* imagelist;
}_w_toolbar;

struct _w_toolbar_reserved {
	struct _w_composite_reserved composite;

};
#define _W_TOOLBAR(x) ((_w_toolbar*)x)
#define _W_TOOLBAR_RESERVED(x) ((struct _w_toolbar_reserved*)x)
typedef struct _w_toolitem {
	w_widgetdata widgetdata;
	w_toolbar *toolbar;
	int index;
} _w_toolitem;
#define _W_TOOLITEM(x) ((_w_toolitem*)x)
void _w_toolbar_class_init (struct _w_toolbar_class* clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_TOOLBAR_H_ */
