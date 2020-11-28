/*
 * control.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "control.h"
#include "toolkit.h"
#include "menu.h"
#include <sys/time.h>
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	wresult ret;
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_widget_is_ok(parent))
		return W_ERROR_INVALID_ARGUMENT;
	if (w_widget_class_id(parent) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_ARGUMENT;
	_w_widget_reserved *preserved = _w_widget_get_reserved(parent);
	preserved->check_open(parent, preserved);
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	_W_WIDGET(widget)->post_event = post_event;
	_w_widget_reserved *reserved = _w_widget_get_reserved(widget);
	_W_WIDGET(widget)->style = reserved->check_style(widget, style);
	_W_WIDGET(widget)->state |= STATE_TAB_LIST;
	return reserved->create_widget(widget, 0, reserved);
}
int enterNotifyEventSignalId = 0;
GQuark GTK_POINTER_WINDOW = 0;
GQuark SWT_GRAB_WIDGET = 0;
w_font* _w_control_default_font(w_control *control) {
	return _w_toolkit_get_system_font(W_TOOLKIT(gtk_toolkit));
}

wresult _w_control_draw_background_0(w_control *_this, w_control *control,
		GdkWindow *window, cairo_t *cr, cairo_region_t *region, w_rect *rect,
		_w_control_reserved *reserved) {
#if USE_CAIRO
	w_rect r;
	memcpy(&r, rect, sizeof(w_rect));
	cairo_t *cairo = cr != 0 ? cr : gdk_cairo_create(window);
	if (cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (region != 0) {
		gdk_cairo_region(cairo, region);
		cairo_clip(cairo);
	}
	if (_W_CONTROL(control)->backgroundImage != 0) {
		w_point pt, tmp;
		tmp.x = tmp.y = 0;
		_w_toolkit_map_0(W_TOOLKIT(gtk_toolkit), _this, control, &pt, &tmp);
		cairo_translate(cairo, -pt.x, -pt.y);
		r.x += pt.x;
		r.y += pt.y;
		cairo_surface_t *surface;
		cairo_pattern_t *pattern = _w_image_create_pattern(
		_W_CONTROL(control)->backgroundImage, &surface);
		if (pattern == 0) {
			if (cairo != cr)
				cairo_destroy(cairo);
			if (surface != 0)
				cairo_surface_destroy(surface);
			return W_ERROR_NO_HANDLES;
		}
		cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
		if ((_W_WIDGET(_this)->style & W_MIRRORED) != 0) {
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
		GdkColor color;
		_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(control);
		creserved->get_background_color(control, &color, creserved);
		cairo_set_source_rgba(cairo, (color.red & 0xFFFF) / (float) 0xFFFF,
				(color.green & 0xFFFF) / (float) 0xFFFF,
				(color.blue & 0xFFFF) / (float) 0xFFFF, 1.0);
	}
	cairo_rectangle(cairo, r.x, r.y, r.width, r.height);
	cairo_fill(cairo);
	if (cairo != cr)
		cairo_destroy(cairo);
	return W_TRUE;
#endif
#if GTK2
#endif
}
wresult _w_control_draw_background(w_control *_this, w_control *control,
		GdkWindow *window, cairo_region_t *region, w_rect *rect,
		_w_control_reserved *reserved) {
	return _w_control_draw_background_0(_this, control, window, 0, region, rect,
			reserved);
}
wresult _w_control_draw_draw_gripper(w_control *control, w_graphics *gc,
		w_rect *rect, wbool vertical, _w_control_reserved *reserved) {
	GtkWidget *paintHandle = reserved->paintHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	GdkWindow *window = gtk_widget_get_window(paintHandle);
	if (window == 0)
		return W_FALSE;
	int orientation =
			vertical ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL;
	int x = rect->x;
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0) {
		x = reserved->get_client_width(control, reserved) - rect->width - x;
	}
#if GTK3
	GtkStyleContext *context = gtk_widget_get_style_context(paintHandle);
	gtk_style_context_save(context);
	gtk_style_context_add_class(context, GTK_STYLE_CLASS_PANE_SEPARATOR);
	gtk_style_context_set_state(context, GTK_STATE_FLAG_NORMAL);
	gtk_render_handle(context, _W_GRAPHICS(gc)->cairo, x, rect->y, rect->width,
			rect->height);
	gtk_style_context_restore(context);
#endif
#if GTK2
#endif
	return W_TRUE;
}
void _w_control_draw_widget(w_control *control, w_graphics *gc,
		_w_control_reserved *reserved) {

}
wresult _w_control_enable_widget(w_control *control, wbool enabled,
		_w_control_reserved *reserved) {
	gtk_widget_set_sensitive(_W_WIDGET(control)->handle, enabled);
	return W_TRUE;
}
GtkWidget* _w_control_enterExitHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	return _W_CONTROL_RESERVED(reserved)->eventHandle(control, reserved);
}
GdkWindow* _w_control_eventWindow(w_widget *control,
		_w_widget_reserved *reserved) {
	GtkWidget *eventHandle = _W_CONTROL_RESERVED(reserved)->eventHandle(control,
			reserved);
	gtk_widget_realize(eventHandle);
	return gtk_widget_get_window(eventHandle);
}
void _w_control_fix_focus(w_control *_this, w_control *focusControl,
		_w_control_reserved *reserved) {
	w_shell *shell;
	W_CONTROL_GET_CLASS(_this)->get_shell(_this, &shell);
	w_control *control = _this;
	while (control != W_CONTROL(shell)
			&& (control = W_CONTROL(_W_CONTROL(control)->parent)) != 0) {
		if (w_control_set_focus(control) > 0)
			return;
	}
	_w_shell_set_saved_focus(shell, focusControl);
	GtkWidget *focusHandle = _W_SHELL_VBOX(shell);
	gtk_widget_set_can_focus(focusHandle, TRUE);
	gtk_widget_grab_focus(focusHandle);
	// widget could be disposed at this point
	if (!w_widget_is_ok(W_WIDGET(_this)))
		return;
	gtk_widget_set_can_focus(focusHandle, FALSE);
}
void _w_control_fix_style(w_control *control, _w_control_reserved *reserved) {
	GtkWidget *fixedHandle = reserved->fixedHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (fixedHandle != 0)
		reserved->fix_style_0(control, fixedHandle, reserved);
}
void _w_control_fix_style_0(w_control *control, GtkWidget *handle,
		_w_control_reserved *reserved) {
	/*
	 * Feature in GTK.  Some GTK themes apply a different background to
	 * the contents of a GtkNotebook.  However, in an SWT TabFolder, the
	 * children are not parented below the GtkNotebook widget, and usually
	 * have their own GtkFixed.  The fix is to look up the correct style
	 * for a child of a GtkNotebook and apply its background to any GtkFixed
	 * widgets that are direct children of an SWT TabFolder.
	 *
	 * Note that this has to be when the theme settings changes and that it
	 * should not override the application background.
	 */
	if ((_W_WIDGET(control)->state & STATE_BACKGROUND) != 0)
		return;
	if ((_W_WIDGET(control)->state & STATE_THEME_BACKGROUND) == 0)
		return;
#if GTK2
#endif
}
wresult _w_control_get_orientation(w_control *control) {
	return _W_WIDGET(control)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT);
}
wresult _w_control_get_text_direction(w_control *control) {
	return _W_WIDGET(control)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT);
}
wresult _w_control_has_focus(w_control *control,
		_w_control_reserved *reserved) {
	return control == _w_toolkit_get_focus_control(W_TOOLKIT(gtk_toolkit));
}
void _w_control_hook_events(w_widget *widget, _w_widget_reserved *reserved) {

	/* Connect the keyboard signals */
	GtkWidget *focusHandle = _W_CONTROL_RESERVED(reserved)->focusHandle(widget,
			reserved);
	int focusMask = GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK
			| GDK_FOCUS_CHANGE_MASK;
	gtk_widget_add_events(focusHandle, focusMask);
	_w_widget_connect(focusHandle, SIGNAL_POPUP_MENU, 0, FALSE);
	_w_widget_connect(focusHandle, SIGNAL_SHOW_HELP, 0, FALSE);
	_w_widget_connect(focusHandle, SIGNAL_KEY_PRESS_EVENT, 0, FALSE);
	_w_widget_connect(focusHandle, SIGNAL_KEY_RELEASE_EVENT, 0, FALSE);
	_w_widget_connect(focusHandle, SIGNAL_FOCUS, 0, FALSE);
	_w_widget_connect(focusHandle, SIGNAL_FOCUS_IN_EVENT, 0, FALSE);
	_w_widget_connect(focusHandle, SIGNAL_FOCUS_OUT_EVENT, 0, FALSE);
	/* Connect the mouse signals */
	GtkWidget *eventHandle = _W_CONTROL_RESERVED(reserved)->eventHandle(widget,
			reserved);
	int eventMask = GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK | GDK_SCROLL_MASK
			| GDK_SMOOTH_SCROLL_MASK;
	gtk_widget_add_events(eventHandle, eventMask);
	_w_widget_connect(eventHandle, SIGNAL_BUTTON_PRESS_EVENT, 0, FALSE);
	_w_widget_connect(eventHandle, SIGNAL_BUTTON_RELEASE_EVENT, 0, FALSE);
	_w_widget_connect(eventHandle, SIGNAL_MOTION_NOTIFY_EVENT, 0, FALSE);
	_w_widget_connect(eventHandle, SIGNAL_SCROLL_EVENT, 0, FALSE);

	/* Connect enter/exit signals */
	GtkWidget *enterExitHandle = _W_CONTROL_RESERVED(reserved)->enterExitHandle(
			widget, reserved);
	int enterExitMask = GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK;
	gtk_widget_add_events(enterExitHandle, enterExitMask);
	_w_widget_connect(enterExitHandle, SIGNAL_ENTER_NOTIFY_EVENT, 0, FALSE);
	_w_widget_connect(enterExitHandle, SIGNAL_LEAVE_NOTIFY_EVENT, 0, FALSE);
	/*
	 * Feature in GTK.  Events such as mouse move are propagate up
	 * the widget hierarchy and are seen by the parent.  This is the
	 * correct GTK behavior but not correct for SWT.  The fix is to
	 * hook a signal after and stop the propagation using a negative
	 * event number to distinguish this case.
	 *
	 * The signal is hooked to the fixedHandle to catch events sent to
	 * lightweight widgets.
	 */
	GtkWidget *fixedHandle = _W_CONTROL_RESERVED(reserved)->fixedHandle(widget,
			reserved);
	GtkWidget *blockHandle = fixedHandle != 0 ? fixedHandle : eventHandle;
	_w_widget_connect(blockHandle, SIGNAL_BUTTON_PRESS_EVENT, 0, TRUE);
	_w_widget_connect(blockHandle, SIGNAL_BUTTON_RELEASE_EVENT, 0, TRUE);
	_w_widget_connect(blockHandle, SIGNAL_MOTION_NOTIFY_EVENT, 0, TRUE);
	/* Connect the event_after signal for both key and mouse */
	_w_widget_connect(eventHandle, SIGNAL_EVENT_AFTER, 0, FALSE);
	if (focusHandle != eventHandle) {
		_w_widget_connect(focusHandle, SIGNAL_EVENT_AFTER, 0, FALSE);
	}

	/* Connect the paint signal */
	GtkWidget *paintHandle = _W_CONTROL_RESERVED(reserved)->paintHandle(widget,
			reserved);
	int paintMask = GDK_EXPOSURE_MASK;
	gtk_widget_add_events(paintHandle, paintMask);
	_w_widget_connect(paintHandle, SIGNAL_EXPOSE_EVENT_INVERSE, 0, FALSE);
	_w_widget_connect(paintHandle, SIGNAL_EXPOSE_EVENT, 0, TRUE);
	/* Connect the Input Method signals */
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_REALIZE, 0,
	TRUE);
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_UNREALIZE, 0,
	FALSE);
	GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
			reserved);
	if (imHandle != 0) {
		if (gtk_toolkit->closures[SIGNAL_COMMIT].signal_id == 0) {
			gtk_toolkit->closures[SIGNAL_COMMIT].signal_id = g_signal_lookup(
					"commit", GTK_TYPE_IM_CONTEXT);
			gtk_toolkit->closures[SIGNAL_PREEDIT_CHANGED].signal_id =
					g_signal_lookup("preedit-changed", GTK_TYPE_IM_CONTEXT);
		}
		_w_widget_connect((GtkWidget*) imHandle, SIGNAL_COMMIT, 0, FALSE);
		_w_widget_connect((GtkWidget*) imHandle, SIGNAL_PREEDIT_CHANGED, 0,
		FALSE);
	}
	_w_widget_connect(paintHandle, SIGNAL_STYLE_SET, 0, FALSE);
	GtkWidget *topHandle = reserved->topHandle(widget, reserved);
	_w_widget_connect(topHandle, SIGNAL_MAP, 0, TRUE);
	_w_widget_connect(topHandle, SIGNAL_DESTROY, 0, TRUE);
#if GTK3
	if (enterNotifyEventSignalId == 0 && GTK_VERSION < VERSION(3, 11, 9)) {
		enterNotifyEventSignalId = g_signal_lookup("enter-notify-event",
		GTK_TYPE_WIDGET);
		GTK_POINTER_WINDOW = g_quark_from_string("gtk-pointer-window");
		SWT_GRAB_WIDGET = g_quark_from_string("swt-grab-widget");
	}
#endif
}
void* _w_control_hover_proc(GtkWidget *widget) {
	gint x = 0, y = 0;
	GdkModifierType mask;
	_gdk_window_get_device_position(0, &x, &y, &mask);
	//sendMouseEvent(SWT.MouseHover, 0, /*time*/0, x[0], y[0], false, mask[0]);
	/* Always return zero in order to cancel the hover timer */
	return 0;
}
GtkWidget* _w_control_topHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	GtkWidget *fixedHandle = _W_CONTROL_RESERVED(reserved)->fixedHandle(control,
			reserved);
	if (fixedHandle != 0)
		return fixedHandle;
	return _W_WIDGET(control)->handle;
}

GtkWidget* _w_control_paintHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	GtkWidget *topHandle = reserved->topHandle(W_WIDGET(control), reserved);
	GtkWidget *paintHandle = _W_WIDGET(control)->handle;
	while (paintHandle != topHandle) {
		if (gtk_widget_get_has_window(paintHandle))
			break;
		paintHandle = gtk_widget_get_parent(paintHandle);
	}
	return paintHandle;
}
GdkWindow* _w_control_paintWindow(w_widget *control,
		_w_widget_reserved *reserved) {
	GtkWidget *paintHandle = _W_CONTROL_RESERVED(reserved)->paintHandle(control,
			reserved);
	gtk_widget_realize(paintHandle);
	return gtk_widget_get_window(paintHandle);
}
wresult _w_control_print(w_control *control, w_graphics *gc) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_graphics_is_ok(gc))
		return W_ERROR_INVALID_ARGUMENT;
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	GtkWidget *topHandle = reserved->widget.topHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	gtk_widget_realize(topHandle);
#if GTK3
	gtk_widget_draw(topHandle, _W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#endif
#if GTK2
#endif
}

w_widget* _w_control_compute_tab_group(w_control *control,
		_w_control_reserved *reserved) {
	if (reserved->is_tab_group(control, reserved) > 0)
		return W_WIDGET(control);
	w_control *parent = W_CONTROL(_W_CONTROL(control)->parent);
	_w_control_reserved *preserved = _W_CONTROL_GET_RESERVED(parent);
	return preserved->compute_tab_group(parent, preserved);
}
/*w_result _w_control_compute_tab_list(w_control *control,
 _w_control_reserved *reserved) {
 if (isTabGroup()) {
 if (getVisible() && getEnabled()) {
 return new Widget[] {this};
 }
 }
 return new Widget[0];
 }*/
