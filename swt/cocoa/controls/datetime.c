/*
 * datetime.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
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
wresult _w_datetime_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;

}
int _w_datetime_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved) {
}
int _w_datetime_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
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
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->widget.compute_size = _w_datetime_compute_size;
	reserved->widget.compute_trim = _w_datetime_compute_trim;
}
#endif
