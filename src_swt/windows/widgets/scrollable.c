/*
 * scrollable.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "scrollable.h"
#include "toolkit.h"
void _w_scrollable_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_control_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= WS_TABSTOP;
	if ((style & W_HSCROLL) != 0)
		*dwStyle |= WS_HSCROLL;
	if ((style & W_VSCROLL) != 0)
		*dwStyle |= WS_VSCROLL;
}
/*
 * public function for scrollbar
 */
void _w_scrollbar_copy(w_widgetdata *from, w_widgetdata *to) {
}
w_scrollable* _w_scrollbar_get_parent(w_scrollbar *scrollbar) {
	return _W_SCROLLBAR(scrollbar)->scrollable;
}
wresult _w_scrollbar_get_enabled(w_scrollbar *scrollbar) {

}
wresult _w_scrollbar_is_enabled(w_scrollbar *scrollbar) {
}
void _w_scrollbar_set_enabled(w_scrollbar *scrollbar, wresult enabled) {
}
wresult _w_scrollbar_get_visible(w_scrollbar *scrollbar) {
}
wresult _w_scrollbar_is_visible(w_scrollbar *scrollbar) {
}
void _w_scrollbar_set_visible(w_scrollbar *scrollbar, wresult visible) {
}
void _w_scrollbar_get_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values) {
}
int _w_scrollbar_get_increment(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_maximum(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_minimum(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_page_increment(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_selection(w_scrollbar *scrollbar) {
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			_w_widget_get_reserved(W_WIDGET(_W_SCROLLBAR(scrollbar)->scrollable)));
	HWND hwnd = reserved->scrolledHandle(
			W_CONTROL(_W_SCROLLBAR(scrollbar)->scrollable));
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_POS;
	GetScrollInfo(hwnd, type, &info);
	return info.nPos;
}
int _w_scrollbar_get_thumb(w_scrollbar *scrollbar) {
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			_w_widget_get_reserved(W_WIDGET(_W_SCROLLBAR(scrollbar)->scrollable)));
	HWND hwnd = reserved->scrolledHandle(
			W_CONTROL(_W_SCROLLBAR(scrollbar)->scrollable));
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_PAGE;
	GetScrollInfo(hwnd, type, &info);
	if (info.nPage != 0)
		--info.nPage;
	return info.nPage;
}
void _w_scrollbar_set_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values) {
	if (values->minimum < 0)
		return;
	if (values->maximum < 0)
		return;
	if (values->thumb < 1)
		return;
	if (values->increment < 1)
		return;
	if (values->pageIncrement < 1)
		return;
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			_w_widget_get_reserved(W_WIDGET(_W_SCROLLBAR(scrollbar)->scrollable)));
	HWND hwnd = reserved->scrolledHandle(
			W_CONTROL(_W_SCROLLBAR(scrollbar)->scrollable));
	int type = _W_SCROLLBAR(scrollbar)->type;
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
	info.nPos = values->selection;
	info.nMin = values->minimum;
	info.nMax = values->maximum;
	info.nPage = values->thumb;
	if (info.nPage != 0)
		info.nPage++;
	SetScrollInfo(hwnd, type, &info, TRUE);
}
void _w_scrollbar_set_increment(w_scrollbar *scrollbar, int increment) {
}
void _w_scrollbar_set_maximum(w_scrollbar *scrollbar, int maximum) {
	if (maximum < 0)
		return;
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			_w_widget_get_reserved(W_WIDGET(_W_SCROLLBAR(scrollbar)->scrollable)));
	HWND hwnd = reserved->scrolledHandle(
			W_CONTROL(_W_SCROLLBAR(scrollbar)->scrollable));
	int type = _W_SCROLLBAR(scrollbar)->type;
	info.fMask = SIF_RANGE | SIF_DISABLENOSCROLL;
	GetScrollInfo(hwnd, type, &info);
	if (maximum - info.nMin - info.nPage < 1)
		return;
	info.nMax = maximum;
	SetScrollInfo(hwnd, type, &info, TRUE);
}
void _w_scrollbar_set_minimum(w_scrollbar *scrollbar, int minimum) {
}
void _w_scrollbar_set_page_increment(w_scrollbar *scrollbar, int pageIncrement) {
}
void _w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection) {
}
void _w_scrollbar_set_thumb(w_scrollbar *scrollbar, int thumb) {
	if (thumb < 0)
		return;
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			_w_widget_get_reserved(W_WIDGET(_W_SCROLLBAR(scrollbar)->scrollable)));
	HWND hwnd = reserved->scrolledHandle(
			W_CONTROL(_W_SCROLLBAR(scrollbar)->scrollable));
	int type = _W_SCROLLBAR(scrollbar)->type;
	info.fMask = SIF_PAGE | SIF_RANGE | SIF_DISABLENOSCROLL;
	GetScrollInfo(hwnd, type, &info);
	info.nPage = thumb;
	if (info.nPage != 0)
		info.nPage++;
	SetScrollInfo(hwnd, type, &info, TRUE);
}
void _w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size) {
}
void _w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect) {
	//parent.forceResize ();
	SCROLLBARINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			_w_widget_get_reserved(W_WIDGET(_W_SCROLLBAR(scrollbar)->scrollable)));
	HWND hwnd = reserved->scrolledHandle(
			W_CONTROL(_W_SCROLLBAR(scrollbar)->scrollable));
	int type = _W_SCROLLBAR(scrollbar)->type;
	info.cbSize = sizeof(SCROLLBARINFO);
	RECT r;
	if ((type & SB_HORZ) != 0) {
		GetScrollBarInfo(hwnd, OBJID_HSCROLL, &info);
		r.left = info.rcScrollBar.left + info.xyThumbTop;
		r.top = info.rcScrollBar.top;
		r.right = info.xyThumbBottom - info.xyThumbTop + r.left;
		r.bottom = info.rcScrollBar.bottom - info.rcScrollBar.top + r.top;
	} else {
		GetScrollBarInfo(hwnd, OBJID_VSCROLL, &info);
		r.left = info.rcScrollBar.left;
		r.top = info.rcScrollBar.top + info.xyThumbTop;
		r.right = info.rcScrollBar.right - info.rcScrollBar.left + r.left;
		r.bottom = info.xyThumbBottom - info.xyThumbTop + r.top;
	}
	MapWindowPoints(0, hwnd, (LPPOINT) &r, 2);
	rect->x = r.left;
	rect->y = r.top;
	rect->width = r.right - r.left;
	rect->height = r.bottom - r.top;
}
void _w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar, w_rect *rect) {
	//parent.forceResize ();
	/*SCROLLBARINFO info;
	 info.cbSize = sizeof(SCROLLINFO);
	 struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
	 _w_widget_get_reserved(_W_SCROLLBAR(scrollbar)->scrollable));
	 HWND hwnd = reserved->scrolledHandle(
	 _W_CONTROL(_W_SCROLLBAR(scrollbar)->scrollable));
	 int type = _W_SCROLLBAR(scrollbar)->type;
	 info.cbSize = sizeof(SCROLLBARINFO);
	 RECT r;
	 memset(&r,0,sizeof(r));
	 if ((type & SB_HORZ) != 0) {
	 GetScrollBarInfo(hwnd, OBJID_HSCROLL, info);
	 int size = GetSystemMetrics (SM_CYHSCROLL);
	 r.top = info.rcScrollBar.top;
	 width = info.rcScrollBar.right - info.rcScrollBar.left;
	 height = size;
	 if (width <= 2 * size) {
	 r.left = info.rcScrollBar.left + width / 2;
	 width = 0;
	 } else {
	 r.left = info.rcScrollBar.left + size;
	 width -= 2 * size;
	 }
	 } else {
	 GetScrollBarInfo(hwnd, OBJID_VSCROLL, info);
	 int size = GetSystemMetrics (SM_CYVSCROLL);
	 r.left = info.rcScrollBar.left;
	 width = size;
	 height = info.rcScrollBar.bottom - info.rcScrollBar.top;
	 if (height <= 2 * size) {
	 r.top = info.rcScrollBar.top + height / 2;
	 height = 0;
	 } else {
	 y = info.rcScrollBar.top + size;
	 height -= 2 * size;
	 }
	 }
	 MapWindowPoints (0, hwnd, &r, 2);
	 rect->x = r.left;
	 rect->y = r.top;
	 rect->width = r.right - r.left;
	 rect->height = r.bottom - r.top;*/
}
/*
 * public function for scrollable
 */