w_control* _w_control_compute_tab_root(w_control *control,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->state & STATE_TAB_LIST) == 0) {
		if (reserved->is_tab_group(control, reserved) > 0) {
			return control;
		}
	}
	w_control *parent = W_CONTROL(_W_CONTROL(control)->parent);
	_w_control_reserved *preserved = _W_CONTROL_GET_RESERVED(parent);
	return preserved->compute_tab_root(parent, preserved);
}
void _w_control_check_buffered(w_control *control,
		_w_control_reserved *reserved) {
	_W_WIDGET(control)->style |= W_DOUBLE_BUFFERED;
}
void _w_control_check_background(w_control *control,
		_w_control_reserved *reserved) {
	w_shell *shell;
	W_CONTROL_GET_CLASS(control)->get_shell(control, &shell);
	if (control == W_CONTROL(shell))
		return;
	_W_WIDGET(control)->state &= ~STATE_PARENT_BACKGROUND;
	w_composite *composite = _W_CONTROL(control)->parent;
	do {
		int mode = _W_COMPOSITE(composite)->backgroundMode;
		if (mode != W_INHERIT_NONE
				|| _W_CONTROL(control)->backgroundAlpha == 0) {
			if (mode == W_INHERIT_DEFAULT
					|| _W_CONTROL(control)->backgroundAlpha == 0) {
				w_control *_control = control;
				do {
					if ((_W_WIDGET(_control)->state & STATE_THEME_BACKGROUND)
							== 0) {
						return;
					}
					_control = W_CONTROL(_W_CONTROL(_control)->parent);
				} while (_control != W_CONTROL(composite));
			}
			_W_WIDGET(control)->state |= STATE_PARENT_BACKGROUND;
			return;
		}
		if (composite == W_COMPOSITE(shell))
			break;
		composite = _W_CONTROL(composite)->parent;
	} while (1);
}
void _w_control_check_foreground(w_control *control,
		_w_control_reserved *reserved) {
	/*
	 * Feature in GTK 3. The widget foreground is inherited from the immediate
	 * parent. This is not the expected behavior for SWT. The fix is to avoid
	 * the inheritance by explicitly setting the default foreground on the widget.
	 *
	 * This can be removed on GTK3.16+.
	 */
	if (GTK_VERSION < VERSION(3, 16, 0)) {
		GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
				W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
		GdkRGBA rgba;
		w_color color = w_toolkit_get_system_color(
				w_widget_get_toolkit(W_WIDGET(control)),
				W_COLOR_WIDGET_FOREGROUND);
		rgba.alpha = W_ALPHA(color) / 255.;
		rgba.red = W_RED(color) / 255.;
		rgba.green = W_GREEN(color) / 255.;
		rgba.blue = W_BLUE(color) / 255.;
		reserved->set_foreground_color(control, topHandle, &rgba, reserved);
	}
}
void _w_control_check_border(w_control *control,
		_w_control_reserved *reserved) {
	if (W_CONTROL_GET_CLASS(control)->get_border_width(control) == 0)
		_W_WIDGET(control)->style &= ~W_BORDER;
}
void _w_control_check_mirrored(w_control *control,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0)
		_W_WIDGET(control)->style |= W_MIRRORED;
}
GtkStyle* _w_control_child_style(w_control *control,
		_w_control_reserved *reserved) {
	w_composite *parent = _W_CONTROL(control)->parent;
	_w_control_reserved *preserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(parent)));
	return preserved->child_style(W_CONTROL(parent), preserved);
}
wresult _w_control_create_widget(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	_W_WIDGET(widget)->state |= STATE_DRAG_DETECT;
	reserved->check_orientation(widget, 0, reserved);
	wresult ret = _w_widget_create_widget(widget, index, reserved);
	if (ret < 0)
		return ret;
	struct _w_widget_class *clazz = _W_WIDGET(widget)->clazz;
	*((void**) &((char*) widget)[clazz->object_used_size]) = 0;
	_w_control_reserved *creserved = _W_CONTROL_RESERVED(reserved);
	creserved->check_background(W_CONTROL(widget), creserved);
	creserved->check_foreground(W_CONTROL(widget), creserved);
	if ((_W_WIDGET(widget)->state & STATE_PARENT_BACKGROUND) != 0)
		creserved->set_parent_background(W_CONTROL(widget), creserved);
	creserved->check_buffered(W_CONTROL(widget), creserved);
	creserved->show_widget(W_CONTROL(widget), creserved);
	creserved->set_initial_bounds(W_CONTROL(widget), creserved);
	creserved->set_ZOrder(W_CONTROL(widget), 0, ZORDER_FIX_CHILDREN, creserved);
	creserved->set_relations(W_CONTROL(widget), creserved);
	creserved->check_mirrored(W_CONTROL(widget), creserved);
	creserved->check_border(W_CONTROL(widget), creserved);
	return ret;
}
int _w_control_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_widget_reserved *reserved) {
	return _w_control_compute_native_size(widget, _W_WIDGET(widget)->handle, e,
			reserved);
}
int _w_control_compute_native_size(w_widget *widget, GtkWidget *h,
		w_event_compute_size *e, _w_widget_reserved *reserved) {
	int width = e->wHint, height = e->hHint;
#if GTK3
	if (e->wHint == W_DEFAULT && e->hHint == W_DEFAULT) {
		GtkRequisition requisition;
		gtk_widget_get_preferred_size(h, NULL, &requisition);
		width = requisition.width;
		height = requisition.height;
	} else if (e->wHint == W_DEFAULT || e->hHint == W_DEFAULT) {
		int natural_size;
		if (e->wHint == W_DEFAULT) {
			gtk_widget_get_preferred_width_for_height(h, height, NULL,
					&natural_size);
			width = natural_size;
		} else {
			gtk_widget_get_preferred_height_for_width(h, width, NULL,
					&natural_size);
			height = natural_size;
		}
	}
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
#else
#endif
}
void _w_control_force_resize(w_control *control,
		_w_control_reserved *reserved) {
	/*
	 * Force size allocation on all children of this widget's
	 * topHandle.  Note that all calls to gtk_widget_size_allocate()
	 * must be preceded by a call to gtk_widget_size_request().
	 */
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	GtkRequisition requisition;
	gtk_widget_size_request(topHandle, &requisition);
	GtkAllocation allocation;
	gtk_widget_get_allocation(topHandle, &allocation);
	gtk_widget_size_allocate(topHandle, &allocation);
}
wresult _w_control_get_accessible(w_control *control,
		w_accessible **accessible) {
	*accessible = 0;
	return 0;
}
wresult _w_control_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(control));
	GtkWidget *topHandle = reserved->topHandle(W_WIDGET(control), reserved);
	GtkAllocation allocation;
	gtk_widget_get_allocation(topHandle, &allocation);
	int width =
			(_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0 ?
					0 : allocation.width;
	if (size != 0) {
		size->width = width;
		size->height =
				(_W_WIDGET(control)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
	}
	if (location != 0) {
		location->x = allocation.x;
		location->y = allocation.y;

		w_control *parent = W_CONTROL(_W_CONTROL(control)->parent);
		if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
			_w_control_reserved *preserved = _W_CONTROL_GET_RESERVED(parent);
			location->x = preserved->get_client_width(parent, preserved) - width
					- location->x;
		}
	}
	return W_TRUE;
}
wresult _w_control_set_bounds(w_control *control, w_point *location,
		w_size *size) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	wresult result = reserved->set_bounds(control, location, size, reserved);
	return result;
}
void _w_control_mark_layout(w_control *control, int flags,
		_w_control_reserved *reserved) {
}
void _w_control_modify_style(w_control *control, GtkWidget *handle,
		GtkRcStyle *style, _w_control_reserved *reserved) {
	gtk_widget_modify_style(handle, style);
	/*
	 * Bug in GTK.  When changing the style of a control that
	 * has had a region set on it, the region is lost.  The
	 * fix is to set the region again.
	 */
	w_region region;
	w_region_init(&region);
	W_CONTROL_GET_CLASS(control)->get_region(control, &region);
	if (w_region_is_ok(&region)) {
		GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
				W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
		gdk_window_shape_combine_region(gtk_widget_get_window(topHandle),
		_W_REGION(&region)->handle, 0, 0);
	}
	w_region_dispose(&region);
}
void _w_control_move_handle(w_control *control, w_point *position,
		_w_control_reserved *reserved) {
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	/* _w_composite_reserved *reserved0 = _W_COMPOSITE_RESERVED(
	 _w_widget_reserved(parent));
	 GtkWidget *parentHandle = reserved0->parenting_handle(parent);*/
	/*
	 * Feature in GTK.  Calling gtk_fixed_move() to move a child causes
	 * the whole parent to redraw.  This is a performance problem. The
	 * fix is temporarily make the parent not visible during the move.
	 *
	 * NOTE: Because every widget in SWT has an X window, the new and
	 * old bounds of the child are correctly redrawn.
	 */
	_w_fixed_move(topHandle, position->x, position->y);
}
void _w_control_resize_handle(w_control *control, w_size *size,
		_w_control_reserved *reserved) {
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	/*gtk_widget_set_size_request(topHandle, size->width, size->height);
	 if (topHandle != _W_WIDGET(control)->handle)
	 gtk_widget_set_size_request(_W_WIDGET(control)->handle, size->width,
	 size->height);*/
	_w_fixed_resize(topHandle, size->width, size->height);
	/*if (topHandle != _W_WIDGET(control)->handle) {
	 GtkWidget* w1 = _W_WIDGET(control)->handle, *w2 = 0;
	 while (w2 != topHandle) {
	 if (_W_IS_FIXED(w1)) {
	 _w_fixed_resize(w1, w2, size->width, size->height);
	 }else{
	 gtk_widget_set_size_request(w1,size->width, size->height);
	 }
	 w2 = w1;
	 w1 = gtk_widget_get_parent(w1);
	 }
	 }*/
}
wresult _w_control_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_reserved *reserved) {
	wresult result = 0;
	w_point _location;
	w_size _size;
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	GdkWindow *redrawWindow = reserved->redrawWindow(W_WIDGET(control),
			reserved);
	GdkWindow *enableWindow = reserved->enableWindow(W_WIDGET(control),
			reserved);
	int sendMove = location != 0;
	w_composite *parent = _W_CONTROL(control)->parent;
	GtkAllocation allocation;
	gtk_widget_get_allocation(topHandle, &allocation);
	if (location != 0) {
		memcpy(&_location, location, sizeof(w_point));
	} else
		memset(&_location, 0, sizeof(w_point));
	if (size != 0) {
		memcpy(&_size, size, sizeof(w_size));
	} else
		memset(&_size, 0, sizeof(w_size));
	if (parent != 0 && (_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		_w_control_reserved *preserved = _W_CONTROL_GET_RESERVED(parent);
		int clientWidth = preserved->get_client_width(W_CONTROL(parent),
				preserved);
		int oldWidth =
				(_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int oldX = clientWidth - oldWidth - allocation.x;
		if (location != 0) {
			sendMove &= location->x != oldX;
			_location.x = clientWidth - (size != 0 ? size->width : oldWidth)
					- location->x;
		} else {
			location = (w_point*) control; // just for update location
			_location.x = clientWidth - (size != 0 ? size->width : oldWidth)
					- oldX;
			_location.y = allocation.y;
		}
	}
	int sameOrigin = W_TRUE, sameExtent = W_TRUE;
	if (location != 0) {
		int oldX = allocation.x;
		int oldY = allocation.y;
		sameOrigin = _location.x == oldX && _location.y == oldY;
		if (!sameOrigin) {
			if (enableWindow != 0) {
				gdk_window_move(enableWindow, _location.x, _location.y);
			}
			reserved->move_handle(control, &_location, reserved);
		}
	}
	int clientWidth = 0;
	if (size != 0) {
		int oldWidth =
				(_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		int oldHeight =
				(_W_WIDGET(control)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
		sameExtent = _size.width == oldWidth && _size.height == oldHeight;
		if (!sameExtent && (_W_WIDGET(control)->style & W_MIRRORED) != 0)
			clientWidth = reserved->get_client_width(control, reserved);
		if (!sameExtent && !(_size.width == 0 && _size.height == 0)) {
			w_size new_sz;
			new_sz.width = MAX(1, _size.width);
			new_sz.height = MAX(1, _size.height);
			if (redrawWindow != 0) {
				gdk_window_resize(redrawWindow, new_sz.width, new_sz.height);
			}
			if (enableWindow != 0) {
				gdk_window_resize(enableWindow, new_sz.width, new_sz.height);
			}
			reserved->resize_handle(control, &new_sz, reserved);
		}
	}
	if (!sameOrigin || !sameExtent) {
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
		GtkAllocation _allocation;
		gtk_widget_get_allocation(topHandle, &_allocation);
		if (location != 0) {
			_allocation.x = _location.x;
			_allocation.y = _location.y;
		}
		if (size != 0) {
			_allocation.width = _size.width;
			_allocation.height = _size.height;
		}
		if (GTK_VERSION >= VERSION(3, 8, 0)
				&& !gtk_widget_get_visible(_W_WIDGET(control)->handle)) {
			gtk_widget_show(_W_WIDGET(control)->handle);
#if GTK3
			gtk_widget_get_preferred_size(topHandle, &requisition, NULL);
#else
		gtk_widget_size_request(topHandle, &requisition);
#endif
			gtk_widget_size_allocate(topHandle, &_allocation);
			gtk_widget_hide(_W_WIDGET(control)->handle);
		} else {
			gtk_widget_size_allocate(topHandle, &_allocation);
		}
	}
	/*
	 * Bug in GTK.  Widgets cannot be sized smaller than 1x1.
	 * The fix is to hide zero-sized widgets and show them again
	 * when they are resized larger.
	 */
	if (!sameExtent) {

		_W_WIDGET(control)->state =
				(_size.width == 0) ?
						_W_WIDGET(control)->state | STATE_ZERO_WIDTH :
						_W_WIDGET(control)->state & ~STATE_ZERO_WIDTH;
		_W_WIDGET(control)->state =
				(_size.height == 0) ?
						_W_WIDGET(control)->state | STATE_ZERO_HEIGHT :
						_W_WIDGET(control)->state & ~STATE_ZERO_HEIGHT;
		if ((_W_WIDGET(control)->state & (STATE_ZERO_WIDTH | STATE_ZERO_HEIGHT))
				!= 0) {
			if (enableWindow != 0) {
				gdk_window_hide(enableWindow);
			}
			gtk_widget_hide(topHandle);
		} else {
			if ((_W_WIDGET(control)->state & STATE_HIDDEN) == 0) {
				if (enableWindow != 0) {
					gdk_window_show_unraised(enableWindow);
				}
				gtk_widget_show(topHandle);
			}
		}

		if ((_W_WIDGET(control)->style & W_MIRRORED) != 0)
			reserved->move_children(control, clientWidth, reserved);
	}
	if (location != 0 && !sameOrigin) {
		w_control *_control = reserved->find_background_control(control,
				reserved);
		if (_control != 0 && _W_CONTROL(_control)->backgroundImage != 0) {
			if (W_CONTROL_GET_CLASS(control)->is_visible(control) > 0) {
				w_rect tmp;
				memset(&tmp, 0, sizeof(w_rect));
				reserved->redraw_widget(control, &tmp,
				REDRAW_ALL | REDRAW_REDRAWALL | REDRAW_TRIM, reserved);
			}
		}
		if (sendMove) {
			w_event e;
			e.type = W_EVENT_MOVE;
			e.platform_event = 0;
			e.data = 0;
			e.widget = W_WIDGET(control);
			_w_widget_send_event(W_WIDGET(control), &e);
		}
		result |= 1;
	}
	if (size != 0 && !sameExtent) {
		w_event e;
		e.type = W_EVENT_RESIZE;
		e.platform_event = 0;
		e.data = 0;
		e.widget = W_WIDGET(control);
		_w_widget_send_event(W_WIDGET(control), &e);
		result |= 2;
	}
	return result;
}
wresult _w_control_set_region_0(w_control *control,
		cairo_region_t *shape_region) {
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(control));
	GtkWidget *topHandle = reserved->topHandle(W_WIDGET(control), reserved);
	GdkWindow *window = gtk_widget_get_window(topHandle);
	gdk_window_shape_combine_region(window, shape_region, 0, 0);
	return W_TRUE;
}
wresult _w_control_set_region(w_control *control, w_region *region) {
	if (region != 0 && !w_region_is_ok(region))
		return W_ERROR_INVALID_ARGUMENT;
	cairo_region_t *shape_region = region == 0 ? 0 : _W_REGION(region)->handle;
	return _w_control_set_region_0(control, shape_region);
}
void _w_control_set_relations(w_control *control,
		_w_control_reserved *reserved) {
	w_composite *parent = _W_CONTROL(control)->parent;
	_w_composite_reserved *preserved = _W_COMPOSITE_GET_RESERVED(parent);
	_w_fixed *parentHandle = (_w_fixed*) preserved->parenting_handle(parent,
			_W_WIDGET_RESERVED(reserved));
	if (parentHandle->first != 0) {
		_w_fixed *last = parentHandle->first->prev;
		w_widget *widget = _GTK_GET_WIDGET(last);
		if (widget != 0 && widget != W_WIDGET(control)) {
			if (w_widget_class_id(widget) >= _W_CLASS_CONTROL) {
				_w_control_reserved *wreserved = _W_CONTROL_GET_RESERVED(
						widget);
				wreserved->add_relation(control, W_CONTROL(widget), wreserved);
			}
		}
	}
}
/*
 * Answers a boolean indicating whether a Label that precedes the receiver in
 * a layout should be read by screen readers as the recevier's label.
 */
wresult _w_control_is_described_by_label(w_control *control,
		_w_control_reserved *reserved) {
	return W_TRUE;
}

gboolean _w_control_is_focus_handle(w_control *control, GtkWidget *widget,
		_w_control_reserved *reserved) {
	return widget
			== reserved->focusHandle(W_WIDGET(control),
					_W_WIDGET_RESERVED(reserved));
}
wresult _w_control_move_above(w_control *control, w_control *_control) {
	if (_control != 0) {
		if (!w_widget_is_ok(W_WIDGET(_control)))
			return W_ERROR_INVALID_ARGUMENT;
		if (control == _control)
			return W_FALSE;
		w_composite *_parent;
		w_control_get_parent(_control, &_parent);
		w_composite *parent;
		w_control_get_parent(control, &parent);
		if (parent != _parent)
			return W_FALSE;
	}
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	return reserved->set_ZOrder(control, _control,
	ZORDER_ABOVE | ZORDER_FIX_RELATION | ZORDER_FIX_CHILDREN, reserved);
}
wresult _w_control_move_below(w_control *control, w_control *_control) {
	if (_control != 0) {
		if (!w_widget_is_ok(W_WIDGET(_control)))
			return W_ERROR_INVALID_ARGUMENT;
		if (control == _control)
			return W_FALSE;
		w_composite *_parent;
		w_control_get_parent(_control, &_parent);
		w_composite *parent;
		w_control_get_parent(control, &parent);
		if (parent != _parent)
			return W_FALSE;
	}
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	return reserved->set_ZOrder(control, _control,
	ZORDER_FIX_RELATION | ZORDER_FIX_CHILDREN, reserved);
}
void _w_control_move_children(w_control *control, int oldWidth,
		_w_control_reserved *reserved) {

}
wresult _w_control_pack(w_control *control, wbool changed) {
	w_size size;
	w_control_compute_size_0(control, &size, W_DEFAULT, W_DEFAULT, changed);
	return w_control_set_bounds(control, 0, &size);
}
wresult _w_control_new_layout_data(w_control *control, void **data,
		size_t size) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		void *layout_data =
				*((void**) &((char*) control)[clazz->object_used_size]);
		if (layout_data != 0) {
			free(layout_data);
		}
	}
	if ((clazz->object_used_size + size) < clazz->object_total_size) {
		_W_WIDGET(control)->state |= STATE_LAYOUT_DATA_LOCALE;
		*data = (void*) &((char*) control)[clazz->object_used_size];
	} else {
		_W_WIDGET(control)->state &= ~STATE_LAYOUT_DATA_LOCALE;
		*data = malloc(size);
		if (*data == 0)
			return W_ERROR_NO_MEMORY;
		*((void**) &((char*) control)[clazz->object_used_size]) = *data;
	}
	return W_TRUE;
}
wresult _w_control_to_control(w_control *control, w_point *result,
		w_point *point) {
	result->x = point->x;
	result->y = point->y;
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	GdkWindow *window = reserved->eventWindow(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	int origin_x = 0, origin_y = 0;
	gdk_window_get_origin(window, &origin_x, &origin_y);
	result->x -= origin_x;
	result->y -= origin_y;
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0)
		result->x = reserved->get_client_width(control, reserved) - result->x;
	return W_TRUE;
}
wresult _w_control_to_display(w_control *control, w_point *result,
		w_point *point) {
	result->x = point->x;
	result->y = point->y;
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	GdkWindow *window = reserved->eventWindow(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	int origin_x = 0, origin_y = 0;
	gdk_window_get_origin(window, &origin_x, &origin_y);
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0)
		result->x = reserved->get_client_width(control, reserved) - result->x;
	result->x -= origin_x;
	result->y -= origin_y;
	return W_TRUE;
}
void _w_control_add_relation(w_control *_this, w_control *control,
		_w_control_reserved *reserved) {
}
void _w_control_remove_relation(w_control *control,
		_w_control_reserved *reserved) {
	if (reserved->is_described_by_label(control, reserved) <= 0)
		return; /* there will not be any */
	/*if (labelRelation != null) {
	 _getAccessible().removeRelation(ACC.RELATION_LABELLED_BY,
	 labelRelation._getAccessible());
	 labelRelation = null;
	 }*/
}
wresult _w_control_drag_detect_0(w_control *control, w_event *event) {
	if (event == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (event->type >= W_EVENT_MOUSEDOWN
			&& event->type <= W_EVENT_MOUSEDOUBLECLICK) {
		return _w_control_drag_detect(control, (w_event_mouse*) event);
	} else
		return W_FALSE;
}

gboolean _w_control_drag_detect_2(w_control *control, int x, int y,
		gboolean filter, gboolean dragOnTimeout, gboolean *consume) {
	gboolean quit = FALSE, dragging = FALSE;

	//428852 DND workaround for GTk3.
	//Gtk3 no longer sends motion events on the same control during thread sleep
	//before a drag started. This is due to underlying gdk changes.
	//Thus for gtk3 we check mouse coords manually
	//Note, input params x/y are relative, the two points below are absolute coords.
	w_point startPos;
	w_point currPos;
#if GTK3
	_w_toolkit_get_cursor_location(W_TOOLKIT(gtk_toolkit), &startPos);
#endif

	while (!quit) {
		GdkEvent *eventPtr = 0;
		/*
		 * There should be an event on the queue already, but
		 * in cases where there isn't one, stop trying after
		 * half a second.
		 */
		struct timeval tp;
		gettimeofday(&tp, NULL);
		uint64_t timecurrent = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		uint64_t timeout = timecurrent + 500;
		while (timecurrent < timeout) {
			eventPtr = gdk_event_get();
			if (eventPtr != 0) {
				break;
			} else {
#if GTK3 //428852
				_w_toolkit_get_cursor_location(W_TOOLKIT(gtk_toolkit),
						&currPos);
				dragging = gtk_drag_check_threshold(_W_WIDGET(control)->handle,
						startPos.x, startPos.y, currPos.x, currPos.y);
				if (dragging)
					break;
#endif
#if GTK2
#endif
			}
			gettimeofday(&tp, 0);
			timecurrent = tp.tv_sec * 1000 + tp.tv_usec / 1000;
		}
		if (dragging)
			return TRUE;  //428852
		if (eventPtr == 0)
			return dragOnTimeout;
		switch (eventPtr->type) {
		case GDK_MOTION_NOTIFY: {
			GdkEventMotion *gdkMotionEvent = (GdkEventMotion*) eventPtr;
			if ((gdkMotionEvent->state & GDK_BUTTON1_MASK) != 0) {
				if (gtk_drag_check_threshold(_W_WIDGET(control)->handle, x, y,
						(int) gdkMotionEvent->x, (int) gdkMotionEvent->y)) {
					dragging = TRUE;
					quit = TRUE;
				}
			} else {
				quit = TRUE;
			}
			int newX = 0, newY = 0;
			_gdk_window_get_device_position(gdkMotionEvent->window, &newX,
					&newY, 0);
			break;
		}
		case GDK_KEY_PRESS:
		case GDK_KEY_RELEASE: {
			GdkEventKey *gdkEvent = (GdkEventKey*) eventPtr;
			if (gdkEvent->keyval == GDK_KEY_Escape)
				quit = TRUE;
			break;
		}
		case GDK_BUTTON_RELEASE:
		case GDK_BUTTON_PRESS:
		case GDK_2BUTTON_PRESS:
		case GDK_3BUTTON_PRESS: {
			gdk_event_put(eventPtr);
			quit = TRUE;
			break;
		}
		default:
			gtk_main_do_event(eventPtr);
		}
		gdk_event_free(eventPtr);
	}
	return dragging;
}
gboolean _w_control_drag_detect_1(w_control *control, int button, int count,
		int stateMask, int x, int y, _w_control_reserved *reserved) {
	if (button != 1 || count != 1)
		return FALSE;
	if (!_w_control_drag_detect_2(control, x, y, FALSE, TRUE, 0))
		return FALSE;
	w_event_mouse event;
	event.event.type = W_EVENT_DRAGDETECT;
	event.event.widget = W_WIDGET(control);
	event.event.platform_event = 0;
	event.event.time = 0;
	event.event.data = 0;
	event.button = button;
	event.x = x;
	event.y = y;
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0) {
		event.x = reserved->get_client_width(control, reserved) - event.x;
	}
	event.statemask = stateMask;
	return _w_widget_send_event(W_WIDGET(control), (w_event*) &event);
}
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event) {
	if (event == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	return _w_control_drag_detect_1(control, event->button, event->clickcount,
			event->statemask, event->x, event->y, reserved);
}
gboolean _w_control_filter_key(w_control *control, int keyval,
		GdkEventKey *event, _w_control_reserved *reserved) {
	GtkIMContext *imHandle = reserved->imHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (imHandle != 0) {
		return gtk_im_context_filter_keypress(imHandle, event);
	}
	return FALSE;
}
w_control* _w_control_find_background_control(w_control *control,
		_w_control_reserved *reserved) {
	if (((_W_WIDGET(control)->state & STATE_BACKGROUND) != 0
			|| _W_CONTROL(control)->backgroundImage != 0)
			&& _W_CONTROL(control)->backgroundAlpha > 0)
		return control;
	if ((_W_WIDGET(control)->state & STATE_PARENT_BACKGROUND) != 0) {
		_w_control_reserved *preserved = _W_CONTROL_RESERVED(
				_w_widget_get_reserved( W_WIDGET(_W_CONTROL(control)->parent)));
		return preserved->find_background_control(
				W_CONTROL(_W_CONTROL(control)->parent), preserved);
	} else
		return 0;
}
/*Menu [] findMenus (Control control) {
 if (menu != null && this != control) return new Menu [] {menu};
 return new Menu [0];
 }
 void _w_control_fix_children (Shell newShell, Shell oldShell, Decorations newDecorations, Decorations oldDecorations, Menu [] menus) {
 oldShell.fixShell (newShell, this);
 oldDecorations.fixDecorations (newDecorations, this, menus);
 }*/
void _w_control_fixed_map_proc_callback(GtkWidget *child, gpointer data) {
	if (gtk_widget_get_visible(child) && gtk_widget_get_child_visible(child)
			&& !gtk_widget_get_mapped(child)) {
		gtk_widget_map(child);
	}
}
long _w_control_fixed_map_proc(GtkWidget *widget) {
	gtk_widget_set_mapped(widget, TRUE);
	gtk_container_forall(GTK_CONTAINER(widget),
			_w_control_fixed_map_proc_callback, 0);
	if (gtk_widget_get_has_window(widget)) {
		gdk_window_show_unraised(gtk_widget_get_window(widget));
	}
	return 0;
}
void _w_control_fix_modal(w_control *control, GtkWindowGroup *group,
		GtkWindowGroup *modalGroup, _w_control_reserved *reserved) {
}
gboolean _w_control_force_focus_0(w_control *control, GtkWidget *focusHandle,
		_w_widget_reserved *reserved) {
	if (gtk_widget_has_focus(focusHandle))
		return TRUE;
	gtk_widget_set_can_focus(focusHandle, TRUE);
	/* When the control is zero sized it must be realized */
	gtk_widget_realize(focusHandle);
	gtk_widget_grab_focus(focusHandle);
// widget could be disposed at this point
	if (!w_widget_is_ok(W_WIDGET(control)))
		return FALSE;
	w_shell *shell;
	w_control_get_shell(control, &shell);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	GtkWidget *handle = gtk_window_get_focus(GTK_WINDOW(shellHandle));
	while (handle != 0) {
		if (handle == focusHandle) {
			/* Cancel any previous ignoreFocus requests */
			gtk_toolkit->ignoreFocus = FALSE;
			return TRUE;
		}
		w_widget *widget = g_object_get_qdata(G_OBJECT(handle),
				gtk_toolkit->quark[0]);
		if (widget != 0 && w_widget_class_id(widget) >= _W_CLASS_CONTROL) {
			return widget == (w_widget*) control;
		}
		handle = gtk_widget_get_parent(handle);
	}
	return FALSE;
}

wresult _w_control_force_focus(w_control *control) {
	if (gtk_toolkit->focusEvent == W_EVENT_FOCUSOUT)
		return FALSE;
	w_shell *shell;
	w_control_get_shell(control, &shell);
	_w_shell_set_saved_focus(shell, control);
	struct _w_control_class *clazz = W_CONTROL_GET_CLASS(control);
	if (clazz->is_enabled(control) <= 0 || clazz->is_visible(control) <= 0)
		return W_FALSE;
	_w_shell_bring_totop(shell, W_FALSE);
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(control));
	return _w_control_force_focus_0(control,
	_W_CONTROL_RESERVED(reserved)->focusHandle(W_WIDGET(control), reserved),
			reserved);
}
wresult _w_control_get_background(w_control *control, w_color *background) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	if (_W_CONTROL(control)->backgroundAlpha == 0) {
		GdkColor color;
		reserved->get_background_color(control, &color, reserved);
		*background = W_RGB((color.red * 0x100), (color.green * 0x100),
				(color.blue * 0x100));
	} else {
		w_control *_control = reserved->find_background_control(control,
				reserved);
		if (_control == 0) {
			_control = control;
		} else {
			reserved = _W_CONTROL_GET_RESERVED(_control);
		}
		GdkColor color;
		reserved->get_background_color(_control, &color, reserved);
		*background = W_RGBA((color.red * 0x100), (color.green * 0x100),
				(color.blue * 0x100),
				(_W_CONTROL(control)->backgroundAlpha * 0x100));
	}
	return W_TRUE;
}
wresult _w_control_get_background_image(w_control *control, w_image *image) {
	if (image == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	w_control *_control = reserved->find_background_control(control, reserved);
	if (_control == 0) {
		_control = control;
	} else {
		reserved = _W_CONTROL_GET_RESERVED(_control);
	}
	if (_W_CONTROL(_control)->backgroundImage != 0) {
		_W_IMAGE(image)->pixbuf = gdk_pixbuf_copy(
		_W_CONTROL(_control)->backgroundImage);
		return W_TRUE;
	}
	return W_FALSE;
}

void _w_control_get_context_background(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
	if (GTK_VERSION >= VERSION(3, 16, 0)) {
		/*if (provider != 0) {
		 return display.gtk_css_parse_background(provider, null);
		 } else {*/
		w_color c = _w_toolkit_get_system_color(W_TOOLKIT(gtk_toolkit),
				W_COLOR_WIDGET_BACKGROUND);
		color->red = W_RED(c) * 0x100;
		color->green = W_GREEN(c) * 0x100;
		color->blue = W_BLUE(c) * 0x100;
		//}
	} else {
		GtkWidget *fontHandle = reserved->fontHandle(W_WIDGET(control),
				_W_WIDGET_RESERVED(reserved));
		GtkStyleContext *context = gtk_widget_get_style_context(fontHandle);
		GdkRGBA rgba;
		gtk_style_context_get_background_color(context, GTK_STATE_FLAG_NORMAL,
				&rgba);
		if (rgba.alpha == 0) {
			w_color c = _w_toolkit_get_system_color(W_TOOLKIT(gtk_toolkit),
					W_COLOR_WIDGET_BACKGROUND);
			color->red = W_RED(c) * 0x100;
			color->green = W_GREEN(c) * 0x100;
			color->blue = W_BLUE(c) * 0x100;
		} else {
			color->red = rgba.red * 0xFFFF;
			color->green = rgba.green * 0xFFFF;
			color->blue = rgba.blue * 0xFFFF;
		}
	}
}
void _w_control_get_context_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
	GtkWidget *fontHandle = reserved->fontHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	/*if (GTK_VERSION >= VERSION(3, 16, 0)) {
	 return display.gtk_css_parse_foreground(provider, null);
	 } else {*/
	GtkStyleContext *context = gtk_widget_get_style_context(fontHandle);
	GdkRGBA rgba;
	/*rgba = display.styleContextGetColor(context, OS.GTK_STATE_FLAG_NORMAL,
	 rgba);
	 return display.toGdkColor(rgba);*/
	//}
}
void _w_control_get_bg_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
#if GTK3
	_w_control_get_context_color(control, color, reserved);
