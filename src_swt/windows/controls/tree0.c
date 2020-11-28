/*
 * tree.c
 *
 *  Created on: 21 août 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "tree.h"
#include "../widgets/toolkit.h"

void _w_tree_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= TVS_SHOWSELALWAYS | TVS_LINESATROOT | TVS_HASBUTTONS
			| TVS_NONEVENHEIGHT;
#if !IsWinCE
	if (EXPLORER_THEME && WIN32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		*dwStyle |= TVS_TRACKSELECT;
		if ((style & W_FULL_SELECTION) != 0)
			*dwStyle |= TVS_FULLROWSELECT;
	} else {
#endif
		if ((style & W_FULL_SELECTION) != 0) {
			*dwStyle |= TVS_FULLROWSELECT;
		} else {
			*dwStyle |= TVS_HASLINES;
		}
#if !IsWinCE
	}
#endif
	if ((style & (W_HSCROLL | W_VSCROLL)) == 0) {
		*dwStyle &= ~(WS_HSCROLL | WS_VSCROLL);
		*dwStyle |= TVS_NOSCROLL;
	} else {
		if ((style & W_HSCROLL) == 0) {
			*dwStyle &= ~WS_HSCROLL;
			*dwStyle |= TVS_NOHSCROLL;
		}
	}
	if (style & W_CHECK) {
		*dwStyle |= TVS_CHECKBOXES;
	}
//	bits |= TVS_NOTOOLTIPS | TVS_DISABLEDRAGDROP;
	*dwStyle |= TVS_DISABLEDRAGDROP;
}
HTREEITEM _tree_find_item(HWND handle, HTREEITEM hFirstItem, int index) {
	if (hFirstItem == 0)
		return 0;
	int nextIndex = 0;
	HTREEITEM hNextItem = hFirstItem;
	while (hNextItem != 0 && nextIndex < index) {
		hNextItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
				(WPARAM) TVGN_NEXT, (LPARAM) hNextItem);
		nextIndex++;
	}
	if (index == nextIndex) {
		return hNextItem;
	}
	return 0;
}
int _tree_get_item_count(HWND handle, HTREEITEM hFirstItem) {
	if (hFirstItem == 0)
		return 0;
	int nextIndex = 0;
	HTREEITEM hNextItem = hFirstItem;
	while (hNextItem != 0) {
		hNextItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
				(WPARAM) TVGN_NEXT, (LPARAM) hNextItem);
		nextIndex++;
	}
	return nextIndex;
}
HTREEITEM _tree_find_previous(HWND handle, HTREEITEM parent, int index) {
	if (index < 0)
		return TVI_LAST;
	if (index == 0)
		return TVI_FIRST;
	HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(handle, TVM_GETNEXTITEM,
			(WPARAM) TVGN_CHILD, (LPARAM) parent);
	HTREEITEM hItem = _tree_find_item(handle, hFirstItem, index - 1);
	return hItem;
}
wresult _w_treeitem_insert_item_0(w_treeitem *item, w_treeitem *subitem,
		const char *text, HTREEITEM after) {
	TV_INSERTSTRUCTW tvInsert;
	struct UnicodeString str;
	unicode_init(&str);
	tvInsert.hParent = _W_TREEITEM(item)->htreeitem;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.iSelectedImage = I_IMAGECALLBACK;
	tvInsert.item.iImage = I_IMAGECALLBACK;
	tvInsert.hInsertAfter = after;
	if (text == 0) {
		tvInsert.item.pszText = LPSTR_TEXTCALLBACKW;
	} else {
		unicode_set(&str, text, -1);
		tvInsert.item.pszText = str.str;
		tvInsert.item.cchTextMax = str.length;
	}
	HTREEITEM hNewItem = (HTREEITEM) SendMessageW(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_INSERTITEMW, 0, (LPARAM) &tvInsert);
	unicode_free(&str);
	if (hNewItem == 0)
		return W_ERROR_ITEM_NOT_ADDED;
	if (subitem != 0) {
		_W_TREEITEM(subitem)->htreeitem = hNewItem;
		_W_TREEITEM(subitem)->tree = _W_TREEITEM(item)->tree;
		_W_TREEITEM(subitem)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
	}
	return W_TRUE;
}
wresult _w_tree_create_parent(w_tree *tree) {
	//forceResize ();
	HWND parent = GetParent(_W_WIDGET(tree)->handle);
	RECT rect;
	struct UnicodeString str;
	GetWindowRect(_W_WIDGET(tree)->handle, &rect);
	MapWindowPoints(0, parent, (LPPOINT) &rect, 2);
	DWORD dwExStyle = 0, dwStyle = 0;
	_w_composite_style(0, 0, _W_WIDGET(tree)->style, &dwExStyle, &dwStyle);
	DWORD oldStyle = GetWindowLong(_W_WIDGET(tree)->handle, GWL_STYLE);
	DWORD newStyle = dwStyle & ~WS_VISIBLE;
	if ((oldStyle & WS_DISABLED) != 0)
		newStyle |= WS_DISABLED;
	if ((oldStyle & WS_VISIBLE) != 0)
		newStyle |= WS_VISIBLE;
	unicode_set(&str, WindowName, -1);
	_W_TREE(tree)->hwndParent = CreateWindowExW(dwExStyle, str.str,
	NULL, newStyle, rect.left, rect.top, rect.right - rect.left,
			rect.bottom - rect.top, parent, 0, hinst,
			NULL);
	unicode_free(&str);
	if (_W_TREE(tree)->hwndParent == 0) {
		return W_ERROR_NO_HANDLES;
	}
	SetWindowLongPtrW(_W_TREE(tree)->hwndParent, GWLP_USERDATA,
			(LONG_PTR) tree);
	int bits = 0;
	if (WIN32_VERSION >= VERSION(4, 10)) {
		bits |= WS_EX_NOINHERITLAYOUT;
		if ((_W_WIDGET(tree)->style & W_RIGHT_TO_LEFT) != 0)
			bits |= WS_EX_LAYOUTRTL;
	}
	unicode_set(&str, WC_HEADERA, -1);
	_W_TREE(tree)->hwndHeader = CreateWindowExW(bits, str.str, NULL,
			HDS_BUTTONS | HDS_FULLDRAG | HDS_DRAGDROP | HDS_HIDDEN | WS_CHILD
					| WS_CLIPSIBLINGS, 0, 0, 0, 0,
			_W_TREE(tree)->hwndParent, 0, hinst,
			NULL);
	unicode_free(&str);
	if (_W_TREE(tree)->hwndHeader == 0) {
		return W_ERROR_NO_HANDLES;
	}
	/*struct _w_widget_reserved *reserved = _w_widget_reserved(W_WIDGET(tree));
	 _W_TREE_RESERVED(reserved)->headerProc =(WNDPROC) SetWindowLongPtrW(
	 _W_TREE(tree)->hwndHeader, GWLP_WNDPROC,(LONG_PTR) _w_widget_proc);*/

	//SetWindowLongPtr (_W_TREE(tree)->hwndHeader, GWLP_ID, hwndHeader);
	/*if (IsDBLocale) {
	 int hIMC = ImmGetContext(handle);
	 ImmAssociateContext(_W_TREE(tree)->hwndParent, hIMC);
	 ImmAssociateContext(_W_TREE(tree)->hwndHeader, hIMC);
	 ImmReleaseContext(_W_CONTROL(tree)->handle, hIMC);
	 }*/
//This code is intentionally commented
//	if (!IsPPC) {
//		if ((style & W_BORDER) != 0) {
//			int oldExStyle = GetWindowLong (handle, GWL_EXSTYLE);
//			oldExStyle &= ~WS_EX_CLIENTEDGE;
//			SetWindowLong (handle, GWL_EXSTYLE, oldExStyle);
//		}
//	}
	_w_tree_insert_column(tree, 0, "", 0);
	HFONT hFont = (HFONT) SendMessageW(_W_WIDGET(tree)->handle, WM_GETFONT, 0,
			0);
	if (hFont != 0)
		SendMessageW(_W_TREE(tree)->hwndHeader, WM_SETFONT, (WPARAM) hFont, 0);
	HWND hwndInsertAfter = GetWindow(_W_WIDGET(tree)->handle, GW_HWNDPREV);
	int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
	SetWindowPos(_W_TREE(tree)->hwndParent, hwndInsertAfter, 0, 0, 0, 0, flags);
	SCROLLINFO info;
	info.cbSize = sizeof(info);
	info.fMask = SIF_RANGE | SIF_PAGE;
	GetScrollInfo(_W_TREE(tree)->hwndParent, SB_HORZ, &info);
	info.nPage = info.nMax + 1;
	SetScrollInfo(_W_TREE(tree)->hwndParent, SB_HORZ, &info, TRUE);
	GetScrollInfo(_W_TREE(tree)->hwndParent, SB_VERT, &info);
	info.nPage = info.nMax + 1;
	SetScrollInfo(_W_TREE(tree)->hwndParent, SB_VERT, &info, TRUE);
	//customDraw = true;
	if ((oldStyle & WS_VISIBLE) != 0) {
		ShowWindow(_W_TREE(tree)->hwndParent, SW_SHOW);
	}
	HWND hwndFocus = GetFocus();
	if (hwndFocus == _W_WIDGET(tree)->handle)
		SetFocus(_W_TREE(tree)->hwndParent);
	SetParent(_W_WIDGET(tree)->handle, _W_TREE(tree)->hwndParent);
	if (hwndFocus == _W_WIDGET(tree)->handle)
		SetFocus(_W_WIDGET(tree)->handle);
	return W_TRUE;
}
HWND _w_tree_top_handle(w_control *control) {
	if (_W_TREE(control)->hwndParent != 0)
		return _W_TREE(control)->hwndParent;
	else
		return _W_WIDGET(control)->handle;
}
int _w_tree_get_header_width(w_tree *tree) {
	int total = 0;
	HDITEM hditem;
	hditem.mask = HDI_WIDTH;
	int hcnt = SendMessageW(_W_TREE(tree)->hwndHeader, HDM_GETITEMCOUNT,
			(WPARAM) 0, (LPARAM) 0);
	for (int i = 0; i < hcnt; i++) {
		if (SendMessageW(_W_TREE(tree)->hwndHeader, HDM_GETITEMW, i,
				(LPARAM) &hditem) == TRUE) {
			total += hditem.cxy;
		}
	}
	return total;
}

