/*
 * cursor.c
 *
 *  Created on: 1 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
void w_cursor_init(w_cursor *cursor) {
	_W_CURSOR(cursor)->handle = 0;
}
void w_cursor_dispose(w_cursor *cursor) {
	if (_W_CURSOR(cursor)->handle != 0) {
		if (_W_CURSOR(cursor)->flags & W_CURSOR_IS_ICON) {
			DestroyIcon((HICON) _W_CURSOR(cursor)->handle);
		} else {
			/*
			 * The MSDN states that one should not destroy a shared
			 * handle, that is, one obtained from LoadCursor.
			 * However, it does not appear to do any harm, so rather
			 * than keep track of how a handle was created, we just
			 * destroy them all. If this causes problems in the future,
			 * put the flag back in.
			 */
			DestroyCursor(_W_CURSOR(cursor)->handle);
		}
		_W_CURSOR(cursor)->handle = 0;
	}
}
wresult w_cursor_create(w_cursor *cursor, w_image *source, int hotspotX,
		int hotspotY) {
	if (cursor == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_cursor_dispose(cursor);
	if (source == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (w_image_is_ok(source) <= 0)
		return W_ERROR_INVALID_ARGUMENT;
	/* Check the hotspots */
	w_size sz;
	w_image_get_size(source, &sz);
	if (hotspotX >= sz.width || hotspotX < 0 || hotspotY >= sz.height
			|| hotspotY < 0) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	HBITMAP hBitmap = _w_image_get_HBITMAP(source);
	/* Create the icon */
	ICONINFO info;
	info.fIcon = FALSE;
	info.hbmColor = hBitmap;
	info.hbmMask = 0;
	info.xHotspot = hotspotX;
	info.yHotspot = hotspotY;
	_W_CURSOR(cursor)->handle = (HCURSOR) CreateIconIndirect(&info);
	if (_W_CURSOR(cursor)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_CURSOR(cursor)->flags = W_CURSOR_IS_ICON;
	return W_TRUE;
}
#endif
