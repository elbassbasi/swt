/*
 * tree_msg.c
 *
 *  Created on: 27 mars 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "tree.h"
#include "../widgets/toolkit.h"
int _TREE_WM_NOTIFY_HEADER0(w_widget *widget, struct _w_event_platform *e,
		NMHEADERW *hdr) {
	switch (hdr->hdr.code) {
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
	case HDN_DIVIDERDBLCLICKW:
	case HDN_DIVIDERDBLCLICKA: {
		/*NMHEADER phdn = new NMHEADER ();
		 MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null && !column.getResizable ()) {
		 return LRESULT.ONE;
		 }
		 ignoreColumnMove = W_TRUE;
		 switch (hdr.code) {
		 case HDN_DIVIDERDBLCLICKW:
		 case HDN_DIVIDERDBLCLICKA:
		 if (column != null) column.pack ();
		 }*/
		break;
	}
	case NM_RELEASEDCAPTURE: {
		/*if (!ignoreColumnMove) {
		 for (int i=0; i<columnCount; i++) {
		 TreeColumn column = columns [i];
		 column.updateToolTip (i);
		 }
		 updateImageList ();
		 }
		 ignoreColumnMove = false;*/
		break;
	}
	case HDN_BEGINDRAG: {
		/*if (ignoreColumnMove) return LRESULT.ONE;
		 NMHEADER phdn = new NMHEADER ();
		 MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 if (phdn.iItem != -1) {
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null && !column.getMoveable ()) {
		 ignoreColumnMove = W_TRUE;
		 return LRESULT.ONE;
		 }
		 }*/
		break;
	}
	case HDN_ENDDRAG: {
		/*NMHEADER phdn = new NMHEADER ();
		 MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 if (phdn.iItem != -1 && phdn.pitem != 0) {
		 HDITEM pitem = new HDITEM ();
		 MoveMemory (pitem, phdn.pitem, HDITEM.sizeof);
		 if ((pitem.mask & HDI_ORDER) != 0 && pitem.iOrder != -1) {
		 int [] order = new int [columnCount];
		 SendMessage (hwndHeader, HDM_GETORDERARRAY, columnCount, order);
		 int index = 0;
		 while (index < order.length) {
		 if (order [index] == phdn.iItem) break;
		 index++;
		 }
		 if (index == order.length) index = 0;
		 if (index == pitem.iOrder) break;
		 int start = Math.min (index, pitem.iOrder);
		 int end = Math.max (index, pitem.iOrder);
		 RECT rect = new RECT (), headerRect = new RECT ();
		 GetClientRect (handle, rect);
		 SendMessage (hwndHeader, HDM_GETITEMRECT, order [start], headerRect);
		 rect.left = Math.max (rect.left, headerRect.left);
		 SendMessage (hwndHeader, HDM_GETITEMRECT, order [end], headerRect);
		 rect.right = Math.min (rect.right, headerRect.right);
		 InvalidateRect (handle, rect, W_TRUE);
		 ignoreColumnMove = false;
		 for (int i=start; i<=end; i++) {
		 TreeColumn column = columns [order [i]];
		 if (!column.isDisposed ()) {
		 column.postEvent (W_Move);
		 }
		 }
		 }
		 }*/
		break;
	}
	case HDN_ITEMCHANGINGW:
	case HDN_ITEMCHANGINGA: {
		if (hdr->pitem != 0) {
			if ((hdr->pitem->mask & HDI_WIDTH) != 0) {
				RECT rect;
				GetClientRect(_W_WIDGET(widget)->handle, &rect);
				HDITEMW oldItem;
				oldItem.mask = HDI_WIDTH;
				SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEMW,
						hdr->iItem, (LPARAM) &oldItem);
				int deltaX = hdr->pitem->cxy - oldItem.cxy;
				RECT headerRect;
				SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEMRECT,
						hdr->iItem, (LPARAM) &headerRect);
				int gridWidth = /*linesVisible ? GRID_WIDTH :*/0;
				rect.left = headerRect.right - gridWidth;
				int newX = rect.left + deltaX;
				rect.right = w_max(rect.right, rect.left + abs(deltaX));
				/*if (explorerTheme || (findImageControl () != null || hooks (W_MeasureItem) || hooks (W_EraseItem) || hooks (W_PaintItem))) {
				 rect.left -= GetSystemMetrics (SM_CXFOCUSBORDER);
				 InvalidateRect (handle, rect, W_TRUE);
				 OffsetRect (rect, deltaX, 0);
				 InvalidateRect (handle, rect, W_TRUE);
				 } else {*/
				int flags = SW_INVALIDATE | SW_ERASE;
				ScrollWindowEx(_W_WIDGET(widget)->handle, deltaX, 0, &rect,
				NULL, 0, NULL, flags);
				//}
				if (SendMessage(_W_TREE(widget)->hwndHeader, HDM_ORDERTOINDEX,
						hdr->iItem, 0) != 0) {
					rect.left = headerRect.left;
					rect.right = newX;
					InvalidateRect(_W_WIDGET(widget)->handle, &rect, TRUE);
				}
				_w_tree_set_scroll_width(W_TREE(widget));
			}
		}
		break;
	}
	case HDN_ITEMCHANGEDW:
	case HDN_ITEMCHANGEDA: {
		/*if (hdr->pitem != 0) {
		 HDITEM pitem = new
		 HDITEM();
		 MoveMemory (pitem, phdn.pitem, HDITEM.sizeof);
		 if ((pitem.mask & HDI_WIDTH) != 0) {
		 if (ignoreColumnMove) {
		 if (!IsWinCE && WIN32_VERSION >= VERSION(6, 0)) {
		 int flags = RDW_UPDATENOW | RDW_ALLCHILDREN;
		 RedrawWindow(handle, null, 0, flags);
		 } else {
		 if ((_W_WIDGET(widget)->style & W_DOUBLE_BUFFERED) == 0) {
		 int oldStyle = style;
		 style |= W_DOUBLE_BUFFERED;
		 UpdateWindow(handle);
		 style = oldStyle;
		 }
		 }
		 }
		 TreeColumn column = columns[phdn.iItem];
		 if (column != null) {
		 column.updateToolTip(phdn.iItem);
		 column.sendEvent(W_Resize);
		 if (isDisposed())
		 return LRESULT.ZERO;
		 TreeColumn [] newColumns = new TreeColumn [columnCount];
		 System.arraycopy(columns, 0, newColumns, 0, columnCount);
		 int [] order = new int [columnCount];
		 SendMessage(hwndHeader, HDM_GETORDERARRAY, columnCount,
		 order);
		 boolean moved = false;
		 for (int i = 0; i < columnCount; i++) {
		 TreeColumn nextColumn = newColumns[order[i]];
		 if (moved && !nextColumn.isDisposed()) {
		 nextColumn.updateToolTip(order[i]);
		 nextColumn.sendEvent(W_Move);
		 }
		 if (nextColumn == column)
		 moved = W_TRUE;
		 }
		 }
		 }
		 setScrollWidth();
		 }*/
		break;
	}
	case HDN_ITEMCLICKW:
	case HDN_ITEMCLICKA: {
		/*NMHEADER phdn = new NMHEADER ();
		 MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null) {
		 column.sendSelectionEvent (W_Selection);
		 }*/
		break;
	}
	case HDN_ITEMDBLCLICKW:
	case HDN_ITEMDBLCLICKA: {
		/*NMHEADER phdn = new NMHEADER ();
		 MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null) {
		 column.sendSelectionEvent (W_DefaultSelection);
		 }*/
		break;
	}
	}
	return W_FALSE;

}

int _TREE_WM_CHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_CHAR (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Feature in Windows.  The tree control beeps
	 * in WM_CHAR when the search for the item that
	 * matches the key stroke fails.  This is the
	 * standard tree behavior but is unexpected when
	 * the key that was typed was ESC, CR or SPACE.
	 * The fix is to avoid calling the tree window
	 * proc in these cases.
	 */
	/*switch ((int)wParam) {
	 case ' ': {
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hItem != 0) {
	 hAnchor = hItem;
	 OS.SendMessage (handle, OS.TVM_ENSUREVISIBLE, 0, hItem);
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE | OS.TVIF_PARAM;
	 tvItem.hItem = hItem;
	 if ((style & SWT.CHECK) != 0) {
	 tvItem.stateMask = OS.TVIS_STATEIMAGEMASK;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 int state = tvItem.state >> 12;
	 if ((state & 0x1) != 0) {
	 state++;
	 } else  {
	 --state;
	 }
	 tvItem.state = state << 12;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 long id = OS.SendMessage (handle, OS.TVM_MAPHTREEITEMTOACCID, hItem, 0);
	 OS.NotifyWinEvent (OS.EVENT_OBJECT_FOCUS, handle, OS.OBJID_CLIENT, (int)id);
	 }
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 if ((style & SWT.MULTI) != 0 && OS.GetKeyState (OS.VK_CONTROL) < 0) {
	 if ((tvItem.state & OS.TVIS_SELECTED) != 0) {
	 tvItem.state &= ~OS.TVIS_SELECTED;
	 } else {
	 tvItem.state |= OS.TVIS_SELECTED;
	 }
	 } else {
	 tvItem.state |= OS.TVIS_SELECTED;
	 }
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 TreeItem item = _getItem (hItem, (int)tvItem.lParam);
	 Event event = new Event ();
	 event.item = item;
	 sendSelectionEvent (SWT.Selection, event, false);
	 if ((style & SWT.CHECK) != 0) {
	 event = new Event ();
	 event.item = item;
	 event.detail = SWT.CHECK;
	 sendSelectionEvent (SWT.Selection, event, false);
	 }
	 }
	 return LRESULT.ZERO;
	 }
	 case SWT.CR: {*/
	/*
	 * Feature in Windows.  Windows sends NM_RETURN from WM_KEYDOWN
	 * instead of using WM_CHAR.  This means that application code
	 * that expects to consume the key press and therefore avoid a
	 * SWT.DefaultSelection event from WM_CHAR will fail.  The fix
	 * is to implement SWT.DefaultSelection in WM_CHAR instead of
	 * using NM_RETURN.
	 */
	/*Event event = new Event ();
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hItem != 0) event.item = _getItem (hItem);
	 sendSelectionEvent (SWT.DefaultSelection, event, false);
	 return LRESULT.ZERO;
	 }
	 case SWT.ESC:
	 return LRESULT.ZERO;
	 }
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_ERASEBKGND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_ERASEBKGND (wParam, lParam);
	 if ((style & SWT.DOUBLE_BUFFERED) != 0) return LRESULT.ONE;
	 if (findImageControl () != null) return LRESULT.ONE;
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_GETOBJECT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Ensure that there is an accessible object created for this
	 * control because support for checked item and tree column
	 * accessibility is temporarily implemented in the accessibility
	 * package.
	 */
	/*if ((style & SWT.CHECK) != 0 || hwndParent != 0) {
	 if (accessible == null) accessible = new_Accessible (this);
	 }
	 return super.WM_GETOBJECT (wParam, lParam);*/
	return W_FALSE;
}

int _TREE_WM_HSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*boolean fixScroll = false;
	 if ((style & SWT.DOUBLE_BUFFERED) != 0) {
	 fixScroll = (style & SWT.VIRTUAL) != 0 || hooks (SWT.EraseItem) || hooks (SWT.PaintItem);
	 }
	 if (fixScroll) {
	 style &= ~SWT.DOUBLE_BUFFERED;
	 if (explorerTheme) {
	 OS.SendMessage (handle, OS.TVM_SETEXTENDEDSTYLE, OS.TVS_EX_DOUBLEBUFFER, 0);
	 }
	 }
	 LRESULT result = super.WM_HSCROLL (wParam, lParam);
	 if (fixScroll) {
	 style |= SWT.DOUBLE_BUFFERED;
	 if (explorerTheme) {
	 OS.SendMessage (handle, OS.TVM_SETEXTENDEDSTYLE, OS.TVS_EX_DOUBLEBUFFER, OS.TVS_EX_DOUBLEBUFFER);
	 }
	 }
	 if (result != null) return result;
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_KEYDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_KEYDOWN (wParam, lParam);
	 if (result != null) return result;
	 switch ((int)wParam) {
	 case OS.VK_LEFT:
	 case OS.VK_RIGHT:*/
	/*
	 * Bug in Windows. The behavior for the left and right keys is not
	 * changed if the orientation changes after the control was created.
	 * The fix is to replace VK_LEFT by VK_RIGHT and VK_RIGHT by VK_LEFT
	 * when the current orientation differs from the orientation used to
	 * create the control.
	 */
	/* boolean isRTL = (style & SWT.RIGHT_TO_LEFT) != 0;
	 if (isRTL != createdAsRTL) {
	 long code = callWindowProc (handle, OS.WM_KEYDOWN, wParam == OS.VK_RIGHT ? OS.VK_LEFT : OS.VK_RIGHT, lParam);
	 return new LRESULT (code);
	 }
	 break;
	 case OS.VK_SPACE:*/
	/*
	 * Ensure that the window proc does not process VK_SPACE
	 * so that it can be handled in WM_CHAR.  This allows the
	 * application to cancel an operation that is normally
	 * performed in WM_KEYDOWN from WM_CHAR.
	 */
	/*return LRESULT.ZERO;
	 case OS.VK_ADD:
	 if (OS.GetKeyState (OS.VK_CONTROL) < 0) {
	 if (hwndHeader != 0) {
	 TreeColumn [] newColumns = new TreeColumn [columnCount];
	 System.arraycopy (columns, 0, newColumns, 0, columnCount);
	 for (int i=0; i<columnCount; i++) {
	 TreeColumn column = newColumns [i];
	 if (!column.isDisposed () && column.getResizable ()) {
	 column.pack ();
	 }
	 }
	 }
	 }
	 break;
	 case OS.VK_UP:
	 case OS.VK_DOWN:
	 case OS.VK_PRIOR:
	 case OS.VK_NEXT:
	 case OS.VK_HOME:
	 case OS.VK_END: {
	 OS.SendMessage (handle, OS.WM_CHANGEUISTATE, OS.UIS_INITIALIZE, 0);
	 if (itemToolTipHandle != 0) OS.ShowWindow (itemToolTipHandle, OS.SW_HIDE);
	 if ((style & SWT.SINGLE) != 0) break;
	 if (OS.GetKeyState (OS.VK_SHIFT) < 0) {
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hItem != 0) {
	 if (hAnchor == 0) hAnchor = hItem;
	 ignoreSelect = ignoreDeselect = true;
	 long code = callWindowProc (handle, OS.WM_KEYDOWN, wParam, lParam);
	 ignoreSelect = ignoreDeselect = false;
	 long hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 long hDeselectItem = hItem;
	 RECT rect1 = new RECT ();
	 if (!OS.TreeView_GetItemRect (handle, hAnchor, rect1, false)) {
	 hAnchor = hItem;
	 OS.TreeView_GetItemRect (handle, hAnchor, rect1, false);
	 }
	 RECT rect2 = new RECT ();
	 OS.TreeView_GetItemRect (handle, hDeselectItem, rect2, false);
	 int flags = rect1.top < rect2.top ? OS.TVGN_PREVIOUSVISIBLE : OS.TVGN_NEXTVISIBLE;
	 while (hDeselectItem != hAnchor) {
	 tvItem.hItem = hDeselectItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 hDeselectItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, flags, hDeselectItem);
	 }
	 long hSelectItem = hAnchor;
	 OS.TreeView_GetItemRect (handle, hNewItem, rect1, false);
	 OS.TreeView_GetItemRect (handle, hSelectItem, rect2, false);
	 tvItem.state = OS.TVIS_SELECTED;
	 flags = rect1.top < rect2.top ? OS.TVGN_PREVIOUSVISIBLE : OS.TVGN_NEXTVISIBLE;
	 while (hSelectItem != hNewItem) {
	 tvItem.hItem = hSelectItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 hSelectItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, flags, hSelectItem);
	 }
	 tvItem.hItem = hNewItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_PARAM;
	 tvItem.hItem = hNewItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 Event event = new Event ();
	 event.item = _getItem (hNewItem, (int)tvItem.lParam);
	 sendSelectionEvent (SWT.Selection, event, false);
	 return new LRESULT (code);
	 }
	 }
	 if (OS.GetKeyState (OS.VK_CONTROL) < 0) {
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hItem != 0) {
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 tvItem.hItem = hItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 boolean oldSelected = (tvItem.state & OS.TVIS_SELECTED) != 0;
	 long  hNewItem = 0;
	 switch ((int)wParam) {
	 case OS.VK_UP:
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_PREVIOUSVISIBLE, hItem);
	 break;
	 case OS.VK_DOWN:
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_NEXTVISIBLE, hItem);
	 break;
	 case OS.VK_HOME:
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_ROOT, 0);
	 break;
	 case OS.VK_PRIOR:
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_FIRSTVISIBLE, 0);
	 if (hNewItem == hItem) {
	 OS.SendMessage (handle, OS.WM_VSCROLL, OS.SB_PAGEUP, 0);
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_FIRSTVISIBLE, 0);
	 }
	 break;
	 case OS.VK_NEXT:
	 RECT rect = new RECT (), clientRect = new RECT ();
	 OS.GetClientRect (handle, clientRect);
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_FIRSTVISIBLE, 0);
	 do {
	 long hVisible = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_NEXTVISIBLE, hNewItem);
	 if (hVisible == 0) break;
	 if (!OS.TreeView_GetItemRect (handle, hVisible, rect, false)) break;
	 if (rect.bottom > clientRect.bottom) break;
	 if ((hNewItem = hVisible) == hItem) {
	 OS.SendMessage (handle, OS.WM_VSCROLL, OS.SB_PAGEDOWN, 0);
	 }
	 } while (hNewItem != 0);
	 break;
	 case OS.VK_END:
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_LASTVISIBLE, 0);
	 break;
	 }
	 if (hNewItem != 0) {
	 OS.SendMessage (handle, OS.TVM_ENSUREVISIBLE, 0, hNewItem);
	 tvItem.hItem = hNewItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 boolean newSelected = (tvItem.state & OS.TVIS_SELECTED) != 0;
	 boolean redraw = !newSelected && getDrawing () && OS.IsWindowVisible (handle);
	 if (redraw) {
	 OS.UpdateWindow (handle);
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 0, 0);
	 }
	 hSelect = hNewItem;
	 ignoreSelect = true;
	 OS.SendMessage (handle, OS.TVM_SELECTITEM, OS.TVGN_CARET, hNewItem);
	 ignoreSelect = false;
	 hSelect = 0;
	 if (oldSelected) {
	 tvItem.state = OS.TVIS_SELECTED;
	 tvItem.hItem = hItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 if (!newSelected) {
	 tvItem.state = 0;
	 tvItem.hItem = hNewItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 if (redraw) {
	 RECT rect1 = new RECT (), rect2 = new RECT ();
	 OS.TreeView_GetItemRect (handle, hItem, rect1, false);
	 OS.TreeView_GetItemRect (handle, hNewItem, rect2, false);
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 1, 0);
	 OS.InvalidateRect (handle, rect1, true);
	 OS.InvalidateRect (handle, rect2, true);
	 OS.UpdateWindow (handle);
	 }
	 return LRESULT.ZERO;
	 }
	 }
	 }
	 long code = callWindowProc (handle, OS.WM_KEYDOWN, wParam, lParam);
	 hAnchor = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 return new LRESULT (code);
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Bug in Windows.  When a tree item that has an image
	 * with alpha is expanded or collapsed, the area where
	 * the image is drawn is not erased before it is drawn.
	 * This means that the image gets darker each time.
	 * The fix is to redraw the selection.
	 *
	 * Feature in Windows.  When multiple item have
	 * the TVIS_SELECTED state, Windows redraws only
	 * the focused item in the color used to show the
	 * selection when the tree loses or gains focus.
	 * The fix is to force Windows to redraw the
	 * selection when focus is gained or lost.
	 */
	/*boolean redraw = (style & SWT.MULTI) != 0;
	 if (!redraw && imageList != null) {
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.TVS_FULLROWSELECT) == 0) {
	 redraw = true;
	 }
	 }
	 if (redraw) redrawSelection ();
	 return super.WM_KILLFOCUS (wParam, lParam);*/
	return W_FALSE;
}

