/*
 * control.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "control.h"
#include "toolkit.h"
const char *___message[] = { "", "WM_ACTIVATE", "WM_CAPTURECHANGED",
		"WM_CHANGEUISTATE", "WM_CHAR", "WM_CLEAR", "WM_CLOSE", "WM_COMMAND",
		"WM_CONTEXTMENU", "WM_CTLCOLOR", "WM_CUT", "WM_CREATE", "WM_DESTROY",
		"WM_DRAWITEM", "WM_ENDSESSION", "WM_ENTERIDLE", "WM_ERASEBKGND",
		"WM_GESTURE", "WM_GETDLGCODE", "WM_GETFONT", "WM_GETOBJECT",
		"WM_GETMINMAXINFO", "WM_HELP", "WM_HSCROLL", "WM_IME_CHAR",
		"WM_IME_COMPOSITION", "WM_IME_COMPOSITION_START",
		"WM_IME_ENDCOMPOSITION", "WM_INITMENUPOPUP", "WM_INPUTLANGCHANGE",
		"WM_HOTKEY", "WM_KEYDOWN", "WM_KEYUP", "WM_KILLFOCUS",
		"WM_LBUTTONDBLCLK", "WM_LBUTTONDOWN", "WM_LBUTTONUP",
		"WM_MBUTTONDBLCLK", "WM_MBUTTONDOWN", "WM_MBUTTONUP", "WM_MEASUREITEM",
		"WM_MENUCHAR", "WM_MENUSELECT", "WM_MOUSEACTIVATE", "WM_MOUSEHOVER",
		"WM_MOUSELEAVE", "WM_MOUSEMOVE", "WM_MOUSEWHEEL", "WM_MOUSEHWHEEL",
		"WM_MOVE", "WM_NCACTIVATE", "WM_NCCALCSIZE", "WM_NCHITTEST",
		"WM_NCLBUTTONDOWN", "WM_NCPAINT", "WM_NOTIFY", "WM_PAINT",
		"WM_PALETTECHANGED", "WM_PARENTNOTIFY", "WM_PASTE", "WM_PRINT",
		"WM_PRINTCLIENT", "WM_QUERYENDSESSION", "WM_QUERYNEWPALETTE",
		"WM_QUERYOPEN", "WM_RBUTTONDBLCLK", "WM_RBUTTONDOWN", "WM_RBUTTONUP",
		"WM_SETCURSOR", "WM_SETFOCUS", "WM_SETFONT", "WM_SETTINGCHANGE",
		"WM_SETREDRAW", "WM_SHOWWINDOW", "WM_SIZE", "WM_SYSCHAR",
		"WM_SYSCOLORCHANGE", "WM_SYSCOMMAND", "WM_SYSKEYDOWN", "WM_SYSKEYUP",
		"WM_TABLET_FLICK", "WM_TIMER", "WM_TOUCH", "WM_UNDO",
		"WM_UNINITMENUPOPUP", "WM_UPDATEUISTATE", "WM_VSCROLL",
		"WM_WINDOWPOSCHANGED", "WM_WINDOWPOSCHANGING", "WM_XBUTTONDBLCLK",
		"WM_XBUTTONDOWN", "WM_XBUTTONUP", "WM_DPICHANGED", "WM_MENUCOMMAND",
		"WM_CTLCOLOR_CHILD", };
/*
 * internal function
 */
void _w_control_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	//if (!OS.IsPPC) {
	if ((style & W_BORDER) != 0)
		*dwExStyle |= WS_EX_CLIENTEDGE;
	//}
//	if ((style & SWT.BORDER) != 0) {
//		if ((style & SWT.FLAT) == 0) bits |= OS.WS_EX_CLIENTEDGE;
//	}
	/*
	 * Feature in Windows NT.  When CreateWindowEx() is called with
	 * WS_EX_LAYOUTRTL or WS_EX_NOINHERITLAYOUT, CreateWindowEx()
	 * fails to create the HWND. The fix is to not use these bits.
	 */
	/*if (OS.WIN32_VERSION < OS.VERSION (4, 10)) {
	 return bits;
	 } */
	*dwExStyle |= WS_EX_NOINHERITLAYOUT;
	if ((style & W_RIGHT_TO_LEFT) != 0)
		*dwExStyle |= WS_EX_LAYOUTRTL;
	/* Force clipping of siblings by setting WS_CLIPSIBLINGS */
	*dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
//	if ((style & SWT.BORDER) != 0) {
//		if ((style & SWT.FLAT) != 0) bits |= OS.WS_BORDER;
//	}
	/*	if (OS.IsPPC) {
	 if ((style & SWT.BORDER) != 0) bits |= OS.WS_BORDER;
	 }
	 return bits;*/

	/*
	 * This code is intentionally commented.  When clipping
	 * of both siblings and children is not enforced, it is
	 * possible for application code to draw outside of the
	 * control.
	 */
//	int bits = OS.WS_CHILD | OS.WS_VISIBLE;
//	if ((style & SWT.CLIP_SIBLINGS) != 0) bits |= OS.WS_CLIPSIBLINGS;
//	if ((style & SWT.CLIP_CHILDREN) != 0) bits |= OS.WS_CLIPCHILDREN;
//	return bits;
}
wresult _w_control_create(w_control *control, w_composite *parent,
		const char *clazz, DWORD dwExStyle, DWORD dwStyle) {
	struct UnicodeString str;
	unicode_set(&str, clazz, -1);
	HWND hwndParent;
	_W_CONTROL(control)->parent = parent;
	if (parent == 0) {
		hwndParent = 0;
	} else {
		if (W_WIDGET_GET_CLASS(parent) == 0)
			return W_ERROR_INVALID_PARENT;
		struct _w_control_reserved *resereved = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(W_WIDGET(parent)));
		hwndParent = resereved->topHandle(W_CONTROL(parent));
	}
	HWND hwnd = CreateWindowExW(dwExStyle, str.str, NULL, dwStyle,
	CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, hwndParent, 0, hinst, control);
	unicode_free(&str);
	if (hwnd == 0) {
		return W_ERROR_NO_HANDLES;
	}
	if (parent != 0) {
		_W_COMPOSITE(parent)->children_count++;
	}
	_W_WIDGET(control)->handle = hwnd;

	WNDPROC window_proc = (WNDPROC) GetWindowLongPtrW(hwnd,
	GWLP_WNDPROC);
	if (window_proc != _w_widget_proc) {
		struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(W_WIDGET(control)));
		if (reserved->default_proc == 0) {
			reserved->default_proc = (WNDPROC) GetWindowLongPtrW(hwnd,
			GWLP_WNDPROC);
		}
		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR) control);
		SetWindowLongPtrW(hwnd, GWLP_WNDPROC, (LONG_PTR) _w_widget_proc);
	}
	//SendMessage (hwnd, WM_SETFONT,win_toolkit->system_font, 0);
	/*	if (OS.IsDBLocale && hwndParent != 0) {
	 long hIMC = OS.ImmGetContext(hwndParent);
	 OS.ImmAssociateContext(handle, hIMC);
	 OS.ImmReleaseContext(hwndParent, hIMC);
	 }*/
	return W_TRUE;
}
void _w_control_def_proc(w_widget *widget, _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	if (_W_CONTROL_RESERVED(reserved)->default_proc != 0) {
		e->result = CallWindowProcW(_W_CONTROL_RESERVED(reserved)->default_proc, e->hwnd, e->msg,
				e->wparam, e->lparam);
	} else
		e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
}
HWND _w_control_h(w_control *control) {
	return _W_WIDGET(control)->handle;
}
/*
 *  function
 */
