/*
 * brush.h
 *
 *  Created on: 13 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_GRAPHICS_PATTERN_H_
#define SWT_GRAPHICS_PATTERN_H_
#include "image.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct w_pattern {
	void *handle[2];
} w_pattern;
#define W_PATTERN(x) ((w_pattern*)x)
wresult w_pattern_init(w_pattern *pattren);
wresult w_pattern_dispose(w_pattern *pattren);
wresult w_pattern_is_ok(w_pattern *pattren);
wresult w_pattern_create_from_image(w_pattern *pattren, w_image *image);
wresult w_pattern_create(w_pattern *pattren, float x1, float y1, float x2,
		float y2, w_color color1, w_color color2);
#ifdef __cplusplus
}
#endif
#endif /* SWT_GRAPHICS_PATTERN_H_ */
