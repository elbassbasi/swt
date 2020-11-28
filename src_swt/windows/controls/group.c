/*
 * group.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "group.h"
#include "../widgets/toolkit.h"
#define CLIENT_INSET 3
/*
 * private
 */
int _w_group_check_style(int style) {
	style |= W_NO_FOCUS;
	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	return style & ~(W_HSCROLL | W_VSCROLL);
}
void _w_group_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	/*
	 * Bug in Windows.  When GetDCEx() is called with DCX_INTERSECTUPDATE,
	 * the HDC that is returned does not include the current update region.
	 * This was confirmed under DEBUG Windows when GetDCEx() complained about
	 * invalid flags.  Therefore, it is not easily possible to get an HDC from
	 * outside of WM_PAINT that includes the current damage and clips children.
	 * Because the receiver has children and draws a frame and label, it is
	 * necessary that the receiver always draw clipped, in the current damaged
	 * area.  The fix is to force the receiver to be fully clipped by including
	 * WS_CLIPCHILDREN and WS_CLIPSIBLINGS in the default style bits.
	 */
	*dwStyle |= BS_GROUPBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
}
wresult _w_group_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_group_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_group_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_BUTTONA, dwExStyle, dwStyle);
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
wresult _w_group_get_text(w_group *group,w_alloc* text) {

}
wresult _w_group_set_text(w_group *group, const char *string) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	//if ((state & HAS_AUTO_DIRECTION) == 0 || !updateTextDirection (AUTO_TEXT_DIRECTION)) {
	//string = fixText (OS.IsWindowEnabled (handle));
	struct UnicodeString str;
	unicode_set(&str, string, -1);
	SetWindowTextW(_W_WIDGET(group)->handle, str.str);
	unicode_free(&str);
	//}
	return W_TRUE;
}
int _w_group_get_client_area(w_widget *widget, w_event_client_area *e,
		_w_widget_reserved *reserved) {
	RECT rect;
	GetClientRect(_W_WIDGET(widget)->handle, &rect);
	HFONT newFont, oldFont = 0;
	HDC hDC = GetDC(_W_WIDGET(widget)->handle);
	newFont = (HFONT) SendMessageW(_W_WIDGET(widget)->handle, WM_GETFONT, 0,
			0);
	if (newFont != 0)
		oldFont = SelectObject(hDC, newFont);
	TEXTMETRICW tm;
	GetTextMetricsW(hDC, &tm);
	if (newFont != 0)
		SelectObject(hDC, oldFont);
	ReleaseDC(_W_WIDGET(widget)->handle, hDC);
	int offsetY = /*COMCTL32_MAJOR >= 6 && IsAppThemed () ? 0 :*/1;
	e->rect->x = CLIENT_INSET;
	e->rect->y = tm.tmHeight + offsetY;
	e->rect->width = w_max(0, rect.right - CLIENT_INSET * 2);
	e->rect->height = w_max(0, rect.bottom - e->rect->y - CLIENT_INSET);
	return W_TRUE;
}
int _GROUP_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	_COMPOSITE_WM_ERASEBKGND(widget, e, reserved);
	RECT rect;
	GetClientRect(_W_WIDGET(widget)->handle, &rect);
	DrawThemeBackground(NULL, (HDC) e->wparam, 0, 0, &rect, NULL);
	e->result = 1;
	return W_TRUE;
}
int _w_group_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int ret = _w_composite_compute_size(widget, e, reserved);
	HWND handle = _W_WIDGET(widget)->handle;
	int length = GetWindowTextLengthW(handle);
	if (length != 0) {
		WCHAR *buffer;
		buffer = _w_toolkit_malloc((length + 3) * sizeof(WCHAR));
		if (buffer != 0) {
			GetWindowTextW(handle, buffer, length + 3);
			//String string = fixText(false);

			/*
			 * If the group has text, and the text is wider than the
			 * client area, pad the width so the text is not clipped.
			 */
			HFONT newFont, oldFont = 0;
			HDC hDC = GetDC(handle);
			newFont = (HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
			if (newFont != 0)
				oldFont = SelectObject(hDC, newFont);
			RECT rect;
			int flags = DT_CALCRECT | DT_SINGLELINE;
			DrawTextW(hDC, buffer, -1, &rect, flags);
			if (newFont != 0)
				SelectObject(hDC, oldFont);
			ReleaseDC(handle, hDC);
			int offsetY =
			W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed() ? 0 : 1;
			e->size->width = w_max(e->size->width,
					rect.right - rect.left + CLIENT_INSET * 6 + offsetY);
			_w_toolkit_free(buffer);
		}
	}
	return ret;
}
int _w_group_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved) {
	int ret = _w_scrollable_compute_trim(widget, e, reserved);
	HWND handle = _W_WIDGET(widget)->handle;
	HFONT newFont, oldFont = 0;
	HDC hDC = GetDC(handle);
	newFont =(HFONT) SendMessageW(handle, WM_GETFONT, 0, 0);
	if (newFont != 0)
		oldFont = SelectObject(hDC, newFont);
	TEXTMETRICW tm;
	GetTextMetricsW(hDC, &tm);
	if (newFont != 0)
		SelectObject(hDC, oldFont);
	ReleaseDC(handle, hDC);
	int offsetY = W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed() ? 0 : 1;
	e->result->x -= CLIENT_INSET;
	e->result->y -= tm.tmHeight + offsetY;
	e->result->width += CLIENT_INSET * 2;
	e->result->height += tm.tmHeight + CLIENT_INSET + offsetY;
	return ret;
}
void _w_group_class_init(struct _w_group_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GROUP;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_group_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_group);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_group);
	/*
	 * public function
	 */
	clazz->get_text = _w_group_get_text;
	clazz->set_text = _w_group_set_text;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_group_create;
	/*
	 * reserved
	 */
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->get_client_area = _w_group_get_client_area;
	reserved->compute_size = _w_group_compute_size;
	reserved->compute_trim = _w_group_compute_trim;
	//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->messages[_WM_ERASEBKGND] = _GROUP_WM_ERASEBKGND;
}
#endif
