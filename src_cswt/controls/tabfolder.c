//
// Created by azeddine on 27/08/2020.
//

#include "tabfolder.h"
#define SPACING 3
#define SELECTION_FOREGROUND W_COLOR_LIST_FOREGROUND
#define SELECTION_BACKGROUND W_COLOR_LIST_BACKGROUND

#define FOREGROUND W_COLOR_WIDGET_FOREGROUND
#define BACKGROUND W_COLOR_WIDGET_BACKGROUND
extern struct wc_tabfolder_renderer default_renderer;
extern struct wc_tabfolder_renderer custom_renderer;
/*
 *
 */
wc_tabitem* wc_tabfolder_priv_get_item(wc_tabfolder_priv *tab, wuint index) {
	wc_tabitem *item = tab->items;
	for (wuint i = 0; i < index; i++) {
		item = item->next;
	}
	return item;
}
wc_tabitem* wc_tabfolder_priv_insert_item(wc_tabfolder_priv *tab, wuint index) {
	wc_tabitem *item = malloc(sizeof(wc_tabitem));
	if (item == 0)
		return 0;
	memset(item, 0, sizeof(wc_tabitem));
	item->shortenedTextWidth = -1;
	item->image = -1;
	if (index >= tab->items_count) {
		if (tab->items == 0) {
			tab->items = item;
			item->prev = item;
			tab->items_count = 1;
		} else {
			wc_tabitem *last = tab->items->prev;
			last->next = item;
			item->prev = last;
			tab->items->prev = item;
			tab->items_count++;
		}
	} else {
		if (index == 0) {
			item->next = tab->items;
			item->prev = tab->items->prev; // last
			tab->items->prev = item;
			tab->items = item;
		} else {
			wc_tabitem *i = wc_tabfolder_priv_get_item(tab, index);
			item->next = i;
			item->prev = i->prev;
			i->prev->next = item;
			i->prev = item;
		}
	}
	return item;
}
int wc_tabfolder_check_style(w_composite *parent, int style) {
	int mask = W_CLOSE | W_TOP | W_BOTTOM | W_FLAT | W_LEFT_TO_RIGHT
			| W_RIGHT_TO_LEFT | W_SINGLE | W_MULTI;
	style = style & mask;
	// TOP and BOTTOM are mutually exclusive.
	// TOP is the default
	if ((style & W_TOP) != 0)
		style = style & ~W_BOTTOM;
	// SINGLE and MULTI are mutually exclusive.
	// MULTI is the default
	if ((style & W_MULTI) != 0)
		style = style & ~W_SINGLE;
	// reduce the flash by not redrawing the entire area on a Resize event
	style |= W_NO_REDRAW_RESIZE;

	//TEMPORARY CODE
	/*
	 * In Right To Left orientation on Windows, all GC calls that use a brush are drawing
	 * offset by one pixel.  This results in some parts of the CTabFolder not drawing correctly.
	 * To alleviate some of the appearance problems, allow the OS to draw the background.
	 * This does not draw correctly but the result is less obviously wrong.
	 */
	if ((style & W_RIGHT_TO_LEFT) != 0)
		return style;
	if ((w_widget_get_style(W_WIDGET(parent)) & W_MIRRORED) != 0
			&& (style & W_LEFT_TO_RIGHT) == 0)
		return style;

	return style | W_DOUBLE_BUFFERED;
}
wresult wc_tabfolder_update_tab_height(w_tabfolder *tabfolder, w_graphics *gc,
		int force) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	int oldHeight = priv->tabHeight;
	w_size sz;
	priv->renderer->compute_size(tabfolder, 0, WC_TABFOLDER_PART_HEADER, W_NONE,
			gc, &sz, W_DEFAULT, W_DEFAULT);
	priv->tabHeight = sz.height;
	/*if (fixedTabHeight == SWT.DEFAULT && controls != null && controls.length > 0) {
	 for (int i = 0; i < controls.length; i++) {
	 if ((controlAlignments[i] & SWT.WRAP) == 0 && !controls[i].isDisposed() && controls[i].getVisible()) {
	 int topHeight = controls[i].computeSize(SWT.DEFAULT, SWT.DEFAULT).y;
	 topHeight +=  renderer.computeTrim(CTabFolderRenderer.PART_HEADER, SWT.NONE, 0,0,0,0).height + 1;
	 tabHeight = Math.max(topHeight, tabHeight);
	 }
	 }
	 }*/
	if (!force && priv->tabHeight == oldHeight)
		return W_FALSE;
	memset(&priv->oldSize, 0, sizeof(w_size));
	return W_TRUE;
}
int wc_tabfolder_get_wrapped_height(w_tabfolder *tabfolder,
		const w_rect *size) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	/*boolean[][] positions = new boolean[1][];
	 Rectangle[] rects = computeControlBounds(size, positions);*/
	int minY = 0x7FFFFFFF, maxY = 0, wrapHeight = 0;
	/*for (int i = 0; i < rects.length; i++) {
	 if (positions[0][i]) {
	 minY = Math.min(minY, rects[i].y);
	 maxY = Math.max(maxY, rects[i].y + rects[i].height);
	 wrapHeight = maxY - minY;
	 }
	 }*/
	return wrapHeight;
}
int wc_tabfolder_get_right_item_edge(w_tabfolder *tabfolder, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	w_rect bounds, trim, tmp;
	w_control_get_bounds(W_CONTROL(tabfolder), &bounds.pt,&bounds.sz);
	memset(&tmp, 0, sizeof(tmp));
	priv->renderer->compute_trim(tabfolder, 0, WC_TABFOLDER_PART_BORDER, W_NONE,
			&trim, &tmp);
	int x = bounds.width - (trim.width + trim.x);
	int width = 0;
	/*for (int i = 0; i < controls.length; i++) {
	 int align = controlAlignments[i];
	 if ((align & SWT.WRAP) == 0 && (align & SWT.LEAD) == 0 && !controls[i].isDisposed() && controls[i].getVisible()) {
	 Point rightSize = controls[i].computeSize(SWT.DEFAULT, SWT.DEFAULT);
	 width += rightSize.x;
	 }
	 }*/
	if (width != 0)
		width += SPACING * 2;
	x -= width;
	return w_max(0, x);
}

