/*
 * shell.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "shell.h"
#include "toolkit.h"
wuint64 _w_shell_check_style(w_widget *widget, wuint64 style) {
	if ((style & W_NO_TRIM) != 0) {
		style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX | W_RESIZE | W_BORDER);
	} else if ((style & W_NO_MOVE) != 0) {
		style |= W_TITLE;
	}
	if ((style & (W_MENU | W_MIN | W_MAX | W_CLOSE)) != 0) {
		style |= W_TITLE;
	}
	style &= ~W_TRANSPARENT;
	if ((style & W_ON_TOP) != 0)
		style &= ~(W_CLOSE | W_TITLE | W_MIN | W_MAX);
	int mask = W_SYSTEM_MODAL | W_APPLICATION_MODAL | W_PRIMARY_MODAL;
	if ((style & W_SHEET) != 0) {
		style &= ~W_SHEET;
		w_composite *parent = _W_CONTROL(widget)->parent;
		style |= parent == 0 ? W_SHELL_TRIM : W_DIALOG_TRIM;
		if ((style & mask) == 0) {
			style |= parent == 0 ? W_APPLICATION_MODAL : W_PRIMARY_MODAL;
		}
	}
	int bits = style & ~mask;
	if ((style & W_SYSTEM_MODAL) != 0)
		return bits | W_SYSTEM_MODAL;
	if ((style & W_APPLICATION_MODAL) != 0)
		return bits | W_APPLICATION_MODAL;
	if ((style & W_PRIMARY_MODAL) != 0)
		return bits | W_PRIMARY_MODAL;
	return bits;
}
const char *DialogClass = "#32770";
const char* _w_shell_window_class(w_widget *parent, int style) {
	//if (IsSP) return DialogClass;
	if ((style & W_TOOL) != 0) {
		int trim = W_TITLE | W_CLOSE | W_MIN | W_MAX | W_BORDER | W_RESIZE;
		if ((style & trim) == 0)
			return WindowShadowName;
	}
	return parent != 0 ? DialogClass : WindowName;
}
void _w_shell_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwExStyle |= WS_EX_MDICHILD;
	*dwExStyle &= ~WS_EX_CLIENTEDGE;
	if ((style & W_TOOL) != 0)
		*dwExStyle |= WS_EX_TOOLWINDOW;

	/*
	 * Feature in Windows.  When a window that does not have a parent
	 * is created, it is automatically added to the Windows Task Bar,
	 * even when it has no title.  The fix is to use WS_EX_TOOLWINDOW
	 * which does not cause the window to appear in the Task Bar.
	 */
#if !IsWinCE
	if (parent == 0) {
		if ((style & W_ON_TOP) != 0) {
			int trim = W_CLOSE | W_MIN | W_MAX;
			if ((style & W_NO_TRIM) != 0 || (style & trim) == 0) {
				*dwExStyle |= WS_EX_TOOLWINDOW;
			}
		}
	}
#endif

	/*
	 * Bug in Windows 98 and NT.  Creating a window with the
	 * WS_EX_TOPMOST extended style can result in a dialog shell
	 * being moved behind its parent.  The exact case where this
	 * happens is a shell with two dialog shell children where
	 * each dialog child has another hidden dialog child with
	 * the WS_EX_TOPMOST extended style.  Clicking on either of
	 * the visible dialogs causes them to become active but move
	 * to the back, behind the parent shell.  The fix is to
	 * disallow the WS_EX_TOPMOST extended style on Windows 98
	 * and NT.
	 */
	if (parent != 0) {
		//if (OS.IsWin95) return bits;
		if (WIN32_VERSION < VERSION(4, 10)) {
			//return bits;
		} else {
			if ((style & W_ON_TOP) != 0)
				*dwExStyle |= WS_EX_TOPMOST;
		}
	} else {
		if ((style & W_ON_TOP) != 0)
			*dwExStyle |= WS_EX_TOPMOST;
	}
	*dwStyle &= ~(WS_TABSTOP | WS_VISIBLE);
	*dwStyle &= ~WS_BORDER;
	if ((style & W_NO_TRIM) != 0) {
		if (parent == 0) {
			*dwStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP;
		}
	} else {
		/*	if (OS.IsPPC) {
		 if ((style & SWT.CLOSE) != 0) *dwExStyle |= OS.WS_EX_CAPTIONOKBTN;
		 }*/
		//if (!OS.IsPPC && !OS.IsSP) {
		if ((style & W_CLOSE) != 0)
			*dwStyle |= WS_SYSMENU;
		//}
		if ((style & W_TITLE) != 0)
			*dwStyle |= WS_CAPTION;

		/* Set the min and max button bits */
		if ((style & W_MIN) != 0)
			*dwStyle |= WS_MINIMIZEBOX;
		if ((style & W_MAX) != 0)
			*dwStyle |= WS_MAXIMIZEBOX;

		if ((style & W_RESIZE) != 0) {
			*dwStyle |= WS_THICKFRAME;
		} else {
			if ((style & W_BORDER) != 0) {
				*dwExStyle |= WS_EX_DLGMODALFRAME;
				*dwStyle |= WS_BORDER;
			}
		}
	}
	*dwExStyle &= ~WS_EX_MDICHILD;
	if ((style & W_TOOL) != 0)
		*dwExStyle |= WS_EX_TOOLWINDOW;
#if !IsWinCE
	if (parent == 0) {
		if ((style & W_ON_TOP) != 0) {
			int trim = W_TITLE | W_CLOSE | W_MIN | W_MAX;
			if ((style & W_NO_TRIM) != 0 || (style & trim) == 0) {
				*dwExStyle |= WS_EX_TOOLWINDOW;
			}
		}
	}
#endif
	if ((style & W_ON_TOP) != 0)
		*dwExStyle |= WS_EX_TOPMOST;
	*dwStyle &= ~WS_CHILD;
	if (!(style & W_NO_TRIM))
		*dwStyle |= WS_OVERLAPPED | WS_CAPTION;
}
#define W_SHELL_ACCEL_GROW 0x10
int _w_shell_add_accel(w_shell *shell, _w_menuitem *item, int accel) {
	struct _w_shell_accel *acc = _W_SHELL(shell)->accel, *newacc;
	int newindex = 0;
	if (acc == 0) {
		acc = (struct _w_shell_accel*) malloc(
				sizeof(struct _w_shell_accel)
						+ W_SHELL_ACCEL_GROW * sizeof(acc->acc[0]));
		if (acc == 0)
			return W_ERROR_NO_MEMORY;
		acc->alloc = W_SHELL_ACCEL_GROW;
		acc->count = 0;
	} else {
		for (int i = 0; i < acc->count; i++) {
			if (acc->acc[i].menu == 0) {
				acc->acc[i].accelerator = accel;
				acc->acc[i].menu = _W_MENUITEM(item)->parentItem;
				acc->acc[i].index = _W_MENUITEM(item)->index;
				return W_TRUE;
			}
		}
		if (acc->count >= acc->alloc) {
			newacc = (struct _w_shell_accel*) malloc(
					sizeof(struct _w_shell_accel)
							+ (acc->alloc + W_SHELL_ACCEL_GROW)
									* sizeof(acc->acc[0]));
			if (newacc == 0)
				return W_ERROR_NO_MEMORY;
			memcpy(newacc, acc,
					sizeof(struct _w_shell_accel)
							+ acc->alloc * sizeof(acc->acc[0]));
			newacc->alloc = acc->alloc + W_SHELL_ACCEL_GROW;
			newacc->count = acc->count;
			free(acc);
			acc = newacc;
		}
	}
	acc->acc[acc->count].accelerator = accel;
	acc->acc[acc->count].menu = _W_MENUITEM(item)->parentItem;
	acc->acc[acc->count].index = _W_MENUITEM(item)->index;
	acc->count++;
	return W_TRUE;
}

int _w_shell_create_accelerators(w_shell *shell) {
	struct _w_shell_accel *acc = _W_SHELL(shell)->accel;
	if (acc == 0)
		return W_FALSE;
	if (acc->count == 0)
		return W_FALSE;
	ACCEL *_acc = malloc(_W_SHELL(shell)->accel->count * sizeof(ACCEL));
	if (_acc == 0)
		return W_FALSE;
	int nAccel = 0;
	for (int i = 0; i < acc->count; i++) {
		if (acc->acc[i].menu != 0) {
			if (_w_menuitem_fill_accel(&_acc[nAccel], &acc->acc[i]))
				nAccel++;
		}
	}
	if (nAccel != 0)
		_W_SHELL(shell)->hAccel = CreateAcceleratorTableW(_acc, nAccel);
	return W_TRUE;
}
wresult _w_shell_balloontip_handle(w_shell *shell,
		_w_shell_reserved *reserved) {
	if (_W_SHELL(shell)->balloonTipHandle == 0) {
		return _w_shell_create_balloontip_handle(shell, reserved);
	}
	return W_TRUE;
}
wresult _w_shell_post_event_platform(w_widget *widget, _w_event_platform *ee,
		_w_widget_reserved *reserved) {
	/*if (handle == 0) return 0;
	 if (e->hwnd == toolTipHandle || e->hwnd == balloonTipHandle || e->hwnd == menuItemToolTipHandle) {
	 return CallWindowProc (ToolTipProc, e->hwnd, e->msg, e->wParam, e->lParam);
	 }
	 if (hwndMDIClient != 0) {
	 return DefFrameProc (e->hwnd, e->hwndMDIClient, e->msg, e->wParam, e->lParam);
	 }
	 if (windowProc != 0) {
	 return CallWindowProc (windowProc, e->hwnd, e->msg, e->wParam, e->lParam);
	 }
	 if ((style & SWT.TOOL) != 0) {
	 int trim = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX | SWT.BORDER | SWT.RESIZE;
	 if ((style & trim) == 0) return DefWindowProc (e->hwnd, e->msg, e->wParam, e->lParam);
	 }
	 if ((style & SWT.NO_MOVE) != 0) {
	 setItemEnabled (SC_MOVE, false);
	 }
	 if (parent != null) {
	 switch (msg) {
	 case WM_KILLFOCUS:
	 case WM_SETFOCUS:
	 return DefWindowProc (e->hwnd, e->msg, e->wParam, e->lParam);
	 }
	 return CallWindowProc (DialogProc, e->hwnd, e->msg, e->wParam, e->lParam);
	 }
	 return DefWindowProc (e->hwnd, e->msg, e->wParam, e->lParam);*/
	return _w_control_post_event_platform(widget, ee, reserved);
}
wresult _w_shell_post_event(w_widget *widget, w_event *ee) {
	if (ee->type == W_EVENT_PLATFORM) {
		return _w_shell_post_event_platform(widget, (_w_event_platform*) ee,
				_w_widget_get_reserved(widget));
	}
	return _w_control_post_event(widget, ee);
}

