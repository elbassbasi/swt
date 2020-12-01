/*
 * composite.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "composite.h"
#include "toolkit.h"

GtkWidget* _w_composite_scrolledHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	int scrolled = (_W_WIDGET(control)->style & (W_HSCROLL | W_VSCROLL)) != 0;
	if (scrolled || (_W_WIDGET(control)->style & W_BORDER) != 0) {
		return gtk_widget_get_parent(_W_WIDGET(control)->handle);
	} else
		return 0;
}

GtkWidget* _w_composite_topHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED( reserved)->scrolledHandle(control, reserved);
	if (scrolledHandle == 0)
		return _W_WIDGET(control)->handle;
	else
		return gtk_widget_get_parent(scrolledHandle);
}
wuint64 _w_composite_check_style(w_widget *control, wuint64 style) {
	/*if (INIT_CAIRO) {
	 style &= ~W_NO_BACKGROUND;
	 }*/
	style &= ~W_TRANSPARENT;
	return style;
}
_w_fixed* _w_composite_iterator_find_next(_w_fixed *fixed, w_control **control,
		int tablist) {
	w_control *c;
	if (tablist) {
		_w_fixed *i = fixed;
		while (i != 0) {
			c = g_object_get_qdata(G_OBJECT(i), gtk_toolkit->quark[0]);
			if (c == 0) {
				*control = 0;
				return 0;
			}
			if (_W_WIDGET(c)->state & STATE_TAB_LIST) {
				*control = c;
				return i->next;
			}
			i = i->next;
		}
		return 0;
	} else {
		*control = g_object_get_qdata(G_OBJECT(fixed), gtk_toolkit->quark[0]);
		if (*control == 0) {
			return 0;
		}
		return fixed->next;
	}
}
wresult _w_composite_iterator_next(w_iterator *it, void *obj) {
	if (_W_COMPOSITE_ITERATOR(it)->i != 0) {
		_W_COMPOSITE_ITERATOR(it)->i = _w_composite_iterator_find_next(
		_W_COMPOSITE_ITERATOR(it)->i, (w_control**) obj,
		_W_COMPOSITE_ITERATOR(it)->tablist);
		if (*((w_control**) obj) == 0)
			return W_FALSE;
		return W_TRUE;
	} else {
		*((w_control**) obj) = 0;
		return W_FALSE;
	}
}
wresult _w_composite_iterator_reset(w_iterator *it) {
	_W_COMPOSITE_ITERATOR(it)->i = _W_COMPOSITE_ITERATOR(it)->first;
	return W_TRUE;
}
wresult _w_composite_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_composite_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_composite_get_count(w_iterator *it) {
	return _W_COMPOSITE_ITERATOR(it)->count;
}
_w_iterator_class _w_composite_iterator_class = { //
		_w_composite_iterator_close, //
				_w_composite_iterator_next, //
				_w_composite_iterator_reset, //
				_w_composite_iterator_remove, //
				_w_composite_get_count };
wresult _w_composite_get_children(w_composite *composite, w_iterator *it) {
	it->base.clazz = &_w_composite_iterator_class;
	_w_composite_reserved *reserved = _W_COMPOSITE_GET_RESERVED(composite);
	_w_fixed *fixed = (_w_fixed*) reserved->parenting_handle(
			W_WIDGET(composite), _W_WIDGET_RESERVED(reserved));
	_W_COMPOSITE_ITERATOR(it)->composite = composite;
	_W_COMPOSITE_ITERATOR(it)->first = fixed->first;
	_W_COMPOSITE_ITERATOR(it)->i = fixed->first;
	_W_COMPOSITE_ITERATOR(it)->count = fixed->count;
	_W_COMPOSITE_ITERATOR(it)->tablist = 0;
	return W_TRUE;
}

wresult _w_composite_changed(w_composite *_this, w_control **changed,
		size_t length) {
	if (changed == 0)
		return W_ERROR_INVALID_ARGUMENT;
	for (int i = 0; i < length; i++) {
		w_control *control = changed[i];
		if (control == 0)
			return W_ERROR_INVALID_ARGUMENT;
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_ERROR_INVALID_ARGUMENT;
		int ancestor = W_FALSE;
		w_composite *composite = _W_CONTROL(control)->parent;
		while (composite != 0) {
			ancestor = composite == _this;
			if (ancestor)
				break;
			composite = _W_CONTROL(composite)->parent;
		}
		if (!ancestor)
			return W_ERROR_INVALID_PARENT;
	}
	for (int i = 0; i < length; i++) {
		w_control *child = changed[i];
		w_composite *composite = _W_CONTROL(child)->parent;
		while (child != W_CONTROL(_this)) {
			/*if (composite.layout == null || !composite.layout.flushCache (child)) {
			 _W_WIDGET(composite)->state |= STATE_LAYOUT_CHANGED;
			 }*/
			child = W_CONTROL(composite);
			composite = _W_CONTROL(child)->parent;
		}
	}
	return W_TRUE;
}
void _w_composite_check_buffered(w_control *control,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->style & W_DOUBLE_BUFFERED) == 0
			&& (_W_WIDGET(control)->style & W_NO_BACKGROUND) != 0) {
		return;
	}
	_w_control_check_buffered(control, reserved);
}
GtkStyle* _w_composite_child_style(w_control *control,
		_w_control_reserved *reserved) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (scrolledHandle != 0)
		return 0;
	return _w_control_child_style(control, reserved);
}
int _w_composite_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	//display.runSkin();
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
	if (_W_COMPOSITE(widget)->layout != 0) {
		if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
			//changed |= (state & LAYOUT_CHANGED) != 0;
			w_layout_compute_size(_W_COMPOSITE(widget)->layout,
					W_COMPOSITE(widget), e->size, wHint, hHint, e->changed);
			//state &= ~LAYOUT_CHANGED;
		} else {
			e->size->width = wHint;
			e->size->height = hHint;
		}
	} else {
		_w_composite_minimum_size(W_COMPOSITE(widget), e->size, wHint, hHint,
		TRUE);
		if (e->size->width == 0)
			e->size->width = DEFAULT_WIDTH;
		if (e->size->height == 0)
			e->size->height = DEFAULT_HEIGHT;
	}
	if (wHint != W_DEFAULT)
		e->size->width = wHint;
	if (hHint != W_DEFAULT)
		e->size->height = hHint;
	w_rect rect, trim;
	rect.x = 0;
	rect.y = 0;
	rect.width = e->size->width;
	rect.height = e->size->height;
	w_scrollable_compute_trim(W_SCROLLABLE(widget), &trim, &rect);
	e->size->width = trim.width;
	e->size->height = trim.height;
	return TRUE;
}
/*@Override
 Widget [] computeTabList () {
 Widget result [] = super.computeTabList ();
 if (result.length == 0) return result;
 Control [] list = tabList != null ? _getTabList () : _getChildren ();
 for (int i=0; i<list.length; i++) {
 Control child = list [i];
 Widget [] childList = child.computeTabList ();
 if (childList.length != 0) {
 Widget [] newResult = new Widget [result.length + childList.length];
 System.arraycopy (result, 0, newResult, 0, result.length);
 System.arraycopy (childList, 0, newResult, result.length, childList.length);
 result = newResult;
 }
 }
 return result;
 }*/
