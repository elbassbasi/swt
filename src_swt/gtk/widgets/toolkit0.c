/*
 * toolkit0.c
 *
 *  Created on: 21 mai 2020
 *      Author: Azeddine El Bassbasi
 */
#include "toolkit.h"
#ifdef __linux
#include <dlfcn.h>
/**
 * Disable warning deprecated
 */
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

struct _w_toolkit *gtk_toolkit;
w_toolkit* w_toolkit_get_platform() {
	if (gtk_toolkit == 0) {
		size_t mem_tmp_size = 0x400;
		gtk_toolkit = malloc(sizeof(struct _w_toolkit) + mem_tmp_size);
		if (gtk_toolkit == 0) {
			fprintf(stderr,
					"SWT Fatal Error : Do not alloc memory for toolkit\n");
			exit(EXIT_FAILURE);
			return 0;
		}
		memset(gtk_toolkit, 0, sizeof(struct _w_toolkit));
		gtk_toolkit->mem_tmp_total_size = mem_tmp_size;
		_w_toolkit_init(gtk_toolkit);
		w_app_dispose_registre((w_disposable*) gtk_toolkit);
	}
	return W_TOOLKIT(gtk_toolkit);
}
void _w_toolkit_dispose(w_disposable *_toolkit) {
	_w_toolkit *toolkit = _W_TOOLKIT(_toolkit);
	pthread_mutex_destroy(&toolkit->mutex);
	pango_tab_array_free(toolkit->empty_tab);
	if (gtk_toolkit->caretId != 0)
		g_source_remove(gtk_toolkit->caretId);
}
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget) {
	if (pthread_equal(pthread_self(), (pthread_t)
	_W_TOOLKIT(toolkit)->guithread.id)) {
		return W_TRUE;
	} else {
		return W_ERROR_THREAD_INVALID_ACCESS;
	}
}
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata) {
	if (pthread_equal(pthread_self(), (pthread_t)
	_W_TOOLKIT(toolkit)->guithread.id)) {
		return W_TRUE;
	} else {
		return W_ERROR_THREAD_INVALID_ACCESS;
	}
}
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (clazz_id >= _W_CLASS_LAST)
		return 0;
	else
		return ((_w_toolkit*) toolkit)->classes[clazz_id];
}
struct w_theme* _w_toolkit_get_theme(w_toolkit *toolkit) {
	return &((_w_toolkit*) toolkit)->theme;
}
wresult _w_toolkit_beep(w_toolkit *toolkit) {
}
wresult _w_toolkit_close(w_toolkit *toolkit) {
}
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit) {
}
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds) {
	bounds->x = 0;
	bounds->y = 0;
	bounds->width = gdk_screen_width();
	bounds->height = gdk_screen_height();
	return TRUE;
}
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
	return _w_toolkit_get_bounds(toolkit, clientArea);
}
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
	_gdk_window_get_device_position(0, &location->x, &location->y, NULL);
	return W_TRUE;
}
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
}
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
}
int _w_toolkit_get_double_click_time(w_toolkit *toolkit) {
}
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit) {
	w_control *focusControl = _W_TOOLKIT(toolkit)->focusControl;
	if (focusControl != 0 && w_widget_is_ok(W_WIDGET(focusControl))) {
		return focusControl;
	}
	w_shell *activeShell = _W_TOOLKIT(toolkit)->activeShell;
	if (activeShell == 0)
		return 0;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(activeShell);
	GtkWidget *handle = gtk_window_get_focus(GTK_WINDOW(shellHandle));
	if (handle == 0)
		return 0;
	do {
		w_widget *widget = g_object_get_qdata(G_OBJECT(handle),
		_W_TOOLKIT(toolkit)->quark[0]);
		if (widget != 0 && w_widget_class_id(widget) >= _W_CLASS_CONTROL) {
			return w_control_is_enabled(W_CONTROL(widget)) ?
					W_CONTROL(widget) : 0;
		}
	} while ((handle = gtk_widget_get_parent(handle)) != 0);
	return 0;
}
wresult _w_toolkit_get_high_contrast(w_toolkit *toolkit) {
}
int _w_toolkit_get_icon_depth(w_toolkit *toolkit) {
}
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
}
_w_shell* _w_shells_iterator_find_next(_w_shell *parent, _w_shell *current) {
	if (parent == 0)
		return current->next;
	else {
		_w_shell *s = current->next;
		while (s != 0) {
			if (_W_CONTROL(s)->parent == (w_composite*) parent)
				return s;
			s = s->next;
		}
		return 0;
	}
}
_w_shell* _w_shells_iterator_find_first(_w_shell *parent) {
	_w_shell *s = gtk_toolkit->shells;
	while (s != 0) {
		if (_W_CONTROL(s)->parent == (w_composite*) parent)
			return s;
		s = s->next;
	}
	return 0;
}
wresult _w_shells_iterator_next(w_iterator *it, void *obj) {
	if (_W_SHELLS_ITERATOR(it)->current != 0) {
		*((_w_shell**) obj) = _W_SHELLS_ITERATOR(it)->current;
		_W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_next(
		_W_SHELLS_ITERATOR(it)->parent,
		_W_SHELLS_ITERATOR(it)->current);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_shells_iterator_reset(w_iterator *it) {
	if (_W_SHELLS_ITERATOR(it)->parent == 0)
		_W_SHELLS_ITERATOR(it)->current = gtk_toolkit->shells;
	else
		_W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_first(
		_W_SHELLS_ITERATOR(it)->parent);
	return W_TRUE;
}
wresult _w_shells_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_shells_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_shells_iterator_count(w_iterator *it) {
	if (_W_SHELLS_ITERATOR(it)->parent == 0)
		return gtk_toolkit->shells_count;
	else {
		if (_W_SHELLS_ITERATOR(it)->count == -1) {
			_w_shell *s = gtk_toolkit->shells;
			size_t count = 0;
			while (s != 0) {
				if (_W_CONTROL(s)->parent
						== (w_composite*) _W_SHELLS_ITERATOR(it)->parent)
					count++;
				s = s->next;
			}
			_W_SHELLS_ITERATOR(it)->count = count;
		}
		return _W_SHELLS_ITERATOR(it)->count;
	}
}
_w_iterator_class _w_shells_iterator_class = { //
		_w_shells_iterator_close, //
				_w_shells_iterator_next, //
				_w_shells_iterator_reset, //
				_w_shells_iterator_remove, //
				_w_shells_iterator_count };
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	_W_SHELLS_ITERATOR(shells)->base.clazz = &_w_shells_iterator_class;
	_W_SHELLS_ITERATOR(shells)->current = gtk_toolkit->shells;
	_W_SHELLS_ITERATOR(shells)->parent = 0;
	_W_SHELLS_ITERATOR(shells)->count = -1;
}
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator) {
	_W_SHELLS_ITERATOR(iterator)->base.clazz = &_w_shells_iterator_class;
	_W_SHELLS_ITERATOR(iterator)->current = _w_shells_iterator_find_first(
	_W_SHELLS_ITERATOR(iterator)->parent);
	_W_SHELLS_ITERATOR(iterator)->parent = shell;
	_W_SHELLS_ITERATOR(iterator)->count = -1;
}
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {
	GtkStyleContext *context;
	GdkRGBA rgba;
	w_color color;
	switch (id) {
	case W_COLOR_INFO_FOREGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->tooltipShellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_TOOLTIP);
		gtk_style_context_invalidate(context);
		return _w_toolkit_GtkStyleContext(context, GTK_STATE_FLAG_NORMAL);
		break;
	case W_COLOR_INFO_BACKGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->tooltipShellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_TOOLTIP);
		gtk_style_context_invalidate(context);
		return _w_toolkit_getBackgroundColor(context, GTK_STATE_FLAG_NORMAL);
		break;
	case W_COLOR_TITLE_BACKGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_SELECTED,
				&rgba);
		return W_RGBA(rgba.red * 0xFF, rgba.green * 0xFF, rgba.blue * 0xFF,
				rgba.alpha * 0xFF);
		break;
	case W_COLOR_TITLE_BACKGROUND_GRADIENT:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_SELECTED,
				&rgba);
		return toGdkRGBA(rgba, 1.3);
		break;
	case W_COLOR_TITLE_INACTIVE_FOREGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		return _w_toolkit_GtkStyleContext(context, GTK_STATE_FLAG_INSENSITIVE);
		break;
	case W_COLOR_TITLE_INACTIVE_BACKGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		gtk_style_context_get_background_color(context,
				GTK_STATE_FLAG_INSENSITIVE, &rgba);
		return W_RGBA(rgba.red * 0xFF, rgba.green * 0xFF, rgba.blue * 0xFF,
				rgba.alpha * 0xFF);
		break;
	case W_COLOR_TITLE_INACTIVE_BACKGROUND_GRADIENT:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		gtk_style_context_get_background_color(context,
				GTK_STATE_FLAG_INSENSITIVE, &rgba);
		return toGdkRGBA(rgba, 1.3);
		break;
		/*case W_COLOR_WIDGET_DARK_SHADOW:
		 gdkRGBA = copyRGBA(COLOR_WIDGET_DARK_SHADOW_RGBA);
		 break;*/
	case W_COLOR_WIDGET_NORMAL_SHADOW: {	//
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL,
				&rgba);
		return toGdkRGBA(rgba, 0.7);
	}
		break;
	case W_COLOR_WIDGET_LIGHT_SHADOW: {
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL,
				&rgba);
		return W_RGBA(rgba.red * 0xFF, rgba.green * 0xFF, rgba.blue * 0xFF,
				rgba.alpha * 0xFF);
	}
		break;
	case W_COLOR_WIDGET_HIGHLIGHT_SHADOW: {	//
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL,
				&rgba);
		return toGdkRGBA(rgba, 1.3);

	}
		break;
	case W_COLOR_WIDGET_BACKGROUND: {
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL,
				&rgba);
		return W_RGBA(rgba.red * 0xFF, rgba.green * 0xFF, rgba.blue * 0xFF,
				rgba.alpha * 0xFF);
	}
		break;
	case W_COLOR_WIDGET_FOREGROUND: {
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		return _w_toolkit_GtkStyleContext(context, GTK_STATE_FLAG_NORMAL);
	}
		break;
		/*case W_COLOR_WIDGET_BORDER:
		 gdkRGBA = copyRGBA(COLOR_WIDGET_BORDER_RGBA);
		 break;*/
	case W_COLOR_LIST_FOREGROUND: {
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_save(context);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		color = _w_toolkit_GtkStyleContext(context, GTK_STATE_FLAG_NORMAL);
		gtk_style_context_restore(context);
		return color;
	}
		break;
	case W_COLOR_LIST_BACKGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_save(context);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL,
				&rgba);
		gtk_style_context_restore(context);
		return W_RGBA(rgba.red * 0xFF, rgba.green * 0xFF, rgba.blue * 0xFF,
				rgba.alpha * 0xFF);
		break;
	case W_COLOR_LIST_SELECTION:
	case W_COLOR_LINK_FOREGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_SELECTED,
				&rgba);
		gtk_style_context_restore(context);
		return W_RGBA(rgba.red * 0xFF, rgba.green * 0xFF, rgba.blue * 0xFF,
				rgba.alpha * 0xFF);
		break;
	case W_COLOR_LIST_SELECTION_TEXT:
	case W_COLOR_TITLE_FOREGROUND:
		context = gtk_widget_get_style_context(
				((_w_toolkit*) (toolkit))->shellHandle);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_VIEW);
		gtk_style_context_add_class(context, GTK_STYLE_CLASS_CELL);
		gtk_style_context_invalidate(context);
		return _w_toolkit_GtkStyleContext(context, GTK_STATE_FLAG_SELECTED);
		break;
	default:
		return W_COLOR_BLACK;
		break;
	}
}
wuchar cursorshapes[] = { GDK_LEFT_PTR, GDK_WATCH, GDK_CROSS, 0,
		GDK_QUESTION_ARROW, GDK_FLEUR, GDK_SIZING, GDK_DOUBLE_ARROW, GDK_SIZING,
		GDK_SB_H_DOUBLE_ARROW, GDK_TOP_SIDE, GDK_BOTTOM_SIDE, GDK_RIGHT_SIDE,
		GDK_LEFT_SIDE, GDK_TOP_RIGHT_CORNER, GDK_BOTTOM_RIGHT_CORNER,
		GDK_BOTTOM_LEFT_CORNER, GDK_TOP_LEFT_CORNER, GDK_SB_UP_ARROW, GDK_XTERM,
		GDK_X_CURSOR, GDK_HAND2 };
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id) {
	if (id <= W_CURSOR_HAND) {
		_w_cursor *cursor =
				(_w_cursor*) &_W_TOOLKIT(toolkit)->system_cursors[id];
		if (cursor->handle == 0) {
			GdkCursorType shape = cursorshapes[id];
			if (shape == 0 && id == W_CURSOR_APPSTARTING) {
			} else {
				_W_CURSOR(cursor)->handle = gdk_cursor_new_for_display(
						gdk_display_get_default(), shape);
			}
		}
		return W_CURSOR(cursor);
	} else
		return 0;
}
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit) {
	if (_W_TOOLKIT(toolkit)->system_font.handle == 0) {
		/* Initialize the system font slot */
		const PangoFontDescription *defaultFont = 0;
#if GTK3
		GtkStyleContext *context = gtk_widget_get_style_context(
		_W_TOOLKIT(toolkit)->shellHandle);
		if ((GTK_VERSION < VERSION(3, 8, 0))) {
			defaultFont = gtk_style_context_get_font(context,
					GTK_STATE_FLAG_NORMAL);
		} else if (GTK_VERSION >= VERSION(3, 18, 0)) {
			gtk_style_context_save(context);
			gtk_style_context_set_state(context, GTK_STATE_FLAG_NORMAL);
			gtk_style_context_get(context, GTK_STATE_FLAG_NORMAL, "font",
					&defaultFont, NULL);
			gtk_style_context_restore(context);
		} else {
			gtk_style_context_get(context, GTK_STATE_FLAG_NORMAL, "font",
					&defaultFont, NULL);
		}
#else
	GtkStyle* style = gtk_widget_get_style (_W_TOOLKIT(toolkit)->shellHandle);
		defaultFont = gtk_style_get_font_desc (style);
#endif
		_W_TOOLKIT(toolkit)->system_font.handle = defaultFont;
	}
	return (w_font*) (&_W_TOOLKIT(toolkit)->system_font);
}
wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
		w_image *image) {
}
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit) {
}
struct w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit) {
}
struct w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit) {
}
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit) {
}
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_touch_enabled(w_toolkit *toolkit) {
}
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
		w_point *result, w_point *point) {
	if (from != 0 && !w_widget_is_ok(W_WIDGET(from)))
		return W_ERROR_INVALID_ARGUMENT;
	if (to != 0 && !w_widget_is_ok(W_WIDGET(to)))
		return W_ERROR_INVALID_ARGUMENT;
	memcpy(result, point, sizeof(w_point));
	if (from == to)
		return TRUE;
	w_point origin;
	if (from != 0) {
		/*_w_control_get_window_origin(from, &origin);
		 if ((_W_WIDGET(from)->style & W_MIRRORED) != 0) {
		 result->x = _w_control_get_client_width(from) - result->x;
		 }*/
		result->x += origin.x;
		result->y += origin.y;
	}
	if (to != 0) {
		/*_w_control_get_window_origin(to, &origin);
		 result->x -= origin.x;
		 result->y -= origin.y;
		 if ((_W_WIDGET(to)->style & W_MIRRORED) != 0) {
		 result->x = _w_control_get_client_width(to) - result->x;
		 }*/
	}
	return TRUE;
}
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle) {
	if (from != 0 && !w_widget_is_ok(W_WIDGET(from)))
		return W_ERROR_INVALID_ARGUMENT;
	if (to != 0 && !w_widget_is_ok(W_WIDGET(to)))
		return W_ERROR_INVALID_ARGUMENT;
	memcpy(result, rectangle, sizeof(w_rect));
	if (from == to)
		return TRUE;
	int fromRTL = FALSE, toRTL = FALSE;
	w_point origin;
	if (from != 0) {
		/*_w_control_get_window_origin(from, &origin);
		 fromRTL = (_W_WIDGET(from)->style & W_MIRRORED) != 0;
		 if (fromRTL) {
		 result->x = _w_control_get_client_width(from) - result->x;
		 }*/
		result->x += origin.x;
		result->y += origin.y;
	}
	if (to != 0) {
		/*_w_control_get_window_origin(to, &origin);
		 result->x -= origin.x;
		 result->y -= origin.y;
		 toRTL = (_W_WIDGET(to)->style & W_MIRRORED) != 0;
		 if (toRTL) {
		 result->x = _w_control_get_client_width(to) - result->x;
		 }*/
	}

	if (fromRTL != toRTL)
		result->x -= result->width;
	return TRUE;
}
wresult _w_toolkit_post(w_toolkit *toolkit, w_event *event) {
}
wresult _w_toolkit_post_quit(w_toolkit *toolkit, int quit) {
	_W_TOOLKIT(toolkit)->exit_code = quit;
	_W_TOOLKIT(toolkit)->exit_loop = 1;
	return W_TRUE;
}
wresult _w_toolkit_read_and_dispatch(w_toolkit *toolkit) {
	/*runSkin ();
	 runDeferredLayouts ();
	 boolean events = false;
	 events |= runSettings ();
	 events |= runPopups ();*/
	/*
	 * This call to gdk_threads_leave() is a temporary work around
	 * to avoid deadlocks when gdk_threads_init() is called by native
	 * code outside of SWT (i.e AWT, etc). It ensures that the current
	 * thread leaves the GTK lock before calling the function below.
	 */
	gdk_threads_leave();
	/*events |=*/g_main_context_iteration(0, FALSE);
	/*if (events) {
	 runDeferredEvents ();
	 return true;
	 }
	 return isDisposed () || runAsyncMessages (false);*/
	w_event _e;
	w_widget *widget = ((_w_toolkit*) toolkit)->widget_free, *next;
	while (widget != 0) {
		w_widget_post_event_proc _proc =
				(w_widget_post_event_proc) widget->post_event;
		next = (w_widget*) widget->handle; //widget = widget->next
		if (_proc != 0) {
			_e.type = W_EVENT_FREE_MEMORY;
			_e.platform_event = 0;
			_e.data = 0;
			_e.widget = widget;
			_proc(widget, &_e);
		} else {
			free(widget);
		}
		widget = next; //widget = widget->next
	}
	((_w_toolkit*) toolkit)->widget_free = 0;
	return W_TRUE;
}
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *location) {
	GdkDisplay *gdkDisplay = gdk_display_get_default();
	GdkDeviceManager *gdkDeviceManager = gdk_display_get_device_manager(
			gdkDisplay);
	GdkScreen *gdkScreen = gdk_screen_get_default();
	GdkDevice *gdkPointer = gdk_device_manager_get_client_pointer(
			gdkDeviceManager);
	gdk_device_warp(gdkPointer, gdkScreen, location->x, location->y);
	return TRUE;
}
int _w_toolkit_run(w_toolkit *toolkit) {
	_W_TOOLKIT(toolkit)->exit_loop = 0;
	while (_W_TOOLKIT(toolkit)->exit_loop == 0) {
		w_toolkit_read_and_dispatch(toolkit);
		w_toolkit_sleep(toolkit);
	}
	return _W_TOOLKIT(toolkit)->exit_code;
}
wresult _w_toolkit_sleep(w_toolkit *toolkit) {
	/*if (gdkEventCount == 0) {
	 gdkEvents = null;
	 gdkEventWidgets = null;
	 }
	 if (settingsChanged) {
	 settingsChanged = false;
	 runSettings = true;
	 return false;
	 }
	 if (getMessageCount () != 0) return true;
	 sendPreExternalEventDispatchEvent ();*/
	if (gtk_toolkit->fds == 0) {
		gtk_toolkit->allocated_nfds = sizeof(gtk_toolkit->fds_tmp)
				/ sizeof(gtk_toolkit->fds_tmp[0]);
		gtk_toolkit->fds = gtk_toolkit->fds_tmp;
	}
	int max_priority = 0;
	int timeout = 0;
	GMainContext *context = g_main_context_default();
	gboolean result = FALSE;
	do {
		if (g_main_context_acquire(context)) {
			result = g_main_context_prepare(context, &max_priority);
			int nfds;
			while ((nfds = g_main_context_query(context, max_priority, &timeout,
					gtk_toolkit->fds, gtk_toolkit->allocated_nfds))
					> gtk_toolkit->allocated_nfds) {
				if (gtk_toolkit->fds != gtk_toolkit->fds_tmp) {
					g_free(gtk_toolkit->fds);
				}
				gtk_toolkit->allocated_nfds = nfds;
				gtk_toolkit->fds = g_malloc(nfds * sizeof(GPollFD));
			}
			GPollFunc poll = g_main_context_get_poll_func(context);
			if (poll != 0) {
				if (nfds > 0 || timeout != 0) {
					/*
					 * Bug in GTK. For some reason, g_main_context_wakeup() may
					 * fail to wake up the UI thread from the polling function.
					 * The fix is to sleep for a maximum of 50 milliseconds.
					 */
					if (timeout < 0)
						timeout = 50;

					/* Exit the OS lock to allow other threads to enter GTK */
					/*Lock lock = OS.lock;
					 int count = lock.lock();
					 for (int i = 0; i < count; i++)
					 lock.unlock();*/
					gtk_toolkit->wake = FALSE;
					poll(gtk_toolkit->fds, nfds, timeout);
					/*for (int i = 0; i < count; i++)
					 lock.lock();
					 lock.unlock();*/
				}
			}
			g_main_context_check(context, max_priority, gtk_toolkit->fds, nfds);
			g_main_context_release(context);
		}
	} while (!result /*&& getMessageCount () == 0*/&& !gtk_toolkit->wake);
	gtk_toolkit->wake = FALSE;
	if (gtk_toolkit->fds != gtk_toolkit->fds_tmp) {
		g_free(gtk_toolkit->fds);
		gtk_toolkit->fds = 0;
	}
	//sendPostExternalEventDispatchEvent ();
	return TRUE;
}
wresult _w_toolkit_exec(w_toolkit *toolkit, w_thread_start function, void *args,
		int sync, wuint ms) {
	pthread_t thread = pthread_self();
	if (pthread_equal(thread, ((_w_toolkit*) toolkit)->guithread.id)) {
		function(args);
		return TRUE;
	}
	struct threads_idle *funcs = 0;
	struct threads_idle *threads_idle = &gtk_toolkit->threads_idle[0];
	pthread_mutex_lock(&((_w_toolkit*) toolkit)->mutex);
	for (int i = 0; i < threads_idle_count; i++) {
		if (threads_idle[i].func == 0) {
			funcs = &threads_idle[i];
			break;
		}
	}
	if (funcs != 0) {
		funcs->func = function;
		funcs->data = args;
		funcs->signalled = 0;
	}
	pthread_mutex_unlock(&((_w_toolkit*) toolkit)->mutex);
	if (funcs == 0) {
		funcs = malloc(sizeof(struct threads_idle));
		if (funcs == 0)
			return TRUE;
		else {
			funcs->func = function;
			funcs->data = args;
		}
	}
	GSourceFunc sourceFunc;
	if (ms == -1) {
		if (sync) {
			sourceFunc = _w_toolkit_sync_exec_GSourceFunc;
		} else {
			sourceFunc = _w_toolkit_async_exec_GSourceFunc;
		}
		gdk_threads_add_idle(_w_toolkit_async_exec_GSourceFunc, funcs);
	} else {
		g_timeout_add(ms, _w_toolkit_async_exec_GSourceFunc, funcs);
	}
	if (sync) {
		while (funcs->signalled != 0) {
			pthread_cond_wait(&gtk_toolkit->condition,
					&gtk_toolkit->condition_mutex);
		}
	}
	return TRUE;
}
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	return _w_toolkit_exec(toolkit, function, args, W_FALSE, -1);
}
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	return _w_toolkit_exec(toolkit, function, args, W_TRUE, -1);
}
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
	return _w_toolkit_exec(toolkit, function, args, W_FALSE, milliseconds);
}
wresult _w_toolkit_update(w_toolkit *toolkit) {
}
wresult _w_toolkit_wake(w_toolkit *toolkit) {
}
void _w_toolkit_init(_w_toolkit *toolkit) {
	char txt[30];
	gtk_init(0, 0);
	toolkit->version = VERSION(gtk_get_major_version(), gtk_get_minor_version(),
			gtk_get_micro_version());
	/**
	 * initialize class
	 */
	_w_toolkit_class_init(toolkit);
	_w_toolkit_subclass_init(toolkit);
	/**
	 * create g_quark
	 */
	for (int i = 0;
			i < (sizeof(gtk_toolkit->quark) / sizeof(gtk_toolkit->quark[0]));
			i++) {
		sprintf(txt, "w_quark_%d", i);
		gtk_toolkit->quark[i] = g_quark_from_string(txt);
	}
	/**
	 * initialize main thread
	 */
	_W_TOOLKIT(toolkit)->guithread.id = pthread_self();
	_w_toolkit_init_widget(toolkit);
	toolkit->empty_tab = pango_tab_array_new(1, FALSE);
	if (toolkit->empty_tab != 0) {
		pango_tab_array_set_tab(toolkit->empty_tab, 0, PANGO_TAB_LEFT, 1);
	}
	_w_theme_gtk_init(&toolkit->theme);
	pthread_mutex_init(&toolkit->mutex, NULL);
	pthread_mutex_init(&toolkit->condition_mutex, NULL);
	pthread_cond_init(&toolkit->condition, NULL);
	_w_toolkit_init_trims(toolkit);
	toolkit->clickCount = 1;
	GdkDisplay *display = gdk_display_get_default();
	const char *type_name = g_type_name(
			((GTypeInstance*) display)->g_class->g_type);
	if (!strcmp(type_name, "GdkX11Display")) {
		toolkit->display_type = W_DISPLAY_X11;
		toolkit->libX11 = dlopen("libX11.so", RTLD_LAZY | RTLD_GLOBAL);
	} else if (!strcmp(type_name, "GdkWaylandDisplay")) {
		toolkit->display_type = W_DISPLAY_WAYLAND;
	}
	toolkit->dispatchEvents[0] = -1;
	_w_transfers_registre();
}
#endif
