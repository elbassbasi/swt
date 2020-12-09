/*
 * text.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "text.h"
#include "../widgets/toolkit.h"
wresult _w_text_append(w_text *text, const char *string) {
}
wresult _w_text_clear_selection(w_text *text) {
}
wresult _w_text_copy(w_text *text) {
}
wresult _w_text_cut(w_text *text) {
}
int _w_text_get_caret_line_number(w_text *text) {
}
wresult _w_text_get_caret_location(w_text *text, w_point *location) {
}
int _w_text_get_caret_position(w_text *text) {
}
int _w_text_get_char_count(w_text *text) {
}
wresult _w_text_get_doubleclick_enabled(w_text *text) {
}
char _w_text_get_echo_char(w_text *text) {
}
wresult _w_text_get_editable(w_text *text) {
}
int _w_text_get_line_count(w_text *text) {
}
w_string_ref* _w_text_get_line_delimiter(w_text *text) {
}
int _w_text_get_line_height(w_text *text) {
}
w_string_ref* _w_text_get_message(w_text *text) {
}
int _w_text_get_position(w_text *text, w_point *point) {
}
wresult _w_text_get_selection(w_text *text, w_range *range) {
}
int _w_text_get_selection_count(w_text *text) {
}
w_string_ref* _w_text_get_selection_text(w_text *text) {
}
int _w_text_get_tabs(w_text *text) {
}
w_string_ref* _w_text_get_text(w_text *text) {
}
w_string_ref* _w_text_get_text_range(w_text *text, w_range *range) {
}
int _w_text_get_text_limit(w_text *text) {
}
int _w_text_get_top_index(w_text *text) {
}
int _w_text_get_top_pixel(w_text *text) {
}
wresult _w_text_insert(w_text *text, const char *string) {
}
wresult _w_text_paste(w_text *text) {
}
wresult _w_text_select_all(w_text *text) {
}
wresult _w_text_set_doubleclick_enabled(w_text *text, int doubleClick) {
}
wresult _w_text_set_echochar(w_text *text, char echo) {
}
wresult _w_text_set_editable(w_text *text, int editable) {
}
wresult _w_text_set_message(w_text *text, const char *message) {
}
wresult _w_text_set_selection(w_text *text, int start) {
}
wresult _w_text_set_selection_range(w_text *text, w_range *selection) {
}
wresult _w_text_set_tabs(w_text *text, int tabs) {
}
wresult _w_text_set_text(w_text *text, const char *string) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	/*string = Display.withCrLf (string);
	 if (hooks (SWT.Verify) || filters (SWT.Verify)) {
	 int length = OS.GetWindowTextLength (handle);
	 string = verifyText (string, 0, length, null);
	 if (string == null) return;
	 }
	 clearSegments (false);
	 int limit = (int)OS.SendMessage (handle, OS.EM_GETLIMITTEXT, 0, 0) & 0x7FFFFFFF;
	 if (string.length () > limit) string = string.substring (0, limit);*/
	struct UnicodeString str;
	unicode_set(&str, string, -1);
	SetWindowTextW(_W_WIDGET(text)->handle, str.str);
	unicode_free(&str);
	/*if ((state & HAS_AUTO_DIRECTION) != 0) {
	 super.updateTextDirection(AUTO_TEXT_DIRECTION);
	 }
	 applySegments ();*/
	/*
	 * Bug in Windows.  When the widget is multi line
	 * text widget, it does not send a WM_COMMAND with
	 * control code EN_CHANGE from SetWindowText () to
	 * notify the application that the text has changed.
	 * The fix is to send the event.
	 */
	/*int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
	 if ((bits & OS.ES_MULTILINE) != 0) {
	 sendEvent (SWT.Modify);
	 // widget could be disposed at this point
	 }*/
	return W_TRUE;
}
wresult _w_text_set_text_limit(w_text *text, int limit) {
}
wresult _w_text_set_top_index(w_text *text, int index) {
}
wresult _w_text_show_selection(w_text *text) {
}
/*
 * private
 */
