/*
 * sash.c
 *
 *  Created on: 8 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "sash.h"
#include "../widgets/toolkit.h"
#define INCREMENT 1
#define  PAGE_INCREMENT 9
wuint64 _w_sash_check_style(w_widget *control, wuint64 style) {
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
unsigned char draw_band_bits[] = { -86, 85, -86, 85, -86, 85, -86, 85 };
void _w_sash_draw_band(w_widget *widget, int x, int y, int width, int height) {
}
wresult _w_sash_set_cursor(w_control *control, w_cursor *cursor) {
	if (cursor == 0) {
		w_toolkit *toolkit = w_widget_get_toolkit(W_WIDGET(control));
		int cursor_id;
		if (_W_WIDGET(control)->style & W_HORIZONTAL) {
			cursor_id = W_CURSOR_SIZENS;
		} else {
			cursor_id = W_CURSOR_SIZEWE;
		}
		cursor = w_toolkit_get_system_cursor(toolkit, cursor_id);
	}
	return _w_control_set_cursor(control, cursor);
}
wresult _w_sash_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	_W_WIDGET(widget)->handle = _w_fixed_new();
	if (_W_WIDGET(widget)->handle == 0) {
		return W_ERROR_NO_HANDLES;
	}
	gtk_widget_set_has_window(_W_WIDGET(widget)->handle, TRUE);
	gtk_widget_set_can_focus(_W_WIDGET(widget)->handle, TRUE);
	g_object_set_qdata(G_OBJECT(_W_WIDGET(widget)->handle),
			gtk_toolkit->quark[0], widget);
	_w_sash_set_cursor(W_CONTROL(widget), 0);
	return W_TRUE;
}
void _w_sash_hook_events(w_widget *widget, _w_widget_reserved *reserved) {
	_w_control_hook_events(widget, reserved);
	gtk_widget_add_events(_W_WIDGET(widget)->handle,
			GDK_POINTER_MOTION_HINT_MASK);
}
gboolean _gtk_sash_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_button_press_event(widget, e, reserved);
	if (result != 0)
		return result;
	GdkEventButton *gdkEvent = e->args[0];
	int button = gdkEvent->button;
	if (button != 1)
		return 0;
	if (gdkEvent->type == GDK_2BUTTON_PRESS) {
		e->result = 0;
		return W_TRUE;
	}
	if (gdkEvent->type == GDK_3BUTTON_PRESS) {
		e->result = 0;
		return W_TRUE;
	}
	GdkWindow *window = gtk_widget_get_window(
	_W_WIDGET(widget)->handle);
	gint origin_x, origin_y;
	gdk_window_get_origin(window, &origin_x, &origin_y);
	_W_SASH(widget)->startX = (int) (gdkEvent->x_root - origin_x);
	_W_SASH(widget)->startY = (int) (gdkEvent->y_root - origin_y);
	GtkAllocation allocation;
	gtk_widget_get_allocation(_W_WIDGET(widget)->handle, &allocation);
	int x = allocation.x;
	int y = allocation.y;
	int width = allocation.width;
	int height = allocation.height;
	_W_SASH(widget)->lastX = x;
	_W_SASH(widget)->lastY = y;
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.bounds.x = _W_SASH(widget)->lastX;
	event.bounds.y = _W_SASH(widget)->lastY;
	event.bounds.width = width;
	event.bounds.height = height;
	/*if ((style & SWT.SMOOTH) == 0) {
	 event.detail = SWT.DRAG;
	 }
	 if ((parent.style & SWT.MIRRORED) != 0) event.x = DPIUtil.autoScaleDown (parent.getClientWidth () - width) - event.x;*/
	int doit = _w_widget_send_event(widget, (w_event*) &event);
	//if (isDisposed ()) return 0;
	if (doit) {
		_W_WIDGET(widget)->state |= STATE_SASH_DRAGGING;
		//Rectangle rect = DPIUtil.autoScaleUp (event.getBounds ());
		_W_SASH(widget)->lastX = event.bounds.x;
		_W_SASH(widget)->lastY = event.bounds.y;
		/*if ((parent.style & SWT.MIRRORED) != 0) _W_SASH(widget)->lastX = parent.getClientWidth () - width - _W_SASH(widget)->lastX;
		 parent.update (true, (style & SWT.SMOOTH) == 0);*/
		_w_sash_draw_band(widget, _W_SASH(widget)->lastX, event.bounds.y, width,
				height);
		/*if ((style & SWT.SMOOTH) != 0) {
		 setBoundsInPixels (rect.x, rect.y, width, height);
		 }*/
	}
	return result;
}

