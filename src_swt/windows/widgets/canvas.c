/*
 * canvas.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "canvas.h"
#include "toolkit.h"
/*
 * Caret
 */
wresult _w_caret_is_ok(w_widgetdata *obj) {
	return W_TRUE;
}
void _w_caret_close(w_widgetdata *obj) {
}
void _w_caret_copy(w_widgetdata *from, w_widgetdata *to) {
}
wresult _w_caret_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
}
wresult _w_caret_get_bounds(w_caret *caret, w_point *location, w_size *size) {
}
wresult _w_caret_get_font(w_caret *caret, w_font **font) {
}
wresult _w_caret_get_image(w_caret *caret, w_image *image) {
}
wresult _w_caret_get_parent(w_caret *caret, w_canvas **parent) {
}
wresult _w_caret_get_visible(w_caret *caret) {
}
wresult _w_caret_is_visible(w_caret *caret) {
}
wresult _w_caret_set_bounds(w_caret *caret, w_point *location, w_size *size) {
}
wresult _w_caret_set_font(w_caret *caret, w_font *font) {
}
wresult _w_caret_set_image(w_caret *caret, w_image *image) {
}
wresult _w_caret_set_visible(w_caret *caret, int visible) {
}
/*
 * IME
 */
wresult _w_ime_is_ok(w_widgetdata *obj) {
}
void _w_ime_close(w_widgetdata *obj) {
}
void _w_ime_copy(w_widgetdata *from, w_widgetdata *to) {
}
wresult _w_ime_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
}
wresult _w_ime_get_caret_offset(w_ime *ime) {
}
wresult _w_ime_get_commit_count(w_ime *ime) {
}
wresult _w_ime_get_composition_offset(w_ime *ime) {
}
wresult _w_ime_get_ranges(w_ime *ime, w_iterator *ranges) {
}
wresult _w_ime_get_text(w_ime *ime, w_alloc alloc, void *user_data) {
}
wresult _w_ime_get_wide_caret(w_ime *ime) {
}
wresult _w_ime_set_composition_offset(w_ime *ime, int offset) {
}
/*
 * Canvas
 */
