/*
 * widget.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_WIDGET_H_
#define SRC_GTK_WIDGETS_WIDGET_H_

#ifdef __linux
#include "custom.h"
enum {
	/*
	 * 2 arguments
	 */
	SIGNAL_2_ARGS = 1, //
	SIGNAL_ACTIVATE = SIGNAL_2_ARGS, //
	SIGNAL_DESTROY, //widget
	SIGNAL_CHANGED,
	SIGNAL_CLICKED,
	SIGNAL_GRAB_FOCUS,
	SIGNAL_HIDE,
	SIGNAL_MAP,
	SIGNAL_OUTPUT,
	SIGNAL_POPUP_MENU,
	SIGNAL_PREEDIT_CHANGED,
	SIGNAL_REALIZE,
	SIGNAL_SELECT,
	SIGNAL_SHOW,
	SIGNAL_UNMAP,
	SIGNAL_UNREALIZE,
	SIGNAL_VALUE_CHANGED,
	SIGNAL_ACTIVATE_INVERSE,
	SIGNAL_DAY_SELECTED,
	SIGNAL_MONTH_CHANGED,
	SIGNAL_DAY_SELECTED_DOUBLE_CLICK,
	SIGNAL_SELECTION_DONE,
	SIGNAL_START_INTERACTIVE_SEARCH,
	SIGNAL_BACKSPACE,
	SIGNAL_BACKSPACE_INVERSE,
	SIGNAL_COPY_CLIPBOARD,
	SIGNAL_COPY_CLIPBOARD_INVERSE,
	SIGNAL_CUT_CLIPBOARD,
	SIGNAL_CUT_CLIPBOARD_INVERSE,
	SIGNAL_PASTE_CLIPBOARD,
	SIGNAL_PASTE_CLIPBOARD_INVERSE,
	SIGNAL_CREATE_MENU_PROXY,
	/*
	 * 3 arguments
	 */
	SIGNAL_3_ARGS,
	SIGNAL_BUTTON_PRESS_EVENT = SIGNAL_3_ARGS, //widget
	SIGNAL_BUTTON_PRESS_EVENT_INVERSE, //widget
	SIGNAL_BUTTON_RELEASE_EVENT, //widget
	SIGNAL_BUTTON_RELEASE_EVENT_INVERSE, //widget
	SIGNAL_COMMIT,
	SIGNAL_CONFIGURE_EVENT,
	SIGNAL_DELETE_EVENT,
	SIGNAL_ENTER_NOTIFY_EVENT,
	SIGNAL_EVENT,
	SIGNAL_EVENT_AFTER,
	SIGNAL_EXPOSE_EVENT,
	SIGNAL_EXPOSE_EVENT_INVERSE,
	SIGNAL_DRAW,
	SIGNAL_FOCUS,
	SIGNAL_FOCUS_IN_EVENT,
	SIGNAL_FOCUS_OUT_EVENT,
	SIGNAL_INPUT,
	SIGNAL_KEY_PRESS_EVENT,
	SIGNAL_KEY_RELEASE_EVENT,
	SIGNAL_LEAVE_NOTIFY_EVENT,
	SIGNAL_MAP_EVENT,
	SIGNAL_MNEMONIC_ACTIVATE,
	SIGNAL_MOTION_NOTIFY_EVENT,
	SIGNAL_MOTION_NOTIFY_EVENT_INVERSE,
	SIGNAL_MOVE_FOCUS,
	SIGNAL_POPULATE_POPUP,
	SIGNAL_SCROLL_EVENT,
	SIGNAL_SHOW_HELP,
	SIGNAL_SIZE_ALLOCATE,
	SIGNAL_STYLE_SET,
	SIGNAL_TOGGLED,
	SIGNAL_UNMAP_EVENT,
	SIGNAL_WINDOW_STATE_EVENT,
	SIGNAL_ROW_DELETED,
	SIGNAL_DIRECTION_CHANGED,
	/*
	 * 4 arguments
	 */
	SIGNAL_4_ARGS,
	SIGNAL_DELETE_RANGE = SIGNAL_4_ARGS,
	SIGNAL_DELETE_TEXT,
	SIGNAL_ROW_ACTIVATED,
	SIGNAL_SCROLL_CHILD,
	SIGNAL_SWITCH_PAGE,
	SIGNAL_TEST_COLLAPSE_ROW,
	SIGNAL_TEST_EXPAND_ROW,
	SIGNAL_STATUS_ICON_POPUP_MENU,
	SIGNAL_ROW_INSERTED,
	SIGNAL_ICON_RELEASE,
	SIGNAL_DELETE_FROM_CURSOR,
	SIGNAL_DELETE_FROM_CURSOR_INVERSE,
	SIGNAL_ROW_HAS_CHILD_TOGGLED,
	/*
	 * 5 arguments
	 */
	SIGNAL_5_ARGS,
	SIGNAL_CHANGE_VALUE = SIGNAL_5_ARGS,
	SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW,
	SIGNAL_INSERT_TEXT,
	SIGNAL_TEXT_BUFFER_INSERT_TEXT,
	SIGNAL_MOVE_CURSOR,
	SIGNAL_MOVE_CURSOR_INVERSE,
	/*
	 * last
	 */
	SIGNAL_6_ARGS,
	/*
	 * drag and drop
	 */
	SIGNAL_DRAG_DATA_GET,
	SIGNAL_DRAG_MOTION,
	SIGNAL_DRAG_DROP,
	SIGNAL_DRAG_LEAVE,
	SIGNAL_DRAG_DATA_RECEIVED,
	SIGNAL_DRAG_END,
	SIGNAL_DRAG_DATA_DELETE,
	/*
	 *
	 */
	LAST_SIGNAL,
	GDK_FILTER_PROC,
};
/* Bidi "auto" text direction */

