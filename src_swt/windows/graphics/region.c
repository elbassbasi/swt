/*
 * region.c
 *
 *  Created on: 1 avr. 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
void w_region_init(w_region *region) {
}
void w_region_dispose(w_region *region) {
}
wresult w_region_create(w_region *region) {
}
wresult w_region_create_rectangle(w_region* region,w_rect* rect) {
}
wresult w_region_add_points(w_region *region, w_point *points, size_t count) {
}
wresult w_region_add_rectangle(w_region *region, w_rect *rect) {
}
wresult w_region_add_region(w_region *region, w_region *_region) {
}
wresult w_region_contains(w_region *region, w_point *pt) {
}
wresult w_region_get_bounds(w_region *region, w_rect *bounds) {
}
wresult w_region_intersect_rect(w_region *region, w_rect *rect) {
}
wresult w_region_intersect_region(w_region *region, w_region *_region) {
}
wresult w_region_intersects(w_region* region,w_rect* rect) {
}
wresult w_region_isok(w_region *region) {
}
wresult w_region_isempty(w_region *region) {
}
wresult w_region_subtract_points(w_region *region, w_point *points,
		size_t count) {
}
wresult w_region_subtract_rectangle(w_region *region, w_rect *rect) {
}
wresult w_region_subtract_region(w_region *region, w_region *_region) {
}
wresult w_region_translate(w_region *region, w_point *pt) {
}
#endif
