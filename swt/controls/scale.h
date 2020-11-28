/*
 * scale.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_SCALE_H_
#define SWT_CONTROLS_SCALE_H_
#include "../widgets/control.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_scale {
	w_control control;
	void *handle[8];
} w_scale;
#define W_SCALE(x) ((w_scale*)x)
struct _w_scale_class {
	struct _w_control_class control;
	int (*get_increment)(w_scale *scale);
	int (*get_maximum)(w_scale *scale);
	int (*get_minimum)(w_scale *scale);
	int (*get_page_increment)(w_scale *scale);
	int (*get_selection)(w_scale *scale);
	wresult (*set_increment)(w_scale *scale, int increment);
	wresult (*set_maximum)(w_scale *scale, int value);
	wresult (*set_minimum)(w_scale *scale, int value);
	wresult (*set_page_increment)(w_scale *scale, int pageIncrement);
	wresult (*set_selection)(w_scale *scale, int value);
};
SWT_PUBLIC wresult w_scale_create(w_scale *scale, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_scale* w_scale_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC int w_scale_get_increment(w_scale *scale);
SWT_PUBLIC int w_scale_get_maximum(w_scale *scale);
SWT_PUBLIC int w_scale_get_minimum(w_scale *scale);
SWT_PUBLIC int w_scale_get_page_increment(w_scale *scale);
SWT_PUBLIC int w_scale_get_selection(w_scale *scale);
SWT_PUBLIC wresult w_scale_set_increment(w_scale *scale, int increment);
SWT_PUBLIC wresult w_scale_set_maximum(w_scale *scale, int value);
SWT_PUBLIC wresult w_scale_set_minimum(w_scale *scale, int value);
SWT_PUBLIC wresult w_scale_set_page_increment(w_scale *scale, int pageIncrement);
SWT_PUBLIC wresult w_scale_set_selection(w_scale *scale, int value);

#define W_SCALE_CLASS(x) ((struct _w_scale_class*)x)
#define W_SCALE_GET_CLASS(x) ((struct _w_scale_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_SCALE_H_ */