int _TREE_WM_LBUTTONDBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*TVHITTESTINFO lpht = new TVHITTESTINFO ();
	 lpht.x = OS.GET_X_LPARAM (lParam);
	 lpht.y = OS.GET_Y_LPARAM (lParam);
	 OS.SendMessage (handle, OS.TVM_HITTEST, 0, lpht);
	 if (lpht.hItem != 0) {
	 if ((style & SWT.CHECK) != 0) {
	 if ((lpht.flags & OS.TVHT_ONITEMSTATEICON) != 0) {
	 Display display = this.display;
	 display.captureChanged = false;
	 sendMouseEvent (SWT.MouseDown, 1, handle, OS.WM_LBUTTONDOWN, wParam, lParam);
	 if (!sendMouseEvent (SWT.MouseDoubleClick, 1, handle, OS.WM_LBUTTONDBLCLK, wParam, lParam)) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 return LRESULT.ZERO;
	 }
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 OS.SetFocus (handle);
	 TVITEM tvItem = new TVITEM ();
	 tvItem.hItem = lpht.hItem;
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_PARAM | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_STATEIMAGEMASK;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 int state = tvItem.state >> 12;
	 if ((state & 0x1) != 0) {
	 state++;
	 } else  {
	 --state;
	 }
	 tvItem.state = state << 12;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 long id = OS.SendMessage (handle, OS.TVM_MAPHTREEITEMTOACCID, tvItem.hItem, 0);
	 OS.NotifyWinEvent (OS.EVENT_OBJECT_FOCUS, handle, OS.OBJID_CLIENT, (int)id);
	 Event event = new Event ();
	 event.item = _getItem (tvItem.hItem, (int)tvItem.lParam);
	 event.detail = SWT.CHECK;
	 sendSelectionEvent (SWT.Selection, event, false);
	 return LRESULT.ZERO;
	 }
	 }
	 }
	 LRESULT result = super.WM_LBUTTONDBLCLK (wParam, lParam);
	 if (result == LRESULT.ZERO) return result;
	 if (lpht.hItem != 0) {
	 int flags = OS.TVHT_ONITEM;
	 if ((style & SWT.FULL_SELECTION) != 0) {
	 flags |= OS.TVHT_ONITEMRIGHT | OS.TVHT_ONITEMINDENT;
	 } else {
	 if (hooks (SWT.MeasureItem)) {
	 lpht.flags &= ~(OS.TVHT_ONITEMICON | OS.TVHT_ONITEMLABEL);
	 if (hitTestSelection (lpht.hItem, lpht.x, lpht.y)) {
	 lpht.flags |= OS.TVHT_ONITEMICON | OS.TVHT_ONITEMLABEL;
	 }
	 }
	 }
	 if ((lpht.flags & flags) != 0) {
	 Event event = new Event ();
	 event.item = _getItem (lpht.hItem);
	 sendSelectionEvent (SWT.DefaultSelection, event, false);
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_LBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * In a multi-select tree, if the user is collapsing a subtree that
	 * contains selected items, clear the selection from these items and
	 * issue a selection event.  Only items that are selected and visible
	 * are cleared.  This code also runs in the case when the white space
	 * below the last item is selected.
	 */
	/*TVHITTESTINFO lpht = new TVHITTESTINFO ();
	 lpht.x = OS.GET_X_LPARAM (lParam);
	 lpht.y = OS.GET_Y_LPARAM (lParam);
	 OS.SendMessage (handle, OS.TVM_HITTEST, 0, lpht);
	 if (lpht.hItem == 0 || (lpht.flags & OS.TVHT_ONITEMBUTTON) != 0) {
	 Display display = this.display;
	 display.captureChanged = false;
	 if (!sendMouseEvent (SWT.MouseDown, 1, handle, OS.WM_LBUTTONDOWN, wParam, lParam)) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 return LRESULT.ZERO;
	 }
	 boolean fixSelection = false, deselected = false;
	 long hOldSelection = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (lpht.hItem != 0 && (style & SWT.MULTI) != 0) {
	 if (hOldSelection != 0) {
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.hItem = lpht.hItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 if ((tvItem.state & OS.TVIS_EXPANDED) != 0) {
	 fixSelection = true;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 long hNext = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_NEXTVISIBLE, lpht.hItem);
	 while (hNext != 0) {
	 if (hNext == hAnchor) hAnchor = 0;
	 tvItem.hItem = hNext;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 if ((tvItem.state & OS.TVIS_SELECTED) != 0) deselected = true;
	 tvItem.state = 0;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 long hItem = hNext = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_NEXTVISIBLE, hNext);
	 while (hItem != 0 && hItem != lpht.hItem) {
	 hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_PARENT, hItem);
	 }
	 if (hItem == 0) break;
	 }
	 }
	 }
	 }
	 dragStarted = gestureCompleted = false;
	 if (fixSelection) {
	 hSelect = lpht.hItem;
	 ignoreDeselect = ignoreSelect = lockSelection = true;
	 }
	 long code = callWindowProc (handle, OS.WM_LBUTTONDOWN, wParam, lParam);*/
	/* Bug 225404 */
	/*if (OS.GetFocus () != handle) OS.SetFocus (handle);
	 if (fixSelection) {
	 hSelect = 0;
	 ignoreDeselect = ignoreSelect = lockSelection = false;
	 }
	 long hNewSelection = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hOldSelection != hNewSelection) hAnchor = hNewSelection;
	 if (dragStarted) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 }*/
	/*
	 * Bug in Windows.  When a tree has no images and an item is
	 * expanded or collapsed, for some reason, Windows changes
	 * the size of the selection.  When the user expands a tree
	 * item, the selection rectangle is made a few pixels larger.
	 * When the user collapses an item, the selection rectangle
	 * is restored to the original size but the selection is not
	 * redrawn, causing pixel corruption.  The fix is to detect
	 * this case and redraw the item.
	 */
	/*if ((lpht.flags & OS.TVHT_ONITEMBUTTON) != 0) {
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.TVS_FULLROWSELECT) == 0) {
	 if (OS.SendMessage (handle, OS.TVM_GETIMAGELIST, OS.TVSIL_NORMAL, 0) == 0) {
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (hItem != 0) {
	 RECT rect = new RECT ();
	 if (OS.TreeView_GetItemRect (handle, hItem, rect, false)) {
	 OS.InvalidateRect (handle, rect, true);
	 }
	 }
	 }
	 }
	 }
	 if (deselected) {
	 Event event = new Event ();
	 event.item = _getItem (lpht.hItem);
	 sendSelectionEvent (SWT.Selection, event, false);
	 }
	 return new LRESULT (code);
	 }*/

	/* Look for check/uncheck */
	/*if ((style & SWT.CHECK) != 0) {
	 if ((lpht.flags & OS.TVHT_ONITEMSTATEICON) != 0) {
	 Display display = this.display;
	 display.captureChanged = false;
	 if (!sendMouseEvent (SWT.MouseDown, 1, handle, OS.WM_LBUTTONDOWN, wParam, lParam)) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 return LRESULT.ZERO;
	 }
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 OS.SetFocus (handle);
	 TVITEM tvItem = new TVITEM ();
	 tvItem.hItem = lpht.hItem;
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_PARAM | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_STATEIMAGEMASK;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 int state = tvItem.state >> 12;
	 if ((state & 0x1) != 0) {
	 state++;
	 } else  {
	 --state;
	 }
	 tvItem.state = state << 12;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 long id = OS.SendMessage (handle, OS.TVM_MAPHTREEITEMTOACCID, tvItem.hItem, 0);
	 OS.NotifyWinEvent (OS.EVENT_OBJECT_FOCUS, handle, OS.OBJID_CLIENT, (int)id);
	 Event event = new Event ();
	 event.item = _getItem (tvItem.hItem, (int)tvItem.lParam);
	 event.detail = SWT.CHECK;
	 sendSelectionEvent (SWT.Selection, event, false);
	 return LRESULT.ZERO;
	 }
	 }*/

	/*
	 * Feature in Windows.  When the tree has the style
	 * TVS_FULLROWSELECT, the background color for the
	 * entire row is filled when an item is painted,
	 * drawing on top of any custom drawing.  The fix
	 * is to emulate TVS_FULLROWSELECT.
	 */
	/*boolean selected = false;
	 boolean fakeSelection = false;
	 if (lpht.hItem != 0) {
	 if ((style & SWT.FULL_SELECTION) != 0) {
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.TVS_FULLROWSELECT) == 0) fakeSelection = true;
	 } else {
	 if (hooks (SWT.MeasureItem)) {
	 selected = hitTestSelection (lpht.hItem, lpht.x, lpht.y);
	 if (selected) {
	 if ((lpht.flags & OS.TVHT_ONITEM) == 0) fakeSelection = true;
	 }
	 }
	 }
	 }*/

	/* Process the mouse when an item is not selected */
	/*if (!selected && (style & SWT.FULL_SELECTION) == 0) {
	 if ((lpht.flags & OS.TVHT_ONITEM) == 0) {
	 Display display = this.display;
	 display.captureChanged = false;
	 if (!sendMouseEvent (SWT.MouseDown, 1, handle, OS.WM_LBUTTONDOWN, wParam, lParam)) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 return LRESULT.ZERO;
	 }
	 long code = callWindowProc (handle, OS.WM_LBUTTONDOWN, wParam, lParam);*/
	/* Bug 225404 */
	/*if (OS.GetFocus () != handle) OS.SetFocus (handle);
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 return new LRESULT (code);
	 }
	 }*/

	/* Get the selected state of the item under the mouse */
	/*TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 boolean hittestSelected = false;
	 if ((style & SWT.MULTI) != 0) {
	 tvItem.hItem = lpht.hItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 hittestSelected = (tvItem.state & OS.TVIS_SELECTED) != 0;
	 }*/

	/* Get the selected state of the last selected item */
	/*long hOldItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if ((style & SWT.MULTI) != 0) {
	 tvItem.hItem = hOldItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);*/

	/* Check for CONTROL or drag selection */
	//if (hittestSelected || (wParam & OS.MK_CONTROL) != 0) {
	/*
	 * Feature in Windows.  When the tree is not drawing focus
	 * and the user selects a tree item while the CONTROL key
	 * is down, the tree window proc sends WM_UPDATEUISTATE
	 * to the top level window, causing controls within the shell
	 * to redraw.  When drag detect is enabled, the tree window
	 * proc runs a modal loop that allows WM_PAINT messages to be
	 * delivered during WM_LBUTTONDOWN.  When WM_SETREDRAW is used
	 * to disable drawing for the tree and a WM_PAINT happens for
	 * a parent of the tree (or a sibling that overlaps), the parent
	 * will draw on top of the tree.  If WM_SETREDRAW is turned back
	 * on without redrawing the entire tree, pixel corruption occurs.
	 * This case only seems to happen when the tree has been given
	 * focus from WM_MOUSEACTIVATE of the shell.  The fix is to
	 * force the WM_UPDATEUISTATE to be sent before disabling
	 * the drawing.
	 *
	 * NOTE:  Any redraw of a parent (or sibling) will be dispatched
	 * during the modal drag detect loop.  This code only fixes the
	 * case where the tree causes a redraw from WM_UPDATEUISTATE.
	 * In SWT, the InvalidateRect() that caused the pixel corruption
	 * is found in Composite.WM_UPDATEUISTATE().
	 */
	/*int uiState = (int)OS.SendMessage (handle, OS.WM_QUERYUISTATE, 0, 0);
	 if ((uiState & OS.UISF_HIDEFOCUS) != 0) {
	 OS.SendMessage (handle, OS.WM_CHANGEUISTATE, OS.UIS_INITIALIZE, 0);
	 }
	 OS.UpdateWindow (handle);
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 0, 0);
	 } else {
	 deselectAll ();
	 }
	 }*/

	/* Do the selection */
	/*Display display = this.display;
	 display.captureChanged = false;
	 if (!sendMouseEvent (SWT.MouseDown, 1, handle, OS.WM_LBUTTONDOWN, wParam, lParam)) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 return LRESULT.ZERO;
	 }
	 hSelect = lpht.hItem;
	 dragStarted = gestureCompleted = false;
	 ignoreDeselect = ignoreSelect = true;
	 long code = callWindowProc (handle, OS.WM_LBUTTONDOWN, wParam, lParam);*/
	/* Bug 225404 */
	/*if (OS.GetFocus () != handle) OS.SetFocus (handle);
	 long hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 if (fakeSelection) {
	 if (hOldItem == 0 || (hNewItem == hOldItem && lpht.hItem != hOldItem)) {
	 OS.SendMessage (handle, OS.TVM_SELECTITEM, OS.TVGN_CARET, lpht.hItem);
	 hNewItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
	 }
	 if (!dragStarted && (state & DRAG_DETECT) != 0 && hooks (SWT.DragDetect)) {
	 dragStarted = dragDetect (handle, lpht.x, lpht.y, false, null, null);
	 }
	 }
	 ignoreDeselect = ignoreSelect = false;
	 hSelect = 0;
	 if (dragStarted) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 }*/

	/*
	 * Feature in Windows.  When the old and new focused item
	 * are the same, Windows does not check to make sure that
	 * the item is actually selected, not just focused.  The
	 * fix is to force the item to draw selected by setting
	 * the state mask.  This is only necessary when the tree
	 * is single select.
	 */
	/*if ((style & SWT.SINGLE) != 0) {
	 if (hOldItem == hNewItem) {
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.state = OS.TVIS_SELECTED;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 tvItem.hItem = hNewItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 }*/

	/* Reselect the last item that was unselected */
	//if ((style & SWT.MULTI) != 0) {
	/* Check for CONTROL and reselect the last item */
	/*if (hittestSelected || (wParam & OS.MK_CONTROL) != 0) {
	 if (hOldItem == hNewItem && hOldItem == lpht.hItem) {
	 if ((wParam & OS.MK_CONTROL) != 0) {
	 tvItem.state ^= OS.TVIS_SELECTED;
	 if (dragStarted) tvItem.state = OS.TVIS_SELECTED;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 } else {
	 if ((tvItem.state & OS.TVIS_SELECTED) != 0) {
	 tvItem.state = OS.TVIS_SELECTED;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 if ((wParam & OS.MK_CONTROL) != 0 && !dragStarted) {
	 if (hittestSelected) {
	 tvItem.state = 0;
	 tvItem.hItem = lpht.hItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 }
	 }
	 RECT rect1 = new RECT (), rect2 = new RECT ();
	 OS.TreeView_GetItemRect (handle, hOldItem, rect1, false);
	 OS.TreeView_GetItemRect (handle, hNewItem, rect2, false);
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 1, 0);
	 OS.InvalidateRect (handle, rect1, true);
	 OS.InvalidateRect (handle, rect2, true);
	 OS.UpdateWindow (handle);
	 }*/

	/* Check for SHIFT or normal select and deselect/reselect items */
	/*if ((wParam & OS.MK_CONTROL) == 0) {
	 if (!hittestSelected || !dragStarted) {
	 tvItem.state = 0;
	 long oldProc = OS.GetWindowLongPtr (handle, OS.GWLP_WNDPROC);
	 OS.SetWindowLongPtr (handle, OS.GWLP_WNDPROC, TreeProc);
	 if ((style & SWT.VIRTUAL) != 0) {
	 long hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_ROOT, 0);
	 deselect (hItem, tvItem, hNewItem);
	 } else {
	 for (int i=0; i<items.length; i++) {
	 TreeItem item = items [i];
	 if (item != null && item.handle != hNewItem) {
	 tvItem.hItem = item.handle;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 }
	 }
	 }
	 tvItem.hItem = hNewItem;
	 tvItem.state = OS.TVIS_SELECTED;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 OS.SetWindowLongPtr (handle, OS.GWLP_WNDPROC, oldProc);
	 if ((wParam & OS.MK_SHIFT) != 0) {
	 RECT rect1 = new RECT ();
	 if (hAnchor == 0) hAnchor = hNewItem;
	 if (OS.TreeView_GetItemRect (handle, hAnchor, rect1, false)) {
	 RECT rect2 = new RECT ();
	 if (OS.TreeView_GetItemRect (handle, hNewItem, rect2, false)) {
	 int flags = rect1.top < rect2.top ? OS.TVGN_NEXTVISIBLE : OS.TVGN_PREVIOUSVISIBLE;
	 tvItem.state = OS.TVIS_SELECTED;
	 long hItem = tvItem.hItem = hAnchor;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 while (hItem != hNewItem) {
	 tvItem.hItem = hItem;
	 OS.SendMessage (handle, OS.TVM_SETITEM, 0, tvItem);
	 hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, flags, hItem);
	 }
	 }
	 }
	 }
	 }
	 }
	 }
	 if ((wParam & OS.MK_SHIFT) == 0) hAnchor = hNewItem;*/

	/* Issue notification */
	/*if (!gestureCompleted) {
	 tvItem.hItem = hNewItem;
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_PARAM;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 Event event = new Event ();
	 event.item = _getItem (tvItem.hItem, (int)tvItem.lParam);
	 sendSelectionEvent (SWT.Selection, event, false);
	 }
	 gestureCompleted = false;*/

	/*
	 * Feature in Windows.  Inside WM_LBUTTONDOWN and WM_RBUTTONDOWN,
	 * the widget starts a modal loop to determine if the user wants
	 * to begin a drag/drop operation or marquee select.  Unfortunately,
	 * this modal loop eats the corresponding mouse up.  The fix is to
	 * detect the cases when the modal loop has eaten the mouse up and
	 * issue a fake mouse up.
	 */
	/*if (dragStarted) {
	 sendDragEvent (1, OS.GET_X_LPARAM (lParam), OS.GET_Y_LPARAM (lParam));
	 } else {
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.TVS_DISABLEDRAGDROP) == 0) {
	 sendMouseEvent (SWT.MouseUp, 1, handle, OS.WM_LBUTTONUP, wParam, lParam);
	 }
	 }
	 dragStarted = false;
	 return new LRESULT (code);*/
	return W_FALSE;
}

int _TREE_WM_MOUSEMOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*Display display = this.display;
	 LRESULT result = super.WM_MOUSEMOVE (wParam, lParam);
	 if (result != null) return result;
	 if (itemToolTipHandle != 0) {*/
	/*
	 * Bug in Windows.  On some machines that do not have XBUTTONs,
	 * the MK_XBUTTON1 and OS.MK_XBUTTON2 bits are sometimes set,
	 * causing mouse capture to become stuck.  The fix is to test
	 * for the extra buttons only when they exist.
	 */
	/*int mask = OS.MK_LBUTTON | OS.MK_MBUTTON | OS.MK_RBUTTON;
	 if (display.xMouse) mask |= OS.MK_XBUTTON1 | OS.MK_XBUTTON2;
	 if ((wParam & mask) == 0) {
	 int x = OS.GET_X_LPARAM (lParam);
	 int y = OS.GET_Y_LPARAM (lParam);
	 int [] index = new int [1];
	 TreeItem [] item = new TreeItem [1];
	 RECT [] cellRect = new RECT [1], itemRect = new RECT [1];
	 if (findCell (x, y, item, index, cellRect, itemRect)) {*/
	/*
	 * Feature in Windows.  When the new tool rectangle is
	 * set using TTM_NEWTOOLRECT and the tooltip is visible,
	 * Windows draws the tooltip right away and the sends
	 * WM_NOTIFY with TTN_SHOW.  This means that the tooltip
	 * shows first at the wrong location and then moves to
	 * the right one.  The fix is to hide the tooltip window.
	 */
	/*if (OS.SendMessage (itemToolTipHandle, OS.TTM_GETCURRENTTOOL, 0, 0) == 0) {
	 if (OS.IsWindowVisible (itemToolTipHandle)) {
	 OS.ShowWindow (itemToolTipHandle, OS.SW_HIDE);
	 }
	 }
	 TOOLINFO lpti = new TOOLINFO ();
	 lpti.cbSize = TOOLINFO.sizeof;
	 lpti.hwnd = handle;
	 lpti.uId = handle;
	 lpti.uFlags = OS.TTF_SUBCLASS | OS.TTF_TRANSPARENT;
	 lpti.left = cellRect [0].left;
	 lpti.top = cellRect [0].top;
	 lpti.right = cellRect [0].right;
	 lpti.bottom = cellRect [0].bottom;
	 OS.SendMessage (itemToolTipHandle, OS.TTM_NEWTOOLRECT, 0, lpti);
	 }
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_MOUSEWHEEL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_MOUSEWHEEL (wParam, lParam);
	 if (itemToolTipHandle != 0) OS.ShowWindow (itemToolTipHandle, OS.SW_HIDE);
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_MOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (itemToolTipHandle != 0) OS.ShowWindow (itemToolTipHandle, OS.SW_HIDE);
	 if (ignoreResize) return null;
	 return super.WM_MOVE (wParam, lParam);*/
	return W_FALSE;
}

int _TREE_WM_RBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Feature in Windows.  The receiver uses WM_RBUTTONDOWN
	 * to initiate a drag/drop operation depending on how the
	 * user moves the mouse.  If the user clicks the right button,
	 * without moving the mouse, the tree consumes the corresponding
	 * WM_RBUTTONUP.  The fix is to avoid calling the window proc for
	 * the tree.
	 */
	/*Display display = this.display;
	 display.captureChanged = false;
	 if (!sendMouseEvent (SWT.MouseDown, 3, handle, OS.WM_RBUTTONDOWN, wParam, lParam)) {
	 if (!display.captureChanged && !isDisposed ()) {
	 if (OS.GetCapture () != handle) OS.SetCapture (handle);
	 }
	 return LRESULT.ZERO;
	 }*/
	/*
	 * This code is intentionally commented.
	 */
//	if (OS.GetCapture () != handle) OS.SetCapture (handle);
	/* Bug 225404 */
	//if (OS.GetFocus () != handle) OS.SetFocus (handle);
	/*
	 * Feature in Windows.  When the user selects a tree item
	 * with the right mouse button, the item remains selected
	 * only as long as the user does not release or move the
	 * mouse.  As soon as this happens, the selection snaps
	 * back to the previous selection.  This behavior can be
	 * observed in the Explorer but is not instantly apparent
	 * because the Explorer explicitly sets the selection when
	 * the user chooses a menu item.  If the user cancels the
	 * menu, the selection snaps back.  The fix is to avoid
	 * calling the window proc and do the selection ourselves.
	 * This behavior is consistent with the table.
	 */
	/*TVHITTESTINFO lpht = new TVHITTESTINFO ();
	 lpht.x = OS.GET_X_LPARAM (lParam);
	 lpht.y = OS.GET_Y_LPARAM (lParam);
	 OS.SendMessage (handle, OS.TVM_HITTEST, 0, lpht);
	 if (lpht.hItem != 0) {
	 boolean fakeSelection = (style & SWT.FULL_SELECTION) != 0;
	 if (!fakeSelection) {
	 if (hooks (SWT.MeasureItem)) {
	 fakeSelection = hitTestSelection (lpht.hItem, lpht.x, lpht.y);
	 } else {
	 int flags = OS.TVHT_ONITEMICON | OS.TVHT_ONITEMLABEL;
	 fakeSelection = (lpht.flags & flags) != 0;
	 }
	 }
	 if (fakeSelection) {
	 if ((wParam & (OS.MK_CONTROL | OS.MK_SHIFT)) == 0) {
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_STATE;
	 tvItem.stateMask = OS.TVIS_SELECTED;
	 tvItem.hItem = lpht.hItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 if ((tvItem.state & OS.TVIS_SELECTED) == 0) {
	 ignoreSelect = true;
	 OS.SendMessage (handle, OS.TVM_SELECTITEM, OS.TVGN_CARET, 0);
	 ignoreSelect = false;
	 OS.SendMessage (handle, OS.TVM_SELECTITEM, OS.TVGN_CARET, lpht.hItem);
	 }
	 }
	 }
	 }
	 return LRESULT.ZERO;*/
	return W_FALSE;
}