#endif
#if GTK2
#endif
}
void _w_control_get_background_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
	_w_control_get_bg_color(control, color, reserved);
}
void _w_control_get_base_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
#if GTK3
	_w_control_get_context_color(control, color, reserved);
#endif
#if GTK2
#endif
}
wresult _w_control_get_border_width(w_control *control) {
	return 0;
}
int _w_control_get_client_width(w_control *control,
		_w_control_reserved *reserved) {
	GtkAllocation allocation;
	gtk_widget_get_allocation(_W_WIDGET(control)->handle, &allocation);
	return allocation.width;
}
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor) {
	*cursor = _W_CONTROL(control)->cursor;
	return W_TRUE;
}
wresult _w_control_get_drag_detect(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_DRAG_DETECT) != 0;
}
wresult _w_control_get_enabled(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_DISABLED) == 0;
}
wresult _w_control_get_font(w_control *control, w_font **font) {
	if (_W_CONTROL(control)->font != 0) {
		*font = _W_CONTROL(control)->font;
	} else {
		*font = _w_toolkit_get_system_font((w_toolkit*) gtk_toolkit);
	}
	return W_TRUE;
}
const PangoFontDescription* _w_control_get_get_font_description(
		w_control *control, _w_control_reserved *reserved) {
	GtkWidget *fontHandle = reserved->focusHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
#if GTK3
	GtkStyleContext *context = gtk_widget_get_style_context(fontHandle);
	if (GTK_VERSION < VERSION(3, 18, 0)) {
		return gtk_style_context_get_font(context, GTK_STATE_FLAG_NORMAL);
	} else {
		return gtk_style_context_get_font(context,
				gtk_widget_get_state_flags(fontHandle));
	}
#endif
#if GTK2
#endif
}
wresult _w_control_get_foreground(w_control *control, w_color *foreground) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	GdkColor color;
	reserved->get_foreground_color(control, &color, reserved);
	*foreground = W_RGB((color.red / 0x100), (color.green / 0x100),
			(color.blue / 0x100));
	return W_TRUE;
}

