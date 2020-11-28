/*
 * sash.h
 *
 *  Created on: 8 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_SASH_H_
#define SRC_WIN32_CONTROLS_SASH_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/control.h"
typedef struct _w_sash {
	struct _w_control control;
	int startX, startY, lastX, lastY;
}_w_sash;

struct _w_sash_reserved {
	struct _w_control_reserved control;

};
#define _W_SASH(x) ((_w_sash*)x)
#define _W_SASH_RESERVED(x) ((struct _w_sash_reserved*)x)
#define STATE_SASH_DRAGGING (1 << 30)
void _w_sash_class_init (struct _w_sash_class* clazz);

#endif
#endif /* SRC_WIN32_CONTROLS_SASH_H_ */