gboolean _gtk_sash_button_release_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_button_release_event(widget, e, reserved);
	if (result != 0)
		return result;
	GdkEventButton *gdkEvent = e->args[0];
	int button = gdkEvent->button;
	if (button != 1) {
		e->result = 0;
		return W_TRUE;
	}
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)) {
		return 0;
	}
	_W_WIDGET(widget)->state &= ~STATE_SASH_DRAGGING;
	GtkAllocation allocation;
	gtk_widget_get_allocation(_W_WIDGET(widget)->handle, &allocation);
	int width = allocation.width;
	int height = allocation.height;
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.bounds.x = _W_SASH(widget)->lastX;
	event.bounds.y = _W_SASH(widget)->lastY;
	event.bounds.width = width;
	event.bounds.height = height;
	_w_sash_draw_band(widget, _W_SASH(widget)->lastX,
	_W_SASH(widget)->lastY, width, height);
	//if ((parent.style & SWT.MIRRORED) != 0) event.x = DPIUtil.autoScaleDown (parent.getClientWidth () - width) - event.x;
	int doit = _w_widget_send_event(widget, (w_event*) &event);
	//if (isDisposed ()) return result;
	if (doit) {
		/*if ((style & SWT.SMOOTH) != 0) {
		 Rectangle rect = DPIUtil.autoScaleUp (event.getBounds ());
		 setBoundsInPixels (rect.x, rect.y, width, height);
		 }*/
	}
	return W_TRUE;
}

gboolean _gtk_sash_draw(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
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
}

gboolean _gtk_sash_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_focus_in_event(widget, e, reserved);
	if (result != 0)
		return result;
	// widget could be disposed at this point
	if (_W_WIDGET(widget)->handle != 0) {
		GtkAllocation allocation;
		gtk_widget_get_allocation(_W_WIDGET(widget)->handle, &allocation);
		_W_SASH(widget)->lastX = allocation.x;
		_W_SASH(widget)->lastY = allocation.y;
	}
	return FALSE;
}

