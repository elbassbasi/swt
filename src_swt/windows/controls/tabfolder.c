/*
 * tabview.c
 *
 *  Created on: 4 mai 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "tabfolder.h"
#include "../widgets/toolkit.h"
/*
 * private
 */
int _w_tabfolder_check_style(int style) {
	/*
	 * When the SWT.TOP style has not been set, force the
	 * tabs to be on the bottom for tab folders on PPC.
	 */
	/*if (IsPPC) {
	 if ((style & W_TOP) == 0) style |= W_BOTTOM;
	 }*/
	style = _w_widget_check_bits(style, W_TOP, W_BOTTOM, W_LEFT, W_RIGHT, 0, 0);

	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
void _w_tabfolder_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= WS_CLIPCHILDREN;
	if ((style & W_NO_FOCUS) != 0)
		*dwStyle |= TCS_FOCUSNEVER;
	if ((style & W_BOTTOM) != 0)
		*dwStyle |= TCS_BOTTOM;
	if ((style & W_LEFT) != 0)
		*dwStyle |= TCS_VERTICAL;
	if ((style & W_RIGHT) != 0)
		*dwStyle |= TCS_VERTICAL | TCS_RIGHT;
	if (style & W_CLOSE) {
		*dwStyle |= TCS_OWNERDRAWFIXED;
		//*dwStyle &= ~WS_CLIPCHILDREN;
	}
	*dwStyle |= TCS_TABS | TCS_TOOLTIPS;
	*dwExStyle |= TCS_EX_REGISTERDROP;
}
/*
 * tabitem
 */
void _w_tabitem_copy(w_widgetdata *from, w_widgetdata *to) {
}
wresult _w_tabitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
}
w_widget* _w_tabitem_get_parent_widget(w_item *item) {
	return W_WIDGET(_W_TABITEM(item)->tabfolder);
}
void* _w_tabitem_get_data(w_item *item) {
	HWND handle = _W_WIDGET(_W_TABITEM(item)->tabfolder)->handle;
	int index = _W_TABITEM(item)->index;
	TCITEMW tcItem;
	tcItem.mask = TCIF_PARAM;
	tcItem.lParam = (LPARAM) 0;
	SendMessageW(handle,
	TCM_GETITEMW, index, (LPARAM) &tcItem);
	return (void*) tcItem.lParam;
}
int _w_tabitem_get_index(w_item *item) {
}
wresult _w_tabitem_get_text(w_item *item, w_alloc *text) {
}
wresult _w_tabitem_set_data(w_item *item, void *data) {
	HWND handle = _W_WIDGET(_W_TABITEM(item)->tabfolder)->handle;
	int index = _W_TABITEM(item)->index;
	TCITEMW tcItem;
	tcItem.mask = TCIF_PARAM;
	tcItem.lParam = (LPARAM) data;
	SendMessageW(handle,
	TCM_SETITEMW, index, (LPARAM) &tcItem);
	return W_TRUE;
}
wresult _w_tabitem_set_text(w_item *item, const char *string) {
}
wresult _w_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
}
int _w_tabitem_get_image(w_tabitem *item) {
}
wresult _w_tabitem_set_image(w_tabitem *item, int image) {
	HWND handle = _W_WIDGET(_W_TABITEM(item)->tabfolder)->handle;
	int index = _W_TABITEM(item)->index;
	TCITEMW tcItem;
	tcItem.mask = TCIF_IMAGE;
	tcItem.iImage = image;
	SendMessageW(handle,
	TCM_SETITEMW, index, (LPARAM) &tcItem);
	return W_TRUE;
}
wresult _w_tabitem_pack(w_tabitem *item, w_control *control) {
	HWND handle = _W_WIDGET(_W_TABITEM(item)->tabfolder)->handle;
	int index = _W_TABITEM(item)->index;
	int selectionIndex = SendMessageW(handle,
	TCM_GETCURSEL, 0, 0);
	if (index == selectionIndex) {
		w_rect r;
		w_scrollable_get_client_area(W_SCROLLABLE(_W_TABITEM(item)->tabfolder),
				&r);
		w_control_set_bounds(control, &r.pt,&r.sz);
		w_control_set_visible(control, W_TRUE);
	}
	return W_TRUE;
}
/*
 * tabfolder
 */