void _w_control_get_fg_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
#if GTK3
	_w_control_get_context_color(control, color, reserved);
#endif
#if GTK2
#endif
}
void _w_control_get_foreground_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
	_w_control_get_fg_color(control, color, reserved);
}
wresult _w_control_get_imcaret_pos(w_control *control, w_point *pos,
		_w_control_reserved *reserved) {
	pos->x = 0;
	pos->y = 0;
	return TRUE;

}

void _w_control_get_text_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
#if GTK3
	_w_control_get_context_color(control, color, reserved);
#endif
#if GTK2
#endif
}
wresult _w_control_get_layout_data(w_control *control, void **data) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(control);
	if ((_W_WIDGET(control)->state & STATE_LAYOUT_DATA_LOCALE) == 0) {
		*data = *((void**) &((char*) control)[clazz->object_used_size]);
	} else {
		*data = (void*) &((char*) control)[clazz->object_used_size];
	}
	return W_TRUE;
}
wresult _w_control_get_menu(w_control *control, w_menu **menu) {
	*menu = _W_CONTROL(control)->menu;
	return W_TRUE;
}
wresult _w_control_get_parent(w_control *control, w_composite **parent) {
	*parent = _W_CONTROL(control)->parent;
	return W_TRUE;
}
/*Control [] getPath () {
 int count = 0;
 Shell shell = getShell ();
 Control control = this;
 while (control != shell) {
 count++;
 control = control.parent;
 }
 control = this;
 Control [] result = new Control [count];
 while (control != shell) {
 result [--count] = control;
 control = control.parent;
 }
 return result;
 }
 */
wresult _w_control_get_region(w_control *control, w_region *region) {
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_region_dispose(region);
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(control));
	GtkWidget *topHandle = reserved->topHandle(W_WIDGET(control), reserved);
	GdkWindow *window = gtk_widget_get_window(topHandle);
	_W_REGION(region)->handle = gdk_window_get_clip_region(window);
	return W_TRUE;
}
wresult _w_control_get_shell(w_control *control, w_shell **shell) {
	w_composite *p = _W_CONTROL(control)->parent;
	return w_control_get_shell(W_CONTROL(p), shell);
}

wresult _w_control_get_toolTip_text(w_control *control, w_alloc alloc,
		void *user_data) {
	//return toolTipText;
	return W_TRUE;
}
wresult _w_control_get_touch_enabled(w_control *control) {
	return W_FALSE;
}
wresult _w_control_get_visible(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_HIDDEN) == 0;
}
void _w_control_get_thickness(GtkWidget *widget, w_point *thickness) {
#if GTK3
	thickness->x = 0, thickness->y = 0;
	GtkBorder tmp;
	GtkStyleContext *context = gtk_widget_get_style_context(widget);

	if (GTK_VERSION < VERSION(3, 18, 0)) {
		gtk_style_context_get_padding(context, GTK_STATE_FLAG_NORMAL, &tmp);
	} else {
		gtk_style_context_get_padding(context,
				gtk_widget_get_state_flags(widget), &tmp);
	}
	gtk_style_context_save(context);
	gtk_style_context_add_class(context, GTK_STYLE_CLASS_FRAME);
	thickness->x += tmp.left;
	thickness->y += tmp.top;
	if (GTK_VERSION < VERSION(3, 18, 0)) {
		gtk_style_context_get_border(context, GTK_STATE_FLAG_NORMAL, &tmp);
	} else {
		gtk_style_context_get_border(context,
				gtk_widget_get_state_flags(widget), &tmp);
	}
	thickness->x += tmp.left;
	thickness->y += tmp.top;
	gtk_style_context_restore(context);
	return;
#else
#endif
}
gboolean _w_control_check_subwindow(w_control *control,
		_w_control_reserved *reserved) {
	return FALSE;
}
wresult _w_control_get_graphics(w_control *control, w_graphics *gc) {
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(control));
	GdkWindow *window = _W_CONTROL_RESERVED(reserved)->paintWindow(
			W_WIDGET(control), reserved);
	if (window == 0)
		return W_ERROR_NO_HANDLES;

#if USE_CAIRO
	cairo_t *cr;
	cr = gdk_cairo_create(window);
	_w_graphics_init(gc, cr);
	w_font *font;
	w_control_get_font(W_CONTROL(control), &font);
	w_graphics_set_font(gc, font);
	w_color background;
	w_control_get_background(W_CONTROL(control), &background);
	w_graphics_set_background(gc, background);
	w_color foreground;
	w_control_get_foreground(W_CONTROL(control), &foreground);
	w_graphics_set_foreground(gc, foreground);
#else
		cr = gdk_gc_new (window);
#endif
	return W_TRUE;
}
GtkIMContext* _w_control_imHandle(w_widget *control,
		_w_widget_reserved *reserved) {
	return 0;
}
wresult _w_control_is_reparentable(w_control *control) {
	return TRUE;
}
gboolean _w_control_is_showing(w_control *control,
		_w_control_reserved *reserved) {
	/*
	 * This is not complete.  Need to check if the
	 * widget is obscurred by a parent or sibling.
	 */
	if (W_CONTROL_GET_CLASS(control)->is_visible(control) <= 0)
		return FALSE;
	w_rect size;
	w_control *_control = control;
	while (_control != 0) {
		W_CONTROL_GET_CLASS(_control)->get_bounds(_control, &size.pt,&size.sz);
		if (size.x == 0 || size.y == 0) {
			return FALSE;
		}
		_control = W_CONTROL(_W_CONTROL(_control)->parent);
	}
	return TRUE;
}
wresult _w_control_is_tab_group(w_control *control,
		_w_control_reserved *reserved) {
	if (_W_WIDGET(control)->state & STATE_TAB_LIST) {
		return W_TRUE;
	}
	int code = reserved->traversal_code(control, 0, 0, reserved);
	if ((code & (W_TRAVERSE_ARROW_PREVIOUS | W_TRAVERSE_ARROW_NEXT)) != 0)
		return W_FALSE;
	return (code & (W_TRAVERSE_TAB_PREVIOUS | W_TRAVERSE_TAB_NEXT)) != 0;
}
wresult _w_control_is_tab_item(w_control *control,
		_w_control_reserved *reserved) {
	if (_W_WIDGET(control)->state & STATE_TAB_LIST) {
		return W_FALSE;
	}
	int code = reserved->traversal_code(control, 0, 0, reserved);
	return (code & (W_TRAVERSE_ARROW_PREVIOUS | W_TRAVERSE_ARROW_NEXT)) != 0;
}
wresult _w_control_is_enabled(w_control *control) {
	if (W_CONTROL_GET_CLASS(control)->get_enabled(control) > 0) {
		w_composite *parent = _W_CONTROL(control)->parent;
		return W_CONTROL_GET_CLASS(parent)->is_enabled(W_CONTROL(parent));
	}
	return W_FALSE;
}
wresult _w_control_is_focus_ancestor(w_control *_this, w_control *control) {
	while (control != 0 && control != _this
			&& w_widget_class_id(W_WIDGET(control)) != _W_CLASS_SHELL) {
		control = (w_control*) _W_CONTROL(control)->parent;
	}
	return control == _this;
}
wresult _w_control_is_focus_control(w_control *control) {
	w_control *focusControl = gtk_toolkit->focusControl;
	if (focusControl != 0 && w_widget_is_ok(W_WIDGET(focusControl))) {
		return control == focusControl;
	}
	return _w_control_has_focus(control, _W_CONTROL_GET_RESERVED(control));
}
wresult _w_control_is_visible(w_control *control) {
	if (W_CONTROL_GET_CLASS(control)->get_visible(control) > 0) {
		w_composite *parent = _W_CONTROL(control)->parent;
		return W_CONTROL_GET_CLASS(parent)->is_visible(W_CONTROL(parent));
	}
	return W_FALSE;
}
gboolean _w_control_mnemonic_hit(w_control *control, int key,
		_w_control_reserved *reserved) {
	return FALSE;
}

gboolean _w_control_mnemonic_match(w_control *control, int key,
		_w_control_reserved *reserved) {
	return FALSE;
}
wresult _w_control_request_layout(w_control *control) {
	w_shell *shell;
	W_CONTROL_GET_CLASS(control)->get_shell(control, &shell);
	W_COMPOSITE_GET_CLASS(shell)->layout_changed(W_COMPOSITE(shell), &control,
			1, W_DEFER);
	return W_TRUE;
}
wresult _w_control_redraw(w_control *control, w_rect *r, int all) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	if (!gtk_widget_get_visible(topHandle))
		return W_FALSE;
	int flags;
	if (all) {
		flags = REDRAW_ALL;
	} else
		flags = 0;
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0 && r != 0) {
		w_rect rect;
		memcpy(&rect, r, sizeof(w_rect));
		rect.x = reserved->get_client_width(control, reserved) - rect.width
				- rect.x;
		r = &rect;
	}
	reserved->redraw_widget(control, r, flags, reserved);
	return W_TRUE;
}
void _w_control_redraw_children(w_control *control,
		_w_control_reserved *reserved) {

}
void _w_control_redraw_widget(w_control *control, w_rect *_rect, int flags,
		_w_control_reserved *reserved) {
	if (!gtk_widget_get_realized(_W_WIDGET(control)->handle))
		return;
	GdkWindow *window = reserved->paintWindow(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	GdkRectangle rect, *r = &rect;
	if (flags & REDRAW_REDRAWALL) {
		rect.width = gdk_window_get_width(window);
		rect.height = gdk_window_get_height(window);
	} else {
		if (_rect != 0) {
			rect.x = _rect->x;
			rect.y = _rect->y;
			rect.width = w_max(0, _rect->width);
			rect.height = w_max(0, _rect->height);
		} else {
			r = 0;
		}
	}
	gdk_window_invalidate_rect(window, r, flags & REDRAW_ALL);
}
gboolean _w_control_send_leave_notify(w_control *control,
		_w_control_reserved *reserved) {
	return FALSE;
}
void gtk_widget_set_align(GtkWidget *widget, GtkAlign hAlign, GtkAlign vAlign) {
	gtk_widget_set_halign(widget, hAlign);
	gtk_widget_set_valign(widget, vAlign);
}
void _w_control_set_background_0(w_control *control,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->state & STATE_BACKGROUND) == 0
			&& _W_CONTROL(control)->backgroundImage == 0) {
		if ((_W_WIDGET(control)->state & STATE_PARENT_BACKGROUND) != 0) {
			reserved->set_parent_background(control, reserved);
		} else {
			reserved->set_widget_background(control, reserved);
		}
		reserved->redraw_widget(control, 0, REDRAW_REDRAWALL, reserved);
	}
}

void _w_control_set_background_1(w_control *control, w_color color,
		_w_control_reserved *reserved) {
	if (((_W_WIDGET(control)->state & STATE_BACKGROUND) == 0) && color == 0)
		return;
	GdkColor gdkColor;
	if (color != 0) {
		gdkColor.red = W_RED(color) * 0x100;
		gdkColor.green = W_GREEN(color) * 0x100;
		gdkColor.blue = W_BLUE(color) * 0x100;
		_W_CONTROL(control)->backgroundAlpha = W_ALPHA(color);
	}
	gboolean set = FALSE;
#if GTK3
	// TODO: Some widgets like GtkLabel (and others) are transparent
	// in GTK3, so one needs to set their background. For now, always
	// set the background. See bug 421836.
	// set = !getBackground().equals(color);
	set = TRUE;
#endif
#if GTK2
#endif
	if (set) {
		if (color == 0) {
			_W_WIDGET(control)->state &= ~STATE_BACKGROUND;
			reserved->set_background_color(control, 0, reserved);
		} else {
			_W_WIDGET(control)->state |= STATE_BACKGROUND;
			reserved->set_background_color(control, &gdkColor, reserved);
		}
		reserved->redraw_children(control, reserved);
	}
}
wresult _w_control_set_background(w_control *control, w_color color) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	_w_control_set_background_1(control, color, reserved);
	if (color != 0) {
		reserved->update_background_mode(control, reserved);
	}
	return W_TRUE;
}
void _w_control_set_background_color_0(w_control *control,
		GtkStyleContext *context, GtkWidget *handle, GdkRGBA *rgba,
		_w_control_reserved *reserved) {
	/*	char *name,*css;
	 int l;
	 char tmp[0x100];
	 css = tmp;
	 if (GTK_VERSION >= VERSION(3, 16, 0)) {
	 // Form background string
	 if (GTK_VERSION >= VERSION(3, 20, 0)) {
	 name = gtk_widget_class_get_css_name(GTK_WIDGET_GET_CLASS(handle));
	 } else {
	 name = gtk_widget_get_name(handle);
	 }
	 if (name == 0)
	 name = "*";
	 l = strlen(name);
	 memcpy(css,name,l);
	 css+=l;
	 name = " {background-color: ";
	 l = strlen(name);
	 memcpy(css,name,l);
	 css+=l;
	 if(rgba == 0){
	 w_color color=w_toolkit_get_system_color(w_widget_get_toolkit(W_WIDGET(control)), W_COLOR_WIDGET_BACKGROUND);
	 GdkRGBA trgba;
	 trgba.red = W_RED(color) / 255.;
	 trgba.green = W_GREEN(color) / 255.;
	 trgba.blue = W_BLUE(color) / 255.;
	 trgba.alpha = W_ALPHA(color) / 255.;
	 rgba = &trgba;
	 }
	 name = gdk_rgba_to_string (rgba);
	 l = strlen(name);
	 memcpy(css,name,l);
	 css+=l;
	 name = ";}";
	 l = strlen(name);
	 memcpy(css,name,l);
	 css+=l;
	 } else {
	 gtk_widget_override_background_color(handle, GTK_STATE_FLAG_NORMAL,
	 rgba);
	 }*/
}

void _w_control_set_background_color_gradient(w_control *control,
		GtkStyleContext *context, GtkWidget *handle, GdkRGBA *rgba,
		_w_control_reserved *reserved) {
	/*char css[0x100];
	 char color[20];
	 if (rgba != null) {
	 String color = display.gtk_rgba_to_css_string(rgba);
	 //Note, use 'background-image' CSS class with caution. Not all themes/widgets support it. (e.g button doesn't).
	 //Use 'background' CSS class where possible instead unless 'background-image' is explicidly supported.
	 sprintf(css,
	 "* {\nbackground-image: -gtk-gradient (linear, 0 0, 0 1, color-stop(0, %s), color-stop(1, %s));\n}\n",
	 color, color);
	 } else {
	 sprintf(css, "* {\n}\n");
	 }
	 //Cache background color
	 cssBackground = css;

	 // Apply background color and any cached foreground color
	 String finalCss = display.gtk_css_create_css_color_string(cssBackground,
	 cssForeground, SWT.BACKGROUND);
	 gtk_css_provider_load_from_css(context, finalCss);*/
}