void _w_shell_center(w_shell *shell) {
	/*if (parent == null) return;
	 Rectangle rect = getBoundsInPixels ();
	 Rectangle parentRect = display.mapInPixels (parent, null, parent.getClientAreaInPixels());
	 int x = Math.max (parentRect.x, parentRect.x + (parentRect.width - rect.width) / 2);
	 int y = Math.max (parentRect.y, parentRect.y + (parentRect.height - rect.height) / 2);
	 Rectangle monitorRect = parent.getMonitor ().getClientArea();
	 if (x + rect.width > monitorRect.x + monitorRect.width) {
	 x = Math.max (monitorRect.x, monitorRect.x + monitorRect.width - rect.width);
	 } else {
	 x = Math.max (x, monitorRect.x);
	 }
	 if (y + rect.height > monitorRect.y + monitorRect.height) {
	 y = Math.max (monitorRect.y, monitorRect.y + monitorRect.height - rect.height);
	 } else {
	 y = Math.max (y, monitorRect.y);
	 }
	 setLocationInPixels (x, y);*/
}
void _w_shell_close_widget(w_shell *shell, _w_event_platform *e) {
	wuint64 style = w_widget_get_style(W_WIDGET(shell));
	w_event event;
	event.type = W_EVENT_CLOSE;
	event.platform_event = (w_event_platform*) e;
	event.widget = W_WIDGET(shell);
	event.data = 0;
	_w_widget_send_event(W_WIDGET(shell), &event);
	if (style & W_DISPOSE_ON_CLOSE) {
		w_widget_dispose(W_WIDGET(shell));
	}
}
void _w_shell_close(w_shell *shell) {
	_w_shell_close_widget(shell, 0);
}
wresult _w_shell_create_balloontip_handle(w_shell *shell,
		_w_shell_reserved *reserved) {
	_W_SHELL(shell)->balloonTipHandle = _CreateWindow(0, TOOLTIPS_CLASSA,
	TTS_ALWAYSTIP | TTS_NOPREFIX | TTS_BALLOON,
	_W_WIDGET(shell)->handle, 0);
	if (_W_SHELL(shell)->balloonTipHandle == 0)
		return W_ERROR_NO_HANDLES;
	if (reserved->ToolTipProc == 0) {
		reserved->ToolTipProc = GetWindowLongPtr(
		_W_SHELL(shell)->balloonTipHandle, GWLP_WNDPROC);
	}
	/*
	 * Feature in Windows.  Despite the fact that the
	 * tool tip text contains \r\n, the tooltip will
	 * not honour the new line unless TTM_SETMAXTIPWIDTH
	 * is set.  The fix is to set TTM_SETMAXTIPWIDTH to
	 * a large value.
	 */
	SendMessageW(_W_SHELL(shell)->balloonTipHandle, TTM_SETMAXTIPWIDTH, 0,
			0x7FFF);
	SetWindowLongPtrW(_W_SHELL(shell)->balloonTipHandle, GWLP_WNDPROC,
			(LONG_PTR) _w_widget_proc);
	return W_TRUE;
}
void _w_shell_set_system_menu(w_shell *shell) {
	HMENU hMenu = GetSystemMenu(_W_WIDGET(shell)->handle, FALSE);
	if (hMenu == 0)
		return;
	int oldCount = GetMenuItemCount(hMenu);
	wuint64 style = _W_WIDGET(shell)->style;
	if ((style & W_RESIZE) == 0) {
		DeleteMenu(hMenu, SC_SIZE, MF_BYCOMMAND);
	}
	if ((style & W_MIN) == 0) {
		DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
	}
	if ((style & W_MAX) == 0) {
		DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
	}
	if ((style & (W_MIN | W_MAX)) == 0) {
		DeleteMenu(hMenu, SC_RESTORE, MF_BYCOMMAND);
	}
	int newCount = GetMenuItemCount(hMenu);
	if ((style & W_CLOSE) == 0 || newCount != oldCount) {
		DeleteMenu(hMenu, SC_TASKLIST, MF_BYCOMMAND);
		MENUITEMINFOW info;
		info.cbSize = sizeof(MENUITEMINFOW);
		info.fMask = MIIM_ID;
		int index = 0;
		while (index < newCount) {
			if (GetMenuItemInfoW(hMenu, index, TRUE, &info)) {
				if (info.wID == SC_CLOSE)
					break;
			}
			index++;
		}
		if (index != newCount) {
			DeleteMenu(hMenu, index - 1, MF_BYPOSITION);
			if ((style & W_CLOSE) == 0) {
				DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			}
		}
	}
}
//@Override
wresult _w_shell_create_handle(w_control *control,
		_w_control_reserved *reserved) {
	int embedded = _W_WIDGET(control)->handle != 0
			&& (_W_WIDGET(control)->state & STATE_FOREIGN_HANDLE) == 0;

	/*
	 * On Windows 98 and NT, setting a window to be the
	 * top most window using HWND_TOPMOST can result in a
	 * parent dialog shell being moved behind its parent
	 * if the dialog has a sibling that is currently on top
	 * This only occurs using SetWindowPos (), not when the
	 * handle is created.
	 */
	/*
	 * The following code is intentionally commented.
	 */
//	if ((style & SWT.ON_TOP) != 0) display.lockActiveWindow = true;
	if (_W_WIDGET(control)->handle == 0 || embedded) {
		wresult result = _w_composite_create_handle(control, reserved);
		if (result <= 0)
			return result;
		if (_W_CONTROL(control)->parent != 0
				|| ((_W_WIDGET(control)->style & W_TOOL) != 0)) {
			_w_shell_set_system_menu(W_SHELL(control));
		}
	} else {
		_W_WIDGET(control)->state |= STATE_CANVAS;
		if ((_W_WIDGET(control)->style & (W_HSCROLL | W_VSCROLL)) == 0) {
			_W_WIDGET(control)->state |= STATE_THEME_BACKGROUND;
		}
		_W_SHELL(control)->windowProc = GetWindowLongPtrW(
		_W_WIDGET(control)->handle, GWLP_WNDPROC);
	}

	/*
	 * The following code is intentionally commented.
	 */
//	if ((style & SWT.ON_TOP) != 0)  display.lockActiveWindow = false;
	if (!embedded) {
		int bits = GetWindowLong(_W_WIDGET(control)->handle, GWL_STYLE);
		bits &= ~(WS_OVERLAPPED | WS_CAPTION);
		bits |= WS_POPUP;
		if ((_W_WIDGET(control)->style & W_TITLE) != 0)
			bits |= WS_CAPTION;
		if ((_W_WIDGET(control)->style & W_NO_TRIM) == 0) {
			if ((_W_WIDGET(control)->style & (W_BORDER | W_RESIZE)) == 0)
				bits |= WS_BORDER;
		}
		/*
		 * Bug in Windows.  When the WS_CAPTION bits are cleared using
		 * SetWindowLong(), Windows does not resize the client area of
		 * the window to get rid of the caption until the first resize.
		 * The fix is to use SetWindowPos() with SWP_DRAWFRAME to force
		 * the frame to be redrawn and resized.
		 */
		SetWindowLong(_W_WIDGET(control)->handle, GWL_STYLE, bits);
		int flags = SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER
				| SWP_NOACTIVATE;
		SetWindowPos(_W_WIDGET(control)->handle, 0, 0, 0, 0, 0, flags);
	}
	if (win_toolkit->IsDBLocale) {
		HIMC hIMC = ImmCreateContext();
		if (hIMC != 0)
			ImmAssociateContext(_W_WIDGET(control)->handle, hIMC);
	}
	return W_TRUE;
}

void _w_shell_create_tooltip(w_shell *shell, _w_tooltip *toolTip) {
	int id = 110;
	toolTip->next = 0;
	if (_W_SHELL(shell)->tooltips == 0) {
		_W_SHELL(shell)->tooltips = toolTip;
		toolTip->prev = toolTip;
	} else {
		_w_tooltip *last = _W_SHELL(shell)->tooltips->prev;
		last->next = toolTip;
		toolTip->prev = last;
		_W_SHELL(shell)->tooltips->prev = toolTip;
	}
	_W_TOOLTIP(toolTip)->id = id + ID_START;
	TOOLINFOW lpti;
	lpti.cbSize = sizeof(TOOLINFOW);
	lpti.hwnd = _W_WIDGET(shell)->handle;
	lpti.uId = _W_TOOLTIP(toolTip)->id;
	lpti.uFlags = TTF_TRACK;
	lpti.lpszText = LPSTR_TEXTCALLBACK;
	HWND hwndToolTip;
	if ((_W_WIDGET(toolTip)->style & W_BALLOON) != 0) {
		hwndToolTip = _W_SHELL(shell)->balloonTipHandle;
	} else {
		hwndToolTip = _W_SHELL(shell)->toolTipHandle;
	}
	SendMessageW(hwndToolTip, TTM_ADDTOOLW, 0, (LPARAM) &lpti);
}

wresult _w_shell_create_tooltip_handle(HWND *hwnd, HWND parent,
		_w_shell_reserved *reserved) {
	HWND toolTipHandle = _CreateWindow(0, TOOLTIPS_CLASSA,
	TTS_ALWAYSTIP | TTS_NOPREFIX, parent, 0);
	if (toolTipHandle == 0)
		return W_ERROR_NO_HANDLES;
	if (reserved->ToolTipProc == 0) {
		reserved->ToolTipProc = (WNDPROC) GetWindowLongPtrW(toolTipHandle,
		GWLP_WNDPROC);
	}
	/*
	 * Feature in Windows.  Despite the fact that the
	 * tool tip text contains \r\n, the tooltip will
	 * not honour the new line unless TTM_SETMAXTIPWIDTH
	 * is set.  The fix is to set TTM_SETMAXTIPWIDTH to
	 * a large value.
	 */
	SendMessageW(toolTipHandle, TTM_SETMAXTIPWIDTH, 0, 0x7FFF);
	SetWindowLongPtrW(toolTipHandle, GWLP_WNDPROC, (LONG_PTR) _w_widget_proc);
	*hwnd = toolTipHandle;
	return W_TRUE;
}

void _w_shell_destroy_tooltip(w_shell *shell, _w_tooltip *toolTip) {
	if (_W_SHELL(shell)->balloonTipHandle != 0) {
		TOOLINFOW lpti;
		lpti.cbSize = sizeof(lpti);
		lpti.uId = toolTip->id;
		lpti.hwnd = _W_WIDGET(shell)->handle;
		SendMessageW(_W_SHELL(shell)->balloonTipHandle, TTM_DELTOOLW, 0,
				(LPARAM) &lpti);
	}
	toolTip->id = 0;
}

//@Override
void destroyWidget() {
	/*fixActiveShell ();
	 super.destroyWidget ();*/
}

//@Override
void dispose() {
	/*
	 * This code is intentionally commented.  On some
	 * platforms, the owner window is repainted right
	 * away when a dialog window exits.  This behavior
	 * is currently unspecified.
	 */
//	/*
//	* Note:  It is valid to attempt to dispose a widget
//	* more than once.  If this happens, fail silently.
//	*/
//	if (!isValidWidget ()) return;
//	if (!isValidThread ()) error (SWT.ERROR_THREAD_INVALID_ACCESS);
//	Display oldDisplay = display;
	//super.dispose ();
	// widget is disposed at this point
//	if (oldDisplay != null) oldDisplay.update ();
}