wresult _w_control_get_graphics(w_control *control, w_graphics *gc) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_graphics_dispose(gc);
	HDC hdc = GetDC(_W_WIDGET(control)->handle);
	if (hdc == 0)
		return W_ERROR_NO_HANDLES;
	_w_graphics_init(gc, hdc);
	_W_GRAPHICS(gc)->state |= W_GRAPHICS_RELEASEDC;
	_W_GRAPHICS(gc)->hwnd = _W_WIDGET(control)->handle;
	return W_TRUE;
}
int _w_control_get_orientation(w_control *control) {
	return _W_WIDGET(control)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT);
}
wresult _w_control_set_orientation(w_control *control, int orientation) {
}
int _w_control_get_text_direction(w_control *control) {
	int flags = WS_EX_LAYOUTRTL | WS_EX_RTLREADING;
	int bits = GetWindowLongW(_W_WIDGET(control)->handle, GWL_EXSTYLE) & flags;
	return bits == 0 || bits == flags ? W_LEFT_TO_RIGHT : W_RIGHT_TO_LEFT;
}
wresult _w_control_set_text_direction(w_control *control, int textDirection) {
}
int _w_control_print(w_control *control, w_graphics *gc) {
	/*if (gc == null) error (SWT.ERROR_NULL_ARGUMENT);
	 if (gc.isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
	 if (!OS.IsWinCE && OS.WIN32_VERSION >= OS.VERSION (5, 1)) {
	 long  topHandle = topHandle ();
	 long  hdc = gc.handle;
	 int state = 0;
	 long  gdipGraphics = gc.getGCData().gdipGraphics;
	 if (gdipGraphics != 0) {
	 long  clipRgn = 0;
	 Gdip.Graphics_SetPixelOffsetMode(gdipGraphics, Gdip.PixelOffsetModeNone);
	 long  rgn = Gdip.Region_new();
	 if (rgn == 0) error(SWT.ERROR_NO_HANDLES);
	 Gdip.Graphics_GetClip(gdipGraphics, rgn);
	 if (!Gdip.Region_IsInfinite(rgn, gdipGraphics)) {
	 clipRgn = Gdip.Region_GetHRGN(rgn, gdipGraphics);
	 }
	 Gdip.Region_delete(rgn);
	 Gdip.Graphics_SetPixelOffsetMode(gdipGraphics, Gdip.PixelOffsetModeHalf);
	 float[] lpXform = null;
	 long  matrix = Gdip.Matrix_new(1, 0, 0, 1, 0, 0);
	 if (matrix == 0) error(SWT.ERROR_NO_HANDLES);
	 Gdip.Graphics_GetTransform(gdipGraphics, matrix);
	 if (!Gdip.Matrix_IsIdentity(matrix)) {
	 lpXform = new float[6];
	 Gdip.Matrix_GetElements(matrix, lpXform);
	 }
	 Gdip.Matrix_delete(matrix);
	 hdc = Gdip.Graphics_GetHDC(gdipGraphics);
	 state = OS.SaveDC(hdc);
	 if (lpXform != null) {
	 OS.SetGraphicsMode(hdc, OS.GM_ADVANCED);
	 OS.SetWorldTransform(hdc, lpXform);
	 }
	 if (clipRgn != 0) {
	 OS.SelectClipRgn(hdc, clipRgn);
	 OS.DeleteObject(clipRgn);
	 }
	 }
	 if (OS.IsWinCE) {
	 OS.UpdateWindow (topHandle);
	 } else {
	 int flags = OS.RDW_UPDATENOW | OS.RDW_ALLCHILDREN;
	 OS.RedrawWindow (topHandle, null, 0, flags);
	 }
	 printWidget (topHandle, hdc, gc);
	 if (gdipGraphics != 0) {
	 OS.RestoreDC(hdc, state);
	 Gdip.Graphics_ReleaseHDC(gdipGraphics, hdc);
	 }
	 return true;
	 }*/
	return W_FALSE;
}
w_accessible* _w_control_get_accessible(w_control *control) {
}
wresult _w_control_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	HWND topHandle = reserved->topHandle(control);
	//forceResize ();
	RECT r;
	GetWindowRect(topHandle, &r);
	HWND hwndParent = GetParent(topHandle);
	MapWindowPoints(0, hwndParent, (LPPOINT) &r, 2);
	if (location != 0) {
		location->x = r.left;
		location->y = r.top;
	}
	if (size != 0) {
		size->width = r.right - r.left;
		size->height = r.bottom - r.top;
	}
	return W_TRUE;
}
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	HWND topHandle = reserved->topHandle(control);
	int f = SWP_NOZORDER | SWP_DRAWFRAME | SWP_NOACTIVATE;
	int x, y, w, h;
	if (location != 0) {
		x = location->x;
		y = location->y;
	} else {
		x = 0;
		y = 0;
	}
	if (size != 0) {
		w = size->width;
		h = size->height;
	} else {
		w = 0;
		h = 0;
	}
	if (SetWindowPos(topHandle, 0, x, y, w, h, f)) {
		return W_TRUE;
	} else
		return W_FALSE;
}
w_region* _w_control_get_region(w_control *control) {
}
wresult _w_control_set_region(w_control *control, w_region *region) {
}
wresult _w_control_move_above(w_control *control, w_control *_control) {
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	HWND topHandle = reserved->topHandle(control), hwndAbove = HWND_TOP;
	if (_control != 0) {
		if (!w_widget_is_ok(W_WIDGET(_control)))
			return W_ERROR_INVALID_ARGUMENT;
		if (_W_CONTROL(control)->parent != _W_CONTROL(_control)->parent)
			return W_FALSE;
		HWND hwnd = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(W_WIDGET(control)))->topHandle(_control);
		if (hwnd == 0 || hwnd == topHandle)
			return W_FALSE;
		hwndAbove = GetWindow(hwnd, GW_HWNDPREV);
		/*
		 * Bug in Windows.  For some reason, when GetWindow ()
		 * with GW_HWNDPREV is used to query the previous window
		 * in the z-order with the first child, Windows returns
		 * the first child instead of NULL.  The fix is to detect
		 * this case and move the control to the top.
		 */
		if (hwndAbove == 0 || hwndAbove == hwnd) {
			hwndAbove = HWND_TOP;
		}
	}
	int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
	SetWindowPos(topHandle, hwndAbove, 0, 0, 0, 0, flags);
	return W_TRUE;
}
wresult _w_control_move_below(w_control *control, w_control *_control) {
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	HWND topHandle = reserved->topHandle(control), hwndAbove = HWND_BOTTOM;
	w_composite *parent = _W_CONTROL(control)->parent;
	if (_control != 0) {
		if (!w_widget_is_ok(W_WIDGET(_control)))
			return W_ERROR_INVALID_ARGUMENT;
		if (parent != _W_CONTROL(_control)->parent)
			return W_FALSE;
		hwndAbove = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(W_WIDGET(control)))->topHandle(_control);
	} else {
		/*
		 * Bug in Windows.  When SetWindowPos() is called
		 * with HWND_BOTTOM on a dialog shell, the dialog
		 * and the parent are moved to the bottom of the
		 * desktop stack.  The fix is to move the dialog
		 * to the bottom of the dialog window stack by
		 * moving behind the first dialog child.
		 */
		w_shell *shell;
		w_control_get_shell(control, &shell);
		if (control == W_CONTROL(shell) && parent != 0) {
			/*
			 * Bug in Windows.  For some reason, when GetWindow ()
			 * with GW_HWNDPREV is used to query the previous window
			 * in the z-order with the first child, Windows returns
			 * the first child instead of NULL.  The fix is to detect
			 * this case and do nothing because the control is already
			 * at the bottom.
			 */
			HWND hwndParent = _W_WIDGET(parent)->handle, hwnd = hwndParent;
			hwndAbove = GetWindow(hwnd, GW_HWNDPREV);
			while (hwndAbove != 0 && hwndAbove != hwnd) {
				if (GetWindow(hwndAbove, GW_OWNER) == hwndParent)
					break;
				hwndAbove = GetWindow(hwnd = hwndAbove, GW_HWNDPREV);
			}
			if (hwndAbove == hwnd)
				return W_TRUE;
		}
	}
	if (hwndAbove == 0 || hwndAbove == topHandle)
		return W_FALSE;
	int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
	SetWindowPos(topHandle, hwndAbove, 0, 0, 0, 0, flags);
	return W_TRUE;
}
wresult _w_control_pack(w_control *control) {
	//setSize (computeSize (W_DEFAULT, W_DEFAULT, changed));
}
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point) {
}
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point) {
}
wresult _w_control_drag_detect(w_control *control, w_event *event) {
}
wresult _w_control_get_drag_detect(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_DRAG_DETECT) != 0;
}
wresult _w_control_set_drag_detect(w_control *control, int dragDetect) {
	/*if (dragDetect) {
	 state |= DRAG_DETECT;
	 } else {
	 state &= ~DRAG_DETECT;
	 }
	 enableDrag (dragDetect);*/
}
wresult _w_control_force_focus(w_control *control) {
	/*if (display.focusEvent == SWT.FocusOut) return false;
	 Decorations shell = menuShell ();
	 shell.setSavedFocus (this);
	 if (!isEnabled () || !isVisible () || !isActive ()) return false;
	 if (isFocusControl ()) return true;
	 shell.setSavedFocus (null);*/
	/*
	 * This code is intentionally commented.
	 *
	 * When setting focus to a control, it is
	 * possible that application code can set
	 * the focus to another control inside of
	 * WM_SETFOCUS.  In this case, the original
	 * control will no longer have the focus
	 * and the call to setFocus() will return
	 * false indicating failure.
	 *
	 * We are still working on a solution at
	 * this time.
	 */
//	if (OS.GetFocus () != OS.SetFocus (handle)) return false;
	SetFocus(_W_WIDGET(control)->handle);
	/* if (isDisposed ()) return false;
	 shell.setSavedFocus (this);
	 return isFocusControl ();*/
	return W_TRUE;
}
wresult _w_control_set_focus(w_control *control) {
	if ((_W_WIDGET(control)->style & W_NO_FOCUS) != 0)
		return W_FALSE;
	return _w_control_force_focus(control);
}
wresult _w_control_is_focus_control(w_control *control) {
	/*Control focusControl = display.focusControl;
	 if (focusControl != null && !focusControl.isDisposed ()) {
	 return this == focusControl;
	 }
	 return hasFocus ();*/
}
int _w_control_get_border_width(w_control *control) {
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	HWND borderHandle = reserved->borderHandle(control);
	int bits1 = GetWindowLong(borderHandle, GWL_EXSTYLE);
	if ((bits1 & WS_EX_CLIENTEDGE) != 0)
		return GetSystemMetrics(SM_CXEDGE);
	if ((bits1 & WS_EX_STATICEDGE) != 0)
		return GetSystemMetrics(SM_CXBORDER);
	int bits2 = GetWindowLong(borderHandle, GWL_STYLE);
	if ((bits2 & WS_BORDER) != 0)
		return GetSystemMetrics(SM_CXBORDER);
	return 0;
}
wresult _w_control_get_drawing(w_control *control) {
	//return drawCount <= 0;
	return TRUE;
}
w_cursor* _w_control_get_cursor(w_control *control) {
	if (_W_CONTROL(control)->cursor == 0) {
		return _w_toolkit_get_system_cursor(W_TOOLKIT(win_toolkit),
				W_CURSOR_ARROW);
	} else {
		return _W_CONTROL(control)->cursor;
	}
}
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor) {
	/*if (cursor != null && cursor.isDisposed()) error(SWT.ERROR_INVALID_ARGUMENT);
	 this.cursor = cursor;
	 if (OS.IsWinCE) {
	 long hCursor = cursor != null ? cursor.handle : 0;
	 OS.SetCursor (hCursor);
	 return;
	 }
	 long hwndCursor = OS.GetCapture ();
	 if (hwndCursor == 0) {
	 POINT pt = new POINT ();
	 if (!OS.GetCursorPos (pt)) return;
	 long hwnd = hwndCursor = OS.WindowFromPoint (pt);
	 while (hwnd != 0 && hwnd != handle) {
	 hwnd = OS.GetParent (hwnd);
	 }
	 if (hwnd == 0) return;
	 }
	 Control control = display.getControl (hwndCursor);
	 if (control == null) control = this;
	 control.setCursor ();*/
}
wresult _w_control_get_enabled(w_control *control) {
	return IsWindowEnabled(_W_WIDGET(control)->handle);
}
wresult _w_control_set_enabled(w_control *control, int enabled) {
	if (EnableWindow(_W_WIDGET(control)->handle, enabled))
		return W_TRUE;
	else
		return W_FALSE;
}
wresult _w_control_is_enabled(w_control *control) {
	return IsWindowEnabled(_W_WIDGET(control)->handle);
}
wresult _w_control_get_font(w_control *control, w_font **font) {
	if (_W_WIDGET(control)->state & STATE_SET_FONT) {
		*font = _W_CONTROL(control)->font;
	} else {
		HFONT hFont = (HFONT) SendMessageW(_W_WIDGET(control)->handle,
		WM_GETFONT, 0, 0);
		if (hFont == 0) {
			*font = w_toolkit_get_system_font(
					w_widget_get_toolkit(W_WIDGET(control)));
		} else {
			_W_CONTROL(control)->_font.handle = hFont;
			*font = (w_font*) & _W_CONTROL(control)->_font;
		}
	}
	return W_TRUE;
}
wresult _w_control_set_font(w_control *control, w_font *font) {
	if (font == 0 || _W_FONT(font)->handle == 0) {
		return W_ERROR_INVALID_ARGUMENT;
	} else {
		_W_CONTROL(control)->font = font;
		_W_WIDGET(control)->state |= STATE_SET_FONT;
		SendMessageW(_W_WIDGET(control)->handle, WM_SETFONT, (WPARAM)
		_W_FONT(font)->handle, 0);
		return W_TRUE;
	}
}
w_color _w_control_get_background(w_control *control) {
	if (_W_CONTROL(control)->background == 0) {
#if IsWinCE
		return GetSysColor (COLOR_WINDOW);
#else
		return GetSysColor(COLOR_BTNFACE);
#endif
	} else
		return _W_CONTROL(control)->background;
}