void _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	_W_SCROLLBAR(scrollbar)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_SCROLLABLE_GET_CLASS(scrollable)->class_scrollbar);
	_W_SCROLLBAR(scrollbar)->type = SB_HORZ;
	_W_SCROLLBAR(scrollbar)->scrollable = scrollable;
}
int _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable) {
	return 0;
}
void _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	_W_SCROLLBAR(scrollbar)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_SCROLLABLE_GET_CLASS(scrollable)->class_scrollbar);
	_W_SCROLLBAR(scrollbar)->type = SB_VERT;
	_W_SCROLLBAR(scrollbar)->scrollable = scrollable;
}
/*
 * post event proc
 */
int _wm_scroll(w_widget *widget, struct _w_event_platform *e, wresult update) {
	int type = e->msg == WM_HSCROLL ? SB_HORZ : SB_VERT;
	if (update) {
		SCROLLINFO info;
		info.cbSize = sizeof(SCROLLINFO);
		info.fMask = SIF_TRACKPOS | SIF_POS | SIF_RANGE;
		GetScrollInfo(e->hwnd, type, &info);
		info.fMask = SIF_POS;
		int code = LOWORD(e->wparam);
		switch (code) {
		case SB_ENDSCROLL:
			return W_FALSE;
		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			/*
			 * Note: On WinCE, the value in SB_THUMBPOSITION is relative to nMin.
			 * Same for SB_THUMBPOSITION 'except' for the very first thumb track
			 * message which has the actual value of nMin. This is a problem when
			 * nMin is not zero.
			 */
			info.nPos = info.nTrackPos;
			break;
		case SB_TOP:
			info.nPos = info.nMin;
			break;
		case SB_BOTTOM:
			info.nPos = info.nMax;
			break;
			/*case SB_LINEDOWN:
			 info.nPos += bar.getIncrement();
			 break;
			 case SB_LINEUP:
			 int increment = bar.getIncrement();
			 info.nPos = w_max(info.nMin, info.nPos - increment);
			 break;
			 case SB_PAGEDOWN:
			 info.nPos += bar.getPageIncrement();
			 break;
			 case SB_PAGEUP:
			 int pageIncrement = bar.getPageIncrement();
			 info.nPos = w_max(info.nMin, info.nPos - pageIncrement);
			 break;*/
		}
		SetScrollInfo(e->hwnd, type, &info, TRUE);
	} else {
		struct _w_widget_reserved *reserved = _w_widget_get_reserved(widget);
		reserved->def_proc(widget, e, reserved);
	}

	/* Do nothing when scrolling is ending */
	int code = LOWORD(e->wparam);
	if (code == SB_ENDSCROLL)
		return W_FALSE;

	/*
	 * Send the event because WM_HSCROLL and
	 * WM_VSCROLL are sent from a modal message
	 * loop in Windows that is active when the
	 * user is scrolling.
	 */
	w_event_scrollbar event;
	w_scrollbar scrollbar;
	memset(&event, 0, sizeof(event));
	if (type == SB_HORZ) {
		event.event.type = W_EVENT_HSCROLL;
		_w_scrollable_get_horizontal_bar(W_SCROLLABLE(widget), &scrollbar);
	} else {
		event.event.type = W_EVENT_VSCROLL;
		_w_scrollable_get_vertical_bar(W_SCROLLABLE(widget), &scrollbar);
	}
	event.event.widget = widget;
	event.scrollbar = &scrollbar;
	switch (code) {
	case SB_THUMBPOSITION:
		event.detail = W_NONE;
		break;
	case SB_THUMBTRACK:
		event.detail = W_DRAG;
		break;
	case SB_TOP:
		event.detail = W_HOME;
		break;
	case SB_BOTTOM:
		event.detail = W_END;
		break;
	case SB_LINEDOWN:
		event.detail = W_ARROW_DOWN;
		break;
	case SB_LINEUP:
		event.detail = W_ARROW_UP;
		break;
	case SB_PAGEDOWN:
		event.detail = W_PAGE_DOWN;
		break;
	case SB_PAGEUP:
		event.detail = W_PAGE_UP;
		break;
	}
	_w_widget_send_event(widget, (w_event*) &event);
	return W_TRUE;
}