//@Override
wresult _w_shell_enable_widget(w_control *control, int enabled) {
	if (enabled) {
		_W_WIDGET(control)->state &= ~STATE_DISABLED;
	} else {
		_W_WIDGET(control)->state |= STATE_DISABLED;
	}
	if (win_toolkit->TrimEnabled) {
		//if (isActive ()) setItemEnabled (OS.SC_CLOSE, enabled);
	} else {
		EnableWindow(_W_WIDGET(control)->handle, enabled);
	}
	return W_TRUE;
}
unsigned char _SHELL_SYSTEM_COLORS[] = {
COLOR_BTNFACE,
COLOR_WINDOW,
COLOR_BTNTEXT,
COLOR_WINDOWTEXT,
COLOR_HIGHLIGHT,
COLOR_SCROLLBAR, };
//@Override
HBRUSH _w_shell_find_brush(HBITMAP value, int lbStyle) {
	if (lbStyle == BS_SOLID) {
		const int SYSTEM_COLORS_LENGTH = sizeof(_SHELL_SYSTEM_COLORS)
				/ sizeof(_SHELL_SYSTEM_COLORS[0]);
		for (int i = 0; i < SYSTEM_COLORS_LENGTH; i++) {
			if (value == GetSysColor(_SHELL_SYSTEM_COLORS[i])) {
				return GetSysColorBrush(_SHELL_SYSTEM_COLORS[i]);
			}
		}
	}
	HBRUSH *brushes = win_toolkit->brushes;
	LOGBRUSH logBrush;
	for (int i = 0; i < BRUSHES_SIZE; i++) {
		HBRUSH hBrush = brushes[i];
		if (hBrush == 0)
			break;
		GetObjectW(hBrush, sizeof(LOGBRUSH), &logBrush);
		switch (logBrush.lbStyle) {
		case BS_SOLID:
			if (lbStyle == BS_SOLID) {
				if (logBrush.lbColor == value)
					return hBrush;
			}
			break;
		case BS_PATTERN:
			if (lbStyle == BS_PATTERN) {
				if (logBrush.lbHatch == value)
					return hBrush;
			}
			break;
		}
	}
	int length = BRUSHES_SIZE;
	HBRUSH hBrush = brushes[--length];
	if (hBrush != 0)
		DeleteObject(hBrush);
	for (int i = BRUSHES_SIZE - 1; i > 0; i--) {
		brushes[i] = brushes[i - 1];
	}
	switch (lbStyle) {
	case BS_SOLID:
		hBrush = CreateSolidBrush((int) value);
		break;
	case BS_PATTERN:
		hBrush = CreatePatternBrush(value);
		break;
	}
	return brushes[0] = hBrush;
}

//@Override
w_control* _w_shell_find_background_control(w_control *control) {
	return _W_CONTROL(control)->background != -1
			|| _W_CONTROL(control)->backgroundImage != 0 ? control : 0;
}

//@Override
w_cursor* _w_shell_find_cursor(w_control *control) {
//return cursor;
}

//@Override
w_control* _w_shell_find_theme_control(w_control *control) {
	return 0;
}

w_tooltip* _w_shell_find_tooltip(w_shell *shell, int id) {
	_w_tooltip *tooltips = _W_SHELL(shell)->tooltips;
	while (tooltips != 0) {
		if (tooltips->id == id) {
			return W_TOOLTIP(tooltips);
		}
		tooltips = tooltips->next;
	}
	return 0;
}

void _w_shell_fix_active_shell(w_shell *shell) {
	/*
	 * Feature in Windows.  When the active shell is disposed
	 * or hidden, Windows normally makes the parent shell active
	 * and assigns focus.  This does not happen when the parent
	 * shell is disabled.  Instead, Windows assigns focus to the
	 * next shell on the desktop (possibly a shell in another
	 * application).  The fix is to activate the disabled parent
	 * shell before disposing or hiding the active shell.
	 */
	HWND hwndParent = GetParent(_W_WIDGET(shell)->handle);
	if (hwndParent != 0 && _W_WIDGET(shell)->handle == GetActiveWindow()) {
		if (!IsWindowEnabled(hwndParent) && IsWindowVisible(hwndParent)) {
			SetActiveWindow(hwndParent);
		}
	}
}

void _w_shell_fix_shell(w_shell *shell, w_shell *newShell, w_control *control) {
	/*if (this == newShell) return;
	 if (control == lastActive) setActiveControl (null);
	 String toolTipText = control.toolTipText;
	 if (toolTipText != null) {
	 control.setToolTipText (this, null);
	 control.setToolTipText (newShell, toolTipText);
	 }*/
}

void _w_shell_fix_tooltip(w_shell *shell) {
	/*
	 * Bug in Windows.  On XP, when a tooltip is
	 * hidden due to a time out or mouse press,
	 * the tooltip remains active although no
	 * longer visible and won't show again until
	 * another tooltip becomes active.  If there
	 * is only one tooltip in the window,  it will
	 * never show again.  The fix is to remove the
	 * current tooltip and add it again every time
	 * the mouse leaves the control.
	 */
	TOOLINFOW lpti;
	if (W_COMCTL32_VERSION >= VERSION(6, 0)) {
		HWND toolTipHandle = _W_SHELL(shell)->toolTipHandle;
		if (toolTipHandle == 0)
			return;
		lpti.cbSize = sizeof(&lpti);
		if (SendMessageW(toolTipHandle, TTM_GETCURRENTTOOLW, 0, (LPARAM) &lpti)
				!= 0) {
			if ((lpti.uFlags & TTF_IDISHWND) != 0) {
				SendMessageW(toolTipHandle, TTM_DELTOOLW, 0, (LPARAM) &lpti);
				SendMessageW(toolTipHandle, TTM_ADDTOOLW, 0, (LPARAM) &lpti);
			}
		}
		lpti.cbSize = sizeof(&lpti);
		HWND menuItemToolTipHandle = _W_SHELL(shell)->menuItemToolTipHandle;
		if (SendMessageW(menuItemToolTipHandle, TTM_GETCURRENTTOOLW, 0,
				(LPARAM) &lpti) != 0) {
			if ((lpti.uFlags & TTF_IDISHWND) != 0) {
				SendMessageW(menuItemToolTipHandle, TTM_DELTOOLW, 0,
						(LPARAM) &lpti);
				SendMessageW(menuItemToolTipHandle, TTM_ADDTOOLW, 0,
						(LPARAM) &lpti);
			}
		}
	}
}
void _w_shell_force_active(w_shell *shell) {
	/*if(!isVisible()) return;
	 OS.SetForegroundWindow (handle);*/
}

//@Override
void forceResize() {
	/* Do nothing */
}
int _w_shell_get_alpha(w_shell *shell) {
	/*checkWidget ();
	 if (!OS.IsWinCE && OS.WIN32_VERSION >= OS.VERSION (5, 1)) {
	 byte [] pbAlpha = new byte [1];
	 if (OS.GetLayeredWindowAttributes (handle, null, pbAlpha, null)) {
	 return pbAlpha [0] & 0xFF;
	 }
	 }
	 return 0xFF;*/
}
//@Override
wresult getBoundsInPixels() {
	/*if (!OS.IsWinCE) {
	 if (OS.IsIconic (handle)) return super.getBoundsInPixels ();
	 }
	 RECT rect = new RECT ();
	 OS.GetWindowRect (handle, rect);
	 int width = rect.right - rect.left;
	 int height = rect.bottom - rect.top;
	 return new Rectangle (rect.left, rect.top, width, height);*/
}

w_tooltip* getCurrentToolTip() {
	/*if (toolTipHandle != 0) {
	 ToolTip tip = getCurrentToolTip (toolTipHandle);
	 if (tip != null) return tip;
	 }
	 if (balloonTipHandle != 0) {
	 ToolTip tip = getCurrentToolTip (balloonTipHandle);
	 if (tip != null) return tip;
	 }
	 if (menuItemToolTipHandle != 0) {
	 ToolTip tip = getCurrentToolTip (menuItemToolTipHandle);
	 if (tip != null) return tip;
	 }
	 return null;*/
}

w_tooltip* getCurrentToolTip(long hwndToolTip) {
	/*if (hwndToolTip == 0) return null;
	 if (OS.SendMessage (hwndToolTip, OS.TTM_GETCURRENTTOOL, 0, 0) != 0) {
	 TOOLINFO lpti = new TOOLINFO ();
	 lpti.cbSize = TOOLINFO.sizeof;
	 if (OS.SendMessage (hwndToolTip, OS.TTM_GETCURRENTTOOL, 0, lpti) != 0) {
	 if ((lpti.uFlags & OS.TTF_IDISHWND) == 0) return findToolTip ((int)lpti.uId);
	 }
	 }
	 return null;*/
}
//@Override
wresult getEnabled() {
	/*checkWidget ();
	 return (state & DISABLED) == 0;*/
}
int _w_shell_get_full_screen(w_shell *shell) {
//return fullScreen;
}
int _w_shell_get_ime_input_mode(w_shell *shell) {
	/*if (!OS.IsDBLocale) return 0;
	 long  hIMC = OS.ImmGetContext (handle);
	 int [] lpfdwConversion = new int [1], lpfdwSentence = new int [1];
	 boolean open = OS.ImmGetOpenStatus (hIMC);
	 if (open) open = OS.ImmGetConversionStatus (hIMC, lpfdwConversion, lpfdwSentence);
	 OS.ImmReleaseContext (handle, hIMC);
	 if (!open) return SWT.NONE;
	 int result = 0;
	 if ((lpfdwConversion [0] & OS.IME_CMODE_ROMAN) != 0) result |= SWT.ROMAN;
	 if ((lpfdwConversion [0] & OS.IME_CMODE_FULLSHAPE) != 0) result |= SWT.DBCS;
	 if ((lpfdwConversion [0] & OS.IME_CMODE_KATAKANA) != 0) return result | SWT.PHONETIC;
	 if ((lpfdwConversion [0] & OS.IME_CMODE_NATIVE) != 0) return result | SWT.NATIVE;
	 return result | SWT.ALPHA;*/
}
//@Override
void getLocationInPixels() {
	/*if (!OS.IsWinCE) {
	 if (OS.IsIconic (handle)) {
	 return super.getLocationInPixels ();
	 }
	 }
	 RECT rect = new RECT ();
	 OS.GetWindowRect (handle, rect);
	 return new Point (rect.left, rect.top);*/
}
int _w_shell_get_maximized(w_shell *shell) {
//return !fullScreen && super.getMaximized ();
}
void _w_shell_get_minimum_size(w_shell *shell, w_size *size) {
	/*int width = Math.max (0, minWidth);
	 int trim = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX;
	 if ((style & SWT.NO_TRIM) == 0 && (style & trim) != 0) {
	 width = Math.max (width, OS.GetSystemMetrics (OS.SM_CXMINTRACK));
	 }
	 int height = Math.max (0, minHeight);
	 if ((style & SWT.NO_TRIM) == 0 && (style & trim) != 0) {
	 if ((style & SWT.RESIZE) != 0) {
	 height = Math.max (height, OS.GetSystemMetrics (OS.SM_CYMINTRACK));
	 } else {
	 RECT rect = new RECT ();
	 int bits1 = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 int bits2 = OS.GetWindowLong (handle, OS.GWL_EXSTYLE);
	 OS.AdjustWindowRectEx (rect, bits1, false, bits2);
	 height = Math.max (height, rect.bottom - rect.top);
	 }
	 }
	 return new Point (width,  height);*/
}
int _w_shell_get_modified(w_shell *shell) {
//return modified;
}
//@Override
wresult getRegion(w_region *region) {
//return region;
}
wresult _w_shell_get_shell(w_control *control, w_shell **shell) {
	*shell = W_SHELL(control);
	return W_TRUE;
}
//@Override
void getSizeInPixels() {
	/*if (!OS.IsWinCE) {
	 if (OS.IsIconic (handle)) return super.getSizeInPixels ();
	 }
	 RECT rect = new RECT ();
	 OS.GetWindowRect (handle, rect);
	 int width = rect.right - rect.left;
	 int height = rect.bottom - rect.top;
	 return new Point (width, height);*/
}
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
	_w_toolkit_get_shells_from_parent(shell, iterator);
	return W_TRUE;
}

struct w_toolbar* _w_shell_get_toolbar(w_shell *shell) {
}

//@Override
w_composite* findDeferredControl() {
//return layoutCount > 0 ? this : null;
}

//@Override
wresult isEnabled() {
//return getEnabled ();
}

//@Override
wresult isVisible() {
//return getVisible ();
}

long hwndMDIClient() {
	/*if (hwndMDIClient == 0) {
	 int widgetStyle = OS.MDIS_ALLCHILDSTYLES | OS.WS_CHILD | OS.WS_CLIPCHILDREN | OS.WS_CLIPSIBLINGS;
	 hwndMDIClient = OS.CreateWindowEx (
	 0,
	 new TCHAR (0, "MDICLIENT", true),
	 null,
	 widgetStyle,
	 0, 0, 0, 0,
	 handle,
	 0,
	 OS.GetModuleHandle (null),
	 new CREATESTRUCT ());
	 //		OS.ShowWindow (hwndMDIClient, OS.SW_SHOW);
	 }
	 return hwndMDIClient;*/
}

