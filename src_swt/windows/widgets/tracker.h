/*
 * tracker.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_TRACKER_H_
#define SRC_WIN32_WIDGETS_TRACKER_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "widget.h"
typedef struct _w_tracker {
	struct _w_widget widget;
}_w_tracker;

struct _w_tracker_reserved {
	struct _w_widget_reserved widget;

};
#define _W_TRACKER(x) ((_w_tracker*)x)
#define _W_TRACKER_RESERVED(x) ((struct _w_tracker_reserved*)x)
void _w_tracker_class_init (struct _w_tracker_class* clazz);


#endif
#endif /* SRC_WIN32_WIDGETS_TRACKER_H_ */