wresult _w_composite_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_CANVAS
			| STATE_CHECK_SUBWINDOW;
	int scrolled = (_W_WIDGET(widget)->style & (W_HSCROLL | W_VSCROLL)) != 0;
	if (!scrolled)
		_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
	GtkWidget *fixedHandle, *scrolledHandle, *handle, **scrolled_ = 0;
	if (scrolled || (_W_WIDGET(widget)->style & W_BORDER) != 0) {
		scrolled_ = &scrolledHandle;
	}
	if (scrolled_ == 0) {
		handle = _w_fixed_new();
		if (handle == 0)
			return W_ERROR_NO_HANDLES;
		fixedHandle = handle;
		gtk_widget_set_has_window(handle, TRUE);
//gtk_widget_set_can_focus(handle, TRUE);
	} else {
		int ret = _w_composite_create_handle_0(widget, index, &fixedHandle,
				scrolled_, &handle);
		if (ret < 0)
			return ret;

		if (scrolled_ == 0) {
			gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
		}
		g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
		if (scrolled_ != 0) {
			g_object_set_qdata(G_OBJECT(*scrolled_), gtk_toolkit->quark[0],
					widget);
		}
	}
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	return W_TRUE;
}
int _w_composite_apply_theme_background(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved) {
	return (_W_CONTROL(scrollable)->backgroundAlpha == 0
			|| (_W_WIDGET(scrollable)->style & (W_HSCROLL | W_VSCROLL)) == 0) ?
			1 : 0;
}
wresult _w_composite_create_handle_0(w_widget *composite, int index,
		GtkWidget **fixed, GtkWidget **scrolled, GtkWidget **handle) {
	GtkAdjustment *vadj = 0, *hadj = 0;
	GtkWidget *socketHandle = 0;
	GtkIMContext *imHandle = 0;
	_w_composite_reserved *reserved;
	*handle = 0;
	if (fixed != 0) {
		*fixed = _w_fixed_new();
		if (*fixed == 0)
			goto _err;
		gtk_widget_set_has_window(*fixed, TRUE);
		g_object_set_qdata(G_OBJECT(*fixed), gtk_toolkit->quark[0], composite);
	}
	if (scrolled != 0) {
		vadj = gtk_adjustment_new(0, 0, 100, 1, 10, 10);
		if (vadj == 0)
			goto _err;
		hadj = gtk_adjustment_new(0, 0, 100, 1, 10, 10);
		if (hadj == 0)
			goto _err;
		*scrolled = gtk_scrolled_window_new(hadj, vadj);
		if (*scrolled == 0)
			goto _err;
		g_object_set_qdata(G_OBJECT(vadj), gtk_toolkit->quark[0], composite);
		g_object_set_qdata(G_OBJECT(hadj), gtk_toolkit->quark[0], composite);
		g_object_set_qdata(G_OBJECT(*scrolled), gtk_toolkit->quark[0],
				composite);
	}
	*handle = (GtkWidget*) _w_fixed_new();
	if (*handle == 0)
		goto _err;
	g_object_set_qdata(G_OBJECT(*handle), gtk_toolkit->quark[0], composite);
	gtk_widget_set_has_window(*handle, TRUE);
	gtk_widget_set_can_focus(*handle, TRUE);
	//if ((_W_WIDGET(composite)->style & W_EMBEDDED) == 0) {
	if ((_W_WIDGET(composite)->state & STATE_CANVAS) != 0) {
		/* Prevent an input method context from being created for the Browser widget */
		//if (display.getData (NO_INPUT_METHOD) == null) {
		imHandle = gtk_im_multicontext_new();
		if (imHandle == 0)
			goto _err;
		//}
	}
	//}
	if (scrolled) {
		if (fixed)
			gtk_container_add(GTK_CONTAINER(*fixed), *scrolled);
		/*
		 * Force the scrolledWindow to have a single child that is
		 * not scrolled automatically.  Calling gtk_container_add()
		 * seems to add the child correctly but cause a warning.
		 */
		//display.setWarnings(false);
		gtk_container_add(GTK_CONTAINER(*scrolled), *handle);
		//display.setWarnings(warnings);

		GtkPolicyType hsp =
				(_W_WIDGET(composite)->style & W_HSCROLL) != 0 ?
						GTK_POLICY_ALWAYS : GTK_POLICY_NEVER;
		GtkPolicyType vsp =
				(_W_WIDGET(composite)->style & W_VSCROLL) != 0 ?
						GTK_POLICY_ALWAYS : GTK_POLICY_NEVER;
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(*scrolled), hsp,
				vsp);
		reserved = _W_COMPOSITE_GET_RESERVED(composite);
		if (reserved->has_border(W_COMPOSITE(composite), reserved)) {
			gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(*scrolled),
					GTK_SHADOW_ETCHED_IN);
		}
	}
	if ((_W_WIDGET(composite)->style & W_NO_REDRAW_RESIZE) != 0
			&& (_W_WIDGET(composite)->style & W_RIGHT_TO_LEFT) == 0) {
		gtk_widget_set_redraw_on_allocate(*handle,
		FALSE);
	}
	/*
	 * Bug in GTK.  When a widget is double buffered and the back
	 * pixmap is null, the double buffer pixmap is filled with the
	 * background of the widget rather than the current contents of
	 * the screen.  If nothing is drawn during an expose event,
	 * the pixels are altered.  The fix is to clear double buffering
	 * when NO_BACKGROUND is set and DOUBLE_BUFFERED
	 * is not explicitly set.
	 */
	if ((_W_WIDGET(composite)->style & W_DOUBLE_BUFFERED) == 0
			&& (_W_WIDGET(composite)->style & W_NO_BACKGROUND) != 0) {
		gtk_widget_set_double_buffered(*handle,
		FALSE);
	}
	_W_COMPOSITE(composite)->imHandle = imHandle;
	return W_TRUE;
	_err: if (vadj != 0)
		g_object_unref(vadj);
	if (hadj != 0)
		g_object_unref(hadj);
	if (scrolled && *scrolled != 0)
		g_object_unref(*scrolled);
	if (fixed && *fixed != 0)
		g_object_unref(*fixed);
	if (*handle != 0)
		g_object_unref(*handle);
	if (imHandle != 0) {
		g_object_unref(imHandle);
	}
	return W_ERROR_NO_HANDLES;
}
wresult _w_composite_draw_background(w_composite *composite, w_graphics *gc,
		w_rect *_rect, w_point *_offset) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_graphics_is_ok(gc))
		return W_ERROR_INVALID_ARGUMENT;
	w_rect rect;
	w_point offset;
	if (_rect == 0) {
		memset(&rect, 0, sizeof(rect));
	} else {
		memcpy(&rect, _rect, sizeof(rect));
	}
	if (_offset == 0) {
		memset(&offset, 0, sizeof(offset));
	} else {
		memcpy(&offset, _offset, sizeof(offset));
	}
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(composite);
	w_control *control = reserved->find_background_control(W_CONTROL(composite),
			reserved);
	if (control != 0) {
		cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
		cairo_save(cairo);
		if (_W_CONTROL(control)->backgroundImage != 0) {
			w_point pt, tmp;
			tmp.x = 0;
			tmp.y = 0;
			_w_toolkit_map_0(W_TOOLKIT(gtk_toolkit), W_CONTROL(composite),
					control, &pt, &tmp);
			cairo_translate(cairo, -pt.x - offset.x, -pt.y - offset.y);
			rect.x += pt.x + offset.x;
			rect.y += pt.y + offset.y;
			cairo_surface_t *surface;
			cairo_pattern_t *pattern = _w_image_create_pattern(
			_W_CONTROL(control)->backgroundImage, &surface);
			if (pattern != 0) {
				cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
				if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
					cairo_matrix_t matrix;
					matrix.xx = -1;
					matrix.yx = 0;
					matrix.xy = 0;
					matrix.yy = 1;
					matrix.x0 = 0;
					matrix.y0 = 0;
					cairo_pattern_set_matrix(pattern, &matrix);
				}
				cairo_set_source(cairo, pattern);
				cairo_surface_destroy(surface);
				cairo_pattern_destroy(pattern);
			} else {
				if (surface != 0)
					cairo_surface_destroy(surface);
				return W_ERROR_NO_HANDLES;
			}
		} else {
			GdkColor color;
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(control);
			creserved->get_background_color(control, &color, creserved);
			cairo_set_source_rgba(cairo, (color.red & 0xFFFF) / (float) 0xFFFF,
					(color.green & 0xFFFF) / (float) 0xFFFF,
					(color.blue & 0xFFFF) / (float) 0xFFFF, 1.0);
		}
		cairo_rectangle(cairo, rect.x, rect.y, rect.width, rect.height);
		cairo_fill(cairo);
		cairo_restore(cairo);
	} else {
		w_graphics_fill_rectangle(gc, &rect);
	}
	return W_TRUE;
}

