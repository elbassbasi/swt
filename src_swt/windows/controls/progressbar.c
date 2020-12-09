/*
 * progressbar.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "progressbar.h"
#include "../widgets/toolkit.h"
#if NTDDI_VERSION < 0x06000000
#define PBM_GETSTEP (WM_USER+13)
#define PBM_GETBKCOLOR (WM_USER+14)
#define PBM_GETBARCOLOR (WM_USER+15)
#define PBM_SETSTATE (WM_USER+16)
#define PBM_GETSTATE (WM_USER+17)

#define PBS_SMOOTHREVERSE 0x10

#define PBST_NORMAL 1
#define PBST_ERROR 2
#define PBST_PAUSED 3
#endif /* (_WIN32_WINNT >= 0x0600) */
int _w_progressbar_get_maximum(w_progressbar *progressbar) {
	return (int) SendMessageW(_W_WIDGET(progressbar)->handle, PBM_GETRANGE, 0,
			0);
}
int _w_progressbar_get_selection(w_progressbar *progressbar) {
	return (int) SendMessageW(_W_WIDGET(progressbar)->handle, PBM_GETPOS, 0, 0);
}
int _w_progressbar_get_state(w_progressbar *progressbar) {
	int state = (int) SendMessageW(_W_WIDGET(progressbar)->handle,
	PBM_GETSTATE, 0, 0);
	switch (state) {
	case PBST_NORMAL:
		return W_NORMAL;
	case PBST_ERROR:
		return W_ERROR;
	case PBST_PAUSED:
		return W_PAUSED;
	}
	return W_NORMAL;
}
wresult _w_progressbar_set_maximum(w_progressbar *progressbar, int value) {
	SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETRANGE32, 0, value);
	return W_TRUE;
}
wresult _w_progressbar_set_selection(w_progressbar *progressbar, int value) {
	SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETPOS, value, 0);
	/*
	 * Bug in Vista.  For some reason, when the progress bar is not in
	 * a normal state, it shows the selection of previous call to
	 * PBM_SETPOS. This is undocumented. The fix is to call PBM_SETPOS
	 * a second time.
	 */
	int state = SendMessageW(_W_WIDGET(progressbar)->handle, PBM_GETSTATE, 0,
			0);
	if (state != PBST_NORMAL) {
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETPOS, value, 0);
	}
	return W_TRUE;
}
wresult _w_progressbar_set_state(w_progressbar *progressbar, int state) {
	switch (state) {
	case W_NORMAL:
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETSTATE, PBST_NORMAL,
				0);
		break;
	case W_ERROR:
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETSTATE, PBST_ERROR,
				0);
		break;
	case W_PAUSED:
		SendMessageW(_W_WIDGET(progressbar)->handle, PBM_SETSTATE, PBST_PAUSED,
				0);
		break;
	}
	return W_TRUE;
}
int _w_progressbar_check_style(int style) {
	style |= W_NO_FOCUS;
	return _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
}
void _w_progressbar_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_control_style(widget, parent, style, dwExStyle, dwStyle);
	if ((style & W_SMOOTH) != 0)
		*dwStyle |= PBS_SMOOTH;
	if ((style & W_VERTICAL) != 0)
		*dwStyle |= PBS_VERTICAL;
	if ((style & W_INDETERMINATE) != 0)
		*dwStyle |= PBS_MARQUEE;
}
wresult _w_progressbar_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_progressbar_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_progressbar_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	PROGRESS_CLASSA, dwExStyle, dwStyle);
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
int _w_progressbar_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);

}
void _w_progressbar_class_init(struct _w_progressbar_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_PROGRESSBAR;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_progressbar_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_progressbar);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_progressbar);
	/*
	 * public function
	 */
	clazz->get_maximum = _w_progressbar_get_maximum;
	clazz->get_selection = _w_progressbar_get_selection;
	clazz->get_state = _w_progressbar_get_state;
	clazz->set_maximum = _w_progressbar_set_maximum;
	clazz->set_selection = _w_progressbar_set_selection;
	clazz->set_state = _w_progressbar_set_state;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_progressbar_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_progressbar_create;
}
#endif