wresult _w_control_set_background(w_control *control, w_color color) {
	_W_CONTROL(control)->background = color | 0xFF000000;
	/*if (_W_CONTROL(control)->hBrush != 0) {
	 DeleteObject(_W_CONTROL(control)->hBrush);
	 }
	 _W_CONTROL(control)->hBrush = CreateSolidBrush(color & 0x00FFFFFF);*/
#if IsWinCE
		InvalidateRect (handle, null, true);
#else
	int flags = RDW_ERASE | RDW_FRAME | RDW_INVALIDATE;
	RedrawWindow(_W_WIDGET(control)->handle, 0, 0, flags);
#endif
	return W_TRUE;
}
w_color _w_control_get_foreground(w_control *control) {
	if (_W_CONTROL(control)->foreground == 0) {
		return GetSysColor(COLOR_WINDOWTEXT);
	} else
		return _W_CONTROL(control)->foreground;
}
wresult _w_control_set_foreground(w_control *control, w_color color) {
	_W_CONTROL(control)->foreground = color;
	InvalidateRect(_W_WIDGET(control)->handle, 0, TRUE);
	return W_TRUE;
}
wresult _w_control_get_layout_data(w_control *control, void **data) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		*data = *((void**) &((char*) control)[clazz->object_used_size]);
	} else {
		*data = (void*) &((char*) control)[clazz->object_used_size];
	}
	return W_TRUE;
}
wresult _w_control_new_layout_data(w_control *control, void **data,
		size_t size) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		void *layout_data =
				*((void**) &((char*) control)[clazz->object_used_size]);
		if (layout_data != 0) {
			free(layout_data);
		}
	}
	if ((clazz->object_used_size + size) < clazz->object_total_size) {
		_W_WIDGET(control)->state |= STATE_LAYOUT_DATA_LOCALE;
		*data = (void*) &((char*) control)[clazz->object_used_size];
	} else {
		_W_WIDGET(control)->state &= ~STATE_LAYOUT_DATA_LOCALE;
		*data = malloc(size);
		if (*data == 0)
			return W_ERROR_NO_MEMORY;
		*((void**) &((char*) control)[clazz->object_used_size]) = *data;
	}
	return W_TRUE;
}
void _w_control_request_layout(w_control *control) {
	w_shell *shell;
	w_control_get_shell(control, &shell);

	//getShell ().layout (new Control[] {this}, SWT.DEFER);
}
struct w_menu* _w_control_get_menu(w_control *control) {
	return _W_CONTROL(control)->menu;
}
wresult _w_control_set_menu(w_control *control, struct w_menu *menu) {
	if (w_widget_is_ok(W_WIDGET(menu))) {
		_W_CONTROL(control)->menu = menu;
		return W_TRUE;
	} else {
		return W_ERROR_INVALID_ARGUMENT;
	}
}
wresult _w_control_get_parent(w_control *control, w_composite **parent) {
	*parent = _W_CONTROL(control)->parent;
	return W_TRUE;
}
wresult _w_control_set_parent(w_control *control, struct w_composite *parent) {
}
wresult _w_control_is_reparentable(w_control *control) {
	return W_TRUE;
}
wresult _w_control_get_shell(w_control *control, w_shell **shell) {
	w_composite *parent = _W_CONTROL(control)->parent;
	return W_CONTROL_GET_CLASS(parent)->get_shell(W_CONTROL(parent), shell);
}
void _w_control_get_toolTip_text(w_control *control, w_string_ref **s) {
}
wresult _w_control_set_toolTip_text(w_control *control, const char *text) {
}
wresult _w_control_get_touch_enabled(w_control *control) {
	//return IsTouchWindow (handle, null);
}
wresult _w_control_set_touch_enabled(w_control *control, int enabled) {
}
wresult _w_control_get_visible(w_control *control) {
	//if (!getDrawing()) return (state & HIDDEN) == 0;
	int bits = GetWindowLongW(_W_WIDGET(control)->handle, GWL_STYLE);
	return (bits & WS_VISIBLE) != 0;
}
wresult _w_control_set_visible(w_control *control, int visible) {
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	HWND tophandle = reserved->topHandle(control);
	int v;
	if (visible)
		v = SW_SHOWDEFAULT;
	else
		v = SW_HIDE;

	ShowWindow(tophandle, v);
	return W_TRUE;
	/*	if (!getDrawing()) {
	 if (((state & HIDDEN) == 0) == visible) return;
	 } else {
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if (((bits & OS.WS_VISIBLE) != 0) == visible) return;
	 }
	 if (visible) {
	 sendEvent (SWT.Show);
	 if (isDisposed ()) return;
	 }

	 Control control = null;
	 boolean fixFocus = false;
	 if (!visible) {
	 if (display.focusEvent != SWT.FocusOut) {
	 control = display.getFocusControl ();
	 fixFocus = isFocusAncestor (control);
	 }
	 }
	 if (!getDrawing()) {
	 state = visible ? state & ~HIDDEN : state | HIDDEN;
	 } else {
	 showWidget (visible);
	 if (isDisposed ()) return;
	 }
	 if (!visible) {
	 sendEvent (SWT.Hide);
	 if (isDisposed ()) return;
	 }
	 if (fixFocus) fixFocus (control);*/
}
wresult _w_control_is_visible(w_control *control) {
	return IsWindowVisible(_W_WIDGET(control)->handle);
}
wresult _w_control_redraw(w_control *control, w_rect *rect, int all) {
	RECT *r, _r;
	if (rect != 0) {
		if (rect->width <= 0 || rect->height <= 0)
			return W_TRUE;
		r = &_r;
	} else {
		r = 0;
	}
	if (!IsWindowVisible(_W_WIDGET(control)->handle))
		return W_TRUE;
	if (r != 0) {
		SetRect(r, rect->x, rect->y, rect->x + rect->width,
				rect->y + rect->height);
	}
#if IsWinCE
		InvalidateRect (_W_CONTROL(control)->handle, r, TRUE);
#else
	int flags = RDW_ERASE | RDW_FRAME | RDW_INVALIDATE;
	if (all)
		flags |= RDW_ALLCHILDREN;
	RedrawWindow(_W_WIDGET(control)->handle, r, 0, flags);
#endif
	return W_TRUE;
}
wresult _w_control_set_redraw(w_control *control, int redraw) {
}
wresult _w_control_set_capture(w_control *control, int capture) {
	if (capture) {
		SetCapture(_W_WIDGET(control)->handle);
	} else {
		if (GetCapture() == _W_WIDGET(control)->handle) {
			ReleaseCapture();
		}
	}
	return W_TRUE;
}
wresult _w_control_traverse(w_control *control, int traversal, w_event *event) {
}
wresult _w_control_update(w_control *control) {
}
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id) {
	SetTimer(_W_WIDGET(control)->handle, id + 0x200, ms, NULL);
	return W_TRUE;
}
wresult _w_control_kill_timer(w_control *control, wushort id) {
	KillTimer(_W_WIDGET(control)->handle, id + 0x200);
	return W_TRUE;
}
/*
 * private
 */
