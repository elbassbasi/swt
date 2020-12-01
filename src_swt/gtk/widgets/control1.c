/*
 * control1.c
 *
 *  Created on: 21 sept. 2020
 *      Author: azeddine
 */
#ifdef __linux
#include "control.h"
#include "toolkit.h"
#include "menu.h"
#include <sys/time.h>
/*
 * signals
 */
gboolean _gtk_control_destroy(w_widget *widget, _w_event_platform *ee,
		_w_widget_reserved *reserved) {
	w_event e;
	if (_W_CONTROL(widget)->dragsource != 0) {
		e.type = W_EVENT_DISPOSE;
		e.widget = W_WIDGET(_W_CONTROL(widget)->dragsource);
		e.data = 0;
		e.time = 0;
		e.platform_event = (w_event_platform*) ee;
		_w_widget_send_event(W_WIDGET(_W_CONTROL(widget)->dragsource),
				(w_event*) &e);
	}
	if (_W_CONTROL(widget)->droptarget != 0) {
		e.type = W_EVENT_DISPOSE;
		e.widget = W_WIDGET(_W_CONTROL(widget)->droptarget);
		e.data = 0;
		e.time = 0;
		e.platform_event = (w_event_platform*) ee;
		_w_widget_send_event(W_WIDGET(_W_CONTROL(widget)->droptarget),
				(w_event*) &e);
	}
	if (!w_widget_is_ok(widget))
		return TRUE;
	wuint64 style = w_widget_get_style(widget);
	_w_control_kill_all_timer(W_CONTROL(widget));
	e.type = W_EVENT_DISPOSE;
	e.widget = widget;
	e.data = 0;
	e.time = 0;
	e.platform_event = (w_event_platform*) ee;
	w_widget_send_event(widget, &e);
	widget->clazz = 0;
	if (style & W_FREE_MEMORY) {
		_w_toolkit_registre_free(widget);
		/*e.type = W_EVENT_FREE_MEMORY;
		 e.widget = widget;
		 e.data = 0;
		 e.time = 0;
		 e.platform_event = (w_event_platform*) ee;
		 w_widget_send_event(widget, &e);*/
	}
	return FALSE;
}
gboolean _w_control_contained_in_region(w_widget *widget, int x, int y,
		_w_widget_reserved *reserved) {
	GtkWidget *topHandle = reserved->topHandle(widget, reserved);
	GdkWindow *window = gtk_widget_get_window(topHandle);
	/* if (drawRegion && eventRegion != 0) {
	 return cairo_region_contains_point(eventRegion, x, y);
	 }*/
	return FALSE;
}
gboolean _gtk_control_button_press_event_0(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved,
		int sendMouseDown) {
	GdkEventButton *gdkEvent = e->args[0];
	w_event_mouse event;
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, _W_CONTROL(widget)->lastInput.x,
	_W_CONTROL(widget)->lastInput.y, reserved))
		return FALSE;
	if (gdkEvent->type == GDK_3BUTTON_PRESS)
		return 0;

	/*
	 * When a shell is created with SWT.ON_TOP and SWT.NO_FOCUS,
	 * do not activate the shell when the user clicks on the
	 * the client area or on the border or a control within the
	 * shell that does not take focus.
	 */
	w_shell *shell;
	w_control_get_shell(W_CONTROL(widget), &shell);
	if (((_W_WIDGET(shell)->style & W_ON_TOP) != 0)
			&& (((_W_WIDGET(shell)->style & W_NO_FOCUS) == 0)
					|| ((_W_WIDGET(shell)->style & W_NO_FOCUS) == 0))) {
		_w_shell_force_active(shell);
	}
	gboolean result = 0;
	if (gdkEvent->type == GDK_BUTTON_PRESS) {
		gboolean dragging = FALSE;
		gtk_toolkit->clickCount = 1;
		GdkEvent *nextEvent = gdk_event_peek();
		if (nextEvent != 0) {
			int eventType = nextEvent->type;
			if (eventType == GDK_2BUTTON_PRESS)
				gtk_toolkit->clickCount = 2;
			if (eventType == GDK_3BUTTON_PRESS)
				gtk_toolkit->clickCount = 3;
			gdk_event_free(nextEvent);
		}
		/*
		 * Feature in GTK: DND detection for X.11 & Wayland support is done through motion notify event
		 * instead of mouse click event. See Bug 503431.
		 */
		if (isX11()) { // Wayland
			if ((_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0) {
				if (gdkEvent->button == 1) {
					gboolean consume = FALSE;
					if (_w_control_drag_detect_2(W_CONTROL(widget), gdkEvent->x,
							gdkEvent->y, TRUE,
							TRUE, &consume)) {
						dragging = TRUE;
						if (consume)
							result = 1;
					}
					if (!w_widget_is_ok(widget))
						return TRUE;
				}
			}
		}
		if (sendMouseDown) {
			event.event.type = W_EVENT_MOUSEDOWN;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent->time;
			event.event.data = 0;
			event.button = gdkEvent->button;
			event.clickcount = gtk_toolkit->clickCount;
			event.x = gdkEvent->x;
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				event.x = _W_CONTROL_RESERVED(reserved)->get_client_width(
						W_CONTROL(widget), _W_CONTROL_RESERVED(reserved))
						- event.x;
			}
			event.y = gdkEvent->y;
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(gdkEvent->state);
			result = _w_widget_send_event(widget, (w_event*) &event);
		}
		if (!w_widget_is_ok(widget))
			return TRUE;
		/*
		 * Feature in GTK: DND detection for X.11 & Wayland support is done through motion notify event
		 * instead of mouse click event. See Bug 503431.
		 */
		if (isX11()) { // Wayland
			if (dragging) {
				event.event.type = W_EVENT_DRAGDETECT;
				event.event.widget = widget;
				event.event.platform_event = (w_event_platform*) e;
				event.event.time = gdkEvent->time;
				event.event.data = 0;
				event.button = gdkEvent->button;
				event.clickcount = gtk_toolkit->clickCount;
				event.x = gdkEvent->x;
				if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
					event.x = _W_CONTROL_RESERVED(reserved)->get_client_width(
							W_CONTROL(widget), _W_CONTROL_RESERVED(reserved))
							- event.x;
				}
				event.y = gdkEvent->y;
				event.detail = 0;
				event.statemask = _w_widget_set_input_state(gdkEvent->state);
				_w_widget_send_event(widget, (w_event*) &event);
				if (!w_widget_is_ok(widget))
					return TRUE;
				if (_W_CONTROL(widget)->dragsource != 0) {
					event.event.widget = W_WIDGET(
							_W_CONTROL(widget)->dragsource);
					_w_widget_send_event(
							W_WIDGET(_W_CONTROL(widget)->dragsource),
							(w_event*) &event);
					if (!w_widget_is_ok(widget))
						return TRUE;
				}
			}
		}
		/*
		 * Pop up the context menu in the button press event for widgets
		 * that have default operating system menus in order to stop the
		 * operating system from displaying the menu if necessary.
		 */
		if ((_W_WIDGET(widget)->state & STATE_MENU) != 0) {
			if (gdkEvent->button == 3) {
				if (_w_control_show_menu(W_CONTROL(widget),
						(int) gdkEvent->x_root, (int) gdkEvent->y_root,
						W_MENU_MOUSE)) {
					result = TRUE;
				}
			}
		}
	} else {
		gtk_toolkit->clickCount = 2;
		event.event.type = W_EVENT_MOUSEDOUBLECLICK;
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = gdkEvent->time;
		event.button = gdkEvent->button;
		event.clickcount = gtk_toolkit->clickCount;
		event.x = gdkEvent->x_root;
		event.y = gdkEvent->y_root;
		event.detail = 0;
		event.statemask = _w_widget_set_input_state(gdkEvent->state);
		result = _w_widget_send_event(widget, (w_event*) &event);

		if (!w_widget_is_ok(widget))
			return TRUE;
	}
	if (w_widget_is_ok(W_WIDGET(shell))) {
		_w_shell_set_active_control_0(shell, W_CONTROL(widget),
				W_EVENT_MOUSEDOWN);
	}
	return result;
}
gboolean _gtk_control_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return _gtk_control_button_press_event_0(widget, e, reserved, TRUE);
}
gboolean _gtk_control_button_release_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved) {
	GdkEventButton *gdkEvent = e->args[0];
	w_event_mouse event;
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, _W_CONTROL(widget)->lastInput.x,
	_W_CONTROL(widget)->lastInput.y, reserved))
		return FALSE;
	memset(&event, 0, sizeof(event));
	event.event.type = W_EVENT_MOUSEUP;
	event.event.widget = widget;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.button = gdkEvent->button;
	event.clickcount = gtk_toolkit->clickCount;
	event.x = gdkEvent->x;
	event.y = gdkEvent->y;
	event.detail = 0;
	event.statemask = _w_widget_set_input_state(gdkEvent->state);
	_w_widget_send_event(widget, (w_event*) &event);
	return 0;
}