void _w_tree_set_scroll_width_0(w_tree *tree, int width) {
	if (_W_TREE(tree)->hwndHeader == 0 || _W_TREE(tree)->hwndParent == 0)
		return;
	//TEMPORARY CODE
	//scrollWidth = width;
	int left = 0;
	RECT rect;
	SCROLLINFO info, _info;
	WINDOWPOS pos;
	info.cbSize = sizeof(info);
	info.fMask = SIF_RANGE | SIF_PAGE;
	int columnCount = SendMessageW(_W_TREE(tree)->hwndHeader,
	HDM_GETITEMCOUNT, 0, 0);
	if (columnCount == 0 && width == 0) {
		GetScrollInfo(_W_TREE(tree)->hwndParent, SB_HORZ, &info);
		info.nPage = info.nMax + 1;
		SetScrollInfo(_W_TREE(tree)->hwndParent, SB_HORZ, &info, TRUE);
		GetScrollInfo(_W_TREE(tree)->hwndParent, SB_VERT, &info);
		info.nPage = info.nMax + 1;
		SetScrollInfo(_W_TREE(tree)->hwndParent, SB_VERT, &info, TRUE);
	} else {
		if ((_W_WIDGET(tree)->style & W_HSCROLL) != 0) {
			GetClientRect(_W_TREE(tree)->hwndParent, &rect);
			GetScrollInfo(_W_TREE(tree)->hwndParent, SB_HORZ, &info);
			info.nMax = width;
			info.nPage = rect.right - rect.left + 1;
			SetScrollInfo(_W_TREE(tree)->hwndParent, SB_HORZ, &info, TRUE);
			info.fMask = SIF_POS;
			GetScrollInfo(_W_TREE(tree)->hwndParent, SB_HORZ, &info);
			left = info.nPos;
		}
	}
	if ((_W_WIDGET(tree)->style & W_HSCROLL) != 0) {
		_info.cbSize = sizeof(_info);
		_info.fMask = SIF_RANGE | SIF_PAGE;
		GetScrollInfo(_W_WIDGET(tree)->handle, SB_HORZ, &_info);
		_info.nPos += TREE_INCREMENT;
		_info.nPage = info.nPage;
		SetScrollInfo(_W_WIDGET(tree)->handle, SB_HORZ, &_info, TRUE);
	}
	GetClientRect(_W_TREE(tree)->hwndParent, &rect);
	HDLAYOUT playout;
	playout.prc = &rect;
	playout.pwpos = &pos;
	SendMessageW(_W_TREE(tree)->hwndHeader, HDM_LAYOUT, 0, (LPARAM) &playout);
	int bits = GetWindowLongW(_W_WIDGET(tree)->handle, GWL_EXSTYLE);
	int b = (bits & WS_EX_CLIENTEDGE) != 0 ? GetSystemMetrics(SM_CXEDGE) : 0;
	int w = pos.cx
			+ (columnCount == 0 && width == 0 ?
					0 : GetSystemMetrics(SM_CXVSCROLL));
	int h = rect.bottom - rect.top - pos.y;
	SetWindowPos(_W_TREE(tree)->hwndHeader, HWND_TOP, pos.x - left, pos.y,
	/*pos.cx*/w + left, pos.cy, SWP_NOACTIVATE);
	//int oldIgnore = ignoreResize;
	//ignoreResize = true;
	SetWindowPos(_W_WIDGET(tree)->handle, 0, pos.x - left - b,
			pos.y + pos.cy - b, w + left + b * 2, h + b * 2,
			/*SWP_NOACTIVATE | */SWP_NOZORDER);
	//ignoreResize = oldIgnore;
}
void _w_tree_set_scroll_width(w_tree *tree) {
	if (_W_TREE(tree)->hwndHeader == 0 || _W_TREE(tree)->hwndParent == 0)
		return;
	_w_tree_set_scroll_width_0(tree, _w_tree_get_header_width(tree));
}
void _w_tree_update_scrollbar(w_tree *tree) {
	if (_W_TREE(tree)->hwndParent != 0) {
		int columnCount = SendMessageW(_W_TREE(tree)->hwndHeader,
		HDM_GETITEMCOUNT, 0, 0);
		if (columnCount != 0) {
			SCROLLINFO info;
			info.cbSize = sizeof(info);
			info.fMask = SIF_ALL;
			int itemCount = SendMessageW(_W_WIDGET(tree)->handle, TVM_GETCOUNT,
					0, 0);
			if (itemCount == 0) {
				GetScrollInfo(_W_TREE(tree)->hwndParent, SB_VERT, &info);
				info.nPage = info.nMax + 1;
				SetScrollInfo(_W_TREE(tree)->hwndParent, SB_VERT, &info, TRUE);
			} else {
				GetScrollInfo(_W_WIDGET(tree)->handle, SB_VERT, &info);
				if (/*!OS.IsWinCE&&*/WIN32_VERSION >= VERSION(4, 10)) {
					if (info.nPage == 0) {
						SCROLLBARINFO psbi;
						psbi.cbSize = sizeof(psbi);
						GetScrollBarInfo(_W_WIDGET(tree)->handle,
						OBJID_VSCROLL, &psbi);
						if ((psbi.rgstate[0] & STATE_SYSTEM_INVISIBLE) != 0) {
							info.nPage = info.nMax + 1;
						}
					}
				}
				SetScrollInfo(_W_TREE(tree)->hwndParent, SB_VERT, &info, TRUE);
			}
		}
	}
}
void _w_tree_def_proc(w_widget *widget, _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	if (_W_WIDGET(widget)->handle == e->hwnd) {
		e->result = CallWindowProcW(_W_CONTROL_RESERVED(reserved)->default_proc,
				e->hwnd, e->msg, e->wparam, e->lparam);
		return;
	}
	if (_W_TREE(widget)->hwndHeader == e->hwnd) {
		e->result = CallWindowProcW(_W_TREE_RESERVED(reserved)->headerProc,
				e->hwnd, e->msg, e->wparam, e->lparam);
		return;
	}
	e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
}
//TODO - take into account grid (add boolean arg) to damage less during redraw
void _w_treeitem_get_bounds_0(w_treeitem *item, RECT *rect, HDC hDC, int index,
		int flags) {
	memset(rect, 0, sizeof(RECT));
	if (!(flags & _W_TREE_BOUNDS_GET_TEXT)
			&& !(flags & _W_TREE_BOUNDS_GET_IMAGE))
		return;
	HWND hwnd = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	/*if ((parent.style & W_VIRTUAL) == 0 && !cached && !parent.painted) {
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_TEXT;
	 tvItem.hItem = handle;
	 tvItem.pszText = OS.LPSTR_TEXTCALLBACK;
	 parent.ignoreCustomDraw = true;
	 OS.SendMessage (hwnd, OS.TVM_SETITEM, 0, tvItem);
	 parent.ignoreCustomDraw = false;
	 }*/
	wresult firstColumn = index == 0;
	int columnCount = 0;
	HWND hwndHeader = _W_TREE(_W_TREEITEM(item)->tree)->hwndHeader;
	if (hwndHeader != 0) {
		columnCount = SendMessageW(hwndHeader,
		HDM_GETITEMCOUNT, 0, 0);
		firstColumn = index == SendMessageW(hwndHeader, HDM_ORDERTOINDEX, 0, 0);
	}
	if (firstColumn) {
		wresult full = columnCount == 0 && (flags & _W_TREE_BOUNDS_GET_TEXT)
				&& (flags & _W_TREE_BOUNDS_GET_IMAGE)
				&& (flags & _W_TREE_BOUNDS_FULLTEXT)
				&& (flags & _W_TREE_BOUNDS_FULLIMAGE);
		*((HTREEITEM*) rect) = _W_TREEITEM(item)->htreeitem;
		if (!(WINBOOL) SendMessageW(hwnd, TVM_GETITEMRECT, !full,
				(LPARAM) rect)) {
			memset(rect, 0, sizeof(RECT));
			return;
		}
		if ((flags & _W_TREE_BOUNDS_GET_IMAGE)
				&& !(flags & _W_TREE_BOUNDS_FULLIMAGE)) {
			HIMAGELIST imagelist = (HIMAGELIST) SendMessageW(hwnd,
			TVM_GETIMAGELIST,
			TVSIL_NORMAL, 0);
			if (imagelist != 0) {
				int cx, cy;
				ImageList_GetIconSize(imagelist, &cx, &cy);
				rect->left -= cx + _W_TREE_INSET;
				if (!(flags & _W_TREE_BOUNDS_GET_TEXT))
					rect->right = rect->left + cx;
			} else {
				if (!(flags & _W_TREE_BOUNDS_GET_TEXT))
					rect->right = rect->left;
			}
		}
		if ((flags & _W_TREE_BOUNDS_FULLTEXT)
				|| (flags & _W_TREE_BOUNDS_FULLIMAGE)
				|| (flags & _W_TREE_BOUNDS_CLIP)) {
			if (hwndHeader != 0) {
				RECT headerRect;
				if (columnCount != 0) {
					if (SendMessageW(hwndHeader, HDM_GETITEMRECT, index,
							(LPARAM) &headerRect) == 0) {
						return;
					}
				} else {
					headerRect.right =
					_W_TREE(_W_TREEITEM(item)->tree)->scrollWidth;
					if (headerRect.right == 0) {
						memcpy(&headerRect, rect, sizeof(RECT));
					}
				}
				if ((flags & _W_TREE_BOUNDS_FULLTEXT)
						&& (flags & _W_TREE_BOUNDS_CLIP))
					rect->right = headerRect.right;
				if ((flags & _W_TREE_BOUNDS_FULLIMAGE))
					rect->left = headerRect.left;
				if ((flags & _W_TREE_BOUNDS_CLIP)
						&& headerRect.right < rect->right) {
					rect->right = headerRect.right;
				}
			}
		}
	} else {
		if (!(0 <= index && index < columnCount))
			return;
		RECT headerRect;
		if (SendMessageW(hwndHeader, HDM_GETITEMRECT, index,
				(LPARAM) &headerRect) == 0) {
			return;
		}
		*((HTREEITEM*) rect) = _W_TREEITEM(item)->htreeitem;
		if (!(WINBOOL) SendMessageW(hwnd, TVM_GETITEMRECT, FALSE,
				(LPARAM) rect)) {
			return;
		}
		rect->left = headerRect.left;
		if ((flags & _W_TREE_BOUNDS_FULLTEXT)
				&& (flags & _W_TREE_BOUNDS_GET_IMAGE)
				&& (flags & _W_TREE_BOUNDS_CLIP)) {
			rect->right = headerRect.right;
		} else {
			rect->right = headerRect.left;
			/*Image image = null;
			 if (index == 0) {
			 image = this.image;
			 } else {
			 if (images != null) image = images [index];
			 }
			 if (image != null) {
			 Point size = parent.getImageSize ();
			 rect.right += size.x;
			 }*/
			if ((flags & _W_TREE_BOUNDS_GET_TEXT)) {
				if ((flags & _W_TREE_BOUNDS_FULLTEXT)
						&& (flags & _W_TREE_BOUNDS_CLIP)) {
					rect->left = rect->right + _W_TREE_INSET;
					rect->right = headerRect.right;
				} else {
					const char *string = 0; /*index == 0 ? text : strings != null ? strings [index] : null;*/
					if (string != 0) {
						RECT textRect;
						WCHAR buffer = 0;/* new TCHAR (parent.getCodePage (), string, false);*/
						int length;
						int flags = DT_NOPREFIX | DT_SINGLELINE | DT_CALCRECT;
						HDC hNewDC = hDC;
						HFONT hFont = 0;
						if (hDC == 0) {
							hNewDC = GetDC(hwnd);
							/*hFont = fontHandle(index);
							 if (hFont == -1)
							 hFont = SendMessage(hwnd, WM_GETFONT, 0, 0);
							 hFont = SelectObject(hNewDC, hFont);*/
						}
						//DrawTextW(hNewDC, buffer, length, &textRect, flags);
						if (hDC == 0) {
							SelectObject(hNewDC, hFont);
							ReleaseDC(hwnd, hNewDC);
						}
						if ((flags & _W_TREE_BOUNDS_GET_IMAGE)) {
							rect->right += textRect.right - textRect.left
									+ _W_TREE_INSET * 3;
						} else {
							rect->left = rect->right + _W_TREE_INSET;
							rect->right = rect->left
									+ (textRect.right - textRect.left)
									+ _W_TREE_INSET;
						}
					}
				}
			}
			if ((flags & _W_TREE_BOUNDS_CLIP)
					&& headerRect.right < rect->right) {
				rect->right = headerRect.right;
			}
		}
	}
	int gridWidth =
	_W_TREE(_W_TREEITEM(item)->tree)->linesVisible && columnCount != 0 ?
	_W_TREE_GRID_WIDTH :
																			0;
	if ((flags & _W_TREE_BOUNDS_GET_TEXT)
			|| !(flags & _W_TREE_BOUNDS_GET_IMAGE)) {
		rect->right = w_max(rect->left, rect->right - gridWidth);
	}
	rect->bottom = w_max(rect->top, rect->bottom - gridWidth);
	return;
}
/*
 * tree column
 */
