/*
 * label.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#if defined( __WIN32__) || defined(__WIN64__)
#include "label.h"
#include "../widgets/toolkit.h"
w_image* _w_label_get_image(w_label *label) {

}
w_string_ref* _w_label_get_text(w_label *label) {

}
wresult _w_label_set_image(w_label *label, w_image *image) {

}
wresult _w_label_set_text(w_label *label, const char *string) {
	if (string == 0)
		string = "";
	struct UnicodeString str;
	unicode_set(&str, string, -1);
	WINBOOL result = SetWindowTextW(_W_WIDGET(label)->handle, str.str);
	unicode_free(&str);
	if (result)
		return W_TRUE;
	else
		return W_ERROR_CANNOT_SET_TEXT;
}
int _w_label_check_style(int style) {
	style |= W_NO_FOCUS;
	if ((style & W_SEPARATOR) != 0) {
		style = _w_widget_check_bits(style, W_VERTICAL, W_HORIZONTAL, 0, 0, 0,
				0);
		return _w_widget_check_bits(style, W_SHADOW_OUT, W_SHADOW_IN,
				W_SHADOW_NONE, 0, 0, 0);
	}
	return _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
}
void _w_label_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_control_style(widget, parent, style, dwExStyle, dwStyle);
	*dwExStyle &= ~WS_EX_CLIENTEDGE;
	if ((style & W_BORDER) != 0) {
		*dwExStyle |= WS_EX_STATICEDGE;
	}
	*dwStyle |= SS_NOTIFY;
	if ((style & W_SEPARATOR) != 0) {
		*dwStyle |= SS_OWNERDRAW;
		return;
	}
	if (WIN32_VERSION >= VERSION(5, 0)) {
		if ((style & W_WRAP) != 0)
			*dwStyle |= SS_EDITCONTROL;
	}
	if ((style & W_CENTER) != 0) {
		*dwStyle |= SS_CENTER;
		return;
	}
	if ((style & W_RIGHT) != 0) {
		*dwStyle |= SS_RIGHT;
		return;
	}
	if ((style & W_WRAP) != 0) {
		*dwStyle |= SS_LEFT;
		return;
	}
	*dwStyle |= SS_LEFTNOWORDWRAP;
}
wresult _w_label_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_label_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_label_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_STATICA, dwExStyle, dwStyle);
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
int _w_label_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	int border = _w_control_get_border_width(W_CONTROL(widget));
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) != 0) {
		int lineWidth = GetSystemMetrics(SM_CXBORDER);
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			width = DEFAULT_WIDTH;
			height = lineWidth * 2;
		} else {
			width = lineWidth * 2;
			height = DEFAULT_HEIGHT;
		}
		if (wHint != W_DEFAULT)
			width = wHint;
		if (hHint != W_DEFAULT)
			height = hHint;
		width += border * 2;
		height += border * 2;
		e->size->width = width;
		e->size->height = height;
		return W_TRUE;
	}
	int bits = GetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE);
	boolean drawText = TRUE;
	boolean drawImage = (bits & SS_OWNERDRAW) == SS_OWNERDRAW;
	if (drawImage) {
		/*if (image != null) {
		 Rectangle rect = image.getBoundsInPixels();
		 width += rect.width;
		 height += rect.height;
		 if (IMAGE_AND_TEXT) {
		 if (text.length () != 0) width += MARGIN;
		 } else {
		 drawText = false;
		 }
		 }*/
	}
	if (drawText) {
		HDC hDC = GetDC(_W_WIDGET(widget)->handle);
		HFONT newFont = SendMessageW(_W_WIDGET(widget)->handle, WM_GETFONT, 0,
				0);
		HFONT oldFont = SelectObject(hDC, newFont);
		int length = GetWindowTextLengthW(_W_WIDGET(widget)->handle);
		if (length == 0) {
			TEXTMETRICW tm;
			GetTextMetricsW(hDC, &tm);
			height = w_max(height, tm.tmHeight);
		} else {
			RECT rect;
			memset(&rect, 0, sizeof(RECT));
			int flags = DT_CALCRECT | DT_EDITCONTROL | DT_EXPANDTABS;
			if ((_W_WIDGET(widget)->style & W_WRAP) != 0
					&& wHint != W_DEFAULT) {
				flags |= DT_WORDBREAK;
				rect.right = w_max(0, wHint - width);
			}
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
			}
			if (l == 0 && buffer != 0) {
				free(buffer);
			}
			width += rect.right - rect.left;
			height = w_max(height, rect.bottom - rect.top);
		}
		if (newFont != 0)
			SelectObject(hDC, oldFont);
		ReleaseDC(_W_WIDGET(widget)->handle, hDC);
	}
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	width += border * 2;
	height += border * 2;
	/*
	 * Feature in WinCE PPC.  Text labels have a trim
	 * of one pixel wide on the right and left side.
	 * The fix is to increase the width to include
	 * this trim.
	 */
#if IsWinCE
	if (!drawImage) width += 2;
#endif
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
void _w_label_class_init(struct _w_label_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LABEL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_label_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_label);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_label);
	/*
	 * public function
	 */
	clazz->get_image = _w_label_get_image;
	clazz->get_text = _w_label_get_text;
	clazz->set_image = _w_label_set_image;
	clazz->set_text = _w_label_set_text;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_label_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->compute_size = _w_label_compute_size;
}
#endif
