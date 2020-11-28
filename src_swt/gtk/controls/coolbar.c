/*
 * coolbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "coolbar.h"
#include "../widgets/toolkit.h"
#define MARGIN_WIDTH 4
#define GRABBER_WIDTH 2
#define MINIMUM_WIDTH ((2 * MARGIN_WIDTH) + GRABBER_WIDTH)

#define CHEVRON_HORIZONTAL_TRIM -1			//platform dependent values
#define CHEVRON_VERTICAL_TRIM -1
#define CHEVRON_LEFT_MARGIN 2
#define CHEVRON_IMAGE_WIDTH 8	//Width to draw the double arrow
#define ROW_SPACING 2
#define CLICK_DISTANCE 3
#define DEFAULT_COOLBAR_WIDTH 0
#define DEFAULT_COOLBAR_HEIGHT 0
wuint64 _w_coolbar_check_style(w_widget *control, wuint64 style) {
	style |= W_NO_FOCUS;
	return (style | W_NO_REDRAW_RESIZE) & ~(W_VSCROLL | W_HSCROLL);
}
wresult _w_coolbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	wresult result = _w_control_create(widget, parent, style,
			post_event);
	if (result > 0) {
		if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
			_W_WIDGET(widget)->style |= W_VERTICAL;
			_W_COOLBAR(widget)->hoverCursor = w_toolkit_get_system_cursor(
					w_widget_get_toolkit(widget), W_CURSOR_SIZENS);
		} else {
			_W_WIDGET(widget)->style |= W_HORIZONTAL;
			_W_COOLBAR(widget)->hoverCursor = w_toolkit_get_system_cursor(
					w_widget_get_toolkit(widget), W_CURSOR_SIZEWE);
		}
		_W_COOLBAR(widget)->dragCursor = w_toolkit_get_system_cursor(
				w_widget_get_toolkit(widget), W_CURSOR_SIZEALL);
		_W_COOLBAR(widget)->items = 0;
	}
	return result;
}
void _w_coolbar_fix_point(w_coolbar *coolbar, w_point *result, int x, int y) {
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		result->x = y;
		result->y = x;
	} else {
		result->x = x;
		result->y = y;
	}
}
void _w_coolbar_fix_rectangle(w_coolbar *coolbar, w_rect *result, int x, int y,
		int width, int height) {
	_w_coolbar_fix_point(coolbar, &result->pt, width, height);
	result->width = width;
	result->height = height;
}
int _w_coolbar_get_width(w_coolbar *coolbar) {
	w_size size;
	w_control_get_bounds(W_CONTROL(coolbar),0, &size);
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0)
		return size.height;
	return size.width;
}
int _w_coolitem_internal_get_minimum_width (w_coolbar *coolbar,__w_coolitem* _item) {
	int width = _item->minimumWidth + MINIMUM_WIDTH;
	if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0 && width < _item->preferredWidth) {
		width += CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM + CHEVRON_LEFT_MARGIN;
	}
	return width;
}
void _w_coolitem_set_bounds (w_coolbar *coolbar,__w_coolitem* _item,w_rect* bounds) {
	memcpy(&_item->itemBounds,bounds,sizeof(w_rect));
	w_rect rect;
	int y = bounds->y,height=bounds->height;
	if (_item->control != 0) {
		int controlWidth = bounds->width - MINIMUM_WIDTH;
		if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0 && bounds->width < _item->preferredWidth) {
			controlWidth -= CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM + CHEVRON_LEFT_MARGIN;
		}
		if (height > _item->preferredHeight) {
			y += (height - _item->preferredHeight) / 2;
			height = _item->preferredHeight;
		}
		_w_coolbar_fix_rectangle(coolbar,&rect,bounds->x + MINIMUM_WIDTH, y, controlWidth, height);
		w_control_set_bounds(_item->control,&rect.pt,&rect.sz);
	}
	//updateChevron();
}
void _w_coolbar_internal_redraw (w_coolbar *coolbar,int x, int y, int width, int height) {
	w_rect rect;
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		rect.x = y;
		rect.y = x;
	} else {
		rect.x = x;
		rect.y = y;
	}
	rect.width = width;
	rect.height = height;
	w_control_redraw(W_CONTROL(coolbar),&rect,FALSE);
}
void _w_coolbar_wrap_items(w_coolbar *coolbar, int maxWidth) {
	__w_coolitem* _item;
	_w_coolitems *_items = _W_COOLBAR(coolbar)->items;
	if(_items == 0) return;
	int itemCount = _items->count;
	if (itemCount <= 1)
		return;
	int start = 0;
	/*CoolItem[] itemsVisual = new CoolItem[itemCount];
	for (int row = 0; row < items.length; row++) {
		System.arraycopy(items[row], 0, itemsVisual, start, items[row].length);
		start += items[row].length;
	}
	CoolItem[][] newItems = new CoolItem[itemCount][];*/
	int rowCount = 0, rowWidth = 0;
	start = 0;
	for (int i = 0; i < itemCount; i++) {
		_item = &_items->items[i];
		int itemWidth =_w_coolitem_internal_get_minimum_width(coolbar, _item);
		if ((i > 0 && _item->wrap)
				|| (maxWidth != W_DEFAULT && rowWidth + itemWidth > maxWidth)) {
			if (i == start) {
				/*newItems[rowCount] = new CoolItem[1];
				newItems[rowCount][0] = item;*/
				start = i + 1;
				rowWidth = 0;
			} else {
				int count = i - start;
				/*newItems[rowCount] = new CoolItem[count];
				System.arraycopy(itemsVisual, start, newItems[rowCount], 0,
						count);*/
				start = i;
				rowWidth = itemWidth;
			}
			_item->newrow = TRUE;
			rowCount++;
		} else {
			_item->newrow = FALSE;
			rowWidth += itemWidth;
		}
	}
	/*if (start < itemCount) {
		int count = itemCount - start;
		newItems[rowCount] = new CoolItem[count];
		System.arraycopy(itemsVisual, start, newItems[rowCount], 0, count);
		rowCount++;
	}
	if (newItems.length != rowCount) {
		CoolItem[][] tmp = new CoolItem[rowCount][];
		System.arraycopy(newItems, 0, tmp, 0, rowCount);
		items = tmp;
	} else {
		items = newItems;
	}*/
}
/**
 * Return the height of the bar after it has
 * been properly laid out for the given width.
 */
