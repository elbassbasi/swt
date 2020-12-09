/*
 * browser.c
 *
 *  Created on: 11 août 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "browser.h"
#include "../widgets/toolkit.h"
#include <dlfcn.h>
#define  ABOUT_BLANK "about:blank"
#define   URI_FILEROOT "file:///"
/*
 *
 */
#define WEBKIT_VERSION_1 (1 << 1)
#define WEBKIT_VERSION_2 (1 << 2)
#define WEBKIT_VERSION_3 (1 << 3)
#define WEBKIT_VERSION_ALL (WEBKIT_VERSION_1 | WEBKIT_VERSION_2 | WEBKIT_VERSION_3)

#define WEBKIT_IGNORETLS (1 << 4)
const char *webkit_function_name[] = { //
		"webkit_web_view_new", //
				"webkit_web_view_get_type", //
				"webkit_web_view_load_uri", //
				"webkit_web_view_load_html_string", //
				"webkit_web_view_load_html", //
		};
const unsigned char webkit_function_version[] = { //
		WEBKIT_VERSION_ALL, //webkit_web_view_new
				WEBKIT_VERSION_ALL, //webkit_web_view_get_type
				WEBKIT_VERSION_ALL, //webkit_web_view_load_uri
				WEBKIT_VERSION_3, //webkit_web_view_load_html_string
				WEBKIT_VERSION_2, //webkit_web_view_load_html
		};
/*
 * public
 */
wresult _w_browser_kit_back(w_browser *browser) {
}
wresult _w_browser_kit_execute(w_browser *browser, const char *script) {
}
wresult _w_browser_kit_close(w_browser *browser) {
}
wresult _w_browser_kit_evaluate(w_browser *browser, const char *script,
		w_value *result) {
}
wresult _w_browser_kit_forward(w_browser *browser) {
}
const char* _w_browser_kit_get_browser_type(w_browser *browser) {
	return "WebKit";
}
wresult _w_browser_kit_get_javascript_enabled(w_browser *browser) {
}
wresult _w_browser_kit_get_text(w_browser *browser, w_alloc alloc,void* user_data) {
}
wresult _w_browser_kit_get_url(w_browser *browser, w_alloc alloc,void* user_data) {
}
wresult _w_browser_kit_is_back_enabled(w_browser *browser) {
}
wresult _w_browser_kit_is_forward_enabled(w_browser *browser) {
}
wresult _w_browser_kit_refresh(w_browser *browser) {
}
wresult _w_browser_kit_set_javascript_enabled(w_browser *browser,
		int enabled) {
}

wresult _w_browser_kit_set_text(w_browser *browser, const char *html_bytes,
		int trusted) {
	struct _w_browser_kit_reserved *reserved = _W_BROWSER_KIT_RESERVED(
			_w_widget_get_reserved(W_WIDGET(browser)));
	return W_TRUE;
}
wresult _w_browser_kit_set_url(w_browser *browser, const char *url,
		const char *postData, const char *headers) {
	struct _w_browser_kit_reserved *reserved = _W_BROWSER_KIT_RESERVED(
			_w_widget_get_reserved(W_WIDGET(browser)));
	return W_TRUE;
}
wresult _w_browser_kit_stop(w_browser *browser) {
}
/*
 *
 */
wresult _w_browser_kit_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	struct _w_browser_kit_reserved *reserved = _W_BROWSER_KIT_RESERVED(
			_w_widget_get_reserved(widget));
}
void _w_browser_kit_dispose(w_widget *widget) {

}
void _w_browser_kit_class_init(struct _w_browser_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BROWSER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_browser_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_browser);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_browser_kit);
	W_WIDGET_CLASS(clazz)->dispose = _w_browser_kit_dispose;
	//
	W_WIDGET_CLASS(clazz)->create = _w_browser_kit_create;
	clazz->back = _w_browser_kit_back;
	clazz->execute = _w_browser_kit_execute;
	clazz->close = _w_browser_kit_close;
	clazz->evaluate = _w_browser_kit_evaluate;
	clazz->forward = _w_browser_kit_forward;
	clazz->get_browser_type = _w_browser_kit_get_browser_type;
	clazz->get_javascript_enabled = _w_browser_kit_get_javascript_enabled;
	clazz->get_text = _w_browser_kit_get_text;
	clazz->get_url = _w_browser_kit_get_url;
	clazz->is_back_enabled = _w_browser_kit_is_back_enabled;
	clazz->is_forward_enabled = _w_browser_kit_is_forward_enabled;
	clazz->refresh = _w_browser_kit_refresh;
	clazz->set_javascript_enabled = _w_browser_kit_set_javascript_enabled;
	clazz->set_text = _w_browser_kit_set_text;
	clazz->set_url = _w_browser_kit_set_url;
	clazz->stop = _w_browser_kit_stop;
	/*
	 *
	 */
	struct _w_browser_reserved *reserved = _W_BROWSER_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
/*
 * public
 */
wresult _w_browser_back(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_execute(w_browser *browser, const char *script) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_close(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_evaluate(w_browser *browser, const char *script,
		w_value *result) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_forward(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
const char* _w_browser_get_browser_type(w_browser *browser) {
	return "empty";
}
wresult _w_browser_get_javascript_enabled(w_browser *browser) {
	return W_FALSE;
}
wresult _w_browser_get_text(w_browser *browser, w_alloc alloc,void* user_data) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_get_url(w_browser *browser, w_alloc alloc,void* user_data) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_is_back_enabled(w_browser *browser) {
	return W_FALSE;
}
wresult _w_browser_is_forward_enabled(w_browser *browser) {
	return W_FALSE;
}
wresult _w_browser_refresh(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_set_javascript_enabled(w_browser *browser, int enabled) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_set_text(w_browser *browser, const char *html,
		int trusted) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_set_url(w_browser *browser, const char *url,
		const char *postData, const char *headers) {
	return W_ERROR_NOT_IMPLEMENTED;
}
wresult _w_browser_stop(w_browser *browser) {
	return W_ERROR_NOT_IMPLEMENTED;
}
/*
 *
 */
void _w_browser_dispose(w_widget *widget) {

}
void _w_browser_empty_class_init(struct _w_browser_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BROWSER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_browser_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_browser);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_browser);
	W_WIDGET_CLASS(clazz)->dispose = _w_browser_dispose;
	//
	clazz->back = _w_browser_back;
	clazz->execute = _w_browser_execute;
	clazz->close = _w_browser_close;
	clazz->evaluate = _w_browser_evaluate;
	clazz->forward = _w_browser_forward;
	clazz->get_browser_type = _w_browser_get_browser_type;
	clazz->get_javascript_enabled = _w_browser_get_javascript_enabled;
	clazz->get_text = _w_browser_get_text;
	clazz->get_url = _w_browser_get_url;
	clazz->is_back_enabled = _w_browser_is_back_enabled;
	clazz->is_forward_enabled = _w_browser_is_forward_enabled;
	clazz->refresh = _w_browser_refresh;
	clazz->set_javascript_enabled = _w_browser_set_javascript_enabled;
	clazz->set_text = _w_browser_set_text;
	clazz->set_url = _w_browser_set_url;
	clazz->stop = _w_browser_stop;
	/*
	 *
	 */
	struct _w_browser_reserved *reserved = _W_BROWSER_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
void _w_browser_class_init(struct _w_browser_class *clazz) {
	_w_browser_empty_class_init(clazz);
}
#endif
