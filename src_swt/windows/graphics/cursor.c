/*
 * cursor.c
 *
 *  Created on: 1 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
void w_cursor_init(w_cursor *cursor){}
void w_cursor_dispose(w_cursor *cursor){}
wresult w_cursor_create(w_cursor *cursor, w_image *source,
		int hotspotX, int hotspotY){}
#endif
