/*
 * cursor.c
 *
 *  Created on: 1 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "graphics.h"
void w_cursor_init(w_cursor *cursor) {
    _W_CURSOR(cursor)->handle = 0;
}
void w_cursor_dispose(w_cursor *cursor) {
    if(cursor !=0 && _W_CURSOR(cursor)->handle != 0){
#if GTK3
            g_object_unref (_W_CURSOR(cursor)->handle);
#else
            gdk_cursor_unref(_W_CURSOR(cursor)->handle);
#endif
        _W_CURSOR(cursor)->handle = 0;
    }
}
wresult w_cursor_create(w_cursor *cursor, w_image *source,
		int hotspotX, int hotspotY) {
    w_cursor_dispose(cursor);
	if (!w_image_is_ok(source)) return W_ERROR_NULL_ARGUMENT;
    _W_CURSOR(cursor)->handle = gdk_cursor_new_from_pixbuf(gdk_display_get_default(),_W_IMAGE(source)->pixbuf,hotspotX,hotspotY);
    return W_TRUE;
}
#endif
