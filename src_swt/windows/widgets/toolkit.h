/*
 * toolkit.h
 *
 *  Created on: 28 févr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_TOOLKIT_H_
#define SRC_WIN32_WIDGETS_TOOLKIT_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "shell.h"
#include "taskbar.h"
#include "tooltip.h"
#include "tracker.h"
#include "tray.h"
#include "glcanvas.h"
#include "ccanvas.h"
#include "../controls/controls.h"
extern const char *WindowName; //$NON-NLS-1$
extern const char *WindowShadowName; //$NON-NLS-1$
extern const char *WindowOwnDCName; //$NON-NLS-1$
extern struct _w_theme_class win32_theme;

#define _W_CLASS_DECL(x) struct _w_##x##_class class_##x;\
	struct _w_##x##_reserved class_##x##_reserved
#define _W_CLASS_DECL_ITEM(x) _W_CLASS_DECL(x);\
		struct _w_##x##item_class class_##x##item
#define BRUSHES_SIZE 32
typedef struct _w_toolkit {
	w_toolkit toolkit;
	int win32_version;
	int comctrl32_version;
	MSG Msg;
	int stop;
	RECT clickRect;
	/* Focus */
	int focusEvent;
	w_control *focusControl;
	w_shell *activeShell;
	_w_shell *shells;
	int shells_count;
	w_widget *widget_free;
	w_thread thread;
	int lastMouse;
	int lastAscii;
	int lastKey;
	int clickCount;
	int lastTime;
	int lastButton;
	int scrollRemainder;
	int scrollHRemainder;
	HWND lastClickHwnd;
	unsigned activePending :1;
	unsigned ignoreActivate :1;
	unsigned ignoreFocus :1;
	unsigned captureChanged :1;
	unsigned xMouse :1;
	unsigned is_class_registred :1;
	unsigned lastVirtual :1;
	unsigned lastNull :1;
	unsigned lastDead :1;
	unsigned mnemonicKeyHit :1;
	w_theme systemtheme;
	HTHEME themes[_W_THEME_CLASS_LAST];
	HBRUSH brushes[BRUSHES_SIZE];
	unsigned char wm_msg[WM_USER + 0x10];
	struct _w_font systemfont;
	struct _w_cursor cursors[W_CURSOR_HAND + 1];
	struct _w_widget_class *classes[_W_CLASS_LAST];
	struct _w_toolkit_class class_toolkit;
	struct _w_scrollbar_class class_scrollbar; //
	_W_CLASS_DECL(shell); //
	_W_CLASS_DECL(button); //
	_W_CLASS_DECL(sash); //
	_W_CLASS_DECL(browser); //
	_W_CLASS_DECL_ITEM(menu); //
	_W_CLASS_DECL_ITEM(tree); //
	struct _w_treecolumn_class class_treecolumn; //
	_W_CLASS_DECL(tabfolder); //
	struct _w_tabitem_class class_tabfolderitem; //
	_W_CLASS_DECL(composite); //
	_W_CLASS_DECL(canvas); //
	_W_CLASS_DECL(ccanvas); //
	_W_CLASS_DECL(glcanvas); //
	_W_CLASS_DECL_ITEM(combo); //
	_W_CLASS_DECL(coolbar); //
	struct _w_coolitem_class class_coolbaritem; //
	_W_CLASS_DECL(datetime); //
	_W_CLASS_DECL(expandbar); //
	struct _w_expanditem_class class_expandbaritem; //
	_W_CLASS_DECL(group); //
	_W_CLASS_DECL(label); //
	_W_CLASS_DECL(link); //
	_W_CLASS_DECL(list); //
	struct _w_listitem_class class_listitem; //
	_W_CLASS_DECL(progressbar); //
	_W_CLASS_DECL(scale); //
	_W_CLASS_DECL(slider); //
	_W_CLASS_DECL(spinner); //
	_W_CLASS_DECL_ITEM(table); //
	struct _w_tablecolumn_class class_tablecolumn; //
	_W_CLASS_DECL(taskbar); //
	struct _w_taskitem_class class_taskbaritem; //
	_W_CLASS_DECL(text); //
	_W_CLASS_DECL(toolbar); //
	struct _w_toolitem_class class_toolbaritem; //
	_W_CLASS_DECL(tooltip); //
	_W_CLASS_DECL(tracker); //
	_W_CLASS_DECL(tray); //
	struct _w_trayitem_class class_trayitem; //
	size_t tmp_alloc;
	size_t tmp_length;
	char tmp[0];
} _w_toolkit;
extern _w_toolkit *win_toolkit;
typedef struct _w_shells_iterator {
	w_basic_iterator base;
	_w_shell *parent;
	_w_shell *current;
	int count;
} _w_shells_iterator;
#define _W_SHELLS_ITERATOR(it) ((_w_shells_iterator*)it)
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator);
void _w_toolkit_add_shell(_w_shell *shell);
void _w_toolkit_remove_shell(_w_shell *shell);
#define _W_TOOLKIT(x) ((_w_toolkit*)x)
#define WIN32_VERSION (win_toolkit->win32_version)
#define W_COMCTL32_VERSION (win_toolkit->comctrl32_version)
HICON _w_toolkit_get_close_button(_w_toolkit *toolkit);
void _w_toolkit_init(_w_toolkit *toolkit);
void _w_toolkit_class_init(struct _w_toolkit *toolkit);
void _w_toolkit_widget_class_init(_w_toolkit *toolkit);
void _w_toolkit_init_dll(_w_toolkit *toolkit);
void _w_toolkit_registre_class(_w_toolkit *toolkit);
void _w_toolkit_registre_free(w_widget *widget, w_widget_post_event_proc _proc);
void* _w_toolkit_malloc(size_t size);
void* _w_toolkit_malloc_all(size_t *size);
void _w_toolkit_free(void *ptr,size_t size);
HTHEME _w_theme_get_htheme(int index, const char *clazz);
/**
 * public function
 */
void _w_toolkit_dispose(w_toolkit *_toolkit);
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget);
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata);
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id);
struct w_theme* _w_toolkit_get_theme(w_toolkit *toolkit);
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args);
wresult _w_toolkit_beep(w_toolkit *toolkit);
wresult _w_toolkit_close(w_toolkit *toolkit);
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit);
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit);
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds);
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea);
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit);
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location);
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length);
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit);
int _w_toolkit_get_double_click_time(w_toolkit *toolkit);
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit);
wresult _w_toolkit_get_high_contrast(w_toolkit *toolkit);
int _w_toolkit_get_icon_depth(w_toolkit *toolkit);
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length);
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells);
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id);
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id);
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit);
wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image *image);
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit);
struct w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit);
struct w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit);
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit);
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit);
wresult _w_toolkit_get_touch_enabled(w_toolkit *toolkit);
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point);
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle);
wresult _w_toolkit_post(w_toolkit *toolkit, w_event *event);
wresult _w_toolkit_post_quit(w_toolkit *toolkit, int quit);
wresult _w_toolkit_read_and_dispatch(w_toolkit *toolkit);
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point);
int _w_toolkit_run(w_toolkit *toolkit);
wresult _w_toolkit_sleep(w_toolkit *toolkit);
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args);
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args);
wresult _w_toolkit_update(w_toolkit *toolkit);
wresult _w_toolkit_wake(w_toolkit *toolkit);
#endif
#endif /* SRC_WIN32_WIDGETS_TOOLKIT_H_ */