gboolean _gtk_control_commit(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	char *text = (char*) e->args[0];
	if (text == 0)
		return FALSE;
	int length = strlen(text);
	if (length == 0)
		return FALSE;
	_w_widget_send_IM_key_event(widget, e, W_EVENT_KEYDOWN, 0, text, length);
	return FALSE;
}

gboolean _gtk_control_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/*
	 * Feature in GTK. Children of a shell will inherit and display the shell's
	 * tooltip if they do not have a tooltip of their own. The fix is to use the
	 * new tooltip API in GTK 2.12 to null the shell's tooltip when the control
	 * being entered does not have any tooltip text set.
	 */
	/*byte [] buffer = null;
	 if (toolTipText != null && toolTipText.length() != 0) {
	 char [] chars = fixMnemonic (toolTipText, false);
	 buffer = Converter.wcsToMbcs(chars, true);
	 }
	 long toolHandle = getShell().handle;
	 GTK.gtk_widget_set_tooltip_text(toolHandle, buffer);*/

	if (gtk_toolkit->currentControl == W_CONTROL(widget))
		return FALSE;
	GdkEventCrossing *gdkEvent = (GdkEventCrossing*) e->args[0];
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, _W_CONTROL(widget)->lastInput.x,
	_W_CONTROL(widget)->lastInput.y, reserved))
		return FALSE;

	/*
	 * It is possible to send out too many enter/exit events if entering a
	 * control through a subwindow. The fix is to return without sending any
	 * events if the GdkEventCrossing subwindow field is set and the control
	 * requests to check the field.
	 */
	if (gdkEvent->subwindow != 0
			&& _W_CONTROL_RESERVED(reserved)->check_subwindow(W_CONTROL(widget),
					_W_CONTROL_RESERVED(reserved)))
		return FALSE;
	if (gdkEvent->mode != GDK_CROSSING_NORMAL
			&& gdkEvent->mode != GDK_CROSSING_UNGRAB)
		return FALSE;
	if ((gdkEvent->state
			& (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK)) != 0)
		return FALSE;
	if (w_widget_is_ok(W_WIDGET(gtk_toolkit->currentControl))) {
		/*display.removeMouseHoverTimeout(display.currentControl.handle);
		 display.currentControl.sendMouseEvent(SWT.MouseExit, 0, gdkEvent.time,
		 gdkEvent.x_root, gdkEvent.y_root, false, gdkEvent.state);*/
	}
	if (w_widget_is_ok(widget)) {
		gtk_toolkit->currentControl = W_CONTROL(widget);
		/*return sendMouseEvent(SWT.MouseEnter, 0, gdkEvent.time, gdkEvent.x_root,
		 gdkEvent.y_root, false, gdkEvent.state) ? 0 : 1;*/
	}
	return 0;
}

