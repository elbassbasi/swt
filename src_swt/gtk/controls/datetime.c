/*
 * datetime.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "datetime.h"
#include "../widgets/toolkit.h"
#define MIN_YEAR 1752 // Gregorian switchover in North America: September 19, 1752
#define MAX_YEAR 9999
#define SPACE_FOR_CURSOR 1
#define GTK2_MANUAL_BORDER_PADDING 2
int _w_datetime_check_style(int style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);
#if GTK2
	//Workaround. Right_to_left is buggy on gtk2. Only allow on gtk3 onwards
	if (isDateWithDropDownButton (style)) {
		style &= ~(SWT.RIGHT_TO_LEFT);
	}
#endif

	style = _w_widget_check_bits(style, W_DATE, W_TIME, W_CALENDAR, 0, 0, 0);
	if ((style & W_DATE) == 0)
		style &= ~W_DROP_DOWN;
	return _w_widget_check_bits(style, W_MEDIUM, W_SHORT, W_LONG, 0, 0, 0);
}
int _w_datetime_get_day(w_datetime *datetime) {
}
int _w_datetime_get_hours(w_datetime *datetime) {
}
int _w_datetime_get_minutes(w_datetime *datetime) {
}
int _w_datetime_get_month(w_datetime *datetime) {
}
int _w_datetime_get_seconds(w_datetime *datetime) {
}
int _w_datetime_get_year(w_datetime *datetime) {
}
wresult _w_datetime_set_date(w_datetime *datetime, int year, int month,
		int day) {
}
wresult _w_datetime_set_day(w_datetime *datetime, int day) {
}
wresult _w_datetime_set_hours(w_datetime *datetime, int hours) {
}
wresult _w_datetime_set_minutes(w_datetime *datetime, int minutes) {
}
wresult _w_datetime_set_month(w_datetime *datetime, int month) {
}
wresult _w_datetime_set_seconds(w_datetime *datetime, int seconds) {
}
wresult _w_datetime_set_time(w_datetime *datetime, int hours, int minutes,
		int seconds) {
}
wresult _w_datetime_set_year(w_datetime *datetime, int year) {
}
void textEntryHandle_Callback(GtkWidget *widget, gpointer data) {
	*((GtkWidget**) data) = widget;
}
GtkWidget* _w_datetime_textEntryHandle(w_widget *widget) {
	if (_W_WIDGET(widget)->style & W_CALENDAR) {
		return 0;
	} else {
		if (((_W_WIDGET(widget)->style & W_DROP_DOWN) != 0
				&& (_W_WIDGET(widget)->style & W_DATE) != 0)) {
			GtkWidget *h = 0;
			gtk_container_forall(GTK_CONTAINER(_W_WIDGET(widget)->handle),
					textEntryHandle_Callback, &h);
			return h;
		} else {
			return _W_WIDGET(widget)->handle;
		}
	}
}
wresult _w_datetime_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle = 0, *textEntryHandle = 0, *handle;
	_W_WIDGET(widget)->style = _w_datetime_check_style(style);
	if (_W_WIDGET(widget)->style & W_CALENDAR) {
		_W_WIDGET(widget)->state |= STATE_HANDLE;
		fixedHandle = _w_fixed_new();
		if (fixedHandle == 0)
			goto _err;
		gtk_widget_set_has_window(fixedHandle, TRUE);
		handle = gtk_calendar_new();
		if (handle == 0)
			goto _err;

		//Calenadar becomes container in this case.
		//containerHandle = handle;

		gtk_container_add(GTK_CONTAINER(fixedHandle), handle);

		GtkCalendarDisplayOptions flags = GTK_CALENDAR_SHOW_HEADING
				| GTK_CALENDAR_SHOW_DAY_NAMES;
		/*if ((style & W_CALENDAR_WEEKNUMBERS) != 0) {
		 flags |= GTK_CALENDAR_SHOW_WEEK_NUMBERS;
		 }*/
		gtk_calendar_set_display_options(GTK_CALENDAR(handle), flags);

	} else {
		fixedHandle = _w_fixed_new();
		if (fixedHandle == 0)
			goto _err;
		gtk_widget_set_has_window(fixedHandle, TRUE);
		if (((_W_WIDGET(widget)->style & W_DROP_DOWN) != 0
				&& (_W_WIDGET(widget)->style & W_DATE) != 0)) {
			//Create box to put entry and button into box.
#if GTK3
			handle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
			if (handle == 0)
				goto _err;
			gtk_box_set_homogeneous(GTK_BOX(handle), FALSE);
#else
	vboxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, FALSE, 0);
	if (vboxHandle == 0)
		goto _err;
#endif
			gtk_container_add(GTK_CONTAINER(fixedHandle), handle);

			//Create entry
			textEntryHandle = gtk_entry_new();
			if (textEntryHandle == 0)
				goto _err;
			gtk_container_add(GTK_CONTAINER(handle), textEntryHandle);
			g_object_set_qdata(G_OBJECT(textEntryHandle), gtk_toolkit->quark[0],
					widget);

			//handle = containerHandle;

			// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
			// reset to default font to get the usual behavior
#if GTK3
			//setFontDescription (defaultFont ().handle);
#endif
		} else {
			GtkAdjustment *adjusment = gtk_adjustment_new(0, -9999, 9999, 1, 0,
					0);
			textEntryHandle = gtk_spin_button_new(adjusment, 1, 0);
			if (textEntryHandle == 0)
				goto _err;

			//in this case,the Entry becomes the container.
			handle = textEntryHandle;
			//containerHandle = textEntryHandle;

			gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(textEntryHandle),
			FALSE);
			gtk_container_add(GTK_CONTAINER(fixedHandle), textEntryHandle);
			gtk_spin_button_set_wrap(GTK_SPIN_BUTTON(textEntryHandle),
					(_W_WIDGET(widget)->style & W_WRAP) != 0);
		}
		gtk_editable_set_editable(GTK_EDITABLE(textEntryHandle),
				(_W_WIDGET(widget)->style & W_READ_ONLY) == 0);
		if (GTK_VERSION <= VERSION(3, 20, 0)) {
			gtk_entry_set_has_frame(GTK_ENTRY(textEntryHandle),
					(_W_WIDGET(widget)->style & W_BORDER) != 0);
		}
	}
	if ((_W_WIDGET(widget)->style & W_DATE)
			|| (_W_WIDGET(widget)->style & W_TIME)) {
		/*String property = System.getProperty("swt.datetime.locale");
		 Locale locale;
		 if (property == null || property.isEmpty()) {
		 locale = Locale.getDefault();
		 } else {
		 locale = Locale.forLanguageTag(property);
		 }

		 dateFormat = getFormat(locale, style);
		 dateFormat.setLenient(false);
		 calendar = Calendar.getInstance(locale);
		 updateControl();
		 selectField(updateField(currentField));*/
	}

	if (_W_WIDGET(widget)->style & W_CALENDAR) {
		/*gtk_calendar_mark_day(calendarHandle,
		 Calendar.getInstance().get(Calendar.DAY_OF_MONTH));*/
	}

	if (((_W_WIDGET(widget)->style & W_DROP_DOWN) != 0
			&& (_W_WIDGET(widget)->style & W_DATE) != 0)) {
		/*down = new Button (this, SWT.ARROW  | SWT.DOWN);
		 GTK.gtk_widget_set_can_focus (down.handle, false);
		 down.addListener (SWT.Selection, event -> {
		 setFocus ();
		 dropDownCalendar (!isDropped ());
		 });

		 popupListener = event -> {
		 if (event.widget == popupShell) {
		 popupShellEvent (event);
		 return;
		 }
		 if (event.widget == popupCalendar) {
		 popupCalendarEvent (event);
		 return;
		 }
		 if (event.widget == DateTime.this) {
		 onDispose (event);
		 return;
		 }
		 if (event.widget == getShell ()) {
		 getDisplay ().asyncExec (() -> {
		 if (isDisposed ()) return;
		 handleFocus (SWT.FocusOut);
		 });
		 }
		 };
		 popupFilter = event -> {
		 Shell shell = ((Control)event.widget).getShell ();
		 if (shell == DateTime.this.getShell ()) {
		 handleFocus (SWT.FocusOut);
		 }
		 };
		 createPopupShell(-1, -1, -1);
		 addListener(SWT.Resize, event->setDropDownButtonSize());*/
	}
	//initAccessible();

	if (((_W_WIDGET(widget)->style & W_DROP_DOWN) != 0
			&& (_W_WIDGET(widget)->style & W_DATE) != 0)) {
		//Date w/ drop down button is in containers.
		//first time round we set the bounds manually for correct Right_to_left behaviour
		/*Point size = computeSizeInPixels(SWT.DEFAULT, SWT.DEFAULT);
		 setBoundsInPixels(0, 0, size.x, size.y);*/
	}
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;

}
int _w_datetime_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->style & W_CALENDAR) != 0) {
		return _w_scrollable_compute_trim(widget, e, reserved);
	}
	int ret = _w_scrollable_compute_trim(widget, e, reserved);
	int xborder = 0, yborder = 0;