int _TREE_WM_PAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if ((state & DISPOSE_SENT) != 0) return LRESULT.ZERO;

	 if (shrink && !ignoreShrink && items != null) {*/
	/* Resize the item array to fit the last item */
	/*int count = items.length - 1;
	 while (count >= 0) {
	 if (items [count] != null) break;
	 --count;
	 }
	 count++;
	 if (items.length > 4 && items.length - count > 3) {
	 int length = Math.max (4, (count + 3) / 4 * 4);
	 TreeItem [] newItems = new TreeItem [length];
	 System.arraycopy (items, 0, newItems, 0, count);
	 items = newItems;
	 }
	 shrink = false;
	 }
	 if ((style & SWT.DOUBLE_BUFFERED) != 0 || findImageControl () != null) {
	 boolean doubleBuffer = true;
	 if (explorerTheme) {
	 int exStyle = (int)OS.SendMessage (handle, OS.TVM_GETEXTENDEDSTYLE, 0, 0);
	 if ((exStyle & OS.TVS_EX_DOUBLEBUFFER) != 0) doubleBuffer = false;
	 }
	 if (doubleBuffer) {
	 GC gc = null;
	 long paintDC = 0;
	 PAINTSTRUCT ps = new PAINTSTRUCT ();
	 boolean hooksPaint = hooks (SWT.Paint) || filters (SWT.Paint);
	 if (hooksPaint) {
	 GCData data = new GCData ();
	 data.ps = ps;
	 data.hwnd = handle;
	 gc = GC.win32_new (this, data);
	 paintDC = gc.handle;
	 } else {
	 paintDC = OS.BeginPaint (handle, ps);
	 }
	 int width = ps.right - ps.left;
	 int height = ps.bottom - ps.top;
	 if (width != 0 && height != 0) {
	 long hDC = OS.CreateCompatibleDC (paintDC);
	 POINT lpPoint1 = new POINT (), lpPoint2 = new POINT ();
	 OS.SetWindowOrgEx (hDC, ps.left, ps.top, lpPoint1);
	 OS.SetBrushOrgEx (hDC, ps.left, ps.top, lpPoint2);
	 long hBitmap = OS.CreateCompatibleBitmap (paintDC, width, height);
	 long hOldBitmap = OS.SelectObject (hDC, hBitmap);
	 RECT rect = new RECT ();
	 OS.SetRect (rect, ps.left, ps.top, ps.right, ps.bottom);
	 drawBackground (hDC, rect);
	 callWindowProc (handle, OS.WM_PAINT, hDC, 0);
	 OS.SetWindowOrgEx (hDC, lpPoint1.x, lpPoint1.y, null);
	 OS.SetBrushOrgEx (hDC, lpPoint2.x, lpPoint2.y, null);
	 OS.BitBlt (paintDC, ps.left, ps.top, width, height, hDC, 0, 0, OS.SRCCOPY);
	 OS.SelectObject (hDC, hOldBitmap);
	 OS.DeleteObject (hBitmap);
	 OS.DeleteObject (hDC);
	 if (hooksPaint) {
	 Event event = new Event ();
	 event.gc = gc;
	 event.setBoundsInPixels(new Rectangle(ps.left, ps.top, ps.right - ps.left, ps.bottom - ps.top));
	 sendEvent (SWT.Paint, event);
	 // widget could be disposed at this point
	 event.gc = null;
	 }
	 }
	 if (hooksPaint) {
	 gc.dispose ();
	 } else {
	 OS.EndPaint (handle, ps);
	 }
	 return LRESULT.ZERO;
	 }
	 }
	 return super.WM_PAINT (wParam, lParam);*/
	return W_FALSE;
}

int _TREE_WM_SETCURSOR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SETCURSOR (wParam, lParam);
	 if (result != null) return result;*/

	/*
	 * Feature in Windows. On Windows 7, the tree control show the
	 * hand cursor when the mouse is over an item.  This is the
	 * correct Windows 7 behavior but not correct for SWT. The fix
	 * is to always ensure a cursor is set.
	 */
	/*if (OS.WIN32_VERSION >= OS.VERSION (6, 1)) {
	 if (wParam == handle) {
	 int hitTest = (short) OS.LOWORD (lParam);
	 if (hitTest == OS.HTCLIENT) {
	 OS.SetCursor (OS.LoadCursor (0, OS.IDC_ARROW));
	 return LRESULT.ONE;
	 }
	 }
	 }
	 return null;*/
	return W_FALSE;
}

int _TREE_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Bug in Windows.  When a tree item that has an image
	 * with alpha is expanded or collapsed, the area where
	 * the image is drawn is not erased before it is drawn.
	 * This means that the image gets darker each time.
	 * The fix is to redraw the selection.
	 *
	 * Feature in Windows.  When multiple item have
	 * the TVIS_SELECTED state, Windows redraws only
	 * the focused item in the color used to show the
	 * selection when the tree loses or gains focus.
	 * The fix is to force Windows to redraw the
	 * selection when focus is gained or lost.
	 */
	/*boolean redraw = (style & SWT.MULTI) != 0;
	 if (!redraw && imageList != null) {
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.TVS_FULLROWSELECT) == 0) {
	 redraw = true;
	 }
	 }
	 if (redraw) redrawSelection ();
	 return super.WM_SETFOCUS (wParam, lParam);*/
	return W_FALSE;
}

int _TREE_WM_SETFONT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SETFONT (wParam, lParam);
	 if (result != null) return result;
	 if (hwndHeader != 0) {*/
	/*
	 * Bug in Windows.  When a header has a sort indicator
	 * triangle, Windows resizes the indicator based on the
	 * size of the n-1th font.  The fix is to always make
	 * the n-1th font be the default.  This makes the sort
	 * indicator always be the default size.
	 */
	/*OS.SendMessage (hwndHeader, OS.WM_SETFONT, 0, lParam);
	 OS.SendMessage (hwndHeader, OS.WM_SETFONT, wParam, lParam);
	 }
	 if (itemToolTipHandle != 0) {
	 OS.ShowWindow (itemToolTipHandle, OS.SW_HIDE);
	 OS.SendMessage (itemToolTipHandle, OS.WM_SETFONT, wParam, lParam);
	 }
	 if (headerToolTipHandle != 0) {
	 OS.SendMessage (headerToolTipHandle, OS.WM_SETFONT, wParam, lParam);
	 updateHeaderToolTips ();
	 }
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_SETREDRAW(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SETREDRAW (wParam, lParam);
	 if (result != null) return result;
	 if (itemToolTipHandle != 0) OS.ShowWindow (itemToolTipHandle, OS.SW_HIDE);*/
	/*
	 * Bug in Windows.  Under certain circumstances, when
	 * WM_SETREDRAW is used to turn off drawing and then
	 * TVM_GETITEMRECT is sent to get the bounds of an item
	 * that is not inside the client area, Windows segment
	 * faults.  The fix is to call the default window proc
	 * rather than the default tree proc.
	 *
	 * NOTE:  This problem is intermittent and happens on
	 * Windows Vista running under the theme manager.
	 */
	/*long code = OS.DefWindowProc (handle, OS.WM_SETREDRAW, wParam, lParam);
	 return code == 0 ? LRESULT.ZERO : new LRESULT (code);*/
	return W_FALSE;
}

int _TREE_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	//if (itemToolTipHandle != 0) OS.ShowWindow (itemToolTipHandle, OS.SW_HIDE);
	/*
	 * Bug in Windows.  When TVS_NOHSCROLL is set when the
	 * size of the tree is zero, the scroll bar is shown the
	 * next time the tree resizes.  The fix is to hide the
	 * scroll bar every time the tree is resized.
	 */
	int bits = GetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE);
	if ((bits & TVS_NOHSCROLL) != 0) {
		ShowScrollBar(_W_WIDGET(widget)->handle, SB_HORZ, FALSE);
	}
	/*
	 * Bug in Windows.  On Vista, when the Explorer theme
	 * is used with a full selection tree, when the tree
	 * is resized to be smaller, the rounded right edge
	 * of the selected items is not drawn.  The fix is the
	 * redraw the entire tree.
	 */
	if (_W_TREE(widget)->explorerTheme
			&& (_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
		InvalidateRect(_W_WIDGET(widget)->handle, NULL, FALSE);
	}
	if (_W_TREE(widget)->ignoreResize)
		return W_FALSE;
	return _COMPOSITE_WM_SIZE(widget, e, reserved);
}

int _TREE_WM_SYSCOLORCHANGE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_SYSCOLORCHANGE (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Bug in Windows.  When the tree is using the explorer
	 * theme, it does not use COLOR_WINDOW_TEXT for the
	 * default foreground color.  The fix is to explicitly
	 * set the foreground.
	 */
	/*if (explorerTheme) {
	 if (foreground == -1) setForegroundPixel (-1);
	 }
	 if ((style & SWT.CHECK) != 0) setCheckboxImageList ();
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_VSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*boolean fixScroll = false;
	 if ((style & SWT.DOUBLE_BUFFERED) != 0) {
	 int code = OS.LOWORD (wParam);
	 switch (code) {
	 case OS.SB_TOP:
	 case OS.SB_BOTTOM:
	 case OS.SB_LINEDOWN:
	 case OS.SB_LINEUP:
	 case OS.SB_PAGEDOWN:
	 case OS.SB_PAGEUP:
	 fixScroll = (style & SWT.VIRTUAL) != 0 || hooks (SWT.EraseItem) || hooks (SWT.PaintItem);
	 break;
	 }
	 }
	 if (fixScroll) {
	 style &= ~SWT.DOUBLE_BUFFERED;
	 if (explorerTheme) {
	 OS.SendMessage (handle, OS.TVM_SETEXTENDEDSTYLE, OS.TVS_EX_DOUBLEBUFFER, 0);
	 }
	 }
	 LRESULT result = super.WM_VSCROLL (wParam, lParam);
	 if (fixScroll) {
	 style |= SWT.DOUBLE_BUFFERED;
	 if (explorerTheme) {
	 OS.SendMessage (handle, OS.TVM_SETEXTENDEDSTYLE, OS.TVS_EX_DOUBLEBUFFER, OS.TVS_EX_DOUBLEBUFFER);
	 }
	 }
	 if (result != null) return result;
	 return result;*/
	return W_FALSE;
}

int _TREE_WM_TIMER(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result = super.WM_TIMER (wParam, lParam);
	 if (result != null) return result;*/

	/* Bug in Windows. When the expandos are visible (or in process of fading away)
	 * and the tree control is hidden the animation timer does not stop calling the
	 * window proc till the tree is visible again. This can cause performance problems
	 * specially in cases there the application has several tree controls in this state.
	 * The fix is to detect a timer that repeats itself several times when the control
	 * is not visible and stop it. The timer is stopped by sending a fake mouse move event.
	 *
	 * Note: Just killing the timer could cause some internal clean up task related to the
	 * animation not to run.
	 */
	/*long bits = OS.SendMessage (handle, OS.TVM_GETEXTENDEDSTYLE, 0, 0);
	 if ((bits & OS.TVS_EX_FADEINOUTEXPANDOS) != 0) {
	 if (!OS.IsWindowVisible (handle)) {
	 if (lastTimerID == wParam) {
	 lastTimerCount++;
	 } else {
	 lastTimerCount = 0;
	 }
	 lastTimerID = wParam;
	 if (lastTimerCount >= TIMER_MAX_COUNT) {
	 OS.CallWindowProc (TreeProc, handle, OS.WM_MOUSEMOVE, 0, 0);
	 lastTimerID = -1;
	 lastTimerCount = 0;
	 }
	 } else {
	 lastTimerID = -1;
	 lastTimerCount = 0;
	 }
	 }
	 return result;*/
	return W_FALSE;
}
;

int _TREE_WM_CTLCOLORCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (findImageControl () != null) {
	 return new LRESULT (OS.GetStockObject (OS.NULL_BRUSH));
	 }*/
	/*
	 * Feature in Windows.  Tree controls send WM_CTLCOLOREDIT
	 * to allow application code to change the default colors.
	 * This is undocumented and conflicts with TVM_SETTEXTCOLOR
	 * and TVM_SETBKCOLOR, the documented way to do this.  The
	 * fix is to ignore WM_CTLCOLOREDIT messages from trees.
	 */
	//return null;
	return W_FALSE;
}

