/*
 * scrollable.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "scrollable.h"
#include "toolkit.h"
/**
 * Disable warning deprecated
 */
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
/*
 * ScrollBar
 */
GtkWidget* _w_scrollbar_get_handle(w_scrollbar *scrollbar) {
	if (_W_SCROLLBAR(scrollbar)->style == W_HSCROLL) {
		return gtk_scrolled_window_get_hscrollbar(
				GTK_SCROLLED_WINDOW(_W_SCROLLBAR(scrollbar)->scrolledHandle));
	} else {
		return gtk_scrolled_window_get_vscrollbar(
				GTK_SCROLLED_WINDOW(_W_SCROLLBAR(scrollbar)->scrolledHandle));
	}
}
GtkAdjustment* _w_scrollbar_get_adjustment(w_scrollbar *scrollbar) {
	if (_W_SCROLLBAR(scrollbar)->style == W_HSCROLL) {
		return gtk_scrolled_window_get_hadjustment(
				GTK_SCROLLED_WINDOW(_W_SCROLLBAR(scrollbar)->scrolledHandle));
	} else {
		return gtk_scrolled_window_get_vadjustment(
				GTK_SCROLLED_WINDOW(_W_SCROLLBAR(scrollbar)->scrolledHandle));
	}
}
void _w_scrollbar_copy(w_widgetdata *from, w_widgetdata *to) {
	memcpy(to, from, sizeof(_w_scrollbar));
}

wresult _w_scrollbar_get_enabled(w_scrollbar *scrollbar) {
	GtkWidget *handle = _w_scrollbar_get_handle(scrollbar);
	return gtk_widget_get_sensitive(handle);
}
wresult _w_scrollbar_get_increment(w_scrollbar *scrollbar) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	return (int) gtk_adjustment_get_step_increment(adjustmentHandle);
}
wresult _w_scrollbar_get_maximum(w_scrollbar *scrollbar) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	return (int) gtk_adjustment_get_upper(adjustmentHandle);
}
wresult _w_scrollbar_get_minimum(w_scrollbar *scrollbar) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	return (int) gtk_adjustment_get_lower(adjustmentHandle);
}
wresult _w_scrollbar_get_page_increment(w_scrollbar *scrollbar) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	return (int) gtk_adjustment_get_page_increment(adjustmentHandle);
}
wresult _w_scrollbar_get_parent(w_scrollbar *scrollbar, w_scrollable **parent) {
	*parent = _W_SCROLLBAR(scrollbar)->parent;
	return W_TRUE;
}
wresult _w_scrollbar_get_selection(w_scrollbar *scrollbar) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	return (int) gtk_adjustment_get_value(adjustmentHandle);
}
wresult _w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size) {
	GtkWidget *handle = _w_scrollbar_get_handle(scrollbar);
	GtkRequisition requisition;
	gtk_widget_get_preferred_size(handle, &requisition,
	NULL);
	size->width = requisition.width;
	size->height = requisition.height;
	return W_TRUE;
}
wresult _w_scrollbar_get_thumb(w_scrollbar *scrollbar) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	return (int) gtk_adjustment_get_page_size(adjustmentHandle);
}
wresult _w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect) {
	GtkWidget *handle = _w_scrollbar_get_handle(scrollbar);
	gint slider_start = 0, slider_end = 0;
	gtk_range_get_slider_range(GTK_RANGE(handle), &slider_start, &slider_end);
	GtkAllocation allocation;
	gtk_widget_get_allocation(handle, &allocation);
	if ((_W_SCROLLBAR(scrollbar)->style & W_VERTICAL) != 0) {
		rect->x = allocation.x;
		rect->y = slider_start;
		rect->width = allocation.width;
		rect->height = slider_end - slider_start;
	} else {
		rect->x = slider_start;
		rect->y = allocation.y;
		rect->width = slider_end - slider_start;
		rect->height = allocation.height;
	}
	w_scrollable *parent = _W_SCROLLBAR(scrollbar)->parent;
	_w_scrollable_reserved *reserved = _W_SCROLLABLE_GET_RESERVED(parent);
	GtkWidget *scrolledHandle = reserved->scrolledHandle(W_WIDGET(parent),
			_W_WIDGET_RESERVED(reserved));
	gint origin_x = 0, origin_y = 0;
	GdkWindow *window = gtk_widget_get_window(scrolledHandle);
	if (window != 0)
		gdk_window_get_origin(window, &origin_x, &origin_y);
	rect->x += origin_x;
	rect->y += origin_y;
	window = gtk_widget_get_window(_W_WIDGET(parent)->handle);
	if (window != 0)
		gdk_window_get_origin(window, &origin_x, &origin_y);
	rect->x -= origin_x;
	rect->y -= origin_y;
	return W_TRUE;
}
wresult _w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar,
		w_rect *rect) {
	GtkWidget *handle = _w_scrollbar_get_handle(scrollbar);
	rect->x = 0;
	rect->y = 0;
	gint has_stepper = 0;
	gtk_widget_style_get(handle, "has-backward-stepper", &has_stepper, NULL);
	wbool hasB = has_stepper != 0;
	gtk_widget_style_get(handle, "has-secondary-backward-stepper", &has_stepper,
	NULL);
	wbool hasB2 = has_stepper != 0;
	gtk_widget_style_get(handle, "has-forward-stepper", &has_stepper, NULL);
	wbool hasF = has_stepper != 0;
	gtk_widget_style_get(handle, "has-secondary-forward-stepper", &has_stepper,
	NULL);
	wbool hasF2 = has_stepper != 0;
	GtkAllocation allocation;
	gtk_widget_get_allocation(handle, &allocation);
	if ((_W_SCROLLBAR(scrollbar)->style & W_VERTICAL) != 0) {
		int stepperSize = allocation.width;
		rect->x = allocation.x;
		if (hasB)
			rect->y += stepperSize;
		if (hasF2)
			rect->y += stepperSize;
		rect->width = allocation.width;
		rect->height = allocation.height - rect->y;
		if (hasB2)
			rect->height -= stepperSize;
		if (hasF)
			rect->height -= stepperSize;
		if (rect->height < 0) {
			gint slider_start = 0, slider_end = 0;
			gtk_range_get_slider_range(GTK_RANGE(handle), &slider_start,
					&slider_end);
			rect->y = slider_start;
			rect->height = 0;
		}
	} else {
		int stepperSize = allocation.height;
		if (hasB)
			rect->x += stepperSize;
		if (hasF2)
			rect->x += stepperSize;
		rect->y = allocation.y;
		rect->width = allocation.width - rect->x;
		if (hasB2)
			rect->width -= stepperSize;
		if (hasF)
			rect->width -= stepperSize;
		rect->height = allocation.height;
		if (rect->width < 0) {
			gint slider_start = 0, slider_end = 0;
			gtk_range_get_slider_range(GTK_RANGE(handle), &slider_start,
					&slider_end);
			rect->x = slider_start;
			rect->width = 0;
		}
	}
	w_scrollable *parent = _W_SCROLLBAR(scrollbar)->parent;
	_w_scrollable_reserved *reserved = _W_SCROLLABLE_GET_RESERVED(parent);
	GtkWidget *scrolledHandle = reserved->scrolledHandle(W_WIDGET(parent),
			_W_WIDGET_RESERVED(reserved));
	gint origin_x = 0, origin_y = 0;
	GdkWindow *window = gtk_widget_get_window(scrolledHandle);
	if (window != 0)
		gdk_window_get_origin(window, &origin_x, &origin_y);
	rect->x += origin_x;
	rect->y += origin_y;
	window = gtk_widget_get_window(_W_WIDGET(parent)->handle);
	if (window != 0)
		gdk_window_get_origin(window, &origin_x, &origin_y);
	rect->x -= origin_x;
	rect->y -= origin_y;
	return W_TRUE;
}
wresult _w_scrollbar_get_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values) {
	return W_TRUE;
}
wresult _w_scrollbar_get_visible(w_scrollbar *scrollbar) {
	GtkWidget *handle = _w_scrollbar_get_handle(scrollbar);
	w_scrollable *parent = _W_SCROLLBAR(scrollbar)->parent;
	_w_scrollable_reserved *reserved = _W_SCROLLABLE_GET_RESERVED(parent);
	GtkWidget *scrolledHandle = reserved->scrolledHandle(W_WIDGET(parent),
			_W_WIDGET_RESERVED(reserved));
	GtkPolicyType hsp = GTK_POLICY_ALWAYS, vsp = GTK_POLICY_ALWAYS;
	gtk_scrolled_window_get_policy(GTK_SCROLLED_WINDOW(scrolledHandle), &hsp,
			&vsp);
	if ((_W_SCROLLBAR(scrollbar)->style & W_HORIZONTAL) != 0) {
		return hsp != GTK_POLICY_NEVER && hsp != GTK_POLICY_EXTERNAL
				&& gtk_widget_get_visible(handle);
	} else {
		return vsp != GTK_POLICY_NEVER && vsp != GTK_POLICY_EXTERNAL
				&& gtk_widget_get_visible(handle);
	}
}
wresult _w_scrollbar_is_enabled(w_scrollbar *scrollbar) {
	if (_w_scrollbar_get_enabled(scrollbar) > 0) {
		w_scrollable *parent = _W_SCROLLBAR(scrollbar)->parent;
		return W_CONTROL_GET_CLASS(parent)->get_enabled(W_CONTROL(parent));
	}
	return W_FALSE;
}
wresult _w_scrollbar_is_visible(w_scrollbar *scrollbar) {
	GtkWidget *handle = _w_scrollbar_get_handle(scrollbar);
	if (_w_scrollbar_get_visible(scrollbar) > 0) {
		w_scrollable *parent = _W_SCROLLBAR(scrollbar)->parent;
		return W_CONTROL_GET_CLASS(parent)->is_visible(W_CONTROL(parent));
	}
	return W_FALSE;
}
wresult _w_scrollbar_set_enabled(w_scrollbar *scrollbar, wbool enabled) {
	GtkWidget *handle = _w_scrollbar_get_handle(scrollbar);
	gtk_widget_set_sensitive(handle, enabled);
	return W_TRUE;
}

