/*
 * composite.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#if (_WIN32_WINNT < 0x0600)
#define _WIN32_WINNT 0x0600
#endif
#include "composite.h"
#include "toolkit.h"
struct _w_composite_iterator {
	void *v_table; //used in c++ as virtual table
	struct _w_iterator_class *clazz;
	w_composite *composite;
	HWND hwndChild;
	wresult init;
	int count;
};
wresult _w_composite_iterator_next(w_iterator *it, void *obj) {
	if (((struct _w_composite_iterator*) it)->init == W_TRUE) {
		((struct _w_composite_iterator*) it)->hwndChild =
				GetWindow(
						_W_WIDGET(((struct _w_composite_iterator* ) it)->composite)->handle,
						GW_CHILD);
		((struct _w_composite_iterator*) it)->init = W_FALSE;
	} else {
		if (((struct _w_composite_iterator*) it)->hwndChild != 0) {
			((struct _w_composite_iterator*) it)->hwndChild = GetWindow(
					((struct _w_composite_iterator*) it)->hwndChild,
					GW_HWNDNEXT);
		}
	}
	if (((struct _w_composite_iterator*) it)->hwndChild != 0) {
		*((w_control**) obj) = (w_control*) GetWindowLongPtrW(
				((struct _w_composite_iterator*) it)->hwndChild, GWLP_USERDATA);
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wresult _w_composite_iterator_reset(w_iterator *it) {
	((struct _w_composite_iterator*) it)->init = W_TRUE;
	return W_TRUE;
}
wresult _w_composite_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_composite_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_composite_iterator_get_count(w_iterator *it) {
	if (((struct _w_composite_iterator*) it)->count < 0) {
		int count = 0;
		HWND hwnd =
				GetWindow(
						_W_WIDGET(((struct _w_composite_iterator* ) it)->composite)->handle,
						GW_CHILD);
		if (hwnd != 0) {
			count++;
			while ((hwnd = GetWindow(hwnd,
			GW_HWNDNEXT)) != 0) {
				//if (GetWindowLongPtrW(hwnd, GWLP_USERDATA) != 0)
				count++;
			}
		}
		((struct _w_composite_iterator*) it)->count = count;
	}
	return ((struct _w_composite_iterator*) it)->count;
}
_w_iterator_class _w_composite_iterator_class = { //
				_w_composite_iterator_close, //
				_w_composite_iterator_next, //
				_w_composite_iterator_reset, //
				_w_composite_iterator_remove, //
				_w_composite_iterator_get_count //
		};
void _w_composite_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_scrollable_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= WS_CLIPCHILDREN;
}
void _w_composite_minimum_size(w_composite *composite, w_size *result,
		int wHint, int hHint, wresult changed) {
	w_iterator children;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	w_rect clientArea, rect;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &clientArea);
	int width = 0, height = 0;
	w_control *child = 0;
	while (w_iterator_next(&children, (void*) &child)) {
		if (child != 0) {
			w_control_get_bounds(child, &rect.pt,&rect.sz);
			width = w_max(width, rect.x - clientArea.x + rect.width);
			height = w_max(height, rect.y - clientArea.y + rect.height);
		}
	}
	result->width = width;
	result->height = height;
}
/*
 * public function
 */
void _w_composite_get_children(w_composite *composite, w_iterator *it) {
	it->base.clazz = &_w_composite_iterator_class;
	((struct _w_composite_iterator*) it)->composite = composite;
	((struct _w_composite_iterator*) it)->init = W_TRUE;
	((struct _w_composite_iterator*) it)->hwndChild = 0;
	((struct _w_composite_iterator*) it)->count = -1;
}
wresult _w_composite_get_layout(w_composite *composite,w_layout** layout) {
	*layout = _W_COMPOSITE(composite)->layout;
	return W_TRUE;
}
int _w_composite_get_layout_deferred(w_composite *composite) {
}
void _w_composite_get_tab_list(w_composite *composite, w_iterator *it) {
}
int _w_composite_is_layout_deferred(w_composite *composite) {
}
void _w_composite_do_layout(w_composite *composite, int changed, int all) {
	w_event _e;
	_e.type = W_EVENT_LAYOUTDETECT;
	_e.platform_event = 0;
	_e.widget = W_WIDGET(composite);
	_e.data = 0;
	_w_widget_send_event(W_WIDGET(composite), &_e);
}
void _w_composite_set_layout(w_composite *composite, w_layout *layout) {
	_W_COMPOSITE(composite)->layout = layout;
}
void _w_composite_set_layout_deferred(w_composite *composite, int defer) {
}
/*
 * post event proc
 */
