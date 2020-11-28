/*
 * shell.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "shell.h"
#include "toolkit.h"
int _w_shell_check_style(w_shell *parent, int style) {
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
		//if ((style & W_TITLE) != 0)
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
			int trim = /*W_TITLE |*/W_CLOSE | W_MIN | W_MAX;
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
	result = _w_control_create(W_CONTROL(widget), (w_composite*) parent,
			_w_shell_window_class(parent, style), dwExStyle, dwStyle);
	if (result > 0) {
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
		_w_toolkit_add_shell(_W_SHELL(widget));
	}
	return result;
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
struct w_toolbar* _w_shell_get_toolbar(w_shell *shell) {
}
int _w_shell_get_alpha(w_shell *shell) {
}
int _w_shell_get_full_screen(w_shell *shell) {
}
void _w_shell_get_minimum_size(w_shell *shell, w_size *size) {
}
int _w_shell_get_modified(w_shell *shell) {
}
int _w_shell_get_ime_input_mode(w_shell *shell) {
}
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
	_w_toolkit_get_shells_from_parent(shell, iterator);
	return W_TRUE;
}
void _w_shell_open(w_shell *shell) {
	ShowWindow(_W_WIDGET(shell)->handle, SW_SHOWDEFAULT);
}
void _w_shell_set_active(w_shell *shell) {
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
					(byte) alpha, LWA_ALPHA);
		}
	}
#endif
}
void _w_shell_set_full_screen(w_shell *shell, int fullScreen) {
}
void _w_shell_set_ime_input_mode(w_shell *shell, int mode) {
}
void _w_shell_set_minimum_size(w_shell *shell, w_size *size) {
}
void _w_shell_set_modified(w_shell *shell, int modified) {
}
void _w_shell_force_active(w_shell *shell) {
}
struct w_button* _w_shell_get_default_button(w_shell *shell) {
}
w_image* _w_shell_get_image(w_shell *shell) {
}
int _w_shell_get_maximized(w_shell *shell) {
}
w_menu* _w_shell_get_menu_bar(w_shell *shell) {
}
int _w_shell_get_minimized(w_shell *shell) {
}
w_string_ref* _w_shell_get_text(w_shell *shell) {
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
wresult _w_shell_set_text(w_shell *shell, const char *text) {
	if (text == 0)
		text = "";
	struct UnicodeString str;
	unicode_set(&str, text, -1);
	WINBOOL result = SetWindowTextW(_W_WIDGET(shell)->handle, str.str);
	unicode_free(&str);
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
wresult _w_shell_get_shell(w_control *control, w_shell **shell) {
	*shell = W_SHELL(control);
	return W_TRUE;
}

int _SHELL_WM_CLOSE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_CLOSE (wParam, lParam);
	 if (result != null) return result;
	 if (isEnabled () && isActive ()) closeWidget ();
	 return LRESULT.ZERO;*/
	w_event event;
	event.type = W_EVENT_CLOSE;
	event.platform_event = (w_event_platform*) e;
	event.widget = widget;
	event.data = 0;
	_w_widget_send_event(widget, &event);
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
	W_WIDGET_CLASS(clazz)->create = _w_shell_create;
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
