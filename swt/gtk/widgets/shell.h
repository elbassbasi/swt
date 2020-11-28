/*
 * shell.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_SHELL_H_
#define SRC_GTK_WIDGETS_SHELL_H_

#ifdef __linux
#include "canvas.h"
#include "tooltip.h"
typedef struct _w_shell {
	struct _w_canvas canvas;
	struct _w_shell *next;
	struct _w_shell *prev;
	struct _w_shell *next_modal;
	struct _w_tooltip *tooltips;
	struct w_menu *menubar;
	GtkAccelGroup *accelGroup;
	GtkWindowGroup *group;
	GtkWindowGroup *modalGroup;
	GtkWidget *tooltipsHandle;
	w_button *defaultButton;
	w_button *saveDefault;
	w_control *lastActive;
	w_control *savedFocus;
	cairo_region_t *originalRegion;
	unsigned mapped :1;
	unsigned moved :1;
	unsigned resized :1;
	unsigned opened :1;
	unsigned fullScreen :1;
	unsigned showWithParent :1;
	unsigned modified :1;
	unsigned center :1;
	unsigned minimized :1;
	unsigned maximized :1;
	unsigned ignoreFocusOut :1;
	int minWidth;
	int minHeight;
	w_rect oldb;
} _w_shell;

typedef struct _w_shell_reserved {
	struct _w_canvas_reserved canvas;
	int move_focus_id;
} _w_shell_reserved;

#define _W_SHELL(x) ((_w_shell*)x)
#define _W_SHELL_RESERVED(x) ((struct _w_shell_reserved*)x)
//state
#define STATE_MODAL_SHELL (1 << (STATE_CANVAS_END + 1))
#define STATE_FOREIGN_HANDLE (1 << (STATE_CANVAS_END + 2))
#define STATE_SHELL_END (STATE_CANVAS_END + 2)
//
#define _W_SHELL_MAXIMUM_TRIM 128
#define _W_SHELL_BORDER 3
#define _W_SHELL_HANDLE(x) _w_widget_hppp(W_WIDGET(x),0)
#define _W_SHELL_VBOX(x) _w_widget_hpp(W_WIDGET(x),0)
#define ISCUSTOMRESIZE(style) ((style & W_NO_TRIM) == 0 && (style & (W_RESIZE | W_ON_TOP)) == (W_RESIZE | W_ON_TOP))
#define isUndecorated(style) ((style & (W_SHELL_TRIM | W_BORDER)) == W_NONE || \
		(style & (W_NO_TRIM | W_ON_TOP)) != 0)

#define GDK_NO_EXPOSE 30
/*
 * functions
 */
wresult _w_shell_create_embedded(w_widget *widget, w_widget *parent,
		wuint64 style, w_widget_post_event_proc post_event, void *handle,
		int embedded);
wresult _w_shell_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event);
wuint64 _w_shell_check_style(w_widget *control, wuint64 style);
void _w_shell_add_tooltip(w_shell *shell, _w_tooltip *toolTip);
void _w_shell_adjust_trim(w_shell *shell);
void _w_shell_bring_totop(w_shell *shell, int force);
void _w_shell_center(w_control *control);
void _w_shell_check_border(w_control *control, _w_control_reserved *reserved);
void _w_shell_check_open(w_widget *widget, _w_widget_reserved *reserved);
GtkStyle* _w_shell_child_style(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_close(w_shell *shell);
void _w_shell_close_widget(w_shell *shell, _w_event_platform *e);
wresult _w_shell_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved);
wresult _w_shell_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved);
w_control* _w_shell_find_background_control(w_control *control,
		_w_control_reserved *reserved);
w_composite* _w_shell_find_deferred_control(w_composite *composite,
		_w_composite_reserved *reserved);
wresult _w_shell_get_toolbar(w_shell *shell, w_toolbar **toolbar);
wresult _w_shell_has_border(w_composite *composite,
		_w_composite_reserved *reserved);
void _w_shell_hook_events(w_widget *shell, _w_widget_reserved *reserved);
wresult _w_shell_is_enabled(w_control *shell);
wresult _w_shell_is_visible(w_control *shell);
wresult _w_shell_request_layout(w_control *shell);
void _w_shell_fix_active_shell(w_shell *_shell);
void _w_shell_fix_shell(w_shell *shell, w_shell *newShell, w_control *control);
void _w_shell_fix_style_0(w_control *control, GtkWidget *handle,
		_w_control_reserved *reserved);
void _w_shell_force_resize(w_control *control, _w_control_reserved *reserved);
void _w_shell_force_resize_0(w_control *control, int width, int height,
		_w_control_reserved *reserved);
wresult _w_shell_get_alpha(w_shell *shell);
int _w_shell_get_resize_mode(w_shell *shell, double x, double y);
wresult _w_shell_get_full_screen(w_shell *shell);
wresult _w_shell_get_maximized(w_shell *shell);
wresult _w_shell_get_minimum_size(w_shell *shell, w_size *size);
w_shell* _w_shell_get_modal_shell(w_shell *_this);
wresult _w_shell_get_modified(w_shell *shell);
wresult _w_shell_get_visible(w_control *shell);
wresult _w_shell_get_region(w_control *shell, w_region *region);
wresult _w_shell_get_ime_input_mode(w_shell *shell);
wresult _w_shell_get_shell(w_control *control, w_shell **shell);
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator);
wresult _w_shell_open(w_shell *shell);
wresult _w_shell_print(w_control *shell, w_graphics *gc);
void _w_shell_remove_tooltip(w_shell *shell, _w_tooltip *toolTip);
wresult _w_shell_set_active(w_shell *shell);
void _w_shell_set_active_control(w_shell *shell, w_control *control);
void _w_shell_set_active_control_0(w_shell *shell, w_control *control,
		int type);