wresult _w_composite_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_composite_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), (w_composite*) parent,
			WindowName, dwExStyle, dwStyle);
	if (result > 0) {
		w_font *systemfont = w_toolkit_get_system_font(
				(w_toolkit*) win_toolkit);
		SendMessageW(_W_WIDGET(widget)->handle, WM_SETFONT,
				(WPARAM) _W_FONT(systemfont)->handle, 0);
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
		_W_WIDGET(widget)->state |= STATE_CANVAS;
		ShowWindow(_W_WIDGET(widget)->handle, SW_NORMAL);
	}
	return result;
}
int _COMPOSITE_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int result = _CONTROL_WM_ERASEBKGND(widget, e, reserved);
	if (result != FALSE)
		return result;
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		/* Return zero to indicate that the background was not erased */
		if ((_W_WIDGET(widget)->style & (W_NO_BACKGROUND | W_TRANSPARENT))
				!= 0) {
			e->result = 0;
			return TRUE;
		}
	}
	return result;
}

int _COMPOSITE_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int result = _CONTROL_WM_GETDLGCODE(widget, e, reserved);
	if (result != FALSE)
		return result;
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		int flags = DLGC_WANTALLKEYS | DLGC_WANTARROWS | DLGC_WANTTAB;
		if ((_W_WIDGET(widget)->style & W_NO_FOCUS) != 0)
			flags |= DLGC_STATIC;
		if (GetWindow(_W_WIDGET(widget)->handle, GW_CHILD) != 0)
			flags |= DLGC_STATIC;
		e->result = flags;
		return TRUE;
	}
	return result;
}

int _COMPOSITE_WM_GETFONT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	reserved->def_proc(widget, e, reserved);
	if (e->result != 0) {
		return TRUE;
	}
	w_font *font;
	if (_W_WIDGET(widget)->state & STATE_SET_FONT) {
		font = W_FONT(&_W_CONTROL(widget)->font);
	} else {
		font = w_toolkit_get_system_font(
				w_widget_get_toolkit(W_WIDGET(widget)));
	}
	e->result = (LRESULT) _W_FONT(font)->handle;
	return TRUE;
}

int _COMPOSITE_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int result = _WIDGET_WM_LBUTTONDOWN(widget, e, reserved);
	/*LRESULT result = super.WM_LBUTTONDOWN (wParam, lParam);
	 if (result == LRESULT.ZERO) return result;*/

	/* Set focus for a canvas with no children */
	/*if ((state & CANVAS) != 0) {
	 if ((style & SWT.NO_FOCUS) == 0 && hooksKeys ()) {
	 if (OS.GetWindow (handle, OS.GW_CHILD) == 0) setFocus ();
	 }
	 }*/
	return result;
}

