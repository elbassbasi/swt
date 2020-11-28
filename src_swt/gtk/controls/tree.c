/*
 * tree.c
 *
 *  Created on: 21 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#if __linux
#include "tree.h"
#include "../widgets/toolkit.h"
void _w_treecolumn_renderer_render(w_widget *widget, GtkCellRenderer *_cell,
		cairo_t *cr, GtkWidget *gtkwidget, const GdkRectangle *background_area,
		const GdkRectangle *cell_area, GtkCellRendererState flags) {
	struct _w_cell_renderer *cell = (struct _w_cell_renderer*) _cell;
	struct _w_cell_renderer_class *clazz =
			(struct _w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
	GtkTreeViewColumn *columnHandle = gtk_tree_view_get_column(
			GTK_TREE_VIEW(_W_WIDGET(widget)->handle),
			_W_TREECOLUMN(&cell->column)->index);
	char *text;
	w_event_tree event;
	w_rect rect;
	struct _w_graphics gc;
	w_tree_attr attr;
	wbool wasSelected = FALSE;
	if (GTK_IS_CELL_RENDERER_TEXT(cell)) {
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_GET_ATTR;
		event.event.widget = widget;
		event.column = W_TREECOLUMN(&cell->column);
		event.item = W_TREEITEM(&cell->item);
		event.rect = 0;
		event.gc = 0;
		event.attr = &attr;
		memset(&attr, 0, sizeof(attr));
		int ret = _w_widget_send_event(widget, (w_event*) &event);
		if (!ret) {
			memset(&attr, 0, sizeof(attr));
		}
	}
	if (_W_TREEITEM(&cell->item)->widgetdata.clazz != 0) {
		if (GTK_IS_CELL_RENDERER_TOGGLE(cell)
				|| ((GTK_IS_CELL_RENDERER_PIXBUF(cell)
						|| GTK_VERSION > VERSION(3, 13, 0))
						&& (_W_TREECOLUMN(&cell->column)->index != 0
								|| (_W_WIDGET(widget)->style & W_CHECK) == 0))) {
			_W_TREE(widget)->drawFlags = (int) flags;
			_W_TREE(widget)->drawState = W_FOREGROUND;
			if (attr.background != 0) {
				_W_TREE(widget)->drawState |= W_BACKGROUND;
			}
			if ((flags & GTK_CELL_RENDERER_SELECTED) != 0)
				_W_TREE(widget)->drawState |= W_SELECTED;
#if !GTK3
#else
			if ((flags & GTK_CELL_RENDERER_SELECTED) == 0) {
				if ((flags & GTK_CELL_RENDERER_FOCUSED) != 0)
					_W_TREE(widget)->drawState |= W_FOCUSED;
			}
#endif
			rect.x = cell_area->x;
			rect.y = cell_area->y;
			rect.width = cell_area->width;
			rect.height = cell_area->height;
			/*GtkTreePath* path = gtk_tree_model_get_path (modelHandle, &_W_TREEITEM(&cell->item)->iter);
			 gtk_tree_view_get_background_area (GTK_TREE_VIEW( _W_WIDGET(widget)->handle), path,columnHandle, &rect);
			 gtk_tree_path_free (path);*/
			// Use the x and width information from the Cairo context. See bug 535124.
#if GTK3
			if (cr
					!= 0&& GTK_VERSION > VERSION(3, 9, 0) && GTK_VERSION <= VERSION(3, 14, 8)) {
				GdkRectangle r2;
				gdk_cairo_get_clip_rectangle(cr, &r2);
				rect.x = r2.x;
				rect.width = r2.width;
			}
#endif
			if ((_W_TREE(widget)->drawState & W_SELECTED) == 0) {
				/*if ((_W_WIDGET(widget)->state & STATE_PARENT_BACKGROUND) != 0 || _W_CONTROL(widget).backgroundImage != 0) {
				 Control control = findBackgroundControl ();
				 if (control != null) {
				 if (cr != 0) {
				 Cairo.cairo_save (cr);
				 if (!GTK.GTK3) {
				 Cairo.cairo_reset_clip (cr);
				 }
				 }
				 drawBackground (control, window, cr, 0, rect.x, rect.y, rect.width, rect.height);
				 if (cr != 0) {
				 Cairo.cairo_restore (cr);
				 }
				 }
				 }*/
			}

			//send out measure before erase
			/*long textRenderer =  getTextRenderer (columnHandle);
			 if (textRenderer != 0) gtk_cell_renderer_get_preferred_size (textRenderer, handle, null, null);*/

			//if (hooks (SWT.EraseItem)) {
			/*
			 * Cache the selection state so that it is not lost if a
			 * PaintListener wants to draw custom selection foregrounds.
			 * See bug 528155.
			 */
			wasSelected = (_W_TREE(widget)->drawState & W_SELECTED) != 0;
			if (wasSelected) {
				/*Control control = findBackgroundControl();
				 if (control == null)
				 control = this;
				 if (!GTK.GTK3) {
				 if (cr != 0) {
				 Cairo.cairo_save(cr);
				 Cairo.cairo_reset_clip(cr);
				 }
				 drawBackground(control, window, cr, 0, rect.x, rect.y,
				 rect.width, rect.height);
				 if (cr != 0) {
				 Cairo.cairo_restore(cr);
				 }
				 }*/
			}
			_w_graphics_init(W_GRAPHICS(&gc), cr);
			if ((_W_TREE(widget)->drawState & W_SELECTED) != 0) {
				/*gc.setBackground(
				 display.getSystemColor(SWT.COLOR_LIST_SELECTION));
				 gc.setForeground(
				 display.getSystemColor(SWT.COLOR_LIST_SELECTION_TEXT));*/
			} else {
				/*gc.setBackground(item.getBackground(columnIndex));
				 gc.setForeground(item.getForeground(columnIndex));*/
			}
			//gc.setFont(item.getFont(columnIndex));
			/*if ((style & SWT.MIRRORED) != 0)
			 rect.x = getClientWidth() - rect.width - rect.x;*/
#if GTK3
			if (cr != 0) {
				// Use the original rectangle, not the Cairo clipping for the y, width, and height values.
				// See bug 535124.
				/*Rectangle rect2 = DPIUtil.autoScaleDown(
				 new Rectangle(rect.x, rect.y, rect.width, rect.height));
				 gc.setClipping(rect2.x, rect2.y, rect2.width, rect2.height);*/
			} else {
				/*Rectangle rect2 = DPIUtil.autoScaleDown(
				 new Rectangle(rect.x, rect.y, rect.width, rect.height));
				 // Caveat: rect2 is necessary because GC#setClipping(Rectangle) got broken by bug 446075
				 gc.setClipping(rect2.x, rect2.y, rect2.width, rect2.height);*/
			}
