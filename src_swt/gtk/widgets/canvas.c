/*
 * canvas.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "canvas.h"
#include "toolkit.h"
/*
 * caret
 */
wresult _w_caret_blink_caret(w_caret *caret) {
	if (!_W_CARET(caret)->isVisible)
		return W_TRUE;
	if (!_W_CARET(caret)->isShowing)
		return _w_caret_show_caret(caret);
	if (_W_CARET(caret)->blinkRate == 0)
		return W_TRUE;
	return _w_caret_hide_caret(caret);
}

wresult _w_caret_draw_caret(w_caret *caret) {
	if (!w_widget_is_ok(W_WIDGET(_W_CARET(caret)->parent)))
		return W_FALSE;
	w_canvas *parent = _W_CARET(caret)->parent;
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(parent);
	GdkWindow *window = reserved->paintWindow(W_WIDGET(parent),
			_W_WIDGET_RESERVED(reserved));
#if USE_CAIRO
	cairo_t *cairo = gdk_cairo_create(window);
	if (cairo == 0)
		return W_ERROR_NO_HANDLES;
	cairo_set_source_rgb(cairo, 1, 1, 1);
	cairo_set_operator(cairo, CAIRO_OPERATOR_DIFFERENCE);
	if (w_image_is_ok(&_W_CARET(caret)->image)) {
		cairo_surface_t *surface = cairo_get_target(cairo);
		int nWidth = 0;
		cairo_surface_type_t surface_type = cairo_surface_get_type(surface);
		if (surface_type == CAIRO_SURFACE_TYPE_IMAGE) {
			nWidth = cairo_image_surface_get_width(surface);
		}
#if CAIRO_HAS_XLIB_SURFACE
		/*if (isX11()) {
		 if (surface_type == CAIRO_SURFACE_TYPE_XLIB) {
		 nWidth = cairo_xlib_surface_get_width(surface);
		 }
		 }*/
#endif
		int nX = _W_CARET(caret)->bounds.x;
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0)
			nX = reserved->get_client_width(W_CONTROL(parent), reserved)
					- nWidth - nX;
		cairo_translate(cairo, nX, _W_CARET(caret)->bounds.y);
		gdk_cairo_set_source_pixbuf(cairo,
		_W_IMAGE(&_W_CARET(caret)->image)->pixbuf, 0, 0);
		cairo_paint(cairo);
	} else {
		int nWidth = _W_CARET(caret)->bounds.width, nHeight =
		_W_CARET(caret)->bounds.height;
		if (nWidth <= 0)
			nWidth = DEFAULT_WIDTH;
		int nX = _W_CARET(caret)->bounds.x;
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0)
			nX = reserved->get_client_width(W_CONTROL(parent), reserved)
					- nWidth - nX;
		cairo_rectangle(cairo, nX, _W_CARET(caret)->bounds.y, nWidth, nHeight);
	}
	cairo_fill(cairo);
	cairo_destroy(cairo);
	return W_TRUE;
#else
#endif
}
wresult _w_caret_get_bounds(w_caret *caret,w_point *location,w_size *size) {
	if (w_image_is_ok(&_W_CARET(caret)->image)) {
		if (size != 0) {
			w_image_get_size(&_W_CARET(caret)->image, size);
		}
		if (location != 0) {
			location->x = _W_CARET(caret)->bounds.x;
			location->y = _W_CARET(caret)->bounds.y;
		}
		return W_TRUE;
	} else {
		if (size != 0) {
			size->width = _W_CARET(caret)->bounds.width;
			size->height = _W_CARET(caret)->bounds.height;
			if (size->width == 0)
				size->width = 1;
		}
		if (location != 0) {
			location->x = _W_CARET(caret)->bounds.x;
			location->y = _W_CARET(caret)->bounds.y;
		}
		return W_TRUE;
	}
}
wresult _w_caret_get_font(w_caret *caret, w_font **font) {
	if (_W_CARET(caret)->font != 0) {
		*font = _W_CARET(caret)->font;
		return W_TRUE;
	}
	return w_control_get_font(W_CONTROL(_W_CARET(caret)->parent), font);
}
wresult _w_caret_get_image(w_caret *caret, w_image *image) {
	return w_image_copy(&_W_CARET(caret)->image, 0, image);
}
w_canvas* _w_caret_get_parent(w_caret *caret) {
	return _W_CARET(caret)->parent;
}
wresult _w_caret_get_visible(w_caret *caret) {
	return _W_CARET(caret)->isVisible;
}

wresult _w_caret_hide_caret(w_caret *caret) {
	if (!_W_CARET(caret)->isShowing)
		return W_TRUE;
	_W_CARET(caret)->isShowing = W_FALSE;
	return _w_caret_draw_caret(caret);
}
wresult _w_caret_is_visible(w_caret *caret) {
	if (_W_CARET(caret)->isVisible) {
		if (w_control_is_visible(W_CONTROL(_W_CARET(caret)->parent))) {
			if (W_CONTROL(_W_CARET(caret)->parent)
					== _w_toolkit_get_focus_control(W_TOOLKIT(gtk_toolkit))) {
				return W_TRUE;
			}
		}

	}
	return W_FALSE;
}