wresult _w_scrollbar_set_increment(w_scrollbar *scrollbar, int value) {
	if (value < 1)
		return W_FALSE;
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_set_step_increment(adjustmentHandle, value);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_scrollbar_set_maximum(w_scrollbar *scrollbar, int value) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	GtkAdjustment adjustment;
	int minimum = gtk_adjustment_get_lower(adjustmentHandle);
	if (value <= minimum)
		return W_FALSE;
	int page_size = gtk_adjustment_get_page_size(adjustmentHandle);
	int _value = gtk_adjustment_get_value(adjustmentHandle);
	int upper = value;
	page_size = w_min(page_size, value - minimum);
	_value = w_min((int) _value, (int) (value - page_size));
	int step_increment = gtk_adjustment_get_step_increment(adjustmentHandle);
	int page_increment = gtk_adjustment_get_page_increment(adjustmentHandle);
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_configure(adjustmentHandle, _value, minimum, upper,
			step_increment, page_increment, page_size);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_scrollbar_set_minimum(w_scrollbar *scrollbar, int value) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	if (value < 0)
		return W_FALSE;
	int maximum = (int) gtk_adjustment_get_upper(adjustmentHandle);
	if (value >= maximum)
		return W_FALSE;
	double adjustment_lower = value;
	double adjustment_page_size = w_min(
			(int) gtk_adjustment_get_page_size(adjustmentHandle),
			maximum - value);
	double adjustment_value = w_max(
			(int) gtk_adjustment_get_value(adjustmentHandle), value);
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_configure(adjustmentHandle, adjustment_value,
			adjustment_lower, maximum,
			gtk_adjustment_get_step_increment(adjustmentHandle),
			gtk_adjustment_get_page_increment(adjustmentHandle),
			adjustment_page_size);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_scrollbar_set_page_increment(w_scrollbar *scrollbar, int value) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	if (value < 1)
		return W_FALSE;
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_set_page_increment(adjustmentHandle, value);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	selection = w_min(selection,
			gtk_adjustment_get_upper(adjustmentHandle)
					- gtk_adjustment_get_page_size(adjustmentHandle));
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_set_value(adjustmentHandle, selection);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_scrollbar_set_thumb(w_scrollbar *scrollbar, int value) {
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	if (value < 1)
		return W_FALSE;
	double adjustment_upper = gtk_adjustment_get_upper(adjustmentHandle);
	double adjustment_lower = gtk_adjustment_get_lower(adjustmentHandle);
	value = (int) w_min(value, (int) (adjustment_upper - adjustment_lower));
	double adjustment_page_size = (double) value;
	double adjustment_value = w_min(
			(int) gtk_adjustment_get_value(adjustmentHandle),
			(int) (adjustment_upper - value));
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_configure(adjustmentHandle, adjustment_value,
			adjustment_lower, adjustment_upper,
			gtk_adjustment_get_step_increment(adjustmentHandle),
			gtk_adjustment_get_page_increment(adjustmentHandle),
			adjustment_page_size);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_scrollbar_set_values(w_scrollbar *scrollbar,
		w_scrollbar_value *values) {
	if (values->minimum < 0)
		return W_FALSE;
	if (values->maximum < 0)
		return W_FALSE;
	if (values->thumb < 1)
		return W_FALSE;
	if (values->increment < 1)
		return W_FALSE;
	if (values->pageIncrement < 1)
		return W_FALSE;
	GtkAdjustment *adjustmentHandle = _w_scrollbar_get_adjustment(scrollbar);
	int thumb = w_min(values->thumb, values->maximum - values->minimum);
	g_signal_handlers_block_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0, 0,
			0, 0, (void*) SIGNAL_VALUE_CHANGED);
	gtk_adjustment_configure(adjustmentHandle,
			w_min(w_max(values->selection, values->minimum),
					values->maximum - thumb), values->minimum, values->maximum,
			values->increment, values->pageIncrement, thumb);
	gtk_adjustment_value_changed(adjustmentHandle);
	g_signal_handlers_unblock_matched(adjustmentHandle, G_SIGNAL_MATCH_DATA, 0,
			0, 0, 0, (void*) SIGNAL_VALUE_CHANGED);
	return W_TRUE;
}
wresult _w_scrollbar_set_visible_0(w_scrollbar *scrollbar, wbool visible) {
	w_scrollable *scrollable = _W_SCROLLBAR(scrollbar)->parent;
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(scrollable));
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(scrollable),
			reserved);
	if (scrolledHandle == 0)
		return W_FALSE;
	GtkPolicyType hsp, vsp;
	gtk_scrolled_window_get_policy(GTK_SCROLLED_WINDOW(scrolledHandle), &hsp,
			&vsp);
	int policy = visible ? GTK_POLICY_ALWAYS : GTK_POLICY_NEVER;
	if (GTK_VERSION >= VERSION(3, 16, 0) && !visible) {
		policy = GTK_POLICY_EXTERNAL;
	}
	if ((_W_SCROLLBAR(scrollbar)->style & W_HSCROLL) != 0) {
		if (hsp == policy)
			return W_FALSE;
		hsp = policy;
	} else {
		if (vsp == policy)
			return W_FALSE;
		vsp = policy;
	}
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle), hsp,
			vsp);
	return W_TRUE;
}
wresult _w_scrollbar_set_visible(w_scrollbar *scrollbar, wbool visible) {
	if (_w_scrollbar_set_visible_0(scrollbar, visible)) {
		/*sendEvent (visible ? SWT.Show : SWT.Hide);
		 parent.sendEvent (SWT.Resize);*/
	}
	return W_TRUE;
}
/*
 * scrollable
 */