wuchar SYSTEM_COLORS[] = {
COLOR_BTNFACE,
COLOR_WINDOW,
COLOR_BTNTEXT,
COLOR_WINDOWTEXT,
COLOR_HIGHLIGHT,
COLOR_SCROLLBAR, };
HBRUSH _w_control_find_brush(w_control *control, ULONG_PTR value, int lbStyle) {
	if (lbStyle == BS_SOLID) {
		for (int i = 0; i < sizeof(SYSTEM_COLORS) / sizeof(SYSTEM_COLORS[0]);
				i++) {
			if (value == GetSysColor(SYSTEM_COLORS[i])) {
				return GetSysColorBrush(SYSTEM_COLORS[i]);
			}
		}
	}
	LOGBRUSH logBrush;
	for (int i = 0; i < BRUSHES_SIZE; i++) {
		HBRUSH hBrush = win_toolkit->brushes[i];
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
	HBRUSH hBrush = win_toolkit->brushes[BRUSHES_SIZE - 1];
	if (hBrush != 0)
		DeleteObject(hBrush);
	memcpy(&win_toolkit->brushes[1], win_toolkit->brushes,
			(BRUSHES_SIZE - 1) * sizeof(HBRUSH));
	switch (lbStyle) {
	case BS_SOLID:
		hBrush = CreateSolidBrush(value);
		break;
	case BS_PATTERN:
		hBrush = CreatePatternBrush((HBITMAP) value);
		break;
	}
	return win_toolkit->brushes[0] = hBrush;
}
void _w_control_fill_background(w_control *control, HDC hDC, int pixel,
		RECT *rect) {
	if (rect->left > rect->right || rect->top > rect->bottom)
		return;
	/*long hPalette = display.hPalette;
	 if (hPalette != 0) {
	 OS.SelectPalette (hDC, hPalette, false);
	 OS.RealizePalette (hDC);
	 }*/
	FillRect(hDC, rect, _w_control_find_brush(control, pixel, BS_SOLID));
}

void _w_control_draw_background_0(w_control *control, HDC hDC, RECT *rect,
		int pixel, int tx, int ty) {
	/*Control control = findBackgroundControl ();
	 if (control != null) {
	 if (control.backgroundImage != null) {
	 fillImageBackground (hDC, control, rect, tx, ty);
	 return;
	 }
	 pixel = control.getBackgroundPixel ();
	 }*/
	/*if (pixel == -1) {
	 if ((_W_WIDGET(control)->state & STATE_THEME_BACKGROUND) != 0) {
	 if (W_COMCTL32_VERSION >= VERSION(6,0) && IsAppThemed ()) {
	 control = findThemeControl ();
	 if (control != null) {
	 fillThemeBackground (hDC, control, rect);
	 return;
	 }
	 }
	 }
	 }*/
	//if (pixel == -1) pixel = getBackgroundPixel ();
	_w_control_fill_background(control, hDC, pixel, rect);
}
void _w_control_draw_background_1(w_control *control, HDC hDC, RECT *rect) {
	_w_control_draw_background_0(control, hDC, rect, -1, 0, 0);
}

void _w_control_draw_background(w_control *control, HDC hDC) {
	RECT rect;
	GetClientRect(_W_WIDGET(control)->handle, &rect);
	_w_control_draw_background_1(control, hDC, &rect);
}
void _w_control_init_graphics(w_control *control, w_graphics *gc, HDC hDC,
		int style) {
	_w_graphics_init(gc, hDC);
	if (WIN32_VERSION >= VERSION(4, 10)) {
		int mask = W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT;
		if ((style & mask) != 0) {
			//data.layout = (style & W_RIGHT_TO_LEFT) != 0 ? LAYOUT_RTL : 0;
		} else {
			int flags = GetLayout(hDC);
			if ((flags & LAYOUT_RTL) != 0) {
				_W_GRAPHICS(gc)->state |= W_GRAPHICS_RIGHT_TO_LEFT
						| W_GRAPHICS_MIRRORED;
			} else {
				_W_GRAPHICS(gc)->state |= W_GRAPHICS_LEFT_TO_RIGHT;
			}
		}
	} else {
		_W_GRAPHICS(gc)->state |= W_GRAPHICS_LEFT_TO_RIGHT;
	}
	/*int foreground = getForegroundPixel ();
	 if (foreground != GetTextColor (hDC)){
	 data.foreground = foreground;
	 }
	 w_control* _control = findBackgroundControl ();
	 if (_control == 0) _control = control;
	 int background = _control.getBackgroundPixel ();
	 if (background != GetBkColor (hDC)){
	 data.background = background;
	 }*/
	w_font *font;
	w_control_get_font(control, &font);
	w_graphics_set_font(gc, font);
	int uiState = (int) SendMessageW(_W_WIDGET(control)->handle,
	WM_QUERYUISTATE, 0, 0);
	if (uiState & UISF_HIDEFOCUS) {
		_W_GRAPHICS(gc)->state |= W_GRAPHICS_UISF_HIDEFOCUS;
	}
	if (uiState & UISF_HIDEACCEL) {
		_W_GRAPHICS(gc)->state |= W_GRAPHICS_UISF_HIDEACCEL;
	}
}
/*
 *
 */
int _CONTROL_WM_DESTROY(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	w_event _e;
	int style = _W_WIDGET(widget)->style;
	WNDPROC last_proc = _W_CONTROL_RESERVED(reserved)->default_proc;
	w_widget_post_event_proc _proc = _W_WIDGET(widget)->post_event;
	_W_WIDGET(widget)->state |= STATE_DISPOSED;
	_e.type = W_EVENT_DISPOSE;
	_e.widget = widget;
	_e.platform_event = (struct w_event_platform*) e;
	_e.data = 0;
	_w_widget_send_event(widget, (w_event*) &_e);
	SetWindowLongPtrW(e->hwnd, GWLP_USERDATA, (LONG_PTR) 0);
	SetWindowLongPtrW(e->hwnd, GWLP_WNDPROC, (LONG_PTR) last_proc);
	reserved->def_proc(widget, e, reserved);
	_W_WIDGET(widget)->clazz = 0;
	if (style & W_FREE_MEMORY) {
		_w_toolkit_registre_free(widget, _proc);
		/*if (_proc != 0) {
		 _e.type = W_EVENT_FREE_MEMORY;
		 _e.platform_event = 0;
		 _e.data = 0;
		 _e.widget = widget;
		 _proc(widget, &_e);
		 } else {
		 free(widget);
		 }*/
	}
	e->result = 0;
	return W_TRUE;
}
int _CONTROL_WM_CREATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	e->result = TRUE;
	return W_TRUE;
}
int _CONTROL_WM_CTLCOLOR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*long hPalette = display.hPalette;
	 if (hPalette != 0) {
	 OS.SelectPalette (wParam, hPalette, false);
	 OS.RealizePalette (wParam);
	 }*/
	w_widget *control = GetWindowLongPtrW((HWND) e->lparam,
	GWLP_USERDATA);
	if (control != 0) {
		struct _w_control_reserved *r = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(control));
		return r->messages[_WM_CTLCOLORCHILD](widget, e, reserved);
	} else
		return W_FALSE;
}
int _CONTROL_WM_CHANGEUISTATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->state & STATE_IGNORE_WM_CHANGEUISTATE) != 0) {
		e->result = 0;
		return W_TRUE;
	}
	return W_FALSE;
}
int _CONTROL_WM_COMMAND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * When the WM_COMMAND message is sent from a
	 * menu, the HWND parameter in LPARAM is zero.
	 */
	if (e->lparam == 0) {
		/*Decorations shell = menuShell ();
		 if (shell.isEnabled ()) {
		 int id = OS.LOWORD (wParam);
		 MenuItem item = display.getMenuItem (id);
		 if (item != null && item.isEnabled ()) {
		 return item.wmCommandChild (wParam, lParam);
		 }
		 }*/
		return W_FALSE;
	} else {
		w_widget *control = (w_widget*) GetWindowLongPtrW((HWND) e->lparam,
		GWLP_USERDATA);
		if (control != 0) {
			struct _w_control_reserved *r = _W_CONTROL_RESERVED(
					_w_widget_get_reserved(control));
			return r->messages[_WM_COMMANDCHILD](control, e, reserved);
		} else
			return W_FALSE;
	}
}
int _CONTROL_WM_DRAWITEM(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	DRAWITEMSTRUCT *st = (DRAWITEMSTRUCT*) e->lparam;
	if (st->CtlType == ODT_MENU) {
		/*MenuItem item = display.getMenuItem (struct.itemID);
		 if (item == null) return null;
		 return item.wmDrawChild (wParam, lParam);*/
	} else {
		w_widget *control = GetWindowLongPtrW(st->hwndItem,
		GWLP_USERDATA);
		if (control != 0) {
			struct _w_control_reserved *r = _W_CONTROL_RESERVED(
					_w_widget_get_reserved(control));
			return r->messages[_WM_DRAWCHILD](control, e, reserved);
		}
	}
	return W_FALSE;
}
int _CONTROL_WM_ERASEBKGND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if ((state & DRAW_BACKGROUND) != 0) {
	 if (findImageControl () != null) return LRESULT.ONE;
	 }
	 if ((state & THEME_BACKGROUND) != 0) {
	 if (OS.IsAppThemed ()) {
	 if (findThemeControl () != null) return LRESULT.ONE;
	 }
	 }*/
	return W_FALSE;
}
int _CONTROL_WM_GETOBJECT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (accessible != null) {
	 long result = accessible.internal_WM_GETOBJECT (wParam, lParam);
	 if (result != 0) return new LRESULT (result);
	 }*/
	return W_FALSE;
}
int _CONTROL_WM_GETDLGCODE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
int _CONTROL_WM_HELP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*HELPINFO lphi = new HELPINFO ();
	 OS.MoveMemory (lphi, lParam, HELPINFO.sizeof);
	 Decorations shell = menuShell ();
	 if (!shell.isEnabled ()) return null;
	 if (lphi.iContextType == OS.HELPINFO_MENUITEM) {
	 MenuItem item = display.getMenuItem (lphi.iCtrlId);
	 if (item != null && item.isEnabled ()) {
	 Widget widget = null;
	 if (item.hooks (SWT.Help)) {
	 widget = item;
	 } else {
	 Menu menu = item.parent;
	 if (menu.hooks (SWT.Help)) widget = menu;
	 }
	 if (widget != null) {
	 long hwndShell = shell.handle;
	 OS.SendMessage (hwndShell, OS.WM_CANCELMODE, 0, 0);
	 widget.postEvent (SWT.Help);
	 return LRESULT.ONE;
	 }
	 }
	 return null;
	 }
	 if (hooks (SWT.Help)) {
	 postEvent (SWT.Help);
	 return LRESULT.ONE;
	 }*/
	return W_FALSE;
}
int _CONTROL_WM_HSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	w_widget *control = (w_widget*) GetWindowLongPtrW((HWND) e->lparam,
	GWLP_USERDATA);
	if (control != 0) {
		struct _w_control_reserved *r = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(control));
		return r->messages[_WM_SCROLLCHILD](control, e, reserved);
	} else
		return W_FALSE;
}
int _CONTROL_WM_INPUTLANGCHANGE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	//menuShell().destroyAccelerators();
	return W_FALSE;
}
int _CONTROL_WM_MEASUREITEM(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*MEASUREITEMSTRUCT struct = new MEASUREITEMSTRUCT ();
	 OS.MoveMemory (struct, lParam, MEASUREITEMSTRUCT.sizeof);
	 if (struct.CtlType == OS.ODT_MENU) {
	 MenuItem item = display.getMenuItem (struct.itemID);
	 if (item == null) return null;
	 return item.wmMeasureChild (wParam, lParam);
	 }
	 long hwnd = OS.GetDlgItem (handle, struct.CtlID);
	 Control control = display.getControl (hwnd);
	 if (control == null) return null;
	 return control.wmMeasureChild (wParam, lParam);*/
	return W_FALSE;
}