gboolean _gtk_control_event_after(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GdkEvent *event = (GdkEvent*) e->args[0];
	switch (event->type) {
	case GDK_BUTTON_PRESS: {
		if (e->widget
				!= _W_CONTROL_RESERVED(reserved)->eventHandle(widget, reserved))
			break;
		/*
		 * Pop up the context menu in the event_after signal to allow
		 * the widget to process the button press.  This allows widgets
		 * such as GtkTreeView to select items before a menu is shown.
		 */
		if ((_W_WIDGET(widget)->state & STATE_MENU) == 0) {
			GdkEventButton *gdkEventButton = (GdkEventButton*) event;
			if (gdkEventButton->button == 3) {
				_w_control_show_menu(W_CONTROL(widget), gdkEventButton->x_root,
						gdkEventButton->y_root, W_MENU_MOUSE);
			}
		}
		break;
	}
	case GDK_FOCUS_CHANGE: {
		if (_W_CONTROL_RESERVED(reserved)->is_focus_handle(W_CONTROL(widget),
				e->widget, _W_CONTROL_RESERVED(reserved)) <= 0)
			break;
		GdkEventFocus *gdkEventFocus = (GdkEventFocus*) event;

		/*
		 * Feature in GTK. The GTK combo box popup under some window managers
		 * is implemented as a GTK_MENU.  When it pops up, it causes the combo
		 * box to lose focus when focus is received for the menu.  The
		 * fix is to check the current grab handle and see if it is a GTK_MENU
		 * and ignore the focus event when the menu is both shown and hidden.
		 *
		 * NOTE: This code runs for all menus.
		 */
		if (gdkEventFocus->in != 0) {
			if (gtk_toolkit->ignoreFocus) {
				gtk_toolkit->ignoreFocus = FALSE;
				break;
			}
		} else {
			gtk_toolkit->ignoreFocus = FALSE;
			GtkWidget *grabHandle = gtk_grab_get_current();
			if (grabHandle != 0) {
				if (G_OBJECT_TYPE (grabHandle) == GTK_TYPE_MENU) {
					gtk_toolkit->ignoreFocus = TRUE;
					break;
				}
			}
		}
		w_event event;
		if (gdkEventFocus->in != 0) {
			event.type = W_EVENT_FOCUSIN;
		} else {
			event.type = W_EVENT_FOCUSOUT;
		}
		event.widget = widget;
		event.time = 0;
		event.platform_event = (w_event_platform*) e;
		event.data = 0;
		_w_widget_send_event(widget, &event);
		break;
	}
	}
	return FALSE;
}
gboolean _gtk_control_draw(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->state & STATE_OBSCURED) != 0)
		return 0;