wresult _w_composite_enable_widget(w_control *control, int enabled,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->state & STATE_CANVAS) != 0)
		return W_TRUE;
	return _w_control_enable_widget(control, enabled, reserved);
}
w_composite* _w_composite_find_deferred_control(w_composite *composite,
		_w_composite_reserved *reserved) {
	if ( _W_COMPOSITE(composite)->layoutCount > 0) {
		return composite;
	} else {
		w_composite *parent = _W_CONTROL(composite)->parent;
		_w_composite_reserved *preserved = _W_COMPOSITE_RESERVED(
				_w_widget_get_reserved(W_WIDGET(parent)));
		return preserved->find_deferred_control(parent, reserved);
	}
}

/*@Override
 Menu [] findMenus (Control control) {
 if (control == this) return new Menu [0];
 Menu result [] = super.findMenus (control);
 Control [] children = _getChildren ();
 for (int i=0; i<children.length; i++) {
 Control child = children [i];
 Menu [] menuList = child.findMenus (control);
 if (menuList.length != 0) {
 Menu [] newResult = new Menu [result.length + menuList.length];
 System.arraycopy (result, 0, newResult, 0, result.length);
 System.arraycopy (menuList, 0, newResult, result.length, menuList.length);
 result = newResult;
 }
 }
 return result;
 }*/

/*@Override
 void fixChildren (Shell newShell, Shell oldShell, Decorations newDecorations, Decorations oldDecorations, Menu [] menus) {
 super.fixChildren (newShell, oldShell, newDecorations, oldDecorations, menus);
 Control [] children = _getChildren ();
 for (int i=0; i<children.length; i++) {
 children [i].fixChildren (newShell, oldShell, newDecorations, oldDecorations, menus);
 }
 }*/

