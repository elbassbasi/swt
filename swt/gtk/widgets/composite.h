/*
 * composite.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_COMPOSITE_H_
#define SRC_GTK_WIDGETS_COMPOSITE_H_

#ifdef __linux
#include "scrollable.h"
#define STATE_COMPOSITE_END (STATE_SCROLLABLE_END + 0)
typedef struct _w_composite {
	struct _w_scrollable scrollable;
	GtkIMContext *imHandle;
	w_layout *layout;
	wushort layoutCount;
	int backgroundMode;
} _w_composite;

typedef struct _w_composite_reserved {
	struct _w_scrollable_reserved scrollable;
	guint signal_scroll_child;
	GtkWidget* (*parenting_handle)(w_widget *composite,
			struct _w_widget_reserved *reserved);
	void (*add_child)(w_composite *parent, GtkWidget *widget,
			struct _w_composite_reserved *reserved);
	wresult (*has_border)(w_composite *composite,
			struct _w_composite_reserved *reserved);
	void (*move_children)(w_composite *composite, int oldWidth,
			struct _w_composite_reserved *reserved);
	w_composite* (*find_deferred_control)(w_composite *composite,
			struct _w_composite_reserved *reserved);
} _w_composite_reserved;
#define _W_COMPOSITE(x) ((_w_composite*)x)
#define _W_COMPOSITE_RESERVED(x) ((struct _w_composite_reserved*)x)
#define _W_COMPOSITE_GET_RESERVED(x) ((struct _w_composite_reserved*)_w_widget_get_reserved(W_WIDGET(x)))
#define _W_COMPOSITE_GET_PARENTING_HANDLE(control,reserved) _W_COMPOSITE_RESERVED(reserved)->parenting_handle\
(W_WIDGET(control), _W_WIDGET_RESERVED(reserved))
typedef struct _w_composite_iterator {
	w_basic_iterator base;
	w_composite *composite;
	_w_fixed *first;
	_w_fixed *i;
	size_t count;
	wbool tablist;
} _w_composite_iterator;
#define _W_COMPOSITE_ITERATOR(it) ((_w_composite_iterator*)it)
/**
 *
 */
GtkWidget* _w_composite_scrolledHandle(w_widget *control,
		 _w_widget_reserved *reserved);
GtkWidget* _w_composite_topHandle(w_widget *control,
		 _w_widget_reserved *reserved);
wuint64 _w_composite_check_style(w_widget *control, wuint64 style);
wresult _w_composite_get_children(w_composite *composite, w_iterator *it);
wresult _w_composite_changed(w_composite *_this, w_control **changed,
		size_t length);
void _w_composite_check_buffered(w_control *control,
		_w_control_reserved *reserved);
GtkStyle* _w_composite_child_style(w_control *control,
		_w_control_reserved *reserved);
int _w_composite_compute_size(w_widget *widget,  w_event_compute_size *e,
		 _w_widget_reserved *reserved);
wresult _w_composite_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved);
int _w_composite_apply_theme_background(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved);
wresult _w_composite_create_handle_0(w_widget *composite, int index,
		GtkWidget **fixed, GtkWidget **scrolled, GtkWidget **handle);
wresult _w_composite_draw_background(w_composite *composite, w_graphics *gc,
		w_rect *_rect, w_point *_offset);
wresult _w_composite_enable_widget(w_control *control, wbool enabled,
		_w_control_reserved *reserved);
w_composite* _w_composite_find_deferred_control(w_composite *composite,
		 _w_composite_reserved *reserved);
void _w_composite_fix_modal(w_control *control, GtkWindowGroup *group,
		GtkWindowGroup *modalGroup, _w_control_reserved *reserved);
void _w_composite_fix_style(w_control *control, _w_control_reserved *reserved);
void _w_composite_fix_zorder(w_composite *composite,
		_w_composite_reserved *reserved);
GtkWidget* _w_composite_focusHandle(w_widget *control,
		_w_widget_reserved *reserved);
gboolean _w_composite_force_focus(w_control *control, GtkWidget *focusHandle,
		_w_widget_reserved *reserved);
int _w_composite_get_backgroundMode(w_composite *composite);
int _w_composite_get_children_count(w_composite *composite);
int _w_composite_get_client_area(w_widget *widget,
		 w_event_client_area *e,  _w_widget_reserved *reserved);