gboolean _gtk_sash_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	int result = _gtk_control_key_press_event(widget, e, reserved);
	if (result != 0)
		return result;
	GdkEventKey *gdkEvent = (GdkEventKey*) e->args[1];
	int keyval = gdkEvent->keyval;
	switch (keyval) {
	case GDK_KEY_Left:
	case GDK_KEY_Right:
	case GDK_KEY_Up:
	case GDK_KEY_Down: {
		int xChange = 0, yChange = 0;
		int stepSize = PAGE_INCREMENT;
		if ((gdkEvent->state & GDK_CONTROL_MASK) != 0)
			stepSize = INCREMENT;
		if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
			if (keyval == GDK_KEY_Up || keyval == GDK_KEY_Down)
				break;
			xChange = keyval == GDK_KEY_Left ? -stepSize : stepSize;
		} else {
			if (keyval == GDK_KEY_Left || keyval == GDK_KEY_Right)
				break;
			yChange = keyval == GDK_KEY_Up ? -stepSize : stepSize;
		}
		w_composite *parent;
		w_control_get_parent(W_CONTROL(widget), &parent);
		int parentBorder = 0;
		GtkAllocation allocation;
		gtk_widget_get_allocation(_W_WIDGET(widget)->handle, &allocation);
		int width = allocation.width;
		int height = allocation.height;
		gtk_widget_get_allocation(_W_WIDGET(parent)->handle, &allocation);
		int parentWidth = allocation.width;
		int parentHeight = allocation.height;
		int newX = _W_SASH(widget)->lastX, newY = _W_SASH(widget)->lastY;
		if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
			newX = w_min(
					w_max(0,
							_W_SASH(widget)->lastX + xChange - parentBorder
									- _W_SASH(widget)->startX),
					parentWidth - width);
		} else {
			newY = w_min(
					w_max(0,
							_W_SASH(widget)->lastY + yChange - parentBorder
									- _W_SASH(widget)->startY),
					parentHeight - height);
		}
		if (newX == _W_SASH(widget)->lastX && newY == _W_SASH(widget)->lastY)
			return result;

		/* Ensure that the pointer image does not change */
		GdkWindow *window = gtk_widget_get_window(
		_W_WIDGET(widget)->handle);
		GdkEventMask grabMask = GDK_POINTER_MOTION_MASK
				| GDK_BUTTON_RELEASE_MASK;
		GdkCursor *gdkCursor;
		if (_W_CONTROL(widget)->cursor != 0) {
			gdkCursor = _W_CURSOR(_W_CONTROL(widget)->cursor)->handle;
		} else {
			if (_W_WIDGET(widget)->style & W_HORIZONTAL) {
				gdkCursor =
						_W_CURSOR(w_toolkit_get_system_cursor((w_toolkit*)gtk_toolkit,W_CURSOR_SIZENS))->handle;
			} else {
				gdkCursor =
						_W_CURSOR(w_toolkit_get_system_cursor((w_toolkit*)gtk_toolkit,W_CURSOR_SIZEWE))->handle;
			}
		}
		GdkGrabStatus ptrGrabResult = gdk_pointer_grab(window,
		FALSE, grabMask, window, gdkCursor,
		GDK_CURRENT_TIME);

		/* The event must be sent because its doit flag is used. */
		w_event_sash event;
		event.event.type = W_EVENT_SELECTION;
		event.event.platform_event = (w_event_platform*) e;
		event.event.data = 0;
		event.event.widget = 0;
		event.bounds.x = newX;
		event.bounds.y = newY;
		event.bounds.width = width;
		event.bounds.height = height;
		if (_W_WIDGET(parent)->style & W_MIRRORED) {
			//event.bounds.x = parent.getClientWidth () - width - event.bounds.x;
		}
		int doit = _w_widget_send_event(widget, (w_event*) &event);
		if (ptrGrabResult == GDK_GRAB_SUCCESS)
			gdk_pointer_ungrab(GDK_CURRENT_TIME);
		if (!w_widget_is_ok(W_WIDGET(widget))) {
			e->result = 0;
			return result;
		}

		if (doit) {
			_W_SASH(widget)->lastX = event.bounds.x;
			_W_SASH(widget)->lastY = event.bounds.y;
			if (_W_WIDGET(parent)->style & W_MIRRORED) {
				//_W_SASH(widget)->lastX = parent.getClientWidth () - width  - _W_SASH(widget)->lastX;
			}
			if ((_W_WIDGET(parent)->style & W_SMOOTH) != 0) {
				w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt,
						&event.bounds.sz);
				if (!w_widget_is_ok(W_WIDGET(widget))) {
					e->result = 0;
					return result;
				}
			}
			int cursorX = event.bounds.x, cursorY = event.bounds.y;
			if ((_W_WIDGET(parent)->style & W_VERTICAL) != 0) {
				cursorY += height / 2;
			} else {
				cursorX += width / 2;
			}
			//display.setCursorLocation (cursorX, cursorY);
		}
	}
		break;
	}

	return result;
}

