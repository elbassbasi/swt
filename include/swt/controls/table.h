/*
 * table.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_TABLE_H_
#define SWT_CONTROLS_TABLE_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_table {
	w_composite composite;
	void *handle[0x10];
} w_table;

#define W_TABLE(x) ((w_table*)x)
typedef struct w_tablecolumn {
	w_item item;
	void *handle[6];
} w_tablecolumn;
#define W_TABLECOLUMN(x) ((w_tablecolumn*)x)
typedef struct w_tableitem {
	w_item item;
	void *handle[6];
} w_tableitem;
#define W_TABLEITEM(x) ((w_tableitem*)x)
struct _w_tablecolumn_class {
	struct _w_item_class item;
	int (*get_alignment)(w_tablecolumn *column);
	int (*get_image)(w_tablecolumn *column);
	wbool (*get_moveable)(w_tablecolumn *column);
	wbool (*get_resizable)(w_tablecolumn *column);
	wresult (*get_tooltip_text)(w_tablecolumn *column,w_alloc alloc,void* user_data);
	int (*get_width)(w_tablecolumn *column);
	wresult (*pack)(w_tablecolumn *column);
	wresult (*set_alignment)(w_tablecolumn *column, int alignment);
	wresult (*set_image)(w_tablecolumn *column, int image);
	wresult (*set_moveable)(w_tablecolumn *column, wbool moveable);
	wresult (*set_resizable)(w_tablecolumn *column, wbool resizable);
	wresult (*set_tooltip_text)(w_tablecolumn *column, const char *string);
	wresult (*set_width)(w_tablecolumn *column, int width);
};
struct _w_tableitem_class {
	struct _w_item_class item;
	wresult (*get_bounds)(w_tableitem *item, w_rect *bounds);
	wresult (*get_bounds_index)(w_tableitem *item, int index, w_rect *bounds);
	wbool (*get_checked)(w_tableitem *item);
	int (*get_image)(w_tableitem *item);
	wresult (*set_checked)(w_tableitem *item, wbool checked);
	wresult (*set_image)(w_tableitem *item, int image);

};
struct _w_table_class {
	struct _w_composite_class composite;
	struct _w_tableitem_class *class_tableitem;
	struct _w_tablecolumn_class *class_tablecolumn;
	wresult (*clear_index)(w_table *table, int index);
	wresult (*clear_range)(w_table *table, int start, int end);
	wresult (*clear_indices)(w_table *table, int *indices, size_t length);
	wresult (*clear_all)(w_table *table);
	wresult (*deselect_indices)(w_table *table, int *indices, size_t length);
	wresult (*deselect_index)(w_table *table, int index);
	wresult (*deselect_range)(w_table *table, int start, int end);
	wresult (*deselect_all)(w_table *table);
	wresult (*get_column)(w_table *table, int index, w_tablecolumn *column);
	int (*get_column_count)(w_table *table);
	size_t (*get_column_order)(w_table *table, int *order, size_t length);
	void (*get_columns)(w_table *table, w_iterator *columns);
	int (*get_gridline_width)(w_table *table);
	int (*get_header_height)(w_table *table);
	wbool (*get_header_visible)(w_table *table);
	w_imagelist* (*get_imagelist)(w_table *table);
	wbool (*get_item)(w_table *table, int index, w_tableitem *item);
	wbool (*get_item_from_point)(w_table *table, w_point *point,
			w_tableitem *item);
	int (*get_item_count)(w_table *table);
	int (*get_item_height)(w_table *table);
	void (*get_items)(w_table *table, w_iterator *items);
	wbool (*get_lines_visible)(w_table *table);
	void (*get_selection)(w_table *table, w_iterator *items);
	int (*get_selection_count)(w_table *table);
	int (*get_selection_index)(w_table *table);
	size_t (*get_selection_indices)(w_table *table, int *indeics,
			size_t length);
	wbool (*get_sort_column)(w_table *table, w_tablecolumn *column);
	int (*get_sort_direction)(w_table *table);
	int (*get_top_index)(w_table *table);
	wbool (*is_selected)(w_table *table, int index);
	wresult (*insert_column)(w_table *table, w_tablecolumn *column,
			const char *string, int index);
	wresult (*insert_item)(w_table *table, w_tableitem *item,
			const char *string, int index);
	wresult (*remove_indices)(w_table *table, int *indices, size_t length);
	wresult (*remove_index)(w_table *table, int index);
	wresult (*remove_range)(w_table *table, int start, int end);
	wresult (*remove_all)(w_table *table);
	wresult (*select_indices)(w_table *table, int *indices, size_t length);
	wresult (*select_index)(w_table *table, int index);
	wresult (*select_range)(w_table *table, int start, int end);
	wresult (*select_all)(w_table *table);
	wresult (*set_column_order)(w_table *table, int *order, size_t length);
	wresult (*set_header_visible)(w_table *table, wbool show);
	wresult (*set_imagelist)(w_table* table,w_imagelist* imagelist);
	wresult (*set_lines_visible)(w_table *table, wbool show);
	wresult (*set_selection_indices)(w_table *table, int *indices,
			size_t length);
	wresult (*set_selection_index)(w_table *table, int index);
	wresult (*set_selection_range)(w_table *table, int start, int end);
	wresult (*set_sort_column)(w_table *table, w_tablecolumn *column);
	wresult (*set_sort_direction)(w_table *table, int direction);
	wresult (*set_top_index)(w_table *table, int index);
	wresult (*show_column)(w_table *table, w_tablecolumn *column);
	wresult (*show_item)(w_table *table, w_tableitem *item);
	wresult (*show_selection)(w_table *table);
};
/*
 * tablecolumn
 */
