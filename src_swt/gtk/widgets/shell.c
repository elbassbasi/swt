/*
 *  shell.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "shell.h"
#include "toolkit.h"
#include "tooltip.h"
/**
 *
 * Disable warning deprecated
 *
 */
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
wresult _w_shell_create_embedded(w_widget *widget, w_widget *parent,
		wuint64 style, w_widget_post_event_proc post_event, void *handle,
		int embedded) {
	if (parent != 0) {
		if (!w_widget_is_ok(parent))
			return W_ERROR_INVALID_ARGUMENT;
		if (w_widget_class_id(parent) != _W_CLASS_SHELL)
			return W_ERROR_INVALID_ARGUMENT;
	}
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	_W_SHELL(widget)->center = parent != 0 && (style & W_SHEET) != 0;
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = _w_shell_check_style(widget, style);
	if (handle != 0) {
		if (embedded) {
			_W_WIDGET(widget)->handle = handle;
		} else {
			_W_WIDGET(widget)->handle = handle;
			_W_WIDGET(widget)->state |= STATE_FOREIGN_HANDLE;
		}
	}
	_w_widget_reserved *reserved = _w_widget_get_reserved(widget);
	//reskinWidget();
	wresult result = reserved->create_widget(widget, 0, reserved);
	if (result > 0) {
		_w_toolkit_add_shell(_W_SHELL(widget));
	}
	return result;
}
wresult _w_shell_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_shell_create_embedded(widget, parent, style, post_event, 0,
			W_FALSE);
}
wuint64 _w_shell_check_style(w_widget *control, wuint64 style) {
	if ((style & W_NO_TRIM) != 0) {
		style &= ~(W_CLOSE | W_MIN | W_MAX | W_RESIZE | W_BORDER);
	} else if ((style & W_NO_MOVE) != 0) {
		style |= W_TITLE;
	}
	if ((style & (W_MENU | W_MIN | W_MAX | W_CLOSE)) != 0) {
		style |= W_TITLE;
	}
	style &= ~W_TRANSPARENT;
	if ((style & W_ON_TOP) != 0)
		style &= ~(W_CLOSE | W_MIN | W_MAX);
	int mask = W_SYSTEM_MODAL | W_APPLICATION_MODAL | W_PRIMARY_MODAL;
	if ((style & W_SHEET) != 0) {
		style &= ~W_SHEET;
		w_composite *parent = _W_CONTROL(control)->parent;
		style |= parent == 0 ? W_SHELL_TRIM : W_DIALOG_TRIM;
		if ((style & mask) == 0) {
			style |= parent == 0 ? W_APPLICATION_MODAL : W_PRIMARY_MODAL;
		}
	}
	int bits = style & ~mask;
	if ((style & W_SYSTEM_MODAL) != 0)
		return bits | W_SYSTEM_MODAL;
	if ((style & W_APPLICATION_MODAL) != 0)
		return bits | W_APPLICATION_MODAL;
	if ((style & W_PRIMARY_MODAL) != 0)
		return bits | W_PRIMARY_MODAL;
	return bits;
}
void _w_shell_add_tooltip(w_shell *shell, _w_tooltip *toolTip) {
	toolTip->next = 0;
	if (_W_SHELL(shell)->tooltips == 0) {
		_W_SHELL(shell)->tooltips = toolTip;
		toolTip->prev = toolTip;
	} else {
		_w_tooltip *last = _W_SHELL(shell)->tooltips->prev;
		last->next = toolTip;
		toolTip->prev = last;
		_W_SHELL(shell)->tooltips->prev = toolTip;
	}
}
void _w_shell_adjust_trim(w_shell *shell) {
	if (gtk_toolkit->ignoreTrim)
		return;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	GtkAllocation allocation;
	gtk_widget_get_allocation(shellHandle, &allocation);
	int width = allocation.width;
	int height = allocation.height;
	GdkWindow *window = gtk_widget_get_window(shellHandle);
	GdkRectangle rect;
	gdk_window_get_frame_extents(window, &rect);
	int trimWidth = w_max(0, rect.width - width);
	int trimHeight = w_max(0, rect.height - height);
	/*
	 * Bug in GTK.  gdk_window_get_frame_extents() fails for various window
	 * managers, causing a large incorrect value to be returned as the trim.
	 * The fix is to ignore the returned trim values if they are too large.
	 */
	if (trimWidth > _W_SHELL_MAXIMUM_TRIM || trimHeight > _W_SHELL_MAXIMUM_TRIM) {
		gtk_toolkit->ignoreTrim = W_TRUE;
		return;
	}
	wbool hasTitle = W_FALSE, hasResize = W_FALSE, hasBorder = W_FALSE;
	wuint64 style = _W_WIDGET(shell)->style;
	if ((style & W_NO_TRIM) == 0) {
		hasTitle = (style & (W_MIN | W_MAX | /*W_TITLE |*/W_MENU)) != 0;
		hasResize = (style & W_RESIZE) != 0;
		hasBorder = (style & W_BORDER) != 0;
	}
	int trimStyle;
	if (hasTitle) {
		if (hasResize) {
			trimStyle = TRIM_TITLE_RESIZE;
		} else if (hasBorder) {
			trimStyle = TRIM_TITLE_BORDER;
		} else {
			trimStyle = TRIM_TITLE;
		}
	} else if (hasResize) {
		trimStyle = TRIM_RESIZE;
	} else if (hasBorder) {
		trimStyle = TRIM_BORDER;
	} else {
		trimStyle = TRIM_NONE;
	}
#if GTK3
	/*
	 * The workaround for bug 445900 seems to cause problems for some
	 * users on GTK2, see bug 492695. The fix is to only adjust the
	 * shell size on GTK3.
	 */
	w_rect bounds;
	w_control_get_bounds(W_CONTROL(shell), &bounds.pt, &bounds.sz);
	int widthAdjustment = gtk_toolkit->trimWidths[trimStyle] - trimWidth;
	int heightAdjustment = gtk_toolkit->trimHeights[trimStyle] - trimHeight;
	if (widthAdjustment == 0 && heightAdjustment == 0)
		return;

	bounds.width += widthAdjustment;
	bounds.height += heightAdjustment;
	_W_SHELL(shell)->oldb.width += widthAdjustment;
	_W_SHELL(shell)->oldb.height += heightAdjustment;
	if (!_w_shell_get_maximized(shell)) {
		_w_control_reserved *reserved = _W_CONTROL_RESERVED(
				_w_widget_get_reserved(W_WIDGET(shell)));
		_w_shell_resize_bounds(W_CONTROL(shell), width + widthAdjustment,
				height + heightAdjustment, W_FALSE, reserved);
	}
#endif
	gtk_toolkit->trimWidths[trimStyle] = trimWidth;
	gtk_toolkit->trimHeights[trimStyle] = trimHeight;
}
void _w_shell_bring_totop(w_shell *shell, int force) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	if (!gtk_widget_get_visible(shellHandle))
		return;
	w_shell *activeShell = gtk_toolkit->activeShell;
	if (activeShell == shell)
		return;
	if (!force) {
		if (activeShell == 0)
			return;
		if (!gtk_toolkit->activePending) {
			GtkWidget *focusHandle = gtk_window_get_focus(
					GTK_WINDOW(_W_SHELL_HANDLE(activeShell)));
			if (focusHandle != 0 && !gtk_widget_has_focus(focusHandle))
				return;
		}
	}
	/*
	 * Bug in GTK.  When a shell that is not managed by the window
	 * manage is given focus, GTK gets stuck in "focus follows pointer"
	 * mode when the pointer is within the shell and its parent when
	 * the shell is hidden or disposed. The fix is to use XSetInputFocus()
	 * to assign focus when ever the active shell has not managed by
	 * the window manager.
	 *
	 * NOTE: This bug is fixed in GTK+ 2.6.8 and above.
	 */
	wbool xFocus = W_FALSE;
	if (activeShell != 0) {
		gtk_toolkit->activeShell = shell;
		gtk_toolkit->activePending = W_TRUE;
	}
	/*
	 * Feature in GTK.  When the shell is an override redirect
	 * window, gdk_window_focus() does not give focus to the
	 * window.  The fix is to use XSetInputFocus() to force
	 * the focus.
	 */
	GdkWindow *window = gtk_widget_get_window(shellHandle);
	if ((xFocus || (_W_WIDGET(shell)->style & W_ON_TOP) != 0)) {
		if (isX11()) {
			/*Display *xDisplay;
			 if (GTK_VERSION >= VERSION(2, 24, 0)) {
			 xDisplay = gdk_x11_display_get_xdisplay(
			 gdk_window_get_display(window));
			 } else {
			 xDisplay = gdk_x11_drawable_get_xdisplay(window);
			 }
			 Window xWindow;
			 #if GTK3
			 xWindow = gdk_x11_window_get_xid(window);
			 #else
			 #endif
			 gdk_error_trap_push();*/
			/* Use CurrentTime instead of the last event time to ensure that the shell becomes active */
			/*XSetInputFocus(xDisplay, xWindow, 2, 0);
			 gdk_error_trap_pop();*/
		} else {
			//	find the proper fix as this doesn't seem to have effect
			//  gtk_window_present(window);
		}
	} else {
		/*
		 * Bug in metacity.  Calling gdk_window_focus() with a timestamp more
		 * recent than the last user interaction time can cause windows not
		 * to come forward in versions > 2.10.0.  The fix is to use the last
		 * user event time.
		 */
		guint32 timestamp = GDK_CURRENT_TIME;
		/*if (isX11()) {
		 GdkScreen *_screen = gdk_screen_get_default();
		 const char *windowManager = gdk_x11_screen_get_window_manager_name(
		 _screen);
		 if (!strcasecmp(windowManager, "metacity")) {
		 timestamp = gtk_toolkit->lastUserEventTime;
		 }
		 }*/
		gdk_window_focus(window, timestamp);
	}
	gtk_toolkit->activeShell = shell;
	gtk_toolkit->activePending = W_TRUE;
}
void _w_shell_center(w_control *control) {
	if (_W_CONTROL(control)->parent == 0)
		return;
	w_rect rect, parentRect, parentClientArea, monitorRect;
	w_toolkit *toolkit = w_widget_get_toolkit(W_WIDGET(control));
	w_control_get_bounds(control, &rect.pt, &rect.sz);
	w_scrollable_get_client_area(W_SCROLLABLE(control), &parentClientArea);
	w_toolkit_map_1(toolkit, W_CONTROL(_W_CONTROL(control)->parent), 0,
			&parentRect, &parentClientArea);
	w_rect pt;
	pt.x = w_max(parentRect.x,
			parentRect.x + (parentRect.width - rect.width) / 2);
	pt.y = w_max(parentRect.y,
			parentRect.y + (parentRect.height - rect.height) / 2);
	w_toolkit_get_client_area(toolkit, &monitorRect);
	if (pt.x + rect.width > monitorRect.x + monitorRect.width) {
		pt.x = w_max(monitorRect.x,
				monitorRect.x + monitorRect.width - rect.width);
	} else {
		pt.x = w_max(pt.x, monitorRect.x);
	}
	if (pt.y + rect.height > monitorRect.y + monitorRect.height) {
		pt.y = w_max(monitorRect.y,
				monitorRect.y + monitorRect.height - rect.height);
	} else {
		pt.y = w_max(pt.y, monitorRect.y);
	}
	w_control_set_bounds(control, &pt.pt, 0);
}
void _w_shell_check_border(w_control *control, _w_control_reserved *reserved) {
	/* Do nothing */
}
void _w_shell_check_open(w_widget *widget, _w_widget_reserved *reserved) {
	if (!_W_SHELL(widget)->opened)
		_W_SHELL(widget)->resized = W_FALSE;
}
GtkStyle* _w_shell_child_style(w_control *control,
		_w_control_reserved *reserved) {
	return 0;
}
wresult _w_shell_close(w_shell *shell) {
	_w_shell_close_widget(shell, 0);
	return W_TRUE;
}
void _w_shell_close_widget(w_shell *shell, _w_event_platform *e) {
	wuint64 style = w_widget_get_style(W_WIDGET(shell));
	w_event event;
	event.type = W_EVENT_CLOSE;
	event.platform_event = (w_event_platform*) e;
	event.widget = W_WIDGET(shell);
	event.data = 0;
	_w_widget_send_event(W_WIDGET(shell), &event);
	if (style & W_DISPOSE_ON_CLOSE) {
		w_widget_dispose(W_WIDGET(shell));
	}
}
wresult _w_shell_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved) {
	_w_scrollable_compute_trim(widget, e, reserved);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	int border = 0;
	int style = _W_WIDGET(widget)->style;
	if ((style & (W_NO_TRIM | W_BORDER | W_SHELL_TRIM)) == 0) {
		border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
	}
	if (ISCUSTOMRESIZE(style)) {
		border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
	}
	int trimWidth = _w_shell_trim_width(W_SHELL(widget)), trimHeight =
			_w_shell_trim_height(W_SHELL(widget));
	e->result->x -= (trimWidth / 2) + border;
	e->result->y -= trimHeight - (trimWidth / 2) + border;
	e->result->width += trimWidth + border * 2;
	e->result->height += trimHeight + border * 2;
	if (_W_SHELL(widget)->menubar != 0) {
		_w_shell_force_resize(W_CONTROL(widget), 0);
		GtkAllocation allocation;
		gtk_widget_get_allocation(
		_W_WIDGET(_W_SHELL(widget)->menubar)->handle, &allocation);
		int menuBarHeight = allocation.height;
		e->result->y -= menuBarHeight;
		e->result->height += menuBarHeight;
	}
	return W_TRUE;
}
wresult _w_shell_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	GtkWindowType type;
	GtkWidget *vboxHandle = 0, *scrolled = 0, *handle = 0;
	GtkWindow *shellHandle = 0;
	GtkWindowGroup *group = 0;
	if (_W_WIDGET(widget)->state & STATE_FOREIGN_HANDLE) {
		shellHandle = (GtkWindow*) _W_WIDGET(widget)->handle;
	}
	_w_control_reserved *parent_reserved;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_CANVAS;
	if (shellHandle == 0) {
		type = GTK_WINDOW_TOPLEVEL;
		if ((_W_WIDGET(widget)->style & W_ON_TOP) != 0)
			type = GTK_WINDOW_POPUP;
		shellHandle = GTK_WINDOW(gtk_window_new(type));
		if (shellHandle == 0)
			goto _err;
		if (_W_CONTROL(widget)->parent != 0) {
			parent_reserved =
					_W_CONTROL_RESERVED(
							_w_widget_get_reserved(W_WIDGET(_W_CONTROL(widget)->parent)));
			gtk_window_set_transient_for(shellHandle,
					GTK_WINDOW(
							parent_reserved->widget.topHandle(W_WIDGET(_W_CONTROL(widget)->parent),_W_WIDGET_RESERVED(parent_reserved))));
			gtk_window_set_destroy_with_parent(shellHandle, TRUE);
			// if child shells are minimizable, we want them to have a
			// taskbar icon, so they can be unminimized
			if ((_W_WIDGET(widget)->style & W_MIN) == 0) {
				gtk_window_set_skip_taskbar_hint(shellHandle, TRUE);
			}

			/*
			 * For systems running Metacity, by applying the dialog type hint
			 * to a window only the close button can be placed on the title bar.
			 * The style hints for the minimize and maximize buttons are ignored.
			 * See bug 445456.
			 *
			 */
//			if (!isUndecorated ()) {
//				OS.gtk_window_set_type_hint (shellHandle, OS.GDK_WINDOW_TYPE_HINT_DIALOG);
//			}
		} else {
			if ((_W_WIDGET(widget)->style & W_ON_TOP) != 0)
				gtk_window_set_keep_above(shellHandle, TRUE);
		}
		/*
		 * Feature in GTK.  The window size must be set when the window
		 * is created or it will not be allowed to be resized smaller that the
		 * initial size by the user.  The fix is to set the size to zero.
		 */
		if ((_W_WIDGET(widget)->style & W_RESIZE) != 0) {
			//gtk_widget_set_size_request(GTK_WIDGET(shellHandle), 0, 0);
			gtk_window_set_resizable(shellHandle, TRUE);
		} else {
			gtk_window_set_resizable(shellHandle, FALSE);
		}
//gtk_window_set_title(shellHandle, "");
		if ((_W_WIDGET(widget)->style & (W_NO_TRIM | W_BORDER | W_SHELL_TRIM))
				== 0) {
			gtk_container_set_border_width(GTK_CONTAINER(shellHandle), 1);
		}
		if ((_W_WIDGET(widget)->style & W_TOOL) != 0) {
			gtk_window_set_type_hint(shellHandle, GDK_WINDOW_TYPE_HINT_UTILITY);
		}
		if ((_W_WIDGET(widget)->style & W_NO_TRIM) != 0) {
			gtk_window_set_decorated(shellHandle, FALSE);
		}

		if (ISCUSTOMRESIZE(_W_WIDGET(widget)->style)) {
			gtk_container_set_border_width(GTK_CONTAINER(shellHandle),
			_W_SHELL_BORDER);
		}
	}