long menuItemToolTipHandle() {
	/*if (menuItemToolTipHandle == 0) createMenuItemToolTipHandle ();
	 return menuItemToolTipHandle;*/
}
void _w_shell_open(w_shell *shell) {
	/*STARTUPINFO lpStartUpInfo = Display.lpStartupInfo;
	 if (lpStartUpInfo == null || (lpStartUpInfo.dwFlags & OS.STARTF_USESHOWWINDOW) == 0) {
	 bringToTop ();
	 if (isDisposed ()) return;
	 }*/
	/*
	 * Feature on WinCE PPC.  A new application becomes
	 * the foreground application only if it has at least
	 * one visible window before the event loop is started.
	 * The workaround is to explicitly force the shell to
	 * be the foreground window.
	 */
	/*if (OS.IsWinCE) OS.SetForegroundWindow (handle);
	 OS.SendMessage (handle, OS.WM_CHANGEUISTATE, OS.UIS_INITIALIZE, 0);
	 setVisible (true);
	 if (isDisposed ()) return;*/
	/*
	 * Bug in Windows XP.  Despite the fact that an icon has been
	 * set for a window, the task bar displays the wrong icon the
	 * first time the window is made visible with ShowWindow() after
	 * a call to BringToTop(), when a long time elapses between the
	 * ShowWindow() and the time the event queue is read.  The icon
	 * in the window trimming is correct but the one in the task
	 * bar does not get updated.  The fix is to call PeekMessage()
	 * with the flag PM_NOREMOVE and PM_QS_SENDMESSAGE to respond
	 * to a cross thread WM_GETICON.
	 *
	 * NOTE: This allows other cross thread messages to be delivered,
	 * most notably WM_ACTIVATE.
	 */
	/*MSG msg = new MSG ();
	 int flags = OS.PM_NOREMOVE | OS.PM_NOYIELD | OS.PM_QS_SENDMESSAGE;
	 OS.PeekMessage (msg, 0, 0, 0, flags);*/
	/*
	 * When no widget has been given focus, or another push button has focus,
	 * give focus to the default button. This avoids overriding the default
	 * button.
	 */
	/*boolean restored = restoreFocus ();
	 if (!restored) {
	 restored = traverseGroup (true);
	 }
	 if (restored) {
	 Control focusControl = display.getFocusControl ();
	 if (focusControl instanceof Button && (focusControl.style & SWT.PUSH) != 0) {
	 restored = false;
	 }
	 }
	 if (!restored) {
	 if (saveDefault != null && !saveDefault.isDisposed ()) {
	 saveDefault.setFocus ();
	 } else {
	 setFocus ();
	 }
	 }*/
}
//@Override
wresult print(w_graphics *gc) {
	/*checkWidget ();
	 if (gc == null) error (SWT.ERROR_NULL_ARGUMENT);
	 if (gc.isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
	 return false;*/
}

void releaseBrushes() {
	/*if (brushes != null) {
	 for (int i=0; i<brushes.length; i++) {
	 if (brushes [i] != 0) OS.DeleteObject (brushes [i]);
	 }
	 }
	 brushes = null;*/
}
//@Override
void removeMenu(w_menu *menu) {
	/*super.removeMenu (menu);
	 if (menu == activeMenu) activeMenu = null;*/
}
//@Override
void requestLayout() {
//layout (null, SWT.DEFER);
}

//@Override
void reskinChildren(int flags) {
	/*Shell [] shells = getShells ();
	 for (int i=0; i<shells.length; i++) {
	 Shell shell = shells [i];
	 if (shell != null) shell.reskin (flags);
	 }
	 if (toolTips != null) {
	 for (int i=0; i<toolTips.length; i++) {
	 ToolTip toolTip = toolTips [i];
	 if (toolTip != null) toolTip.reskin (flags);
	 }
	 }
	 super.reskinChildren (flags);*/
}

LRESULT selectPalette(long hPalette) {
	/*long hDC = OS.GetDC (handle);
	 long hOld = OS.SelectPalette (hDC, hPalette, false);
	 int result = OS.RealizePalette (hDC);
	 if (result > 0) {
	 OS.InvalidateRect (handle, null, true);
	 } else {
	 OS.SelectPalette (hDC, hOld, true);
	 OS.RealizePalette (hDC);
	 }
	 OS.ReleaseDC (handle, hDC);
	 return (result > 0) ? LRESULT.ONE : LRESULT.ZERO;*/
}
//@Override
wresult sendKeyEvent(int type, int msg, long /*int*/wParam, long /*int*/lParam,
		w_event *event) {
	/*if (!isEnabled () || !isActive ()) return false;
	 return super.sendKeyEvent (type, msg, wParam, lParam, event);*/
}
void _w_shell_set_active(w_shell *shell) {
	/*if (!isVisible ()) return;
	 bringToTop ();*/
// widget could be disposed at this point
}
void setActiveControl(w_control *control) {
//setActiveControl (control, SWT.None);
}

void setActiveControl(w_control *control, int type) {
	/*if (control != null && control.isDisposed ()) control = null;
	 if (lastActive != null && lastActive.isDisposed ()) lastActive = null;
	 if (lastActive == control) return;*/

	/*
	 * Compute the list of controls to be activated and
	 * deactivated by finding the first common parent
	 * control.
	 */
	/*Control [] activate = (control == null) ? new Control [0] : control.getPath ();
	 Control [] deactivate = (lastActive == null) ? new Control [0] : lastActive.getPath ();
	 lastActive = control;
	 int index = 0, length = Math.min (activate.length, deactivate.length);
	 while (index < length) {
	 if (activate [index] != deactivate [index]) break;
	 index++;
	 }*/

	/*
	 * It is possible (but unlikely), that application
	 * code could have destroyed some of the widgets. If
	 * this happens, keep processing those widgets that
	 * are not disposed.
	 */
	/*for (int i=deactivate.length-1; i>=index; --i) {
	 if (!deactivate [i].isDisposed ()) {
	 deactivate [i].sendEvent (SWT.Deactivate);
	 }
	 }
	 for (int i=activate.length-1; i>=index; --i) {
	 if (!activate [i].isDisposed ()) {
	 Event event = new Event ();
	 event.detail = type;
	 activate [i].sendEvent (SWT.Activate, event);
	 }
	 }*/
}
void _w_shell_set_alpha(w_shell *shell, int alpha) {
#if !IsWinCE
	if (WIN32_VERSION >= VERSION(5, 1)) {
		alpha &= 0xFF;
		int bits = GetWindowLong(_W_WIDGET(shell)->handle, GWL_EXSTYLE);
		if (alpha == 0xFF) {
			SetWindowLong(_W_WIDGET(shell)->handle, GWL_EXSTYLE,
					bits & ~WS_EX_LAYERED);
			int flags = RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN;
			RedrawWindow(_W_WIDGET(shell)->handle, 0, 0, flags);
		} else {
			SetWindowLong(_W_WIDGET(shell)->handle, GWL_EXSTYLE,
					bits | WS_EX_LAYERED);
			SetLayeredWindowAttributes(_W_WIDGET(shell)->handle, 0,
					(byte) alpha,
					LWA_ALPHA);
		}
	}
#endif
}
//@Override
void setBoundsInPixels(int x, int y, int width, int height, int flags,
		boolean defer) {
//if (fullScreen) setFullScreen (false);
	/*
	 * Bug in Windows.  When a window has alpha and
	 * SetWindowPos() is called with SWP_DRAWFRAME,
	 * the contents of the window are copied rather
	 * than allowing the windows underneath to draw.
	 * This causes pixel corruption.  The fix is to
	 * clear the SWP_DRAWFRAME bits.
	 */
	/*int bits = OS.GetWindowLong (handle, OS.GWL_EXSTYLE);
	 if ((bits & OS.WS_EX_LAYERED) != 0) {
	 flags &= ~OS.SWP_DRAWFRAME;
	 }
	 super.setBoundsInPixels (x, y, width, height, flags, false);*/
}

//@Override
void setEnabled(int enabled) {
	/*if (((state & DISABLED) == 0) == enabled) return;
	 super.setEnabled (enabled);
	 if (enabled && handle == OS.GetActiveWindow ()) {
	 if (!restoreFocus ()) traverseGroup (true);
	 }*/
}
void _w_shell_set_full_screen(w_shell *shell, int fullScreen) {
	/*if (this.fullScreen == fullScreen) return;
	 int stateFlags = fullScreen ? OS.SW_SHOWMAXIMIZED : OS.SW_RESTORE;
	 int styleFlags = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 int mask = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX;
	 if ((style & mask) != 0) {
	 if (fullScreen) {
	 styleFlags &= ~(OS.WS_CAPTION | OS.WS_MAXIMIZEBOX | OS.WS_MINIMIZEBOX);
	 } else {
	 styleFlags |= OS.WS_CAPTION;
	 if ((style & SWT.MAX) != 0) styleFlags |= OS.WS_MAXIMIZEBOX;
	 if ((style & SWT.MIN) != 0) styleFlags |= OS.WS_MINIMIZEBOX;
	 }
	 }
	 if (fullScreen) wasMaximized = getMaximized ();
	 boolean visible = isVisible ();
	 OS.SetWindowLong (handle, OS.GWL_STYLE, styleFlags);
	 if (wasMaximized) {
	 OS.ShowWindow (handle, OS.SW_HIDE);
	 stateFlags = OS.SW_SHOWMAXIMIZED;
	 }
	 if (visible) OS.ShowWindow (handle, stateFlags);
	 OS.UpdateWindow (handle);
	 this.fullScreen = fullScreen;*/
}
void _w_shell_set_ime_input_mode(w_shell *shell, int mode) {
	/*if (!OS.IsDBLocale) return;
	 boolean imeOn = mode != SWT.NONE;
	 long hIMC = OS.ImmGetContext (handle);
	 OS.ImmSetOpenStatus (hIMC, imeOn);
	 if (imeOn) {
	 int [] lpfdwConversion = new int [1], lpfdwSentence = new int [1];
	 if (OS.ImmGetConversionStatus (hIMC, lpfdwConversion, lpfdwSentence)) {
	 int newBits = 0;
	 int oldBits = OS.IME_CMODE_NATIVE | OS.IME_CMODE_KATAKANA;
	 if ((mode & SWT.PHONETIC) != 0) {
	 newBits = OS.IME_CMODE_KATAKANA | OS.IME_CMODE_NATIVE;
	 oldBits = 0;
	 } else {
	 if ((mode & SWT.NATIVE) != 0) {
	 newBits = OS.IME_CMODE_NATIVE;
	 oldBits = OS.IME_CMODE_KATAKANA;
	 }
	 }
	 boolean fullShape = (mode & SWT.DBCS) != 0;
	 if ((mode & SWT.NATIVE) != 0) {
	 long hkl = OS.GetKeyboardLayout (0);
	 int langid = OS.PRIMARYLANGID (OS.LOWORD (hkl));
	 if (langid == OS.LANG_JAPANESE) {
	 fullShape = true;
	 }
	 }
	 if (fullShape) {
	 newBits |= OS.IME_CMODE_FULLSHAPE;
	 } else {
	 oldBits |= OS.IME_CMODE_FULLSHAPE;
	 }
	 if ((mode & SWT.ROMAN) != 0) {
	 newBits |= OS.IME_CMODE_ROMAN;
	 } else {
	 oldBits |= OS.IME_CMODE_ROMAN;
	 }
	 lpfdwConversion [0] |= newBits;
	 lpfdwConversion [0] &= ~oldBits;
	 OS.ImmSetConversionStatus (hIMC, lpfdwConversion [0], lpfdwSentence [0]);
	 }
	 }
	 OS.ImmReleaseContext (handle, hIMC);*/
}
void _w_shell_set_minimum_size(w_shell *shell, w_size *size) {
	/*int widthLimit = 0, heightLimit = 0;
	 int trim = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX;
	 if ((style & SWT.NO_TRIM) == 0 && (style & trim) != 0) {
	 widthLimit = OS.GetSystemMetrics (OS.SM_CXMINTRACK);
	 if ((style & SWT.RESIZE) != 0) {
	 heightLimit = OS.GetSystemMetrics (OS.SM_CYMINTRACK);
	 } else {
	 RECT rect = new RECT ();
	 int bits1 = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 int bits2 = OS.GetWindowLong (handle, OS.GWL_EXSTYLE);
	 OS.AdjustWindowRectEx (rect, bits1, false, bits2);
	 heightLimit = rect.bottom - rect.top;
	 }
	 }
	 minWidth = Math.max (widthLimit, width);
	 minHeight = Math.max (heightLimit, height);
	 Point size = getSizeInPixels ();
	 int newWidth = Math.max (size.x, minWidth);
	 int newHeight = Math.max (size.y, minHeight);
	 if (minWidth <= widthLimit) minWidth = SWT.DEFAULT;
	 if (minHeight <= heightLimit) minHeight = SWT.DEFAULT;
	 if (newWidth != size.x || newHeight != size.y) setSizeInPixels (newWidth, newHeight);*/
}
void _w_shell_set_modified(w_shell *shell, int modified) {
//this.modified = modified;
}
void setItemEnabled(int cmd, boolean enabled) {
	/*long hMenu = OS.GetSystemMenu (handle, false);
	 if (hMenu == 0) return;
	 int flags = OS.MF_ENABLED;
	 if (!enabled) flags = OS.MF_DISABLED | OS.MF_GRAYED;
	 OS.EnableMenuItem (hMenu, cmd, OS.MF_BYCOMMAND | flags);*/
}

