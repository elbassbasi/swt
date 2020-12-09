/*
 * text.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "text.h"
#include "../widgets/toolkit.h"
#define SPACE_FOR_CURSOR 1
wresult _w_text_append(w_text *text, const char *string) {
}
wresult _w_text_clear_selection(w_text *text) {
}
wresult _w_text_copy(w_text *text) {
}
wresult _w_text_cut(w_text *text) {
}
int _w_text_get_caret_line_number(w_text *text) {
}
wresult _w_text_get_caret_location(w_text *text, w_point *location) {
}
int _w_text_get_caret_position(w_text *text) {
}
int _w_text_get_char_count(w_text *text) {
}
wresult _w_text_get_doubleclick_enabled(w_text *text) {
}
char _w_text_get_echo_char(w_text *text) {
}
wresult _w_text_get_editable(w_text *text) {
}
int _w_text_get_line_count(w_text *text) {
}
w_string_ref* _w_text_get_line_delimiter(w_text *text) {
}
int _w_text_get_line_height(w_text *text) {
}
w_string_ref* _w_text_get_message(w_text *text) {
}
int _w_text_get_position(w_text *text, w_point *point) {
}
wresult _w_text_get_selection(w_text *text, w_range *range) {
}
int _w_text_get_selection_count(w_text *text) {
}
w_string_ref* _w_text_get_selection_text(w_text *text) {
}
int _w_text_get_tabs(w_text *text) {
}
w_string_ref* _w_text_get_text(w_text *text) {
}
w_string_ref* _w_text_get_text_range(w_text *text, w_range *range) {
}
int _w_text_get_text_limit(w_text *text) {
}
int _w_text_get_top_index(w_text *text) {
}
int _w_text_get_top_pixel(w_text *text) {
}
wresult _w_text_insert(w_text *text, const char *string) {
}
wresult _w_text_paste(w_text *text) {
}
wresult _w_text_select_all(w_text *text) {
}
wresult _w_text_set_doubleclick_enabled(w_text *text, int doubleClick) {
}
wresult _w_text_set_echochar(w_text *text, char echo) {
}
wresult _w_text_set_editable(w_text *text, int editable) {
}
wresult _w_text_set_message(w_text *text, const char *message) {
}
wresult _w_text_set_selection(w_text *text, int start) {
}
wresult _w_text_set_selection_range(w_text *text, w_range *selection) {
}
wresult _w_text_set_tabs(w_text *text, int tabs) {
}
wresult _w_text_set_text(w_text *text, const char *string) {
}
wresult _w_text_set_text_limit(w_text *text, int limit) {
}
wresult _w_text_set_top_index(w_text *text, int index) {
}
wresult _w_text_show_selection(w_text *text) {
}
int _w_text_check_style(int style) {
	if ((style & W_SEARCH) != 0) {
		style |= W_SINGLE | W_BORDER;
		style &= ~W_PASSWORD;
		/*
		 * NOTE: ICON_CANCEL has the same value as H_SCROLL and
		 * ICON_SEARCH has the same value as V_SCROLL so they are
		 * cleared because W_SINGLE is set.
		 */
	}
	if ((style & W_SINGLE) != 0 && (style & W_MULTI) != 0) {
		style &= ~W_MULTI;
	}
	style = _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
	if ((style & W_SINGLE) != 0)
		style &= ~(W_HSCROLL | W_VSCROLL | W_WRAP);
	if ((style & W_WRAP) != 0) {
		style |= W_MULTI;
		style &= ~W_HSCROLL;
	}
	if ((style & W_MULTI) != 0)
		style &= ~W_PASSWORD;
	if ((style & (W_SINGLE | W_MULTI)) != 0)
		return style;
	if ((style & (W_HSCROLL | W_VSCROLL)) != 0)
		return style | W_MULTI;
	return style | W_SINGLE;
}

wresult _w_text_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
}

int _w_text_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved) {
	w_event_compute_size ee;
	w_size widthNative;
	return W_TRUE;
}

int _w_text_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int w, h;
	return W_TRUE;
}
void _w_text_class_init(struct _w_text_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TEXT;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_text_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_text);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_text);
	/*
	 * public function
	 */
	clazz->append = _w_text_append;
	clazz->clear_selection = _w_text_clear_selection;
	clazz->copy = _w_text_copy;
	clazz->cut = _w_text_cut;
	clazz->get_caret_line_number = _w_text_get_caret_line_number;
	clazz->get_caret_location = _w_text_get_caret_location;
	clazz->get_caret_position = _w_text_get_caret_position;
	clazz->get_char_count = _w_text_get_char_count;
	clazz->get_doubleclick_enabled = _w_text_get_doubleclick_enabled;
	clazz->get_echo_char = _w_text_get_echo_char;
	clazz->get_editable = _w_text_get_editable;
	clazz->get_line_count = _w_text_get_line_count;
	clazz->get_line_delimiter = _w_text_get_line_delimiter;
	clazz->get_line_height = _w_text_get_line_height;
	clazz->get_message = _w_text_get_message;
	clazz->get_position = _w_text_get_position;
	clazz->get_selection = _w_text_get_selection;
	clazz->get_selection_count = _w_text_get_selection_count;
	clazz->get_selection_text = _w_text_get_selection_text;
	clazz->get_tabs = _w_text_get_tabs;
	clazz->get_text = _w_text_get_text;
	clazz->get_text_range = _w_text_get_text_range;
	clazz->get_text_limit = _w_text_get_text_limit;
	clazz->get_top_index = _w_text_get_top_index;
	clazz->get_top_pixel = _w_text_get_top_pixel;
	clazz->insert = _w_text_insert;
	clazz->paste = _w_text_paste;
	clazz->select_all = _w_text_select_all;
	clazz->set_doubleclick_enabled = _w_text_set_doubleclick_enabled;
	clazz->set_echochar = _w_text_set_echochar;
	clazz->set_editable = _w_text_set_editable;
	clazz->set_message = _w_text_set_message;
	clazz->set_selection = _w_text_set_selection;
	clazz->set_selection_range = _w_text_set_selection_range;
	clazz->set_tabs = _w_text_set_tabs;
	clazz->set_text = _w_text_set_text;
	clazz->set_text_limit = _w_text_set_text_limit;
	clazz->set_top_index = _w_text_set_top_index;
	clazz->show_selection = _w_text_show_selection;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_text_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->widget.compute_size = _w_text_compute_size;
	reserved->widget.compute_trim = _w_text_compute_trim;
}
#endif
