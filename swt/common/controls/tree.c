/*
 * tree.c
 *
 *  Created on: 17 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>

/*
 * tree column
 */
int w_treecolumn_get_alignment(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_alignment(column);
	} else
		return 0;
}
int w_treecolumn_get_id(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_id(column);
	} else
		return -1;
}
int w_treecolumn_get_image(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_image(column);
	} else
		return -1;
}
wbool w_treecolumn_get_moveable(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_moveable(column);
	} else
		return W_FALSE;
}
int w_treecolumn_get_order(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_order(column);
	} else
		return -1;
}
wbool w_treecolumn_get_resizable(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_resizable(column);
	} else
		return W_FALSE;
}
wresult w_treecolumn_get_tooltip_text(w_treecolumn *column,w_alloc alloc,void* user_data) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_tooltip_text(column,alloc,user_data);
	} else
		return 0;
}
int w_treecolumn_get_width(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->get_width(column);
	} else
		return 0;
}
wresult w_treecolumn_pack(w_treecolumn *column) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->pack(column);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_alignment(w_treecolumn *column, int alignment) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_alignment(column, alignment);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_id(w_treecolumn *column, int id) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_id(column, id);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_image(w_treecolumn *column, int image) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_image(column, image);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_moveable(w_treecolumn *column, wbool moveable) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_moveable(column, moveable);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_order(w_treecolumn *column, int order) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_order(column, order);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_resizable(w_treecolumn *column, wbool resizable) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_resizable(column, resizable);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_tooltip_text(w_treecolumn *column, const char *text) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_tooltip_text(column, text);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treecolumn_set_width(w_treecolumn *column, int width) {
	if (W_WIDGETDATA_CHECK(column)) {
		return W_TREECOLUMN_GET_CLASS(column)->set_width(column, width);
	} else
		return W_ERROR_NO_HANDLES;
}
/*
 * tree item
 */