wresult _w_caret_is_focus_caret(w_caret *caret) {
	return caret == gtk_toolkit->currentCaret;
}
gboolean _w_caret_proc(gpointer clientData) {
	gtk_toolkit->caretId = 0;
	if (gtk_toolkit->currentCaret == 0) {
		return FALSE;
	}
	if (_w_caret_blink_caret(gtk_toolkit->currentCaret)) {
		int blinkRate = _W_CARET(gtk_toolkit->currentCaret)->blinkRate;
		if (blinkRate == 0)
			return 0;
		gtk_toolkit->caretId = g_timeout_add(blinkRate, _w_caret_proc, 0);
	} else {
		gtk_toolkit->currentCaret = 0;
	}
	return FALSE;
}
void _w_caret_set_current_caret(w_caret *caret) {
	if (gtk_toolkit->caretId != 0)
		g_source_remove(gtk_toolkit->caretId);
	gtk_toolkit->caretId = 0;
	gtk_toolkit->currentCaret = caret;
	if (caret == 0)
		return;
	int blinkRate = _W_CARET(gtk_toolkit->currentCaret)->blinkRate;
	gtk_toolkit->caretId = g_timeout_add(blinkRate, _w_caret_proc, 0);
}
void _w_caret_kill_focus(w_caret *caret) {
	if (gtk_toolkit->currentCaret != caret)
		return;
	_w_caret_set_current_caret(0);
	if (_W_CARET(caret)->isVisible)
		_w_caret_hide_caret(caret);
}
void _w_caret_copy(w_widgetdata *from, w_widgetdata *to) {
	w_widgetdata_close(to);
}
wresult _w_caret_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	return obj1 == obj2;
}
void _w_caret_close(w_widgetdata *widgetdata) {
	w_canvas *parent = _W_CARET(widgetdata)->parent;
	w_caret* caret;
	w_canvas_get_caret(parent, &caret);
	if (W_CARET(widgetdata) == caret)
		w_canvas_set_caret(parent, W_CARET(widgetdata));
	if (gtk_toolkit->currentCaret == W_CARET(widgetdata)) {
		_w_caret_hide_caret(W_CARET(widgetdata));
		_w_caret_set_current_caret(0);
	}
	_W_CARET(widgetdata)->parent = 0;
	w_image_dispose(&_W_CARET(widgetdata)->image);
	_W_CARET(widgetdata)->clazz = 0;
}
wresult _w_caret_set_bounds(w_caret *caret,w_point *location,w_size *size) {
	int is_equal = W_TRUE;
	if (location != 0) {
		if (_W_CARET(caret)->bounds.x != location->x ||
		_W_CARET(caret)->bounds.y != location->y)
			is_equal = W_FALSE;
	}
	if (size != 0) {
		if (_W_CARET(caret)->bounds.width != size->width ||
		_W_CARET(caret)->bounds.height != size->height)
			is_equal = W_FALSE;
	}
	if (is_equal)
		return W_TRUE;
	int isFocus = _w_caret_is_focus_caret(caret);
	if (isFocus && _W_CARET(caret)->isVisible)
		_w_caret_hide_caret(caret);
	if (location != 0) {
		_W_CARET(caret)->bounds.x = location->x;
		_W_CARET(caret)->bounds.y = location->y;
	}
	if (size != 0) {
		_W_CARET(caret)->bounds.width = size->width;
		_W_CARET(caret)->bounds.height = size->height;
	}
	_w_canvas_update_caret(_W_CARET(caret)->parent);
	if (isFocus && _W_CARET(caret)->isVisible)
		_w_caret_show_caret(caret);
	return W_TRUE;
}

void _w_caret_set_focus(w_caret *caret) {
	if (gtk_toolkit->currentCaret == caret)
		return;
	_w_caret_set_current_caret(caret);
	if (_W_CARET(caret)->isVisible)
		_w_caret_show_caret(caret);
}
wresult _w_caret_set_font(w_caret *caret, w_font *font) {
	if (font != 0 && !w_font_is_ok(font)) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	_W_CARET(caret)->font = font;
	return W_TRUE;
}
wresult _w_caret_set_image(w_caret *caret, w_image *image) {
	if (image != 0 && !w_image_is_ok(image)) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	int isFocus = _w_caret_is_focus_caret(caret);
	if (isFocus && _W_CARET(caret)->isVisible)
		_w_caret_hide_caret(caret);
	wresult result = w_image_copy(image, 0, &_W_CARET(caret)->image);
	if (isFocus && _W_CARET(caret)->isVisible)
		_w_caret_show_caret(caret);
	return result;
}
wresult _w_caret_set_visible(w_caret *caret, int visible) {
	if (visible == _W_CARET(caret)->isVisible)
		return W_TRUE;
	_W_CARET(caret)->isVisible = visible;
	if (!_w_caret_is_focus_caret(caret))
		return W_TRUE;
	if (_W_CARET(caret)->isVisible) {
		_w_caret_show_caret(caret);
	} else {
		_w_caret_hide_caret(caret);
	}
	return W_TRUE;
}