#define HAS_AUTO_DIRECTION  (0)

/* Bidi flag and for auto text direction */

#define AUTO_TEXT_DIRECTION  (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)

/* Default size for widgets */

#define DEFAULT_WIDTH  (64)
#define DEFAULT_HEIGHT  (64)

/* Global state flags */
#define STATE_HANDLE (1 << 0)
#define STATE_OBSCURED (1 << 1)
#define STATE_WIDGET_END (1)

typedef struct _w_event_platform {
	w_event event;
	int msg;
	int result;
	GtkWidget *widget;
	void *args[6];
} _w_event_platform;
struct _w_widget_reserved;
typedef gboolean (*_gtk_signal)(struct w_widget *widget, _w_event_platform *e,
		struct _w_widget_reserved *reserved);

typedef struct _w_widget {
	void *v_table; //not used in c
	struct _w_widget_class *clazz;
	int ref;
	wuint id;
	wuint64 style;
	GtkWidget *handle;
	w_widget_post_event_proc post_event;
	void *data[4];
	wuint state;
} _w_widget;
typedef struct _w_widget_reserved _w_widget_reserved;
struct _w_widget_reserved {
	_gtk_signal signals[LAST_SIGNAL];
	GtkWidget* (*topHandle)(w_widget *control, _w_widget_reserved *reserved);
	wresult (*compute_size)(w_widget *widget, w_event_compute_size *e,
			_w_widget_reserved *reserved);
	wresult (*get_client_area)(w_widget *widget, w_event_client_area *e,
			_w_widget_reserved *reserved);
	wresult (*compute_trim)(w_widget *widget, w_event_compute_trim *e,
			_w_widget_reserved *reserved);
	void (*hook_events)(w_widget *widget, _w_widget_reserved *reserved);
	void (*check_open)(w_widget *widget, _w_widget_reserved *reserved);
	void (*check_orientation)(w_widget *widget, w_widget *parent,
			_w_widget_reserved *reserved);
	wresult (*create_widget)(w_widget *widget, int index,
			_w_widget_reserved *reserved);
	wresult (*create_handle)(w_widget *widget, int index,
			_w_widget_reserved *reserved);
	void (*set_orientation)(w_widget *widget, int create,
			_w_widget_reserved *reserved);
	wuint64 (*check_style)(w_widget *control, wuint64 style);
	wresult (*set_tab_group_focus)(w_widget *widget, int next,
			_w_widget_reserved *reserved);
	wresult (*set_tab_item_focus)(w_widget *widget, int next,
			_w_widget_reserved *reserved);
};
#define _W_WIDGET(x) ((_w_widget*)x)
#define _W_WIDGET_RESERVED(x) ((struct _w_widget_reserved*)x)
#define _GTK_GET_WIDGET(x) (g_object_get_qdata(G_OBJECT(x), gtk_toolkit->quark[0]))
typedef struct _w_item {
	struct _w_widgetdata_class *clazz;
	struct _w_item *next;
	w_widget *parent;
	wuint index;
} _w_item;
#define _W_ITEM(x) ((_w_item*)x)
void _w_item_init_class(struct _w_item_class *clazz);
typedef struct _w_widget_find_child {
	int index;
	int count;
	GtkWidget *widget;
	GtkWidget *prev;
	GtkWidget *next;
} _w_widget_find_child;
typedef struct _w_widget_handles {
	GtkWidget *handle;
	GtkWidget *box;
	GtkWidget *label;
	GtkWidget *image;
	GtkWidget *arrow;
} _w_widget_handles;