#else
#endif
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_ERASE;
			event.event.widget = widget;
			event.column = W_TREECOLUMN(&cell->column);
			event.item = W_TREEITEM(&cell->item);
			event.rect = &rect;
			event.gc = W_GRAPHICS(&gc);
			/*Rectangle eventRect = new Rectangle(rect.x, rect.y, rect.width,
			 rect.height);
			 event.setBounds(DPIUtil.autoScaleDown(eventRect));*/
			event.detail = _W_TREE(widget)->drawState;
			int ret = w_widget_send_event(widget, (w_event*) &event);
			if (ret) {
#if GTK3
				//drawForegroundRGBA = null;
#else
#endif
				_W_TREE(widget)->drawState = event.doit ? event.detail : 0;
				_W_TREE(widget)->drawFlags &= ~(GTK_CELL_RENDERER_FOCUSED
						| GTK_CELL_RENDERER_SELECTED);
				if ((_W_TREE(widget)->drawState & W_SELECTED) != 0)
					_W_TREE(widget)->drawFlags |= GTK_CELL_RENDERER_SELECTED;
				if ((_W_TREE(widget)->drawState & W_FOCUSED) != 0)
					_W_TREE(widget)->drawFlags |= GTK_CELL_RENDERER_FOCUSED;
				if ((_W_TREE(widget)->drawState & W_SELECTED) != 0) {
#if !GTK3
#endif
				} else {
					if (wasSelected) {
#if GTK3
						//drawForegroundRGBA = gc.getForeground().handleRGBA;
# else
#endif
					}
				}
			}
			w_graphics_dispose(W_GRAPHICS(&gc));
			//}//if (hooks (SWT.EraseItem))
		}
	}
	if ((_W_TREE(widget)->drawState & W_BACKGROUND) != 0
			&& (_W_TREE(widget)->drawState & W_SELECTED) == 0) {
		/*
		 GC gc = getGC(cr);
		 gc.setBackground (item.getBackground (columnIndex));
		 GdkRectangle rect = new GdkRectangle ();
		 OS.memmove (rect, background_area, GdkRectangle.sizeof);
		 gc.fillRectangle(DPIUtil.autoScaleDown(new Rectangle(rect.x, rect.y, rect.width, rect.height)));
		 gc.dispose ();*/
	}
	if ((_W_TREE(widget)->drawState & W_FOREGROUND)
			!= 0|| GTK_IS_CELL_RENDERER_TOGGLE (cell)) {
		/*if (drawForeground != null && GTK_IS_CELL_RENDERER_TEXT (cell) && !GTK.GTK3) {
		 OS.g_object_set (cell, OS.foreground_gdk, drawForeground, 0);
		 } else if (drawForegroundRGBA != null && GTK.GTK_IS_CELL_RENDERER_TEXT (cell) && GTK.GTK3) {
		 OS.g_object_set (cell, OS.foreground_rgba, drawForegroundRGBA, 0);
		 }*/
		if (GTK_IS_CELL_RENDERER_TEXT(cell)) {
			text = 0;
			w_value value;
			GdkRGBA rgba;
			if (attr.background != 0) {
				g_object_set(cell, "background-set", TRUE, NULL);
				rgba.alpha = 1;
				rgba.red = W_RED(attr.background);
				rgba.green = W_GREEN(attr.background);
				rgba.blue = W_BLUE(attr.background);
				g_object_set(cell, "background-rgba", &rgba, NULL);
			} else {
				g_object_set(cell, "background-set", FALSE, NULL);
			}
			if (attr.foreground != 0) {
				g_object_set(cell, "foreground-set", TRUE, NULL);
				rgba.alpha = 1;
				rgba.red = W_RED(attr.foreground);
				rgba.green = W_GREEN(attr.foreground);
				rgba.blue = W_BLUE(attr.foreground);
				g_object_set(cell, "foreground-gdk", &rgba, NULL);
			} else {
				g_object_set(cell, "foreground-set", FALSE, NULL);
			}
			if (attr.font != 0 && _W_FONT(attr.font)->handle != 0) {
				g_object_set(cell, "font-desc", _W_FONT(attr.font)->handle,
				NULL);
			} else {
				g_object_set(cell, "font-desc", NULL,
				NULL);
			}
			if (_W_TREECOLUMN(&cell->column)->index == 0) {
				gtk_tree_model_get(modelHandle, &_W_TREEITEM(&cell->item)->iter,
						COLUMN_TEXT, &text, -1);
			}
			if (text == 0) {
				memset(&event, 0, sizeof(event));
				w_value_init((w_value*) &value);
				char *mem_tmp = (char*) _w_toolkit_malloc(
						W_TOOLKIT(gtk_toolkit),
						gtk_toolkit->mem_tmp_total_size);
				w_value_set_string(&value, mem_tmp,
						gtk_toolkit->mem_tmp_total_size, W_VALUE_USER_MEMORY);
				event.event.type = W_EVENT_ITEM_GET_VALUE;
				event.event.widget = widget;
				event.column = W_TREECOLUMN(&cell->column);
				event.item = W_TREEITEM(&cell->item);
				event.rect = 0;
				event.gc = 0;
				event.value = &value;
				int ret = _w_widget_send_event(widget, (w_event*) &event);
				if (ret) {
					text = w_value_string_copy_is_needed((w_value*) &value,
							mem_tmp, gtk_toolkit->mem_tmp_total_size);
				}
				_w_toolkit_free(W_TOOLKIT(gtk_toolkit), mem_tmp);
			}
			if (text == 0)
				text = "";
			g_object_set(cell, "text", text, NULL);
			clazz->default_render(_cell, cr, gtkwidget, background_area,
					cell_area, flags);
		}
		if (GTK_IS_CELL_RENDERER_TOGGLE(cell)) {
			int info = 0, b;
			gtk_tree_model_get(modelHandle, &_W_TREEITEM(&cell->item)->iter,
					COLUMN_INFO, &info, -1);
			if (info & COLUMN_INFO_CHECK) {
				b = TRUE;
			} else {
				b = FALSE;
			}
			g_object_set(cell, "active", b, NULL);
			clazz->default_render(_cell, cr, gtkwidget, background_area,
					cell_area, flags);
			return;
		}
		if (GTK_IS_CELL_RENDERER_PIXBUF(cell)) {
			int index;
			GdkPixbuf *pixbuf = 0;
			gtk_tree_model_get(modelHandle, &_W_TREEITEM(&cell->item)->iter,
					COLUMN_IMAGE, &index, -1);
			if (index >= 0) {
				w_imagelist *imagelist = _W_TABLEBASE(widget)->imagelist;
				if (imagelist != 0 && _W_IMAGELIST(imagelist)->images != 0
						&& _W_IMAGELIST(imagelist)->images->count > index) {
					pixbuf = _W_IMAGELIST(imagelist)->images->images[index];
				}
			}
			g_object_set(cell, "pixbuf", pixbuf, NULL);
			clazz->default_render(_cell, cr, gtkwidget, background_area,
					cell_area, flags);
			return;
		}
#if GTK3
		clazz->default_render(_cell, cr, gtkwidget, background_area, cell_area,
				flags);
#else
#endif
	}
	if (_W_TREEITEM(&cell->item)->widgetdata.clazz != 0) {
		if (GTK_IS_CELL_RENDERER_TEXT(cell)) {
			//if (hooks (SWT.PaintItem)) {
			if (wasSelected)
				_W_TREE(widget)->drawState |= W_SELECTED;
			GdkRectangle r2; //cell_area
			rect.x = cell_area->x;
			rect.y = cell_area->y;
			rect.width = cell_area->width;
			rect.height = cell_area->height;
			/*long path = GTK.gtk_tree_model_get_path (modelHandle, iter);
			 GTK.gtk_tree_view_get_background_area (handle, path, columnHandle, rect);
			 GTK.gtk_tree_path_free (path);*/
			// Use the x and width information from the Cairo context. See bug 535124 and 465309.
			if (cr
					!= 0&& GTK_VERSION > VERSION(3, 9, 0) && GTK_VERSION <= VERSION(3, 14, 8)) {
				gdk_cairo_get_clip_rectangle(cr, &r2);
				rect.x = r2.x;
				rect.width = r2.width;
			}
			_W_TREE(widget)->ignoreSize = TRUE;
			int contentX, contentWidth;
			GtkRequisition req;
			gtk_cell_renderer_get_preferred_size(GTK_CELL_RENDERER(cell),
			_W_WIDGET(widget)->handle, &req, NULL);
			contentWidth = req.width;
			gtk_tree_view_column_cell_get_position(columnHandle,
					GTK_CELL_RENDERER(cell), &contentX, NULL);
			_W_TREE(widget)->ignoreSize = FALSE;
			/*Image image = item.getImage (columnIndex);
			 int imageWidth = 0;
			 if (image != null) {
			 Rectangle bounds;
			 if(GTK.GTK3 && DPIUtil.useCairoAutoScale()) {
			 bounds = image.getBounds ();
			 } else {
			 bounds = image.getBoundsInPixels ();
			 }
			 imageWidth = bounds.width;
			 }*/
			// Account for the image width on GTK3, see bug 535124.
#if GTK3
			if (cr != 0) {
				/*rect.x -= imageWidth;
				 rect.width +=imageWidth;*/
			}
#endif
			/*contentX -= imageWidth;
			 contentWidth += imageWidth;*/

			// Account for the expander arrow offset in x position
			if (gtk_tree_view_get_expander_column(
					GTK_TREE_VIEW(_W_WIDGET(widget)->handle)) == columnHandle) {
				/* indent */
				/*GdkRectangle rect3 = new GdkRectangle ();
				 GTK.gtk_widget_realize (handle);
				 path = GTK.gtk_tree_model_get_path (modelHandle, iter);
				 GTK.gtk_tree_view_get_cell_area (handle, path, columnHandle, rect3);
				 contentX[0] += rect3.x;*/
			}
			_w_graphics_init(W_GRAPHICS(&gc), cr);
			if ((_W_TREE(widget)->drawState & W_SELECTED) != 0) {
				/*Color background, foreground;
				 if (GTK.gtk_widget_has_focus (handle) || GTK.GTK3) {
				 background = display.getSystemColor (SWT.COLOR_LIST_SELECTION);
				 foreground = display.getSystemColor (SWT.COLOR_LIST_SELECTION_TEXT);
				 } else {*/
				/*
				 * Feature in GTK. When the widget doesn't have focus, then
				 * gtk_paint_flat_box () changes the background color state_type
				 * to GTK_STATE_ACTIVE. The fix is to use the same values in the GC.
				 */
				/*background = Color.gtk_new (display, display.COLOR_LIST_SELECTION_INACTIVE);
				 foreground = Color.gtk_new (display, display.COLOR_LIST_SELECTION_TEXT_INACTIVE);
				 }
				 gc.setBackground (background);
				 gc.setForeground (foreground);*/
			} else {
				/*gc.setBackground (item.getBackground (columnIndex));
				 Color foreground;
				 if (GTK.GTK3) {
				 foreground = drawForegroundRGBA != null ? Color.gtk_new (display, drawForegroundRGBA) : item.getForeground (columnIndex);
				 } else {
				 foreground = drawForeground != null ? Color.gtk_new (display, drawForeground) : item.getForeground (columnIndex);
				 }
				 gc.setForeground (foreground);*/
			}
			memset(&event, 0, sizeof(event));
			event.event.type = W_EVENT_ITEM_PAINT;
			event.event.widget = widget;
			event.column = W_TREECOLUMN(&cell->column);
			event.item = W_TREEITEM(&cell->item);
			event.gc = W_GRAPHICS(&gc);
			event.rect = &rect;
			if (attr.font != 0) {
				w_graphics_set_font(W_GRAPHICS(&gc), attr.font);
			} else {
				w_font *font;
				w_control_get_font(W_CONTROL(widget), &font);
				w_graphics_set_font(W_GRAPHICS(&gc), font);
			}
			/*if ((style & W_MIRRORED) != 0) {
			 rect.x = getClientWidth() - rect.width - rect.x;
			 }*/
			// Caveat: rect2 is necessary because GC#setClipping(Rectangle) got broken by bug 446075
			//w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &rect);
			rect.x += contentX;
			rect.width = cell_area->width;			//contentWidth;
			event.detail = _W_TREE(widget)->drawState;			//drawState;
			int ret = w_widget_send_event(widget, (w_event*) &event);
			w_graphics_dispose(W_GRAPHICS(&gc));
			//}//if (hooks (PaintItem))
		}
	}

}
#if GTK3
void _w_treecolumn_renderer_get_preferred_width(w_widget *widget,
		GtkCellRenderer *_cell, GtkWidget *gtkwidget, gint *minimum_size,
		gint *natural_size) {
	struct _w_cell_renderer *cell = (struct _w_cell_renderer*) _cell;
	struct _w_cell_renderer_class *clazz =
			(struct _w_cell_renderer_class*) GTK_CELL_RENDERER_GET_CLASS(_cell);
	clazz->default_get_preferred_width(_cell, gtkwidget, minimum_size,
			natural_size);
	if (GTK_IS_CELL_RENDERER_TEXT(cell)) {
		gint contentWidth = *minimum_size, contentHeight = 0;
#if GTK3
		gtk_cell_renderer_get_preferred_height_for_width(_cell, gtkwidget,
				contentWidth, &contentHeight, NULL);
#endif
		int imageWidth = 0;
		if (_W_TABLEBASE(widget)->imagelist != 0
				&& _W_IMAGELIST(_W_TABLEBASE(widget)->imagelist)->images != 0) {
			imageWidth =
			_W_IMAGELIST(_W_TABLEBASE(widget)->imagelist)->images->width;
		}
		contentWidth += imageWidth;
		w_event_tree event;
		w_rect rect;
		w_graphics gc;
		memset(&event, 0, sizeof(event));
		_w_graphics_init(&gc, 0);
		rect.x = 0;
		rect.y = 0;
		rect.width = contentWidth;
		rect.height = contentHeight;
		event.event.type = W_EVENT_ITEM_MEASURE;
		event.event.widget = widget;
		event.column = W_TREECOLUMN(&cell->column);
		event.item = W_TREEITEM(&cell->item);
		event.rect = &rect;
		event.gc = &gc;
		int ret = _w_widget_send_event(widget, (w_event*) &event);
		if (contentHeight < rect.height)
			contentHeight = rect.height;
#if GTK3
		*minimum_size = contentWidth;
		gtk_cell_renderer_set_fixed_size(_cell, contentWidth, contentHeight);
#else
#endif
		return;
	}
}
#else
#endif
void _w_treecolumn_cell_data(GtkTreeViewColumn *tree_column,
		GtkCellRenderer *cell, GtkTreeModel *tree_model, GtkTreeIter *iter,
		gpointer data) {
	struct _w_cell_renderer *c = (struct _w_cell_renderer*) cell;
	_W_TREECOLUMN(&c->column)->tree = W_TREE(data);
	_W_TREECOLUMN(&c->column)->index = (intptr_t) g_object_get_qdata(
			G_OBJECT(tree_column), gtk_toolkit->quark[1]);
	_W_TREECOLUMN(&c->column)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_TREE_CLASS(W_WIDGET_GET_CLASS(W_TREE(data)))->class_treecolumn);

	_W_TREEITEM(&c->item)->tree = W_TREE(data);
	_W_TREEITEM(&c->item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_TREE_CLASS(W_WIDGET_GET_CLASS(W_TREE(data)))->class_treeitem);
	memcpy(&_W_TREEITEM(&c->item)->iter, iter, sizeof(GtkTreeIter));
}
void _w_tree_registre_signal(struct _w_tree_reserved *reserved) {
	if (reserved->signal_changed_id == 0) {
		//signal
		reserved->signal_changed_id = g_signal_lookup("changed",
				gtk_tree_selection_get_type());
		reserved->signal_row_activated_id = g_signal_lookup("row-activated",
				gtk_tree_view_get_type());
		reserved->signal_test_expand_row_id = g_signal_lookup("test-expand-row",
				gtk_tree_view_get_type());
		reserved->signal_test_collapse_row_id = g_signal_lookup(
				"test-collapse-row", gtk_tree_view_get_type());
		reserved->signal_expand_collapse_cursor_row_id = g_signal_lookup(
				"expand-collapse-cursor-row", gtk_tree_view_get_type());
		reserved->signal_row_has_child_toggled_id = g_signal_lookup(
				"row-has-child-toggled", gtk_tree_model_get_type());
		reserved->signal_start_interactive_search_id = g_signal_lookup(
				"start-interactive-search", gtk_tree_view_get_type());
		reserved->signal_row_inserted_id = g_signal_lookup("row-inserted",
				gtk_tree_model_get_type());
		reserved->signal_row_deleted_id = g_signal_lookup("row-deleted",
				gtk_tree_model_get_type());
	}
}
/*
 * tree column
 */
