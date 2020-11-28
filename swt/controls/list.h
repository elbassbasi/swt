/*
 * list.h
 *
 *  Created on: 16 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_LIST_H_
#define SWT_CONTROLS_LIST_H_
#include "../widgets/scrollable.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_list {
	w_scrollable scrollable;
	void *handle[8];
} w_list;
#define W_LIST(x) ((w_list*)x)
typedef struct w_listitem {
	w_item item;
	void *handle[6];
} w_listitem;
#define W_LISTITEM(x) ((w_listitem*)x)
struct _w_listitem_class {
	struct _w_item_class item;
};
struct _w_list_class {
	struct _w_scrollable_class scrollable;
	struct _w_listitem_class *class_listitem;
	wresult (*insert)(w_list *list, w_listitem *item, const char *string,
			int index);
	wresult (*deselect_indices)(w_list *list, int *indices, size_t length);
	wresult (*deselect_index)(w_list *list, int index);
	wresult (*deselect_range)(w_list *list, int start, int end);
	wresult (*deselect_all)(w_list *list);
	int (*get_focus_index)(w_list *list);
	wresult (*get_item)(w_list *list, int index, w_listitem *item);
	int (*get_item_count)(w_list *list);
	int (*get_item_height)(w_list *list);
	void (*get_items)(w_list *list, w_iterator *items);
	void (*get_selection)(w_list *list, w_iterator *selections);
	int (*get_selection_count)(w_list *list);
	int (*get_selection_index)(w_list *list);
	int (*get_top_index)(w_list *list);
	int (*index_of)(w_list *list, const char *string, int start);
	wresult (*is_selected)(w_list *list, int index);
	wresult (*remove_indices)(w_list *list, int *indices, size_t length);
	wresult (*remove_index)(w_list *list, int index);
	wresult (*remove_range)(w_list *list, int start, int end);
	wresult (*remove_string)(w_list *list, const char *string);
	wresult (*remove_all)(w_list *list);
	wresult (*select_indices)(w_list *list, int *indices, size_t length);
	wresult (*select_index)(w_list *list, int index);
	wresult (*select_range)(w_list *list, int start, int end);
	wresult (*select_all)(w_list *list);
	wresult (*set_item)(w_list *list, int index, const char *string);
	wresult (*set_items)(w_list *list, const char **items, size_t length);
	wresult (*set_selection_indices)(w_list *list, int *indices,
			size_t length);
	wresult (*set_selection_strings)(w_list *list, const char **items,
			size_t length);
	wresult (*set_selection_index)(w_list *list, int index);
	wresult (*set_selection_range)(w_list *list, int start, int end);
	wresult (*set_top_index)(w_list *list, int index);
	wresult (*show_selection)(w_list *list);
};
SWT_PUBLIC wresult w_list_create(w_list *button, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_list* w_list_new(struct w_toolkit *toolkit, w_composite *parent,
                              wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_list_insert(w_list *list, w_listitem *item,
		const char *string, int index);
SWT_PUBLIC wresult w_list_deselect_indices(w_list *list, int *indices,
		size_t length);
SWT_PUBLIC wresult w_list_deselect_index(w_list *list, int index);
SWT_PUBLIC wresult w_list_deselect_range(w_list *list, int start, int end);
SWT_PUBLIC wresult w_list_deselect_all(w_list *list);
SWT_PUBLIC int w_list_get_focus_index(w_list *list);
SWT_PUBLIC wresult w_list_get_item(w_list *list, int index, w_listitem *item);
SWT_PUBLIC int w_list_get_item_count(w_list *list);
SWT_PUBLIC int w_list_get_item_height(w_list *list);
SWT_PUBLIC void w_list_get_items(w_list *list, w_iterator *items);
SWT_PUBLIC void w_list_get_selection(w_list *list, w_iterator *selections);
SWT_PUBLIC int w_list_get_selection_count(w_list *list);
SWT_PUBLIC int w_list_get_selection_index(w_list *list);
SWT_PUBLIC int w_list_get_top_index(w_list *list);
SWT_PUBLIC int w_list_index_of(w_list *list, const char *string, int start);
SWT_PUBLIC wresult w_list_is_selected(w_list *list, int index);
SWT_PUBLIC wresult w_list_remove_indices(w_list *list, int *indices,
		size_t length);
SWT_PUBLIC wresult w_list_remove_index(w_list *list, int index);
SWT_PUBLIC wresult w_list_remove_range(w_list *list, int start, int end);
SWT_PUBLIC wresult w_list_remove_string(w_list *list, const char *string);
SWT_PUBLIC wresult w_list_remove_all(w_list *list);
SWT_PUBLIC wresult w_list_select_indices(w_list *list, int *indices,
		size_t length);
SWT_PUBLIC wresult w_list_select_index(w_list *list, int index);
SWT_PUBLIC wresult w_list_select_range(w_list *list, int start, int end);
SWT_PUBLIC wresult w_list_select_all(w_list *list);
SWT_PUBLIC wresult w_list_set_item(w_list *list, int index,
		const char *string);
SWT_PUBLIC wresult w_list_set_items(w_list *list, const char **items,
		size_t length);
SWT_PUBLIC wresult w_list_set_selection_indices(w_list *list, int *indices,
		size_t length);
SWT_PUBLIC wresult w_list_set_selection_strings(w_list *list,
		const char **items, size_t length);
SWT_PUBLIC wresult w_list_set_selection_index(w_list *list, int index);
SWT_PUBLIC wresult w_list_set_selection_range(w_list *list, int start,
		int end);
SWT_PUBLIC wresult w_list_set_top_index(w_list *list, int index);
SWT_PUBLIC wresult w_list_show_selection(w_list *list);

#define W_LIST_CLASS(x) ((struct _w_list_class*)x)
#define W_LIST_GET_CLASS(x) ((struct _w_list_class*)W_WIDGET_GET_CLASS(x))
#define W_LISTITEM_CLASS(x) ((struct _w_listitem_class*)x)
#define W_LISTITEM_GET_CLASS(x) ((struct _w_listitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_LIST_H_ */
