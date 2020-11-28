/*
 * list.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "list.h"
#include "../widgets/toolkit.h"
#define TEXT_COLUMN  0
int _w_list_check_style(int style) {
	return _w_widget_check_bits(style, W_SINGLE, W_MULTI, 0, 0, 0, 0);
}
wresult _w_list_insert(w_list *list, w_listitem *item, const char *string,
		int index) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkTreeIter iter;
	/*if (!(0 <= index && index <= items.length)) {
	 error (SWT.ERROR_INVALID_RANGE);
	 }*/
	GtkTreeModel *model = gtk_tree_view_get_model(
			GTK_TREE_VIEW(_W_WIDGET(list)->handle));
	gtk_list_store_insert(GTK_LIST_STORE(model), &iter, index);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, string, 1, 0, -1);
	return W_TRUE;
}
wresult _w_list_deselect_indices(w_list *list, int *indices, size_t length) {
}
wresult _w_list_deselect_index(w_list *list, int index) {
}
wresult _w_list_deselect_range(w_list *list, int start, int end) {
}
wresult _w_list_deselect_all(w_list *list) {
}
int _w_list_get_focus_index(w_list *list) {
}
wresult _w_list_get_item(w_list *list, int index, w_listitem *item) {
}
int _w_list_get_item_count(w_list *list) {
}
int _w_list_get_item_height(w_list *list) {
}
void _w_list_get_items(w_list *list, w_iterator *items) {
}
void _w_list_get_selection(w_list *list, w_iterator *selections) {
}
int _w_list_get_selection_count(w_list *list) {
}
int _w_list_get_selection_index(w_list *list) {
}
int _w_list_get_top_index(w_list *list) {
}
int _w_list_index_of(w_list *list, const char *string, int start) {
}
wbool _w_list_is_selected(w_list *list, int index) {
}
wresult _w_list_remove_indices(w_list *list, int *indices, size_t length) {
}
wresult _w_list_remove_index(w_list *list, int index) {
}
wresult _w_list_remove_range(w_list *list, int start, int end) {
}
wresult _w_list_remove_string(w_list *list, const char *string) {
}
wresult _w_list_remove_all(w_list *list) {
}
wresult _w_list_select_indices(w_list *list, int *indices, size_t length) {
}
wresult _w_list_select_index(w_list *list, int index) {
}
wresult _w_list_select_range(w_list *list, int start, int end) {
}
wresult _w_list_select_all(w_list *list) {
}
wresult _w_list_set_item(w_list *list, int index, const char *string) {
}
wresult _w_list_set_items(w_list *list, const char **items, size_t length) {
}
wresult _w_list_set_selection_indices(w_list *list, int *indices,
		size_t length) {
}
wresult _w_list_set_selection_strings(w_list *list, const char **items,
		size_t length) {
}
wresult _w_list_set_selection_index(w_list *list, int index) {
}
wresult _w_list_set_selection_range(w_list *list, int start, int end) {
}
wresult _w_list_set_top_index(w_list *list, int index) {
}
wresult _w_list_show_selection(w_list *list) {
}
wresult _w_list_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle = 0, *scrolledHandle = 0, *handle = 0;
	GtkListStore *modelHandle = 0;
	GtkCellRenderer *textRenderer = 0;
	GtkTreeViewColumn *columnHandle = 0;
	GtkTreeSelection *selectionHandle;
	style = _w_list_check_style(style);
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	scrolledHandle = gtk_scrolled_window_new(0, 0);
	if (scrolledHandle == 0)
		goto _err;
	/*
	 * Columns:
	 * 0 - text
	 */
	modelHandle = gtk_list_store_new(2, G_TYPE_STRING,G_TYPE_POINTER, 0);
	if (modelHandle == 0)
		goto _err;
	handle = gtk_tree_view_new_with_model(GTK_TREE_MODEL(modelHandle));
	if (handle == 0)
		goto _err;
	textRenderer = gtk_cell_renderer_text_new();
	if (textRenderer == 0)
		goto _err;
	columnHandle = gtk_tree_view_column_new();
	if (columnHandle == 0)
		goto _err;
	gtk_tree_view_column_pack_start(columnHandle, textRenderer, TRUE);
	gtk_tree_view_column_add_attribute(columnHandle, textRenderer, "text",
	TEXT_COLUMN);
	gtk_tree_view_column_set_min_width(columnHandle, 0);
	gtk_tree_view_insert_column(GTK_TREE_VIEW(handle), columnHandle, 0);
	gtk_container_add(GTK_CONTAINER(fixedHandle), scrolledHandle);
	gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);

	GtkSelectionMode mode =
			(style & W_MULTI) != 0 ?
					GTK_SELECTION_MULTIPLE : GTK_SELECTION_BROWSE;
	selectionHandle = gtk_tree_view_get_selection(GTK_TREE_VIEW(handle));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(selectionHandle), mode);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(handle), FALSE);
	GtkPolicyType hsp =
			(style & W_HSCROLL) != 0 ? GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
	GtkPolicyType vsp =
			(style & W_VSCROLL) != 0 ? GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle), hsp,
			vsp);
	if ((style & W_BORDER) != 0)
		gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledHandle),
				GTK_SHADOW_ETCHED_IN);
	/*
	 * Bug in GTK. When a treeview is the child of an override shell,
	 * and if the user has ever invokes the interactive search field,
	 * and the treeview is disposed on a focus out event, it segment
	 * faults. The fix is to disable the search field in an override
	 * shell.
	 */
	w_shell *shell;
	w_control_get_shell(W_CONTROL(parent),&shell);
	if ((_W_WIDGET(shell)->style & W_ON_TOP) != 0) {
		gtk_tree_view_set_search_column(GTK_TREE_VIEW(handle), -1);
	}
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(scrolledHandle), gtk_toolkit->quark[0], widget);
	//g_object_set_qdata(G_OBJECT(scrolledHandle), gtk_toolkit->quark[0], widget);
	_W_WIDGET(widget)->handle = handle;
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
void _w_list_class_init(struct _w_list_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LIST;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_list_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_list);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_list);
	/*
	 * public function
	 */
	clazz->insert = _w_list_insert;
	clazz->deselect_indices = _w_list_deselect_indices;
	clazz->deselect_index = _w_list_deselect_index;
	clazz->deselect_range = _w_list_deselect_range;
	clazz->deselect_all = _w_list_deselect_all;
	clazz->get_focus_index = _w_list_get_focus_index;
	clazz->get_item = _w_list_get_item;
	clazz->get_item_count = _w_list_get_item_count;
	clazz->get_item_height = _w_list_get_item_height;
	clazz->get_items = _w_list_get_items;
	clazz->get_selection = _w_list_get_selection;
	clazz->get_selection_count = _w_list_get_selection_count;
	clazz->get_selection_index = _w_list_get_selection_index;
	clazz->get_top_index = _w_list_get_top_index;
	clazz->index_of = _w_list_index_of;
	clazz->is_selected = _w_list_is_selected;
	clazz->remove_indices = _w_list_remove_indices;
	clazz->remove_index = _w_list_remove_index;
	clazz->remove_range = _w_list_remove_range;
	clazz->remove_string = _w_list_remove_string;
	clazz->remove_all = _w_list_remove_all;
	clazz->select_indices = _w_list_select_indices;
	clazz->select_index = _w_list_select_index;
	clazz->select_range = _w_list_select_range;
	clazz->select_all = _w_list_select_all;
	clazz->set_item = _w_list_set_item;
	clazz->set_items = _w_list_set_items;
	clazz->set_selection_indices = _w_list_set_selection_indices;
	clazz->set_selection_strings = _w_list_set_selection_strings;
	clazz->set_selection_index = _w_list_set_selection_index;
	clazz->set_selection_range = _w_list_set_selection_range;
	clazz->set_top_index = _w_list_set_top_index;
	clazz->show_selection = _w_list_show_selection;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_list_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hpp;
	reserved->fixedHandle = _w_widget_hpp;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle = _w_widget_hp;
}
#endif