int _w_treecolumn_get_alignment(w_treecolumn *column) {
}
int _w_treecolumn_get_id(w_treecolumn *column) {
	GtkTreeViewColumn *tree_column = gtk_tree_view_get_column(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle),
			_W_TREECOLUMN(column)->index);
	if (tree_column != 0) {
		return gtk_tree_view_column_get_sort_column_id(tree_column);
	} else
		return -1;
}
int _w_treecolumn_get_image(w_treecolumn *column) {
}
wbool _w_treecolumn_get_moveable(w_treecolumn *column) {
}
wbool _w_treecolumn_get_resizable(w_treecolumn *column) {
}
w_string_ref* _w_treecolumn_get_tooltip_text(w_treecolumn *column) {
}
int _w_treecolumn_get_width(w_treecolumn *column) {
}
wresult _w_treecolumn_pack(w_treecolumn *column) {
}
wresult _w_treecolumn_set_alignment(w_treecolumn *column, int alignment) {
	GtkTreeViewColumn *tree_column = gtk_tree_view_get_column(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle),
			_W_TREECOLUMN(column)->index);
	if (tree_column != 0) {
		GtkCellRenderer *renderer = g_object_get_qdata(G_OBJECT(tree_column),
				gtk_toolkit->quark[2]);
		float al;
		if (alignment & W_RIGHT) {
			al = 1;
		} else if (alignment & W_CENTER) {
			al = 0.5;
		} else {
			al = 0;
		}
		if (renderer != 0) {
			g_object_set(G_OBJECT(renderer), "xalign", al, NULL);
		}
		gtk_tree_view_column_set_alignment(tree_column, al);
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_treecolumn_set_id(w_treecolumn *column, int id) {
	GtkTreeViewColumn *tree_column = gtk_tree_view_get_column(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle),
			_W_TREECOLUMN(column)->index);
	if (tree_column != 0) {
		gtk_tree_view_column_set_sort_column_id(tree_column, id);
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_treecolumn_set_image(w_treecolumn *column, int image) {
}
wresult _w_treecolumn_set_moveable(w_treecolumn *column, wbool moveable) {
}
wresult _w_treecolumn_set_resizable(w_treecolumn *column, wbool resizable) {
}
wresult _w_treecolumn_set_tooltip_text(w_treecolumn *column, const char *text) {
}
wresult _w_treecolumn_set_width(w_treecolumn *column, int width) {
}
w_widget* _w_treecolumn_get_parent_widget(w_item *column) {
	return W_WIDGET(_W_TREECOLUMN(column)->tree);
}
void* _w_treecolumn_get_data(w_item *column) {
	GtkTreeViewColumn *tree_column = gtk_tree_view_get_column(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle),
			_W_TREECOLUMN(column)->index);
	if (tree_column != 0) {
		return g_object_get_qdata(G_OBJECT(tree_column), gtk_toolkit->quark[1]);
	} else
		return 0;
}
int _w_treecolumn_get_index(w_item *column) {
	return _W_TREECOLUMN(column)->index;
}
w_string_ref* _w_treecolumn_get_text(w_item *item) {
}
wresult _w_treecolumn_set_data(w_item *column, void *data) {
	GtkTreeViewColumn *tree_column = gtk_tree_view_get_column(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREECOLUMN(column)->tree)->handle),
			_W_TREECOLUMN(column)->index);
	if (tree_column != 0) {
		g_object_set_qdata(G_OBJECT(tree_column), gtk_toolkit->quark[1], data);
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_treecolumn_set_text(w_item *item, const char *string) {
}
/*
 * tree item
 */
wresult _w_treeitem_clear(w_treeitem *item, int index, wbool all) {
}
wresult _w_treeitem_clear_all(w_treeitem *item, wbool all) {
}
void _w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
	// TODO fully test on early and later versions of GTK
	// shifted a bit too far right on later versions of GTK - however, old Tree also had this problem
	/*checkWidget ();
	 if (!parent.checkData (this)) error (SWT.ERROR_WIDGET_DISPOSED);
	 long parentHandle = parent.handle;
	 long column = OS.gtk_tree_view_get_column (parentHandle, 0);
	 if (column == 0) return new Rectangle (0, 0, 0, 0);
	 long textRenderer = parent.getTextRenderer (column);
	 long pixbufRenderer = parent.getPixbufRenderer (column);
	 if (textRenderer == 0 || pixbufRenderer == 0)  return new Rectangle (0, 0, 0, 0);

	 long path = OS.gtk_tree_model_get_path (parent.modelHandle, handle);
	 OS.gtk_widget_realize (parentHandle);

	 boolean isExpander = OS.gtk_tree_model_iter_n_children (parent.modelHandle, handle) > 0;
	 boolean isExpanded = OS.gtk_tree_view_row_expanded (parentHandle, path);
	 OS.gtk_tree_view_column_cell_set_cell_data (column, parent.modelHandle, handle, isExpander, isExpanded);

	 GdkRectangle rect = new GdkRectangle ();
	 OS.gtk_tree_view_get_cell_area (parentHandle, path, column, rect);
	 if ((parent.getStyle () & SWT.MIRRORED) != 0) rect.x = parent.getClientWidth () - rect.width - rect.x;
	 int right = rect.x + rect.width;

	 int [] x = new int [1], w = new int [1];
	 parent.ignoreSize = true;
	 gtk_cell_renderer_get_preferred_size (textRenderer, parentHandle, w, null);
	 parent.ignoreSize = false;
	 rect.width = w [0];
	 int [] buffer = new int [1];
	 OS.gtk_tree_path_free (path);

	 OS.gtk_widget_style_get (parentHandle, OS.horizontal_separator, buffer, 0);
	 int horizontalSeparator = buffer[0];
	 rect.x += horizontalSeparator;

	 OS.gtk_tree_view_column_cell_get_position (column, textRenderer, x, null);
	 rect.x += x [0];
	 if (parent.columnCount > 0) {
	 if (rect.x + rect.width > right) {
	 rect.width = Math.max (0, right - rect.x);
	 }
	 }
	 int width = OS.gtk_tree_view_column_get_visible (column) ? rect.width + 1 : 0;
	 Rectangle r = new Rectangle (rect.x, rect.y, width, rect.height + 1);
	 if (parent.getHeaderVisible() && OS.GTK_VERSION > OS.VERSION(3, 9, 0)) {
	 r.y += parent.getHeaderHeightInPixels();
	 }
	 return r;*/
}
wbool _w_treeitem_get_checked(w_treeitem *item) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle));
	int info = 0;
	wbool ret;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (info & COLUMN_INFO_CHECK) {
		ret = W_TRUE;
	} else {
		ret = W_FALSE;
	}
	return ret;
}
wbool _w_treeitem_get_expanded(w_treeitem *item) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	GtkTreePath *path = gtk_tree_model_get_path(modelHandle,
			&_W_TREEITEM(item)->iter);
	wbool expanded = gtk_tree_view_row_expanded(GTK_TREE_VIEW(handle), path);
	gtk_tree_path_free(path);
	return expanded;
}
wbool _w_treeitem_get_grayed(w_treeitem *item) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle));
	int info = 0;
	wbool ret;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (info & COLUMN_INFO_GRAYED) {
		ret = W_TRUE;
	} else {
		ret = W_FALSE;
	}
	return ret;
}
wbool _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	wuint n = gtk_tree_model_iter_n_children(modelHandle,
			&_W_TREEITEM(item)->iter);
	if (gtk_tree_model_iter_nth_child(modelHandle, &_W_TREEITEM(subitem)->iter,
			&_W_TREEITEM(item)->iter, index)) {
		_W_TREEITEM(subitem)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
		_W_TREEITEM(subitem)->tree = _W_TREEITEM(item)->tree;
		return W_TRUE;
	}
	return W_FALSE;
}
wbool _w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
	return _w_treeitem_get_item(item, 0, child);
}
int _w_treeitem_get_item_count(w_treeitem *item) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	return gtk_tree_model_iter_n_children(modelHandle, &_W_TREEITEM(item)->iter);
}
void _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
}
int _w_treeitem_get_image(w_treeitem *item) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int image = -1;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_IMAGE,
			&image, -1);
	return W_TRUE;
}
wbool _w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	wuint n = gtk_tree_model_iter_n_children(modelHandle,
			&_W_TREEITEM(item)->iter);
	if (n > 0) {
		gtk_tree_model_iter_nth_child(modelHandle, &_W_TREEITEM(child)->iter,
				&_W_TREEITEM(item)->iter, n - 1);
		_W_TREEITEM(child)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
		_W_TREEITEM(child)->tree = _W_TREEITEM(item)->tree;
		return W_TRUE;
	}
	return W_FALSE;
}
wbool _w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	memcpy(&_W_TREEITEM(next)->iter, &_W_TREEITEM(item)->iter,
			sizeof(GtkTreeIter));
	if (gtk_tree_model_iter_next(modelHandle, &_W_TREEITEM(next)->iter)) {
		_W_TREEITEM(next)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
		_W_TREEITEM(next)->tree = _W_TREEITEM(item)->tree;
		return W_TRUE;
	}
	return W_FALSE;
}
wbool _w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	if (gtk_tree_model_iter_parent(modelHandle, &_W_TREEITEM(parent)->iter,
			&_W_TREEITEM(item)->iter)) {
		_W_TREEITEM(parent)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
		_W_TREEITEM(parent)->tree = _W_TREEITEM(item)->tree;
		return W_TRUE;
	} else {
		return W_FALSE;
	}
}
wbool _w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	memcpy(&_W_TREEITEM(prev)->iter, &_W_TREEITEM(item)->iter,
			sizeof(GtkTreeIter));
	if (gtk_tree_model_iter_next(modelHandle, &_W_TREEITEM(prev)->iter)) {
		_W_TREEITEM(prev)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
		_W_TREEITEM(prev)->tree = _W_TREEITEM(item)->tree;
		return W_TRUE;
	}
	return W_FALSE;
}
void _w_treeitem_remove_children_flags(GtkTreeModel *modelHandle,
		GtkTreeIter *parent) {
	int info = 0;
	GtkTreeIter tmp;
	gtk_tree_model_get(modelHandle, parent, COLUMN_INFO, &info, -1);
	if (info & COLUMN_INFO_HAS_CHILDREN) {
		while (gtk_tree_model_iter_children(modelHandle, &tmp, parent)) {
			gtk_tree_store_remove(GTK_TREE_STORE(modelHandle), &tmp);
		}
		info &= ~ COLUMN_INFO_HAS_CHILDREN;
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle), parent, COLUMN_INFO,
				info, -1);
	}
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		const char *text, int index) {
	GtkWidget *_handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(_handle));
	GtkTreeIter *parentIter = 0, *handle, tmp;
	if (subitem != 0) {
		handle = &_W_TREEITEM(subitem)->iter;
	} else {
		handle = &tmp;
	}
	if (_W_TREEITEM(item)->iter.stamp != 0) {
		parentIter = &_W_TREEITEM(item)->iter;
	} else {
		parentIter = 0;
	}
	if (parentIter != 0) {
		_w_treeitem_remove_children_flags(modelHandle, parentIter);
	}
	/*
	 * Feature in GTK.  It is much faster to append to a tree store
	 * than to insert at the end using gtk_tree_store_insert().
	 */
	if (index < 0) {
		gtk_tree_store_append(GTK_TREE_STORE(modelHandle), handle, parentIter);
	} else {
		int count = gtk_tree_model_iter_n_children(modelHandle, parentIter);
		if (index > count) {
			return -W_ERROR_INVALID_RANGE;
		}
		gtk_tree_store_insert(GTK_TREE_STORE(modelHandle), handle, parentIter,
				index);
	}
	/*int id = getId(item.handle, false);
	 items[id] = item;
	 modelChanged = true;
	 */
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), handle, COLUMN_INFO, 0,
			COLUMN_IMAGE, -1, COLUMN_TEXT, text, COLUMN_USER_DATA, 0, -1);
	if (subitem != 0) {
		_W_TREEITEM(subitem)->widgetdata.clazz =
		_W_TREEITEM(item)->widgetdata.clazz;
		_W_TREEITEM(subitem)->tree = _W_TREEITEM(item)->tree;
	}
	return W_TRUE;
}
wresult _w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		const char *text, w_treeitem *after) {
}
wresult _w_treeitem_remove_all(w_treeitem *item) {
	GtkWidget *_handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(_handle));
	GtkTreeIter iter;
	int length = gtk_tree_model_iter_n_children(modelHandle,
			&_W_TREEITEM(item)->iter);
	if (length == 0)
		return W_TRUE;
	/*if (parent.fixAccessibility ()) {
	 parent.ignoreAccessibility = true;
	 }*/
	GtkTreeSelection *selection = gtk_tree_view_get_selection(
			GTK_TREE_VIEW(_handle));
	gint value;
	while (gtk_tree_model_iter_children(modelHandle, &iter,
			&_W_TREEITEM(item)->iter)) {
		/*gtk_tree_model_get(modelHandle, iter, Tree.ID_COLUMN, value, -1);
		 TreeItem item = value[0] != -1 ? parent.items[value[0]] : null;
		 if (item != null && !item.isDisposed()) {
		 item.dispose();
		 } else {
		 g_signal_handlers_block_matched(selection, G_SIGNAL_MATCH_DATA, 0,
		 0, 0, 0, CHANGED);*/
		gtk_tree_store_remove(GTK_TREE_STORE(modelHandle), &iter);
		/*g_signal_handlers_unblock_matched(selection, G_SIGNAL_MATCH_DATA, 0,
		 0, 0, 0, CHANGED);
		 }*/
	}
	/*if (parent.fixAccessibility ()) {
	 parent.ignoreAccessibility = false;
	 OS.g_object_notify (parent.handle, OS.model);
	 }*/
}
wresult _w_treeitem_set_checked(w_treeitem *item, wbool checked) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle));
	int info = 0;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (checked) {
		info |= COLUMN_INFO_CHECK;
	} else {
		info &= ~COLUMN_INFO_CHECK;
	}
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_INFO, info, -1);
	return W_TRUE;
}
wresult _w_treeitem_set_expanded(w_treeitem *item, wbool expanded) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	GtkTreePath *path = gtk_tree_model_get_path(modelHandle,
			&_W_TREEITEM(item)->iter);
	if (expanded != gtk_tree_view_row_expanded(GTK_TREE_VIEW(handle), path)) {
		if (expanded) {
			g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_TEST_EXPAND_ROW);
			gtk_tree_view_expand_row(GTK_TREE_VIEW(handle), path, W_FALSE);
			g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_TEST_EXPAND_ROW);
		} else {
			g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_TEST_COLLAPSE_ROW);
			gtk_widget_realize(handle);
			gtk_tree_view_collapse_row(GTK_TREE_VIEW(handle), path);
			g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_TEST_COLLAPSE_ROW);
		}
	}
	gtk_tree_path_free(path);
	return W_TRUE;
}
wresult _w_treeitem_set_grayed(w_treeitem *item, wbool grayed) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle));
	int info = 0;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
			&info, -1);
	if (grayed) {
		info |= COLUMN_INFO_GRAYED;
	} else {
		info &= ~COLUMN_INFO_GRAYED;
	}
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_INFO, info, -1);
	return W_TRUE;
}
wresult _w_treeitem_set_image(w_treeitem *item, int image) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_IMAGE, image, -1);
	return W_TRUE;
}
wresult _w_treeitem_set_has_children(w_treeitem *item) {
	GtkTreeIter child;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle));
	if (!gtk_tree_model_iter_has_child(modelHandle, &_W_TREEITEM(item)->iter)) {
		int info = 0;
		gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_INFO,
				&info, -1);
		info |= COLUMN_INFO_HAS_CHILDREN;
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle),
				&_W_TREEITEM(item)->iter, COLUMN_INFO, info, -1);
		gtk_tree_store_append(GTK_TREE_STORE(modelHandle), &child,
				&_W_TREEITEM(item)->iter);
	}
	return W_TRUE;
}
wresult _w_treeitem_set_item_count(w_treeitem *item, int count) {
	/*int itemCount = OS.gtk_tree_model_iter_n_children (modelHandle, parentIter);
	 if (count == itemCount) return;
	 boolean isVirtual = (style & SWT.VIRTUAL) != 0;
	 if (!isVirtual) setRedraw (false);
	 remove (parentIter, count, itemCount - 1);
	 if (isVirtual) {
	 if (fixAccessibility ()) {
	 ignoreAccessibility = true;
	 }
	 for (int i=itemCount; i<count; i++) {
	 long iter = OS.g_malloc (OS.GtkTreeIter_sizeof ());
	 if (iter == 0) error (SWT.ERROR_NO_HANDLES);
	 OS.gtk_tree_store_append (modelHandle, iter, parentIter);
	 OS.gtk_tree_store_set (modelHandle, iter, ID_COLUMN, -1, -1);
	 OS.g_free (iter);
	 }
	 if (fixAccessibility ()) {
	 ignoreAccessibility = false;
	 OS.g_object_notify (handle, OS.model);
	 }
	 } else {
	 for (int i=itemCount; i<count; i++) {
	 new TreeItem (this, parentIter, SWT.NONE, i, true);
	 }
	 }
	 if (!isVirtual) setRedraw (true);
	 modelChanged = true;*/
}
w_widget* _w_treeitem_get_parent_widget(w_item *item) {
	return W_WIDGET(_W_TREEITEM(item)->tree);
}
void* _w_treeitem_get_data(w_item *item) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle));
	void *data = 0;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_USER_DATA,
			&data, -1);
	return data;
}
int _w_treeitem_get_index(w_item *item) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	int index = -1;
	GtkTreeIter parent;
	if (gtk_tree_model_iter_parent(modelHandle, &parent,
			&_W_TREEITEM(item)->iter)) {
	}
	return index;
}
wresult _w_treeitem_get_text(w_item *item, w_alloc text,void* user_data) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(_W_TREEITEM(item)->tree)->handle));
	char *t = 0;
	gtk_tree_model_get(modelHandle, &_W_TREEITEM(item)->iter, COLUMN_TEXT, &t,
			-1);
	return w_alloc_set_text(text,user_data, t, -1);
}
wresult _w_treeitem_set_data(w_item *item, void *data) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_USER_DATA, data, -1);
	return W_TRUE;
}
wresult _w_treeitem_set_text(w_item *item, const char *string) {
	GtkWidget *handle = _W_WIDGET(_W_TREEITEM(item)->tree)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(handle));
	gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &_W_TREEITEM(item)->iter,
			COLUMN_TEXT, string, -1);
	return W_TRUE;
}
void _w_treeitem_copy(w_widgetdata *from, w_widgetdata *to) {

}
wbool _w_treeitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {

}
/*
 * tree
 */
