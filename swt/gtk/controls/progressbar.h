/*
 * progressbar.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_CONTROLS_PROGRESSBAR_H_
#define SRC_GTK_CONTROLS_PROGRESSBAR_H_
#ifdef __linux
#include "../widgets/control.h"

typedef struct _w_progressbar {
	struct _w_control control;
	int maximum;
	int selection;
	guint timerId;
}_w_progressbar;

struct _w_progressbar_reserved {
	struct _w_control_reserved control;

};
#define _W_PROGRESSBAR(x) ((_w_progressbar*)x)
#define _W_PROGRESSBAR_RESERVED(x) ((struct _w_progressbar_reserved*)x)
void _w_progressbar_class_init (struct _w_progressbar_class* clazz);


#endif
#endif /* SRC_GTK_CONTROLS_PROGRESSBAR_H_ */