#if USE_CAIRO
	cairo_t *cairo = (cairo_t*) e->args[0];
	w_event_paint event;
	_w_graphics gc;
	_w_graphics_init(W_GRAPHICS(&gc), cairo);
	gdk_cairo_get_clip_rectangle(cairo, &gc.clipping);
	/*
	 * Modify the drawing of the widget with cairo_clip.
	 * Doesn't modify input handling at this time.
	 * See bug 529431.
	 */
	/*if (drawRegion) {
	 cairoClipRegion(cairo);
	 }
	 if (!hooksPaint())
	 return 0;*/
	memset(&event, 0, sizeof(event));
//event.count = 1;
	event.event.type = W_EVENT_PAINT;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.event.data = 0;
	event.bounds.x = gc.clipping.x;
	event.bounds.y = gc.clipping.y;
	event.bounds.width = gc.clipping.width;
	event.bounds.height = gc.clipping.height;
	if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
		event.bounds.x = _w_control_get_client_width(W_CONTROL(widget),
				_W_CONTROL_RESERVED(reserved)) - event.bounds.width
				- event.bounds.x;
	}
	/*
	 * Pass the region into the GCData so that GC.fill* methods can be aware of the region
	 * and clip themselves accordingly. Only relevant on GTK3.10+, see bug 475784.
	 */
	/*if (drawRegion)
	 data.regionSet = eventRegion;*/
//	data.damageRgn = gdkEvent.region;
	event.gc = W_GRAPHICS(&gc);
// Note: use GC#setClipping(x,y,width,height) because GC#setClipping(Rectangle) got broken by bug 446075
	w_graphics_set_clipping_rect(W_GRAPHICS(&gc), &event.bounds);
	w_font *font;
	w_control_get_font(W_CONTROL(widget), &font);
	w_graphics_set_font(W_GRAPHICS(&gc), font);
	w_color background;
	w_control_get_background(W_CONTROL(widget), &background);
	w_graphics_set_background(W_GRAPHICS(&gc), background);
	w_color foreground;
	w_control_get_foreground(W_CONTROL(widget), &foreground);
	w_graphics_set_foreground(W_GRAPHICS(&gc), foreground);
	_W_CONTROL_RESERVED(reserved)->draw_widget(W_CONTROL(widget),
			W_GRAPHICS(&gc), _W_CONTROL_RESERVED(reserved));
	_w_widget_send_event(widget, (w_event*) &event);
	w_graphics_dispose(W_GRAPHICS(&gc));
#else
#endif
	return 0;
}
gboolean _gtk_control_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->state & STATE_OBSCURED) != 0)
		return FALSE;
#if USE_CAIRO
	w_control *control = 0;	// findBackgroundControl();
	int draw = control != 0 && _W_CONTROL(control)->backgroundImage != 0;
#if GTK3
	if (!draw && (_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		GdkRGBA rgba;
		GtkStyleContext *context = gtk_widget_get_style_context(
		_W_WIDGET(widget)->handle);
		if (GTK_VERSION < VERSION(3, 18, 0)) {
			gtk_style_context_get_background_color(context,
					GTK_STATE_FLAG_NORMAL, &rgba);
		} else {
			gtk_style_context_get_background_color(context,
					gtk_widget_get_state_flags(_W_WIDGET(widget)->handle),
					&rgba);
		}
		draw = rgba.alpha == 0;
	}
#endif
	if (draw) {
#if GTK3
		cairo_t *cairo = (cairo_t*) e->args[0];
		GdkRectangle rect;
		gdk_cairo_get_clip_rectangle(cairo, &rect);
		if (control == 0)
			control = W_CONTROL(widget);
		GdkWindow *window = gtk_widget_get_window(
		_W_WIDGET(widget)->handle);
		/*if (window != 0) {
		 drawBackground(control, window, 0, 0, rect.x, rect.y, rect.width,
		 rect.height);
		 } else {
		 drawBackground(control, 0, cairo, 0, rect.x, rect.y, rect.width,
		 rect.height);
		 }*/
#else
#endif
	}
#endif
	return _gtk_widget_expose_inverse(widget, e, reserved);
}
gboolean _gtk_control_focus(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	/* Stop GTK traversal for every widget */
	return TRUE;
}