//@Override
void setParent() {
	/* Do nothing.  Not necessary for Shells */
}
//@Override
void setRegion(w_region *region) {
	/*if ((style & SWT.NO_TRIM) == 0) return;
	 if (region != null) {
	 Rectangle bounds = region.getBounds ();
	 setSize (bounds.x + bounds.width, bounds.y + bounds.height);
	 }
	 super.setRegion (region);*/
}

void setToolTipText(long /*int*/hwnd, const char *text) {
	/*if (OS.IsWinCE) return;
	 TOOLINFO lpti = new TOOLINFO ();
	 lpti.cbSize = TOOLINFO.sizeof;
	 lpti.hwnd = handle;
	 lpti.uId = hwnd;
	 long hwndToolTip = toolTipHandle ();
	 if (text == null) {
	 OS.SendMessage (hwndToolTip, OS.TTM_DELTOOL, 0, lpti);
	 } else {
	 if (OS.SendMessage (hwndToolTip, OS.TTM_GETTOOLINFO, 0, lpti) != 0) {
	 OS.SendMessage (hwndToolTip, OS.TTM_UPDATE, 0, 0);
	 } else {
	 lpti.uFlags = OS.TTF_IDISHWND | OS.TTF_SUBCLASS;
	 lpti.lpszText = OS.LPSTR_TEXTCALLBACK;
	 OS.SendMessage (hwndToolTip, OS.TTM_ADDTOOL, 0, lpti);
	 }
	 }*/
}

void setToolTipText(NMTTDISPINFO lpnmtdi, void *buffer) {
	/*
	 * Ensure that the current position of the mouse
	 * is inside the client area of the shell.  This
	 * prevents tool tips from popping up over the
	 * shell trimmings.
	 */
	/*if (!hasCursor ()) return;
	 long hHeap = OS.GetProcessHeap ();
	 if (lpstrTip != 0) OS.HeapFree (hHeap, 0, lpstrTip);
	 int byteCount = buffer.length;
	 lpstrTip = OS.HeapAlloc (hHeap, OS.HEAP_ZERO_MEMORY, byteCount);
	 OS.MoveMemory (lpstrTip, buffer, byteCount);
	 lpnmtdi.lpszText = lpstrTip;*/
}

void setToolTipText(NMTTDISPINFO lpnmtdi, char *buffer) {
	/*
	 * Ensure that the current position of the mouse
	 * is inside the client area of the shell.  This
	 * prevents tool tips from popping up over the
	 * shell trimmings.
	 */
	/*if (!hasCursor ()) return;
	 long hHeap = OS.GetProcessHeap ();
	 if (lpstrTip != 0) OS.HeapFree (hHeap, 0, lpstrTip);
	 int byteCount = buffer.length * 2;
	 lpstrTip = OS.HeapAlloc (hHeap, OS.HEAP_ZERO_MEMORY, byteCount);
	 OS.MoveMemory (lpstrTip, buffer, byteCount);
	 lpnmtdi.lpszText = lpstrTip;*/
}

void setToolTipTitle(long /*int*/hwndToolTip, const char *text, int icon) {
	/*
	 * Bug in Windows.  For some reason, when TTM_SETTITLE
	 * is used to set the title of a tool tip, Windows leaks
	 * GDI objects.  This happens even when TTM_SETTITLE is
	 * called with TTI_NONE and NULL.  The documentation
	 * states that Windows copies the icon and that the
	 * programmer must free the copy but does not provide
	 * API to get the icon.  For example, when TTM_SETTITLE
	 * is called with ICON_ERROR, when TTM_GETTITLE is used
	 * to query the title and the icon, the uTitleBitmap
	 * field in the TTGETTITLE struct is zero.  The fix
	 * is to remember these values, only set them when then
	 * change and leak less.
	 *
	 * NOTE:  This only happens on Vista.
	 */
	/*if (hwndToolTip != toolTipHandle && hwndToolTip != balloonTipHandle && hwndToolTip != menuItemToolTipHandle) {
	 return;
	 }
	 if (hwndToolTip == toolTipHandle || hwndToolTip == menuItemToolTipHandle) {
	 if (text == toolTitle || (toolTitle != null && toolTitle.equals (text))) {
	 if (icon == toolIcon) return;
	 }
	 toolTitle = text;
	 toolIcon = icon;
	 } else {
	 if (hwndToolTip == balloonTipHandle) {
	 if (text == balloonTitle || (balloonTitle != null && balloonTitle.equals (text))) {
	 if (icon == toolIcon) return;
	 }
	 balloonTitle = text;
	 balloonIcon = icon;
	 }
	 }*/
	if (text != 0) {
		/*
		 * Feature in Windows. The text point to by pszTitle
		 * must not exceed 100 characters in length, including
		 * the null terminator.
		 */
		/*if (text.length () > 99) text = text.substring (0, 99);
		 TCHAR pszTitle = new TCHAR (getCodePage (), text, true);
		 OS.SendMessage (hwndToolTip, OS.TTM_SETTITLE, icon, pszTitle);*/
	} else {
//OS.SendMessage (hwndToolTip, OS.TTM_SETTITLE, 0, 0);
	}
}

//@Override
void setVisible(int visible) {
	/*
	 * Feature in Windows.  When ShowWindow() is called used to hide
	 * a window, Windows attempts to give focus to the parent. If the
	 * parent is disabled by EnableWindow(), focus is assigned to
	 * another windows on the desktop.  This means that if you hide
	 * a modal window before the parent is enabled, the parent will
	 * not come to the front.  The fix is to change the modal state
	 * before hiding or showing a window so that this does not occur.
	 */
	/*int mask = SWT.PRIMARY_MODAL | SWT.APPLICATION_MODAL | SWT.SYSTEM_MODAL;
	 if ((style & mask) != 0) {
	 if (visible) {
	 display.setModalShell (this);
	 if ((style & (SWT.APPLICATION_MODAL | SWT.SYSTEM_MODAL)) != 0) {
	 display.setModalDialog (null);
	 }
	 Control control = display._getFocusControl ();
	 if (control != null && !control.isActive ()) {
	 bringToTop ();
	 if (isDisposed ()) return;
	 }
	 long hwndShell = OS.GetActiveWindow ();
	 if (hwndShell == 0) {
	 if (parent != null) hwndShell = parent.handle;
	 }
	 if (hwndShell != 0) {
	 OS.SendMessage (hwndShell, OS.WM_CANCELMODE, 0, 0);
	 }
	 OS.ReleaseCapture ();
	 } else {
	 display.clearModal (this);
	 }
	 } else {
	 updateModal ();
	 }*/

	/*
	 * Bug in Windows.  Calling ShowOwnedPopups() to hide the
	 * child windows of a hidden window causes the application
	 * to be deactivated.  The fix is to call ShowOwnedPopups()
	 * to hide children before hiding the parent.
	 */
	/*if (showWithParent && !visible) {
	 if (!OS.IsWinCE) OS.ShowOwnedPopups (handle, false);
	 }
	 if (!visible) fixActiveShell ();
	 if (visible && center && !moved) {
	 center ();
	 if (isDisposed ()) return;
	 }
	 super.setVisible (visible);
	 if (isDisposed ()) return;
	 if (showWithParent != visible) {
	 showWithParent = visible;
	 if (visible) {
	 if (!OS.IsWinCE) OS.ShowOwnedPopups (handle, true);
	 }
	 }*/

	/* Make the foreign window parent appear in the task bar */
	if (visible) {
		/*if (parent != null && (parent.state & FOREIGN_HANDLE) != 0) {
		 long hwndParent = parent.handle;
		 int style = OS.GetWindowLong (hwndParent, OS.GWL_EXSTYLE);
		 if ((style & OS.WS_EX_TOOLWINDOW) != 0) {
		 OS.SetWindowLong (hwndParent, OS.GWL_EXSTYLE, style & ~OS.WS_EX_TOOLWINDOW);*/
		/*
		 * Bug in Windows.  The window does not show in the task bar when
		 * WS_EX_TOOLWINDOW is removed after the window has already been shown.
		 * The fix is to hide and shown the shell.
		 */
		/*ShowWindow (hwndParent, OS.SW_HIDE);
		 ShowWindow (hwndParent, OS.SW_RESTORE);
		 }
		 }*/
	}
}

//@Override
void subclass() {
	/*super.subclass ();
	 if (ToolTipProc != 0) {
	 long newProc = display.windowProc;
	 if (toolTipHandle != 0) {
	 OS.SetWindowLongPtr (toolTipHandle, OS.GWLP_WNDPROC, newProc);
	 }
	 if (balloonTipHandle != 0) {
	 OS.SetWindowLongPtr (balloonTipHandle, OS.GWLP_WNDPROC, newProc);
	 }
	 if (menuItemToolTipHandle != 0) {
	 OS.SetWindowLongPtr (menuItemToolTipHandle, OS.GWLP_WNDPROC, newProc);
	 }
	 }*/
}

long toolTipHandle() {
	/*if (toolTipHandle == 0) createToolTipHandle ();
	 return toolTipHandle;*/
}

//@Override
boolean translateAccelerator(MSG msg) {
	/*if (!isEnabled () || !isActive ()) return false;
	 if (menuBar != null && !menuBar.isEnabled ()) return false;
	 return translateMDIAccelerator (msg) || translateMenuAccelerator (msg);*/
}

//@Override
boolean traverseEscape() {
	/*if (parent == null) return false;
	 if (!isVisible () || !isEnabled ()) return false;
	 close ();
	 return true;*/
}

