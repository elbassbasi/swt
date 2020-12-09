/*
 * imagelist.c
 *
 *  Created on: 19 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "graphics.h"
void w_imagelist_init(w_imagelist *imagelist) {
}
void w_imagelist_dispose(w_imagelist *imagelist) {
}
wresult w_imagelist_is_ok(w_imagelist *imagelist) {
}
wresult w_imagelist_create(w_imagelist *imagelist, w_size *size,
		int initialCount) {
}
int w_imagelist_add(w_imagelist *imagelist, w_image *image) {
}
wresult w_imagelist_replace(w_imagelist *imagelist, int index,
		w_image *image) {
}
wresult w_imagelist_remove(w_imagelist *imagelist, int index) {
}
wresult w_imagelist_remove_all(w_imagelist *imagelist) {
}
wresult w_imagelist_get_image(w_imagelist *imagelist, int index, int copy,
		w_image *image) {
}
wresult w_imagelist_get_size(w_imagelist *imagelist, w_size *size) {
}
int w_imagelist_get_count(w_imagelist *imagelist) {
}
wresult w_imagelist_draw(w_imagelist *imagelist, struct w_graphics *graphics,
		int index, w_point *pt, int flags) {
}
#endif