wresult _w_caret_show_caret(w_caret *caret) {
	if (_W_CARET(caret)->isShowing)
		return W_TRUE;
	_W_CARET(caret)->isShowing = W_TRUE;
	return _w_caret_draw_caret(caret);
}
int _w_caret_get_caret_blink_time() {
	GtkSettings *settings = gtk_settings_get_default();
	if (settings == 0)
		return 500;
	int buffer;
	g_object_get(settings, "gtk-cursor-blink", &buffer, NULL);
	if (buffer == 0)
		return 0;
	g_object_get(settings, "gtk-cursor-blink-time", &buffer, NULL);
	if (buffer == 0)
		return 500;
	/*
	 * By experimentation, GTK application don't use the whole
	 * blink cycle time.  Instead, they divide up the time, using
	 * an effective blink rate of about 1/2 the total time.
	 */
	return buffer / 2;
}
/*
 * IME
 */
int _w_ime_get_caret_offset(w_ime *ime) {
	return _W_IME(ime)->startOffset + _W_IME(ime)->caretOffset;
}
int _w_ime_get_commit_count(w_ime *ime) {
	return _W_IME(ime)->commitCount;
}
int _w_ime_get_composition_offset(w_ime *ime) {
	return _W_IME(ime)->startOffset;
}
size_t _w_ime_get_ranges(w_ime *ime, w_range *ranges, w_textstyle *styles,
		size_t _length) {
	int length = strlen(_W_IME(ime)->preeditString);
	char *chars = _W_IME(ime)->preeditString;
	PangoAttrList *pangoAttrs = _W_IME(ime)->pangoAttrs;
	if (pangoAttrs != 0) {
		int count = 0;
		PangoAttrIterator *iterator = pango_attr_list_get_iterator(pangoAttrs);
		while (pango_attr_iterator_next(iterator))
			count++;
		pango_attr_iterator_destroy(iterator);
		if (styles != 0 || ranges != 0) {
			iterator = pango_attr_list_get_iterator(pangoAttrs);
			PangoAttrColor *attrColor;
			PangoAttrInt *attrInt;
			int start = 0;
			int end = 0;
			size_t ll = w_min(count, _length);
			for (int i = 0; i < ll; i++) {
				pango_attr_iterator_range(iterator, &start, &end);
				if (ranges != 0) {
					/*ranges[i].begin = g_utf16_pointer_to_offset(preeditString,
					 preeditString + start);
					 ranges[i].end = g_utf16_pointer_to_offset(preeditString,
					 preeditString + end) - 1;*/
				}
				if (styles != 0) {
					PangoAttribute *attr = pango_attr_iterator_get(iterator,
							PANGO_ATTR_FOREGROUND);
					if (attr != 0) {
						attrColor = (PangoAttrColor*) attr;
						styles[i].foreground = W_RGB(
								attrColor->color.red / 0x100,
								attrColor->color.green / 0x100,
								attrColor->color.blue / 0x100);
					}
					attr = pango_attr_iterator_get(iterator,
							PANGO_ATTR_BACKGROUND);
					if (attr != 0) {
						attrColor = (PangoAttrColor*) attr;
						styles[i].background = W_RGB(
								attrColor->color.red / 0x100,
								attrColor->color.green / 0x100,
								attrColor->color.blue / 0x100);
					}
					attr = pango_attr_iterator_get(iterator,
							PANGO_ATTR_UNDERLINE);
					if (attr != 0) {
						attrInt = (PangoAttrInt*) attr;
						styles[i].underline = attrInt->value
								!= PANGO_UNDERLINE_NONE;
						styles[i].underlineStyle = W_UNDERLINE_SINGLE;
						switch (attrInt->value) {
						case PANGO_UNDERLINE_DOUBLE:
							styles[i].underlineStyle = W_UNDERLINE_DOUBLE;
							break;
						case PANGO_UNDERLINE_ERROR:
							styles[i].underlineStyle = W_UNDERLINE_ERROR;
							break;
						}
						if (styles[i].underline) {
							attr = pango_attr_iterator_get(iterator,
									PANGO_ATTR_UNDERLINE_COLOR);
							if (attr != 0) {
								attrColor = (PangoAttrColor*) attr;
								styles[i].underlineColor = W_RGB(
										attrColor->color.red / 0x100,
										attrColor->color.green / 0x100,
										attrColor->color.blue / 0x100);
							}
						}
					}
				}
				pango_attr_iterator_next(iterator);
			}
			pango_attr_iterator_destroy(iterator);
			//pango_attr_list_unref(pangoAttrs);
		}
		return count;
	}
	return 0;
}
wresult _w_ime_get_text(w_ime *ime, w_alloc text, void *user_data) {
	return w_alloc_set_text(text, user_data, _W_IME(ime)->text, -1);
}
wresult _w_ime_get_wide_caret(w_ime *ime) {
	return W_FALSE;
}
void _w_ime_copy(w_widgetdata *from, w_widgetdata *to) {
	w_widgetdata_close(to);
}
wresult _w_ime_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	return obj1 == obj2;
}
void _w_ime_close(w_widgetdata *widgetdata) {
	w_canvas *parent = _W_IME(widgetdata)->parent;
	w_ime* ime;
	w_canvas_get_ime(parent,&ime);
	if (W_IME(widgetdata) == ime)
		w_canvas_set_ime(parent, 0);
	_W_IME(widgetdata)->parent = 0;
	_W_IME(widgetdata)->text = 0;
	_W_IME(widgetdata)->pangoAttrs = 0;
	_W_CARET(widgetdata)->clazz = 0;
}
wresult _w_ime_set_composition_offset(w_ime *ime, int offset) {
	if (offset < 0)
		return W_FALSE;
	if (_W_IME(ime)->startOffset != -1) {
		_W_IME(ime)->startOffset = offset;
	}
	return W_TRUE;
}
/*
 * canvas
 */