wresult _w_composite_get_layout(w_composite *composite,w_layout** layout);
int _w_composite_get_layout_deferred(w_composite *composite);
wresult _w_composite_get_tab_list(w_composite *composite, w_iterator *it);
wbool _w_composite_has_border(w_composite *composite,
		 _w_composite_reserved *reserved);
void _w_composite_hook_events(w_widget *widget, _w_widget_reserved *reserved);
GtkIMContext* _w_composite_imHandle(w_widget *control,
		_w_widget_reserved *reserved);
wresult _w_composite_is_layout_deferred(w_composite *composite);
wresult _w_composite_is_tab_group(w_control *control,
		_w_control_reserved *reserved);
void _w_composite_layout(w_composite *composite, int flags);
wresult _w_composite_layout_changed(w_composite *_this, w_control **changed,
		size_t length, int flags);
void _w_composite_mark_layout(w_control *_this, int flags,
		_w_control_reserved *reserved);
void _w_composite_move_above(w_composite *_this, GtkWidget *child,
		GtkWidget *sibling, _w_control_reserved *reserved);
void _w_composite_move_below(w_composite *_this, GtkWidget *child,
		GtkWidget *sibling, _w_control_reserved *reserved);
void _w_composite_move_children(w_composite *composite, int oldWidth);
void _w_composite_minimum_size(w_composite *composite, w_size *size, int wHint,
		int hHint, wbool changed);
GtkWidget* _w_composite_parenting_handle(w_widget *control,
		 _w_widget_reserved *reserved);
void _w_composite_print_widget(w_control *control, w_graphics *gc,
		void *drawable, int depth, w_point *loc,
		_w_control_reserved *reserved);
void _w_composite_redraw_children(w_control *control,
		_w_control_reserved *reserved);
void _w_composite_resize_handle(w_control *control, w_size *size,
		 _w_control_reserved *reserved);
wresult _w_composite_set_background_mode(w_composite *composite, int mode);
wresult _w_composite_set_bounds_0(w_control *control,w_point *location,
		w_size *size,_w_control_reserved *reserved);
wresult _w_composite_set_focus(w_control *control);
wresult _w_composite_set_layout(w_composite *composite, w_layout *layout);
wresult _w_composite_set_layout_deferred(w_composite *composite, int defer);
void _w_composite_set_orientation(w_control *control, wbool create,
		_w_control_reserved *reserved);
int _w_composite_set_scrollbar_visible(w_widget *scrollable, int style,
		int visible,  _w_widget_reserved *reserved);
wresult _w_composite_set_tab_group_focus(w_widget *widget, wbool next,
		_w_widget_reserved *reserved);
wresult _w_composite_set_tab_item_focus(w_widget *widget, wbool next,
		_w_widget_reserved *reserved);
wresult _w_composite_set_tab_list(w_composite *composite, w_control **tabList,
		size_t length);
void _w_composite_show_widget(w_control *control,
		_w_control_reserved *reserved);
wresult _w_composite_check_subwindow(w_control *control,
		_w_control_reserved *reserved);
wresult _w_composite_translate_mnemonic(w_control *_this, w_event_key *event,
		w_control *control, _w_control_reserved *reserved);
int _w_composite_traversal_code(w_control *control, int key, GdkEventKey *event,
		_w_control_reserved *reserved);
wresult _w_composite_translate_traversal(w_control *control,
		GdkEventKey *keyEvent, _w_control_reserved *reserved);
void _w_composite_update_background_mode(w_control *control,
		_w_control_reserved *reserved);
void _w_composite_update_layout(w_control *_this, int flags,
		_w_control_reserved *reserved);
void _w_composite_add_child_0(w_composite *parent, GtkWidget *widget,
		 _w_composite_reserved *reserved);
void _w_composite_add_child(w_composite *parent, GtkWidget *widget);
/*
 * signals
 */

gboolean _gtk_composite_destroy( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_button_press_event( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_draw( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_key_press_event( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_focus( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_focus_in_event( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_focus_out_event( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_map( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_realize( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_scroll_child( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
gboolean _gtk_composite_style_set( w_widget *widget,
		 _w_event_platform *e,  _w_widget_reserved *reserved);
void _w_composite_class_init(struct  _w_composite_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_COMPOSITE_H_ */