void _w_composite_fix_modal(w_control *control, GtkWindowGroup *group,
		GtkWindowGroup *modalGroup, _w_control_reserved *reserved) {
	_w_fixed *fixed =
			(_w_fixed*) _W_COMPOSITE_RESERVED(reserved)->parenting_handle(
					W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	_w_fixed *i = fixed->first;
	while (i != 0) {
		w_control *child = (w_control*) g_object_get_qdata(G_OBJECT(i),
				gtk_toolkit->quark[0]);
		if (child != 0) {
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
			creserved->fix_modal(child, group, modalGroup, creserved);
		}
		i = i->next;
	}
}

void _w_composite_fix_style(w_control *control, _w_control_reserved *reserved) {
	_w_control_fix_style(control, reserved);
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (scrolledHandle == 0)
		reserved->fix_style_0(control, _W_WIDGET(control)->handle, reserved);
	_w_fixed *fixed =
			(_w_fixed*) _W_COMPOSITE_RESERVED(reserved)->parenting_handle(
					W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	_w_fixed *i = fixed->first;
	while (i != 0) {
		w_control *child = (w_control*) g_object_get_qdata(G_OBJECT(i),
				gtk_toolkit->quark[0]);
		if (child != 0) {
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
			creserved->fix_style(child, creserved);
		}
		i = i->next;
	}
}

void _w_composite_fix_zorder(w_composite *composite,
		_w_composite_reserved *reserved) {
	if ((_W_WIDGET(composite)->state & STATE_CANVAS) != 0)
		return;
	GtkWidget *parentHandle = reserved->parenting_handle(W_WIDGET(composite),
			_W_WIDGET_RESERVED(reserved));
	GdkWindow *parentWindow = gtk_widget_get_window(parentHandle);
	if (parentWindow == 0)
		return;
	GdkWindow *redrawWindow = _W_CONTROL_RESERVED(reserved)->redrawWindow(
			W_WIDGET(composite), _W_CONTROL_RESERVED(reserved));
	void *userData;
	GList *windows = gdk_window_peek_children(parentWindow);
	while (windows != 0) {
		GdkWindow *window = (GdkWindow*) windows->data;
		if (window != redrawWindow) {
			gdk_window_get_user_data(window, &userData);
			if (userData == 0 || !_W_IS_FIXED(userData)) {
				gdk_window_lower(window);
			}
		}
		windows = windows->next;
	}
}

GtkWidget* _w_composite_focusHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	/*if (socketHandle != 0)
	 return socketHandle;*/
	return _W_WIDGET(control)->handle;
}

gboolean _w_composite_force_focus(w_control *control, GtkWidget *focusHandle,
		_w_widget_reserved *reserved) {
	/*if (socketHandle != 0)
	 gtk_widget_set_can_focus(focusHandle, TRUE);*/

	gboolean result = _w_control_force_focus_0(control, focusHandle, reserved);
	/*if (socketHandle != 0)
	 gtk_widget_set_can_focus(focusHandle, FALSE);*/
	return result;
}
int _w_composite_get_backgroundMode(w_composite *composite) {
	return _W_COMPOSITE(composite)->backgroundMode;
}
int _w_composite_get_children_count(w_composite *composite) {
	_w_composite_reserved *reserved = _W_COMPOSITE_GET_RESERVED(composite);
	_w_fixed *fixed = (_w_fixed*) reserved->parenting_handle(
			W_WIDGET(composite), _W_WIDGET_RESERVED(reserved));
	return fixed->count;
	/*int count = 0;
	 _w_fixed *i = fixed->first;
	 while (i != 0) {
	 count++;
	 i = i->next;
	 }
	 return count;*/
}
int _w_composite_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		if ((_W_WIDGET(widget)->state & STATE_ZERO_WIDTH) != 0
				&& (_W_WIDGET(widget)->state & STATE_ZERO_HEIGHT) != 0) {
			memset(e->rect, 0, sizeof(w_rect));
			return W_TRUE;
		}
		_W_CONTROL_RESERVED(reserved)->force_resize(W_CONTROL(widget),
				_W_CONTROL_RESERVED(reserved));
		GtkWidget *clientHandle =
		_W_CONTROL_RESERVED(reserved)->clientHandle(widget, reserved);
		GtkAllocation allocation;
		gtk_widget_get_allocation(clientHandle, &allocation);
		int width =
				(_W_WIDGET(widget)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int height =
				(_W_WIDGET(widget)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
		e->rect->x = 0;
		e->rect->y = 0;
		e->rect->width = width;
		e->rect->height = height;
		return W_TRUE;
	}
	return _w_scrollable_get_client_area(widget, e, reserved);
}
wresult _w_composite_get_layout(w_composite *composite, w_layout **layout) {
	*layout = _W_COMPOSITE(composite)->layout;
	return W_TRUE;
}
int _w_composite_get_layout_deferred(w_composite *composite) {
	return _W_COMPOSITE(composite)->layoutCount > 0;
}
wresult _w_composite_get_tab_list(w_composite *composite, w_iterator *it) {
	it->base.clazz = &_w_composite_iterator_class;
	_w_composite_reserved *reserved = _W_COMPOSITE_GET_RESERVED(composite);
	_w_fixed *fixed = (_w_fixed*) reserved->parenting_handle(
			W_WIDGET(composite), _W_WIDGET_RESERVED(reserved));
	_W_COMPOSITE_ITERATOR(it)->composite = composite;
	_W_COMPOSITE_ITERATOR(it)->first = fixed->first;
	_W_COMPOSITE_ITERATOR(it)->i = fixed->first;
	_W_COMPOSITE_ITERATOR(it)->count = fixed->count;
	_W_COMPOSITE_ITERATOR(it)->tablist = 1;
	return W_TRUE;
}
int _w_composite_has_border(w_composite *composite,
		_w_composite_reserved *reserved) {
	return (_W_WIDGET(composite)->style & W_BORDER) != 0;
}
void _w_composite_hook_events(w_widget *widget, _w_widget_reserved *reserved) {
	_w_scrollable_hook_events(widget, reserved);
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		gtk_widget_add_events(_W_WIDGET(widget)->handle,
				GDK_POINTER_MOTION_HINT_MASK);
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(widget, reserved);
		if (scrolledHandle != 0) {
			if (_W_COMPOSITE_RESERVED(reserved)->signal_scroll_child == 0) {
				_W_COMPOSITE_RESERVED(reserved)->signal_scroll_child =
						g_signal_lookup("scroll-child",
								G_TYPE_FROM_INSTANCE(scrolledHandle));
			}
			_w_widget_connect(scrolledHandle, SIGNAL_SCROLL_CHILD,
			_W_COMPOSITE_RESERVED(reserved)->signal_scroll_child,
			FALSE);
		}
	}
}
GtkIMContext* _w_composite_imHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	return _W_COMPOSITE(control)->imHandle;
}
wresult _w_composite_is_layout_deferred(w_composite *composite) {
	_w_composite_reserved *reserved = _W_COMPOSITE_GET_RESERVED(composite);
	return reserved->find_deferred_control(composite, reserved) != 0;
}
wresult _w_composite_is_tab_group(w_control *control,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->state & STATE_CANVAS) != 0)
		return W_TRUE;
	return _w_control_is_tab_group(control, reserved);
}
void _w_composite_layout(w_composite *composite, int flags) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(composite);
	reserved->mark_layout(W_CONTROL(composite), flags, reserved);
	reserved->update_layout(W_CONTROL(composite), flags, reserved);
}
wresult _w_composite_layout_changed(w_composite *_this, w_control **changed,
		size_t length, int flags) {
	if (changed != 0) {
		for (int i = 0; i < length; i++) {
			w_control *control = changed[i];
			if (control == 0)
				return W_ERROR_INVALID_ARGUMENT;
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_ERROR_INVALID_ARGUMENT;
			int ancestor = W_FALSE;
			w_composite *composite = _W_CONTROL(control)->parent;
			while (composite != 0) {
				ancestor = composite == _this;
				if (ancestor)
					break;
				composite = _W_CONTROL(composite)->parent;
			}
			if (!ancestor)
				return W_ERROR_INVALID_PARENT;
		}
		int updateCount = 0;
		/*Composite [] update = new Composite [16];
		 for (int i = 0; i < length; i++) {
		 w_control* child = changed[i];
		 w_composite *composite = _W_CONTROL(child)->parent;
		 while (child != _this) {
		 if (composite.layout != null) {
		 composite.state |= LAYOUT_NEEDED;
		 if (!composite.layout.flushCache(child)) {
		 composite.state |= LAYOUT_CHANGED;
		 }
		 }
		 if (updateCount == update.length) {
		 Composite [] newUpdate = new Composite [update.length + 16];
		 System.arraycopy(update, 0, newUpdate, 0, update.length);
		 update = newUpdate;
		 }
		 child = update[updateCount++] = composite;
		 composite = child.parent;
		 }
		 }
		 if ((flags & SWT.DEFER) != 0) {
		 setLayoutDeferred(true);
		 display.addLayoutDeferred(this);
		 }
		 for (int i = updateCount - 1; i >= 0; i--) {
		 update[i].updateLayout(false);
		 }*/
	} else {
		/*if (layout == null && (flags & SWT.ALL) == 0)
		 return;*/
		_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(_this);
		reserved->mark_layout(W_CONTROL(_this), flags, reserved);
		if ((flags & W_DEFER) != 0) {
			_w_composite_set_layout_deferred(_this, W_TRUE);
			//display.addLayoutDeferred(this);
		}
		reserved->update_layout(W_CONTROL(_this), flags, reserved);
	}
	return W_TRUE;
}

void _w_composite_mark_layout(w_control *_this, int flags,
		_w_control_reserved *reserved) {
	//if (layout != null) {
	_W_WIDGET(_this)->state |= STATE_LAYOUT_NEEDED;
	if (flags & W_CHANGED)
		_W_WIDGET(_this)->state |= STATE_LAYOUT_CHANGED;
	//}
	if (flags & W_ALL) {
		_w_fixed *fixed =
				(_w_fixed*) _W_COMPOSITE_RESERVED(reserved)->parenting_handle(
						W_WIDGET(_this), _W_WIDGET_RESERVED(reserved));
		_w_fixed *i = fixed->first;
		while (i != 0) {
			w_control *child = (w_control*) g_object_get_qdata(G_OBJECT(i),
					gtk_toolkit->quark[0]);
			if (child != 0) {
				_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
				creserved->mark_layout(child, flags, creserved);
			}
			i = i->next;
		}
	}
}

