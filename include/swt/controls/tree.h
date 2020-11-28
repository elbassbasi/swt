/*
 * tree.h
 *
 *  Created on: 16 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_TREE_H_
#define SWT_CONTROLS_TREE_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_treecolumn {
	w_item item;
	void *handle[6];
} w_treecolumn;
#define W_TREECOLUMN(x) ((w_treecolumn*)x)
typedef struct w_treeitem {
	w_item item;
	void *handle[6];
} w_treeitem;
#define W_TREEITEM(x) ((w_treeitem*)x)
typedef struct w_tree {
	w_composite composite;
	void *handle[10];
} w_tree;
#define W_TREE(x) ((w_tree*)x)
typedef struct w_tree_attr {
	w_font *font;
	w_color background;
	w_color foreground;
} w_tree_attr;
typedef struct w_event_tree {
	w_event event;
	union {
		int detail;
		struct {
			unsigned doit :1;
			unsigned selected :1;
			unsigned focused :1;
			unsigned background :1;
			unsigned forground :1;
			unsigned hot :1;
		};
	};
	w_rect *rect;
	w_treecolumn *column;
	w_treeitem *item;
	w_graphics *gc;
	w_value *value;
	w_tree_attr *attr;
} w_event_tree;
struct _w_treecolumn_class {
	struct _w_item_class item;
	int (*get_alignment)(w_treecolumn *column);
	int (*get_id)(w_treecolumn *column);
	int (*get_image)(w_treecolumn *column);
	wbool (*get_moveable)(w_treecolumn *column);
	int (*get_order)(w_treecolumn *column);
	wbool (*get_resizable)(w_treecolumn *column);
	wresult (*get_tooltip_text)(w_treecolumn *column, w_alloc alloc,void* user_data);
	int (*get_width)(w_treecolumn *column);
	wresult (*pack)(w_treecolumn *column);
	wresult (*set_alignment)(w_treecolumn *column, int alignment);
	wresult (*set_id)(w_treecolumn *column, int id);
	wresult (*set_image)(w_treecolumn *column, int image);
	wresult (*set_moveable)(w_treecolumn *column, wbool moveable);
	wresult (*set_order)(w_treecolumn *column, int order);
	wresult (*set_resizable)(w_treecolumn *column, wbool resizable);
	wresult (*set_tooltip_text)(w_treecolumn *column, const char *text);
	wresult (*set_width)(w_treecolumn *column, int width);
};
struct _w_treeitem_class {
	struct _w_item_class item;
	wresult (*clear)(w_treeitem *item, int index, wbool all);
	wresult (*clear_all)(w_treeitem *item, wbool all);
	void (*get_bounds)(w_treeitem *item, w_rect *bounds);
	wbool (*get_checked)(w_treeitem *item);
	wbool (*get_expanded)(w_treeitem *item);
	wbool (*get_first_child)(w_treeitem *item, w_treeitem *child);
	wbool (*get_grayed)(w_treeitem *item);
	wbool (*get_item)(w_treeitem *item, int index, w_treeitem *subitem);
	int (*get_item_count)(w_treeitem *item);
	void (*get_items)(w_treeitem *item, w_iterator *items);
	int (*get_image)(w_treeitem *item);
	wbool (*get_last_child)(w_treeitem *item, w_treeitem *child);
	wbool (*get_next_sibling)(w_treeitem *item, w_treeitem *next);
	wbool (*get_parent_item)(w_treeitem *item, w_treeitem *parent);
	wbool (*get_prev_sibling)(w_treeitem *item, w_treeitem *prev);
	wresult (*insert_item)(w_treeitem *item, w_treeitem *subitem,
			const char *text, int index);
	wresult (*insert_item_after)(w_treeitem *item, w_treeitem *subitem,
			const char *text, w_treeitem *after);
	wresult (*remove_all)(w_treeitem *item);
	wresult (*set_checked)(w_treeitem *item, wbool checked);
	wresult (*set_expanded)(w_treeitem *item, wbool expanded);
	wresult (*set_grayed)(w_treeitem *item, wbool grayed);
	wresult (*set_has_children)(w_treeitem *item);
	wresult (*set_image)(w_treeitem *item, int image);
	wresult (*set_item_count)(w_treeitem *item, int count);
};
struct _w_tree_class {
	struct _w_composite_class composite;
	struct _w_treeitem_class *class_treeitem;
	struct _w_treecolumn_class *class_treecolumn;
	wresult (*deselect)(w_tree *tree, w_treeitem *item);
	wresult (*deselect_all)(w_tree *tree);
	int (*get_gridline_width)(w_tree *tree);
	int (*get_header_height)(w_tree *tree);
	wbool (*get_header_visible)(w_tree *tree);
	wbool (*get_column)(w_tree *tree, int index, w_treecolumn *column);
	int (*get_column_count)(w_tree *tree);
	void (*get_columns)(w_tree *tree, w_iterator *columns);
	w_imagelist* (*get_imagelist)(w_tree *tree);
	wbool (*get_item)(w_tree *tree, w_point *point, w_treeitem *item);
	int (*get_item_height)(w_tree *tree);
	wbool (*get_lines_visible)(w_tree *tree);
	wbool (*get_parent_item)(w_tree *tree, w_treeitem *item);
	wbool (*get_root_item)(w_tree *tree, w_treeitem *root);
	void (*get_selection)(w_tree *tree, w_iterator *selection);
	int (*get_selection_count)(w_tree *tree);
	wbool (*get_sort_column)(w_tree *tree, w_treecolumn *column);
	int (*get_sort_direction)(w_tree *tree);
	wbool (*get_top_item)(w_tree *tree, w_treeitem *topitem);
	wresult (*insert_column)(w_tree *tree, w_treecolumn *column,
			const char *text, int index);
	wresult (*remove_all)(w_tree *tree);
	wresult (*set_imagelist)(w_tree *tree, w_imagelist *imagelist);
	wresult (*set_insert_mark)(w_tree *tree, w_treeitem *item, wbool before);
	wresult (*set_item_height)(w_tree *tree, int itemHeight);
	wresult (*set_lines_visible)(w_tree *tree, wbool show);
	wresult (*select)(w_tree *tree, w_treeitem *item);
	wresult (*select_all)(w_tree *tree);
	wresult (*set_header_visible)(w_tree *tree, wbool show);
	wresult (*set_selection)(w_tree *tree, w_treeitem *item);
	wresult (*set_sort_column)(w_tree *tree, w_treecolumn *column);
	wresult (*set_sort_direction)(w_tree *tree, int direction);
	wresult (*set_top_item)(w_tree *tree, w_treeitem *item);
	wresult (*show_column)(w_tree *tree, w_treecolumn *column);
	wresult (*show_item)(w_tree *tree, w_treeitem *item);
	wresult (*show_selection)(w_tree *tree);
	wresult (*sort)(w_tree *tree);
};

/*
 * tree column
 */