GtkWidget* _w_widget_h0(w_widget *widget, _w_widget_reserved *reserved);
GtkWidget* _w_widget_h(w_widget *widget, _w_widget_reserved *reserved);
GtkWidget* _w_widget_hp(w_widget *widget, _w_widget_reserved *reserved);
GtkWidget* _w_widget_hpp(w_widget *widget, _w_widget_reserved *reserved);
GtkWidget* _w_widget_hppp(w_widget *widget, _w_widget_reserved *reserved);
int _w_widget_check_bits(int style, int int0, int int1, int int2, int int3,
		int int4, int int5);
_w_widget_reserved* _w_widget_get_reserved(w_widget *widget);
void _w_widget_find_children(GtkWidget *widget, gpointer data);
void _w_widget_children_count(GtkWidget *widget, gpointer data);
gboolean _w_widget_proc(GtkWidget *widget, int msg, void *args0, void *args1,
		void *args2);
gboolean _w_widget_proc_2(GtkWidget *widget, void *msg);
gboolean _w_widget_proc_3(GtkWidget *widget, void *args0, void *msg);
gboolean _w_widget_proc_4(GtkWidget *widget, void *args0, void *args1,
		void *msg);
gboolean _w_widget_proc_5(GtkWidget *widget, void *args0, void *args1,
		void *args2, void *msg);
void _w_signal_lookup();
void _w_widget_connect(GtkWidget *widget, wushort signal, wushort signal_id,
		gboolean after);
int _w_widget_set_input_state(int state);
int _w_translate_key(int key);
int _w_untranslate_key(int key);
void _w_widget_set_location_state(w_event_key *event, GdkEventKey *keyEvent);
gboolean _w_widget_set_key_state(w_event_key *event, GdkEventKey *keyEvent,
		int unicode_length);
gboolean _w_widget_send_IM_key_event(w_widget *widget, _w_event_platform *e,
		int type, GdkEventKey *keyEvent, const char *chars, int length);
gboolean _w_widget_send_key_event(w_widget *widget, _w_event_platform *e);
void _w_widget_get_handles_callback(GtkWidget *widget, gpointer data);
void _w_widget_get_handles(_w_widget_handles *handles);
void _w_widget_check_open(w_widget *widget, _w_widget_reserved *reserved);
void _w_widget_check_orientation(w_widget *widget, w_widget *parent,
		_w_widget_reserved *reserved);
wresult _w_widget_create_widget(w_widget *widget, int index,
		_w_widget_reserved *reserved);
wresult _w_widget_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved);
void _w_widget_set_orientation(w_widget *widget, int create,
		_w_widget_reserved *reserved);
void _w_widget_hook_events(w_widget *widget, _w_widget_reserved *reserved);
GdkWindow* _gdk_window_get_device_position(GdkWindow *window, gint *x, gint *y,
		GdkModifierType *mask);
int _w_widget_font_height(const PangoFontDescription *font, GtkWidget *widgetHandle);
void _gtk_widget_get_preferred_size(GtkWidget *widget,
		GtkRequisition *requisition);
int _w_widget_send_event(w_widget *widget, w_event *event);
void _w_widget_set_font_description(w_widget *control, GtkWidget *widget,
		PangoFontDescription *font, _w_widget_reserved *reserved);
gboolean _gtk_widget_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
gboolean _gtk_widget_expose(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
/*
 * resource data public function
 */

wresult _w_widgetdata_is_ok(w_widgetdata *obj);
void _w_widgetdata_close(w_widgetdata *obj);
wresult _w_widget_is_ok(w_widget *widget);
void __w_resource_dispose(w_widget *widget);
void _w_widget_dispose(w_widget *widget);
int _w_widget_post_event(w_widget *widget, w_event *ee);
gboolean _gtk_signal_null(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
void _w_widget_class_init(struct _w_widget_class *clazz);

#endif
#endif /* SRC_GTK_WIDGETS_WIDGET_H_ */