int _TREE_WM_NOTIFY_GETDISPINFO(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	NMTVDISPINFOW *lptvdi = (NMTVDISPINFOW*) e->lparam;
	int result = W_FALSE;
	w_event_tree event;
	w_value value;
	w_treeitem item;
	w_treecolumn column;
	if ((_W_WIDGET(widget)->style & W_VIRTUAL) != 0) {
		/*
		 * Feature in Windows.  When a new tree item is inserted
		 * using TVM_INSERTITEM, a TVN_GETDISPINFO is sent before
		 * TVM_INSERTITEM returns and before the item is added to
		 * the items array.  The fix is to check for null.
		 *
		 * NOTE: This only happens on XP with the version 6.00 of
		 * COMCTL32.DLL.
		 */
		boolean checkVisible = TRUE;
		/*
		 * When an item is being deleted from a virtual tree, do not
		 * allow the application to provide data for a new item that
		 * becomes visible until the item has been removed from the
		 * items array.  Because arbitrary application code can run
		 * during the callback, the items array might be accessed
		 * in an inconsistent state.  Rather than answering the data
		 * right away, queue a redraw for later.
		 */
		/*if (!_W_TREE(widget)->ignoreShrink) {
		 if (items != null && lptvdi.lParam != -1) {
		 if (items [(int)lptvdi.lParam] != null && items [(int)lptvdi.lParam].cached) {
		 checkVisible = false;
		 }
		 }
		 }
		 if (checkVisible) {
		 if (!getDrawing () || !OS.IsWindowVisible (handle)) break;
		 RECT itemRect = new RECT ();
		 if (!OS.TreeView_GetItemRect (handle, lptvdi.hItem, itemRect, false)) {
		 break;
		 }
		 RECT rect = new RECT ();
		 OS.GetClientRect (handle, rect);
		 if (!OS.IntersectRect (rect, rect, itemRect)) break;
		 if (ignoreShrink) {
		 OS.InvalidateRect (handle, rect, true);
		 break;
		 }
		 }*/
	}
	//if (items == null) return W_FALSE;
	/*
	 * Bug in Windows.  If the lParam field of TVITEM
	 * is changed during custom draw using TVM_SETITEM,
	 * the lItemlParam field of the NMTVCUSTOMDRAW struct
	 * is not updated until the next custom draw.  The
	 * fix is to query the field from the item instead
	 * of using the struct.
	 */
	/*int id = (int)lptvdi.lParam;
	 if ((style & SWT.VIRTUAL) != 0) {
	 if (id == -1) {
	 TVITEM tvItem = new TVITEM ();
	 tvItem.mask = OS.TVIF_HANDLE | OS.TVIF_PARAM;
	 tvItem.hItem = lptvdi.hItem;
	 OS.SendMessage (handle, OS.TVM_GETITEM, 0, tvItem);
	 id = (int)tvItem.lParam;
	 }
	 }
	 TreeItem item = _getItem (lptvdi.hItem, id);*/
	/*
	 * Feature in Windows.  When a new tree item is inserted
	 * using TVM_INSERTITEM, a TVN_GETDISPINFO is sent before
	 * TVM_INSERTITEM returns and before the item is added to
	 * the items array.  The fix is to check for null.
	 *
	 * NOTE: This only happens on XP with the version 6.00 of
	 * COMCTL32.DLL.
	 *
	 * Feature in Windows.  When TVM_DELETEITEM is called with
	 * TVI_ROOT to remove all items from a tree, under certain
	 * circumstances, the tree sends TVN_GETDISPINFO for items
	 * that are about to be disposed.  The fix is to check for
	 * disposed items.
	 */
	/*if (item == null)
	 break;
	 if (item.isDisposed())
	 break;
	 if (!item.cached) {
	 if ((style & W_VIRTUAL) != 0) {
	 if (!checkData(item, false))
	 break;
	 }
	 if (painted)
	 item.cached = true;
	 }
	 int index = 0;
	 if (hwndHeader != 0) {
	 index = (int) SendMessage(hwndHeader, HDM_ORDERTOINDEX, 0, 0);
	 }*/
	if ((lptvdi->item.mask & TVIF_TEXT) != 0) {
		WCHAR *string = 0;
		int string_length;
		/*if (index == 0) {
		 string = item.text;
		 } else {
		 String [] strings = item.strings;
		 if (strings != null)
		 string = strings[index];
		 }*/
		/*
		 * init tree item
		 */
		w_treeitem item;
		_W_TREEITEM(&item)->tree = W_TREE(widget);
		_W_TREEITEM(&item)->htreeitem = (HTREEITEM) lptvdi->item.hItem;
		_W_TREEITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(widget)->class_treeitem);
		/*
		 * init tree column
		 */
		w_treecolumn column;
		_W_TREECOLUMN(&column)->tree = W_TREE(widget);
		_W_TREECOLUMN(&column)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(widget)->class_treecolumn);
		_W_TREECOLUMN(&column)->index = 0;
		/*
		 * init event
		 */
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_GET_VALUE;
		event.event.widget = widget;
		event.event.platform_event = (struct w_event_platform*) e;
		event.column = &column;
		event.item = &item;
		event.value = &value;
		w_value_init(&value);
		w_value_set_string(&value,
				(char*) &win_toolkit->tmp[win_toolkit->tmp_alloc / 2],
				win_toolkit->tmp_alloc / sizeof(WCHAR), W_VALUE_USER_MEMORY);
		result = w_widget_send_event(widget, (w_event*) &event);
		if (result) {
			result = W_TRUE;
			char *text = w_value_string_copy_is_needed(&value,
					(char*) &win_toolkit->tmp[win_toolkit->tmp_alloc / 2],
					win_toolkit->tmp_alloc / sizeof(WCHAR));
			lptvdi->item.cchTextMax = w_utf8_to_utf16(text, -1,
					lptvdi->item.pszText, lptvdi->item.cchTextMax);
		} else {
			result = W_FALSE;
		}
		w_value_free(&value);
		/*if (string != 0) {
		 int byteCount = w_min (buffer.length (), lptvdi.cchTextMax - 1) * TCHAR.sizeof;
		 MoveMemory (lptvdi.pszText, buffer, byteCount);
		 MoveMemory (lptvdi.pszText + byteCount, new byte [TCHAR.sizeof], TCHAR.sizeof);
		 lptvdi.cchTextMax = Math.min(lptvdi.cchTextMax,
		 string.length() + 1);
		 }*/
	}
	/*if ((lptvdi.mask & (TVIF_IMAGE | TVIF_SELECTEDIMAGE)) != 0) {
	 Image image = null;
	 if (index == 0) {
	 image = item.image;
	 } else {
	 Image [] images = item.images;
	 if (images != null)
	 image = images[index];
	 }
	 lptvdi.iImage = lptvdi.iSelectedImage = I_IMAGENONE;
	 if (image != null) {
	 lptvdi.iImage = lptvdi.iSelectedImage = imageIndex(image, index);
	 }
	 if (explorerTheme&& IsWindowEnabled(handle)) {
	 if (findImageControl() != null) {
	 lptvdi.iImage = lptvdi.iSelectedImage = I_IMAGENONE;
	 }
	 }
	 }*/
	return result;
}
int _TREE_WM_NOTIFY_CDDS_PREPAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (explorerTheme) {
	 if ((OS.IsWindowEnabled (handle) && hooks (SWT.EraseItem)) || findImageControl () != null) {
	 RECT rect = new RECT ();
	 OS.SetRect (rect, nmcd.left, nmcd.top, nmcd.right, nmcd.bottom);
	 drawBackground (nmcd.hdc, rect);
	 }
	 }*/
	e->result = CDRF_NOTIFYITEMDRAW | CDRF_NOTIFYPOSTPAINT;
	return W_TRUE;
}
struct cdds_item {
	w_widget *widget;
	struct _w_event_platform *e;
	struct _w_widget_reserved *reserved;
	int columnCount;
	int clrText;
	int clrTextBk;
	int sortIndex;
	int clrSortBk;
	unsigned draw :1;
	unsigned clear :1;
	unsigned drawItem :1;
	unsigned drawText :1;
	unsigned drawImage :1;
	unsigned drawBackground :1;
	unsigned drawForeground :1;
	unsigned selected :1;
	unsigned hot :1;
	unsigned measureEvent :1;
	NMTVCUSTOMDRAW *nmcd;
	w_event_tree event;
	w_graphics gc;
	w_tree_attr attr;
	w_treeitem item;
	w_treecolumn column;
	RECT rect;
	RECT clientRect;
	RECT *backgroundRect;
	RECT rect_tmp;
	w_rect r;
	HDITEMW hdItem;
};
void _TREE_WM_NOTIFY_CDDS_init_var(struct cdds_item *item) {

	/*
	 * init tree item
	 */
	//w_treeitem _item;
	_W_TREEITEM(&item->item)->tree = W_TREE(item->widget);
	_W_TREEITEM(&item->item)->htreeitem =
			(HTREEITEM) item->nmcd->nmcd.dwItemSpec;
	_W_TREEITEM(&item->item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_TREE_GET_CLASS(item->widget)->class_treeitem);
	/*
	 * init tree column
	 */
	//w_treecolumn column;
	_W_TREECOLUMN(&item->column)->tree = W_TREE(item->widget);
	_W_TREECOLUMN(&item->column)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_TREE_GET_CLASS(item->widget)->class_treecolumn);
	_W_TREECOLUMN(&item->column)->index = 0;
	/*
	 * get column count
	 */
	item->columnCount = 1;
	if (_W_TREE(item->widget)->hwndHeader != 0) {
		item->columnCount = SendMessageW(_W_TREE(item->widget)->hwndHeader,
		HDM_GETITEMCOUNT, 0, 0);
	}
}
void _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_getAttr(struct cdds_item *item) {
	memset(&item->event, 0, sizeof(item->event));
	item->event.event.type = W_EVENT_ITEM_GET_ATTR;
	item->event.event.widget = item->widget;
	item->event.event.platform_event = (struct w_event_platform*) item->e;
	item->event.item = &item->item;
	item->event.column = &item->column;
	item->event.attr = &item->attr;
	memset(&item->attr, 0, sizeof(item->attr));
	int ret = _w_widget_send_event(item->widget, (w_event*) &item->event);
	if (!ret) {
		memset(&item->attr, 0, sizeof(item->attr));
	}
}
int _w_tree_send_measure_item_event(struct cdds_item *item, int detail) {
	HDC hDC = item->nmcd->nmcd.hdc;
	//RECT itemRect= item.getBounds (index, true, true, false, false, false, hDC);
	RECT itemRect;
	_w_treeitem_get_bounds_0(&item->item, &itemRect, hDC,
	_W_TREECOLUMN(&item->column)->index,
	_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_GET_IMAGE);

	int nSavedDC = SaveDC(hDC);
	memset(&item->event, 0, sizeof(item->event));
	w_graphics_init(&item->gc);
	if (item->attr.font != 0) {
		w_graphics_set_font(&item->gc, item->attr.font);
	}
	item->r.x = itemRect.left;
	item->r.y = itemRect.top;
	item->r.width = itemRect.right - itemRect.left;
	item->r.height = itemRect.bottom - itemRect.top;
	item->event.detail = detail;
	item->event.attr = 0;
	item->event.gc = &item->gc;
	item->event.column = &item->column;
	item->event.item = &item->item;
	item->event.rect = &item->r;
	item->event.event.widget = item->widget;
	item->event.event.platform_event = (struct w_event_platform*) item->e;
	item->event.event.type = W_EVENT_ITEM_MEASURE;
	int ret = w_widget_send_event(item->widget, (w_event*) &item->event);
	w_graphics_dispose(&item->gc);
	RestoreDC(hDC, nSavedDC);
	/*if (isDisposed() || item.isDisposed())
	 return null;
	 Rectangle rect = event.getBoundsInPixels();*/
	if (ret) {
		if (_W_TREE(item->widget)->hwndHeader != 0) {
			/*if (columnCount == 0) {
			 if (rect.x + rect.width > scrollWidth) {
			 setScrollWidth(scrollWidth = rect.x + rect.width);
			 }
			 }*/
		}
		if (item->r.height > SendMessageW(_W_WIDGET(item->widget)->handle,
		TVM_GETITEMHEIGHT, 0, 0)) {
			SendMessageW(_W_WIDGET(item->widget)->handle,
			TVM_SETITEMHEIGHT, item->r.height, 0);
		}
	}
	return ret;
}
int _TREE_WM_NOTIFY_CDDS_ITEMPREPAINT(w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	//NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	struct cdds_item item_tmp, *item;
	item = &item_tmp;
	item->widget = widget;
	item->e = e;
	item->nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	_W_TREE(widget)->hooksEraseItem = FALSE;
	/*
	 * Even when custom draw is being ignored, the font needs
	 * to be selected into the HDC so that the item bounds are
	 * measured correctly.
	 */
	//TreeItem item = getItem (nmcd);
	/*
	 * Feature in Windows.  When a new tree item is inserted
	 * using TVM_INSERTITEM and the tree is using custom draw,
	 * a NM_CUSTOMDRAW is sent before TVM_INSERTITEM returns
	 * and before the item is added to the items array.  The
	 * fix is to check for null.
	 *
	 * NOTE: This only happens on XP with the version 6.00 of
	 * COMCTL32.DLL,
	 */
	if (item->nmcd->nmcd.dwItemSpec == 0)
		return W_FALSE;
	int i, ret;
	//w_rect r;
	//RECT rect;
	//HDITEMW hdItem;
	//w_event_tree event;
	//w_graphics gc;
	//w_tree_attr attr;
	_TREE_WM_NOTIFY_CDDS_init_var(item);
	HDC hDC = item->nmcd->nmcd.hdc;
	int index = 0;
	if (_W_TREE(widget)->hwndHeader != 0) {
		index = (int) SendMessageW(_W_TREE(widget)->hwndHeader,
		HDM_ORDERTOINDEX, 0, 0);
		_W_TREECOLUMN(&item->column)->index = index;
	}
	_TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_getAttr(item);
	if (w_font_isok(item->attr.font)) {
		SelectObject(hDC, _W_FONT(item->attr.font)->handle);
	}
	if (_W_TREE(widget)->ignoreCustomDraw
			|| item->nmcd->nmcd.rc.left == item->nmcd->nmcd.rc.right) {
		if (w_font_isok(item->attr.font)) {
			e->result = CDRF_NEWFONT;
		} else {
			e->result = CDRF_DODEFAULT;
		}
		return W_TRUE;
	}
	RECT *clipRect = 0;
	if (_W_TREE(item->widget)->hwndHeader != 0) {
		wresult clip = !_W_TREE(widget)->printClient;
#if !IsWinCE
		if (WIN32_VERSION >= VERSION(6, 0)) {
			clip = W_TRUE;
		}
#endif
		if (clip) {
			clipRect = &item->rect;
			item->hdItem.mask = HDI_WIDTH;
			SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEM, index,
					(LPARAM) &item->hdItem);
			SetRect(clipRect, item->nmcd->nmcd.rc.left, item->nmcd->nmcd.rc.top,
					item->nmcd->nmcd.rc.left + item->hdItem.cxy,
					item->nmcd->nmcd.rc.bottom);
		}
	}
	int clrText = 0, clrTextBk = 0;
	if (IsWindowEnabled(_W_WIDGET(widget)->handle)) {
		/*clrText = item.cellForeground != null ? item.cellForeground [index] : -1;
		 if (clrText == -1) clrText = item.foreground;
		 clrTextBk = item.cellBackground != null ? item.cellBackground [index] : -1;
		 if (clrTextBk == -1) clrTextBk = item.background;*/
		clrText = item->attr.foreground;
		clrTextBk = item->attr.background;
	}
	int clrSortBk = 0;
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		/*if (_W_TREE(widget)->sortColumn != -1 && sortDirection != W_NONE) {
		 if (findImageControl () == null) {
		 if (indexOf (sortColumn) == index) {
		 clrSortBk = getSortColumnPixel ();
		 if (clrTextBk == -1) clrTextBk = clrSortBk;
		 }
		 }
		 }*/
	}
	wresult selected = _w_tree_is_item_selected(widget, item->nmcd);
	wresult hot = _W_TREE(widget)->explorerTheme
			&& (item->nmcd->nmcd.uItemState & CDIS_HOT) != 0;
	wresult focused = _W_TREE(widget)->explorerTheme
			&& (item->nmcd->nmcd.uItemState & CDIS_FOCUS) != 0;
	if (IsWindowVisible(_W_WIDGET(widget)->handle)
			&& item->nmcd->nmcd.rc.left < item->nmcd->nmcd.rc.right
			&& item->nmcd->nmcd.rc.top < item->nmcd->nmcd.rc.bottom) {
		if (w_font_isok(item->attr.font))
			SelectObject(hDC, _W_FONT(item->attr.font)->handle);
		if (_W_TREE(widget)->linesVisible) {
			DrawEdge(hDC, &item->nmcd->nmcd.rc, BDR_SUNKENINNER, BF_BOTTOM);
		}
//TODO - BUG - measure and erase sent when first column is clipped
		/*Event measureEvent = null;
		 Rectangle boundsInPixels = null;
		 if (hooks (SWT.MeasureItem)) {

		 measureEvent = sendMeasureItemEvent (item, index, hDC, selected ? SWT.SELECTED : 0);
		 boundsInPixels = measureEvent.getBoundsInPixels ();
		 if (isDisposed () || item.isDisposed ()) return null;
		 }*/
		ret = _w_tree_send_measure_item_event(item, selected ? W_SELECTED : 0);
		item->measureEvent = ret;
		_W_TREE(widget)->selectionForeground = 0;
		_W_TREE(widget)->ignoreDrawForeground =
		_W_TREE(widget)->ignoreDrawBackground =
		_W_TREE(widget)->ignoreDrawSelection =
		_W_TREE(widget)->ignoreDrawFocus =
		_W_TREE(widget)->ignoreDrawHot =
		_W_TREE(widget)->ignoreFullSelection = W_FALSE;
//if (hooks (SWT.EraseItem)) {
		/*RECT rect = new RECT ();
		 OS.SetRect (rect, nmcd.left, nmcd.top, nmcd.right, nmcd.bottom);
		 RECT cellRect = item.getBounds (index, true, true, true, true, true, hDC);*/
		RECT cellRect;
		_w_treeitem_get_bounds_0(&item->item, &cellRect, hDC, index,
				_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_FULLTEXT
						| _W_TREE_BOUNDS_GET_IMAGE | _W_TREE_BOUNDS_FULLIMAGE
						| _W_TREE_BOUNDS_CLIP);
		if (clrSortBk != 0) {
			_w_control_draw_background_0(W_CONTROL(widget), hDC, &cellRect,
					clrSortBk, 0, 0);
		} else {
			if (IsWindowEnabled(
			_W_WIDGET(widget)->handle) /*|| findImageControl () != null*/) {
				//_w_control_draw_background_1(W_CONTROL(widget),hDC,&nmcd->nmcd.rc);
			} else {
				/*_w_control_fill_background(W_CONTROL(widget), hDC,
				 GetBkColor(hDC), &nmcd->nmcd.rc);*/
			}
		}
		int nSavedDC = SaveDC(hDC);
		_w_graphics_init(&item->gc, hDC);
		if (selected && _W_TREE(widget)->explorerTheme) {
			w_graphics_set_foreground(&item->gc, GetSysColor(COLOR_WINDOWTEXT));
			//data.foreground = OS.GetSysColor (COLOR_WINDOWTEXT);
		} else {
			w_graphics_set_foreground(&item->gc, GetTextColor(hDC));
			//data.foreground = OS.GetTextColor (hDC);
		}
		w_graphics_set_background(&item->gc, GetBkColor(hDC));
//data.background = OS.GetBkColor (hDC);
		if (!selected) {
			if (clrText != 0)
				w_graphics_set_foreground(&item->gc, clrText);
			if (clrTextBk != 0)
				w_graphics_set_background(&item->gc, clrTextBk);
		}
		int uiState = (int) SendMessageW(_W_WIDGET(item->widget)->handle,
		WM_QUERYUISTATE, 0, 0);
		if (uiState & UISF_HIDEFOCUS) {
			_W_GRAPHICS(&item->gc)->state |= W_GRAPHICS_UISF_HIDEFOCUS;
		}
		if (uiState & UISF_HIDEACCEL) {
			_W_GRAPHICS(&item->gc)->state |= W_GRAPHICS_UISF_HIDEACCEL;
		}
		w_graphics_set_font(&item->gc, item->attr.font);
		memset(&item->event, 0, sizeof(item->event));
		item->event.event.platform_event = (struct w_event_platform*) e;
		item->event.event.widget = widget;
		item->event.column = &item->column;
		item->event.item = &item->item;
		item->event.gc = &item->gc;
		item->event.forground = 1;
//event.detail = W_FOREGROUND;
		if (clrTextBk != 0)
			//event.detail |= W_BACKGROUND;
			item->event.background = 1;
		if (hot)
			//event.detail |= W_HOT;
			item->event.hot = 1;
		if (selected)
			//event.detail |= W_SELECTED;
			item->event.selected = 1;
//if ((nmcd.uItemState & OS.CDIS_FOCUS) != 0) {
		if (SendMessageW(_W_WIDGET(widget)->handle, TVM_GETNEXTITEM,
		TVGN_CARET, 0) == item->nmcd->nmcd.dwItemSpec) {
			if (_W_WIDGET(widget)->handle == GetFocus()) {
				int uiState = (int) SendMessageW(_W_WIDGET(widget)->handle,
				WM_QUERYUISTATE, 0, 0);
				if ((uiState & UISF_HIDEFOCUS) == 0) {
					if (!_W_TREE(widget)->explorerTheme || !selected) {
						focused = TRUE;
						item->event.focused = 1;
					}
				}
			}
		}
		item->r.x = item->rect.left;
		item->r.y = item->rect.top;
		item->r.width = item->rect.right - item->rect.left;
		item->r.height = item->rect.bottom - item->rect.top;
		item->event.rect = &item->r;
//w_graphics_set_clipping_rect(&gc, &r);
		item->event.event.type = W_EVENT_ITEM_ERASE;
		item->event.doit = 1;
		ret = w_widget_send_event(widget, (w_event*) &item->event);
		int newTextClr = w_graphics_get_foreground(&item->gc); //data.foreground;
		w_graphics_dispose(&item->gc);
		RestoreDC(hDC, nSavedDC);
		if (ret) {
			_W_TREE(widget)->hooksEraseItem = TRUE;
			//if (isDisposed () || item.isDisposed ()) return null;
			if (item->event.doit) {
				_W_TREE(widget)->ignoreDrawForeground = !item->event.forground;/*  (event.detail
				 & W_FOREGROUND) == 0;*/
				_W_TREE(widget)->ignoreDrawBackground = !item->event.background;/*(event.detail
				 & W_BACKGROUND) == 0;*/
				_W_TREE(widget)->ignoreDrawSelection = !item->event.selected; /*(event.detail
				 & W_SELECTED) == 0;*/
				_W_TREE(widget)->ignoreDrawFocus = !item->event.focused;/* (event.detail & W_FOCUSED)
				 == 0;*/
				_W_TREE(widget)->ignoreDrawHot = !item->event.hot; /*(event.detail & W_HOT) == 0;*/
			} else {
				_W_TREE(widget)->ignoreDrawForeground =
				_W_TREE(widget)->ignoreDrawBackground =
				_W_TREE(widget)->ignoreDrawSelection =
				_W_TREE(widget)->ignoreDrawFocus =
				_W_TREE(widget)->ignoreDrawHot = W_TRUE;
			}
			if (selected && _W_TREE(widget)->ignoreDrawSelection)
				_W_TREE(widget)->ignoreDrawHot = TRUE;
			if (!_W_TREE(widget)->ignoreDrawBackground && clrTextBk != 0) {
				boolean draw = !selected && !hot;
				if (!_W_TREE(widget)->explorerTheme && selected)
					draw = !_W_TREE(widget)->ignoreDrawSelection;
				if (draw) {
					if (_W_TREE(widget)->hwndHeader == 0) {
						if ((_W_WIDGET(widget)->style & W_FULL_SELECTION)
								!= 0) {
							//fillBackground (hDC, clrTextBk, rect);
						} else {
							/*RECT textRect = item.getBounds (index, true, false, false, false, true, hDC);
							 if (measureEvent != null) {
							 textRect.right = Math.min (cellRect.right, boundsInPixels.x + boundsInPixels.width);
							 }
							 fillBackground (hDC, clrTextBk, textRect);*/
						}
					} else {
						//fillBackground (hDC, clrTextBk, cellRect);
					}
				}
			}
			if (_W_TREE(widget)->ignoreDrawSelection)
				_W_TREE(widget)->ignoreFullSelection = TRUE;
			if (!_W_TREE(widget)->ignoreDrawSelection
					|| !_W_TREE(widget)->ignoreDrawHot) {
				if (!selected && !hot) {
					_W_TREE(widget)->selectionForeground = clrText =
							GetSysColor(
							COLOR_HIGHLIGHTTEXT);
				}
				if (_W_TREE(widget)->explorerTheme) {
					/*if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) == 0) {
					 RECT pRect = item.getBounds (index, true, true, false, false, false, hDC);
					 RECT pClipRect = item.getBounds (index, true, true, true, false, true, hDC);
					 if (measureEvent != null) {
					 pRect.right = Math.min (pClipRect.right, boundsInPixels.x + boundsInPixels.width);
					 } else {
					 pRect.right += _W_TREE_EXPLORER_EXTRA;
					 pClipRect.right += _W_TREE_EXPLORER_EXTRA;
					 }
					 pRect.left -= _W_TREE_EXPLORER_EXTRA;
					 pClipRect.left -= _W_TREE_EXPLORER_EXTRA;
					 long  hTheme = OS.OpenThemeData (handle, Display.TREEVIEW);
					 int iStateId = selected ? OS.TREIS_SELECTED : OS.TREIS_HOT;
					 if (OS.GetFocus () != handle && selected && !hot) iStateId = OS.TREIS_SELECTEDNOTFOCUS;
					 OS.DrawThemeBackground (hTheme, hDC, OS.TVP_TREEITEM, iStateId, pRect, pClipRect);
					 OS.CloseThemeData (hTheme);
					 }*/
				} else {
					/*
					 * Feature in Windows.  When the tree has the style
					 * TVS_FULLROWSELECT, the background color for the
					 * entire row is filled when an item is painted,
					 * drawing on top of any custom drawing.  The fix
					 * is to emulate TVS_FULLROWSELECT.
					 */
					if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
						if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0
								&& _W_TREE(widget)->hwndHeader == 0) {
							//fillBackground (hDC, OS.GetBkColor (hDC), rect);
						} else {
							//fillBackground (hDC, OS.GetBkColor (hDC), cellRect);
						}
					} else {
						/*RECT textRect = item.getBounds (index, true, false, false, false, true, hDC);
						 if (measureEvent != null) {
						 textRect.right = Math.min (cellRect.right, boundsInPixels.x + boundsInPixels.width);
						 }
						 fillBackground (hDC, OS.GetBkColor (hDC), textRect);*/
					}
				}
			} else {
				if (selected || hot) {
					_W_TREE(widget)->selectionForeground = clrText = newTextClr;
					_W_TREE(widget)->ignoreDrawSelection =
					_W_TREE(widget)->ignoreDrawHot = TRUE;
				}
				if (_W_TREE(widget)->explorerTheme) {
					item->nmcd->nmcd.uItemState |= CDIS_DISABLED;
					/*
					 * Feature in Windows.  On Vista only, when the text
					 * color is unchanged and an item is asked to draw
					 * disabled, it uses the disabled color.  The fix is
					 * to modify the color so that is it no longer equal.
					 */
					int newColor =
					/*clrText == 0 ? getForegroundPixel () :*/clrText;
					if (item->nmcd->clrText == newColor) {
						item->nmcd->clrText |= 0x20000000;
						if (item->nmcd->clrText == newColor)
							item->nmcd->clrText &= ~0x20000000;
					} else {
						item->nmcd->clrText = newColor;
					}
				}
			}
			if (focused && !_W_TREE(widget)->ignoreDrawFocus
					&& (_W_WIDGET(widget)->style & W_FULL_SELECTION) == 0) {
				/*RECT textRect = item.getBounds (index, true, _W_TREE(widget)->explorerTheme, false, false, true, hDC);
				 if (measureEvent != null) {
				 textRect.right = Math.min (cellRect.right, boundsInPixels.x + boundsInPixels.width);
				 }
				 nmcd.uItemState &= ~OS.CDIS_FOCUS;
				 OS.MoveMemory (lParam, nmcd, NMTVCUSTOMDRAW.sizeof);
				 focusRect = textRect;*/
			}
			if (_W_TREE(widget)->explorerTheme) {
				if (selected || (hot && _W_TREE(widget)->ignoreDrawHot))
					item->nmcd->nmcd.uItemState &= ~CDIS_HOT;
			}
			RECT itemRect;
			_w_treeitem_get_bounds_0(&item->item, &itemRect, hDC, index,
			_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_GET_IMAGE);
			SaveDC(hDC);
			SelectClipRgn(hDC, 0);
			if (_W_TREE(widget)->explorerTheme) {
				itemRect.left -= _W_TREE_EXPLORER_EXTRA;
				itemRect.right += _W_TREE_EXPLORER_EXTRA;
			}
			//TODO - bug in Windows selection or SWT itemRect
			/*if (selected)*/itemRect.right++;
			if (_W_TREE(widget)->linesVisible)
				itemRect.bottom++;
			if (clipRect != 0) {
				IntersectClipRect(hDC, clipRect->left, clipRect->top,
						clipRect->right, clipRect->bottom);
			}
			ExcludeClipRect(hDC, itemRect.left, itemRect.top, itemRect.right,
					itemRect.bottom);
			e->result = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
			return W_TRUE;
		} //if (hooks (SWT.EraseItem))
		/*
		 * Feature in Windows.  When the tree has the style
		 * TVS_FULLROWSELECT, the background color for the
		 * entire row is filled when an item is painted,
		 * drawing on top of any custom drawing.  The fix
		 * is to emulate TVS_FULLROWSELECT.
		 */
		if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
			int bits = GetWindowLongW(_W_WIDGET(widget)->handle, GWL_STYLE);
			if ((bits & TVS_FULLROWSELECT) == 0) {
				if (selected) {
					//fillBackground (hDC, OS.GetBkColor (hDC), &nmcd->nmcd.rc);
				} else {
					//if (IsWindowEnabled (handle)) drawBackground (hDC, &nmcd->nmcd.rc);
				}
				item->nmcd->nmcd.uItemState &= ~CDIS_FOCUS;
			}
		}
	}
	int result = W_FALSE;
	if (clrText == 0 && clrTextBk == 0 && !w_font_isok(item->attr.font)) {
		e->result = CDRF_DODEFAULT | CDRF_NOTIFYPOSTPAINT;
		result = W_TRUE;
	} else {
		result = W_TRUE;
		e->result = CDRF_NEWFONT | CDRF_NOTIFYPOSTPAINT;
		if (w_font_isok(item->attr.font))
			SelectObject(hDC, _W_FONT(item->attr.font)->handle);
		if (IsWindowEnabled(_W_WIDGET(widget)->handle)
				&& IsWindowVisible(_W_WIDGET(widget)->handle)) {
			/*
			 * Feature in Windows.  Windows does not fill the entire cell
			 * with the background color when TVS_FULLROWSELECT is not set.
			 * The fix is to fill the cell with the background color.
			 */
			if (clrTextBk != 0) {
				int bits = GetWindowLongW(_W_WIDGET(widget)->handle,
				GWL_STYLE);
				if ((bits & TVS_FULLROWSELECT) == 0) {
					if (_W_TREE(widget)->hwndHeader != 0) {
						item->hdItem.mask = HDI_WIDTH;
						SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEM,
								index, (LPARAM) &item->hdItem);
						SetRect(&item->rect, item->nmcd->nmcd.rc.left,
								item->nmcd->nmcd.rc.top,
								item->nmcd->nmcd.rc.left + item->hdItem.cxy,
								item->nmcd->nmcd.rc.bottom);
						if (W_COMCTL32_VERSION < VERSION(6, 0)
								|| !IsAppThemed()) {
							RECT itemRect;
							*((HTREEITEM*) &itemRect) =
							_W_TREEITEM(&item->item)->htreeitem;
							if (SendMessageW(_W_WIDGET(widget)->handle,
							TVM_GETITEMRECT, TRUE, (LPARAM) &itemRect)) {
								item->rect.left = w_min(itemRect.left,
										item->rect.right);
							}
						}
						if ((_W_WIDGET(widget)->style & W_FULL_SELECTION)
								!= 0) {
							//if (!selected) fillBackground (hDC, clrTextBk, rect);
						} else {
							//fillBackground (hDC, clrTextBk, rect);
						}
					} else {
						if ((_W_WIDGET(widget)->style & W_FULL_SELECTION)
								!= 0) {
							//if (!selected) fillBackground (hDC, clrTextBk, &nmcd->nmcd.rc);
						}
					}
				}
			}
			if (!selected) {
				item->nmcd->clrText =
						clrText /*== -1 ? getForegroundPixel () : clrText*/;
				item->nmcd->clrTextBk =
						clrTextBk /*== -1 ? getBackgroundPixel () : clrTextBk*/;
			}
		}
	}
	if (IsWindowEnabled(_W_WIDGET(widget)->handle)) {
		/*
		 * On Vista only, when an item is asked to draw disabled,
		 * the background of the text is not filled with the
		 * background color of the tree.  This is true for both
		 * regular and full selection trees.  In order to draw a
		 * background image, mark the item as disabled using
		 * CDIS_DISABLED (when not selected) and set the text
		 * to the regular text color to avoid drawing disabled.
		 */
		if (_W_TREE(widget)->explorerTheme) {
			if (/*findImageControl () !=  null*/1) {
				if (!selected
						&& (item->nmcd->nmcd.uItemState
								& (CDIS_HOT | CDIS_SELECTED)) == 0) {
					item->nmcd->nmcd.uItemState |= CDIS_DISABLED;
					/*
					 * Feature in Windows.  On Vista only, when the text
					 * color is unchanged and an item is asked to draw
					 * disabled, it uses the disabled color.  The fix is
					 * to modify the color so it is no longer equal.
					 */
					/*int newColor = clrText == 0 ? getForegroundPixel () : clrText;
					 if (nmcd.clrText == newColor) {
					 nmcd.clrText |= 0x20000000;
					 if (nmcd.clrText == newColor) nmcd.clrText &= ~0x20000000;
					 } else {
					 nmcd.clrText = newColor;
					 }
					 OS.MoveMemory (lParam, nmcd, NMTVCUSTOMDRAW.sizeof);
					 if (clrTextBk != 0) {
					 if ((style & SWT.FULL_SELECTION) != 0) {
					 RECT rect = new RECT ();
					 if (columnCount != 0) {
					 HDITEM hdItem = new HDITEM ();
					 hdItem.mask = OS.HDI_WIDTH;
					 OS.SendMessage (hwndHeader, OS.HDM_GETITEM, index, hdItem);
					 OS.SetRect (rect, nmcd.left, nmcd.top, nmcd.left + hdItem.cxy, nmcd.bottom);
					 } else {
					 OS.SetRect (rect, nmcd.left, nmcd.top, nmcd.right, nmcd.bottom);
					 }
					 fillBackground (hDC, clrTextBk, rect);
					 } else {
					 RECT textRect = item.getBounds (index, true, false, true, false, true, hDC);
					 fillBackground (hDC, clrTextBk, textRect);
					 }
					 }*/
				}
			}
		}
	} else {
		/*
		 * Feature in Windows.  When the tree is disabled, it draws
		 * with a gray background over the sort column.  The fix is
		 * to fill the background with the sort column color.
		 */
		if (clrSortBk != -1) {
			item->hdItem.mask = HDI_WIDTH;
			SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEM, index,
					(LPARAM) &item->hdItem);
			SetRect(&item->rect, item->nmcd->nmcd.rc.left,
					item->nmcd->nmcd.rc.top,
					item->nmcd->nmcd.rc.left + item->hdItem.cxy,
					item->nmcd->nmcd.rc.bottom);
			//fillBackground (hDC, clrSortBk, rect);
		}
	}
	SaveDC(hDC);
	if (clipRect != 0) {
		HRGN hRgn = CreateRectRgn(clipRect->left, clipRect->top,
				clipRect->right, clipRect->bottom);
		POINT lpPoint;
		GetWindowOrgEx(hDC, &lpPoint);
		OffsetRgn(hRgn, -lpPoint.x, -lpPoint.y);
		SelectClipRgn(hDC, hRgn);
		DeleteObject(hRgn);
	}
	return result;
}

