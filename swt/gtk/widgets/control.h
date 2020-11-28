/*
 * control.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_CONTROL_H_
#define SRC_GTK_WIDGETS_CONTROL_H_
#ifdef __linux
#include "menu.h"
#define STATE_CANVAS (1 << (STATE_WIDGET_END + 1))
#define STATE_MENU (1 << (STATE_WIDGET_END + 2))
#define STATE_DRAG_DETECT (1 << (STATE_WIDGET_END + 3))
#define STATE_TIMER_START (1 << (STATE_WIDGET_END + 4))
#define STATE_THEME_BACKGROUND (1 << (STATE_WIDGET_END + 5))
#define STATE_CHECK_SUBWINDOW (1 << (STATE_WIDGET_END + 6))
#define STATE_LAYOUT_DATA_LOCALE (1 << (STATE_WIDGET_END + 7))
#define STATE_DRAW_REGION (1 << (STATE_WIDGET_END + 8))
#define STATE_ZERO_WIDTH  (1 << (STATE_WIDGET_END + 9))
#define STATE_ZERO_HEIGHT (1 << (STATE_WIDGET_END + 10))
#define STATE_BACKGROUND (1 << (STATE_WIDGET_END + 11))
#define STATE_PARENT_BACKGROUND (1 << (STATE_WIDGET_END + 12))
#define STATE_DISABLED (1 << (STATE_WIDGET_END + 13))
#define STATE_ENABLE_WINDOW (1 << (STATE_WIDGET_END + 14))
#define STATE_REDRAW_WINDOW (1 << (STATE_WIDGET_END + 15))
#define STATE_TAB_LIST (1 << (STATE_WIDGET_END + 16))
#define STATE_LAYOUT_NEEDED (1 << (STATE_WIDGET_END + 17))
#define STATE_LAYOUT_CHANGED (1 << (STATE_WIDGET_END + 18))
#define STATE_LAYOUT_CHILD (1 << (STATE_WIDGET_END + 19))
#define STATE_HIDDEN (1 << (STATE_WIDGET_END + 20))
#define STATE_FONT (1 << (STATE_WIDGET_END + 21))
#define STATE_FOREGROUND (1 << (STATE_WIDGET_END + 22))
#define STATE_DROPTARGET_IS_ACTIVE (1 << (STATE_WIDGET_END + 23))
#define STATE_CONTROL_END (STATE_WIDGET_END + 23)

/*
 * zorder
 */
#define ZORDER_ABOVE (1 << 0)
#define ZORDER_FIX_RELATION (1 << 1)
#define ZORDER_FIX_CHILDREN (1 << 2)
/*
 * redraw
 */
