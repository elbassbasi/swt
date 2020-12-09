/*
 * table.c
 *
 *  Created on: 22 août 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "tree.h"
#include "../widgets/toolkit.h"
wuint64 _w_table_check_style(w_widget* widget,wuint64 style) {
	/*
	 * Feature in Windows.  Even when WS_HSCROLL or
	 * WS_VSCROLL is not specified, Windows creates
	 * trees and tables with scroll bars.  The fix
	 * is to set H_SCROLL and V_SCROLL.
	 *
	 * NOTE: This code appears on all platforms so that
	 * applications have consistent scroll bar behavior.
	 */
	if ((style & W_NO_SCROLL) == 0) {
		style |= W_HSCROLL | W_VSCROLL;
	}
	/* GTK is always FULL_SELECTION */
	style |= W_FULL_SELECTION;
	return _w_widget_check_bits(style, W_SINGLE, W_MULTI, 0, 0, 0, 0);
}
/**
 * tablecolumn
 */

int _w_tablecolumn_get_alignment(w_tablecolumn *column) {
}
int _w_tablecolumn_get_image(w_tablecolumn *column) {
}
wresult _w_tablecolumn_get_moveable(w_tablecolumn *column) {
}
wresult _w_tablecolumn_get_resizable(w_tablecolumn *column) {
}
wresult _w_tablecolumn_get_tooltip_text(w_tablecolumn *column,
		w_alloc alloc,void* user_data) {
}
int _w_tablecolumn_get_width(w_tablecolumn *column) {
}
wresult _w_tablecolumn_pack(w_tablecolumn *column) {
}
wresult _w_tablecolumn_set_alignment(w_tablecolumn *column, int alignment) {
}
wresult _w_tablecolumn_set_image(w_tablecolumn *column, int image) {
}
wresult _w_tablecolumn_set_moveable(w_tablecolumn *column, int moveable) {
}
wresult _w_tablecolumn_set_resizable(w_tablecolumn *column, int resizable) {
}
wresult _w_tablecolumn_set_tooltip_text(w_tablecolumn *column,
		const char *string) {
}
wresult _w_tablecolumn_set_width(w_tablecolumn *column, int width) {
}
/**
 * tableitem
 */
wresult _w_tableitem_get_bounds(w_tableitem *item, w_rect *bounds) {
}
wresult _w_tableitem_get_bounds_index(w_tableitem *item, int index,
		w_rect *bounds) {
}
wresult _w_tableitem_get_checked(w_tableitem *item) {
}
int _w_tableitem_get_image(w_tableitem *item) {
}
wresult _w_tableitem_set_checked(w_tableitem *item, int checked) {
}
wresult _w_tableitem_set_image(w_tableitem *item, int image) {
}
/**
 * table
 *
 */
wresult _w_table_clear_index(w_table *table, int index) {
}
wresult _w_table_clear_range(w_table *table, int start, int end) {
}
wresult _w_table_clear_indices(w_table *table, int *indices, size_t length) {
}
wresult _w_table_clear_all(w_table *table) {
}
wresult _w_table_deselect_indices(w_table *table, int *indices,
		size_t length) {
}
wresult _w_table_deselect_index(w_table *table, int index) {
}
wresult _w_table_deselect_range(w_table *table, int start, int end) {
}
wresult _w_table_deselect_all(w_table *table) {
}
wresult _w_table_get_column(w_table *table, int index, w_tablecolumn *column) {
}
int _w_table_get_column_count(w_table *table) {
}
size_t _w_table_get_column_order(w_table *table, int *order, size_t length) {
}
void _w_table_get_columns(w_table *table, w_iterator *columns) {
}
int _w_table_get_gridline_width(w_table *table) {
}
int _w_table_get_header_height(w_table *table) {
}
wresult _w_table_get_header_visible(w_table *table) {
}
wresult _w_table_get_item(w_table *table, int index, w_tabitem *item) {
}
wresult _w_table_get_item_from_point(w_table *table, w_point *point,
		w_tabitem *item) {
}
int _w_table_get_item_count(w_table *table) {
}
int _w_table_get_item_height(w_table *table) {
}
void _w_table_get_items(w_table *table, w_iterator *items) {
}
wresult _w_table_get_lines_visible(w_table *table) {
}
void _w_table_get_selection(w_table *table, w_iterator *items) {
}
int _w_table_get_selection_count(w_table *table) {
}
int _w_table_get_selection_index(w_table *table) {
}
size_t _w_table_get_selection_indices(w_table *table, int *indeics,
		size_t length) {
}
wresult _w_table_get_sort_column(w_table *table, w_tablecolumn *column) {
}
int _w_table_get_sort_direction(w_table *table) {
}
int _w_table_get_top_index(w_table *table) {
}
wresult _w_table_is_selected(w_table *table, int index) {
}
wresult _w_table_insert_column(w_table *table, w_tablecolumn *column,
		const char *string, int index) {
}
wresult _w_table_insert_item(w_table *table, w_tableitem *item,
		const char *string, int index) {
}
wresult _w_table_remove_indices(w_table *table, int *indices, size_t length) {
}
wresult _w_table_remove_index(w_table *table, int index) {
}
wresult _w_table_remove_range(w_table *table, int start, int end) {
}
wresult _w_table_remove_all(w_table *table) {
}
wresult _w_table_select_indices(w_table *table, int *indices, size_t length) {
}
wresult _w_table_select_index(w_table *table, int index) {
}
wresult _w_table_select_range(w_table *table, int start, int end) {
}
wresult _w_table_select_all(w_table *table) {
}
wresult _w_table_set_column_order(w_table *table, int *order, size_t length) {
}
wresult _w_table_set_header_visible(w_table *table, int show) {
}
wresult _w_table_set_lines_visible(w_table *table, int show) {
}
wresult _w_table_set_selection_indices(w_table *table, int *indices,
		size_t length) {
}
wresult _w_table_set_selection_index(w_table *table, int index) {
}
wresult _w_table_set_selection_range(w_table *table, int start, int end) {
}
wresult _w_table_set_sort_column(w_table *table, w_tablecolumn *column) {
}
wresult _w_table_set_sort_direction(w_table *table, int direction) {
}
wresult _w_table_set_top_index(w_table *table, int index) {
}
wresult _w_table_show_column(w_table *table, w_tablecolumn *column) {
}
wresult _w_table_show_item(w_table *table, w_tablecolumn *item) {
}
wresult _w_table_show_selection(w_table *table) {
}
/**
 *
 */