int _COMPOSITE_WM_NCHITTEST(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_NCHITTEST (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Bug in Windows.  For some reason, under circumstances
	 * that are not understood, when one scrolled window is
	 * embedded in another and the outer window scrolls the
	 * inner horizontally by moving the location of the inner
	 * one, the vertical scroll bars of the inner window no
	 * longer function.  Specifically, WM_NCHITTEST returns
	 * HTCLIENT instead of HTVSCROLL.  The fix is to detect
	 * the case where the result of WM_NCHITTEST is HTCLIENT
	 * and the point is not in the client area, and redraw
	 * the trim, which somehow fixes the next WM_NCHITTEST.
	 */
	/*if (OS.IsAppThemed ()) {
	 if ((state & CANVAS)!= 0) {
	 long code = callWindowProc (handle, OS.WM_NCHITTEST, wParam, lParam);
	 if (code == OS.HTCLIENT) {
	 RECT rect = new RECT ();
	 OS.GetClientRect (handle, rect);
	 POINT pt = new POINT ();
	 pt.x = OS.GET_X_LPARAM (lParam);
	 pt.y = OS.GET_Y_LPARAM (lParam);
	 OS.MapWindowPoints (0, handle, pt, 1);
	 if (!OS.PtInRect (rect, pt)) {
	 int flags = OS.RDW_FRAME | OS.RDW_INVALIDATE;
	 OS.RedrawWindow (handle, null, 0, flags);
	 }
	 }
	 return new LRESULT (code);
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _COMPOSITE_WM_PARENTNOTIFY(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}

int _COMPOSITE_WM_PAINT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	w_event_paint event;
	_w_graphics gc;
	PAINTSTRUCT ps;
	w_color forground, background;
	int uiState, ret;
	if ((_W_WIDGET(widget)->state & STATE_DISPOSE_SENT) != 0) {
		e->result = 0;
		return TRUE;
	}
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) == 0
			|| (_W_WIDGET(widget)->state & STATE_FOREIGN_HANDLE) != 0) {
		return _CONTROL_WM_PAINT(widget, e, reserved);
	}
	/* Set the clipping bits */
	int oldBits = GetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE);
	int newBits = oldBits | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	if (newBits != oldBits)
		SetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE, newBits);

	HDC hDC = BeginPaint(_W_WIDGET(widget)->handle, &ps);

	/* Paint the control and the background */

	/* Use the buffered paint when possible */
	wresult bufferedPaint = FALSE;
	if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED) != 0) {
		if ((_W_WIDGET(widget)->style
				& (W_NO_MERGE_PAINTS | W_RIGHT_TO_LEFT | W_TRANSPARENT)) == 0) {
			bufferedPaint = TRUE;
		}
	}
	if (bufferedPaint) {
		int width = ps.rcPaint.right - ps.rcPaint.left;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;
		if (width != 0 && height != 0) {
			HDC phdc;
			int flags = BPBF_COMPATIBLEBITMAP;
			HPAINTBUFFER hBufferedPaint = BeginBufferedPaint(hDC, &ps.rcPaint,
					flags, NULL, &phdc);
			_w_control_init_graphics(W_CONTROL(widget), W_GRAPHICS(&gc), phdc,
					0);
			if ((_W_WIDGET(widget)->style & W_NO_BACKGROUND) != 0) {
				/* This code is intentionally commented because it may be slow to copy bits from the screen */
				//paintGC.copyArea (image, ps.left, ps.top);
			} else {
				//drawBackground(phdc, &ps.rcPaint);
			}
			event.event.type = W_EVENT_PAINT;
			event.event.platform_event = (w_event_platform*) e;
			event.event.widget = widget;
			event.event.data = 0;
			event.bounds.x = ps.rcPaint.left;
			event.bounds.y = ps.rcPaint.top;
			event.bounds.width = width;
			event.bounds.height = height;
			event.gc = W_GRAPHICS(&gc);
			_w_widget_send_event(widget, (w_event*) &event);
			/* if (data.focusDrawn && !isDisposed())
			 updateUIState();*/
			w_graphics_dispose(W_GRAPHICS(&gc));
			EndBufferedPaint(hBufferedPaint, TRUE);
		}
	} else {
		/* Get the system region for the paint HDC */
		HRGN sysRgn = 0;
		if ((_W_WIDGET(widget)->style & (W_DOUBLE_BUFFERED | W_TRANSPARENT))
				!= 0 || (_W_WIDGET(widget)->style & W_NO_MERGE_PAINTS) != 0) {
			sysRgn = CreateRectRgn(0, 0, 0, 0);
			if (GetRandomRgn(hDC, sysRgn, SYSRGN) == 1) {
				if ((GetLayout(hDC) & LAYOUT_RTL) != 0) {
					int nBytes = GetRegionData(sysRgn, 0, NULL);
					LPRGNDATA lpRgnData = (LPRGNDATA) _w_toolkit_malloc(nBytes);
					if (lpRgnData != 0) {
						GetRegionData(sysRgn, nBytes, lpRgnData);
						static XFORM _rgnXForm[] = { -1, 0, 0, 1, 0, 0 };
						HRGN newSysRgn = ExtCreateRegion(_rgnXForm, nBytes,
								lpRgnData);
						DeleteObject(sysRgn);
						_w_toolkit_free(lpRgnData);
						sysRgn = newSysRgn;
					}
				}
				POINT pt;
				MapWindowPoints(0, _W_WIDGET(widget)->handle, &pt, 1);
				OffsetRgn(sysRgn, pt.x, pt.y);
			}
		}

		/* Send the paint event */
		int width = ps.rcPaint.right - ps.rcPaint.left;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;
		if (width != 0 && height != 0) {
			w_surface image;
			w_surface_init(&image);
			w_graphics_init(W_GRAPHICS(&gc));
			if ((_W_WIDGET(widget)->style & (W_DOUBLE_BUFFERED | W_TRANSPARENT))
					!= 0) {
				w_size size = { width, height };
				w_surface_create(&image, &size);
				w_surface_get_graphics(&image, W_GRAPHICS(&gc));
				/*gc = new GC(image, paintGC.getStyle() & W_RIGHT_TO_LEFT);
				 GCData gcData = gc.getGCData();
				 gcData.uiState = data.uiState;
				 gc.setForeground(getForeground());
				 gc.setBackground(getBackground());
				 gc.setFont(getFont());*/
				if ((_W_WIDGET(widget)->style & W_TRANSPARENT) != 0) {
					BitBlt(hDC, 0, 0, width, height,
					_W_GRAPHICS(&gc)->handle, ps.rcPaint.left, ps.rcPaint.top,
					SRCCOPY);
				}
				OffsetRgn(sysRgn, -ps.rcPaint.left, -ps.rcPaint.top);
				SelectClipRgn(hDC, sysRgn);
				OffsetRgn(sysRgn, ps.rcPaint.left, ps.rcPaint.top);
				SetMetaRgn(hDC);
				SetWindowOrgEx(hDC, ps.rcPaint.left, ps.rcPaint.top, NULL);
				SetBrushOrgEx(hDC, ps.rcPaint.left, ps.rcPaint.top, NULL);
				if ((_W_WIDGET(widget)->style
						& (W_NO_BACKGROUND | W_TRANSPARENT)) != 0) {
					/* This code is intentionally commented because it may be slow to copy bits from the screen */
					//paintGC.copyArea (image, ps.left, ps.top);
				} else {
					//drawBackground(gc.handle, &ps.rcPaint);
				}
			} else {
				_w_control_init_graphics(W_CONTROL(widget), W_GRAPHICS(&gc),
						hDC, 0);
			}
			RECT rect;
			if ((_W_WIDGET(widget)->style & W_NO_MERGE_PAINTS)
					!= 0&& GetRgnBox (sysRgn, &rect) == COMPLEXREGION) {
				int nBytes = GetRegionData(sysRgn, 0, NULL);
				LPRGNDATA lpRgnData = (LPRGNDATA) _w_toolkit_malloc(nBytes);
				if (lpRgnData != 0) {
					GetRegionData(sysRgn, nBytes, lpRgnData);
					int count = lpRgnData->rdh.nCount;
					for (int i = 0; i < count; i++) {
						int offset = 8 + (i << 2);
						SetRect(&rect, ((int*) lpRgnData)[offset],
								((int*) lpRgnData)[offset + 1],
								((int*) lpRgnData)[offset + 2],
								((int*) lpRgnData)[offset + 3]);
						if ((_W_WIDGET(widget)->style
								& (W_DOUBLE_BUFFERED | W_NO_BACKGROUND
										| W_TRANSPARENT)) == 0) {
							//drawBackground(gc.handle, &rect);
						}
						//event.count = count - 1 - i;
						event.event.type = W_EVENT_PAINT;
						event.event.platform_event = (w_event_platform*) e;
						event.event.widget = widget;
						event.event.data = 0;
						event.bounds.x = rect.left;
						event.bounds.y = rect.top;
						event.bounds.width = rect.right - rect.left;
						event.bounds.height = rect.bottom - rect.top;
						event.gc = W_GRAPHICS(&gc);
						_w_widget_send_event(widget, (w_event*) &event);
					}
					_w_toolkit_free(lpRgnData);
				}
			} else {
				if ((_W_WIDGET(widget)->style
						& (W_DOUBLE_BUFFERED | W_NO_BACKGROUND | W_TRANSPARENT))
						== 0) {
					//drawBackground(hDC, &ps.rcPaint);
				}
				event.event.type = W_EVENT_PAINT;
				event.event.platform_event = (w_event_platform*) e;
				event.event.widget = widget;
				event.event.data = 0;
				event.bounds.x = ps.rcPaint.left;
				event.bounds.y = ps.rcPaint.top;
				event.bounds.width = width;
				event.bounds.height = height;
				event.gc = W_GRAPHICS(&gc);
				_w_widget_send_event(widget, (w_event*) &event);
			}
			// widget could be disposed at this point
			if ((_W_WIDGET(widget)->style & (W_DOUBLE_BUFFERED | W_TRANSPARENT))
					!= 0) {
				/*if (w_widget_is_ok(widget)) {
				 GCData gcData = gc.getGCData();
				 if (gcData.focusDrawn && !isDisposed())
				 updateUIState();
				 }
				 gc.dispose();
				 if (w_widget_is_ok(widget))
				 paintGC.drawImage(image, DPIUtil.autoScaleDown(ps.left),
				 DPIUtil.autoScaleDown(ps.top));
				 image.dispose();
				 gc = paintGC;*/
			}
			/* Dispose the paint GC */
			w_graphics_dispose(W_GRAPHICS(&gc));
			w_surface_dispose(&image);
		}
		if (sysRgn != 0)
			DeleteObject(sysRgn);
		/*if (data.focusDrawn && !isDisposed())
		 updateUIState();*/
	}
	EndPaint(_W_WIDGET(widget)->handle, &ps);

	/* Restore the clipping bits */
	if (w_widget_is_ok(widget)) {
		if (newBits != oldBits) {
			/*
			 * It is possible (but unlikely), that application
			 * code could have disposed the widget in the paint
			 * event.  If this happens, don't attempt to restore
			 * the style.
			 */
			SetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE, oldBits);
		}
	}
	e->result = 0;
	return TRUE;
}

