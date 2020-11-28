/*
 * expandbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_EXPANDBAR_H_
#define SRC_WIN32_CONTROLS_EXPANDBAR_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
typedef struct _w_expandbar {
	struct _w_composite composite;
}_w_expandbar;

struct _w_expandbar_reserved {
	struct _w_composite_reserved composite;

};
#define _W_EXPANDBAR(x) ((_w_expandbar*)x)
#define _W_EXPANDBAR_RESERVED(x) ((struct _w_expandbar_reserved*)x)
void _w_expandbar_class_init (struct _w_expandbar_class* clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_EXPANDBAR_H_ */