#if GTK3
	vboxHandle = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	if (vboxHandle == 0)
		goto _err;
	gtk_box_set_homogeneous(GTK_BOX(vboxHandle), FALSE);
#else
#endif
	if (_w_composite_create_handle_0(widget, index, 0, &scrolled, &handle) < 0)
		goto _err;
	group = gtk_window_group_new();
	if (group == 0)
		goto _err;
	_W_SHELL(widget)->group = group;
	_W_WIDGET(widget)->handle = handle;
	gtk_container_add(GTK_CONTAINER(vboxHandle), scrolled);
	gtk_box_set_child_packing(GTK_BOX(vboxHandle), scrolled, TRUE, TRUE, 0,
			GTK_PACK_END);
	gtk_container_add(GTK_CONTAINER(shellHandle), vboxHandle);
	g_object_set_qdata(G_OBJECT(shellHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(vboxHandle), gtk_toolkit->quark[0], widget);
	gtk_widget_show_all(vboxHandle);
	/*
	 * Feature in GTK.  Realizing the shell triggers a size allocate event,
	 * which may be confused for a resize event from the window manager if
	 * received too late.  The fix is to realize the window during creation
	 * to avoid confusion.
	 */
	gtk_widget_realize(GTK_WIDGET(shellHandle));
	return W_TRUE;
	_err: if (vboxHandle != 0)
		gtk_widget_destroy(vboxHandle);
	if (shellHandle != 0
			&& !(_W_WIDGET(widget)->state & STATE_FOREIGN_HANDLE)) {
		gtk_widget_destroy(GTK_WIDGET(shellHandle));
	}
	if (scrolled != 0)
		gtk_widget_destroy(scrolled);
	if (handle != 0)
		gtk_widget_destroy(handle);
	if (group != 0)
		g_object_unref(G_OBJECT(group));
	return W_ERROR_NO_HANDLES;
}
w_control* _w_shell_find_background_control(w_control *control,
		_w_control_reserved *reserved) {
	return (_W_WIDGET(control)->state & STATE_BACKGROUND) != 0
			|| _W_CONTROL(control)->backgroundImage != 0 ? control : 0;
}
w_composite* _w_shell_find_deferred_control(w_composite *composite,
		_w_composite_reserved *reserved) {
	return _W_COMPOSITE(composite)->layoutCount > 0 ? composite : 0;
}
wresult _w_shell_get_toolbar(w_shell *shell, w_toolbar **toolbar) {
	*toolbar = 0;
	return W_TRUE;
}
wresult _w_shell_has_border(w_composite *composite,
		_w_composite_reserved *reserved) {
	return W_FALSE;
}
void _w_shell_hook_events(w_widget *shell, _w_widget_reserved *reserved) {
	_w_composite_hook_events(shell, reserved);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	if (_W_SHELL_RESERVED(reserved)->move_focus_id == 0) {
		_W_SHELL_RESERVED(reserved)->move_focus_id = g_signal_lookup(
				"move-focus", gtk_window_get_type());
	}
	_w_widget_connect(shellHandle, SIGNAL_KEY_PRESS_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_WINDOW_STATE_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_SIZE_ALLOCATE, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_CONFIGURE_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_DELETE_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_MAP_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_ENTER_NOTIFY_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_MOVE_FOCUS,
	_W_SHELL_RESERVED(reserved)->move_focus_id,
	FALSE);
#if GTK3
	_w_widget_connect(shellHandle, SIGNAL_FOCUS_IN_EVENT, 0, FALSE);
	_w_widget_connect(shellHandle, SIGNAL_FOCUS_OUT_EVENT, 0, FALSE);
#else
#endif
	if (ISCUSTOMRESIZE(_W_WIDGET(shell)->style)) {
		int mask = GDK_POINTER_MOTION_MASK | GDK_BUTTON_RELEASE_MASK
				| GDK_BUTTON_PRESS_MASK | GDK_ENTER_NOTIFY_MASK
				| GDK_LEAVE_NOTIFY_MASK;
		gtk_widget_add_events(shellHandle, mask);
		_w_widget_connect(shellHandle, SIGNAL_EXPOSE_EVENT, 0, FALSE);
		_w_widget_connect(shellHandle, SIGNAL_LEAVE_NOTIFY_EVENT, 0, FALSE);
		_w_widget_connect(shellHandle, SIGNAL_MOTION_NOTIFY_EVENT, 0, FALSE);
		_w_widget_connect(shellHandle, SIGNAL_BUTTON_PRESS_EVENT, 0, FALSE);
	}
}
wresult _w_shell_is_enabled(w_control *shell) {
	return W_CONTROL_GET_CLASS(shell)->get_enabled(shell);
}
wresult _w_shell_is_visible(w_control *shell) {
	return W_CONTROL_GET_CLASS(shell)->get_visible(shell);
}
wresult _w_shell_request_layout(w_control *shell) {
	_w_composite_layout(W_COMPOSITE(shell), W_DEFER);
	return W_TRUE;
}
void _w_shell_fix_active_shell(w_shell *_shell) {
	if (gtk_toolkit->activeShell == _shell) {
		_w_shell *shell = 0;
		if (_W_CONTROL(_shell)->parent != 0
				&& w_control_is_visible(W_CONTROL(_W_CONTROL(_shell)->parent)))
			w_control_get_shell(W_CONTROL(_W_CONTROL(_shell)->parent),
					(w_shell**) &_shell);
		wuint64 style = _W_WIDGET(_shell)->style;
		if (shell == 0 && isUndecorated(style)) {
			_w_shell *shells = gtk_toolkit->shells;
			while (shells != 0) {
				if (shells != 0 && w_control_is_visible(W_CONTROL(shells))) {
					shell = shells;
					break;
				}
				shells = shells->next;
			}
		}
		if (shell != 0)
			_w_shell_bring_totop(W_SHELL(shell), W_FALSE);
	}
}
void _w_shell_fix_shell(w_shell *shell, w_shell *newShell, w_control *control) {
	/*if (this == newShell) return;
	 if (control == lastActive) setActiveControl (null);
	 String toolTipText = control.toolTipText;
	 if (toolTipText != null) {
	 control.setToolTipText (this, null);
	 control.setToolTipText (newShell, toolTipText);
	 }*/
}
void _w_shell_fix_style_0(w_control *control, GtkWidget *handle,
		_w_control_reserved *reserved) {

}
void _w_shell_force_resize(w_control *control, _w_control_reserved *reserved) {
	GtkAllocation allocation;
	gtk_widget_get_allocation(_W_SHELL_VBOX(control), &allocation);
	if (isWayland()) {
		/*
		 * Bug 540163: We sometimes are getting the container's allocation
		 * before Shell is fully opened, which gets an incorrect allocation.
		 * Fix is to use the calculated box width/height if bounds have been set.
		 */
		GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
		int border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
		int boxWidth = _W_SHELL(control)->oldb.width - 2 * border;
		int boxHeight = _W_SHELL(control)->oldb.height - 2 * border;
		if (boxWidth != allocation.width || boxHeight != allocation.height) {
			allocation.width = w_max(boxWidth, allocation.width);
			allocation.height = w_max(boxHeight, allocation.height);
		}
	}
	_w_shell_force_resize_0(control, allocation.width, allocation.height,
			reserved);
}
void _w_shell_force_resize_0(w_control *control, int width, int height,
		_w_control_reserved *reserved) {
	int clientWidth = 0;
	int style = _W_WIDGET(control)->style;
#if GTK3
	if ((style & W_MIRRORED) != 0)
		clientWidth = _w_control_get_client_width(control, 0);
#endif
	GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	GtkRequisition minimumSize, naturalSize;
	GtkAllocation allocation;
	int border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
	allocation.x = border;
	allocation.y = border;
	allocation.width = width;
	allocation.height = height;
	// Call gtk_widget_get_preferred_size() on GTK 3.20+ to prevent warnings.
	// See bug 486068.
	gtk_widget_get_preferred_size(vboxHandle, &minimumSize, &naturalSize);
	/*
	 * Bug 535075, 536153: On Wayland, we need to set the position of the GtkBox container
	 * relative to the shellHandle to prevent window contents rendered with offset.
	 */
	if (isWayland()) {
		int dest_x;
		int dest_y;
		gtk_widget_translate_coordinates(vboxHandle, shellHandle, 0, 0, &dest_x,
				&dest_y);
		if (dest_x != -1 && dest_y != -1) {
			w_composite *parent = _W_CONTROL(control)->parent;
			wuint64 style = w_widget_get_style(W_WIDGET(parent));
			if (!(parent != 0 && (style & W_ON_TOP) != 0)) {
				allocation.x += dest_x;
				allocation.y += dest_y;
			}
		}
	}
#if GTK3
	gtk_widget_size_allocate(vboxHandle, &allocation);
#else
#endif

#if GTK3
	if ((style & W_MIRRORED) != 0)
		_w_composite_move_children(W_COMPOSITE(control), clientWidth);
#endif
}
wresult _w_shell_get_alpha(w_shell *shell) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
#if H_GTK_VERSION > VERSION(3,22,0)
	if (gdk_screen_is_composited(
			gtk_window_get_screen(GTK_WINDOW(shellHandle)))) {
#else
		if (gtk_widget_is_composited(shellHandle)) {
#endif
		/*
		 * Feature in GTK: gtk_window_get_opacity() is deprecated on GTK3.8
		 * onward. Use gtk_widget_get_opacity() instead.
		 */
#if H_GTK_VERSION > VERSION(3,22,0)
		return gtk_widget_get_opacity(shellHandle) * 255;
#else
	return gtk_window_get_opacity(GTK_WINDOW(shellHandle)) * 255);
#endif
	}
	return 0xFF;
}
int _w_shell_get_resize_mode(w_shell *shell, double x, double y) {
	GtkAllocation allocation;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	gtk_widget_get_allocation(shellHandle, &allocation);
	int width = allocation.width;
	int height = allocation.height;
	int border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
	int mode = 0;
	if (y >= height - border) {
		mode = GDK_BOTTOM_SIDE;
		if (x >= width - border - 16)
			mode = GDK_BOTTOM_RIGHT_CORNER;
		else if (x <= border + 16)
			mode = GDK_BOTTOM_LEFT_CORNER;
	} else if (x >= width - border) {
		mode = GDK_RIGHT_SIDE;
		if (y >= height - border - 16)
			mode = GDK_BOTTOM_RIGHT_CORNER;
		else if (y <= border + 16)
			mode = GDK_TOP_RIGHT_CORNER;
	} else if (y <= border) {
		mode = GDK_TOP_SIDE;
		if (x <= border + 16)
			mode = GDK_TOP_LEFT_CORNER;
		else if (x >= width - border - 16)
			mode = GDK_TOP_RIGHT_CORNER;
	} else if (x <= border) {
		mode = GDK_LEFT_SIDE;
		if (y <= border + 16)
			mode = GDK_TOP_LEFT_CORNER;
		else if (y >= height - border - 16)
			mode = GDK_BOTTOM_LEFT_CORNER;
	}
	return mode;
}
wresult _w_shell_get_full_screen(w_shell *shell) {
	return _W_SHELL(shell)->fullScreen;
}
wresult _w_shell_get_maximized(w_shell *shell) {
	return !(_W_SHELL(shell)->fullScreen) && (_W_SHELL(shell)->maximized);
}
wresult _w_shell_get_minimum_size(w_shell *shell, w_size *size) {
	size->width = w_max(1,
	_W_SHELL(shell)->minWidth + _w_shell_trim_width(shell));
	size->height = w_max(1,
	_W_SHELL(shell)->minHeight + _w_shell_trim_height(shell));
	return W_TRUE;
}
w_shell* _w_shell_get_modal_shell(w_shell *_this) {
	w_shell *shell = 0;
	_w_shell *modal = gtk_toolkit->modalShells;
	int bits = W_APPLICATION_MODAL | W_SYSTEM_MODAL;
	while (modal != 0) {
		if ((_W_WIDGET(modal)->style & bits) != 0) {
			_w_shell *control = _W_SHELL(_this);
			while (control != 0) {
				if (control == modal)
					break;
				control = _W_SHELL(_W_CONTROL(control)->parent);
			}
			if (control != modal)
				return W_SHELL(modal);
			break;
		}
		if ((_W_WIDGET(modal)->style & W_PRIMARY_MODAL) != 0) {
			if (shell == 0)
				W_CONTROL_GET_CLASS(_this)->get_shell(W_CONTROL(_this), &shell);
			if (_W_CONTROL(modal)->parent == (w_composite*) shell)
				return W_SHELL(modal);
		}
		modal = modal->next_modal;
	}
	return 0;
}
wresult _w_shell_get_modified(w_shell *shell) {
	return _W_SHELL(shell)->modified;
}
wresult _w_shell_get_visible(w_control *shell) {
	return gtk_widget_get_visible(_W_SHELL_HANDLE(shell));
}
wresult _w_shell_get_region(w_control *shell, w_region *region) {
	if (_W_SHELL(shell)->originalRegion != 0) {
		w_region_dispose(region);
		_W_REGION(region)->handle = cairo_region_copy(
		_W_SHELL(shell)->originalRegion);
		if (_W_REGION(region)->handle != 0) {
			return W_TRUE;
		} else
			return W_ERROR_NO_HANDLES;
	}
	return _w_control_get_region(shell, region);
}
wresult _w_shell_get_ime_input_mode(w_shell *shell) {
	return W_NONE;
}
wresult _w_shell_get_shell(w_control *control, w_shell **shell) {
	*shell = W_SHELL(control);
	return W_TRUE;
}
wresult _w_shell_get_shells(w_shell *shell, w_iterator *iterator) {
	_w_toolkit_get_shells_from_parent(shell, iterator);
	return W_TRUE;
}
wresult _w_shell_open(w_shell *shell) {
	_w_shell_bring_totop(shell, W_FALSE);
	w_composite *parent = _W_CONTROL(shell)->parent;
	if (w_widget_class_id(W_WIDGET(parent)) == _W_CLASS_SHELL
			&& !w_control_is_visible(W_CONTROL(parent))) {
		_w_shell_open(W_SHELL(parent));
	}
	_w_shell_set_visible(W_CONTROL(shell), TRUE);
	/*
	 * When no widget has been given focus, or another push button has focus,
	 * give focus to the default button. This avoids overriding the default
	 * button.
	 */
	/*boolean restored = restoreFocus();
	 if (!restored) {
	 restored = traverseGroup(true);
	 }
	 if (restored) {
	 Control focusControl = display.getFocusControl();
	 if (focusControl instanceof Button && (focusControl.style & W_PUSH) != 0) {
	 restored = false;
	 }
	 }*/
//if (!restored) {
	/* If a shell is opened during the FocusOut event of a widget,
	 * it is required to set focus to all shells except for ON_TOP
	 * shells in order to maintain consistency with other platforms.
	 */
	/*if ((style & W_ON_TOP) == 0)
	 display.focusEvent = W_None;

	 if (defaultButton != null && !defaultButton.isDisposed()) {
	 defaultButton.setFocus();
	 } else {
	 setFocus();
	 }*/
//}
	return W_TRUE;
}
wresult _w_shell_print(w_control *shell, w_graphics *gc) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_graphics_is_ok(gc))
		return W_ERROR_INVALID_ARGUMENT;
	return W_FALSE;
}
void _w_shell_remove_tooltip(w_shell *shell, _w_tooltip *toolTip) {
	if (toolTip == _W_SHELL(shell)->tooltips) {
		_W_SHELL(shell)->tooltips = toolTip->next;
		if (_W_SHELL(shell)->tooltips != 0)
			_W_SHELL(shell)->tooltips->prev = toolTip->prev;
	} else {
		if (toolTip->next == 0) {
			_W_SHELL(shell)->tooltips->prev = toolTip->prev;
		} else {
			toolTip->next->prev = toolTip->prev;
		}
		toolTip->prev->next = toolTip->next;
	}
}
wresult _w_shell_set_active(w_shell *shell) {
	_w_shell_bring_totop(shell, W_FALSE);
	return W_TRUE;
}
void _w_shell_set_active_control(w_shell *shell, w_control *control) {
	_w_shell_set_active_control_0(shell, control, W_NONE);
}
w_control* _w_shell_path_next(w_shell *shell, w_control *control) {
	if (control == W_CONTROL(shell))
		return 0;
	return (w_control*) _W_CONTROL(control)->parent;
}
void _w_shell_set_active_control_0(w_shell *shell, w_control *control,
		int type) {
	if (control != 0 && !w_widget_is_ok(W_WIDGET(control)))
		control = 0;
	w_control *lastActive = _W_SHELL(shell)->lastActive;
	if (lastActive != 0 && !w_widget_is_ok(W_WIDGET(lastActive)))
		lastActive = 0;
	if (lastActive == control)
		return;

	/*
	 * Compute the list of controls to be activated and
	 * deactivated by finding the first common parent
	 * control.
	 */
	w_control *activate = control;
	w_control *deactivate = lastActive;
	_W_SHELL(shell)->lastActive = control;
	int index = 0;
	while (activate != 0 && deactivate != 0) {
		if (activate != deactivate)
			break;
		activate = _w_shell_path_next(shell, activate);
		deactivate = _w_shell_path_next(shell, deactivate);
		index++;
	}
	w_event e;
	/*
	 * It is possible (but unlikely), that application
	 * code could have destroyed some of the widgets. If
	 * this happens, keep processing those widgets that
	 * are not disposed.
	 */
	deactivate = lastActive;
	w_control *next;
	for (int i = 0; i < index; i++) {
		if (w_widget_is_ok(W_WIDGET(deactivate))) {
			e.type = W_EVENT_DEACTIVATE;
			e.time = 0;
			e.platform_event = 0;
			e.widget = W_WIDGET(deactivate);
			e.data = 0;
			_w_widget_send_event(W_WIDGET(deactivate), &e);
		}
		deactivate = _w_shell_path_next(shell, deactivate);
	}
	activate = control;
	for (int i = 0; i < index; i++) {
		if (w_widget_is_ok(W_WIDGET(activate))) {
			e.type = W_EVENT_ACTIVATE;
			e.time = 0;
			e.platform_event = 0;
			e.widget = W_WIDGET(activate);
			e.data = 0;
			//e.detail = type;
			_w_widget_send_event(W_WIDGET(activate), &e);
		}
	}
}
wresult _w_shell_set_alpha(w_shell *shell, int alpha) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
#if H_GTK_VERSION > VERSION(3,22,0)
	if (gdk_screen_is_composited(
			gtk_window_get_screen(GTK_WINDOW(shellHandle)))) {
#else
		if (gtk_widget_is_composited(shellHandle)) {
#endif
		/*
		 * Feature in GTK: gtk_window_set_opacity() is deprecated on GTK3.8
		 * onward. Use gtk_widget_set_opacity() instead.
		 */
		alpha &= 0xFF;
#if H_GTK_VERSION > VERSION(3,22,0)
		gtk_widget_set_opacity(shellHandle, (double) alpha / 255.0);
#else
			gtk_window_set_opacity(GTK_WINDOW(shellHandle), alpha / 255.0);
#endif
	}
	return W_TRUE;
}
void _w_shell_resize_bounds(w_control *control, int width, int height,
		int notify, _w_control_reserved *reserved) {
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(control));
	GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
	GdkWindow *redrawWindow = reserved->redrawWindow(W_WIDGET(control),
			reserved);
	if (redrawWindow != 0) {
		gdk_window_resize(redrawWindow, width, height);
	}
	GdkWindow *enableWindow = reserved->enableWindow(W_WIDGET(control),
			reserved);
	if (enableWindow != 0) {
		gdk_window_resize(enableWindow, width, height);
	}
	int border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
	int boxWidth = width - 2 * border;
	int boxHeight = height - 2 * border;