SWT_PUBLIC int w_tablecolumn_get_alignment(w_tablecolumn *column);
SWT_PUBLIC int w_tablecolumn_get_image(w_tablecolumn *column);
SWT_PUBLIC wbool w_tablecolumn_get_moveable(w_tablecolumn *column);
SWT_PUBLIC wbool w_tablecolumn_get_resizable(w_tablecolumn *column);
SWT_PUBLIC wresult w_tablecolumn_get_tooltip_text(w_tablecolumn *column,
		w_alloc alloc,void* user_data);
SWT_PUBLIC int w_tablecolumn_get_width(w_tablecolumn *column);
SWT_PUBLIC wresult w_tablecolumn_pack(w_tablecolumn *column);
SWT_PUBLIC wresult w_tablecolumn_set_alignment(w_tablecolumn *column,
		int alignment);
SWT_PUBLIC wresult w_tablecolumn_set_image(w_tablecolumn *column, int image);
SWT_PUBLIC wresult w_tablecolumn_set_moveable(w_tablecolumn *column,
		wbool moveable);
SWT_PUBLIC wresult w_tablecolumn_set_resizable(w_tablecolumn *column,
		wbool resizable);
SWT_PUBLIC wresult w_tablecolumn_set_tooltip_text(w_tablecolumn *column,
		const char *string);
SWT_PUBLIC wresult w_tablecolumn_set_width(w_tablecolumn *column, int width);
/*
 * tableitem
 */
SWT_PUBLIC wresult w_tableitem_get_bounds(w_tableitem *item, w_rect *bounds);
SWT_PUBLIC wresult w_tableitem_get_bounds_index(w_tableitem *item, int index,
		w_rect *bounds);
SWT_PUBLIC wbool w_tableitem_get_checked(w_tableitem *item);
SWT_PUBLIC int w_tableitem_get_image(w_tableitem *item);
SWT_PUBLIC wresult w_tableitem_set_checked(w_tableitem *item, wbool checked);
SWT_PUBLIC wresult w_tableitem_set_image(w_tableitem *item, int image);
/*
 * table
 */