w_imagelist* _w_tabfolder_get_imagelist(w_tabfolder *tabfolder) {

}
wresult _w_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item) {
}
wresult _w_tabfolder_get_item_p(w_tabfolder *tabfolder, w_point *point,
		w_tabitem *item) {
}
int _w_tabfolder_get_item_count(w_tabfolder *tabfolder) {
}
void _w_tabfolder_get_items(w_tabfolder *tabfolder, w_iterator *items) {
}
wresult _w_tabfolder_get_selection(w_tabfolder *tabfolder, w_tabitem *item) {
}
wresult _w_tabfolder_insert_item(w_tabfolder *tabfolder, w_tabitem *item,
		const char *text, int index) {
	int count = SendMessageW(_W_WIDGET(tabfolder)->handle, TCM_GETITEMCOUNT, 0,
			0);
	if (index >= 0) {
		if (index > count)
			return W_ERROR_INVALID_RANGE;
	} else {
		index = count;
	}
	if (text == 0)
		text = "";
	struct UnicodeString str;
	TCITEMW tcItem;
	tcItem.mask = TCIF_TEXT;
	unicode_set(&str, text, -1);
	tcItem.pszText = str.str;
	tcItem.cchTextMax = str.length;
	int i = SendMessageW(_W_WIDGET(tabfolder)->handle, TCM_INSERTITEMW, index,
			(LPARAM) &tcItem);
	unicode_free(&str);
	if (i == -1) {
		return W_ERROR_ITEM_NOT_ADDED;
	}
	if (item != 0) {
		_W_TABITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TABFOLDER_GET_CLASS(tabfolder)->class_tabfolderitem);
		_W_TABITEM(item)->tabfolder = tabfolder;
		_W_TABITEM(item)->index = i;
	}
	if (_W_WIDGET(tabfolder)->style & W_CLOSE) {
		/*RECT r;
		if (SendMessageW(_W_CONTROL(tabfolder)->handle,
		TCM_GETITEMRECT, i, (LPARAM) &r) == TRUE) {
			int cx = r.right - r.left + W_TABITEM_CLOSE_WIDTH + 30;
			int cy = r.bottom - r.top;
			SendMessageW(_W_CONTROL(tabfolder)->handle,
			TCM_SETITEMSIZE, i, MAKELPARAM(cx, cy));
		}*/
	}
	return W_TRUE;
}

