/*
 * sash.c
 *
 *  Created on: 8 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "sash.h"
#include "../widgets/toolkit.h"
int _w_sash_check_style(int style) {
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
unsigned char draw_band_bits[] = { -86, 0, 85, 0, -86, 0, 85, 0, -86, 0, 85, 0,
		-86, 0, 85, 0 };
void _w_sash_draw_band(w_widget *widget, int x, int y, int width, int height) {
	if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0)
		return;
	w_composite *parent;
	w_control_get_parent(W_CONTROL(widget),&parent);
	HWND hwndTrack = _W_WIDGET(parent)->handle;
	HBITMAP stippleBitmap = CreateBitmap(8, 8, 1, 1, draw_band_bits);
	HBRUSH stippleBrush = CreatePatternBrush(stippleBitmap);
	HDC hDC = GetDCEx(hwndTrack, 0, DCX_CACHE);
	HBRUSH oldBrush = (HBRUSH) SelectObject(hDC, stippleBrush);
	PatBlt(hDC, x, y, width, height, PATINVERT);
	SelectObject(hDC, oldBrush);
	ReleaseDC(hwndTrack, hDC);
	DeleteObject(stippleBrush);
	DeleteObject(stippleBitmap);
}
wresult _w_sash_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_sash_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_control_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
			WindowClass, dwExStyle, dwStyle);
	if (result > 0) {
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
	}
	return result;
}
/*
 * messages
 */
int _SASH_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int ret = _WIDGET_WM_LBUTTONDOWN(widget, e, reserved);
	/*if (e->result == 0)
	 return ret;*/

	/* Compute the banding rectangle */
	HWND hwndTrack = GetParent(_W_WIDGET(widget)->handle);
	POINT pt;
	POINTSTOPOINT(pt, e->lparam);
	RECT rect;
	GetWindowRect(_W_WIDGET(widget)->handle, &rect);
	MapWindowPoints(_W_WIDGET(widget)->handle, 0, &pt, 1);
	_W_SASH(widget)->startX = pt.x - rect.left;
	_W_SASH(widget)->startY = pt.y - rect.top;
	MapWindowPoints(0, hwndTrack, (LPPOINT) &rect, 2);
	_W_SASH(widget)->lastX = rect.left;
	_W_SASH(widget)->lastY = rect.top;
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	/* The event must be sent because doit flag is used */
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.platform_event = (w_event_platform*) e;
	event.event.data = 0;
	event.event.widget = widget;
	event.bounds.x = _W_SASH(widget)->lastX;
	event.bounds.y = _W_SASH(widget)->lastY;
	event.bounds.width = width;
	event.bounds.height = height;
	int doit = _w_widget_send_event(widget, (w_event*) &event);
	if (!w_widget_is_ok(W_WIDGET(widget))) {
		e->result = 0;
		return ret;
	}
	/*			event.setBoundsInPixels(new Rectangle(lastX, lastY, width, height));
	 if ((style & SWT.SMOOTH) == 0) {
	 event.detail = SWT.DRAG;
	 }
	 sendSelectionEvent (SWT.Selection, event, true);
	 if (isDisposed ()) return LRESULT.ZERO;
	 */
	/* Draw the banding rectangle */
	//Rectangle bounds = event.getBoundsInPixels();
	if (doit) {
		_W_WIDGET(widget)->state |= STATE_SASH_DRAGGING;
		_W_SASH(widget)->lastX = event.bounds.x;
		_W_SASH(widget)->lastY = event.bounds.y;
		//menuShell ().bringToTop ();
		//if (isDisposed ()) return LRESULT.ZERO;
#if IsWinCE
			OS.UpdateWindow (hwndTrack);
#else
		int flags = RDW_UPDATENOW | RDW_ALLCHILDREN;
		RedrawWindow(hwndTrack, 0, 0, flags);
#endif
		_w_sash_draw_band(widget, event.bounds.x, event.bounds.y, width,
				height);
		if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
			w_rect r = { event.bounds.x, event.bounds.y, width, height };
			w_control_set_bounds(W_CONTROL(widget), &r.pt,&r.sz);
			// widget could be disposed at this point
		}
	}
	return ret;
}
int _SASH_WM_LBUTTONUP(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int ret = _WIDGET_WM_LBUTTONUP(widget, e, reserved);
	/*if (e->result == 0)
	 return ret;*/

	/* Compute the banding rectangle */
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING))
		return ret;
	_W_WIDGET(widget)->state &= ~STATE_SASH_DRAGGING;
	RECT rect;
	GetWindowRect(_W_WIDGET(widget)->handle, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	/* The event must be sent because doit flag is used */
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.platform_event = (w_event_platform*) e;
	event.event.data = 0;
	event.event.widget = widget;
	event.bounds.x = _W_SASH(widget)->lastX;
	event.bounds.y = _W_SASH(widget)->lastY;
	event.bounds.width = width;
	event.bounds.height = height;
	int doit = _w_widget_send_event(widget, (w_event*) &event);
	if (!w_widget_is_ok(W_WIDGET(widget))) {
		return ret;
	}
	_w_sash_draw_band(widget, _W_SASH(widget)->lastX,
	_W_SASH(widget)->lastY, width, height);
	if (doit) {
		if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
			w_rect r = { event.bounds.x, event.bounds.y, width, height };
			w_control_set_bounds(W_CONTROL(widget),&r.pt,&r.sz);
			// widget could be disposed at this point
		}
	}
	return ret;
}
int _SASH_WM_MOUSEMOVE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int ret = _CONTROL_WM_MOVE(widget, e, reserved);
	//if (ret)
	//	return ret;
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)
			|| (e->wparam & MK_LBUTTON) == 0)
		return ret;

	/* Compute the banding rectangle */
	POINT pt;
	POINTSTOPOINT(pt, e->lparam);
	HWND hwndTrack = GetParent(_W_WIDGET(widget)->handle);
	MapWindowPoints(_W_WIDGET(widget)->handle, hwndTrack, &pt, 1);
	RECT rect, clientRect;
	GetWindowRect(_W_WIDGET(widget)->handle, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	GetClientRect(hwndTrack, &clientRect);
	int newX = _W_SASH(widget)->lastX, newY = _W_SASH(widget)->lastY;
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		int clientWidth = clientRect.right - clientRect.left;
		newX = w_min(w_max(0, pt.x - _W_SASH(widget)->startX),
				clientWidth - width);
	} else {
		int clientHeight = clientRect.bottom - clientRect.top;
		newY = w_min(w_max(0, pt.y - _W_SASH(widget)->startY),
				clientHeight - height);
	}
	if (newX == _W_SASH(widget)->lastX && newY == _W_SASH(widget)->lastY)
		return ret;
	_w_sash_draw_band(widget, _W_SASH(widget)->lastX,
	_W_SASH(widget)->lastY, width, height);

	/* The event must be sent because doit flag is used */
	/*event.setBoundsInPixels(new Rectangle(newX, newY, width, height));
	 if ((style & SWT.SMOOTH) == 0) {
	 event.detail = SWT.DRAG;
	 }
	 sendSelectionEvent(SWT.Selection, event, true);*/
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.platform_event = (w_event_platform*) e;
	event.event.data = 0;
	event.event.widget = widget;
	event.bounds.x = newX;
	event.bounds.y = newY;
	event.bounds.width = width;
	event.bounds.height = height;
	int doit = _w_widget_send_event(widget, (w_event*) &event);
	if (!w_widget_is_ok(W_WIDGET(widget))) {
		e->result = 0;
		return ret;
	}
	if (doit) {
		//w_rect boundsInPixels = event.getBoundsInPixels();
		_W_SASH(widget)->lastX = event.bounds.x;
		_W_SASH(widget)->lastY = event.bounds.y;
	}
