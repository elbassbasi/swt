/*
 * coolbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_CONTROLS_COOLBAR_H_
#define SRC_GTK_CONTROLS_COOLBAR_H_
#ifdef __linux
#include "../widgets/composite.h"
typedef struct __w_coolitem {
	w_rect itemBounds;
	int preferredWidth;
	int preferredHeight;
	int minimumWidth;
	int minimumHeight;
	int requestedWidth;
	unsigned ideal:1;
	unsigned wrap;
	unsigned newrow;
	w_control* control;
} __w_coolitem;
typedef struct _w_coolitems {
	size_t alloc;
	size_t count;
	__w_coolitem items[0];
} _w_coolitems;
typedef struct _w_coolbar {
	struct _w_composite composite;
	_w_coolitems *items;
	w_cursor *hoverCursor;
	w_cursor *dragCursor;
} _w_coolbar;

struct _w_coolbar_reserved {
	struct _w_composite_reserved composite;

};
#define _W_COOLBAR(x) ((_w_coolbar*)x)
#define _W_COOLBAR_RESERVED(x) ((struct _w_coolbar_reserved*)x)

#define _W_COOLBAR_GROW 4

typedef struct _w_coolitem {
	w_widgetdata widgetdata;
	w_coolbar *coolbar;
	int index;
} _w_coolitem;
#define _W_COOLITEM(x) ((_w_coolitem*)x)
void _w_coolbar_class_init(struct _w_coolbar_class *clazz);
#endif
#endif /* SRC_GTK_CONTROLS_COOLBAR_H_ */