int _CONTROL_WM_MENUCHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Feature in Windows.  When the user types Alt+<key>
	 * and <key> does not match a mnemonic in the System
	 * menu or the menu bar, Windows beeps.  This beep is
	 * unexpected and unwanted by applications that look
	 * for Alt+<key>.  The fix is to detect the case and
	 * stop Windows from beeping by closing the menu.
	 */
	/*int type = OS.HIWORD (wParam);
	 if (type == 0 || type == OS.MF_SYSMENU) {
	 display.mnemonicKeyHit = false;
	 return new LRESULT (OS.MAKELRESULT (0, OS.MNC_CLOSE));
	 }*/
	return W_FALSE;
}

int _CONTROL_WM_MENUSELECT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*int code = OS.HIWORD (wParam);
	 Shell shell = getShell ();
	 OS.KillTimer (this.handle, Menu.ID_TOOLTIP_TIMER);
	 if (activeMenu != null)
	 activeMenu.hideCurrentToolTip ();
	 if (code == 0xFFFF && lParam == 0) {
	 Menu menu = shell.activeMenu;
	 while (menu != null) {*/
	/*
	 * When the user cancels any menu that is not the
	 * menu bar, assume a mnemonic key was pressed to open
	 * the menu from WM_SYSCHAR.  When the menu was invoked
	 * using the mouse, this assumption is wrong but not
	 * harmful.  This variable is only used in WM_SYSCHAR
	 * and WM_SYSCHAR is only sent after the user has pressed
	 * a mnemonic.
	 */
	//display.mnemonicKeyHit = true;
	/*
	 * It is possible (but unlikely), that application
	 * code could have disposed the widget in the hide
	 * event.  If this happens, stop searching up the
	 * parent list because there is no longer a link
	 * to follow.
	 */
	/*menu.sendEvent (SWT.Hide);
	 if (menu.isDisposed ()) break;
	 menu = menu.getParentMenu ();
	 }*/
	/*
	 * The shell may be disposed because of sending the hide
	 * event to the last active menu menu but setting a field
	 * to null in a destroyed widget is not harmful.
	 */
	/*shell.activeMenu = null;
	 return null;
	 }
	 if ((code & OS.MF_SYSMENU) != 0) return null;
	 if ((code & OS.MF_HILITE) != 0) {
	 MenuItem item = null;
	 Decorations menuShell = menuShell ();
	 if ((code & OS.MF_POPUP) != 0) {
	 int index = OS.LOWORD (wParam);
	 MENUITEMINFO info = new MENUITEMINFO ();
	 info.cbSize = MENUITEMINFO.sizeof;
	 info.fMask = OS.MIIM_SUBMENU;
	 if (OS.GetMenuItemInfo (lParam, index, true, info)) {
	 Menu newMenu = menuShell.findMenu (info.hSubMenu);
	 if (newMenu != null) {
	 item = newMenu.cascade;
	 activeMenu = newMenu;
	 activeMenu.selectedMenuItem = newMenu.cascade;
	 OS.SetTimer (this.handle, Menu.ID_TOOLTIP_TIMER, OS.TTM_GETDELAYTIME, 0);
	 }
	 }
	 } else {
	 Menu newMenu = menuShell.findMenu (lParam);
	 if (newMenu != null) {
	 int id = OS.LOWORD (wParam);
	 item = display.getMenuItem (id);
	 }
	 activeMenu = (newMenu == null) ? menu : newMenu;
	 if (item != null && activeMenu != null) {
	 activeMenu.selectedMenuItem = item;
	 OS.SetTimer (this.handle, Menu.ID_TOOLTIP_TIMER, OS.TTM_GETDELAYTIME, 0);
	 }
	 }
	 if (item != null) item.sendEvent (SWT.Arm);
	 }*/
	return W_FALSE;
}
int _CONTROL_WM_MOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*state |= MOVE_OCCURRED;
	 if (findImageControl () != null) {
	 if (this != getShell ()) redrawChildren ();
	 } else {
	 if ((state & THEME_BACKGROUND) != 0) {
	 if (OS.IsAppThemed ()) {
	 if (OS.IsWindowVisible (handle)) {
	 if (findThemeControl () != null) redrawChildren ();
	 }
	 }
	 }
	 }
	 if ((state & MOVE_DEFERRED) == 0) sendEvent (SWT.Move);*/
	// widget could be disposed at this point
	return W_FALSE;
}
int _CONTROL_WM_NCHITTEST(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (!OS.IsWindowEnabled (handle)) return null;
	 if (!isActive ()) return new LRESULT (OS.HTTRANSPARENT);*/
	return W_FALSE;
}

