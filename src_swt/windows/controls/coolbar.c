/*
 * coolbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "coolbar.h"
#include "../widgets/toolkit.h"
/*
 * private
 */
int _w_coolbar_check_style(int style) {
	style |= W_NO_FOCUS;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
void _w_coolbar_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= CCS_NODIVIDER | CCS_NORESIZE;
	*dwStyle |= RBS_VARHEIGHT | RBS_DBLCLKTOGGLE;
	if ((style & W_FLAT) == 0)
		*dwStyle |= RBS_BANDBORDERS;
}
wresult _w_coolbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_coolbar_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_coolbar_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), W_COMPOSITE(parent),
	REBARCLASSNAMEA, dwExStyle, dwStyle);
	if (result > 0) {
		_W_WIDGET(widget)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);
		/*
		 * Feature in Windows.  When the control is created,
		 * it does not use the default system font.  A new HFONT
		 * is created and destroyed when the control is destroyed.
		 * This means that a program that queries the font from
		 * this control, uses the font in another control and then
		 * destroys this control will have the font unexpectedly
		 * destroyed in the other control.  The fix is to assign
		 * the font ourselves each time the control is created.
		 * The control will not destroy a font that it did not
		 * create.
		 */
		w_font *systemfont = w_toolkit_get_system_font(
				(w_toolkit*) win_toolkit);
		SendMessageW(_W_WIDGET(widget)->handle, WM_SETFONT,
				(WPARAM) _W_FONT(systemfont)->handle, 0);
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
		/*
		 * Ensure that either of HORIZONTAL or VERTICAL is set.
		 * NOTE: HORIZONTAL and VERTICAL have the same values
		 * as H_SCROLL and V_SCROLL so it is necessary to first
		 * clear these bits to avoid scroll bars and then reset
		 * the bits using the original style supplied by the
		 * programmer.
		 *
		 * NOTE: The CCS_VERT style cannot be applied when the
		 * widget is created because of this conflict.
		 */
		if ((style & W_VERTICAL) != 0) {
			_W_WIDGET(widget)->style |= W_VERTICAL;
			int bits = GetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE);
			SetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE,
					bits | CCS_VERT);
		} else {
			_W_WIDGET(widget)->style |= W_HORIZONTAL;
		}
	}
	return result;

}
/*
 *
 */
