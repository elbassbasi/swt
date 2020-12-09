/*
 * toolbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "toolbar.h"
#include "../widgets/toolkit.h"
/*
 * private
 */
int _w_toolbar_check_style(int style) {
	/*
	 * On Windows, only flat tool bars can be traversed.
	 */
	if ((style & W_FLAT) == 0)
		style |= W_NO_FOCUS;

	/*
	 * A vertical tool bar cannot wrap because TB_SETROWS
	 * fails when the toolbar has TBSTYLE_WRAPABLE.
	 */
	if ((style & W_VERTICAL) != 0)
		style &= ~W_WRAP;

	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
void _w_toolbar_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= CCS_NORESIZE | TBSTYLE_TOOLTIPS | TBSTYLE_CUSTOMERASE;
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed())
		*dwStyle |= TBSTYLE_TRANSPARENT;
	if ((style & W_SHADOW_OUT) == 0)
		*dwStyle |= CCS_NODIVIDER;
	if ((style & W_WRAP) != 0)
		*dwStyle |= TBSTYLE_WRAPABLE;
	if ((style & W_FLAT) != 0)
		*dwStyle |= TBSTYLE_FLAT;
	/*
	 * Feature in Windows.  When a tool bar has the style
	 * TBSTYLE_LIST and has a drop down item, Window leaves
	 * too much padding around the button.  This affects
	 * every button in the tool bar and makes the preferred
	 * height too big.  The fix is to set the TBSTYLE_LIST
	 * when the tool bar contains both text and images.
	 *
	 * NOTE: Tool bars with CCS_VERT must have TBSTYLE_LIST
	 * set before any item is added or the tool bar does
	 * not lay out properly.  The work around does not run
	 * in this case.
	 */
	if (W_COMCTL32_VERSION < VERSION(6, 0) || !IsAppThemed()) {
		if ((style & W_RIGHT) != 0)
			*dwStyle |= TBSTYLE_LIST;
	}
}
w_imagelist* _w_toolbar_get_imagelist(w_toolbar *toolbar) {
	return _W_TOOLBAR(toolbar)->imagelist;
}
wresult _w_toolbar_get_item(w_toolbar *toolbar, int index, w_toolitem *item) {
}
wresult _w_toolbar_get_item_from_point(w_toolbar *toolbar, w_point *point,
		w_toolitem *item) {
}
int _w_toolbar_get_item_count(w_toolbar *toolbar) {
}
void _w_toolbar_get_items(w_toolbar *toolbar, w_iterator *items) {
}
int _w_toolbar_get_row_count(w_toolbar *toolbar) {
}
wresult _w_toolbar_set_imagelist(w_toolbar *toolbar, w_imagelist *imagelist) {
	wresult result = TRUE;
	HIMAGELIST _imagelist = 0;
	if (imagelist == 0) {
		_imagelist = 0;
		_W_TOOLBAR(toolbar)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->imagelist == 0) {
			_imagelist = 0;
			_W_TOOLBAR(toolbar)->imagelist = 0;
			result = W_ERROR_INVALID_ARGUMENT;
		} else {
			_imagelist = _W_IMAGELIST(imagelist)->imagelist;
			_W_TOOLBAR(toolbar)->imagelist = imagelist;
		}
	}
	SendMessageW(_W_WIDGET(toolbar)->handle, TB_SETIMAGELIST, 0,
			(LPARAM) _imagelist);
	return result;
}
int _w_toolbar_widget_style(int style) {
	if ((style & W_DROP_DOWN) != 0)
		return BTNS_DROPDOWN;
	if ((style & W_PUSH) != 0)
		return BTNS_BUTTON;
	if ((style & W_CHECK) != 0)
		return BTNS_CHECK;
	/*
	 * This code is intentionally commented.  In order to
	 * consistently support radio tool items across platforms,
	 * the platform radio behavior is not used.
	 */
//	if ((style & W_RADIO) != 0) return BTNS_CHECKGROUP;
	if ((style & W_RADIO) != 0)
		return BTNS_CHECK;
	if ((style & W_SEPARATOR) != 0)
		return BTNS_SEP;
	return BTNS_BUTTON;
}
void _w_toolbar_set_drop_down_items(w_toolbar *toolbar, wresult set) {
	HWND handle = _W_WIDGET(toolbar)->handle;
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	TBBUTTONINFOW info;
	WCHAR tmp[3];
	info.cbSize = sizeof(TBBUTTONINFOW);
	/*
	 * Feature in Windows.  When the first button in a tool bar
	 * is a drop down item, Window leaves too much padding around
	 * the button.  This affects every button in the tool bar and
	 * makes the preferred height too big.  The fix is clear the
	 * BTNS_DROPDOWN before Windows lays out the tool bar and set
	 * the bit afterwards.
	 *
	 * NOTE:  This work around only runs when the tool bar contains
	 * only images.
	 */
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		wresult hasText = FALSE, hasImage = FALSE;
		for (int i = 0; i < count; i++) {
			info.dwMask = TBIF_IMAGE | TBIF_TEXT | TBIF_BYINDEX;
			info.iImage = 0;
			info.pszText = tmp;
			info.cchText = 2;
			tmp[0] = 0;
			if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
					!= 0) {
				if (!hasText) {
					hasText = tmp[0] != 0;
				}
				if (!hasImage)
					hasImage = info.iImage > 0;
				if (hasText && hasImage)
					break;
			}
		}
		if (hasImage && !hasText) {
			for (int i = 0; i < count; i++) {
				info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
						!= 0) {
					if ((info.fsStyle & BTNS_DROPDOWN) != 0) {
						if (set) {
							info.fsStyle |= BTNS_DROPDOWN;
						} else {
							info.fsStyle &= ~BTNS_DROPDOWN;
						}
						SendMessageW(handle, TB_SETBUTTONINFO, i,
								(LPARAM) &info);
					}
				}
			}
		}
	}
}
void _w_toolbar_layout_items(w_toolbar *toolbar) {
	HWND handle = _W_WIDGET(toolbar)->handle;
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	TBBUTTONINFOW info;
	WCHAR tmp[3];
	/*
	 * Feature in Windows.  When a tool bar has the style
	 * TBSTYLE_LIST and has a drop down item, Window leaves
	 * too much padding around the button.  This affects
	 * every button in the tool bar and makes the preferred
	 * height too big.  The fix is to set the TBSTYLE_LIST
	 * when the tool bar contains both text and images.
	 *
	 * NOTE: Tool bars with CCS_VERT must have TBSTYLE_LIST
	 * set before any item is added or the tool bar does
	 * not lay out properly.  The work around does not run
	 * in this case.
	 */
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		if ((_W_WIDGET(toolbar)->style & W_RIGHT) != 0
				&& (_W_WIDGET(toolbar)->style & W_VERTICAL) == 0) {
			wresult hasText = FALSE, hasImage = FALSE;
			for (int i = 0; i < count; i++) {
				info.cbSize = sizeof(TBBUTTONINFOW);
				info.dwMask = TBIF_IMAGE | TBIF_TEXT | TBIF_BYINDEX;
				info.iImage = 0;
				info.pszText = tmp;
				info.cchText = 2;
				tmp[0] = 0;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
						!= 0) {
					if (!hasText) {
						hasText = tmp[0] != 0;
					}
					if (!hasImage)
						hasImage = info.iImage > 0;
					if (hasText && hasImage)
						break;
				}
			}
			int oldBits = GetWindowLongW(handle, GWL_STYLE), newBits = oldBits;
			if (hasText && hasImage) {
				newBits |= TBSTYLE_LIST;
			} else {
				newBits &= ~TBSTYLE_LIST;
			}
			if (newBits != oldBits) {
				_w_toolbar_set_drop_down_items(toolbar, FALSE);
				SetWindowLongW(handle, GWL_STYLE, newBits);
				/*
				 * Feature in Windows.  For some reason, when the style
				 * is changed to TBSTYLE_LIST, Windows does not lay out
				 * the tool items.  The fix is to use WM_SETFONT to force
				 * the tool bar to redraw and lay out.
				 */
				HFONT hFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
				SendMessageW(handle, WM_SETFONT, (WPARAM) hFont, 0);
				_w_toolbar_set_drop_down_items(toolbar, TRUE);
			}
		}
	}

	if ((_W_WIDGET(toolbar)->style & W_WRAP) != 0) {
		SendMessageW(handle, TB_AUTOSIZE, 0, 0);
	}
	/*
	 *  When the tool bar is vertical, make the width of each button
	 *  be the width of the widest button in the tool bar.  Note that
	 *  when the tool bar contains a drop down item, it needs to take
	 *  into account extra padding.
	 */
	if ((_W_WIDGET(toolbar)->style & W_VERTICAL) != 0) {
		if (count > 1) {
			info.cbSize = sizeof(TBBUTTONINFOW);
			info.dwMask = TBIF_SIZE | TBIF_BYINDEX;
			LRESULT size = SendMessageW(handle, TB_GETBUTTONSIZE, 0, 0);
			int info_cx = LOWORD(size);
			int index = 0, extraPadding = 0;
			while (index < count) {
				info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, index,
						(LPARAM) &info) != 0) {
					if ((info.fsStyle & BTNS_DROPDOWN) != 0) {
						/*
						 * Specifying 1 pixel extra padding to avoid truncation
						 * of widest item in the tool-bar when a tool-bar has
						 * SWT.VERTICAL style and any of the items in the
						 * tool-bar has SWT.DROP_DOWN style, Refer bug#437206
						 */
						extraPadding = 1;
						break;
					}
				}
				index++;
			}
			if (index < count) {
				LRESULT padding = SendMessageW(handle, TB_GETPADDING, 0, 0);
				info_cx += LOWORD (padding + extraPadding) * 2;
			}
			for (int i = 0; i < count; i++) {
				info.dwMask = TBIF_STYLE | TBIF_BYINDEX;
				if (SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info)
						!= 0) {
					if ((info.fsStyle & BTNS_SEP) == 0) {
						info.dwMask = TBIF_SIZE;
						info.cx = info_cx;
						SendMessageW(handle, TB_SETBUTTONINFOW, i,
								(LPARAM) &info);
					}
				}
			}
		}
	}

	/*
	 * Feature on Windows. When SWT.WRAP or SWT.VERTICAL are set
	 * the separator items with control are implemented using BTNS_BUTTON
	 * instead of BTNS_SEP. When that is the case and TBSTYLE_LIST is
	 * set, the layout of the ToolBar recalculates the width for all
	 * BTNS_BUTTON based on the text and bitmap of the item.
	 * This is not strictly wrong, but the user defined width for the
	 * separators has to be respected if set.
	 * The fix is to detect this case and reset the cx width for the item.
	 */
	if ((_W_WIDGET(toolbar)->style & (W_WRAP | W_VERTICAL)) != 0) {
		int bits = GetWindowLongW(handle, GWL_STYLE);
		if ((bits & TBSTYLE_LIST) != 0) {
			info.cbSize = sizeof(TBBUTTONINFOW);
			info.dwMask = TBIF_SIZE;
			for (int i = 0; i < count; i++) {
				/*ToolItem item = items[i];
				 if (item != null && item.cx > 0) {
				 info.cx = item.cx;
				 SendMessageW(handle, TB_SETBUTTONINFOW, i,(LPARAM) &info);
				 }*/
			}
		}
	}

	/*for (int i = 0; i < count; i++) {
	 ToolItem item = items[i];
	 if (item != null)
	 item.resizeControl();
	 }*/
}
void _w_toolbar_set_row_count(w_toolbar *toolbar, int count) {
	if ((_W_WIDGET(toolbar)->style & W_VERTICAL) != 0) {
		HWND handle = _W_WIDGET(toolbar)->handle;
		/*
		 * Feature in Windows.  When the TB_SETROWS is used to set the
		 * number of rows in a tool bar, the tool bar is resized to show
		 * the items.  This is unexpected.  The fix is to save and restore
		 * the current size of the tool bar.
		 */
		RECT rect;
		GetWindowRect(handle, &rect);
		w_composite *parent;
		w_control_get_parent(W_CONTROL(toolbar),&parent);
		MapWindowPoints(0, _W_WIDGET(parent)->handle, (LPPOINT) &rect, 2);
		_W_WIDGET(toolbar)->state |= STATE_IGNORE_RESIZE;
		//ignoreResize = true;
		/*
		 * Feature in Windows.  When the last button in a tool bar has the
		 * style BTNS_SEP and TB_SETROWS is used to set the number of rows
		 * in the tool bar, depending on the number of buttons, the toolbar
		 * will wrap items with the style BTNS_CHECK, even when the fLarger
		 * flags is used to force the number of rows to be larger than the
		 * number of items.  The fix is to set the number of rows to be two
		 * larger than the actual number of rows in the tool bar.  When items
		 * are being added, as long as the number of rows is at least one
		 * item larger than the count, the tool bar is laid out properly.
		 * When items are being removed, setting the number of rows to be
		 * one more than the item count has no effect.  The number of rows
		 * is already one more causing TB_SETROWS to do nothing.  Therefore,
		 * choosing two instead of one as the row increment fixes both cases.
		 */
		count += 2;
		SendMessageW(handle, TB_SETROWS, MAKEWPARAM(count, 1), 0);
		int flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER;
		SetWindowPos(handle, 0, 0, 0, rect.right - rect.left,
				rect.bottom - rect.top, flags);
		//ignoreResize = false;
		_W_WIDGET(toolbar)->state &= ~STATE_IGNORE_RESIZE;
	}
}
wresult _w_toolbar_insert(w_toolbar *toolbar, w_toolitem *item,
		const char *text, int style, int index) {
	int count = (int) SendMessageW(_W_WIDGET(toolbar)->handle, TB_BUTTONCOUNT,
			0, 0);
	if (!(0 <= index && index <= count)) {
		index = count;
		//return W_ERROR_INVALID_RANGE;
	}
	int bits = _w_toolbar_widget_style(style);
	TBBUTTON lpButton;
	WCHAR *t = 0;
	memset(&lpButton, 0, sizeof(lpButton));
	lpButton.idCommand = 1;
	lpButton.fsStyle = (byte) bits;
	lpButton.fsState = (byte) TBSTATE_ENABLED;
	int l, _l;
	if (text != 0) {
		l = strlen(text);
		_l = w_utf8_to_utf16(text, l, 0, 0);
		t = _w_toolkit_malloc((_l + 1) * sizeof(WCHAR));
		w_utf8_to_utf16(text, l, t, _l + 1);
		lpButton.iString = (INT_PTR) t;
		lpButton.fsStyle |= BTNS_SHOWTEXT;
	}
	/*
	 * Bug in Windows.  Despite the fact that the image list
	 * index has never been set for the item, Windows always
	 * assumes that the image index for the item is valid.
	 * When an item is inserted, the image index is zero.
	 * Therefore, when the first image is inserted and is
	 * assigned image index zero, every item draws with this
	 * image.  The fix is to set the image index to none
	 * when the item is created.  This is not necessary in
	 * the case when the item has the BTNS_SEP style because
	 * separators cannot show images.
	 */
	if ((bits & BTNS_SEP) == 0)
		lpButton.iBitmap = I_IMAGENONE;
	LRESULT result = SendMessageW(_W_WIDGET(toolbar)->handle, TB_INSERTBUTTONW,
			index, (LPARAM) &lpButton);
	if (t != 0)
		_w_toolkit_free(t,(_l + 1) * sizeof(WCHAR));
	if (result == 0) {
		return W_ERROR_ITEM_NOT_ADDED;
	}
	if (item != 0) {
		_W_TOOLITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TOOLBAR_GET_CLASS(toolbar)->class_toolbaritem);
		_W_TOOLITEM(item)->toolbar = toolbar;
		_W_TOOLITEM(item)->index = index;
	}
	if ((style & W_VERTICAL) != 0)
		_w_toolbar_set_row_count(toolbar, count + 1);
	_w_toolbar_layout_items(toolbar);
	return TRUE;
}
wresult _w_toolitem_get_bounds(w_toolitem *item, w_rect *rect) {
	w_toolbar *toolbar = _W_TOOLITEM(item)->toolbar;
	int index = _W_TOOLITEM(item)->index;
	RECT r;
	SendMessageW(_W_WIDGET(toolbar)->handle, TB_GETITEMRECT, index,
			(LPARAM) &r);
	rect->x = r.left;
	rect->y = r.top;
	rect->width = r.right - r.left;
	rect->height = r.bottom - r.top;
	return TRUE;
}
w_control* _w_toolitem_get_control(w_toolitem *item) {
}
wresult _w_toolitem_get_enabled(w_toolitem *item) {
}
wresult _w_toolitem_get_selection(w_toolitem *item) {
}
wresult _w_toolitem_get_tooltip_text(w_toolitem *item, w_alloc *text) {
}
int _w_toolitem_get_width(w_toolitem *item) {
}
wresult _w_toolitem_is_enabled(w_toolitem *item) {
}
wresult _w_toolitem_set_control(w_toolitem *item, w_control *control) {
}
wresult _w_toolitem_set_enabled(w_toolitem *item, wresult enabled) {
}
wresult _w_toolitem_set_image(w_toolitem *item, int image) {
	w_toolbar *parent = _W_TOOLITEM(item)->toolbar;
	int index = _W_TOOLITEM(item)->index;
	TBBUTTONINFOW info;
	info.cbSize = sizeof(TBBUTTONINFOW);
	info.dwMask = TBIF_IMAGE | TBIF_BYINDEX;
	info.iImage = image;
	SendMessageW(_W_WIDGET(parent)->handle, TB_SETBUTTONINFOW, index,
			(LPARAM) &info);
	return TRUE;
}
wresult _w_toolitem_set_selection(w_toolitem *item, wresult selected) {
}
wresult _w_toolitem_set_tooltip_text(w_toolitem *item, const char *string) {
}
wresult _w_toolitem_set_width(w_toolitem *item, int width) {
}

