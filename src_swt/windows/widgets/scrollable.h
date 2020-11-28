/*
 * scrollable.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_SCROLLABLE_H_
#define SRC_WIN32_WIDGETS_SCROLLABLE_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "control.h"
#define STATE_CANVAS (1 << (STATE_CONTROL_END + 0))
#define STATE_SCROLLABLE_END (STATE_CONTROL_END + 1)
typedef struct _w_scrollable {
	struct _w_control control;
} _w_scrollable;

typedef struct _w_scrollbar {
	w_widgetdata widgetdata;
	w_scrollable *scrollable;
	int type;
} _w_scrollbar;

struct _w_scrollable_reserved {
	struct _w_control_reserved control;
	HWND (*scrolledHandle)(w_control *control);

};
#define _W_SCROLLABLE(x) ((_w_scrollable*)x)
#define _W_SCROLLABLE_RESERVED(x) ((struct _w_scrollable_reserved*)x)
#define _W_SCROLLBAR(x) ((_w_scrollbar*)x)
/*
 * public function for scrollbar
 */
void _w_scrollbar_copy(w_widgetdata *from, w_widgetdata *to);
w_scrollable* _w_scrollbar_get_parent(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_enabled(w_scrollbar *scrollbar);
wresult _w_scrollbar_is_enabled(w_scrollbar *scrollbar);
void _w_scrollbar_set_enabled(w_scrollbar *scrollbar, wresult enabled);
wresult _w_scrollbar_get_visible(w_scrollbar *scrollbar);
wresult _w_scrollbar_is_visible(w_scrollbar *scrollbar);
void _w_scrollbar_set_visible(w_scrollbar *scrollbar, wresult visible);
void _w_scrollbar_get_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values);
int _w_scrollbar_get_increment(w_scrollbar *scrollbar);
int _w_scrollbar_get_maximum(w_scrollbar *scrollbar);
int _w_scrollbar_get_minimum(w_scrollbar *scrollbar);
int _w_scrollbar_get_page_increment(w_scrollbar *scrollbar);
int _w_scrollbar_get_selection(w_scrollbar *scrollbar);
int _w_scrollbar_get_thumb(w_scrollbar *scrollbar);
void _w_scrollbar_set_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values);
void _w_scrollbar_set_increment(w_scrollbar *scrollbar, int increment);
void _w_scrollbar_set_maximum(w_scrollbar *scrollbar, int maximum);
void _w_scrollbar_set_minimum(w_scrollbar *scrollbar, int minimum);
void _w_scrollbar_set_page_increment(w_scrollbar *scrollbar, int pageIncrement);
void _w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection);
void _w_scrollbar_set_thumb(w_scrollbar *scrollbar, int thumb);
void _w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size);
void _w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect);
void _w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar, w_rect *rect);
/*
 * public function for scrollable
 */
void _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar);
int _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable);
void _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar);
/*
 * post event proc
 */
void _w_scrollable_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle);
int _w_scrollable_get_client_area(w_widget *widget,
		struct w_event_client_area *e, struct _w_widget_reserved *reserved);
int _w_scrollable_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved);
int _wm_scroll(w_widget *widget, struct _w_event_platform *e, wresult update);
int _SCROLLABLE_WM_MOUSEWHEEL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SCROLLABLE_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _wm_scrollbale_ScrollWheel(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _wm_scrollbale_Scroll(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SCROLLABLE_WM_VSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SCROLLABLE_WM_HSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
void _w_scrollable_class_init(struct _w_scrollable_class *clazz);
#endif
#endif /* SRC_WIN32_WIDGETS_SCROLLABLE_H_ */