#define _W_TREECOLUMN_MOVEABLE 1
#define _W_TREECOLUMN_RESIZABLE 2
struct _w_tree_columns* _w_tree_columns_resize(w_tree *tree, int count) {
	struct _w_tree_columns *columns = _W_TREE(tree)->column_info;
	if (columns == 0) {
		columns = malloc(sizeof(struct _w_tree_columns) + count * sizeof(char));
		if (columns == 0)
			return 0;
		for (int i = 0; i < count; i++) {
			columns->flags[i] =
			_W_TREECOLUMN_MOVEABLE | _W_TREECOLUMN_RESIZABLE;
		}
		columns->alloc = count;
	} else {
		if (columns->alloc <= count) {
			columns = realloc(columns,
					sizeof(struct _w_tree_columns) + count * sizeof(char));
			if (columns == 0)
				return 0;
			for (int i = columns->alloc; i < count; i++) {
				columns->flags[i] = _W_TREECOLUMN_MOVEABLE
						| _W_TREECOLUMN_RESIZABLE;
			}
			columns->alloc = count;
		}
	}
	_W_TREE(tree)->column_info = columns;
	return columns;
}
int _w_treecolumn_get_alignment(w_treecolumn *column) {
	HDITEMW hdItem;
	hdItem.mask = HDI_FORMAT;
	hdItem.fmt = 0;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_GETITEMW,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	int alignment = 0;
	if (hdItem.fmt & HDF_LEFT)
		alignment |= W_LEFT;
	if (hdItem.fmt & HDF_CENTER)
		alignment |= W_CENTER;
	if (hdItem.fmt & HDF_RIGHT)
		alignment |= W_RIGHT;
	return alignment;
}
int _w_treecolumn_get_order(w_treecolumn *column) {
	HDITEMW hdItem;
	hdItem.mask = HDI_ORDER;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_GETITEM,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	return hdItem.iOrder;
}
int _w_treecolumn_get_image(w_treecolumn *column) {
	HDITEMW hdItem;
	hdItem.mask = HDI_IMAGE;
	hdItem.iImage = -1;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_GETITEM,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	return hdItem.iImage;
}
wresult _w_treecolumn_get_moveable_0(w_tree *tree, int index) {
	if (index == 0)
		return W_FALSE;
	struct _w_tree_columns *column_info =
	_W_TREE(tree)->column_info;
	if (column_info != 0 && column_info->alloc > index) {
		return column_info->flags[index] & _W_TREECOLUMN_MOVEABLE;
	} else {
		return W_TRUE;
	}
}
wresult _w_treecolumn_get_moveable(w_treecolumn *column) {
	return _w_treecolumn_get_moveable_0(_W_TREECOLUMN(column)->tree,
	_W_TREECOLUMN(column)->index);
}
wresult _w_treecolumn_get_resizable_0(w_tree *tree, int index) {
	struct _w_tree_columns *column_info =
	_W_TREE(tree)->column_info;
	if (column_info != 0 && column_info->alloc > index) {
		return column_info->flags[index] & _W_TREECOLUMN_RESIZABLE;
	} else {
		return W_TRUE;
	}
}
wresult _w_treecolumn_get_resizable(w_treecolumn *column) {
	return _w_treecolumn_get_resizable_0(_W_TREECOLUMN(column)->tree,
	_W_TREECOLUMN(column)->index);
}
wresult _w_treecolumn_get_tooltip_text(w_treecolumn *column, w_alloc *text) {
	return 0;
}
int _w_treecolumn_get_width(w_treecolumn *column) {
	HDITEMW hdItem;
	hdItem.mask = HDI_WIDTH;
	hdItem.cxy = 0;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_GETITEM,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	return hdItem.cxy;
}
wresult _w_treecolumn_pack(w_treecolumn *column) {
	//int columnWidth = 0;
	RECT headerRect, rect;
	TVITEMW tvItem;
	struct UnicodeString str;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader,
	HDM_GETITEMRECT, _W_TREECOLUMN(column)->index, (LPARAM) &headerRect);
	HDC hDC = GetDC(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle);
	HFONT oldFont = 0, newFont = (HFONT) SendMessageW(
			_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle, WM_GETFONT, 0, 0);
	if (newFont != 0)
		oldFont = SelectObject(hDC, newFont);
	tvItem.mask = TVIF_HANDLE | TVIF_PARAM | TVIF_STATE;
	tvItem.hItem = (HTREEITEM) SendMessageW(
			_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle, TVM_GETNEXTITEM,
	TVGN_ROOT, 0);
	while (tvItem.hItem != 0) {
		SendMessageW(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle,
		TVM_GETITEMW, 0, (LPARAM) &tvItem);
		/*TreeItem item = tvItem.lParam != -1 ? parent.items [(int)tvItem.lParam] : null;
		 if (item != null) {
		 int itemRight = 0;
		 if (parent.hooks (SWT.MeasureItem)) {
		 int detail = (tvItem.state & OS.TVIS_SELECTED) != 0 ? SWT.SELECTED : 0;
		 Event event = parent.sendMeasureItemEvent (item, index, hDC, detail);
		 if (isDisposed () || parent.isDisposed ()) break;
		 Rectangle bounds = event.getBoundsInPixels();
		 itemRight = bounds.x + bounds.width;
		 } else {
		 long hFont = item.fontHandle (index);
		 if (hFont != -1) hFont = OS.SelectObject (hDC, hFont);
		 RECT itemRect = item.getBounds (index, true, true, false, false, false, hDC);
		 if (hFont != -1) OS.SelectObject (hDC, hFont);
		 itemRight = itemRect.right;
		 }
		 columnWidth = Math.max (columnWidth, itemRight - headerRect.left);
		 }*/
		tvItem.hItem = (HTREEITEM) SendMessageW(
				_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle,
		TVM_GETNEXTITEM, TVGN_NEXTVISIBLE, (LPARAM) tvItem.hItem);
	}
	unicode_alloc_tmp(&str);
	tvItem.mask = HDI_TEXT;
	tvItem.pszText = str.str;
	tvItem.cchTextMax = str.length;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_GETITEMW,
	_W_TREECOLUMN(column)->index, (LPARAM) &tvItem);
	if (tvItem.cchTextMax >= str.length) {
		unicode_alloc(&str, tvItem.cchTextMax);
		if (str.str == 0)
			unicode_alloc_tmp(&str);
		else {
			tvItem.pszText = str.str;
			tvItem.cchTextMax = str.length;
			SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader,
			HDM_GETITEMW,
			_W_TREECOLUMN(column)->index, (LPARAM) &tvItem);
		}
	}
	int flags = DT_CALCRECT | DT_NOPREFIX;
	DrawTextW(hDC, str.str, str.length, &rect, flags);
	int headerWidth = rect.right - rect.left + _W_TREE_HEADER_MARGIN;
	if (IsAppThemed())
		headerWidth += _W_TREE_HEADER_EXTRA;
	/*if (image != null || parent.sortColumn == this) {
	 Image headerImage = null;
	 if (parent.sortColumn == this && parent.sortDirection != SWT.NONE) {
	 headerWidth += Tree.SORT_WIDTH;
	 } else {
	 headerImage = image;
	 }
	 if (headerImage != null) {
	 Rectangle bounds = headerImage.getBoundsInPixels ();
	 headerWidth += bounds.width;
	 }
	 int margin = 0;
	 if (hwndHeader != 0) {
	 margin = (int)OS.SendMessage (hwndHeader, OS.HDM_GETBITMAPMARGIN, 0, 0);
	 } else {
	 margin = OS.GetSystemMetrics (OS.SM_CXEDGE) * 3;
	 }
	 headerWidth += margin * 2;
	 }*/
	if (newFont != 0)
		SelectObject(hDC, oldFont);
	ReleaseDC(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, hDC);
	unicode_free(&str);
	//int gridWidth = parent.linesVisible ? Tree.GRID_WIDTH : 0;
	//setWidthInPixels (Math.max (headerWidth, columnWidth + gridWidth));
	return W_TRUE;
}
wresult _w_treecolumn_set_alignment(w_treecolumn *column, int alignment) {
	HDITEMW hdItem;
	RECT rect, headerRect;
	hdItem.mask = HDI_FORMAT;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_GETITEMW,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	hdItem.fmt &= ~HDF_JUSTIFYMASK;
	if ((alignment & W_LEFT) == W_LEFT)
		hdItem.fmt |= HDF_LEFT;
	if ((alignment & W_CENTER) == W_CENTER)
		hdItem.fmt |= HDF_CENTER;
	if ((alignment & W_RIGHT) == W_RIGHT)
		hdItem.fmt |= HDF_RIGHT;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_SETITEMW,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	//_w_control_force_resize(_W_TREECOLUMN(column)->tree);
	GetClientRect(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle, &rect);
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader,
	HDM_GETITEMRECT, _W_TREECOLUMN(column)->index, (LPARAM) &headerRect);
	rect.left = headerRect.left;
	rect.right = headerRect.right;
	InvalidateRect(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle, &rect,
	TRUE);
	return W_TRUE;
}
wresult _w_treecolumn_set_image(w_treecolumn *column, int image) {
	HDITEMW hdItem;
	hdItem.mask = HDI_IMAGE;
	hdItem.iImage = image;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_SETITEM,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	return W_TRUE;
}
wresult _w_treecolumn_set_moveable(w_treecolumn *column, wresult moveable) {
	if (_W_TREECOLUMN(column)->index == 0)
		return W_FALSE;
	struct _w_tree_columns *column_info =
	_W_TREE(_W_TREECOLUMN(column)->tree)->column_info;
	if (moveable) {
		if (column_info != 0
				&& column_info->alloc > _W_TREECOLUMN(column)->index) {
			column_info->flags[_W_TREECOLUMN(column)->index] |=
			_W_TREECOLUMN_MOVEABLE;
		}
	} else {
		column_info = _w_tree_columns_resize(_W_TREECOLUMN(column)->tree,
		_W_TREECOLUMN(column)->index + 1);
		if (column_info == 0)
			return W_ERROR_NO_MEMORY;
		column_info->flags[_W_TREECOLUMN(column)->index] &=
				~_W_TREECOLUMN_MOVEABLE;

	}
	return W_TRUE;
}
wresult _w_treecolumn_set_resizable(w_treecolumn *column, wresult resizable) {
	struct _w_tree_columns *column_info =
	_W_TREE(_W_TREECOLUMN(column)->tree)->column_info;
	if (resizable) {
		if (column_info != 0
				&& column_info->alloc > _W_TREECOLUMN(column)->index) {
			column_info->flags[_W_TREECOLUMN(column)->index] |=
			_W_TREECOLUMN_RESIZABLE;
		}
	} else {
		column_info = _w_tree_columns_resize(_W_TREECOLUMN(column)->tree,
		_W_TREECOLUMN(column)->index + 1);
		if (column_info == 0)
			return W_ERROR_NO_MEMORY;
		column_info->flags[_W_TREECOLUMN(column)->index] &=
				~_W_TREECOLUMN_RESIZABLE;

	}
	return W_TRUE;
}
wresult _w_treecolumn_set_tooltip_text(w_treecolumn *column,
		const char *text) {
	return W_FALSE;
}
wresult _w_treecolumn_set_width(w_treecolumn *column, int width) {
	HDITEMW hdItem;
	RECT rect, headerRect;
	if (width < 0)
		return W_FALSE;
	hdItem.mask = HDI_WIDTH;
	hdItem.cxy = width;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_SETITEMW,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader,
	HDM_GETITEMRECT, _W_TREECOLUMN(column)->index, (LPARAM) &headerRect);
	//_w_control_force_resize(_W_TREECOLUMN(column)->tree);
	GetClientRect(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle, &rect);
	rect.left = headerRect.left;
	InvalidateRect(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle, &rect,
	TRUE);
	_w_tree_set_scroll_width(_W_TREECOLUMN(column)->tree);
	return W_TRUE;
}
wresult _w_treecolumn_set_order(w_treecolumn *column, int order) {
	HDITEMW hdItem;
	hdItem.mask = HDI_ORDER;
	hdItem.iOrder = order;
	SendMessageW(_W_TREE(_W_TREECOLUMN(column)->tree)->hwndHeader, HDM_SETITEMW,
	_W_TREECOLUMN(column)->index, (LPARAM) &hdItem);
	return W_TRUE;
}
w_widget* _w_treecolumn_get_parent_widget(w_item *item) {
	return W_WIDGET(_W_TREECOLUMN(item)->tree);
}
void* _w_treecolumn_get_data(w_item *item) {
	HDITEMW hdItem;
	hdItem.mask = HDI_LPARAM;
	hdItem.lParam = 0;
	SendMessageW(_W_TREE(_W_TREECOLUMN(item)->tree)->hwndHeader, HDM_GETITEMW,
	_W_TREECOLUMN(item)->index, (LPARAM) &hdItem);
	return (void*) hdItem.lParam;
}
int _w_treecolumn_get_index(w_item *item) {
	return _W_TREECOLUMN(item)->index;
}
wresult _w_treecolumn_get_text(w_item *item, w_alloc *text) {
	WCHAR *wtext = 0, *new_wtext = 0;
	wresult result = W_FALSE;
	HDITEMW hdItem;
	hdItem.mask = HDI_TEXT;
	hdItem.pszText = win_toolkit->tmp_wchar;
	hdItem.cchTextMax = sizeof(win_toolkit->tmp_wchar)
			/ sizeof(win_toolkit->tmp_wchar[0]);
	if (SendMessageW(_W_TREE(_W_TREECOLUMN(item)->tree)->hwndHeader,
	HDM_GETITEMW,
	_W_TREECOLUMN(item)->index, (LPARAM) &hdItem)) {
		wtext = hdItem.pszText;
		if (hdItem.cchTextMax
				> sizeof(win_toolkit->tmp_wchar)
						/ sizeof(win_toolkit->tmp_wchar[0])) {
			new_wtext = malloc(hdItem.cchTextMax * sizeof(WCHAR));
			if (new_wtext == 0) {
				return W_FALSE;
			}
			hdItem.mask = HDI_TEXT;
			hdItem.pszText = new_wtext;
			//hdItem.cchTextMax = hdItem.cchTextMax;
			if (SendMessageW(_W_TREE(_W_TREECOLUMN(item)->tree)->hwndHeader,
			HDM_GETITEMW,
			_W_TREECOLUMN(item)->index, (LPARAM) &hdItem)) {
				wtext = hdItem.pszText;
			} else
				wtext = 0;
		}
	}
	if (wtext) {
		char *str = 0;
		size_t sz, new_sz;
		new_sz = w_utf8_from_utf16(wtext, -1, 0, 0);
		//sz = text->realloc(text, new_sz, &str);
		if (str != 0)
			w_utf8_from_utf16(wtext, -1, str, sz);
		result = W_TRUE;
	}
	if (new_wtext != 0) {
		free(new_wtext);
	}
	return result;
}
wresult _w_treecolumn_set_data(w_item *item, void *data) {
	HDITEMW hdItem;
	hdItem.mask = HDI_LPARAM;
	hdItem.lParam = (LPARAM) data;
	SendMessageW(_W_TREE(_W_TREECOLUMN(item)->tree)->hwndHeader, HDM_SETITEMW,
	_W_TREECOLUMN(item)->index, (LPARAM) &hdItem);
	return W_TRUE;
}
wresult _w_treecolumn_set_text(w_item *item, const char *string) {
	HDITEMW hdItem;
	struct UnicodeString str;
	unicode_set(&str, string, -1);
	hdItem.mask = HDI_TEXT;
	hdItem.pszText = str.str;
	hdItem.cchTextMax = str.length;
	SendMessageW(_W_TREE(_W_TREECOLUMN(item)->tree)->hwndHeader, HDM_SETITEMW,
	_W_TREECOLUMN(item)->index, (LPARAM) &hdItem);
	unicode_free(&str);
	return W_TRUE;

}
/*
 * tree item
 */
