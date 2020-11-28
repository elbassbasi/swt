/*
 * link.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_LINK_H_
#define SWT_CONTROLS_LINK_H_
#include "../widgets/control.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_link {
	w_control control;
	void *handle[8];
} w_link;
#define W_LINK(x) ((w_link*)x)
struct _w_link_class {
	struct _w_control_class control;
	wresult (*get_text)(w_link *link,w_alloc alloc,void* user_data);
	wresult (*set_text)(w_link *link, const char *string);
};
SWT_PUBLIC wresult w_link_create(w_link *link, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_link* w_link_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_link_get_text(w_link *link,w_alloc alloc,void* user_data);
SWT_PUBLIC wresult w_link_set_text(w_link *link, const char *string);
#define W_LINK_CLASS(x) ((struct _w_link_class*)x)
#define W_LINK_GET_CLASS(x) ((struct _w_link_class*)W_WIDGET_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_LINK_H_ */
