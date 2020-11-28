/*
 * scrollable.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_SCROLLABLE_H_
#define SRC_GTK_WIDGETS_SCROLLABLE_H_

#ifdef __linux
#include "control.h"
#define STATE_VSCROLLBAR_DRAGSEND (1 << (STATE_CONTROL_END + 1))
#define STATE_HSCROLLBAR_DRAGSEND (1 << (STATE_CONTROL_END + 2))
#define STATE_SCROLLABLE_END (STATE_CONTROL_END + 2)
typedef struct _w_scrollable {
	struct _w_control control;
	int vdetail;
	int hdetail;
} _w_scrollable;

typedef struct _w_scrollbar {
	struct w_widgetdata widgetdata;
	w_scrollable *parent;
	GtkWidget *scrolledHandle;
	int style;
} _w_scrollbar;
typedef struct _w_scrollable_reserved _w_scrollable_reserved;
struct _w_scrollable_reserved {
	struct _w_control_reserved control;
	GtkWidget* (*scrolledHandle)(w_widget *scrollable,
			struct _w_widget_reserved *reserved);
	int (*set_scrollbar_visible)(w_widget *scrollable, int style, int visible,
			struct _w_widget_reserved *reserved);
	int (*apply_theme_background)(w_scrollable *scrollable,
			_w_scrollable_reserved *reserved);
	void (*redraw_background_image)(w_scrollable *scrollable,
				_w_scrollable_reserved *reserved);
	void (*update_scrollbar_value)(w_scrollable *scrollable,wuint64 style,
				_w_scrollable_reserved *reserved);
};
#define _W_SCROLLABLE(x) ((_w_scrollable*)x)
#define _W_SCROLLABLE_RESERVED(x) ((_w_scrollable_reserved*)x)
#define _W_SCROLLABLE_GET_RESERVED(x) ((_w_scrollable_reserved*)_w_widget_get_reserved(W_WIDGET(x)))
#define _W_SCROLLBAR(x) ((_w_scrollbar*)x)

/*
 * ScrollBar
 */
GtkWidget* _w_scrollbar_get_handle(w_scrollbar *scrollbar);
GtkAdjustment* _w_scrollbar_get_adjustment(w_scrollbar *scrollbar);
void _w_scrollbar_copy(w_widgetdata *from, w_widgetdata *to);
wresult _w_scrollbar_get_enabled(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_increment(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_maximum(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_minimum(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_page_increment(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_parent(w_scrollbar *scrollbar,w_scrollable **parent);
wresult _w_scrollbar_get_selection(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size);
wresult _w_scrollbar_get_thumb(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect);
wresult _w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar, w_rect *rect);
wresult _w_scrollbar_get_values(w_scrollbar *scrollbar,
		 w_scrollbar_value *values);
wresult _w_scrollbar_get_visible(w_scrollbar *scrollbar);
wresult _w_scrollbar_is_enabled(w_scrollbar *scrollbar);
wresult _w_scrollbar_is_visible(w_scrollbar *scrollbar);
wresult _w_scrollbar_set_enabled(w_scrollbar *scrollbar, wbool enabled);
wresult _w_scrollbar_set_increment(w_scrollbar *scrollbar, int value);
wresult _w_scrollbar_set_maximum(w_scrollbar *scrollbar, int value);
wresult _w_scrollbar_set_minimum(w_scrollbar *scrollbar, int value);
wresult _w_scrollbar_set_page_increment(w_scrollbar *scrollbar, int value);
wresult _w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection);
wresult _w_scrollbar_set_thumb(w_scrollbar *scrollbar, int value);
wresult _w_scrollbar_set_values(w_scrollbar *scrollbar,
		 w_scrollbar_value *values);
wresult _w_scrollbar_set_visible_0(w_scrollbar *scrollbar, wbool visible);
wresult _w_scrollbar_set_visible(w_scrollbar *scrollbar, wbool visible);
/*
 * scrollable
 */
wresult _w_scrollable_compute_trim(w_widget *widget,  w_event_compute_trim *e,
		 _w_widget_reserved *reserved);
void _w_scrollable_update_background_mode(w_control *control,
		_w_control_reserved *reserved);
int _w_scrollable_apply_theme_background(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved);
wresult _w_scrollable_get_border_width(w_control *control);
wresult _w_scrollable_get_client_area(w_widget *widget,
		 w_event_client_area *e,  _w_widget_reserved *reserved);
wresult _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) ;
wresult _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable);
wresult _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar);
int _w_scrollable_hscrollbar_width(w_scrollable *scrollable,
		 _w_scrollable_reserved *reserved);
wresult _w_scrollable_send_leave_notify(w_control *control,
		_w_control_reserved *reserved);
void _w_scrollable_set_orientation(w_control *control, wbool create,
		_w_control_reserved *reserved);
int _w_scrollable_set_scrollbar_visible_0(w_widget *scrollable, int style,
		int visible,  _w_widget_reserved *reserved);
void _w_scrollable_redraw_background_image(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved);
void _w_scrollable_redraw_widget(w_control *control, w_rect *_rect, int flags,
		_w_control_reserved *reserved);
void _w_scrollable_resize_handle(w_control *control, w_size *size,
		_w_control_reserved *reserved);
void _w_scrollable_show_widget(w_control *control,
		_w_control_reserved *reserved);
void _w_scrollable_update_scrollbar_value(w_scrollable *scrollable,
		wuint64 style, _w_scrollable_reserved *reserved);
int _w_scrollable_vscrollbar_width(w_scrollable *scrollable,
		 _w_scrollable_reserved *reserved);
void _w_scrollable_hook_events(w_widget *widget, _w_widget_reserved *reserved);
/*
 * signals
 */
wuint64 _gtk_scrollbar_style(GtkWidget *scrolledHandle, GtkWidget *adjustment);
wuint64 _gtk_scrollbar_style0(GtkWidget *scrolledHandle, GtkWidget *adjustment);
gboolean _gtk_scrollbar_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved);
gboolean _gtk_scrollbar_change_value(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_scrollbar_range_get_slider_range(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved);
gboolean _gtk_scrollbar_value_changed(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_scrollbar_event_after(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_scrollable_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved);
gboolean _gtk_scrollable_change_value( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_scrollable_value_changed( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_scrollable_event_after( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_scrollable_scroll_event( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
void _w_scrollable_class_init(struct _w_scrollable_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_SCROLLABLE_H_ */
