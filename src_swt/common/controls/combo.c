/*
 * combo.c
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>
wresult w_combo_create(w_combo *combo, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(combo), toolkit, W_WIDGET(parent), style,
			_W_CLASS_COMBO, post_event);
}
w_combo* w_combo_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_COMBO(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_COMBO,post_event));
}
wresult w_combo_insert(w_combo *combo, w_comboitem *item, const char *string,
		int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->insert(combo, item, string, index);
	} else
		return result;
}
wresult w_combo_clear_selection(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->clear_selection(combo);
	} else
		return result;
}
wresult w_combo_copy(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->copy(combo);
	} else
		return result;
}
wresult w_combo_cut(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->cut(combo);
	} else
		return result;
}
wresult w_combo_deselect(w_combo *combo, int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->deselect(combo, index);
	} else
		return result;
}
wresult w_combo_deselect_all(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->deselect_all(combo);
	} else
		return result;
}
wresult w_combo_get_item(w_combo *combo, int index, w_comboitem *item) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_item(combo, index, item);
	} else
		return result;
}
int w_combo_get_item_count(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_item_count(combo);
	} else
		return 0;
}
int w_combo_get_item_height(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_item_height(combo);
	} else
		return 0;
}
void w_combo_get_items(w_combo *combo, w_iterator *items) {
	w_iterator_close(items);
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		W_COMBO_GET_CLASS(combo)->get_items(combo, items);
	}
}
wresult w_combo_get_list_visible(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_list_visible(combo);
	} else
		return W_FALSE;
}
wresult w_combo_set_list_visible(w_combo *combo, int visible) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->set_list_visible(combo, visible);
	} else
		return result;
}
wresult w_combo_get_selection(w_combo *combo, w_point *pt) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_selection(combo, pt);
	} else
		return W_FALSE;
}
int w_combo_get_selection_index(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_selection_index(combo);
	} else
		return -1;
}
wresult w_combo_get_text(w_combo *combo,w_alloc alloc,void* user_data) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_text(combo,alloc,user_data);
	} else
		return 0;
}
int w_combo_get_text_height(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_text_height(combo);
	} else
		return 0;
}
int w_combo_get_text_limit(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_text_limit(combo);
	} else
		return 0;
}
int w_combo_get_visible_item_count(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->get_visible_item_count(combo);
	} else
		return 0;
}
int w_combo_indexof(w_combo *combo, const char *string, int start) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->indexof(combo, string, start);
	} else
		return -1;
}
wresult w_combo_paste(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->paste(combo);
	} else
		return result;
}
wresult w_combo_remove_index(w_combo *combo, int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->remove_index(combo, index);
	} else
		return result;
}
wresult w_combo_remove_range(w_combo *combo, int start, int end) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->remove_range(combo, start, end);
	} else
		return result;
}
wresult w_combo_remove_string(w_combo *combo, const char *string) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->remove_string(combo, string);
	} else
		return result;
}
wresult w_combo_remove_all(w_combo *combo) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->remove_all(combo);
	} else
		return result;
}
wresult w_combo_select(w_combo *combo, int index) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->select(combo, index);
	} else
		return result;
}
wresult w_combo_set_item(w_combo *combo, int index, const char *string) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->set_item(combo, index, string);
	} else
		return result;
}
wresult w_combo_set_selection(w_combo *combo, w_point *selection) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->set_selection(combo, selection);
	} else
		return result;
}
wresult w_combo_set_text(w_combo *combo, const char *string) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->set_text(combo, string);
	} else
		return result;
}
wresult w_combo_set_text_limit(w_combo *combo, int limit) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->set_text_limit(combo, limit);
	} else
		return result;
}
wresult w_combo_set_visible_item_count(w_combo *combo, int count) {
	wresult result = W_WIDGET_CHECK0(combo);
	if (result > 0) {
		return W_COMBO_GET_CLASS(combo)->set_visible_item_count(combo, count);
	} else
		return result;
}