wresult _w_toolbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_toolbar_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_toolbar_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	TOOLBARCLASSNAMEA, dwExStyle, dwStyle);
	if (result > 0) {
		_W_WIDGET(widget)->state &= ~STATE_CANVAS;

		/*
		 * Feature in Windows.  When TBSTYLE_FLAT is used to create
		 * a flat toolbar, for some reason TBSTYLE_TRANSPARENT is
		 * also set.  This causes the toolbar to flicker when it is
		 * moved or resized.  The fix is to clear TBSTYLE_TRANSPARENT.
		 *
		 * NOTE:  This work around is unnecessary on XP.  There is no
		 * flickering and clearing the TBSTYLE_TRANSPARENT interferes
		 * with the XP theme.
		 */
		if ((style & W_FLAT) != 0) {
			if (W_COMCTL32_VERSION < VERSION(6, 0) || !IsAppThemed()) {
				int bits = GetWindowLongW(_W_WIDGET(widget)->handle,
				GWL_STYLE);
				bits &= ~TBSTYLE_TRANSPARENT;
				SetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE, bits);
			}
		}

		/*
		 * Feature in Windows.  Despite the fact that the
		 * tool tip text contains \r\n, the tooltip will
		 * not honour the new line unless TTM_SETMAXTIPWIDTH
		 * is set.  The fix is to set TTM_SETMAXTIPWIDTH to
		 * a large value.
		 */
		/*
		 * These lines are intentionally commented.  The tool
		 * bar currently sets this value to 300 so it is not
		 * necessary to set TTM_SETMAXTIPWIDTH.
		 */
		//	long /*int*/ hwndToolTip = OS.SendMessage (handle, OS.TB_GETTOOLTIPS, 0, 0);
		//	OS.SendMessage (hwndToolTip, OS.TTM_SETMAXTIPWIDTH, 0, 0x7FFF);
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

		/* Set the button struct, bitmap and button sizes */
		SendMessageW(_W_WIDGET(widget)->handle, TB_BUTTONSTRUCTSIZE,
				sizeof(TBBUTTON), 0);
		SendMessageW(_W_WIDGET(widget)->handle, TB_SETBITMAPSIZE, 0, 0);
		SendMessageW(_W_WIDGET(widget)->handle, TB_SETBUTTONSIZE, 0, 0);

		/* Set the extended style bits */
		int bits = TBSTYLE_EX_DRAWDDARROWS | TBSTYLE_EX_MIXEDBUTTONS
				| TBSTYLE_EX_HIDECLIPPEDBUTTONS;
		if (W_COMCTL32_VERSION >= VERSION(6, 0))
			bits |= TBSTYLE_EX_DOUBLEBUFFER;
		SendMessageW(_W_WIDGET(widget)->handle, TB_SETEXTENDEDSTYLE, 0, bits);
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
			/*
			 * Feature in Windows.  When a tool bar has the style
			 * TBSTYLE_LIST and has a drop down item, Window leaves
			 * too much padding around the button.  This affects
			 * every button in the tool bar and makes the preferred
			 * height too big.  The fix is to set the TBSTYLE_LIST
			 * when the tool bar contains both text and images.
			 *
			 * NOTE: Tool bars with CCS_VERT must have TBSTYLE_LIST
			 * set before any item is added or the tool bar does
			 * not lay out properly.  The work around does not run
			 * in this case.
			 */
			if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
				if ((style & W_RIGHT) != 0)
					bits |= TBSTYLE_LIST;
			}
			SetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE,
					bits | CCS_VERT);
		} else {
			_W_WIDGET(widget)->style |= W_HORIZONTAL;
		}
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
	}
	return result;
}
int _w_toolbar_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved) {
	int ret = _w_scrollable_compute_trim(widget, e, reserved);
	int bits = GetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE);
	if ((bits & CCS_NODIVIDER) == 0) {
		e->result->height += 2;
	}
	return ret;
}
int _w_toolbar_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	HWND handle = _W_WIDGET(widget)->handle;
	RECT rect, oldRect;
	TBBUTTONINFOW info;
	int count = (int) SendMessageW(handle, TB_BUTTONCOUNT, 0, 0);
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		for (int i = 0; i < count; i++) {
			SendMessageW(handle, TB_GETITEMRECT, i, (LPARAM) &rect);
			height = w_max(height, rect.bottom);
			info.cbSize = sizeof(TBBUTTONINFOW);
			info.dwMask = TBIF_SIZE | TBIF_BYINDEX | TBIF_STYLE;
			SendMessageW(handle, TB_GETBUTTONINFOW, i, (LPARAM) &info);
			if ((info.fsStyle & BTNS_SEP) != 0) {
				width = w_max(width, info.cx);
			} else {
				width = w_max(width, rect.right);
			}
		}
	} else {
		GetWindowRect(handle, &oldRect);
		int oldWidth = oldRect.right - oldRect.left;
		int oldHeight = oldRect.bottom - oldRect.top;
		int border = _w_control_get_border_width(W_CONTROL(widget));
		int newWidth = wHint == W_DEFAULT ? 0x3FFF : wHint + border * 2;
		int newHeight = hHint == W_DEFAULT ? 0x3FFF : hHint + border * 2;
		wresult redraw = _w_control_get_drawing(W_CONTROL(widget))
				&& IsWindowVisible(handle);
		//ignoreResize = true;
		_W_WIDGET(widget)->state |= STATE_IGNORE_RESIZE;
		if (redraw)
			UpdateWindow(handle);
		int flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOREDRAW | SWP_NOZORDER;
		SetWindowPos(handle, 0, 0, 0, newWidth, newHeight, flags);
		if (count != 0) {
			SendMessageW(handle, TB_GETITEMRECT, count - 1, (LPARAM) &rect);
			width = w_max(width, rect.right);
			height = w_max(height, rect.bottom);
		}
		SetWindowPos(handle, 0, 0, 0, oldWidth, oldHeight, flags);
		if (redraw)
			ValidateRect(handle, NULL);
		//ignoreResize = false;
		_W_WIDGET(widget)->state &= ~STATE_IGNORE_RESIZE;
	}

	/*
	 * From the Windows SDK for TB_SETBUTTONSIZE:
	 *
	 *   "If an application does not explicitly
	 *	set the button size, the size defaults
	 *	to 24 by 22 pixels".
	 */
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	w_event_compute_trim ee;
	w_rect r, trim;
	ee.rect = &r;
	ee.result = &trim;
	r.x = 0;
	r.y = 0;
	r.width = width;
	r.height = height;
	_w_toolbar_compute_trim(widget, &ee, reserved);
	width = trim.width;
	height = trim.height;
	e->size->width = width;
	e->size->height = height;
	return TRUE;
}
void _w_toolbar_class_init(struct _w_toolbar_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TOOLBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_toolbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_toolbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_toolbar);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_toolbar_create;
	clazz->insert = _w_toolbar_insert;
	clazz->get_imagelist = _w_toolbar_get_imagelist;
	clazz->get_item = _w_toolbar_get_item;
	clazz->get_item_from_point = _w_toolbar_get_item_from_point;
	clazz->get_item_count = _w_toolbar_get_item_count;
	clazz->get_items = _w_toolbar_get_items;
	clazz->set_imagelist = _w_toolbar_set_imagelist;
	clazz->get_row_count = _w_toolbar_get_row_count;
	/*
	 *
	 */
	struct _w_toolitem_class *class_item = clazz->class_toolbaritem;
	W_WIDGETDATA_CLASS(class_item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	W_WIDGETDATA_CLASS(class_item)->close = _w_widgetdata_close;
	class_item->get_bounds = _w_toolitem_get_bounds;
	class_item->get_control = _w_toolitem_get_control;
	class_item->get_enabled = _w_toolitem_get_enabled;
	class_item->get_selection = _w_toolitem_get_selection;
	class_item->get_tooltip_text = _w_toolitem_get_tooltip_text;
	class_item->get_width = _w_toolitem_get_width;
	class_item->is_enabled = _w_toolitem_is_enabled;
	class_item->set_control = _w_toolitem_set_control;
	class_item->set_enabled = _w_toolitem_set_enabled;
	class_item->set_image = _w_toolitem_set_image;
	class_item->set_selection = _w_toolitem_set_selection;
	class_item->set_tooltip_text = _w_toolitem_set_tooltip_text;
	class_item->set_width = _w_toolitem_set_width;

	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->compute_size = _w_toolbar_compute_size;
	reserved->compute_trim = _w_toolbar_compute_trim;
	//messages
	struct _w_control_reserved *msg = reserved;
}
#endif
