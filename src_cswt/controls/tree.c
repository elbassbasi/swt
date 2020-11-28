//
// Created by azeddine on 27/08/2020.
//

#include "tree.h"
/*
 * tree column
 */
wresult wc_treecolumn_is_ok(w_widgetdata *obj){}
void wc_treecolumn_close(w_widgetdata *obj){}
void wc_treecolumn_copy(w_widgetdata *from, w_widgetdata *to){}
wresult wc_treecolumn_equals(w_widgetdata *obj1, w_widgetdata *obj2){}
w_widget* wc_treecolumn_get_parent_widget(w_item *item){}
void* wc_treecolumn_get_data(w_item *item){}
int wc_treecolumn_get_index(w_item *item){}
wresult wc_treecolumn_get_text(w_item *item, w_alloc *text){}
wresult wc_treecolumn_set_data(w_item *item, void *data){}
wresult wc_treecolumn_set_text(w_item *item, const char *string){}
int wc_treecolumn_get_alignment(w_treecolumn *column){}
int wc_treecolumn_get_id(w_treecolumn *column){}
int wc_treecolumn_get_image(w_treecolumn *column){}
wresult wc_treecolumn_get_moveable(w_treecolumn *column){}
int wc_treecolumn_get_order(w_treecolumn *column){}
wresult wc_treecolumn_get_resizable(w_treecolumn *column){}
wresult wc_treecolumn_get_tooltip_text(w_treecolumn *column, w_alloc *text){}
int wc_treecolumn_get_width(w_treecolumn *column){}
wresult wc_treecolumn_pack(w_treecolumn *column){}
wresult wc_treecolumn_set_alignment(w_treecolumn *column, int alignment){}
wresult wc_treecolumn_set_id(w_treecolumn *column, int id){}
wresult wc_treecolumn_set_image(w_treecolumn *column, int image){}
wresult wc_treecolumn_set_moveable(w_treecolumn *column, int moveable){}
wresult wc_treecolumn_set_order(w_treecolumn *column, int order){}
wresult wc_treecolumn_set_resizable(w_treecolumn *column, int resizable){}
wresult wc_treecolumn_set_tooltip_text(w_treecolumn *column, const char *text){}
wresult wc_treecolumn_set_width(w_treecolumn *column, int width){}
/*
 * tree item
 */
wresult wc_treeitem_is_ok(w_widgetdata *obj){}
void wc_treeitem_close(w_widgetdata *obj){}
void wc_treeitem_copy(w_widgetdata *from, w_widgetdata *to){}
wresult wc_treeitem_equals(w_widgetdata *obj1, w_widgetdata *obj2){}
w_widget* wc_treeitem_get_parent_widget(w_item *item){}
void* wc_treeitem_get_data(w_item *item){}
int wc_treeitem_get_index(w_item *item){}
wresult wc_treeitem_get_text(w_item *item, w_alloc *text){}
wresult wc_treeitem_set_data(w_item *item, void *data){}
wresult wc_treeitem_set_text(w_item *item, const char *string){}
wresult wc_treeitem_clear(w_treeitem *item, int index, int all){}
wresult wc_treeitem_clear_all(w_treeitem *item, int all){}
void wc_treeitem_get_bounds(w_treeitem *item, w_rect *bounds){}
wresult wc_treeitem_get_checked(w_treeitem *item){}
wresult wc_treeitem_get_expanded(w_treeitem *item){}
wresult wc_treeitem_get_first_child(w_treeitem *item, w_treeitem *child){}
wresult wc_treeitem_get_grayed(w_treeitem *item){}
wresult wc_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem){}
int wc_treeitem_get_item_count(w_treeitem *item){}
void wc_treeitem_get_items(w_treeitem *item, w_iterator *items){}
int wc_treeitem_get_image(w_treeitem *item){}
wresult wc_treeitem_get_last_child(w_treeitem *item, w_treeitem *child){}
wresult wc_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next){}
wresult wc_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent){}
wresult wc_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev){}
wresult wc_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
                        const char *text, int index){}
wresult wc_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
                              const char *text, w_treeitem *after){}