int wc_tabfolder_get_left_item_edge(w_tabfolder *tabfolder, w_graphics *gc,
		int part, wc_tabitem *item) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	w_rect trim, tmp;
	memset(&tmp, 0, sizeof(tmp));
	priv->renderer->compute_trim(tabfolder, item, part, W_NONE, &trim, &tmp);
	int x = -trim.x;
	int width = 0;
	/*for (int i = 0; i < controls.length; i++) {
	 if ((controlAlignments[i] & SWT.LEAD) != 0 && !controls[i].isDisposed() && controls[i].getVisible()) {
	 width += controls[i].computeSize(SWT.DEFAULT, SWT.DEFAULT).x;
	 }
	 }*/
	if (width != 0)
		width += SPACING * 2;
	x += width;
	return w_max(0, x);
}

wresult wc_tabfolder_set_item_size(w_tabfolder *tabfolder, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	wc_tabitem *tab;
	int changed = W_FALSE;
	//if (IsDisposed())
	//	return changed;
	w_rect bounds;
	w_size result;
	w_control_get_bounds(W_CONTROL(tabfolder), &bounds.pt,&bounds.sz);
	if (bounds.width <= 0 || bounds.height <= 0)
		return changed;
	/*ToolBar chevron = getChevron();
	 if (chevron != null) chevron.setVisible(false);
	 showChevron = false;*/
	if (priv->single) {
		/*showChevron = chevronVisible && items.length > 1;
		 if (showChevron) {
		 chevron.setVisible(true);
		 }*/
		if (priv->selectedItem != 0) {
			/*CTabItem* tab = this->data.selectedItem;
			 int width = renderer.computeSize(selectedIndex, SWT.SELECTED, gc, SWT.DEFAULT, SWT.DEFAULT).x;
			 width = Math.min(width, getRightItemEdge(gc) - getLeftItemEdge(gc, CTabFolderRenderer.PART_BORDER));
			 if (tab.height != tabHeight || tab.width != width) {
			 changed = true;
			 tab.shortenedText = null;
			 tab.shortenedTextWidth = 0;
			 tab.height = tabHeight;
			 tab.width = width;
			 tab.closeRect.width = tab.closeRect.height = 0;
			 if (showClose || tab.showClose) {
			 Point closeSize = renderer.computeSize(CTabFolderRenderer.PART_CLOSE_BUTTON, SWT.SELECTED, gc, SWT.DEFAULT, SWT.DEFAULT);
			 tab.closeRect.width = closeSize.x;
			 tab.closeRect.height = closeSize.y;
			 }
			 }*/
		}
		return changed;
	}

	if (priv->items == 0)
		return changed;
	int tabAreaWidth = w_max(0,
			wc_tabfolder_get_right_item_edge(tabfolder, gc)
					- wc_tabfolder_get_left_item_edge(tabfolder, gc,
							WC_TABFOLDER_PART_BORDER, 0));
	// First, try the minimum tab size at full compression.
	int minWidth = 0;
	tab = priv->items;
	int i = 0;
	while (tab != 0) {
		//int index = priority[i];
		int index = i;
		int state = WC_TABFOLDER_MINIMUM_SIZE;
		if (tab == priv->selectedItem)
			state |= W_SELECTED;
		priv->renderer->compute_size(tabfolder, tab, index, state, gc, &result,
				W_DEFAULT, W_DEFAULT);
		tab->bounds.width = result.width; //minWidths= result.width
		minWidth += tab->bounds.width;
		if (minWidth > tabAreaWidth)
			break;
		tab = tab->next;
		i++;
	}
	if (minWidth > tabAreaWidth) {
		// full compression required and a chevron
		/*showChevron = chevronVisible && items.length > 1;
		 if (showChevron) {
		 tabAreaWidth -= chevron.computeSize(SWT.DEFAULT, SWT.DEFAULT).x;
		 chevron.setVisible(true);
		 }*/
		/*widths = minWidths;
		 int index = selectedIndex != -1 ? selectedIndex : 0;
		 if (tabAreaWidth < widths[index]) {
		 widths[index] = Math.max(0, tabAreaWidth);
		 }*/
		tab = priv->selectedItem != 0 ? priv->selectedItem : priv->items;
		if (tabAreaWidth < tab->bounds.width) {
			tab->bounds.width = w_max(0, tabAreaWidth);
		}
	} else {
		int maxWidth = 0;
		tab = priv->items;
		int i = 0;
		while (tab != 0) {
			int state = 0;
			if (tab == priv->selectedItem)
				state |= W_SELECTED;
			priv->renderer->compute_size(tabfolder, tab, i, state, gc, &result,
					W_DEFAULT, W_DEFAULT);
			tab->bounds.height = result.width; //maxWidths= result.width
			maxWidth += tab->bounds.height;
			tab = tab->next;
			i++;
		}
		if (maxWidth <= tabAreaWidth) {
			// no compression required
			tab = priv->items;
			while (tab != 0) {
				tab->bounds.width = tab->bounds.height;
				tab = tab->next;
			}
		} else {
			// determine compression for each item
			int extra = (tabAreaWidth - minWidth) / priv->items_count;
			while (W_TRUE) {
				int large = 0, totalWidth = 0;
				tab = priv->items;
				while (tab != 0) {
					if (tab->bounds.height > tab->bounds.width + extra) {
						totalWidth += tab->bounds.width + extra;
						large++;
					} else {
						totalWidth += tab->bounds.height;
					}
					tab = tab->next;
				}
				if (totalWidth >= tabAreaWidth) {
					extra--;
					break;
				}
				if (large == 0 || tabAreaWidth - totalWidth < large)
					break;
				extra++;
			}
			tab = priv->items;
			while (tab != 0) {
				tab->bounds.width = w_min(tab->bounds.height,
						tab->bounds.width + extra);
				tab = tab->next;
			}
		}
	}
	tab = priv->items;
	i = 0;
	int style = w_widget_get_style(W_WIDGET(tabfolder));
	while (tab != 0) {
		if (tab->bounds.height != priv->tabHeight
				|| tab->bounds.width != tab->bounds.width) {
			changed = W_TRUE;
			//tab->shortenedText = null;
			//tab->shortenedTextWidth = 0;
			tab->bounds.height = priv->tabHeight;
			//tab->closeRect.width = tab.closeRect.height = 0;
			if ((style & W_CLOSE) || tab->showClose) {
				/*if (i == selectedIndex || showUnselectedClose) {
				 Point closeSize = renderer.computeSize(CTabFolderRenderer.PART_CLOSE_BUTTON, SWT.NONE, gc, SWT.DEFAULT, SWT.DEFAULT);
				 tab.closeRect.width = closeSize.x;
				 tab.closeRect.height = closeSize.y;
				 }*/
			}
		}
		i++;
		tab = tab->next;
	}
	return changed;
}
void wc_tabitem_get_close_rect(w_tabfolder *tabfolder, wc_tabitem *item,
		w_graphics *gc, w_rect *closeRect) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	wuint64 style = w_widget_get_style(W_WIDGET(tabfolder));
	if ((style & W_CLOSE) || item->showClose) {
		w_size closeSize;
		w_rect trim, r;
		memset(&r, 0, sizeof(r));
		priv->renderer->compute_size(tabfolder, 0,
				WC_TABFOLDER_PART_CLOSE_BUTTON, W_SELECTED, gc, &closeSize,
				W_DEFAULT, W_DEFAULT);
		closeRect->width = closeSize.width;
		closeRect->height = closeSize.height;
		if (priv->single) {
			priv->renderer->compute_trim(tabfolder, item, 0, W_NONE, &trim, &r);
			int leftItemEdge = wc_tabfolder_get_left_item_edge(tabfolder, gc,
					WC_TABFOLDER_PART_BORDER, 0);
			closeRect->x = leftItemEdge - trim.x;
			priv->renderer->compute_trim(tabfolder, 0, WC_TABFOLDER_PART_BORDER,
					W_NONE, &trim, &r);
			int borderBottom = trim.height + trim.y;
			int borderTop = -trim.y;
			if (style & W_BOTTOM) {
				w_control_get_bounds(W_CONTROL(tabfolder), &trim.pt,&trim.sz);
				closeRect->y = trim.height - borderBottom - priv->tabHeight
						+ (priv->tabHeight - closeSize.height) / 2;
			} else {
				closeRect->y = borderTop
						+ (priv->tabHeight - closeSize.height) / 2;
			}
		} else {
			int state = W_NONE;
			if (item == priv->selectedItem)
				state |= W_SELECTED;
			priv->renderer->compute_trim(tabfolder, item, 0, state, &trim, &r);
			closeRect->x = item->bounds.x + item->bounds.width
					- (trim.width + trim.x) - closeSize.width;
			priv->renderer->compute_trim(tabfolder, 0, WC_TABFOLDER_PART_BORDER,
					W_NONE, &trim, &r);
			int borderBottom = trim.height + trim.y;
			int borderTop = -trim.y;
			if (style & W_BOTTOM) {
				w_control_get_bounds(W_CONTROL(tabfolder), &trim.pt,&trim.sz);
				closeRect->y = trim.height - borderBottom - priv->tabHeight
						+ (priv->tabHeight - closeSize.height) / 2;
			} else {
				closeRect->y = borderTop
						+ (priv->tabHeight - closeSize.height) / 2;
			}
		}
	} else {
		closeRect->x = closeRect->y = 0;
		closeRect->width = closeRect->height = 0;
	}
}
wresult wc_tabfolder_set_item_location(w_tabfolder *tabfolder, w_graphics *gc) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	wc_tabitem *item;
	w_rect trim, bounds, screen, tmp;
	w_size closeButtonSize;
	int i;
	int changed = W_FALSE;
	if (priv->items == 0)
		return W_FALSE;
	w_toolkit_get_bounds(w_widget_get_toolkit(W_WIDGET(tabfolder)), &screen);
	memset(&tmp, 0, sizeof(tmp));
	priv->renderer->compute_trim(tabfolder, 0, WC_TABFOLDER_PART_BORDER, W_NONE,
			&trim, &tmp);
	int borderBottom = trim.height + trim.y;
	int borderTop = -trim.y;
	w_control_get_bounds(W_CONTROL(tabfolder), &bounds.pt,&bounds.sz);
	int style = w_widget_get_style(W_WIDGET(tabfolder));
	int y = (style & W_BOTTOM) ?
			w_max(borderBottom,
					bounds.height - borderBottom - priv->tabHeight) :
			borderTop;
	priv->renderer->compute_size(tabfolder, 0, WC_TABFOLDER_PART_CLOSE_BUTTON,
			0, gc, &closeButtonSize, W_DEFAULT, W_DEFAULT);
	int leftItemEdge = wc_tabfolder_get_left_item_edge(tabfolder, gc,
			WC_TABFOLDER_PART_BORDER, 0);
	if (priv->single) {
		int defaultX = screen.width + 10; // off screen
		wc_tabitem *item = priv->items;
		int i = 0;
		while (item != 0) {
			if (item == priv->selectedItem) {
				priv->firstItem = priv->selectedItem;
				int oldX = item->bounds.x, oldY = item->bounds.y;
				item->bounds.x = leftItemEdge;
				item->bounds.y = y;
				item->showing = W_TRUE;
				/*if (this->data.showClose || item->showClose) {
				 item.closeRect.x = leftItemEdge - renderer.computeTrim(i, SWT.NONE, 0, 0, 0, 0).x;
				 item.closeRect.y = onBottom ? size.y - borderBottom - tabHeight + (tabHeight - closeButtonSize.y)/2: borderTop + (tabHeight - closeButtonSize.y)/2;
				 }*/
				if (item->bounds.x != oldX || item->bounds.y != oldY)
					changed = W_TRUE;
			} else {
				item->bounds.x = defaultX;
				item->showing = W_FALSE;
			}
			i++;
			item = item->next;
		}
	} else {
		int rightItemEdge = wc_tabfolder_get_right_item_edge(tabfolder, gc);
		int maxWidth = rightItemEdge - leftItemEdge;
		int width = 0;
		item = priv->items;
		i = 0;
		while (item != 0) {
			//CTabItem item = items[priority[i]];
			width += item->bounds.width;
			item->showing =
					i == 0 ?
							W_TRUE :
							item->bounds.width > 0 && width <= maxWidth;
			item = item->next;
			i++;
		}
		int x = wc_tabfolder_get_left_item_edge(tabfolder, gc,
				WC_TABFOLDER_PART_HEADER, 0);
		int defaultX = screen.width + 10; // off screen
		int firstIndex = priv->items_count - 1;
		priv->firstItem = priv->items->prev;
		wc_tabitem *item = priv->items;
		i = 0;
		while (item != 0) {
			if (!item->showing) {
				if (item->bounds.x != defaultX)
					changed = W_TRUE;
				item->bounds.x = defaultX;
			} else {
				//this->data.firstIndex = w_min(this->data.firstIndex, i);
				if (i < firstIndex) {
					firstIndex = i;
					priv->firstItem = item;
				}
				if (item->bounds.x != x || item->bounds.y != y)
					changed = W_TRUE;
				item->bounds.x = x;
				item->bounds.y = y;
				int state = W_NONE;
				if (item == priv->selectedItem)
					state |= W_SELECTED;
				//Rectangle edgeTrim = renderer.computeTrim(i, state, 0, 0, 0, 0);
				//item.closeRect.x = item.x + item.width  - (edgeTrim.width + edgeTrim.x) - closeButtonSize.x;
				//item.closeRect.y = onBottom ? size.y - borderBottom - tabHeight + (tabHeight - closeButtonSize.y)/2: borderTop + (tabHeight - closeButtonSize.y)/2;
				x = x + item->bounds.width;
				//if (!this->data.simple && item == this->data.selectedItem) x -= renderer.curveIndent; //TODO: fix next item position
			}
			item = item->next;
			i++;
		}
	}
	return changed;
}