w_caret* _w_canvas_get_caret(w_canvas *canvas) {
	return _W_CANVAS(canvas)->caret;

}
wresult _w_canvas_create_caret(w_canvas *canvas, w_caret *caret) {
	memset(caret, 0, sizeof(_w_caret));
	_W_CARET(caret)->clazz = &gtk_toolkit->class_caret;
	_W_CARET(caret)->blinkRate = _w_caret_get_caret_blink_time();
	_W_CARET(caret)->isVisible = W_TRUE;
	_W_CARET(caret)->parent = canvas;
	if (_W_CANVAS(canvas)->caret == 0) {
		_w_canvas_set_caret(canvas, caret);
	}
	return W_TRUE;
}
wresult _w_canvas_create_ime(w_canvas *canvas, w_ime *ime) {
	memset(ime, 0, sizeof(_w_ime));
	_W_IME(ime)->text = "";
	_W_IME(ime)->startOffset = -1;
	if (_W_CANVAS(canvas)->ime == 0) {
		_w_canvas_set_ime(canvas, ime);
	}
	return W_TRUE;
}
wresult _w_canvas_get_imcaret_pos(w_control *control, w_point *pos,
		_w_control_reserved *reserved) {
	if (_W_CANVAS(control)->caret == 0)
		return _w_control_get_imcaret_pos(control, pos, reserved);
	w_caret *caret = _W_CANVAS(control)->caret;
	pos->x = _W_CARET(caret)->bounds.x;
	pos->y = _W_CARET(caret)->bounds.y;
	return W_TRUE;
}
void _w_canvas_redraw_widget(w_control *control, w_rect *rect, int flags,
		_w_control_reserved *reserved) {
	w_caret *caret = _W_CANVAS(control)->caret;
	int isFocus = caret != 0 && _w_caret_is_focus_caret(caret);
	if (isFocus)
		_w_caret_kill_focus(caret);
	_w_scrollable_redraw_widget(control, rect, flags, reserved);
	if (isFocus)
		_w_caret_set_focus(caret);
}
wresult _w_canvas_scroll(w_canvas *canvas, w_point *_dest, w_rect *_rect,
		int all) {
	w_point dest;
	w_rect rect, tmp;
	if (_dest == 0)
		memset(&dest, 0, sizeof(w_point));
	else
		memcpy(&dest, _dest, sizeof(w_point));
	if (_rect == 0)
		memset(&rect, 0, sizeof(w_rect));
	else
		memcpy(&rect, _rect, sizeof(w_rect));
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(canvas);
	if ((_W_WIDGET(canvas)->style & W_MIRRORED) != 0) {
		int clientWidth = reserved->get_client_width(W_CONTROL(reserved),
				reserved);
		rect.x = clientWidth - rect.width - rect.x;
		dest.x = clientWidth - rect.width - dest.x;
	}
	int deltaX = dest.x - rect.x, deltaY = dest.y - rect.y;
	if (deltaX == 0 && deltaY == 0)
		return W_TRUE;
	if (W_CONTROL_GET_CLASS(canvas)->is_visible(W_CONTROL(canvas)) < 0)
		return W_FALSE;
	w_caret *caret = _W_CANVAS(canvas)->caret;
	int isFocus = caret != 0 && _w_caret_is_focus_caret(caret);
	if (isFocus)
		_w_caret_kill_focus(caret);
	GdkWindow *window = reserved->paintWindow(W_WIDGET(canvas),
			_W_WIDGET_RESERVED(reserved));
	cairo_region_t *visibleRegion;
#if GTK3
	visibleRegion = gdk_window_get_visible_region(window);
#else
		visibleRegion = gdk_drawable_get_visible_region(window);
#endif
	GdkRectangle srcRect;
	srcRect.x = rect.x;
	srcRect.y = rect.y;
	/*
	 * Feature in GTK: for 3.16+ the "visible" region in Canvas includes
	 * the scrollbar dimensions in its calculations. This means the "previous"
	 * location the scrollbars are re-painted when scrolling, causing the
	 * hopping effect. See bug 480458.
	 */
	if (GTK_VERSION >= VERSION(3, 16, 0)) {
		GtkWidget *hBarHandle = 0;
		GtkWidget *vBarHandle = 0;
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(canvas),
				_W_WIDGET_RESERVED(reserved));
		if (GTK_IS_SCROLLED_WINDOW(scrolledHandle)) {
			hBarHandle = gtk_scrolled_window_get_hscrollbar(
					GTK_SCROLLED_WINDOW(scrolledHandle));
			vBarHandle = gtk_scrolled_window_get_vscrollbar(
					GTK_SCROLLED_WINDOW(scrolledHandle));
		}
		GtkRequisition requisition;
		if (hBarHandle != 0) {
			_gtk_widget_get_preferred_size(hBarHandle, &requisition);
			if (requisition.height > 0) {
				srcRect.y = rect.y - requisition.height;
			}
		}
		if (vBarHandle != 0) {
			_gtk_widget_get_preferred_size(vBarHandle, &requisition);
			if (requisition.width > 0) {
				srcRect.x = rect.x - requisition.width;
			}
		}
	}
	srcRect.width = rect.width;
	srcRect.height = rect.height;

	cairo_region_t *copyRegion = cairo_region_create_rectangle(&srcRect);
	cairo_region_intersect(copyRegion, visibleRegion);
	cairo_region_t *invalidateRegion = cairo_region_create_rectangle(&srcRect);
	cairo_region_subtract(invalidateRegion, visibleRegion);
	cairo_region_translate(invalidateRegion, deltaX, deltaY);
	GdkRectangle copyRect;
	cairo_region_get_extents(copyRegion, &copyRect);
	if (copyRect.width != 0 && copyRect.height != 0) {
		_w_control_update(W_CONTROL(canvas));
	}
	w_control *control = reserved->find_background_control(W_CONTROL(canvas),
			reserved);
	if (control == 0)
		control = W_CONTROL(canvas);
	if (_W_CONTROL(control)->backgroundImage != 0) {
		reserved->redraw_widget(W_CONTROL(canvas), &rect, 0, reserved);
		tmp.x = dest.x;
		tmp.y = dest.y;
		tmp.width = rect.width;
		tmp.height = rect.height;
		reserved->redraw_widget(W_CONTROL(canvas), &tmp, 0, reserved);
	} else {
#if GTK3
#if H_GDK_VERSION > VERSION(3,22,0)
		GdkDrawingContext *context = gdk_window_begin_draw_frame(window, 0);
		cairo_t *cairo = gdk_drawing_context_get_cairo_context(context);
#else
		cairo_t *cairo = gdk_cairo_create(window);
#endif
		if (cairo_version() < CAIRO_VERSION_ENCODE(1, 12, 0)) {
			gdk_cairo_set_source_window(cairo, window, 0, 0);
		} else {
			cairo_push_group(cairo);
			gdk_cairo_set_source_window(cairo, window, 0, 0);
			cairo_paint(cairo);
			cairo_pop_group_to_source(cairo);
		}
		cairo_matrix_t matrix;
		matrix.xx = 1;
		matrix.yx = 0;
		matrix.xy = 0;
		matrix.yy = 1;
		matrix.x0 = -deltaX;
		matrix.y0 = -deltaY;
		cairo_pattern_set_matrix(cairo_get_source(cairo), &matrix);
		cairo_rectangle(cairo, copyRect.x + deltaX, copyRect.y + deltaY,
				copyRect.width, copyRect.height);
		cairo_clip(cairo);
		cairo_paint(cairo);
#if H_GDK_VERSION > VERSION(3,22,0)
		gdk_window_end_draw_frame(window, context);
#else
		cairo_destroy(cairo);
#endif
#else
#endif
		int disjoint = (dest.x + rect.width < rect.x)
				|| (rect.x + rect.width < dest.x)
				|| (dest.y + rect.height < rect.y)
				|| (rect.y + rect.height < dest.y);
		if (disjoint) {
			GdkRectangle __rect;
			__rect.x = rect.x;
			__rect.y = rect.y;
			__rect.width = rect.width;
			__rect.height = rect.height;
			cairo_region_union_rectangle(invalidateRegion, &__rect);
		} else {
			GdkRectangle __rect;
			if (deltaX != 0) {
				int newX = dest.x - deltaX;
				if (deltaX < 0)
					newX = dest.x + rect.width;
				__rect.x = newX;
				__rect.y = rect.y;
				__rect.width = abs(deltaX);
				__rect.height = rect.height;
				cairo_region_union_rectangle(invalidateRegion, &__rect);
			}
			if (deltaY != 0) {
				int newY = dest.y - deltaY;
				if (deltaY < 0)
					newY = dest.y + rect.height;
				__rect.x = rect.x;
				__rect.y = newY;
				__rect.width = rect.width;
				__rect.height = abs(deltaY);
				cairo_region_union_rectangle(invalidateRegion, &__rect);
			}
		}
		gdk_window_invalidate_region(window, invalidateRegion, all);
	}
	cairo_region_destroy(visibleRegion);
	cairo_region_destroy(copyRegion);
	cairo_region_destroy(invalidateRegion);
	if (all) {
		struct _w_composite_reserved *reserved = _W_COMPOSITE_GET_RESERVED(
				canvas);
		_w_fixed *fixed = (_w_fixed*) reserved->parenting_handle(
				W_WIDGET(canvas), _W_WIDGET_RESERVED(reserved));
		_w_fixed *i = fixed->first;
		while (i != 0) {
			w_control *child = (w_control*) g_object_get_qdata(G_OBJECT(i),
					gtk_toolkit->quark[0]);
			if (child != 0) {
				w_rect _r, _r2;
				w_control_get_bounds(child, &_r.pt, &_r.sz);
				if (w_min(rect.x + rect.width, _r.x + _r.width)
						>= w_max(rect.x, _r.x)
						&& w_min(rect.y + rect.height, _r.y + _r.height)
								>= w_max(rect.y, _r.y)) {
					_r2.x = _r.x + deltaX;
					_r2.y = _r.y + deltaY;
					w_control_set_bounds(child, &_r2.pt, 0);
				}
			}
			i = i->next;
		}
	}
	if (isFocus)
		_w_caret_set_focus(caret);
	/*
	 * Due to overlay drawing of scrollbars current method of scrolling leaves scrollbar and notifiers for them inside the canvas
	 * after scroll. Fix is to redraw once done.
	 */
	if (GTK_VERSION >= VERSION(3, 16, 0)) {
		W_CONTROL_GET_CLASS(canvas)->redraw(W_CONTROL(canvas), 0, 0);
	}
	return W_TRUE;
}

