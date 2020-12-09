/*
 * list.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "list.h"
#include "../widgets/toolkit.h"
/*
 * private
 */
int _w_list_check_style(int style) {
	style = _w_widget_check_bits(style, W_SINGLE, W_MULTI, 0, 0, 0, 0);
	return style;
}
void _w_list_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_scrollable_style(widget, parent, style, dwExStyle, dwStyle);
	*dwStyle |= LBS_NOTIFY | LBS_NOINTEGRALHEIGHT;
	if ((style & W_SINGLE) != 0) {
		return;
	}
	if ((style & W_MULTI) != 0) {
		if ((style & W_SIMPLE) != 0) {
			*dwStyle |= LBS_MULTIPLESEL;
			return;
		}
		*dwStyle |= LBS_EXTENDEDSEL;
		return;
	}
}
/*
 * public
 */
wresult _w_list_insert(w_list *list, w_listitem *item, const char *string,
		int index) {
}
wresult _w_list_deselect_indices(w_list *list, int *indices, size_t length) {
}
wresult _w_list_deselect_index(w_list *list, int index) {
}
wresult _w_list_deselect_range(w_list *list, int start, int end) {
}
wresult _w_list_deselect_all(w_list *list) {
}
int _w_list_get_focus_index(w_list *list) {
}
wresult _w_list_get_item(w_list *list, int index, w_listitem *item) {
}
int _w_list_get_item_count(w_list *list) {
}
int _w_list_get_item_height(w_list *list) {
}
void _w_list_get_items(w_list *list, w_iterator *items) {
}
void _w_list_get_selection(w_list *list, w_iterator *selections) {
}
int _w_list_get_selection_count(w_list *list) {
}
int _w_list_get_selection_index(w_list *list) {
}
int _w_list_get_top_index(w_list *list) {
}
int _w_list_index_of(w_list *list, const char *string, int start) {
}
wresult _w_list_is_selected(w_list *list, int index) {
}
wresult _w_list_remove_indices(w_list *list, int *indices, size_t length) {
}
wresult _w_list_remove_index(w_list *list, int index) {
}
wresult _w_list_remove_range(w_list *list, int start, int end) {
}
wresult _w_list_remove_string(w_list *list, const char *string) {
}
wresult _w_list_remove_all(w_list *list) {
}
wresult _w_list_select_indices(w_list *list, int *indices, size_t length) {
}
wresult _w_list_select_index(w_list *list, int index) {
}
wresult _w_list_select_range(w_list *list, int start, int end) {
}
wresult _w_list_select_all(w_list *list) {
}
wresult _w_list_set_item(w_list *list, int index, const char *string) {
}
wresult _w_list_set_items(w_list *list, const char **items, size_t length) {
}
wresult _w_list_set_selection_indices(w_list *list, int *indices,
		size_t length) {
}
wresult _w_list_set_selection_strings(w_list *list, const char **items,
		size_t length) {
}
wresult _w_list_set_selection_index(w_list *list, int index) {
}
wresult _w_list_set_selection_range(w_list *list, int start, int end) {
}
wresult _w_list_set_top_index(w_list *list, int index) {
}
wresult _w_list_show_selection(w_list *list) {
}
wresult _w_list_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_list_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_list_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create_0(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_LISTBOXA, dwExStyle, dwStyle);
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
int _w_list_post_event(w_widget *widget, w_event *e) {
	return _w_scrollable_post_event(widget, e);

}
void _w_list_class_init(struct _w_list_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LIST;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_list_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_list);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_list);
	/*
	 * public function
	 */
	clazz->insert=_w_list_insert;
	clazz->deselect_indices=_w_list_deselect_indices;
	clazz->deselect_index=_w_list_deselect_index;
	clazz->deselect_range=_w_list_deselect_range;
	clazz->deselect_all=_w_list_deselect_all;
	clazz->get_focus_index=_w_list_get_focus_index;
	clazz->get_item=_w_list_get_item;
	clazz->get_item_count=_w_list_get_item_count;
	clazz->get_item_height=_w_list_get_item_height;
	clazz->get_items=_w_list_get_items;
	clazz->get_selection=_w_list_get_selection;
	clazz->get_selection_count=_w_list_get_selection_count;
	clazz->get_selection_index=_w_list_get_selection_index;
	clazz->get_top_index=_w_list_get_top_index;
	clazz->index_of=_w_list_index_of;
	clazz->is_selected=_w_list_is_selected;
	clazz->remove_indices=_w_list_remove_indices;
	clazz->remove_index=_w_list_remove_index;
	clazz->remove_range=_w_list_remove_range;
	clazz->remove_string=_w_list_remove_string;
	clazz->remove_all=_w_list_remove_all;
	clazz->select_indices=_w_list_select_indices;
	clazz->select_index=_w_list_select_index;
	clazz->select_range=_w_list_select_range;
	clazz->select_all=_w_list_select_all;
	clazz->set_item=_w_list_set_item;
	clazz->set_items=_w_list_set_items;
	clazz->set_selection_indices=_w_list_set_selection_indices;
	clazz->set_selection_strings=_w_list_set_selection_strings;
	clazz->set_selection_index=_w_list_set_selection_index;
	clazz->set_selection_range=_w_list_set_selection_range;
	clazz->set_top_index=_w_list_set_top_index;
	clazz->show_selection=_w_list_show_selection;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_list_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_list_create;
}
#endif