gboolean _gtk_control_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
// widget could be disposed at this point
	if (_W_WIDGET(widget)->handle != 0) {
		w_control *oldControl = gtk_toolkit->imControl;
		if (oldControl != W_CONTROL(widget)) {
			if (w_widget_is_ok(W_WIDGET(oldControl))) {
				_w_control_reserved *oldreserved = _W_CONTROL_GET_RESERVED(
						oldControl);
				GtkIMContext *oldIMHandle = oldreserved->imHandle(
						W_WIDGET(oldControl), _W_WIDGET_RESERVED(oldreserved));
				if (oldIMHandle != 0)
					gtk_im_context_reset(oldIMHandle);
			}
		}
		//if (hooks(SWT.KeyDown) || hooks(SWT.KeyUp)) {
		GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
				reserved);
		if (imHandle != 0)
			gtk_im_context_focus_in(imHandle);
		//}
	}
	return 0;
}

gboolean _gtk_control_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
// widget could be disposed at this point
	if (_W_WIDGET(widget)->handle != 0) {
		//if (hooks(SWT.KeyDown) || hooks(SWT.KeyUp)) {
		GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
				reserved);
		if (imHandle != 0) {
			gtk_im_context_focus_out(imHandle);
		}
		//}
	}
	return 0;
}

gboolean _gtk_control_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GdkEventKey *gdkEvent = (GdkEventKey*) e->args[0];
	if (!_W_CONTROL_RESERVED(reserved)->has_focus(W_CONTROL(widget),
			_W_CONTROL_RESERVED(reserved))) {
		/*
		 * Feature in GTK.  On AIX, the IME window deactivates the current shell and even
		 * though the widget receiving the key event is not in focus, it should filter the input in
		 * order to get it committed.  The fix is to detect that the widget shell is not active
		 * and call filterKey() only.
		 */
		if (_w_toolkit_get_active_shell(W_TOOLKIT(gtk_toolkit)) == 0) {
			if (_w_control_filter_key(W_CONTROL(widget), gdkEvent->keyval,
					gdkEvent, _W_CONTROL_RESERVED(reserved)))
				return TRUE;
		}
		return FALSE;
	}

	if (_w_control_translate_mnemonic_0(W_CONTROL(widget), gdkEvent->keyval,
			gdkEvent, _W_CONTROL_RESERVED(reserved)))
		return TRUE;
	// widget could be disposed at this point
	if (!w_widget_is_ok(widget))
		return FALSE;

	if (_w_control_filter_key(W_CONTROL(widget), gdkEvent->keyval, gdkEvent,
			_W_CONTROL_RESERVED(reserved)))
		return TRUE;
	// widget could be disposed at this point
	if (!w_widget_is_ok(widget))
		return FALSE;

	if (_W_CONTROL_RESERVED(reserved)->translate_traversal(W_CONTROL(widget),
			gdkEvent, _W_CONTROL_RESERVED(reserved)))
		return 1;
	// widget could be disposed at this point
	if (!w_widget_is_ok(widget))
		return FALSE;
	return _w_widget_send_key_event(widget, e);
}

gboolean _gtk_control_key_release_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (!_w_control_has_focus(W_CONTROL(widget), _W_CONTROL_RESERVED(reserved)))
		return FALSE;
	GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
			reserved);
	if (imHandle != 0) {
		if (gtk_im_context_filter_keypress(imHandle, (GdkEventKey*) e->args[0]))
			return TRUE;
	}
	return _w_widget_send_key_event(widget, e);
}

gboolean _gtk_control_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (gtk_toolkit->currentControl != W_CONTROL(widget))
		return FALSE;
	GdkEventCrossing *gdkEvent = (GdkEventCrossing*) e->args[0];
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, _W_CONTROL(widget)->lastInput.x,
	_W_CONTROL(widget)->lastInput.y, reserved))
		return FALSE;
	//display.removeMouseHoverTimeout(handle);
	gboolean result = FALSE;
	if ( _W_CONTROL_RESERVED(reserved)->send_leave_notify(W_CONTROL(widget),
			_W_CONTROL_RESERVED(reserved)) > 0
			|| _w_toolkit_get_cursor_control(W_TOOLKIT(gtk_toolkit)) == 0) {
		if (gdkEvent->mode != GDK_CROSSING_NORMAL
				&& gdkEvent->mode != GDK_CROSSING_UNGRAB)
			return FALSE;
		if ((gdkEvent->state
				& (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK | GDK_BUTTON3_MASK))
				!= 0)
			return FALSE;
		/*result =
		 sendMouseEvent(SWT.MouseExit, 0, gdkEvent.time, gdkEvent.x_root,
		 gdkEvent.y_root, false, gdkEvent.state) ? 0 : 1;*/
		gtk_toolkit->currentControl = 0;
	}
	return result;
}