gboolean _gtk_sash_motion_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_motion_notify_event(widget, e, reserved);
	if (result != 0)
		return result;
	if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)) {
		e->result = 0;
		return W_TRUE;
	}
	GdkEventMotion *gdkEvent = e->args[0];
	int eventX, eventY, eventState;
	if (gdkEvent->is_hint != 0) {
		int pointer_x, pointer_y;
		GdkModifierType mask;
		gdk_window_get_device_position(gdkEvent->window,
				gdk_event_get_device((GdkEvent*) gdkEvent), &pointer_x,
				&pointer_y, &mask);
		eventX = pointer_x;
		eventY = pointer_y;
		eventState = mask;
	} else {
		int origin_x, origin_y;
		gdk_window_get_origin(gdkEvent->window, &origin_x, &origin_y);
		eventX = (int) (gdkEvent->x_root - origin_x);
		eventY = (int) (gdkEvent->y_root - origin_y);
		eventState = gdkEvent->state;
	}
	if ((eventState & GDK_BUTTON1_MASK) == 0)
		return 0;
	GtkAllocation allocation;
	gtk_widget_get_allocation(_W_WIDGET(widget)->handle, &allocation);
	int x = allocation.x;
	int y = allocation.y;
	int width = allocation.width;
	int height = allocation.height;
	int parentBorder = 0;
	w_composite *parent;
	w_control_get_parent(W_CONTROL(widget), &parent);
	gtk_widget_get_allocation(_W_WIDGET(parent)->handle, &allocation);
	int parentWidth = allocation.width;
	int parentHeight = allocation.height;
	int newX = _W_SASH(widget)->lastX, newY = _W_SASH(widget)->lastY;
	if ((_W_WIDGET(widget)->style & W_VERTICAL) != 0) {
		newX = w_min(
				w_max(0, eventX + x - _W_SASH(widget)->startX - parentBorder),
				parentWidth - width);
	} else {
		newY = w_min(
				w_max(0, eventY + y - _W_SASH(widget)->startY - parentBorder),
				parentHeight - height);
	}
	if (newX == _W_SASH(widget)->lastX && newY == _W_SASH(widget)->lastY)
		return 0;
	_w_sash_draw_band(widget, _W_SASH(widget)->lastX,
	_W_SASH(widget)->lastY, width, height);
	w_event_sash event;
	event.event.type = W_EVENT_SELECTION;
	event.event.widget = widget;
	event.event.data = 0;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.bounds.x = newX;
	event.bounds.y = newY;
	event.bounds.width = width;
	event.bounds.height = height;
	/*if ((style & SWT.SMOOTH) == 0) {
	 event.detail = SWT.DRAG;
	 }
	 if ((parent.style & SWT.MIRRORED) != 0) event.x = DPIUtil.autoScaleDown (parent.getClientWidth () - width) - event.x;*/
	int doit = _w_widget_send_event(widget, (w_event*) &event);
	//if (isDisposed ()) return 0;
	//Rectangle rect = DPIUtil.autoScaleUp (event.getBounds ());
	if (doit) {
		_W_SASH(widget)->lastX = event.bounds.x;
		_W_SASH(widget)->lastY = event.bounds.y;
		//if ((parent.style & SWT.MIRRORED) != 0) _W_SASH(widget)->lastX = parent.getClientWidth () - width  - _W_SASH(widget)->lastX;
	}
	/*parent.update (true, (style & SWT.SMOOTH) == 0);
	 drawBand (_W_SASH(widget)->lastX, _W_SASH(widget)->lastY, width, height);
	 if ((style & SWT.SMOOTH) != 0) {
	 setBoundsInPixels (rect.x, _W_SASH(widget)->lastY, width, height);
	 }*/
	return result;
}
gboolean _gtk_sash_realize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	_w_sash_set_cursor(W_CONTROL(widget), _W_CONTROL(widget)->cursor);
	return _gtk_control_realize(widget, e, reserved);
}
int _w_sash_compute_size(w_widget *widget, struct w_event_compute_size *e,
		_w_widget_reserved *reserved) {
	if (e->wHint != W_DEFAULT && e->wHint < 0)
		e->wHint = 0;
	if (e->hHint != W_DEFAULT && e->hHint < 0)
		e->hHint = 0;
	int border = W_CONTROL_GET_CLASS(widget)->get_border_width(
			W_CONTROL(widget));
	e->size->width = border * 2;
	e->size->height = border * 2;
	if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
		e->size->width += DEFAULT_WIDTH;
		e->size->height += 3;
	} else {
		e->size->width += 3;
		e->size->height += DEFAULT_HEIGHT;
	}
	if (e->wHint != W_DEFAULT)
		e->size->width = e->wHint + (border * 2);
	if (e->hHint != W_DEFAULT)
		e->size->height = e->hHint + (border * 2);
	return W_TRUE;
}
void _w_sash_class_init(struct _w_sash_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SASH;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_sash_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_sash);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_sash);
	/*
	 * reserved
	 */
	struct _w_sash_reserved *reserved = _W_SASH_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_h;
	_W_WIDGET_RESERVED(reserved)->create_handle = _w_sash_create_handle;
	_W_WIDGET_RESERVED(reserved)->check_style = _w_sash_check_style;
	_W_WIDGET_RESERVED(reserved)->hook_events = _w_sash_hook_events;
	_W_WIDGET_RESERVED(reserved)->compute_size = _w_sash_compute_size;
	_W_CONTROL_RESERVED(reserved)->fixedHandle = _w_widget_h;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_RESERVED(reserved)->signals;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_sash_button_press_event;
	signals[SIGNAL_BUTTON_RELEASE_EVENT] = _gtk_sash_button_release_event;
	signals[SIGNAL_DRAW] = _gtk_sash_draw;
	signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_sash_focus_in_event;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_sash_key_press_event;
	signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_sash_motion_notify_event;
	signals[SIGNAL_REALIZE] = _gtk_sash_realize;
}

#endif