int _w_coolbar_layout_items(w_coolbar *coolbar) {
	int y = 0, width;
	w_rect rect;
	w_scrollable_get_client_area(W_SCROLLABLE(coolbar), &rect);
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		width = rect.height;
	} else {
		width = rect.width;
	}
	_w_coolbar_wrap_items(coolbar, width);
	int rowSpacing =
			(_W_WIDGET(coolbar)->style & W_FLAT) != 0 ? 0 : ROW_SPACING;
	__w_coolitem* _item;
	_w_coolitems *_items = _W_COOLBAR(coolbar)->items;
	if (_items != 0) {
		int i = 0, row = 0, row_start, count;
		int itemcount = _items->count;
		while (i < itemcount) {
			int x = 0;
			row_start = i;

			/* determine the height and the available width for the row */
			int rowHeight = 0;
			int available = width;
			count = 0;
			while (i < itemcount) {
				_item = &_items->items[i];
				rowHeight = w_max(rowHeight, _item->preferredHeight);
				available -= _w_coolitem_internal_get_minimum_width(coolbar, _item);
				i++;
				count++;
				if (_item->newrow)
					break;
			}
			if (row > 0)
				y += rowSpacing;

			/* lay the items out */
			i = row_start;
			while (i < itemcount) {
				_item = &_items->items[i];
				int newWidth = available + _w_coolitem_internal_get_minimum_width(coolbar, _item);
				if (i + 1 < count) {
					newWidth = w_min(newWidth, _item->requestedWidth);
					available -= (newWidth - _w_coolitem_internal_get_minimum_width(coolbar, _item));
				}
				w_rect oldBounds, newBounds, damage;
				memcpy(&oldBounds,&_item->itemBounds,sizeof(w_rect));
				newBounds.x = x;
				newBounds.y = y;
				newBounds.width = newWidth;
				newBounds.height = rowHeight;
				if (!w_rect_equals(&oldBounds, &newBounds)) {
					_w_coolitem_set_bounds(coolbar,_item,&newBounds);

					memset(&damage, 0, sizeof(rect));
					/* Cases are in descending order from most area to redraw to least. */
					if (oldBounds.y != newBounds.y) {
						damage = newBounds;
						w_rect_add(&damage, &oldBounds);
						/* Redraw the row separator as well. */
						damage.y -= rowSpacing;
						damage.height += 2 * rowSpacing;
					} else if (oldBounds.height != newBounds.height) {
						/*
						 * Draw from the bottom of the gripper to the bottom of the new area.
						 * (Bottom of the gripper is -3 from the bottom of the item).
						 */
						damage.y = newBounds.y
								+ w_min(oldBounds.height, newBounds.height) - 3;
						damage.height = newBounds.y + newBounds.height
								+ rowSpacing;
						damage.x = oldBounds.x - MARGIN_WIDTH;
						damage.width = oldBounds.width + MARGIN_WIDTH;
					} else if (oldBounds.x != newBounds.x) {
						/* Redraw only the difference between the separators. */
						damage.x = w_min(oldBounds.x, newBounds.x);
						damage.width = abs(
								oldBounds.x - newBounds.x) + MINIMUM_WIDTH;
						damage.y = oldBounds.y;
						damage.height = oldBounds.height;
					}
					_w_coolbar_internal_redraw(coolbar,damage.x, damage.y, damage.width,
							damage.height);
				}
				x += newWidth;
				i++;
				if (_item->newrow)
					break;
			}
			y += rowHeight;
		}
	}
	return y;
}
/*
 *
 */