gboolean _gtk_control_mnemonic_activate(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = FALSE;
	GdkEventKey *keyEvent = (GdkEventKey*) gtk_get_current_event();
	if (keyEvent != 0) {
		if (keyEvent->type == GDK_KEY_PRESS) {
			w_control *focusControl = _w_toolkit_get_focus_control(
					W_TOOLKIT(gtk_toolkit));
			GtkWidget *focusHandle = 0;
			if (w_widget_is_ok(W_WIDGET(focusControl))) {
				_w_control_reserved *freserved = _W_CONTROL_GET_RESERVED(
						focusControl);
				focusHandle = freserved->focusHandle(W_WIDGET(focusControl),
						_W_WIDGET_RESERVED(freserved));
			}
			if (focusHandle != 0) {
				gtk_toolkit->mnemonicControl = W_CONTROL(widget);
				gtk_widget_event(focusHandle, (GdkEvent*) keyEvent);
				gtk_toolkit->mnemonicControl = 0;
			}
			result = 1;
		}
		gdk_event_free((GdkEvent*) keyEvent);
	}
	return result;
}

gboolean _gtk_control_motion_notify_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved) {
	w_event_mouse event;
	int result;
	GdkEventMotion *gdkEvent = (GdkEventMotion*) e->args[0];
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, _W_CONTROL(widget)->lastInput.x,
	_W_CONTROL(widget)->lastInput.y, reserved))
		return FALSE;

	/*
	 * Feature in GTK: DND detection for X.11 & Wayland support is done through motion notify event
	 * instead of mouse click event. See Bug 503431.
	 */
	if (isWayland()) { // Wayland
		gboolean dragging = FALSE;
		if ((_W_WIDGET(widget)->state & STATE_DRAG_DETECT) != 0) {
			gboolean consume = FALSE;
			if (_w_control_drag_detect_2(W_CONTROL(widget), gdkEvent->x,
					gdkEvent->y, TRUE, TRUE, &consume)) {
				dragging = TRUE;
				if (consume)
					result = TRUE;
				if (!w_widget_is_ok(widget))
					return TRUE;
			} else {
			}
		}
		if (dragging) {
			//gtk_event_controller_handle_event(dragGesture, event);
			GdkEventButton *gdkEvent1 = (GdkEventButton*) e->args[0];
			if (gdkEvent1->type == GDK_3BUTTON_PRESS)
				return FALSE;
			event.event.type = W_EVENT_DRAGDETECT;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent1->time;
			event.event.data = 0;
			event.button = gdkEvent1->button;
			event.clickcount = gtk_toolkit->clickCount;
			event.x = gdkEvent1->x;
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				event.x = _W_CONTROL_RESERVED(reserved)->get_client_width(
						W_CONTROL(widget), _W_CONTROL_RESERVED(reserved))
						- event.x;
			}
			event.y = gdkEvent1->y;
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(gdkEvent1->state);
			if (_w_widget_send_event(widget, (w_event*) &event)) {
				return TRUE;
			}
			if (!w_widget_is_ok(widget))
				return TRUE;
			if (_W_CONTROL(widget)->dragsource != 0) {
				event.event.widget = W_WIDGET(_W_CONTROL(widget)->dragsource);
				_w_widget_send_event(W_WIDGET(_W_CONTROL(widget)->dragsource),
						(w_event*) &event);
			}
		}
	}
	if (W_CONTROL(widget) == gtk_toolkit->currentControl) {
		//display.addMouseHoverTimeout(handle);
	}
	double x = gdkEvent->x_root, y = gdkEvent->y_root;
	int state = gdkEvent->state;
	if (gdkEvent->is_hint != 0) {
		int pointer_x, pointer_y;
		GdkModifierType mask;
		GdkWindow *window = _W_CONTROL_RESERVED(reserved)->eventWindow(widget,
				reserved);
		_gdk_window_get_device_position(window, &pointer_x, &pointer_y, &mask);
		x = pointer_x;
		y = pointer_y;
		state = mask;
	}