wresult _w_table_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;

}
int _w_table_post_event(w_widget *widget, w_event *e) {
	return _w_control_post_event(widget, e);

}
void _w_table_class_init(struct _w_table_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TABLE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_table_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_table);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_table);
	/*
	 * public function
	 */
	clazz->clear_index=_w_table_clear_index;
	clazz->clear_range=_w_table_clear_range;
	clazz->clear_indices=_w_table_clear_indices;
	clazz->clear_all=_w_table_clear_all;
	clazz->deselect_indices=_w_table_deselect_indices;
	clazz->deselect_index=_w_table_deselect_index;
	clazz->deselect_range=_w_table_deselect_range;
	clazz->deselect_all=_w_table_deselect_all;
	clazz->get_column=_w_table_get_column;
	clazz->get_column_count=_w_table_get_column_count;
	clazz->get_column_order=_w_table_get_column_order;
	clazz->get_columns=_w_table_get_columns;
	clazz->get_gridline_width=_w_table_get_gridline_width;
	clazz->get_header_height=_w_table_get_header_height;
	clazz->get_header_visible=_w_table_get_header_visible;
	clazz->get_item=_w_table_get_item;
	clazz->get_item_from_point=_w_table_get_item_from_point;
	clazz->get_item_count=_w_table_get_item_count;
	clazz->get_item_height=_w_table_get_item_height;
	clazz->get_items=_w_table_get_items;
	clazz->get_lines_visible=_w_table_get_lines_visible;
	clazz->get_selection=_w_table_get_selection;
	clazz->get_selection_count=_w_table_get_selection_count;
	clazz->get_selection_index=_w_table_get_selection_index;
	clazz->get_selection_indices=_w_table_get_selection_indices;
	clazz->get_sort_column=_w_table_get_sort_column;
	clazz->get_sort_direction=_w_table_get_sort_direction;
	clazz->get_top_index=_w_table_get_top_index;
	clazz->is_selected=_w_table_is_selected;
	clazz->insert_column=_w_table_insert_column;
	clazz->insert_item=_w_table_insert_item;
	clazz->remove_indices=_w_table_remove_indices;
	clazz->remove_index=_w_table_remove_index;
	clazz->remove_range=_w_table_remove_range;
	clazz->remove_all=_w_table_remove_all;
	clazz->select_indices=_w_table_select_indices;
	clazz->select_index=_w_table_select_index;
	clazz->select_range=_w_table_select_range;
	clazz->select_all=_w_table_select_all;
	clazz->set_column_order=_w_table_set_column_order;
	clazz->set_header_visible=_w_table_set_header_visible;
	clazz->set_lines_visible=_w_table_set_lines_visible;
	clazz->set_selection_indices=_w_table_set_selection_indices;
	clazz->set_selection_index=_w_table_set_selection_index;
	clazz->set_selection_range=_w_table_set_selection_range;
	clazz->set_sort_column=_w_table_set_sort_column;
	clazz->set_sort_direction=_w_table_set_sort_direction;
	clazz->set_top_index=_w_table_set_top_index;
	clazz->show_column=_w_table_show_column;
	clazz->show_item=_w_table_show_item;
	clazz->show_selection=_w_table_show_selection;
	/*
	 * table column
	 */
	struct _w_tablecolumn_class *tablecolumn = clazz->class_tablecolumn;
	tablecolumn->get_alignment=_w_tablecolumn_get_alignment;
	tablecolumn->get_image=_w_tablecolumn_get_image;
	tablecolumn->get_moveable=_w_tablecolumn_get_moveable;
	tablecolumn->get_resizable=_w_tablecolumn_get_resizable;
	tablecolumn->get_tooltip_text=_w_tablecolumn_get_tooltip_text;
	tablecolumn->get_width=_w_tablecolumn_get_width;
	tablecolumn->pack=_w_tablecolumn_pack;
	tablecolumn->set_alignment=_w_tablecolumn_set_alignment;
	tablecolumn->set_image=_w_tablecolumn_set_image;
	tablecolumn->set_moveable=_w_tablecolumn_set_moveable;
	tablecolumn->set_resizable=_w_tablecolumn_set_resizable;
	tablecolumn->set_tooltip_text=_w_tablecolumn_set_tooltip_text;
	tablecolumn->set_width=_w_tablecolumn_set_width;
	/*
	 * table item
	 */
	struct _w_tableitem_class *tableitem = clazz->class_tableitem;
	tableitem->get_bounds=_w_tableitem_get_bounds;
	tableitem->get_bounds_index=_w_tableitem_get_bounds_index;
	tableitem->get_checked=_w_tableitem_get_checked;
	tableitem->get_image=_w_tableitem_get_image;
	tableitem->set_checked=_w_tableitem_set_checked;
	tableitem->set_image=_w_tableitem_set_image;

	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_table_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_table_create;
}
#endif
