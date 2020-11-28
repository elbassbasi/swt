/*
 * table.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_table_create(w_table *table, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(table), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TABLE, post_event);
}
w_table* w_table_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_TABLE(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TABLE,post_event));
}
/*
 * table
 */
wresult w_table_clear_index(w_table *table, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->clear_index(table, index);
	} else
		return result;
}
wresult w_table_clear_range(w_table *table, int start, int end) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->clear_range(table, start, end);
	} else
		return result;
}
wresult w_table_clear_indices(w_table *table, int *indices, size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->clear_indices(table, indices, length);
	} else
		return result;
}
wresult w_table_clear_all(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->clear_all(table);
	} else
		return result;
}
wresult w_table_deselect_indices(w_table *table, int *indices, size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->deselect_indices(table, indices,
				length);
	} else
		return result;
}
wresult w_table_deselect_index(w_table *table, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->deselect_index(table, index);
	} else
		return result;
}
wresult w_table_deselect_range(w_table *table, int start, int end) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->deselect_range(table, start, end);
	} else
		return result;
}
wresult w_table_deselect_all(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->deselect_all(table);
	} else
		return result;
}
wresult w_table_get_column(w_table *table, int index, w_tablecolumn *column) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_column(table, index, column);
	} else
		return result;
}
int w_table_get_column_count(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_column_count(table);
	} else
		return result;
}
size_t w_table_get_column_order(w_table *table, int *order, size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_column_order(table, order, length);
	} else
		return result;
}
void w_table_get_columns(w_table *table, w_iterator *columns) {
	w_iterator_close(columns);
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_columns(table, columns);
	}
}
int w_table_get_gridline_width(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_gridline_width(table);
	} else
		return 0;
}
int w_table_get_header_height(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_header_height(table);
	} else
		return 0;
}
wbool w_table_get_header_visible(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_header_visible(table);
	} else
		return W_FALSE;
}
w_imagelist* w_table_get_imagelist(w_table *table){
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_imagelist(table);
	} else
		return 0;
}
wbool w_table_get_item(w_table *table, int index, w_tableitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_item(table,index, item);
	} else
		return W_FALSE;
}
wbool w_table_get_item_from_point(w_table *table, w_point *point,
		w_tableitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_item_from_point(table, point, item);
	} else
		return W_FALSE;
}
int w_table_get_item_count(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_item_count(table);
	} else
		return 0;
}
int w_table_get_item_height(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_item_height(table);
	} else
		return 0;
}
void w_table_get_items(w_table *table, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		W_TABLE_GET_CLASS(table)->get_items(table, items);
	}
}
wbool w_table_get_lines_visible(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_lines_visible(table);
	} else
		return W_FALSE;
}
void w_table_get_selection(w_table *table, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_selection(table, items);
	}
}
int w_table_get_selection_count(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_selection_count(table);
	} else
		return 0;
}
int w_table_get_selection_index(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_selection_index(table);
	} else
		return -1;
}
size_t w_table_get_selection_indices(w_table *table, int *indics,
		size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_selection_indices(table, indics,length);
	} else
		return 0;
}
wbool w_table_get_sort_column(w_table *table, w_tablecolumn *column) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_sort_column(table,column);
	} else
		return W_FALSE;
}
int w_table_get_sort_direction(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_sort_direction(table);
	} else
		return 0;
}
int w_table_get_top_index(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->get_top_index(table);
	} else
		return 0;
}
wbool w_table_is_selected(w_table *table, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->is_selected(table, index);
	} else
		return W_FALSE;
}
wresult w_table_insert_column(w_table *table, w_tablecolumn *column,
		const char *string, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->insert_column(table, column, string,
				index);
	} else
		return result;
}
wresult w_table_insert_item(w_table *table, w_tableitem *item,
		const char *string, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->insert_item(table, item, string, index);
	} else
		return result;
}
wresult w_table_remove_indices(w_table *table, int *indices, size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->remove_indices(table, indices, length);
	} else
		return result;
}
wresult w_table_remove_index(w_table *table, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->remove_index(table, index);
	} else
		return result;
}
wresult w_table_remove_range(w_table *table, int start, int end) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->remove_range(table, start, end);
	} else
		return result;
}
wresult w_table_remove_all(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->remove_all(table);
	} else
		return result;
}
wresult w_table_select_indices(w_table *table, int *indices, size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->select_indices(table, indices, length);
	} else
		return result;
}
wresult w_table_select_index(w_table *table, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->select_index(table, index);
	} else
		return result;
}
wresult w_table_select_range(w_table *table, int start, int end) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->select_range(table, start, end);
	} else
		return result;
}
wresult w_table_select_all(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->select_all(table);
	} else
		return result;
}
wresult w_table_set_column_order(w_table *table, int *order, size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_column_order(table, order, length);
	} else
		return result;
}
wresult w_table_set_header_visible(w_table *table, wbool show) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_header_visible(table, show);
	} else
		return result;
}
wresult w_table_set_imagelist(w_table* table,w_imagelist* imagelist){
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_imagelist(table, imagelist);
	} else
		return result;
}
wresult w_table_set_lines_visible(w_table *table, wbool show) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_lines_visible(table, show);
	} else
		return result;
}
wresult w_table_set_selection_indices(w_table *table, int *indices,
		size_t length) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_selection_indices(table, indices,
				length);
	} else
		return result;
}
wresult w_table_set_selection_index(w_table *table, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_selection_index(table, index);
	} else
		return result;
}
wresult w_table_set_selection_range(w_table *table, int start, int end) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_selection_range(table, start, end);
	} else
		return result;
}
wresult w_table_set_sort_column(w_table *table, w_tablecolumn *column) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_sort_column(table, column);
	} else
		return result;
}
wresult w_table_set_sort_direction(w_table *table, int direction) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_sort_direction(table, direction);
	} else
		return result;
}
wresult w_table_set_top_index(w_table *table, int index) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->set_top_index(table, index);
	} else
		return result;
}
wresult w_table_show_column(w_table *table, w_tablecolumn *column) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->show_column(table, column);
	} else
		return result;
}
wresult w_table_show_item(w_table *table, w_tableitem *item) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->show_item(table, item);
	} else
		return result;
}
wresult w_table_show_selection(w_table *table) {
	wresult result = W_WIDGET_CHECK0(table);
	if (result > 0) {
		return W_TABLE_GET_CLASS(table)->show_selection(table);
	} else
		return result;
}
/*
 * tablecolumn
 */