#define REDRAW_REDRAWALL (1 << 0)
#define REDRAW_ALL (1 << 1)
#define REDRAW_TRIM (1 << 2)
typedef struct _w_control_timer {
	w_control *control;
	struct _w_control_timer *next;
	unsigned short id;
} _w_control_timer;
typedef struct _w_control {
	struct _w_widget widget;
	w_cursor *cursor;
	w_menu *menu;
	w_menu *menus;
	w_composite *parent;
	GdkPixbuf *backgroundImage;
	GdkWindow *window;
	int backgroundAlpha;
	int drawCount;
	gulong enterNotifyEventId;
	union {
		PangoFontDescription *defaultfont;
		w_font *font;
	};
	struct _w_control_timer timer;
	w_point lastInput;
	GtkCssProvider *provider;
	GdkWindow *enableWindow;
	GdkWindow *redrawWindow;
	w_dragsource *dragsource;
	w_droptarget *droptarget;
} _w_control;
typedef struct _w_control_reserved _w_control_reserved;
struct _w_control_reserved {
	_w_widget_reserved widget;
	GtkWidget* (*fixedHandle)(w_widget *control, _w_widget_reserved *reserved);
	GtkWidget* (*clientHandle)(w_widget *control, _w_widget_reserved *reserved);
	GtkWidget* (*focusHandle)(w_widget *control, _w_widget_reserved *reserved);
	GtkWidget* (*eventHandle)(w_widget *control, _w_widget_reserved *reserved);
	GtkWidget* (*enterExitHandle)(w_widget *control,
			_w_widget_reserved *reserved);
	GtkWidget* (*paintHandle)(w_widget *control, _w_widget_reserved *reserved);
	GtkIMContext* (*imHandle)(w_widget *control, _w_widget_reserved *reserved);
	GtkWidget* (*fontHandle)(w_widget *control, _w_widget_reserved *reserved);
	GdkWindow* (*paintWindow)(w_widget *control, _w_widget_reserved *reserved);
	GdkWindow* (*eventWindow)(w_widget *control, _w_widget_reserved *reserved);
	GdkWindow* (*enableWindow)(w_widget *control,
			_w_control_reserved *reserved);
	GdkWindow* (*redrawWindow)(w_widget *control,
			_w_control_reserved *reserved);
	void (*force_resize)(w_control *control, _w_control_reserved *reserved);
	int (*get_client_width)(w_control *control, _w_control_reserved *reserved);
	wresult (*set_bounds)(w_control *control,w_point* location,w_size* size,
			_w_control_reserved *reserved);
	void (*move_handle)(w_control *control, w_point *position,
			_w_control_reserved *reserved);
	void (*resize_handle)(w_control *control, w_size *size,
			_w_control_reserved *reserved);
	void (*move_children)(w_control *control, int oldWidth,
			_w_control_reserved *reserved);
	void (*draw_widget)(w_control *control, w_graphics *gc,
			_w_control_reserved *reserved);
	void (*set_orientation)(w_control *control, int create,
			_w_control_reserved *reserved);
	wresult (*set_ZOrder)(w_control *control, w_control *sibling, int flags,
			_w_control_reserved *reserved);
	GtkStyle* (*child_style)(w_control *control, _w_control_reserved *reserved);
	w_control* (*find_background_control)(w_control *control,
			_w_control_reserved *reserved);
	void (*set_background_color)(w_control *control, GdkColor *color,
			_w_control_reserved *reserved);
	void (*set_background)(w_control *control, _w_control_reserved *reserved);
	void (*set_font_description)(w_control *control, PangoFontDescription *font,
			_w_control_reserved *reserved);
	void (*set_foreground_color)(w_control *control, GtkWidget *handle,
			GdkRGBA *rgba, _w_control_reserved *reserved);
	void (*set_foreground_color_0)(w_control *control, GdkRGBA *rgba,
			_w_control_reserved *reserved);
	void (*set_parent_background)(w_control *control,
			_w_control_reserved *reserved);
	void (*set_widget_background)(w_control *control,
			_w_control_reserved *reserved);
	void (*get_background_color)(w_control *control, GdkColor *color,
			_w_control_reserved *reserved);
	void (*get_foreground_color)(w_control *control, GdkColor *color,
			_w_control_reserved *reserved);
	void (*check_border)(w_control *control, _w_control_reserved *reserved);
	void (*check_buffered)(w_control *control, _w_control_reserved *reserved);
	void (*check_background)(w_control *control, _w_control_reserved *reserved);
	void (*check_foreground)(w_control *control, _w_control_reserved *reserved);
	void (*check_mirrored)(w_control *control, _w_control_reserved *reserved);
	wresult (*check_subwindow)(w_control *control,
			_w_control_reserved *reserved);
	void (*fix_focus)(w_control *control, w_control *focusControl,
			_w_control_reserved *reserved);
	void (*fix_style)(w_control *control, _w_control_reserved *reserved);
	void (*fix_style_0)(w_control *control, GtkWidget *handle,
			_w_control_reserved *reserved);
	void (*fix_modal)(w_control *control, GtkWindowGroup *group,
			GtkWindowGroup *modalGroup, _w_control_reserved *reserved);
	wresult (*has_focus)(w_control *control, _w_control_reserved *reserved);
	void (*show_widget)(w_control *control, _w_control_reserved *reserved);
	void (*set_initial_bounds)(w_control *control,
			_w_control_reserved *reserved);
	void (*set_relations)(w_control *control, _w_control_reserved *reserved);
	wresult (*set_cursor)(w_control *control, GdkCursor *cursor,
			_w_control_reserved *reserved);
	wresult (*traverse_escape)(w_control *control,
			_w_control_reserved *reserved);
	wresult (*traverse_item)(w_control *control, int next,
			_w_control_reserved *reserved);
	wresult (*traverse_group)(w_control *control, wbool next,
			_w_control_reserved *reserved);
	wresult (*traverse_return)(w_control *control,
			_w_control_reserved *reserved);
	int (*traversal_code)(w_control *control, int key, GdkEventKey *event,
			_w_control_reserved *reserved);
	gboolean (*traverse_page)(w_control *control, gboolean next,
			_w_control_reserved *reserved);
	gboolean (*traverse_mnemonic)(w_control *control, int key,
			_w_control_reserved *reserved);
	gboolean (*mnemonic_hit)(w_control *control, int key,
			_w_control_reserved *reserved);
	gboolean (*mnemonic_match)(w_control *control, int key,
			_w_control_reserved *reserved);
	wresult (*translate_mnemonic)(w_control *_this, w_event_key *event,
			w_control *control, _w_control_reserved *reserved);
	wresult (*translate_traversal)(w_control *control, GdkEventKey *keyEvent,
			_w_control_reserved *reserved);
	w_widget* (*compute_tab_group)(w_control *control,
			_w_control_reserved *reserved);
	w_control* (*compute_tab_root)(w_control *control,
			_w_control_reserved *reserved);
	wresult (*compute_tab_list)(w_control *control,
			_w_control_reserved *reserved);
	wresult (*is_tab_group)(w_control *control, _w_control_reserved *reserved);
	wresult (*is_tab_item)(w_control *control, _w_control_reserved *reserved);
	wresult (*enable_widget)(w_control *control, wbool enabled,
			_w_control_reserved *reserved);
	wresult (*get_imcaret_pos)(w_control *control, w_point *pos,
			_w_control_reserved *reserved);
	void (*redraw_widget)(w_control *control, w_rect *rect, int flags,
			_w_control_reserved *reserved);
	void (*redraw_children)(w_control *control, _w_control_reserved *reserved);
	gboolean (*force_focus)(w_control *control, GtkWidget *focusHandle,
			_w_widget_reserved *reserved);
	void (*mark_layout)(w_control *control, int flags,
			_w_control_reserved *reserved);
	void (*update_layout)(w_control *control, int flags,
			_w_control_reserved *reserved);
#if GTK2
	void (*print_widget)(w_control *control, w_graphics *gc, void *drawable,
			int depth, w_point *loc, _w_control_reserved *reserved);
#endif
	void (*update_background_mode)(w_control *control,
			_w_control_reserved *reserved);
	wresult (*send_leave_notify)(w_control *control,
			_w_control_reserved *reserved);
	wresult (*is_described_by_label)(w_control *control,
			_w_control_reserved *reserved);
	gboolean (*is_focus_handle)(w_control *control, GtkWidget *widget,
			_w_control_reserved *reserved);
	void (*add_relation)(w_control *_this, w_control *control,
			_w_control_reserved *reserved);
	void (*get_window_origin)(w_control *control, w_point *pt,
			_w_control_reserved *reserved);
};
#define _W_CONTROL(x) ((_w_control*)x)
#define _W_CONTROL_RESERVED(x) ((_w_control_reserved*)x)
#define _W_CONTROL_GET_RESERVED(x) ((_w_control_reserved*)_w_widget_get_reserved(W_WIDGET(x)))