void _w_composite_move_above(w_composite *_this, GtkWidget *child,
		GtkWidget *sibling, _w_control_reserved *reserved) {
	if (child == sibling)
		return;
	GtkWidget *parentHandle =
	_W_COMPOSITE_RESERVED(reserved)->parenting_handle(W_WIDGET(_this),
			_W_WIDGET_RESERVED(reserved));
#if GTK3
	_w_fixed_restack(parentHandle, child, sibling, W_TRUE);
	return;
#else
#endif
}

void _w_composite_move_below(w_composite *_this, GtkWidget *child,
		GtkWidget *sibling, _w_control_reserved *reserved) {
	if (child == sibling)
		return;
	GtkWidget *parentHandle =
	_W_COMPOSITE_RESERVED(reserved)->parenting_handle(W_WIDGET(_this),
			_W_WIDGET_RESERVED(reserved));
	GtkWidget *fixedHandle = reserved->fixedHandle(W_WIDGET(_this),
			_W_WIDGET_RESERVED(reserved));
	if (sibling == 0 && parentHandle == fixedHandle) {
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(_this),
				_W_WIDGET_RESERVED(reserved));
		_w_composite_move_above(_this, child,
				scrolledHandle != 0 ? scrolledHandle : _W_WIDGET(_this)->handle,
				reserved);
		return;
	}
#if GTK3
	_w_fixed_restack(parentHandle, child, sibling, W_FALSE);
	return;
#else
#endif
}

void _w_composite_move_children(w_composite *composite, int oldWidth) {
	_w_control_reserved *reserved;
	_w_composite_reserved *reserved1 = _W_COMPOSITE_GET_RESERVED(composite);
	int clientWidth = _W_CONTROL_RESERVED(reserved1)->get_client_width(
			W_CONTROL(composite), _W_CONTROL_RESERVED(reserved1));
	_w_fixed *fixed = (_w_fixed*) reserved1->parenting_handle(
			W_WIDGET(composite), _W_WIDGET_RESERVED(reserved1));
	_w_fixed *_child = fixed->first;
	while (_child != 0) {
		w_control *child = (w_control*) g_object_get_qdata(
				G_OBJECT(_child->child), gtk_toolkit->quark[0]);
		if (w_widget_is_ok(W_WIDGET(child))) {
			reserved = _W_CONTROL_GET_RESERVED(child);
			GtkWidget *topHandle = reserved->widget.topHandle(W_WIDGET(child),
					_W_WIDGET_RESERVED(reserved));
			GtkAllocation allocation;
			gtk_widget_get_allocation(topHandle, &allocation);
			int x = allocation.x;
			int y = allocation.y;
			int controlWidth = allocation.width;
			if (oldWidth > 0)
				x = oldWidth - controlWidth - x;
			x = clientWidth - controlWidth - x;
			/*GdkWindow *enableWindow = _W_CONTROL(child)->fixed->enableWindow;
			 if (enableWindow != 0) {
			 gdk_window_move(enableWindow, x, y);
			 }*/
//child.moveHandle(x, y);
			/*
			 * Cause a size allocation this widget's topHandle.  Note that
			 * all calls to gtk_widget_size_allocate() must be preceded by
			 * a call to gtk_widget_size_request().
			 */
			GtkRequisition requisition;
#if GTK3
			gtk_widget_get_preferred_size(topHandle, &requisition, NULL);
#else
			gtk_widget_size_request(topHandle, &requisition);
#endif
			allocation.x = x;
			allocation.y = y;
			gtk_widget_size_allocate(topHandle, &allocation);
		}
		_child = _child->next;
	}
}
void _w_composite_minimum_size(w_composite *composite, w_size *size, int wHint,
		int hHint, int changed) {
	w_iterator children;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	/*
	 * Since getClientArea can be overridden by subclasses, we cannot
	 * call getClientAreaInPixels directly.
	 */
	w_rect clientArea, rect;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &clientArea);
	int width = 0, height = 0;
	w_control *child = 0;
	while (w_iterator_next(&children, &child)) {
		if (child != 0) {
			w_control_get_bounds(child, &rect.pt, &rect.sz);
			width = w_max(width, rect.x - clientArea.x + rect.width);
			height = w_max(height, rect.y - clientArea.y + rect.height);
		}
	}
	size->width = width;
	size->height = height;
}
GtkWidget* _w_composite_parenting_handle(w_widget *control,
		_w_widget_reserved *reserved) {
	if ((_W_WIDGET(control)->state & STATE_CANVAS) != 0)
		return _W_WIDGET(control)->handle;
	GtkWidget *fixedHandle = _W_CONTROL_RESERVED(reserved)->fixedHandle(control,
			reserved);
	return fixedHandle != 0 ? fixedHandle : _W_WIDGET(control)->handle;
}
#if GTK2
void _w_composite_print_widget(w_control *control, w_graphics *gc,
		void *drawable, int depth, w_point *loc,
		_w_control_reserved *reserved) {
	w_region oldClip, newClip;
	w_rect rect, tmp, clientRect;
	w_region_init(&oldClip);
	w_region_init(&newClip);
	//w_region_create(&oldClip);
	w_region_create(&newClip);
	w_graphics_get_clipping_region(gc, &oldClip);
	W_CONTROL_GET_CLASS(control)->get_bounds(control, &rect);
	w_region_add_region(&newClip, &oldClip);
	tmp.x = loc->x;
	tmp.y = loc->y;
	tmp.width = rect.width;
	tmp.height = rect.height;
	w_region_intersect_rect(&newClip, &tmp);
	w_graphics_set_clipping_region(gc, &newClip);
	_w_control_print_widget(control, gc, drawable, depth, loc, reserved);
	w_scrollable_get_client_area(W_SCROLLABLE(control), &clientRect);
	_w_toolkit_map_0(W_TOOLKIT(gtk_toolkit), control,
			W_CONTROL(_W_CONTROL(control)->parent), &tmp.pt, &clientRect.pt);
	clientRect.x = loc->x + tmp.x - rect.x;
	clientRect.y = loc->y + tmp.y - rect.y;
	w_region_intersect_rect(&newClip, &clientRect);
	w_graphics_set_clipping_region(gc, &newClip);
	/*
	 * children
	 */
	_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(control,
			reserved);
	_w_fixed *first = fixed->first;
	_w_fixed *i = 0;
	if (first != 0) {
		i = first->prev;
	}
	while (i != first) {
		w_control *child = (w_control*) _GTK_GET_WIDGET(i);
		if (w_control_get_visible(child) > 0) {
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
			W_CONTROL_GET_CLASS(child)->get_bounds(child, &tmp);
			tmp.x += loc->x;
			tmp.y += loc->y;
			creserved->print_widget(child, gc, drawable, depth, &tmp.pt,
					creserved);
		}
		i = i->prev;
	}
	/*
	 * end children
	 */
	w_graphics_set_clipping_region(gc, &oldClip);
	w_region_dispose(&oldClip);
	w_region_dispose(&newClip);
}
#endif
void _w_composite_redraw_children(w_control *control,
		_w_control_reserved *reserved) {
	_w_control_redraw_children(control, reserved);
	_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(control,
			reserved);
	_w_fixed *i = fixed->first;
	w_rect rect;
	memset(&rect, 0, sizeof(rect));
	while (i != 0) {
		w_control *child = (w_control*) _GTK_GET_WIDGET(i);
		if (child != 0
				&& (_W_WIDGET(child)->state & STATE_PARENT_BACKGROUND) != 0) {
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
			creserved->redraw_widget(control, &rect,
			REDRAW_REDRAWALL | REDRAW_TRIM, creserved);
			creserved->redraw_children(child, creserved);
		}
		i = i->next;
	}
}