void gtk_css_provider_load_from_css(w_control *control,
		GtkStyleContext *context, const char *css) {
	/* Utility function. */
	//@param css : a 'css java' string like "{\nbackground: red;\n}".
	if (_W_CONTROL(control)->provider == 0) {
		_W_CONTROL(control)->provider = gtk_css_provider_new();
		gtk_style_context_add_provider(context,
				GTK_STYLE_PROVIDER(_W_CONTROL(control)->provider),
				GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
		g_object_unref(_W_CONTROL(control)->provider);
	}
	gtk_css_provider_load_from_data(_W_CONTROL(control)->provider, css, -1,
	NULL);
}

void _w_control_set_background_color_1(w_control *control, GtkWidget *handle,
		GdkColor *color, _w_control_reserved *reserved) {
#if GTK3
	GdkRGBA rgba;
	double alpha = 1.0;
	if (color == 0) {
		if ((_W_WIDGET(control)->state & STATE_PARENT_BACKGROUND) != 0) {
			alpha = 0;
			w_control *c = reserved->find_background_control(control, reserved);
			if (c == 0)
				c = control;
			_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(c);
			creserved->get_background_color(c, color, creserved);
		}
	} else {
		alpha = _W_CONTROL(control)->backgroundAlpha;
	}
	if (color != 0) {
		rgba.red = (color->red & 0xFFFF) / (float) 0xFFFF;
		rgba.green = (color->green & 0xFFFF) / (float) 0xFFFF;
		rgba.blue = (color->blue & 0xFFFF) / (float) 0xFFFF;
		rgba.alpha = alpha / (float) 255;
	}
	GtkStyleContext *context = gtk_widget_get_style_context(handle);
	_w_control_set_background_color_0(control, context, handle, &rgba,
			reserved);
	gtk_style_context_invalidate(context);
	return;
#endif
#if GTK2
#endif
}

void _w_control_set_background_color(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
	_w_control_set_background_color_1(control, _W_WIDGET(control)->handle,
			color, reserved);
}
wresult _w_control_set_background_image(w_control *control, w_image *image,
		_w_control_reserved *reserved) {
	if (image != 0 && !w_image_is_ok(image))
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_CONTROL(control)->backgroundAlpha > 0)
		return W_TRUE;
	_W_CONTROL(control)->backgroundAlpha = 255;
	if (image == 0) {
		_W_CONTROL(control)->backgroundImage = 0;
	} else {
		_W_CONTROL(control)->backgroundImage = gdk_pixbuf_copy(
		_W_IMAGE(image)->pixbuf);
	}
	if (_W_CONTROL(control)->backgroundImage != 0) {
		_w_control_set_background_pixmap(control,
		_W_CONTROL(control)->backgroundImage, reserved);
		w_rect r;
		memset(&r, 0, sizeof(r));
		reserved->redraw_widget(control, &r, REDRAW_REDRAWALL, reserved);
	} else {
		reserved->set_widget_background(control, reserved);
	}
	reserved->redraw_children(control, reserved);
	return W_TRUE;
}
wresult _w_control_set_background_pixmap(w_control *control, GdkPixbuf *image,
		_w_control_reserved *reserved) {
	GtkWidget *paintHandle = reserved->paintHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	GdkWindow *window = gtk_widget_get_window(paintHandle);
	if (window != 0) {
		/*if (image.pixmap != 0) {
		 gdk_window_set_back_pixmap (window, pixmap, false);
		 } else if (image.surface != 0) {*/
#if GTK3
		cairo_surface_t *surface = 0;
		cairo_pattern_t *pattern = _w_image_create_pattern(image, &surface);
		if (pattern == 0)
			return W_ERROR_NO_HANDLES;
		cairo_pattern_set_extend(pattern, CAIRO_EXTEND_REPEAT);
		gdk_window_set_background_pattern(window, pattern);
		cairo_pattern_destroy(pattern);
		cairo_surface_destroy(surface);
		return W_TRUE;
#endif
		/*
		 * TODO This code code is commented because it does not work since the pixmap
		 * created with gdk_pixmap_foreign_new() does not have colormap. Another option
		 * would be to create a pixmap on the fly from the surface.
		 *
		 * For now draw background in windowProc().
		 */
//			long /*int*/ surface = image.surface;
//			int type = Cairo.cairo_surface_get_type(surface);
//			switch (type) {
//				case Cairo.CAIRO_SURFACE_TYPE_XLIB:
//					long /*int*/ pixmap = OS.gdk_pixmap_foreign_new(Cairo.cairo_xlib_surface_get_drawable(surface));
//					OS.gdk_window_set_back_pixmap (window, pixmap, false);
//					OS.g_object_unref(pixmap);
//					break;
//			}
		//}
	}
	return W_FALSE;
}

wresult _w_control_set_capture(w_control *control, int capture) {
	return W_TRUE;
}
wresult _w_control_set_cursor_0(w_control *control, GdkCursor *cursor,
		_w_control_reserved *reserved) {
	GdkWindow *window = reserved->eventWindow(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (window != 0) {
		gdk_window_set_cursor(window, cursor);
		gdk_flush();
	}
	return W_TRUE;
}
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor) {
	if (cursor != 0 && _W_CURSOR(cursor)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_CONTROL(control)->cursor = cursor;
	GdkCursor *hcursor = 0;
	if (cursor != 0) {
		hcursor = _W_CURSOR(cursor)->handle;
	}
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	return reserved->set_cursor(control, hcursor, reserved);
}
wresult _w_control_set_drag_detect(w_control *control, int dragDetect) {
	if (dragDetect) {
		_W_WIDGET(control)->state |= STATE_DRAG_DETECT;
	} else {
		_W_WIDGET(control)->state &= ~STATE_DRAG_DETECT;
	}
	return W_TRUE;
}
gboolean enterNotifyEventFunc(GSignalInvocationHint *ihint,
		guint n_param_values, const GValue *param_values, gpointer data) {
	/* 427776: this workaround listens to the enter-notify-event signal on all
	 * GtkWidgets. If enableWindow (the data parameter) has been added to the
	 * internal hash table of the widget, a record is kept as the lifetime of
	 * enableWindow is controlled here, so we'll need to remove that reference
	 * when we destroy enableWindow. this internal hash table was removed in
	 * GTK 3.11.9 so once only newer GTK is targeted, this workaround can be
	 * removed. */
	gpointer instance = g_value_peek_pointer(param_values);
	GHashTable *hashTable = (GHashTable*) g_object_get_qdata(G_OBJECT(instance),
			GTK_POINTER_WINDOW);

	// there will only ever be one item in the hash table
	if (hashTable != 0) {
		GList *firstItem = g_hash_table_get_values(hashTable);
		gpointer gdkWindow = firstItem->data;
		// data is actually enableWindow
		if (gdkWindow == data)
			g_object_set_qdata(G_OBJECT(gdkWindow), SWT_GRAB_WIDGET, instance);
	}

	return TRUE; // keep the signal connected
}
wresult _w_control_set_enabled(w_control *_this, int enabled) {
	if (((_W_WIDGET(_this)->state & STATE_DISABLED) == 0) == enabled)
		return W_TRUE;
	w_control *control = 0;
	gboolean fixFocus = FALSE;
	if (!enabled) {
		if (gtk_toolkit->focusEvent != W_EVENT_FOCUSOUT) {
			control = _w_toolkit_get_focus_control(W_TOOLKIT(gtk_toolkit));
			fixFocus = _w_control_is_focus_ancestor(_this, control);
		}
	}
	if (enabled) {
		_W_WIDGET(_this)->state &= ~STATE_DISABLED;
	} else {
		_W_WIDGET(_this)->state |= STATE_DISABLED;
	}
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(_this);
	reserved->enable_widget(_this, enabled, reserved);
	if (!w_widget_is_ok(W_WIDGET(_this)))
		return W_FALSE;
	GdkWindow *enableWindow = 0;
	if (enabled) {
		enableWindow = reserved->enableWindow(W_WIDGET(_this), reserved);
		if (_W_CONTROL(_this)->enableWindow != 0) {
			_w_control_cleanup_enable_window(_this, reserved);
		}
	} else {
		gtk_widget_realize(_W_WIDGET(_this)->handle);
		w_composite *parent = _W_CONTROL(_this)->parent;
		_w_control_reserved *preserved = _W_CONTROL_GET_RESERVED(parent);
		GtkWidget *parentHandle = preserved->eventHandle(W_WIDGET(parent),
				_W_WIDGET_RESERVED(preserved));
		GdkWindow *window = preserved->eventWindow(W_WIDGET(parent),
				_W_WIDGET_RESERVED(preserved));
		GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
				W_WIDGET(_this), _W_WIDGET_RESERVED(reserved));
		GdkWindowAttr attributes;
		GtkAllocation allocation;
		gtk_widget_get_allocation(topHandle, &allocation);
		attributes.x = allocation.x;
		attributes.y = allocation.y;
		attributes.width =
				(_W_WIDGET(_this)->state & STATE_ZERO_WIDTH) != 0 ?
						0 : allocation.width;
		attributes.height =
				(_W_WIDGET(_this)->state & STATE_ZERO_HEIGHT) != 0 ?
						0 : allocation.height;
		attributes.event_mask = (0xFFFFFFFF & ~(1 << 15)); // ExposureMask
		attributes.wclass = GDK_INPUT_ONLY;
		attributes.window_type = GDK_WINDOW_CHILD;
		_W_CONTROL(_this)->enableWindow = gdk_window_new(window, &attributes,
				GDK_WA_X | GDK_WA_Y);
		enableWindow = reserved->enableWindow(W_WIDGET(_this), reserved);
		if (enableWindow != 0) {
			/* 427776: we need to listen to all enter-notify-event signals to
			 * see if this new GdkWindow has been added to a widget's internal
			 * hash table, so when the GdkWindow is destroyed we can also remove
			 * that reference. */
			if (enterNotifyEventSignalId != 0)
				_W_CONTROL(control)->enterNotifyEventId =
						g_signal_add_emission_hook(enterNotifyEventSignalId, 0,
								enterNotifyEventFunc, enableWindow, 0);

			gdk_window_set_user_data(enableWindow, parentHandle);

			//if (!GDK_WINDOWING_X11) {
			gdk_window_raise(enableWindow);
			//} else {
			gdk_window_restack(window, gtk_widget_get_window(topHandle), TRUE);
			//}
			if (gtk_widget_get_visible(topHandle))
				gdk_window_show_unraised(enableWindow);
		}
	}
	if (fixFocus)
		reserved->fix_focus(_this, control, reserved);
	return W_TRUE;
}
void _w_control_cleanup_enable_window(w_control *_this,
		_w_control_reserved *reserved) {
	GdkWindow *enableWindow = reserved->enableWindow(W_WIDGET(_this), reserved);
	if (enterNotifyEventSignalId != 0) {
		if (_W_CONTROL(_this)->enterNotifyEventId > 0)
			g_signal_remove_emission_hook(enterNotifyEventSignalId,
			_W_CONTROL(_this)->enterNotifyEventId);
		_W_CONTROL(_this)->enterNotifyEventId = 0;

		/*
		 * 427776: now we can remove any reference to the GdkWindow
		 * in a widget's internal hash table. this internal hash
		 * table was removed in GTK 3.11.9 so once only newer GTK is
		 * targeted, this workaround can be removed.
		 */
		GtkWidget *grabWidget = g_object_get_qdata(G_OBJECT(enableWindow),
				SWT_GRAB_WIDGET);
		if (grabWidget != 0) {
			g_object_set_qdata(G_OBJECT(grabWidget), GTK_POINTER_WINDOW, 0);
			g_object_set_qdata(G_OBJECT(enableWindow), SWT_GRAB_WIDGET, 0);
		}
	}

	gdk_window_set_user_data(enableWindow, 0);
	gdk_window_destroy(enableWindow);
	_W_CONTROL(_this)->enableWindow = 0;
}
wresult _w_control_set_focus(w_control *control) {
	if ((_W_WIDGET(control)->style & W_NO_FOCUS) != 0)
		return W_FALSE;
	return _w_control_force_focus(control);
}
wresult _w_control_set_font(w_control *control, w_font *font) {
	if (((_W_WIDGET(control)->state & STATE_FONT) == 0) && font == 0)
		return W_TRUE;
	_W_CONTROL(control)->font = font;
	PangoFontDescription *fontDesc;
	if (font == 0) {
		fontDesc = _W_FONT(_w_control_default_font(control))->handle;
	} else {
		if (!w_font_is_ok(font))
			return W_ERROR_INVALID_ARGUMENT;
		fontDesc = _W_FONT(font)->handle;
	}
	if (font == 0) {
		_W_WIDGET(control)->state &= ~STATE_FONT;
	} else {
		_W_WIDGET(control)->state |= STATE_FONT;
	}
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	reserved->set_font_description(control, fontDesc, reserved);
	return W_TRUE;
}
void _w_control_set_font_description(w_control *control,
		PangoFontDescription *font, _w_control_reserved *reserved) {
	_w_widget_set_font_description(W_WIDGET(control),
	_W_WIDGET(control)->handle, font, _W_WIDGET_RESERVED(reserved));
}