wresult wc_tabfolder_update_items(w_tabfolder *tabfolder, w_graphics *gc,
		wc_tabitem *showItem) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	if (!priv->single && !priv->mru && showItem != 0) {
		// make sure selected item will be showing
		/*int firstIndex = showIndex;
		 if (priority[0] < showIndex) {
		 int maxWidth = getRightItemEdge(gc)
		 - getLeftItemEdge(gc, PART_BORDER);
		 int width = 0;
		 int[] widths = new int[items.length];
		 for (int i = priority[0]; i <= showIndex; i++) {
		 int state = MINIMUM_SIZE;
		 if (i == selectedIndex)
		 state |= W_SELECTED;
		 widths[i] = renderer.computeSize(i, state, gc, SWT.DEFAULT,
		 SWT.DEFAULT).x;
		 width += widths[i];
		 if (width > maxWidth)
		 break;
		 }
		 if (width > maxWidth) {
		 width = 0;
		 for (int i = showIndex; i >= 0; i--) {
		 int state = MINIMUM_SIZE;
		 if (i == selectedIndex)
		 state |= SWT.SELECTED;
		 if (widths[i] == 0)
		 widths[i] = renderer.computeSize(i, state, gc,
		 SWT.DEFAULT, SWT.DEFAULT).x;
		 width += widths[i];
		 if (width > maxWidth)
		 break;
		 firstIndex = i;
		 }
		 } else {
		 firstIndex = priority[0];
		 for (int i = showIndex + 1; i < items.length; i++) {
		 int state = MINIMUM_SIZE;
		 if (i == selectedIndex)
		 state |= SWT.SELECTED;
		 widths[i] = renderer.computeSize(i, state, gc, SWT.DEFAULT,
		 SWT.DEFAULT).x;
		 width += widths[i];
		 if (width >= maxWidth)
		 break;
		 }
		 if (width < maxWidth) {
		 for (int i = priority[0] - 1; i >= 0; i--) {
		 int state = MINIMUM_SIZE;
		 if (i == selectedIndex)
		 state |= SWT.SELECTED;
		 if (widths[i] == 0)
		 widths[i] = renderer.computeSize(i, state, gc,
		 SWT.DEFAULT, SWT.DEFAULT).x;
		 width += widths[i];
		 if (width > maxWidth)
		 break;
		 firstIndex = i;
		 }
		 }
		 }

		 }
		 if (firstIndex != priority[0]) {
		 int index = 0;
		 // enumerate tabs from first visible to the last existing one (sorted ascending)
		 for (int i = firstIndex; i < items.length; i++) {
		 priority[index++] = i;
		 }
		 // enumerate hidden tabs on the left hand from first visible one
		 // in the inverse order (sorted descending) so that the originally
		 // first opened tab is always at the end of the list
		 for (int i = firstIndex - 1; i >= 0; i--) {
		 priority[index++] = i;
		 }
		 }*/
	}

	int oldShowChevron = priv->showChevron;
	int changed = wc_tabfolder_set_item_size(tabfolder, gc);
	changed |= wc_tabfolder_set_item_location(tabfolder, gc);
	//setButtonBounds(gc);
	changed |= priv->showChevron != oldShowChevron;
	/*if (changed && getToolTipText() != null) {
	 Point pt = getDisplay().getCursorLocation();
	 pt = toControl(pt);
	 _setToolTipText(pt.x, pt.y);
	 }*/
	return changed;
}
void wc_tabfolder_redraw_tabs(w_tabfolder *tabfolder) {
	w_rect bounds, trim, tmp;
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	w_control_get_bounds(W_CONTROL(tabfolder), &bounds.pt,&bounds.sz);
	memset(&tmp, 0, sizeof(tmp));
	priv->renderer->compute_trim(tabfolder, 0, WC_TABFOLDER_PART_BODY, W_NONE,
			&trim, &tmp);
	int style = w_widget_get_style(W_WIDGET(tabfolder));
	if (style & W_BOTTOM) {
		int h = trim.height + trim.y - priv->marginHeight;
		tmp.x = 0;
		tmp.y = bounds.height - h - 1;
		tmp.width = bounds.width;
		tmp.height = h + 1;
	} else {
		tmp.x = 0;
		tmp.y = 0;
		tmp.width = bounds.width;
		tmp.height = -trim.y - priv->marginHeight + 1;
	}
	w_control_redraw(W_CONTROL(tabfolder), &tmp, W_FALSE);
}
void wc_tabfolder_update(w_tabfolder *tabfolder, int flags) {
	if (flags == 0)
		return;
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	w_rect rectBefore, rectAfter;
	w_graphics gc;
	w_scrollable_get_client_area(W_SCROLLABLE(tabfolder), &rectBefore);
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(tabfolder), &gc);
	wc_tabfolder_update_tab_height(tabfolder, &gc, W_FALSE);
	wc_tabfolder_update_items(tabfolder, &gc, priv->selectedItem);
	if ((flags & WC_TABFOLDER_REDRAW) != 0) {
		w_control_redraw(W_CONTROL(tabfolder), 0, W_TRUE);
	} else if ((flags & WC_TABFOLDER_REDRAW_TABS) != 0) {
		wc_tabfolder_redraw_tabs(tabfolder);
	}
	w_scrollable_get_client_area(W_SCROLLABLE(tabfolder), &rectAfter);
	if (!w_rect_equals(&rectBefore, &rectAfter)) {
		/*notifyListeners(SWT.Resize, new Event());
		 layout();*/
	}
	w_graphics_dispose(&gc);
}
void wc_tabfolder_show_item(w_tabfolder *tabfolder, wc_tabitem *item) {
	if (item == 0)
		return;
	/*if (item.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 int index = indexOf(item);
	 if (index == -1) SWT.error(SWT.ERROR_INVALID_ARGUMENT);*
	 int idx = -1;
	 for (int i = 0; i < priority.length; i++) {
	 if (priority[i] == index) {
	 idx = i;
	 break;
	 }
	 }
	 if (mru) {
	 // move to front of mru order
	 int[] newPriority = new int[priority.length];
	 System.arraycopy(priority, 0, newPriority, 1, idx);
	 System.arraycopy(priority, idx+1, newPriority, idx+1, priority.length - idx - 1);
	 newPriority[0] = index;
	 priority = newPriority;
	 }*/
	if (item->showing)
		return;
	wc_tabfolder_update(tabfolder, WC_TABFOLDER_REDRAW_TABS);
}
void wc_tabfolder_set_selection_0(w_tabfolder *tabfolder, wc_tabitem *selection,
		int notify) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	wc_tabitem *oldSelectedItem = priv->selectedItem;
	priv->selectedItem = selection;
	if (selection != 0) {
		if (oldSelectedItem == selection) {
			wc_tabfolder_show_item(tabfolder, selection);
			return;
		} else {
			if (oldSelectedItem != 0) {
				//oldSelectedItem->closeImageState = SWT.BACKGROUND;
				oldSelectedItem->state &= ~W_SELECTED;
			}
			//selection->closeImageState = SWT.NONE;
			selection->showing = W_FALSE;
			selection->state |= W_SELECTED;

			wc_tabfolder_show_item(tabfolder, selection);
			w_control_redraw(W_CONTROL(tabfolder), 0, W_TRUE);
		}
	}
	if (notify && priv->selectedItem != oldSelectedItem
			&& priv->selectedItem != 0) {
		w_event_tabitem e;
		w_tabitem item;
		e.event.type = W_EVENT_ITEM_GET_CONTROL;
		e.event.platform_event = 0;
		e.event.widget = W_WIDGET(tabfolder);
		e.event.time = 0;
		e.event.data = 0;
		e.control = 0;
		e.item = &item;
		_WC_TABITEM(&item)->clazz = W_WIDGETDATA_CLASS(
				W_TABFOLDER_GET_CLASS(tabfolder)->class_tabfolderitem);
		_WC_TABITEM(&item)->parent = tabfolder;
		_WC_TABITEM(&item)->item = selection;
		_WC_TABITEM(&item)->index = 0;
		if (w_widget_send_event(W_WIDGET(tabfolder), (w_event*) &e)) {
			w_control *newControl = e.control;
			w_control *oldControl = 0;
			oldControl = priv->selectedcontrol;

			if (newControl != oldControl) {
				if (w_widget_is_ok(W_WIDGET(newControl))) {
					w_rect clientArea;
					w_scrollable_get_client_area(W_SCROLLABLE(tabfolder),
							&clientArea);
					w_control_set_bounds(newControl, &clientArea.pt,&clientArea.sz);
					w_control_set_visible(newControl, W_TRUE);
				}
				if (w_widget_is_ok(W_WIDGET(oldControl))) {
					w_control_set_visible(oldControl, W_FALSE);
				}
			}
			priv->selectedcontrol = newControl;
		}
		e.event.type = W_EVENT_ITEM_SELECTION;
		w_widget_send_event(W_WIDGET(tabfolder), (w_event*) &e);
		/*Event event = new Event();
		 event.item = getItem(selectedIndex);
		 notifyListeners(SWT.Selection, event);*/
	}
}
/*
 *
 */