#if GTK3
	if (W_CONTROL(widget) != gtk_toolkit->currentControl) {
		if (gtk_toolkit->currentControl != 0
				&& w_widget_is_ok(W_WIDGET(gtk_toolkit->currentControl))) {
			//display.removeMouseHoverTimeout(display.currentControl.handle);
			w_point pt, tmp;
			tmp.x = x;
			tmp.y = y;
			_w_toolkit_map_0(W_TOOLKIT(gtk_toolkit), W_CONTROL(widget),
					gtk_toolkit->currentControl, &pt, &tmp);
			event.event.type = W_EVENT_MOUSEEXIT;
			event.event.widget = W_WIDGET(gtk_toolkit->currentControl);
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent->time;
			event.event.data = 0;
			event.button = 0;
			event.clickcount = gtk_toolkit->clickCount;
			_w_widget_reserved *creserved = _w_widget_get_reserved(
					W_WIDGET(gtk_toolkit->currentControl));
			if (gdkEvent->is_hint) {
				event.x = pt.x;
				event.y = pt.y;
			} else {
				GdkWindow *window = _W_CONTROL_RESERVED(creserved)->eventWindow(
						W_WIDGET(gtk_toolkit->currentControl), creserved);
				int origin_x = 0, origin_y = 0;
				gdk_window_get_origin(window, &origin_x, &origin_y);
				event.x = pt.x - origin_x;
				event.y = pt.y - origin_y;
			}
			if ((_W_WIDGET(gtk_toolkit->currentControl)->style & W_MIRRORED)
					!= 0) {
				event.x = _W_CONTROL_RESERVED(creserved)->get_client_width(
						gtk_toolkit->currentControl,
						_W_CONTROL_RESERVED(creserved)) - event.x;
			}
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(state);
			_w_widget_send_event(W_WIDGET(gtk_toolkit->currentControl),
					(w_event*) &event);
		}
		if (w_widget_is_ok(widget)) {
			gtk_toolkit->currentControl = W_CONTROL(widget);
			event.event.type = W_EVENT_MOUSEENTER;
			event.event.widget = widget;
			event.event.platform_event = (w_event_platform*) e;
			event.event.time = gdkEvent->time;
			event.event.data = 0;
			event.button = 0;
			event.clickcount = gtk_toolkit->clickCount;
			if (gdkEvent->is_hint) {
				event.x = x;
				event.y = y;
			} else {
				GdkWindow *window = _W_CONTROL_RESERVED(reserved)->eventWindow(
						widget, reserved);
				int origin_x = 0, origin_y = 0;
				gdk_window_get_origin(window, &origin_x, &origin_y);
				event.x = x - origin_x;
				event.y = y - origin_y;
			}
			if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
				event.x = _W_CONTROL_RESERVED(reserved)->get_client_width(
						W_CONTROL(widget), _W_CONTROL_RESERVED(reserved))
						- event.x;
			}
			event.detail = 0;
			event.statemask = _w_widget_set_input_state(state);
			_w_widget_send_event(widget, (w_event*) &event);
		}
	}
#endif
	event.event.type = W_EVENT_MOUSEMOVE;
	event.event.widget = widget;
	event.event.platform_event = (w_event_platform*) e;
	event.event.time = gdkEvent->time;
	event.event.data = 0;
	event.button = 0;
	event.clickcount = gtk_toolkit->clickCount;
	if (gdkEvent->is_hint) {
		event.x = x;
		event.y = y;
	} else {
		GdkWindow *window = _W_CONTROL_RESERVED(reserved)->eventWindow(widget,
				reserved);
		int origin_x = 0, origin_y = 0;
		gdk_window_get_origin(window, &origin_x, &origin_y);
		event.x = x - origin_x;
		event.y = y - origin_y;
	}
	if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
		event.x = _W_CONTROL_RESERVED(reserved)->get_client_width(
				W_CONTROL(widget), _W_CONTROL_RESERVED(reserved)) - event.x;
	}
	event.detail = 0;
	event.statemask = _w_widget_set_input_state(state);
	result = _w_widget_send_event(widget, (w_event*) &event);
	return result;
}

gboolean _gtk_control_popup_menu(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (!_w_control_has_focus(W_CONTROL(widget), _W_CONTROL_RESERVED(reserved)))
		return FALSE;
	int x, y;
	GdkModifierType mask;
	_gdk_window_get_device_position(0, &x, &y, &mask);
	return _w_control_show_menu(W_CONTROL(widget), x, y, W_MENU_KEYBOARD);
}

gboolean _gtk_control_preedit_changed(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
//display.showIMWindow(this);
	return FALSE;
}

