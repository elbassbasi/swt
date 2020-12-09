/*
 * combo.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "combo.h"
#include "../widgets/toolkit.h"
wresult _w_combo_insert(w_combo *combo, w_comboitem *item, const char *string,
		int index) {
	SendMessageA (_W_WIDGET(combo)->handle, CB_INSERTSTRING, index, string);
}
wresult _w_combo_clear_selection(w_combo *combo) {
}
wresult _w_combo_copy(w_combo *combo) {
}
wresult _w_combo_cut(w_combo *combo) {
}
wresult _w_combo_deselect(w_combo *combo, int index) {
}
wresult _w_combo_deselect_all(w_combo *combo) {
}
wresult _w_combo_get_item(w_combo *combo, int index, w_comboitem *item) {
}
int _w_combo_get_item_count(w_combo *combo) {
}
int _w_combo_get_item_height(w_combo *combo) {
}
void _w_combo_get_items(w_combo *combo, w_iterator *items) {
}
wresult _w_combo_get_list_visible(w_combo *combo) {
}
wresult _w_combo_set_list_visible(w_combo *combo, wresult visible) {
}
wresult _w_combo_get_selection(w_combo *combo, w_point *pt) {
}
int _w_combo_get_selection_index(w_combo *combo) {
}
w_string_ref* _w_combo_get_text(w_combo *combo) {
}
int _w_combo_get_text_height(w_combo *combo) {
}
int _w_combo_get_text_limit(w_combo *combo) {
}
int _w_combo_get_visible_item_count(w_combo *combo) {
}
int _w_combo_indexof(w_combo *combo, const char *string, int start) {
}
wresult _w_combo_paste(w_combo *combo) {
}
wresult _w_combo_remove_index(w_combo *combo, int index) {
}
wresult _w_combo_remove_range(w_combo *combo, int start, int end) {
}
wresult _w_combo_remove_string(w_combo *combo, const char *string) {
}
wresult _w_combo_remove_all(w_combo *combo) {
}
wresult _w_combo_select(w_combo *combo, int index) {
}
wresult _w_combo_set_item(w_combo *combo, int index, const char *string) {
}
wresult _w_combo_set_selection(w_combo *combo, w_point *selection) {
}
wresult _w_combo_set_text(w_combo *combo, const char *string) {
}
wresult _w_combo_set_text_limit(w_combo *combo, int limit) {
}
wresult _w_combo_set_visible_item_count(w_combo *combo, int count) {
}
/*
 * private
 */
int _w_combo_check_style(int style) {
	/*
	 * Feature in Windows.  It is not possible to create
	 * a combo box that has a border using Windows style
	 * bits.  All combo boxes draw their own border and
	 * do not use the standard Windows border styles.
	 * Therefore, no matter what style bits are specified,
	 * clear the BORDER bits so that the SWT style will
	 * match the Windows widget.
	 *
	 * The Windows behavior is currently implemented on
	 * all platforms.
	 */
	style &= ~W_BORDER;

	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);
	style = _w_widget_check_bits(style, W_DROP_DOWN, W_SIMPLE, 0, 0, 0, 0);
	if ((style & W_SIMPLE) != 0)
		return style & ~W_READ_ONLY;
	return style;
}
void _w_combo_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_composite_style(widget, parent, style, dwExStyle, dwStyle);
	*dwExStyle &= ~WS_EX_NOINHERITLAYOUT;
	*dwStyle |=
	CBS_AUTOHSCROLL | CBS_NOINTEGRALHEIGHT | WS_HSCROLL | WS_VSCROLL;
	if ((style & W_SIMPLE) != 0) {
		*dwStyle |= CBS_SIMPLE;
		return;
	}
	if ((style & W_READ_ONLY) != 0) {
		*dwStyle |= CBS_DROPDOWNLIST;
		return;
	}
	*dwStyle |= CBS_DROPDOWN;
}
/*
 * public
 */
wresult _w_combo_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_combo_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_combo_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_COMBOBOXA, dwExStyle, dwStyle);
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
void _w_combo_class_init(struct _w_combo_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMBO;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_combo_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_combo);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_combo);
	/*
	 * public function
	 */
	clazz->insert = _w_combo_insert;
	clazz->clear_selection = _w_combo_clear_selection;
	clazz->copy = _w_combo_copy;
	clazz->cut = _w_combo_cut;
	clazz->deselect = _w_combo_deselect;
	clazz->deselect_all = _w_combo_deselect_all;
	clazz->get_item = _w_combo_get_item;
	clazz->get_item_count = _w_combo_get_item_count;
	clazz->get_item_height = _w_combo_get_item_height;
	clazz->get_items = _w_combo_get_items;
	clazz->get_list_visible = _w_combo_get_list_visible;
	clazz->set_list_visible = _w_combo_set_list_visible;
	clazz->get_selection = _w_combo_get_selection;
	clazz->get_selection_index = _w_combo_get_selection_index;
	clazz->get_text = _w_combo_get_text;
	clazz->get_text_height = _w_combo_get_text_height;
	clazz->get_text_limit = _w_combo_get_text_limit;
	clazz->get_visible_item_count = _w_combo_get_visible_item_count;
	clazz->indexof = _w_combo_indexof;
	clazz->paste = _w_combo_paste;
	clazz->remove_index = _w_combo_remove_index;
	clazz->remove_range = _w_combo_remove_range;
	clazz->remove_string = _w_combo_remove_string;
	clazz->remove_all = _w_combo_remove_all;
	clazz->select = _w_combo_select;
	clazz->set_item = _w_combo_set_item;
	clazz->set_selection = _w_combo_set_selection;
	clazz->set_text = _w_combo_set_text;
	clazz->set_text_limit = _w_combo_set_text_limit;
	clazz->set_visible_item_count = _w_combo_set_visible_item_count;
	W_WIDGET_CLASS(clazz)->create = _w_combo_create;
	/*
	 * reserved
	 */
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	//messages
	struct _w_control_reserved *msg = _W_CONTROL_RESERVED(reserved);
	msg->messages[_WM_ERASEBKGND] = _CONTROL_WM_ERASEBKGND;
	msg->messages[_WM_PAINT] = _CONTROL_WM_PAINT;
}
#endif
