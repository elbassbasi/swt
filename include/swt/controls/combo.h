/*
 * combo.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_COMBO_H_
#define SWT_CONTROLS_COMBO_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_combo {
	w_composite composite;
	void *handle[0x10];
} w_combo;
#define W_COMBO(x) ((w_combo*)x)
typedef struct w_comboitem {
	w_item item;
	void *handle[6];
} w_comboitem;
#define W_COMBOITEM(x) ((w_comboitem*)x)
struct _w_comboitem_class {
	struct _w_item_class item;
};
struct _w_combo_class {
	struct _w_composite_class composite;
	struct _w_comboitem_class *class_comboitem;
	wresult (*insert)(w_combo *combo, w_comboitem *item, const char *string,
			int index);
	wresult (*clear_selection)(w_combo *combo);
	wresult (*copy)(w_combo *combo);
	wresult (*cut)(w_combo *combo);
	wresult (*deselect)(w_combo *combo, int index);
	wresult (*deselect_all)(w_combo *combo);
	wresult (*get_item)(w_combo *combo, int index, w_comboitem *item);
	int (*get_item_count)(w_combo *combo);
	int (*get_item_height)(w_combo *combo);
	void (*get_items)(w_combo *combo, w_iterator *items);
	wbool (*get_list_visible)(w_combo *combo);
	wresult (*set_list_visible)(w_combo *combo, wbool visible);
	wbool (*get_selection)(w_combo *combo, w_point *pt);
	int (*get_selection_index)(w_combo *combo);
	wresult (*get_text)(w_combo *combo,w_alloc alloc,void* user_data);
	int (*get_text_height)(w_combo *combo);
	int (*get_text_limit)(w_combo *combo);
	int (*get_visible_item_count)(w_combo *combo);
	int (*indexof)(w_combo *combo, const char *string, int start);
	wresult (*paste)(w_combo *combo);
	wresult (*remove_index)(w_combo *combo, int index);
	wresult (*remove_range)(w_combo *combo, int start, int end);
	wresult (*remove_string)(w_combo *combo, const char *string);
	wresult (*remove_all)(w_combo *combo);
	wresult (*select)(w_combo *combo, int index);
	wresult (*set_item)(w_combo *combo, int index, const char *string);
	wresult (*set_selection)(w_combo *combo, w_point *selection);
	wresult (*set_text)(w_combo *combo, const char *string);
	wresult (*set_text_limit)(w_combo *combo, int limit);
	wresult (*set_visible_item_count)(w_combo *combo, int count);
};
SWT_PUBLIC wresult  w_combo_create(w_combo *combo, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_combo*  w_combo_new(struct w_toolkit *toolkit, w_composite *parent,
                                 wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_combo_insert(w_combo *combo, w_comboitem *item, const char *string,
		int index);
SWT_PUBLIC wresult w_combo_clear_selection(w_combo *combo);
SWT_PUBLIC wresult w_combo_copy(w_combo *combo);
SWT_PUBLIC wresult w_combo_cut(w_combo *combo);
SWT_PUBLIC wresult w_combo_deselect(w_combo *combo, int index);
SWT_PUBLIC wresult w_combo_deselect_all(w_combo *combo);
SWT_PUBLIC wresult w_combo_get_item(w_combo *combo, int index, w_comboitem *item);
SWT_PUBLIC int w_combo_get_item_count(w_combo *combo);
SWT_PUBLIC int w_combo_get_item_height(w_combo *combo);
SWT_PUBLIC void w_combo_get_items(w_combo *combo, w_iterator *items);
SWT_PUBLIC wbool w_combo_get_list_visible(w_combo *combo);
SWT_PUBLIC wresult w_combo_set_list_visible(w_combo *combo, wbool visible);
SWT_PUBLIC wbool w_combo_get_selection(w_combo *combo, w_point *pt);
SWT_PUBLIC int w_combo_get_selection_index(w_combo *combo);
SWT_PUBLIC wresult w_combo_get_text(w_combo *combo,w_alloc alloc,void* user_data);
SWT_PUBLIC int w_combo_get_text_height(w_combo *combo);
SWT_PUBLIC int w_combo_get_text_limit(w_combo *combo);
SWT_PUBLIC int w_combo_get_visible_item_count(w_combo *combo);
SWT_PUBLIC int w_combo_indexof(w_combo *combo, const char *string, int start);
SWT_PUBLIC wresult w_combo_paste(w_combo *combo);
SWT_PUBLIC wresult w_combo_remove_index(w_combo *combo, int index);
SWT_PUBLIC wresult w_combo_remove_range(w_combo *combo, int start, int end);
SWT_PUBLIC wresult w_combo_remove_string(w_combo *combo, const char *string);
SWT_PUBLIC wresult w_combo_remove_all(w_combo *combo);
SWT_PUBLIC wresult w_combo_select(w_combo *combo, int index);
SWT_PUBLIC wresult w_combo_set_item(w_combo *combo, int index, const char *string);
SWT_PUBLIC wresult w_combo_set_selection(w_combo *combo, w_point *selection);
SWT_PUBLIC wresult w_combo_set_text(w_combo *combo, const char *string);
SWT_PUBLIC wresult w_combo_set_text_limit(w_combo *combo, int limit);
SWT_PUBLIC wresult w_combo_set_visible_item_count(w_combo *combo, int count);

#define W_COMBO_CLASS(x) ((struct _w_combo_class*)x)
#define W_COMBO_GET_CLASS(x) ((struct _w_combo_class*)W_WIDGET_GET_CLASS(x))
#define W_COMBOITEM_CLASS(x) ((struct _w_comboitem_class*)x)
#define W_COMBOITEM_GET_CLASS(x) ((struct _w_comboitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_COMBO_H_ */