/*
 * internal function
 */
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event);
extern int enterNotifyEventSignalId;
extern GQuark GTK_POINTER_WINDOW;
extern GQuark SWT_GRAB_WIDGET;
w_font* _w_control_default_font(w_control *control);
wresult _w_control_draw_background_0(w_control *_this, w_control *control,
		GdkWindow *window, cairo_t *cr, cairo_region_t *region, w_rect *rect,
		_w_control_reserved *reserved);
wresult _w_control_draw_background(w_control *_this, w_control *control,
		GdkWindow *window, cairo_region_t *region, w_rect *rect,
		_w_control_reserved *reserved);
wresult _w_control_draw_draw_gripper(w_control *control, w_graphics *gc,
		w_rect *rect, wbool vertical, _w_control_reserved *reserved);
void _w_control_draw_widget(w_control *control, w_graphics *gc,
		_w_control_reserved *reserved);
wresult _w_control_enable_widget(w_control *control, wbool enabled,
		_w_control_reserved *reserved);
GtkWidget* _w_control_enterExitHandle(w_widget *control,
		_w_widget_reserved *reserved);
GdkWindow* _w_control_eventWindow(w_widget *control,
		_w_widget_reserved *reserved);
void _w_control_fix_focus(w_control *_this, w_control *focusControl,
		_w_control_reserved *reserved);
