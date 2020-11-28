/*
 * browser.c
 *
 *  Created on: 12 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_browser_create(w_browser *browser, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(browser), toolkit, W_WIDGET(parent), style,_W_CLASS_BROWSER,
			post_event);
}
w_browser* w_browser_new(struct w_toolkit *toolkit, w_composite *parent,
                         wuint64 style, w_widget_post_event_proc post_event) {
	return W_BROWSER(_w_widget_new(toolkit, W_WIDGET(parent), style,_W_CLASS_BROWSER, post_event));
}
wresult w_browser_back(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->back(browser);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_execute(w_browser *browser, const char *script) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->execute(browser, script);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_close(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->close(browser);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_evaluate(w_browser *browser, const char *script,
		w_value *result) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->evaluate(browser, script, result);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_forward(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->forward(browser);
	} else
		return W_ERROR_NO_HANDLES;
}
const char* w_browser_get_browser_type(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->get_browser_type(browser);
	} else
		return 0;
}
wbool w_browser_get_javascript_enabled(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->get_javascript_enabled(browser);
	} else
		return W_FALSE;
}
wresult w_browser_get_text(w_browser *browser,w_alloc alloc, void *user_data) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->get_text(browser,alloc,user_data);
	} else
		return 0;
}
wresult w_browser_get_url(w_browser *browser,w_alloc alloc, void *user_data) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->get_url(browser,alloc,user_data);
	} else
		return 0;
}
wbool w_browser_is_back_enabled(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->is_back_enabled(browser);
	} else
		return W_FALSE;
}
wbool w_browser_is_forward_enabled(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->is_forward_enabled(browser);
	} else
		return W_FALSE;
}
wresult w_browser_refresh(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->refresh(browser);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_set_javascript_enabled(w_browser *browser, wbool enabled) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->set_javascript_enabled(browser,
				enabled);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_set_text(w_browser *browser, const char *html,
		wbool trusted) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->set_text(browser, html, trusted);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_set_url(w_browser *browser, const char *url,
		const char *postData, const char *headers) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->set_url(browser, url, postData,
				headers);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_browser_stop(w_browser *browser) {
	if (W_WIDGET_CHECK(browser)) {
		return W_BROWSER_GET_CLASS(browser)->stop(browser);
	} else
		return W_ERROR_NO_HANDLES;
}