int _COMPOSITE_WM_PRINTCLIENT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_PRINTCLIENT (wParam, lParam);
	 if (result != null) return result;
	 if ((state & CANVAS) != 0) {
	 forceResize ();
	 int nSavedDC = OS.SaveDC (wParam);
	 RECT rect = new RECT ();
	 OS.GetClientRect (handle, rect);
	 if ((style & (SWT.NO_BACKGROUND | SWT.TRANSPARENT)) == 0) {
	 drawBackground (wParam, rect);
	 }
	 if (hooks (SWT.Paint) || filters (SWT.Paint)) {
	 GCData data = new GCData ();
	 data.device = display;
	 data.foreground = getForegroundPixel ();
	 Control control = findBackgroundControl ();
	 if (control == null) control = this;
	 data.background = control.getBackgroundPixel ();
	 data.font = Font.win32_new(display, OS.SendMessage (handle, OS.WM_GETFONT, 0, 0));
	 data.uiState = (int)OS.SendMessage (handle, OS.WM_QUERYUISTATE, 0, 0);
	 GC gc = GC.win32_new (wParam, data);
	 Event event = new Event ();
	 event.gc = gc;
	 event.setBoundsInPixels(new Rectangle(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));
	 sendEvent (SWT.Paint, event);
	 event.gc = null;
	 gc.dispose ();
	 }
	 OS.RestoreDC (wParam, nSavedDC);
	 }
	 return result;*/
	return W_FALSE;
}