#if GTK3
	if ((_W_WIDGET(control)->style & W_RESIZE) == 0) {
		gtk_widget_set_size_request(vboxHandle, boxWidth, boxHeight);
	}
#else
#endif
	_w_shell_force_resize_0(control, boxWidth, boxHeight, reserved);
	if (notify) {
		_W_SHELL(control)->resized = 1;
		w_event _e;
		_e.type = W_EVENT_RESIZE;
		_e.widget = W_WIDGET(control);
		_e.time = 0;
		_e.platform_event = 0;
		_e.data = 0;
		_w_widget_send_event(W_WIDGET(control), &_e);
		_e.type = W_EVENT_LAYOUTDETECT;
		_w_widget_send_event(W_WIDGET(control), &_e);
	}
}
wresult _w_shell_set_bounds_0(w_control *control, w_point *location,
		w_size *size, _w_control_reserved *reserved) {
	wresult result = _w_control_set_bounds_0(control, location, size, reserved);
	if (result < 0)
		return result;
	w_event event;
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(control));
	if (location != 0) {
		int x_pos, y_pos;
		gtk_window_get_position(shellHandle, &x_pos, &y_pos);
		gtk_window_move(shellHandle, location->x, location->y);
		/*
		 * Bug in GTK: gtk_window_get_position () is not always up-to-date right after
		 * gtk_window_move (). The random delays cause problems like bug 445900.
		 *
		 * The workaround is to wait for the position change to be processed.
		 * The limit 1000 is an experimental value. I've seen cases where about 200
		 * iterations were necessary.
		 */
		for (int i = 0; i < 1000; i++) {
			int x2_pos, y2_pos;
			gtk_window_get_position(shellHandle, &x2_pos, &y2_pos);
			if (x2_pos == location->x && y2_pos == location->y) {
				break;
			}
		}
		if (x_pos != location->x || y_pos != location->y) {
			_W_SHELL(control)->moved = TRUE;
			_W_SHELL(control)->oldb.x = location->x;
			_W_SHELL(control)->oldb.y = location->y;
			event.type = W_EVENT_MOVE;
			event.widget = W_WIDGET(control);
			event.time = 0;
			event.platform_event = 0;
			event.data = 0;
			_w_widget_send_event(W_WIDGET(control), &event);
			if (w_widget_is_ok(W_WIDGET(control)) <= 0)
				return 0;
			result |= 1;
		}
	}
	if (size != 0) {
		int width = size->width;
		int height = size->height;
		//width = Math.max(1, Math.max(minWidth, width - trimWidth()));
		//height = Math.max(1, Math.max(minHeight, height - trimHeight()));
		/*
		 * If the shell is created without a RESIZE style bit, and the
		 * minWidth/minHeight has been set, allow the resize.
		 */
		if ((_W_WIDGET(control)->style & W_RESIZE) != 0
				|| (_W_SHELL(control)->minHeight != 0
						|| _W_SHELL(control)->minWidth != 0))
			gtk_window_resize(shellHandle, width, height);
		int changed = width != _W_SHELL(control)->oldb.width
				|| height != _W_SHELL(control)->oldb.height;
		if (changed) {
			_W_SHELL(control)->oldb.width = width;
			_W_SHELL(control)->oldb.height = height;
			result |= 2;		//resized
		}
		_w_shell_resize_bounds(control, width, height, changed, reserved);
	}
	return result;
}
wresult _w_shell_set_cursor(w_control *control, GdkCursor *cursor,
		_w_control_reserved *reserved) {
	GdkWindow *enableWindow = reserved->enableWindow(W_WIDGET(control),
			reserved);
	if (enableWindow != 0) {
		gdk_window_set_cursor(enableWindow, cursor);
		if (isX11()) {
			/*XDisplay* xDisplay = gdk_x11_display_get_xdisplay(
			 gdk_display_get_default());
			 XFlush(xDisplay);*/
#if H_GDK_VERSION > VERSION(3,22,0)
			gdk_display_flush(gdk_window_get_display(enableWindow));
#else
			gdk_flush();
#endif
		} else {
#if H_GDK_VERSION > VERSION(3,22,0)
			gdk_display_flush(gdk_window_get_display(enableWindow));
#else
			gdk_flush();
#endif
		}
	}
	return _w_control_set_cursor_0(control, cursor, reserved);
}