wresult _w_canvas_set_bounds(w_control *control,w_point *location,w_size *size,
		_w_control_reserved *reserved) {
	w_caret *caret = _W_CANVAS(control)->caret;
	int isFocus = caret != 0 && _w_caret_is_focus_caret(caret);
	if (isFocus)
		_w_caret_kill_focus(caret);
	wresult result = _w_composite_set_bounds_0(control, location, size, reserved);
	if (isFocus)
		_w_caret_set_focus(caret);
	return result;
}
void _w_canvas_set_foreground_color(w_control *control, GtkWidget *handle,
		GdkRGBA *rgba, _w_control_reserved *reserved) {
	/* For CSS foreground colors (GTK3.16+) we need to make sure the default
	 * background color stays set if no background color is specified. If no
	 * background color has been set, set cssBackground to be COLOR_WIDGET_BACKGROUND.
	 */
	/*if (cssBackground == null) {
	 GdkRGBA defaultBackground = display.toGdkRGBA(
	 display.COLOR_WIDGET_BACKGROUND);
	 cssBackground = "SwtFixed {background-color: "
	 + display.gtk_rgba_to_css_string(defaultBackground) + ";}";
	 }*/
	_w_control_set_foreground_color_1(control, handle, rgba, reserved);
}
wresult _w_canvas_set_caret(w_canvas *canvas, w_caret *caret) {
	w_caret *newCaret = caret;
	w_caret *oldCaret = _W_CANVAS(canvas)->caret;
	_W_CANVAS(canvas)->caret = newCaret;
	if (W_CONTROL(canvas)
			== _w_toolkit_get_focus_control(W_TOOLKIT(gtk_toolkit))) {
		if (oldCaret != 0)
			_w_caret_kill_focus(oldCaret);
		if (newCaret != 0) {
			if (!w_widgetdata_is_ok(W_WIDGETDATA(newCaret)))
				return W_ERROR_INVALID_ARGUMENT;
			_w_caret_set_focus(newCaret);
		}
	}
	return W_TRUE;
}
wresult _w_canvas_set_ime(w_canvas *canvas, w_ime *ime) {
	if (ime != 0 && !w_widgetdata_is_ok(W_WIDGETDATA(ime)))
		return W_ERROR_INVALID_ARGUMENT;
	_W_CANVAS(canvas)->ime = ime;
	return W_TRUE;
}
w_ime* _w_canvas_get_ime(w_canvas *canvas) {
	return _W_CANVAS(canvas)->ime;
}
wresult _w_canvas_set_font(w_control *control, w_font *font) {
	w_caret *caret = _W_CANVAS(control)->caret;
	if (caret != 0)
		_w_caret_set_font(caret, font);
	return _w_control_set_font(control, font);
}