int _w_text_check_style(int style) {
	if ((style & W_SEARCH) != 0) {
		style |= W_SINGLE | W_BORDER;
		style &= ~W_PASSWORD;
		/*
		 * NOTE: ICON_CANCEL has the same value as H_SCROLL and
		 * ICON_SEARCH has the same value as V_SCROLL so they are
		 * cleared because SWT.SINGLE is set.
		 */
	}
	if ((style & W_SINGLE) != 0 && (style & W_MULTI) != 0) {
		style &= ~W_MULTI;
	}
	style = _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
	if ((style & W_SINGLE) != 0)
		style &= ~(W_HSCROLL | W_VSCROLL | W_WRAP);
	if ((style & W_WRAP) != 0) {
		style |= W_MULTI;
		style &= ~W_HSCROLL;
	}
	if ((style & W_MULTI) != 0)
		style &= ~W_PASSWORD;
	if ((style & (W_SINGLE | W_MULTI)) != 0)
		return style;
	if ((style & (W_HSCROLL | W_VSCROLL)) != 0)
		return style | W_MULTI;
	return style | W_SINGLE;
}
void _w_text_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_scrollable_style(widget, parent, style, dwExStyle, dwStyle);
	if (style & W_BORDER) {
		*dwExStyle |= WS_EX_CLIENTEDGE;
	}
	*dwStyle |= ES_AUTOHSCROLL;
	if ((style & W_PASSWORD) != 0)
		*dwStyle |= ES_PASSWORD;
	if ((style & W_CENTER) != 0)
		*dwStyle |= ES_CENTER;
	if ((style & W_RIGHT) != 0)
		*dwStyle |= ES_RIGHT;
	if ((style & W_READ_ONLY) != 0)
		*dwStyle |= ES_READONLY;
	if ((style & W_SINGLE) != 0) {
		/*
		 * Feature in Windows.  When a text control is read-only,
		 * uses COLOR_3DFACE for the background .  If the text
		 * controls single-line and is within a tab folder or
		 * some other themed control, using WM_ERASEBKGND and
		 * WM_CTRCOLOR to draw the theme background results in
		 * pixel corruption.  The fix is to use an ES_MULTILINE
		 * text control instead.
		 * Refer Bug438901:- ES_MULTILINE doesn't apply for:
		 * W_PASSWORD | W_READ_ONLY style combination.
		 */
		if ((style & W_READ_ONLY) != 0) {
			if ((style & (W_BORDER | W_HSCROLL | W_VSCROLL | W_PASSWORD))
					== 0) {
				if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
					*dwStyle |= ES_MULTILINE;
				}
			}
		}
		return;
	}
	*dwStyle |= ES_MULTILINE | ES_NOHIDESEL | ES_AUTOVSCROLL;
	if ((style & W_WRAP) != 0)
		*dwStyle &= ~(WS_HSCROLL | ES_AUTOHSCROLL);
}
/*
 * public
 */
wresult _w_text_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_text_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_text_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_EDITA, dwExStyle, dwStyle);
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
int _w_text_compute_trim(w_widget *widget, struct w_event_compute_trim *e,
		struct _w_widget_reserved *reserved) {
	int result = _w_scrollable_compute_trim(widget, e, reserved);
	/*
	 * The preferred height of a single-line text widget
	 * has been hand-crafted to be the same height as
	 * the single-line text widget in an editable combo
	 * box.
	 */
	WPARAM margins = SendMessageW(_W_WIDGET(widget)->handle, EM_GETMARGINS, 0,
			0);
	e->result->x -= LOWORD(margins);
	e->result->width += LOWORD (margins) + HIWORD(margins);
	if ((_W_WIDGET(widget)->style & W_HSCROLL) != 0)
		e->result->width++;
	if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
		e->result->x -= 1;
		e->result->y -= 1;
		e->result->width += 2;
		e->result->height += 2;
	}
	return result;
}
int _w_text_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	//int border = _w_control_get_border_width(W_CONTROL(widget));
	if (wHint == W_DEFAULT || hHint == W_DEFAULT) {
		HFONT newFont, oldFont = 0;
		HDC hDC = GetDC(_W_WIDGET(widget)->handle);
		newFont = (HFONT) SendMessageW(_W_WIDGET(widget)->handle, WM_GETFONT,
				0, 0);
		if (newFont != 0)
			oldFont = SelectObject(hDC, newFont);
		TEXTMETRICW tm;
		GetTextMetricsW(hDC, &tm);
		int count =
				(_W_WIDGET(widget)->style & W_SINGLE) != 0 ?
						1 : SendMessageW(_W_WIDGET(widget)->handle,
						EM_GETLINECOUNT, 0, 0);
		height = count * tm.tmHeight;
		RECT rect;
		memset(&rect, 0, sizeof(rect));
		int flags = DT_CALCRECT | DT_EDITCONTROL | DT_NOPREFIX;
		boolean wrap = (_W_WIDGET(widget)->style & W_MULTI) != 0
				&& (_W_WIDGET(widget)->style & W_WRAP) != 0;
		if (wrap && wHint != W_DEFAULT) {
			flags |= DT_WORDBREAK;
			rect.right = wHint;
		}
		int length = GetWindowTextLengthW(_W_WIDGET(widget)->handle);
		if (length != 0) {
			int l;
			if (length < 0x100) {
				l = length + 1;
			} else
				l = 0;
			WCHAR *buffer, tmp[l];
			if (l == 0) {
				buffer = malloc((length + 1) * sizeof(WCHAR));
			} else {
				buffer = tmp;
			}
			if (buffer != 0) {
				GetWindowTextW(_W_WIDGET(widget)->handle, buffer, length + 1);
				DrawTextW(hDC, buffer, length, &rect, flags);
				//buffer.clear ();
				if (l == 0 && buffer != 0) {
					free(buffer);
				}
			}
			width = rect.right - rect.left;
		}
		if (wrap && hHint == W_DEFAULT) {
			int newHeight = rect.bottom - rect.top;
			if (newHeight != 0)
				height = newHeight;
		}
		/*if ((_W_WIDGET(widget)->style & W_SINGLE) != 0 && message.length () > 0) {
		 SetRect (rect, 0, 0, 0, 0);
		 TCHAR buffer = new TCHAR (getCodePage (), message, false);
		 DrawText (hDC, buffer, buffer.length (), rect, flags);
		 width = w_max (width, rect.right - rect.left);
		 }*/
		if (newFont != 0)
			SelectObject(hDC, oldFont);
		ReleaseDC(_W_WIDGET(widget)->handle, hDC);
	}
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	struct w_event_compute_trim ee;
	w_rect rect;
	w_rect result;
	ee.result = &result;
	ee.rect = &rect;
	rect.x = rect.y = 0;
	rect.width = width;
	rect.height = height;
	_w_text_compute_trim(widget, &ee, reserved);
	/*Rectangle trim = computeTrimInPixels (0, 0, width, height);
	 return new Point (trim.width, trim.height);*/
	e->size->width = ee.result->width;
	e->size->height = ee.result->height;
	return W_TRUE;
}
/**
 * messages
 */