int _SCROLLABLE_WM_MOUSEWHEEL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	//return wmScrollWheel ((state & CANVAS) != 0, wParam, lParam);
	return W_FALSE;
}

int _SCROLLABLE_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	reserved->def_proc(widget, e, reserved);
	_CONTROL_WM_SIZE(widget, e, reserved);
	return W_TRUE;
}

int _wm_scrollbale_ScrollWheel(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_MOUSEWHEEL (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Translate WM_MOUSEWHEEL to WM_VSCROLL or WM_HSCROLL.
	 */
	/*if (update) {
	 if ((wParam & (OS.MK_SHIFT | OS.MK_CONTROL)) != 0) return result;
	 boolean vertical = verticalBar != null && verticalBar.getEnabled ();
	 boolean horizontal = horizontalBar != null && horizontalBar.getEnabled ();
	 int msg = vertical ? OS.WM_VSCROLL : horizontal ? OS.WM_HSCROLL : 0;
	 if (msg == 0) return result;
	 int [] linesToScroll = new int [1];
	 OS.SystemParametersInfo (OS.SPI_GETWHEELSCROLLLINES, 0, linesToScroll, 0);
	 int delta = OS.GET_WHEEL_DELTA_WPARAM (wParam);
	 boolean pageScroll = linesToScroll [0] == OS.WHEEL_PAGESCROLL;
	 ScrollBar bar = vertical ? verticalBar : horizontalBar;
	 SCROLLINFO info = new SCROLLINFO ();
	 info.cbSize = SCROLLINFO.sizeof;
	 info.fMask = OS.SIF_POS;
	 OS.GetScrollInfo (handle, bar.scrollBarType (), info);
	 if (vertical && !pageScroll) delta *= linesToScroll [0];
	 int increment = pageScroll ? bar.getPageIncrement () : bar.getIncrement ();
	 info.nPos -=  increment * delta / OS.WHEEL_DELTA;
	 OS.SetScrollInfo (handle, bar.scrollBarType (), info, true);
	 OS.SendMessage (handle, msg, OS.SB_THUMBPOSITION, 0);
	 return LRESULT.ZERO;
	 }*/

	/*
	 * When the native widget scrolls inside WM_MOUSEWHEEL, it
	 * may or may not send a WM_VSCROLL or WM_HSCROLL to do the
	 * actual scrolling.  This depends on the implementation of
	 * each native widget.  In order to ensure that application
	 * code is notified when the scroll bar moves, compare the
	 * scroll bar position before and after the WM_MOUSEWHEEL.
	 * If the native control sends a WM_VSCROLL or WM_HSCROLL,
	 * then the application has already been notified.  If not
	 * explicitly send the event.
	 */
	/*int vPosition = verticalBar == null ? 0 : verticalBar.getSelection ();
	 int hPosition = horizontalBar == null ? 0 : horizontalBar.getSelection ();
	 long code = callWindowProc (handle, OS.WM_MOUSEWHEEL, wParam, lParam);
	 if (verticalBar != null) {
	 int position = verticalBar.getSelection ();
	 if (position != vPosition) {
	 Event event = new Event ();
	 event.detail = position < vPosition ? SWT.PAGE_UP : SWT.PAGE_DOWN;
	 verticalBar.sendSelectionEvent (SWT.Selection, event, true);
	 }
	 }
	 if (horizontalBar != null) {
	 int position = horizontalBar.getSelection ();
	 if (position != hPosition) {
	 Event event = new Event ();
	 event.detail = position < hPosition ? SWT.PAGE_UP : SWT.PAGE_DOWN;
	 horizontalBar.sendSelectionEvent (SWT.Selection, event, true);
	 }
	 }
	 return new LRESULT (code);*/
	return W_FALSE;
}

int _wm_scrollbale_Scroll(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*	LRESULT result = null;
	 if (update) {
	 int type = msg == OS.WM_HSCROLL ? OS.SB_HORZ : OS.SB_VERT;
	 SCROLLINFO info = new SCROLLINFO ();
	 info.cbSize = SCROLLINFO.sizeof;
	 info.fMask = OS.SIF_TRACKPOS | OS.SIF_POS | OS.SIF_RANGE;
	 OS.GetScrollInfo (hwnd, type, info);
	 info.fMask = OS.SIF_POS;
	 int code = OS.LOWORD (wParam);
	 switch (code) {
	 case OS.SB_ENDSCROLL:  return null;
	 case OS.SB_THUMBPOSITION:
	 case OS.SB_THUMBTRACK:
	 info.nPos = info.nTrackPos;
	 break;
	 case OS.SB_TOP:
	 info.nPos = info.nMin;
	 break;
	 case OS.SB_BOTTOM:
	 info.nPos = info.nMax;
	 break;
	 case OS.SB_LINEDOWN:
	 info.nPos += bar.getIncrement ();
	 break;
	 case OS.SB_LINEUP:
	 int increment = bar.getIncrement ();
	 info.nPos = Math.max (info.nMin, info.nPos - increment);
	 break;
	 case OS.SB_PAGEDOWN:
	 info.nPos += bar.getPageIncrement ();
	 break;
	 case OS.SB_PAGEUP:
	 int pageIncrement = bar.getPageIncrement ();
	 info.nPos = Math.max (info.nMin, info.nPos - pageIncrement);
	 break;
	 }
	 OS.SetScrollInfo (hwnd, type, info, true);
	 } else {
	 long code = callWindowProc (hwnd, msg, wParam, lParam);
	 result = code == 0 ? LRESULT.ZERO : new LRESULT (code);
	 }
	 bar.wmScrollChild (wParam, lParam);
	 return result;*/
	return W_FALSE;
}
int _SCROLLABLE_WM_VSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _CONTROL_WM_VSCROLL(widget, e, reserved);
	if (result)
		return result;
	if ((_W_WIDGET(widget)->style & W_VSCROLL) && e->lparam == 0) {
		return _wm_scroll(widget, e,
				(_W_WIDGET(widget)->state & STATE_CANVAS) != 0);
	}
	return result;
}
int _SCROLLABLE_WM_HSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _CONTROL_WM_VSCROLL(widget, e, reserved);
	if (result)
		return result;
	if ((_W_WIDGET(widget)->style & W_HSCROLL) && e->lparam == 0) {
		return _wm_scroll(widget, e,
				(_W_WIDGET(widget)->state & STATE_CANVAS) != 0);
	}
	return result;
}
int _w_scrollable_get_client_area(w_widget *widget,
		struct w_event_client_area *e, struct _w_widget_reserved *reserved) {
	RECT rect;
	HWND scrolledHandle = _W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(
			W_CONTROL(widget));
	GetClientRect(scrolledHandle, &rect);
	e->rect->x = rect.left;
	e->rect->y = rect.top;
	e->rect->width = rect.right - rect.left;
	e->rect->height = rect.bottom - rect.top;
	if (scrolledHandle != _W_WIDGET(widget)->handle) {
		GetClientRect(_W_WIDGET(widget)->handle, &rect);
		MapWindowPoints(_W_WIDGET(widget)->handle, scrolledHandle,
				(POINT*) &rect, 2);
		e->rect->x = -rect.left;
		e->rect->y = -rect.top;
	}
	return W_TRUE;
}
int _w_scrollable_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved) {
	RECT rect;
	HWND scrolledHandle = _W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(
			W_CONTROL(widget));
	SetRect(&rect, e->rect->x, e->rect->y, e->rect->x + e->rect->width,
			e->rect->y + e->rect->height);
	int bits1 = GetWindowLong(scrolledHandle, GWL_STYLE);
	int bits2 = GetWindowLong(scrolledHandle, GWL_EXSTYLE);
	AdjustWindowRectEx(&rect, bits1, FALSE, bits2);
	if (_W_WIDGET(widget)->style & W_HSCROLL)
		rect.bottom += GetSystemMetrics(SM_CYHSCROLL);
	if (_W_WIDGET(widget)->style & W_VSCROLL)
		rect.right += GetSystemMetrics(SM_CXVSCROLL);
	e->result->width = rect.right - rect.left;
	e->result->height = rect.bottom - rect.top;
	e->result->x = rect.left;
	e->result->y = rect.top;
	return W_TRUE;
}
int _w_scrollable_post_event(w_widget *widget, struct w_event *ee) {
	return _w_control_post_event(widget, ee);
}
void _w_scrollable_class_init(struct _w_scrollable_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	/*
	 * scrollable class init
	 */
	clazz->get_horizontal_bar = _w_scrollable_get_horizontal_bar;
	clazz->get_scrollbars_mode = _w_scrollable_get_scrollbars_mode;
	clazz->get_vertical_bar = _w_scrollable_get_vertical_bar;
	W_WIDGET_CLASS(clazz)->post_event = _w_scrollable_post_event;
	/*
	 * scrollbar class
	 */
	clazz->class_scrollbar = &win_toolkit->class_scrollbar;
	if (win_toolkit->class_scrollbar.get_values == 0) {
		struct _w_scrollbar_class *scrollbar = &win_toolkit->class_scrollbar;
		scrollbar->widgetdata.toolkit = win_toolkit;
		scrollbar->widgetdata.is_ok = _w_widgetdata_is_ok;
		scrollbar->widgetdata.close = _w_widgetdata_close;
		scrollbar->widgetdata.copy = _w_scrollbar_copy;
		scrollbar->get_parent = _w_scrollbar_get_parent;
		scrollbar->get_enabled = _w_scrollbar_get_enabled;
		scrollbar->is_enabled = _w_scrollbar_is_enabled;
		scrollbar->set_enabled = _w_scrollbar_set_enabled;
		scrollbar->get_visible = _w_scrollbar_get_visible;
		scrollbar->is_visible = _w_scrollbar_is_visible;
		scrollbar->set_visible = _w_scrollbar_set_visible;
		scrollbar->get_values = _w_scrollbar_get_values;
		scrollbar->get_increment = _w_scrollbar_get_increment;
		scrollbar->get_maximum = _w_scrollbar_get_maximum;
		scrollbar->get_minimum = _w_scrollbar_get_minimum;
		scrollbar->get_page_increment = _w_scrollbar_get_page_increment;
		scrollbar->get_selection = _w_scrollbar_get_selection;
		scrollbar->get_thumb = _w_scrollbar_get_thumb;
		scrollbar->set_values = _w_scrollbar_set_values;
		scrollbar->set_increment = _w_scrollbar_set_increment;
		scrollbar->set_maximum = _w_scrollbar_set_maximum;
		scrollbar->set_minimum = _w_scrollbar_set_minimum;
		scrollbar->set_page_increment = _w_scrollbar_set_page_increment;
		scrollbar->set_selection = _w_scrollbar_set_selection;
		scrollbar->set_thumb = _w_scrollbar_set_thumb;
		scrollbar->get_size = _w_scrollbar_get_size;
		scrollbar->get_thumb_bounds = _w_scrollbar_get_thumb_bounds;
		scrollbar->get_thumb_track_bounds = _w_scrollbar_get_thumb_track_bounds;
		//
		scrollbar->widgetdata.is_ok = _w_widgetdata_is_ok;
		scrollbar->widgetdata.close = _w_widgetdata_close;
		//scrollbar->widgetdata.copy=_w_widgetdata_is_ok;
		//scrollbar->widgetdata.equals=_w_widgetdata_is_ok;
	}
	/*
	 * reserved
	 */
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->scrolledHandle = _w_control_h;
//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->get_client_area = _w_scrollable_get_client_area;
	msg->compute_trim = _w_scrollable_compute_trim;
	msg->messages[_WM_HSCROLL] = _SCROLLABLE_WM_HSCROLL;
	msg->messages[_WM_MOUSEWHEEL] = _SCROLLABLE_WM_MOUSEWHEEL;
	msg->messages[_WM_SIZE] = _SCROLLABLE_WM_SIZE;
	msg->messages[_WM_VSCROLL] = _SCROLLABLE_WM_VSCROLL;
}
#endif