void _TREE_WM_NOTIFY_CDDS_init(struct cdds_item *item) {
	_TREE_WM_NOTIFY_CDDS_init_var(item);
	HDC hDC = item->nmcd->nmcd.hdc;
	if ((_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0 /*|| findImageControl () != null*/
	|| _W_TREE(item->widget)->ignoreDrawSelection
			|| _W_TREE(item->widget)->explorerTheme) {
		SetBkMode(hDC, TRANSPARENT);
	}
	item->selected = _w_tree_is_item_selected(item->widget, item->nmcd);
	item->hot = _W_TREE(item->widget)->explorerTheme
			&& (item->nmcd->nmcd.uItemState & CDIS_HOT) != 0;
	if (IsWindowEnabled(_W_WIDGET(item->widget)->handle)) {
		if (_W_TREE(item->widget)->explorerTheme) {
			int bits = GetWindowLongW(_W_WIDGET(item->widget)->handle,
			GWL_STYLE);
			if ((bits & TVS_TRACKSELECT) != 0) {
				if ((_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0
						&& (item->selected || item->hot)) {
					SetTextColor(hDC, GetSysColor(COLOR_WINDOWTEXT));
				} else {
					//SetTextColor (hDC, getForegroundPixel ());
				}
			}
		}
	}
	//int* order =0;
	HWND scrolledHandle = _w_tree_scrolled_handle(item->widget);
	GetClientRect(scrolledHandle, &item->clientRect);
	if (_W_TREE(item->widget)->hwndHeader != 0) {
		MapWindowPoints(_W_TREE(item->widget)->hwndParent,
		_W_WIDGET(item->widget)->handle, (LPPOINT) &item->clientRect, 2);
		/*if (columnCount != 0) {
		 order = new int [columnCount];
		 OS.SendMessage (_W_TREE(widget)->hwndHeader, OS.HDM_GETORDERARRAY, columnCount, order);
		 }*/
	}
	item->sortIndex = -1;
	item->clrSortBk = 0;
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		if (_W_TREE(item->widget)->sortColumn != -1
				&& _W_TREE(item->widget)->sortDirection != W_NONE) {
			/*if (findImageControl () == null) {
			 sortIndex = indexOf (sortColumn);
			 clrSortBk = getSortColumnPixel ();
			 }*/
		}
	}
}
void _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_drawtext(struct cdds_item *item) {
	/*
	 * Bug in Windows.  When DrawText() is used with DT_VCENTER
	 * and DT_ENDELLIPSIS, the ellipsis can draw outside of the
	 * rectangle when the rectangle is empty.  The fix is avoid
	 * all text drawing for empty rectangles.
	 */
	w_value value;
	int ret;
	if (item->rect.left < item->rect.right) {
		WCHAR *string = 0;
		int string_length;
		ret = 0;
		if (_W_TREECOLUMN(&item->column)->index == 0) {
			TV_ITEMW tvi;
			tvi.mask = TVIF_TEXT | TVIF_HANDLE;
			tvi.hItem = (HTREEITEM) item->nmcd->nmcd.dwItemSpec;
			tvi.pszText = win_toolkit->tmp;
			tvi.cchTextMax = win_toolkit->tmp_alloc / sizeof(WCHAR);
			string_length = SendMessageW(
			_W_WIDGET(item->widget)->handle, TVM_GETITEMW, 0, (WPARAM) &tvi);
			string = tvi.pszText;
			string_length = tvi.cchTextMax;
		} else {
			memset(&item->event, 0, sizeof(item->event));
			item->event.event.type = W_EVENT_ITEM_GET_VALUE;
			item->event.event.widget = item->widget;
			item->event.event.platform_event =
					(struct w_event_platform*) item->e;
			item->event.column = &item->column;
			item->event.item = &item->item;
			item->event.value = &value;
			w_value_init(&value);
			w_value_set_string(&value,
					(char*) &win_toolkit->tmp[win_toolkit->tmp_alloc / 2],
					win_toolkit->tmp_alloc / sizeof(WCHAR),
					W_VALUE_USER_MEMORY);
			ret = w_widget_send_event(item->widget, (w_event*) &item->event);
			if (ret) {
				ret = 0;
				char *text =
						w_value_string_copy_is_needed(&value,
								(char*) &win_toolkit->tmp[win_toolkit->tmp_alloc
										/ 2 * sizeof(WCHAR)],
										win_toolkit->tmp_alloc / sizeof(WCHAR));
				string_length = w_utf8_to_utf16(text, -1, 0, 0);
				if ((string_length + 1)
						>= win_toolkit->tmp_alloc / sizeof(WCHAR)) {
					string = malloc(string_length + 1);
					ret = 1;
				} else {
					string = win_toolkit->tmp;
				}
				if (string != 0)
					w_utf8_to_utf16(text, -1, string, string_length + 1);
			} else
				ret = 0;
			/*String [] strings  = item.strings;
			 if (strings != null) string = strings [index];*/
		}
		if (string != 0) {
			HFONT lastfont;
			HDC hDC = item->nmcd->nmcd.hdc;
			int clrText = -1, clrTextBk = -1;
			if (w_font_isok(item->attr.font))
				lastfont = SelectObject(hDC,
				_W_FONT(item->attr.font)->handle);
			if (item->clrText != 0)
				clrText = SetTextColor(hDC, item->clrText & 0x00FFFFFF);
			if (item->clrTextBk != 0)
				clrTextBk = SetBkColor(hDC, item->clrTextBk & 0x00FFFFFF);
			int flags = DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER;
			if (_W_TREECOLUMN(&item->column)->index != 0)
				flags |= DT_END_ELLIPSIS;
			item->hdItem.mask = HDI_FORMAT;
			item->hdItem.fmt = 0;
			SendMessageW(_W_TREE(item->widget)->hwndHeader,
			HDM_GETITEMW, _W_TREECOLUMN(&item->column)->index,
					(LPARAM) &item->hdItem);
			if (item->hdItem.fmt & HDF_LEFT)
				flags |= DT_LEFT;
			if (item->hdItem.fmt & HDF_CENTER)
				flags |= DT_CENTER;
			if (item->hdItem.fmt & HDF_RIGHT)
				flags |= DT_RIGHT;
			if (!_W_TREE(item->widget)->ignoreDrawForeground)
				DrawTextW(hDC, string, string_length, &item->rect, flags);
			DrawTextW(hDC, string, string_length, &item->rect,
					flags | DT_CALCRECT);
			if (w_font_isok(item->attr.font))
				SelectObject(hDC, lastfont);
			if (clrText != -1)
				SetTextColor(hDC, clrText);
			if (clrTextBk != -1)
				SetBkColor(hDC, clrTextBk);
			if (ret == 1)
				free(string);
		}
	}
}
void _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_drawitem(struct cdds_item *item) {
	HDC hDC = item->nmcd->nmcd.hdc;
	if (_W_TREECOLUMN(&item->column)->index != 0) {
		/*if (hooks (SWT.MeasureItem)) {
		 sendMeasureItemEvent (item, index, hDC, selected ? SWT.SELECTED : 0);
		 if (isDisposed () || item.isDisposed ()) break;
		 }*/
		item->measureEvent = _w_tree_send_measure_item_event(item,
				item->selected ? W_SELECTED : 0);

		RECT cellRect;
		_w_treeitem_get_bounds_0(&item->item, &cellRect, hDC,
		_W_TREECOLUMN(&item->column)->index,
				_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_FULLTEXT
						| _W_TREE_BOUNDS_GET_IMAGE | _W_TREE_BOUNDS_FULLIMAGE
						| _W_TREE_BOUNDS_CLIP);
		int nSavedDC = SaveDC(hDC);
		_w_graphics_init(&item->gc, hDC);
		w_graphics_set_foreground(&item->gc, GetTextColor(hDC));
		w_graphics_set_background(&item->gc, GetBkColor(hDC));
		if (!item->selected
				|| (_W_WIDGET(item->widget)->style & W_FULL_SELECTION) == 0) {
			if (item->clrText != 0)
				w_graphics_set_foreground(&item->gc, item->clrText);
			if (item->clrTextBk != 0)
				w_graphics_set_background(&item->gc, item->clrTextBk);
		}
		if (w_font_isok(item->attr.font)) {
			w_graphics_set_font(&item->gc, item->attr.font);
		}
		int uiState = (int) SendMessageW(_W_WIDGET(item->widget)->handle,
		WM_QUERYUISTATE, 0, 0);
		if (uiState & UISF_HIDEFOCUS) {
			_W_GRAPHICS(&item->gc)->state |= W_GRAPHICS_UISF_HIDEFOCUS;
		}
		if (uiState & UISF_HIDEACCEL) {
			_W_GRAPHICS(&item->gc)->state |= W_GRAPHICS_UISF_HIDEACCEL;
		}
		item->event.item = &item->item;
		item->event.column = &item->column;
		item->event.gc = &item->gc;
		item->event.forground = 1;
		item->event.attr = 0;
		item->event.doit = 1;
		if (item->clrTextBk != 0)
			item->event.background = 1;
		if ((_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0) {
			if (item->hot)
				item->event.hot = 1;
			if (item->selected)
				item->event.selected = 1;
			if (!_W_TREE(item->widget)->explorerTheme) {
				//if ((nmcd.uItemState & OS.CDIS_FOCUS) != 0) {
				if (SendMessageW(_W_WIDGET(item->widget)->handle,
				TVM_GETNEXTITEM, TVGN_CARET, 0)
						== item->nmcd->nmcd.dwItemSpec) {
					if (_W_WIDGET(item->widget)->handle == GetFocus()) {
						int uiState = SendMessageW(
						_W_WIDGET(item->widget)->handle,
						WM_QUERYUISTATE, 0, 0);
						if ((uiState & UISF_HIDEFOCUS) == 0)
							item->event.focused = 1;
					}
				}
			}
		}
		item->event.rect = &item->r;
		item->r.x = cellRect.left;
		item->r.y = cellRect.top;
		item->r.width = cellRect.right - cellRect.left;
		item->r.height = cellRect.bottom - cellRect.top;
		//w_graphics_set_clipping_rect(&gc, &r);
		item->event.event.type = W_EVENT_ITEM_ERASE;
		int ret = w_widget_send_event(item->widget, (w_event*) &item->event);
		int newTextClr = w_graphics_get_foreground(&item->gc);
		w_graphics_dispose(&item->gc);
		RestoreDC(hDC, nSavedDC);
		//if (isDisposed () || item.isDisposed ()) break;
		if (ret) {
			//if (event.doit) {
			if (item->event.doit) {
				_W_TREE(item->widget)->ignoreDrawForeground =
						!item->event.forground;
				/*(event.detail & W_FOREGROUND) == 0;*/
				_W_TREE(item->widget)->ignoreDrawBackground =
						!item->event.background;
				/*(event.detail & W_BACKGROUND) == 0;*/
				if ((_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0) {
					_W_TREE(item->widget)->ignoreDrawSelection =
							!item->event.selected;
					/*(event.detail & W_SELECTED) == 0;*/
					_W_TREE(item->widget)->ignoreDrawFocus =
							!item->event.focused; /*(event.detail
							 & W_FOCUSED) == 0;*/
					_W_TREE(item->widget)->ignoreDrawHot = !item->event.hot;/*(event.detail
					 & W_HOT) == 0;*/
				}
			} else {
				_W_TREE(item->widget)->ignoreDrawForeground =
				_W_TREE(item->widget)->ignoreDrawBackground =
				_W_TREE(item->widget)->ignoreDrawSelection =
				_W_TREE(item->widget)->ignoreDrawFocus =
				_W_TREE(item->widget)->ignoreDrawHot =
				TRUE;
			}
			if (item->selected && _W_TREE(item->widget)->ignoreDrawSelection)
				_W_TREE(item->widget)->ignoreDrawHot = TRUE;
			if ((_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0) {
				if (_W_TREE(item->widget)->ignoreDrawSelection)
					_W_TREE(item->widget)->ignoreFullSelection = TRUE;
				if (!_W_TREE(item->widget)->ignoreDrawSelection
						|| !_W_TREE(item->widget)->ignoreDrawHot) {
					if (!item->selected && !item->hot) {
						_W_TREE(item->widget)->selectionForeground =
								GetSysColor(
								COLOR_HIGHLIGHTTEXT);
					} else {
						if (!_W_TREE(item->widget)->explorerTheme) {
							item->drawBackground = TRUE;
							_W_TREE(item->widget)->ignoreDrawBackground =
							FALSE;
							if ((_W_WIDGET(item->widget)->handle == GetFocus() /*|| display.getHighContrast ()*/)
									&& IsWindowEnabled(
									_W_WIDGET(item->widget)->handle)) {
								item->clrTextBk = GetSysColor(
								COLOR_HIGHLIGHT);
							} else {
								item->clrTextBk = GetSysColor(
								COLOR_3DFACE);
							}
							if (!_W_TREE(item->widget)->ignoreFullSelection
									&& _W_TREECOLUMN(&item->column)->index
											== item->columnCount - 1) {
								//RECT selectionRect;
								SetRect(&item->rect_tmp,
										item->backgroundRect->left,
										item->backgroundRect->top,
										item->nmcd->nmcd.rc.right,
										item->backgroundRect->bottom);
								item->backgroundRect = &item->rect_tmp;
							}
						} else {
							/*RECT pRect;
							 SetRect (&pRect, nmcd->nmcd.rc.left, nmcd->nmcd.rc.top, nmcd->nmcd.rc.right, nmcd->nmcd.rc.bottom);
							 if (_W_TREE(widget)->hwndHeader != 0) {
							 int totalWidth = 0;
							 HDITEM hdItem = new HDITEM ();
							 hdItem.mask = OS.HDI_WIDTH;
							 for (int j=0; j<columnCount; j++) {
							 OS.SendMessage (hwndHeader, OS.HDM_GETITEM, j, hdItem);
							 totalWidth += hdItem.cxy;
							 }
							 if (totalWidth > clientRect.right - clientRect.left) {
							 pRect.left = 0;
							 pRect.right = totalWidth;
							 } else {
							 pRect.left = clientRect.left;
							 pRect.right = clientRect.right;
							 }
							 if (index == columnCount - 1) {
							 RECT selectionRect = new RECT ();
							 OS.SetRect (selectionRect, backgroundRect.left, backgroundRect.top, pRect.right, backgroundRect.bottom);
							 backgroundRect = selectionRect;
							 }
							 }
							 long hTheme = OS.OpenThemeData (handle, Display.TREEVIEW);
							 int iStateId = selected ? OS.TREIS_SELECTED : OS.TREIS_HOT;
							 if (OS.GetFocus () != handle && selected && !hot) iStateId = OS.TREIS_SELECTEDNOTFOCUS;
							 OS.DrawThemeBackground (hTheme, hDC, OS.TVP_TREEITEM, iStateId, pRect, backgroundRect);
							 OS.CloseThemeData (hTheme);*/
						}
					}
				} else {
					if (item->selected) {
						_W_TREE(item->widget)->selectionForeground = newTextClr;
						if (!_W_TREE(item->widget)->explorerTheme) {
							if (item->clrTextBk == 0 && IsWindowEnabled(
							_W_WIDGET(item->widget)->handle)) {
								/*Control control = findBackgroundControl ();
								 if (control == null) control = this;
								 clrTextBk = control.getBackgroundPixel ();*/
							}
						}
					}
				}
			}
		}						//if (hooks (SWT.EraseItem))
		if (_W_TREE(item->widget)->selectionForeground != 0)
			item->clrText = _W_TREE(item->widget)->selectionForeground;
	}
	if (!_W_TREE(item->widget)->ignoreDrawBackground) {
		if (item->clrTextBk != 0) {
			//if (drawBackground) fillBackground (hDC, clrTextBk, backgroundRect);
		} else {
			/*Control control = findImageControl ();
			 if (control != null) {
			 if (i == 0) {
			 int right = Math.min (rect.right, width);
			 OS.SetRect (rect, rect.left, rect.top, right, rect.bottom);
			 if (drawBackground) fillImageBackground (hDC, control, rect, 0, 0);
			 } else {
			 if (drawBackground) fillImageBackground (hDC, control, rect, 0, 0);
			 }
			 }*/
		}
	}
	item->rect.left += _W_TREE_INSET - 1;
	if (item->drawImage) {
		int image = -1;
		if (_W_TREECOLUMN(&item->column)->index == 0) {
			TV_ITEMW tvi;
			tvi.mask = TVIF_IMAGE | TVIF_HANDLE;
			tvi.hItem = (HTREEITEM) item->nmcd->nmcd.dwItemSpec;
			tvi.iImage = -1;
			SendMessageW(_W_WIDGET(item->widget)->handle, TVM_GETITEMW, 0,
					(WPARAM) &tvi);
			image = tvi.iImage;
		} else {
			/*Image [] images  = item.images;
			 if (images != null) image = images [index];*/
		}
		HIMAGELIST himagelist = (HIMAGELIST) SendMessageW(
		_W_WIDGET(item->widget)->handle, TVM_GETIMAGELIST,
		TVSIL_NORMAL, 0);
		int cx = 0, cy = 0;
		if (himagelist != 0)
			ImageList_GetIconSize(himagelist, &cx, &cy);
		int inset =
		_W_TREECOLUMN(&item->column)->index != 0 ? _W_TREE_INSET : 0;
		int offset =
		_W_TREECOLUMN(&item->column)->index != 0 ?
		_W_TREE_INSET :
													_W_TREE_INSET + 2;
		if (image != -1) {
			//Rectangle bounds = image.getBounds (); // Points
			//if (size == null) size = DPIUtil.autoScaleDown (getImageSize ()); // To Points
			if (!_W_TREE(item->widget)->ignoreDrawForeground) {
				//int y1 = rect.top + (index == 0 ? (getItemHeight () - size.y) / 2 : 0);
				int y1 = item->rect.top;
				int x1 = w_max(item->rect.left, item->rect.left - inset + 1);
				if (himagelist != 0)
					ImageList_DrawEx(himagelist, image, hDC, x1, y1, cx, cy, -1,
							-1, ILD_NORMAL);
				/*GC gc = GC.win32_new (hDC, data);
				 gc.setClipping (DPIUtil.autoScaleDown(new Rectangle(x1, rect.top, rect.right - x1, rect.bottom - rect.top)));
				 gc.drawImage (image, 0, 0, bounds.width, bounds.height, DPIUtil.autoScaleDown(x1), DPIUtil.autoScaleDown(y1), size.x, size.y);
				 OS.SelectClipRgn (hDC, 0);
				 gc.dispose ();*/
			}
			SetRect(&item->rect, item->rect.left + cx + offset, item->rect.top,
					item->rect.right - inset, item->rect.bottom);
		} else {
			if (_W_TREECOLUMN(&item->column)->index == 0) {
				if (himagelist) {
					//if (size == null) size = getImageSize ();
					item->rect.left = w_min(item->rect.left + cx + offset,
							item->rect.right);
				}
			} else {
				SetRect(&item->rect, item->rect.left + offset, item->rect.top,
						item->rect.right - inset, item->rect.bottom);
			}
		}
	}
	if (item->drawText) {
		_TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_drawtext(item);
	}
}
void _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_paintitem(struct cdds_item *item) {
	HDC hDC = item->nmcd->nmcd.hdc;
	//if (hooks (SWT.PaintItem)) {
	//RECT itemRect = item.getBounds (index, true, true, false, false, false, hDC);
	RECT itemRect;
	_w_treeitem_get_bounds_0(&item->item, &itemRect, hDC,
	_W_TREECOLUMN(&item->column)->index,
	_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_GET_IMAGE);
	int nSavedDC = SaveDC(hDC);
	_w_graphics_init(&item->gc, hDC);
	if (w_font_isok(item->attr.font) > 0) {
		w_graphics_set_font(&item->gc, item->attr.font);
	}
	w_graphics_set_foreground(&item->gc, GetTextColor(hDC));
	w_graphics_set_background(&item->gc, GetBkColor(hDC));
	if (item->selected
			&& (_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0) {
		if (_W_TREE(item->widget)->selectionForeground != 0)
			w_graphics_set_foreground(&item->gc,
			_W_TREE(item->widget)->selectionForeground);
	} else {
		if (item->clrText != 0)
			w_graphics_set_foreground(&item->gc, item->clrText);
		if (item->clrTextBk != 0)
			w_graphics_set_background(&item->gc, item->clrTextBk);
	}
	int uiState = (int) SendMessageW(_W_WIDGET(item->widget)->handle,
	WM_QUERYUISTATE, 0, 0);
	if (uiState & UISF_HIDEFOCUS) {
		_W_GRAPHICS(&item->gc)->state |= W_GRAPHICS_UISF_HIDEFOCUS;
	}
	if (uiState & UISF_HIDEACCEL) {
		_W_GRAPHICS(&item->gc)->state |= W_GRAPHICS_UISF_HIDEACCEL;
	}
	item->event.item = &item->item;
	item->event.column = &item->column;
	item->event.gc = &item->gc;
	//event.detail |= W_FOREGROUND;
	item->event.forground = 1;
	if (item->clrTextBk != 0)
		//event.detail |= W_BACKGROUND;
		item->event.background = 1;
	if (item->hot)
		//event.detail |= W_HOT;
		item->event.hot = 1;
	if (item->selected && (_W_TREECOLUMN(&item->column)->index == 0 /*nmcd.iSubItem == 0*/
	|| (_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0)) {
		//event.detail |= W_SELECTED;
		item->event.selected = 1;
	}
	if (!_W_TREE(item->widget)->explorerTheme) {
		//if ((nmcd.uItemState & OS.CDIS_FOCUS) != 0) {
		if (SendMessageW(_W_WIDGET(item->widget)->handle, TVM_GETNEXTITEM,
		TVGN_CARET, 0) == item->nmcd->nmcd.dwItemSpec) {
			if (_W_TREECOLUMN(&item->column)->index == 0 /*nmcd.iSubItem == 0*/
			|| (_W_WIDGET(item->widget)->style & W_FULL_SELECTION) != 0) {
				if (_W_WIDGET(item->widget)->handle == GetFocus()) {
					int uiState = (int) SendMessageW(
					_W_WIDGET(item->widget)->handle, WM_QUERYUISTATE, 0, 0);
					if ((uiState & UISF_HIDEFOCUS) == 0)
						//event.detail |= W_FOCUSED;
						item->event.focused = 1;
				}
			}
		}
	}
	item->event.rect = &item->r;
	//RECT cellRect = item.getBounds (index, true, true, true, true, true, hDC);
	RECT cellRect;
	_w_treeitem_get_bounds_0(&item->item, &cellRect, hDC,
	_W_TREECOLUMN(&item->column)->index,
			_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_GET_IMAGE
					| _W_TREE_BOUNDS_FULLTEXT | _W_TREE_BOUNDS_FULLIMAGE
					| _W_TREE_BOUNDS_CLIP);
	//int cellWidth = cellRect.right - cellRect.left;
	//int cellHeight = cellRect.bottom - cellRect.top;
	item->r.x = cellRect.left;
	item->r.y = cellRect.top;
	item->r.width = cellRect.right - cellRect.left;
	item->r.height = cellRect.bottom - cellRect.top;
	w_graphics_set_clipping_rect(&item->gc, &item->r);
	/*item->r.x = itemRect.left;
	 item->r.y = itemRect.top;
	 item->r.width = itemRect.right - itemRect.left;
	 item->r.height = itemRect.bottom - itemRect.top;*/
	item->event.event.type = W_EVENT_ITEM_PAINT;

	int ret = w_widget_send_event(item->widget, (w_event*) &item->event);
	//if (data.focusDrawn) focusRect = null;
	w_graphics_dispose(&item->gc);
	RestoreDC(hDC, nSavedDC);
	//if (isDisposed () || item.isDisposed ()) break;
	//}if (hooks (SWT.PaintItem))
}

int _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT(w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	if (_W_TREE(widget)->ignoreCustomDraw)
		return W_FALSE;
	struct cdds_item item_tmp, *item;
	item = &item_tmp;
	item->widget = widget;
	item->e = e;
	item->nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	if (item->nmcd->nmcd.rc.left == item->nmcd->nmcd.rc.right) {
		e->result = CDRF_DODEFAULT;
		return W_TRUE;
	}
	HDC hDC = item->nmcd->nmcd.hdc;
	RestoreDC(hDC, -1);

	/*
	 * Feature in Windows.  When a new tree item is inserted
	 * using TVM_INSERTITEM and the tree is using custom draw,
	 * a NM_CUSTOMDRAW is sent before TVM_INSERTITEM returns
	 * and before the item is added to the items array.  The
	 * fix is to check for null.
	 *
	 * NOTE: This only happens on XP with the version 6.00 of
	 * COMCTL32.DLL,
	 */
	//if (item == null) return null;
	if (item->nmcd->nmcd.dwItemSpec == 0)
		return W_FALSE;

	/*
	 * Feature in Windows.  Under certain circumstances, Windows
	 * sends CDDS_ITEMPOSTPAINT for an empty rectangle.  This is
	 * not a problem providing that graphics do not occur outside
	 * the rectangle.  The fix is to test for the rectangle and
	 * draw nothing.
	 *
	 * NOTE:  This seems to happen when both I_IMAGECALLBACK
	 * and LPSTR_TEXTCALLBACK are used at the same time with
	 * TVM_SETITEM.
	 */
	if (item->nmcd->nmcd.rc.left >= item->nmcd->nmcd.rc.right
			|| item->nmcd->nmcd.rc.top >= item->nmcd->nmcd.rc.bottom)
		return W_FALSE;
	if (!IsWindowVisible(_W_WIDGET(widget)->handle))
		return W_FALSE;
	int i, ret;
	_TREE_WM_NOTIFY_CDDS_init(item);

	int x = 0;
	//w_point *size = 0;
	for (int i = 0; i < item->columnCount; i++) {
//int index = order == null ? i : order [i], width = nmcd.right - nmcd.left;
		int index = i;
		int width = item->nmcd->nmcd.rc.right - item->nmcd->nmcd.rc.left;
		if (_W_TREE(widget)->hwndHeader != 0) {
			/*hdItem.mask = HDI_WIDTH;
			 SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEM, index,
			 (LPARAM) &hdItem);
			 width = hdItem.cxy;*/
			SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEMRECT, index,
					(LPARAM) &item->rect);
			width = item->rect.right - item->rect.left;
			x = item->rect.left;
		}
		/*x += width;*/
		if (x > item->clientRect.right)
			break;
		_W_TREECOLUMN(&item->column)->index = i;
		_TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_getAttr(item);
		if (i == 0) {
			if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
				item->clear =
						!_W_TREE(widget)->explorerTheme
								&& !_W_TREE(widget)->ignoreDrawSelection /*&& findImageControl () == null*/;
				if (item->clear
						|| (item->selected
								&& !_W_TREE(widget)->ignoreDrawSelection)
						|| (item->hot && !_W_TREE(widget)->ignoreDrawHot)) {
					item->draw = TRUE;
					RECT pClipRect;
					SetRect(&pClipRect, width, item->nmcd->nmcd.rc.top,
							item->nmcd->nmcd.rc.right,
							item->nmcd->nmcd.rc.bottom);
					if (_W_TREE(widget)->explorerTheme) {
						/*if (hooks (SWT.EraseItem)) {
						 RECT itemRect = item.getBounds (index, true, true, false, false, true, hDC);
						 itemRect.left -= _W_TREE_EXPLORER_EXTRA;
						 itemRect.right += _W_TREE_EXPLORER_EXTRA + 1;
						 pClipRect.left = itemRect.left;
						 pClipRect.right = itemRect.right;
						 if (_W_TREE(widget)->hwndHeader != 0) {
						 HDITEM hdItem = new HDITEM ();
						 hdItem.mask = OS.HDI_WIDTH;
						 OS.SendMessage (hwndHeader, OS.HDM_GETITEM, index, hdItem);
						 pClipRect.right = Math.min (pClipRect.right, nmcd.left + hdItem.cxy);
						 }
						 }
						 RECT pRect = new RECT ();
						 OS.SetRect (pRect, nmcd.left, nmcd.top, nmcd.right, nmcd.bottom);
						 if (columnCount > 0 && hwndHeader != 0) {
						 int totalWidth = 0;
						 HDITEM hdItem = new HDITEM ();
						 hdItem.mask = OS.HDI_WIDTH;
						 for (int j=0; j<columnCount; j++) {
						 OS.SendMessage (hwndHeader, OS.HDM_GETITEM, j, hdItem);
						 totalWidth += hdItem.cxy;
						 }
						 if (totalWidth > clientRect.right - clientRect.left) {
						 pRect.left = 0;
						 pRect.right = totalWidth;
						 } else {
						 pRect.left = clientRect.left;
						 pRect.right = clientRect.right;
						 }
						 }
						 draw = false;
						 long hTheme = OS.OpenThemeData (handle, Display.TREEVIEW);
						 int iStateId = selected ? OS.TREIS_SELECTED : OS.TREIS_HOT;
						 if (OS.GetFocus () != handle && selected && !hot) iStateId = OS.TREIS_SELECTEDNOTFOCUS;
						 OS.DrawThemeBackground (hTheme, hDC, OS.TVP_TREEITEM, iStateId, pRect, pClipRect);
						 OS.CloseThemeData (hTheme);*/
					}
					if (item->draw) {
						//fillBackground (hDC, GetBkColor (hDC), &pClipRect);
					}
				}
			}
		} //if (i == 0)
		if (x + width > item->clientRect.left) {
			item->backgroundRect = 0;
			item->drawItem = TRUE;
			item->drawText = TRUE;
			item->drawImage = TRUE;
			item->drawBackground = FALSE;
			if (i == 0) {
				item->drawItem = item->drawImage = item->drawText = FALSE;
				/*if (findImageControl () != null) {
				 if (explorerTheme) {
				 if (OS.IsWindowEnabled (handle) && !hooks (SWT.EraseItem)) {
				 Image image = null;
				 if (index == 0) {
				 image = item.image;
				 } else {
				 Image [] images  = item.images;
				 if (images != null) image = images [index];
				 }
				 if (image != null) {
				 Rectangle bounds = image.getBounds (); // Points
				 if (size == null) size = DPIUtil.autoScaleDown (getImageSize ()); // To Points
				 if (!ignoreDrawForeground) {
				 GCData data = new GCData();
				 data.device = display;
				 GC gc = GC.win32_new (hDC, data);
				 RECT iconRect = item.getBounds (index, false, true, false, false, true, hDC); // Pixels
				 gc.setClipping (DPIUtil.autoScaleDown(new Rectangle(iconRect.left, iconRect.top, iconRect.right - iconRect.left, iconRect.bottom - iconRect.top)));
				 gc.drawImage (image, 0, 0, bounds.width, bounds.height, DPIUtil.autoScaleDown(iconRect.left), DPIUtil.autoScaleDown(iconRect.top), size.x, size.y);
				 OS.SelectClipRgn (hDC, 0);
				 gc.dispose ();
				 }
				 }
				 }
				 } else {
				 drawItem = drawText = drawBackground = true;
				 rect = item.getBounds (index, true, false, false, false, true, hDC);
				 if (linesVisible) {
				 rect.right++;
				 rect.bottom++;
				 }
				 }
				 }*/
				if (item->selected && !_W_TREE(widget)->ignoreDrawSelection
						&& !_W_TREE(widget)->ignoreDrawBackground) {
					//if (!_W_TREE(widget)->explorerTheme) fillBackground (hDC, OS.GetBkColor (hDC), rect);
					item->drawBackground = FALSE;
				}
				item->backgroundRect = &item->rect;
				//if (hooks (SWT.EraseItem)) {
				if (_W_TREE(widget)->hooksEraseItem) {
					item->drawItem = item->drawText = item->drawImage = TRUE;
					_w_treeitem_get_bounds_0(&item->item, &item->rect, hDC,
							index,
							_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_GET_IMAGE
									| _W_TREE_BOUNDS_CLIP);
					//rect = item.getBounds (index, true, true, false, false, true, hDC);
					if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
						item->backgroundRect = &item->rect;
					} else {
						//backgroundRect = item.getBounds (index, true, false, false, false, true, hDC);
						_w_treeitem_get_bounds_0(&item->item,
								item->backgroundRect, hDC, index,
								_W_TREE_BOUNDS_GET_TEXT | _W_TREE_BOUNDS_CLIP);
					}
				} // if (hooks (SWT.EraseItem))
			} else {
				_W_TREE(widget)->selectionForeground = 0;
				_W_TREE(widget)->ignoreDrawForeground =
				_W_TREE(widget)->ignoreDrawBackground =
				_W_TREE(widget)->ignoreDrawSelection =
				_W_TREE(widget)->ignoreDrawFocus =
				_W_TREE(widget)->ignoreDrawHot =
				FALSE;
				SetRect(&item->rect, x, item->nmcd->nmcd.rc.top, x + width,
						item->nmcd->nmcd.rc.bottom);
				item->backgroundRect = &item->rect;
			}
			item->clrText = 0, item->clrTextBk = 0;
			//long hFont = item.fontHandle (index);
			if (_W_TREE(widget)->selectionForeground != 0)
				item->clrText = _W_TREE(widget)->selectionForeground;
			if (IsWindowEnabled(_W_WIDGET(widget)->handle)) {
				item->drawForeground = FALSE;
				if (item->selected) {
					if (i != 0
							&& (_W_WIDGET(widget)->style & W_FULL_SELECTION)
									== 0) {
						//SetTextColor (hDC, getForegroundPixel ());
						//SetBkColor (hDC, getBackgroundPixel ());
						item->drawForeground = item->drawBackground = TRUE;
					}
				} else {
					item->drawForeground = item->drawBackground = TRUE;
				}
				if (item->drawForeground) {
					/*clrText = item.cellForeground != null ? item.cellForeground [index] : -1;
					 if (clrText == -1) clrText = item.foreground;*/
					item->clrText = item->attr.foreground;
				}
				if (item->drawBackground) {
					/*clrTextBk = item.cellBackground != null ? item.cellBackground [index] : -1;
					 if (clrTextBk == -1) clrTextBk = item.background;
					 if (clrTextBk == -1 && index == sortIndex) clrTextBk = clrSortBk;*/
					item->clrTextBk = item->attr.background;
				}
			} else {
				if (item->clrTextBk == 0 && index == item->sortIndex) {
					item->drawBackground = TRUE;
					item->clrTextBk = item->clrSortBk;
				}
			}
			if (_W_TREE(widget)->explorerTheme) {
				if (item->selected
						|| (item->nmcd->nmcd.uItemState & CDIS_HOT) != 0) {
					if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
						item->drawBackground = FALSE;
					} else {
						if (i == 0) {
							item->drawBackground = FALSE;
							//if (!hooks (SWT.EraseItem)) drawText = FALSE;
						}
					}
				}
			}
			if (item->drawItem) {
				_TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_drawitem(item);
			}
			if (_W_TREE(widget)->selectionForeground != 0)
				item->clrText = _W_TREE(widget)->selectionForeground;
			_TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT_paintitem(item);
		}
		/*x += width;
		 if (x > clientRect.right)
		 break;*/
	}
	if (_W_TREE(widget)->linesVisible) {
		if ((_W_WIDGET(widget)->style & W_FULL_SELECTION) != 0) {
			if (_W_TREE(widget)->hwndHeader != 0) {
				item->hdItem.mask = HDI_WIDTH;
				SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEMW, 0,
						(LPARAM) &item->hdItem);
				SetRect(&item->rect,
						item->nmcd->nmcd.rc.left + item->hdItem.cxy,
						item->nmcd->nmcd.rc.top, item->nmcd->nmcd.rc.right,
						item->nmcd->nmcd.rc.bottom);
				DrawEdge(hDC, &item->rect, BDR_SUNKENINNER, BF_BOTTOM);
			}
		}
		/*OS.SetRect (rect, nmcd.left, nmcd.top, nmcd.right, nmcd.bottom);*/
		DrawEdge(hDC, &item->nmcd->nmcd.rc, BDR_SUNKENINNER, BF_BOTTOM);
	}
	if (!_W_TREE(widget)->ignoreDrawFocus /*&& focusRect != null*/) {
		/*OS.DrawFocusRect (hDC, focusRect);
		 focusRect = null;*/
	} else {
		if (!_W_TREE(widget)->explorerTheme) {
			if (_W_WIDGET(widget)->handle == GetFocus()) {
				/*int uiState = (int)OS.SendMessage (handle, OS.WM_QUERYUISTATE, 0, 0);
				 if ((uiState & OS.UISF_HIDEFOCUS) == 0) {
				 long  hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_CARET, 0);
				 if (hItem == item.handle) {
				 if (!ignoreDrawFocus && findImageControl () != null) {
				 if ((style & SWT.FULL_SELECTION) != 0) {
				 RECT focusRect = new RECT ();
				 OS.SetRect (focusRect, 0, nmcd.top, clientRect.right + 1, nmcd.bottom);
				 OS.DrawFocusRect (hDC, focusRect);
				 } else {
				 int index = (int)OS.SendMessage (hwndHeader, OS.HDM_ORDERTOINDEX, 0, 0);
				 RECT focusRect = item.getBounds (index, true, false, false, false, false, hDC);
				 RECT clipRect = item.getBounds (index, true, false, false, false, true, hDC);
				 OS.IntersectClipRect (hDC, clipRect.left, clipRect.top, clipRect.right, clipRect.bottom);
				 OS.DrawFocusRect (hDC, focusRect);
				 OS.SelectClipRgn (hDC, 0);
				 }
				 }
				 }
				 }*/
			}
		}
	}
	e->result = CDRF_DODEFAULT;
	return W_TRUE;
}
int _TREE_WM_NOTIFY_CDDS_POSTPAINT(w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	if (_W_TREE(widget)->ignoreCustomDraw)
		return W_FALSE;
	NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	RECT rect;
	HDITEM hdItem;
	if (IsWindowVisible(_W_WIDGET(widget)->handle)) {
		if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			if (/*sortColumn != null && sortDirection != W_NONE*/0) {
				if (/*findImageControl () == null*/0) {
					int index = _W_TREE(widget)->sortColumn;
					if (index != -1) {
						int top = nmcd->nmcd.rc.top;
						/*
						 * Bug in Windows.  For some reason, during a collapse,
						 * when TVM_GETNEXTITEM is sent with TVGN_LASTVISIBLE
						 * and the collapse causes the item being collapsed
						 * to become the last visible item in the tree, the
						 * message takes a long time to process.  In order for
						 * the slowness to happen, the children of the item
						 * must have children.  Times of up to 11 seconds have
						 * been observed with 23 children, each having one
						 * child.  The fix is to use the bottom partially
						 * visible item rather than the last possible item
						 * that could be visible.
						 *
						 * NOTE: This problem only happens on Vista during
						 * WM_NOTIFY with NM_CUSTOMDRAW and CDDS_POSTPAINT.
						 */
						/*long hItem = 0;
						 if (!OS.IsWinCE && OS.WIN32_VERSION >= OS.VERSION (6, 0)) {
						 hItem = getBottomItem ();
						 } else {
						 hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_LASTVISIBLE, 0);
						 }
						 if (hItem != 0) {
						 RECT rect = new RECT ();
						 if (OS.TreeView_GetItemRect (handle, hItem, rect, false)) {
						 top = rect.bottom;
						 }
						 }
						 RECT rect = new RECT ();
						 OS.SetRect (rect, nmcd.left, top, nmcd.right, nmcd.bottom);
						 RECT headerRect = new RECT ();
						 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, index, headerRect);
						 rect.left = headerRect.left;
						 rect.right = headerRect.right;
						 fillBackground (nmcd.hdc, getSortColumnPixel (), rect);*/
					}
				}
			}
		}
		if (_W_TREE(widget)->linesVisible) {
			HDC hDC = nmcd->nmcd.hdc;
			if (_W_TREE(widget)->hwndHeader != 0) {
				int x = 0;
				hdItem.mask = HDI_WIDTH;
				/*
				 * get column count
				 */
				int columnCount = 1;
				if (_W_TREE(widget)->hwndHeader != 0) {
					columnCount = SendMessageW(_W_TREE(widget)->hwndHeader,
					HDM_GETITEMCOUNT, 0, 0);
				}
				for (int i = 0; i < columnCount; i++) {
					int index = (int) SendMessageW(_W_TREE(widget)->hwndHeader,
					HDM_ORDERTOINDEX, i, 0);
					SendMessageW(_W_TREE(widget)->hwndHeader, HDM_GETITEM,
							index, (LPARAM) &hdItem);
					SetRect(&rect, x, nmcd->nmcd.rc.top, x + hdItem.cxy,
							nmcd->nmcd.rc.bottom);
					DrawEdge(hDC, &rect, BDR_SUNKENINNER, BF_RIGHT);
					x += hdItem.cxy;
				}
			}
			int height = 0;
			/*
			 * Bug in Windows.  For some reason, during a collapse,
			 * when TVM_GETNEXTITEM is sent with TVGN_LASTVISIBLE
			 * and the collapse causes the item being collapsed
			 * to become the last visible item in the tree, the
			 * message takes a long time to process.  In order for
			 * the slowness to happen, the children of the item
			 * must have children.  Times of up to 11 seconds have
			 * been observed with 23 children, each having one
			 * child.  The fix is to use the bottom partially
			 * visible item rather than the last possible item
			 * that could be visible.
			 *
			 * NOTE: This problem only happens on Vista during
			 * WM_NOTIFY with NM_CUSTOMDRAW and CDDS_POSTPAINT.
			 */
			/*long hItem = 0;
			 if (!OS.IsWinCE && OS.WIN32_VERSION >= OS.VERSION (6, 0)) {
			 hItem = getBottomItem ();
			 } else {
			 hItem = OS.SendMessage (handle, OS.TVM_GETNEXTITEM, OS.TVGN_LASTVISIBLE, 0);
			 }
			 if (hItem != 0) {
			 if (OS.TreeView_GetItemRect (handle, hItem, rect, false)) {
			 height = rect.bottom - rect.top;
			 }
			 }
			 if (height == 0) {
			 height = (int)OS.SendMessage (handle, OS.TVM_GETITEMHEIGHT, 0, 0);
			 OS.GetClientRect (handle, rect);
			 OS.SetRect (rect, rect.left, rect.top, rect.right, rect.top + height);
			 OS.DrawEdge (hDC, rect, OS.BDR_SUNKENINNER, OS.BF_BOTTOM);
			 }
			 if (height != 0) {
			 while (rect.bottom < nmcd.bottom) {
			 int top = rect.top + height;
			 OS.SetRect (rect, rect.left, top, rect.right, top + height);
			 OS.DrawEdge (hDC, rect, OS.BDR_SUNKENINNER, OS.BF_BOTTOM);
			 }
			 }*/
		}
	}
	e->result = CDRF_DODEFAULT;
	return W_TRUE;
}
int _TREE_WM_NOTIFY_CUSTOMDRAW(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	NMTVCUSTOMDRAW *nmcd = (NMTVCUSTOMDRAW*) e->lparam;
	if (nmcd->nmcd.hdr.hwndFrom == _W_TREE(widget)->hwndHeader)
		return W_FALSE;
	/*if (hooks (SWT.MeasureItem)) {
	 if (hwndHeader == 0) createParent ();
	 }
	 if (!customDraw && findImageControl () == null) {
	 if (OS.COMCTL32_MAJOR >= 6 && OS.IsAppThemed ()) {
	 if (sortColumn == null || sortDirection == SWT.NONE) {
	 break;
	 }
	 }
	 }*/

	switch (nmcd->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		return _TREE_WM_NOTIFY_CDDS_PREPAINT(widget, e, reserved);
	case CDDS_ITEMPREPAINT:
		return _TREE_WM_NOTIFY_CDDS_ITEMPREPAINT(widget, e, reserved);
	case CDDS_ITEMPOSTPAINT:
		return _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT(widget, e, reserved);
	case CDDS_POSTPAINT:
		return _TREE_WM_NOTIFY_CDDS_POSTPAINT(widget, e, reserved);
	}
	return W_FALSE;
}
int _TREE_WM_NOTIFY_DBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
int _TREE_WM_NOTIFY_ITEMCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
int _TREE_WM_NOTIFY_SELCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
int _TREE_WM_NOTIFY_SELCHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	NMTREEVIEWW *treeView = (NMTREEVIEWW*) e->lparam;
	TVITEMW tvItem;
	w_event_tree event;
	w_treeitem item;
	if ((_W_WIDGET(widget)->style & W_MULTI) != 0) {
		if (_W_TREE(widget)->lockSelection) {
			/* Restore the old selection state of both items */
			if (_W_TREE(widget)->oldSelected) {
				tvItem.hItem = treeView->itemOld.hItem;
				tvItem.mask = TVIF_STATE;
				tvItem.stateMask = TVIS_SELECTED;
				tvItem.state = TVIS_SELECTED;
				SendMessageW(e->hwnd, TVM_SETITEMW, 0, (LPARAM) &tvItem);
			}
			if (!_W_TREE(widget)->newSelected
					&& _W_TREE(widget)->ignoreSelect) {
				tvItem.hItem = treeView->itemNew.hItem;
				tvItem.mask = TVIF_STATE;
				tvItem.stateMask = TVIS_SELECTED;
				tvItem.state = 0;
				SendMessageW(e->hwnd, TVM_SETITEMW, 0, (LPARAM) &tvItem);
			}
		}
	}
	if (!_W_TREE(widget)->ignoreSelect) {
		_W_TREE(widget)->hAnchor = treeView->itemNew.hItem;
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SELECTION;
		event.event.widget = widget;
		event.item = &item;
		_W_TREEITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(widget)->class_treeitem);
		_W_TREEITEM(&item)->tree = W_TREE(widget);
		_W_TREEITEM(&item)->htreeitem = (HTREEITEM) treeView->itemNew.hItem;
		_w_widget_send_event(widget, (w_event*) &event);
	}
	_w_tree_update_scrollbar(W_TREE(widget));
	return W_FALSE;
}
int _TREE_WM_NOTIFY_ITEMEXPANDED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if ((_W_WIDGET(widget)->style & W_VIRTUAL) != 0)
	 _W_WIDGET(widget)->style |= W_DOUBLE_BUFFERED;
	 if (hooks(SWT.EraseItem) || hooks(SWT.PaintItem))
	 _W_WIDGET(widget)->style |= W_DOUBLE_BUFFERED;
	 if (findImageControl() != null && getDrawing()) {
	 DefWindowProcW(_W_CONTROL(widget)->handle, WM_SETREDRAW, 1, 0);
	 InvalidateRect(_W_CONTROL(widget)->handle, NULL, TRUE);
	 }*/
	/*
	 * Bug in Windows.  When TVM_SETINSERTMARK is used to set
	 * an insert mark for a tree and an item is expanded or
	 * collapsed near the insert mark, the tree does not redraw
	 * the insert mark properly.  The fix is to hide and show
	 * the insert mark whenever an item is expanded or collapsed.
	 */
	/*if (hInsert != 0) {
	 OS.SendMessage (handle, OS.TVM_SETINSERTMARK, insertAfter ? 1 : 0, hInsert);
	 }*/
	/*
	 * Bug in Windows.  When a tree item that has an image
	 * with alpha is expanded or collapsed, the area where
	 * the image is drawn is not erased before it is drawn.
	 * This means that the image gets darker each time.
	 * The fix is to redraw the item.
	 */
	/*if (imageList != null) {
	 NMTREEVIEW treeView = new NMTREEVIEW ();
	 OS.MoveMemory (treeView, lParam, NMTREEVIEW.sizeof);
	 TVITEM tvItem = treeView.itemNew;
	 if (tvItem.hItem != 0) {
	 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.TVS_FULLROWSELECT) == 0) {
	 RECT rect = new RECT ();
	 if (OS.TreeView_GetItemRect (handle, tvItem.hItem, rect, false)) {
	 OS.InvalidateRect (handle, rect, true);
	 }
	 }
	 }
	 }*/
	_w_tree_update_scrollbar(W_TREE(widget));
	return W_FALSE;
}