void _w_control_fix_style(w_control *control, _w_control_reserved *reserved);
void _w_control_fix_style_0(w_control *control, GtkWidget *handle,
		_w_control_reserved *reserved);
wresult _w_control_get_orientation(w_control *control);
wresult _w_control_get_text_direction(w_control *control);
wresult _w_control_has_focus(w_control *control, _w_control_reserved *reserved);
void _w_control_hook_events(w_widget *widget, _w_widget_reserved *reserved);
void* _w_control_hover_proc(GtkWidget *widget);
GtkWidget* _w_control_topHandle(w_widget *control,
		_w_widget_reserved *reserved);
GtkWidget* _w_control_paintHandle(w_widget *control,
		_w_widget_reserved *reserved);
GdkWindow* _w_control_paintWindow(w_widget *control,
		_w_widget_reserved *reserved);
wresult _w_control_print(w_control *control, w_graphics *gc);
w_widget* _w_control_compute_tab_group(w_control *control,
		_w_control_reserved *reserved);
w_control* _w_control_compute_tab_root(w_control *control,
		_w_control_reserved *reserved);
void _w_control_check_buffered(w_control *control,
		_w_control_reserved *reserved);
void _w_control_check_background(w_control *control,
		_w_control_reserved *reserved);
void _w_control_check_foreground(w_control *control,
		_w_control_reserved *reserved);
void _w_control_check_border(w_control *control, _w_control_reserved *reserved);
void _w_control_check_mirrored(w_control *control,
		_w_control_reserved *reserved);
GtkStyle* _w_control_child_style(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_create_widget(w_widget *widget, int index,
		_w_widget_reserved *reserved);
int _w_control_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_widget_reserved *reserved);
int _w_control_compute_native_size(w_widget *widget, GtkWidget *h,
		w_event_compute_size *e, _w_widget_reserved *reserved);
void _w_control_force_resize(w_control *control, _w_control_reserved *reserved);
wresult _w_control_get_accessible(w_control *control, w_accessible **accessible);
wresult _w_control_get_bounds(w_control *control,w_point* location,w_size* size);
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size);
void _w_control_mark_layout(w_control *control, int flags,
		_w_control_reserved *reserved);
void _w_control_modify_style(w_control *control, GtkWidget *handle,
		GtkRcStyle *style, _w_control_reserved *reserved);
void _w_control_move_handle(w_control *control, w_point *position,
		_w_control_reserved *reserved);
void _w_control_resize_handle(w_control *control, w_size *size,
		_w_control_reserved *reserved);
wresult _w_control_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_reserved *reserved);
wresult _w_control_set_region_0(w_control *control,
		cairo_region_t *shape_region);