wresult _w_tabfolder_set_imagelist(w_tabfolder *tabfolder,
		w_imagelist *imagelist) {
	wresult result = TRUE;
	HIMAGELIST _imagelist = 0;
	if (imagelist == 0) {
		_imagelist = 0;
		_W_TABFOLDER(tabfolder)->imagelist = 0;
	} else {
		if (_W_IMAGELIST(imagelist)->imagelist == 0) {
			_imagelist = 0;
			_W_TABFOLDER(tabfolder)->imagelist = 0;
			result = W_ERROR_INVALID_ARGUMENT;
		} else {
			_imagelist = _W_IMAGELIST(imagelist)->imagelist;
			_W_TABFOLDER(tabfolder)->imagelist = imagelist;
		}
	}
	SendMessageW(_W_WIDGET(tabfolder)->handle, TCM_SETIMAGELIST, 0,
			(LPARAM) _imagelist);
	return result;
}
wresult _w_tabfolder_set_selection(w_tabfolder *tabfolder, int index) {
}
wresult _w_tabfolder_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_tabfolder_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_tabfolder_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_TABCONTROLA, dwExStyle, dwStyle);
	if (result > 0) {
		w_font *systemfont = w_toolkit_get_system_font(
				(w_toolkit*) win_toolkit);
		SendMessageW(_W_WIDGET(widget)->handle, WM_SETFONT,
				(WPARAM) _W_FONT(systemfont)->handle, 0);
		if (style & W_CLOSE) {
			SendMessageW(_W_WIDGET(widget)->handle, TCM_SETPADDING, (WPARAM) 0,
			 (LPARAM) MAKELPARAM(W_TABITEM_CLOSE_WIDTH + 2, 0));
		}
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
		_W_TABFOLDER(widget)->lastindex = -1;
	}
	return result;
}
void _w_tabitem_get_close_button_rect(w_widget *widget, int iItem, RECT *rcItem,
		RECT *rcCloseButton, wresult bItemSelected, wresult bVistaThemeActive) {
	rcCloseButton->top = rcItem->top + 2
			+ ((rcItem->bottom - rcItem->top - W_TABITEM_CLOSE_HEIGTH) / 2);
	rcCloseButton->bottom = rcCloseButton->top + W_TABITEM_CLOSE_HEIGTH;
	rcCloseButton->right = rcItem->right - 2;
	rcCloseButton->left = rcCloseButton->right - W_TABITEM_CLOSE_WIDTH;
	if (bVistaThemeActive)
		rcCloseButton->left -= 1; // the close button does not look 'symetric' with a width of 16, give it 17
	if (bItemSelected) {
		OffsetRect(rcCloseButton, -1, 0);
		if (bVistaThemeActive) {
			/*int iItems = GetItemCount();
			 if (iItems > 1 && iItem == iItems - 1)
			 OffsetRect(rcCloseButton,-2, 0);*/
		}
	} else {
		if (bVistaThemeActive) {
			/*int iItems = GetItemCount();
			 if (iItems > 1 && iItem < iItems - 1)
			 OffsetRect(rcCloseButton,2, 0);*/
		}
	}
}
void _w_tabfolder_draw_close_button(HDC hdc, RECT *r, int hot) {
	POINT pt;
	COLORREF color;
	if (hot) {
		//color = RGB(0xFF, 0, 0);
		color = GetSysColor(COLOR_HIGHLIGHTTEXT);
	} else {
		//color = RGB(0, 0, 0);
		color = GetSysColor(COLOR_GRAYTEXT);
	}
	HPEN newPen = CreatePen(PS_SOLID | PS_GEOMETRIC | PS_ENDCAP_SQUARE, 2,
			color);
	HPEN lastpen = SelectObject(hdc, newPen);
	//if ((r.left < rt.right) && (r.top < rt.bottom)
	//		&& (r.right > rt.left) && (r.bottom > rt.top)) {
	/*if (hit) {
	 r2.left = r.right - 16;
	 r2.top = r.top;
	 r2.right = r.right;
	 r2.bottom = r.bottom;
	 DrawFrameControl(hdc, &r2, DFC_BUTTON, DFCS_BUTTONPUSH);
	 }*/
	//Rectangle(hdc,r->left,r->top,r->right,r->bottom);
	MoveToEx(hdc, r->left, r->top, &pt);
	LineTo(hdc, r->right, r->bottom);
	MoveToEx(hdc, r->left, r->bottom, &pt);
	LineTo(hdc, r->right, r->top);
	SelectObject(hdc, lastpen);
	DeleteObject(newPen);
}
/*
 * message
 */
