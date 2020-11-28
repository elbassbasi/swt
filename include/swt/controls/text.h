/*
 * text.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_TEXT_H_
#define SWT_CONTROLS_TEXT_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_text {
	w_scrollable scrollable;
	void *handle[8];
} w_text;
#define W_TEXT(x) ((w_text*)x)
#define W_TEXT_CLASS(x) ((struct _w_text_class*)x)
#define W_TEXT_GET_CLASS(x) ((struct _w_text_class*)W_WIDGET_GET_CLASS(x))
struct _w_text_class {
	struct _w_scrollable_class scrollable;
	wresult (*append)(w_text *text, const char *string, size_t length,int enc);
	wresult (*clear_selection)(w_text *text);
	wresult (*copy)(w_text *text);
	wresult (*cut)(w_text *text);
	wresult (*get_caret_line_number)(w_text *text);
	wresult (*get_caret_location)(w_text *text, w_point *location);
	wresult (*get_caret_position)(w_text *text,int enc);
	wresult (*get_char_count)(w_text *text,int enc);
	wresult (*get_doubleclick_enabled)(w_text *text);
	wresult (*get_echo_char)(w_text *text);
	wresult (*get_editable)(w_text *text);
	wresult (*get_line_count)(w_text *text);
	wresult (*get_line_delimiter)(w_text *text);
	wresult (*get_line_height)(w_text *text);
	wresult (*get_message)(w_text *text, w_alloc alloc, void *user_data,int enc);
	wresult (*get_position)(w_text *text, w_point *point,int enc);
	wresult (*get_selection)(w_text *text, w_range *range,int enc);
	wresult (*get_selection_count)(w_text *text,int enc);
	wresult (*get_selection_text)(w_text *text, w_alloc alloc, void *user_data,int enc);
	wresult (*get_tabs)(w_text *text);
	wresult (*get_text)(w_text *text, w_alloc alloc, void *user_data,int enc);
	wresult (*get_text_range)(w_text *text, w_range *range, w_alloc alloc,
			void *user_data,int enc);
	wresult (*get_text_limit)(w_text *text,int enc);
	wresult (*get_top_index)(w_text *text);
	wresult (*get_top_pixel)(w_text *text);
	wresult (*insert)(w_text *text, const char *string, size_t length,int enc);
	wresult (*paste)(w_text *text);
	wresult (*select_all)(w_text *text);
	wresult (*set_doubleclick_enabled)(w_text *text, wbool doubleClick);
	wresult (*set_echochar)(w_text *text, int echo);
	wresult (*set_editable)(w_text *text, wbool editable);
	wresult (*set_message)(w_text *text, const char *message, size_t length,int enc);
	wresult (*set_selection)(w_text *text, int start,int enc);
	wresult (*set_selection_range)(w_text *text, w_range *selection,int enc);
	wresult (*set_tabs)(w_text *text, int tabs);
	wresult (*set_text)(w_text *text, const char *string, size_t length,int enc);
	wresult (*set_text_limit)(w_text *text, int limit);
	wresult (*set_top_index)(w_text *text, int index);
	wresult (*show_selection)(w_text *text);
};
SWT_PUBLIC wresult w_text_create(w_text *text, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_text* w_text_new(struct w_toolkit *toolkit, w_composite *parent,
		wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_text_append(w_text *text, const char*, size_t length,int enc);
SWT_PUBLIC wresult w_text_clear_selection(w_text *text);
SWT_PUBLIC wresult w_text_copy(w_text *text);
SWT_PUBLIC wresult w_text_cut(w_text *text);
SWT_PUBLIC wresult w_text_get_caret_line_number(w_text *text);
SWT_PUBLIC wresult w_text_get_caret_location(w_text *text, w_point *location);
SWT_PUBLIC wresult w_text_get_caret_position(w_text *text,int enc);
SWT_PUBLIC wresult w_text_get_char_count(w_text *text,int enc);
SWT_PUBLIC wresult w_text_get_doubleclick_enabled(w_text *text);
SWT_PUBLIC wresult w_text_get_echo_char(w_text *text);
SWT_PUBLIC wresult w_text_get_editable(w_text *text);
SWT_PUBLIC wresult w_text_get_line_count(w_text *text);
SWT_PUBLIC wresult w_text_get_line_delimiter(w_text *text);
SWT_PUBLIC wresult w_text_get_line_height(w_text *text);
SWT_PUBLIC wresult w_text_get_message(w_text *text, w_alloc alloc,
		void *user_data,int enc);
SWT_PUBLIC wresult w_text_get_position(w_text *text, w_point *point,int enc);
SWT_PUBLIC wresult w_text_get_selection(w_text *text, w_range *range,int enc);
SWT_PUBLIC wresult w_text_get_selection_count(w_text *text,int enc);
SWT_PUBLIC wresult w_text_get_selection_text(w_text *text, w_alloc alloc,
		void *user_data,int enc);
SWT_PUBLIC wresult w_text_get_tabs(w_text *text);
SWT_PUBLIC wresult w_text_get_text(w_text *text, w_alloc alloc,
		void *user_data,int enc);
SWT_PUBLIC wresult w_text_get_text_range(w_text *text, w_range *range,
		w_alloc alloc, void *user_data,int enc);
SWT_PUBLIC wresult w_text_get_text_limit(w_text *text,int enc);
SWT_PUBLIC wresult w_text_get_top_index(w_text *text);
SWT_PUBLIC wresult w_text_get_top_pixel(w_text *text);
SWT_PUBLIC wresult w_text_insert(w_text *text, const char *string,
		size_t length,int enc);
SWT_PUBLIC wresult w_text_paste(w_text *text);
SWT_PUBLIC wresult w_text_select_all(w_text *text);
SWT_PUBLIC wresult w_text_set_doubleclick_enabled(w_text *text,
		wbool doubleClick);
SWT_PUBLIC wresult w_text_set_echochar(w_text *text, char echo);
SWT_PUBLIC wresult w_text_set_editable(w_text *text, wbool editable);
SWT_PUBLIC wresult w_text_set_message(w_text *text, const char *message,
		size_t length,int enc);
SWT_PUBLIC wresult w_text_set_selection(w_text *text, int start,int enc);
SWT_PUBLIC wresult w_text_set_selection_range(w_text *text, w_range *selection,int enc);
SWT_PUBLIC wresult w_text_set_tabs(w_text *text, int tabs);
SWT_PUBLIC wresult w_text_set_text(w_text *text, const char *string,
		size_t length,int enc);
SWT_PUBLIC wresult w_text_set_text_limit(w_text *text, int limit);
SWT_PUBLIC wresult w_text_set_top_index(w_text *text, int index);
SWT_PUBLIC wresult w_text_show_selection(w_text *text);
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_TEXT_H_ */
