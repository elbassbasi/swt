/*
 * taskbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_TASKBAR_H_
#define SRC_WIN32_WIDGETS_TASKBAR_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "widget.h"
typedef struct _w_taskbar {
	struct _w_widget widget;
}_w_taskbar;

struct _w_taskbar_reserved {
	struct _w_widget_reserved widget;

};
#define _W_TASKBAR(x) ((_w_taskbar*)x)
#define _W_TASKBAR_RESERVED(x) ((struct _w_taskbar_reserved*)x)
void _w_taskbar_class_init (struct _w_taskbar_class* clazz);



#endif
#endif /* SRC_WIN32_WIDGETS_TASKBAR_H_ */
