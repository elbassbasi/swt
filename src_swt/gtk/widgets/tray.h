/*
 * tray.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_TRAY_H_
#define SRC_GTK_WIDGETS_TRAY_H_
#ifdef __linux
#include "widget.h"
typedef struct _w_tray {
	struct _w_widget widget;
} _w_tray;

typedef struct _w_tray_reserved {
	_w_widget_reserved widget;

} _w_tray_reserved;
#define _W_TRAY(x) ((_w_tray*)x)
#define _W_TRAY_RESERVED(x) ((struct _w_tray_reserved*)x)
void _w_tray_class_init(struct _w_tray_class *clazz);

#endif
#endif /* SRC_GTK_WIDGETS_TRAY_H_ */