int _TEXT_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_CLEAR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_CUT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_IME_CHAR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_PASTE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_UNDO(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_CTLCOLORCHILD(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_COMMANDCHILD(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
int _TEXT_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
void _w_text_class_init(struct _w_text_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TEXT;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_text_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_text);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_text);
	/*
	 * public function
	 */
	/*
	 * public function
	 */
	clazz->append = _w_text_append;
	clazz->clear_selection = _w_text_clear_selection;
	clazz->copy = _w_text_copy;
	clazz->cut = _w_text_cut;
	clazz->get_caret_line_number = _w_text_get_caret_line_number;
	clazz->get_caret_location = _w_text_get_caret_location;
	clazz->get_caret_position = _w_text_get_caret_position;
	clazz->get_char_count = _w_text_get_char_count;
	clazz->get_doubleclick_enabled = _w_text_get_doubleclick_enabled;
	clazz->get_echo_char = _w_text_get_echo_char;
	clazz->get_editable = _w_text_get_editable;
	clazz->get_line_count = _w_text_get_line_count;
	clazz->get_line_delimiter = _w_text_get_line_delimiter;
	clazz->get_line_height = _w_text_get_line_height;
	clazz->get_message = _w_text_get_message;
	clazz->get_position = _w_text_get_position;
	clazz->get_selection = _w_text_get_selection;
	clazz->get_selection_count = _w_text_get_selection_count;
	clazz->get_selection_text = _w_text_get_selection_text;
	clazz->get_tabs = _w_text_get_tabs;
	clazz->get_text = _w_text_get_text;
	clazz->get_text_range = _w_text_get_text_range;
	clazz->get_text_limit = _w_text_get_text_limit;
	clazz->get_top_index = _w_text_get_top_index;
	clazz->get_top_pixel = _w_text_get_top_pixel;
	clazz->insert = _w_text_insert;
	clazz->paste = _w_text_paste;
	clazz->select_all = _w_text_select_all;
	clazz->set_doubleclick_enabled = _w_text_set_doubleclick_enabled;
	clazz->set_echochar = _w_text_set_echochar;
	clazz->set_editable = _w_text_set_editable;
	clazz->set_message = _w_text_set_message;
	clazz->set_selection = _w_text_set_selection;
	clazz->set_selection_range = _w_text_set_selection_range;
	clazz->set_tabs = _w_text_set_tabs;
	clazz->set_text = _w_text_set_text;
	clazz->set_text_limit = _w_text_set_text_limit;
	clazz->set_top_index = _w_text_set_top_index;
	clazz->show_selection = _w_text_show_selection;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_text_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->compute_size = _w_text_compute_size;
	reserved->compute_trim = _w_text_compute_trim;
	//messages
	struct _w_control_reserved *msg = reserved;
	msg->messages[_WM_CHAR] = _TEXT_WM_CHAR;
	msg->messages[_WM_CLEAR] = _TEXT_WM_CLEAR;
	msg->messages[_WM_CUT] = _TEXT_WM_CUT;
	msg->messages[_WM_ERASEBKGND] = _TEXT_WM_ERASEBKGND;
	msg->messages[_WM_GETDLGCODE] = _TEXT_WM_GETDLGCODE;
	msg->messages[_WM_GETOBJECT] = _TEXT_WM_GETOBJECT;
	msg->messages[_WM_IME_CHAR] = _TEXT_WM_IME_CHAR;
	msg->messages[_WM_LBUTTONDBLCLK] = _TEXT_WM_LBUTTONDBLCLK;
	msg->messages[_WM_LBUTTONDOWN] = _TEXT_WM_LBUTTONDOWN;
	msg->messages[_WM_PASTE] = _TEXT_WM_PASTE;
	msg->messages[_WM_UNDO] = _TEXT_WM_UNDO;
	msg->messages[_WM_CTLCOLORCHILD] = _TEXT_WM_CTLCOLORCHILD;
	msg->messages[_WM_COMMANDCHILD] = _TEXT_WM_COMMANDCHILD;
	msg->messages[_WM_KEYDOWN] = _TEXT_WM_KEYDOWN;
}
#endif
