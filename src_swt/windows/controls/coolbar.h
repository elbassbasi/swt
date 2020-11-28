/*
 * coolbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_COOLBAR_H_
#define SRC_WIN32_CONTROLS_COOLBAR_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
#define STATE_IGNORE_RESIZE (1 << (STATE_COMPOSITE_END + 0))


#define SEPARATOR_WIDTH 2
#define MAX_WIDTH 0x7FFF
#define DEFAULT_COOLBAR_WIDTH 0
#define DEFAULT_COOLBAR_HEIGHT 0

typedef struct _w_coolbar {
	struct _w_composite composite;
} _w_coolbar;

struct _w_coolbar_reserved {
	struct _w_composite_reserved composite;

};
#define _W_COOLBAR(x) ((_w_coolbar*)x)
#define _W_COOLBAR_RESERVED(x) ((struct _w_coolbar_reserved*)x)

typedef struct _w_coolitem {
	w_widgetdata widgetdata;
	w_coolbar *coolbar;
	int index;
} _w_coolitem;
#define _W_COOLITEM(x) ((_w_coolitem*)x)

void _w_coolbar_class_init(struct _w_coolbar_class *clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_COOLBAR_H_ */
