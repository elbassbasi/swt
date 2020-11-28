/*
 * scale.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "scale.h"
#include "../widgets/toolkit.h"
int _w_scale_check_style(int style) {
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
void _w_scale_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_control_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= WS_TABSTOP | TBS_BOTH | TBS_AUTOTICKS;
	if ((style & W_HORIZONTAL) != 0) {
		*dwStyle |= TBS_HORZ | TBS_DOWNISLEFT;
		return;
	}
	*dwStyle |= TBS_VERT;
}
int _w_scale_get_increment(w_scale *scale) {
}
int _w_scale_get_maximum(w_scale *scale) {
}
int _w_scale_get_minimum(w_scale *scale) {
}
int _w_scale_get_page_increment(w_scale *scale) {
}
int _w_scale_get_selection(w_scale *scale) {
}
wresult _w_scale_set_increment(w_scale *scale, int increment) {
}
wresult _w_scale_set_maximum(w_scale *scale, int value) {
}
wresult _w_scale_set_minimum(w_scale *scale, int value) {
}
wresult _w_scale_set_page_increment(w_scale *scale, int pageIncrement) {
}
wresult _w_scale_set_selection(w_scale *scale, int value) {
}
wresult _w_scale_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_scale_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_scale_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), W_COMPOSITE(parent),
	TRACKBAR_CLASSA, dwExStyle, dwStyle);
	if (result > 0) {
		w_font *systemfont = w_toolkit_get_system_font(
				(w_toolkit*) win_toolkit);
		SendMessageW(_W_WIDGET(widget)->handle, WM_SETFONT,
				(WPARAM) _W_FONT(systemfont)->handle, 0);
		//state |= THEME_BACKGROUND | DRAW_BACKGROUND;
		SendMessageW(_W_WIDGET(widget)->handle, TBM_SETRANGEMAX, 0, 100);
		SendMessageW(_W_WIDGET(widget)->handle, TBM_SETPAGESIZE, 0, 10);
		SendMessageW(_W_WIDGET(widget)->handle, TBM_SETTICFREQ, 10, 0);
		//createdAsRTL = (style & SWT.RIGHT_TO_LEFT) != 0;
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
	}
	return result;

}
int _w_scale_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);

}
void _w_scale_class_init(struct _w_scale_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SCALE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_scale_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_scale);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_scale);
	/*
	 * public function
	 */
	clazz->get_increment=_w_scale_get_increment;
	clazz->get_maximum=_w_scale_get_maximum;
	clazz->get_minimum=_w_scale_get_minimum;
	clazz->get_page_increment=_w_scale_get_page_increment;
	clazz->get_selection=_w_scale_get_selection;
	clazz->set_increment=_w_scale_set_increment;
	clazz->set_maximum=_w_scale_set_maximum;
	clazz->set_minimum=_w_scale_set_minimum;
	clazz->set_page_increment=_w_scale_set_page_increment;
	clazz->set_selection=_w_scale_set_selection;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_scale_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_scale_create;
}
#endif