wresult _w_scrollable_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved) {
	GtkWidget *fixedHandle = _W_CONTROL_RESERVED(reserved)->fixedHandle(widget,
			_W_WIDGET_RESERVED(reserved));
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(widget),
			_W_WIDGET_RESERVED(reserved));
	int border = 0;
	if (fixedHandle != 0)
		border += gtk_container_get_border_width(GTK_CONTAINER(fixedHandle));
	if (scrolledHandle != 0)
		border += gtk_container_get_border_width(GTK_CONTAINER(scrolledHandle));
	e->result->x = e->rect->x - border;
	e->result->y = e->rect->y - border;
	e->result->width = e->rect->width + (border * 2);
	e->result->height = e->rect->height + (border * 2);
	e->result->height += _w_scrollable_hscrollbar_width(W_SCROLLABLE(widget),
			_W_SCROLLABLE_RESERVED(reserved));
	e->result->width += _w_scrollable_vscrollbar_width(W_SCROLLABLE(widget),
			_W_SCROLLABLE_RESERVED(reserved));
	if (scrolledHandle != 0) {
		if (gtk_scrolled_window_get_shadow_type(
				GTK_SCROLLED_WINDOW(scrolledHandle)) != GTK_SHADOW_NONE) {
			w_point thickness;
			_w_control_get_thickness(scrolledHandle, &thickness);
			int xthickness = thickness.x;
			int ythickness = thickness.y;
			e->result->x -= xthickness;
			e->result->y -= ythickness;
			e->result->width += xthickness * 2;
			e->result->height += ythickness * 2;
		}
	}
	return W_TRUE;
}
void _w_scrollable_update_background_mode(w_control *control,
		_w_control_reserved *reserved) {
	_w_control_update_background_mode(control, reserved);
	int applyThemeBackground =
	_W_SCROLLABLE_RESERVED(reserved)->apply_theme_background(
			W_SCROLLABLE(control), _W_SCROLLABLE_RESERVED(reserved));
	switch (applyThemeBackground) {
	case 0:
		_W_WIDGET(control)->state &= ~STATE_THEME_BACKGROUND;
		break;
	case 1:
		_W_WIDGET(control)->state |= STATE_THEME_BACKGROUND;
		break;
	default: /* No change */
		break;
	}
	_w_control_update_background_mode(control, reserved);
}
int _w_scrollable_apply_theme_background(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved) {
	return (_W_CONTROL(scrollable)->backgroundAlpha == 0) ? 1 : 0;
}
wresult _w_scrollable_get_border_width(w_control *control) {
	int border = 0;
	_w_scrollable_reserved *reserved = _W_SCROLLABLE_GET_RESERVED(control);
	GtkWidget *fixedHandle = _W_CONTROL_RESERVED(reserved)->fixedHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	GtkWidget *scrolledHandle = reserved->scrolledHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (fixedHandle != 0)
		border += gtk_container_get_border_width(GTK_CONTAINER(fixedHandle));
	if (scrolledHandle != 0) {
		border += gtk_container_get_border_width(GTK_CONTAINER(scrolledHandle));
		if (gtk_scrolled_window_get_shadow_type(
				GTK_SCROLLED_WINDOW(scrolledHandle)) != GTK_SHADOW_NONE) {
			w_point thickness;
			_w_control_get_thickness(scrolledHandle, &thickness);
			border += thickness.x;
		}
	}
	return border;
}
wresult _w_scrollable_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_widget_reserved *reserved) {
	GtkAllocation allocation;
	_W_CONTROL_RESERVED(reserved)->force_resize(W_CONTROL(widget),
			_W_CONTROL_RESERVED(reserved));
	GtkWidget *clientHandle = _W_CONTROL_RESERVED(reserved)->clientHandle(
			W_WIDGET(widget), _W_WIDGET_RESERVED(reserved));
	gtk_widget_get_allocation(clientHandle, &allocation);
	e->rect->x = allocation.x;
	e->rect->y = allocation.y;
	e->rect->width = allocation.width;
	e->rect->height = allocation.height;
	return W_TRUE;
}
wresult _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	if (_W_WIDGET(scrollable)->style & W_HSCROLL) {
		_w_widget_reserved *reserved = _w_widget_get_reserved(
				W_WIDGET(scrollable));
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(scrollable),
				reserved);
		if (scrolledHandle != 0) {
			_W_SCROLLBAR(scrollbar)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					&gtk_toolkit->class_scrollbar);
			_W_SCROLLBAR(scrollbar)->parent = scrollable;
			_W_SCROLLBAR(scrollbar)->scrolledHandle = scrolledHandle;
			_W_SCROLLBAR(scrollbar)->style = W_HSCROLL;
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable) {
	if (GTK_VERSION >= VERSION(3, 16, 0)) {
		_w_widget_reserved *reserved = _w_widget_get_reserved(
				W_WIDGET(scrollable));
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(scrollable),
				reserved);
		if (gtk_scrolled_window_get_overlay_scrolling(
				GTK_SCROLLED_WINDOW(scrolledHandle)))
			return W_SCROLLBAR_OVERLAY;
	}
	return W_NONE;
}
wresult _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
	if (_W_WIDGET(scrollable)->style & W_VSCROLL) {
		_w_widget_reserved *reserved = _w_widget_get_reserved(
				W_WIDGET(scrollable));
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(scrollable),
				reserved);
		if (scrolledHandle != 0) {
			_W_SCROLLBAR(scrollbar)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					&gtk_toolkit->class_scrollbar);
			_W_SCROLLBAR(scrollbar)->parent = scrollable;
			_W_SCROLLBAR(scrollbar)->scrolledHandle = scrolledHandle;
			_W_SCROLLBAR(scrollbar)->style = W_VSCROLL;
			return W_TRUE;
		}
	}
	return W_FALSE;
}
int _w_scrollable_hscrollbar_width(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved) {
	GtkWidget *scrolledHandle = reserved->scrolledHandle(W_WIDGET(scrollable),
			_W_WIDGET_RESERVED(reserved));
	if (scrolledHandle != 0) {
		GtkWidget *hBarHandle = 0;
		hBarHandle = gtk_scrolled_window_get_hscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		if (hBarHandle == 0)
			return 0;
		GtkRequisition requisition;
#if GTK3
		gtk_widget_get_preferred_size(hBarHandle, &requisition, NULL);
#else
	gtk_widget_size_request(hBarHandle, &requisition);
#endif
		int padding;
		gtk_widget_style_get(scrolledHandle, "scrollbar-spacing", &padding,
		NULL);
		int spacing = padding;
		return requisition.height + spacing;
	} else
		return 0;
}
wresult _w_scrollable_send_leave_notify(w_control *control,
		_w_control_reserved *reserved) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	return scrolledHandle != 0;
}
void _w_scrollable_set_orientation(w_control *control, wbool create,
		_w_control_reserved *reserved) {
	_w_control_set_orientation_0(control, create, reserved);
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if ((_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0 || !create) {
		GtkTextDirection dir =
				(_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0 ?
						GTK_TEXT_DIR_RTL : GTK_TEXT_DIR_LTR;
		if (scrolledHandle != 0) {
			gtk_widget_set_direction(scrolledHandle, dir);
		}
	}
	if ((_W_WIDGET(control)->style & W_MIRRORED) != 0 || !create) {
		if ((_W_WIDGET(control)->state & STATE_CANVAS) != 0) {
			if ((_W_WIDGET(control)->style & W_HSCROLL) != 0) {
				GtkWidget *handle = gtk_scrolled_window_get_hscrollbar(
						GTK_SCROLLED_WINDOW(scrolledHandle));
				gtk_range_set_inverted(GTK_RANGE(handle),
						(_W_WIDGET(control)->style & W_RIGHT_TO_LEFT) != 0);
			}
		}
	}
}
int _w_scrollable_set_scrollbar_visible_0(w_widget *scrollable, int style,
		int visible, _w_widget_reserved *reserved) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(scrollable, reserved);
	if (scrolledHandle == 0)
		return W_FALSE;
	GtkPolicyType hsp, vsp;
	gtk_scrolled_window_get_policy(GTK_SCROLLED_WINDOW(scrolledHandle), &hsp,
			&vsp);
	GtkPolicyType policy = visible ? GTK_POLICY_ALWAYS : GTK_POLICY_NEVER;
#if GTK3
	if (gtk_get_minor_version() >= 16 && !visible) {
		policy = GTK_POLICY_EXTERNAL;
	}
#else
	if ( !visible) {
		policy = GTK_POLICY_EXTERNAL;
	}
#endif
	if ((style & W_HORIZONTAL) != 0) {
		if (hsp == policy)
			return W_FALSE;
		hsp = policy;
	} else {
		if (vsp == policy)
			return W_FALSE;
		vsp = policy;
	}
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle), hsp,
			vsp);
	return W_TRUE;
}
void _w_scrollable_redraw_background_image(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved) {
}