wresult _w_control_set_region(w_control *control, w_region *region);
void _w_control_set_relations(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_is_described_by_label(w_control *control,
		_w_control_reserved *reserved);
gboolean _w_control_is_focus_handle(w_control *control, GtkWidget *widget,
		_w_control_reserved *reserved);
wresult _w_control_move_above(w_control *control, w_control *_control);
wresult _w_control_move_below(w_control *control, w_control *_control);
void _w_control_move_children(w_control *control, int oldWidth,
		_w_control_reserved *reserved);
wresult _w_control_pack(w_control *control, wbool changed);
wresult _w_control_new_layout_data(w_control *control, void **data,
		size_t size);
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point);
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point);
void _w_control_add_relation(w_control *_this, w_control *control,
		_w_control_reserved *reserved);
void _w_control_remove_relation(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_drag_detect_0(w_control *control, w_event *event);
gboolean _w_control_drag_detect_2(w_control *control, int x, int y,
		gboolean filter, gboolean dragOnTimeout, gboolean *consume);
gboolean _w_control_drag_detect_1(w_control *control, int button, int count,
		int stateMask, int x, int y, _w_control_reserved *reserved);
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event);
gboolean _w_control_filter_key(w_control *control, int keyval,
		GdkEventKey *event, _w_control_reserved *reserved);
w_control* _w_control_find_background_control(w_control *control,
		_w_control_reserved *reserved);
void _w_control_fixed_map_proc_callback(GtkWidget *child, gpointer data);
long _w_control_fixed_map_proc(GtkWidget *widget);
void _w_control_fix_modal(w_control *control, GtkWindowGroup *group,
		GtkWindowGroup *modalGroup, _w_control_reserved *reserved);
gboolean _w_control_force_focus_0(w_control *control, GtkWidget *focusHandle,
		_w_widget_reserved *reserved);