SWT_PUBLIC int w_treecolumn_get_alignment(w_treecolumn *column);
SWT_PUBLIC int w_treecolumn_get_id(w_treecolumn *column);
SWT_PUBLIC int w_treecolumn_get_image(w_treecolumn *column);
SWT_PUBLIC wbool w_treecolumn_get_moveable(w_treecolumn *column);
SWT_PUBLIC int w_treecolumn_get_order(w_treecolumn *column);
SWT_PUBLIC wbool w_treecolumn_get_resizable(w_treecolumn *column);
SWT_PUBLIC wresult w_treecolumn_get_tooltip_text(w_treecolumn *column,
		w_alloc alloc,void* user_data);
SWT_PUBLIC int w_treecolumn_get_width(w_treecolumn *column);
SWT_PUBLIC wresult w_treecolumn_pack(w_treecolumn *column);
SWT_PUBLIC wresult w_treecolumn_set_alignment(w_treecolumn *column,
		int alignment);
SWT_PUBLIC wresult w_treecolumn_set_id(w_treecolumn *column, int id);
SWT_PUBLIC wresult w_treecolumn_set_image(w_treecolumn *column, int image);
SWT_PUBLIC wresult w_treecolumn_set_moveable(w_treecolumn *column,
		wbool moveable);
SWT_PUBLIC wresult w_treecolumn_set_order(w_treecolumn *column, int order);
SWT_PUBLIC wresult w_treecolumn_set_resizable(w_treecolumn *column,
		wbool resizable);