void _w_scrollable_redraw_widget(w_control *control, w_rect *_rect, int flags,
		_w_control_reserved *reserved) {
	_w_control_redraw_widget(control, _rect, flags, reserved);
	if (!gtk_widget_get_realized(_W_WIDGET(control)->handle))
		return;
	if (!(flags & REDRAW_TRIM))
		return;
	GtkWidget *topHandle = _W_WIDGET_RESERVED(reserved)->topHandle(
			W_WIDGET(control), _W_WIDGET_RESERVED(reserved));
	GtkWidget *paintHandle = reserved->paintHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (topHandle == paintHandle)
		return;
	GdkWindow *window = gtk_widget_get_window(topHandle);
	GdkRectangle rect;
	if (flags & REDRAW_REDRAWALL) {
		GtkAllocation allocation;
		gtk_widget_get_allocation(topHandle, &allocation);
		rect.width = allocation.width;
		rect.height = allocation.height;
	} else {
		gint destX = 0, destY = 0;
		gtk_widget_translate_coordinates(paintHandle, topHandle, _rect->x,
				_rect->y, &destX, &destY);
		rect.x = destX;
		rect.y = destY;
		rect.width = w_max(0, _rect->width);
		rect.height = w_max(0, _rect->height);
	}
	gdk_window_invalidate_rect(window, &rect, (flags & REDRAW_ALL));
}
void _w_scrollable_resize_handle(w_control *control, w_size *size,
		_w_control_reserved *reserved) {
#if GTK3
	_w_control_resize_handle(control, size, reserved);
	/*GtkWidget *fixedHandle = reserved->fixedHandle(W_WIDGET(control),
	 _W_WIDGET_RESERVED(reserved));
	 if (fixedHandle != 0) {
	 OS.swt_fixed_resize(OS.gtk_widget_get_parent(fixedHandle), fixedHandle,
	 width, height);
	 }
	 GtkWidget *scrolledHandle =
	 _W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_SCROLLABLE(control), reserved);
	 GtkWidget* child = scrolledHandle != 0 ? scrolledHandle :_W_WIDGET(control)->handle;
	 OS.swt_fixed_resize(OS.gtk_widget_get_parent(child), child, width, height);*/
#endif
#if GTK2
#endif
}

