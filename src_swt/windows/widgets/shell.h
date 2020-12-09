/*
 * shell.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_SHELL_H_
#define SRC_WIN32_WIDGETS_SHELL_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "canvas.h"
#include "tooltip.h"
struct _w_shell_accel_item {
	int accelerator;
	HMENU menu;
	int index;
	int id;
};
struct _w_shell_accel {
	int alloc;
	int count;
	struct _w_shell_accel_item acc[0];
};
wresult _w_menuitem_fill_accel(ACCEL *accel, struct _w_shell_accel_item *item);
typedef struct _w_shell {
	struct _w_canvas canvas;
	struct _w_shell *next;
	struct _w_shell *prev;
	struct _w_tooltip *tooltips;
	struct w_menu *menubar;
	struct _w_shell_accel *accel;
	w_button *defaultButton;
	int id;
	unsigned fullScreen :1;
	unsigned modified :1;
	unsigned opened :1;
	unsigned moved :1;
	unsigned resized :1;
	unsigned center :1;
	unsigned wasMaximized :1;
	unsigned showWithParent :1;
	unsigned swFlags : 4;
	int minWidth;
	int minHeight;
	w_rect oldrect;
	HACCEL hAccel;
	HIMC hIMC;
	HWND toolTipHandle;
	HWND balloonTipHandle;
	HWND menuItemToolTipHandle;
	WNDPROC windowProc;
} _w_shell;
typedef struct _w_shell_reserved _w_shell_reserved;
struct _w_shell_reserved {
	struct _w_canvas_reserved canvas;
	WNDPROC ToolTipProc;
	WNDPROC DialogProc;

};
#define _W_SHELL(x) ((_w_shell*)x)
#define _W_SHELL_RESERVED(x) ((_w_shell_reserved*)x)
#define _W_SHELL_GET_RESERVED(x) ((_w_shell_reserved*)_w_widget_get_reserved(W_WIDGET(x)))
/*
 *
 */
wresult _w_shell_create_embedded(w_widget *widget, w_widget *parent,
		wuint64 style, w_widget_post_event_proc post_event, void *handle,
		int embedded);
wresult _w_shell_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event);
wuint64 _w_shell_check_style(w_widget *widget, wuint64 style);
int _w_shell_add_accel(w_shell *shell, _w_menuitem *item, int accel);
int _w_shell_create_accelerators(w_shell *shell);
void _w_shell_destroy_accelerators(w_shell *shell);
wresult _w_shell_balloontip_handle(w_shell *shell, _w_shell_reserved *reserved);
wresult _w_shell_post_event_platform(w_widget *widget, _w_event_platform *ee,
		_w_widget_reserved *reserved);
wresult _w_shell_post_event(w_widget *widget, w_event *ee);
void _w_shell_center(w_shell *shell);
void _w_shell_close_widget(w_shell *shell, _w_event_platform *e);
wresult _w_shell_close(w_shell *shell);
wresult _w_shell_create_balloontip_handle(w_shell *shell,
		_w_shell_reserved *reserved);
void _w_shell_set_system_menu(w_shell *shell);
wresult _w_shell_create_handle(w_control *control,
		_w_control_reserved *reserved);
void _w_shell_create_tooltip(w_shell *shell, _w_tooltip *toolTip);
wresult _w_shell_create_tooltip_handle(HWND *hwnd, HWND parent,
		_w_shell_reserved *reserved);
void _w_shell_destroy_tooltip(w_shell *shell, _w_tooltip *toolTip);
wresult _w_shell_enable_widget(w_control *control, int enabled);
HBRUSH _w_shell_find_brush(wintptr value, int lbStyle);
w_control* _w_shell_find_background_control(w_control *control);
w_cursor* _w_shell_find_cursor(w_control *control);
w_control* _w_shell_find_theme_control(w_control *control);
w_tooltip* _w_shell_find_tooltip(w_shell *shell, int id);
void _w_shell_fix_active_shell(w_shell *shell);
void _w_shell_fix_shell(w_shell *shell, w_shell *newShell, w_control *control);
void _w_shell_fix_tooltip(w_shell *shell);
wresult _w_shell_force_active(w_shell *shell);
void _w_shell_force_resize(w_shell *shell);
wresult _w_shell_get_alpha(w_shell *shell);
wresult _w_shell_get_bounds(w_control *control, w_point *location,
		w_size *size);
w_tooltip* _w_shell_get_current_tooltip_0(w_shell *shell, HWND hwndToolTip);
w_tooltip* _w_shell_get_current_tooltip(w_shell *shell);
wresult _w_shell_get_enabled(w_control *control);
wresult _w_shell_get_full_screen(w_shell *shell);
wresult _w_shell_get_ime_input_mode(w_shell *shell);
wresult _w_shell_get_maximized(w_shell *shell);
wresult _w_shell_get_minimum_size(w_shell *shell, w_size *size);
wresult _w_shell_get_modified(w_shell *shell);
wresult _w_shell_get_region(w_control *control, w_region *region);
wresult _w_shell_get_shell(w_control *control, w_shell **shell);
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator);
wresult _w_shell_get_toolbar(w_shell *shell, w_toolbar **toolbar);
w_composite* _w_shell_find_deferred_control(w_composite *composite);
wresult _w_shell_is_enabled(w_control *control);
wresult _w_shell_is_visible(w_control *control);
wresult _w_shell_open(w_shell *shell);
wresult _w_shell_print(w_control *control, w_graphics *gc);
wresult _w_shell_release_brushes(w_control *control,
		_w_control_reserved *reserved);