wresult wc_treeitem_remove_all(w_treeitem *item){}
wresult wc_treeitem_set_checked(w_treeitem *item, int checked){}
wresult wc_treeitem_set_expanded(w_treeitem *item, int expanded){}
wresult wc_treeitem_set_grayed(w_treeitem *item, int grayed){}
wresult wc_treeitem_set_has_children(w_treeitem *item){}
wresult wc_treeitem_set_image(w_treeitem *item, int image){}
wresult wc_treeitem_set_item_count(w_treeitem *item, int count){}
/*
 * tree
 */
wresult wc_tree_create(w_widget *widget, w_widget *parent, wuint64 style,
                             w_widget_post_event_proc post_event) {
    return wc_control_create(widget, parent, style | W_HSCROLL | W_VSCROLL , post_event, sizeof(struct wc_tree_priv));
}
wresult wc_tree_deselect(w_tree *tree, w_treeitem *item){}
wresult wc_tree_deselect_all(w_tree *tree){}
int wc_tree_get_gridline_width(w_tree *tree){}
int wc_tree_get_header_height(w_tree *tree){}
wresult wc_tree_get_header_visible(w_tree *tree){}
wresult wc_tree_get_column(w_tree *tree, int index, w_treecolumn *column){}
int wc_tree_get_column_count(w_tree *tree){}
void wc_tree_get_columns(w_tree *tree, w_iterator *columns){}
w_imagelist* wc_tree_get_imagelist(w_tree *tree){}
wresult wc_tree_get_item(w_tree *tree, w_point *point, w_treeitem *item){}
int wc_tree_get_item_height(w_tree *tree){}
wresult wc_tree_get_lines_visible(w_tree *tree){}
wresult wc_tree_get_parent_item(w_tree *tree, w_treeitem *item){}
wresult wc_tree_get_root_item(w_tree *tree, w_treeitem *root){}
void wc_tree_get_selection(w_tree *tree, w_iterator *selection){}
int wc_tree_get_selection_count(w_tree *tree){}
wresult wc_tree_get_sort_column(w_tree *tree, w_treecolumn *column){}
int wc_tree_get_sort_direction(w_tree *tree){}
wresult wc_tree_get_top_item(w_tree *tree, w_treeitem *topitem){}
wresult wc_tree_insert_column(w_tree *tree, w_treecolumn *column,
                          const char *text, int index){}
wresult wc_tree_remove_all(w_tree *tree){}
wresult wc_tree_set_imagelist(w_tree *tree, w_imagelist *imagelist){}
wresult wc_tree_set_insert_mark(w_tree *tree, w_treeitem *item, int before){}
wresult wc_tree_set_item_height(w_tree *tree, int itemHeight){}
wresult wc_tree_set_lines_visible(w_tree *tree, int show){}
wresult wc_tree_select(w_tree *tree, w_treeitem *item){}
wresult wc_tree_select_all(w_tree *tree){}
wresult wc_tree_set_header_visible(w_tree *tree, int show){}
wresult wc_tree_set_selection(w_tree *tree, w_treeitem *item){}
wresult wc_tree_set_sort_column(w_tree *tree, w_treecolumn *column){}
wresult wc_tree_set_sort_direction(w_tree *tree, int direction){}
wresult wc_tree_set_top_item(w_tree *tree, w_treeitem *item){}
wresult wc_tree_show_column(w_tree *tree, w_treecolumn *column){}
wresult wc_tree_show_item(w_tree *tree, w_treeitem *item){}
wresult wc_tree_show_selection(w_tree *tree){}
wresult wc_tree_sort(w_tree *tree){}
/*
 * event
 */