int _TREE_WM_NOTIFY_ITEMEXPANDING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	NMTREEVIEW *treeView = (NMTREEVIEW*) e->lparam;
	wresult runExpanded = W_FALSE;
	w_event_tree event;
	w_treeitem item;
	TVITEMW tvItem;
	/*if (itemToolTipHandle != 0) ShowWindow (itemToolTipHandle, OS.SW_HIDE);
	 if ((style & SWT.VIRTUAL) != 0) style &= ~SWT.DOUBLE_BUFFERED;
	 if (hooks (SWT.EraseItem) || hooks (SWT.PaintItem)) style &= ~SWT.DOUBLE_BUFFERED;
	 if (findImageControl () != null && getDrawing () && OS.IsWindowVisible (handle)) {
	 OS.DefWindowProc (handle, OS.WM_SETREDRAW, 0, 0);
	 }*/
	/*
	 * Bug in Windows.  When TVM_SETINSERTMARK is used to set
	 * an insert mark for a tree and an item is expanded or
	 * collapsed near the insert mark, the tree does not redraw
	 * the insert mark properly.  The fix is to hide and show
	 * the insert mark whenever an item is expanded or collapsed.
	 */
	/*if (hInsert != 0) {
	 OS.SendMessage (handle, OS.TVM_SETINSERTMARK, 0, 0);
	 }*/
	if (!_W_TREE(widget)->ignoreExpand) {
		/*
		 * Feature in Windows.  In some cases, TVM_ITEMEXPANDING
		 * is sent from within TVM_DELETEITEM for the tree item
		 * being destroyed.  By the time the message is sent,
		 * the item has already been removed from the list of
		 * items.  The fix is to check for null.
		 */
		memset(&event, 0, sizeof(event));
		event.event.widget = widget;
		event.item = &item;
		_W_TREEITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(widget)->class_treeitem);
		_W_TREEITEM(&item)->tree = W_TREE(widget);
		_W_TREEITEM(&item)->htreeitem = (HTREEITEM) treeView->itemNew.hItem;
		switch (treeView->action) {
		case TVE_EXPAND:
			/*
			 * Bug in Windows.  When the numeric keypad asterisk
			 * key is used to expand every item in the tree, Windows
			 * sends TVN_ITEMEXPANDING to items in the tree that
			 * have already been expanded.  The fix is to detect
			 * that the item is already expanded and ignore the
			 * notification.
			 */
			if ((treeView->itemNew.state & TVIS_EXPANDED) == 0) {
				event.event.type = W_EVENT_ITEM_EXPAND;
				_w_widget_send_event(widget, (w_event*) &event);
				/**
				 * if tree item has no children remove + image
				 */
				HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(
				_W_WIDGET(widget)->handle, TVM_GETNEXTITEM,
				TVGN_CHILD, (LPARAM) treeView->itemNew.hItem);
				if (hFirstItem == 0) {
					tvItem.mask = TVIF_CHILDREN;
					tvItem.cChildren = 0;
					tvItem.hItem = treeView->itemNew.hItem;
					HRESULT result = SendMessageW(_W_WIDGET(widget)->handle,
					TVM_SETITEMW, 0, (LPARAM) &tvItem);
				}
			}
			break;
		case TVE_COLLAPSE:

			event.event.type = W_EVENT_ITEM_COLLAPSE;
			_w_widget_send_event(widget, (w_event*) &event);
			break;
		}
		/*
		 * Bug in Windows.  When all of the items are deleted during
		 * TVN_ITEMEXPANDING, Windows does not send TVN_ITEMEXPANDED.
		 * The fix is to detect this case and run the TVN_ITEMEXPANDED
		 * code in this method.
		 */
		HTREEITEM hFirstItem = (HTREEITEM) SendMessageW(e->hwnd,
		TVM_GETNEXTITEM,
		TVGN_CHILD, (LPARAM) treeView->itemNew.hItem);
		runExpanded = hFirstItem == 0;
	}
	if (runExpanded) {
		_TREE_WM_NOTIFY_ITEMEXPANDED(widget, e, reserved);
	}
	return W_FALSE;
}