#if IsWinCE
		UpdateWindow(hwndTrack);
#else
	int flags = RDW_UPDATENOW | RDW_ALLCHILDREN;
	RedrawWindow(hwndTrack, 0, 0, flags);
#endif
	_w_sash_draw_band(widget, _W_SASH(widget)->lastX,
	_W_SASH(widget)->lastY, width, height);
	if ((_W_WIDGET(widget)->style & W_SMOOTH) != 0) {
		w_rect r = { _W_SASH(widget)->lastX, _W_SASH(widget)->lastY, width,
				height };
		w_control_set_bounds(W_CONTROL(widget), &r.pt,&r.sz);
		// widget could be disposed at this point
	}
	return ret;
}
int _SASH_WM_SETCURSOR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int ret = _CONTROL_WM_SETCURSOR(widget, e, reserved);
	if (ret)
		return ret;
	int hitTest = (short) LOWORD(e->lparam);
	if (hitTest == HTCLIENT) {
		HCURSOR hCursor = 0;
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			hCursor = LoadCursor(0, IDC_SIZENS);
		} else {
			hCursor = LoadCursor(0, IDC_SIZEWE);
		}
		SetCursor(hCursor);
	}
	e->result = 1;
	return W_TRUE;
}
void _w_sash_class_init(struct _w_sash_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SASH;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_sash_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_sash);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_sash);

	W_WIDGET_CLASS(clazz)->create = _w_sash_create;
	/*
	 * reserved
	 */
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->messages[_WM_SETCURSOR] = _SASH_WM_SETCURSOR;
	msg->messages[_WM_MOUSEMOVE] = _SASH_WM_MOUSEMOVE;
	msg->messages[_WM_LBUTTONUP] = _SASH_WM_LBUTTONUP;
	msg->messages[_WM_LBUTTONDOWN] = _SASH_WM_LBUTTONDOWN;
}

#endif