void _w_canvas_update_caret(w_canvas *canvas) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(canvas);
	GtkIMContext *imHandle = reserved->imHandle(W_WIDGET(canvas),
			_W_WIDGET_RESERVED(reserved));
	if (imHandle == 0)
		return;
	w_caret *caret = _W_CANVAS(canvas)->caret;
	GdkRectangle rect;
	rect.x = _W_CARET(caret)->bounds.x;
	rect.y = _W_CARET(caret)->bounds.y;
	rect.width = _W_CARET(caret)->bounds.width;
	rect.height = _W_CARET(caret)->bounds.height;
	gtk_im_context_set_cursor_location(imHandle, &rect);
}
/*
 * signals
 */

gboolean _gtk_ime_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
			reserved);
	if (imHandle != 0)
		gtk_im_context_reset(imHandle);
	return 0;
}

gboolean _gtk_ime_commit(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	w_ime *ime = _W_CANVAS(widget)->ime;
	int doit = W_TRUE;
	_W_IME(ime)->pangoAttrs = 0;
	_W_IME(ime)->caretOffset = _W_IME(ime)->commitCount = 0;
	char *textPtr = (char*) e->args[0];
	if (textPtr != 0 && _W_IME(ime)->inComposition) {
		int length = strlen(textPtr);
		if (length != 0) {
			w_event_ime event;
			event.event.type = W_EVENT_IMECOMPOSITION;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = 0;
			event.event.data = 0;
			event.detail = W_COMPOSITION_CHANGED;
			event.range.start = _W_IME(ime)->startOffset;
			event.range.end = _W_IME(ime)->startOffset + length;
			event.text = _W_IME(ime)->text = textPtr;
			_W_IME(ime)->commitCount = length;
			doit = w_widget_send_event(widget, (w_event*) &event);
			_W_IME(ime)->text = "";
			_W_IME(ime)->startOffset = -1;
			_W_IME(ime)->commitCount = 0;
		}
	}
	_W_IME(ime)->inComposition = W_FALSE;
	return doit ? 0 : 1;
}

