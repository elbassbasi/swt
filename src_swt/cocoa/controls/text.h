/*
 * text.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_TEXT_H_
#define SRC_COCOA_CONTROLS_TEXT_H_
#ifdef __APPLE__
#include "../widgets/scrollable.h"
typedef struct _w_text {
	struct _w_scrollable scrollable;
	char* message;
}_w_text;

struct _w_text_reserved {
	struct _w_scrollable_reserved scrollable;

};
#define _W_TEXT(x) ((_w_text*)x)
#define _W_TEXT_RESERVED(x) ((struct _w_text_reserved*)x)
void _w_text_class_init (struct _w_text_class* clazz);


#endif
#endif /* SRC_GTK_CONTROLS_TEXT_H_ */
