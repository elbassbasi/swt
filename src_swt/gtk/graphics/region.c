/*
 * region.c
 *
 *  Created on: 1 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "graphics.h"
void w_region_init(w_region *region) {
	_W_REGION(region)->handle = 0;
}
void w_region_dispose(w_region *region) {
	if (_W_REGION(region)->handle != 0) {
		cairo_region_destroy(_W_REGION(region)->handle);
		_W_REGION(region)->handle = 0;
	}
}
wresult w_region_is_ok(w_region *region) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_FALSE;
	return W_TRUE;
}
wresult w_region_create(w_region *region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle != 0) {
		cairo_region_destroy(_W_REGION(region)->handle);
	}
	_W_REGION(region)->handle = cairo_region_create();
	return W_TRUE;
}
void _gdk_region_get_rectangles(cairo_region_t *region,
		GdkRectangle **rectangles, int *n_rectangles) {
#if GTK3
	int num = cairo_region_num_rectangles(region);
	if (n_rectangles != 0)
		*n_rectangles = num;
	GdkRectangle *_rectangles = g_malloc(sizeof(GdkRectangle) * num);
	if (_rectangles != 0) {
		for (int n = 0; n < num; n++) {
			cairo_region_get_rectangle(region, n, &_rectangles[n]);
		}
	}
	*rectangles = _rectangles;
#else
	gdk_region_get_rectangles (region, rectangles, n_rectangles);
#endif
}
wresult w_region_create_rectangle(w_region *region, w_rect *rect) {
	if (region == 0 || rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle != 0) {
		cairo_region_destroy(_W_REGION(region)->handle);
	}
	cairo_rectangle_int_t r;
	r.x = rect->x;
	r.y = rect->y;
	r.width = rect->width;
	r.height = rect->height;
	_W_REGION(region)->handle = cairo_region_create_rectangle(&r);
	return W_TRUE;
}
wresult w_region_create_ellipse(w_region* region,w_rect* rect){
	if (region == 0 || rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle != 0) {
		cairo_region_destroy(_W_REGION(region)->handle);
	}
	//_W_REGION(region)->handle = cairo_region_create_rectangle(&r);
	return W_TRUE;
}
#define GDK_EVEN_ODD_RULE 0
cairo_region_t* gdk_region_polygon(int *pointArray, int npoints,
		int fill_rule) {
#if GTK3
	int minX = pointArray[0], maxX = minX;
	int minY = pointArray[1], maxY = minY;
	int count = npoints * 2;
	for (int i = 2; i < count; i += 2) {
		int x = pointArray[i], y = pointArray[i + 1];
		if (x < minX)
			minX = x;
		if (x > maxX)
			maxX = x;
		if (y < minY)
			minY = y;
		if (y > maxY)
			maxY = y;
	}
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,
			maxX - minX, maxY - minY);
	if (surface == 0)
		return 0;
	cairo_t *cairo = cairo_create(surface);
	if (cairo == 0)
		return 0;
	cairo_move_to(cairo, pointArray[0] - minX, pointArray[1] - minY);
	for (int i = 2; i < count; i += 2) {
		cairo_line_to(cairo, pointArray[i] - minX, pointArray[i + 1] - minY);
	}
	cairo_close_path(cairo);
	cairo_set_source_rgb(cairo, 1, 1, 1);
	cairo_fill_rule_t cairo_rule = CAIRO_FILL_RULE_WINDING;
	if (fill_rule == GDK_EVEN_ODD_RULE) {
		cairo_rule = CAIRO_FILL_RULE_EVEN_ODD;
	}
	cairo_set_fill_rule(cairo, cairo_rule);
	cairo_fill(cairo);
	cairo_destroy(cairo);
	cairo_region_t *polyRgn = gdk_cairo_region_create_from_surface(surface);
	cairo_region_translate(polyRgn, minX, minY);
	cairo_surface_destroy(surface);
	return polyRgn;
#else
		return gdk_region_polygon(pointArray, npoints, fill_rule);
#endif
}

wresult w_region_add_points(w_region *region, w_point *points, size_t count) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (points == 0)
		return W_ERROR_NULL_ARGUMENT;
	/*
	 * Bug in GTK. If gdk_region_polygon() is called with one point,
	 * it segment faults. The fix is to make sure that it is called
	 * with enough points for a polygon.
	 */
	if (count < 3)
		return W_FALSE;
	cairo_region_t *polyRgn = gdk_region_polygon((int*) points, count,
	GDK_EVEN_ODD_RULE);
	cairo_region_union(_W_REGION(region)->handle, polyRgn);
	cairo_region_destroy(polyRgn);
	return W_TRUE;
}
wresult w_region_add_rectangle(w_region *region, w_rect *rect) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkRectangle gdkRect;
	gdkRect.x = rect->x;
	gdkRect.y = rect->y;
	gdkRect.width = rect->width;
	gdkRect.height = rect->height;
	cairo_region_union_rectangle(_W_REGION(region)->handle, &gdkRect);
	return W_TRUE;
}
wresult w_region_add_region(w_region *region, w_region *_region) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_region == 0 || _W_REGION(_region)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	cairo_region_union(_W_REGION(region)->handle, _W_REGION(_region)->handle);
	return W_TRUE;
}
wresult w_region_contains(w_region *region, w_point *pt) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_FALSE;
	return cairo_region_contains_point(_W_REGION(region)->handle, pt->x, pt->y);
}
wresult w_region_get_bounds(w_region *region, w_rect *bounds) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	GdkRectangle gdkRect;
	cairo_region_get_extents(_W_REGION(region)->handle, &gdkRect);
	bounds->x = gdkRect.x;
	bounds->y = gdkRect.y;
	bounds->width = gdkRect.width;
	bounds->height = gdkRect.height;
	return W_TRUE;
}
wresult w_region_intersect_rect(w_region *region, w_rect *rect) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkRectangle gdkRect;
	gdkRect.x = rect->x;
	gdkRect.y = rect->y;
	gdkRect.width = rect->width;
	gdkRect.height = rect->height;
	cairo_region_t *rectRgn = cairo_region_create_rectangle(&gdkRect);
	cairo_region_intersect(_W_REGION(region)->handle, rectRgn);
	cairo_region_destroy(rectRgn);
	return W_TRUE;
}
wresult w_region_intersect_region(w_region *region, w_region *_region) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_region == 0 || _W_REGION(_region)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	cairo_region_intersect(_W_REGION(region)->handle,
	_W_REGION(_region)->handle);
	return W_TRUE;
}
wresult w_region_intersects(w_region *region, w_rect *rect) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_FALSE;
	if (rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkRectangle gdkRect;
	gdkRect.x = rect->x;
	gdkRect.y = rect->y;
	gdkRect.width = rect->width;
	gdkRect.height = rect->height;
	return cairo_region_contains_rectangle(_W_REGION(region)->handle, &gdkRect)
			!= CAIRO_REGION_OVERLAP_IN;
}
wresult w_region_is_empty(w_region *region) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_TRUE;
	return cairo_region_is_empty(_W_REGION(region)->handle);
}
wresult w_region_subtract_points(w_region *region, w_point *points,
		size_t count) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (points == 0)
		return W_ERROR_NULL_ARGUMENT;
	/**
	 * Bug in GTK. If gdk_region_polygon() is called with one point,
	 * it segment faults. The fix is to make sure that it is called
	 * with enough points for a polygon.
	 */
	if (count < 3)
		return W_FALSE;
	cairo_region_t *polyRgn = gdk_region_polygon((int*) points, count,
	GDK_EVEN_ODD_RULE);
	cairo_region_subtract(_W_REGION(region)->handle, polyRgn);
	cairo_region_destroy(polyRgn);
	return W_TRUE;
}
wresult w_region_subtract_rectangle(w_region *region, w_rect *rect) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkRectangle gdkRect;
	gdkRect.x = rect->x;
	gdkRect.y = rect->y;
	gdkRect.width = rect->width;
	gdkRect.height = rect->height;
	cairo_region_t *rectRgn = cairo_region_create_rectangle(&gdkRect);
	cairo_region_subtract(_W_REGION(region)->handle, rectRgn);
	cairo_region_destroy(rectRgn);
	return W_TRUE;
}
wresult w_region_subtract_region(w_region *region, w_region *_region) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_region == 0 || _W_REGION(_region)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	cairo_region_subtract(_W_REGION(region)->handle,
	_W_REGION(_region)->handle);
	return W_TRUE;
}
wresult w_region_translate(w_region *region, w_point *pt) {
	if (region == 0 || _W_REGION(region)->handle == 0)
		return W_ERROR_NO_HANDLES;
	cairo_region_translate(_W_REGION(region)->handle, pt->x, pt->y);
	return W_TRUE;
}
#endif