wresult _w_shell_set_enabled(w_control *_this, int enabled) {
	if (((_W_WIDGET(_this)->state & STATE_DISABLED) == 0) == enabled)
		return W_TRUE;
	w_control *control = 0;
	wbool fixFocus = W_FALSE;
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
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			_w_widget_get_reserved( W_WIDGET(_this)));
	reserved->enable_widget(_this, enabled, reserved);
	if (!w_widget_is_ok(W_WIDGET(_this)))
		return W_TRUE;
	if (enabled) {
		GdkWindow *enableWindow = reserved->enableWindow(W_WIDGET(_this),
				reserved);
		if (enableWindow != 0) {
			_w_control_cleanup_enable_window(_this, reserved);
		}
	} else {
		GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
		GtkWidget *parentHandle = shellHandle;
		gtk_widget_realize(parentHandle);
		GdkWindow *window = gtk_widget_get_window(parentHandle);
		w_rect rect;
		W_CONTROL_GET_CLASS(control)->get_bounds(control, &rect.pt, &rect.sz);
		GdkWindowAttr attributes;
		attributes.width = rect.width;
		attributes.height = rect.height;
		attributes.event_mask = (0xFFFFFFFF & ~(1 << 15)); //ExposureMask = 1 << 15
		attributes.wclass = GDK_INPUT_ONLY;
		attributes.window_type = GDK_WINDOW_CHILD;
		GdkWindow *enableWindow = gdk_window_new(window, &attributes, 0);
		if (enableWindow != 0) {
			_W_CONTROL(control)->window = enableWindow;
			_W_WIDGET(control)->state |= STATE_ENABLE_WINDOW;
			_W_WIDGET(control)->state &= ~STATE_REDRAW_WINDOW;
			if (_W_CONTROL(control)->cursor != 0) {
				gdk_window_set_cursor(enableWindow,
				_W_CURSOR(_W_CONTROL(control)->cursor)->handle);
				if (!isX11()) {
					/*XDisplay* xDisplay = gdk_x11_display_get_xdisplay(
					 gdk_display_get_default());
					 XFlush(xDisplay);*/
#if H_GDK_VERSION > VERSION(3,22,0)
					gdk_display_flush(gdk_window_get_display(enableWindow));
#else
			gdk_flush();
#endif
				} else {
#if H_GDK_VERSION > VERSION(3,22,0)
					gdk_display_flush(gdk_window_get_display(enableWindow));
#else
			gdk_flush();
#endif
				}
			}
			/* 427776: we need to listen to all enter-notify-event signals to
			 * see if this new GdkWindow has been added to a widget's internal
			 * hash table, so when the GdkWindow is destroyed we can also remove
			 * that reference. */
			if (enterNotifyEventSignalId != 0)
				_W_CONTROL(control)->enterNotifyEventId =
						g_signal_add_emission_hook(enterNotifyEventSignalId, 0,
								enterNotifyEventFunc, enableWindow, 0);

			gdk_window_set_user_data(enableWindow, parentHandle);
			gdk_window_show(enableWindow);
		}
	}
	if (fixFocus)
		_w_control_fix_focus(_this, control, reserved);
	if (enabled && gtk_toolkit->activeShell == W_SHELL(_this)) {
		if (!_w_shell_restore_focus(W_SHELL(control))) {
			_w_control_reserved *creserved = _W_CONTROL_RESERVED(
					_w_widget_get_reserved( W_WIDGET(control)));
			creserved->traverse_group(control, W_FALSE, creserved);
		}
	}
	return W_TRUE;
}
wresult _w_shell_set_full_screen(w_shell *shell, int fullScreen) {
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(shell));
	if (fullScreen) {
		gtk_window_fullscreen(shellHandle);
		_W_SHELL(shell)->fullScreen = fullScreen;
	} else {
		gtk_window_unfullscreen(shellHandle);
		if (_W_SHELL(shell)->maximized) {
			_w_shell_set_maximized(shell, W_TRUE);
		}
		_W_SHELL(shell)->fullScreen = 0;
	}
	return W_TRUE;
}
wresult _w_shell_set_ime_input_mode(w_shell *shell, int mode) {
//nothing
	return W_FALSE;
}
void _w_shell_set_initial_bounds(w_control *control,
		_w_control_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	GtkAllocation allocation;
	if ((_W_WIDGET(control)->state & STATE_FOREIGN_HANDLE) != 0) {
		gtk_widget_get_allocation(shellHandle, &allocation);
	} else {
		allocation.width = gdk_screen_width() * 5 / 8;
		allocation.height = gdk_screen_height() * 5 / 8;
		GdkScreen *screen = gdk_screen_get_default();
		if (screen != 0) {
			if (gdk_screen_get_n_monitors(screen) > 1) {
				GdkWindow *paintWindow = reserved->paintWindow(
						W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
				int monitorNumber = gdk_screen_get_monitor_at_window(screen,
						paintWindow);
				GdkRectangle dest;
				gdk_screen_get_monitor_geometry(screen, monitorNumber, &dest);
				allocation.width = dest.width * 5 / 8;
				allocation.height = dest.height * 5 / 8;
			}
		}
		if ((_W_WIDGET(control)->style & W_RESIZE) != 0) {
			gtk_window_resize(GTK_WINDOW(shellHandle), allocation.width,
					allocation.height);
		}
	}
	_w_shell_resize_bounds(control, allocation.width, allocation.height,
			W_FALSE, reserved);
}
wresult _w_shell_set_maximized(w_shell *shell, int maximized) {
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(shell));
	if (maximized) {
		_W_SHELL(shell)->maximized = 1;
		gtk_window_maximize(shellHandle);
	} else {
		_W_SHELL(shell)->maximized = 0;
		gtk_window_unmaximize(shellHandle);
	}
	return W_TRUE;
}
wresult _w_shell_set_menu_bar(w_shell *shell, w_menu *menu) {
	if (_W_SHELL(shell)->menubar == menu)
		return W_TRUE;
	wbool both = menu != 0 && _W_SHELL(shell)->menubar != 0;
	if (menu != 0) {
		if (!w_widget_is_ok(W_WIDGET(menu)))
			return W_ERROR_INVALID_ARGUMENT;
		if ((_W_WIDGET(menu)->style & W_BAR) == 0)
			return W_ERROR_MENU_NOT_BAR;
		if (_W_MENU(menu)->parent != W_CONTROL(shell))
			return W_ERROR_INVALID_PARENT;
	}
	if (_W_SHELL(shell)->menubar != 0) {
		GtkWidget *menuHandle =
		_W_WIDGET(_W_SHELL(shell)->menubar)->handle;
		gtk_widget_hide(menuHandle);
		_w_shell_destroy_accel_group(shell);
	}
	_W_SHELL(shell)->menubar = menu;
	if (_W_SHELL(shell)->menubar != 0) {
		GtkWidget *menuHandle =
		_W_WIDGET(_W_SHELL(shell)->menubar)->handle;
		gtk_widget_show(menuHandle);
		_w_shell_create_accel_group(shell);
		//menuBar.addAccelerators (accelGroup);
	}
	GtkWidget *vboxHandle = _W_SHELL_VBOX(shell);
	GtkAllocation allocation;
	gtk_widget_get_allocation(vboxHandle, &allocation);
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(shell));
	_w_shell_resize_bounds(W_CONTROL(shell), allocation.width,
			allocation.height, !both, _W_CONTROL_RESERVED(reserved));
	return W_TRUE;
}
wresult _w_shell_set_minimized(w_shell *shell, int minimized) {
	if (_W_SHELL(shell)->minimized == minimized)
		return W_TRUE;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	if (minimized) {
		_W_SHELL(shell)->minimized = 1;
		gtk_window_iconify(GTK_WINDOW(shellHandle));
	} else {
		_W_SHELL(shell)->minimized = 0;
		gtk_window_deiconify(GTK_WINDOW(shellHandle));
		_w_shell_bring_totop(shell, W_FALSE);
	}
	return W_TRUE;
}
wresult _w_shell_set_minimum_size(w_shell *shell, w_size *size) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	GdkGeometry geometry;
	int trimWidth = _w_shell_trim_width(shell);
	int trimHeight = _w_shell_trim_height(shell);
	_W_SHELL(shell)->minWidth = geometry.min_width = w_max(size->width,
			trimWidth) - trimWidth;
	_W_SHELL(shell)->minHeight = geometry.min_height = w_max(size->height,
			trimHeight) - trimHeight;
	gtk_window_set_geometry_hints(GTK_WINDOW(shellHandle), 0, &geometry,
			GDK_HINT_MIN_SIZE);
	return W_TRUE;
}
wresult _w_shell_set_modified(w_shell *shell, int modified) {
	if (modified) {
		_W_SHELL(shell)->modified = 1;
	} else {
		_W_SHELL(shell)->modified = 0;
	}
	return W_TRUE;
}
wresult _w_shell_set_region(w_control *shell, w_region *_region) {
	if (_region != 0 && !w_region_is_ok(_region))
		return W_ERROR_INVALID_ARGUMENT;
	wresult ret = W_TRUE;
	if ((_W_WIDGET(shell)->style & W_NO_TRIM) == 0)
		return ret;
	cairo_region_t *region = 0;
	if (_region != 0) {
		w_rect bounds;
		w_region_get_bounds(_region, &bounds);
		bounds.width += bounds.x;
		bounds.height += bounds.y;
		W_CONTROL_GET_CLASS(shell)->set_bounds(shell, 0, &bounds.sz);
		region = _W_REGION(_region)->handle;
	}
	if ((_W_WIDGET(shell)->style & W_RIGHT_TO_LEFT) != 0) {
		_W_SHELL(shell)->originalRegion = cairo_region_copy(region);
		_w_shell_mirror_region(_region, &region);
	} else {
		_W_SHELL(shell)->originalRegion = 0;
	}
	_w_control_set_region_0(shell, region);
	return ret;
}