int _TABFOLDER_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _COMPOSITE_WM_SIZE(widget, e, reserved);
	TCITEMW tcItem;
	int index = SendMessageW(e->hwnd, TCM_GETCURSEL, 0, 0);
	if (index != -1) {
		tcItem.mask = TCIF_PARAM;
		tcItem.lParam = 0;
		SendMessageW(e->hwnd,
		TCM_GETITEMW, index, (LPARAM) &tcItem);
		w_control *control = (w_control*) tcItem.lParam;
		if (control != 0) {
			w_rect r;
			w_scrollable_get_client_area(W_SCROLLABLE(widget), &r);
			w_control_set_bounds(control, &r.pt,&r.sz);
		}
	}
	return result;
}
int _TABFOLDER_WM_DRAWCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	reserved->window_proc(widget, e, reserved);
	if (_W_WIDGET(widget)->style & W_CLOSE) {
		int tmp_count = win_toolkit->tmp_alloc / sizeof(WCHAR);
		DRAWITEMSTRUCT *lpDIS = (DRAWITEMSTRUCT*) e->lparam;
		TC_ITEMW tci;
		tci.mask = TCIF_IMAGE | TCIF_STATE | TCIF_TEXT;
		tci.pszText = &win_toolkit->tmp[tmp_count / 2];
		tci.cchTextMax = tmp_count / 2;
		tci.dwStateMask = TCIS_HIGHLIGHTED;
		if (!SendMessageW(_W_WIDGET(widget)->handle, TCM_GETITEMW,
				lpDIS->itemID, (LPARAM) &tci)) {
			return FALSE;
		}
		_w_graphics gc;
		w_themedata data;
		w_rect bounds;
		//int nSavedDC = SaveDC(lpDIS->hDC);
		bounds.x = lpDIS->rcItem.left;
		bounds.y = lpDIS->rcItem.top;
		bounds.width = lpDIS->rcItem.right - lpDIS->rcItem.left;
		bounds.height = lpDIS->rcItem.bottom - lpDIS->rcItem.top;
		RECT rect;
		memcpy(&rect, &lpDIS->rcItem, sizeof(RECT));
		_w_control_init_graphics(W_CONTROL(widget), W_GRAPHICS(&gc), lpDIS->hDC,
				0);
		wresult bVistaHotTracked = FALSE;
		wresult bCloseable = TRUE;
		w_toolkit *toolkit = w_widget_get_toolkit(widget);
		w_theme *theme = w_toolkit_get_theme(toolkit);
		/*w_themedata_init(&data);
		w_theme_open_data(theme, W_THEME_CLASS_TABITEM, &data);*/
		if (WIN32_VERSION >= VERSION(6, 0)) {
			bVistaHotTracked = GetTextColor(lpDIS->hDC)
					== GetSysColor(COLOR_HOTLIGHT);
			if ((lpDIS->itemState & ODS_SELECTED) != 0) {
				// get the real tab item rect
				bounds.x += 1;
				bounds.width -= 3;
				bounds.height -= 1;
			} else {
				bounds.x -= 3;
				bounds.y -= 3;
				bounds.width += 3;
				bounds.height += 3;
			}
			if ((lpDIS->itemState & ODS_SELECTED) != 0) {
				data.state |= W_THEME_SELECTED;
				/*if (nTabIndex == 0) {
				 // First item
				 if (nTabIndex == GetItemCount() - 1)
				 iPartId = TABP_TOPTABITEMBOTHEDGE; // First & Last item
				 else
				 iPartId = TABP_TOPTABITEMLEFTEDGE;
				 } else if (nTabIndex == GetItemCount() - 1) {
				 // Last item
				 iPartId = TABP_TOPTABITEMRIGHTEDGE;
				 } else {
				 iPartId = TABP_TOPTABITEM;
				 }*/
			} else {
				bounds.y += 2;
				/*iStateId = bVistaHotTracked ? TIS_HOT : TIS_NORMAL;
				 if (nTabIndex == 0) {
				 // First item
				 if (nTabIndex == GetItemCount() - 1)
				 iPartId = TABP_TABITEMBOTHEDGE; // First & Last item
				 else
				 iPartId = TABP_TABITEMLEFTEDGE;
				 } else if (nTabIndex == GetItemCount() - 1) {
				 // Last item
				 iPartId = TABP_TABITEMRIGHTEDGE;
				 } else {
				 iPartId = TABP_TABITEM;
				 }*/
			}

			if ((lpDIS->itemState & ODS_HOTLIGHT) != 0)
				data.state |= W_THEME_HOT;
			if ((lpDIS->itemState & ODS_FOCUS) != 0)
				data.state |= W_THEME_FOCUSED;
			if ((lpDIS->itemState & ODS_SELECTED) != 0)
				data.state |= W_THEME_SELECTED;
			if ((lpDIS->itemState & ODS_DISABLED) != 0)
				data.state |= W_THEME_DISABLED;
			//w_themedata_draw_background(&data, W_GRAPHICS(&gc), &bounds);
		}
		/*if (   (!g_xpStyle.IsThemeActive() || !g_xpStyle.IsAppThemed())
		 || (hTheme == NULL && bVistaThemeActive) )
		 pDC->FillSolidRect(&lpDIS->rcItem, GetSysColor(COLOR_BTNFACE));*/
		if (tci.iImage >= 0) {
			HIMAGELIST hImagelist = (HIMAGELIST) SendMessageW(
					_W_WIDGET(widget)->handle,
			TCM_GETIMAGELIST, 0, 0);
			if (hImagelist != 0) {
				IMAGEINFO ii;
				ImageList_GetImageInfo(hImagelist, 0, &ii);
				rect.left += ((lpDIS->itemState & ODS_SELECTED) != 0) ? 8 : 4;
				ImageList_Draw(hImagelist, tci.iImage, lpDIS->hDC, rect.left,
						rect.top + 2, ILD_TRANSPARENT);
				rect.left += (ii.rcImage.right - ii.rcImage.left);
				if ((lpDIS->itemState & ODS_SELECTED) == 0)
					rect.left += 4;

			}
		}
		// Draw 'Close button' at right side
		if (bCloseable) {
			RECT rcCloseButton;
			_w_tabitem_get_close_button_rect(widget, lpDIS->itemID,
					&lpDIS->rcItem, &rcCloseButton,
					(lpDIS->itemState & ODS_SELECTED) != 0, TRUE);
			_w_tabfolder_draw_close_button(lpDIS->hDC, &rcCloseButton, FALSE);
			/*
			 rect.right = rcCloseButton.left - 2;
			 if (bSelected)
			 rect.left += hTheme ? 4 : 2;*/
		}
		COLORREF crOldColor = CLR_NONE;
		if (tci.dwState & TCIS_HIGHLIGHTED)
			crOldColor = SetTextColor(lpDIS->hDC, RGB(192, 0, 0));
		else if (bVistaHotTracked)
			crOldColor = SetTextColor(lpDIS->hDC, GetSysColor(COLOR_BTNTEXT));

		rect.top += ((lpDIS->itemState & ODS_SELECTED) != 0) ? 4 : 3;
		char *text = (char*) win_toolkit->tmp;
		int count = w_utf8_from_utf16(tci.pszText, -1, text, tmp_count);
		text[tmp_count - 1] = 0;
		bounds.x = rect.left;
		bounds.y = rect.top;
		bounds.width = rect.right - rect.left;
		bounds.height = rect.bottom - rect.top;
		/*w_themedata_draw_text(&data, W_GRAPHICS(&gc), &bounds, text, count,
				W_THEME_DRAW_LEFT | W_THEME_DRAW_VCENTER);*/

		if (crOldColor != CLR_NONE)
			SetTextColor(lpDIS->hDC, crOldColor);

		int iOldBkMode = SetBkMode(lpDIS->hDC, TRANSPARENT);
		SetBkMode(lpDIS->hDC, iOldBkMode);

		//if (hTheme){
		memcpy(&rect, &lpDIS->rcItem, sizeof(RECT));
		if ((lpDIS->itemState & ODS_SELECTED) != 0) {
			rect.left -= 2 + 1;
			rect.right += 2 + 1;
		} else {
			//rect.top += 1;
		}
		ExcludeClipRect(lpDIS->hDC, rect.left, rect.top, rect.right,
				rect.bottom);
		//}
		//RestoreDC(lpDIS->hDC, nSavedDC);
		//w_themedata_close(&data);
		return TRUE;
	}
	return FALSE;
}
int _TABFOLDER_WM_MOUSEMOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _WIDGET_WM_MOUSEMOVE(widget, e, reserved);
	if (_W_WIDGET(widget)->style & W_CLOSE) {
		int index, selectedindex, draw_close, draw_last, last_left, lastindex;
		HPEN newPen, lastpen;
		TCHITTESTINFO pinfo;
		RECT r, rtmp;
		pinfo.pt.x = GET_X_LPARAM(e->lparam);
		pinfo.pt.y = GET_Y_LPARAM(e->lparam);
		pinfo.flags = 0;
		draw_close = 0;
		draw_last = 0;
		lastindex = _W_TABFOLDER(widget)->lastindex;
		index = (int) SendMessageW(_W_WIDGET(widget)->handle, TCM_HITTEST, 0,
				(LPARAM) &pinfo);
		selectedindex = (int) SendMessageW(_W_WIDGET(widget)->handle,
		TCM_GETCURSEL, 0, 0);
		if (index != -1) {
			if (SendMessageW(_W_WIDGET(widget)->handle,
			TCM_GETITEMRECT, index, (LPARAM) &r) == TRUE) {
				last_left = r.left;
				r.left = r.right - 16;
				if (PtInRect(&r, pinfo.pt)) {
					if (lastindex != index) {
						draw_close = 1;
						_W_TABFOLDER(widget)->lastindex = index;
					}
				} else {
					if (lastindex != -1) {
						draw_last = 1;
						_W_TABFOLDER(widget)->lastindex = -1;
					}
				}
			}
			r.left = last_left;
		} else {
			if (lastindex != -1) {
				draw_last = 1;
				_W_TABFOLDER(widget)->lastindex = -1;
			}
		}
		if (draw_close || draw_last) {
			HDC hdc = GetDC(_W_WIDGET(widget)->handle);
			if (draw_close) {
				_w_tabitem_get_close_button_rect(widget, index, &r, &rtmp,
						selectedindex == index, TRUE);
				_w_tabfolder_draw_close_button(hdc, &rtmp, TRUE);
				SetCapture(_W_WIDGET(widget)->handle);
			}
			if (draw_last) {
				if (lastindex != -1) {
					if (SendMessageW(_W_WIDGET(widget)->handle,
					TCM_GETITEMRECT, lastindex, (LPARAM) &r) == TRUE) {
						_w_tabitem_get_close_button_rect(widget, index, &r,
								&rtmp, selectedindex == lastindex, TRUE);
						_w_tabfolder_draw_close_button(hdc, &rtmp, FALSE);
					}
					ReleaseCapture();
				}
			}
			ReleaseDC(_W_WIDGET(widget)->handle, hdc);
		}
	}
	return result;
}
int _TABFOLDER_WM_MOUSELEAVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _WIDGET_WM_MOUSELEAVE(widget, e, reserved);
	RECT r, rtmp;
	if (_W_WIDGET(widget)->style & W_CLOSE) {
		if (_W_TABFOLDER(widget)->lastindex != -1) {
			HPEN newPen, lastpen;
			if (SendMessageW(_W_WIDGET(widget)->handle,
			TCM_GETITEMRECT, _W_TABFOLDER(widget)->lastindex,
					(LPARAM) &r) == TRUE) {
				HDC hdc = GetDC(_W_WIDGET(widget)->handle);
				_w_tabitem_get_close_button_rect(widget,
				_W_TABFOLDER(widget)->lastindex, &r, &rtmp, FALSE,
				TRUE);
				_w_tabfolder_draw_close_button(hdc, &rtmp, FALSE);
				ReleaseDC(_W_WIDGET(widget)->handle, hdc);
			}
			_W_TABFOLDER(widget)->lastindex = -1;
		}

	}
	return result;
}

