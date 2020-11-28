/*
 * sash.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_SASH_H_
#define SWT_CONTROLS_SASH_H_
#include "../widgets/control.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_sash {
	w_control control;
	void *handle[8];
} w_sash;
#define W_SASH(x) ((w_sash*)x)
struct _w_sash_class {
	struct _w_control_class control;
};
typedef struct w_event_sash {
	w_event event;
	w_rect bounds;
}w_event_sash;
SWT_PUBLIC wresult w_sash_create(w_sash *sash, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_sash* w_sash_new(struct w_toolkit *toolkit, w_composite *parent,
                              wuint64 style, w_widget_post_event_proc post_event);
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_SASH_H_ */