int _CONTROL_WM_NOTIFY(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	w_widget *control = GetWindowLongPtrW(((NMHDR*) e->lparam)->hwndFrom,
	GWLP_USERDATA);
	if (control != 0) {
		struct _w_control_reserved *r = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(control));
		return r->messages[_WM_NOTIFYCHILD](control, e, reserved);
	} else
		return W_FALSE;
}

int _CONTROL_WM_PAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->state & STATE_DISPOSE_SENT) != 0) {
		e->result = 0;
		return W_TRUE;
	}
	return _WIDGET_WM_PAINT(widget, e, reserved);
}
int _CONTROL_WM_SETCURSOR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*int hitTest = (short) OS.LOWORD (lParam);
	 if (hitTest == OS.HTCLIENT) {
	 Control control = display.getControl (wParam);
	 if (control == null) return null;
	 Cursor cursor = control.findCursor ();
	 if (cursor != null) {
	 OS.SetCursor (cursor.handle);
	 return LRESULT.ONE;
	 }
	 }*/
	return W_FALSE;
}
int _CONTROL_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	_W_WIDGET(widget)->state |= STATE_RESIZE_OCCURRED;
	//if (_W_WIDGET(widget)->state & STATE_RESIZE_DEFERRED) {
	w_event event;
	memset(&event, 0, sizeof(event));
	event.platform_event = (w_event_platform*) e;
	event.type = W_EVENT_RESIZE;
	event.widget = widget;
	_w_widget_send_event(widget, &event);
	//}
	return W_FALSE;
}

int _CONTROL_WM_SYSCOMMAND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Check to see if the command is a system command or
	 * a user menu item that was added to the System menu.
	 * When a user item is added to the System menu,
	 * WM_SYSCOMMAND must always return zero.
	 *
	 * NOTE: This is undocumented.
	 */
	/*if ((wParam & 0xF000) == 0) {
	 Decorations shell = menuShell ();
	 if (shell.isEnabled ()) {
	 MenuItem item = display.getMenuItem (OS.LOWORD (wParam));
	 if (item != null) item.wmCommandChild (wParam, lParam);
	 }
	 return LRESULT.ZERO;
	 }*/

	/* Process the System Command */
	/*int cmd = (int)wParam & 0xFFF0;
	 switch (cmd) {
	 case OS.SC_KEYMENU:*/
	/*
	 * When lParam is zero, one of F10, Shift+F10, Ctrl+F10 or
	 * Ctrl+Shift+F10 was pressed.  If there is no menu bar and
	 * the focus control is interested in keystrokes, give the
	 * key to the focus control.  Normally, F10 with no menu bar
	 * moves focus to the System menu but this can be achieved
	 * using Alt+Space.  To allow the application to see F10,
	 * avoid running the default window proc.
	 *
	 * NOTE:  When F10 is pressed, WM_SYSCOMMAND is sent to the
	 * shell, not the focus control.  This is undocumented Windows
	 * behavior.
	 */
	/*if (lParam == 0) {
	 Decorations shell = menuShell ();
	 Menu menu = shell.getMenuBar ();
	 if (menu == null) {
	 Control control = display._getFocusControl ();
	 if (control != null) {
	 if (control.hooks (SWT.KeyDown) || control.hooks (SWT.KeyUp)) {
	 display.mnemonicKeyHit = false;
	 return LRESULT.ZERO;
	 }
	 }
	 }
	 } else {*/
	/*
	 * When lParam is not zero, Alt+<key> was pressed.  If the
	 * application is interested in keystrokes and there is a
	 * menu bar, check to see whether the key that was pressed
	 * matches a mnemonic on the menu bar.  Normally, Windows
	 * matches the first character of a menu item as well as
	 * matching the mnemonic character.  To allow the application
	 * to see the keystrokes in this case, avoid running the default
	 * window proc.
	 *
	 * NOTE: When the user types Alt+Space, the System menu is
	 * activated.  In this case the application should not see
	 * the keystroke.
	 */
	/*if (hooks (SWT.KeyDown) || hooks (SWT.KeyUp)) {
	 if (lParam != ' ') {
	 Decorations shell = menuShell ();
	 Menu menu = shell.getMenuBar ();
	 if (menu != null) {
	 char key = (char) lParam;
	 if (key != 0) {
	 key = Character.toUpperCase (key);
	 MenuItem [] items = menu.getItems ();
	 for (int i=0; i<items.length; i++) {
	 MenuItem item = items [i];
	 String text = item.getText ();
	 char mnemonic = findMnemonic (text);
	 if (text.length () > 0 && mnemonic == 0) {
	 char ch = text.charAt (0);
	 if (Character.toUpperCase (ch) == key) {
	 display.mnemonicKeyHit = false;
	 return LRESULT.ZERO;
	 }
	 }
	 }
	 }
	 } else {
	 display.mnemonicKeyHit = false;
	 }
	 }
	 }
	 }
	 // FALL THROUGH
	 case OS.SC_HSCROLL:
	 case OS.SC_VSCROLL:*/
	/*
	 * Do not allow keyboard traversal of the menu bar
	 * or scrolling when the shell is not enabled.
	 */
	/*Decorations shell = menuShell ();
	 if (!shell.isEnabled () || !shell.isActive ()) {
	 return LRESULT.ZERO;
	 }
	 break;
	 case OS.SC_MINIMIZE:*/
	/* Save the focus widget when the shell is minimized */
	/*menuShell ().saveFocus ();
	 break;
	 }*/
	return W_FALSE;
}
int _CONTROL_WM_TABLET_FLICK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (!hooks (SWT.Gesture) && !filters (SWT.Gesture)) return null;
	 Event event = new Event ();
	 FLICK_DATA fData = new FLICK_DATA ();
	 long [] source = new long [1];
	 source[0] = wParam;
	 OS.MoveMemory (fData, source, OS.FLICK_DATA_sizeof ());
	 FLICK_POINT fPoint = new FLICK_POINT ();
	 source [0] = lParam;
	 OS.MoveMemory (fPoint, source, OS.FLICK_POINT_sizeof ());

	 switch (fData.iFlickDirection) {
	 case OS.FLICKDIRECTION_RIGHT:
	 event.xDirection = 1;
	 event.yDirection = 0;
	 break;
	 case OS.FLICKDIRECTION_UPRIGHT:
	 event.xDirection = 1;
	 event.yDirection = -1;
	 break;
	 case OS.FLICKDIRECTION_UP:
	 event.xDirection = 0;
	 event.yDirection = -1;
	 break;
	 case OS.FLICKDIRECTION_UPLEFT:
	 event.xDirection = -1;
	 event.yDirection = -1;
	 break;
	 case OS.FLICKDIRECTION_LEFT:
	 event.xDirection = -1;
	 event.yDirection = 0;
	 break;
	 case OS.FLICKDIRECTION_DOWNLEFT:
	 event.xDirection = -1;
	 event.yDirection = 1;
	 break;
	 case OS.FLICKDIRECTION_DOWN:
	 event.xDirection = 0;
	 event.yDirection = 1;
	 break;
	 case OS.FLICKDIRECTION_DOWNRIGHT:
	 event.xDirection = 1;
	 event.yDirection = 1;
	 break;
	 }
	 event.setLocationInPixels(fPoint.x, fPoint.y);
	 event.type = SWT.Gesture;
	 event.detail = SWT.GESTURE_SWIPE;
	 setInputState (event, SWT.Gesture);
	 sendEvent (SWT.Gesture, event);
	 return event.doit ? null : LRESULT.ONE;*/
	return W_FALSE;
}

int _CONTROL_WM_TOUCH(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = null;
	 if (hooks (SWT.Touch) || filters (SWT.Touch)) {
	 int cInputs = OS.LOWORD (wParam);
	 long hHeap = OS.GetProcessHeap ();
	 long pInputs = OS.HeapAlloc (hHeap, OS.HEAP_ZERO_MEMORY,  cInputs * TOUCHINPUT.sizeof);
	 if (pInputs != 0) {
	 if (OS.GetTouchInputInfo (lParam, cInputs, pInputs, TOUCHINPUT.sizeof)) {
	 TOUCHINPUT ti [] = new TOUCHINPUT [cInputs];
	 for (int i = 0; i < cInputs; i++){
	 ti [i] = new TOUCHINPUT ();
	 OS.MoveMemory (ti [i], pInputs + i * TOUCHINPUT.sizeof, TOUCHINPUT.sizeof);
	 }
	 sendTouchEvent (ti);
	 OS.CloseTouchInputHandle (lParam);
	 result = LRESULT.ZERO;
	 }
	 OS.HeapFree (hHeap, 0, pInputs);
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _CONTROL_WM_TIMER(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (wParam == Menu.ID_TOOLTIP_TIMER && activeMenu != null) {
	 OS.KillTimer (this.handle, Menu.ID_TOOLTIP_TIMER);
	 activeMenu.wmTimer (wParam, lParam);
	 }*/
	if (e->wparam >= 0x200) {
		w_event_time event;
		memset(&event, 0, sizeof(event));
		event.event.platform_event = (w_event_platform*) e;
		event.event.type = W_EVENT_TIMER;
		event.event.widget = widget;
		event.id = e->wparam - 0x200;
		_w_widget_send_event(widget, (w_event*) &event);
	}
	return W_FALSE;
}
int _CONTROL_WM_VSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	w_widget *control = (w_widget*) GetWindowLongPtrW((HWND) e->lparam,
	GWLP_USERDATA);
	if (control != 0) {
		struct _w_control_reserved *r = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(control));
		return r->messages[_WM_SCROLLCHILD](control, e, reserved);
	} else
		return W_FALSE;
}

int _CONTROL_WM_WINDOWPOSCHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*try {
	 display.resizeCount++;
	 long code = callWindowProc (handle, OS.WM_WINDOWPOSCHANGED, wParam, lParam);
	 return code == 0 ? LRESULT.ZERO : new LRESULT (code);
	 } finally {
	 --display.resizeCount;
	 }*/
	return W_FALSE;
}