wresult _w_control_set_foreground(w_control *control, w_color color) {
	if (((_W_WIDGET(control)->state & STATE_FOREGROUND) == 0) && color == 0)
		return W_FALSE;
	GdkColor *gdkColor = 0, tgdkColor;
	if (color != 0) {
		tgdkColor.red = W_RED(color) * 0x100;
		tgdkColor.green = W_GREEN(color) * 0x100;
		tgdkColor.blue = W_BLUE(color) * 0x100;
		tgdkColor.pixel = 0;
		gdkColor = &tgdkColor;
	}
	gboolean set = FALSE;
#if GTK3
	w_color _color;
	W_CONTROL_GET_CLASS(control)->get_foreground(control, &_color);
	set = _color != color;
#endif
#if GTK2
#endif
	if (set) {
		if (color == 0) {
			_W_WIDGET(control)->state &= ~STATE_FOREGROUND;
		} else {
			_W_WIDGET(control)->state |= STATE_FOREGROUND;
		}
		_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
		reserved->set_foreground_color_0(control, gdkColor, reserved);
	}
}
void _w_control_set_foreground_color_0(w_control *control, GdkColor *color,
		_w_control_reserved *reserved) {
	GdkRGBA *rgba = 0, trgba;
	if (GTK_VERSION >= VERSION(3, 16, 0)) {
		if (color != 0) {
			trgba.alpha = 1.0;
			trgba.red = (color->red & 0xFFFF) / (float) 0xFFFF;
			trgba.green = (color->green & 0xFFFF) / (float) 0xFFFF;
			trgba.blue = (color->blue & 0xFFFF) / (float) 0xFFFF;
			rgba = &trgba;
		}
		reserved->set_foreground_color(control, _W_WIDGET(control)->handle,
				rgba, reserved);
	} else {
		reserved->set_foreground_color(control, _W_WIDGET(control)->handle,
				color, reserved);
	}
}
void _w_control_set_foreground_color_1(w_control *control, GtkWidget *handle,
		GdkRGBA *rgba, _w_control_reserved *reserved) {
	/*GdkRGBA toSet;
	 if (rgba != null) {
	 toSet = rgba;
	 } else {
	 GdkColor defaultForeground = display.COLOR_WIDGET_FOREGROUND;
	 toSet = display.toGdkRGBA(defaultForeground);
	 }
	 long context = OS.gtk_widget_get_style_context(handle);
	 // Form foreground string
	 String color = display.gtk_rgba_to_css_string(toSet);
	 String
	 name = OS.GTK_VERSION >= OS.VERSION(3, 20, 0) ? display.gtk_widget_class_get_css_name(handle)
	 : display.gtk_widget_get_name(handle);
	 GdkRGBA selectedForeground = display.toGdkRGBA(
	 getDisplay().COLOR_LIST_SELECTION_TEXT);
	 String
	 selection = OS.GTK_VERSION >= OS.VERSION(3, 20, 0) ? " selection" : ":selected";
	 String css = "* {color: " + color + ";}\n" + name + selection + " {color: "
	 + display.gtk_rgba_to_css_string(selectedForeground) + ";}";

	 // Cache foreground color
	 cssForeground = css;

	 // Apply foreground color and any cached background color
	 String finalCss = display.gtk_css_create_css_color_string(cssBackground,
	 cssForeground, SWT.FOREGROUND);
	 gtk_css_provider_load_from_css(context, finalCss);*/
}
void _w_control_set_initial_bounds(w_control *control,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0
			&& (_W_WIDGET(control)->state & STATE_ZERO_HEIGHT) != 0) {
		/*
		 * Feature in GTK.  On creation, each widget's allocation is
		 * initialized to a position of (-1, -1) until the widget is
		 * first sized.  The fix is to set the value to (0, 0) as
		 * expected by SWT.
		 */
		/*long topHandle = topHandle();
		 GtkAllocation allocation = new GtkAllocation();
		 if ((parent.style & SWT.MIRRORED) != 0) {
		 allocation.x = parent.getClientWidth();
		 } else {
		 allocation.x = 0;
		 }
		 allocation.y = 0;
		 if (OS.GTK3) {
		 OS.gtk_widget_set_visible(topHandle, true);
		 }
		 OS.gtk_widget_set_allocation(topHandle, allocation);*/
	} else {
		/*resizeHandle(1, 1);
		 forceResize();*/
	}
}
wresult _w_control_set_menu(w_control *control, w_menu *menu) {
	_W_CONTROL(control)->menu = 0;
	if (w_widget_is_ok(W_WIDGET(menu))) {
		if ((_W_WIDGET(menu)->style & W_POP_UP) == 0) {
			return W_ERROR_MENU_NOT_POP_UP;
		}
		w_shell *shell1;
		w_control_get_shell(_W_MENU(menu)->parent, &shell1);
		w_shell *shell2;
		w_control_get_shell(control, &shell2);
		if (shell1 != shell2) {
			return W_ERROR_INVALID_PARENT;
		}
		_W_CONTROL(control)->menu = menu;
		return W_TRUE;
	}
	return W_ERROR_INVALID_ARGUMENT;
}
void _w_control_set_orientation_0(w_control *control, wbool create,
		_w_control_reserved *reserved) {
	if ((_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0 || !create) {
		GtkTextDirection dir =
				(_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0 ?
						GTK_TEXT_DIR_RTL : GTK_TEXT_DIR_LTR;
		if (_W_WIDGET(control)->handle != 0)
			gtk_widget_set_direction(_W_WIDGET(control)->handle, dir);
		GtkWidget *fixedHandle = reserved->fixedHandle(W_WIDGET(control),
				_W_WIDGET_RESERVED(reserved));
		if (fixedHandle != 0)
			gtk_widget_set_direction(fixedHandle, dir);
	}
}
wresult _w_control_set_orientation(w_control *control, int orientation) {
	int flags = W_RIGHT_TO_LEFT | W_LEFT_TO_RIGHT;
	if ((orientation & flags) == 0 || (orientation & flags) == flags)
		return W_TRUE;
	_W_WIDGET(control)->style &= ~flags;
	_W_WIDGET(control)->style |= orientation & flags;
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved(W_WIDGET(control)));
	reserved->set_orientation(control, W_FALSE, reserved);
	_W_WIDGET(control)->style &= ~W_MIRRORED;
	reserved->check_mirrored(control, reserved);
	return W_TRUE;
}
wresult _w_control_set_parent(w_control *control, w_composite *parent) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_widget_is_ok(W_WIDGET(parent)))
		return W_ERROR_INVALID_ARGUMENT;
	w_composite *lastparent = _W_CONTROL(control)->parent;
	if (lastparent == parent)
		return W_TRUE;
	if (W_CONTROL_GET_CLASS(control)->is_reparentable(control) <= 0)
		return W_FALSE;
	gtk_widget_realize(_W_WIDGET(parent)->handle);
	_w_control_reserved *preserved;
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	GtkAllocation allocation;
	w_rect r;
	gtk_widget_get_allocation(topHandle, &allocation);
	r.x = allocation.x;
	r.y = allocation.y;
	r.width =
			(_W_WIDGET(control)->state & STATE_ZERO_WIDTH) != 0 ?
					0 : allocation.width;
	r.height =
			(_W_WIDGET(control)->state & STATE_ZERO_HEIGHT) != 0 ?
					0 : allocation.height;
	if ((_W_WIDGET(lastparent)->style & W_MIRRORED) != 0) {
		preserved = _W_CONTROL_GET_RESERVED(lastparent);
		r.x = preserved->get_client_width(W_CONTROL(lastparent), preserved)
				- r.width - r.x;
	}
	preserved = _W_CONTROL_GET_RESERVED(parent);
	if ((_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
		r.x = preserved->get_client_width(W_CONTROL(parent), preserved)
				- r.width - r.x;
	}
	/*releaseParent();
	 Shell newShell = parent.getShell(), oldShell = getShell();
	 Decorations newDecorations = parent.menuShell(), oldDecorations =
	 menuShell();
	 Menu [] menus = oldShell.findMenus (this);
	 if (oldShell != newShell || oldDecorations != newDecorations) {
	 fixChildren(newShell, oldShell, newDecorations, oldDecorations, menus);
	 newDecorations.fixAccelGroup();
	 oldDecorations.fixAccelGroup();
	 }*/
	GtkWidget *newParent = _W_COMPOSITE_RESERVED(preserved)->parenting_handle(
			W_CONTROL(parent), reserved);
	gtk_widget_reparent(topHandle, newParent);
#if GTK3
	_w_fixed_move(topHandle, r.x, r.y);
#endif
#if GTK2
#endif
	/*
	 * Restore the original widget size since GTK does not keep it.
	 */
	reserved->resize_handle(control, &r.sz, reserved);
	/*
	 * Cause a size allocation this widget's topHandle.  Note that
	 * all calls to gtk_widget_size_allocate() must be preceded by
	 * a call to gtk_widget_size_request().
	 */
	GtkRequisition requisition;
	gtk_widget_size_request(topHandle, &requisition);
	allocation.x = r.x;
	allocation.y = r.y;
	allocation.width = r.width;
	allocation.height = r.height;
	gtk_widget_size_allocate(topHandle, &allocation);
	_W_CONTROL(control)->parent = parent;
	reserved->set_ZOrder(control, 0, ZORDER_FIX_RELATION, reserved);
	//reskin(SWT.ALL);
	return W_TRUE;
}
void _w_control_set_parent_background(w_control *control,
		_w_control_reserved *reserved) {
	/*setBackgroundColor (handle, null);
	 if (fixedHandle != 0) setBackgroundColor (fixedHandle, null);*/
}
void _w_control_set_parent_window(w_control *control, GtkWidget *widget,
		_w_control_reserved *reserved) {
}

gboolean _w_control_set_radio_selection(w_control *control, gboolean value,
		_w_control_reserved *reserved) {
	return FALSE;
}
wresult _w_control_set_redraw(w_control *control, int redraw) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	GdkWindow *redrawWindow;
	if (redraw) {
		if ((--_W_CONTROL(control)->drawCount) == 0) {
			redrawWindow = _W_CONTROL(control)->redrawWindow;
			if (redrawWindow != 0) {
				GdkWindow *window = reserved->paintWindow(W_WIDGET(control),
						_W_WIDGET_RESERVED(reserved));
				/* Explicitly hiding the window avoids flicker on GTK+ >= 2.6 */
				gdk_window_hide(redrawWindow);
				gdk_window_destroy(redrawWindow);
				gdk_window_set_events(window,
						(GdkEventMask) gtk_widget_get_events(
								reserved->paintHandle(W_WIDGET(control),
										_W_WIDGET_RESERVED(reserved))));
				_W_CONTROL(control)->redrawWindow = 0;
			}
		}
	} else {
		if (_W_CONTROL(control)->drawCount++ == 0) {
			if (gtk_widget_get_realized(_W_WIDGET(control)->handle)) {
				GdkWindow *window = reserved->paintWindow(W_WIDGET(control),
						_W_WIDGET_RESERVED(reserved));
				w_rect rect;
				W_CONTROL_GET_CLASS(control)->get_bounds(control, &rect.pt,&rect.sz);
				GdkWindowAttr attributes;
				attributes.width = rect.width;
				attributes.height = rect.height;
				attributes.event_mask = GDK_EXPOSURE_MASK;
				attributes.window_type = GDK_WINDOW_CHILD;
				redrawWindow = gdk_window_new(window, &attributes, 0);
				_W_CONTROL(control)->redrawWindow = redrawWindow;
				if (redrawWindow != 0) {
					int mouseMask = GDK_BUTTON_PRESS_MASK
							| GDK_BUTTON_RELEASE_MASK | GDK_ENTER_NOTIFY_MASK
							| GDK_LEAVE_NOTIFY_MASK | GDK_POINTER_MOTION_MASK
							| GDK_POINTER_MOTION_HINT_MASK
							| GDK_BUTTON_MOTION_MASK | GDK_BUTTON1_MOTION_MASK
							| GDK_BUTTON2_MOTION_MASK | GDK_BUTTON3_MOTION_MASK;
					gdk_window_set_events(window,
							(GdkEventMask) (gdk_window_get_events(window)
									& ~mouseMask));
#if GTK3
					gdk_window_set_background_pattern(redrawWindow, 0);
#endif
#if GTK2
#endif
					gdk_window_show(redrawWindow);
				}
			}
		}
	}
}
wresult _w_control_set_tab_item_focus(w_widget *widget, wbool next,
		_w_widget_reserved *reserved) {
	if (!_w_control_is_showing(W_CONTROL(widget),
			_W_CONTROL_RESERVED(reserved)))
		return W_FALSE;
	return W_CONTROL_GET_CLASS(widget)->force_focus(W_CONTROL(widget));
}
wresult _w_control_set_text_direction(w_control *control, int textDirection) {
	return W_FALSE;
}
wresult _w_control_set_toolTip_text(w_control *control, const char *text,
		size_t length) {
	w_shell *shell;
	W_CONTROL_GET_CLASS(control)->get_shell(control, &shell);
	_w_control_set_toolTip_text_0(control, shell, text);
	/* toolTipText = string;*/
	return W_TRUE;
}
void _w_control_set_toolTip_text_0(w_control *control, w_shell *shell,
		const char *newString) {
	/*
	 * Feature in GTK.  In order to prevent children widgets
	 * from inheriting their parent's tooltip, the tooltip is
	 * a set on a shell only. In order to force the shell tooltip
	 * to update when a new tip string is set, the existing string
	 * in the tooltip is set to null, followed by running a query.
	 * The real tip text can then be set.
	 *
	 * Note that this will only run if the control for which the
	 * tooltip is being set is the current control (i.e. the control
	 * under the pointer).
	 */
	if (gtk_toolkit->currentControl == control) {
		_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
		GtkWidget *eventHandle = reserved->eventHandle(W_WIDGET(control),
				_W_WIDGET_RESERVED(reserved));
		_w_shell_set_tooltip_text(shell, eventHandle, newString);
	}
}

wresult _w_control_set_touch_enabled(w_control *control, int enabled) {
	return W_TRUE;
}
wresult _w_control_set_visible(w_control *control, wbool visible) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	GtkWidget *topHandle = reserved->widget.topHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (visible) {
		/*
		 * It is possible (but unlikely), that application
		 * code could have disposed the widget in the show
		 * event.  If this happens, just return.
		 */
		/*sendEvent (SWT.Show);
		 if (isDisposed ()) return;
		 state &= ~HIDDEN;
		 if ((state & (ZERO_WIDTH | ZERO_HEIGHT)) == 0) {
		 if (enableWindow != 0) OS.gdk_window_show_unraised (enableWindow);*/
		gtk_widget_show(topHandle);
		//}
	} else {
		/*
		 * Bug in GTK.  Invoking gtk_widget_hide() on a widget that has
		 * focus causes a focus_out_event to be sent. If the client disposes
		 * the widget inside the event, GTK GP's.  The fix is to reassign focus
		 * before hiding the widget.
		 *
		 * NOTE: In order to stop the same widget from taking focus,
		 * temporarily clear and set the GTK_VISIBLE flag.
		 */
		/*Control control = null;
		 boolean fixFocus = false;
		 if (display.focusEvent != SWT.FocusOut) {
		 control = display.getFocusControl ();
		 fixFocus = isFocusAncestor (control);
		 }
		 state |= HIDDEN;
		 if (fixFocus) {
		 if (OS.GTK3) {
		 OS.gtk_widget_set_can_focus (topHandle, false);
		 } else {
		 gtk_widget_set_visible (topHandle, false);
		 }
		 fixFocus (control);
		 if (isDisposed ()) return;
		 if (OS.GTK3) {
		 OS.gtk_widget_set_can_focus (topHandle, true);
		 } else {
		 gtk_widget_set_visible (topHandle, true);
		 }
		 }*/
		gtk_widget_hide(topHandle);
		/*if (isDisposed ()) return;
		 if (enableWindow != 0) OS.gdk_window_hide (enableWindow);
		 sendEvent (SWT.Hide);*/
	}
}
wresult _w_control_set_ZOrder(w_control *control, w_control *sibling, int flags,
		_w_control_reserved *reserved) {
	int index = 0, siblingIndex = 0, oldNextIndex = -1;
	if (flags & ZORDER_FIX_RELATION) {
		/* determine the receiver's and sibling's indexes in the parent */
		/*children = parent._getChildren ();
		 while (index < children.length) {
		 if (children [index] == this) break;
		 index++;
		 }
		 if (sibling != null) {
		 while (siblingIndex < children.length) {
		 if (children [siblingIndex] == sibling) break;
		 siblingIndex++;
		 }
		 }*/
		/* remove "Labelled by" relationships that will no longer be valid */
		/*removeRelation ();
		 if (index + 1 < children.length) {
		 oldNextIndex = index + 1;
		 children [oldNextIndex].removeRelation ();
		 }
		 if (sibling != null) {
		 if (above) {
		 sibling.removeRelation ();
		 } else {
		 if (siblingIndex + 1 < children.length) {
		 children [siblingIndex + 1].removeRelation ();
		 }
		 }
		 }*/
	}

	/*long  topHandle = topHandle ();
	 long  siblingHandle = sibling != null ? sibling.topHandle () : 0;
	 long  window = gtk_widget_get_window (topHandle);
	 if (window != 0) {
	 long  siblingWindow = 0;
	 if (sibling != null) {
	 if ((flags & ZORDER_ABOVE) && sibling.enableWindow != 0) {
	 siblingWindow = enableWindow;
	 } else {
	 siblingWindow = OS.gtk_widget_get_window (siblingHandle);
	 }
	 }
	 long  redrawWindow = fixChildren ? parent.redrawWindow : 0;
	 if (!OS.GDK_WINDOWING_X11 () || (siblingWindow == 0 && (!above || redrawWindow == 0))) {
	 if (flags & ZORDER_ABOVE) {
	 OS.gdk_window_raise (window);
	 if (redrawWindow != 0) OS.gdk_window_raise (redrawWindow);
	 if (enableWindow != 0) OS.gdk_window_raise (enableWindow);
	 } else {
	 if (enableWindow != 0) OS.gdk_window_lower (enableWindow);
	 OS.gdk_window_lower (window);
	 }
	 } else {
	 long siblingW = siblingWindow != 0 ? siblingWindow : redrawWindow;
	 boolean stack_mode = above;
	 if (redrawWindow != 0 && siblingWindow == 0) stack_mode = false;
	 restackWindow (window, siblingW, stack_mode);
	 if (enableWindow != 0) {
	 restackWindow (enableWindow, window, true);
	 }
	 }
	 }
	 if (flags & ZORDER_FIX_CHILDREN) {
	 if (flags & ZORDER_ABOVE) {
	 parent.moveAbove (topHandle, siblingHandle);
	 } else {
	 parent.moveBelow (topHandle, siblingHandle);
	 }
	 }*/
	/*  Make sure that the parent internal windows are on the bottom of the stack	*/
//if (!(flags & ZORDER_ABOVE) && (flags & ZORDER_FIX_CHILDREN)) 	parent.fixZOrder ();
	if (flags & ZORDER_FIX_RELATION) {
		/* determine the receiver's new index in the parent */
		/*if (sibling != null) {
		 if (flags & ZORDER_ABOVE) {
		 index = siblingIndex - (index < siblingIndex ? 1 : 0);
		 } else {
		 index = siblingIndex + (siblingIndex < index ? 1 : 0);
		 }
		 } else {
		 if (flags & ZORDER_ABOVE) {
		 index = 0;
		 } else {
		 index = children.length - 1;
		 }
		 }*/

		/* add new "Labelled by" relations as needed */
		/*children = parent._getChildren ();
		 if (0 < index) {
		 children [index - 1].addRelation (this);
		 }
		 if (index + 1 < children.length) {
		 addRelation (children [index + 1]);
		 }*/
		if (oldNextIndex != -1) {
			//if (oldNextIndex <= index) oldNextIndex--;
			/* the last two conditions below ensure that duplicate relations are not hooked */
			/*if (0 < oldNextIndex && oldNextIndex != index && oldNextIndex != index + 1) {
			 children [oldNextIndex - 1].addRelation (children [oldNextIndex]);
			 }*/
		}
	}
}
void _w_control_set_widget_background(w_control *control,
		_w_control_reserved *reserved) {
	GdkColor *color, tcolor;
	if ((_W_WIDGET(control)->state & STATE_BACKGROUND) != 0) {
		reserved->get_background_color(control, &tcolor, reserved);
		color = &tcolor;
	} else {
		color = 0;
	}
	GtkWidget *fixedHandle = reserved->fixedHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (fixedHandle != 0) {
		_w_control_set_background_color_1(control, fixedHandle, color,
				reserved);
	}
	_w_control_set_background_color_1(control, _W_WIDGET(control)->handle,
			color, reserved);
}
gboolean _w_control_show_menu(w_control *control, int x, int y, int detail) {
	w_event_menu_detect event;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_MENUDETECT;
	event.event.widget = W_WIDGET(control);
	event.location.x = x;
	event.location.y = y;
	event.detail = detail;
	int ret = _w_widget_send_event(W_WIDGET(control), (w_event*) &event);
//widget could be disposed at this point
	if (!w_widget_is_ok(W_WIDGET(control)))
		return FALSE;
	if (ret && w_widget_is_ok(W_WIDGET(event.menu))) {
		/*boolean hooksKeys = hooks(SWT.KeyDown) || hooks(SWT.KeyUp);
		 menu.createIMMenu(hooksKeys ? imHandle() : 0);*/
		if (event.location.x != x || event.location.y != y) {
			w_menu_set_location(event.menu, &event.location);
		}
		w_menu_set_visible(event.menu, W_TRUE);
		return TRUE;
	}
	return FALSE;
}
void _w_control_show_widget(w_control *control, _w_control_reserved *reserved) {
// Comment this line to disable zero-sized widgets
	_W_WIDGET(control)->state |= STATE_ZERO_WIDTH | STATE_ZERO_HEIGHT;
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	w_composite *parent = _W_CONTROL(control)->parent;
	_w_composite_reserved *preserved = _W_COMPOSITE_GET_RESERVED(parent);
	GtkWidget *parentHandle = preserved->parenting_handle(W_WIDGET(parent),
			_W_WIDGET_RESERVED(preserved));
//parent.setParentWindow(topHandle);
	gtk_container_add(GTK_CONTAINER(parentHandle), topHandle);
	GtkWidget *fixedHandle = reserved->fixedHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (_W_WIDGET(control)->handle != 0
			&& _W_WIDGET(control)->handle != topHandle)
		gtk_widget_show(_W_WIDGET(control)->handle);
	if ((_W_WIDGET(control)->state & (STATE_ZERO_WIDTH | STATE_ZERO_HEIGHT))
			== 0) {
		if (fixedHandle != 0)
			gtk_widget_show(fixedHandle);
	}
	if (fixedHandle != 0) {
		reserved->fix_style_0(control, fixedHandle, reserved);
	}
}
wresult _w_control_traverse(w_control *control, int traversal,
		w_event_key *event) {
	if (event == 0)
		return W_ERROR_NULL_ARGUMENT;
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(control);
	return _w_control_traverse_0(control, traversal, event->character,
			event->keycode, event->keylocation, event->statemask, event->doit,
			reserved);
}

