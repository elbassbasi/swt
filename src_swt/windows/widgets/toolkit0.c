/*
 * toolkit.c
 *
 *  Created on: 28 févr. 2019
 *      Author: azeddine El Bassbasi
 */
#include "toolkit.h"
#if defined( __WIN32__) || defined(__WIN64__)
w_toolkit* w_toolkit_get_platform() {
	if (win_toolkit == 0) {
		const int total_size = 0x10000;
		const int toolkit_size = sizeof(_w_toolkit);
		const int tmp_size = total_size - toolkit_size;
		win_toolkit = calloc(1, total_size);
		if (win_toolkit == 0) {
			fprintf(stderr, "Error : Do not initialize toolkit\n");
			exit(EXIT_FAILURE);
			return 0;
		}
		win_toolkit->tmp_alloc = tmp_size;
		_w_toolkit_init(win_toolkit);
	}
	return (w_toolkit*) win_toolkit;
}
void _w_toolkit_init_messages(struct _w_toolkit *toolkit);

void _w_toolkit_init(_w_toolkit *toolkit) {
	_w_toolkit_class_init(toolkit);
	_w_toolkit_widget_class_init(toolkit);
	_w_toolkit_init_messages(toolkit);
	_w_toolkit_init_dll(toolkit);
	toolkit->systemtheme.clazz = &win32_theme;
	w_thread_init(&toolkit->thread);
	((_w_thread*) (&(toolkit)->thread))->threadId = GetCurrentThreadId();

}
void _w_toolkit_dispose(w_toolkit *_toolkit) {
	_w_toolkit *toolkit = _W_TOOLKIT(_toolkit);

}