SWT_PUBLIC wresult w_treecolumn_set_tooltip_text(w_treecolumn *column,
		const char *text);
SWT_PUBLIC wresult w_treecolumn_set_width(w_treecolumn *column, int width);
/*
 * tree item
 */
SWT_PUBLIC wresult w_treeitem_clear(w_treeitem *item, int index, wbool all);
SWT_PUBLIC wresult w_treeitem_clear_all(w_treeitem *item, wbool all);
SWT_PUBLIC void w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds);
SWT_PUBLIC wbool w_treeitem_get_checked(w_treeitem *item);
SWT_PUBLIC wbool w_treeitem_get_expanded(w_treeitem *item);
SWT_PUBLIC wbool w_treeitem_get_first_child(w_treeitem *item,
		w_treeitem *child);
SWT_PUBLIC wbool w_treeitem_get_grayed(w_treeitem *item);
SWT_PUBLIC wbool w_treeitem_get_item(w_treeitem *item, int index,
		w_treeitem *subitem);
SWT_PUBLIC int w_treeitem_get_item_count(w_treeitem *item);
SWT_PUBLIC void w_treeitem_get_items(w_treeitem *item, w_iterator *items);
SWT_PUBLIC int w_treeitem_get_image(w_treeitem *item);
SWT_PUBLIC wbool w_treeitem_get_last_child(w_treeitem *item,
		w_treeitem *child);
SWT_PUBLIC wbool w_treeitem_get_next_sibling(w_treeitem *item,
		w_treeitem *next);
SWT_PUBLIC wbool w_treeitem_get_parent_item(w_treeitem *item,
		w_treeitem *parent);
SWT_PUBLIC wbool w_treeitem_get_prev_sibling(w_treeitem *item,
		w_treeitem *prev);
SWT_PUBLIC wresult w_treeitem_insert_item(w_treeitem *item,
		w_treeitem *subitem, const char *text, int index);
SWT_PUBLIC wresult w_treeitem_insert_item_after(w_treeitem *item,
		w_treeitem *subitem, const char *text, w_treeitem *after);
