/*
 * toolkit.h
 *
 *  Created on: 28 févr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_TOOLKIT_H_
#define SRC_GTK_WIDGETS_TOOLKIT_H_

#ifdef __linux
#include "shell.h"
#include "theme.h"
#include "taskbar.h"
#include "tooltip.h"
#include "tracker.h"
#include "tray.h"
#include "glcanvas.h"
#include "ccanvas.h"
#include "../controls/controls.h"
struct _w_signal_closures {
	guint signal_id;
	GClosure *widget_closures;
	GClosure *control_closures;
};
#define _W_CLASS_DECL(x) struct _w_##x##_class class_##x;\
	struct _w_##x##_reserved class_##x##_reserved
#define _W_CLASS_DECL_ITEM(x) _W_CLASS_DECL(x);\
		struct _w_##x##item_class class_##x##item

struct threads_idle {
	w_thread_start func;
	void *data;
	wuint signalled;
};
void GLogFunc_Empty(const gchar *log_domain, GLogLevelFlags log_level,
		const gchar *message, gpointer user_data);
enum {
	W_DISPLAY_UNKNOWN,
	W_DISPLAY_X11,
	W_DISPLAY_WAYLAND,
	/*
	 * trim
	 */
	TRIM_NONE = 0,
	TRIM_BORDER = 1,
	TRIM_RESIZE = 2,
	TRIM_TITLE_BORDER = 3,
	TRIM_TITLE_RESIZE = 4,
	TRIM_TITLE = 5
};

typedef struct _w_toolkit {
	w_toolkit toolkit;
	int version;
	int exit_code;
	/*
	 * 0 : index of widget
	 * 1 : id
	 *
	 */
	GQuark quark[4];
	char *commandline;
	/* Focus */
	int focusEvent;
	w_control *focusControl;
	w_control *currentControl;
	w_control *mnemonicControl;
	w_control *imControl;
	w_caret *currentCaret;
	w_widget *widget_free;
	void *libX11;
	guint caretId;
	w_shell *activeShell;
	_w_shell *shells;
	int shells_count;
	_w_shell *modalShells;
	struct _w_font system_font;
	unsigned activePending :1;
	unsigned ignoreActivate :1;
	unsigned ignoreFocus :1;
	unsigned display_type :2;
	unsigned ignoreTrim :1;
	unsigned modalDialog :1;
	unsigned exit_loop : 1;
	unsigned wake : 1;
	int clickCount;
	int lastUserEventTime;
	gint latinKeyGroup;
	PangoTabArray *empty_tab;
	struct _w_signal_closures closures[LAST_SIGNAL];
	_w_cursor system_cursors[W_CURSOR_HAND + 1];
	int trimWidths[6];
	int trimHeights[6];
	int dispatchEvents[10];
	/* Custom Resize */
	double resizeLocationX, resizeLocationY;
	int resizeBoundsX, resizeBoundsY, resizeBoundsWidth, resizeBoundsHeight;
	int resizeMode;
	/*
	 * theme
	 */
	_w_theme_gtk theme;
	GtkWidget *shellHandle, *tooltipShellHandle, *fixedHandle, *buttonHandle,
			*arrowHandle, *frameHandle, *entryHandle, *checkButtonHandle,
			*radioButtonHandle, *notebookHandle, *treeHandle, *progressHandle,
			*toolbarHandle, *labelHandle, *separatorHandle;
	/*
	 * threads
	 */
	struct threads_idle threads_idle[0x10];
	pthread_mutex_t mutex;
	pthread_cond_t condition;
	pthread_mutex_t condition_mutex;
	w_thread guithread;
	GPollFD* fds;
	size_t allocated_nfds;
	GPollFD fds_tmp[0x10];

	/*
	 * clipboard
	 */
	GtkClipboard *GTKCLIPBOARD;
	GtkClipboard *GTKPRIMARYCLIPBOARD;
	GtkClipboard *TARGET;
	/*
	 * classes
	 */
	struct _w_widget_class *classes[_W_CLASS_LAST];
	struct _w_toolkit_class class_toolkit;
	/*
	 * shell
	 */
	struct _w_shell_class class_shell;
	struct _w_shell_reserved class_shell_reserved;
	/*
	 * canvas
	 */
	struct _w_canvas_class class_canvas;
	struct _w_canvas_reserved class_canvas_reserved;
	struct _w_caret_class class_caret;
	struct _w_ime_class class_ime;
	/*
	 * composite
	 */
	struct _w_composite_class class_composite;
	struct _w_composite_reserved class_composite_reserved;
	struct _w_scrollbar_class class_scrollbar;
	/*
	 * ccanvas
	 */
	struct _w_ccanvas_class class_ccanvas;
	struct _w_ccanvas_reserved class_ccanvas_reserved;
	/*
	 * dragsource
	 */
	struct _w_dragsource_class class_dragsource;
	/*
	 * droptarget
	 */
	struct _w_droptarget_class class_droptarget;
	/*
	 *
	 */
	struct _w_tabitem_class class_tabfolderitem;
	struct _w_treecolumn_class class_treecolumn; //

	_W_CLASS_DECL(button); //
	_W_CLASS_DECL_ITEM(menu); //
	_W_CLASS_DECL(browser); //
	_W_CLASS_DECL_ITEM(tree); //
	_W_CLASS_DECL(tabfolder); //
	_W_CLASS_DECL(sash); //
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
	/*
	 * temporary memory
	 */
	size_t mem_tmp_total_size;
	size_t mem_tmp_allocated;
	char mem_tmp[0];
} _w_toolkit;
extern _w_toolkit *gtk_toolkit;
extern _w_theme_class gtk_theme;
typedef struct _w_shells_iterator {
	w_basic_iterator base;
	_w_shell *parent;
	_w_shell *current;
	int count;
} _w_shells_iterator;
#define _W_SHELLS_ITERATOR(it) ((_w_shells_iterator*)it)
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator);

