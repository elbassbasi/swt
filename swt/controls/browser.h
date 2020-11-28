/*
 * browser.h
 *
 *  Created on: 12 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_BROWSER_H_
#define SWT_CONTROLS_BROWSER_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_browser {
	w_composite composite;
	void *handle[0x10];
} w_browser;

#define W_BROWSER(x) ((w_browser*)x)

struct _w_browser_class {
	struct _w_composite_class composite;
	wresult (*back)(w_browser *browser);
	wresult (*execute)(w_browser *browser, const char *script);
	wresult (*close)(w_browser *browser);
	wresult (*evaluate)(w_browser *browser, const char *script,
			w_value *result);
	wresult (*forward)(w_browser *browser);
	const char* (*get_browser_type)(w_browser *browser);
	wresult (*get_javascript_enabled)(w_browser *browser);
	wresult (*get_text)(w_browser *browser, w_alloc alloc, void *user_data);
	wresult (*get_url)(w_browser *browser, w_alloc alloc, void *user_data);
	wresult (*is_back_enabled)(w_browser *browser);
	wresult (*is_forward_enabled)(w_browser *browser);
	wresult (*refresh)(w_browser *browser);
	wresult (*set_javascript_enabled)(w_browser *browser, wresult enabled);
	wresult (*set_text)(w_browser *browser, const char *html, wresult trusted);
	wresult (*set_url)(w_browser *browser, const char *url,
			const char *postData, const char *headers);
	wresult (*stop)(w_browser *browser);
};
SWT_PUBLIC wresult w_browser_create(w_browser *browser,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_browser* w_browser_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_browser_back(w_browser *browser);
SWT_PUBLIC wresult w_browser_execute(w_browser *browser, const char *script);
SWT_PUBLIC wresult w_browser_close(w_browser *browser);
SWT_PUBLIC wresult w_browser_evaluate(w_browser *browser, const char *script,
		w_value *result);
SWT_PUBLIC wresult w_browser_forward(w_browser *browser);
SWT_PUBLIC const char* w_browser_get_browser_type(w_browser *browser);
SWT_PUBLIC wresult w_browser_get_javascript_enabled(w_browser *browser);
SWT_PUBLIC wresult w_browser_get_text(w_browser *browser,w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_browser_get_url(w_browser *browser,w_alloc alloc, void *user_data);
SWT_PUBLIC wresult w_browser_is_back_enabled(w_browser *browser);
SWT_PUBLIC wresult w_browser_is_forward_enabled(w_browser *browser);
SWT_PUBLIC wresult w_browser_refresh(w_browser *browser);
SWT_PUBLIC wresult w_browser_set_javascript_enabled(w_browser *browser,
		int enabled);
SWT_PUBLIC wresult w_browser_set_text(w_browser *browser, const char *html,
		int trusted);
SWT_PUBLIC wresult w_browser_set_url(w_browser *browser, const char *url,
		const char *postData, const char *headers);
SWT_PUBLIC wresult w_browser_stop(w_browser *browser);

#define W_BROWSER_CLASS(x) ((struct _w_browser_class*)x)
#define W_BROWSER_GET_CLASS(x) ((struct _w_browser_class*)W_WIDGET_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_BROWSER_H_ */
