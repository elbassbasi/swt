/*
 * slider.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "slider.h"
#include "../widgets/toolkit.h"
int _w_slider_check_style(int style) {
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
void _w_slider_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_control_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= WS_TABSTOP;
	/*
	 * Bug in Windows.  If a scroll bar control is given a border,
	 * dragging the scroll bar thumb eats away parts of the border
	 * while the thumb is dragged.  The fix is to clear WS_BORDER.
	 */
	if ((style & W_BORDER) != 0)
		*dwStyle &= ~WS_BORDER;
	if ((style & W_HORIZONTAL) != 0) {
		*dwStyle |= SBS_HORZ;
		return;
	}
	*dwStyle |= SBS_VERT;
}
wresult _w_slider_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_slider_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_slider_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_SCROLLBARA, dwExStyle, dwStyle);
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
int _w_slider_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);

}
void _w_slider_class_init(struct _w_slider_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SLIDER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_slider_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_slider);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_slider);
	/*
	 * public function
	 */
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_slider_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_slider_create;
}
#endif