wresult _w_control_traverse_0(w_control *_this, int traversal, char character,
		int keyCode, int keyLocation, int stateMask, wbool doit,
		_w_control_reserved *reserved) {
	if (traversal == W_TRAVERSE_NONE) {
		switch (keyCode) {
		case W_ESC: {
			traversal = W_TRAVERSE_ESCAPE;
			doit = TRUE;
			break;
		}
		case W_CR: {
			traversal = W_TRAVERSE_RETURN;
			doit = TRUE;
			break;
		}
		case W_ARROW_DOWN:
		case W_ARROW_RIGHT: {
			traversal = W_TRAVERSE_ARROW_NEXT;
			doit = FALSE;
			break;
		}
		case W_ARROW_UP:
		case W_ARROW_LEFT: {
			traversal = W_TRAVERSE_ARROW_PREVIOUS;
			doit = FALSE;
			break;
		}
		case W_TAB: {
			traversal =
					(stateMask & W_SHIFT) != 0 ?
							W_TRAVERSE_TAB_PREVIOUS : W_TRAVERSE_TAB_NEXT;
			doit = TRUE;
			break;
		}
		case W_PAGE_DOWN: {
			if ((stateMask & W_CTRL) != 0) {
				traversal = W_TRAVERSE_PAGE_NEXT;
				doit = TRUE;
			}
			break;
		}
		case W_PAGE_UP: {
			if ((stateMask & W_CTRL) != 0) {
				traversal = W_TRAVERSE_PAGE_PREVIOUS;
				doit = TRUE;
			}
			break;
		}
		default: {
			if (character != 0 && (stateMask & (W_ALT | W_CTRL)) == W_ALT) {
				traversal = W_TRAVERSE_MNEMONIC;
				doit = TRUE;
			}
			break;
		}
		}
	}
	w_event_key event;
	event.character = character;
	event.detail = traversal;
	event.doit = doit;
	event.keycode = keyCode;
	event.keylocation = keyLocation;
	event.statemask = stateMask;
	w_shell *shell;
	W_CONTROL_GET_CLASS(_this)->get_shell(_this, &shell);

	gboolean all = FALSE;
	switch (traversal) {
	case W_TRAVERSE_ESCAPE:
	case W_TRAVERSE_RETURN:
	case W_TRAVERSE_PAGE_NEXT:
	case W_TRAVERSE_PAGE_PREVIOUS: {
		all = TRUE;
		/* traversal is a valid traversal action */
	}
		break;
	case W_TRAVERSE_ARROW_NEXT:
	case W_TRAVERSE_ARROW_PREVIOUS:
	case W_TRAVERSE_TAB_NEXT:
	case W_TRAVERSE_TAB_PREVIOUS: {
		/* traversal is a valid traversal action */
		break;
	}
	case W_TRAVERSE_MNEMONIC: {
		wresult result = reserved->translate_mnemonic(_this, &event, 0,
				reserved);
		if (result > 0)
			return result;
		_w_control_reserved *sreserved = _W_CONTROL_GET_RESERVED(shell);
		result = sreserved->translate_mnemonic(W_CONTROL(shell), &event, _this,
				sreserved);
		return result;
	}
	default: {
		/* traversal is not a valid traversal action */
		return W_FALSE;
	}
	}

	w_control *control = _this;
	do {

		if (_w_control_traverse_1(control, &event, reserved))
			return W_TRUE;
		if (!event.doit /*&& control.hooks(SWT.Traverse)*/)
			return W_FALSE;
		if (control == W_CONTROL(shell))
			return W_FALSE;
		control = W_CONTROL(_W_CONTROL(control)->parent);
	} while (all && control != 0);
	return W_FALSE;
}
wresult _w_control_translate_mnemonic(w_control *_this, w_event_key *event,
		w_control *control, _w_control_reserved *reserved) {
	if (control == _this)
		return W_FALSE;
	if (W_CONTROL_GET_CLASS(_this)->is_visible(_this) <= 0
			|| W_CONTROL_GET_CLASS(_this)->is_enabled(_this) <= 0)
		return W_FALSE;
	event->doit = _this == gtk_toolkit->mnemonicControl
			|| reserved->mnemonic_match(_this, event->character, reserved);
	return _w_control_traverse_1(_this, event, reserved);
}
gboolean _w_control_translate_mnemonic_0(w_control *_this, int keyval,
		GdkEventKey *gdkEvent, _w_control_reserved *reserved) {
	int key = gdk_keyval_to_unicode(keyval);
	if (key < 0x20)
		return W_FALSE;
	w_shell *shell;
	W_CONTROL_GET_CLASS(_this)->get_shell(_this, &shell);
	if (gdkEvent->state == 0) {
		int code = reserved->traversal_code(_this, keyval, gdkEvent, reserved);
		if ((code & W_TRAVERSE_MNEMONIC) == 0)
			return W_FALSE;
	} else {
		int mask = GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK;
		if ((gdkEvent->state & mask)
				!= gtk_window_get_mnemonic_modifier(
						GTK_WINDOW(_W_SHELL_HANDLE(shell))))
			return W_FALSE;
	}
	if (W_CONTROL_GET_CLASS(shell)->is_visible(W_CONTROL(shell)) > 0
			&& W_CONTROL_GET_CLASS(shell)->is_enabled(W_CONTROL(shell)) > 0) {
		w_event_key event;
		event.event.type = W_EVENT_TRAVERSE;
		event.event.widget = W_WIDGET(_this);
		event.event.platform_event = 0;
		event.event.time = 0;
		event.event.data = 0;
		event.detail = W_TRAVERSE_MNEMONIC;
		int unicode_length = w_utf8_to_utf16(gdkEvent->string, -1, 0, 0);

		if (_w_widget_set_key_state(&event, gdkEvent, unicode_length)) {
			wresult result = reserved->translate_mnemonic(_this, &event, 0,
					reserved);
			if (result > 0)
				return result;
			_w_control_reserved *sreserved = _W_CONTROL_GET_RESERVED(shell);
			result = sreserved->translate_mnemonic(W_CONTROL(shell), &event,
					_this, sreserved);
			return result;
		}
	}
	return W_FALSE;
}
wresult _w_control_translate_traversal(w_control *control,
		GdkEventKey *keyEvent, _w_control_reserved *reserved) {
	int detail = W_TRAVERSE_NONE;
	int key = keyEvent->keyval;
	int code = reserved->traversal_code(control, key, keyEvent, reserved);
	gboolean all = FALSE;
	switch (key) {
	case GDK_KEY_Escape: {
		all = W_TRUE;
		detail = W_TRAVERSE_ESCAPE;
		break;
	}
	case GDK_KEY_KP_Enter:
	case GDK_KEY_Return: {
		all = W_TRUE;
		detail = W_TRAVERSE_RETURN;
		break;
	}
	case GDK_KEY_ISO_Left_Tab:
	case GDK_KEY_Tab: {
		gboolean next = (keyEvent->state & GDK_SHIFT_MASK) == 0;
		detail = next ? W_TRAVERSE_TAB_NEXT : W_TRAVERSE_TAB_PREVIOUS;
		break;
	}
	case GDK_KEY_Up:
	case GDK_KEY_Left:
	case GDK_KEY_Down:
	case GDK_KEY_Right: {
		gboolean next = key == GDK_KEY_Down || key == GDK_KEY_Right;
		w_composite *parent = _W_CONTROL(control)->parent;
		if (parent != 0 && (_W_WIDGET(parent)->style & W_MIRRORED) != 0) {
			if (key == GDK_KEY_Left || key == GDK_KEY_Right)
				next = !next;
		}
		detail = next ? W_TRAVERSE_ARROW_NEXT : W_TRAVERSE_ARROW_PREVIOUS;
		break;
	}
	case GDK_KEY_Page_Up:
	case GDK_KEY_Page_Down: {
		all = W_TRUE;
		if ((keyEvent->state & GDK_CONTROL_MASK) == 0)
			return W_FALSE;
		detail =
				key == GDK_KEY_Page_Down ?
						W_TRAVERSE_PAGE_NEXT : W_TRAVERSE_PAGE_PREVIOUS;
		break;
	}
	default:
		return W_FALSE;
	}
	w_event_key event;
	event.event.type = W_EVENT_TRAVERSE;
	event.event.type = W_EVENT_TRAVERSE;
	event.event.widget = W_WIDGET(control);
	event.event.platform_event = 0;
	event.event.time = keyEvent->time;
	event.event.data = 0;
	event.doit = (code & detail) != 0;
	event.detail = detail;
	int unicode_length = w_utf8_to_utf16(keyEvent->string, -1, 0, 0);

	if (!_w_widget_set_key_state(&event, keyEvent, unicode_length)) {
		return W_FALSE;
	}

	w_shell *shell;
	W_CONTROL_GET_CLASS(control)->get_shell(control, &shell);
	w_control *c = control;
	do {

		if (_w_control_traverse_1(control, &event, reserved))
			return W_TRUE;
		if (!event.doit /*&& control.hooks(SWT.Traverse)*/)
			return W_FALSE;
		if (control == W_CONTROL(shell))
			return W_FALSE;
		control = W_CONTROL(_W_CONTROL(control)->parent);
	} while (all && control != 0);
	return W_FALSE;
}
int _w_control_traversal_code(w_control *control, int key, GdkEventKey *event,
		_w_control_reserved *reserved) {
	int code = W_TRAVERSE_RETURN | W_TRAVERSE_TAB_NEXT | W_TRAVERSE_TAB_PREVIOUS
			| W_TRAVERSE_PAGE_NEXT | W_TRAVERSE_PAGE_PREVIOUS;
	w_shell *shell;
	W_CONTROL_GET_CLASS(control)->get_shell(control, &shell);
	if (_W_CONTROL(shell)->parent != 0)
		code |= W_TRAVERSE_ESCAPE;
	return code;
}
wresult _w_control_traverse_1(w_control *control, w_event_key *event,
		_w_control_reserved *reserved) {
	/*
	 * It is possible (but unlikely), that application
	 * code could have disposed the widget in the traverse
	 * event.  If this happens, return true to stop further
	 * event processing.
	 */
	w_event_key tevent;
	memcpy(&tevent, event, sizeof(w_event_key));
	tevent.event.type = W_EVENT_TRAVERSE;
	int doit = _w_widget_send_event(W_WIDGET(control), (w_event*) &tevent);
	if (!w_widget_is_ok(W_WIDGET(control)))
		return W_TRUE;
	if (!doit)
		return W_FALSE;
	switch (event->detail) {
	case W_TRAVERSE_NONE:
		return W_TRUE;
	case W_TRAVERSE_ESCAPE:
		return reserved->traverse_escape(control, reserved);
	case W_TRAVERSE_RETURN:
		return reserved->traverse_return(control, reserved);
	case W_TRAVERSE_TAB_NEXT:
		return reserved->traverse_group(control, W_TRUE, reserved);
	case W_TRAVERSE_TAB_PREVIOUS:
		return reserved->traverse_group(control, W_FALSE, reserved);
	case W_TRAVERSE_ARROW_NEXT:
		return reserved->traverse_item(control, W_TRUE, reserved);
	case W_TRAVERSE_ARROW_PREVIOUS:
		return reserved->traverse_item(control, W_FALSE, reserved);
	case W_TRAVERSE_MNEMONIC:
		return reserved->traverse_mnemonic(control, event->character, reserved);
	case W_TRAVERSE_PAGE_NEXT:
		return reserved->traverse_page(control, W_TRUE, reserved);
	case W_TRAVERSE_PAGE_PREVIOUS:
		return reserved->traverse_page(control, W_FALSE, reserved);
	}
	return W_FALSE;
}
wresult _w_control_traverse_escape(w_control *control, wbool next,
		_w_control_reserved *reserved) {
	return W_FALSE;
}
w_control* computeTabList(w_control *root, wbool next, w_control *c);
wresult _w_control_traverse_group(w_control *control, wbool next,
		_w_control_reserved *reserved) {
	/*Control root = computeTabRoot();
	 Widget group = computeTabGroup();
	 Widget [] list = root.computeTabList ();
	 int length = list.length;
	 int index = 0;
	 while (index < length) {
	 if (list[index] == group)
	 break;
	 index++;
	 }*/
	/*
	 * It is possible (but unlikely), that application
	 * code could have disposed the widget in focus in
	 * or out events.  Ensure that a disposed widget is
	 * not accessed.
	 */
	/*if (index == length)
	 return false;
	 int start = index, offset = (next) ? 1 : -1;
	 while ((index = ((index + offset + length) % length)) != start) {
	 Widget widget = list[index];
	 if (!widget.isDisposed() && widget.setTabGroupFocus(next)) {
	 return true;
	 }
	 }
	 if (group.isDisposed())
	 return false;
	 return group.setTabGroupFocus(next);*/
}