wresult wc_tabitem_is_ok(w_widgetdata *obj) {
	return W_TRUE;
}
void wc_tabitem_close(w_widgetdata *obj) {

}
void wc_tabitem_copy(w_widgetdata *from, w_widgetdata *to) {
	_WC_TABITEM(to)->clazz = _WC_TABITEM(from)->clazz;
	_WC_TABITEM(to)->parent = _WC_TABITEM(from)->parent;
	_WC_TABITEM(to)->item = _WC_TABITEM(from)->item;
}
wresult wc_tabitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	if (_WC_TABITEM(obj1)->parent != _WC_TABITEM(obj2)->parent)
		return W_FALSE;
	if (_WC_TABITEM(obj1)->item != _WC_TABITEM(obj2)->item)
		return W_FALSE;
	return W_TRUE;
}
w_widget* wc_tabitem_get_parent_widget(w_item *item) {
	return (w_widget*) _WC_TABITEM(item)->parent;
}
void* wc_tabitem_get_data(w_item *item) {
	return _WC_TABITEM(item)->item->userdata;
}
int wc_tabitem_get_index(w_item *item) {
	return _WC_TABITEM(item)->index;
}
wresult wc_tabitem_get_text(w_item *item, w_alloc *text) {
	//return w_alloc_set_text(text, _WC_TABITEM(item)->item->text, -1);
}
wresult wc_tabitem_set_data(w_item *item, void *data) {
	_WC_TABITEM(item)->item->userdata = data;
	return W_TRUE;
}
wresult wc_tabitem_set_text(w_item *item, const char *string) {
	_WC_TABITEM(item)->item->text = strdup(string);
	return W_TRUE;
}
wresult wc_tabitem_get_bounds(w_tabitem *item, w_rect *rect) {
	if (rect == 0)
		return W_ERROR_NULL_ARGUMENT;
	memcpy(rect, &_WC_TABITEM(item)->item->bounds, sizeof(w_rect));
	return W_TRUE;
}
int wc_tabitem_get_image(w_tabitem *item) {
	return _WC_TABITEM(item)->item->image;
}
wresult wc_tabitem_set_image(w_tabitem *item, int image) {
	_WC_TABITEM(item)->item->image = image;
	return W_TRUE;
}
wresult wc_tabitem_pack(w_tabitem *item, w_control *control) {
}