wresult _w_tree_deselect(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_deselect_all(w_tree *tree) {
}
int _w_tree_get_gridline_width(w_tree *tree) {
}
int _w_tree_get_header_height(w_tree *tree) {
}
wbool _w_tree_get_header_visible(w_tree *tree) {
}
wbool _w_tree_get_column(w_tree *tree, int index, w_treecolumn *column) {
}
int _w_tree_get_column_count(w_tree *tree) {
}
int _w_tree_get_column_order(w_tree *tree, int *orders, size_t length) {
}
void _w_tree_get_columns(w_tree *tree, w_iterator *columns) {
}
w_imagelist* _w_tree_get_imagelist(w_tree *tree) {
	return _W_TABLEBASE(tree)->imagelist;
}
wbool _w_tree_get_item(w_tree *tree, w_point *point, w_treeitem *item) {
	if (point == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkTreePath *path = 0;
	gtk_widget_realize(_W_WIDGET(tree)->handle);
	int x = point->x;
	int y = point->y;
	/*
	 * On GTK4 the header is included in the entire widget's surface, so we must subtract
	 * its size from the y-coordinate. This does not apply on GTK3 as the header and
	 * "main-widget" have separate GdkWindows.
	 */
#if GTK4
#endif
	if ((_W_WIDGET(tree)->style & W_MIRRORED) != 0) {
		_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(tree);
		x = reserved->get_client_width(W_CONTROL(tree), reserved) - x;
	}
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(tree)->handle);
	GtkTreeViewColumn *columnHandle = 0;
	if (!gtk_tree_view_get_path_at_pos(handle, x, y, &path, &columnHandle, NULL,
	NULL))
		return W_FALSE;
	if (path == 0)
		return W_FALSE;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(handle);
	GtkTreeIter *iter = &_W_TREEITEM(item)->iter;
	if (gtk_tree_model_get_iter(modelHandle, iter, path)) {
		gboolean overExpander = FALSE;
		if (gtk_tree_view_get_expander_column(handle) == columnHandle) {
			GdkRectangle rect;
			gtk_tree_view_get_cell_area(handle, path, columnHandle, &rect);
			if ((_W_WIDGET(tree)->style & W_MIRRORED) != 0) {
				overExpander = x > rect.x + rect.width;
			} else {
				overExpander = x < rect.x;
			}
		}
		if (!overExpander) {
			_W_TREEITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					W_TREE_GET_CLASS(tree)->class_treeitem);
			_W_TREEITEM(item)->tree = tree;
		}
	}
	gtk_tree_path_free(path);
	return W_TRUE;
}
int _w_tree_get_item_height(w_tree *tree) {
}
wbool _w_tree_get_lines_visible(w_tree *tree) {
}
wbool _w_tree_get_parent_item(w_tree *tree, w_treeitem *item) {
}
wbool _w_tree_get_root_item(w_tree *tree, w_treeitem *root) {
	_W_TREEITEM(root)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_TREE_GET_CLASS(tree)->class_treeitem);
	memset(&_W_TREEITEM(root)->iter, 0, sizeof(GtkTreeIter));
	_W_TREEITEM(root)->tree = tree;
	return W_TRUE;
}
void _w_tree_get_selection(w_tree *tree, w_iterator *selection) {
}
int _w_tree_get_selection_count(w_tree *tree) {
	GtkTreeSelection *selection = gtk_tree_view_get_selection(
			GTK_TREE_VIEW(_W_WIDGET(tree)->handle));
	return gtk_tree_selection_count_selected_rows(selection);
}
wbool _w_tree_get_sort_column(w_tree *tree, w_treecolumn *column) {
}
int _w_tree_get_sort_direction(w_tree *tree) {
}
wbool _w_tree_get_top_item(w_tree *tree, w_treeitem *topitem) {
}
wresult _w_tree_insert_column(w_tree *tree, w_treecolumn *column,
		const char *text, int index) {
	if (index == 0)
		return W_FALSE;
	if (text == 0)
		text = "";
	GtkTreeViewColumn *columnHandle;
	GtkCellRenderer *renderer = 0;
	GtkWidget *boxHandle = 0, *labelHandle = 0, *imageHandle;

	columnHandle = gtk_tree_view_column_new();
	if (columnHandle == 0) {
		goto _err;
	}
	//checkRenderer = gtk_cell_renderer_text_new();
	boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	if (boxHandle == 0) {
		goto _err;
	}
	labelHandle = gtk_label_new_with_mnemonic(text);
	if (labelHandle == 0) {
		goto _err;
	}
	imageHandle = gtk_image_new();
	if (imageHandle == 0) {
		goto _err;
	}
	gtk_container_add(GTK_CONTAINER(boxHandle), imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_widget_show_all(boxHandle);
	gtk_tree_view_column_set_widget(columnHandle, boxHandle);
	gtk_tree_view_column_set_resizable(columnHandle, TRUE);
	gtk_tree_view_column_set_clickable(columnHandle, TRUE);
	//gtk_tree_view_column_set_visible(columnHandle, FALSE);
	renderer = _w_text_renderer_new(W_WIDGET(tree));
	if (renderer == 0) {
		goto _err;
	}
	gtk_tree_view_column_pack_start(columnHandle, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(columnHandle, renderer,
			_w_treecolumn_cell_data, tree, 0);
	//gtk_tree_view_column_pack_end(columnHandle, renderer, FALSE);
	g_object_set_qdata(G_OBJECT(renderer), gtk_toolkit->quark[0], tree);
	g_object_set_qdata(G_OBJECT(columnHandle), gtk_toolkit->quark[2], renderer);
	/* Set alignment */
	/*if ((style & swt::RIGHT) != 0) {
	 g_object_set(textRenderer, OS.xalign, 1f, 0);
	 gtk_tree_view_column_pack_end (columnHandle, textRenderer, true);
	 gtk_tree_view_column_pack_end (columnHandle, pixbufRenderer, false);
	 gtk_tree_view_column_set_alignment(columnHandle, 1.0f);
	 } else if ((style & swt::CENTER) != 0) {
	 g_object_set(textRenderer, OS.xalign, 0.5f, 0);
	 gtk_tree_view_column_pack_start (columnHandle, pixbufRenderer, false);
	 gtk_tree_view_column_pack_end (columnHandle, textRenderer, true);
	 gtk_tree_view_column_set_alignment(columnHandle, 0.5f);
	 } else if ((style & swt::LEFT) != 0) {
	 gtk_tree_view_column_pack_start (columnHandle, pixbufRenderer, false);
	 gtk_tree_view_column_pack_start (columnHandle, textRenderer, true);
	 gtk_tree_view_column_set_alignment(columnHandle, 0.0f);
	 }*/
	GtkWidget *_handle = _W_WIDGET(tree)->handle;
	int count;
	if (index >= 0) {
		count = gtk_tree_view_insert_column(GTK_TREE_VIEW(_handle),
				columnHandle, index);
		index = count - 1;
	} else {
		count = gtk_tree_view_append_column(GTK_TREE_VIEW(_handle),
				columnHandle);
	}
	gtk_tree_view_column_set_sort_column_id(columnHandle, index);
	for (int i = index; i < count; i++) {
		GtkTreeViewColumn *_columnHandle = gtk_tree_view_get_column(
				GTK_TREE_VIEW(_W_WIDGET(tree)->handle), i);
		if (_columnHandle != 0)
			g_object_set_qdata(G_OBJECT(_columnHandle), gtk_toolkit->quark[1],
					(gpointer) ((long) i));
	}
	if (column != 0) {
		_W_TREECOLUMN(column)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_CLASS(W_WIDGET_GET_CLASS(tree))->class_treecolumn);
		_W_TREECOLUMN(column)->tree = tree;
		_W_TREECOLUMN(column)->index = (intptr_t) g_object_get_qdata(
				G_OBJECT(columnHandle), gtk_toolkit->quark[1]);
	}
	return 1;
	_err: if (columnHandle)
		g_object_unref(columnHandle);
	return W_ERROR_NO_HANDLES;
}
wresult _w_tree_remove_all(w_tree *tree) {
}
wresult _w_tree_set_imagelist(w_tree *tree, w_imagelist *imagelist) {
	return _w_table_set_imagelist(W_TABLE(tree), imagelist);
}
wresult _w_tree_set_insert_mark(w_tree *tree, w_treeitem *item, wbool before) {
}
wresult _w_tree_set_item_height(w_tree *tree, int itemHeight) {
}
wresult _w_tree_set_lines_visible(w_tree *tree, wbool show) {
}
wresult _w_tree_select(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_select_all(w_tree *tree) {
}
wresult _w_tree_set_column_order(w_tree *tree, int *order, size_t length) {
}
wresult _w_tree_set_header_visible(w_tree *tree, wbool show) {
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(_W_WIDGET(tree)->handle),
			show);
	return W_TRUE;
}
wresult _w_tree_set_selection(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_set_sort_column(w_tree *tree, w_treecolumn *column) {
}
wresult _w_tree_set_sort_direction(w_tree *tree, int direction) {
}
wresult _w_tree_set_top_item(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_show_column(w_tree *tree, w_treecolumn *column) {
}
wresult _w_tree_show_item(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_show_selection(w_tree *tree) {
}
wresult _w_tree_sort(w_tree *tree) {
}
void _w_tree_connect_signal(w_widget *widget,
		struct _w_control_reserved *reserved) {
	_w_composite_hook_events(widget, reserved);
	GtkTreeSelection *selection = gtk_tree_view_get_selection(
			GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
	_w_tree_registre_signal(_W_TREE_RESERVED(reserved));
	_w_widget_connect((GtkWidget*) selection, SIGNAL_CHANGED,
	_W_TREE_RESERVED(reserved)->signal_changed_id, FALSE);
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_ROW_ACTIVATED,
	_W_TREE_RESERVED(reserved)->signal_row_activated_id, FALSE);
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_TEST_EXPAND_ROW,
	_W_TREE_RESERVED(reserved)->signal_test_expand_row_id, FALSE);
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_TEST_COLLAPSE_ROW,
	_W_TREE_RESERVED(reserved)->signal_test_collapse_row_id, FALSE);
	_w_widget_connect(_W_WIDGET(widget)->handle,
			SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW,
			_W_TREE_RESERVED(reserved)->signal_expand_collapse_cursor_row_id,
			FALSE);
	_w_widget_connect((GtkWidget*) modelHandle, SIGNAL_ROW_HAS_CHILD_TOGGLED,
	_W_TREE_RESERVED(reserved)->signal_row_has_child_toggled_id, FALSE);
	_w_widget_connect(_W_WIDGET(widget)->handle,
			SIGNAL_START_INTERACTIVE_SEARCH,
			_W_TREE_RESERVED(reserved)->signal_start_interactive_search_id,
			FALSE);
	//if (fixAccessibility()) {
	_w_widget_connect((GtkWidget*) modelHandle, SIGNAL_ROW_INSERTED,
	_W_TREE_RESERVED(reserved)->signal_row_inserted_id, TRUE);
	_w_widget_connect((GtkWidget*) modelHandle, SIGNAL_ROW_DELETED,
	_W_TREE_RESERVED(reserved)->signal_row_deleted_id, TRUE);
	//}
}
wresult _w_tree_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_table_create_0(widget, parent, style, post_event, W_TRUE);
}
void _w_tree_dispose(w_widget *widget) {

}

gboolean _gtk_tree_button_press_event(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	GdkEventButton *gdkEvent = e->args[0];
	if (gdkEvent->window
			!= gtk_tree_view_get_bin_window(
					GTK_TREE_VIEW(_W_WIDGET(widget)->handle)))
		return FALSE;
	gboolean result = _gtk_composite_button_press_event(widget, e, reserved);
	if (result != 0)
		return result;
	GtkTreeView *handle = GTK_TREE_VIEW(_W_WIDGET(widget)->handle);
	/*
	 * Feature in GTK. In multi-select tree view there is a problem with using DnD operations while also selecting multiple items.
	 * When doing a DnD, GTK de-selects all other items except for the widget being dragged from. By disabling the selection function
	 * in GTK in the case that additional items aren't being added (CTRL_MASK or SHIFT_MASK) and the item being dragged is already
	 * selected, we can give the DnD handling to MOTION-NOTIFY. Seee Bug 503431
	 */
	if ((_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0 && !isX11()
			&& gdkEvent->type == GDK_BUTTON_PRESS) { // Wayland
			// check to see if there is another event coming in that is not a double/triple click, this is to prevent Bug 514531
		GdkEvent *nextEvent = gdk_event_peek();
		if (nextEvent == 0) {
			GtkTreePath *path = 0;
			GtkTreeSelection *selection = gtk_tree_view_get_selection(handle);
			if (gtk_tree_view_get_path_at_pos(handle, gdkEvent->x, gdkEvent->y,
					&path, NULL, NULL, NULL) && path != 0) {
				//  selection count is used in the case of clicking an already selected item while holding Control
				/*selectionCountOnPress = _w_tree_get_selection_count(
				 W_TREE(widget));*/
				if (gtk_tree_selection_path_is_selected(selection, path)) {
					if (((gdkEvent->state & (GDK_CONTROL_MASK | GDK_SHIFT_MASK))
							== 0)
							|| ((gdkEvent->state & GDK_CONTROL_MASK) != 0)) {
						/**
						 * Disable selection on a mouse click if there are multiple items already selected. Also,
						 * if control is currently being held down, we will designate the selection logic over to release
						 * instead by first disabling the selection.
						 * E.g to reproduce: Open DNDExample, select "Tree", select multiple items, try dragging.
						 *   without line below, only one item is selected for drag.
						 */
						/*GtkTreeSelectionFunc gtk_false_funcPtr =
						 GTK.GET_FUNCTION_POINTER_gtk_false();
						 gtk_tree_selection_set_select_function(selection,
						 gtk_false_funcPtr, 0, 0);*/
					}
				}
			}
		} else {
			gdk_event_free(nextEvent);
		}
	}
	/*
	 * Feature in GTK.  In a multi-select tree view, when multiple items are already
	 * selected, the selection state of the item is toggled and the previous selection
	 * is cleared. This is not the desired behaviour when bringing up a popup menu
	 * Also, when an item is reselected with the right button, the tree view issues
	 * an unwanted selection event. The workaround is to detect that case and not
	 * run the default handler when the item is already part of the current selection.
	 */
	int button = gdkEvent->button;
	if (button == 3 && gdkEvent->type == GDK_BUTTON_PRESS) {
		GtkTreePath *path = 0;
		if (gtk_tree_view_get_path_at_pos(handle, gdkEvent->x, gdkEvent->y,
				&path, NULL, NULL, NULL)) {
			if (path != 0) {
				GtkTreeSelection *selection = gtk_tree_view_get_selection(
						handle);
				if (gtk_tree_selection_path_is_selected(selection, path))
					result = TRUE;
				gtk_tree_path_free(path);
			}
		}
	}

	/*
	 * Feature in GTK.  When the user clicks in a single selection GtkTreeView
	 * and there are no selected items, the first item is selected automatically
	 * before the click is processed, causing two selection events.  The is fix
	 * is the set the cursor item to be same as the clicked item to stop the
	 * widget from automatically selecting the first item.
	 */
	if ((_W_WIDGET(widget)->style & W_SINGLE) != 0
			&& _w_tree_get_selection_count(W_TREE(widget)) == 0) {
		GtkTreePath *path = 0;
		if (gtk_tree_view_get_path_at_pos(handle, gdkEvent->x, gdkEvent->y,
				&path, NULL, NULL, NULL)) {
			if (path != 0) {
				GtkTreeSelection *selection = gtk_tree_view_get_selection(
						handle);
				g_signal_handlers_block_matched(selection, G_SIGNAL_MATCH_DATA,
						0, 0, 0, 0, (void*) SIGNAL_CHANGED);
				gtk_tree_view_set_cursor(handle, path, 0, FALSE);
				g_signal_handlers_unblock_matched(selection,
						G_SIGNAL_MATCH_DATA, 0, 0, 0, 0,
						(void*) SIGNAL_CHANGED);
				gtk_tree_path_free(path);
			}
		}
	}

	/*
	 * Bug 312568: If mouse double-click pressed, manually send a DefaultSelection.
	 * Bug 518414: Added rowActivated guard flag to only send a DefaultSelection when the
	 * double-click triggers a 'row-activated' signal. Note that this relies on the fact
	 * that 'row-activated' signal comes before double-click event. This prevents
	 * opening of the current highlighted item when double clicking on any expander arrow.
	 */
	if (gdkEvent->type == GDK_2BUTTON_PRESS && _W_TREE(widget)->rowActivated) {
		//sendTreeDefaultSelection();
		_W_TREE(widget)->rowActivated = FALSE;
	}

	return result;
}

gboolean _gtk_tree_row_activated(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	//rowActivated = true;
	return FALSE;
}

gboolean _gtk_tree_key_press_event(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*GdkEventKey keyEvent = new GdkEventKey ();
	 OS.memmove (keyEvent, event, GdkEventKey.sizeof);
	 int key = keyEvent.keyval;
	 keyPressDefaultSelectionHandler (event, key);
	 return super.gtk_key_press_event (widget, event);*/
	return FALSE;
}

gboolean _gtk_tree_button_release_event(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*GdkEventButton gdkEvent = new GdkEventButton ();
	 OS.memmove (gdkEvent, event, GdkEventButton.sizeof);
	 // Check region since super.gtk_button_release_event() isn't called
	 lastInput.x = (int) gdkEvent.x;
	 lastInput.y = (int) gdkEvent.y;
	 if (containedInRegion(lastInput.x, lastInput.y)) return 0;
	 if (gdkEvent.window != GTK.gtk_tree_view_get_bin_window (handle)) return 0;*/
	/*
	 * Feature in GTK. In multi-select tree view there is a problem with using DnD operations while also selecting multiple items.
	 * When doing a DnD, GTK de-selects all other items except for the widget being dragged from. By disabling the selection function
	 * in GTK in the case that additional items aren't being added (CTRL_MASK or SHIFT_MASK) and the item being dragged is already
	 * selected, we can give the DnD handling to MOTION-NOTIFY. On release, we can then re-enable the selection method
	 * and also select the item in the tree by moving the selection logic to release instead. See Bug 503431.
	 */
	/*if ((state & DRAG_DETECT) != 0 && hooks (SWT.DragDetect) && !OS.isX11()) { //Wayland
	 long [] path = new long [1];
	 long selection = GTK.gtk_tree_view_get_selection (handle);
	 // free up the selection function on release.
	 GTK.gtk_tree_selection_set_select_function(selection,0,0,0);
	 if (GTK.gtk_tree_view_get_path_at_pos (handle, (int)gdkEvent.x, (int)gdkEvent.y, path, null, null, null) &&
	 path[0] != 0 && GTK.gtk_tree_selection_path_is_selected (selection, path[0])) {
	 selectionCountOnRelease = getSelectionCount();
	 if ((gdkEvent.state & (GDK.GDK_CONTROL_MASK|GDK.GDK_SHIFT_MASK)) == 0) {
	 GTK.gtk_tree_view_set_cursor(handle, path[0], 0,  false);
	 }
	 // Check to see if there has been a new tree item selected when holding Control in Path.
	 // If not, deselect the item.
	 if ((gdkEvent.state & GDK.GDK_CONTROL_MASK) != 0 && selectionCountOnRelease == selectionCountOnPress) {
	 GTK.gtk_tree_selection_unselect_path (selection,path[0]);
	 }
	 }
	 }
	 return super.gtk_button_release_event (widget, event);*/
	return FALSE;
}

gboolean _gtk_tree_changed(struct w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	GtkTreePath *path = 0;
	w_treeitem item;
	w_event_tree event;
	gtk_tree_view_get_cursor(GTK_TREE_VIEW(_W_WIDGET(widget)->handle), &path,
			0);
	if (path != 0) {
		GtkTreeModel *modelHandle = gtk_tree_view_get_model(
				GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
		memset(&event, 0, sizeof(event));
		event.event.type = W_EVENT_ITEM_SELECTION;
		event.event.widget = widget;
		event.item = &item;
		_W_TREEITEM(&item)->widgetdata.clazz =
		W_TREE_GET_CLASS(widget)->class_treeitem;
		_W_TREEITEM(&item)->tree = W_TREE(widget);
		gtk_tree_model_get_iter(modelHandle, &_W_TREEITEM(&item)->iter, path);
		_w_widget_send_event(widget, (w_event*) &event);
		gtk_tree_path_free(path);
	}
	return FALSE;
}

gboolean _gtk_tree_event_after(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*switch (GDK.GDK_EVENT_TYPE (gdkEvent)) {
	 case GDK.GDK_EXPOSE: {*/
	/*
	 * Bug in GTK. SWT connects the expose-event 'after' the default
	 * handler of the signal. If the tree has no children, then GTK
	 * sends expose signal only 'before' the default signal handler.
	 * The fix is to detect this case in 'event_after' and send the
	 * expose event.
	 */
	/*if (GTK.gtk_tree_model_iter_n_children (modelHandle, 0) == 0) {
	 gtk_expose_event (widget, gdkEvent);
	 }
	 break;
	 }
	 }
	 return super.gtk_event_after (widget, gdkEvent);*/
	return FALSE;
}

gboolean _gtk_tree_expand_collapse_cursor_row(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	// FIXME - this flag is never cleared.  It should be cleared when the expand all operation completes.
	/*if (expand != 0 && open_all != 0) expandAll = true;
	 return 0;*/
	return FALSE;
}

gboolean _gtk_tree_draw(struct w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
#if USE_CAIRO
	/*boolean haveBoundsChanged = boundsChangedSinceLastDraw;
	 boundsChangedSinceLastDraw = false;
	 if ((state & OBSCURED) != 0) return 0;*/
	/*
	 * Bug 537960: JFace tree viewers miss a repaint when resized by a SashForm
	 *
	 * If a listener of type SWT.MeasureItem, SWT.PaintItem and or SWT.EraseItem is registered,
	 * GTK will sometimes not invalidate the tree widget pixel cache when the tree is resized.
	 * As a result, a few of the bottom rows of JFace tree viewers that use styled text are often not drawn on resize.
	 * If the tree was resized since the last paint, we ignore this draw request
	 * and queue another draw request so that the pixel cache is properly invalidated.
	 */
	/*if (GTK.GTK_VERSION >= OS.VERSION(3, 14, 0) && ownerDraw && haveBoundsChanged) {
	 GTK.gtk_widget_queue_draw(handle);
	 return 0;
	 }
	 drawInheritedBackground	(0, cairo);
	 return super.gtk_draw (widget, cairo);*/
#else
	/*if ((state & OBSCURED) != 0) return 0;
	drawInheritedBackground	(eventPtr, 0);
	return super.gtk_expose_event (widget, eventPtr);*/
#endif
	return FALSE;
}

gboolean _gtk_tree_motion_notify_event(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*long window = GDK.GDK_EVENT_WINDOW (event);
	 if (window != GTK.gtk_tree_view_get_bin_window (handle)) return 0;
	 return super.gtk_motion_notify_event (widget, event);*/
	return FALSE;
}

gboolean _gtk_tree_row_deleted(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*if (ignoreAccessibility) {
	 OS.g_signal_stop_emission_by_name (model, OS.row_deleted);
	 }
	 return 0;*/
	return FALSE;
}

gboolean _gtk_tree_row_has_child_toggled(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*
	 * Feature in GTK. The expanded state of a row that lost
	 * its children is not persisted by GTK. So, the row
	 * doesn't exhibit the expanded state after obtaining the
	 * children. The fix is to preserve the expanded state
	 * and use this callback, as it is invoked when a row has
	 * gotten the first child row or lost its last child row.
	 */
	/*int [] index = new int [1];
	 GTK.gtk_tree_model_get (modelHandle, iter, ID_COLUMN, index, -1);
	 if (index [0] >= items.length) return 0;
	 TreeItem item = items [index [0]];
	 if (item == null) return 0;
	 int childCount = GTK.gtk_tree_model_iter_n_children (modelHandle, item.handle);
	 if (childCount != 0 && item.isExpanded) {
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEST_EXPAND_ROW);
	 GTK.gtk_tree_view_expand_row (handle, path, false);
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEST_EXPAND_ROW);
	 }
	 return 0;*/
	return FALSE;
}

gboolean _gtk_tree_row_inserted(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*if (ignoreAccessibility) {
	 OS.g_signal_stop_emission_by_name (model, OS.row_inserted);
	 }
	 return 0;*/
	return FALSE;
}

gboolean _gtk_tree_start_interactive_search(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*if (!searchEnabled()) {
	 OS.g_signal_stop_emission_by_name(widget, OS.start_interactive_search);
	 return 1;
	 }
	 return 0;*/
	return FALSE;
}

gboolean _gtk_tree_test_collapse_row(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*int [] index = new int [1];
	 GTK.gtk_tree_model_get (modelHandle, iter, ID_COLUMN, index, -1);
	 TreeItem item = items [index [0]];
	 Event event = new Event ();
	 event.item = item;
	 boolean oldModelChanged = modelChanged;
	 modelChanged = false;
	 sendEvent (SWT.Collapse, event);*/
	/*
	 * Bug in GTK.  Collapsing the target row during the test_collapse_row
	 * handler will cause a segmentation fault if the animation code is allowed
	 * to run.  The fix is to block the animation if the row is already
	 * collapsed.
	 */
	/*boolean changed = modelChanged || !GTK.gtk_tree_view_row_expanded (handle, path);
	 modelChanged = oldModelChanged;
	 if (isDisposed () || item.isDisposed ()) return 1;
	 item.isExpanded = false;*/
	/*
	 * Bug in GTK.  Expanding or collapsing a row which has no more
	 * children causes the model state to become invalid, causing
	 * GTK to give warnings and behave strangely.  Other changes to
	 * the model can cause expansion to fail when using the multiple
	 * expansion keys (such as *).  The fix is to stop the expansion
	 * if there are model changes.
	 *
	 * Note: This callback must return 0 for the collapsing
	 * animation to occur.
	 */
	/*if (changed) {
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEST_COLLAPSE_ROW);
	 GTK.gtk_tree_view_collapse_row (handle, path);
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEST_COLLAPSE_ROW);
	 return 1;
	 }
	 return 0;*/
	return FALSE;
}

gboolean _gtk_tree_test_expand_row(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
	_w_treeitem_remove_children_flags(modelHandle, (GtkTreeIter*) e->args[0]);
	gint index;
	//gtk_tree_model_get (modelHandle, e->args[0], ID_COLUMN, index, -1);
	w_treeitem item;
	w_event_tree event;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_ITEM_EXPAND;
	event.event.widget = widget;
	event.item = &item;
	_W_TREEITEM(&item)->widgetdata.clazz =
	W_TREE_GET_CLASS(widget)->class_treeitem;
	_W_TREEITEM(&item)->tree = W_TREE(widget);
	memcpy(&_W_TREEITEM(&item)->iter, e->args[0], sizeof(GtkTreeIter));
	//boolean oldModelChanged = modelChanged;
	//modelChanged = false;
	_w_widget_send_event(widget, (w_event*) &event);
	//sendEvent (SWT.Expand, event);
	/*
	 * Bug in GTK.  Expanding the target row during the test_expand_row
	 * handler will cause a segmentation fault if the animation code is allowed
	 * to run.  The fix is to block the animation if the row is already
	 * expanded.
	 */
	/*boolean changed = modelChanged || GTK.gtk_tree_view_row_expanded (handle, path);
	 modelChanged = oldModelChanged;
	 if (isDisposed () || item.isDisposed ()) return 1;
	 item.isExpanded = true;*/
	/*
	 * Bug in GTK.  Expanding or collapsing a row which has no more
	 * children causes the model state to become invalid, causing
	 * GTK to give warnings and behave strangely.  Other changes to
	 * the model can cause expansion to fail when using the multiple
	 * expansion keys (such as *).  The fix is to stop the expansion
	 * if there are model changes.
	 *
	 * Bug in GTK.  test-expand-row does not get called for each row
	 * in an expand all operation.  The fix is to block the initial
	 * expansion and only expand a single level.
	 *
	 * Note: This callback must return 0 for the collapsing
	 * animation to occur.
	 */
	/*if (changed || expandAll) {
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEST_EXPAND_ROW);
	 GTK.gtk_tree_view_expand_row (handle, path, false);
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEST_EXPAND_ROW);
	 return 1;
	 }*/
	return FALSE;
}

gboolean _gtk_tree_toggled(struct w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
	_w_treeitem item;
	if (gtk_tree_model_get_iter_from_string(modelHandle, &item.iter,
			(char*) e->args[0])) {
		item.widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TREE_GET_CLASS(widget)->class_treeitem);
		item.tree = W_TREE(widget);
		GtkTreeModel *modelHandle = gtk_tree_view_get_model(
				GTK_TREE_VIEW(_W_WIDGET(widget)->handle));
		int info = 0;
		gtk_tree_model_get(modelHandle, &item.iter, COLUMN_INFO, &info, -1);
		if (info & COLUMN_INFO_CHECK) {
			info &= ~COLUMN_INFO_CHECK;
		} else {
			info |= COLUMN_INFO_CHECK;
		}
		gtk_tree_store_set(GTK_TREE_STORE(modelHandle), &item.iter, COLUMN_INFO,
				info, -1);
		/*Event event = new Event();
		 event.detail = SWT.CHECK;
		 event.item = item;
		 sendSelectionEvent(SWT.Selection, event, false);*/
	}
	return FALSE;
}

void _gtk_tree_widget_size_request(GtkWidget *widget,
		GtkRequisition *requisition) {
	/*
	 * Bug in GTK.  For some reason, gtk_widget_size_request() fails
	 * to include the height of the tree view items when there are
	 * no columns visible.  The fix is to temporarily make one column
	 * visible.
	 */
	/*if (columnCount == 0) {
	 super.gtk_widget_size_request (widget, requisition);
	 return;
	 }
	 long columns = GTK.gtk_tree_view_get_columns (handle), list = columns;
	 boolean fixVisible = columns != 0;
	 while (list != 0) {
	 long column = OS.g_list_data (list);
	 if (GTK.gtk_tree_view_column_get_visible (column)) {
	 fixVisible = false;
	 break;
	 }
	 list = OS.g_list_next (list);
	 }
	 long columnHandle = 0;
	 if (fixVisible) {
	 columnHandle = OS.g_list_data (columns);
	 GTK.gtk_tree_view_column_set_visible (columnHandle, true);
	 }
	 super.gtk_widget_size_request (widget, requisition);
	 if (fixVisible) {
	 GTK.gtk_tree_view_column_set_visible (columnHandle, false);
	 }
	 if (columns != 0) OS.g_list_free (columns);*/
}

void _w_tree_class_init(struct _w_tree_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));

	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TREE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tree_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tree);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tree);
	if (sizeof(_w_tree) > sizeof(w_tree)) {
		fprintf(stderr, "Warning : sizeof(_w_tree) > sizeof(w_tree)");
	}
	W_WIDGET_CLASS(clazz)->create = _w_tree_create;
	W_WIDGET_CLASS(clazz)->dispose = _w_tree_dispose;

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
	treecolumn->item.widgetdata.toolkit = gtk_toolkit;
	treecolumn->get_alignment = _w_treecolumn_get_alignment;
	treecolumn->get_id = _w_treecolumn_get_id;
	treecolumn->get_image = _w_treecolumn_get_image;
	treecolumn->get_moveable = _w_treecolumn_get_moveable;
	treecolumn->get_resizable = _w_treecolumn_get_resizable;
	treecolumn->get_tooltip_text = _w_treecolumn_get_tooltip_text;
	treecolumn->get_width = _w_treecolumn_get_width;
	treecolumn->pack = _w_treecolumn_pack;
	treecolumn->set_alignment = _w_treecolumn_set_alignment;
	treecolumn->set_id = _w_treecolumn_set_id;
	treecolumn->set_image = _w_treecolumn_set_image;
	treecolumn->set_moveable = _w_treecolumn_set_moveable;
	treecolumn->set_resizable = _w_treecolumn_set_resizable;
	treecolumn->set_tooltip_text = _w_treecolumn_set_tooltip_text;
	treecolumn->set_width = _w_treecolumn_set_width;
	treecolumn->item.widgetdata.close = _w_widgetdata_close;
	treecolumn->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	//treecolumn->item.widgetdata.copy = _w_treeitem_copy;
	//treecolumn->item.widgetdata.equals = _w_treeitem_equals;
	treecolumn->item.get_parent_widget = _w_treecolumn_get_parent_widget;
	treecolumn->item.get_data = _w_treecolumn_get_data;
	treecolumn->item.get_index = _w_treecolumn_get_index;
	treecolumn->item.get_text = _w_treecolumn_get_text;
	treecolumn->item.set_data = _w_treecolumn_set_data;
	treecolumn->item.set_text = _w_treecolumn_set_text;
	/*
	 * tree item
	 */
	struct _w_treeitem_class *treeitem = clazz->class_treeitem;
	treeitem->item.widgetdata.toolkit = gtk_toolkit;
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
	treeitem->set_item_count = _w_treeitem_set_item_count;
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
	/*
	 * reserved
	 */
	struct _w_tree_reserved *reserved = _W_TREE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);

	reserved->column.renderer_render = _w_treecolumn_renderer_render;
