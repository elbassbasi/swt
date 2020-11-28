/*
 * widget.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_WIDGET_H_
#define SRC_WIN32_WIDGETS_WIDGET_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "../graphics/graphics.h"

#define STATE_DISPOSE_SENT (1 << 0)
#define STATE_DRAG_DETECT (1 << 1)
#define STATE_DISPOSED (1 << 2)
#define STATE_TRACK_MOUSE (1 << 2)
#define STATE_WIDGET_END (3)
enum {
	_WM_ACTIVATE = 1,
	_WM_CAPTURECHANGED,
	_WM_CHANGEUISTATE,
	_WM_CHAR,
	_WM_CLEAR,
	_WM_CLOSE,
	_WM_COMMAND,
	_WM_CONTEXTMENU,
	_WM_CTLCOLOR,
	_WM_CUT,
	_WM_CREATE,
	_WM_DESTROY,
	_WM_DRAWITEM,
	_WM_ENDSESSION,
	_WM_ENTERIDLE,
	_WM_ERASEBKGND,
	_WM_GESTURE,
	_WM_GETDLGCODE,
	_WM_GETFONT,
	_WM_GETOBJECT,
	_WM_GETMINMAXINFO,
	_WM_HELP,
	_WM_HSCROLL,
	_WM_IME_CHAR,
	_WM_IME_COMPOSITION,
	_WM_IME_COMPOSITION_START,
	_WM_IME_ENDCOMPOSITION,
	_WM_INITMENUPOPUP,
	_WM_INPUTLANGCHANGE,
	_WM_HOTKEY,
	_WM_KEYDOWN,
	_WM_KEYUP,
	_WM_KILLFOCUS,
	_WM_LBUTTONDBLCLK,
	_WM_LBUTTONDOWN,
	_WM_LBUTTONUP,
	_WM_MBUTTONDBLCLK,
	_WM_MBUTTONDOWN,
	_WM_MBUTTONUP,
	_WM_MEASUREITEM,
	_WM_MENUCHAR,
	_WM_MENUSELECT,
	_WM_MOUSEACTIVATE,
	_WM_MOUSEHOVER,
	_WM_MOUSELEAVE,
	_WM_MOUSEMOVE,
	_WM_MOUSEWHEEL,
	_WM_MOUSEHWHEEL,
	_WM_MOVE,
	_WM_NCACTIVATE,
	_WM_NCCALCSIZE,
	_WM_NCHITTEST,
	_WM_NCLBUTTONDOWN,
	_WM_NCPAINT,
	_WM_NOTIFY,
	_WM_PAINT,
	_WM_PALETTECHANGED,
	_WM_PARENTNOTIFY,
	_WM_PASTE,
	_WM_PRINT,
	_WM_PRINTCLIENT,
	_WM_QUERYENDSESSION,
	_WM_QUERYNEWPALETTE,
	_WM_QUERYOPEN,
	_WM_RBUTTONDBLCLK,
	_WM_RBUTTONDOWN,
	_WM_RBUTTONUP,
	_WM_SETCURSOR,
	_WM_SETFOCUS,
	_WM_SETFONT,
	_WM_SETTINGCHANGE,
	_WM_SETREDRAW,
	_WM_SHOWWINDOW,
	_WM_SIZE,
	_WM_SYSCHAR,
	_WM_SYSCOLORCHANGE,
	_WM_SYSCOMMAND,
	_WM_SYSKEYDOWN,
	_WM_SYSKEYUP,
	_WM_TABLET_FLICK,
	_WM_TIMER,
	_WM_TOUCH,
	_WM_UNDO,
	_WM_UNINITMENUPOPUP,
	_WM_UPDATEUISTATE,
	_WM_VSCROLL,
	_WM_WINDOWPOSCHANGED,
	_WM_WINDOWPOSCHANGING,
	_WM_XBUTTONDBLCLK,
	_WM_XBUTTONDOWN,
	_WM_XBUTTONUP,
	_WM_DPICHANGED,
	_WM_MENUCOMMAND,
	_WM_CTLCOLORCHILD,
	_WM_NOTIFYCHILD,
	_WM_COMMANDCHILD,
	_WM_DRAWCHILD,
	_WM_SCROLLCHILD,
	_WM_LAST
};

/* Default size for widgets */
#define DEFAULT_WIDTH 64
#define DEFAULT_HEIGHT 64

/* Bidi UCC to enforce text direction */
#define LRE 0x202a
#define RLE 0x202b

/* Bidi flag and for auto text direction */
#define AUTO_TEXT_DIRECTION (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)

typedef struct _w_event_platform {
	w_event event;
	UINT msg;
	LRESULT result;
	HWND hwnd;
	WPARAM wparam;
	LPARAM lparam;
} _w_event_platform;

typedef struct _w_widget {
	void *v_table; //used in c++ as virtual table
	struct _w_widget_class *clazz;
	volatile int ref;
	wuint id;
	wuint64 style;
	union {
		HWND handle;
		HMENU handleMenu;
	};
	w_widget_post_event_proc post_event;
	void *data[5];
	wuint state;
} _w_widget;

typedef struct _w_widget_reserved {
	void (*destroy)(w_widget *widget);
	void (*def_proc)(w_widget *widget, _w_event_platform *e,
			struct _w_widget_reserved *reserved);
} _w_widget_reserved;
typedef int (*dispatch_message)(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
#define _W_WIDGET(x) ((_w_widget*)x)
#define _W_WIDGET_RESERVED(x) ((struct _w_widget_reserved*)x)
struct _w_widget_reserved* _w_widget_get_reserved(w_widget *widget);
int _w_widget_send_event(struct w_widget *widget, struct w_event *event);
int _w_widget_post_event(struct w_widget *widget, struct w_event *e);
LRESULT CALLBACK _w_widget_proc(HWND hWnd, UINT message, WPARAM wParam,
		LPARAM lParam);
int _w_widget_check_bits(int style, int int0, int int1, int int2, int int3,
		int int4, int int5);
int w_min(int a, int b);
int w_max(int a, int b);
wresult _w_widgetdata_is_ok(w_widgetdata *obj);
void _w_widgetdata_close(w_widgetdata *obj);
int _w_numpad_key(int key);
int _w_translate_key(int key);
int _w_untranslate_key(int key);
int _w_ascii_key(int key);
int _w_shifted_key(int key);
int _w_control_key(int key);
void _w_set_input_state(w_event *event);
int _w_set_location_mask(w_event_key *event);
int _w_set_key_state(w_event_key *event);
int _w_send_key_event(w_event_key *event);
int _w_widget_show_menu(w_widget *widget, int x, int y, int detail);
int _WIDGET_WM_CAPTURECHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_CHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_CONTEXTMENU(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_IME_CHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_KEYDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_KEYUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_SYSCHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_SYSKEYDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_SYSKEYUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MOUSEMOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_LBUTTONDBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_LBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_LBUTTONUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MBUTTONDBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MBUTTONUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MOUSEHOVER(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MOUSELEAVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MOUSEWHEEL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_MOUSEHWHEEL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_RBUTTONDBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_RBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_RBUTTONUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_XBUTTONDBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_XBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_XBUTTONUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _WIDGET_WM_PAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _MENU_WM_MENUCOMMAND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _MENU_WM_INITMENUPOPUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _MENU_WM_UNINITMENUPOPUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
void _w_widget_class_init(struct _w_widget_class *clazz);
#endif
#endif /* SRC_WIN32_WIDGETS_WIDGET_H_ */