wresult wc_tabfolder_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	style = wc_tabfolder_check_style(W_COMPOSITE(widget), style);
	wresult result = wc_control_create(widget, parent, style, post_event,
			sizeof(struct wc_tabfolder_priv));
	if (result > 0) {
		struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
		priv->renderer = &custom_renderer;
		priv->fixedTabHeight = W_DEFAULT;
		priv->showUnselectedImage = W_TRUE;

	}
	return result;
}
w_imagelist* wc_tabfolder_get_imagelist(w_tabfolder *tabfolder) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	return priv->imagelist;
}
wresult wc_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item) {

}
wresult wc_tabfolder_get_item_p(w_tabfolder *tabfolder, w_point *point,
		w_tabitem *item) {
}
int wc_tabfolder_get_item_count(w_tabfolder *tabfolder) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	return priv->items_count;
}
void wc_tabfolder_get_items(w_tabfolder *tabfolder, w_iterator *items) {
}
wresult wc_tabfolder_get_selection(w_tabfolder *tabfolder, w_tabitem *item) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	if (priv->selectedItem != 0) {
		if (item != 0) {
			_WC_TABITEM(item)->clazz = W_WIDGETDATA_CLASS(
					W_TABFOLDER_GET_CLASS(tabfolder)->class_tabfolderitem);
			_WC_TABITEM(item)->parent = tabfolder;
			_WC_TABITEM(item)->item = priv->selectedItem;
		}
		return W_TRUE;
	}
	return W_FALSE;
}
wresult wc_tabfolder_insert_item(w_tabfolder *tabfolder, w_tabitem *item,
		const char *text, int index) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	wc_tabitem *i = wc_tabfolder_priv_insert_item(priv, index);
	if (i == 0)
		return W_ERROR_NO_MEMORY;
	if (text != 0) {
		i->text = strdup(text);
		if (i->text == 0)
			return W_ERROR_NO_MEMORY;
	}
	//this->data.priorityLength++;
	//this->data.priorityIndex = 0;
	//this->data.priorityItem = static_cast<CTabItem*>(this->data.items.first);
	if (priv->items_count == 1) {
		wc_tabfolder_update(tabfolder,
				WC_TABFOLDER_UPDATE_TAB_HEIGHT | WC_TABFOLDER_REDRAW);
		priv->selectedItem = i;
		i->state = W_SELECTED;
	} else {
		wc_tabfolder_update(tabfolder, WC_TABFOLDER_REDRAW_TABS);
	}
	if (item != 0) {
		_WC_TABITEM(item)->clazz = W_WIDGETDATA_CLASS(
				W_TABFOLDER_GET_CLASS(tabfolder)->class_tabfolderitem);
		_WC_TABITEM(item)->parent = tabfolder;
		_WC_TABITEM(item)->item = i;
	}
	return W_TRUE;
}
wresult wc_tabfolder_set_imagelist(w_tabfolder *tabfolder,
		w_imagelist *imagelist) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(tabfolder));
	priv->imagelist = imagelist;
	return W_TRUE;
}
wresult wc_tabfolder_set_selection(w_tabfolder *tabfolder, int index) {

}
int wc_tabfolder_paint(w_widget *widget, w_event_paint *e) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	if (priv->inDispose)
		return W_TRUE;
	w_font *font;
	w_control_get_font(W_CONTROL(widget),&font);
	if (priv->fontChanged) {
		priv->fontChanged = W_FALSE;
		// handle case where  default font changes
		if (!wc_tabfolder_update_tab_height(W_TABFOLDER(widget), e->gc,
				W_FALSE)) {
			wc_tabfolder_update_items(W_TABFOLDER(widget), e->gc,
					priv->selectedItem);
			w_control_redraw(W_CONTROL(widget), 0, W_TRUE);
			return W_TRUE;
		}
	}

	w_font *gcFont = w_graphics_get_font(e->gc);
	w_color gcBackground = w_graphics_get_background(e->gc);
	w_color gcForeground = w_graphics_get_foreground(e->gc);

