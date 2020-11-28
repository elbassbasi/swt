/*
 * spinner.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_SPINNER_H_
#define SWT_CONTROLS_SPINNER_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_spinner {
	w_composite composite;
	void *handle[0x10];
} w_spinner;
typedef struct w_spinner_value {
	int selection;
	int minimum;
	int maximum;
	int digits;
	int increment;
	int pageIncrement;
} w_spinner_value;

#define W_SPINNER(x) ((w_spinner*)x)

struct _w_spinner_class {
	struct _w_composite_class composite;
	wresult (*copy)(w_spinner *spinner);
	wresult (*cut)(w_spinner *spinner);
	int (*get_digits)(w_spinner *spinner);
	int (*get_increment)(w_spinner *spinner);
	int (*get_maximum)(w_spinner *spinner);
	int (*get_minimum)(w_spinner *spinner);
	int (*get_page_increment)(w_spinner *spinner);
	int (*get_selection)(w_spinner *spinner);
	wresult (*get_text)(w_spinner *spinner,w_alloc alloc, void *user_data);
	int (*get_text_limit)(w_spinner *spinner);
	wresult (*get_values)(w_spinner *spinner, w_spinner_value *value);
	wresult (*paste)(w_spinner *spinner);
	wresult (*set_digits)(w_spinner *spinner, int value);
	wresult (*set_increment)(w_spinner *spinner, int value);
	wresult (*set_maximum)(w_spinner *spinner, int value);
	wresult (*set_minimum)(w_spinner *spinner, int value);
	wresult (*set_page_increment)(w_spinner *spinner, int value);
	wresult (*set_selection)(w_spinner *spinner, int value);
	wresult (*set_text_limit)(w_spinner *spinner, int limit);
	wresult (*set_values)(w_spinner *spinner, w_spinner_value *value);
};
SWT_PUBLIC wresult w_spinner_create(w_spinner *spinner, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_spinner* w_spinner_new(struct w_toolkit *toolkit, w_composite *parent,
                                    wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_spinner_copy(w_spinner *spinner);
SWT_PUBLIC wresult w_spinner_cut(w_spinner *spinner);
SWT_PUBLIC int w_spinner_get_digits(w_spinner *spinner);
SWT_PUBLIC int w_spinner_get_increment(w_spinner *spinner);
SWT_PUBLIC int w_spinner_get_maximum(w_spinner *spinner);
SWT_PUBLIC int w_spinner_get_minimum(w_spinner *spinner);
SWT_PUBLIC int w_spinner_get_page_increment(w_spinner *spinner);
SWT_PUBLIC int w_spinner_get_selection(w_spinner *spinner);
SWT_PUBLIC wresult w_spinner_get_text(w_spinner *spinner,w_alloc alloc, void *user_data);
SWT_PUBLIC int w_spinner_get_text_limit(w_spinner *spinner);
SWT_PUBLIC wresult w_spinner_get_values(w_spinner *spinner, w_spinner_value *value);
SWT_PUBLIC wresult w_spinner_paste(w_spinner *spinner);
SWT_PUBLIC wresult w_spinner_set_digits(w_spinner *spinner, int value);
SWT_PUBLIC wresult w_spinner_set_increment(w_spinner *spinner, int value);
SWT_PUBLIC wresult w_spinner_set_maximum(w_spinner *spinner, int value);
SWT_PUBLIC wresult w_spinner_set_minimum(w_spinner *spinner, int value);
SWT_PUBLIC wresult w_spinner_set_page_increment(w_spinner *spinner, int value);
SWT_PUBLIC wresult w_spinner_set_selection(w_spinner *spinner, int value);
SWT_PUBLIC wresult w_spinner_set_text_limit(w_spinner *spinner, int limit);
SWT_PUBLIC wresult w_spinner_set_values(w_spinner *spinner, w_spinner_value *value);

#define W_SPINNER_CLASS(x) ((struct _w_spinner_class*)x)
#define W_SPINNER_GET_CLASS(x) ((struct _w_spinner_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_SPINNER_H_ */