int w_tablecolumn_get_alignment(w_tablecolumn *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->get_alignment(column);
	} else
		return 0;
}
int w_tablecolumn_get_image(w_tablecolumn *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->get_image(column);
	} else
		return -1;
}
wbool w_tablecolumn_get_moveable(w_tablecolumn *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->get_moveable(column);
	} else
		return W_FALSE;
}
wbool w_tablecolumn_get_resizable(w_tablecolumn *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->get_resizable(column);
	} else
		return W_FALSE;
}
wresult w_tablecolumn_get_tooltip_text(w_tablecolumn *column,
		w_alloc alloc,void* user_data) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->get_tooltip_text(column, alloc,user_data);
	} else
		return result;
}
int w_tablecolumn_get_width(w_tablecolumn *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->get_width(column);
	} else
		return 0;
}
wresult w_tablecolumn_pack(w_tablecolumn *column) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->pack(column);
	} else
		return result;
}
wresult w_tablecolumn_set_alignment(w_tablecolumn *column, int alignment) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->set_alignment(column, alignment);
	} else
		return result;
}
wresult w_tablecolumn_set_image(w_tablecolumn *column, int image) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->set_image(column, image);
	} else
		return result;
}
wresult w_tablecolumn_set_moveable(w_tablecolumn *column, wbool moveable) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->set_moveable(column, moveable);
	} else
		return result;
}
wresult w_tablecolumn_set_resizable(w_tablecolumn *column, wbool resizable) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->set_resizable(column, resizable);
	} else
		return result;
}
wresult w_tablecolumn_set_tooltip_text(w_tablecolumn *column,
		const char *string) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->set_tooltip_text(column, string);
	} else
		return result;
}
wresult w_tablecolumn_set_width(w_tablecolumn *column, int width) {
	wresult result = W_WIDGETDATA_CHECK0(column);
	if (result > 0) {
		return W_TABLECOLUMN_GET_CLASS(column)->set_width(column, width);
	} else
		return result;
}
/*
 * tableitem
 */
wresult w_tableitem_get_bounds(w_tableitem *item, w_rect *bounds) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TABLEITEM_GET_CLASS(item)->get_bounds(item, bounds);
	} else
		return result;
}
wresult w_tableitem_get_bounds_index(w_tableitem *item, int index,
		w_rect *bounds) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TABLEITEM_GET_CLASS(item)->get_bounds_index(item, index,
				bounds);
	} else
		return result;
}
wbool w_tableitem_get_checked(w_tableitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TABLEITEM_GET_CLASS(item)->get_checked(item);
	} else
		return W_FALSE;
}
int w_tableitem_get_image(w_tableitem *item) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TABLEITEM_GET_CLASS(item)->get_image(item);
	} else
		return -1;
}
wresult w_tableitem_set_checked(w_tableitem *item, wbool checked) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TABLEITEM_GET_CLASS(item)->set_checked(item, checked);
	} else
		return result;
}
wresult w_tableitem_set_image(w_tableitem *item, int image) {
	wresult result = W_WIDGETDATA_CHECK0(item);
	if (result > 0) {
		return W_TABLEITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return result;
}