//@Override
void unsubclass() {
	/*super.unsubclass ();
	 if (ToolTipProc != 0) {
	 if (toolTipHandle != 0) {
	 OS.SetWindowLongPtr (toolTipHandle, OS.GWLP_WNDPROC, ToolTipProc);
	 }
	 if (toolTipHandle != 0) {
	 OS.SetWindowLongPtr (toolTipHandle, OS.GWLP_WNDPROC, ToolTipProc);
	 }
	 if (menuItemToolTipHandle != 0) {
	 OS.SetWindowLongPtr (menuItemToolTipHandle, OS.GWLP_WNDPROC, ToolTipProc);
	 }
	 }*/
}

void updateModal() {
	/*if (Display.TrimEnabled) {
	 setItemEnabled (OS.SC_CLOSE, isActive ());
	 } else {
	 OS.EnableWindow (handle, isActive ());
	 }*/
}

//@Override
CREATESTRUCT* widgetCreateStruct() {
//return null;
}

//@Override
long widgetParent() {
	/*if (handle != 0) return handle;
	 return parent != null ? parent.handle : 0;*/
}

//@Override
int widgetExtStyle() {
	/*int bits = super.widgetExtStyle () & ~OS.WS_EX_MDICHILD;
	 if ((style & SWT.TOOL) != 0) bits |= OS.WS_EX_TOOLWINDOW;*/

	/*
	 * Feature in Windows.  When a window that does not have a parent
	 * is created, it is automatically added to the Windows Task Bar,
	 * even when it has no title.  The fix is to use WS_EX_TOOLWINDOW
	 * which does not cause the window to appear in the Task Bar.
	 */
	/*if (!OS.IsWinCE) {
	 if (parent == null) {
	 if ((style & SWT.ON_TOP) != 0) {
	 int trim = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX;
	 if ((style & SWT.NO_TRIM) != 0 || (style & trim) == 0) {
	 bits |= OS.WS_EX_TOOLWINDOW;
	 }
	 }
	 }
	 }*/

	/*
	 * Bug in Windows 98 and NT.  Creating a window with the
	 * WS_EX_TOPMOST extended style can result in a dialog shell
	 * being moved behind its parent.  The exact case where this
	 * happens is a shell with two dialog shell children where
	 * each dialog child has another hidden dialog child with
	 * the WS_EX_TOPMOST extended style.  Clicking on either of
	 * the visible dialogs causes them to become active but move
	 * to the back, behind the parent shell.  The fix is to
	 * disallow the WS_EX_TOPMOST extended style on Windows 98
	 * and NT.
	 */
	/*if (parent != null) {
	 if (OS.IsWin95) return bits;
	 if (OS.WIN32_VERSION < OS.VERSION (4, 10)) {
	 return bits;
	 }
	 }
	 if ((style & SWT.ON_TOP) != 0) bits |= OS.WS_EX_TOPMOST;
	 return bits;*/
}

//@Override
TCHAR windowClass() {
	/*if (OS.IsSP) return DialogClass;
	 if ((style & SWT.TOOL) != 0) {
	 int trim = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX | SWT.BORDER | SWT.RESIZE;
	 if ((style & trim) == 0) return display.windowShadowClass;
	 }
	 return parent != null ? DialogClass : super.windowClass ();*/
}

//@Override
long windowProc() {
	/*if (windowProc != 0) return windowProc;
	 if (OS.IsSP) return DialogProc;
	 if ((style & SWT.TOOL) != 0) {
	 int trim = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX | SWT.BORDER | SWT.RESIZE;
	 if ((style & trim) == 0) return super.windowProc ();
	 }
	 return parent != null ? DialogProc : super.windowProc ();*/
}

//@Override
long /*int*/windowProc(long /*int*/hwnd, int msg, long /*int*/wParam,
		long /*int*/lParam) {
	/*if (handle == 0) return 0;
	 if((style & SWT.NO_MOVE) != 0 && msg == OS.WM_NCLBUTTONDOWN && wParam == OS.HTCAPTION) return 0;
	 if (hwnd == toolTipHandle || hwnd == balloonTipHandle || hwnd == menuItemToolTipHandle) {
	 switch (msg) {
	 case OS.WM_TIMER: {
	 if (wParam != ToolTip.TIMER_ID) break;
	 ToolTip tip = getCurrentToolTip (hwnd);
	 if (tip != null && tip.autoHide) {
	 tip.setVisible (false);
	 }
	 break;
	 }
	 case OS.WM_LBUTTONDOWN: {
	 ToolTip tip = getCurrentToolTip (hwnd);
	 if (tip != null) {
	 tip.setVisible (false);
	 tip.sendSelectionEvent (SWT.Selection);
	 }
	 break;
	 }
	 }
	 return callWindowProc (hwnd, msg, wParam, lParam);
	 }
	 return super.windowProc (hwnd, msg, wParam, lParam);*/
}

//@Override
int widgetStyle() {
	/*int bits = super.widgetStyle ();
	 if (handle != 0) return bits | OS.WS_CHILD;
	 bits &= ~OS.WS_CHILD;*/
	/*
	 * Feature in WinCE.  Calling CreateWindowEx () with WS_OVERLAPPED
	 * and a parent window causes the new window to become a WS_CHILD of
	 * the parent instead of a dialog child.  The fix is to use WS_POPUP
	 * for a window with a parent.
	 *
	 * Feature in WinCE PPC.  A window without a parent with WS_POPUP
	 * always shows on top of the Pocket PC 'Today Screen'. The fix
	 * is to not set WS_POPUP for a window without a parent on WinCE
	 * devices.
	 *
	 * NOTE: WS_POPUP causes CreateWindowEx () to ignore CW_USEDEFAULT
	 * and causes the default window location and size to be zero.
	 */
	/*if (OS.IsWinCE) {
	 if (OS.IsSP) return bits | OS.WS_POPUP;
	 return parent == null ? bits : bits | OS.WS_POPUP;
	 }*/

	/*
	 * Use WS_OVERLAPPED for all windows, either dialog or top level
	 * so that CreateWindowEx () will respect CW_USEDEFAULT and set
	 * the default window location and size.
	 *
	 * NOTE:  When a WS_OVERLAPPED window is created, Windows gives
	 * the new window WS_CAPTION style bits.  These two constants are
	 * as follows:
	 *
	 * 	WS_OVERLAPPED = 0
	 * 	WS_CAPTION = WS_BORDER | WS_DLGFRAME
	 *
	 */
//return bits | OS.WS_OVERLAPPED | OS.WS_CAPTION;
}

/*
 * public function
 */
wresult _w_shell_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
//boolean embedded = handle != 0 && (state & FOREIGN_HANDLE) == 0;

	/*
	 * On Windows 98 and NT, setting a window to be the
	 * top most window using HWND_TOPMOST can result in a
	 * parent dialog shell being moved behind its parent
	 * if the dialog has a sibling that is currently on top
	 * This only occurs using SetWindowPos (), not when the
	 * handle is created.
	 */
	/*
	 * The following code is intentionally commented.
	 */
//	if ((style & SWT.ON_TOP) != 0) display.lockActiveWindow = true;
	wresult result;
	DWORD dwExStyle = 0, dwStyle = 0;
	_w_shell_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), (w_composite*) parent,
			_w_shell_window_class(parent, style), dwExStyle, dwStyle);
	if (result > 0) {
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
		_w_toolkit_add_shell(_W_SHELL(widget));
	}
	return result;
}
struct w_button* _w_shell_get_default_button(w_shell *shell) {
}
w_image* _w_shell_get_image(w_shell *shell) {
}

w_menu* _w_shell_get_menu_bar(w_shell *shell) {
}
int _w_shell_get_minimized(w_shell *shell) {
}
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc, void *user_data,
		int enc) {
	wresult result;
	int length = GetWindowTextLengthW(_W_WIDGET(shell)->handle);
	WCHAR *s = _w_toolkit_malloc(length);
	if (s != 0) {
		GetWindowTextW(_W_WIDGET(shell)->handle, s, length);
		result = _win_text_set(s, length, alloc, user_data, enc);
		_w_toolkit_free(s, length);
		return result;
	} else
		return W_ERROR_NO_MEMORY;
}
wresult _w_shell_set_default_button(w_shell *shell, struct w_button *button) {
}
wresult _w_shell_set_image(w_shell *shell, w_image *image) {
}
wresult _w_shell_set_maximized(w_shell *shell, int maximized) {
}
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu) {
	if (_W_SHELL(shell)->menubar == menu)
		return W_TRUE;
	if (menu != 0) {
		if (!w_widget_is_ok(W_WIDGET(menu)))
			return W_ERROR_INVALID_ARGUMENT;
		if ((_W_WIDGET(menu)->style & W_BAR) == 0)
			return W_ERROR_MENU_NOT_BAR;
//if (menu.parent != this) error (SWT.ERROR_INVALID_PARENT);
	}
	_W_SHELL(shell)->menubar = menu;
	HMENU hMenu = menu != 0 ? _W_MENU(menu)->handle : 0;
	if (SetMenu(_W_WIDGET(shell)->handle, hMenu)) {
		return W_TRUE;
	} else
		return W_ERROR_CANNOT_SET_MENU;
}
wresult _w_shell_set_minimized(w_shell *shell, int minimized) {
}
wresult _w_shell_set_text(w_shell *shell, const char *text, size_t length,
		int enc) {
	if (text == 0)
		text = "";
	size_t newlength;
	WCHAR *s = _win_text_fix(text, length, &newlength, enc);
	WINBOOL result = SetWindowTextW(_W_WIDGET(shell)->handle, s);
	_win_text_free(text, s, newlength);
	if (result)
		return W_TRUE;
	else
		return W_ERROR_CANNOT_SET_TEXT;
	/* Use the character encoding for the default locale */
//TCHAR buffer = new TCHAR (0, string, true);
	/* Ensure that the title appears in the task bar.*/
	/*if ((state & FOREIGN_HANDLE) != 0) {
	 int hHeap = OS.GetProcessHeap ();
	 int byteCount = buffer.length () * TCHAR.sizeof;
	 int pszText = OS.HeapAlloc (hHeap, OS.HEAP_ZERO_MEMORY, byteCount);
	 OS.MoveMemory (pszText, buffer, byteCount);
	 OS.DefWindowProc (handle, OS.WM_SETTEXT, 0, pszText);
	 if (pszText != 0) OS.HeapFree (hHeap, 0, pszText);
	 } else {
	 OS.SetWindowText (handle, buffer);
	 }*/
}

int _SHELL_WM_CLOSE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_CLOSE (wParam, lParam);
	 if (result != null) return result;
	 if (isEnabled () && isActive ()) closeWidget ();
	 return LRESULT.ZERO;*/
	_w_shell_close_widget(W_SHELL(widget), e);
	e->result = 0;
	return W_FALSE;
}

