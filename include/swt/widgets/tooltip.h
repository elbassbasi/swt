/*
 * tooltip.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_WIDGETS_TOOLTIP_H_
#define SWT_WIDGETS_TOOLTIP_H_
#include "widget.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_tooltip {
	w_widget widget;
	void *handle[0x10];
} w_tooltip;

#define W_TOOLTIP(x) ((w_tooltip*)x)

struct _w_tooltip_class {
	struct _w_widget_class widget;
	wbool (*get_auto_hide)(w_tooltip *tooltip);
	wresult (*get_message)(w_tooltip *tooltip,w_alloc alloc,void* user_data);
	w_shell* (*get_parent)(w_tooltip *tooltip);
	wresult (*get_text)(w_tooltip *tooltip,w_alloc alloc,void* user_data);
	wbool (*get_visible)(w_tooltip *tooltip);
	wbool (*is_visible)(w_tooltip *tooltip);
	wresult (*set_autohide)(w_tooltip *tooltip, wbool autoHide);
	wresult (*set_location)(w_tooltip *tooltip, w_point *location);
	wresult (*set_message)(w_tooltip *tooltip, const char *string);
	wresult (*set_text)(w_tooltip *tooltip, const char *string);
	wresult (*set_visible)(w_tooltip *tooltip, wbool visible);
};
SWT_PUBLIC wresult w_tooltip_create(w_tooltip *tooltip, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_tooltip* w_tooltip_new(struct w_toolkit *toolkit, w_composite *parent,
                                    wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wbool w_tooltip_get_auto_hide(w_tooltip *tooltip);
SWT_PUBLIC wresult w_tooltip_get_message(w_tooltip *tooltip,w_alloc alloc,void* user_data);
SWT_PUBLIC w_shell* w_tooltip_get_parent(w_tooltip *tooltip);
SWT_PUBLIC wresult w_tooltip_get_text(w_tooltip *tooltip,w_alloc alloc,void* user_data);
SWT_PUBLIC wbool w_tooltip_get_visible(w_tooltip *tooltip);
SWT_PUBLIC wbool w_tooltip_is_visible(w_tooltip *tooltip);
SWT_PUBLIC wresult w_tooltip_set_autohide(w_tooltip *tooltip, wbool autoHide);
SWT_PUBLIC wresult w_tooltip_set_location(w_tooltip *tooltip, w_point *location);
SWT_PUBLIC wresult w_tooltip_set_message(w_tooltip *tooltip, const char *string);
SWT_PUBLIC wresult w_tooltip_set_text(w_tooltip *tooltip, const char *string);
SWT_PUBLIC wresult w_tooltip_set_visible(w_tooltip *tooltip, wbool visible);

#define W_TOOLTIP_CLASS(x) ((struct _w_tooltip_class*)x)
#define W_TOOLTIP_GET_CLASS(x) ((struct _w_rooltip_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_TOOLTIP_H_ */
