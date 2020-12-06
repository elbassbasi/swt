/*
 * composite.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_COMPOSITE_H_
#define SRC_WIN32_WIDGETS_COMPOSITE_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "scrollable.h"
#define STATE_FOREIGN_HANDLE (1 << (STATE_SCROLLABLE_END + 0))
#define STATE_COMPOSITE_END (STATE_SCROLLABLE_END + 1)
typedef struct _w_composite {
	struct _w_scrollable scrollable;
	w_layout *layout;
	size_t children_count;
} _w_composite;

struct _w_composite_reserved {
	struct _w_scrollable_reserved scrollable;

};
#define _W_COMPOSITE(x) ((_w_composite*)x)
#define _W_COMPOSITE_RESERVED(x) ((struct _w_composite_reserved*)x)
/*
 * public function
 */
void _w_composite_get_children(w_composite *composite, w_iterator *it);
wresult _w_composite_get_layout(w_composite *composite,w_layout** layout);
int _w_composite_get_layout_deferred(w_composite *composite);
void _w_composite_get_tab_list(w_composite *composite, w_iterator *it);
int _w_composite_is_layout_deferred(w_composite *composite);
void _w_composite_do_layout(w_composite *composite, int changed, int all);
void _w_composite_set_layout(w_composite *composite, w_layout *layout);
void _w_composite_set_layout_deferred(w_composite *composite, int defer);
wresult _w_composite_create_handle (w_control *control, _w_control_reserved *reserved);
/*
 * post event proc
 */
void _w_composite_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle);
wresult _w_composite_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event);
int _w_composite_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved);
int _w_composite_paint(w_widget *widget, _w_event_platform *e);
int _COMPOSITE_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_GETFONT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_PARENTNOTIFY(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_PRINTCLIENT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_UPDATEUISTATE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_NCPAINT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _COMPOSITE_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
void _w_composite_class_init(struct _w_composite_class *clazz);
#endif
#endif /* SRC_WIN32_WIDGETS_COMPOSITE_H_ */