int _COMPOSITE_WM_SETFONT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (e->lparam != 0)
		InvalidateRect(e->hwnd, NULL, TRUE);
	return FALSE;
}

int _COMPOSITE_WM_SIZE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int result = W_FALSE;
	w_event _e;
	if ((_W_WIDGET(widget)->state & STATE_RESIZE_DEFERRED) != 0) {
		result = _SCROLLABLE_WM_SIZE(widget, e, reserved);
	} else {
		/* Begin deferred window positioning */
//setResizeChildren (false);
		/* Resize and Layout */
		result = _SCROLLABLE_WM_SIZE(widget, e, reserved);
		/*
		 * It is possible (but unlikely), that application
		 * code could have disposed the widget in the resize
		 * event.  If this happens, end the processing of the
		 * Windows message by returning the result of the
		 * WM_SIZE message.
		 */
//if (isDisposed ()) return result;
		/*if (layout != null) {
		 markLayout (false, false);
		 updateLayout (false, false);
		 }*/

		/* End deferred window positioning */
//setResizeChildren (true);
		_e.type = W_EVENT_LAYOUTDETECT;
		_e.platform_event = e;
		_e.widget = widget;
		_e.data = 0;
		_w_widget_send_event(widget, &_e);
	}

	/* Damage the widget to cause a repaint */
	if (IsWindowVisible(_W_WIDGET(widget)->handle)) {
		if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
			if ((_W_WIDGET(widget)->style & W_NO_REDRAW_RESIZE) == 0) {
				//if (hooks (SWT.Paint)) {
				InvalidateRect(_W_WIDGET(widget)->handle, NULL, TRUE);
				//}
			}
		}
		/*if (IsAppThemed ()) {
		 if (findThemeControl () != null) redrawChildren ();
		 }*/
	}

	return result;
}

int _COMPOSITE_WM_SYSCOLORCHANGE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SYSCOLORCHANGE (wParam, lParam);
	 if (result != null) return result;
	 long hwndChild = OS.GetWindow (handle, OS.GW_CHILD);
	 while (hwndChild != 0) {
	 OS.SendMessage (hwndChild, OS.WM_SYSCOLORCHANGE, 0, 0);
	 hwndChild = OS.GetWindow (hwndChild, OS.GW_HWNDNEXT);
	 }
	 return result;*/
	return W_FALSE;
}