void _w_scrollable_show_widget(w_control *control,
		_w_control_reserved *reserved) {
	_w_control_show_widget(control, reserved);
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(W_WIDGET(control),
			_W_WIDGET_RESERVED(reserved));
	if (scrolledHandle != 0)
		gtk_widget_show(scrolledHandle);
}
void _w_scrollable_update_scrollbar_value(w_scrollable *scrollable,
		wuint64 style, _w_scrollable_reserved *reserved) {
	reserved->redraw_background_image(scrollable, reserved);
}
int _w_scrollable_vscrollbar_width(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved) {
	GtkWidget *scrolledHandle = reserved->scrolledHandle(W_WIDGET(scrollable),
			_W_WIDGET_RESERVED(reserved));
	if (scrolledHandle != 0) {
		GtkWidget *vBarHandle = 0;
		vBarHandle = gtk_scrolled_window_get_vscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		if (vBarHandle == 0)
			return 0;
		GtkRequisition requisition;
#if GTK3
		gtk_widget_get_preferred_size(vBarHandle, &requisition, NULL);
#else
	gtk_widget_size_request(vBarHandle, &requisition);
#endif
		int padding;
		gtk_widget_style_get(scrolledHandle, "scrollbar-spacing", &padding,
		NULL);
		int spacing = padding;
		return requisition.width + spacing;
	} else
		return 0;
}
void _w_scrollable_hook_events(w_widget *widget, _w_widget_reserved *reserved) {
	_w_control_hook_events(widget, reserved);
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(widget,
			_W_WIDGET_RESERVED(reserved));
	if (scrolledHandle != 0) {
		GtkWidget *handle = gtk_scrolled_window_get_vscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		GtkAdjustment *adjustmentHandle = gtk_scrolled_window_get_vadjustment(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		_w_widget_connect(handle, SIGNAL_CHANGE_VALUE, 0, FALSE);
		_w_widget_connect((GtkWidget*) adjustmentHandle, SIGNAL_VALUE_CHANGED,
				0, FALSE);
		_w_widget_connect(handle, SIGNAL_EVENT_AFTER, 0, FALSE);
		_w_widget_connect(handle, SIGNAL_BUTTON_PRESS_EVENT, 0, FALSE);
		handle = gtk_scrolled_window_get_hscrollbar(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		adjustmentHandle = gtk_scrolled_window_get_hadjustment(
				GTK_SCROLLED_WINDOW(scrolledHandle));
		_w_widget_connect(handle, SIGNAL_CHANGE_VALUE, 0, FALSE);
		_w_widget_connect((GtkWidget*) adjustmentHandle, SIGNAL_VALUE_CHANGED,
				0, FALSE);
		_w_widget_connect(handle, SIGNAL_EVENT_AFTER, 0, FALSE);
		_w_widget_connect(handle, SIGNAL_BUTTON_PRESS_EVENT, 0, FALSE);
	}
}

/*
 * signals
 */
wuint64 _gtk_scrollbar_style(GtkWidget *scrolledHandle, GtkWidget *adjustment) {
	if (adjustment
			== (GtkWidget*) gtk_scrolled_window_get_hscrollbar(
					GTK_SCROLLED_WINDOW(scrolledHandle))) {
		return W_VSCROLL;
	} else {
		return W_HSCROLL;
	}
}
wuint64 _gtk_scrollbar_style0(GtkWidget *scrolledHandle,
		GtkWidget *adjustment) {
	if (adjustment
			== (GtkWidget*) gtk_scrolled_window_get_hadjustment(
					GTK_SCROLLED_WINDOW(scrolledHandle))) {
		return W_VSCROLL;
	} else {
		return W_HSCROLL;
	}
}
gboolean _gtk_scrollbar_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(widget, reserved);
	wuint64 style = _gtk_scrollbar_style(scrolledHandle, e->widget);
	if (style & W_VSCROLL) {
		_W_SCROLLABLE(widget)->vdetail = GTK_SCROLL_NONE;
		_W_WIDGET(widget)->state &= ~ STATE_VSCROLLBAR_DRAGSEND;
	} else {
		_W_SCROLLABLE(widget)->hdetail = GTK_SCROLL_NONE;
		_W_WIDGET(widget)->state &= ~ STATE_HSCROLLBAR_DRAGSEND;
	}
	return FALSE;
}

gboolean _gtk_scrollbar_change_value(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(widget, reserved);
	wuint64 style = _gtk_scrollbar_style(scrolledHandle, e->widget);
	if (style & W_VSCROLL) {
		_W_SCROLLABLE(widget)->vdetail = (intptr_t) e->args[0];
	} else {
		_W_SCROLLABLE(widget)->hdetail = (intptr_t) e->args[0];
	}
	return FALSE;
}

gboolean _gtk_scrollbar_range_get_slider_range(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved) {
	int *slider_start = e->args[0];
	int *slider_end = e->args[1];
	if (GTK_VERSION >= VERSION(2, 20, 0)) {
		gtk_range_get_slider_range(GTK_RANGE(e->widget), slider_start,
				slider_end);
	} else {
		*slider_start = 0; // GTK_RANGE_SLIDER_START (e->widget);
		*slider_end = 0; //GTK_RANGE_SLIDER_END (e->widget);
	}
	return FALSE;
}

gboolean _gtk_scrollbar_value_changed(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	w_event_scrollbar event;
	w_scrollbar scrollbar;
	memset(&event, 0, sizeof(event));
	GtkWidget *scrolledHandle =
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(widget, reserved);
	wuint64 style = _gtk_scrollbar_style0(scrolledHandle, e->widget);
	int detail;
	gboolean dragSent;
	if (style & W_VSCROLL) {
		event.event.type = W_EVENT_VSCROLL;
		_W_SCROLLBAR(&scrollbar)->style = W_VSCROLL;
		if (_W_SCROLLABLE(widget)->vdetail == GTK_SCROLL_JUMP) {
			_W_WIDGET(widget)->state |= STATE_VSCROLLBAR_DRAGSEND;
			dragSent = TRUE;
		} else {
			_W_WIDGET(widget)->state &= ~STATE_VSCROLLBAR_DRAGSEND;
			dragSent = FALSE;
		}
		detail = _W_SCROLLABLE(widget)->vdetail;
	} else {
		event.event.type = W_EVENT_HSCROLL;
		_W_SCROLLBAR(&scrollbar)->style = W_HSCROLL;
		if (_W_SCROLLABLE(widget)->hdetail == GTK_SCROLL_JUMP) {
			_W_WIDGET(widget)->state |= STATE_HSCROLLBAR_DRAGSEND;
			dragSent = TRUE;
		} else {
			_W_WIDGET(widget)->state &= ~STATE_HSCROLLBAR_DRAGSEND;
			dragSent = FALSE;
		}
		detail = _W_SCROLLABLE(widget)->hdetail;
	}
	_W_SCROLLBAR(&scrollbar)->scrolledHandle = scrolledHandle;
	switch (detail) {
	case GTK_SCROLL_NONE:
		event.detail = W_NONE;
		break;
	case GTK_SCROLL_JUMP:
		event.detail = W_DRAG;
		break;
	case GTK_SCROLL_START:
		event.detail = W_HOME;
		break;
	case GTK_SCROLL_END:
		event.detail = W_END;
		break;
	case GTK_SCROLL_PAGE_DOWN:
	case GTK_SCROLL_PAGE_RIGHT:
	case GTK_SCROLL_PAGE_FORWARD:
		event.detail = W_PAGE_DOWN;
		break;
	case GTK_SCROLL_PAGE_UP:
	case GTK_SCROLL_PAGE_LEFT:
	case GTK_SCROLL_PAGE_BACKWARD:
		event.detail = W_PAGE_UP;
		break;
	case GTK_SCROLL_STEP_DOWN:
	case GTK_SCROLL_STEP_RIGHT:
	case GTK_SCROLL_STEP_FORWARD:
		event.detail = W_ARROW_DOWN;
		break;
	case GTK_SCROLL_STEP_UP:
	case GTK_SCROLL_STEP_LEFT:
	case GTK_SCROLL_STEP_BACKWARD:
		event.detail = W_ARROW_UP;
		break;
	}
	if (!dragSent)
		detail = GTK_SCROLL_NONE;
	event.event.platform_event = (w_event_platform*) e;
	event.event.widget = widget;
	event.scrollbar = &scrollbar;
	_W_SCROLLBAR(&scrollbar)->parent = W_SCROLLABLE(widget);
	_W_SCROLLBAR(&scrollbar)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			&gtk_toolkit->class_scrollbar);
	_w_widget_send_event(widget, (w_event*) &event);
	_W_SCROLLABLE_RESERVED(reserved)->update_scrollbar_value(
			W_SCROLLABLE(widget), style, _W_SCROLLABLE_RESERVED(reserved));
#if GTK3
	gtk_widget_queue_draw(_W_WIDGET(widget)->handle);
#endif
	return 0;
}

