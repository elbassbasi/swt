/*
 * datatime.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_DATETIME_H_
#define SRC_COCOA_CONTROLS_DATETIME_H_
#ifdef __APPLE__
#include "../widgets/composite.h"
typedef struct _w_datetime {
	struct _w_composite composite;
}_w_datetime;

struct _w_datetime_reserved {
	struct _w_composite_reserved composite;

};
#define _W_DATETIME(x) ((_w_datetime*)x)
#define _W_DATETIME_RESERVED(x) ((struct _w_datetime_reserved*)x)
void _w_datetime_class_init (struct _w_datetime_class* clazz);
#endif
#endif /* SRC_GTK_CONTROLS_DATETIME_H_ */
