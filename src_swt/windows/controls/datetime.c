/*
 * datatime.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "datetime.h"
#include "../widgets/toolkit.h"
/*
 * private
 */
int _w_datetime_check_style(int style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);
	style = _w_widget_check_bits(style, W_DATE, W_TIME, W_CALENDAR, 0, 0, 0);
	style = _w_widget_check_bits(style, W_MEDIUM, W_SHORT, W_LONG, 0, 0, 0);
	if ((style & W_DATE) == 0)
		style &= ~W_DROP_DOWN;
	return style;
}
void _w_datetime_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= WS_TABSTOP;
	if ((style & W_CALENDAR) != 0) {
		*dwStyle |= MCS_NOTODAY;
		return;
	}
	/*
	 * Bug in Windows: When WS_CLIPCHILDREN is set in a
	 * Date and Time Picker, the widget draws on top of
	 * the updown control. The fix is to clear the bits.
	 */
	*dwStyle &= ~WS_CLIPCHILDREN;
	if ((style & W_TIME) != 0) {
		*dwStyle |= DTS_TIMEFORMAT;
	}
	if ((style & W_DATE) != 0) {
		*dwStyle |= ((style & W_MEDIUM) != 0 ?
		DTS_SHORTDATECENTURYFORMAT :
												DTS_LONGDATEFORMAT);
		if ((style & W_DROP_DOWN) == 0)
			*dwStyle |= DTS_UPDOWN;
	}
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
wresult _w_datetime_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_datetime_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_datetime_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), W_COMPOSITE(parent),
			(style & W_CALENDAR) != 0 ? MONTHCAL_CLASSA : DATETIMEPICK_CLASSA,
			dwExStyle, dwStyle);
	if (result > 0) {
		w_font *systemfont = w_toolkit_get_system_font(
				(w_toolkit*) win_toolkit);
		SendMessageW(_W_WIDGET(widget)->handle, WM_SETFONT,
				(WPARAM) _W_FONT(systemfont)->handle, 0);
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
	}
	return result;

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
}
#endif