wresult _w_shell_mirror_region(w_region *region, cairo_region_t **mirrored) {
	if (region == 0)
		return W_FALSE;
	*mirrored = cairo_region_create();
	if (*mirrored == 0)
		return W_ERROR_NO_HANDLES;
	cairo_region_t *rgn = _W_REGION(region)->handle;
	int nRects = 0;
	GdkRectangle *rects = 0;
	w_rect bounds;
	_gdk_region_get_rectangles(rgn, &rects, &nRects);
	if (rects != 0) {
		w_region_get_bounds(region, &bounds);
		GdkRectangle *rect;
		for (int i = 0; i < nRects; i++) {
			rect = &rects[i];
			rect->x = bounds.x + bounds.width - rect->x - rect->width;
			cairo_region_union_rectangle(*mirrored, rect);
		}
		g_free(rects);
		return W_TRUE;
	}
	return W_ERROR_NO_MEMORY;
}
void _w_shell_set_relations(w_control *control, _w_control_reserved *reserved) {

}
wresult _w_shell_set_text(w_shell *shell, const char *string, size_t length,
		int enc) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	size_t newlength;
	char *s = _gtk_text_fix(string, length, &newlength,
			enc | _GTK_TEXT_FIX_REQUIRED_NULL | _GTK_TEXT_FIX_CALCUL_LENGTH);
	if (s == 0)
		return W_ERROR_NO_MEMORY;
	char *ss = s;
	char tmp[10];
	/*
	 * GTK bug 82013.  For some reason, if the title string
	 * is less than 7 bytes long and is not terminated by
	 * a space, some window managers occasionally draw
	 * garbage after the last character in  the title.
	 * The fix is to pad the title.
	 */
	if (newlength < 6) {
		memcpy(tmp, s, newlength);
		for (int i = newlength; i <= 6; i++) {
			tmp[i] = ' ';
		}
		tmp[7] = 0;
		ss = tmp;
	}
	gtk_window_set_title(GTK_WINDOW(shellHandle), ss);
	_gtk_text_free(string, s);
	return W_TRUE;
}
wresult _w_shell_set_visible(w_control *control, int visible) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	w_composite *parent = _W_CONTROL(control)->parent;
	w_event e;
	int mask = W_PRIMARY_MODAL | W_APPLICATION_MODAL | W_SYSTEM_MODAL;
	if ((_W_WIDGET(control)->style & mask) != 0) {
		if (visible) {
			_w_toolkit_set_modal_shell(gtk_toolkit, W_SHELL(control));
			gtk_window_set_modal(GTK_WINDOW(shellHandle), TRUE);
		} else {
			_w_toolkit_clear_modal(gtk_toolkit, W_SHELL(control));
			gtk_window_set_modal(GTK_WINDOW(shellHandle), FALSE);
		}
		/*
		 * When in full-screen mode, the OS will always consider it to be the top of the display stack unless it is a dialog.
		 * This fix will give modal windows dialog-type priority if the parent is in full-screen, allowing it to be popped
		 * up in front of the full-screen window.
		 */
		if (parent != 0) {
			w_shell *shell;
			w_control_get_shell(W_CONTROL(parent), &shell);
			if (_W_SHELL(shell)->fullScreen)
				gtk_window_set_type_hint(GTK_WINDOW(shellHandle),
						GDK_WINDOW_TYPE_HINT_DIALOG);
		}
	} else {
		_w_shell_update_modal(W_SHELL(control));
	}
	_W_SHELL(control)->showWithParent = visible;
	if (gtk_widget_get_mapped(shellHandle) == visible)
		return W_TRUE;
	if (visible) {
		if (parent != 0 && (_W_WIDGET(control)->style & W_SHEET)) {
			_w_shell_center(control);
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_FALSE;
		}
		e.type = W_EVENT_SHOW;
		e.widget = W_WIDGET(control);
		e.platform_event = 0;
		e.data = 0;
		_w_widget_send_event(W_WIDGET(control), &e);
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_FALSE;

		gtk_widget_show(shellHandle);
		/*
		 * In order to ensure that the shell is visible
		 * and fully painted, dispatch events such as
		 * GDK_MAP and GDK_CONFIGURE, until the GDK_MAP
		 * event for the shell is received.
		 *
		 * Note that if the parent is minimized or withdrawn
		 * from the desktop, this should not be done since
		 * the shell not will be mapped until the parent is
		 * unminimized or shown on the desktop.
		 */
		_W_SHELL(control)->mapped = W_FALSE;
		gtk_widget_show(shellHandle);
		_w_widget_reserved *reserved = _w_widget_get_reserved(
				W_WIDGET(control));
		GdkWindow *enableWindow = _W_CONTROL_RESERVED(reserved)->enableWindow(
				W_WIDGET(control), _W_CONTROL_RESERVED(reserved));
		if (enableWindow != 0)
			gdk_window_raise(enableWindow);
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_FALSE;
		//if (!GTK_IS_PLUG (shellHandle)) {
		gtk_toolkit->dispatchEvents[0] = GDK_EXPOSE;
		gtk_toolkit->dispatchEvents[1] = GDK_FOCUS_CHANGE;
		gtk_toolkit->dispatchEvents[2] = GDK_CONFIGURE;
		gtk_toolkit->dispatchEvents[3] = GDK_MAP;
		gtk_toolkit->dispatchEvents[4] = GDK_UNMAP;
		gtk_toolkit->dispatchEvents[5] = GDK_NO_EXPOSE;
		gtk_toolkit->dispatchEvents[6] = GDK_WINDOW_STATE;
		gtk_toolkit->dispatchEvents[7] = -1;
		_w_toolkit_put_gdk_events();
		wbool iconic = W_FALSE;
		w_shell *shell;
		if (parent != 0) {
			w_control_get_shell(W_CONTROL(parent), &shell);
		} else
			shell = 0;
		do {
			/*
			 * This call to gdk_threads_leave() is a temporary work around
			 * to avoid deadlocks when gdk_threads_init() is called by native
			 * code outside of SWT (i.e AWT, etc). It ensures that the current
			 * thread leaves the GTK lock before calling the function below.
			 */
			gdk_threads_leave();
			g_main_context_iteration(0, FALSE);
			if (!w_widget_is_ok(W_WIDGET(control)))
				break;
			iconic = _W_SHELL(control)->minimized
					|| (shell != 0 && _W_SHELL(shell)->minimized);
		} while (!_W_SHELL(control)->mapped && !iconic);
		gtk_toolkit->dispatchEvents[0] = 0;
		if (!w_widget_is_ok(W_WIDGET(control)))
			return W_FALSE;
		if (!iconic) {
			_w_control_update_0(control, W_TRUE, W_TRUE);
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_FALSE;
			_w_shell_adjust_trim(W_SHELL(control));
		}
		//}//if (!GTK_IS_PLUG (shellHandle))
		_W_SHELL(control)->mapped = W_TRUE;

		if ((_W_WIDGET(control)->style & mask) != 0) {
			/*gdk_pointer_ungrab(gtk_widget_get_window(shellHandle),
			 GDK_CURRENT_TIME);*/
			gdk_pointer_ungrab(GDK_CURRENT_TIME);
		}
		_W_SHELL(control)->opened = W_TRUE;
		if (!_W_SHELL(control)->moved) {
			_W_SHELL(control)->moved = W_TRUE;
			w_rect location;
			W_CONTROL_GET_CLASS(control)->get_bounds(control, &location.pt,
					&location.sz);
			_W_SHELL(control)->oldb.x = location.x;
			_W_SHELL(control)->oldb.y = location.y;
			e.type = W_EVENT_MOVE;
			e.widget = W_WIDGET(control);
			e.platform_event = 0;
			e.data = 0;
			e.time = 0;
			_w_widget_send_event(W_WIDGET(control), &e);
			if (!w_widget_is_ok(W_WIDGET(control)))
				return W_FALSE;
		}
		if (!_W_SHELL(control)->resized) {
			_W_SHELL(control)->resized = 1;
			GtkAllocation allocation;
			GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
			gtk_widget_get_allocation(vboxHandle, &allocation);
			int border = 0;
			if ((_W_WIDGET(control)->style
					& (W_NO_TRIM | W_BORDER | W_SHELL_TRIM)) == 0) {
				border = gtk_container_get_border_width(
						GTK_CONTAINER(shellHandle));
			}
			_W_SHELL(control)->oldb.width = allocation.width + 2 * border;
			_W_SHELL(control)->oldb.height = allocation.height + 2 * border;
			e.type = W_EVENT_RESIZE;
			e.widget = W_WIDGET(control);
			e.platform_event = 0;
			e.data = 0;
			e.time = 0;
			_w_widget_send_event(W_WIDGET(control), &e);
			e.type = W_EVENT_LAYOUTDETECT;
			e.widget = W_WIDGET(control);
			e.platform_event = 0;
			e.data = 0;
			e.time = 0;
			_w_widget_send_event(W_WIDGET(control), &e);
		}
	} else {
		_w_shell_fix_active_shell(W_SHELL(control));
		gtk_widget_hide(shellHandle);
		e.type = W_EVENT_HIDE;
		e.widget = W_WIDGET(control);
		e.platform_event = 0;
		e.data = 0;
		_w_widget_send_event(W_WIDGET(control), &e);
	}
	return W_TRUE;
}
wresult _w_shell_set_ZOrder(w_control *control, w_control *sibling, int flags,
		_w_control_reserved *reserved) {
	/*
	 * Bug in GTK+.  Changing the toplevel window Z-order causes
	 * X to send a resize event.  Before the shell is mapped, these
	 * resize events always have a size of 200x200, causing extra
	 * layout work to occur.  The fix is to modify the Z-order only
	 * if the shell has already been mapped at least once.
	 */
	/* Shells are never included in labelled-by relations */
	if (_W_SHELL(control)->mapped) {
		flags &= ~ ZORDER_FIX_RELATION;
		return _w_control_set_ZOrder(control, sibling, flags, reserved);
	}
	return W_TRUE;
}
void* _w_shell_map_proc(GtkWidget *handle, void *arg0, void *user_data) {
	w_shell *shell = (w_shell*) g_object_get_qdata(G_OBJECT(handle),
			gtk_toolkit->quark[0]);
	if (shell != 0) {
		_W_SHELL(shell)->mapped = W_TRUE;
	}
	gtk_toolkit->dispatchEvents[0] = 0;
	return 0;
}
void _w_shell_show_widget(w_control *control, _w_control_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	if ((_W_WIDGET(control)->state & STATE_FOREIGN_HANDLE) != 0) {
		/*
		 * In case of foreign handles, activeShell might not be initialised as
		 * no focusIn events are generated on the window until the window loses
		 * and gain focus.
		 */
		if (gtk_window_is_active(GTK_WINDOW(shellHandle))) {
			gtk_toolkit->activeShell = W_SHELL(control);
			gtk_toolkit->activePending = W_TRUE;
		}
		GList *children = gtk_container_get_children(
				GTK_CONTAINER(shellHandle));
		GList *list = children;
		while (list != 0) {
			gtk_container_remove(GTK_CONTAINER(shellHandle),
					GTK_WIDGET(list->data));
			list = list->next;
		}
		g_list_free(children);
	}
	GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
	gtk_widget_show_all(vboxHandle);
}
GtkWidget* _w_shell_size_allocate_proc(GtkWidget *handle,
		GtkAllocation *allocation) {
	int offset = 16;
	gint x = 0, y = 0;
	_gdk_window_get_device_position(0, &x, &y, 0);
	y += offset;
	GdkScreen *screen = gdk_screen_get_default();
	if (screen != 0) {
		int monitorNumber = gdk_screen_get_monitor_at_point(screen, x, y);
		GdkRectangle dest;
		gdk_screen_get_monitor_geometry(screen, monitorNumber, &dest);
		GtkAllocation allocation;
		gtk_widget_get_allocation(handle, &allocation);
		int width = allocation.width;
		int height = allocation.height;
		if (x + width > dest.x + dest.width) {
			x = (dest.x + dest.width) - width;
		}
		if (y + height > dest.y + dest.height) {
			y = (dest.y + dest.height) - height;
		}
	}
	gtk_window_move(GTK_WINDOW(handle), x, y);
	return 0;
}
GtkWidget* _w_shell_size_request_proc(GtkWidget *handle,
		GtkAllocation *allocation) {
	gtk_widget_hide(handle);
	return 0;
}
wresult _w_shell_traverse_escape(w_control *control,
		_w_control_reserved *reserved) {
	if (_W_CONTROL(control)->parent == 0)
		return W_FALSE;
	struct _w_control_class *clazz = W_CONTROL_GET_CLASS(control);
	if (clazz->is_visible(control) <= 0 || !clazz->is_enabled(control) <= 0)
		return W_FALSE;
	_w_shell_close(W_SHELL(control));
	return W_TRUE;
}
int _w_shell_trim_height(w_shell *shell) {
	int style = _W_WIDGET(shell)->style;
	if ((style & W_NO_TRIM) != 0)
		return 0;
	if (_W_SHELL(shell)->fullScreen)
		return 0;
	int hasTitle = W_FALSE, hasResize = W_FALSE, hasBorder = W_FALSE;
	hasTitle = (style & (W_MIN | W_MAX | /*W_TITLE |*/W_MENU)) != 0;
	hasResize = (style & W_RESIZE) != 0;
	hasBorder = (style & W_BORDER) != 0;
	if (hasTitle) {
		if (hasResize)
			return gtk_toolkit->trimHeights[TRIM_TITLE_RESIZE];
		if (hasBorder)
			return gtk_toolkit->trimHeights[TRIM_TITLE_BORDER];
		return gtk_toolkit->trimHeights[TRIM_TITLE];
	}
	if (hasResize)
		return gtk_toolkit->trimHeights[TRIM_RESIZE];
	if (hasBorder)
		return gtk_toolkit->trimHeights[TRIM_BORDER];
	return gtk_toolkit->trimHeights[TRIM_NONE];
}
int _w_shell_trim_width(w_shell *shell) {
	wuint64 style = _W_WIDGET(shell)->style;
	if ((style & W_NO_TRIM) != 0)
		return 0;
	if (_W_SHELL(shell)->fullScreen)
		return 0;
	int hasTitle = W_FALSE, hasResize = W_FALSE, hasBorder = W_FALSE;
	hasTitle = (style & (W_MIN | W_MAX | /*W_TITLE |*/W_MENU)) != 0;
	hasResize = (style & W_RESIZE) != 0;
	hasBorder = (style & W_BORDER) != 0;
	if (hasTitle) {
		if (hasResize)
			return gtk_toolkit->trimWidths[TRIM_TITLE_RESIZE];
		if (hasBorder)
			return gtk_toolkit->trimWidths[TRIM_TITLE_BORDER];
		return gtk_toolkit->trimWidths[TRIM_TITLE];
	}
	if (hasResize)
		return gtk_toolkit->trimWidths[TRIM_RESIZE];
	if (hasBorder)
		return gtk_toolkit->trimWidths[TRIM_BORDER];
	return gtk_toolkit->trimWidths[TRIM_NONE];
}
void _w_shell_update_modal(w_shell *_this) {
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(_this));
	/*if (GTK_IS_PLUG(shellHandle))
	 return;*/
	GtkWindowGroup *group = 0;
	wbool isModalShell = W_FALSE;
	if (!gtk_toolkit->modalDialog) {
		w_shell *modal = _w_shell_get_modal_shell(_this);
		int mask = W_PRIMARY_MODAL | W_APPLICATION_MODAL | W_SYSTEM_MODAL;
		w_composite *shell = 0;
		if (modal == 0) {
			if ((_W_WIDGET(_this)->style & mask) != 0) {
				shell = W_COMPOSITE(_this);
				/*
				 * Feature in GTK. If a modal shell is reassigned to
				 * a different group, then it's modal state is not.
				 * persisted against the new group.
				 * The fix is to reset the modality before it is changed
				 * into a different group and then, set back after it
				 * assigned into new group.
				 */
				isModalShell = gtk_window_get_modal(shellHandle);
				if (isModalShell)
					gtk_window_set_modal(shellHandle, FALSE);
			}
		} else {
			shell = W_COMPOSITE(modal);
		}
		w_composite *topModalShell = shell;
		while (shell != 0) {
			if ((_W_WIDGET(shell)->style & mask) == 0) {
				w_shell *tmp;
				w_control_get_shell(W_CONTROL(shell), &tmp);
				group = _W_SHELL(tmp)->group;
				break;
			}
			topModalShell = shell;
			shell = _W_CONTROL(shell)->parent;
		}
		/*
		 * If a modal shell doesn't have any parent (or modal shell as it's parent),
		 * then we incorrectly add the modal shell to the default group, due to which
		 * children of that modal shell are not interactive. The fix is to ensure
		 * that whenever there is a modal shell in the hierarchy, then we always
		 * add the modal shell's group to that modal shell and it's modelless children
		 * in a different group.
		 */
		if (group == 0 && topModalShell != 0) {
			w_shell *tmp;
			w_control_get_shell(W_CONTROL(topModalShell), &tmp);
			group = _W_SHELL(tmp)->group;
		}
	}
	if (group == 0) {
		/*
		 * Feature in GTK. Starting with GTK version 2.10, GTK
		 * doesn't assign windows to a default group. The fix is to
		 * get the handle of the default group and add windows to the
		 * group.
		 */
		group = gtk_window_get_group(0);
	}
	if (group != 0) {
		gtk_window_group_add_window(group, shellHandle);
		if (isModalShell)
			gtk_window_set_modal(shellHandle, W_TRUE);
	} else {
		if (_W_SHELL(_this)->modalGroup != 0) {
			gtk_window_group_remove_window(_W_SHELL(_this)->modalGroup,
					shellHandle);
		}
	}
	_W_SHELL(_this)->modalGroup = group;
}
void _w_shell_update_minimized(w_shell *_this, int minimized) {
	_w_shell *shell = gtk_toolkit->shells, *tmp;
	while (shell != 0) {
		if (_W_CONTROL(shell)->parent == W_COMPOSITE(_this)) {
			wbool update = W_FALSE;
			tmp = shell;
			while (tmp != 0 && W_SHELL(tmp) != _this
					&& !isUndecorated(_W_WIDGET(tmp)->style)) {
				tmp = (_w_shell*) _W_CONTROL(tmp)->parent;
			}
			if (tmp != 0 && W_SHELL(tmp) != _this)
				update = W_TRUE;
			if (update) {
				if (minimized) {
					if (w_control_is_visible(W_CONTROL(shell))) {
						shell->showWithParent = W_TRUE;
						gtk_widget_hide(_W_SHELL_HANDLE(shell));
					}
				} else {
					if (shell->showWithParent) {
						shell->showWithParent = W_FALSE;
						gtk_widget_show(_W_SHELL_HANDLE(shell));
					}
				}
			}
		}
		shell = shell->next;
	}
}
wresult _w_shell_force_active(w_shell *shell) {
	_w_shell_bring_totop(shell, W_TRUE);
	return W_TRUE;
}
wresult _w_shell_get_bounds(w_control *control, w_point *location,
		w_size *size) {
	GtkWindow *shellHandle = GTK_WINDOW(_W_SHELL_HANDLE(control));
	if (location != 0) {
		int x, y;
		gtk_window_get_position(shellHandle, &x, &y);
		location->x = x;
		location->y = y;
	}
	if (size != 0) {
		GtkWidget *vboxHandle = _W_SHELL_VBOX(control);
		GtkAllocation allocation;
		gtk_widget_get_allocation(vboxHandle, &allocation);
		int width = allocation.width;
		int height = allocation.height;
		int border = 0;
		if ((_W_WIDGET(control)->style & (W_NO_TRIM | W_BORDER | W_SHELL_TRIM))
				== 0) {
			border = gtk_container_get_border_width(GTK_CONTAINER(shellHandle));
		}
		size->width = width + _w_shell_trim_width(W_SHELL(control))
				+ 2 * border;
		size->height = height + _w_shell_trim_height(W_SHELL(control))
				+ 2 * border;
	}
	return W_TRUE;
}
void _w_shell_set_tooltip_text(w_shell *shell, GtkWidget *tipWidget,
		const char *string) {
	_w_shell_set_tooltip_text_0(shell, tipWidget, tipWidget, string);
}