int _CONTROL_WM_WINDOWPOSCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Bug in Windows.  When WM_SETREDRAW is used to turn off drawing
	 * for a control and the control is moved or resized, Windows does
	 * not redraw the area where the control once was in the parent.
	 * The fix is to detect this case and redraw the area.
	 */
	/*if (!getDrawing()) {
	 Shell shell = getShell ();
	 if (shell != this) {
	 WINDOWPOS lpwp = new WINDOWPOS ();
	 OS.MoveMemory (lpwp, lParam, WINDOWPOS.sizeof);
	 if ((lpwp.flags & OS.SWP_NOMOVE) == 0 || (lpwp.flags & OS.SWP_NOSIZE) == 0) {
	 RECT rect = new RECT ();
	 OS.GetWindowRect (topHandle (), rect);
	 int width = rect.right - rect.left;
	 int height = rect.bottom - rect.top;
	 if (width != 0 && height != 0) {
	 long hwndParent = parent == null ? 0 : parent.handle;
	 OS.MapWindowPoints (0, hwndParent, rect, 2);
	 long rgn1 = OS.CreateRectRgn (rect.left, rect.top, rect.right, rect.bottom);
	 long rgn2 = OS.CreateRectRgn (lpwp.x, lpwp.y, lpwp.x + lpwp.cx, lpwp.y + lpwp.cy);
	 OS.CombineRgn (rgn1, rgn1, rgn2, OS.RGN_DIFF);
	 int flags = OS.RDW_ERASE | OS.RDW_FRAME | OS.RDW_INVALIDATE | OS.RDW_ALLCHILDREN;
	 OS.RedrawWindow (hwndParent, null, rgn1, flags);
	 OS.DeleteObject (rgn1);
	 OS.DeleteObject (rgn2);
	 }
	 }
	 }
	 }*/
	return W_FALSE;
}

int _CONTROL_WM_CTLCOLORCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*Control control = findBackgroundControl ();
	 if (control == null) {
	 if ((state & THEME_BACKGROUND) != 0) {
	 if (OS.IsAppThemed ()) {
	 control = findThemeControl ();
	 if (control != null) {
	 RECT rect = new RECT ();
	 OS.GetClientRect (handle, rect);
	 OS.SetTextColor (wParam, getForegroundPixel ());
	 OS.SetBkColor (wParam, getBackgroundPixel ());
	 fillThemeBackground (wParam, control, rect);
	 OS.SetBkMode (wParam, OS.TRANSPARENT);
	 return new LRESULT (OS.GetStockObject (OS.NULL_BRUSH));
	 }
	 }
	 }
	 if (foreground == -1) return null;
	 }
	 if (control == null) control = this;
	 int forePixel = getForegroundPixel ();
	 int backPixel = control.getBackgroundPixel ();
	 OS.SetTextColor (wParam, forePixel);
	 OS.SetBkColor (wParam, backPixel);
	 if (control.backgroundImage != null) {
	 RECT rect = new RECT ();
	 OS.GetClientRect (handle, rect);
	 long hwnd = control.handle;
	 long hBitmap = control.backgroundImage.handle;
	 OS.MapWindowPoints (handle, hwnd, rect, 2);
	 POINT lpPoint = new POINT ();
	 OS.GetWindowOrgEx (wParam, lpPoint);
	 OS.SetBrushOrgEx (wParam, -rect.left - lpPoint.x, -rect.top - lpPoint.y, lpPoint);
	 long hBrush = findBrush (hBitmap, OS.BS_PATTERN);
	 if ((state & DRAW_BACKGROUND) != 0) {
	 long hOldBrush = OS.SelectObject (wParam, hBrush);
	 OS.MapWindowPoints (hwnd, handle, rect, 2);
	 OS.PatBlt (wParam, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, OS.PATCOPY);
	 OS.SelectObject (wParam, hOldBrush);
	 }
	 OS.SetBkMode (wParam, OS.TRANSPARENT);
	 return new LRESULT (hBrush);
	 }
	 long hBrush = findBrush (backPixel, OS.BS_SOLID);
	 if ((state & DRAW_BACKGROUND) != 0) {
	 RECT rect = new RECT ();
	 OS.GetClientRect (handle, rect);
	 long hOldBrush = OS.SelectObject (wParam, hBrush);
	 OS.PatBlt (wParam, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, OS.PATCOPY);
	 OS.SelectObject (wParam, hOldBrush);
	 }
	 return new LRESULT (hBrush);*/
	return W_FALSE;
}

/*
 * post event proc
 */