wresult _w_coolitem_is_last_item_of_row(w_coolbar *coolbar, int index) {
	int count = (int) SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDCOUNT,
			0, 0);
	if (index + 1 == count)
		return TRUE;
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);
	rbBand.fMask = RBBIM_STYLE;
	SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDINFO, index + 1,
			(LPARAM) &rbBand);
	return (rbBand.fStyle & RBBS_BREAK) != 0;
}
int _w_coolitem_get_margin(w_coolbar *coolbar, int index) {
	int margin = 0;
	if (W_COMCTL32_VERSION >= VERSION(6, 0)) {
		MARGINS margins;
		SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDMARGINS, 0,
				(LPARAM) &margins);
		margin += margins.cxLeftWidth + margins.cxRightWidth;
	}
	RECT rect;
	SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDBORDERS, index,
			(LPARAM) &rect);
	if ((_W_WIDGET(coolbar)->style & W_FLAT) != 0) {
		/*
		 * Bug in Windows.  When the style bit  RBS_BANDBORDERS is not set
		 * the rectangle returned by RBS_BANDBORDERS is four pixels too small.
		 * The fix is to add four pixels to the result.
		 */
		if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
			margin += rect.top + 4;
		} else {
			margin += rect.left + 4;
		}
	} else {
		if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
			margin += rect.top + rect.bottom;
		} else {
			margin += rect.left + rect.right;
		}
	}
	if ((_W_WIDGET(coolbar)->style & W_FLAT) == 0) {
		if (!_w_coolitem_is_last_item_of_row(coolbar, index)) {
			margin += SEPARATOR_WIDTH;
		}
	}
	return margin;
}
wresult _w_coolitem_compute_size(w_coolitem *coolitem, w_size *result,
		int wHint, int hHint) {
	if (_W_COOLITEM(coolitem)->index < 0) {
		memset(result, 0, sizeof(w_size));
		return TRUE;
	}
	result->width = wHint;
	result->height = hHint;
	if (wHint == W_DEFAULT)
		result->width = 32;
	if (hHint == W_DEFAULT)
		result->height = 32;
	w_coolbar *parent = _W_COOLITEM(coolitem)->coolbar;
	if ((_W_WIDGET(parent)->style & W_VERTICAL) != 0) {
		result->height += _w_coolitem_get_margin(parent,
		_W_COOLITEM(coolitem)->index);
	} else {
		result->width += _w_coolitem_get_margin(parent,
		_W_COOLITEM(coolitem)->index);
	}
	return TRUE;
}
wresult _w_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds) {
}
w_control* _w_coolitem_get_control(w_coolitem *coolitem) {

}
wresult _w_coolitem_get_minimum_size(w_coolitem *coolitem, w_size *size) {
}
int _w_coolitem_get_order(w_coolitem *coolitem) {
}
wresult _w_coolitem_get_preferred_size(w_coolitem *coolitem, w_size *size) {
}
wresult _w_coolitem_get_size(w_coolitem *coolitem, w_size *size) {
}
int _w_coolitem_get_wrap_indice(w_coolitem *coolitem) {
}
wresult _w_coolitem_set_control(w_coolitem *coolitem, w_control *control) {
	w_coolbar *parent = _W_COOLITEM(coolitem)->coolbar;
	if (control != 0) {
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_ERROR_INVALID_ARGUMENT;
		w_composite* p;
		w_control_get_parent(control,&p);

		if ( p != W_COMPOSITE(parent))
			return W_ERROR_INVALID_PARENT;
	}
	int index = _W_COOLITEM(coolitem)->index;
	if (index < 0)
		return W_FALSE;
	w_control *newControl = control, *oldControl;
	HWND hwnd = _W_WIDGET(parent)->handle;
	HWND hwndChild = 0;
	if (newControl != 0) {
		_w_widget_reserved *reserved = _w_widget_get_reserved(
				W_WIDGET(control));
		hwndChild = _W_CONTROL_RESERVED(reserved)->topHandle(control);
	}
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);
	rbBand.fMask = RBBIM_CHILD;
	rbBand.hwndChild = 0;
	SendMessageW(hwnd, RB_GETBANDINFOW, index, (LPARAM) &rbBand);
	oldControl = (w_control*) GetWindowLongPtrW(rbBand.hwndChild,
	GWLP_USERDATA);
	rbBand.hwndChild = hwndChild;

	/*
	 * Feature in Windows.  When Windows sets the rebar band child,
	 * it makes the new child visible and hides the old child and
	 * moves the new child to the top of the Z-order.  The fix is
	 * to save and restore the visibility and Z-order.
	 */
	HWND hwndAbove = 0;
	if (newControl != 0) {
		hwndAbove = GetWindow(hwndChild, GW_HWNDPREV);
	}
	boolean hideNew = newControl != 0 && !w_control_get_visible(newControl);
	boolean showOld = oldControl != 0 && w_control_get_visible(oldControl);
	SendMessageW(hwnd, RB_SETBANDINFO, index, (LPARAM) &rbBand);
	if (hideNew)
		w_control_set_visible(newControl, FALSE);
	if (showOld)
		w_control_set_visible(oldControl, TRUE);
	if (hwndAbove != 0 && hwndAbove != hwndChild) {
		int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
		SetWindowPos(hwndChild, hwndAbove, 0, 0, 0, 0, flags);
	}
}
wresult _w_coolitem_set_layout(w_coolitem *coolitem, int order,
		int wrapindices, w_size *sizes) {
}
wresult _w_coolitem_set_minimum_size(w_coolitem *coolitem, w_size *size) {
}
wresult _w_coolitem_set_order(w_coolitem *coolitem, int order) {
}
wresult _w_coolitem_set_preferred_size(w_coolitem *coolitem, w_size *size) {
}
wresult _w_coolitem_set_size(w_coolitem *coolitem, w_size *size) {
	int index = _W_COOLITEM(coolitem)->index;
	if (index == -1)
		return FALSE;
	int width = w_max(0, size->width);
	int height = w_max(0, size->height);
	w_coolbar *parent = _W_COOLITEM(coolitem)->coolbar;
	HWND hwnd = _W_WIDGET(parent)->handle;
	int cx, cyChild, cxIdeal;
	if ((_W_WIDGET(parent)->style & W_VERTICAL) != 0) {
		cx = height;
		cyChild = width;
		cxIdeal = w_max(0, height - _w_coolitem_get_margin(parent, index));
	} else {
		cx = width;
		cyChild = height;
		cxIdeal = w_max(0, width - _w_coolitem_get_margin(parent, index));
	}
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);

	/* Get the child size fields first so we don't overwrite them. */
	rbBand.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE;
	SendMessageW(hwnd, RB_GETBANDINFO, index, (LPARAM) &rbBand);

	/* Set the size fields we are currently modifying. */
	/*if (!ideal)*/rbBand.cxIdeal = cxIdeal;
	/*if (!minimum)*/rbBand.cyMinChild = cyChild;
	rbBand.cyChild = cyChild;

	/*
	 * Do not set the size for the last item on the row.
	 */
	//if (!_w_coolitem_is_last_item_of_row(parent, index)) {
	if (W_COMCTL32_VERSION >= VERSION(6, 0)) {
		MARGINS margins;
		SendMessageW(hwnd, RB_GETBANDMARGINS, 0, (LPARAM) &margins);
		cx -= margins.cxLeftWidth + margins.cxRightWidth;
	}
	int separator =
			(_W_WIDGET(parent)->style & W_FLAT) == 0 ? SEPARATOR_WIDTH : 0;
	rbBand.cx = cx - separator;
	rbBand.fMask |= RBBIM_SIZE;
	//}
	SendMessageW(hwnd, RB_SETBANDINFO, index, (LPARAM) &rbBand);
	return TRUE;
}
wresult _w_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
}