int _COMPOSITE_WM_SYSCOMMAND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SYSCOMMAND (wParam, lParam);
	 if (result != null) return result;*/

	/*
	 * Check to see if the command is a system command or
	 * a user menu item that was added to the system menu.
	 *
	 * NOTE: This is undocumented.
	 */
//if ((wParam & 0xF000) == 0) return result;
	/*
	 * Bug in Windows.  When a vertical or horizontal scroll bar is
	 * hidden or shown while the opposite scroll bar is being scrolled
	 * by the user (with WM_HSCROLL code SB_LINEDOWN), the scroll bar
	 * does not redraw properly.  The fix is to detect this case and
	 * redraw the non-client area.
	 */
	/*int cmd = (int)wParam & 0xFFF0;
	 switch (cmd) {
	 case OS.SC_HSCROLL:
	 case OS.SC_VSCROLL:
	 boolean showHBar = horizontalBar != null && horizontalBar.getVisible ();
	 boolean showVBar = verticalBar != null && verticalBar.getVisible ();
	 long code = callWindowProc (handle, OS.WM_SYSCOMMAND, wParam, lParam);
	 if ((showHBar != (horizontalBar != null && horizontalBar.getVisible ())) ||
	 (showVBar != (verticalBar != null && verticalBar.getVisible ()))) {
	 int flags = OS.RDW_FRAME | OS.RDW_INVALIDATE | OS.RDW_UPDATENOW;
	 OS.RedrawWindow (handle, null, 0, flags);
	 }
	 if (code == 0) return LRESULT.ZERO;
	 return new LRESULT (code);
	 }*/

	/* Return the result */
//return result;
	return W_FALSE;
}

int _COMPOSITE_WM_UPDATEUISTATE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_UPDATEUISTATE (wParam, lParam);
	 if (result != null) return result;
	 if ((state & CANVAS) != 0 && hooks (SWT.Paint)) {
	 OS.InvalidateRect (handle, null, true);
	 }
	 return result;*/
	return W_FALSE;
}

