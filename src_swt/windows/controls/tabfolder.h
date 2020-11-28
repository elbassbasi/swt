/*
 * tabview.h
 *
 *  Created on: 4 mai 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_TABFOLDER_H_
#define SRC_WIN32_CONTROLS_TABFOLDER_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
#define STATE_TABFOLDER_IS_SELECTED (1 << 31)
typedef struct _w_tabitem {
	w_widgetdata widgetdata;
	w_tabfolder *tabfolder;
	int index;
} _w_tabitem;
#define _W_TABITEM(x) ((_w_tabitem*)x)
typedef struct _w_tabfolder {
	struct _w_composite composite;
	w_imagelist* imagelist;
	int lastindex;
} _w_tabfolder;
#define _W_TABFOLDER(x) ((_w_tabfolder*)x)
struct _w_tabfolder_reserved {
	struct _w_composite_reserved composite;

};
#define _W_TABFOLDER_RESERVED(x) ((struct _w_tabfolder_reserved*)x)
#define W_TABITEM_CLOSE_WIDTH 8
#define W_TABITEM_CLOSE_HEIGTH 8
void _w_tabfolder_class_init(struct _w_tabfolder_class *clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_TABFOLDER_H_ */