wresult _w_treeitem_clear(w_treeitem *item, int index, wresult all) {
}
wresult _w_treeitem_clear_all(w_treeitem *item, wresult all) {
}
void _w_treeitem_get_bounds_i(w_treeitem *item, int index, w_rect *bounds) {
	RECT r;
	_w_treeitem_get_bounds_0(item, &r, NULL, 0,
			_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_GET_IMAGE
					| _W_TREE_BOUNDS_FULLTEXT | _W_TREE_BOUNDS_CLIP);
	bounds->x = r.left;
	bounds->y = r.top;
	bounds->width = r.right - r.left;
	bounds->height = r.bottom - r.top;
}
void _w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
	RECT r;
	_w_treeitem_get_bounds_0(item, &r, NULL, 0,
	_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_CLIP);
	bounds->x = r.left;
	bounds->y = r.top;
	bounds->width = r.right - r.left;
	bounds->height = r.bottom - r.top;
}
wresult _w_treeitem_get_checked(w_treeitem *item) {
	TVITEMW tvItem;
	tvItem.mask = TVIF_HANDLE | TVIF_STATE;
	tvItem.stateMask = TVIS_STATEIMAGEMASK;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_GETITEMW, 0, (LPARAM) &tvItem);
	return (result != 0) && (((tvItem.state >> 12) & 1) == 0);
}
wresult _w_treeitem_get_expanded(w_treeitem *item) {
	/*
	 * Bug in Windows.  Despite the fact that TVM_GETITEMSTATE claims
	 * to return only the bits specified by the stateMask, when called
	 * with TVIS_EXPANDED, the entire state is returned.  The fix is
	 * to explicitly check for the TVIS_EXPANDED bit.
	 */
	int state = SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_GETITEMSTATE, (WPARAM) _W_TREEITEM(item)->htreeitem,
			(LPARAM) TVIS_EXPANDED);
	return (state & TVIS_EXPANDED) != 0;
}
wresult _w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
	HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_GETNEXTITEM,
	TVGN_CHILD, (LPARAM) _W_TREEITEM(item)->htreeitem);
	if (hFirstItem == 0) {
		return W_FALSE;
	}
	if (child != 0) {
		_W_TREEITEM(child)->htreeitem = hFirstItem;
		_W_TREEITEM(child)->tree = _W_TREEITEM(item)->tree;
		_W_TREEITEM(child)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
	}
	return W_TRUE;
}
wresult _w_treeitem_get_grayed(w_treeitem *item) {
	if ((_W_WIDGET(_W_TREEITEM(item)->tree)->style & W_CHECK) == 0)
		return W_FALSE;
	TVITEMW tvItem;
	tvItem.mask = TVIF_HANDLE | TVIF_STATE;
	tvItem.stateMask = TVIS_STATEIMAGEMASK;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_GETITEMW, 0, (LPARAM) &tvItem);
	return (result != 0) && ((tvItem.state >> 12) > 2);
}
wresult _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
	HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_GETNEXTITEM,
	TVGN_CHILD, (LPARAM) _W_TREEITEM(item)->htreeitem);
	if (hFirstItem == 0) {
		return W_FALSE;
	}
	HTREEITEM hItem = _tree_find_item(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle, hFirstItem, index);
	if (hItem == 0) {
		return W_FALSE;
	}
	if (subitem != 0) {
		_W_TREEITEM(subitem)->htreeitem = hItem;
		_W_TREEITEM(subitem)->tree = _W_TREEITEM(item)->tree;
		_W_TREEITEM(subitem)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
	}
	return W_TRUE;
}
int _w_treeitem_get_item_count(w_treeitem *item) {
	HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_GETNEXTITEM,
	TVGN_CHILD, (LPARAM) _W_TREEITEM(item)->htreeitem);
	return _tree_get_item_count(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle, hFirstItem);
}
void _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
}
int _w_treeitem_get_image(w_treeitem *item) {
}
wresult _w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
}
wresult _w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
}
w_tree* _w_treeitem_get_parent(w_treeitem *item) {
}
wresult _w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	HTREEITEM hItem = (HTREEITEM) SendMessageW(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_GETNEXTITEM,
	TVGN_PARENT, (LPARAM) _W_TREEITEM(item)->htreeitem);
	if (hItem == 0)
		return W_FALSE;
	if (parent != 0) {
		_W_TREEITEM(parent)->htreeitem = hItem;
		_W_TREEITEM(parent)->tree = _W_TREEITEM(item)->tree;
		_W_TREEITEM(parent)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
	}
	return W_TRUE;
}
wresult _w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		const char *text, int index) {
	HTREEITEM i = _tree_find_previous(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	_W_TREEITEM(item)->htreeitem, index);
	if (i == 0)
		return W_ERROR_INVALID_RANGE;
	return _w_treeitem_insert_item_0(item, subitem, text, i);
}
wresult _w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		const char *text, w_treeitem *after) {
	if (W_WIDGETDATA_CHECK(after)) {
		return _w_treeitem_insert_item_0(item, subitem, text,
		_W_TREEITEM(after)->htreeitem);
	} else
		return W_ERROR_INVALID_ARGUMENT;
}
wresult _w_treeitem_remove_all(w_treeitem *item) {
}
wresult _w_treeitem_set_checked(w_treeitem *item, wresult checked) {
	if ((_W_WIDGET(_W_TREEITEM(item)->tree)->style & W_CHECK) == 0)
		return W_FALSE;
	TVITEMW tvItem;
	tvItem.mask = TVIF_HANDLE | TVIF_STATE;
	tvItem.stateMask = TVIS_STATEIMAGEMASK;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	SendMessageW(
			_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_GETITEMW, 0,
			(LPARAM) &tvItem);
	int state = tvItem.state >> 12;
	if (checked) {
		if ((state & 0x1) != 0)
			state++;
	} else {
		if ((state & 0x1) == 0)
			--state;
	}
	state <<= 12;
	if (tvItem.state == state)
		return W_TRUE;
	//if ((parent.style & SWT.VIRTUAL) != 0) cached = true;
	tvItem.state = state;
	SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_SETITEMW, 0,
			(LPARAM) &tvItem);
	/*
	 * Bug in Windows.  When TVM_SETITEM is used to set
	 * the state image of an item inside TVN_GETDISPINFO,
	 * the new state is not redrawn.  The fix is to force
	 * a redraw.
	 */
	//if ((parent.style & SWT.VIRTUAL) != 0) {
	//if (parent.currentItem == this && OS.IsWindowVisible (hwnd)) {
	RECT rect;
	if (TreeView_GetItemRect(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
			_W_TREEITEM(item)->htreeitem, &rect, FALSE)) {
		InvalidateRect(_W_WIDGET(_W_TREEITEM(item)->tree)->handle, &rect,
		TRUE);
	}
	//}
	//}
	return W_TRUE;
}
wresult _w_treeitem_set_expanded(w_treeitem *item, wresult expanded) {
}
wresult _w_treeitem_set_grayed(w_treeitem *item, wresult grayed) {
	if ((_W_WIDGET(_W_TREEITEM(item)->tree)->style & W_CHECK) == 0)
		return W_FALSE;
	TVITEMW tvItem;
	tvItem.mask = TVIF_HANDLE | TVIF_STATE;
	tvItem.stateMask = TVIS_STATEIMAGEMASK;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_GETITEMW, 0,
			(LPARAM) &tvItem);
	int state = tvItem.state >> 12;
	if (grayed) {
		if (state <= 2)
			state += 2;
	} else {
		if (state > 2)
			state -= 2;
	}
	state <<= 12;
	if (tvItem.state == state)
		return W_TRUE;
	//if ((parent.style & SWT.VIRTUAL) != 0) cached = true;
	tvItem.state = state;
	SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle, TVM_SETITEMW, 0,
			(LPARAM) &tvItem);
	/*
	 * Bug in Windows.  When TVM_SETITEM is used to set
	 * the state image of an item inside TVN_GETDISPINFO,
	 * the new state is not redrawn.  The fix is to force
	 * a redraw.
	 */
	//if ((parent.style & SWT.VIRTUAL) != 0) {
	//	if (parent.currentItem == this && OS.IsWindowVisible (hwnd)) {
	RECT rect;
	if (TreeView_GetItemRect(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
			_W_TREEITEM(item)->htreeitem, &rect, FALSE)) {
		InvalidateRect(_W_WIDGET(_W_TREEITEM(item)->tree)->handle, &rect,
		TRUE);
	}
	//	}
	//}
	return W_TRUE;
}
wresult _w_treeitem_set_has_children(w_treeitem *item) {
	TVITEMW tvItem;
	tvItem.mask = TVIF_CHILDREN;
	tvItem.cChildren = 1;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_SETITEMW, 0, (LPARAM) &tvItem);
	if (result == 0)
		return W_FALSE;
	else
		return W_TRUE;
}
wresult _w_treeitem_set_image(w_treeitem *item, int image) {
	TVITEMW tvItem;
	tvItem.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvItem.iImage = image;
	tvItem.iSelectedImage = image;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_SETITEMW, 0, (LPARAM) &tvItem);
	if (result == 0)
		return W_FALSE;
	else
		return W_TRUE;
}
w_widget* _w_treeitem_get_parent_widget(w_item *item) {
	return W_WIDGET(_W_TREEITEM(item)->tree);
}
void* _w_treeitem_get_data(w_item *item) {
	TVITEMW tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.lParam = 0;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_GETITEMW, 0, (LPARAM) &tvItem);
	return (void*) tvItem.lParam;
}
int _w_treeitem_get_index(w_item *item) {
}
wresult _w_treeitem_get_text(w_item *item, w_alloc *text) {
}
wresult _w_treeitem_set_data(w_item *item, void *data) {
	TVITEMW tvItem;
	tvItem.mask = TVIF_PARAM;
	tvItem.lParam = (LPARAM) data;
	tvItem.hItem = _W_TREEITEM(item)->htreeitem;
	HRESULT result = SendMessageW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle,
	TVM_SETITEMW, 0, (LPARAM) &tvItem);
	if (result == 0)
		return W_FALSE;
	else
		return W_TRUE;
}
wresult _w_treeitem_set_text(w_item *item, const char *string) {
}
void _w_treeitem_copy(w_widgetdata *from, w_widgetdata *to) {

}
wresult _w_treeitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {

}
/*
 * tree
 */