void _w_composite_resize_handle(w_control *control, w_size *size,
		_w_control_reserved *reserved) {
	_w_control_resize_handle(control, size, reserved);
	/*if (socketHandle != 0) {
	 #if GTK3
	 swt_fixed_resize(handle, socketHandle, width, height);
	 #else
	 gtk_widget_set_size_request(socketHandle, width, height);
	 #endif
	 }*/
}
wresult _w_composite_set_background_mode(w_composite *composite, int mode) {
	_W_COMPOSITE(composite)->backgroundMode = mode;
	_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(composite,
			_w_widget_get_reserved(W_WIDGET(composite)));
	_w_fixed *i = fixed->first;
	w_rect rect;
	memset(&rect, 0, sizeof(rect));
	while (i != 0) {
		w_control *child = (w_control*) _GTK_GET_WIDGET(i);
		if (child != 0) {
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
			creserved->update_background_mode(child, creserved);
		}
		i = i->next;
	}
	return W_TRUE;
}
wresult _w_composite_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_reserved *reserved) {
	int result = _w_control_set_bounds_0(control, location, size, reserved);
	if (result < 0)
		return result;
	if (result & 2) { // widget is resized
		reserved->mark_layout(control, 0, reserved);
		reserved->update_layout(control, 0, reserved);
	}
	return result;
}
wresult _w_composite_set_focus(w_control *control) {
	_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(control,
			_w_widget_get_reserved(W_WIDGET(control)));
	_w_fixed *i = fixed->first;
	while (i != 0) {
		w_control *child = (w_control*) _GTK_GET_WIDGET(i);
		if (w_control_get_visible(child) > 0) {
			if (W_CONTROL_GET_CLASS(child)->set_focus(child) > 0)
				return W_TRUE;
		}
		i = i->next;
	}
	return _w_control_set_focus(control);
}
wresult _w_composite_set_layout(w_composite *composite, w_layout *layout) {
	_W_COMPOSITE(composite)->layout = layout;
	return W_TRUE;
}

wresult _w_composite_set_layout_deferred(w_composite *composite, int defer) {
	if (!defer) {
		_W_COMPOSITE(composite)->layoutCount--;
		if (_W_COMPOSITE(composite)->layoutCount == 0) {
			if ((_W_WIDGET(composite)->state & STATE_LAYOUT_CHILD) != 0
					|| (_W_WIDGET(composite)->state & STATE_LAYOUT_NEEDED)
							!= 0) {
				_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(
						composite);
				reserved->update_layout(W_CONTROL(composite), W_ALL, reserved);
			}
		}
	} else {
		_W_COMPOSITE(composite)->layoutCount++;
	}
	return W_TRUE;
}
void _w_composite_set_orientation(w_control *control, int create,
		_w_control_reserved *reserved) {
	_w_scrollable_set_orientation(control, create, reserved);
	if (!create) {
		int flags = W_RIGHT_TO_LEFT | W_LEFT_TO_RIGHT;
		int orientation = _W_WIDGET(control)->style & flags;
		_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(control,
				reserved);
		_w_fixed *i = fixed->first;
		while (i != 0) {
			w_control *child = (w_control*) _GTK_GET_WIDGET(i);
			w_control_set_orientation(child, orientation);
			i = i->next;
		}
		if (((_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0)
				!= ((_W_WIDGET(control)->style & W_MIRRORED) != 0)) {
			reserved->move_children(control, -1, reserved);
		}
	}
}

int _w_composite_set_scrollbar_visible(w_widget *scrollable, int style,
		int visible, _w_widget_reserved *reserved) {
	int changed = _w_scrollable_set_scrollbar_visible_0(scrollable, style,
			visible, reserved);
	if (changed /*&& _W_COMPOSITE(scrollable)->layout != 0*/) {
		_W_CONTROL_RESERVED(reserved)->mark_layout(W_CONTROL(scrollable), 0,
				_W_CONTROL_RESERVED(reserved));
		_W_CONTROL_RESERVED(reserved)->update_layout(W_CONTROL(scrollable), 0,
				_W_CONTROL_RESERVED(reserved));
	}
	return changed;
}

wresult _w_composite_set_tab_group_focus(w_widget *widget, int next,
		_w_widget_reserved *reserved) {
	wresult setTabItemFocus;
	wresult isTabItem;
	isTabItem = _W_CONTROL_RESERVED(reserved)->is_tab_item(W_CONTROL(widget),
			_W_CONTROL_RESERVED(reserved));
	if (isTabItem > 0) {
		setTabItemFocus = reserved->set_tab_item_focus(widget, next, reserved);
		return setTabItemFocus;
	}
	int takeFocus = (_W_WIDGET(widget)->style & W_NO_FOCUS) == 0;
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0)
		takeFocus = W_TRUE/*hooksKeys()*/;
	//if (socketHandle != 0)
	takeFocus = W_TRUE;
	setTabItemFocus = reserved->set_tab_item_focus(widget, next, reserved);
	if (takeFocus && setTabItemFocus > 0)
		return W_TRUE;
	_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(widget,
			reserved);
	_w_fixed *i = fixed->first;
	while (i != 0) {
		w_control *child = (w_control*) _GTK_GET_WIDGET(i);
		if (child != 0) {
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
			isTabItem = creserved->is_tab_item(child, creserved);
			setTabItemFocus =
			_W_WIDGET_RESERVED(creserved)->set_tab_item_focus(W_WIDGET(child),
					next, _W_WIDGET_RESERVED(creserved));
			if (isTabItem > 0 && setTabItemFocus > 0)
				return W_TRUE;
		}
		i = i->next;
	}
	return W_FALSE;
}

wresult _w_composite_set_tab_item_focus(w_widget *widget, int next,
		_w_widget_reserved *reserved) {
	wresult result = _w_control_set_tab_item_focus(widget, next, reserved);
	if (result <= 0)
		return result;
	/*if (socketHandle != 0) {
	 int direction = next ? OS.GTK_DIR_TAB_FORWARD : OS.GTK_DIR_TAB_BACKWARD;
	 OS.GTK_WIDGET_UNSET_FLAGS(socketHandle, OS.GTK_HAS_FOCUS);
	 OS.gtk_widget_child_focus(socketHandle, direction);
	 OS.GTK_WIDGET_SET_FLAGS(socketHandle, OS.GTK_HAS_FOCUS);
	 }*/
	return result;
}
wresult _w_composite_set_tab_list(w_composite *composite, w_control **tabList,
		size_t length) {
	if (tabList != 0) {
		for (int i = 0; i < length; i++) {
			w_control *control = tabList[i];
			if (control == 0)
				return W_ERROR_INVALID_ARGUMENT;
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_ERROR_INVALID_ARGUMENT;
			if (_W_CONTROL(control)->parent != composite)
				return W_ERROR_INVALID_PARENT;
		}
		/*
		 * remove tablist from all children
		 */
		_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(
				composite, _w_widget_get_reserved(W_WIDGET(composite)));
		_w_fixed *i = fixed->first;
		while (i != 0) {
			w_control *child = (w_control*) _GTK_GET_WIDGET(i);
			if (child != 0) {
				_W_WIDGET(child)->state &= ~STATE_TAB_LIST;
			}
			i = i->next;
		}
		for (int i = 0; i < length; i++) {
			w_control *control = tabList[i];
			_W_WIDGET(control)->state |= STATE_TAB_LIST;
		}
	}
	return W_TRUE;
}
void _w_composite_show_widget(w_control *control,
		_w_control_reserved *reserved) {
	_w_scrollable_show_widget(control, reserved);
	/*if (socketHandle != 0) {
	 gtk_widget_show(socketHandle);
	 embeddedHandle = gtk_socket_get_id(socketHandle);
	 }*/
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (scrolledHandle == 0)
		reserved->fix_style_0(control, _W_WIDGET(control)->handle, reserved);
}