SWT_PUBLIC wresult w_treeitem_remove_all(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_set_checked(w_treeitem *item, wbool checked);
SWT_PUBLIC wresult w_treeitem_set_expanded(w_treeitem *item, wbool expanded);
SWT_PUBLIC wresult w_treeitem_set_grayed(w_treeitem *item, wbool grayed);
SWT_PUBLIC wresult w_treeitem_set_has_children(w_treeitem *item);
SWT_PUBLIC wresult w_treeitem_set_image(w_treeitem *item, int image);
SWT_PUBLIC wresult w_treeitem_set_item_count(w_treeitem *item, int count);
/*
 * tree
 */
SWT_PUBLIC wresult w_tree_create(w_tree *tree, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_tree* w_tree_new(struct w_toolkit *toolkit, w_composite *parent,
                              wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_tree_deselect(w_tree *tree, w_treeitem *item);
SWT_PUBLIC wresult w_tree_deselect_all(w_tree *tree);
SWT_PUBLIC int w_tree_get_gridline_width(w_tree *tree);
SWT_PUBLIC int w_tree_get_header_height(w_tree *tree);
SWT_PUBLIC wbool w_tree_get_header_visible(w_tree *tree);
SWT_PUBLIC wbool w_tree_get_column(w_tree *tree, int index,
		w_treecolumn *column);
SWT_PUBLIC int w_tree_get_column_count(w_tree *tree);
SWT_PUBLIC void w_tree_get_columns(w_tree *tree, w_iterator *columns);
SWT_PUBLIC w_imagelist* w_tree_get_imagelist(w_tree *tree);
SWT_PUBLIC wbool w_tree_get_item(w_tree *tree, w_point *point,
		w_treeitem *item);
SWT_PUBLIC int w_tree_get_item_height(w_tree *tree);
SWT_PUBLIC wbool w_tree_get_lines_visible(w_tree *tree);
SWT_PUBLIC wbool w_tree_get_parent_item(w_tree *tree, w_treeitem *item);
SWT_PUBLIC wbool w_tree_get_root_item(w_tree *tree, w_treeitem *root);
SWT_PUBLIC void w_tree_get_selection(w_tree *tree, w_iterator *selection);
SWT_PUBLIC int w_tree_get_selection_count(w_tree *tree);
SWT_PUBLIC wbool w_tree_get_sort_column(w_tree *tree, w_treecolumn *column);
SWT_PUBLIC int w_tree_get_sort_direction(w_tree *tree);
SWT_PUBLIC wbool w_tree_get_top_item(w_tree *tree, w_treeitem *topitem);
SWT_PUBLIC wresult w_tree_insert_column(w_tree *tree, w_treecolumn *column,
		const char *text, int index);
SWT_PUBLIC wresult w_tree_remove_all(w_tree *tree);
SWT_PUBLIC wresult w_tree_set_imagelist(w_tree *tree, w_imagelist *imagelist);
SWT_PUBLIC wresult w_tree_set_insert_mark(w_tree *tree, w_treeitem *item,
		wbool before);
SWT_PUBLIC wresult w_tree_set_item_height(w_tree *tree, int itemHeight);
SWT_PUBLIC wresult w_tree_set_lines_visible(w_tree *tree, wbool show);
SWT_PUBLIC wresult w_tree_select(w_tree *tree, w_treeitem *item);
SWT_PUBLIC wresult w_tree_select_all(w_tree *tree);
SWT_PUBLIC wresult w_tree_set_header_visible(w_tree *tree, wbool show);
SWT_PUBLIC wresult w_tree_set_selection(w_tree *tree, w_treeitem *item);
SWT_PUBLIC wresult w_tree_set_sort_column(w_tree *tree, w_treecolumn *column);
SWT_PUBLIC wresult w_tree_set_sort_direction(w_tree *tree, int direction);
SWT_PUBLIC wresult w_tree_set_top_item(w_tree *tree, w_treeitem *item);
SWT_PUBLIC wresult w_tree_show_column(w_tree *tree, w_treecolumn *column);
SWT_PUBLIC wresult w_tree_show_item(w_tree *tree, w_treeitem *item);
SWT_PUBLIC wresult w_tree_show_selection(w_tree *tree);
SWT_PUBLIC wresult w_tree_sort(w_tree *tree);

#define W_TREE_CLASS(x) ((struct _w_tree_class*)x)
#define W_TREE_GET_CLASS(x) ((struct _w_tree_class*)W_WIDGET_GET_CLASS(x))
#define W_TREECOLUMN_CLASS(x) ((struct _w_treecolumn_class*)x)
#define W_TREECOLUMN_GET_CLASS(x) ((struct _w_treecolumn_class*)W_WIDGETDATA_GET_CLASS(x))
#define W_TREEITEM_CLASS(x) ((struct _w_treeitem_class*)x)
#define W_TREEITEM_GET_CLASS(x) ((struct _w_treeitem_class*)W_WIDGETDATA_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_TREE_H_ */