int _COMPOSITE_WM_NCPAINT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/*LRESULT result = super.wmNCPaint (hwnd, wParam, lParam);
	 if (result != null) return result;
	 long borderHandle = borderHandle ();
	 if ((state & CANVAS) != 0 || (hwnd == borderHandle && handle != borderHandle)) {
	 if (OS.IsAppThemed ()) {
	 int bits1 = OS.GetWindowLong (hwnd, OS.GWL_EXSTYLE);
	 if ((bits1 & OS.WS_EX_CLIENTEDGE) != 0) {
	 long code = 0;
	 int bits2 = OS.GetWindowLong (hwnd, OS.GWL_STYLE);
	 if ((bits2 & (OS.WS_HSCROLL | OS.WS_VSCROLL)) != 0) {
	 code = callWindowProc (hwnd, OS.WM_NCPAINT, wParam, lParam);
	 }
	 long hDC = OS.GetWindowDC (hwnd);
	 RECT rect = new RECT ();
	 OS.GetWindowRect (hwnd, rect);
	 rect.right -= rect.left;
	 rect.bottom -= rect.top;
	 rect.left = rect.top = 0;
	 int border = OS.GetSystemMetrics (OS.SM_CXEDGE);
	 OS.ExcludeClipRect (hDC, border, border, rect.right - border, rect.bottom - border);
	 OS.DrawThemeBackground (display.hEditTheme (), hDC, OS.EP_EDITTEXT, OS.ETS_NORMAL, rect, null);
	 OS.ReleaseDC (hwnd, hDC);
	 return new LRESULT (code);
	 }
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _COMPOSITE_WM_NOTIFY(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
//switch (hdr.code) {
	/*
	 * Feature in Windows.  When the tool tip control is
	 * created, the parent of the tool tip is the shell.
	 * If SetParent () is used to reparent the tool bar
	 * into a new shell, the tool tip is not reparented
	 * and pops up underneath the new shell.  The fix is
	 * to make sure the tool tip is a topmost window.
	 */
	/*case OS.TTN_SHOW:
	 case OS.TTN_POP: {*/
	/*
	 * Bug in Windows 98 and NT.  Setting the tool tip to be the
	 * top most window using HWND_TOPMOST can result in a parent
	 * dialog shell being moved behind its parent if the dialog
	 * has a sibling that is currently on top.  The fix is to
	 * lock the z-order of the active window.
	 *
	 * Feature in Windows.  Using SetWindowPos() with HWND_NOTOPMOST
	 * to clear the topmost state of a window whose parent is already
	 * topmost clears the topmost state of the parent.  The fix is to
	 * check if the parent is already on top and neither set or clear
	 * the topmost status of the tool tip.
	 */
	/*long hwndParent = hdr.hwndFrom;
	 do {
	 hwndParent = OS.GetParent (hwndParent);
	 if (hwndParent == 0) break;
	 int bits = OS.GetWindowLong (hwndParent, OS.GWL_EXSTYLE);
	 if ((bits & OS.WS_EX_TOPMOST) != 0) break;
	 } while (true);
	 if (hwndParent != 0) break;*/
	/*
	 * Bug in Windows.  TTN_SHOW is sent for inactive shells.  When
	 * SetWindowPos is called as a reaction, inactive shells can
	 * wrongly end up on top.  The fix is to swallow such requests.
	 *
	 * A visible effect is that spurious tool tips can show up and
	 * disappear in a split second.  This is a mostly harmless
	 * feature that can also be observed in the Windows Explorer.
	 * See bug 491627 for more details.
	 */
	/*if (display.getActiveShell () == null) return LRESULT.ONE;

	 display.lockActiveWindow = true;
	 int flags = OS.SWP_NOACTIVATE | OS.SWP_NOMOVE | OS.SWP_NOSIZE;
	 long hwndInsertAfter = hdr.code == OS.TTN_SHOW ? OS.HWND_TOPMOST : OS.HWND_NOTOPMOST;
	 OS.SetWindowPos (hdr.hwndFrom, hwndInsertAfter, 0, 0, 0, 0, flags);
	 display.lockActiveWindow = false;
	 break;
	 }
	 case OS.TTN_GETDISPINFOA:
	 case OS.TTN_GETDISPINFOW: {
	 NMTTDISPINFO lpnmtdi;
	 if (hdr.code == OS.TTN_GETDISPINFOA) {
	 lpnmtdi = new NMTTDISPINFOA ();
	 OS.MoveMemory ((NMTTDISPINFOA)lpnmtdi, lParam, NMTTDISPINFOA.sizeof);
	 } else {
	 lpnmtdi = new NMTTDISPINFOW ();
	 OS.MoveMemory ((NMTTDISPINFOW)lpnmtdi, lParam, NMTTDISPINFOW.sizeof);
	 }
	 String string = toolTipText (lpnmtdi);
	 if (string != null) {
	 Shell shell = getShell ();
	 string = Display.withCrLf (string);*/
	/*
	 * Bug in Windows.  On Windows 7, tool tips hang when displaying large
	 * strings. The fix is to limit the tool tip string to 4Kb.
	 */
	/*if (string.length() > TOOLTIP_LIMIT) {
	 string = string.substring(0, TOOLTIP_LIMIT);
	 }*/
	/*
	 * Bug 475858: In Japanese like languages where mnemonics are not taken from the
	 * source label text but appended in parentheses like "(&M)" at end. In order to
	 * allow the reuse of such label text as a tool-tip text as well, "(&M)" like
	 * character sequence has to be removed from the end of CJK-style mnemonics.
	 */