#define _W_TOOLKIT(x) ((_w_toolkit*)x)
#define GTK_VERSION (gtk_toolkit->version)
#define isX11() (gtk_toolkit->display_type == W_DISPLAY_X11)
#define isWayland() (gtk_toolkit->display_type == W_DISPLAY_WAYLAND)
#define threads_idle_count (sizeof(gtk_toolkit->threads_idle)/ sizeof(gtk_toolkit->threads_idle[0]))
void _w_toolkit_init(_w_toolkit *toolkit);
void _w_toolkit_init_trims(_w_toolkit *toolkit);
void _w_toolkit_class_init(_w_toolkit *toolkit);
void _w_toolkit_subclass_init(_w_toolkit *toolkit);
void _w_toolkit_init_widget(_w_toolkit *toolkit);
wresult _w_toolkit_set_warnings(_w_toolkit *toolkit, int warning);
w_color _w_toolkit_GtkStyleContext(GtkStyleContext *context,
		GtkStateFlags flag);
w_color _w_toolkit_getBackgroundColor(GtkStyleContext *context,
		GtkStateFlags state);
w_color toGdkRGBA(GdkRGBA rgba, double brightness);
gboolean _w_toolkit_async_exec_GSourceFunc(gpointer user_data);
gboolean _w_toolkit_sync_exec_GSourceFunc(gpointer user_data);
void _w_toolkit_set_modal_shell(struct _w_toolkit *toolkit, w_shell *shell);
void _w_toolkit_clear_modal(struct _w_toolkit *toolkit, w_shell *shell);
void _w_toolkit_add_shell(_w_shell *shell);
void _w_toolkit_remove_shell(_w_shell *shell);
void _w_toolkit_put_gdk_events();
void _w_toolkit_registre_free(w_widget *widget);

/**
 * public function
 */
void _w_toolkit_dispose(w_disposable *_toolkit);
void* _w_toolkit_malloc(w_toolkit *toolkit, size_t size);
void* _w_toolkit_realloc(w_toolkit *toolkit, void *ptr, size_t size);
void _w_toolkit_free(w_toolkit *toolkit, void *ptr);
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
/*
 *
 */
wresult _w_toolkit_open_clipboard(w_toolkit *toolkit, w_clipboard *clipboard,
		int clipboards);
int _w_messagebox_open(w_toolkit *toolkit, w_messagebox *messagebox);
#endif
#endif /* SRC_GTK_WIDGETS_TOOLKIT_H_ */