void _w_shell_set_tooltip_text_0(w_shell *shell, GtkWidget *rootWidget,
		GtkWidget *tipWidget, const char *string) {
	struct _mnemonics buffer;
	memset(&buffer, 0, sizeof(buffer));
	if (string != 0 && strlen(string) > 0) {
		_mnemonics_fix(&buffer, string, -1, W_FALSE);
	}
	gchar *oldTooltip = gtk_widget_get_tooltip_text(rootWidget);
	wbool same = W_FALSE;
	if (buffer.str == 0 && oldTooltip == 0) {
		same = W_TRUE;
	} else if (buffer.str != 0 && oldTooltip != 0) {
		same = strcmp(oldTooltip, buffer.str) == 0;
	}
	if (oldTooltip != 0)
		g_free(oldTooltip);
	if (!same) {
		gtk_widget_set_tooltip_text(rootWidget, buffer.str);
	}
	_mnemonics_free(&buffer, string);
}
void _w_shell_get_window_origin(w_control *control, w_point *pt,
		_w_control_reserved *reserved) {
	if (!_W_SHELL(control)->mapped) {
		/*
		 * Special case: The handle attributes are not initialized until the
		 * shell is made visible, so gdk_window_get_origin () always returns {0, 0}.
		 *
		 * Once the shell is realized, gtk_window_get_position () includes
		 * window trims etc. from the window manager. That's why getLocation ()
		 * is not safe to use for coordinate mappings after the shell has been made visible.
		 */
		w_rect r;
		W_CONTROL_GET_CLASS(control)->get_bounds(control, &r.pt, &r.sz);
		pt->x = r.x;
		pt->y = r.y;
		return;
	}
	return _w_control_get_window_origin(control, pt, reserved);
}
gint _w_shell_image_compare(gconstpointer data1, gconstpointer data2) {
	wuint data1_width = gdk_pixbuf_get_width((GdkPixbuf*) data1);
	wuint data1_height = gdk_pixbuf_get_height((GdkPixbuf*) data1);
	wuint data2_width = gdk_pixbuf_get_width((GdkPixbuf*) data2);
	wuint data2_height = gdk_pixbuf_get_height((GdkPixbuf*) data2);
	if (data1_width == data2_width && data1_height == data2_height) {
		int transparent1 = 0;	//data1.getTransparencyType ();
		int transparent2 = 0;	//data2.getTransparencyType ();
		if (transparent1 == W_TRANSPARENCY_ALPHA)
			return -1;
		if (transparent2 == W_TRANSPARENCY_ALPHA)
			return 1;
		if (transparent1 == W_TRANSPARENCY_MASK)
			return -1;
		if (transparent2 == W_TRANSPARENCY_MASK)
			return 1;
		if (transparent1 == W_TRANSPARENCY_PIXEL)
			return -1;
		if (transparent2 == W_TRANSPARENCY_PIXEL)
			return 1;
		return 0;
	}
	return data1_width > data2_width || data1_height > data2_height ? -1 : 1;
}
w_widget* _w_shell_compute_tab_group(w_control *control,
		_w_control_reserved *reserved) {
	return W_WIDGET(control);
}

w_control* _w_shell_compute_tab_root(w_control *control,
		_w_control_reserved *reserved) {
	return control;
}
GtkAccelGroup* _w_shell_create_accel_group(w_shell *shell) {
	if (_W_SHELL(shell)->accelGroup == 0) {
		_W_SHELL(shell)->accelGroup = gtk_accel_group_new();
		if (_W_SHELL(shell)->accelGroup == 0)
			return 0;
		GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
		gtk_window_add_accel_group(GTK_WINDOW(shellHandle),
		_W_SHELL(shell)->accelGroup);
	}
	return _W_SHELL(shell)->accelGroup;
}

void _w_shell_destroy_accel_group(w_shell *shell) {
	if (_W_SHELL(shell)->accelGroup == 0)
		return;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	gtk_window_remove_accel_group(GTK_WINDOW(shellHandle),
	_W_SHELL(shell)->accelGroup);
//TEMPORARY CODE
	g_object_unref(_W_SHELL(shell)->accelGroup);
	_W_SHELL(shell)->accelGroup = 0;
}

void _w_shell_fix_accel_group(w_shell *shell) {
	if (_W_SHELL(shell)->menubar == 0)
		return;
	_w_shell_destroy_accel_group(shell);
	_w_shell_create_accel_group(shell);
	//menuBar.addAccelerators(accelGroup);
}
wresult _w_shell_get_default_button(w_shell *shell, w_button **_button) {
	w_button *button =
	_W_SHELL(shell)->defaultButton != 0 ?
	_W_SHELL(shell)->defaultButton :
											_W_SHELL(shell)->saveDefault;
	if (button != 0 && !w_widget_is_ok(W_WIDGET(button))) {
		*_button = 0;
	} else {
		*_button = button;
	}
	return W_TRUE;
}
void _w_shell_images_destroy(gpointer data) {
	if (data != 0) {
		g_object_unref(data);
	}
}
wresult _w_shell_get_images(w_shell *shell, w_image *images, size_t length) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	GList *icons = gtk_window_get_icon_list(GTK_WINDOW(shellHandle));
	size_t l = g_list_length(icons);
	if (images != 0) {
		size_t i = 0;
		while (icons != 0) {
			if (i >= length) {
				break;
			}
			w_image_dispose(&images[i]);
			_W_IMAGE(&images[i])->pixbuf = GDK_PIXBUF(icons->data);
			_W_IMAGE(&images[i])->flags = 0;
			icons->data = 0;
			i++;
			icons = icons->next;
		}
	}
	g_list_free_full(icons, _w_shell_images_destroy);
	return l;
}
wresult _w_shell_get_menu_bar(w_shell *shell, w_menu **menu) {
	*menu = _W_SHELL(shell)->menubar;
	return W_TRUE;
}
wresult _w_shell_get_minimized(w_shell *shell) {
	return _W_SHELL(shell)->minimized;
}
wresult _w_shell_get_text(w_shell *shell, w_alloc alloc, void *user_data,
		int enc) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	const char *text = gtk_window_get_title(GTK_WINDOW(shellHandle));
	_gtk_alloc_set_text(alloc, user_data, text, -1, enc);
	return W_TRUE;
}
wresult _w_shell_is_reparentable(w_control *shell) {
	return W_FALSE;
}

wresult _w_shell_is_tab_group(w_control *control,
		_w_control_reserved *reserved) {
	return W_TRUE;
}

wresult _w_shell_is_tab_item(w_control *control,
		_w_control_reserved *reserved) {
	return W_FALSE;
}
int _w_shell_restore_focus(w_shell *shell) {
	if (_W_SHELL(shell)->savedFocus != 0
			&& !w_widget_is_ok(W_WIDGET(_W_SHELL(shell)->savedFocus)))
		_W_SHELL(shell)->savedFocus = 0;
	wbool restored = _W_SHELL(shell)->savedFocus != 0
			&& W_CONTROL_GET_CLASS(_W_SHELL(shell)->savedFocus)->set_focus(
			_W_SHELL(shell)->savedFocus) > 0;
	_W_SHELL(shell)->savedFocus = 0;
	return restored;
}
wresult _w_shell_set_default_button(w_shell *shell, w_button *button) {
	GtkWidget *buttonHandle = 0;
	if (button != 0) {
		if (!w_widget_is_ok(W_WIDGET(button)))
			return W_ERROR_INVALID_ARGUMENT;
		w_shell *_shell;
		w_control_get_shell(W_CONTROL(button), &_shell);
		if (_shell != shell)
			return W_ERROR_INVALID_PARENT;
		buttonHandle = _W_WIDGET(button)->handle;
	}
	_W_SHELL(shell)->saveDefault = _W_SHELL(shell)->defaultButton = button;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	gtk_window_set_default(GTK_WINDOW(shellHandle), buttonHandle);
	return W_TRUE;
}
wresult _w_shell_set_images(w_shell *shell, w_image *images, size_t length) {
	GList *pixbufs = 0;
	if (images != 0) {
		for (size_t i = 0; i < 0; i++) {
			GdkPixbuf *pixbuf = _W_IMAGE(&images[i])->pixbuf;
			if (pixbuf != 0) {
				pixbufs = g_list_append(pixbufs, pixbuf);
			}
		}
		pixbufs = g_list_sort(pixbufs, _w_shell_image_compare);
	}
	GtkWidget *shellHandle = _W_SHELL_HANDLE(shell);
	gtk_window_set_icon_list(GTK_WINDOW(shellHandle), pixbufs);
	if (pixbufs != 0)
		g_list_free(pixbufs);
	return W_TRUE;
}
void _w_shell_set_orientation(w_control *control, int create,
		_w_control_reserved *reserved) {
	_w_composite_set_orientation(control, create, reserved);
	if (!create) {
		if (_W_SHELL(control)->menubar != 0)
			_w_menu_set_orientation_0(_W_SHELL(control)->menubar,
			_W_WIDGET(control)->style & (W_RIGHT_TO_LEFT | W_LEFT_TO_RIGHT));
	}
}
void _w_shell_set_saved_focus(w_shell *shell, w_control *control) {
	if (W_CONTROL(shell) == control)
		return;
	_W_SHELL(shell)->savedFocus = control;
}
wresult _w_shell_traverse_item(w_control *control, int next,
		_w_control_reserved *reserved) {
	return W_FALSE;
}

