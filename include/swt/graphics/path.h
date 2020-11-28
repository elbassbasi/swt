/*
 * path.h
 *
 *  Created on: 13 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_GRAPHICS_PATH_H_
#define SWT_GRAPHICS_PATH_H_
#include "rect.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_pathdata {
	int type;
	float pt[6];
} w_pathdata;
typedef struct w_path {
	void *handle[2];
} w_path;
#define W_PATH(x) ((w_path*)x)
void w_path_init(w_path *path);
void w_path_dispose(w_path *path);
wresult w_path_create(w_path *path);
wresult w_path_create_from_path(w_path *path, w_path *frompath, float flatness);
wresult w_path_create_from_data(w_path *path, size_t length, char *types,
		float *pt);
wresult w_path_add_arc(w_path *path, w_rectf *rect, float startAngle,
		float arcAngle);
wresult w_path_add_path(w_path *path, w_path *frompath);
wresult w_path_add_rectangle(w_path *path, w_rectf *rect);
wresult w_path_add_string(w_path *path, const char *string, int length,
		w_pointf *pt, w_font *font);
wresult w_path_close(w_path *path);
wresult w_path_contains(w_path *path, w_pointf *pt, w_graphics *gc,
		wbool outline);
wresult w_path_cubicto(w_path *path, float cx1, float cy1, float cx2, float cy2,
		float x, float y);
wresult w_path_get_bounds(w_path *path, w_rectf *bounds);
wresult w_path_get_current_point(w_path *path, w_pointf *point);
wresult w_path_get_path_data(w_path *path, w_iterator *data);
wresult w_path_lineto(w_path *path, float x, float y);
wresult w_path_is_ok(w_path *path);
wresult w_path_move_to(w_path *path, float x, float y);
wresult w_path_quadto(w_path *path, float cx, float cy, float x, float y);

#ifdef __cplusplus
}
#endif

#endif /* SWT_GRAPHICS_PATH_H_ */