//char [] chars = fixMnemonic (string, false, true);
	/*
	 * Ensure that the orientation of the tool tip matches
	 * the orientation of the control.
	 */
	/*Widget widget = null;
	 long hwnd = hdr.idFrom;
	 if ((lpnmtdi.uFlags & OS.TTF_IDISHWND) != 0) {
	 widget = display.getControl (hwnd);
	 } else {
	 if (hdr.hwndFrom == shell.toolTipHandle || hdr.hwndFrom == shell.balloonTipHandle) {
	 widget = shell.findToolTip ((int)hdr.idFrom);
	 }
	 }
	 if (widget != null) {
	 int style = widget.getStyle();
	 int flags = SWT.RIGHT_TO_LEFT | SWT.FLIP_TEXT_DIRECTION;
	 if ((style & flags) != 0 && (style & flags) != flags) {
	 lpnmtdi.uFlags |= OS.TTF_RTLREADING;
	 } else {
	 lpnmtdi.uFlags &= ~OS.TTF_RTLREADING;
	 }
	 }

	 if (hdr.code == OS.TTN_GETDISPINFOA) {
	 byte [] bytes = new byte [chars.length * 2];
	 OS.WideCharToMultiByte (getCodePage (), 0, chars, chars.length, bytes, bytes.length, null, null);
	 shell.setToolTipText (lpnmtdi, bytes);
	 OS.MoveMemory (lParam, (NMTTDISPINFOA)lpnmtdi, NMTTDISPINFOA.sizeof);
	 } else {
	 shell.setToolTipText (lpnmtdi, chars);
	 OS.MoveMemory (lParam, (NMTTDISPINFOW)lpnmtdi, NMTTDISPINFOW.sizeof);
	 }
	 return LRESULT.ZERO;
	 }
	 break;
	 }
	 }*/
	return _CONTROL_WM_NOTIFY(widget, e, reserved);
}
int _w_composite_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	//display.runSkin ();
	w_size size;
	w_layout *layout;
	w_composite_get_layout(W_COMPOSITE(widget),&layout);
	if (layout != 0) {
		if (e->wHint == W_DEFAULT || e->hHint == W_DEFAULT) {
			//changed |= (state & LAYOUT_CHANGED) != 0;
			//state &= ~LAYOUT_CHANGED;
			w_layout_compute_size(layout, W_COMPOSITE(widget), &size, e->wHint,
					e->hHint,W_TRUE);
		} else {
			size.width = e->wHint;
			size.height = e->hHint;
		}
	} else {
		_w_composite_minimum_size(W_COMPOSITE(widget), &size, e->wHint,
				e->hHint, TRUE);
		if (size.width == 0)
			size.width = DEFAULT_WIDTH;
		if (size.height == 0)
			size.height = DEFAULT_HEIGHT;
	}
	if (e->wHint != W_DEFAULT)
		size.width = e->wHint;
	if (e->hHint != W_DEFAULT)
		size.height = e->hHint;
	/*
	 * Since computeTrim can be overridden by subclasses, we cannot
	 * call computeTrimInPixels directly.
	 */
	w_rect rect, trim;
	rect.x = 0;
	rect.y = 0;
	rect.width = size.width;
	rect.height = size.height;
	w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim, &rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return TRUE;
}
void _w_composite_class_init(struct _w_composite_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_composite_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_composite);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_composite);
	/*
	 * composite class
	 */
	clazz->get_children = _w_composite_get_children;
	clazz->get_layout = _w_composite_get_layout;
	clazz->get_layout_deferred = _w_composite_get_layout_deferred;
	clazz->get_tab_list = _w_composite_get_tab_list;
	clazz->is_layout_deferred = _w_composite_is_layout_deferred;
	clazz->do_layout = _w_composite_do_layout;
	clazz->set_layout = _w_composite_set_layout;
	clazz->set_layout_deferred = _w_composite_set_layout_deferred;
	W_WIDGET_CLASS(clazz)->create = _w_composite_create;

	_w_toolkit_registre_class(win_toolkit);
	/*
	 * reserved
	 */
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_CONTROL_RESERVED(reserved)->compute_size = _w_composite_compute_size;
//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->messages[_WM_ERASEBKGND] = _COMPOSITE_WM_ERASEBKGND;
	msg->messages[_WM_GETDLGCODE] = _COMPOSITE_WM_GETDLGCODE;
	msg->messages[_WM_GETFONT] = _COMPOSITE_WM_GETFONT;
	msg->messages[_WM_LBUTTONDOWN] = _COMPOSITE_WM_LBUTTONDOWN;
	msg->messages[_WM_NCHITTEST] = _COMPOSITE_WM_NCHITTEST;
	msg->messages[_WM_PARENTNOTIFY] = _COMPOSITE_WM_PARENTNOTIFY;
	msg->messages[_WM_PAINT] = _COMPOSITE_WM_PAINT;
	msg->messages[_WM_PRINTCLIENT] = _COMPOSITE_WM_PRINTCLIENT;
	msg->messages[_WM_SETFONT] = _COMPOSITE_WM_SETFONT;
	msg->messages[_WM_SIZE] = _COMPOSITE_WM_SIZE;
	msg->messages[_WM_SYSCOLORCHANGE] = _COMPOSITE_WM_SYSCOLORCHANGE;
	msg->messages[_WM_SYSCOMMAND] = _COMPOSITE_WM_SYSCOMMAND;
	msg->messages[_WM_UPDATEUISTATE] = _COMPOSITE_WM_UPDATEUISTATE;
	msg->messages[_WM_NCPAINT] = _COMPOSITE_WM_NCPAINT;
	msg->messages[_WM_NOTIFY] = _COMPOSITE_WM_NOTIFY;
}
#endif