wresult _w_composite_check_subwindow(w_control *control,
		_w_control_reserved *reserved) {
	return (_W_WIDGET(control)->state & STATE_CHECK_SUBWINDOW) != 0;
}

wresult _w_composite_translate_mnemonic(w_control *_this, w_event_key *event,
		w_control *control, _w_control_reserved *reserved) {
	wresult result = _w_control_translate_mnemonic(_this, event, control,
			reserved);
	if (result > 0)
		return result;
	if (control != 0) {
		_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(_this,
				reserved);
		_w_fixed *i = fixed->first;
		while (i != 0) {
			w_control *child = (w_control*) _GTK_GET_WIDGET(i);
			if (child != 0) {
				_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
				result = creserved->translate_mnemonic(child, event, control,
						creserved);
				if (result > 0)
					return result;
			}
			i = i->next;
		}
	}
	return W_FALSE;
}

int _w_composite_traversal_code(w_control *control, int key, GdkEventKey *event,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->state & STATE_CANVAS) != 0) {
		if ((_W_WIDGET(control)->style & W_NO_FOCUS) != 0)
			return 0;
		/*if (hooksKeys())*/
		return 0;
	}
	return _w_control_traversal_code(control, key, event, reserved);
}

wresult _w_composite_translate_traversal(w_control *control,
		GdkEventKey *keyEvent, _w_control_reserved *reserved) {
	/*if (socketHandle != 0)
	 return false;*/
	return _w_control_translate_traversal(control, keyEvent, reserved);
}

void _w_composite_update_background_mode(w_control *control,
		_w_control_reserved *reserved) {
	_w_scrollable_update_background_mode(control, reserved);
	_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(control,
			reserved);
	_w_fixed *i = fixed->first;
	while (i != 0) {
		w_control *child = (w_control*) _GTK_GET_WIDGET(i);
		if (child != 0) {
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
			creserved->update_background_mode(child, creserved);
		}
		i = i->next;
	}
}

void _w_composite_update_layout(w_control *_this, int flags,
		_w_control_reserved *reserved) {
	w_composite *parent =
	_W_COMPOSITE_RESERVED(reserved)->find_deferred_control(W_COMPOSITE(_this),
			_W_COMPOSITE_RESERVED(reserved));
	if (parent != 0) {
		_W_WIDGET(parent)->state |= STATE_LAYOUT_CHILD;
		return;
	}
	if ((_W_WIDGET(_this)->state & STATE_LAYOUT_NEEDED) != 0) {
		int changed = (_W_WIDGET(_this)->state & STATE_LAYOUT_CHANGED) != 0;
		_W_WIDGET(_this)->state &=
				~(STATE_LAYOUT_NEEDED | STATE_LAYOUT_CHANGED);
		//display.runSkin();
		//layout.layout(_this, changed);
		w_event _e;
		_e.type = W_EVENT_LAYOUTDETECT;
		_e.platform_event = 0;
		_e.widget = W_WIDGET(_this);
		_e.data = 0;
		_w_widget_send_event(W_WIDGET(_this), &_e);
	}
	if (flags & W_ALL) {
		_W_WIDGET(_this)->state &= ~STATE_LAYOUT_CHILD;
		_w_fixed *fixed = (_w_fixed*) _W_COMPOSITE_GET_PARENTING_HANDLE(_this,
				reserved);
		_w_fixed *i = fixed->first;
		while (i != 0) {
			w_control *child = (w_control*) _GTK_GET_WIDGET(i);
			if (child != 0) {
				_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
				creserved->update_layout(child, flags, creserved);
			}
			i = i->next;
		}
	}
}

void _w_composite_add_child_0(w_composite *parent, GtkWidget *widget,
		_w_composite_reserved *reserved) {
	GtkWidget *_p = reserved->parenting_handle(W_WIDGET(parent),
			_W_WIDGET_RESERVED(reserved));
	gtk_container_add(GTK_CONTAINER(_p), widget);
}
void _w_composite_add_child(w_composite *parent, GtkWidget *widget) {
	_w_composite_reserved *reserved = _W_COMPOSITE_GET_RESERVED(parent);
	reserved->add_child(parent, widget, reserved);
}

/*
 * signals
 */

gboolean _gtk_composite_destroy(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return _gtk_control_destroy(widget, e, reserved);

}

gboolean _gtk_composite_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved) {
	gboolean result = _gtk_scrollable_button_press_event(widget, e, reserved);
	if (result != 0)
		return result;
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		if ((_W_WIDGET(widget)->style & W_NO_FOCUS) == 0) {
			GdkEventButton *gdkEvent = (GdkEventButton*) e->args[0];
			if (gdkEvent->button == 1) {
				if (_w_composite_get_children_count(W_COMPOSITE(widget)) == 0)
					W_CONTROL_GET_CLASS(widget)->set_focus(W_CONTROL(widget));
			}
		}
	}
	return result;
}
gboolean _gtk_composite_draw(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
#if USE_CAIRO
	if (GTK_VERSION >= VERSION(3, 16, 0)) {
		GtkStyleContext *context = gtk_widget_get_style_context(e->widget);
		GtkAllocation allocation;
		gtk_widget_get_allocation(e->widget, &allocation);
		int width =
				(_W_WIDGET(widget)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int height =
				(_W_WIDGET(widget)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
// We specify a 0 value for x & y as we want the whole widget to be
// colored, not some portion of it.
		gtk_render_background(context, (cairo_t*) e->args[0], 0, 0, width,
				height);
	}
	return _gtk_control_draw(widget, e, reserved);
#else
#endif
}

gboolean _gtk_composite_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_key_press_event(widget, e, reserved);
	if (result != 0)
		return result;
	/*
	 * Feature in GTK.  The default behavior when the return key
	 * is pressed is to select the default button.  This is not the
	 * expected behavior for Composite and its subclasses.  The
	 * fix is to avoid calling the default handler.
	 */
	GtkWidget *socketHandle = 0;
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0 && socketHandle == 0) {
		GdkEventKey *keyEvent = (GdkEventKey*) e->args[0];
		int key = keyEvent->keyval;
		switch (key) {
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
			return TRUE;
		}
	}
	return result;
}

