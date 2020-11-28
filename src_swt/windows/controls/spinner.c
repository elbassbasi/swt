/*
 * spinner.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "spinner.h"
#include "../widgets/toolkit.h"
/*
 * private
 */
int _w_spinner_check_style(int style) {
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
void _w_spinner_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwExStyle &= ~WS_EX_CLIENTEDGE;
}
wresult _w_spinner_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	wresult result = _w_composite_create(widget, parent, style, post_event);
	if (result < 0)
		return result;
	_W_WIDGET(widget)->state &= ~(STATE_CANVAS | STATE_THEME_BACKGROUND);
	int textExStyle = (style & W_BORDER) != 0 ? WS_EX_CLIENTEDGE : 0;
	int textStyle = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_CLIPSIBLINGS;
	if ((style & W_READ_ONLY) != 0)
		textStyle |= ES_READONLY;
	if (WIN32_VERSION >= VERSION(4, 10)) {
		if ((style & W_RIGHT_TO_LEFT) != 0)
			textExStyle |= WS_EX_LAYOUTRTL;
	}
	_W_SPINNER(widget)->hwndText = CreateWindowExW(textExStyle, L"EDIT", 0,
			textStyle, 0, 0, 0, 0,
			_W_WIDGET(widget)->handle, 0, hinst,
			NULL);
	if (_W_SPINNER(widget)->hwndText != 0) {

		SetWindowLongPtrW(_W_SPINNER(widget)->hwndText, GWLP_USERDATA,
				(LONG_PTR) widget);
		int upDownStyle = WS_CHILD | WS_VISIBLE | UDS_AUTOBUDDY;
		if ((style & W_WRAP) != 0)
			upDownStyle |= UDS_WRAP;
		if ((style & W_BORDER) != 0) {
			if ((style & W_RIGHT_TO_LEFT) != 0) {
				upDownStyle |= UDS_ALIGNLEFT;
			} else {
				upDownStyle |= UDS_ALIGNRIGHT;
			}
		}
		_W_SPINNER(widget)->hwndUpDown = CreateWindowExW(0,
		UPDOWN_CLASSW,
		NULL, upDownStyle, 0, 0, 0, 0,
		_W_WIDGET(widget)->handle, 0, hinst,
		NULL);
		if (_W_SPINNER(widget)->hwndUpDown != 0) {
			int flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
			SetWindowPos(_W_SPINNER(widget)->hwndText,
			_W_SPINNER(widget)->hwndUpDown, 0, 0, 0, 0, flags);
			SetWindowLongPtrW(_W_SPINNER(widget)->hwndUpDown, GWLP_USERDATA,
					(LONG_PTR) widget);
			/*if (OS.IsDBLocale) {
			 long hIMC = OS.ImmGetContext (handle);
			 OS.ImmAssociateContext (hwndText, hIMC);
			 OS.ImmAssociateContext (hwndUpDown, hIMC);
			 OS.ImmReleaseContext (handle, hIMC);
			 }*/
			SendMessageW(_W_SPINNER(widget)->hwndUpDown, UDM_SETRANGE32, 0,
					100);
			int msg;
#if IsWinCE
	msg = UDM_SETPOS;
#else
			msg = UDM_SETPOS32;
#endif
			SendMessageW(_W_SPINNER(widget)->hwndUpDown, msg, 0, 0);
			_W_SPINNER(widget)->pageIncrement = 10;
			_W_SPINNER(widget)->digits = 0;
			SetWindowTextW(_W_SPINNER(widget)->hwndText, L"0");
		}
		return result;
	}
	DestroyWindow(_W_WIDGET(widget)->handle);
	return W_ERROR_NO_HANDLES;
}
int _SPINNER_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int ret = _COMPOSITE_WM_SIZE(widget, e,reserved);
	int width = LOWORD(((_w_event_platform* ) e)->lparam);
	int height = HIWORD(((_w_event_platform* ) e)->lparam);
	int upDownWidth = GetSystemMetrics(SM_CXVSCROLL);
	int textWidth = width - upDownWidth;
	int border = GetSystemMetrics(SM_CXEDGE);
	int flags = SWP_NOZORDER | SWP_DRAWFRAME | SWP_NOACTIVATE;
	SetWindowPos(_W_SPINNER(widget)->hwndText, 0, 0, 0,
			textWidth + border, height, flags);
	SetWindowPos(_W_SPINNER(widget)->hwndUpDown, 0, textWidth, 0,
			upDownWidth, height, flags);
	return ret;
}
void _w_spinner_class_init(struct _w_spinner_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SPINNER;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_spinner_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_spinner);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_spinner);
	/*
	 * public function
	 */
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_spinner_create;
}
#endif