gboolean _gtk_ime_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	w_event_ime event;
	w_ime *ime = _W_CANVAS(widget)->ime;
	_W_IME(ime)->pangoAttrs = 0;
	_W_IME(ime)->commitCount = 0;
	GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
			reserved);
	gchar *preeditString = 0;
	PangoAttrList *pangoAttrs = 0;
	gint cursorPos = 0;
	gtk_im_context_get_preedit_string(imHandle, &preeditString, &pangoAttrs,
			&cursorPos);
	_W_IME(ime)->caretOffset = cursorPos;
	char *chars = 0;
	size_t text_length = 0;
	size_t chars_length = 0;
	if (preeditString != 0) {
		_W_IME(ime)->pangoAttrs = pangoAttrs;
		_W_IME(ime)->preeditString = preeditString;
		chars = preeditString;
		chars_length = strlen(chars);
	}
	if (chars != 0) {
		if (text_length == 0) {
			/*
			 * Bug in GTK. In Solaris, the IME sends multiple
			 * preedit_changed signals with an empty text.
			 * This behavior is not correct for SWT and can
			 * cause the editor to replace its current selection
			 * with an empty string. The fix is to ignore any
			 * preedit_changed signals with an empty text when
			 * the preedit buffer is already empty.
			 */
			if (chars_length == 0)
				return 0;
			_W_IME(ime)->startOffset = -1;
		}
		int end = _W_IME(ime)->startOffset + text_length;
		if (_W_IME(ime)->startOffset == -1) {
			event.event.type = W_EVENT_IMECOMPOSITION;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = 0;
			event.event.data = 0;
			event.detail = W_COMPOSITION_SELECTION;
			event.range.start = 0;
			event.range.end = 0;
			event.text = 0;
			w_widget_send_event(widget, (w_event*) &event);
			_W_IME(ime)->startOffset = event.range.start;
			end = event.range.end;
		}
		_W_IME(ime)->inComposition = W_TRUE;

		event.event.type = W_EVENT_IMECOMPOSITION;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = 0;
		event.event.data = 0;
		event.detail = W_COMPOSITION_CHANGED;
		event.range.start = _W_IME(ime)->startOffset;
		event.range.end = end;
		event.text = chars;
		w_widget_send_event(widget, (w_event*) &event);
	}
	return 1;
}
gboolean _gtk_canvas_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (_W_CANVAS(widget)->ime != 0) {
		gboolean result = _gtk_ime_button_press_event(widget, e, reserved);
		if (result != 0)
			return result;
	}
	return _gtk_composite_button_press_event(widget, e, reserved);
}

gboolean _gtk_canvas_commit(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (_W_CANVAS(widget)->ime != 0) {
		gboolean result = _gtk_ime_commit(widget, e, reserved);
		if (result != 0)
			return result;
	}
	return _gtk_control_commit(widget, e, reserved);
}