wresult _w_shell_traverse_return(w_control *control,
		_w_control_reserved *reserved) {
	w_button *button =
	_W_SHELL(control)->defaultButton != 0 ?
	_W_SHELL(control)->defaultButton :
											_W_SHELL(control)->saveDefault;
	if (button == 0 || !w_widget_is_ok(W_WIDGET(button)))
		return W_FALSE;
	/*
	 * Bug in GTK.  When a default button that is disabled is
	 * activated using the Enter key, GTK GP's.  The fix is to
	 * detect this case and stop GTK from processing the Enter
	 * key.
	 */
	struct _w_control_class *clazz = W_CONTROL_GET_CLASS(button);
	if (clazz->is_visible(W_CONTROL(button)) <= 0
			|| clazz->is_enabled(W_CONTROL(button)) <= 0)
		return W_TRUE;
	GtkWidget *shellHandle = _W_SHELL_HANDLE(control);
	return gtk_window_activate_default(GTK_WINDOW(shellHandle));
}
/*
 * signals
 */
gboolean _gtk_shell_destroy(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (_W_SHELL(widget)->originalRegion != 0) {
		cairo_region_destroy(_W_SHELL(widget)->originalRegion);
		_W_SHELL(widget)->originalRegion = 0;
	}
	_w_toolkit_remove_shell(_W_SHELL(widget));
	_w_shell_fix_active_shell(W_SHELL(widget));
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	gtk_widget_hide(shellHandle);
	_w_shell_destroy_accel_group(W_SHELL(widget));
	_w_toolkit_clear_modal(gtk_toolkit, W_SHELL(widget));
	if (gtk_toolkit->activeShell == W_SHELL(widget))
		gtk_toolkit->activeShell = 0;
	if (_W_SHELL(widget)->tooltipsHandle != 0)
		g_object_unref(G_OBJECT(_W_SHELL(widget)->tooltipsHandle));
	_W_SHELL(widget)->tooltipsHandle = 0;
	if (_W_SHELL(widget)->group != 0)
		g_object_unref(G_OBJECT(_W_SHELL(widget)->group));
	_W_SHELL(widget)->group = _W_SHELL(widget)->modalGroup = 0;
	_W_SHELL(widget)->lastActive = 0;
	gboolean ret = _gtk_composite_destroy(widget, e, reserved);
	if (gtk_toolkit->shells == 0) {
		w_toolkit_post_quit(W_TOOLKIT(gtk_toolkit), EXIT_SUCCESS);
	}
	return ret;
}
gboolean _gtk_shell_button_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget == shellHandle) {
		wuint64 style = _W_WIDGET(widget)->style;
		if (ISCUSTOMRESIZE(style)) {
			if ((style & W_ON_TOP) != 0 && (style & W_NO_FOCUS) == 0) {
				_w_shell_force_active(W_SHELL(widget));
			}
			GdkEventButton *gdkEvent = (GdkEventButton*) e->args[0];
			if (gdkEvent->button == 1) {
				gtk_toolkit->resizeLocationX = gdkEvent->x_root;
				gtk_toolkit->resizeLocationY = gdkEvent->y_root;
				int x = 0, y = 0;
				gtk_window_get_position(GTK_WINDOW(shellHandle), &x, &y);
				gtk_toolkit->resizeBoundsX = x;
				gtk_toolkit->resizeBoundsY = y;
				GtkAllocation allocation;
				gtk_widget_get_allocation(shellHandle, &allocation);
				gtk_toolkit->resizeBoundsWidth = allocation.width;
				gtk_toolkit->resizeBoundsHeight = allocation.height;
			}
		}
		return FALSE;
	}
	return _gtk_canvas_button_press_event(widget, e, reserved);
}

gboolean _gtk_shell_configure_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	int x = 0, y = 0;
	gtk_window_get_position(GTK_WINDOW(shellHandle), &x, &y);

	if (W_CONTROL_GET_CLASS(widget)->is_visible(W_CONTROL(widget)) <= 0) {
		return FALSE; //We shouldn't handle move/resize events if shell is hidden.
	}
	if (!_W_SHELL(widget)->moved || _W_SHELL(widget)->oldb.x != x
			|| _W_SHELL(widget)->oldb.y != y) {
		_W_SHELL(widget)->moved = W_TRUE;
		_W_SHELL(widget)->oldb.x = x;
		_W_SHELL(widget)->oldb.y = y;
		w_event event;
		event.type = W_EVENT_MOVE;
		event.widget = widget;
		event.platform_event = (w_event_platform*) e;
		event.time = 0;
		event.data = 0;
		w_widget_send_event(widget, &event);
		// widget could be disposed at this point
	}
	return FALSE;
}

gboolean _gtk_shell_delete_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (W_CONTROL_GET_CLASS(widget)->is_enabled(W_CONTROL(widget)) > 0) {
		_w_shell_close_widget(W_SHELL(widget), 0);
	}
	return TRUE;
}

gboolean _gtk_shell_enter_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget != shellHandle) {
		return _gtk_control_enter_notify_event(widget, e, reserved);
	}
	return FALSE;
}

gboolean _gtk_shell_draw(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
#if USE_CAIRO
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget == shellHandle) {
		if (ISCUSTOMRESIZE(_W_WIDGET(widget)->style)) {
			int width;
			int height;
			GdkWindow *window = gtk_widget_get_window(e->widget);
#if GTK3
			width = gdk_window_get_width(window);
			height = gdk_window_get_height(window);
#else
#endif
			int border = gtk_container_get_border_width(
					GTK_CONTAINER(e->widget));
			GtkStyleContext *context = gtk_widget_get_style_context(
					shellHandle);
			//draw shell frame on GTK3
			gtk_style_context_save(context);
			cairo_t *cairo = (cairo_t*) e->args[0];
			gtk_render_frame(context, cairo, 0, 0, width, border);
			gtk_render_frame(context, cairo, 0, height - border, width, border);
			gtk_render_frame(context, cairo, 0, border, border,
					height - border - border);
			gtk_render_frame(context, cairo, width - border, border, border,
					height - border - border);
			gtk_render_frame(context, cairo, 0 + 10, 0 + 10, width - 20,
					height - 20);
			gtk_style_context_restore(context);
			return TRUE;
		}
		return FALSE;
	}
	return _gtk_canvas_draw(widget, e, reserved);
#else
#endif
	return FALSE;
}

gboolean _gtk_shell_focus(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return _gtk_composite_focus(widget, e, reserved);
}

gboolean _gtk_shell_focus_in_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget != shellHandle) {
		return _gtk_canvas_focus_in_event(widget, e, reserved);
	}
	gtk_toolkit->activeShell = W_SHELL(widget);
	gtk_toolkit->activePending = W_FALSE;
	w_event event;
	event.type = W_EVENT_ACTIVATE;
	event.widget = widget;
	event.platform_event = (w_event_platform*) e;
	event.time = 0;
	event.data = 0;
	w_widget_send_event(widget, &event);
	return FALSE;
}

gboolean _gtk_shell_focus_out_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget != shellHandle) {
		return _gtk_canvas_focus_out_event(widget, e, reserved);
	}
	w_event event;
	event.type = W_EVENT_DEACTIVATE;
	event.widget = widget;
	event.platform_event = (w_event_platform*) e;
	event.time = 0;
	event.data = 0;
	w_widget_send_event(widget, &event);
	_w_shell_set_active_control(W_SHELL(widget), 0);
	if (gtk_toolkit->activeShell == W_SHELL(widget)
			&& !_W_SHELL(widget)->ignoreFocusOut) {
		gtk_toolkit->activeShell = 0;
		gtk_toolkit->activePending = W_FALSE;
	}
	return FALSE;
}

gboolean _gtk_shell_leave_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget == shellHandle) {
		wuint64 style = _W_WIDGET(widget)->style;
		if (ISCUSTOMRESIZE(style)) {
			GdkEventCrossing *gdkEvent = (GdkEventCrossing*) e->args[0];
			if ((gdkEvent->state & GDK_BUTTON1_MASK) == 0) {
				GdkWindow *window = gtk_widget_get_window(shellHandle);
				gdk_window_set_cursor(window, 0);
				gtk_toolkit->resizeMode = 0;
			}
		}
		return FALSE;
	}
	return _gtk_control_leave_notify_event(widget, e, reserved);
}

gboolean _gtk_shell_move_focus(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	w_control *control = _w_toolkit_get_focus_control(W_TOOLKIT(gtk_toolkit));
	if (control != 0) {
		_w_widget_reserved *freserved = _w_widget_get_reserved(
				W_WIDGET(control));
		GtkWidget *focusHandle =
		_W_CONTROL_RESERVED(freserved)->focusHandle(W_WIDGET(control),
				freserved);
		gtk_widget_child_focus(focusHandle,
				(GtkDirectionType) ((intptr_t) e->args[0]));
	}
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	g_signal_stop_emission_by_name(shellHandle, "move-focus");
	return TRUE;
}

gboolean _gtk_shell_motion_notify_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget == shellHandle) {
		wuint64 style = _W_WIDGET(widget)->style;
		if (ISCUSTOMRESIZE(style)) {
			GdkEventMotion *gdkEvent = (GdkEventMotion*) e->args[0];
			if ((gdkEvent->state & GDK_BUTTON1_MASK) != 0) {
				int border = gtk_container_get_border_width(
						GTK_CONTAINER(shellHandle));
				int dx = (int) (gdkEvent->x_root - gtk_toolkit->resizeLocationX);
				int dy = (int) (gdkEvent->y_root - gtk_toolkit->resizeLocationY);
				int x = gtk_toolkit->resizeBoundsX;
				int y = gtk_toolkit->resizeBoundsY;
				int width = gtk_toolkit->resizeBoundsWidth;
				int height = gtk_toolkit->resizeBoundsHeight;
				int newWidth = w_max(width - dx,
						w_max(_W_SHELL(widget)->minWidth, border + border));
				int newHeight = w_max(height - dy,
						w_max(_W_SHELL(widget)->minHeight, border + border));
				switch (gtk_toolkit->resizeMode) {
				case GDK_LEFT_SIDE:
					x += width - newWidth;
					width = newWidth;
					break;
				case GDK_TOP_LEFT_CORNER:
					x += width - newWidth;
					width = newWidth;
					y += height - newHeight;
					height = newHeight;
					break;
				case GDK_TOP_SIDE:
					y += height - newHeight;
					height = newHeight;
					break;
				case GDK_TOP_RIGHT_CORNER:
					width = w_max(width + dx,
							w_max(_W_SHELL(widget)->minWidth, border + border));
					y += height - newHeight;
					height = newHeight;
					break;
				case GDK_RIGHT_SIDE:
					width = w_max(width + dx,
							w_max(_W_SHELL(widget)->minWidth, border + border));
					break;
				case GDK_BOTTOM_RIGHT_CORNER:
					width = w_max(width + dx,
							w_max(_W_SHELL(widget)->minWidth, border + border));
					height = w_max(height + dy,
							w_max(_W_SHELL(widget)->minHeight,
									border + border));
					break;
				case GDK_BOTTOM_SIDE:
					height = w_max(height + dy,
							w_max(_W_SHELL(widget)->minHeight,
									border + border));
					break;
				case GDK_BOTTOM_LEFT_CORNER:
					x += width - newWidth;
					width = newWidth;
					height = w_max(height + dy,
							w_max(_W_SHELL(widget)->minHeight,
									border + border));
					break;
				}
				if (x != gtk_toolkit->resizeBoundsX
						|| y != gtk_toolkit->resizeBoundsY) {
					gdk_window_move_resize(gtk_widget_get_window(shellHandle),
							x, y, width, height);
				} else {
					gtk_window_resize(GTK_WINDOW(shellHandle), width, height);
				}
			} else {
				GdkCursorType mode = (GdkCursorType) _w_shell_get_resize_mode(
						W_SHELL(widget), gdkEvent->x, gdkEvent->y);
				if (mode != gtk_toolkit->resizeMode) {
					GdkWindow *window = gtk_widget_get_window(shellHandle);
					GdkCursor *cursor = gdk_cursor_new_for_display(
							gdk_display_get_default(), mode);
					gdk_window_set_cursor(window, cursor);
					gdk_cursor_unref(cursor);
					gtk_toolkit->resizeMode = mode;
				}
			}
		}
		return FALSE;
	}
	return _gtk_control_motion_notify_event(widget, e, reserved);
}

gboolean _gtk_shell_key_press_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	if (e->widget == shellHandle) {
		/* Stop menu mnemonics when the shell is disabled */
		if ((_W_WIDGET(widget)->state & STATE_DISABLED) != 0)
			return TRUE;

		if (w_widget_is_ok(W_WIDGET(_W_SHELL(widget)->menubar))) {
			w_control *focusControl = _w_toolkit_get_focus_control(
					W_TOOLKIT(gtk_toolkit));
			if (w_widget_is_ok(W_WIDGET(focusControl))) {
				gchar *accel = 0;
				GtkSettings *setting = gtk_settings_get_default();
				g_object_get(setting, "gtk-menu-bar-accel", &accel, NULL);
				if (accel != 0) {
					guint keyval;
					GdkModifierType mods;
					gtk_accelerator_parse(accel, &keyval, &mods);
					g_free(accel);
					if (keyval != 0) {
						GdkEventKey *keyEvent = (GdkEventKey*) e->args[0];
						int mask = gtk_accelerator_get_default_mod_mask();
						if (keyEvent->keyval == keyval
								&& (keyEvent->state & mask) == (mods & mask)) {
							_w_widget_reserved *freserved =
									_w_widget_get_reserved(
											W_WIDGET(focusControl));
							e->widget =
							_W_CONTROL_RESERVED(freserved)->focusHandle(
									W_WIDGET(focusControl), freserved);
							gboolean ret =
									freserved->signals[SIGNAL_KEY_PRESS_EVENT](
											W_WIDGET(focusControl), e,
											freserved);
							e->widget = shellHandle;
							return ret;
						}
					}
				}
			}
		}
		return FALSE;
	}
	return _gtk_composite_key_press_event(widget, e, reserved);
}