wresult _w_coolitem_compute_size(w_coolitem *coolitem, w_size *result,
		int wHint, int hHint) {
	w_coolbar *coolbar = _W_COOLITEM(coolitem)->coolbar;
	result->width = wHint, result->height = hHint;
	if (wHint == W_DEFAULT)
		result->width = 32;
	if (hHint == W_DEFAULT)
		result->height = 32;
	if ((_W_WIDGET(coolbar)->style & W_VERTICAL) != 0) {
		result->height += MINIMUM_WIDTH;
	} else {
		result->width += MINIMUM_WIDTH;
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
	w_coolbar *coolbar = _W_COOLITEM(coolitem)->coolbar;
	if (control != 0) {
		if (w_widget_is_ok(W_WIDGET(control)))
			return W_ERROR_INVALID_ARGUMENT;
		w_composite *parent;
		w_control_get_parent(control,&parent);
		if (parent !=(w_composite*) coolbar)
			return W_ERROR_INVALID_PARENT;
	}
	_w_coolitems *_items = _W_COOLBAR(coolbar)->items;
	if (_items == 0)
		return W_ERROR_INVALID_ARGUMENT;
	int index = _W_COOLITEM(coolitem)->index;
	if (!(index >= 0 && index < _items->count))
		return W_ERROR_INVALID_ARGUMENT;
	__w_coolitem* _item = &_items->items[index];
	_item->control = control;
	if (control != 0) {
		int controlWidth = _item->itemBounds.width - MINIMUM_WIDTH;
		if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0
				&& _item->itemBounds.width < _item->preferredWidth) {
			controlWidth -= CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM
					+ CHEVRON_LEFT_MARGIN;
		}
		w_rect bounds;
		_w_coolbar_fix_rectangle(coolbar, &bounds,
				_item->itemBounds.x + MINIMUM_WIDTH, _item->itemBounds.y,
				controlWidth, _item->itemBounds.height);
		w_control_set_bounds(control, &bounds.pt,&bounds.sz);
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
	w_coolbar *coolbar = _W_COOLITEM(coolitem)->coolbar;
	_w_coolitems *_items = _W_COOLBAR(coolbar)->items;
	if (_items == 0)
		return W_ERROR_INVALID_ARGUMENT;
	int index = _W_COOLITEM(coolitem)->index;
	if (!(index >= 0 && index < _items->count))
		return W_ERROR_INVALID_ARGUMENT;
	__w_coolitem* _item = &_items->items[index];
	w_size point;
	_w_coolbar_fix_point(coolbar,(w_point*) &point, size->width, size->height);
	int width = w_max(point.width, _item->minimumWidth + MINIMUM_WIDTH);
	int height = point.height;
	if (!_item->ideal) {
		_item->preferredWidth = width;
		_item->preferredHeight = height;
	}
	_item->itemBounds.width = _item->requestedWidth = width;
	_item->itemBounds.height = height;
	if (_item->control != 0) {
		int controlWidth = width - MINIMUM_WIDTH;
		if ((_W_WIDGET(coolbar)->style & W_DROP_DOWN) != 0
				&& width < _item->preferredWidth) {
			controlWidth -= CHEVRON_IMAGE_WIDTH + CHEVRON_HORIZONTAL_TRIM
					+ CHEVRON_LEFT_MARGIN;
		}
		_w_coolbar_fix_point(coolbar, (w_point*) &point, controlWidth, height);
		w_control_set_bounds(_item->control,0,&point);
	}
//parent.relayout();
//updateChevron();
	return TRUE;
}
wresult _w_coolitem_set_wrap_indice(w_coolitem *coolitem, int wrap_indice) {
}

wresult _w_coolbar_get_item(w_coolbar *coolbar, int index, w_coolitem *item) {
}
int _w_coolbar_get_item_count(w_coolbar *coolbar) {
	if (_W_COOLBAR(coolbar)->items != 0) {
		return _W_COOLBAR(coolbar)->items->count;
	} else
		return 0;
}
void _w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items) {

}
wbool _w_coolbar_get_locked(w_coolbar *coolbar) {
}
wresult _w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item, int style,
		int index) {
	int itemCount = _w_coolbar_get_item_count(coolbar), row = 0;
	if (!(0 <= index && index <= itemCount)) {
		index = itemCount;
		//return W_ERROR_INVALID_RANGE;
	}
	__w_coolitem* _item;
	_w_coolitems *_items = _W_COOLBAR(coolbar)->items;
	if (_items == 0) {
		_items = malloc(
				sizeof(_w_coolitems) + _W_COOLBAR_GROW * sizeof(__w_coolitem ));
		if (_items == 0)
			return W_ERROR_NO_MEMORY;
		_items->alloc = _W_COOLBAR_GROW;
		_items->count = 1;
		_item = &_items->items[0];
		_W_COOLBAR(coolbar)->items = _items;
	} else {
		if (_items->alloc <= _items->count) {
			_w_coolitems *_items = realloc(_items,
					sizeof(_w_coolitems)
							+ (_W_COOLBAR(coolbar)->items->alloc
									+ _W_COOLBAR_GROW) * sizeof(__w_coolitem ));
			if (_items == 0)
				return W_ERROR_NO_MEMORY;
			_W_COOLBAR(coolbar)->items = _items;
			_items->alloc += _W_COOLBAR_GROW;
		}
		if (index != itemCount) {
			for (int i = itemCount; i >= index; i--) {
				memcpy(&_items[i + 1], &_items[i], sizeof(__w_coolitem ));
			}
		}
		_items->count++;
		_item = &_items->items[index];
	}
	memset(_item, 0, sizeof(__w_coolitem ));
	_item->requestedWidth = MINIMUM_WIDTH;
	_w_coolbar_layout_items(coolbar);
	if (item != 0) {
		_W_COOLITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_COOLBAR_GET_CLASS(coolbar)->class_coolbaritem);
		_W_COOLITEM(item)->coolbar = coolbar;
		_W_COOLITEM(item)->index = index;
	}
	return TRUE;
}
wresult _w_coolbar_set_locked(w_coolbar *coolbar, wbool locked) {
}
int _w_coolbar_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int width = 0, height = 0;
	_w_coolbar_wrap_items(W_COOLBAR(widget),
			(_W_WIDGET(widget)->style & W_VERTICAL) != 0 ? e->hHint : e->wHint);
	wbool flat = (_W_WIDGET(widget)->style & W_FLAT) != 0;
	__w_coolitem* _item;
	_w_coolitems *_items = _W_COOLBAR(widget)->items;
	if (_items != 0) {
		int i = 0, row = 0;
		int count = _items->count;
		while (i < count) {
			int rowWidth = 0, rowHeight = 0;
			while (i < count) {
				_item = &_items->items[i];
				rowWidth += _item->preferredWidth;
				rowHeight = w_max(rowHeight, _item->preferredHeight);
				i++;
				if (_item->newrow)
					break;
			}
			height += rowHeight;
			if (!flat && row > 0)
				height += ROW_SPACING;
			width = w_max(width, rowWidth);
			row++;
		}
	}
	_w_coolbar_wrap_items(W_COOLBAR(widget),
			_w_coolbar_get_width(W_COOLBAR(widget)));
	if (width == 0)
		width = DEFAULT_COOLBAR_WIDTH;
	if (height == 0)
		height = DEFAULT_COOLBAR_HEIGHT;
	w_point size;
	_w_coolbar_fix_point(W_COOLBAR(widget), &size, width, height);
	if (e->wHint != W_DEFAULT)
		size.x = e->wHint;
	if (e->hHint != W_DEFAULT)
		size.y = e->hHint;
	w_rect trim, rect;
	rect.x = 0;
	rect.y = 0;
	rect.width = size.x;
	rect.height = size.y;
	w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim, &rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
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
	class_item->item.widgetdata.toolkit =W_TOOLKIT(gtk_toolkit);
	class_item->item.widgetdata.close = _w_widgetdata_close;
	class_item->item.widgetdata.is_ok = _w_widgetdata_is_ok;
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
	reserved->widget.compute_size = _w_coolbar_compute_size;
	_W_WIDGET_RESERVED(reserved)->check_style = _w_coolbar_check_style;
}
#endif
