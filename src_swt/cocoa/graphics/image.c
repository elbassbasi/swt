/*
 * image.c
 *
 *  Created on: 15 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "graphics.h"
void w_image_init(w_image *image) {
}
void w_image_dispose(w_image *image) {

}
wresult w_image_is_ok(w_image *image) {
}
wresult w_image_create_from_file(w_image *image, const char *file) {
	return W_TRUE;
}
wresult w_image_create_from_colors(w_image *image, w_size *size,
		w_color *colors, size_t length) {
}
wresult w_image_create_from_buffer(w_image *image, const void *buffer,
		size_t size) {
}
wresult w_image_get_size(w_image *image, w_size *size) {
	return W_TRUE;
}
wresult w_image_get_pixels(w_image *image, w_color *colors, size_t length) {
}
wresult w_image_save_to_file(w_image *image, const char *file,
const char *type) {
}
wresult w_image_save_to_buffer(w_image *image,w_alloc alloc,void* user_data, const char *type) {
}

wresult w_image_copy_area(w_image *image, w_rect *rect, w_image *destimage) {
}
wresult w_image_resize(w_image *image, w_size *size, w_image *destimage) {
}
wresult w_image_to_greyscale(w_image *image, w_image *destimage) {
}
wresult w_image_to_disabled(w_image *image, w_color background,
w_color shadow, w_image *destimage) {
}
void w_surface_init(w_surface *bitmap) {
}
wresult w_surface_is_ok(w_surface *bitmap) {
}
void w_surface_dispose(w_surface *bitmap) {
}
wresult w_surface_create(w_surface *bitmap, w_size *size) {
}
wresult w_surface_get_image(w_surface *bitmap, w_image *destimage) {
}
wresult w_surface_get_graphics(w_surface *bitmap, w_graphics *graphics) {
}

#endif