void _w_shell_remove_menu(w_control *control, w_menu *menu,
		_w_control_reserved *reserved);
wresult _w_shell_request_layout(w_control *control);
LRESULT _w_shell_select_palette(w_control *control, _w_event_platform *e,
		HPALETTE hPalette);
wresult _w_shell_send_keyevent(w_control *control, int type,
		_w_event_platform *e, w_event *event);
wresult _w_shell_set_active(w_shell *shell);
void _w_shell_set_active_control(w_shell *shell, w_control *control, int type);
wresult _w_shell_set_alpha(w_shell *shell, int alpha);
wresult _w_shell_set_bounds(w_control *control, w_point *location, w_size *size,
		int flags, int defer);
wresult _w_shell_set_enabled(w_control *control, int enabled);
wresult _w_shell_set_full_screen(w_shell *shell, int fullScreen);
wresult _w_shell_set_ime_input_mode(w_shell *shell, int mode);
wresult _w_shell_set_minimum_size(w_shell *shell, w_size *size);
wresult _w_shell_set_modified(w_shell *shell, int modified);
void _w_shell_set_item_enabled(w_shell *shell, int cmd, int enabled);
void _w_shell_set_region(w_region *region);
void _w_shell_set_tooltip_text_1(w_shell *shell, HWND hwnd, const char *text,
		int length);
void _w_shell_set_tooltip_text_0(w_shell *shell, NMTTDISPINFOW *lpnmtdi,
		void *buffer);
void _w_shell_set_tooltip_title(w_shell *shell, HWND hwndToolTip,
		const char *text, int icon);
wresult _w_shell_set_visible_0(w_control *control, int visible);
wresult _w_shell_set_visible(w_control *control, int visible);
wresult _w_shell_subclass(w_control *control, _w_control_reserved *reserved);
HWND _w_shell_tooltip_handle(w_shell *shell);
wresult _w_shell_translate_accelerator(w_control *control, MSG *msg,
		_w_control_reserved *reserved);
wresult _w_shell_traverse_escape(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_unsubclass(w_control *control, _w_control_reserved *reserved);
void _w_shell_update_modal(w_shell *shell);
HWND _w_shell_widget_parent(w_control *control, _w_control_reserved *reserved);
DWORD _w_shell_widget_extstyle_0(w_control *control,
		_w_control_reserved *reserved);
DWORD _w_shell_widget_extstyle(w_control *control,
		_w_control_reserved *reserved);
const char* _w_shell_window_class(w_control *control,
		_w_control_reserved *reserved);
WNDPROC _w_shell_get_window_proc(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_window_proc(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
DWORD _w_shell_widget_style_0(w_control *control,
		_w_control_reserved *reserved);
DWORD _w_shell_widget_style(w_control *control, _w_control_reserved *reserved);
wresult _w_shell_get_default_button(w_shell *shell, w_button **button);
wresult _w_shell_get_images(w_shell *shell, w_image *images, size_t length);
wresult _w_shell_get_menu_bar(w_shell *shell, w_menu **menu);
wresult _w_shell_get_minimized(w_shell *shell);
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc, void *user_data,
		int enc);
wresult _w_shell_set_default_button_0(w_shell *shell, w_button *button,
		int save);
wresult _w_shell_set_default_button(w_shell *shell, w_button *button);
wresult _w_shell_set_images(w_shell *shell, w_image *images, size_t length);
wresult _w_shell_set_maximized_0(w_shell *shell, int maximized);
wresult _w_shell_set_maximized(w_shell *shell, int maximized);
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu);
wresult _w_shell_set_minimized_0(w_shell *shell, int minimized);
wresult _w_shell_set_minimized(w_shell *shell, int minimized);
wresult _w_shell_set_text(w_shell *shell, const char *text, size_t length,
		int enc);
wresult _w_shell_check_border(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_check_composited(w_control *control, w_composite *parent,
		_w_control_reserved *reserved);
wresult _w_shell_check_opened(w_control *control,
		_w_control_reserved *reserved);
w_widget* _w_shell_compute_tab_group(w_control *control,
		_w_control_reserved *reserved);
w_control* _w_shell_compute_tab_root(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved);
wresult _w_shell_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_widget_reserved *reserved);
wresult _w_shell_is_reparentable(w_control *control);
wresult _w_shell_is_tab_group(w_control *control,
		_w_control_reserved *reserved);
wresult _w_shell_is_tab_item(w_control *control, _w_control_reserved *reserved);
wresult _w_shell_set_orientation(w_control *control, int orientation);
wresult _w_shell_traverse_item(w_control *control, int next,
		_w_control_reserved *reserved);
wresult _w_shell_traverse_return(w_control *control,
		_w_control_reserved *reserved);
wresult _SHELL_WM_CLOSE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_KILLFOCUS(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_NCACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_QUERYOPEN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_SETFOCUS(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_ACTIVATE_0(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_ACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_DESTROY(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_ENTERIDLE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_GETMINMAXINFO(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_MOUSEACTIVATE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_MOVE_0(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_MOVE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_NCLBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_PALETTECHANGED(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_QUERYNEWPALETTE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_SHOWWINDOW(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_WINDOWPOSCHANGING_0(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
wresult _SHELL_WM_WINDOWPOSCHANGING(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
void _w_shell_class_init(struct _w_shell_class *clazz);
#endif
#endif /* SRC_WIN32_WIDGETS_SHELL_H_ */