w_caret* _w_canvas_get_caret(w_canvas *canvas) {
	return _W_CANVAS(canvas)->caret;

}
wresult _w_canvas_create_caret(w_canvas *canvas, w_caret *caret) {
	memset(caret, 0, sizeof(_w_caret));
	_W_CARET(caret)->clazz = &win_toolkit->class_caret;
	_W_CARET(caret)->isVisible = W_TRUE;
	_W_CARET(caret)->parent = canvas;
	if (_W_CANVAS(canvas)->caret == 0) {
		_w_canvas_set_caret(canvas, caret);
	}
	return W_TRUE;
}
wresult _w_canvas_create_ime(w_canvas *canvas, w_ime *ime) {
	memset(ime, 0, sizeof(_w_ime));
	_W_IME(ime)->text = "";
	_W_IME(ime)->startOffset = -1;
	if (_W_CANVAS(canvas)->ime == 0) {
		_w_canvas_set_ime(canvas, ime);
	}
	return W_TRUE;
}
wresult _w_canvas_scroll(w_canvas *canvas, w_point *_dest, w_rect *_rect,
		int all) {

}
wresult _w_canvas_set_caret(w_canvas *canvas, w_caret *caret) {
	w_caret *newCaret = caret;
	w_caret *oldCaret = _W_CANVAS(canvas)->caret;
	_W_CANVAS(canvas)->caret = newCaret;
	if (W_CONTROL(canvas)
			== _w_toolkit_get_focus_control(W_TOOLKIT(win_toolkit))) {
		if (oldCaret != 0)
			_w_caret_kill_focus(oldCaret);
		if (newCaret != 0) {
			if (!w_widgetdata_is_ok(W_WIDGETDATA(newCaret)))
				return W_ERROR_INVALID_ARGUMENT;
			_w_caret_set_focus(newCaret);
		}
	}
	return W_TRUE;
}
wresult _w_canvas_set_ime(w_canvas *canvas, w_ime *ime) {
	if (ime != 0 && !w_widgetdata_is_ok(W_WIDGETDATA(ime)))
		return W_ERROR_INVALID_ARGUMENT;
	_W_CANVAS(canvas)->ime = ime;
	return W_TRUE;
}
w_ime* _w_canvas_get_ime(w_canvas *canvas) {
	return _W_CANVAS(canvas)->ime;
}
wresult _w_canvas_set_font(w_control *control, w_font *font) {
	w_caret *caret = _W_CANVAS(control)->caret;
	if (caret != 0)
		_w_caret_set_font(caret, font);
	return _w_control_set_font(control, font);
}
const char* _w_canvas_window_class(w_control *control,
		_w_control_reserved *reserved) {
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
	W_CONTROL_CLASS(clazz)->set_font = _w_canvas_set_font;
	clazz->create_caret = _w_canvas_create_caret;
	clazz->create_ime = _w_canvas_create_ime;
	clazz->scroll = _w_canvas_scroll;
	clazz->get_caret = _w_canvas_get_caret;
	clazz->set_caret = _w_canvas_set_caret;
	clazz->get_ime = _w_canvas_get_ime;
	clazz->set_ime = _w_canvas_set_ime;
	/*
	 * caret class
	 */
	if (win_toolkit->class_caret.widgetdata.toolkit == 0) {
		struct _w_caret_class *caret = &win_toolkit->class_caret;
		caret->widgetdata.toolkit = W_TOOLKIT(win_toolkit);
		caret->widgetdata.is_ok = _w_widgetdata_is_ok;
		caret->widgetdata.close = _w_caret_close;
		caret->widgetdata.copy = _w_caret_copy;
		caret->widgetdata.equals = _w_caret_equals;
		caret->get_bounds = _w_caret_get_bounds;
		caret->get_font = _w_caret_get_font;
		caret->get_image = _w_caret_get_image;
		caret->get_parent = _w_caret_get_parent;
		caret->get_visible = _w_caret_get_visible;
		caret->is_visible = _w_caret_is_visible;
		caret->set_bounds = _w_caret_set_bounds;
		caret->set_font = _w_caret_set_font;
		caret->set_image = _w_caret_set_image;
		caret->set_visible = _w_caret_set_visible;

	}
	/*
	 * IME class
	 */
	if (win_toolkit->class_ime.widgetdata.toolkit == 0) {
		struct _w_ime_class *ime = &win_toolkit->class_ime;
		ime->widgetdata.toolkit = W_TOOLKIT(win_toolkit);
		ime->widgetdata.is_ok = _w_widgetdata_is_ok;
		ime->widgetdata.close = _w_ime_close;
		ime->widgetdata.copy = _w_ime_copy;
		ime->widgetdata.equals = _w_ime_equals;
		ime->get_caret_offset = _w_ime_get_caret_offset;
		ime->get_commit_count = _w_ime_get_commit_count;
		ime->get_composition_offset = _w_ime_get_composition_offset;
		ime->get_ranges = _w_ime_get_ranges;
		ime->get_text = _w_ime_get_text;
		ime->get_wide_caret = _w_ime_get_wide_caret;
		ime->set_composition_offset = _w_ime_set_composition_offset;

	}
	/*
	 * reserved
	 */
	_w_canvas_reserved *reserved = _W_CANVAS_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_CONTROL_RESERVED(reserved)->window_class = _w_canvas_window_class;
	//messages
	dispatch_message *msg = _W_CONTROL_RESERVED(reserved)->messages;
	msg[_WM_CHAR] = _CANVAS_WM_CHAR;
	msg[_WM_IME_COMPOSITION] = _CANVAS_WM_IME_COMPOSITION;
	msg[_WM_IME_COMPOSITION_START] = _CANVAS_WM_IME_COMPOSITION_START;
	msg[_WM_IME_ENDCOMPOSITION] = _CANVAS_WM_IME_ENDCOMPOSITION;
	msg[_WM_INPUTLANGCHANGE] = _CANVAS_WM_INPUTLANGCHANGE;
	msg[_WM_KEYDOWN] = _CANVAS_WM_KEYDOWN;
	msg[_WM_KILLFOCUS] = _CANVAS_WM_KILLFOCUS;
	msg[_WM_LBUTTONDOWN] = _CANVAS_WM_LBUTTONDOWN;
	msg[_WM_SETFOCUS] = _CANVAS_WM_SETFOCUS;
	msg[_WM_SIZE] = _CANVAS_WM_SIZE;
	msg[_WM_WINDOWPOSCHANGED] = _CANVAS_WM_WINDOWPOSCHANGED;
	msg[_WM_WINDOWPOSCHANGING] = _CANVAS_WM_WINDOWPOSCHANGING;
}
#endif

