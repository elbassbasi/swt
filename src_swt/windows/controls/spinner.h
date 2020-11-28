/*
 * spinner.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_SPINNER_H_
#define SRC_WIN32_CONTROLS_SPINNER_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
typedef struct _w_spinner {
	struct _w_composite composite;
	HWND hwndText;
	HWND hwndUpDown;
	int pageIncrement;
	int digits;
}_w_spinner;

struct _w_spinner_reserved {
	struct _w_composite_reserved composite;

};
#define _W_SPINNER(x) ((_w_spinner*)x)
#define _W_SPINNER_RESERVED(x) ((struct _w_spinner_reserved*)x)
void _w_spinner_class_init (struct _w_spinner_class* clazz);


#endif
#endif /* SRC_WIN32_CONTROLS_SPINNER_H_ */