#if GTK3
	reserved->column.renderer_get_preferred_width =
			_w_treecolumn_renderer_get_preferred_width;
#else
	reserved->column.rendererGetSizeProc = _w_treecolumn_rendererGetSizeProc;
#endif
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hpp;
	_W_CONTROL_RESERVED(reserved)->fixedHandle = _w_widget_hpp;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle = _w_widget_hp;
	//signal
	_gtk_signal *signals = _W_WIDGET_RESERVED(reserved)->signals;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_tree_button_press_event;
	signals[SIGNAL_ROW_ACTIVATED] = _gtk_tree_row_activated;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_tree_key_press_event;
	signals[SIGNAL_BUTTON_RELEASE_EVENT] = _gtk_tree_button_release_event;
	signals[SIGNAL_CHANGED] = _gtk_tree_changed;
	signals[SIGNAL_EVENT_AFTER] = _gtk_tree_event_after;
	signals[SIGNAL_EXPAND_COLLAPSE_CURSOR_ROW] =
			_gtk_tree_expand_collapse_cursor_row;
	signals[SIGNAL_DRAW] = _gtk_tree_draw;
	signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_tree_motion_notify_event;
	signals[SIGNAL_ROW_DELETED] = _gtk_tree_row_deleted;
	signals[SIGNAL_ROW_HAS_CHILD_TOGGLED] = _gtk_tree_row_has_child_toggled;
	signals[SIGNAL_ROW_INSERTED] = _gtk_tree_row_inserted;
	signals[SIGNAL_START_INTERACTIVE_SEARCH] =
			_gtk_tree_start_interactive_search;
	signals[SIGNAL_TEST_COLLAPSE_ROW] = _gtk_tree_test_collapse_row;
	signals[SIGNAL_TEST_EXPAND_ROW] = _gtk_tree_test_expand_row;
	signals[SIGNAL_TOGGLED] = _gtk_tree_toggled;
}
#endif