gboolean _gtk_shell_size_allocate(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	GtkAllocation widgetAllocation;
	gtk_widget_get_allocation(shellHandle, &widgetAllocation);
	int width = widgetAllocation.width;
	int height = widgetAllocation.height;

//	Bug 474235: on Wayland gtk_size_allocate() is called more frequently, causing an
//  infinitely recursive resize call. This causes non-resizable Shells/Dialogs to
//  crash. Fix: only call resizeBounds() on resizable Shells.
#if GTK3
	if ((!(_W_SHELL(widget)->resized) || _W_SHELL(widget)->oldb.width != width
			|| _W_SHELL(widget)->oldb.height != height)) { //Wayland
#endif
		_W_SHELL(widget)->oldb.width = width;
		_W_SHELL(widget)->oldb.height = height;
		_w_shell_resize_bounds(W_CONTROL(widget), width, height, W_TRUE,
				_W_CONTROL_RESERVED(reserved)); //this is called to resize child widgets when the shell is resized.
	}
	return FALSE;
}

gboolean _gtk_shell_realize(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_composite_realize(widget, e, reserved);
	GtkWidget *shellHandle = _W_SHELL_HANDLE(widget);
	GdkWindow *window = gtk_widget_get_window(shellHandle);
	wuint64 style = _W_WIDGET(widget)->style;
	if ((style & W_SHELL_TRIM) != W_SHELL_TRIM) {
		int decorations = 0;
		int functions = 0;
		if ((style & W_NO_TRIM) == 0) {
			if ((style & W_MIN) != 0) {
				decorations |= GDK_DECOR_MINIMIZE;
				functions |= GDK_FUNC_MINIMIZE;
			}
			if ((style & W_MAX) != 0) {
				decorations |= GDK_DECOR_MAXIMIZE;
				functions |= GDK_FUNC_MAXIMIZE;
			}
			if ((style & W_RESIZE) != 0) {
				decorations |= GDK_DECOR_RESIZEH;
				functions |= GDK_FUNC_RESIZE;
			}
			if ((style & W_BORDER) != 0)
				decorations |= GDK_DECOR_BORDER;
			if ((style & W_MENU) != 0)
				decorations |= GDK_DECOR_MENU;
			if ((style & W_TITLE) != 0)
				decorations |= GDK_DECOR_TITLE;
			if ((style & W_CLOSE) != 0)
				functions |= GDK_FUNC_CLOSE;
			/*
			 * Feature in GTK.  Under some Window Managers (Sawmill), in order
			 * to get any border at all from the window manager it is necessary to
			 * set GDK_DECOR_BORDER.  The fix is to force these bits when any
			 * kind of border is requested.
			 */
			if ((style & W_RESIZE) != 0)
				decorations |= GDK_DECOR_BORDER;
			if ((style & W_NO_MOVE) == 0)
				functions |= GDK_FUNC_MOVE;
		}
		gdk_window_set_decorations(window, (GdkWMDecoration) decorations);

		/*
		 * For systems running Metacity, this call forces the style hints to
		 * be displayed in a window's titlebar. Otherwise, the decorations
		 * set by the function gdk_window_set_decorations (window,
		 * decorations) are ignored by the window manager.
		 */
		gdk_window_set_functions(window, (GdkWMFunction) functions);
	} else if ((style & W_NO_MOVE) != 0) {
		// if the GDK_FUNC_ALL bit is present, all the other style
		// bits specified as a parameter will be removed from the window
		gdk_window_set_functions(window,
				(GdkWMFunction) (GDK_FUNC_ALL | GDK_FUNC_MOVE));
	}
	if ((style & W_ON_TOP) != 0) {
		gdk_window_set_override_redirect(window, W_TRUE);
	}
	return result;
}

gboolean _gtk_shell_window_state_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GdkEventWindowState *gdkEvent = (GdkEventWindowState*) e->args[0];
	w_event _e;
	if (gdkEvent->new_window_state & GDK_WINDOW_STATE_ICONIFIED) {
		_W_SHELL(widget)->minimized = 1;
	} else {
		_W_SHELL(widget)->minimized = 1;
	}
	if (gdkEvent->new_window_state & GDK_WINDOW_STATE_MAXIMIZED) {
		_W_SHELL(widget)->maximized = 1;
	} else {
		_W_SHELL(widget)->maximized = 0;
	}
	if (gdkEvent->new_window_state & GDK_WINDOW_STATE_FULLSCREEN) {
		_W_SHELL(widget)->fullScreen = 1;
	} else {
		_W_SHELL(widget)->fullScreen = 0;
	}
	if ((gdkEvent->changed_mask & GDK_WINDOW_STATE_ICONIFIED) != 0) {
		_e.widget = widget;
		_e.platform_event = (w_event_platform*) e;
		_e.data = 0;
		if (_W_SHELL(widget)->minimized) {
			_e.type = W_EVENT_ICONIFY;
			_w_widget_send_event(widget, &_e);
		} else {
			_e.type = W_EVENT_DEICONIFY;
			_w_widget_send_event(widget, &_e);
		}
		_w_shell_update_minimized(W_SHELL(widget), _W_SHELL(widget)->minimized);
	}
	return FALSE;
}
void _w_shell_class_init(struct _w_shell_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SHELL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_shell_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_shell);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_shell);
	if (sizeof(_w_shell) > (sizeof(w_shell) - sizeof(void*))) {
		fprintf(stderr, "Warning : sizeof(_w_shell) > sizeof(w_shell)");
	}
	/*
	 * shell class
	 */
	W_WIDGET_CLASS(clazz)->create = _w_shell_create;
	W_CONTROL_CLASS(clazz)->get_shell = _w_shell_get_shell;
	W_CONTROL_CLASS(clazz)->is_enabled = _w_shell_is_enabled;
	W_CONTROL_CLASS(clazz)->is_visible = _w_shell_is_visible;
	W_CONTROL_CLASS(clazz)->request_layout = _w_shell_request_layout;
	W_CONTROL_CLASS(clazz)->get_visible = _w_shell_get_visible;
	W_CONTROL_CLASS(clazz)->get_region = _w_shell_get_region;
	W_CONTROL_CLASS(clazz)->print = _w_shell_print;
	W_CONTROL_CLASS(clazz)->set_enabled = _w_shell_set_enabled;
	W_CONTROL_CLASS(clazz)->set_region = _w_shell_set_region;
	W_CONTROL_CLASS(clazz)->set_visible = _w_shell_set_visible;
	W_CONTROL_CLASS(clazz)->get_bounds = _w_shell_get_bounds;
	W_CONTROL_CLASS(clazz)->is_reparentable = _w_shell_is_reparentable;
	clazz->close = _w_shell_close;
	clazz->get_toolbar = _w_shell_get_toolbar;
	clazz->get_alpha = _w_shell_get_alpha;
	clazz->get_full_screen = _w_shell_get_full_screen;
	clazz->get_minimum_size = _w_shell_get_minimum_size;
	clazz->get_modified = _w_shell_get_modified;
	clazz->get_ime_input_mode = _w_shell_get_ime_input_mode;
	clazz->get_shells = _w_shell_get_shells;
	clazz->open = _w_shell_open;
	clazz->set_active = _w_shell_set_active;
	clazz->set_alpha = _w_shell_set_alpha;
	clazz->set_full_screen = _w_shell_set_full_screen;
	clazz->set_ime_input_mode = _w_shell_set_ime_input_mode;
	clazz->set_minimum_size = _w_shell_set_minimum_size;
	clazz->set_modified = _w_shell_set_modified;
	clazz->force_active = _w_shell_force_active;
	clazz->get_default_button = _w_shell_get_default_button;
	clazz->get_images = _w_shell_get_images;
	clazz->get_maximized = _w_shell_get_maximized;
	clazz->get_menu_bar = _w_shell_get_menu_bar;
	clazz->get_minimized = _w_shell_get_minimized;
	clazz->get_text = _w_shell_get_text;
	clazz->set_default_button = _w_shell_set_default_button;
	clazz->set_images = _w_shell_set_images;
	clazz->set_maximized = _w_shell_set_maximized;
	clazz->set_menu_bar = _w_shell_set_menu_bar;
	clazz->set_minimized = _w_shell_set_minimized;
	clazz->set_text = _w_shell_set_text;
	/*
	 * reserved
	 */
	_w_shell_reserved *reserved = _W_SHELL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hppp;
	_W_WIDGET_RESERVED(reserved)->compute_trim = _w_shell_compute_trim;
	_W_WIDGET_RESERVED(reserved)->check_open = _w_shell_check_open;
	_W_WIDGET_RESERVED(reserved)->create_handle = _w_shell_create_handle;
	_W_WIDGET_RESERVED(reserved)->hook_events = _w_shell_hook_events;
	_W_WIDGET_RESERVED(reserved)->check_style = _w_shell_check_style;
	_W_CONTROL_RESERVED(reserved)->fixedHandle = _w_widget_h0;
	_W_CONTROL_RESERVED(reserved)->set_bounds = _w_shell_set_bounds_0;
	_W_CONTROL_RESERVED(reserved)->check_border = _w_shell_check_border;
	_W_CONTROL_RESERVED(reserved)->find_background_control =
			_w_shell_find_background_control;
	_W_CONTROL_RESERVED(reserved)->force_resize = _w_shell_force_resize;
	_W_CONTROL_RESERVED(reserved)->child_style = _w_shell_child_style;
	_W_CONTROL_RESERVED(reserved)->fix_style_0 = _w_shell_fix_style_0;
	_W_CONTROL_RESERVED(reserved)->set_cursor = _w_shell_set_cursor;
	_W_CONTROL_RESERVED(reserved)->set_initial_bounds =
			_w_shell_set_initial_bounds;
	_W_CONTROL_RESERVED(reserved)->set_relations = _w_shell_set_relations;
	_W_CONTROL_RESERVED(reserved)->set_ZOrder = _w_shell_set_ZOrder;
	_W_CONTROL_RESERVED(reserved)->show_widget = _w_shell_show_widget;
	_W_CONTROL_RESERVED(reserved)->traverse_escape = _w_shell_traverse_escape;
	_W_CONTROL_RESERVED(reserved)->get_window_origin =
			_w_shell_get_window_origin;
	_W_CONTROL_RESERVED(reserved)->compute_tab_group =
			_w_shell_compute_tab_group;
	_W_CONTROL_RESERVED(reserved)->compute_tab_root = _w_shell_compute_tab_root;
	_W_CONTROL_RESERVED(reserved)->is_tab_group = _w_shell_is_tab_group;
	_W_CONTROL_RESERVED(reserved)->is_tab_item = _w_shell_is_tab_item;
	_W_CONTROL_RESERVED(reserved)->set_orientation = _w_shell_set_orientation;
	_W_CONTROL_RESERVED(reserved)->traverse_item = _w_shell_traverse_item;
	_W_CONTROL_RESERVED(reserved)->traverse_return = _w_shell_traverse_return;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle = _w_widget_hp;
	_W_COMPOSITE_RESERVED(reserved)->has_border = _w_shell_has_border;
	_W_COMPOSITE_RESERVED(reserved)->find_deferred_control =
			_w_shell_find_deferred_control;
	/*
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_RESERVED(reserved)->signals;
	signals[SIGNAL_DESTROY] = _gtk_shell_destroy;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_shell_button_press_event;
	signals[SIGNAL_CONFIGURE_EVENT] = _gtk_shell_configure_event;
	signals[SIGNAL_DELETE_EVENT] = _gtk_shell_delete_event;
	signals[SIGNAL_ENTER_NOTIFY_EVENT] = _gtk_shell_enter_notify_event;
	signals[SIGNAL_DRAW] = _gtk_shell_draw;
	signals[SIGNAL_FOCUS] = _gtk_shell_focus;
	signals[SIGNAL_FOCUS_IN_EVENT] = _gtk_shell_focus_in_event;
	signals[SIGNAL_FOCUS_OUT_EVENT] = _gtk_shell_focus_out_event;
	signals[SIGNAL_LEAVE_NOTIFY_EVENT] = _gtk_shell_leave_notify_event;
	signals[SIGNAL_MOVE_FOCUS] = _gtk_shell_move_focus;
	signals[SIGNAL_MOTION_NOTIFY_EVENT] = _gtk_shell_motion_notify_event;
	signals[SIGNAL_KEY_PRESS_EVENT] = _gtk_shell_key_press_event;
	signals[SIGNAL_SIZE_ALLOCATE] = _gtk_shell_size_allocate;
	signals[SIGNAL_REALIZE] = _gtk_shell_realize;
	signals[SIGNAL_WINDOW_STATE_EVENT] = _gtk_shell_window_state_event;
}
#endif