wresult _w_control_force_focus(w_control *control);
wresult _w_control_get_background(w_control *control, w_color *background);
wresult _w_control_get_background_image(w_control *control, w_image *image);
void _w_control_get_context_background(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
void _w_control_get_context_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
void _w_control_get_bg_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
void _w_control_get_background_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
wresult _w_control_get_border_width(w_control *control);
int _w_control_get_client_width(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor);
wresult _w_control_get_drag_detect(w_control *control);
wresult _w_control_get_enabled(w_control *control);
wresult _w_control_get_font(w_control *control, w_font **font);
const PangoFontDescription* _w_control_get_get_font_description(
		w_control *control, _w_control_reserved *reserved);
wresult _w_control_get_foreground(w_control *control, w_color *foreground);
void _w_control_get_fg_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
void _w_control_get_foreground_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
wresult _w_control_get_imcaret_pos(w_control *control, w_point *pos,
		_w_control_reserved *reserved);
void _w_control_get_text_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
wresult _w_control_get_layout_data(w_control *control, void **data);
wresult _w_control_get_menu(w_control *control, w_menu **menu);
wresult _w_control_get_parent(w_control *control, w_composite **parent);
wresult _w_control_get_region(w_control *control, w_region *region);
wresult _w_control_get_shell(w_control *control,w_shell**shell);
wresult _w_control_get_toolTip_text(w_control *control,w_alloc alloc,void* user_data);
wresult _w_control_get_touch_enabled(w_control *control);
wresult _w_control_get_visible(w_control *control);
void _w_control_get_thickness(GtkWidget *widget, w_point *thickness);
gboolean _w_control_check_subwindow(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_get_graphics(w_control *control, w_graphics *gc);
GtkIMContext* _w_control_imHandle(w_widget *control,
		_w_widget_reserved *reserved);
wresult _w_control_is_reparentable(w_control *control);
gboolean _w_control_is_showing(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_is_tab_group(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_is_tab_item(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_is_enabled(w_control *control);
wresult _w_control_is_focus_ancestor(w_control *_this, w_control *control);
wresult _w_control_is_focus_control(w_control *control);
wresult _w_control_is_visible(w_control *control);
gboolean _w_control_mnemonic_hit(w_control *control, int key,
		_w_control_reserved *reserved);
gboolean _w_control_mnemonic_match(w_control *control, int key,
		_w_control_reserved *reserved);
wresult _w_control_request_layout(w_control *control);
wresult _w_control_redraw(w_control *control, w_rect *r, int all);
void _w_control_redraw_children(w_control *control,
		_w_control_reserved *reserved);
void _w_control_redraw_widget(w_control *control, w_rect *_rect, int flags,
		_w_control_reserved *reserved);
gboolean _w_control_send_leave_notify(w_control *control,
		_w_control_reserved *reserved);
void gtk_widget_set_align(GtkWidget *widget, GtkAlign hAlign, GtkAlign vAlign);
void _w_control_set_background_0(w_control *control,
		_w_control_reserved *reserved);
void _w_control_set_background_1(w_control *control, w_color color,
		_w_control_reserved *reserved);
wresult _w_control_set_background(w_control *control, w_color color);
void _w_control_set_background_color_0(w_control *control,
		GtkStyleContext *context, GtkWidget *handle, GdkRGBA *rgba,
		_w_control_reserved *reserved);
void _w_control_set_background_color_gradient(w_control *control,
		GtkStyleContext *context, GtkWidget *handle, GdkRGBA *rgba,
		_w_control_reserved *reserved);
void gtk_css_provider_load_from_css(w_control *control,
		GtkStyleContext *context, const char *css);
void _w_control_set_background_color_1(w_control *control, GtkWidget *handle,
		GdkColor *color, _w_control_reserved *reserved);
void _w_control_set_background_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
wresult _w_control_set_background_image(w_control *control, w_image *image,
		_w_control_reserved *reserved);
wresult _w_control_set_background_pixmap(w_control *control, GdkPixbuf *image,
		_w_control_reserved *reserved);
wresult _w_control_set_capture(w_control *control, int capture);
wresult _w_control_set_cursor_0(w_control *control, GdkCursor *cursor,
		_w_control_reserved *reserved);
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor);
wresult _w_control_set_drag_detect(w_control *control, int dragDetect);
gboolean enterNotifyEventFunc(GSignalInvocationHint *ihint,
		guint n_param_values, const GValue *param_values, gpointer data);
wresult _w_control_set_enabled(w_control *_this, int enabled);
void _w_control_cleanup_enable_window(w_control *_this,
		_w_control_reserved *reserved);
wresult _w_control_set_focus(w_control *control);
wresult _w_control_set_font(w_control *control, w_font *font);
void _w_control_set_font_description(w_control *control,
		PangoFontDescription *font, _w_control_reserved *reserved);
wresult _w_control_set_foreground(w_control *control, w_color color);
void _w_control_set_foreground_color_0(w_control *control, GdkColor *color,
		_w_control_reserved *reserved);
void _w_control_set_foreground_color_1(w_control *control, GtkWidget *handle,
		GdkRGBA *rgba, _w_control_reserved *reserved);
void _w_control_set_initial_bounds(w_control *control,
		_w_control_reserved *reserved);
wresult _w_control_set_menu(w_control *control, w_menu *menu);
void _w_control_set_orientation_0(w_control *control, wbool create,
		_w_control_reserved *reserved);
wresult _w_control_set_orientation(w_control *control, int orientation);
wresult _w_control_set_parent(w_control *control, w_composite *parent);
void _w_control_set_parent_background(w_control *control,
		_w_control_reserved *reserved);
void _w_control_set_parent_window(w_control *control, GtkWidget *widget,
		_w_control_reserved *reserved);
gboolean _w_control_set_radio_selection(w_control *control, gboolean value,
		_w_control_reserved *reserved);
wresult _w_control_set_redraw(w_control *control, int redraw);
wresult _w_control_set_tab_item_focus(w_widget *widget, wbool next,
		_w_widget_reserved *reserved);
wresult _w_control_set_text_direction(w_control *control, int textDirection);
wresult _w_control_set_toolTip_text(w_control *control, const char *text,size_t length);
void _w_control_set_toolTip_text_0(w_control *control, w_shell *shell,
		const char *newString);
wresult _w_control_set_touch_enabled(w_control *control, int enabled);
wresult _w_control_set_visible(w_control *control, wbool visible);
wresult _w_control_set_ZOrder(w_control *control, w_control *sibling, int flags,
		_w_control_reserved *reserved);
void _w_control_set_widget_background(w_control *control,
		_w_control_reserved *reserved);
gboolean _w_control_show_menu(w_control *control, int x, int y, int detail);
void _w_control_show_widget(w_control *control, _w_control_reserved *reserved);
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event);
wresult _w_control_traverse_0(w_control *_this, int traversal, char character,
		int keyCode, int keyLocation, int stateMask, wbool doit,
		_w_control_reserved *reserved);
wresult _w_control_translate_mnemonic(w_control *_this, w_event_key *event,
		w_control *control, _w_control_reserved *reserved);
gboolean _w_control_translate_mnemonic_0(w_control *_this, int keyval,
		GdkEventKey *gdkEvent, _w_control_reserved *reserved);
wresult _w_control_translate_traversal(w_control *control,
		GdkEventKey *keyEvent, _w_control_reserved *reserved);
int _w_control_traversal_code(w_control *control, int key, GdkEventKey *event,
		_w_control_reserved *reserved);
wresult _w_control_traverse_1(w_control *control, w_event_key *event,
		_w_control_reserved *reserved);
wresult _w_control_traverse_escape(w_control *control, wbool next,
		_w_control_reserved *reserved);
w_control* computeTabList(w_control *root, wbool next, w_control *c);
wresult _w_control_traverse_group(w_control *control, wbool next,
		_w_control_reserved *reserved);
wresult _w_control_traverse_item(w_control *control, wbool next,
		_w_control_reserved *reserved);
gboolean _w_control_traverse_return(w_control *control,
		_w_control_reserved *reserved);
gboolean _w_control_traverse_page(w_control *control, gboolean next,
		_w_control_reserved *reserved);
gboolean _w_control_traverse_mnemonic(w_control *control, int key,
		_w_control_reserved *reserved);
wresult _w_control_update_0(w_control *control, wbool all, wbool flush);
wresult _w_control_update(w_control *control);
void _w_control_update_background_mode(w_control *control,
		_w_control_reserved *reserved);
void _w_control_update_layout(w_control *control, int flags,
		_w_control_reserved *reserved);
void _w_control_get_window_origin(w_control *control, w_point *pt,
		_w_control_reserved *reserved);
GdkWindow* _w_control_enableWindow(w_widget *control,
		_w_control_reserved *reserved);
GdkWindow* _w_control_redrawWindow(w_widget *control,
		_w_control_reserved *reserved);
gboolean _w_timer_listenner(gpointer user_data);
wresult _w_control_set_timer(w_control *control, wint64 ms, unsigned short id);
void _w_control_kill_all_timer(w_control *control);
wresult _w_control_kill_timer(w_control *control, unsigned short id);
wresult _w_control_set_tab(w_control *control, wbool tab);
wresult _w_control_get_tab(w_control *control);
/*
 * signals
 */
gboolean _gtk_control_destroy(w_widget *widget, _w_event_platform *ee,
		_w_widget_reserved *reserved);
gboolean _gtk_control_button_press_event_0(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved,
		wbool sendMouseDown);
gboolean _gtk_control_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved);
gboolean _gtk_control_commit(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_event_after(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_draw(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_focus(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_key_release_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_mnemonic_activate(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved);
gboolean _gtk_control_popup_menu(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_realize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_scroll_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_show_help(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_style_set(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_control_unrealize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
void _w_control_class_init(struct _w_control_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_CONTROL_H_ */