int _SHELL_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_KILLFOCUS (wParam, lParam);
	 saveFocus ();
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_NCACTIVATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_NCACTIVATE (wParam, lParam);
	 if (result != null) return result;
	 if (wParam == 0) {
	 if (display.lockActiveWindow) return LRESULT.ZERO;
	 Control control = display.findControl (lParam);
	 if (control != null) {
	 Shell shell = getShell ();
	 Decorations decorations = control.menuShell ();
	 if (decorations.getShell () == shell) {
	 if (this instanceof Shell) return LRESULT.ONE;
	 if (display.ignoreRestoreFocus) {
	 if (display.lastHittest != OS.HTCLIENT) {
	 result = LRESULT.ONE;
	 }
	 }
	 }
	 }
	 }
	 if (!(this instanceof Shell)) {
	 long hwndShell = getShell().handle;
	 OS.SendMessage (hwndShell, OS.WM_NCACTIVATE, wParam, lParam);
	 }
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_QUERYOPEN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_QUERYOPEN (wParam, lParam);
	 if (result != null) return result;
	 sendEvent (SWT.Deiconify);
	 // widget could be disposed at this point
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SETFOCUS (wParam, lParam);
	 if (isDisposed ()) return result;
	 if (savedFocus != this) restoreFocus ();
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = W_FALSE;
	w_event ee;
	/*boolean changed = true;
	 if (resized) {
	 int newWidth = 0, newHeight = 0;
	 switch (e->wparam) {
	 case SIZE_MAXIMIZED:
	 GetWindowRect (handle, maxRect);
	 case SIZE_RESTORED:
	 newWidth = LOWORD (lParam);
	 newHeight = HIWORD (lParam);
	 break;
	 case SIZE_MINIMIZED:
	 Rectangle rect = getClientAreaInPixels ();
	 newWidth = rect.width;
	 newHeight = rect.height;
	 break;
	 }
	 changed = newWidth != oldWidth || newHeight != oldHeight;
	 if (changed) {
	 oldWidth = newWidth;
	 oldHeight = newHeight;
	 }
	 }
	 if (changed) {*/
	result = _CANVAS_WM_SIZE(widget, e, reserved);
//if (isDisposed ()) return result;
//}
	if (e->wparam == SIZE_MINIMIZED) {
		memset(&ee, 0, sizeof(ee));
		ee.type = W_EVENT_ICONIFY;
		ee.platform_event = e;
		ee.widget = widget;
		_w_widget_send_event(widget, &ee);
// widget could be disposed at this point
	}
	return result;
}

int _SHELL_WM_SYSCOMMAND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SYSCOMMAND (wParam, lParam);
	 if (result != null) return result;
	 if (!(this instanceof Shell)) {
	 int cmd = (int)wParam & 0xFFF0;
	 switch (cmd) {
	 case OS.SC_CLOSE: {
	 OS.PostMessage (handle, OS.WM_CLOSE, 0, 0);
	 return LRESULT.ZERO;
	 }
	 case OS.SC_NEXTWINDOW: {
	 traverseDecorations (true);
	 return LRESULT.ZERO;
	 }
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _wm_shell_ACTIVATE_0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_ACTIVATE (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Feature in AWT.  When an AWT Window is activated,
	 * for some reason, it seems to forward the WM_ACTIVATE
	 * message to the parent.  Normally, the parent is an
	 * AWT Frame.  When AWT is embedded in SWT, the SWT
	 * shell gets the WM_ACTIVATE and assumes that it came
	 * from Windows.  When an SWT shell is activated it
	 * restores focus to the last control that had focus.
	 * If this control is an embedded composite, it takes
	 * focus from the AWT Window.  The fix is to ignore
	 * WM_ACTIVATE messages that come from AWT Windows.
	 */
	/*if (OS.GetParent (lParam) == handle) {
	 TCHAR buffer = new TCHAR (0, 128);
	 OS.GetClassName (lParam, buffer, buffer.length ());
	 String className = buffer.toString (0, buffer.strlen ());
	 if (className.equals (Display.AWT_WINDOW_CLASS)) {
	 return LRESULT.ZERO;
	 }
	 }
	 int loWord = OS.LOWORD (wParam);
	 if (loWord != 0) {*/
	/*
	 * When the high word of wParam is non-zero, the activation
	 * state of the window is being changed while the window is
	 * minimized. If this is the case, do not report activation
	 * events or restore the focus.
	 */
	/*if (OS.HIWORD (wParam) != 0) return result;
	 Control control = display.findControl (lParam);
	 if (control == null || control instanceof Shell) {
	 if (this instanceof Shell) {
	 Event event = new Event ();
	 event.detail = loWord == OS.WA_CLICKACTIVE ? SWT.MouseDown : SWT.None;
	 sendEvent (SWT.Activate, event);
	 if (isDisposed ()) return LRESULT.ZERO;
	 }
	 }
	 if (restoreFocus ()) return LRESULT.ZERO;
	 } else {
	 Display display = this.display;
	 boolean lockWindow = display.isXMouseActive ();
	 if (lockWindow) display.lockActiveWindow = true;
	 Control control = display.findControl (lParam);
	 if (control == null || control instanceof Shell) {
	 if (this instanceof Shell) {
	 sendEvent (SWT.Deactivate);
	 if (!isDisposed ()) {
	 Shell shell = getShell ();
	 shell.setActiveControl (null);
	 // widget could be disposed at this point
	 }
	 }
	 }
	 if (lockWindow) display.lockActiveWindow = false;
	 if (isDisposed ()) return LRESULT.ZERO;
	 saveFocus ();
	 }
	 return result;*/
	return W_FALSE;
}
int _SHELL_WM_ACTIVATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Bug in Windows XP.  When a Shell is deactivated, the
	 * IME composition window does not go away. This causes
	 * repaint issues.  The fix is to commit the composition
	 * string.
	 */
	/*if (OS.LOWORD (wParam) == 0 && OS.IsDBLocale && hIMC != 0) {
	 if (OS.ImmGetOpenStatus (hIMC)) {
	 OS.ImmNotifyIME (hIMC, OS.NI_COMPOSITIONSTR, OS.CPS_COMPLETE, 0);
	 }
	 }*/

	/* Process WM_ACTIVATE */
	/*LRESULT result = _wm_shell_ACTIVATE_0 (wParam, lParam);
	 if (OS.LOWORD (wParam) == 0) {
	 if (lParam == 0 || (lParam != toolTipHandle && lParam != balloonTipHandle
	 && lParam != menuItemToolTipHandle)) {
	 ToolTip tip = getCurrentToolTip ();
	 if (tip != null) tip.setVisible (false);
	 }
	 }
	 return parent != null ? LRESULT.ZERO : result;*/
	return W_FALSE;
}

int _SHELL_WM_DESTROY(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
//LRESULT result = super.WM_DESTROY (wParam, lParam);
	/*
	 * When the shell is a WS_CHILD window of a non-SWT
	 * window, the destroy code does not get called because
	 * the non-SWT window does not call dispose ().  Instead,
	 * the destroy code is called here in WM_DESTROY.
	 */
	/*int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.WS_CHILD) != 0) {
	 releaseParent ();
	 release (false);
	 }
	 return result;*/
	_w_toolkit_remove_shell(_W_SHELL(widget));
	if (win_toolkit->shells == 0) {
		w_toolkit_post_quit(W_TOOLKIT(win_toolkit), EXIT_SUCCESS);
	}
	return W_FALSE;
}

int _SHELL_WM_ERASEBKGND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_ERASEBKGND (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Feature in Windows.  When a shell is resized by dragging
	 * the resize handles, Windows temporarily fills in black
	 * rectangles where the new contents of the shell should
	 * draw.  The fix is to always draw the background of shells.
	 *
	 * NOTE: This only happens on Vista.
	 */
	/*if (OS.WIN32_VERSION == OS.VERSION (6, 0)) {
	 drawBackground (wParam);
	 return LRESULT.ONE;
	 }
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_ENTERIDLE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_ENTERIDLE (wParam, lParam);
	 if (result != null) return result;
	 Display display = this.display;
	 if (display.runAsyncMessages (false)) display.wakeThread ();
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_GETMINMAXINFO(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_GETMINMAXINFO (wParam, lParam);
	 if (result != null) return result;
	 if (minWidth != SWT.DEFAULT || minHeight != SWT.DEFAULT) {
	 MINMAXINFO info = new MINMAXINFO ();
	 OS.MoveMemory (info, lParam, MINMAXINFO.sizeof);
	 if (minWidth != SWT.DEFAULT) info.ptMinTrackSize_x = minWidth;
	 if (minHeight != SWT.DEFAULT) info.ptMinTrackSize_y = minHeight;
	 OS.MoveMemory (lParam, info, MINMAXINFO.sizeof);
	 return LRESULT.ZERO;
	 }
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_MOUSEACTIVATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_MOUSEACTIVATE (wParam, lParam);
	 if (result != null) return result;*/

	/*
	 * Check for WM_MOUSEACTIVATE when an MDI shell is active
	 * and stop the normal shell activation but allow the mouse
	 * down to be delivered.
	 */
	/*int hittest = (short) OS.LOWORD (lParam);
	 switch (hittest) {
	 case OS.HTERROR:
	 case OS.HTTRANSPARENT:
	 case OS.HTNOWHERE:
	 break;
	 default: {
	 Control control = display._getFocusControl ();
	 if (control != null) {
	 Decorations decorations = control.menuShell ();
	 if (decorations.getShell () == this && decorations != this) {
	 display.ignoreRestoreFocus = true;
	 display.lastHittest = hittest;
	 display.lastHittestControl = null;
	 if (hittest == OS.HTMENU || hittest == OS.HTSYSMENU) {
	 display.lastHittestControl = control;
	 return null;
	 }
	 return new LRESULT (OS.MA_NOACTIVATE);
	 }
	 }
	 }
	 }
	 if (hittest == OS.HTMENU) return null;*/

	/*
	 * Get the current location of the cursor,
	 * not the location of the cursor when the
	 * WM_MOUSEACTIVATE was generated.  This is
	 * strictly incorrect but is necessary in
	 * order to support Activate and Deactivate
	 * events for embedded widgets that have
	 * their own event loop.  In that case, the
	 * cursor location reported by GetMessagePos()
	 * is the one for our event loop, not the
	 * embedded widget's event loop.
	 */
	/*POINT pt = new POINT ();
	 if (!OS.GetCursorPos (pt)) {
	 int pos = OS.GetMessagePos ();
	 OS.POINTSTOPOINT (pt, pos);
	 }
	 long hwnd = OS.WindowFromPoint (pt);
	 if (hwnd == 0) return null;
	 Control control = display.findControl (hwnd);*/

	/*
	 * When a shell is created with SWT.ON_TOP and SWT.NO_FOCUS,
	 * do not activate the shell when the user clicks on the
	 * the client area or on the border or a control within the
	 * shell that does not take focus.
	 */
	/*if (control != null && (control.state & CANVAS) != 0) {
	 if ((control.style & SWT.NO_FOCUS) != 0) {
	 int bits = SWT.ON_TOP | SWT.NO_FOCUS;
	 if ((style & bits) == bits) {
	 if (hittest == OS.HTBORDER || hittest == OS.HTCLIENT) {
	 return new LRESULT (OS.MA_NOACTIVATE);
	 }
	 }
	 }
	 }

	 long code = callWindowProc (handle, OS.WM_MOUSEACTIVATE, wParam, lParam);
	 setActiveControl (control, SWT.MouseDown);
	 return new LRESULT (code);*/
	return W_FALSE;
}
int _wm_shell_MOVE_0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (moved) {
	 Point location = getLocationInPixels ();
	 if (location.x == oldX && location.y == oldY) {
	 return null;
	 }
	 oldX = location.x;
	 oldY = location.y;
	 }
	 return super.WM_MOVE (wParam, lParam);*/
	return W_FALSE;
}
int _SHELL_WM_MOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = _wm_shell_MOVE_0 (wParam, lParam);
	 if (result != null) return result;
	 ToolTip tip = getCurrentToolTip ();
	 if (tip != null) tip.setVisible (false);
	 return result;*/
	return W_FALSE;
}

int _SHELL_WM_NCHITTEST(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (!OS.IsWindowEnabled (handle)) return null;
	 if (!isEnabled () || !isActive ()) {
	 if (!Display.TrimEnabled) return new LRESULT (OS.HTNOWHERE);
	 long hittest = callWindowProc (handle, OS.WM_NCHITTEST, wParam, lParam);
	 if (hittest == OS.HTCLIENT || hittest == OS.HTMENU) hittest = OS.HTBORDER;
	 return new LRESULT (hittest);
	 }
	 if (menuBar != null && !menuBar.getEnabled ()) {
	 long hittest = callWindowProc (handle, OS.WM_NCHITTEST, wParam, lParam);
	 if (hittest == OS.HTMENU) hittest = OS.HTBORDER;
	 return new LRESULT (hittest);
	 }
	 return null;*/
	return W_FALSE;
}

