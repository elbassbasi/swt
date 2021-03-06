/*
 * group.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_GROUP_H_
#define SRC_COCOA_CONTROLS_GROUP_H_
#ifdef __APPLE__
#include "../widgets/composite.h"
typedef struct _w_group {
	struct _w_composite composite;
}_w_group;

struct _w_group_reserved {
	struct _w_composite_reserved composite;

};
#define _W_GROUP(x) ((_w_group*)x)
#define _W_GROUP_RESERVED(x) ((struct _w_group_reserved*)x)
void _w_group_class_init (struct _w_group_class* clazz);
#endif
#endif /* SRC_GTK_CONTROLS_GROUP_H_ */
