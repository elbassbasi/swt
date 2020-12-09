/*
 * canvas.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "canvas.h"
w_caret* _w_canvas_get_caret(w_canvas *canvas) {

}
wresult _w_canvas_set_caret(w_canvas *canvas, w_caret *caret) {

}
const char* _w_canvas_window_class(w_control *control,
		_w_control_reserved *reserved){
	//if (display.useOwnDC) return display.windowOwnDCClass;
	return _w_scrollable_window_class(control, reserved);
}
int _CANVAS_WM_CHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _WIDGET_WM_CHAR(widget, e, reserved);
	/* if (result != null) return result;
	 if (caret != null) {
	 switch ((int)wParam) {
	 case SWT.DEL:
	 case SWT.BS:
	 case SWT.ESC:
	 break;
	 default: {
	 if (OS.GetKeyState (OS.VK_CONTROL) >= 0) {
	 int [] value = new int [1];
	 if (OS.SystemParametersInfo (OS.SPI_GETMOUSEVANISH, 0, value, 0)) {
	 if (value [0] != 0) OS.SetCursor (0);
	 }
	 }
	 }
	 }
	 }*/
	return result;
}

int _CANVAS_WM_IME_COMPOSITION(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (ime != null) {
	 LRESULT result = ime.WM_IME_COMPOSITION (wParam, lParam);
	 if (result != null) return result;
	 }
	 return super.WM_IME_COMPOSITION (wParam, lParam);*/
	return W_FALSE;
}

int _CANVAS_WM_IME_COMPOSITION_START(w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	/*if (ime != null) {
	 LRESULT result = ime.WM_IME_COMPOSITION_START (wParam, lParam);
	 if (result != null) return result;
	 }
	 return super.WM_IME_COMPOSITION_START (wParam, lParam);*/
	return W_FALSE;
}

int _CANVAS_WM_IME_ENDCOMPOSITION(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (ime != null) {
	 LRESULT result = ime.WM_IME_ENDCOMPOSITION (wParam, lParam);
	 if (result != null) return result;
	 }
	 return super.WM_IME_ENDCOMPOSITION (wParam, lParam);*/
	return W_FALSE;
}

int _CANVAS_WM_INPUTLANGCHANGE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result  = super.WM_INPUTLANGCHANGE (wParam, lParam);
	 if (caret != null && caret.isFocusCaret ()) {
	 caret.setIMEFont ();
	 caret.resizeIME ();
	 }
	 return result;*/
	return W_FALSE;
}

int _CANVAS_WM_KEYDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _WIDGET_WM_KEYDOWN(widget, e, reserved);
	/*LRESULT result = super.WM_KEYDOWN (wParam, lParam);
	 if (result != null) return result;
	 if (ime != null) {
	 ime.WM_KEYDOWN (wParam, lParam);
	 }*/
	return result;
}

int _CANVAS_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (ime != null) {
	 LRESULT result = ime.WM_KILLFOCUS (wParam, lParam);
	 if (result != null) return result;
	 }
	 Caret caret = this.caret;
	 LRESULT result  = super.WM_KILLFOCUS (wParam, lParam);
	 if (caret != null) caret.killFocus ();
	 return result;*/
	return W_FALSE;
}

int _CANVAS_WM_LBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (ime != null) {
	 LRESULT result = ime.WM_LBUTTONDOWN (wParam, lParam);
	 if (result != null) return result;
	 }*/
	return _COMPOSITE_WM_LBUTTONDOWN(widget, e, reserved);
}

int _CANVAS_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result  = super.WM_SETFOCUS (wParam, lParam);
	 if (caret != null && caret.isFocusCaret ()) caret.setFocus ();
	 return result;*/
	return W_FALSE;
}

int _CANVAS_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _COMPOSITE_WM_SIZE(widget, e, reserved);
	//if (caret != null && caret.isFocusCaret ()) caret.resizeIME ();
	return result;
}

int _CANVAS_WM_WINDOWPOSCHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	//LRESULT result  = super.WM_WINDOWPOSCHANGED (wParam, lParam);
	//if (result != null) return result;
	/*
	 * Bug in Windows.  When a window with style WS_EX_LAYOUTRTL
	 * that contains a caret is resized, Windows does not move the
	 * caret in relation to the mirrored origin in the top right.
	 * The fix is to hide the caret in WM_WINDOWPOSCHANGING and
	 * show the caret in WM_WINDOWPOSCHANGED.
	 */
	/*boolean isFocus = (style & SWT.RIGHT_TO_LEFT) != 0 && caret != null && caret.isFocusCaret ();
	 if (isFocus) caret.setFocus ();
	 return result;*/
	return W_FALSE;
}

int _CANVAS_WM_WINDOWPOSCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT result  = super.WM_WINDOWPOSCHANGING (wParam, lParam);
	 if (result != null) return result;*/
	/*
	 * Bug in Windows.  When a window with style WS_EX_LAYOUTRTL
	 * that contains a caret is resized, Windows does not move the
	 * caret in relation to the mirrored origin in the top right.
	 * The fix is to hide the caret in WM_WINDOWPOSCHANGING and
	 * show the caret in WM_WINDOWPOSCHANGED.
	 */
	/*boolean isFocus = (style & SWT.RIGHT_TO_LEFT) != 0 && caret != null && caret.isFocusCaret ();
	 if (isFocus) caret.killFocus ();
	 return result;*/
	return W_FALSE;
}

void _w_canvas_class_init(struct _w_canvas_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_canvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_canvas);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_canvas);
	/**
	 *
	 */
	clazz->get_caret = _w_canvas_get_caret;
	clazz->set_caret = _w_canvas_set_caret;
	/*
	 * reserved
	 */
	struct _w_canvas_reserved *reserved = _W_CANVAS_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_CONTROL_RESERVED(reserved)->window_class = _w_canvas_window_class;
	//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->messages[_WM_CHAR] = _CANVAS_WM_CHAR;
	msg->messages[_WM_IME_COMPOSITION] = _CANVAS_WM_IME_COMPOSITION;
	msg->messages[_WM_IME_COMPOSITION_START] =
			_CANVAS_WM_IME_COMPOSITION_START;
	msg->messages[_WM_IME_ENDCOMPOSITION] = _CANVAS_WM_IME_ENDCOMPOSITION;
	msg->messages[_WM_INPUTLANGCHANGE] = _CANVAS_WM_INPUTLANGCHANGE;
	msg->messages[_WM_KEYDOWN] = _CANVAS_WM_KEYDOWN;
	msg->messages[_WM_KILLFOCUS] = _CANVAS_WM_KILLFOCUS;
	msg->messages[_WM_LBUTTONDOWN] = _CANVAS_WM_LBUTTONDOWN;
	msg->messages[_WM_SETFOCUS] = _CANVAS_WM_SETFOCUS;
	msg->messages[_WM_SIZE] = _CANVAS_WM_SIZE;
	msg->messages[_WM_WINDOWPOSCHANGED] = _CANVAS_WM_WINDOWPOSCHANGED;
	msg->messages[_WM_WINDOWPOSCHANGING] = _CANVAS_WM_WINDOWPOSCHANGING;
}
#endif