wresult _w_tree_deselect(w_tree *tree, w_treeitem *item) {
	/*if (item == null) error (SWT.ERROR_NULL_ARGUMENT);
	 if (item.isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 tvItem.hItem = item.handle;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);*/
}
wresult _w_tree_deselect_all(w_tree *tree) {
	/*
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 if ((style & SWT.SINGLE) != 0) {
	 long  hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hItem != 0) {
	 tvItem.hItem = hItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 } else {
	 long  oldProc = OS.GetWindowLongPtr (handle, OS.GWLP_WNDPROC);
	 OS.SetWindowLongPtr (handle, OS.GWLP_WNDPROC, TreeProc);
	 if ((style & SWT.VIRTUAL) != 0) {
	 long  hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_ROOT, 0);
	 deselect (hItem, tvItem, 0);
	 } else {
	 for (int i=0; i<items.length; i++) {
	 TreeItem item = items [i];
	 if (item != null) {
	 tvItem.hItem = item.handle;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 }
	 }
	 OS.SetWindowLongPtr (handle, OS.GWLP_WNDPROC, oldProc);
	 }*/
}
int _w_tree_get_gridline_width(w_tree *tree) {
	//return GRID_WIDTH;
}
int _w_tree_get_header_height(w_tree *tree) {
	/*if (hwndHeader == 0) return 0;
	 RECT rect = new RECT ();
	 OS.GetWindowRect (hwndHeader, rect);
	 return rect.bottom - rect.top;*/
}
wresult _w_tree_get_header_visible(w_tree *tree) {
	/*if (hwndHeader == 0) return false;
	 int bits = OS.GetWindowLong (hwndHeader, OS.GWL_STYLE);
	 return (bits & OS.WS_VISIBLE) != 0;*/
}
wresult _w_tree_get_column(w_tree *tree, int index, w_treecolumn *column) {
	if (column == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_TREE(tree)->hwndHeader == 0) {
		wresult result = _w_tree_create_parent(tree);
		if (result <= 0)
			return result;
	}
	int columnCount = SendMessageW(_W_TREE(tree)->hwndHeader,
	HDM_GETITEMCOUNT, 0, 0);
	if (index >= columnCount)
		return W_ERROR_INVALID_RANGE;
	_W_TREECOLUMN(column)->tree = tree;
	_W_TREECOLUMN(column)->index = 0;
	_W_TREECOLUMN(column)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_TREE_GET_CLASS(tree)->class_treecolumn);
	return W_TRUE;
}
int _w_tree_get_column_count(w_tree *tree) {
	if (_W_TREE(tree)->hwndHeader == 0)
		return 1;
	return SendMessageW(_W_TREE(tree)->hwndHeader,
	HDM_GETITEMCOUNT, 0, 0);
}
int _w_tree_get_column_order(w_tree *tree, int *orders, size_t length) {
	if (_W_TREE(tree)->hwndHeader == 0) {
		if (orders != 0 && length >= 1)
			orders[0] = 0;
		return 1;
	} else {
		SendMessageW(_W_TREE(tree)->hwndHeader, HDM_GETORDERARRAY, length,
				(LPARAM) orders);
		return SendMessageW(_W_TREE(tree)->hwndHeader,
		HDM_GETITEMCOUNT, 0, 0);
	}
}
void _w_tree_get_columns(w_tree *tree, w_iterator *columns) {
}
w_imagelist* _w_tree_get_imagelist(w_tree *tree) {
	return _W_TREE(tree)->imagelist;
}
wresult _w_tree_get_item(w_tree *tree, w_point *point, w_treeitem *item) {
	TVHITTESTINFO lpht;
	lpht.pt.x = point->x;
	lpht.pt.y = point->y;
	SendMessageW(_W_WIDGET(tree)->handle, TVM_HITTEST, 0, (LPARAM) &lpht);
	if (lpht.hItem != 0) {
		int flags = TVHT_ONITEM;
		if ((_W_WIDGET(tree)->style & W_FULL_SELECTION) != 0) {
			flags |= TVHT_ONITEMRIGHT | TVHT_ONITEMINDENT;
		} else {
			/*if (hooks (SWT.MeasureItem)) {
			 lpht.flags &= ~(TVHT_ONITEMICON | TVHT_ONITEMLABEL);
			 if (hitTestSelection (lpht.hItem, lpht.x, lpht.y)) {
			 lpht.flags |= TVHT_ONITEMICON | TVHT_ONITEMLABEL;
			 }
			 }*/
		}
		if ((lpht.flags & flags) != 0) {
			_W_TREEITEM(item)->htreeitem = lpht.hItem;
			_W_TREEITEM(item)->tree = tree;
			_W_TREEITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					W_TREE_GET_CLASS(tree)->class_treeitem);
			return W_TRUE;
		}
	}
	return W_FALSE;
}
int _w_tree_get_item_height(w_tree *tree) {
	return SendMessageW(_W_WIDGET(tree)->handle, TVM_GETITEMHEIGHT, 0, 0);
}
wresult _w_tree_get_lines_visible(w_tree *tree) {
	return _W_TREE(tree)->linesVisible;
}
wresult _w_tree_get_parent_item(w_tree *tree, w_treeitem *item) {
	return W_FALSE;
}
wresult _w_tree_get_root_item(w_tree *tree, w_treeitem *root) {
	_W_TREEITEM(root)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_TREE_GET_CLASS(tree)->class_treeitem);
	_W_TREEITEM(root)->htreeitem = TVI_ROOT;
	_W_TREEITEM(root)->tree = tree;
	return W_TRUE;
}
void _w_tree_get_selection(w_tree *tree, w_iterator *selection) {
}
int _w_tree_get_selection_count(w_tree *tree) {
	/*if ((style & SWT.SINGLE) != 0) {
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hItem == 0) return 0;
	 int state = 0;
	 if (OS.IsWinCE) {
	 TVITEM tvItem = new TVITEM ();
	 tvItem.hItem = hItem;
	 tvItem.mask = OS.TVIF_STATE;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 state = tvItem.state;
	 } else {
	 state = (int)OS.SendMessage (handle, OS.TVM_GETITEMSTATE, hItem, OS.TVIS_SELECTED);
	 }
	 return (state & OS.TVIS_SELECTED) == 0 ? 0 : 1;
	 }
	 int count = 0;
	 long oldProc = OS.GetWindowLongPtr (handle, OS.GWLP_WNDPROC);
	 TVITEM tvItem = null;
	 if (OS.IsWinCE) {
	 tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_STATE;
	 }
	 OS.SetWindowLongPtr (handle, OS.GWLP_WNDPROC, TreeProc);
	 if ((style & SWT.VIRTUAL) != 0) {
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_ROOT, 0);
	 count = getSelection (hItem, tvItem, null, 0, -1, false, true);
	 } else {
	 for (int i=0; i<items.length; i++) {
	 TreeItem item = items [i];
	 if (item != null) {
	 long hItem = item.handle;
	 int state = 0;
	 if (OS.IsWinCE) {
	 tvItem.hItem = hItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 state = tvItem.state;
	 } else {
	 state = (int)OS.SendMessage (handle, OS.TVM_GETITEMSTATE, hItem, OS.TVIS_SELECTED);
	 }
	 if ((state & OS.TVIS_SELECTED) != 0) count++;
	 }
	 }
	 }
	 OS.SetWindowLongPtr (handle, OS.GWLP_WNDPROC, oldProc);
	 return count;*/
}
wresult _w_tree_get_sort_column(w_tree *tree, w_treecolumn *column) {
	if (_W_TREE(tree)->sortColumn != 0) {
		_W_TREECOLUMN(column)->tree = tree;
		_W_TREECOLUMN(column)->index = _W_TREE(tree)->sortColumn;
		_W_TREECOLUMN(column)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(tree)->class_treecolumn);
		return W_TRUE;
	} else
		return W_FALSE;
}
int _w_tree_get_sort_direction(w_tree *tree) {
	return _W_TREE(tree)->sortDirection;
}
wresult _w_tree_get_top_item(w_tree *tree, w_treeitem *topitem) {
	HTREEITEM hItem =(HTREEITEM) SendMessageW(_W_WIDGET(tree)->handle, TVM_GETNEXTITEM,
	TVGN_FIRSTVISIBLE, 0);
	if (hItem != 0) {
		_W_TREEITEM(topitem)->tree = tree;
		_W_TREEITEM(topitem)->htreeitem = hItem;
		_W_TREEITEM(topitem)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(tree)->class_treeitem);
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_tree_insert_column(w_tree *tree, w_treecolumn *column,
		const char *text, int index) {
	if (_W_TREE(tree)->hwndHeader == 0) {
		wresult result = _w_tree_create_parent(tree);
		if (result <= 0)
			return result;
	}
	if (text == 0)
		text = "";
	int columnCount = SendMessageW(_W_TREE(tree)->hwndHeader,
	HDM_GETITEMCOUNT, 0, 0);
	if (index < 0)
		index = columnCount;
	else if (index > columnCount)
		return W_ERROR_INVALID_RANGE;
	HDITEMW hdItem;
	struct UnicodeString str;
	unicode_set(&str, text, -1);
	hdItem.mask = HDI_TEXT | HDI_WIDTH;
	hdItem.pszText = str.str;
	hdItem.cchTextMax = str.length;
	hdItem.cxy = 50;
	index = SendMessageW(_W_TREE(tree)->hwndHeader, HDM_INSERTITEMW, index,
			(LPARAM) &hdItem);
	unicode_free(&str);
	if (index < 0)
		return W_ERROR_ITEM_NOT_ADDED;
	if (column != 0) {
		_W_TREECOLUMN(column)->tree = tree;
		_W_TREECOLUMN(column)->index = index;
		_W_TREECOLUMN(column)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(tree)->class_treecolumn);

	}
	/* When the first column is created, hide the horizontal scroll bar */
	if (columnCount == 0) {
		//scrollWidth = 0;
		if ((_W_WIDGET(tree)->style & W_HSCROLL) != 0) {
			int bits = GetWindowLong(_W_WIDGET(tree)->handle, GWL_STYLE);
			bits |= TVS_NOHSCROLL;
			SetWindowLong(_W_WIDGET(tree)->handle, GWL_STYLE, bits);
		}
		/*
		 * Bug in Windows.  When TVS_NOHSCROLL is set after items
		 * have been inserted into the tree, Windows shows the
		 * scroll bar.  The fix is to check for this case and
		 * explicitly hide the scroll bar explicitly.
		 */
		int count = SendMessage(_W_WIDGET(tree)->handle, TVM_GETCOUNT, 0, 0);
		if (count != 0) {
			/*if (!OS.IsWinCE)*/ShowScrollBar(_W_WIDGET(tree)->handle, SB_HORZ,
			FALSE);
		}
		/*createItemToolTips();
		 if (itemToolTipHandle != 0) {
		 OS.SendMessage (itemToolTipHandle, OS.TTM_SETDELAYTIME, OS.TTDT_AUTOMATIC, -1);
		 }*/
	}
	_w_tree_set_scroll_width(tree);
	//updateImageList();
	_w_tree_update_scrollbar(tree);

	/* Redraw to hide the items when the first column is created */
	if (columnCount == 0
			&& SendMessage(_W_WIDGET(tree)->handle, TVM_GETCOUNT, 0, 0) != 0) {
		InvalidateRect(_W_WIDGET(tree)->handle, NULL, TRUE);
	}

	/* Add the tool tip item for the header */
	/*if (headerToolTipHandle != 0) {
	 RECT rect = new
	 RECT();
	 if (OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, index, rect) != 0) {
	 TOOLINFO lpti = new
	 TOOLINFO();
	 lpti.cbSize = TOOLINFO.sizeof;
	 lpti.uFlags = OS.TTF_SUBCLASS;
	 lpti.hwnd = hwndHeader;
	 lpti.uId = column.id = display.nextToolTipId++;
	 lpti.left = rect.left;
	 lpti.top = rect.top;
	 lpti.right = rect.right;
	 lpti.bottom = rect.bottom;
	 lpti.lpszText = OS.LPSTR_TEXTCALLBACK;
	 OS.SendMessage (headerToolTipHandle, OS.TTM_ADDTOOL, 0, lpti);
	 }*/
	return W_TRUE;
}
wresult _w_tree_remove_all(w_tree *tree) {
}
wresult _w_tree_set_imagelist(w_tree *tree, w_imagelist *imagelist) {
	HIMAGELIST list;
	int ret = W_TRUE;
	if (imagelist == 0) {
		list = 0;
	} else {
		list = _W_IMAGELIST(imagelist)->imagelist;
		if (list == 0) {
			imagelist = 0;
			ret = W_ERROR_INVALID_ARGUMENT;
		}
	}
	_W_TREE(tree)->imagelist = imagelist;
	SendMessageW(_W_WIDGET(tree)->handle, TVM_SETIMAGELIST, TVSIL_NORMAL,
			(LPARAM) list);
	return ret;
}
wresult _w_tree_set_insert_mark(w_tree *tree, w_treeitem *item,
		wresult before) {
	/*long hItem = 0;
	 if (item != null) {
	 if (item.isDisposed()) error(SWT.ERROR_INVALID_ARGUMENT);
	 hItem = item.handle;
	 }
	 hInsert = hItem;
	 insertAfter = !before;
	 OS.SendMessage (handle, OS.TVM_SETINSERTMARK, insertAfter ? 1 : 0, hInsert);*/
}
wresult _w_tree_set_item_height(w_tree *tree, int itemHeight) {
	/*if (itemHeight < -1) error (SWT.ERROR_INVALID_ARGUMENT);
	 OS.SendMessage (handle, OS.TVM_SETITEMHEIGHT, itemHeight, 0);*/
}
wresult _w_tree_set_lines_visible(w_tree *tree, wresult show) {
	if (_W_TREE(tree)->linesVisible == show)
		return W_TRUE;
	_W_TREE(tree)->linesVisible = show;
	//if (_W_TREE(tree)->hwndParent == 0 && _W_TREE(tree)->linesVisible) customDraw = true;
	InvalidateRect(_W_WIDGET(tree)->handle, 0, TRUE);
	return W_TRUE;
}
wresult _w_tree_select(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_select_all(w_tree *tree) {
}
wresult _w_tree_set_column_order(w_tree *tree, int *order, size_t length) {
	/*if (order == null) error (SWT.ERROR_NULL_ARGUMENT);
	 if (columnCount == 0) {
	 if (order.length != 0) error (SWT.ERROR_INVALID_ARGUMENT);
	 return;
	 }
	 if (order.length != columnCount) error (SWT.ERROR_INVALID_ARGUMENT);
	 int [] oldOrder = new int [columnCount];
	 OS.SendMessage (hwndHeader, OS.HDM_GETORDERARRAY, columnCount, oldOrder);
	 boolean reorder = false;
	 boolean [] seen = new boolean [columnCount];
	 for (int i=0; i<order.length; i++) {
	 int index = order [i];
	 if (index < 0 || index >= columnCount) error (SWT.ERROR_INVALID_RANGE);
	 if (seen [index]) error (SWT.ERROR_INVALID_ARGUMENT);
	 seen [index] = true;
	 if (index != oldOrder [i]) reorder = true;
	 }
	 if (reorder) {
	 RECT [] oldRects = new RECT [columnCount];
	 for (int i=0; i<columnCount; i++) {
	 oldRects [i] = new RECT ();
	 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, i, oldRects [i]);
	 }
	 OS.SendMessage (hwndHeader, OS.HDM_SETORDERARRAY, order.length, order);
	 OS.InvalidateRect (handle, null, true);
	 updateImageList ();
	 TreeColumn [] newColumns = new TreeColumn [columnCount];
	 System.arraycopy (columns, 0, newColumns, 0, columnCount);
	 RECT newRect = new RECT ();
	 for (int i=0; i<columnCount; i++) {
	 TreeColumn column = newColumns [i];
	 if (!column.isDisposed ()) {
	 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, i, newRect);
	 if (newRect.left != oldRects [i].left) {
	 column.updateToolTip (i);
	 column.sendEvent (SWT.Move);
	 }
	 }
	 }
	 }*/
}
wresult _w_tree_set_header_visible(w_tree *tree, wresult show) {
	wresult result;
	if (_W_TREE(tree)->hwndHeader == 0) {
		if (!show)
			return W_TRUE;
		result = _w_tree_create_parent(tree);
		if (result <= 0)
			return result;
	}
	int bits = GetWindowLong(_W_TREE(tree)->hwndHeader, GWL_STYLE);
	if (show) {
		if ((bits & HDS_HIDDEN) == 0)
			return W_TRUE;
		bits &= ~HDS_HIDDEN;
		SetWindowLongW(_W_TREE(tree)->hwndHeader, GWL_STYLE, bits);
		ShowWindow(_W_TREE(tree)->hwndHeader, SW_SHOW);
	} else {
		if ((bits & HDS_HIDDEN) != 0)
			return W_TRUE;
		bits |= HDS_HIDDEN;
		SetWindowLongW(_W_TREE(tree)->hwndHeader, GWL_STYLE, bits);
		ShowWindow(_W_TREE(tree)->hwndHeader, SW_HIDE);
	}
	_w_tree_set_scroll_width(tree);
//updateHeaderToolTips();
	_w_tree_update_scrollbar(tree);
	return W_TRUE;
}
wresult _w_tree_set_selection(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_set_sort_column(w_tree *tree, w_treecolumn *column) {
	/*if (column != null && column.isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
	 if (sortColumn != null && !sortColumn.isDisposed ()) {
	 sortColumn.setSortDirection (SWT.NONE);
	 }
	 sortColumn = column;
	 if (sortColumn != null && sortDirection != SWT.NONE) {
	 sortColumn.setSortDirection (sortDirection);
	 }*/
}
wresult _w_tree_set_sort_direction(w_tree *tree, int direction) {
	/*if ((direction & (SWT.UP | SWT.DOWN)) == 0 && direction != SWT.NONE) return;
	 sortDirection = direction;
	 if (sortColumn != null && !sortColumn.isDisposed ()) {
	 sortColumn.setSortDirection (direction);
	 }*/
}
wresult _w_tree_set_top_item(w_tree *tree, w_treeitem *item) {
	/*if (item == null) error (SWT.ERROR_NULL_ARGUMENT);
	 if (item.isDisposed ()) error (SWT.ERROR_INVALID_ARGUMENT);
	 long  hItem = item.handle;
	 long hTopItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_FIRSTVISIBLE, 0);
	 if (hItem == hTopItem) return;
	 boolean fixScroll = checkScroll (hItem), redraw = false;
	 if (fixScroll) {
	 OS.SendMessage (handle, OS.WM_SETREDRAW, 1, 0);
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 0, 0);
	 } else {
	 redraw = getDrawing () && OS.IsWindowVisible (handle);
	 if (redraw) OS.DefWindowProc (handle, OS.WM_SETREDRAW, 0, 0);
	 }
	 SCROLLINFO hInfo = null;
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 long  hParent = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_PARENT, hItem);
	 if (hParent != 0 && (bits & (OS.TVS_NOHSCROLL | OS.TVS_NOSCROLL)) == 0) {
	 hInfo = new SCROLLINFO ();
	 hInfo.cbSize = SCROLLINFO.sizeof;
	 hInfo.fMask = OS.SIF_ALL;
	 OS.GetScrollInfo (handle, OS.SB_HORZ, hInfo);
	 }
	 OS.SendMessage (handle, OS.TVM_SELECTITEM, OS.TVGN_FIRSTVISIBLE, hItem);
	 if (hParent != 0) {
	 if (hInfo != null) {
	 long  hThumb = OS.MAKELPARAM (OS.SB_THUMBPOSITION, hInfo.nPos);
	 OS.SendMessage (handle, OS.WM_HSCROLL, hThumb, 0);
	 }
	 } else {
	 OS.SendMessage (handle, OS.WM_HSCROLL, OS.SB_TOP, 0);
	 }
	 if (fixScroll) {
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 1, 0);
	 OS.SendMessage (handle, OS.WM_SETREDRAW, 0, 0);
	 } else {
	 if (redraw) {
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 1, 0);
	 OS.InvalidateRect (handle, null, true);
	 }
	 }
	 updateScrollBar ();*/
}
wresult _w_tree_show_column(w_tree *tree, w_treecolumn *column) {
	/*if (column == null) error (SWT.ERROR_NULL_ARGUMENT);
	 if (column.isDisposed ()) error(SWT.ERROR_INVALID_ARGUMENT);
	 if (column.parent != this) return;
	 int index = indexOf (column);
	 if (index == -1) return;
	 if (0 <= index && index < columnCount) {
	 forceResize ();
	 RECT rect = new RECT ();
	 OS.GetClientRect (hwndParent, rect);
	 OS.MapWindowPoints (hwndParent, handle, rect, 2);
	 RECT headerRect = new RECT ();
	 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, index, headerRect);
	 boolean scroll = headerRect.left < rect.left;
	 if (!scroll) {
	 int width = Math.min (rect.right - rect.left, headerRect.right - headerRect.left);
	 scroll = headerRect.left + width > rect.right;
	 }
	 if (scroll) {
	 SCROLLINFO info = new SCROLLINFO ();
	 info.cbSize = SCROLLINFO.sizeof;
	 info.fMask = OS.SIF_POS;
	 info.nPos = Math.max (0, headerRect.left - Tree.INSET / 2);
	 OS.SetScrollInfo (hwndParent, OS.SB_HORZ, info, true);
	 setScrollWidth ();
	 }
	 }*/
}
wresult _w_tree_show_item(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_show_selection(w_tree *tree) {
}
wresult _w_tree_sort(w_tree *tree) {
}
/*
 * overrided
 */
wresult _w_tree_set_bounds(w_control *control, w_rect *rect, int flags) {
	wresult result = _w_control_set_bounds(control, rect, flags);
	_w_tree_set_scroll_width(W_TREE(control));
	return result;
}
#define TVS_EX_DOUBLEBUFFER 0x0004
#define TVS_EX_RICHTOOLTIP 0x0010
#define TVS_EX_FADEINOUTEXPANDOS 0x0040
wresult _w_tree_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	wresult result;
	DWORD dwExStyle = 0, dwStyle = 0;
	_w_tree_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_TREEVIEWA, dwExStyle, dwStyle);
	if (result > 0) {
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
		_W_TREE(widget)->sortColumn = -1;
#if !IsWinCE
		/* Use the Explorer theme */
		if (EXPLORER_THEME) {

			if (WIN32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
				_W_TREE(widget)->explorerTheme = TRUE;
				SetWindowTheme(_W_WIDGET(widget)->handle, L"EXPLORER", NULL);
				int bits = TVS_EX_DOUBLEBUFFER | TVS_EX_RICHTOOLTIP;
				//if (ENABLE_TVS_EX_FADEINOUTEXPANDOS) bits |= TVS_EX_FADEINOUTEXPANDOS;
				/*
				 * This code is intentionally commented.
				 */
				//			if ((style & SWT.FULL_SELECTION) == 0) bits |= OS.TVS_EX_AUTOHSCROLL;
				SendMessageW(_W_WIDGET(widget)->handle, TVM_SETEXTENDEDSTYLE,
						0, bits);
				/*
				 * Bug in Windows.  When the tree is using the explorer
				 * theme, it does not use COLOR_WINDOW_TEXT for the
				 * default foreground color.  The fix is to explicitly
				 * set the foreground.
				 */
				//setForegroundPixel (-1);
			}
		}
#endif

		/*
		 * Feature in Windows.  In version 5.8 of COMCTL32.DLL,
		 * if the font is changed for an item, the bounds for the
		 * item are not updated, causing the text to be clipped.
		 * The fix is to detect the version of COMCTL32.DLL, and
		 * if it is one of the versions with the problem, then
		 * use version 5.00 of the control (a version that does
		 * not have the problem).  This is the recommended work
		 * around from the MSDN.
		 */
		/*if (!OS.IsWinCE) {
		 if (OS.COMCTL32_MAJOR < 6) {
		 OS.SendMessage (handle, OS.CCM_SETVERSION, 5, 0);
		 }
		 }*/

		/* Set the checkbox image list */
		//if ((style & SWT.CHECK) != 0) setCheckboxImageList ();
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
		HFONT hFont = GetStockObject(SYSTEM_FONT);
		//SendMessageW (_W_CONTROL(widget)->handle, WM_SETFONT,(WPARAM) hFont, 0);

		//createdAsRTL = (style & SWT.RIGHT_TO_LEFT) != 0;
	}
	return result;
}
void _w_tree_dispose(w_widget *resource) {
	_w_control_dispose(resource);
}
wresult _w_tree_is_item_selected(w_widget *widget, NMTVCUSTOMDRAW *nmcd) {
	wresult selected = W_FALSE;
	if (IsWindowEnabled(_W_WIDGET(widget)->handle)) {
		TVITEM tvItem;
		tvItem.mask = TVIF_HANDLE | TVIF_STATE;
		tvItem.hItem = (HTREEITEM) nmcd->nmcd.dwItemSpec;
		SendMessageW(_W_WIDGET(widget)->handle, TVM_GETITEM, 0,
				(LPARAM) &tvItem);
		if ((tvItem.state & (TVIS_SELECTED | TVIS_DROPHILITED)) != 0) {
			selected = W_TRUE;
			/*
			 * Feature in Windows.  When the mouse is pressed and the
			 * selection is first drawn for a tree, the previously
			 * selected item is redrawn but the the TVIS_SELECTED bits
			 * are not cleared.  When the user moves the mouse slightly
			 * and a drag and drop operation is not started, the item is
			 * drawn again and this time with TVIS_SELECTED is cleared.
			 * This means that an item that contains colored cells will
			 * not draw with the correct background until the mouse is
			 * moved.  The fix is to test for the selection colors and
			 * guess that the item is not selected.
			 *
			 * NOTE: This code does not work when the foreground and
			 * background of the tree are set to the selection colors
			 * but this does not happen in a regular application.
			 */
			if (_W_WIDGET(widget)->handle == GetFocus()) {
				if (GetTextColor(nmcd->nmcd.hdc)
						!= GetSysColor(COLOR_HIGHLIGHTTEXT)) {
					selected = W_FALSE;
				} else {
					if (GetBkColor(nmcd->nmcd.hdc)
							!= GetSysColor(COLOR_HIGHLIGHT)) {
						selected = W_FALSE;
					}
				}
			}
		} else {
			if (nmcd->nmcd.dwDrawStage == CDDS_ITEMPOSTPAINT) {
				/*
				 * Feature in Windows.  When the mouse is pressed and the
				 * selection is first drawn for a tree, the item is drawn
				 * selected, but the TVIS_SELECTED bits for the item are
				 * not set.  When the user moves the mouse slightly and
				 * a drag and drop operation is not started, the item is
				 * drawn again and this time TVIS_SELECTED is set.  This
				 * means that an item that is in a tree that has the style
				 * TVS_FULLROWSELECT and that also contains colored cells
				 * will not draw the entire row selected until the user
				 * moves the mouse.  The fix is to test for the selection
				 * colors and guess that the item is selected.
				 *
				 * NOTE: This code does not work when the foreground and
				 * background of the tree are set to the selection colors
				 * but this does not happen in a regular application.
				 */
				if (GetTextColor(nmcd->nmcd.hdc)
						== GetSysColor(COLOR_HIGHLIGHTTEXT)) {
					if (GetBkColor(nmcd->nmcd.hdc)
							== GetSysColor(COLOR_HIGHLIGHT)) {
						selected = W_TRUE;
					}
				}
			}
		}
	}
	return selected;
}
HWND _w_tree_scrolled_handle(w_widget *widget) {
	if (_W_TREE(widget)->hwndHeader == 0)
		return _W_WIDGET(widget)->handle;
	return /*columnCount == 0 &&*/
	_W_TREE(widget)->scrollWidth == 0 ?
			_W_WIDGET(widget)->handle :
										_W_TREE(widget)->hwndParent;
}

void _w_tree_class_init(struct _w_tree_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));

	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TREE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tree_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tree);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tree);
	W_WIDGET_CLASS(clazz)->post_event = _w_tree_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_tree_create;
	W_WIDGET_CLASS(clazz)->dispose = _w_tree_dispose;
	W_CONTROL_CLASS(clazz)->set_bounds = _w_tree_set_bounds;

	clazz->deselect = _w_tree_deselect;
	clazz->deselect_all = _w_tree_deselect_all;
	clazz->get_gridline_width = _w_tree_get_gridline_width;
	clazz->get_header_height = _w_tree_get_header_height;
	clazz->get_header_visible = _w_tree_get_header_visible;
	clazz->get_column = _w_tree_get_column;
	clazz->get_column_count = _w_tree_get_column_count;
	clazz->get_columns = _w_tree_get_columns;
	clazz->get_imagelist = _w_tree_get_imagelist;
	clazz->get_item = _w_tree_get_item;
	clazz->get_item_height = _w_tree_get_item_height;
	clazz->get_lines_visible = _w_tree_get_lines_visible;
	clazz->get_parent_item = _w_tree_get_parent_item;
	clazz->get_root_item = _w_tree_get_root_item;
	clazz->get_selection = _w_tree_get_selection;
	clazz->get_selection_count = _w_tree_get_selection_count;
	clazz->get_sort_column = _w_tree_get_sort_column;
	clazz->get_sort_direction = _w_tree_get_sort_direction;
	clazz->get_top_item = _w_tree_get_top_item;
	clazz->insert_column = _w_tree_insert_column;
	clazz->remove_all = _w_tree_remove_all;
	clazz->set_imagelist = _w_tree_set_imagelist;
	clazz->set_insert_mark = _w_tree_set_insert_mark;
	clazz->set_item_height = _w_tree_set_item_height;
	clazz->set_lines_visible = _w_tree_set_lines_visible;
	clazz->select = _w_tree_select;
	clazz->select_all = _w_tree_select_all;
	clazz->set_header_visible = _w_tree_set_header_visible;
	clazz->set_selection = _w_tree_set_selection;
	clazz->set_sort_column = _w_tree_set_sort_column;
	clazz->set_sort_direction = _w_tree_set_sort_direction;
	clazz->set_top_item = _w_tree_set_top_item;
	clazz->show_column = _w_tree_show_column;
	clazz->show_item = _w_tree_show_item;
	clazz->show_selection = _w_tree_show_selection;
	clazz->sort = _w_tree_sort;
	/*
	 * tree column
	 */
	struct _w_treecolumn_class *treecolumn = clazz->class_treecolumn;
	treecolumn->get_alignment = _w_treecolumn_get_alignment;
	treecolumn->get_id = _w_treecolumn_get_order;
	treecolumn->get_image = _w_treecolumn_get_image;
	treecolumn->get_moveable = _w_treecolumn_get_moveable;
	treecolumn->get_resizable = _w_treecolumn_get_resizable;
	treecolumn->get_tooltip_text = _w_treecolumn_get_tooltip_text;
	treecolumn->get_width = _w_treecolumn_get_width;
	treecolumn->pack = _w_treecolumn_pack;
	treecolumn->set_alignment = _w_treecolumn_set_alignment;
	treecolumn->set_id = _w_treecolumn_set_order;
	treecolumn->set_image = _w_treecolumn_set_image;
	treecolumn->set_moveable = _w_treecolumn_set_moveable;
	treecolumn->set_resizable = _w_treecolumn_set_resizable;
	treecolumn->set_tooltip_text = _w_treecolumn_set_tooltip_text;
	treecolumn->set_width = _w_treecolumn_set_width;
	treecolumn->item.get_data = _w_treecolumn_get_data;
	treecolumn->item.get_index = _w_treecolumn_get_index;
	treecolumn->item.get_text = _w_treecolumn_get_text;
	treecolumn->item.set_data = _w_treecolumn_set_data;
	treecolumn->item.set_text = _w_treecolumn_set_text;
	treecolumn->item.widgetdata.close = _w_widgetdata_close;
	treecolumn->item.widgetdata.is_ok = _w_widgetdata_is_ok;
