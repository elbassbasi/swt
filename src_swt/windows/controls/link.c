/*
 * link.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "link.h"
#include "../widgets/toolkit.h"
w_string_ref* _w_link_get_text(w_link *link) {

}
wresult _w_link_set_text(w_link *link, const char *string) {

}
void _w_link_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_control_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= WS_TABSTOP;
}
wresult _w_link_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	//style = _w_link_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_link_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), W_COMPOSITE(parent),
			"SysLink", dwExStyle, dwStyle);
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
void _w_link_class_init(struct _w_link_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LINK;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_link_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_link);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_link);
	/*
	 * public function
	 */
	clazz->get_text = _w_link_get_text;
	clazz->set_text = _w_link_set_text;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_link_create;
}
#endif