wresult w_treeitem_clear(w_treeitem *item, int index, wbool all) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->clear(item, index, all);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_clear_all(w_treeitem *item, wbool all) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->clear_all(item, all);
	} else
		return W_ERROR_NO_HANDLES;
}
void w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
	if (W_WIDGETDATA_CHECK(item)) {
		W_TREEITEM_GET_CLASS(item)->get_bounds(item, bounds);
	} else {
		memset(bounds, 0, sizeof(w_rect));
	}
}
wbool w_treeitem_get_checked(w_treeitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_checked(item);
	} else
		return W_FALSE;
}
wbool w_treeitem_get_expanded(w_treeitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_expanded(item);
	} else
		return W_FALSE;
}
wbool w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
	w_widgetdata_close(W_WIDGETDATA(child));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_first_child(item, child);
	} else
		return W_FALSE;
}
wbool w_treeitem_get_grayed(w_treeitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_grayed(item);
	} else
		return W_FALSE;
}
wbool w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_item(item, index, subitem);
	} else
		return W_FALSE;
}
int w_treeitem_get_item_count(w_treeitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_item_count(item);
	} else
		return 0;
}
void w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
	w_iterator_close(items);
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_items(item, items);
	}
}
int w_treeitem_get_image(w_treeitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_image(item);
	} else
		return -1;
}
wbool w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
	w_widgetdata_close(W_WIDGETDATA(child));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_last_child(item, child);
	} else
		return W_FALSE;
}
wbool w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
	w_widgetdata_close(W_WIDGETDATA(next));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_next_sibling(item, next);
	} else
		return W_FALSE;
}
wbool w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
	w_widgetdata_close(W_WIDGETDATA(parent));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_parent_item(item, parent);
	} else
		return W_FALSE;
}
wbool w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
	w_widgetdata_close(W_WIDGETDATA(prev));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->get_prev_sibling(item, prev);
	} else
		return W_FALSE;
}
wresult w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		const char *text, int index) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->insert_item(item, subitem, text,
				index);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		const char *text, w_treeitem *after) {
	w_widgetdata_close(W_WIDGETDATA(subitem));
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->insert_item_after(item, subitem,
				text, after);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_remove_all(w_treeitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->remove_all(item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_set_checked(w_treeitem *item, wbool checked) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->set_checked(item, checked);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_set_expanded(w_treeitem *item, wbool expanded) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->set_expanded(item, expanded);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_set_grayed(w_treeitem *item, wbool grayed) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->set_grayed(item, grayed);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_set_has_children(w_treeitem *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->set_has_children(item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_set_image(w_treeitem *item, int image) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->set_image(item, image);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_treeitem_set_item_count(w_treeitem *item, int count) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_TREEITEM_GET_CLASS(item)->set_item_count(item, count);
	} else
		return W_ERROR_NO_HANDLES;
}
/*
 * tree
 */
wresult w_tree_create(w_tree *tree, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(tree), toolkit, W_WIDGET(parent), style,
			_W_CLASS_TREE, post_event);
}
w_tree* w_tree_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return W_TREE(
			_w_widget_new(toolkit,W_WIDGET(parent),style,_W_CLASS_TREE,post_event));
}
wresult w_tree_deselect(w_tree *tree, w_treeitem *item) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->deselect(tree, item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_deselect_all(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->deselect_all(tree);
	} else
		return W_ERROR_NO_HANDLES;
}
int w_tree_get_gridline_width(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_gridline_width(tree);
	} else
		return 0;
}
int w_tree_get_header_height(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_header_height(tree);
	} else
		return 0;
}
wbool w_tree_get_header_visible(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_header_visible(tree);
	} else
		return W_FALSE;
}
wbool w_tree_get_column(w_tree *tree, int index, w_treecolumn *column) {
	w_widgetdata_close(W_WIDGETDATA(column));
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_column(tree, index, column);
	} else
		return W_FALSE;
}
int w_tree_get_column_count(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_column_count(tree);
	} else
		return 0;
}
void w_tree_get_columns(w_tree *tree, w_iterator *columns) {
	w_iterator_close(columns);
	if (W_WIDGET_CHECK(tree)) {
		W_TREE_GET_CLASS(tree)->get_columns(tree, columns);
	}
}
w_imagelist* w_tree_get_imagelist(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_imagelist(tree);
	} else
		return 0;
}
wbool w_tree_get_item(w_tree *tree, w_point *point, w_treeitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_item(tree, point, item);
	} else
		return W_FALSE;
}
int w_tree_get_item_height(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_item_height(tree);
	} else
		return 0;
}
wbool w_tree_get_lines_visible(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_lines_visible(tree);
	} else
		return W_FALSE;
}
wbool w_tree_get_parent_item(w_tree *tree, w_treeitem *item) {
	w_widgetdata_close(W_WIDGETDATA(item));
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_parent_item(tree, item);
	} else
		return W_FALSE;
}
wbool w_tree_get_root_item(w_tree *tree, w_treeitem *root) {
	w_widgetdata_close(W_WIDGETDATA(root));
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_root_item(tree, root);
	} else
		return W_FALSE;
}
void w_tree_get_selection(w_tree *tree, w_iterator *selection) {
	w_iterator_close(selection);
	if (W_WIDGET_CHECK(tree)) {
		W_TREE_GET_CLASS(tree)->get_selection(tree, selection);
	}
}
int w_tree_get_selection_count(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_selection_count(tree);
	} else
		return 0;
}
wbool w_tree_get_sort_column(w_tree *tree, w_treecolumn *column) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_sort_column(tree, column);
	} else
		return W_FALSE;
}
int w_tree_get_sort_direction(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_sort_direction(tree);
	} else
		return 0;
}
wbool w_tree_get_top_item(w_tree *tree, w_treeitem *topitem) {
	w_widgetdata_close(W_WIDGETDATA(topitem));
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->get_top_item(tree, topitem);
	} else
		return W_FALSE;
}
wresult w_tree_insert_column(w_tree *tree, w_treecolumn *column,
		const char *text, int index) {
	w_widgetdata_close(W_WIDGETDATA(column));
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->insert_column(tree, column, text, index);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_remove_all(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->remove_all(tree);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_imagelist(w_tree *tree, w_imagelist *imagelist) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_imagelist(tree, imagelist);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_insert_mark(w_tree *tree, w_treeitem *item, wbool before) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_insert_mark(tree, item, before);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_item_height(w_tree *tree, int itemHeight) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_item_height(tree, itemHeight);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_lines_visible(w_tree *tree, wbool show) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_lines_visible(tree, show);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_select(w_tree *tree, w_treeitem *item) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->select(tree, item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_select_all(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->select_all(tree);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_header_visible(w_tree *tree, wbool show) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_header_visible(tree, show);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_selection(w_tree *tree, w_treeitem *item) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_selection(tree, item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_sort_column(w_tree *tree, w_treecolumn *column) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_sort_column(tree, column);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_set_sort_direction(w_tree *tree, int direction) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_sort_direction(tree, direction);
	} else
		return W_ERROR_NO_HANDLES;
}

wresult w_tree_set_top_item(w_tree *tree, w_treeitem *item) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->set_top_item(tree, item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_show_column(w_tree *tree, w_treecolumn *column) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->show_column(tree, column);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_show_item(w_tree *tree, w_treeitem *item) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->show_item(tree, item);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_tree_show_selection(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->show_selection(tree);
	} else
		return W_ERROR_NO_HANDLES;
}

wresult w_tree_sort(w_tree *tree) {
	if (W_WIDGET_CHECK(tree)) {
		return W_TREE_GET_CLASS(tree)->sort(tree);
	} else
		return W_ERROR_NO_HANDLES;
}

