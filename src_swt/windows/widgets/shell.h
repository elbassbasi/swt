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
wresult _w_menuitem_fill_accel (ACCEL* accel,struct _w_shell_accel_item* item);
typedef struct _w_shell {
	struct _w_canvas canvas;
	struct _w_shell *next;
	struct _w_shell *prev;
	struct _w_tooltip *tooltips;
	struct w_menu *menubar;
	struct _w_shell_accel *accel;
	int id;
	HACCEL hAccel;
	HWND toolTipHandle;
	HWND balloonTipHandle;
	HWND menuItemToolTipHandle;
	WNDPROC windowProc;
} _w_shell;

struct _w_shell_reserved {
	struct _w_canvas_reserved canvas;
	WNDPROC ToolTipProc;
	WNDPROC DialogProc;

};
#define _W_SHELL(x) ((_w_shell*)x)
#define _W_SHELL_RESERVED(x) ((struct _w_shell_reserved*)x)
/*
 *
 */
#define _W_SHELL_MAXIMUM_TRIM 128
#define _W_SHELL_BORDER 3
#define ISCUSTOMRESIZE(style) ((style & W_NO_TRIM) == 0 && (style & (W_RESIZE | W_ON_TOP)) == (W_RESIZE | W_ON_TOP))
wuint64 _w_shell_check_style(w_widget *widget, wuint64 style);
const char* _w_shell_window_class(w_widget *parent, int style);
void _w_shell_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle);
int _w_shell_add_accel(w_shell *shell, _w_menuitem *item, int accel);
int _w_shell_create_accelerators(w_shell *shell);
/*
 * public function
 */
wresult _w_shell_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event);
void _w_shell_close(w_shell *shell);
w_toolbar* _w_shell_get_toolbar(w_shell *shell);
int _w_shell_get_alpha(w_shell *shell);
int _w_shell_get_full_screen(w_shell *shell);
void _w_shell_get_minimum_size(w_shell *shell, w_size *size);
int _w_shell_get_modified(w_shell *shell);
int _w_shell_get_ime_input_mode(w_shell *shell);
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator);
void _w_shell_open(w_shell *shell);
void _w_shell_set_active(w_shell *shell);
void _w_shell_set_alpha(w_shell *shell, int alpha);
void _w_shell_set_full_screen(w_shell *shell, int fullScreen);
void _w_shell_set_ime_input_mode(w_shell *shell, int mode);
void _w_shell_set_minimum_size(w_shell *shell, w_size *size);
void _w_shell_set_modified(w_shell *shell, int modified);
void _w_shell_force_active(w_shell *shell);
w_button* _w_shell_get_default_button(w_shell *shell);
w_image* _w_shell_get_image(w_shell *shell);
int _w_shell_get_maximized(w_shell *shell);
w_menu* _w_shell_get_menu_bar(w_shell *shell);
int _w_shell_get_minimized(w_shell *shell);
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc, void *user_data,
		int enc);
wresult _w_shell_set_default_button(w_shell *shell, struct w_button *button);
wresult _w_shell_set_image(w_shell *shell, w_image *image);
wresult _w_shell_set_maximized(w_shell *shell, int maximized);
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu);
wresult _w_shell_set_minimized(w_shell *shell, int minimized);
wresult _w_shell_set_text(w_shell *shell, const char *text, size_t length,
		int enc);
wresult _w_shell_get_shell(w_control *control,w_shell** shell);
int _SHELL_WM_CLOSE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_NCACTIVATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_QUERYOPEN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_SYSCOMMAND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _wm_shell_ACTIVATE_0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_ACTIVATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_DESTROY(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_ERASEBKGND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_ENTERIDLE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_GETMINMAXINFO(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_MOUSEACTIVATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _wm_shell_MOVE_0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_MOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_NCHITTEST(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_NCLBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_PALETTECHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_QUERYNEWPALETTE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_SETCURSOR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_SHOWWINDOW(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _wm_shell_WINDOWPOSCHANGING_0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _SHELL_WM_WINDOWPOSCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
void _w_shell_class_init(struct _w_shell_class *clazz);
#endif
#endif /* SRC_WIN32_WIDGETS_SHELL_H_ */