SWT_PUBLIC wresult w_table_create(w_table *table, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_table* w_table_new(struct w_toolkit *toolkit, w_composite *parent,
                                wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_table_clear_index(w_table *table, int index);
SWT_PUBLIC wresult w_table_clear_range(w_table *table, int start, int end);
SWT_PUBLIC wresult w_table_clear_indices(w_table *table, int *indices,
		size_t length);
SWT_PUBLIC wresult w_table_clear_all(w_table *table);
SWT_PUBLIC wresult w_table_deselect_indices(w_table *table, int *indices,
		size_t length);
SWT_PUBLIC wresult w_table_deselect_index(w_table *table, int index);
SWT_PUBLIC wresult w_table_deselect_range(w_table *table, int start, int end);
SWT_PUBLIC wresult w_table_deselect_all(w_table *table);
SWT_PUBLIC wresult w_table_get_column(w_table *table, int index,
		w_tablecolumn *column);
SWT_PUBLIC int w_table_get_column_count(w_table *table);
SWT_PUBLIC size_t w_table_get_column_order(w_table *table, int *order,
		size_t length);
SWT_PUBLIC void w_table_get_columns(w_table *table, w_iterator *columns);
SWT_PUBLIC int w_table_get_gridline_width(w_table *table);
SWT_PUBLIC int w_table_get_header_height(w_table *table);
SWT_PUBLIC wbool w_table_get_header_visible(w_table *table);
SWT_PUBLIC w_imagelist* w_table_get_imagelist(w_table *table);
SWT_PUBLIC wbool w_table_get_item(w_table *table, int index,
		w_tableitem *item);
SWT_PUBLIC wbool w_table_get_item_from_point(w_table *table, w_point *point,
		w_tableitem *item);
SWT_PUBLIC int w_table_get_item_count(w_table *table);
SWT_PUBLIC int w_table_get_item_height(w_table *table);
SWT_PUBLIC void w_table_get_items(w_table *table, w_iterator *items);
SWT_PUBLIC wbool w_table_get_lines_visible(w_table *table);
SWT_PUBLIC void w_table_get_selection(w_table *table, w_iterator *items);
SWT_PUBLIC int w_table_get_selection_count(w_table *table);
SWT_PUBLIC int w_table_get_selection_index(w_table *table);
SWT_PUBLIC size_t w_table_get_selection_indices(w_table *table, int *indeics,
		size_t length);
SWT_PUBLIC wbool w_table_get_sort_column(w_table *table,
		w_tablecolumn *column);
SWT_PUBLIC int w_table_get_sort_direction(w_table *table);
SWT_PUBLIC int w_table_get_top_index(w_table *table);
SWT_PUBLIC wbool w_table_is_selected(w_table *table, int index);
SWT_PUBLIC wresult w_table_insert_column(w_table *table, w_tablecolumn *column,
		const char *string, int index);
SWT_PUBLIC wresult w_table_insert_item(w_table *table, w_tableitem *item,
		const char *string, int index);
SWT_PUBLIC wresult w_table_remove_indices(w_table *table, int *indices,
		size_t length);
SWT_PUBLIC wresult w_table_remove_index(w_table *table, int index);
SWT_PUBLIC wresult w_table_remove_range(w_table *table, int start, int end);
SWT_PUBLIC wresult w_table_remove_all(w_table *table);
SWT_PUBLIC wresult w_table_select_indices(w_table *table, int *indices,
		size_t length);
SWT_PUBLIC wresult w_table_select_index(w_table *table, int index);
SWT_PUBLIC wresult w_table_select_range(w_table *table, int start, int end);
SWT_PUBLIC wresult w_table_select_all(w_table *table);
SWT_PUBLIC wresult w_table_set_column_order(w_table *table, int *order,
		size_t length);
SWT_PUBLIC wresult w_table_set_header_visible(w_table *table, wbool show);
SWT_PUBLIC wresult w_table_set_imagelist(w_table* table,w_imagelist* imagelist);
SWT_PUBLIC wresult w_table_set_lines_visible(w_table *table, wbool show);
SWT_PUBLIC wresult w_table_set_selection_indices(w_table *table, int *indices,
		size_t length);
SWT_PUBLIC wresult w_table_set_selection_index(w_table *table, int index);
SWT_PUBLIC wresult w_table_set_selection_range(w_table *table, int start,
		int end);
SWT_PUBLIC wresult w_table_set_sort_column(w_table *table,
		w_tablecolumn *column);
SWT_PUBLIC wresult w_table_set_sort_direction(w_table *table, int direction);
SWT_PUBLIC wresult w_table_set_top_index(w_table *table, int index);
SWT_PUBLIC wresult w_table_show_column(w_table *table, w_tablecolumn *column);
SWT_PUBLIC wresult w_table_show_item(w_table *table, w_tableitem *item);
SWT_PUBLIC wresult w_table_show_selection(w_table *table);

#define W_TABLE_CLASS(x) ((struct _w_table_class*)x)
#define W_TABLE_GET_CLASS(x) ((struct _w_table_class*)W_WIDGET_GET_CLASS(x))
#define W_TABLECOLUMN_CLASS(x) ((struct _w_tablecolumn_class*)x)
#define W_TABLECOLUMN_GET_CLASS(x) ((struct _w_tablecolumn_class*)W_WIDGETDATA_GET_CLASS(x))
#define W_TABLEITEM_CLASS(x) ((struct _w_tableitem_class*)x)
#define W_TABLEITEM_GET_CLASS(x) ((struct _w_tableitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_TABLE_H_ */