gboolean _gtk_canvas_draw(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->state & STATE_OBSCURED) != 0)
		return FALSE;
	int isFocus = w_widgetdata_is_ok(W_WIDGETDATA(_W_CANVAS(widget)->caret))
			&& _w_caret_is_focus_caret(_W_CANVAS(widget)->caret);
	if (isFocus)
		_w_caret_kill_focus(_W_CANVAS(widget)->caret);
	gboolean result = _gtk_composite_draw(widget, e, reserved);
	if (isFocus)
		_w_caret_set_focus(_W_CANVAS(widget)->caret);
	return result;
}

gboolean _gtk_canvas_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_composite_focus_in_event(widget, e, reserved);
	if (w_widgetdata_is_ok(W_WIDGETDATA(_W_CANVAS(widget)->caret)))
		_w_caret_set_focus(_W_CANVAS(widget)->caret);
	return result;
}

gboolean _gtk_canvas_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_composite_focus_out_event(widget, e, reserved);
	if (w_widgetdata_is_ok(W_WIDGETDATA(_W_CANVAS(widget)->caret)))
		_w_caret_kill_focus(_W_CANVAS(widget)->caret);
	return result;
}

gboolean _gtk_canvas_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (_W_CANVAS(widget)->ime != 0) {
		gboolean result = _gtk_ime_preedit_changed(widget, e, reserved);
		if (result != 0)
			return result;
	}
	return _gtk_control_preedit_changed(widget, e, reserved);
}
void _w_canvas_class_init(struct _w_canvas_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_canvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_canvas);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_canvas);
	/**
	 *
	 */
	W_CONTROL_CLASS(clazz)->set_font = _w_canvas_set_font;
	clazz->create_caret = _w_canvas_create_caret;
	clazz->create_ime = _w_canvas_create_ime;
	clazz->scroll = _w_canvas_scroll;
	clazz->get_caret = _w_canvas_get_caret;
	clazz->set_caret = _w_canvas_set_caret;
	clazz->get_ime = _w_canvas_get_ime;
	clazz->set_ime = _w_canvas_set_ime;
	/*
	 * caret class
	 */
	if (gtk_toolkit->class_caret.widgetdata.toolkit == 0) {
		struct _w_caret_class *caret = &gtk_toolkit->class_caret;
		caret->widgetdata.toolkit = W_TOOLKIT(gtk_toolkit);
		caret->widgetdata.is_ok = _w_widgetdata_is_ok;
		caret->widgetdata.close = _w_caret_close;
		caret->widgetdata.copy = _w_caret_copy;
		caret->widgetdata.equals = _w_caret_equals;
		caret->get_bounds = _w_caret_get_bounds;
		caret->get_font = _w_caret_get_font;
		caret->get_image = _w_caret_get_image;
		caret->get_parent = _w_caret_get_parent;
		caret->get_visible = _w_caret_get_visible;
		caret->is_visible = _w_caret_is_visible;
		caret->set_bounds = _w_caret_set_bounds;
		caret->set_font = _w_caret_set_font;
		caret->set_image = _w_caret_set_image;
		caret->set_visible = _w_caret_set_visible;

	}
	/*
	 * IME class
	 */
	if (gtk_toolkit->class_ime.widgetdata.toolkit == 0) {
		struct _w_ime_class *ime = &gtk_toolkit->class_ime;
		ime->widgetdata.toolkit = W_TOOLKIT(gtk_toolkit);
		ime->widgetdata.is_ok = _w_widgetdata_is_ok;
		ime->widgetdata.close = _w_ime_close;
		ime->widgetdata.copy = _w_ime_copy;
		ime->widgetdata.equals = _w_ime_equals;
		ime->get_caret_offset = _w_ime_get_caret_offset;
		ime->get_commit_count = _w_ime_get_commit_count;
		ime->get_composition_offset = _w_ime_get_composition_offset;
		ime->get_ranges = _w_ime_get_ranges;
		ime->get_text = _w_ime_get_text;
		ime->get_wide_caret = _w_ime_get_wide_caret;
		ime->set_composition_offset = _w_ime_set_composition_offset;

	}
	/*
	 * reserved
	 */
	_w_canvas_reserved *reserved = _W_CANVAS_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	//_W_WIDGET_RESERVED(reserved)->hook_events = _w_canvas_hook_events;
	_W_CONTROL_RESERVED(reserved)->get_imcaret_pos = _w_canvas_get_imcaret_pos;
	_W_CONTROL_RESERVED(reserved)->redraw_widget = _w_canvas_redraw_widget;
	_W_CONTROL_RESERVED(reserved)->set_bounds = _w_canvas_set_bounds;
	_W_CONTROL_RESERVED(reserved)->set_foreground_color =
			_w_canvas_set_foreground_color;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_RESERVED(reserved)->signals;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_canvas_button_press_event;
	signals[SIGNAL_COMMIT] = _gtk_canvas_commit;
	signals[SIGNAL_DRAW] = _gtk_canvas_draw;
	signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_canvas_focus_in_event;
	signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_canvas_focus_out_event;
	signals[SIGNAL_PREEDIT_CHANGED] = _gtk_canvas_preedit_changed;
}
#endif