// Useful for debugging paint problems
//{
//Point size = getSize();
//gc.setBackground(getDisplay().getSystemColor(SWT.COLOR_GREEN));
//gc.fillRectangle(-10, -10, size.x + 20, size.y+20);
//}
	w_rect bodyRect;
	w_control_get_bounds(W_CONTROL(widget), &bodyRect.pt,&bodyRect.sz);
	bodyRect.x = bodyRect.y = 0;

	priv->renderer->draw(W_TABFOLDER(widget), 0, WC_TABFOLDER_PART_BODY,
			W_BACKGROUND | W_FOREGROUND, &bodyRect, e->gc);

	w_graphics_set_font(e->gc, gcFont);
	w_graphics_set_foreground(e->gc, gcForeground);
	w_graphics_set_background(e->gc, gcBackground);

	priv->renderer->draw(W_TABFOLDER(widget), 0, WC_TABFOLDER_PART_HEADER,
			W_BACKGROUND | W_FOREGROUND, &bodyRect, e->gc);

	w_graphics_set_font(e->gc, gcFont);
	w_graphics_set_foreground(e->gc, gcForeground);
	w_graphics_set_background(e->gc, gcBackground);

	if (!priv->single) {
		int i = 0;
		wc_tabitem *item = priv->items;
		while (item != 0) {
			if (item != priv->selectedItem
					&& w_rect_intersects(&item->bounds, &e->bounds)) {
				priv->renderer->draw(W_TABFOLDER(widget), item, i,
						W_BACKGROUND | W_FOREGROUND | item->state,
						&item->bounds, e->gc);
			}
			i++;
			item = item->next;
		}
	}

	w_graphics_set_font(e->gc, gcFont);
	w_graphics_set_foreground(e->gc, gcForeground);
	w_graphics_set_background(e->gc, gcBackground);

	if (priv->selectedItem != 0) {
		priv->renderer->draw(W_TABFOLDER(widget), priv->selectedItem, 0,
				priv->selectedItem->state | W_BACKGROUND | W_FOREGROUND,
				&priv->selectedItem->bounds, e->gc);
	}

	w_graphics_set_font(e->gc, gcFont);
	w_graphics_set_foreground(e->gc, gcForeground);
	w_graphics_set_background(e->gc, gcBackground);

	/*if (hoverTb) {
	 Rectangle trim = renderer.computeTrim(CTabFolderRenderer.PART_BORDER, SWT.NONE, 0, 0, 0, 0);
	 int x = getSize().x - (trim.width + trim.x);
	 hoverRect = new Rectangle(x - 16 - SPACING, 2, 16, getTabHeight() - 2);
	 gc.setForeground(gc.getDevice().getSystemColor(SWT.COLOR_WIDGET_NORMAL_SHADOW));
	 x = hoverRect.x;
	 int y = hoverRect.y;
	 gc->SetBackground(gc.getDevice().getSystemColor(SWT.COLOR_WHITE));
	 gc.fillRectangle(x + hoverRect.width - 6, y, 5, 5);
	 gc.drawRectangle(x + hoverRect.width - 6, y, 5, 5);
	 gc.drawLine(x + hoverRect.width - 6, y+2, x + hoverRect.width - 6 + 5, y + 2);
	 gc.fillRectangle(x, y, 5 , 2);
	 gc.drawRectangle(x, y, 5 , 2);
	 }*/
	w_graphics_set_font(e->gc, gcFont);
	w_graphics_set_foreground(e->gc, gcForeground);
	w_graphics_set_background(e->gc, gcBackground);
	return W_TRUE;
}
int wc_tabfolder_mousemove(w_widget *widget, w_event_mouse *e) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	//_setToolTipText(event.x, event.y);
	int close = W_FALSE;
	w_graphics gc;
	w_rect closeRect;
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(widget), &gc);
	wc_tabitem *item = priv->items;
	while (item != 0) {
		close = W_FALSE;
		if (w_rect_contains(&item->bounds, e->x, e->y)) {
			close = W_TRUE;
			wc_tabitem_get_close_rect(W_TABFOLDER(widget), item, &gc,
					&closeRect);
			if (w_rect_contains(&closeRect, e->x, e->y)) {
				if (item->closeImageState != W_SELECTED
						&& item->closeImageState != W_HOT) {
					item->closeImageState = W_HOT;
					w_control_redraw(W_CONTROL(widget), &closeRect, W_FALSE);
				}
			} else {
				if (item->closeImageState != W_NONE) {
					item->closeImageState = W_NONE;
					w_control_redraw(W_CONTROL(widget), &closeRect, W_FALSE);
				}
			}
			if ((item->state & W_HOT) == 0) {
				item->state |= W_HOT;
				w_control_redraw(W_CONTROL(widget), &item->bounds, W_FALSE);
			}
		}
		if (item != priv->selectedItem && item->closeImageState != W_BACKGROUND
				&& !close) {
			item->closeImageState = W_BACKGROUND;
			w_control_redraw(W_CONTROL(widget), &closeRect, W_FALSE);
		}
		if ((item->state & W_HOT) != 0 && !close) {
			item->state &= ~W_HOT;
			w_control_redraw(W_CONTROL(widget), &item->bounds, W_FALSE);
		}
		if (item == priv->selectedItem && item->closeImageState != W_NONE
				&& !close) {
			item->closeImageState = W_NONE;
			w_control_redraw(W_CONTROL(widget), &closeRect, W_FALSE);
		}
		item = item->next;
	}
	return W_TRUE;
}
int wc_tabfolder_mousedown(w_widget *widget, w_event_mouse *event) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	/*if (hoverTb && hoverRect.contains(x, y) && !hovering) {
	 hovering = true;
	 updateItems();
	 hoverTimerRunning = true;
	 SetTimer(2000);
	 return;
	 }*/
	if (event->button != 1)
		return W_TRUE;
	wc_tabitem *item = 0, *tab;
	if (priv->single) {
		if (priv->selectedItem != 0) {
			if (w_rect_contains(&priv->selectedItem->bounds, event->x,
					event->y)) {
				item = priv->selectedItem;
			}
		}
	} else {
		tab = priv->items;
		while (tab != 0) {
			if (w_rect_contains(&tab->bounds, event->x, event->y)) {
				item = tab;
			}
			tab = tab->next;
		}
	}
	wuint64 style = w_widget_get_style(widget);
	if (item != 0) {
		if ((style & W_CLOSE) || item->showClose) {
			w_rect closeRect;
			w_graphics gc;
			w_graphics_init(&gc);
			w_control_get_graphics(W_CONTROL(widget), &gc);
			wc_tabitem_get_close_rect(W_TABFOLDER(widget), item, &gc,
					&closeRect);
			w_graphics_dispose(&gc);
			if (w_rect_contains(&closeRect, event->x, event->y)) {
				item->closeImageState = W_SELECTED;
				w_control_redraw(W_CONTROL(widget), &closeRect, W_FALSE);
				w_control_update(W_CONTROL(widget));
				return W_TRUE;
			}
		}
		if (item->showing) {
			wc_tabitem *oldSelectedItem = priv->selectedItem;
			wc_tabfolder_set_selection_0(W_TABFOLDER(widget), item, W_TRUE);
			if (oldSelectedItem == priv->selectedItem) {
				/* If the click is on the selected tabitem, then set focus to the tabfolder */
				w_control_force_focus(W_CONTROL(widget));
			}
		}
		return W_TRUE;
	}
	return W_TRUE;
}
int wc_tabfolder_mouseup(w_widget *widget, w_event_mouse *e) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	return W_TRUE;
}
int wc_tabfolder_resize(w_widget *widget, w_event *e) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	wuint64 style = w_widget_get_style(widget);
	if (priv->inDispose)
		return W_TRUE;
	if (priv->ignoreResize)
		return W_TRUE;
	w_graphics gc;
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(widget), &gc);
	if (wc_tabfolder_update_items(W_TABFOLDER(widget), &gc,
			priv->selectedItem)) {
		wc_tabfolder_redraw_tabs(W_TABFOLDER(widget));
	}
	w_rect bounds;
	w_control_get_bounds(W_CONTROL(widget), &bounds.pt,&bounds.sz);
	if (priv->oldSize.height == 0) {
		w_control_redraw(W_CONTROL(widget), 0, W_TRUE);
	} else {
		if ((style & W_BOTTOM) && bounds.height != priv->oldSize.height) {
			w_control_redraw(W_CONTROL(widget), 0, W_TRUE);
		} else {
			w_rect trim, r;
			memset(&r, 0, sizeof(w_rect));
			int x1 = w_min(bounds.width, priv->oldSize.width);
			priv->renderer->compute_trim(W_TABFOLDER(widget), 0,
					WC_TABFOLDER_PART_BODY, W_NONE, &trim, &r);
			if (bounds.width != priv->oldSize.width)
				x1 -= trim.width + trim.x - priv->marginWidth + 2;
			if (!priv->simple)
				x1 -= 5; // rounded top right corner
			int y1 = w_min(bounds.height, priv->oldSize.height);
			if (bounds.height != priv->oldSize.height)
				y1 -= trim.height + trim.y - priv->marginHeight;
			int x2 = w_max(bounds.width, priv->oldSize.height);
			int y2 = w_max(bounds.height, priv->oldSize.height);
			r.x = 0;
			r.y = y1;
			r.width = x2;
			r.height = y2 - y1;
			w_control_redraw(W_CONTROL(widget), &r, W_FALSE);
			r.x = x1;
			r.y = 0;
			r.width = x2 - x1;
			r.height = y2;
			w_control_redraw(W_CONTROL(widget), &r, W_FALSE);
			/*if (hoverTb) {
			 this->Redraw(hoverRect.x, hoverRect.y, hoverRect.width, hoverRect.height, false);
			 }*/
		}
	}
	priv->oldSize.width = bounds.width;
	priv->oldSize.height = bounds.height;
	return W_TRUE;
}
int wc_tabfolder_compute_trim(w_widget *widget, w_event_compute_trim *e) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	wuint64 style = w_widget_get_style(widget);
	priv->renderer->compute_trim(W_TABFOLDER(widget), 0, WC_TABFOLDER_PART_BODY,
			W_NONE, e->result, e->rect);
	int wrapHeight = wc_tabfolder_get_wrapped_height(W_TABFOLDER(widget),
			e->rect);
	if (style & W_BOTTOM) {
		e->result->height += wrapHeight;
	} else {
		e->result->y -= wrapHeight;
		e->result->height += wrapHeight;
	}
	return W_TRUE;
}
int wc_tabfolder_compute_size(w_widget *widget, w_event_compute_size *e) {
	if (e->wHint != W_DEFAULT && e->wHint != W_DEFAULT) {
		e->size->width = e->wHint;
		e->size->height = e->hHint;
		return W_TRUE;
	}
	w_rect bounds, result;
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	if (e->wHint == W_DEFAULT)
		e->size->width = result.width;
	else
		e->size->width = e->wHint;
	if (e->hHint == W_DEFAULT)
		e->size->height = result.height;
	else
		e->size->height = e->hHint;
	return W_TRUE;
}
int wc_tabfolder_client_area(w_widget *widget, w_event_client_area *e) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	wuint64 style = w_widget_get_style(widget);
	w_rect bounds;
	memset(&bounds, 0, sizeof(w_rect));
	priv->renderer->compute_trim(W_TABFOLDER(widget), 0, WC_TABFOLDER_PART_BODY,
			W_FILL, e->rect, &bounds);
	w_control_get_bounds(W_CONTROL(widget), &bounds.pt,&bounds.sz);
	int wrapHeight = wc_tabfolder_get_wrapped_height(W_TABFOLDER(widget),
			&bounds);
	if (style & W_BOTTOM) {
		e->rect->height += wrapHeight;
	} else {
		e->rect->y -= wrapHeight;
		e->rect->height += wrapHeight;
	}
	e->rect->x = -e->rect->x;
	e->rect->y = -e->rect->y;
	if (priv->minimized) {
		e->rect->width = 0;
		e->rect->height = 0;
	} else {
		e->rect->width = bounds.width - e->rect->width;
		e->rect->height = bounds.height - e->rect->height;
	}
	return W_TRUE;
}
int wc_tabfolder_layout(w_widget *widget, w_event *e) {
	struct wc_tabfolder_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	wuint64 style = w_widget_get_style(widget);
	w_tabitem item;
	w_widgetdata_init(W_WIDGETDATA(&item));
	w_tabfolder_get_selection(W_TABFOLDER(widget), &item);
	if (w_widgetdata_is_ok(W_WIDGETDATA(&item))) {
		w_control *control = w_tabitem_get_control(&item);
		if (w_widget_is_ok(W_WIDGET(control))) {
			w_rect rect;
			w_scrollable_get_client_area(W_SCROLLABLE(widget), &rect);
			w_control_set_bounds(control, &rect.pt,&rect.sz);
		}

	}
	w_widgetdata_close(W_WIDGETDATA(&item));
	return W_TRUE;
}
int wc_tabfolder_post_event(w_widget *widget, w_event *e) {
	switch (e->type) {
	case W_EVENT_PAINT:
		return wc_tabfolder_paint(widget, (w_event_paint*) e);
		break;
	case W_EVENT_RESIZE:
		return wc_tabfolder_resize(widget, e);
		break;
	case W_EVENT_MOUSEMOVE:
		return wc_tabfolder_mousemove(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEDOWN:
		return wc_tabfolder_mousedown(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEUP:
		return wc_tabfolder_mouseup(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_COMPUTE_TRIM:
		return wc_tabfolder_compute_trim(widget, (w_event_compute_trim*) e);
		break;
	case W_EVENT_COMPUTE_SIZE:
		return wc_tabfolder_compute_size(widget, (w_event_compute_size*) e);
		break;
	case W_EVENT_CLIENT_AREA:
		return wc_tabfolder_client_area(widget, (w_event_client_area*) e);
		break;
	case W_EVENT_LAYOUTDETECT:
		return wc_tabfolder_layout(widget, e);
		break;
	}
	return widget->clazz->next_class->post_event(widget, e);
}
void wc_tabfolder_init_class(struct _w_tabfolder_class *clazz) {
	wc_composite_init_class(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TABFOLDER;
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tabfolder);
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tabfolder_class);
	wc_composite_init_class_priv(W_COMPOSITE_CLASS(clazz),
			sizeof(wc_tabfolder_priv));
	W_WIDGET_CLASS(clazz)->create = wc_tabfolder_create;
	W_WIDGET_CLASS(clazz)->post_event = wc_tabfolder_post_event;
	clazz->get_imagelist = wc_tabfolder_get_imagelist;
	clazz->get_item = wc_tabfolder_get_item;
	clazz->get_item_p = wc_tabfolder_get_item_p;
	clazz->get_item_count = wc_tabfolder_get_item_count;
	clazz->get_items = wc_tabfolder_get_items;
	clazz->get_selection = wc_tabfolder_get_selection;
	clazz->insert_item = wc_tabfolder_insert_item;
	clazz->set_imagelist = wc_tabfolder_set_imagelist;
	clazz->set_selection = wc_tabfolder_set_selection;
	struct _w_tabitem_class *item = clazz->class_tabfolderitem;
	W_WIDGETDATA_CLASS(item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
	W_WIDGETDATA_CLASS(item)->is_ok = wc_tabitem_is_ok;
	W_WIDGETDATA_CLASS(item)->close = wc_tabitem_close;
	W_WIDGETDATA_CLASS(item)->copy = wc_tabitem_copy;
	W_WIDGETDATA_CLASS(item)->equals = wc_tabitem_equals;
	W_ITEM_CLASS(item)->get_parent_widget = wc_tabitem_get_parent_widget;
	W_ITEM_CLASS(item)->get_data = wc_tabitem_get_data;
	W_ITEM_CLASS(item)->get_index = wc_tabitem_get_index;
	W_ITEM_CLASS(item)->get_text = wc_tabitem_get_text;
	W_ITEM_CLASS(item)->set_data = wc_tabitem_set_data;
	W_ITEM_CLASS(item)->set_text = wc_tabitem_set_text;
	item->get_bounds = wc_tabitem_get_bounds;
	item->get_image = wc_tabitem_get_image;
	item->set_image = wc_tabitem_set_image;
	item->pack = wc_tabitem_pack;

}