wresult _w_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
}
int _w_coolbar_get_item_count(w_coolbar *coolbar) {
}
void _w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {
}
wresult _w_coolbar_get_locked(w_coolbar *coolbar) {
}
wresult _w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
	int count = (int) SendMessageW(_W_WIDGET(coolbar)->handle, RB_GETBANDCOUNT,
			0, 0);
	if (!(0 <= index && index <= count)) {
		index = count;
		//return W_ERROR_INVALID_RANGE;
	}
	REBARBANDINFOW rbBand;
	rbBand.cbSize = sizeof(REBARBANDINFOW);
	rbBand.fMask = RBBIM_TEXT | RBBIM_STYLE | RBBIM_ID;
	rbBand.fStyle = RBBS_VARIABLEHEIGHT | RBBS_GRIPPERALWAYS;
	if ((style & W_DROP_DOWN) != 0) {
		rbBand.fStyle |= RBBS_USECHEVRON;
	}
	WCHAR lpText[2];
	lpText[0] = 0;
	rbBand.lpText = lpText;
	rbBand.wID = 0;

	/*
	 * Feature in Windows.  When inserting an item at end of a row,
	 * sometimes, Windows will begin to place the item on the right
	 * side of the cool bar.  The fix is to resize the new items to
	 * the maximum size and then resize the next to last item to the
	 * ideal size.
	 */
	/*int lastIndex = getLastIndexOfRow (index - 1);
	 wresult fixLast = index == lastIndex + 1;
	 if (fixLast) {
	 rbBand.fMask |= RBBIM_SIZE;
	 rbBand.cx = MAX_WIDTH;
	 }*/

	/*
	 * Feature in Windows. Is possible that the item at index zero
	 * has the RBBS_BREAK flag set. When a new item is inserted at
	 * position zero, the previous item at position zero moves to
	 * a new line.  The fix is to detect this case and clear the
	 * RBBS_BREAK flag on the previous item before inserting the
	 * new item.
	 */
	if (index == 0 && count > 0) {
		//getItem (0).setWrap (false);
	}

	/* Insert the item */
	if (SendMessageW(_W_WIDGET(coolbar)->handle, RB_INSERTBAND, index,
			(LPARAM) &rbBand) == 0) {
		return W_ERROR_ITEM_NOT_ADDED;
	}

	/* Resize the next to last item to the ideal size */
	/*if (fixLast) {
	 resizeToPreferredWidth (lastIndex);
	 }*/
	if (item != 0) {
		_W_COOLITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_COOLBAR_GET_CLASS(coolbar)->class_coolbaritem);
		_W_COOLITEM(item)->coolbar = coolbar;
		_W_COOLITEM(item)->index = index;
	}
	return TRUE;
}
wresult _w_coolbar_set_locked(w_coolbar *coolbar, wresult locked) {
}
int _w_coolbar_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	HWND handle = _W_WIDGET(widget)->handle;
	int width = 0, height = 0;
	int border = _w_control_get_border_width(W_CONTROL(widget));
	int newWidth = e->wHint == W_DEFAULT ? 0x3FFF : e->wHint + (border * 2);
	int newHeight = e->hHint == W_DEFAULT ? 0x3FFF : e->hHint + (border * 2);
	int count = (int) SendMessageW(handle, RB_GETBANDCOUNT, 0, 0);
	if (count != 0) {
		//ignoreResize = true;
		_W_WIDGET(widget)->state |= STATE_IGNORE_RESIZE;
		boolean redraw = FALSE;
		if (IsWindowVisible(handle)) {
			if (W_COMCTL32_VERSION >= VERSION(6, 0)) {
				redraw = TRUE;
				UpdateWindow(handle);
				DefWindowProcW(handle, WM_SETREDRAW, 0, 0);
			} else {
				redraw = _w_control_get_drawing(W_CONTROL(widget));
				if (redraw) {
					UpdateWindow(handle);
					SendMessageW(handle, WM_SETREDRAW, 0, 0);
				}
			}
		}
		RECT oldRect;
		GetWindowRect(handle, &oldRect);
		int oldWidth = oldRect.right - oldRect.left;
		int oldHeight = oldRect.bottom - oldRect.top;
		int flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER;
		SetWindowPos(handle, 0, 0, 0, newWidth, newHeight, flags);
		RECT rect;
		SendMessageW(handle, RB_GETRECT, count - 1, (LPARAM) &rect);
		height = w_max(height, rect.bottom);
		SetWindowPos(handle, 0, 0, 0, oldWidth, oldHeight, flags);
		REBARBANDINFOW rbBand;
		rbBand.cbSize = sizeof(REBARBANDINFOW);
		rbBand.fMask = RBBIM_IDEALSIZE | RBBIM_STYLE;
		int rowWidth = 0;
		for (int i = 0; i < count; i++) {
			SendMessageW(handle, RB_GETBANDINFO, i, (LPARAM) &rbBand);
			if ((rbBand.fStyle & RBBS_BREAK) != 0) {
				width = w_max(width, rowWidth);
				rowWidth = 0;
			}
			rowWidth += rbBand.cxIdeal
					+ _w_coolitem_get_margin(W_COOLBAR(widget), i);
		}
		width = w_max(width, rowWidth);
		if (redraw) {
			if (W_COMCTL32_VERSION >= VERSION(6, 0)) {
				DefWindowProcW(handle, WM_SETREDRAW, 1, 0);
			} else {
				SendMessageW(handle, WM_SETREDRAW, 1, 0);
			}
		}
		//ignoreResize = false;
		_W_WIDGET(widget)->state &= ~STATE_IGNORE_RESIZE;
	}
	if (width == 0)
		width = DEFAULT_COOLBAR_WIDTH;
	if (height == 0)
		height = DEFAULT_COOLBAR_HEIGHT;
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		int tmp = width;
		width = height;
		height = tmp;
	}
	if (e->wHint != W_DEFAULT)
		width = e->wHint;
	if (e->hHint != W_DEFAULT)
		height = e->hHint;
	height += border * 2;
	width += border * 2;
	e->size->width = width;
	e->size->height = height;
	return TRUE;
}
void _w_coolbar_class_init(struct _w_coolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_coolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_coolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_coolbar);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_coolbar_create;
	clazz->get_item = _w_coolbar_get_item;
	clazz->get_item_count = _w_coolbar_get_item_count;
	clazz->get_items = _w_coolbar_get_items;
	clazz->get_locked = _w_coolbar_get_locked;
	clazz->insert_item = _w_coolbar_insert_item;
	clazz->set_locked = _w_coolbar_set_locked;

	struct _w_coolitem_class *class_item = clazz->class_coolbaritem;
	W_WIDGETDATA_CLASS(class_item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	W_WIDGETDATA_CLASS(class_item)->close = _w_widgetdata_close;
	class_item->compute_size = _w_coolitem_compute_size;
	class_item->get_bounds = _w_coolitem_get_bounds;
	class_item->get_control = _w_coolitem_get_control;
	class_item->get_minimum_size = _w_coolitem_get_minimum_size;
	class_item->get_order = _w_coolitem_get_order;
	class_item->get_preferred_size = _w_coolitem_get_preferred_size;
	class_item->get_size = _w_coolitem_get_size;
	class_item->get_wrap_indice = _w_coolitem_get_wrap_indice;
	class_item->set_control = _w_coolitem_set_control;
	class_item->set_layout = _w_coolitem_set_layout;
	class_item->set_minimum_size = _w_coolitem_set_minimum_size;
	class_item->set_order = _w_coolitem_set_order;
	class_item->set_preferred_size = _w_coolitem_set_preferred_size;
	class_item->set_size = _w_coolitem_set_size;
	class_item->set_wrap_indice = _w_coolitem_set_wrap_indice;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->compute_size = _w_coolbar_compute_size;
	//messages
	struct _w_control_reserved *msg = reserved;
}
#endif
