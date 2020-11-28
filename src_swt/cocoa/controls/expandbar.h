/*
 * expandbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_EXPANDBAR_H_
#define SRC_COCOA_CONTROLS_EXPANDBAR_H_
#ifdef __APPLE__
#include "../widgets/composite.h"
typedef struct _w_expandbar {
	struct _w_composite composite;
}_w_expandbar;
typedef struct _w_expanditem {
	w_widgetdata widgetdata;
	w_expandbar* parent;
} _w_expanditem;
#define _W_EXPANDITEM(x) ((_w_expanditem*)x)
struct _w_expandbar_reserved {
	struct _w_composite_reserved composite;

};
#define _W_EXPANDBAR(x) ((_w_expandbar*)x)
#define _W_EXPANDBAR_RESERVED(x) ((struct _w_expandbar_reserved*)x)
void _w_expandbar_class_init (struct _w_expandbar_class* clazz);
#endif
#endif /* SRC_GTK_CONTROLS_EXPANDBAR_H_ */