gboolean _gtk_scrollbar_event_after(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	w_event_scrollbar event;
	w_scrollbar scrollbar;
	GdkEvent *gtkEvent = (GdkEvent*) e->args[0];
	switch (gtkEvent->type) {
	case GDK_BUTTON_RELEASE: {
		GtkWidget *scrolledHandle =
		_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle(widget, reserved);
		wuint64 style = _gtk_scrollbar_style(scrolledHandle, e->widget);
		GdkEventButton *gdkEventButton = (GdkEventButton*) gtkEvent;
		int detail;
		wbool dragSent;
		if (style & W_VSCROLL) {
			detail = _W_SCROLLABLE(widget)->vdetail;
			dragSent = _W_WIDGET(widget)->state & STATE_VSCROLLBAR_DRAGSEND;
		} else {
			detail = _W_SCROLLABLE(widget)->hdetail;
			dragSent = _W_WIDGET(widget)->state & STATE_HSCROLLBAR_DRAGSEND;
		}
		if (gdkEventButton->button == 1 && detail == GTK_SCROLL_JUMP) {
			if (style & W_VSCROLL) {
				event.event.type = W_EVENT_VSCROLL;
				_W_SCROLLBAR(&scrollbar)->style = W_VSCROLL;
			} else {
				event.event.type = W_EVENT_HSCROLL;
				_W_SCROLLBAR(&scrollbar)->style = W_HSCROLL;
			}
			_W_SCROLLBAR(&scrollbar)->scrolledHandle = scrolledHandle;
			_W_SCROLLBAR(&scrollbar)->parent = W_SCROLLABLE(widget);
			_W_SCROLLBAR(&scrollbar)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					&gtk_toolkit->class_scrollbar);
			event.event.widget = widget;
			event.event.time = 0;
			event.event.data = 0;
			event.event.platform_event = (w_event_platform*) e;
			event.scrollbar = &scrollbar;
			if (!dragSent) {
				event.detail = W_DRAG;
				_w_widget_send_event(widget, (w_event*) &event);
			}
			if (style & W_VSCROLL) {
				event.event.type = W_EVENT_VSCROLL;
			} else {
				event.event.type = W_EVENT_HSCROLL;
			}
			event.event.widget = widget;
			event.event.time = 0;
			event.event.data = 0;
			event.event.platform_event = (w_event_platform*) e;
			event.scrollbar = &scrollbar;
			event.detail = 0;
			_w_widget_send_event(widget, (w_event*) &event);
		}
		if (style & W_VSCROLL) {
			_W_SCROLLABLE(widget)->vdetail = GTK_SCROLL_NONE;
			_W_WIDGET(widget)->state &= ~STATE_VSCROLLBAR_DRAGSEND;
		} else {
			_W_SCROLLABLE(widget)->hdetail = GTK_SCROLL_NONE;
			_W_WIDGET(widget)->state &= ~STATE_HSCROLLBAR_DRAGSEND;
		}
		break;
	}
	}
	return FALSE;
}
gboolean _gtk_scrollable_button_press_event(w_widget *widget,
		_w_event_platform *e, _w_widget_reserved *reserved) {
	if (GTK_IS_SCROLLBAR(e->widget)) {
		return _gtk_scrollbar_button_press_event(widget, e, reserved);
	} else
		return _gtk_control_button_press_event(widget, e, reserved);
}