void _w_toolkit_registre_free(w_widget *widget,
		w_widget_post_event_proc _proc) {
	widget->post_event = _proc; // widget->proc = _proc
	widget->handle = win_toolkit->widget_free; //widget->next =
	win_toolkit->widget_free = widget;
}
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget) {
	if (GetCurrentThreadId()
			!= ((_w_thread*) (&((_w_toolkit*) toolkit)->thread))->threadId) {
		return W_ERROR_THREAD_INVALID_ACCESS;
	} else
		return W_TRUE;
}
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata) {
	if (GetCurrentThreadId()
			!= ((_w_thread*) (&((_w_toolkit*) toolkit)->thread))->threadId) {
		return W_ERROR_THREAD_INVALID_ACCESS;
	} else
		return W_TRUE;
}
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (clazz_id >= _W_CLASS_LAST)
		return 0;
	else
		return ((_w_toolkit*) toolkit)->classes[clazz_id];
}
w_theme* _w_toolkit_get_theme(w_toolkit *toolkit) {
	return &((_w_toolkit*) toolkit)->systemtheme;
}
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	PostThreadMessageW(
			((_w_thread*) &((_w_toolkit*) toolkit)->thread)->threadId,
			WM_ASYNCEXEC, (WPARAM) args, (LPARAM) function);
	return TRUE;
}
wresult _w_toolkit_beep(w_toolkit *toolkit) {
}
wresult _w_toolkit_close(w_toolkit *toolkit) {
}
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit) {
}
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *rect) {
	if (GetSystemMetrics(SM_CMONITORS) < 2) {
		rect->width = GetSystemMetrics(SM_CXSCREEN);
		rect->height = GetSystemMetrics(SM_CYSCREEN);
		rect->x = 0;
		rect->y = 0;
	} else {
		rect->x = GetSystemMetrics(SM_XVIRTUALSCREEN);
		rect->y = GetSystemMetrics(SM_YVIRTUALSCREEN);
		rect->width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		rect->height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}
	return TRUE;
}
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
}
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
}
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
}
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
}
int _w_toolkit_get_double_click_time(w_toolkit *toolkit) {
}
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit) {
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
	_w_shell *s = win_toolkit->shells;
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
		_W_SHELLS_ITERATOR(it)->current = win_toolkit->shells;
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
		return win_toolkit->shells_count;
	else {
		if (_W_SHELLS_ITERATOR(it)->count == -1) {
			_w_shell *s = win_toolkit->shells;
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
	_W_SHELLS_ITERATOR(shells)->current = win_toolkit->shells;
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
wuchar _system_colors[] = { //
		[W_COLOR_WIDGET_DARK_SHADOW] = COLOR_3DDKSHADOW, //
				[W_COLOR_WIDGET_NORMAL_SHADOW] = COLOR_3DSHADOW, //
				[W_COLOR_WIDGET_LIGHT_SHADOW] = COLOR_3DLIGHT, //
				[W_COLOR_WIDGET_HIGHLIGHT_SHADOW] = COLOR_3DHIGHLIGHT, //
				[W_COLOR_WIDGET_BACKGROUND] = COLOR_3DFACE, //
				[W_COLOR_WIDGET_BORDER] = COLOR_WINDOWFRAME, //
				[W_COLOR_WIDGET_FOREGROUND] = COLOR_WINDOWTEXT, //
				[W_COLOR_LIST_FOREGROUND] = COLOR_WINDOWTEXT, //
				[W_COLOR_LIST_BACKGROUND] = COLOR_WINDOW, //
				[W_COLOR_LIST_SELECTION] = COLOR_HIGHLIGHT, //
				[W_COLOR_LIST_SELECTION_TEXT] = COLOR_HIGHLIGHTTEXT, //
				[W_COLOR_LINK_FOREGROUND] = COLOR_HOTLIGHT, //
				[W_COLOR_INFO_FOREGROUND] = COLOR_INFOTEXT, //
				[W_COLOR_INFO_BACKGROUND] = COLOR_INFOBK, //
				[W_COLOR_TITLE_FOREGROUND] = COLOR_CAPTIONTEXT, //
				[W_COLOR_TITLE_BACKGROUND] = COLOR_ACTIVECAPTION, //
				[W_COLOR_TITLE_BACKGROUND_GRADIENT
						] = COLOR_GRADIENTACTIVECAPTION, //
				[W_COLOR_TITLE_INACTIVE_FOREGROUND] = COLOR_INACTIVECAPTIONTEXT, //
				[W_COLOR_TITLE_INACTIVE_BACKGROUND] = COLOR_INACTIVECAPTION, //
				[W_COLOR_TITLE_INACTIVE_BACKGROUND_GRADIENT
						] = COLOR_GRADIENTINACTIVECAPTION, //

		};
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {
	w_color color = 0x00000000;
	if (id < sizeof(_system_colors) / sizeof(_system_colors[0])) {
		int nIndex = _system_colors[id];
		if (nIndex != 0) {
			color = GetSysColor(nIndex);
			if (color == 0) {
				switch (id) {
				case W_COLOR_TITLE_BACKGROUND_GRADIENT:
					color = GetSysColor(COLOR_ACTIVECAPTION);
					break;
				case W_COLOR_TITLE_INACTIVE_BACKGROUND_GRADIENT:
					color = GetSysColor(COLOR_INACTIVECAPTION);
					break;
				}

			}
		}
	}
	return color;
}
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint style) {
	if (((unsigned int) style) <= W_CURSOR_HAND) {
		return (w_cursor*) &((_w_toolkit*) toolkit)->cursors[style];
	} else
		return 0;
}
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit) {
	_w_toolkit *t = (_w_toolkit*) toolkit;
	/*#if !IsWinCE
	 NONCLIENTMETRICS info = OS.IsUnicode ? (NONCLIENTMETRICS) new NONCLIENTMETRICSW () : new NONCLIENTMETRICSA ();
	 info.cbSize = NONCLIENTMETRICS.sizeof;
	 if (OS.SystemParametersInfo (OS.SPI_GETNONCLIENTMETRICS, 0, info, 0)) {
	 LOGFONT logFont = OS.IsUnicode ? (LOGFONT) ((NONCLIENTMETRICSW)info).lfMessageFont : ((NONCLIENTMETRICSA)info).lfMessageFont;
	 hFont = OS.CreateFontIndirect (logFont);
	 lfSystemFont = hFont != 0 ? logFont : null;
	 }
	 #endif*/
	if (t->systemfont.handle == 0) {
		t->systemfont.handle = GetStockObject(DEFAULT_GUI_FONT);
		if (t->systemfont.handle == 0)
			t->systemfont.handle = GetStockObject(SYSTEM_FONT);
	}
	return (w_font*) &t->systemfont;
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
}
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
		w_rect *result, w_rect *rectangle) {
}
wresult _w_toolkit_post(w_toolkit *toolkit, w_event *event) {
}
wresult _w_toolkit_post_quit(w_toolkit *toolkit, int quit) {
	PostQuitMessage(quit);
	return TRUE;
}
wresult _w_toolkit_read_and_dispatch(w_toolkit *toolkit) {
}
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point) {
}
int _w_toolkit_run(w_toolkit *toolkit) {
	w_event _e;
	while (GetMessageW(&((_w_toolkit*) toolkit)->Msg, NULL, 0, 0)) {
		TranslateMessage(&((_w_toolkit*) toolkit)->Msg);
		DispatchMessageW(&((_w_toolkit*) toolkit)->Msg);
		if (((_w_toolkit*) toolkit)->Msg.message == WM_ASYNCEXEC) {
			((w_thread_start) ((_w_toolkit*) toolkit)->Msg.lParam)(
					(void*) ((_w_toolkit*) toolkit)->Msg.wParam);
		}
		w_widget *widget = ((_w_toolkit*) toolkit)->widget_free, *next;
		while (widget != 0) {
			w_widget_post_event_proc _proc =
					(w_widget_post_event_proc) widget->post_event;
			next = widget->handle; //widget = widget->next
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
	}
	return 1;
}
wresult _w_toolkit_sleep(w_toolkit *toolkit) {
}
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
}
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
}
wresult _w_toolkit_update(w_toolkit *toolkit) {
}
wresult _w_toolkit_wake(w_toolkit *toolkit) {
}
#endif