int _TREE_WM_NOTIFY_BEGINDRAG(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
int _TREE_WM_NOTIFYCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	switch (((NMHDR*) e->lparam)->code) {
	case TVN_GETDISPINFOA:
	case TVN_GETDISPINFOW:
		return _TREE_WM_NOTIFY_GETDISPINFO(widget, e, reserved);
		break;
	case NM_CUSTOMDRAW:
		return _TREE_WM_NOTIFY_CUSTOMDRAW(widget, e, reserved);
		break;
	case NM_DBLCLK:
		return _TREE_WM_NOTIFY_DBLCLK(widget, e, reserved);
		break;
		/*
		 * Bug in Windows.  On Vista, when TVM_SELECTITEM is called
		 * with TVGN_CARET in order to set the selection, for some
		 * reason, Windows deselects the previous two items that
		 * were selected.  The fix is to stop the selection from
		 * changing on all but the item that is supposed to be
		 * selected.
		 */
	case TVN_ITEMCHANGINGA:
	case TVN_ITEMCHANGINGW:
		return _TREE_WM_NOTIFY_ITEMCHANGING(widget, e, reserved);
		break;
	case TVN_SELCHANGINGA:
	case TVN_SELCHANGINGW:
		return _TREE_WM_NOTIFY_SELCHANGING(widget, e, reserved);
		break;
	case TVN_SELCHANGEDA:
	case TVN_SELCHANGEDW:
		return _TREE_WM_NOTIFY_SELCHANGED(widget, e, reserved);
		break;
	case TVN_ITEMEXPANDINGA:
	case TVN_ITEMEXPANDINGW:
		return _TREE_WM_NOTIFY_ITEMEXPANDING(widget, e, reserved);
		break;
	case TVN_ITEMEXPANDEDA:
	case TVN_ITEMEXPANDEDW:
		return _TREE_WM_NOTIFY_ITEMEXPANDED(widget, e, reserved);
		break;
	case TVN_BEGINDRAGA:
	case TVN_BEGINDRAGW:
		if ((GetKeyState(VK_LBUTTON) >= 0)) {
			return _TREE_WM_NOTIFY_BEGINDRAG(widget, e, reserved);
		}
		break;
	case TVN_BEGINRDRAGA:
	case TVN_BEGINRDRAGW:
		return _TREE_WM_NOTIFY_BEGINDRAG(widget, e, reserved);
		break;
	}
	return W_FALSE;
}

