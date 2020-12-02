/*
 * combo.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "combo.h"
#include "../widgets/toolkit.h"
wresult _w_combo_insert(w_combo *combo, w_comboitem *item, const char *string,
		int index) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	return W_TRUE;
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
wresult _w_combo_set_list_visible(w_combo *combo, int visible) {
}
wresult _w_combo_get_selection(w_combo *combo, w_point *pt) {
}
int _w_combo_get_selection_index(w_combo *combo) {
}
wresult _w_combo_get_text(w_combo *combo, w_alloc alloc,void* user_data) {
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
wresult _w_combo_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
}
int _w_combo_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
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
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_combo_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
}
#endif
