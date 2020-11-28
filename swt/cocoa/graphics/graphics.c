/*
 * graphics.c
 *
 *  Created on: 15 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "graphics.h"
#include "../widgets/toolkit.h"
#include <math.h>
unsigned char LINE_DOT[] = { 1, 1, 0 };
unsigned char LINE_DASH[] = { 3, 1, 0 };
unsigned char LINE_DASHDOT[] = { 3, 1, 1, 1, 0 };
unsigned char LINE_DASHDOTDOT[] = { 3, 1, 1, 1, 1, 1, 0 };
unsigned char LINE_DOT_ZERO[] = { 3, 3, 0 };
unsigned char LINE_DASH_ZERO[] = { 18, 6, 0 };
unsigned char LINE_DASHDOT_ZERO[] = { 9, 6, 3, 6, 0 };
unsigned char LINE_DASHDOTDOT_ZERO[] = { 9, 3, 3, 3, 3, 3, 0 };
void w_graphics_init(w_graphics *gc) {
#if USE_CAIRO
	_W_GRAPHICS(gc)->cairo = 0;
#else
#endif
}
void w_graphics_dispose(w_graphics *gc) {
}
wbool w_graphics_is_ok(w_graphics *gc) {
}
wresult _w_graphics_check(w_graphics *gc, int mask) {
}

wresult w_graphics_copy_area_to_image(w_graphics *gc, w_surface *bitmap, int x,
		int y) {
}
wresult w_graphics_copy_area(w_graphics *gc, int srcX, int srcY, int width,
		int height, int destX, int destY, wbool paint) {

}
wresult w_graphics_draw_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
}
wresult w_graphics_draw_focus(w_graphics *gc, w_rect *rect) {
}
wresult w_graphics_draw_image(w_graphics *gc, w_image *image,
w_rect *src, w_rect *dest,int state) {
}
wresult w_graphics_draw_line(w_graphics *gc, w_point *pt1, w_point *pt2) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
}
wresult w_graphics_draw_oval(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
}
wresult w_graphics_draw_path(w_graphics *gc, w_path *path) {
}
wresult w_graphics_draw_point(w_graphics *gc, w_point *pt) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
}
wresult w_graphics_draw_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
}
wresult w_graphics_draw_polyline(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
}
wresult w_graphics_draw_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
}
wresult w_graphics_draw_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
}
wresult w_graphics_draw_text(w_graphics *gc, const char *string, size_t length,
		w_rect *rect, int flags) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (string == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (*string == 0)
		return W_OK;
}
wresult w_graphics_fill_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
}
wresult w_graphics_fill_gradientrectangle(w_graphics *gc, w_rect *rect,
		wbool vertical) {

}
wresult w_graphics_fill_oval(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
}
wresult w_graphics_fill_path(w_graphics *gc, w_path *path) {
}
wresult w_graphics_fill_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
}
wresult w_graphics_fill_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
}
wresult w_graphics_fill_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
}
int w_graphics_get_advance_width(w_graphics *gc, int ch) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
}
wbool w_graphics_get_advanced(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return W_FALSE;
	// return data.cairo != 0;
}
int w_graphics_get_alpha(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0xFF;
	return _W_GRAPHICS(gc)->alpha;
}
int w_graphics_get_antialias(w_graphics *gc) {
}
w_color w_graphics_get_background(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return W_COLOR_WHITE;
	return _W_GRAPHICS(gc)->background;
}
w_pattern* w_graphics_get_background_pattern(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	return _W_GRAPHICS(gc)->backgroundPattern;
}
int w_graphics_get_char_width(w_graphics *gc, int ch) {
}
void w_graphics_getSize(w_graphics *gc, int *width, int *height) {
}
wresult w_graphics_get_clipping_rect(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
}
wresult w_graphics_get_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
}
int w_graphics_get_fill_rule(w_graphics *gc) {

}
w_font* w_graphics_get_font(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	return _W_GRAPHICS(gc)->font;
}
wresult w_graphics_get_font_metrics(w_graphics *gc,
		w_fontmetrics *fontMetrics) {
}
w_color w_graphics_get_foreground(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	return _W_GRAPHICS(gc)->foreground;
}
w_pattern* w_graphics_get_foreground_pattern(w_graphics *gc) {

}
int w_graphics_get_interpolation(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return W_DEFAULT;
}
w_line_attributes* w_graphics_get_line_attributes(w_graphics *gc) {
}
int w_graphics_get_line_cap(w_graphics *gc) {
}
w_line_dash* w_graphics_get_line_dash(w_graphics *gc) {
}
int w_graphics_get_line_join(w_graphics *gc) {
}
int w_graphics_get_line_style(w_graphics *gc) {
}
float w_graphics_get_line_width(w_graphics *gc) {
}
int w_graphics_get_style(w_graphics *gc) {
}
int w_graphics_get_text_antialias(w_graphics *gc) {
}
wresult w_graphics_get_transform(w_graphics *gc,
w_transform *Transform) {
}
wbool w_graphics_get_XOR_mode(w_graphics *gc) {
}
wbool w_graphics_is_clipped(w_graphics *gc) {
}
wresult w_graphics_set_advanced(w_graphics *gc, wbool advanced) {
}
wresult w_graphics_set_antialias(w_graphics *gc, int antialias) {
}
wresult w_graphics_set_alpha(w_graphics *gc, int alpha) {
}
wresult w_graphics_set_background(w_graphics *gc, w_color color) {
}
wresult w_graphics_set_background_pattern(w_graphics *gc, w_pattern *pattern) {
}
void w_graphics_reset_clipping(w_graphics *gc) {
}
wresult w_graphics_set_clipping_path(w_graphics *gc, w_path *path) {
}
wresult w_graphics_set_clipping_rect(w_graphics *gc, w_rect *r) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (r == 0)
		return W_ERROR_NULL_ARGUMENT;
	return W_TRUE;
}
wresult w_graphics_set_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	return W_TRUE;
}
wresult w_graphics_set_fill_rule(w_graphics *gc, int rule) {
}
wresult w_graphics_set_font(w_graphics *gc, w_font *font) {
	return W_TRUE;
}
wresult w_graphics_set_foreground(w_graphics *gc, w_color color) {
	return W_TRUE;
}
wresult w_graphics_set_foreground_pattern(w_graphics *gc, w_pattern *pattern) {
}
wresult w_graphics_set_interpolation(w_graphics *gc, int interpolation) {
}
wresult w_graphics_set_line_attributes(w_graphics *gc,
		w_line_attributes *attributes) {
}
wresult w_graphics_set_line_cap(w_graphics *gc, int cap) {
	return W_TRUE;
}
wresult w_graphics_set_line_dash(w_graphics *gc, int *dashes, int length) {
	return W_TRUE;
}
wresult w_graphics_set_line_join(w_graphics *gc, int join) {
	return W_TRUE;
}
wresult w_graphics_set_line_style(w_graphics *gc, int lineStyle) {
	return W_TRUE;
}
wresult w_graphics_set_line_width(w_graphics *gc, float lineWidth) {
	return W_TRUE;
}
wresult w_graphics_set_XOR_mode(w_graphics *gc, wbool _xor) {
}
wresult w_graphics_set_text_antialias(w_graphics *gc, int antialias) {
}
wresult w_graphics_set_transform(w_graphics *gc, w_transform *Transform) {
}
wresult w_graphics_text_extent(w_graphics *gc, const char *string, int length,
		w_size *size, int flags) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	return W_TRUE;
}
#endif
