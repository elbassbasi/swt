/*
 * combo.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_COMBO_H_
#define SRC_WIN32_CONTROLS_COMBO_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
typedef struct _w_combo {
	struct _w_composite composite;
}_w_combo;

struct _w_combo_reserved {
	struct _w_composite_reserved composite;

};
#define _W_COMBO(x) ((_w_combo*)x)
#define _W_COMBO_RESERVED(x) ((struct _w_combo_reserved*)x)
void _w_combo_class_init (struct _w_combo_class* clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_COMBO_H_ */