int wc_tree_paint(w_widget *widget, w_event_paint *e) {
    struct wc_tree_priv *priv = wc_control_get_priv(W_CONTROL(widget));
    w_rect r;
    w_control_get_bounds(W_CONTROL(widget),&r.pt,&r.sz);
    w_graphics_set_background(e->gc,W_COLOR_WHITE);
    w_graphics_fill_rectangle(e->gc,&r);
    r.width -=1;
    r.height -=1;
    w_graphics_set_foreground(e->gc,W_COLOR_DARK_BLUE);
    w_graphics_draw_rectangle(e->gc,&r);
    return W_TRUE;
}
int wc_tree_mousedown(w_widget *widget, w_event_mouse *e) {
    struct wc_tree_priv *priv = wc_control_get_priv(W_CONTROL(widget));
    return W_TRUE;
}
int wc_tree_mouseup(w_widget *widget, w_event_mouse *e) {
    struct wc_tree_priv *priv = wc_control_get_priv(W_CONTROL(widget));
    return W_TRUE;
}
int wc_tree_compute_size(w_widget *widget, w_event_compute_size *e) {
    if(e->wHint != W_DEFAULT && e->wHint != W_DEFAULT){
        e->size->width = e->wHint;
        e->size->height = e->hHint;
        return W_TRUE;
    }
    w_rect bounds,result;
    struct wc_tree_priv *priv = wc_control_get_priv(W_CONTROL(widget));
    if(e->wHint == W_DEFAULT) e->size->width = result.width;
    else e->size->width = e->wHint;
    if(e->hHint == W_DEFAULT) e->size->height = result.height;
    else e->size->height = e->hHint;
    return W_TRUE;
}
int wc_tree_post_event(w_widget *widget, w_event *e) {
    switch (e->type) {
        case W_EVENT_PAINT:
            return wc_tree_paint(widget, (w_event_paint*) e);
            break;
        case W_EVENT_MOUSEDOWN:
            return wc_tree_mousedown(widget, (w_event_mouse*) e);
            break;
        case W_EVENT_MOUSEUP:
            return wc_tree_mouseup(widget, (w_event_mouse*) e);
            break;
        case W_EVENT_COMPUTE_SIZE:
            return wc_tree_compute_size(widget,(w_event_compute_size*)e);
            break;
    }
    return widget->clazz->next_class->post_event(widget, e);
}
void wc_tree_init_class(struct _w_tree_class *clazz){
    wc_composite_init_class_priv(W_COMPOSITE_CLASS(clazz),sizeof(wc_tree_priv));
    W_WIDGET_CLASS(clazz)->create = wc_tree_create;
    W_WIDGET_CLASS(clazz)->post_event = wc_tree_post_event;
    clazz->deselect=wc_tree_deselect;
    clazz->deselect_all=wc_tree_deselect_all;
    clazz->get_gridline_width=wc_tree_get_gridline_width;
    clazz->get_header_height=wc_tree_get_header_height;
    clazz->get_header_visible=wc_tree_get_header_visible;
    clazz->get_column=wc_tree_get_column;
    clazz->get_column_count=wc_tree_get_column_count;
    clazz->get_columns=wc_tree_get_columns;
    clazz->get_imagelist=wc_tree_get_imagelist;
    clazz->get_item=wc_tree_get_item;
    clazz->get_item_height=wc_tree_get_item_height;
    clazz->get_lines_visible=wc_tree_get_lines_visible;
    clazz->get_parent_item=wc_tree_get_parent_item;
    clazz->get_root_item=wc_tree_get_root_item;
    clazz->get_selection=wc_tree_get_selection;
    clazz->get_selection_count=wc_tree_get_selection_count;
    clazz->get_sort_column=wc_tree_get_sort_column;
    clazz->get_sort_direction=wc_tree_get_sort_direction;
    clazz->get_top_item=wc_tree_get_top_item;
    clazz->insert_column=wc_tree_insert_column;
    clazz->remove_all=wc_tree_remove_all;
    clazz->set_imagelist=wc_tree_set_imagelist;
    clazz->set_insert_mark=wc_tree_set_insert_mark;
    clazz->set_item_height=wc_tree_set_item_height;
    clazz->set_lines_visible=wc_tree_set_lines_visible;
    clazz->select=wc_tree_select;
    clazz->select_all=wc_tree_select_all;
    clazz->set_header_visible=wc_tree_set_header_visible;
    clazz->set_selection=wc_tree_set_selection;
    clazz->set_sort_column=wc_tree_set_sort_column;
    clazz->set_sort_direction=wc_tree_set_sort_direction;
    clazz->set_top_item=wc_tree_set_top_item;
    clazz->show_column=wc_tree_show_column;
    clazz->show_item=wc_tree_show_item;
    clazz->show_selection=wc_tree_show_selection;
    clazz->sort=wc_tree_sort;
    /*
     * treecolumn class
     */
    struct _w_treecolumn_class *column = clazz->class_treecolumn;
    W_WIDGETDATA_CLASS(column)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
    W_WIDGETDATA_CLASS(column)->is_ok=wc_treecolumn_is_ok;
    W_WIDGETDATA_CLASS(column)->close=wc_treecolumn_close;
    W_WIDGETDATA_CLASS(column)->copy=wc_treecolumn_copy;
    W_WIDGETDATA_CLASS(column)->equals=wc_treecolumn_equals;
    W_ITEM_CLASS(column)->get_parent_widget=wc_treecolumn_get_parent_widget;
    W_ITEM_CLASS(column)->get_data=wc_treecolumn_get_data;
    W_ITEM_CLASS(column)->get_index=wc_treecolumn_get_index;
    W_ITEM_CLASS(column)->get_text=wc_treecolumn_get_text;
    W_ITEM_CLASS(column)->set_data=wc_treecolumn_set_data;
    W_ITEM_CLASS(column)->set_text=wc_treecolumn_set_text;
    column->get_alignment=wc_treecolumn_get_alignment;
    column->get_id=wc_treecolumn_get_id;
    column->get_image=wc_treecolumn_get_image;
    column->get_moveable=wc_treecolumn_get_moveable;
    column->get_order=wc_treecolumn_get_order;
    column->get_resizable=wc_treecolumn_get_resizable;
    column->get_tooltip_text=wc_treecolumn_get_tooltip_text;
    column->get_width=wc_treecolumn_get_width;
    column->pack=wc_treecolumn_pack;
    column->set_alignment=wc_treecolumn_set_alignment;
    column->set_id=wc_treecolumn_set_id;
    column->set_image=wc_treecolumn_set_image;
    column->set_moveable=wc_treecolumn_set_moveable;
    column->set_order=wc_treecolumn_set_order;
    column->set_resizable=wc_treecolumn_set_resizable;
    column->set_tooltip_text=wc_treecolumn_set_tooltip_text;
    column->set_width=wc_treecolumn_set_width;
    /*
     * treeitem class
     */
    struct _w_treeitem_class *item = clazz->class_treeitem;
    W_WIDGETDATA_CLASS(item)->toolkit = W_WIDGET_CLASS(clazz)->toolkit;
    W_WIDGETDATA_CLASS(item)->is_ok=wc_treeitem_is_ok;
    W_WIDGETDATA_CLASS(item)->close=wc_treeitem_close;
    W_WIDGETDATA_CLASS(item)->copy=wc_treeitem_copy;
    W_WIDGETDATA_CLASS(item)->equals=wc_treeitem_equals;
    W_ITEM_CLASS(item)->get_parent_widget=wc_treeitem_get_parent_widget;
    W_ITEM_CLASS(item)->get_data=wc_treeitem_get_data;
    W_ITEM_CLASS(item)->get_index=wc_treeitem_get_index;
    W_ITEM_CLASS(item)->get_text=wc_treeitem_get_text;
    W_ITEM_CLASS(item)->set_data=wc_treeitem_set_data;
    W_ITEM_CLASS(item)->set_text=wc_treeitem_set_text;
    item->clear=wc_treeitem_clear;
    item->clear_all=wc_treeitem_clear_all;
    item->get_bounds=wc_treeitem_get_bounds;
    item->get_checked=wc_treeitem_get_checked;
    item->get_expanded=wc_treeitem_get_expanded;
    item->get_first_child=wc_treeitem_get_first_child;
    item->get_grayed=wc_treeitem_get_grayed;
    item->get_item=wc_treeitem_get_item;
    item->get_item_count=wc_treeitem_get_item_count;
    item->get_items=wc_treeitem_get_items;
    item->get_image=wc_treeitem_get_image;
    item->get_last_child=wc_treeitem_get_last_child;
    item->get_next_sibling=wc_treeitem_get_next_sibling;
    item->get_parent_item=wc_treeitem_get_parent_item;
    item->get_prev_sibling=wc_treeitem_get_prev_sibling;
    item->insert_item=wc_treeitem_insert_item;
    item->insert_item_after=wc_treeitem_insert_item_after;
    item->remove_all=wc_treeitem_remove_all;
    item->set_checked=wc_treeitem_set_checked;
    item->set_expanded=wc_treeitem_set_expanded;
    item->set_grayed=wc_treeitem_set_grayed;
    item->set_has_children=wc_treeitem_set_has_children;
    item->set_image=wc_treeitem_set_image;
    item->set_item_count=wc_treeitem_set_item_count;
}