//treecolumn->item.resourcedata.copy = _w_treeitem_copy;
//treecolumn->item.resourcedata.equals = _w_treeitem_equals;
	treecolumn->item.widgetdata.toolkit = (w_toolkit*) win_toolkit;
	/*
	 * tree item
	 */
	struct _w_treeitem_class *treeitem = clazz->class_treeitem;
	treeitem->clear = _w_treeitem_clear;
	treeitem->clear_all = _w_treeitem_clear_all;
	treeitem->get_bounds = _w_treeitem_get_bounds;
	treeitem->get_checked = _w_treeitem_get_checked;
	treeitem->get_expanded = _w_treeitem_get_expanded;
	treeitem->get_first_child = _w_treeitem_get_first_child;
	treeitem->get_grayed = _w_treeitem_get_grayed;
	treeitem->get_item = _w_treeitem_get_item;
	treeitem->get_item_count = _w_treeitem_get_item_count;
	treeitem->get_items = _w_treeitem_get_items;
	treeitem->get_image = _w_treeitem_get_image;
	treeitem->get_last_child = _w_treeitem_get_last_child;
	treeitem->get_next_sibling = _w_treeitem_get_next_sibling;
	treeitem->get_parent_item = _w_treeitem_get_parent_item;
	treeitem->get_prev_sibling = _w_treeitem_get_prev_sibling;
	treeitem->insert_item = _w_treeitem_insert_item;
	treeitem->insert_item_after = _w_treeitem_insert_item_after;
	treeitem->remove_all = _w_treeitem_remove_all;
	treeitem->set_checked = _w_treeitem_set_checked;
	treeitem->set_expanded = _w_treeitem_set_expanded;
	treeitem->set_grayed = _w_treeitem_set_grayed;
	treeitem->set_image = _w_treeitem_set_image;
	treeitem->set_has_children = _w_treeitem_set_has_children;
	treeitem->item.get_parent_widget = _w_treeitem_get_parent_widget;
	treeitem->item.get_data = _w_treeitem_get_data;
	treeitem->item.get_index = _w_treeitem_get_index;
	treeitem->item.get_text = _w_treeitem_get_text;
	treeitem->item.set_data = _w_treeitem_set_data;
	treeitem->item.set_text = _w_treeitem_set_text;
	treeitem->item.widgetdata.close = _w_widgetdata_close;
	treeitem->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	treeitem->item.widgetdata.copy = _w_treeitem_copy;
	treeitem->item.widgetdata.equals = _w_treeitem_equals;
	treeitem->item.widgetdata.toolkit = (w_toolkit*) win_toolkit;
	/*
	 * reserved
	 */
	struct _w_tree_reserved *reserved = _W_TREE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_CONTROL_RESERVED(reserved)->topHandle = _w_tree_top_handle;
	_W_CONTROL_RESERVED(reserved)->widget.def_proc = _w_tree_def_proc;