int _CONTROL_WM_NULL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
int _w_control_dispath_message(w_widget *widget, struct _w_event_platform *ee,
		struct _w_widget_reserved *reserved) {
	wresult ret = W_FALSE;
	if (ee->msg
			< (sizeof(win_toolkit->wm_msg) / sizeof(win_toolkit->wm_msg[0]))) {
		unsigned char msgid = win_toolkit->wm_msg[ee->msg];
		if (msgid != 0) {
			/*printf("%.*s %8x %8x %8x\n", 20, ___message[msgid],
			 (unsigned int) ee->hwnd, (unsigned int) ee->wparam,
			 (unsigned int) ee->lparam);*/
			ret = _W_CONTROL_RESERVED(reserved)->messages[msgid](widget, ee,
					_W_WIDGET_RESERVED(reserved));
		}
	}
	if (ret == W_FALSE) {
		reserved->def_proc(widget, ee, reserved);
	}
	return ret;
}
int _w_control_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	e->size->width = e->wHint, e->size->height = e->hHint;
	if (e->wHint == W_DEFAULT)
		e->size->width = 40;
	if (e->hHint == W_DEFAULT)
		e->size->height = 40;
	return W_TRUE;
}
int _w_control_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _w_control_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _w_control_post_event(w_widget *widget, struct w_event *ee) {
	struct _w_widget_reserved *reserved;
	switch (ee->type) {
	case W_EVENT_PLATFORM:
		reserved = _w_widget_get_reserved(widget);
		return _w_control_dispath_message(widget,
				(struct _w_event_platform*) ee, reserved);
		break;
	case W_EVENT_COMPUTE_SIZE:
		reserved = _w_widget_get_reserved(widget);
		return _W_CONTROL_RESERVED(reserved)->compute_size(widget,
				(struct w_event_compute_size*) ee, reserved);
		break;
	case W_EVENT_CLIENT_AREA:
		reserved = _w_widget_get_reserved(widget);
		return _W_CONTROL_RESERVED(reserved)->get_client_area(widget,
				(struct w_event_client_area*) ee, reserved);
		break;
	case W_EVENT_COMPUTE_TRIM:
		reserved = _w_widget_get_reserved(widget);
		return _W_CONTROL_RESERVED(reserved)->compute_trim(widget,
				(struct w_event_compute_trim*) ee, reserved);
		break;
	case W_EVENT_LAYOUTDETECT: {
		w_layout *layout;
		w_composite_get_layout(W_COMPOSITE(widget), &layout);
		w_layout_do_layout(layout, W_COMPOSITE(widget), W_TRUE);
		return W_TRUE;
	}
		break;
	}
	return W_FALSE;
}
void _w_control_dispose(w_widget *widget) {
	//if ((_W_WIDGET(widget)->state & STATE_DISPOSED) == 0) {
	if (widget->clazz != 0) {
		struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
				_w_widget_get_reserved( W_WIDGET(widget)));
		HWND tophandle = reserved->topHandle(W_CONTROL(widget));
		DestroyWindow(tophandle);
	}
}
void _w_control_class_init(struct _w_control_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
//clazz
	W_WIDGET_CLASS(clazz)->post_event = _w_control_post_event;
	W_WIDGET_CLASS(clazz)->dispose = _w_control_dispose;
	clazz->get_graphics = _w_control_get_graphics;
	clazz->get_orientation = _w_control_get_orientation;
	clazz->set_orientation = _w_control_set_orientation;
	clazz->get_text_direction = _w_control_get_text_direction;
	clazz->set_text_direction = _w_control_set_text_direction;
	clazz->print = _w_control_print;
	clazz->get_accessible = _w_control_get_accessible;
	clazz->get_bounds = _w_control_get_bounds;
	clazz->set_bounds = _w_control_set_bounds;
	clazz->get_region = _w_control_get_region;
	clazz->set_region = _w_control_set_region;
	clazz->move_above = _w_control_move_above;
	clazz->move_below = _w_control_move_below;
	clazz->pack = _w_control_pack;
	clazz->to_control = _w_control_to_control;
	clazz->to_display = _w_control_to_display;
	clazz->drag_detect = _w_control_drag_detect;
	clazz->get_drag_detect = _w_control_get_drag_detect;
	clazz->set_drag_detect = _w_control_set_drag_detect;
	clazz->set_focus = _w_control_set_focus;
	clazz->force_focus = _w_control_force_focus;
	clazz->is_focus_control = _w_control_is_focus_control;
	clazz->get_border_width = _w_control_get_border_width;
	clazz->get_cursor = _w_control_get_cursor;
	clazz->set_cursor = _w_control_set_cursor;
	clazz->get_enabled = _w_control_get_enabled;
	clazz->set_enabled = _w_control_set_enabled;
	clazz->is_enabled = _w_control_is_enabled;
	clazz->get_font = _w_control_get_font;
	clazz->set_font = _w_control_set_font;
	clazz->get_background = _w_control_get_background;
	clazz->set_background = _w_control_set_background;
	clazz->get_foreground = _w_control_get_foreground;
	clazz->set_foreground = _w_control_set_foreground;
	clazz->get_layout_data = _w_control_get_layout_data;
	clazz->new_layout_data = _w_control_new_layout_data;
	clazz->request_layout = _w_control_request_layout;
	clazz->get_menu = _w_control_get_menu;
	clazz->set_menu = _w_control_set_menu;
	clazz->get_parent = _w_control_get_parent;
	clazz->set_parent = _w_control_set_parent;
	clazz->is_reparentable = _w_control_is_reparentable;
	clazz->get_shell = _w_control_get_shell;
	clazz->get_toolTip_text = _w_control_get_toolTip_text;
	clazz->set_toolTip_text = _w_control_set_toolTip_text;
	clazz->get_touch_enabled = _w_control_get_touch_enabled;
	clazz->set_touch_enabled = _w_control_set_touch_enabled;
	clazz->get_visible = _w_control_get_visible;
	clazz->set_visible = _w_control_set_visible;
	clazz->is_visible = _w_control_is_visible;
	clazz->redraw = _w_control_redraw;
	clazz->set_redraw = _w_control_set_redraw;
	clazz->set_capture = _w_control_set_capture;
	clazz->traverse = _w_control_traverse;
	clazz->update = _w_control_update;
	clazz->set_timer = _w_control_set_timer;
	clazz->kill_timer = _w_control_kill_timer;

	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->topHandle = _w_control_h;
	reserved->borderHandle = _w_control_h;
	reserved->widget.def_proc = _w_control_def_proc;
	//messages
	struct _w_control_reserved *msg = reserved;
	for (size_t i = 0; i < (sizeof(msg->messages) / sizeof(msg->messages[0]));
			i++) {
		msg->messages[i] = _CONTROL_WM_NULL;
	}
	msg->compute_size = _w_control_compute_size;
	msg->get_client_area = _w_control_get_client_area;
	msg->compute_trim = _w_control_compute_trim;
	msg->messages[_WM_DESTROY] = _CONTROL_WM_DESTROY;
	msg->messages[_WM_SIZE] = _CONTROL_WM_SIZE;
	msg->messages[_WM_TIMER] = _CONTROL_WM_TIMER;
	msg->messages[_WM_CREATE] = _CONTROL_WM_CREATE;
	msg->messages[_WM_CTLCOLOR] = _CONTROL_WM_CTLCOLOR;
	msg->messages[_WM_CAPTURECHANGED] = _WIDGET_WM_CAPTURECHANGED;
	msg->messages[_WM_CHANGEUISTATE] = _CONTROL_WM_CHANGEUISTATE;
	msg->messages[_WM_CHAR] = _WIDGET_WM_CHAR;
	msg->messages[_WM_COMMAND] = _CONTROL_WM_COMMAND;
	msg->messages[_WM_CONTEXTMENU] = _WIDGET_WM_CONTEXTMENU;
	msg->messages[_WM_DRAWITEM] = _CONTROL_WM_DRAWITEM;
	msg->messages[_WM_ERASEBKGND] = _CONTROL_WM_ERASEBKGND;
	msg->messages[_WM_GETOBJECT] = _CONTROL_WM_GETOBJECT;
	msg->messages[_WM_HELP] = _CONTROL_WM_HELP;
	msg->messages[_WM_HSCROLL] = _CONTROL_WM_HSCROLL;
	msg->messages[_WM_IME_CHAR] = _WIDGET_WM_IME_CHAR;
	msg->messages[_WM_INPUTLANGCHANGE] = _CONTROL_WM_INPUTLANGCHANGE;
	msg->messages[_WM_KEYDOWN] = _WIDGET_WM_KEYDOWN;
	msg->messages[_WM_KEYUP] = _WIDGET_WM_KEYUP;
	msg->messages[_WM_KILLFOCUS] = _WIDGET_WM_KILLFOCUS;
	msg->messages[_WM_MEASUREITEM] = _CONTROL_WM_MEASUREITEM;
	msg->messages[_WM_MENUCHAR] = _CONTROL_WM_MENUCHAR;
	msg->messages[_WM_MENUSELECT] = _CONTROL_WM_MENUSELECT;
	msg->messages[_WM_MOVE] = _CONTROL_WM_MOVE;
	msg->messages[_WM_NCHITTEST] = _CONTROL_WM_NCHITTEST;
	msg->messages[_WM_NOTIFY] = _CONTROL_WM_NOTIFY;
	msg->messages[_WM_PAINT] = _CONTROL_WM_PAINT;
	msg->messages[_WM_SETCURSOR] = _CONTROL_WM_SETCURSOR;
	msg->messages[_WM_SETFOCUS] = _WIDGET_WM_SETFOCUS;
	msg->messages[_WM_SYSCHAR] = _WIDGET_WM_SYSCHAR;
	msg->messages[_WM_SYSCOMMAND] = _CONTROL_WM_SYSCOMMAND;
	msg->messages[_WM_SYSKEYDOWN] = _WIDGET_WM_SYSKEYDOWN;
	msg->messages[_WM_SYSKEYUP] = _WIDGET_WM_SYSKEYUP;
	msg->messages[_WM_TABLET_FLICK] = _CONTROL_WM_TABLET_FLICK;
	msg->messages[_WM_TOUCH] = _CONTROL_WM_TOUCH;
	msg->messages[_WM_VSCROLL] = _CONTROL_WM_VSCROLL;
	msg->messages[_WM_WINDOWPOSCHANGED] = _CONTROL_WM_WINDOWPOSCHANGED;
	msg->messages[_WM_WINDOWPOSCHANGING] = _CONTROL_WM_WINDOWPOSCHANGING;
	msg->messages[_WM_CTLCOLORCHILD] = _CONTROL_WM_CTLCOLORCHILD;
	msg->messages[_WM_GETDLGCODE] = _CONTROL_WM_GETDLGCODE;

	//mouse message
	msg->messages[_WM_LBUTTONDBLCLK] = _WIDGET_WM_LBUTTONDBLCLK;
	msg->messages[_WM_LBUTTONDOWN] = _WIDGET_WM_LBUTTONDOWN;
	msg->messages[_WM_LBUTTONUP] = _WIDGET_WM_LBUTTONUP;
	msg->messages[_WM_MBUTTONDBLCLK] = _WIDGET_WM_MBUTTONDBLCLK;
	msg->messages[_WM_MBUTTONDOWN] = _WIDGET_WM_MBUTTONDOWN;
	msg->messages[_WM_MBUTTONUP] = _WIDGET_WM_MBUTTONUP;
	msg->messages[_WM_MOUSEHOVER] = _WIDGET_WM_MOUSEHOVER;
	msg->messages[_WM_MOUSELEAVE] = _WIDGET_WM_MOUSELEAVE;
	msg->messages[_WM_MOUSEMOVE] = _WIDGET_WM_MOUSEMOVE;
	msg->messages[_WM_MOUSEWHEEL] = _WIDGET_WM_MOUSEWHEEL;
	msg->messages[_WM_RBUTTONDBLCLK] = _WIDGET_WM_RBUTTONDBLCLK;
	msg->messages[_WM_RBUTTONDOWN] = _WIDGET_WM_RBUTTONDOWN;
	msg->messages[_WM_RBUTTONUP] = _WIDGET_WM_RBUTTONUP;
	msg->messages[_WM_XBUTTONDBLCLK] = _WIDGET_WM_XBUTTONDBLCLK;
	msg->messages[_WM_XBUTTONDOWN] = _WIDGET_WM_XBUTTONDOWN;
	msg->messages[_WM_XBUTTONUP] = _WIDGET_WM_XBUTTONUP;
	msg->messages[_WM_MENUCOMMAND] = _MENU_WM_MENUCOMMAND;
	msg->messages[_WM_INITMENUPOPUP] = _MENU_WM_INITMENUPOPUP;
	msg->messages[_WM_UNINITMENUPOPUP] = _MENU_WM_UNINITMENUPOPUP;
}
#endif