gboolean _gtk_scrollable_change_value(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (GTK_IS_SCROLLBAR(e->widget)) {
		return _gtk_scrollbar_change_value(widget, e, reserved);
	} else
		return FALSE;
}

gboolean _gtk_scrollable_value_changed(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (GTK_IS_ADJUSTMENT(e->widget)) {
		return _gtk_scrollbar_value_changed(widget, e, reserved);
	} else {
		return FALSE;
	}
}

gboolean _gtk_scrollable_event_after(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (GTK_IS_SCROLLBAR(e->widget)) {
		return _gtk_scrollbar_event_after(widget, e, reserved);
	} else
		return _gtk_control_event_after(widget, e, reserved);
}

gboolean _gtk_scrollable_scroll_event(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	gboolean result = _gtk_control_scroll_event(widget, e, reserved);

	/*
	 * Feature in GTK.  Scrolled windows do not scroll if the scrollbars
	 * are hidden.  This is not a bug, but is inconsistent with other platforms.
	 * The fix is to set the adjustment values directly.
	 */
	if ((_W_WIDGET(widget)->state & STATE_CANVAS) != 0) {
		w_scrollbar scrollBar;
		w_widgetdata_init(W_WIDGETDATA(&scrollBar));
		GdkEventScroll *gdkEvent = (GdkEventScroll*) e->args[0];
		if (gdkEvent->direction == GDK_SCROLL_SMOOTH) {
			double delta_x = 0, delta_y = 0;
			if (gdk_event_get_scroll_deltas((GdkEvent*) gdkEvent, &delta_x,
					&delta_y)) {
				if (delta_x != 0) {
					_w_scrollable_get_horizontal_bar(W_SCROLLABLE(widget),
							&scrollBar);
					if (w_widgetdata_is_ok(W_WIDGETDATA(&scrollBar))) {
						GtkWidget *handle = _w_scrollbar_get_handle(&scrollBar);
						if (!gtk_widget_get_visible(handle)
								&& _w_scrollbar_get_enabled(&scrollBar) > 0) {
							GtkAdjustment *adjustmentHandle =
									_w_scrollbar_get_adjustment(&scrollBar);
							double page_size = gtk_adjustment_get_page_size(
									adjustmentHandle);
							double delta = pow(page_size, 2.0 / 3.0) * delta_x;
							int value = (int) w_max(
									gtk_adjustment_get_lower(adjustmentHandle),
									w_min(
											gtk_adjustment_get_upper(
													adjustmentHandle)
													- page_size,
											gtk_adjustment_get_value(
													adjustmentHandle) + delta));
							gtk_adjustment_set_value(adjustmentHandle, value);
							result = TRUE;
						}
					}
				}
				if (delta_y != 0) {
					_w_scrollable_get_vertical_bar(W_SCROLLABLE(widget),
							&scrollBar);
					if (w_widgetdata_is_ok(W_WIDGETDATA(&scrollBar))) {
						GtkWidget *handle = _w_scrollbar_get_handle(&scrollBar);
						if (!gtk_widget_get_visible(handle)
								&& _w_scrollbar_get_enabled(&scrollBar) > 0) {
							GtkAdjustment *adjustmentHandle =
									_w_scrollbar_get_adjustment(&scrollBar);
							double page_size = gtk_adjustment_get_page_size(
									adjustmentHandle);
							double delta = pow(page_size, 2.0 / 3.0) * delta_x;
							int value = (int) w_max(
									gtk_adjustment_get_lower(adjustmentHandle),
									w_min(
											gtk_adjustment_get_upper(
													adjustmentHandle)
													- page_size,
											gtk_adjustment_get_value(
													adjustmentHandle) + delta));
							gtk_adjustment_set_value(adjustmentHandle, value);
							result = TRUE;
						}
					}
				}
			}
		} else {
			if (gdkEvent->direction == GDK_SCROLL_UP
					|| gdkEvent->direction == GDK_SCROLL_DOWN) {
				_w_scrollable_get_vertical_bar(W_SCROLLABLE(widget),
						&scrollBar);
			} else {
				_w_scrollable_get_horizontal_bar(W_SCROLLABLE(widget),
						&scrollBar);
			}
			if (w_widgetdata_is_ok(W_WIDGETDATA(&scrollBar))) {
				GtkWidget *handle = _w_scrollbar_get_handle(&scrollBar);
				if (!gtk_widget_get_visible(handle)
						&& _w_scrollbar_get_enabled(&scrollBar) > 0) {
					GtkAdjustment *adjustmentHandle =
							_w_scrollbar_get_adjustment(&scrollBar);
					/* Calculate wheel delta to match GTK+ 2.4 and higher */
					double page_size = gtk_adjustment_get_page_size(
							adjustmentHandle);
					int wheel_delta = (int) pow(page_size, 2.0 / 3.0);
					if (gdkEvent->direction == GDK_SCROLL_UP
							|| gdkEvent->direction == GDK_SCROLL_LEFT)
						wheel_delta = -wheel_delta;
					int value = (int) w_max(
							gtk_adjustment_get_lower(adjustmentHandle),
							w_min(
									gtk_adjustment_get_upper(adjustmentHandle)
											- page_size,
									gtk_adjustment_get_value(adjustmentHandle)
											+ wheel_delta));
					gtk_adjustment_set_value(adjustmentHandle, value);
					result = TRUE;
				}
			}
		}
		w_widgetdata_close(W_WIDGETDATA(&scrollBar));
	}
	return result;
}
void _w_scrollable_class_init(struct _w_scrollable_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	/*
	 * scrollable class
	 */
	clazz->get_horizontal_bar = _w_scrollable_get_horizontal_bar;
	clazz->get_scrollbars_mode = _w_scrollable_get_scrollbars_mode;
	clazz->get_vertical_bar = _w_scrollable_get_vertical_bar;
	W_CONTROL_CLASS(clazz)->get_border_width = _w_scrollable_get_border_width;
	/*
	 * scrollbar class
	 */
	clazz->class_scrollbar = &gtk_toolkit->class_scrollbar;
	if (gtk_toolkit->class_scrollbar.get_values == 0) {
		struct _w_scrollbar_class *scrollbar = &gtk_toolkit->class_scrollbar;
		scrollbar->widgetdata.toolkit = (w_toolkit*) gtk_toolkit;
		scrollbar->widgetdata.is_ok = _w_widgetdata_is_ok;
		scrollbar->widgetdata.close = _w_widgetdata_close;
		scrollbar->widgetdata.copy = _w_scrollbar_copy;
		scrollbar->get_parent = _w_scrollbar_get_parent;
		scrollbar->get_enabled = _w_scrollbar_get_enabled;
		scrollbar->is_enabled = _w_scrollbar_is_enabled;
		scrollbar->set_enabled = _w_scrollbar_set_enabled;
		scrollbar->get_visible = _w_scrollbar_get_visible;
		scrollbar->is_visible = _w_scrollbar_is_visible;
		scrollbar->set_visible = _w_scrollbar_set_visible;
		scrollbar->get_values = _w_scrollbar_get_values;
		scrollbar->get_increment = _w_scrollbar_get_increment;
		scrollbar->get_maximum = _w_scrollbar_get_maximum;
		scrollbar->get_minimum = _w_scrollbar_get_minimum;
		scrollbar->get_page_increment = _w_scrollbar_get_page_increment;
		scrollbar->get_selection = _w_scrollbar_get_selection;
		scrollbar->get_thumb = _w_scrollbar_get_thumb;
		scrollbar->set_values = _w_scrollbar_set_values;
		scrollbar->set_increment = _w_scrollbar_set_increment;
		scrollbar->set_maximum = _w_scrollbar_set_maximum;
		scrollbar->set_minimum = _w_scrollbar_set_minimum;
		scrollbar->set_page_increment = _w_scrollbar_set_page_increment;
		scrollbar->set_selection = _w_scrollbar_set_selection;
		scrollbar->set_thumb = _w_scrollbar_set_thumb;
		scrollbar->get_size = _w_scrollbar_get_size;
		scrollbar->get_thumb_bounds = _w_scrollbar_get_thumb_bounds;
		scrollbar->get_thumb_track_bounds = _w_scrollbar_get_thumb_track_bounds;
	}
	/*
	 * reserved
	 */
	_w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->set_scrollbar_visible = _w_scrollable_set_scrollbar_visible_0;
	reserved->apply_theme_background = _w_scrollable_apply_theme_background;
	reserved->redraw_background_image = _w_scrollable_redraw_background_image;
	reserved->update_scrollbar_value = _w_scrollable_update_scrollbar_value;
	_W_WIDGET_RESERVED(reserved)->hook_events = _w_scrollable_hook_events;
	_W_WIDGET_RESERVED(reserved)->compute_trim = _w_scrollable_compute_trim;
	_W_WIDGET_RESERVED(reserved)->get_client_area =
			_w_scrollable_get_client_area;
	_W_CONTROL_RESERVED(reserved)->clientHandle = _w_widget_h;
	_W_CONTROL_RESERVED(reserved)->update_background_mode =
			_w_scrollable_update_background_mode;
	_W_CONTROL_RESERVED(reserved)->set_orientation =
			_w_scrollable_set_orientation;
	_W_CONTROL_RESERVED(reserved)->send_leave_notify =
			_w_scrollable_send_leave_notify;
	_W_CONTROL_RESERVED(reserved)->redraw_widget = _w_scrollable_redraw_widget;
	_W_CONTROL_RESERVED(reserved)->resize_handle = _w_scrollable_resize_handle;
	_W_CONTROL_RESERVED(reserved)->show_widget = _w_scrollable_show_widget;
	/**
	 * signals
	 */
	_gtk_signal *signals = _W_WIDGET_RESERVED(reserved)->signals;
	signals[SIGNAL_BUTTON_PRESS_EVENT] = _gtk_scrollable_button_press_event;
	signals[SIGNAL_CHANGE_VALUE] = _gtk_scrollable_change_value;
	signals[SIGNAL_VALUE_CHANGED] = _gtk_scrollable_value_changed;
	signals[SIGNAL_EVENT_AFTER] = _gtk_scrollable_event_after;
	signals[SIGNAL_SCROLL_EVENT] = _gtk_scrollable_scroll_event;

}
#endif