gboolean _gtk_composite_focus(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *socketHandle = 0;
	if (e->widget == socketHandle)
		return 0;
	return _gtk_control_focus(widget, e, reserved);
}

gboolean _gtk_composite_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_focus_in_event(widget, e, reserved);
	return (_W_WIDGET(widget)->state & STATE_CANVAS) != 0 ? TRUE : result;
}

gboolean _gtk_composite_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_focus_out_event(widget, e, reserved);
	return (_W_WIDGET(widget)->state & STATE_CANVAS) != 0 ? TRUE : result;
}

gboolean _gtk_composite_map(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	_w_composite_fix_zorder(W_COMPOSITE(widget),
			_W_COMPOSITE_RESERVED(reserved));
	return FALSE;
}

gboolean _gtk_composite_realize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_realize(widget, e, reserved);
	;
	if ((_W_WIDGET(widget)->style & W_NO_BACKGROUND) != 0) {
		GtkWidget *paintHandle = _W_CONTROL_RESERVED(reserved)->paintHandle(
				widget, reserved);
		GdkWindow *window = gtk_widget_get_window(paintHandle);
		if (window != 0) {
#if GTK3
			gdk_window_set_background_pattern(window, 0);
#endif
#if GTK2
#endif
		}
	}
	/*if (socketHandle != 0) {
	 embeddedHandle = OS.gtk_socket_get_id(socketHandle);
	 }*/
	return result;
}

gboolean _gtk_composite_scroll_child(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/* Stop GTK scroll child signal for canvas */
	g_signal_stop_emission(e->widget,
	_W_COMPOSITE_RESERVED(reserved)->signal_scroll_child, 0);
	return TRUE;
}

gboolean _gtk_composite_style_set(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_style_set(widget, e, reserved);
	if ((_W_WIDGET(widget)->style & W_NO_BACKGROUND) != 0) {
		GtkWidget *paintHandle = _W_CONTROL_RESERVED(reserved)->paintHandle(
				widget, reserved);
		GdkWindow *window = gtk_widget_get_window(paintHandle);
#if GTK3
		/*if (window != 0)
		 gdk_window_set_back_pixmap(window, 0, FALSE);*/
#endif
#if GTK2
		if (window != 0)
			gdk_window_set_back_pixmap(window, 0, FALSE);
#endif
	}
	return result;
}

void _w_composite_class_init(struct _w_composite_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_composite_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_composite);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_composite);
	/*
	 * composite class
	 */
	W_CONTROL_CLASS(clazz)->set_focus = _w_composite_set_focus;
	clazz->get_children = _w_composite_get_children;
	clazz->get_layout = _w_composite_get_layout;
	clazz->get_layout_deferred = _w_composite_get_layout_deferred;
	clazz->get_tab_list = _w_composite_get_tab_list;
	clazz->is_layout_deferred = _w_composite_is_layout_deferred;
	clazz->set_layout = _w_composite_set_layout;
	clazz->set_layout_deferred = _w_composite_set_layout_deferred;
	/*
	 * reserved
	 */
	_w_composite_reserved *reserved = _W_COMPOSITE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->hook_events = _w_composite_hook_events;
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_composite_topHandle;
	_W_CONTROL_RESERVED(reserved)->fixedHandle = _w_composite_topHandle;
	_W_WIDGET_RESERVED(reserved)->create_handle = _w_composite_create_handle;
	_W_WIDGET_RESERVED(reserved)->get_client_area =
			_w_composite_get_client_area;
	_W_WIDGET_RESERVED(reserved)->compute_size = _w_composite_compute_size;
	_W_WIDGET_RESERVED(reserved)->check_style = _w_composite_check_style;
	_W_WIDGET_RESERVED(reserved)->set_tab_group_focus =
			_w_composite_set_tab_group_focus;
	_W_WIDGET_RESERVED(reserved)->set_tab_item_focus =
			_w_composite_set_tab_item_focus;
	_W_CONTROL_RESERVED(reserved)->check_buffered = _w_composite_check_buffered;
	_W_CONTROL_RESERVED(reserved)->set_bounds = _w_composite_set_bounds_0;
	_W_CONTROL_RESERVED(reserved)->resize_handle = _w_composite_resize_handle;
	_W_CONTROL_RESERVED(reserved)->child_style = _w_composite_child_style;
	_W_CONTROL_RESERVED(reserved)->enable_widget = _w_composite_enable_widget;
	_W_CONTROL_RESERVED(reserved)->fix_modal = _w_composite_fix_modal;
	_W_CONTROL_RESERVED(reserved)->fix_style = _w_composite_fix_style;
	_W_CONTROL_RESERVED(reserved)->focusHandle = _w_composite_focusHandle;
	_W_CONTROL_RESERVED(reserved)->force_focus = _w_composite_force_focus;
	_W_CONTROL_RESERVED(reserved)->imHandle = _w_composite_imHandle;
	_W_CONTROL_RESERVED(reserved)->is_tab_group = _w_composite_is_tab_group;
	_W_CONTROL_RESERVED(reserved)->mark_layout = _w_composite_mark_layout;
	_W_CONTROL_RESERVED(reserved)->update_layout = _w_composite_update_layout;
#if GTK2
	_W_CONTROL_RESERVED(reserved)->print_widget = _w_composite_print_widget;
#endif
	_W_CONTROL_RESERVED(reserved)->redraw_children =
			_w_composite_redraw_children;
	_W_CONTROL_RESERVED(reserved)->set_orientation =
			_w_composite_set_orientation;
	_W_CONTROL_RESERVED(reserved)->show_widget = _w_composite_show_widget;
	_W_CONTROL_RESERVED(reserved)->check_subwindow =
			_w_composite_check_subwindow;
	_W_CONTROL_RESERVED(reserved)->translate_mnemonic =
			_w_composite_translate_mnemonic;
	_W_CONTROL_RESERVED(reserved)->traversal_code = _w_composite_traversal_code;
	_W_CONTROL_RESERVED(reserved)->translate_traversal =
			_w_composite_translate_traversal;
	_W_CONTROL_RESERVED(reserved)->update_background_mode =
			_w_composite_update_background_mode;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle =
			_w_composite_scrolledHandle;
	_W_SCROLLABLE_RESERVED(reserved)->apply_theme_background =
			_w_composite_apply_theme_background;
	_W_SCROLLABLE_RESERVED(reserved)->set_scrollbar_visible =
			_w_composite_set_scrollbar_visible;
	reserved->has_border = _w_composite_has_border;
	reserved->parenting_handle = _w_composite_parenting_handle;
	reserved->add_child = _w_composite_add_child_0;
	reserved->find_deferred_control = _w_composite_find_deferred_control;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_RESERVED(reserved)->signals;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_composite_button_press_event;
	signals[SIGNAL_DRAW] = _gtk_composite_draw;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_composite_key_press_event;
	signals[SIGNAL_FOCUS] = _gtk_composite_focus;
	signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_composite_focus_in_event;
	signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_composite_focus_out_event;
	signals[SIGNAL_MAP] = _gtk_composite_map;
	signals[SIGNAL_REALIZE] = _gtk_composite_realize;
	signals[SIGNAL_SCROLL_CHILD] = _gtk_composite_scroll_child;
}
#endif