int _TABFOLDER_WM_NOTIFYCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	NMHDR *hdr = (NMHDR*) e->lparam;
	switch (hdr->code) {
	case TCN_SELCHANGE:
	case TCN_SELCHANGING: {
		w_tabitem item;
		int index = SendMessageW(hdr->hwndFrom, TCM_GETCURSEL, 0, 0);
		if (index != -1) {
			_W_TABITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					W_TABFOLDER_GET_CLASS(widget)->class_tabfolderitem);
			_W_TABITEM(&item)->tabfolder = W_TABFOLDER(widget);
			_W_TABITEM(&item)->index = index;
			w_control *control = (w_control*) w_tabitem_get_control(&item);
			if (control != 0) {
				if (hdr->code == TCN_SELCHANGE) {
					w_rect r;
					w_scrollable_get_client_area(W_SCROLLABLE(widget), &r);
					w_control_set_bounds(control, &r.pt,&r.sz);
				}
				wresult visible;
				if (hdr->code == TCN_SELCHANGE)
					visible = W_TRUE;
				else
					visible = W_FALSE;
				w_control_set_visible(control, visible);
			}
		}
		if (hdr->code == TCN_SELCHANGE) {
			w_event_item _e;
			_W_TABITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					W_TABFOLDER_GET_CLASS(widget)->class_tabfolderitem);
			_W_TABITEM(&item)->tabfolder = W_TABFOLDER(widget);
			_W_TABITEM(&item)->index = index;
			_e.event.type = W_EVENT_ITEM_SELECTION;
			_e.event.widget = widget;
			_e.event.platform_event = (struct w_event_platform*) e;
			_e.event.data = 0;
			_e.item = (w_item*) &item;
			return _w_widget_send_event(widget, (w_event*) &_e);
		}
	}
	}
	return W_TRUE;
}
int _w_tabfolder_get_client_area(w_widget *widget,
		struct w_event_client_area *e, struct _w_widget_reserved *reserved) {
	RECT rect;
	GetClientRect(_W_WIDGET(widget)->handle, &rect);
	SendMessageW(_W_WIDGET(widget)->handle, TCM_ADJUSTRECT, 0, (LPARAM) &rect);
	e->rect->x = rect.left;
	e->rect->y = rect.top;
	e->rect->width = rect.right - rect.left;
	e->rect->height = rect.bottom - rect.top;
	return W_TRUE;
}
void _w_tabfolder_class_init(struct _w_tabfolder_class *clazz) {
	/*
	 * class
	 */
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TABFOLDER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tabfolder_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tabfolder);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tabfolder);
	W_WIDGET_CLASS(clazz)->create = _w_tabfolder_create;

	clazz->get_imagelist = _w_tabfolder_get_imagelist;
	clazz->get_item = _w_tabfolder_get_item;
	clazz->get_item_p = _w_tabfolder_get_item_p;
	clazz->get_item_count = _w_tabfolder_get_item_count;
	clazz->get_items = _w_tabfolder_get_items;
	clazz->get_selection = _w_tabfolder_get_selection;
	clazz->insert_item = _w_tabfolder_insert_item;
	clazz->set_imagelist = _w_tabfolder_set_imagelist;
	clazz->set_selection = _w_tabfolder_set_selection;
	/*
	 * class item
	 */
	struct _w_tabitem_class *item = clazz->class_tabfolderitem;
	item->get_bounds = _w_tabitem_get_bounds;
	item->get_image = _w_tabitem_get_image;
	item->set_image = _w_tabitem_set_image;
	item->pack = _w_tabitem_pack;
	item->item.get_parent_widget = _w_tabitem_get_parent_widget;
	item->item.get_data = _w_tabitem_get_data;
	item->item.get_index = _w_tabitem_get_index;
	item->item.get_text = _w_tabitem_get_text;
	item->item.set_data = _w_tabitem_set_data;
	item->item.set_text = _w_tabitem_set_text;
	item->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	item->item.widgetdata.close = _w_widgetdata_close;
	item->item.widgetdata.copy = _w_tabitem_copy;
	item->item.widgetdata.equals = _w_tabitem_equals;
	item->item.widgetdata.toolkit = (w_toolkit*) win_toolkit;
	/*
	 * reserved
	 */
	struct _w_tabfolder_reserved *reserved = _W_TABFOLDER_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->get_client_area = _w_tabfolder_get_client_area;
	msg->messages[_WM_SIZE] = _TABFOLDER_WM_SIZE;
	msg->messages[_WM_NOTIFYCHILD] = _TABFOLDER_WM_NOTIFYCHILD;
	msg->messages[_WM_MOUSEMOVE] = _TABFOLDER_WM_MOUSEMOVE;
	msg->messages[_WM_MOUSELEAVE] = _TABFOLDER_WM_MOUSELEAVE;
	msg->messages[_WM_DRAWCHILD] = _TABFOLDER_WM_DRAWCHILD;

}
#endif