int _TREE_WM_NOTIFY_HEADER(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Feature in Windows.  On NT, the automatically created
	 * header control is created as a UNICODE window, not an
	 * ANSI window despite the fact that the parent is created
	 * as an ANSI window.  This means that it sends UNICODE
	 * notification messages to the parent window on NT for
	 * no good reason.  The data and size in the NMHEADER and
	 * HDITEM structs is identical between the platforms so no
	 * different message is actually necessary.  Despite this,
	 * Windows sends different messages.  The fix is to look
	 * for both messages, despite the platform.  This works
	 * because only one will be sent on either platform, never
	 * both.
	 */
	NMHEADERW *hdr = (NMHEADERW*) e->lparam;
	switch (hdr->hdr.code) {
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
	case HDN_DIVIDERDBLCLICKW:
	case HDN_DIVIDERDBLCLICKA: {
		if (!_w_treecolumn_get_resizable_0(W_TREE(widget), hdr->iItem)) {
			e->result = 1;
			return W_TRUE;
		}
		_W_TREE(widget)->ignoreColumnMove = W_TRUE;
		switch (((NMHDR*) e->lparam)->code) {
		case HDN_DIVIDERDBLCLICKW:
		case HDN_DIVIDERDBLCLICKA:
			//if (column != null) column.pack ();
			break;
		}
		break;
	}
		/*case OS.NM_CUSTOMDRAW: {
		 NMCUSTOMDRAW nmcd = new NMCUSTOMDRAW();
		 OS.MoveMemory(nmcd, lParam, NMCUSTOMDRAW.sizeof);
		 switch (nmcd.dwDrawStage) {
		 case OS.CDDS_PREPAINT: {*/
		/* Drawing here will be deleted by further drawing steps, even with OS.CDRF_SKIPDEFAULT.
		 Changing the TextColor and returning OS.CDRF_NEWFONT has no effect. */
		/*return new LRESULT (customHeaderDrawing() ? OS.CDRF_NOTIFYITEMDRAW | OS.CDRF_NOTIFYPOSTPAINT : OS.CDRF_DODEFAULT);
		 }
		 case OS.CDDS_ITEMPREPAINT: {
		 // draw background
		 RECT rect = new RECT();
		 OS.SetRect(rect, nmcd.left, nmcd.top, nmcd.right, nmcd.bottom);
		 int pixel = getHeaderBackgroundPixel();
		 if ((nmcd.uItemState & OS.CDIS_SELECTED) != 0) {
		 pixel = getDifferentColor(pixel);
		 } else if (columns[(int) nmcd.dwItemSpec] == sortColumn && sortDirection != SWT.NONE) {
		 pixel = getSlightlyDifferentColor(pixel);
		 }
		 long brush = OS.CreateSolidBrush(pixel);
		 OS.FillRect(nmcd.hdc, rect, brush);
		 OS.DeleteObject(brush);

		 return new LRESULT(OS.CDRF_SKIPDEFAULT); // if we got here, we will paint everything ourself
		 }
		 case OS.CDDS_POSTPAINT: {
		 // get the cursor position
		 POINT cursorPos = new POINT();
		 OS.GetCursorPos(cursorPos);
		 OS.MapWindowPoints(0, hwndHeader, cursorPos, 1);

		 // drawing all cells
		 int highlightedHeaderDividerX = -1;
		 int lastColumnRight = -1;
		 RECT [] rects = new RECT [columnCount];
		 for (int i=0; i<columnCount; i++) {
		 rects [i] = new RECT ();
		 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, i, rects [i]);
		 if (rects[i].right > lastColumnRight) {
		 lastColumnRight = rects[i].right;
		 }

		 if (columns[i] == sortColumn && sortDirection != SWT.NONE) {
		 // the display.getSortImage looks terrible after scaling up.
		 long pen = OS.CreatePen (OS.PS_SOLID, 1, getHeaderForegroundPixel());
		 long  oldPen = OS.SelectObject (nmcd.hdc, pen);
		 int center = rects[i].left + (rects[i].right - rects[i].left) / 2;*/
		/*
		 * Sort indicator size needs to scale as per the Native Windows OS DPI level
		 * when header is custom drawn. For more details refer bug 537097.
		 */
		/*int leg = DPIUtil.autoScaleUpUsingNativeDPI(3);
		 if (sortDirection == SWT.UP) {
		 OS.Polyline(nmcd.hdc, new int[] {center-leg, 1+leg, center+1, 0}, 2);
		 OS.Polyline(nmcd.hdc, new int[] {center+leg, 1+leg, center-1, 0}, 2);
		 } else if (sortDirection == SWT.DOWN) {
		 OS.Polyline(nmcd.hdc, new int[] {center-leg, 1, center+1, 1+leg+1}, 2);
		 OS.Polyline(nmcd.hdc, new int[] {center+leg, 1, center-1, 1+leg+1}, 2);
		 }
		 OS.SelectObject (nmcd.hdc, oldPen);
		 OS.DeleteObject (pen);
		 }*/

		/* Windows 7 and 10 always draw a nearly invisible vertical line between the columns, even if lines are disabled.
		 This line uses no fixed color constant, but calculates it from the background color.
		 The method getSlightlyDifferentColor gives us a color, that is near enough to the windows algorithm. */
		/*long pen = OS.CreatePen (OS.PS_SOLID, getGridLineWidthInPixels(), getSlightlyDifferentColor(getHeaderBackgroundPixel()));
		 long oldPen = OS.SelectObject (nmcd.hdc, pen);
		 OS.Polyline(nmcd.hdc, new int[] {rects[i].right-1, rects[i].top, rects[i].right-1, rects[i].bottom}, 2);
		 OS.SelectObject (nmcd.hdc, oldPen);
		 OS.DeleteObject (pen);

		 pen = OS.CreatePen (OS.PS_SOLID, getGridLineWidthInPixels(), OS.GetSysColor(OS.COLOR_3DFACE));
		 oldPen = OS.SelectObject (nmcd.hdc, pen);*/
		/* To differentiate headers, always draw header column separator. */
//OS.Polyline(nmcd.hdc, new int[] {rects[i].right-1, rects[i].top, rects[i].right-1, rects[i].bottom}, 2);
		/* To differentiate header & content area, always draw the line separator between header & first row. */
		/*if (i == 0) OS.Polyline(nmcd.hdc, new int[] {nmcd.left, nmcd.bottom-1, nmcd.right, nmcd.bottom-1}, 2);
		 OS.SelectObject (nmcd.hdc, oldPen);
		 OS.DeleteObject (pen);

		 if (headerItemDragging && highlightedHeaderDividerX == -1) {
		 int distanceToLeftBorder = cursorPos.x - rects[i].left;
		 int distanceToRightBorder = rects[i].right - cursorPos.x;
		 if (distanceToLeftBorder >= 0 && distanceToRightBorder >= 0) {
		 // the cursor is in the current rectangle
		 highlightedHeaderDividerX = distanceToLeftBorder <= distanceToRightBorder ? rects[i].left-1 : rects[i].right;
		 }
		 }

		 int x = rects[i].left + INSET + 2;
		 if (columns[i].image != null) {
		 GCData data = new GCData();
		 data.device = display;
		 GC gc = GC.win32_new (nmcd.hdc, data);
		 int y = Math.max (0, (nmcd.bottom - columns[i].image.getBoundsInPixels().height) / 2);
		 gc.drawImage (columns[i].image, DPIUtil.autoScaleDown(x), DPIUtil.autoScaleDown(y));
		 x += columns[i].image.getBoundsInPixels().width + 12;
		 gc.dispose ();
		 }

		 if (columns[i].text != null) {
		 int flags = OS.DT_NOPREFIX | OS.DT_SINGLELINE | OS.DT_VCENTER;
		 if ((columns[i].style & SWT.CENTER) != 0) flags |= OS.DT_CENTER;
		 if ((columns[i].style & SWT.RIGHT) != 0) flags |= OS.DT_RIGHT;
		 TCHAR buffer = new TCHAR (getCodePage (), columns[i].text, false);
		 OS.SetBkMode(nmcd.hdc, OS.TRANSPARENT);
		 OS.SetTextColor(nmcd.hdc, getHeaderForegroundPixel());
		 RECT textRect = new RECT();
		 textRect.left = x;
		 textRect.top = rects[i].top;
		 textRect.right = rects[i].right;
		 textRect.bottom = rects[i].bottom;
		 OS.DrawText (nmcd.hdc, buffer, buffer.length (), textRect, flags);
		 }
		 }

		 if (lastColumnRight < nmcd.right) {
		 // draw background of the 'no column' area
		 RECT rect = new RECT();
		 OS.SetRect(rect, lastColumnRight, nmcd.top, nmcd.right, nmcd.bottom-1);
		 long brush = OS.CreateSolidBrush(getHeaderBackgroundPixel());
		 OS.FillRect(nmcd.hdc, rect, brush);
		 OS.DeleteObject(brush);
		 }

		 // always draw the highlighted border at the end, to avoid overdrawing by other borders.
		 if (highlightedHeaderDividerX != -1) {
		 long pen = OS.CreatePen (OS.PS_SOLID, 4, OS.GetSysColor(OS.COLOR_HIGHLIGHT));
		 long oldPen = OS.SelectObject (nmcd.hdc, pen);
		 OS.Polyline(nmcd.hdc, new int[] {highlightedHeaderDividerX, nmcd.top, highlightedHeaderDividerX, nmcd.bottom}, 2);
		 OS.SelectObject (nmcd.hdc, oldPen);
		 OS.DeleteObject (pen);
		 }

		 return new LRESULT(OS.CDRF_DODEFAULT);
		 }
		 }
		 break;
		 }
		 case OS.NM_RELEASEDCAPTURE: {
		 if (!ignoreColumnMove) {
		 for (int i=0; i<columnCount; i++) {
		 TreeColumn column = columns [i];
		 column.updateToolTip (i);
		 }
		 updateImageList ();
		 }
		 ignoreColumnMove = false;
		 break;
		 }
		 case OS.HDN_BEGINDRAG: {
		 if (ignoreColumnMove) return LRESULT.ONE;
		 NMHEADER phdn = new NMHEADER ();
		 OS.MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 if (phdn.iItem != -1) {
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null && !column.getMoveable ()) {
		 ignoreColumnMove = true;
		 return LRESULT.ONE;
		 }
		 headerItemDragging = true;
		 }
		 break;
		 }
		 case OS.HDN_ENDDRAG: {
		 headerItemDragging = false;
		 NMHEADER phdn = new NMHEADER ();
		 OS.MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 if (phdn.iItem != -1 && phdn.pitem != 0) {
		 HDITEM pitem = new HDITEM ();
		 OS.MoveMemory (pitem, phdn.pitem, HDITEM.sizeof);
		 if ((pitem.mask & OS.HDI_ORDER) != 0 && pitem.iOrder != -1) {
		 int [] order = new int [columnCount];
		 OS.SendMessage (hwndHeader, OS.HDM_GETORDERARRAY, columnCount, order);
		 int index = 0;
		 while (index < order.length) {
		 if (order [index] == phdn.iItem) break;
		 index++;
		 }
		 if (index == order.length) index = 0;
		 if (index == pitem.iOrder) break;
		 int start = Math.min (index, pitem.iOrder);
		 int end = Math.max (index, pitem.iOrder);
		 RECT rect = new RECT (), headerRect = new RECT ();
		 OS.GetClientRect (handle, rect);
		 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, order [start], headerRect);
		 rect.left = Math.max (rect.left, headerRect.left);
		 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, order [end], headerRect);
		 rect.right = Math.min (rect.right, headerRect.right);
		 OS.InvalidateRect (handle, rect, true);
		 ignoreColumnMove = false;
		 for (int i=start; i<=end; i++) {
		 TreeColumn column = columns [order [i]];
		 if (!column.isDisposed ()) {
		 column.postEvent (SWT.Move);
		 }
		 }
		 }
		 }
		 break;
		 }
		 case OS.HDN_ITEMCHANGINGW:
		 case OS.HDN_ITEMCHANGINGA: {
		 NMHEADER phdn = new NMHEADER ();
		 OS.MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 if (phdn.pitem != 0) {
		 HDITEM newItem = new HDITEM ();
		 OS.MoveMemory (newItem, phdn.pitem, HDITEM.sizeof);
		 if ((newItem.mask & OS.HDI_WIDTH) != 0) {
		 RECT rect = new RECT ();
		 OS.GetClientRect (handle, rect);
		 HDITEM oldItem = new HDITEM ();
		 oldItem.mask = OS.HDI_WIDTH;
		 OS.SendMessage (hwndHeader, OS.HDM_GETITEM, phdn.iItem, oldItem);
		 int deltaX = newItem.cxy - oldItem.cxy;
		 RECT headerRect = new RECT ();
		 OS.SendMessage (hwndHeader, OS.HDM_GETITEMRECT, phdn.iItem, headerRect);
		 int gridWidth = linesVisible ? GRID_WIDTH : 0;
		 rect.left = headerRect.right - gridWidth;
		 int newX = rect.left + deltaX;
		 rect.right = Math.max (rect.right, rect.left + Math.abs (deltaX));
		 if (explorerTheme || (findImageControl () != null || hooks (SWT.MeasureItem) || hooks (SWT.EraseItem) || hooks (SWT.PaintItem))) {
		 rect.left -= OS.GetSystemMetrics (OS.SM_CXFOCUSBORDER);
		 OS.InvalidateRect (handle, rect, true);
		 OS.OffsetRect (rect, deltaX, 0);
		 OS.InvalidateRect (handle, rect, true);
		 } else {
		 int flags = OS.SW_INVALIDATE | OS.SW_ERASE;
		 OS.ScrollWindowEx (handle, deltaX, 0, rect, null, 0, null, flags);
		 }
		 if (OS.SendMessage (hwndHeader, OS.HDM_ORDERTOINDEX, phdn.iItem, 0) != 0) {
		 rect.left = headerRect.left;
		 rect.right = newX;
		 OS.InvalidateRect (handle, rect, true);
		 }
		 setScrollWidth ();
		 }
		 }
		 break;
		 }
		 case OS.HDN_ITEMCHANGEDW:
		 case OS.HDN_ITEMCHANGEDA: {
		 NMHEADER phdn = new NMHEADER ();
		 OS.MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 if (phdn.pitem != 0) {
		 HDITEM pitem = new HDITEM ();
		 OS.MoveMemory (pitem, phdn.pitem, HDITEM.sizeof);
		 if ((pitem.mask & OS.HDI_WIDTH) != 0) {
		 if (ignoreColumnMove) {
		 int flags = OS.RDW_UPDATENOW | OS.RDW_ALLCHILDREN;
		 OS.RedrawWindow (handle, null, 0, flags);
		 }
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null) {
		 column.updateToolTip (phdn.iItem);
		 column.sendEvent (SWT.Resize);
		 if (isDisposed ()) return LRESULT.ZERO;
		 TreeColumn [] newColumns = new TreeColumn [columnCount];
		 System.arraycopy (columns, 0, newColumns, 0, columnCount);
		 int [] order = new int [columnCount];
		 OS.SendMessage (hwndHeader, OS.HDM_GETORDERARRAY, columnCount, order);
		 boolean moved = false;
		 for (int i=0; i<columnCount; i++) {
		 TreeColumn nextColumn = newColumns [order [i]];
		 if (moved && !nextColumn.isDisposed ()) {
		 nextColumn.updateToolTip (order [i]);
		 nextColumn.sendEvent (SWT.Move);
		 }
		 if (nextColumn == column) moved = true;
		 }
		 }
		 }
		 setScrollWidth ();
		 }
		 break;
		 }
		 case OS.HDN_ITEMCLICKW:
		 case OS.HDN_ITEMCLICKA: {
		 NMHEADER phdn = new NMHEADER ();
		 OS.MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null) {
		 column.sendSelectionEvent (SWT.Selection);
		 }
		 break;
		 }
		 case OS.HDN_ITEMDBLCLICKW:
		 case OS.HDN_ITEMDBLCLICKA: {
		 NMHEADER phdn = new NMHEADER ();
		 OS.MoveMemory (phdn, lParam, NMHEADER.sizeof);
		 TreeColumn column = columns [phdn.iItem];
		 if (column != null) {
		 column.sendSelectionEvent (SWT.DefaultSelection);
		 }
		 break;
		 }*/
	}
	return W_FALSE;
}

int _TREE_WM_NOTIFY_TOOLTIP0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*switch (hdr.code) {
	 case OS.NM_CUSTOMDRAW: {
	 NMTTCUSTOMDRAW nmcd = new NMTTCUSTOMDRAW ();
	 OS.MoveMemory (nmcd, lParam, NMTTCUSTOMDRAW.sizeof);
	 return wmNotifyToolTip (nmcd, lParam);
	 }
	 case OS.TTN_SHOW: {
	 LRESULT result = super.wmNotify (hdr, wParam, lParam);
	 if (result != null) return result;
	 int pos = OS.GetMessagePos ();
	 POINT pt = new POINT();
	 OS.POINTSTOPOINT (pt, pos);
	 OS.ScreenToClient (handle, pt);
	 int [] index = new int [1];
	 TreeItem [] item = new TreeItem [1];
	 RECT [] cellRect = new RECT [1], itemRect = new RECT [1];
	 if (findCell (pt.x, pt.y, item, index, cellRect, itemRect)) {
	 RECT toolRect = toolTipRect (itemRect [0]);
	 OS.MapWindowPoints (handle, 0, toolRect, 2);
	 int width = toolRect.right - toolRect.left;
	 int height = toolRect.bottom - toolRect.top;
	 int flags = OS.SWP_NOACTIVATE | OS.SWP_NOZORDER | OS.SWP_NOSIZE;
	 if (isCustomToolTip ()) flags &= ~OS.SWP_NOSIZE;
	 OS.SetWindowPos (itemToolTipHandle, 0, toolRect.left, toolRect.top, width, height, flags);
	 return LRESULT.ONE;
	 }
	 return result;
	 }
	 }
	 return null;*/
	return W_FALSE;
}

int _TREE_WM_NOTIFY_TOOLTIP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*switch (nmcd.dwDrawStage) {
	 case OS.CDDS_PREPAINT: {
	 if (isCustomToolTip ()) {
	 //TEMPORARY CODE
	 //nmcd.uDrawFlags |= OS.DT_CALCRECT;
	 //OS.MoveMemory (lParam, nmcd, NMTTCUSTOMDRAW.sizeof);
	 return new LRESULT (OS.CDRF_NOTIFYPOSTPAINT | OS.CDRF_NEWFONT);
	 }
	 break;
	 }
	 case OS.CDDS_POSTPAINT: {
	 if (OS.SendMessage (itemToolTipHandle, OS.TTM_GETCURRENTTOOL, 0, 0) != 0) {
	 TOOLINFO lpti = new TOOLINFO ();
	 lpti.cbSize = TOOLINFO.sizeof;
	 if (OS.SendMessage (itemToolTipHandle, OS.TTM_GETCURRENTTOOL, 0, lpti) != 0) {
	 int [] index = new int [1];
	 TreeItem [] item = new TreeItem [1];
	 RECT [] cellRect = new RECT [1], itemRect = new RECT [1];
	 int pos = OS.GetMessagePos ();
	 POINT pt = new POINT();
	 OS.POINTSTOPOINT (pt, pos);
	 OS.ScreenToClient (handle, pt);
	 if (findCell (pt.x, pt.y, item, index, cellRect, itemRect)) {
	 long hDC = OS.GetDC (handle);
	 long hFont = item [0].fontHandle (index [0]);
	 if (hFont == -1) hFont = OS.SendMessage (handle, OS.WM_GETFONT, 0, 0);
	 long oldFont = OS.SelectObject (hDC, hFont);
	 boolean drawForeground = true;
	 cellRect [0] = item [0].getBounds (index [0], true, true, false, false, false, hDC);
	 if (hooks (SWT.EraseItem)) {
	 Event event = sendEraseItemEvent (item [0], nmcd, index [0], cellRect [0]);
	 if (isDisposed () || item [0].isDisposed ()) break;
	 if (event.doit) {
	 drawForeground = (event.detail & SWT.FOREGROUND) != 0;
	 } else {
	 drawForeground = false;
	 }
	 }
	 if (drawForeground) {
	 int nSavedDC = OS.SaveDC (nmcd.hdc);
	 int gridWidth = getLinesVisible () ? Table.GRID_WIDTH : 0;
	 RECT insetRect = toolTipInset (cellRect [0]);
	 OS.SetWindowOrgEx (nmcd.hdc, insetRect.left, insetRect.top, null);
	 GCData data = new GCData ();
	 data.device = display;
	 data.foreground = OS.GetTextColor (nmcd.hdc);
	 data.background = OS.GetBkColor (nmcd.hdc);
	 data.font = Font.win32_new (display, hFont);
	 GC gc = GC.win32_new (nmcd.hdc, data);
	 int x = cellRect [0].left + INSET;
	 if (index [0] != 0) x -= gridWidth;
	 Image image = item [0].getImage (index [0]);
	 if (image != null || index [0] == 0) {
	 Point size = getImageSize ();
	 RECT imageRect = item [0].getBounds (index [0], false, true, false, false, false, hDC);
	 if (imageList == null) size.x = imageRect.right - imageRect.left;
	 if (image != null) {
	 Rectangle rect = image.getBounds (); // Points
	 gc.drawImage (image, rect.x, rect.y, rect.width, rect.height, DPIUtil.autoScaleDown(x), DPIUtil.autoScaleDown(imageRect.top), DPIUtil.autoScaleDown(size.x), DPIUtil.autoScaleDown(size.y));
	 x += INSET + (index [0] == 0 ? 1 : 0);
	 }
	 x += size.x;
	 } else {
	 x += INSET;
	 }
	 String string = item [0].getText (index [0]);
	 if (string != null) {
	 int flags = OS.DT_NOPREFIX | OS.DT_SINGLELINE | OS.DT_VCENTER;
	 TreeColumn column = columns != null ? columns [index [0]] : null;
	 if (column != null) {
	 if ((column.style & SWT.CENTER) != 0) flags |= OS.DT_CENTER;
	 if ((column.style & SWT.RIGHT) != 0) flags |= OS.DT_RIGHT;
	 }
	 TCHAR buffer = new TCHAR (getCodePage (), string, false);
	 RECT textRect = new RECT ();
	 OS.SetRect (textRect, x, cellRect [0].top, cellRect [0].right, cellRect [0].bottom);
	 OS.DrawText (nmcd.hdc, buffer, buffer.length (), textRect, flags);
	 }
	 gc.dispose ();
	 OS.RestoreDC (nmcd.hdc, nSavedDC);
	 }
	 if (hooks (SWT.PaintItem)) {
	 itemRect [0] = item [0].getBounds (index [0], true, true, false, false, false, hDC);
	 sendPaintItemEvent (item [0], nmcd, index[0], itemRect [0]);
	 }
	 OS.SelectObject (hDC, oldFont);
	 OS.ReleaseDC (handle, hDC);
	 }
	 break;
	 }
	 }
	 break;
	 }
	 }
	 return null;*/
	return W_FALSE;
}
int _w_tree_post_event(w_widget *widget, struct w_event *ee) {
	if (ee->type == W_EVENT_PLATFORM) {
		struct _w_event_platform *e = (struct _w_event_platform*) ee;
		if (e->hwnd == _W_TREE(widget)->hwndParent) {
			switch (e->msg) {
			case WM_SIZE: {
				_w_tree_set_scroll_width(W_TREE(widget));
				_w_tree_update_scrollbar(W_TREE(widget));
				//int total = _w_tree_get_header_width(W_TREE(widget));
				//_w_tree_update_size(W_TREE(widget), total);
				e->result = TRUE;
			}
				break;
			case WM_HSCROLL: {
				RECT rc;
				if (GetClientRect(_W_TREE(widget)->hwndParent, &rc) != TRUE)
					return FALSE;
				int cx = rc.right - rc.left;
				int pos = GetScrollPos(_W_TREE(widget)->hwndParent, SB_HORZ);
				int total = _w_tree_get_header_width(W_TREE(widget));
				switch (LOWORD(e->wparam)) {
				case SB_LINELEFT:
					pos -= GetSystemMetrics(SM_CXVSCROLL);
					break;
				case SB_LINERIGHT:
					pos += GetSystemMetrics(SM_CXVSCROLL);
					break;
				case SB_PAGELEFT:
					pos -= cx;
					break;
				case SB_PAGERIGHT:
					pos += cx;
					break;
				case SB_LEFT:
					pos = 0;
					break;
				case SB_RIGHT:

					pos = total - cx;
					break;
				case SB_THUMBTRACK:
					pos = HIWORD(e->wparam);
					break;
				}
				if (pos < 0)
					pos = 0;
				else if (pos > total - cx)
					pos = total - cx;

				SetScrollPos(_W_TREE(widget)->hwndParent, SB_HORZ, pos, FALSE);
				//_w_tree_update_size(W_TREE(widget),total);
				_w_tree_set_scroll_width(W_TREE(widget));
				e->result = TRUE;
				return W_TRUE;
			}
				break;
			case WM_VSCROLL: {
				SCROLLINFO info;
				info.cbSize = sizeof(SCROLLINFO);
				info.fMask = SIF_ALL;
				GetScrollInfo(_W_TREE(widget)->hwndParent, SB_VERT, &info);
				/*
				 * Update the nPos field to match the nTrackPos field
				 * so that the tree scrolls when the scroll bar of the
				 * parent is dragged.
				 *
				 * NOTE: For some reason, this code is only necessary
				 * on Windows Vista.
				 */
				if (/*!IsWinCE &&*/WIN32_VERSION >= VERSION(6, 0)) {
					if (LOWORD (e->wparam) == SB_THUMBTRACK) {
						info.nPos = info.nTrackPos;
					}
				}
				SetScrollInfo(_W_WIDGET(widget)->handle, SB_VERT, &info, TRUE);
				e->result = SendMessageW(_W_WIDGET(widget)->handle, WM_VSCROLL,
						e->wparam, e->lparam);
				GetScrollInfo(_W_WIDGET(widget)->handle, SB_VERT, &info);
				SetScrollInfo(_W_TREE(widget)->hwndParent, SB_VERT, &info,
				TRUE);
				return W_TRUE;
			}
				break;
			case WM_NOTIFY: {
				NMHDR *hdr = (NMHDR*) e->lparam;
				if (hdr != 0) {
					if (hdr->hwndFrom == _W_TREE(widget)->hwndHeader) {
						_TREE_WM_NOTIFY_HEADER0(widget, e, (NMHEADERW*) hdr);
					} else {
						e->result = SendMessageW(hdr->hwndFrom, WM_NOTIFY_CHILD,
								e->wparam, e->lparam);
					}
				}
			}
				break;
			case WM_DESTROY: {
				SetWindowLongPtrW(e->hwnd, GWLP_USERDATA, (LONG_PTR) 0);
				e->result = 0;
				return W_TRUE;
			}
			default:
				e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam,
						e->lparam);
				return W_TRUE;
				break;
			}
		}
//e->result = DefWindowProcW(e->hwnd, e->msg, e->wparam, e->lparam);
//return W_TRUE;
	}
	return _w_control_post_event(widget, ee);
}
int _TREE_WM_NOTIFY(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (((NMHDR*) e->lparam)->hwndFrom == itemToolTipHandle) {
	 LRESULT result = wmNotifyToolTip (hdr, wParam, lParam);
	 if (result != null) return result;
	 }*/
	if (((NMHDR*) e->lparam)->hwndFrom == _W_TREE(widget)->hwndHeader) {
		return _TREE_WM_NOTIFY_HEADER(widget, e, reserved);
	}
	return _COMPOSITE_WM_NOTIFY(widget, e, reserved);
}

#endif