wresult _w_control_traverse_item(w_control *control, wbool next,
		_w_control_reserved *reserved) {
	_w_fixed *topHandle = (_w_fixed*) _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	if (next) {
		_w_fixed *child = topHandle->next;
		while (child != topHandle) {
			if (child == 0) {
				w_composite *parent = _W_CONTROL(control)->parent;
				_w_composite_reserved *preserved = _W_COMPOSITE_GET_RESERVED(
						parent);
				_w_fixed *hparent = (_w_fixed*) preserved->parenting_handle(
						W_WIDGET(parent), _W_WIDGET_RESERVED(preserved));
				child = hparent->first;
				if (child == topHandle)
					break;
			}
			if (w_widget_is_ok(W_WIDGET(child))) {
				_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
				if (creserved->is_tab_item(W_CONTROL(child), creserved) > 0) {
					if (_W_WIDGET_RESERVED(creserved)->set_tab_item_focus(
							W_WIDGET(child), next,
							_W_WIDGET_RESERVED(creserved))) {
						return W_TRUE;
					}
				}
			}
			child = child->next;
		}
	} else {
		_w_fixed *child = topHandle->prev;
		while (child != topHandle) {
			if (w_widget_is_ok(W_WIDGET(child))) {
				_w_control_reserved *creserved = _W_CONTROL_GET_RESERVED(child);
				if (creserved->is_tab_item(W_CONTROL(child), creserved) > 0) {
					if (_W_WIDGET_RESERVED(creserved)->set_tab_item_focus(
							W_WIDGET(child), next,
							_W_WIDGET_RESERVED(creserved))) {
						return W_TRUE;
					}
				}
			}
			child = child->prev;
		}
	}
	return W_FALSE;
}

gboolean _w_control_traverse_return(w_control *control,
		_w_control_reserved *reserved) {
	return FALSE;
}

gboolean _w_control_traverse_page(w_control *control, gboolean next,
		_w_control_reserved *reserved) {
	return FALSE;
}

gboolean _w_control_traverse_mnemonic(w_control *control, int key,
		_w_control_reserved *reserved) {
	return reserved->mnemonic_hit(control, key, reserved);
}
wresult _w_control_update_0(w_control *control, wbool all, wbool flush) {
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(control));
	GtkWidget *topHandle = reserved->topHandle(W_WIDGET(control), reserved);
	if (!gtk_widget_get_visible(topHandle))
		return W_FALSE;
	if (!gtk_widget_get_realized(_W_WIDGET(control)->handle))
		return W_FALSE;
	GdkWindow *window = _W_CONTROL_RESERVED(reserved)->paintWindow(
			W_WIDGET(control), reserved);
	if (flush) {
		//display.flushExposes(window, all);
		gdk_display_flush(gdk_display_get_default());
		gdk_display_flush(gdk_display_get_default());
	}
	/*
	 * Do not send expose events on GTK 3.16.0+
	 * It's worth checking whether can be removed on all GTK 3 versions.
	 */
	if (GTK_VERSION < VERSION(3, 16, 0)) {
		gdk_window_process_updates(window, all);
	}
	gdk_display_flush(gdk_display_get_default());
	return W_TRUE;
}
wresult _w_control_update(w_control *control) {
	return _w_control_update_0(control, W_FALSE, W_TRUE);
}
void _w_control_update_background_mode(w_control *control,
		_w_control_reserved *reserved) {
	int oldState = _W_WIDGET(control)->state & STATE_PARENT_BACKGROUND;
	reserved->check_background(control, reserved);
	if (oldState != (_W_WIDGET(control)->state & STATE_PARENT_BACKGROUND)) {
		reserved->set_background(control, reserved);
	}
}
void _w_control_update_layout(w_control *control, int flags,
		_w_control_reserved *reserved) {
	/* Do nothing */
}
void _w_control_get_window_origin(w_control *control, w_point *pt,
		_w_control_reserved *reserved) {
	GdkWindow *window = reserved->eventWindow(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	gdk_window_get_origin(window, &pt->x, &pt->y);
}
GdkWindow* _w_control_enableWindow(w_widget *control,
		_w_control_reserved *reserved) {
	return 0;
}
GdkWindow* _w_control_redrawWindow(w_widget *control,
		_w_control_reserved *reserved) {
	return 0;
}
gboolean _w_timer_listenner(gpointer user_data) {
	_w_control_timer *timer = (_w_control_timer*) user_data;
	w_event_time e;
	e.event.type = W_EVENT_TIMER;
	e.event.platform_event = 0;
	e.event.time = 0;
	e.event.widget = W_WIDGET(timer->control);
	e.event.data = 0;
	_w_widget_send_event(W_WIDGET(timer->control), (w_event*) &e);
	return TRUE;
}
wresult _w_control_set_timer(w_control *control, wint64 ms, unsigned short id) {
	_w_control_timer *timer, *last;
	if (_W_CONTROL(control)->timer.control == 0) {
		_W_CONTROL(control)->timer.control = control;
		_W_CONTROL(control)->timer.id = id;
		timer = &_W_CONTROL(control)->timer;
	} else {
		timer = &_W_CONTROL(control)->timer;
		while (timer != 0) {
			if (timer->id == id)
				break;
			last = timer;
			timer = timer->next;
		}
		if (timer == 0) {
			timer = malloc(sizeof(_w_control_timer));
			if (timer == 0)
				return W_ERROR_NO_MEMORY;
			last->next = timer;
			timer->next = 0;
			timer->control = control;
			timer->id = id;
		} else {
			g_source_remove_by_user_data(timer);
		}
	}
	g_timeout_add(ms, _w_timer_listenner, timer);
	return W_TRUE;
}
void _w_control_kill_all_timer(w_control *control) {
	_w_control_timer *timer, *next;
	timer = &_W_CONTROL(control)->timer;
	if (timer->control != 0) {
		g_source_remove_by_user_data(timer);
	}
	timer = timer->next;
	while (timer != 0) {
		g_source_remove_by_user_data(timer);
		next = timer->next;
		free(timer);
		timer = next;
	}
}
wresult _w_control_kill_timer(w_control *control, unsigned short id) {
	_w_control_timer *timer, *last;
	timer = &_W_CONTROL(control)->timer;
	if (timer->control == 0) {
		last = timer;
		timer = timer->next;
	}
	while (timer != 0) {
		if (timer->id == id)
			break;
		last = timer;
		timer = timer->next;
	}
	if (timer != 0) {
		g_source_remove_by_user_data(timer);
		if (timer == &_W_CONTROL(control)->timer) {
			timer->control = 0;
		} else {
			if (last != 0)
				last->next = timer->next;
			free(timer);
		}
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_control_set_tab(w_control *control, wbool tab) {
	if (tab) {
		_W_WIDGET(control)->state |= STATE_TAB_LIST;
	} else {
		_W_WIDGET(control)->state &= ~STATE_TAB_LIST;
	}
	return W_TRUE;
}
wresult _w_control_get_tab(w_control *control) {
	return (_W_WIDGET(control)->state & STATE_TAB_LIST) != 0;

}
wresult _w_control_create_dragsource_effect(w_control *control,
		w_dragsource_effect *effect) {

}
wresult _w_control_create_droptarget_effect(w_control *control,
		w_droptarget_effect *effect) {

}
void _w_control_class_init(struct _w_control_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
//clazz
	W_WIDGET_CLASS(clazz)->create = _w_control_create;
	clazz->get_graphics = _w_control_get_graphics;
	clazz->get_orientation = _w_control_get_orientation;
	clazz->set_orientation = _w_control_set_orientation;
	clazz->get_text_direction = _w_control_get_text_direction;
	clazz->set_text_direction = _w_control_set_text_direction;
	clazz->print = _w_control_print;
	clazz->get_accessible = _w_control_get_accessible;
	clazz->get_bounds = _w_control_get_bounds;
	clazz->set_bounds = _w_control_set_bounds;
	clazz->get_region = _w_control_get_region;
	clazz->set_region = _w_control_set_region;
	clazz->move_above = _w_control_move_above;
	clazz->move_below = _w_control_move_below;
	clazz->pack = _w_control_pack;
	clazz->to_control = _w_control_to_control;
	clazz->to_display = _w_control_to_display;
	clazz->drag_detect = _w_control_drag_detect;
	clazz->get_drag_detect = _w_control_get_drag_detect;
	clazz->set_drag_detect = _w_control_set_drag_detect;
	clazz->set_focus = _w_control_set_focus;
	clazz->force_focus = _w_control_force_focus;
	clazz->is_focus_control = _w_control_is_focus_control;
	clazz->get_border_width = _w_control_get_border_width;
	clazz->get_cursor = _w_control_get_cursor;
	clazz->set_cursor = _w_control_set_cursor;
	clazz->get_enabled = _w_control_get_enabled;
	clazz->set_enabled = _w_control_set_enabled;
	clazz->is_enabled = _w_control_is_enabled;
	clazz->get_font = _w_control_get_font;
	clazz->set_font = _w_control_set_font;
	clazz->get_background = _w_control_get_background;
	clazz->set_background = _w_control_set_background;
	clazz->get_foreground = _w_control_get_foreground;
	clazz->set_foreground = _w_control_set_foreground;
	clazz->get_layout_data = _w_control_get_layout_data;
	clazz->new_layout_data = _w_control_new_layout_data;
	clazz->request_layout = _w_control_request_layout;
	clazz->get_menu = _w_control_get_menu;
	clazz->set_menu = _w_control_set_menu;
	clazz->get_parent = _w_control_get_parent;
	clazz->set_parent = _w_control_set_parent;
	clazz->is_reparentable = _w_control_is_reparentable;
	clazz->get_shell = _w_control_get_shell;
	clazz->get_toolTip_text = _w_control_get_toolTip_text;
	clazz->set_toolTip_text = _w_control_set_toolTip_text;
	clazz->get_touch_enabled = _w_control_get_touch_enabled;
	clazz->set_touch_enabled = _w_control_set_touch_enabled;
	clazz->get_visible = _w_control_get_visible;
	clazz->set_visible = _w_control_set_visible;
	clazz->is_visible = _w_control_is_visible;
	clazz->redraw = _w_control_redraw;
	clazz->set_redraw = _w_control_set_redraw;
	clazz->set_capture = _w_control_set_capture;
	clazz->traverse = _w_control_traverse;
	clazz->update = _w_control_update;
	clazz->set_timer = _w_control_set_timer;
	clazz->kill_timer = _w_control_kill_timer;
	clazz->set_tab = _w_control_set_tab;
	clazz->get_tab = _w_control_get_tab;
	clazz->create_dragsource_effect = _w_control_create_dragsource_effect;
	clazz->create_droptarget_effect = _w_control_create_droptarget_effect;
	/*
	 *	reserved
	 */
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);

	_W_WIDGET_RESERVED(reserved)->topHandle = _w_control_topHandle;
	_W_WIDGET_RESERVED(reserved)->create_widget = _w_control_create_widget;
	_W_WIDGET_RESERVED(reserved)->hook_events = _w_control_hook_events;
	reserved->fixedHandle = _w_widget_h;
	reserved->clientHandle = _w_widget_h;
	reserved->focusHandle = _w_widget_h;
	reserved->eventHandle = _w_widget_h;
	reserved->enterExitHandle = _w_control_enterExitHandle;
	reserved->paintHandle = _w_control_paintHandle;
	reserved->imHandle = _w_control_imHandle;
	reserved->fontHandle = _w_widget_h;
	reserved->paintWindow = _w_control_paintWindow;
	reserved->eventWindow = _w_control_eventWindow;
	reserved->enableWindow = _w_control_enableWindow;
	reserved->redrawWindow = _w_control_redrawWindow;
	reserved->force_resize = _w_control_force_resize;
	reserved->get_client_width = _w_control_get_client_width;
	reserved->set_bounds = _w_control_set_bounds_0;
	reserved->move_handle = _w_control_move_handle;
	reserved->resize_handle = _w_control_resize_handle;
	reserved->move_children = _w_control_move_children;
	reserved->draw_widget = _w_control_draw_widget;
	reserved->set_orientation = _w_control_set_orientation;
	reserved->set_ZOrder = _w_control_set_ZOrder;
	reserved->child_style = _w_control_child_style;
	reserved->find_background_control = _w_control_find_background_control;
	reserved->set_background_color = _w_control_set_background_color;
	reserved->set_background = _w_control_set_background_0;
	reserved->set_font_description = _w_control_set_font_description;
	reserved->set_foreground_color = _w_control_set_foreground_color_0;
	reserved->set_foreground_color_0 = _w_control_set_foreground_color_1;
	reserved->set_parent_background = _w_control_set_parent_background;
	reserved->set_widget_background = _w_control_set_widget_background;
	reserved->get_background_color = _w_control_get_background_color;
	reserved->get_foreground_color = _w_control_get_foreground_color;
	reserved->check_border = _w_control_check_border;
	reserved->check_buffered = _w_control_check_buffered;
	reserved->check_background = _w_control_check_background;
	reserved->check_foreground = _w_control_check_foreground;
	reserved->check_mirrored = _w_control_check_mirrored;
	reserved->check_subwindow = _w_control_check_subwindow;
	reserved->fix_focus = _w_control_fix_focus;
	reserved->fix_style = _w_control_fix_style;
	reserved->fix_style_0 = _w_control_fix_style_0;
	reserved->fix_modal = _w_control_fix_modal;
	reserved->has_focus = _w_control_has_focus;
	reserved->show_widget = _w_control_show_widget;
	reserved->set_initial_bounds = _w_control_set_initial_bounds;
	reserved->set_relations = _w_control_set_relations;
	reserved->set_cursor = _w_control_set_cursor_0;
	reserved->traverse_escape = _w_control_traverse_escape;
	reserved->traverse_item = _w_control_traverse_item;
	reserved->traverse_group = _w_control_traverse_group;
	reserved->traverse_return = _w_control_traverse_return;
	reserved->traversal_code = _w_control_traversal_code;
	reserved->traverse_page = _w_control_traverse_page;
	reserved->traverse_mnemonic = _w_control_traverse_mnemonic;
	reserved->mnemonic_hit = _w_control_mnemonic_hit;
	reserved->mnemonic_match = _w_control_mnemonic_match;
	reserved->translate_mnemonic = _w_control_translate_mnemonic;
	reserved->translate_traversal = _w_control_translate_traversal;
	reserved->compute_tab_group = _w_control_compute_tab_group;
	reserved->compute_tab_root = _w_control_compute_tab_root;
	//reserved->compute_tab_list=_w_control_compute_tab_list;
	reserved->is_tab_group = _w_control_is_tab_group;
	reserved->is_tab_item = _w_control_is_tab_item;
	reserved->enable_widget = _w_control_enable_widget;
	reserved->get_imcaret_pos = _w_control_get_imcaret_pos;
	reserved->redraw_widget = _w_control_redraw_widget;
	reserved->redraw_children = _w_control_redraw_children;
	reserved->force_focus = _w_control_force_focus_0;
	reserved->mark_layout = _w_control_mark_layout;
	reserved->update_layout = _w_control_update_layout;
#if GTK2
	reserved->print_widget=_w_control_print_widget;
#endif
	reserved->update_background_mode = _w_control_update_background_mode;
	reserved->send_leave_notify = _w_control_send_leave_notify;
	reserved->is_described_by_label = _w_control_is_described_by_label;
	reserved->is_focus_handle = _w_control_is_focus_handle;
	reserved->add_relation = _w_control_add_relation;
	reserved->get_window_origin = _w_control_get_window_origin;
	/*
	 * signals
	 */
	_gtk_signal *signals = reserved->widget.signals;
	signals[SIGNAL_DESTROY] = _gtk_control_destroy;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_control_button_press_event;
	signals[SIGNAL_BUTTON_RELEASE_EVENT] = _gtk_control_button_release_event;
	signals[SIGNAL_COMMIT] = _gtk_control_commit;
	signals[SIGNAL_ENTER_NOTIFY_EVENT] = _gtk_control_enter_notify_event;
	signals[SIGNAL_EVENT_AFTER] = _gtk_control_event_after;
	signals[SIGNAL_DRAW] = _gtk_control_draw;
	signals[SIGNAL_EXPOSE_EVENT_INVERSE] = _gtk_control_expose_inverse;
	signals[SIGNAL_FOCUS] = _gtk_control_focus;
	signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_control_focus_in_event;
	signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_control_focus_out_event;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_control_key_press_event;
	signals[SIGNAL_KEY_RELEASE_EVENT] = _gtk_control_key_release_event;
	signals[SIGNAL_LEAVE_NOTIFY_EVENT] = _gtk_control_leave_notify_event;
	signals[SIGNAL_MNEMONIC_ACTIVATE] = _gtk_control_mnemonic_activate;
	signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_control_motion_notify_event;
	signals[SIGNAL_POPUP_MENU] = _gtk_control_popup_menu;
	signals[SIGNAL_PREEDIT_CHANGED] = _gtk_control_preedit_changed;
	signals[SIGNAL_REALIZE] = _gtk_control_realize;
	signals[SIGNAL_SCROLL_EVENT] = _gtk_control_scroll_event;
	signals[SIGNAL_SHOW_HELP] = _gtk_control_show_help;
	signals[SIGNAL_STYLE_SET] = _gtk_control_style_set;
	signals[SIGNAL_UNREALIZE] = _gtk_control_unrealize;
}
#endif