int _SHELL_WM_NCLBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_NCLBUTTONDOWN (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * When the normal activation was interrupted in WM_MOUSEACTIVATE
	 * because the active shell was an MDI shell, set the active window
	 * to the top level shell but lock the active window and stop focus
	 * changes.  This allows the user to interact the top level shell
	 * in the normal manner.
	 */
	/*if (!display.ignoreRestoreFocus) return result;
	 Display display = this.display;
	 display.lockActiveWindow = true;
	 long code = callWindowProc (handle, OS.WM_NCLBUTTONDOWN, wParam, lParam);
	 display.lockActiveWindow = false;
	 Control focusControl = display.lastHittestControl;
	 if (focusControl != null && !focusControl.isDisposed ()) {
	 focusControl.setFocus ();
	 }
	 display.lastHittestControl = null;
	 display.ignoreRestoreFocus = false;
	 return new LRESULT (code);*/
	return W_FALSE;
}

int _SHELL_WM_PALETTECHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (wParam != handle) {
	 long hPalette = display.hPalette;
	 if (hPalette != 0) return selectPalette (hPalette);
	 }
	 return super.WM_PALETTECHANGED (wParam, lParam);*/
	return W_FALSE;
}

int _SHELL_WM_QUERYNEWPALETTE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*long hPalette = display.hPalette;
	 if (hPalette != 0) return selectPalette (hPalette);
	 return super.WM_QUERYNEWPALETTE (wParam, lParam);*/
	return W_FALSE;
}

int _SHELL_WM_SETCURSOR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Feature in Windows.  When the shell is disabled
	 * by a Windows standard dialog (like a MessageBox
	 * or FileDialog), clicking in the shell does not
	 * bring the shell or the dialog to the front. The
	 * fix is to detect this case and bring the shell
	 * forward.
	 */
	/*int msg = OS.HIWORD (lParam);
	 if (msg == OS.WM_LBUTTONDOWN) {
	 if (!Display.TrimEnabled) {
	 Shell modalShell = display.getModalShell ();
	 if (modalShell != null && !isActive ()) {
	 long hwndModal = modalShell.handle;
	 if (OS.IsWindowEnabled (hwndModal)) {
	 OS.SetActiveWindow (hwndModal);
	 }
	 }
	 }
	 if (!OS.IsWindowEnabled (handle)) {
	 long hwndPopup = OS.GetLastActivePopup (handle);
	 if (hwndPopup != 0 && hwndPopup != handle) {
	 if (display.getControl (hwndPopup) == null) {
	 if (OS.IsWindowEnabled (hwndPopup)) {
	 OS.SetActiveWindow (hwndPopup);
	 }
	 }
	 }
	 }
	 }*/
	/*
	 * When the shell that contains a cursor is disabled,
	 * WM_SETCURSOR is called with HTERROR.  Normally,
	 * when a control is disabled, the parent will get
	 * mouse and cursor events.  In the case of a disabled
	 * shell, there is no enabled parent.  In order to
	 * show the cursor when a shell is disabled, it is
	 * necessary to override WM_SETCURSOR when called
	 * with HTERROR to set the cursor but only when the
	 * mouse is in the client area of the shell.
	 */
	/*int hitTest = (short) OS.LOWORD (lParam);
	 if (hitTest == OS.HTERROR) {
	 if (!getEnabled ()) {
	 Control control = display.getControl (wParam);
	 if (control == this && cursor != null) {
	 POINT pt = new POINT ();
	 int pos = OS.GetMessagePos ();
	 OS.POINTSTOPOINT (pt, pos);
	 OS.ScreenToClient (handle, pt);
	 RECT rect = new RECT ();
	 OS.GetClientRect (handle, rect);
	 if (OS.PtInRect (rect, pt)) {
	 OS.SetCursor (cursor.handle);
	 switch (msg) {
	 case OS.WM_LBUTTONDOWN:
	 case OS.WM_RBUTTONDOWN:
	 case OS.WM_MBUTTONDOWN:
	 case OS.WM_XBUTTONDOWN:
	 OS.MessageBeep (OS.MB_OK);
	 }
	 return LRESULT.ONE;
	 }
	 }
	 }
	 }
	 return super.WM_SETCURSOR (wParam, lParam);*/
	return W_FALSE;
}

int _SHELL_WM_SHOWWINDOW(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SHOWWINDOW (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Bug in Windows.  If the shell is hidden while the parent
	 * is iconic,  Windows shows the shell when the parent is
	 * deiconified.  This does not happen if the shell is hidden
	 * while the parent is not an icon.  The fix is to track
	 * visible state for the shell and refuse to show the shell
	 * when the parent is shown.
	 */
	/*if (lParam == OS.SW_PARENTOPENING) {
	 Control control = this;
	 while (control != null) {
	 Shell shell = control.getShell ();
	 if (!shell.showWithParent) return LRESULT.ZERO;
	 control = control.parent;
	 }
	 }
	 return result;*/
	return W_FALSE;
}
int _wm_shell_WINDOWPOSCHANGING_0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_WINDOWPOSCHANGING (wParam, lParam);
	 if (result != null) return result;
	 if (display.lockActiveWindow) {
	 WINDOWPOS lpwp = new WINDOWPOS ();
	 OS.MoveMemory (lpwp, lParam, WINDOWPOS.sizeof);
	 lpwp.flags |= OS.SWP_NOZORDER;
	 OS.MoveMemory (lParam, lpwp, WINDOWPOS.sizeof);
	 }
	 return result;*/
	return W_FALSE;
}
int _SHELL_WM_WINDOWPOSCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = _wm_shell_WINDOWPOSCHANGING_0 (wParam,lParam);
	 if (result != null) return result;
	 WINDOWPOS lpwp = new WINDOWPOS ();
	 OS.MoveMemory (lpwp, lParam, WINDOWPOS.sizeof);
	 if ((lpwp.flags & OS.SWP_NOSIZE) == 0) {
	 lpwp.cx = Math.max (lpwp.cx, minWidth);
	 int trim = SWT.TITLE | SWT.CLOSE | SWT.MIN | SWT.MAX;
	 if ((style & SWT.NO_TRIM) == 0 && (style & trim) != 0) {
	 lpwp.cx = Math.max (lpwp.cx, OS.GetSystemMetrics (OS.SM_CXMINTRACK));
	 }
	 lpwp.cy = Math.max (lpwp.cy, minHeight);
	 if ((style & SWT.NO_TRIM) == 0 && (style & trim) != 0) {
	 if ((style & SWT.RESIZE) != 0) {
	 lpwp.cy = Math.max (lpwp.cy, OS.GetSystemMetrics (OS.SM_CYMINTRACK));
	 } else {
	 RECT rect = new RECT ();
	 int bits1 = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 int bits2 = OS.GetWindowLong (handle, OS.GWL_EXSTYLE);
	 OS.AdjustWindowRectEx (rect, bits1, false, bits2);
	 lpwp.cy = Math.max (lpwp.cy, rect.bottom - rect.top);
	 }
	 }
	 OS.MoveMemory (lParam, lpwp, WINDOWPOS.sizeof);
	 }
	 return result;*/
	return W_FALSE;
}
void _w_shell_class_init(struct _w_shell_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SHELL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_shell_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_shell);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_shell);
	/*
	 * shell class
	 */
	W_WIDGET_CLASS(clazz)->create = _w_shell_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_shell_post_event;
	W_CONTROL_CLASS(clazz)->get_shell = _w_shell_get_shell;
	clazz->close = _w_shell_close;
	clazz->get_toolbar = _w_shell_get_toolbar;
	clazz->get_alpha = _w_shell_get_alpha;
	clazz->get_full_screen = _w_shell_get_full_screen;
	clazz->get_minimum_size = _w_shell_get_minimum_size;
	clazz->get_modified = _w_shell_get_modified;
	clazz->get_ime_input_mode = _w_shell_get_ime_input_mode;
	clazz->get_shells = _w_shell_get_shells;
	clazz->open = _w_shell_open;
	clazz->set_active = _w_shell_set_active;
	clazz->set_alpha = _w_shell_set_alpha;
	clazz->set_full_screen = _w_shell_set_full_screen;
	clazz->set_ime_input_mode = _w_shell_set_ime_input_mode;
	clazz->set_minimum_size = _w_shell_set_minimum_size;
	clazz->set_modified = _w_shell_set_modified;
	clazz->force_active = _w_shell_force_active;
	clazz->get_default_button = _w_shell_get_default_button;
	clazz->get_images = _w_shell_get_image;
	clazz->get_maximized = _w_shell_get_maximized;
	clazz->get_menu_bar = _w_shell_get_menu_bar;
	clazz->get_minimized = _w_shell_get_minimized;
	clazz->get_text = _w_shell_get_text;
	clazz->set_default_button = _w_shell_set_default_button;
	clazz->set_images = _w_shell_set_image;
	clazz->set_maximized = _w_shell_set_maximized;
	clazz->set_menu_bar = _w_shell_set_menu_bar;
	clazz->set_minimized = _w_shell_set_minimized;
	clazz->set_text = _w_shell_set_text;
	/*
	 * Overrider class
	 */

	/*
	 * reserved
	 */
	struct _w_shell_reserved *reserved = _W_SHELL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_w_toolkit_registre_class(win_toolkit);
//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->messages[_WM_CLOSE] = _SHELL_WM_CLOSE;
	msg->messages[_WM_KILLFOCUS] = _SHELL_WM_KILLFOCUS;
	msg->messages[_WM_NCACTIVATE] = _SHELL_WM_NCACTIVATE;
	msg->messages[_WM_QUERYOPEN] = _SHELL_WM_QUERYOPEN;
	msg->messages[_WM_SETFOCUS] = _SHELL_WM_SETFOCUS;
	msg->messages[_WM_SIZE] = _SHELL_WM_SIZE;
	msg->messages[_WM_SYSCOMMAND] = _SHELL_WM_SYSCOMMAND;
	msg->messages[_WM_ACTIVATE] = _SHELL_WM_ACTIVATE;
	msg->messages[_WM_DESTROY] = _SHELL_WM_DESTROY;
	msg->messages[_WM_ERASEBKGND] = _SHELL_WM_ERASEBKGND;
	msg->messages[_WM_ENTERIDLE] = _SHELL_WM_ENTERIDLE;
	msg->messages[_WM_GETMINMAXINFO] = _SHELL_WM_GETMINMAXINFO;
	msg->messages[_WM_MOUSEACTIVATE] = _SHELL_WM_MOUSEACTIVATE;
	msg->messages[_WM_MOVE] = _SHELL_WM_MOVE;
	msg->messages[_WM_NCHITTEST] = _SHELL_WM_NCHITTEST;
	msg->messages[_WM_NCLBUTTONDOWN] = _SHELL_WM_NCLBUTTONDOWN;
	msg->messages[_WM_PALETTECHANGED] = _SHELL_WM_PALETTECHANGED;
	msg->messages[_WM_QUERYNEWPALETTE] = _SHELL_WM_QUERYNEWPALETTE;
	msg->messages[_WM_SETCURSOR] = _SHELL_WM_SETCURSOR;
	msg->messages[_WM_SHOWWINDOW] = _SHELL_WM_SHOWWINDOW;
	msg->messages[_WM_WINDOWPOSCHANGING] = _SHELL_WM_WINDOWPOSCHANGING;
}
#endif
