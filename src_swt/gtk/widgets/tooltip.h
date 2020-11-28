/*
 * tooltip.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_TOOLTIP_H_
#define SRC_GTK_WIDGETS_TOOLTIP_H_
#ifdef __linux
#include "widget.h"

typedef struct _w_tooltip {
	struct _w_widget widget;
	struct _w_tooltip* next;
	struct _w_tooltip* prev;
}_w_tooltip;

struct _w_tooltip_reserved {
	struct _w_widget_reserved widget;

};
#define _W_TOOLTIP(x) ((_w_tooltip*)x)
#define _W_TOOLTIP_RESERVED(x) ((struct _w_tooltip_reserved*)x)
void _w_tooltip_class_init (struct _w_tooltip_class* clazz);

#endif
#endif /* SRC_GTK_WIDGETS_TOOLTIP_H_ */