wresult _w_shell_set_alpha(w_shell *shell, int alpha);
void _w_shell_resize_bounds(w_control *control, int width, int height,
		int notify, _w_control_reserved *reserved);
wresult _w_shell_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_reserved *reserved);
wresult _w_shell_set_cursor(w_control *control, GdkCursor *cursor,
		_w_control_reserved *reserved);
wresult _w_shell_set_enabled(w_control *control, int enabled);
wresult _w_shell_set_full_screen(w_shell *shell, int fullScreen);
wresult _w_shell_set_ime_input_mode(w_shell *shell, int mode);
void _w_shell_set_initial_bounds(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_set_maximized(w_shell *shell, int maximized);
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu);
wresult _w_shell_set_minimized(w_shell *shell, int minimized);
wresult _w_shell_set_minimum_size(w_shell *shell, w_size *size);
wresult _w_shell_set_modified(w_shell *shell, int modified);
wresult _w_shell_set_region(w_control *shell, w_region *_region);
wresult _w_shell_mirror_region(w_region *region, cairo_region_t **mirrored);
void _w_shell_set_relations(w_control *control, _w_control_reserved *reserved);
wresult _w_shell_set_text(w_shell *shell, const char *string, size_t length,
		int enc);
wresult _w_shell_set_text_unicode(w_shell *shell, const wchar_t *string,
		size_t length);
wresult _w_shell_set_visible(w_control *control, int visible);
wresult _w_shell_set_ZOrder(w_control *control, w_control *sibling, int flags,
		_w_control_reserved *reserved);
void* _w_shell_map_proc(GtkWidget *handle, void *arg0, void *user_data);
void _w_shell_show_widget(w_control *control, _w_control_reserved *reserved);
GtkWidget* _w_shell_size_allocate_proc(GtkWidget *handle,
		GtkAllocation *allocation);
GtkWidget* _w_shell_size_request_proc(GtkWidget *handle,
		GtkAllocation *allocation);
wresult _w_shell_traverse_escape(w_control *control,
		_w_control_reserved *reserved);
int _w_shell_trim_height(w_shell *shell);
int _w_shell_trim_width(w_shell *shell);
void _w_shell_update_modal(w_shell *shell);
void _w_shell_update_minimized(w_shell *shell, int minimized);
wresult _w_shell_force_active(w_shell *shell);
wresult _w_shell_get_bounds(w_control *control,w_point *location, w_size *size);
void _w_shell_set_tooltip_text(w_shell *shell, GtkWidget *tipWidget,
		const char *string);
void _w_shell_set_tooltip_text_0(w_shell *shell, GtkWidget *rootWidget,
		GtkWidget *tipWidget, const char *string);
void _w_shell_get_window_origin(w_control *control, w_point *pt,
		_w_control_reserved *reserved);
gint _w_shell_image_compare(gconstpointer data1, gconstpointer data2);
w_widget* _w_shell_compute_tab_group(w_control *control,
		_w_control_reserved *reserved);
w_control* _w_shell_compute_tab_root(w_control *control,
		_w_control_reserved *reserved);
GtkAccelGroup* _w_shell_create_accel_group(w_shell *shell);
void _w_shell_destroy_accel_group(w_shell *shell);
void _w_shell_fix_accel_group(w_shell *shell);
wresult _w_shell_get_default_button(w_shell *shell,w_button** _button);
void _w_shell_images_destroy(gpointer data);
wresult _w_shell_get_images(w_shell *shell, w_image *images, size_t length);
wresult _w_shell_get_menu_bar(w_shell *shell,w_menu** menu);
wresult _w_shell_get_minimized(w_shell *shell);
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc,void* user_data,int enc);
wresult _w_shell_is_reparentable(w_control *shell);
wresult _w_shell_is_tab_group(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_is_tab_item(w_control *control, _w_control_reserved *reserved);
int _w_shell_restore_focus(w_shell *shell);
wresult _w_shell_set_default_button(w_shell *shell, w_button *button);
wresult _w_shell_set_images(w_shell *shell, w_image *images, size_t length);
void _w_shell_set_orientation(w_control *control, int create,
		_w_control_reserved *reserved);
void _w_shell_set_saved_focus(w_shell *shell, w_control *control);
wresult _w_shell_traverse_item(w_control *control, int next,
		_w_control_reserved *reserved);
wresult _w_shell_traverse_return(w_control *control,
		_w_control_reserved *reserved);
/*
 * signals
 */
gboolean _gtk_shell_destroy(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_configure_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_delete_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_draw(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_focus(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_move_focus(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_motion_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_size_allocate(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_realize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_shell_window_state_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
void _w_shell_class_init(struct _w_shell_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_SHELL_H_ */
