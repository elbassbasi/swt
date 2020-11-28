/*
 * list.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

wresult w_list_create(w_list *list, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(list), toolkit, W_WIDGET(parent), style,
			_W_CLASS_LIST, post_event);
}
w_list* w_list_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_LIST(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_LIST,post_event));
}
wresult w_list_insert(w_list *list, w_listitem *item, const char *string,
		int index) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->insert(list, item, string, index);
	} else
		return res;
}
wresult w_list_deselect_indices(w_list *list, int *indices, size_t length) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->deselect_indices(list, indices, length);
	} else
		return res;
}
wresult w_list_deselect_index(w_list *list, int index) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->deselect_index(list, index);
	} else
		return res;
}
wresult w_list_deselect_range(w_list *list, int start, int end) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->select_range(list, start, end);
	} else
		return res;
}
wresult w_list_deselect_all(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->deselect_all(list);
	} else
		return res;
}
int w_list_get_focus_index(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_focus_index(list);
	} else
		return -1;
}
wresult w_list_get_item(w_list *list, int index, w_listitem *item) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_item(list,index, item);
	} else
		return res;
}
int w_list_get_item_count(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_item_count(list);
	} else
		return 0;
}
int w_list_get_item_height(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_item_height(list);
	} else
		return 0;
}
void w_list_get_items(w_list *list, w_iterator *items) {
	w_iterator_close(items);
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_items(list, items);
	}
}
void w_list_get_selection(w_list *list, w_iterator *selections) {
	w_iterator_close(selections);
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_selection(list, selections);
	}
}
int w_list_get_selection_count(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_selection_count(list);
	} else
		return 0;
}
int w_list_get_selection_index(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_selection_index(list);
	} else
		return -1;
}
int w_list_get_top_index(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->get_top_index(list);
	} else
		return -1;
}
int w_list_index_of(w_list *list, const char *string, int start) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->index_of(list, string, start);
	} else
		return -1;
}
wbool w_list_is_selected(w_list *list, int index) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->is_selected(list, index);
	} else
		return W_FALSE;
}
wresult w_list_remove_indices(w_list *list, int *indices, size_t length) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->remove_indices(list, indices, length);
	} else
		return res;
}
wresult w_list_remove_index(w_list *list, int index) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->remove_index(list, index);
	} else
		return res;
}
wresult w_list_remove_range(w_list *list, int start, int end) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->remove_range(list, start, end);
	} else
		return res;
}
wresult w_list_remove_string(w_list *list, const char *string) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->remove_string(list, string);
	} else
		return res;
}
wresult w_list_remove_all(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->remove_all(list);
	} else
		return res;
}
wresult w_list_select_indices(w_list *list, int *indices, size_t length) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->select_indices(list, indices, length);
	} else
		return res;
}
wresult w_list_select_index(w_list *list, int index) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->select_index(list, index);
	} else
		return res;
}
wresult w_list_select_range(w_list *list, int start, int end) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->select_range(list, start, end);
	} else
		return res;
}
wresult w_list_select_all(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->select_all(list);
	} else
		return res;
}
wresult w_list_set_item(w_list *list, int index, const char *string) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->set_item(list, index, string);
	} else
		return res;
}
wresult w_list_set_items(w_list *list, const char **items, size_t length) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->set_items(list, items, length);
	} else
		return res;
}
wresult w_list_set_selection_indices(w_list *list, int *indices,
		size_t length) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->set_selection_indices(list, indices,
				length);
	} else
		return res;
}
wresult w_list_set_selection_strings(w_list *list, const char **items,
		size_t length) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->set_selection_strings(list, items,
				length);
	} else
		return res;
}
wresult w_list_set_selection_index(w_list *list, int index) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->set_selection_index(list, index);
	} else
		return res;
}
wresult w_list_set_selection_range(w_list *list, int start, int end) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->set_selection_range(list, start, end);
	} else
		return res;
}
wresult w_list_set_top_index(w_list *list, int index) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->set_top_index(list, index);
	} else
		return res;
}
wresult w_list_show_selection(w_list *list) {
	wresult res = W_WIDGET_CHECK0(list);
	if (res > 0) {
		return W_LIST_GET_CLASS(list)->show_selection(list);
	} else
		return res;
}