//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->messages[_WM_CHAR] = _TREE_WM_CHAR;
	msg->messages[_WM_ERASEBKGND] = _TREE_WM_ERASEBKGND;
	msg->messages[_WM_GETOBJECT] = _TREE_WM_GETOBJECT;
	msg->messages[_WM_HSCROLL] = _TREE_WM_HSCROLL;
	msg->messages[_WM_KEYDOWN] = _TREE_WM_KEYDOWN;
	msg->messages[_WM_KILLFOCUS] = _TREE_WM_KILLFOCUS;
	msg->messages[_WM_LBUTTONDBLCLK] = _TREE_WM_LBUTTONDBLCLK;
	msg->messages[_WM_LBUTTONDOWN] = _TREE_WM_LBUTTONDOWN;
	msg->messages[_WM_MOUSEMOVE] = _TREE_WM_MOUSEMOVE;
	msg->messages[_WM_MOUSEWHEEL] = _TREE_WM_MOUSEWHEEL;
	msg->messages[_WM_MOVE] = _TREE_WM_MOVE;
	msg->messages[_WM_RBUTTONDOWN] = _TREE_WM_RBUTTONDOWN;
	msg->messages[_WM_PAINT] = _TREE_WM_PAINT;
	msg->messages[_WM_SETCURSOR] = _TREE_WM_SETCURSOR;
	msg->messages[_WM_SETFOCUS] = _TREE_WM_SETFOCUS;
	msg->messages[_WM_SETFONT] = _TREE_WM_SETFONT;
	msg->messages[_WM_SETREDRAW] = _TREE_WM_SETREDRAW;
	msg->messages[_WM_SIZE] = _TREE_WM_SIZE;
	msg->messages[_WM_SYSCOLORCHANGE] = _TREE_WM_SYSCOLORCHANGE;
	msg->messages[_WM_VSCROLL] = _TREE_WM_VSCROLL;
	msg->messages[_WM_TIMER] = _TREE_WM_TIMER;
	msg->messages[_WM_CTLCOLORCHILD] = _TREE_WM_CTLCOLORCHILD;
	msg->messages[_WM_NOTIFY] = _TREE_WM_NOTIFY;
	msg->messages[_WM_NOTIFYCHILD] = _TREE_WM_NOTIFYCHILD;
}
#endif