gboolean _gtk_control_realize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
			reserved);
	if (imHandle != 0) {
		GtkWidget *paintHandle = _W_CONTROL_RESERVED(reserved)->paintHandle(
				widget, reserved);
		GdkWindow *window = gtk_widget_get_window(paintHandle);
		gtk_im_context_set_client_window(GTK_IM_CONTEXT(imHandle), window);
	}
	if (_W_CONTROL(widget)->backgroundImage != 0) {
		_w_control_set_background_pixmap(W_CONTROL(widget),
		_W_CONTROL(widget)->backgroundImage, _W_CONTROL_RESERVED(reserved));
	}
	return FALSE;
}

gboolean _gtk_control_scroll_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GdkEventScroll *gdkEvent = (GdkEventScroll*) e->args[0];
	_W_CONTROL(widget)->lastInput.x = (int) gdkEvent->x;
	_W_CONTROL(widget)->lastInput.y = (int) gdkEvent->y;
	if (_w_control_contained_in_region(widget, _W_CONTROL(widget)->lastInput.x,
	_W_CONTROL(widget)->lastInput.y, reserved))
		return FALSE;
	w_event_mouse event;
	event.event.type = 0;
	switch (gdkEvent->direction) {
	case GDK_SCROLL_UP:
		event.event.type = W_EVENT_MOUSEVERTICALWHEEL;
		event.clickcount = 3;
		event.detail = W_SCROLL_LINE;
		break;
	case GDK_SCROLL_DOWN:
		event.event.type = W_EVENT_MOUSEVERTICALWHEEL;
		event.clickcount = -3;
		event.detail = W_SCROLL_LINE;
		break;
	case GDK_SCROLL_LEFT:
		event.event.type = W_EVENT_MOUSEHORIZONTALWHEEL;
		event.clickcount = 3;
		event.detail = 0;
		break;
	case GDK_SCROLL_RIGHT:
		event.event.type = W_EVENT_MOUSEHORIZONTALWHEEL;
		event.clickcount = -3;
		event.detail = 0;
		break;
	case GDK_SCROLL_SMOOTH: {
		double delta_x = 0, delta_y = 0;
		if (gdk_event_get_scroll_deltas((GdkEvent*) gdkEvent, &delta_x,
				&delta_y)) {
			if (delta_x != 0) {
				event.event.type = W_EVENT_MOUSEHORIZONTALWHEEL;
				event.clickcount = -3 * delta_x;
				event.detail = 0;
			}
			if (delta_y != 0) {
				event.event.type = W_EVENT_MOUSEVERTICALWHEEL;
				event.clickcount = -3 * delta_y;
				event.detail = W_SCROLL_LINE;
			}
		}
	}
		break;
	}
	if (event.event.type != 0) {
		event.event.widget = widget;
		event.event.platform_event = (w_event_platform*) e;
		event.event.time = gdkEvent->time;
		event.event.data = 0;
		event.button = 0;
		event.x = gdkEvent->x_root;
		event.y = gdkEvent->y_root;
		GdkWindow *window = _W_CONTROL_RESERVED(reserved)->eventWindow(widget,
				reserved);
		int origin_x = 0, origin_y = 0;
		gdk_window_get_origin(window, &origin_x, &origin_y);
		event.x = event.x - origin_x;
		event.y = event.y - origin_y;
		if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
			event.x = _W_CONTROL_RESERVED(reserved)->get_client_width(
					W_CONTROL(widget), _W_CONTROL_RESERVED(reserved)) - event.x;
		}
		event.statemask = _w_widget_set_input_state(gdkEvent->state);
		return _w_widget_send_event(widget, (w_event*) &event);
	}
	return FALSE;
}

gboolean _gtk_control_show_help(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (!_w_control_has_focus(W_CONTROL(widget), _W_CONTROL_RESERVED(reserved)))
		return FALSE;
	w_event event;
	w_control *control = W_CONTROL(widget);
	while (control != 0) {
		event.type = W_EVENT_HELP;
		event.platform_event = (w_event_platform*) e;
		event.time = 0;
		event.widget = W_WIDGET(control);
		event.data = 0;
		if (_w_widget_send_event(W_WIDGET(control), &event)) {
			return TRUE;
		}
		control = W_CONTROL(_W_CONTROL(control)->parent);
	}
	return FALSE;
}

gboolean _gtk_control_style_set(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (_W_CONTROL(widget)->backgroundImage != 0) {
		_w_control_set_background_pixmap(W_CONTROL(widget),
		_W_CONTROL(widget)->backgroundImage, _W_CONTROL_RESERVED(reserved));
	}
	return FALSE;
}

gboolean _gtk_control_unrealize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkIMContext *imHandle = _W_CONTROL_RESERVED(reserved)->imHandle(widget,
			reserved);
	if (imHandle != 0)
		gtk_im_context_set_client_window(GTK_IM_CONTEXT(imHandle), 0);
	return FALSE;
}
#endif