#if GTK3
	GtkBorder tmp;
	GtkWidget *textEntryHandle = _w_datetime_textEntryHandle(widget);
	GtkStyleContext *context = gtk_widget_get_style_context(textEntryHandle);
	if (GTK_VERSION < VERSION(3, 18, 0)) {
		gtk_style_context_get_padding(context, GTK_STATE_FLAG_NORMAL, &tmp);
	} else {
		gtk_style_context_get_padding(context,
				gtk_widget_get_state_flags(textEntryHandle), &tmp);
	}
	e->result->x -= tmp.left;
	e->result->y -= tmp.top;
	e->result->width += tmp.left + tmp.right;
	e->result->height += tmp.top + tmp.bottom;
	if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
		if (GTK_VERSION < VERSION(3, 18, 0)) {
			gtk_style_context_get_border(context, GTK_STATE_FLAG_NORMAL, &tmp);
		} else {
			gtk_style_context_get_border(context,
					gtk_widget_get_state_flags(textEntryHandle), &tmp);
		}
		e->result->x -= tmp.left;
		e->result->y -= tmp.top;
		e->result->width += tmp.left + tmp.right;
		e->result->height += tmp.top + tmp.bottom;
	}
#else
#endif
	e->result->x -= xborder;
	e->result->y -= yborder;
	e->result->width += 2 * xborder;
	e->result->height += 2 * yborder;
	e->result->width += SPACE_FOR_CURSOR;
	return ret;
}
int _w_datetime_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {

	int width = 0, height = 0;
	//For Date and Time, we cache the preffered size as there is no need to recompute it.
	/*if (!changed && (isDate () || isTime ()) && prefferedSize != null) {
	 width = (wHint != SWT.DEFAULT) ? wHint : prefferedSize.x;
	 height= (hHint != SWT.DEFAULT) ? hHint : prefferedSize.y;
	 return new Point (width,height);
	 }*/

	if (e->wHint == W_DEFAULT || e->hHint == W_DEFAULT) {
		if (_W_WIDGET(widget)->style & W_CALENDAR) {
			GtkWidget *containerHandle = _W_WIDGET(widget)->handle;
			_w_control_compute_native_size(widget, containerHandle, e,
					reserved);
			width = e->size->width;
			height = e->size->height;
		} else {
			GtkWidget *textEntryHandle= _w_datetime_textEntryHandle(widget);
			_w_control_compute_native_size(widget, textEntryHandle, e,
					reserved);
			w_event_compute_trim ee;
			w_rect rect, trim;
			ee.rect = &rect;
			ee.result = &trim;
			rect.x = rect.y = 0;
			rect.width = e->size->width;
			rect.height = e->size->height;
			_w_datetime_compute_trim(widget, &ee, reserved);
			if (((_W_WIDGET(widget)->style & W_DROP_DOWN) != 0
					&& (_W_WIDGET(widget)->style & W_DATE) != 0)) {
				GtkWidget *down;
				int wHint = e->wHint;
				int hHint = e->hHint;
				e->wHint = e->hHint = W_DEFAULT;
				_w_control_compute_native_size(widget, down, e, reserved);
				e->wHint = wHint;
				e->hHint = hHint;
				width = trim.width + e->size->width;
				height = w_max(trim.height, e->size->height);
			} else if (((_W_WIDGET(widget)->style & W_DATE) != 0)
					|| ((_W_WIDGET(widget)->style & W_TIME) != 0)) {
#if GTK3
				width = trim.width;
				height = trim.height;
#else
#endif
			}
		}
	}
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	if (e->wHint != W_DEFAULT)
		width = e->wHint;
	if (e->hHint != W_DEFAULT)
		height = e->hHint;
	int borderWidth = _w_control_get_border_width(W_CONTROL(widget));

	e->size->width = width + 2 * borderWidth;
	e->size->height = height + 2 * borderWidth;
	return TRUE;
}
void _w_datetime_class_init(struct _w_datetime_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_DATETIME;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_datetime_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_datetime);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_datetime);
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_datetime_create;
	/*
	 * public function
	 */
	clazz->get_day = _w_datetime_get_day;
	clazz->get_hours = _w_datetime_get_hours;
	clazz->get_minutes = _w_datetime_get_minutes;
	clazz->get_month = _w_datetime_get_month;
	clazz->get_seconds = _w_datetime_get_seconds;
	clazz->get_year = _w_datetime_get_year;
	clazz->set_date = _w_datetime_set_date;
	clazz->set_day = _w_datetime_set_day;
	clazz->set_hours = _w_datetime_set_hours;
	clazz->set_minutes = _w_datetime_set_minutes;
	clazz->set_month = _w_datetime_set_month;
	clazz->set_seconds = _w_datetime_set_seconds;
	clazz->set_time = _w_datetime_set_time;
	clazz->set_year = _w_datetime_set_year;
	/*
	 * reserved
	 */
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	reserved->fixedHandle = _w_widget_hp;
	reserved->widget.compute_size = _w_datetime_compute_size;
	reserved->widget.compute_trim = _w_datetime_compute_trim;
}
#endif
