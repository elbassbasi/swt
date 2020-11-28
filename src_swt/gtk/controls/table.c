/*
 * table.c
 *
 *  Created on: 22 août 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "tree.h"
#include "../widgets/toolkit.h"
void _w_cell_renderer_render(GtkCellRenderer *cell, cairo_t *cr,
		GtkWidget *widget, const GdkRectangle *background_area,
		const GdkRectangle *cell_area, GtkCellRendererState flags) {
	w_widget *tree = ((struct _w_cell_renderer*) cell)->widget;
	if (tree != 0) {
		_w_column_reserved *reserved =
				(_w_column_reserved*) _w_widget_get_reserved(tree);
		reserved->renderer_render(tree, cell, cr, widget, background_area,
				cell_area, flags);
	}
}
#if GTK3
void _w_cell_renderer_get_preferred_width(GtkCellRenderer *cell,
		GtkWidget *widget, gint *minimum_size, gint *natural_size) {
	w_widget *tree = ((struct _w_cell_renderer*) cell)->widget;
	if (tree != 0) {
		_w_column_reserved *reserved =
				(_w_column_reserved*) _w_widget_get_reserved(tree);
		reserved->renderer_get_preferred_width(tree, cell, widget, minimum_size,
				natural_size);
	}
}
#else
void _w_cell_renderer_GetSize(GtkCellRenderer *cell, GtkWidget *widget,
		const GdkRectangle *cell_area, gint *x_offset, gint *y_offset,
		gint *width, gint *height) {
	w_widget *tree = ((struct _w_cell_renderer*) cell)->widget;
	if (tree != 0) {
		_w_column_reserved *reserved = (_w_column_reserved*) _w_widget_reserved(
				tree);
		reserved->rendererGetSizeProc(tree, cell, widget, cell_area, x_offset,
				y_offset, width, height);
	}
}
#endif
void _w_cell_renderer_ClassInit(struct _w_cell_renderer_class *klass) {
	klass->default_render = klass->cellClass.render;
	klass->cellClass.render = _w_cell_renderer_render;
#if GTK3
	klass->default_get_preferred_width = klass->cellClass.get_preferred_width;
	klass->cellClass.get_preferred_width = _w_cell_renderer_get_preferred_width;
#else
	klass->default_get_size = klass->cellClass.get_size;
	klass->cellClass.get_size =  _w_cell_renderer_GetSize;
#endif
}
void _w_cell_renderer_init(struct _w_cell_renderer *cell) {
	cell->column.widgetdata.clazz = 0;
	cell->item.widgetdata.clazz = 0;
}
GType _w_registre_renderer_class(const char *name, GType parent) {
	GTypeInfo renderer_info = { sizeof(struct _w_cell_renderer_class),
	NULL, /* base_init */
	NULL, /* base_finalize */
	(GClassInitFunc) _w_cell_renderer_ClassInit,
	NULL, /* class_finalize */
	NULL, /* class_data */
	sizeof(struct _w_cell_renderer), 0, /* n_preallocs */
	(GInstanceInitFunc) _w_cell_renderer_init, NULL };
	return g_type_register_static(parent, name, &renderer_info, (GTypeFlags) 0);
}
GType _w_text_renderer_type = 0;
GtkCellRenderer* _w_text_renderer_new(w_widget *widget) {
	if (_w_text_renderer_type == 0) {
		_w_text_renderer_type = _w_registre_renderer_class("_WTextRenderer",
		GTK_TYPE_CELL_RENDERER_TEXT);
	}
	struct _w_cell_renderer *renderer = (struct _w_cell_renderer*) g_object_new(
			_w_text_renderer_type, NULL);
	if (renderer != 0) {
		renderer->widget = widget;
	}
	return (GtkCellRenderer*) renderer;
}
GType _w_pixbuf_renderer_type = 0;
GtkCellRenderer* _w_pixbuf_renderer_new(w_widget *widget) {
	if (_w_text_renderer_type == 0) {
		_w_pixbuf_renderer_type = _w_registre_renderer_class("_WPixbufRenderer",
		GTK_TYPE_CELL_RENDERER_PIXBUF);
	}
	struct _w_cell_renderer *renderer = (struct _w_cell_renderer*) g_object_new(
			_w_pixbuf_renderer_type, NULL);
	if (renderer != 0) {
		renderer->widget = widget;
	}
	return (GtkCellRenderer*) renderer;
}
GType _w_toggle_renderer_type = 0;
GtkCellRenderer* _w_toggle_renderer_new(w_widget *widget) {
	if (_w_toggle_renderer_type == 0) {
		_w_toggle_renderer_type = _w_registre_renderer_class("_WToggleRenderer",
		GTK_TYPE_CELL_RENDERER_TOGGLE);
	}
	struct _w_cell_renderer *renderer = (struct _w_cell_renderer*) g_object_new(
			_w_toggle_renderer_type, NULL);
	if (renderer != 0) {
		renderer->widget = widget;
	}
	return (GtkCellRenderer*) renderer;
}
int _w_table_check_style(int style) {
	/*
	 * Feature in Windows.  Even when WS_HSCROLL or
	 * WS_VSCROLL is not specified, Windows creates
	 * trees and tables with scroll bars.  The fix
	 * is to set H_SCROLL and V_SCROLL.
	 *
	 * NOTE: This code appears on all platforms so that
	 * applications have consistent scroll bar behavior.
	 */
	if ((style & W_NO_SCROLL) == 0) {
		style |= W_HSCROLL | W_VSCROLL;
	}
	/* GTK is always FULL_SELECTION */
	style |= W_FULL_SELECTION;
	return _w_widget_check_bits(style, W_SINGLE, W_MULTI, 0, 0, 0, 0);
}
/**
 * tablecolumn
 */

int _w_tablecolumn_get_alignment(w_tablecolumn *column) {
}
int _w_tablecolumn_get_image(w_tablecolumn *column) {
}
wbool _w_tablecolumn_get_moveable(w_tablecolumn *column) {
}
wbool _w_tablecolumn_get_resizable(w_tablecolumn *column) {
}
wresult _w_tablecolumn_get_tooltip_text(w_tablecolumn *column,
		w_alloc *text) {
}
int _w_tablecolumn_get_width(w_tablecolumn *column) {
}
wresult _w_tablecolumn_pack(w_tablecolumn *column) {
}
wresult _w_tablecolumn_set_alignment(w_tablecolumn *column, int alignment) {
}
wresult _w_tablecolumn_set_image(w_tablecolumn *column, int image) {
}
wresult _w_tablecolumn_set_moveable(w_tablecolumn *column, wbool moveable) {
}
wresult _w_tablecolumn_set_resizable(w_tablecolumn *column, wbool resizable) {
}
wresult _w_tablecolumn_set_tooltip_text(w_tablecolumn *column,
		const char *string) {
}
wresult _w_tablecolumn_set_width(w_tablecolumn *column, int width) {
}
/**
 * tableitem
 */
wresult _w_tableitem_get_bounds(w_tableitem *item, w_rect *bounds) {
}
wresult _w_tableitem_get_bounds_index(w_tableitem *item, int index,
		w_rect *bounds) {
}
wbool _w_tableitem_get_checked(w_tableitem *item) {
}
int _w_tableitem_get_image(w_tableitem *item) {
}
wresult _w_tableitem_set_checked(w_tableitem *item, wbool checked) {
}
wresult _w_tableitem_set_image(w_tableitem *item, int image) {
}
/**
 * table
 *
 */
wresult _w_table_clear_index(w_table *table, int index) {
}
wresult _w_table_clear_range(w_table *table, int start, int end) {
}
wresult _w_table_clear_indices(w_table *table, int *indices, size_t length) {
}
wresult _w_table_clear_all(w_table *table) {
}
wresult _w_table_deselect_indices(w_table *table, int *indices,
		size_t length) {
}
wresult _w_table_deselect_index(w_table *table, int index) {
}
wresult _w_table_deselect_range(w_table *table, int start, int end) {
}
wresult _w_table_deselect_all(w_table *table) {
}
wresult _w_table_get_column(w_table *table, int index, w_tablecolumn *column) {
}
int _w_table_get_column_count(w_table *table) {
}
size_t _w_table_get_column_order(w_table *table, int *order, size_t length) {
}
void _w_table_get_columns(w_table *table, w_iterator *columns) {
}
w_imagelist* _w_table_get_imagelist(w_table *table) {
	return _W_TABLEBASE(table)->imagelist;
}
int _w_table_get_gridline_width(w_table *table) {
}
int _w_table_get_header_height(w_table *table) {
}
wbool _w_table_get_header_visible(w_table *table) {
}
wbool _w_table_get_item(w_table *table, int index, w_tabitem *item) {
}
wbool _w_table_get_item_from_point(w_table *table, w_point *point,
		w_tabitem *item) {
}
int _w_table_get_item_count(w_table *table) {
}
int _w_table_get_item_height(w_table *table) {
}
void _w_table_get_items(w_table *table, w_iterator *items) {
}
wbool _w_table_get_lines_visible(w_table *table) {
}
void _w_table_get_selection(w_table *table, w_iterator *items) {
}
int _w_table_get_selection_count(w_table *table) {
}
int _w_table_get_selection_index(w_table *table) {
}
size_t _w_table_get_selection_indices(w_table *table, int *indeics,
		size_t length) {
}
wbool _w_table_get_sort_column(w_table *table, w_tablecolumn *column) {
}
int _w_table_get_sort_direction(w_table *table) {
}
int _w_table_get_top_index(w_table *table) {
}
wbool _w_table_is_selected(w_table *table, int index) {
}
wresult _w_table_insert_column(w_table *table, w_tablecolumn *column,
		const char *string, int index) {
}
wresult _w_table_insert_item(w_table *table, w_tableitem *item,
		const char *string, int index) {
	GtkWidget *_handle = _W_WIDGET(table)->handle;
	GtkTreeModel *modelHandle = gtk_tree_view_get_model(GTK_TREE_VIEW(_handle));
	GtkTreeIter handle;
	/*
	 * Feature in GTK.  It is much faster to append to a tree store
	 * than to insert at the end using gtk_tree_store_insert().
	 */
	if (index < 0) {
		gtk_list_store_append(GTK_LIST_STORE(modelHandle), &handle);
	} else {
		gtk_list_store_insert(GTK_LIST_STORE(modelHandle), &handle,
				index);
	}
	/*int id = getId(item.handle, false);
	 items[id] = item;
	 modelChanged = true;
	 */
	gtk_list_store_set(GTK_LIST_STORE(modelHandle), &handle, COLUMN_INFO, 0,
			COLUMN_IMAGE, -1, COLUMN_TEXT, string, COLUMN_USER_DATA, 0, -1);
	if (item != 0) {
		_W_TREEITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_TABLE_GET_CLASS(table)->class_tableitem);
		memset(&_W_TREEITEM(item)->iter, 0, sizeof(GtkTreeIter));
		_W_TREEITEM(item)->tree = table;
		return W_TRUE;
	}
	return W_TRUE;
}
wresult _w_table_remove_indices(w_table *table, int *indices, size_t length) {
}
wresult _w_table_remove_index(w_table *table, int index) {
}
wresult _w_table_remove_range(w_table *table, int start, int end) {
}
wresult _w_table_remove_all(w_table *table) {
}
wresult _w_table_set_imagelist(w_table *table, w_imagelist *imagelist) {
	if (imagelist == 0) {
		_W_TABLEBASE(table)->imagelist = 0;
		gtk_cell_renderer_set_fixed_size(_W_TABLEBASE(table)->pixbuf_renderer, 0,
				0);
	} else {
		if (_W_IMAGELIST(imagelist)->images == 0) {
			_W_TABLEBASE(table)->imagelist = 0;
			gtk_cell_renderer_set_fixed_size(_W_TABLEBASE(table)->pixbuf_renderer,
					0, 0);
			return W_ERROR_INVALID_ARGUMENT;
		} else {
			_W_TABLEBASE(table)->imagelist = imagelist;
			gtk_cell_renderer_set_fixed_size(_W_TABLEBASE(table)->pixbuf_renderer,
			_W_IMAGELIST(imagelist)->images->width,
			_W_IMAGELIST(imagelist)->images->height);
		}
	}
	//_w_control_update(W_CONTROL(tree));
	return W_TRUE;
}
wresult _w_table_select_indices(w_table *table, int *indices, size_t length) {
}
wresult _w_table_select_index(w_table *table, int index) {
}
wresult _w_table_select_range(w_table *table, int start, int end) {
}
wresult _w_table_select_all(w_table *table) {
}
wresult _w_table_set_column_order(w_table *table, int *order, size_t length) {
}
wresult _w_table_set_header_visible(w_table *table, wbool show) {
}
wresult _w_table_set_lines_visible(w_table *table, wbool show) {
}
wresult _w_table_set_selection_indices(w_table *table, int *indices,
		size_t length) {
}
wresult _w_table_set_selection_index(w_table *table, int index) {
}
wresult _w_table_set_selection_range(w_table *table, int start, int end) {
}
wresult _w_table_set_sort_column(w_table *table, w_tablecolumn *column) {
}
wresult _w_table_set_sort_direction(w_table *table, int direction) {
}
wresult _w_table_set_top_index(w_table *table, int index) {
}
wresult _w_table_show_column(w_table *table, w_tablecolumn *column) {
}
wresult _w_table_show_item(w_table *table, w_tablecolumn *item) {
}
wresult _w_table_show_selection(w_table *table) {
}
/**
 *
 */
wresult _w_table_insert_first_column(w_table *table,
		struct _w_table_reserved *reserved) {
	GtkTreeViewColumn *columnHandle;
	GtkCellRenderer *renderer = 0;
	GtkWidget *boxHandle = 0, *labelHandle = 0, *imageHandle;

	columnHandle = gtk_tree_view_column_new();
	if (columnHandle == 0) {
		goto _err;
	}
	boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 3);
	if (boxHandle == 0) {
		goto _err;
	}
	labelHandle = gtk_label_new_with_mnemonic("");
	if (labelHandle == 0) {
		goto _err;
	}
	imageHandle = gtk_image_new();
	if (imageHandle == 0) {
		goto _err;
	}
	gtk_container_add(GTK_CONTAINER(boxHandle), imageHandle);
	gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
	gtk_widget_show_all(boxHandle);
	//OS.gtk_widget_show (labelHandle);
	gtk_tree_view_column_set_widget(columnHandle, boxHandle);
	//g_object_ref(renderer);

	gtk_tree_view_column_set_resizable(columnHandle, TRUE);
	gtk_tree_view_column_set_clickable(columnHandle, TRUE);
	//gtk_tree_view_column_set_visible(columnHandle, FALSE);
	gtk_tree_view_column_set_sort_column_id(columnHandle, 0);
	if ((_W_WIDGET(table)->style & W_CHECK) != 0) {
		renderer = _w_toggle_renderer_new(W_WIDGET(table));
		if (renderer == 0)
			goto _err;
		g_object_ref(renderer);
		gtk_tree_view_column_pack_start(columnHandle, renderer, FALSE);
		gtk_tree_view_column_set_cell_data_func(columnHandle, renderer,
				_w_treecolumn_cell_data, table, 0);
		g_object_set_qdata(G_OBJECT(renderer), gtk_toolkit->quark[0], table);
		if (renderer != 0) {
			if (_W_TABLEBASE_RESERVED(reserved)->signal_toggled_id == 0) {
				_W_TABLEBASE_RESERVED(reserved)->signal_toggled_id =
						g_signal_lookup("toggled",
								gtk_cell_renderer_toggle_get_type());
			}
			_w_widget_connect((GtkWidget*) renderer, SIGNAL_TOGGLED,
			_W_TABLEBASE_RESERVED(reserved)->signal_toggled_id,
			FALSE);
		}
	}
	renderer = _w_pixbuf_renderer_new(W_WIDGET(table));
	if (renderer == 0)
		goto _err;
	_W_TABLEBASE(table)->pixbuf_renderer = renderer;
	gtk_tree_view_column_pack_start(columnHandle, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(columnHandle, renderer,
			_w_treecolumn_cell_data, table, 0);
	g_object_set_qdata(G_OBJECT(renderer), gtk_toolkit->quark[0], table);
	gtk_cell_renderer_set_fixed_size(renderer, 0, 0);
	renderer = _w_text_renderer_new(W_WIDGET(table));
	if (renderer == 0) {
		goto _err;
	}
	gtk_tree_view_column_pack_start(columnHandle, renderer, FALSE);
	gtk_tree_view_column_set_cell_data_func(columnHandle, renderer,
			_w_treecolumn_cell_data, table, 0);
	//gtk_tree_view_column_pack_end(columnHandle, renderer, FALSE);
	g_object_set_qdata(G_OBJECT(renderer), gtk_toolkit->quark[0], table);
	/* Set alignment */
	/*if ((style & swt::RIGHT) != 0) {
	 g_object_set(textRenderer, OS.xalign, 1f, 0);
	 gtk_tree_view_column_pack_end (columnHandle, textRenderer, true);
	 gtk_tree_view_column_pack_end (columnHandle, pixbufRenderer, false);
	 gtk_tree_view_column_set_alignment(columnHandle, 1.0f);
	 } else if ((style & swt::CENTER) != 0) {
	 g_object_set(textRenderer, OS.xalign, 0.5f, 0);
	 gtk_tree_view_column_pack_start (columnHandle, pixbufRenderer, false);
	 gtk_tree_view_column_pack_end (columnHandle, textRenderer, true);
	 gtk_tree_view_column_set_alignment(columnHandle, 0.5f);
	 } else if ((style & swt::LEFT) != 0) {
	 gtk_tree_view_column_pack_start (columnHandle, pixbufRenderer, false);
	 gtk_tree_view_column_pack_start (columnHandle, textRenderer, true);
	 gtk_tree_view_column_set_alignment(columnHandle, 0.0f);
	 }*/
	GtkWidget *_handle = _W_WIDGET(table)->handle;
	gtk_tree_view_insert_column(GTK_TREE_VIEW(_handle), columnHandle, 0);
	g_object_set_qdata(G_OBJECT(columnHandle), gtk_toolkit->quark[1],
			(gpointer) ((long) 0));
	return W_TRUE;
	_err: if (columnHandle)
		g_object_unref(columnHandle);
	return W_ERROR_NO_HANDLES;
}
wresult _w_table_create_0(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event, wbool is_tree) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_widget_is_ok(widget))
		return W_ERROR_INVALID_ARGUMENT;
	style = _w_table_check_style(style);
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	GtkWidget *scrolledHandle = 0, *handle = 0, *fixed;
	GtkListStore *modelHandle = 0;
	GtkSelectionMode mode;
	GtkTreeSelection *selectionHandle;
	GtkPolicyType hsp;
	GtkPolicyType vsp;
	fixed = (GtkWidget*) _w_fixed_new();
	if (fixed == 0)
		goto _err;
	gtk_widget_set_has_window(fixed, TRUE);
	scrolledHandle = gtk_scrolled_window_new(0, 0);
	if (scrolledHandle == 0)
		goto _err;
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle),
			GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledHandle),
			GTK_SHADOW_IN);
	if (is_tree) {
		modelHandle = gtk_tree_store_new(COLUMN_COUNT,
		G_TYPE_INT, G_TYPE_INT,
		G_TYPE_STRING,
		G_TYPE_POINTER);
	} else {
		modelHandle = gtk_list_store_new(COLUMN_COUNT,
		G_TYPE_INT, G_TYPE_INT,
		G_TYPE_STRING,
		G_TYPE_POINTER);
	}
	if (modelHandle == 0)
		goto _err;
	handle = gtk_tree_view_new_with_model(GTK_TREE_MODEL(modelHandle));
	if (handle == 0)
		goto _err;

	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(handle), FALSE);

	gtk_container_add(GTK_CONTAINER(fixed), scrolledHandle);
	gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
	g_object_set_qdata(G_OBJECT(modelHandle), gtk_toolkit->quark[0], widget);
	//tree_registre_signal(toolkit, reserved);
	mode = (style & W_MULTI) != 0 ?
			GTK_SELECTION_MULTIPLE : GTK_SELECTION_BROWSE;
	selectionHandle = gtk_tree_view_get_selection(GTK_TREE_VIEW(handle));
	gtk_tree_selection_set_mode(selectionHandle, mode);
	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(handle), FALSE);
	hsp = (style & W_HSCROLL) != 0 ? GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
	vsp = (style & W_VSCROLL) != 0 ? GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle), hsp,
			vsp);
	if ((style & W_BORDER) != 0)
		gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledHandle),
				GTK_SHADOW_ETCHED_IN);
	/*
	 * We enable fixed-height-mode for performance reasons (see bug 490203).
	 */
	/*	if ((widget->GetHandle<GtkControl_t>().style & swt::VIRTUAL) != 0) {
	 g_object_set(handle, fixed_height_mode, true, 0);
	 }*/
	/*if (!searchEnabled(style)) {
	 gtk_tree_view_set_search_column(GTK_TREE_VIEW(handle), -1);
	 }*/
	g_object_set_qdata(G_OBJECT(fixed), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(scrolledHandle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(selectionHandle), gtk_toolkit->quark[0],
			widget);
	_W_WIDGET(widget)->handle = handle;
	_W_WIDGET(widget)->state = 0;
	_W_WIDGET(widget)->post_event = post_event;
	_W_WIDGET(widget)->style = style;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	struct _w_table_reserved *reserved =
			(struct _w_table_reserved*) _w_widget_get_reserved(widget);
	_w_composite_add_child(W_COMPOSITE(parent), GTK_WIDGET(fixed));
	if(is_tree){
	_w_tree_connect_signal(widget, _W_CONTROL_RESERVED(reserved));
	}else{

	}
	_w_table_insert_first_column(W_TABLE(widget), reserved);
	gtk_widget_show_all(GTK_WIDGET(fixed));
	return W_TRUE;
	_err: if (scrolledHandle)
		gtk_widget_destroy(scrolledHandle);
	if (modelHandle)
		g_object_unref(modelHandle);
	if (handle)
		gtk_widget_destroy(handle);
	return W_ERROR_NO_HANDLES;
}
wresult _w_table_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	return _w_table_create_0(widget, parent, style, post_event, W_FALSE);
}
int _w_table_post_event(w_widget *widget, w_event *e) {
	return _w_widget_post_event(widget, e);

}
void _w_table_class_init(struct _w_table_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TABLE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_table_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_table);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tree);
	/*
	 * public function
	 */
	clazz->clear_index = _w_table_clear_index;
	clazz->clear_range = _w_table_clear_range;
	clazz->clear_indices = _w_table_clear_indices;
	clazz->clear_all = _w_table_clear_all;
	clazz->deselect_indices = _w_table_deselect_indices;
	clazz->deselect_index = _w_table_deselect_index;
	clazz->deselect_range = _w_table_deselect_range;
	clazz->deselect_all = _w_table_deselect_all;
	clazz->get_column = _w_table_get_column;
	clazz->get_column_count = _w_table_get_column_count;
	clazz->get_column_order = _w_table_get_column_order;
	clazz->get_columns = _w_table_get_columns;
	clazz->get_imagelist = _w_table_get_imagelist;
	clazz->get_gridline_width = _w_table_get_gridline_width;
	clazz->get_header_height = _w_table_get_header_height;
	clazz->get_header_visible = _w_table_get_header_visible;
	clazz->get_item = _w_table_get_item;
	clazz->get_item_from_point = _w_table_get_item_from_point;
	clazz->get_item_count = _w_table_get_item_count;
	clazz->get_item_height = _w_table_get_item_height;
	clazz->get_items = _w_table_get_items;
	clazz->get_lines_visible = _w_table_get_lines_visible;
	clazz->get_selection = _w_table_get_selection;
	clazz->get_selection_count = _w_table_get_selection_count;
	clazz->get_selection_index = _w_table_get_selection_index;
	clazz->get_selection_indices = _w_table_get_selection_indices;
	clazz->get_sort_column = _w_table_get_sort_column;
	clazz->get_sort_direction = _w_table_get_sort_direction;
	clazz->get_top_index = _w_table_get_top_index;
	clazz->is_selected = _w_table_is_selected;
	clazz->insert_column = _w_table_insert_column;
	clazz->insert_item = _w_table_insert_item;
	clazz->remove_indices = _w_table_remove_indices;
	clazz->remove_index = _w_table_remove_index;
	clazz->remove_range = _w_table_remove_range;
	clazz->remove_all = _w_table_remove_all;
	clazz->set_imagelist = _w_table_set_imagelist;
	clazz->select_indices = _w_table_select_indices;
	clazz->select_index = _w_table_select_index;
	clazz->select_range = _w_table_select_range;
	clazz->select_all = _w_table_select_all;
	clazz->set_column_order = _w_table_set_column_order;
	clazz->set_header_visible = _w_table_set_header_visible;
	clazz->set_lines_visible = _w_table_set_lines_visible;
	clazz->set_selection_indices = _w_table_set_selection_indices;
	clazz->set_selection_index = _w_table_set_selection_index;
	clazz->set_selection_range = _w_table_set_selection_range;
	clazz->set_sort_column = _w_table_set_sort_column;
	clazz->set_sort_direction = _w_table_set_sort_direction;
	clazz->set_top_index = _w_table_set_top_index;
	clazz->show_column = _w_table_show_column;
	clazz->show_item = _w_table_show_item;
	clazz->show_selection = _w_table_show_selection;
	/*
	 * table column
	 */
	struct _w_tablecolumn_class *tablecolumn = clazz->class_tablecolumn;
	tablecolumn->item.widgetdata.toolkit = gtk_toolkit;
	tablecolumn->get_alignment = _w_tablecolumn_get_alignment;
	tablecolumn->get_image = _w_tablecolumn_get_image;
	tablecolumn->get_moveable = _w_tablecolumn_get_moveable;
	tablecolumn->get_resizable = _w_tablecolumn_get_resizable;
	tablecolumn->get_tooltip_text = _w_tablecolumn_get_tooltip_text;
	tablecolumn->get_width = _w_tablecolumn_get_width;
	tablecolumn->pack = _w_tablecolumn_pack;
	tablecolumn->set_alignment = _w_tablecolumn_set_alignment;
	tablecolumn->set_image = _w_tablecolumn_set_image;
	tablecolumn->set_moveable = _w_tablecolumn_set_moveable;
	tablecolumn->set_resizable = _w_tablecolumn_set_resizable;
	tablecolumn->set_tooltip_text = _w_tablecolumn_set_tooltip_text;
	tablecolumn->set_width = _w_tablecolumn_set_width;
	tablecolumn->item.widgetdata.close = _w_widgetdata_close;
	tablecolumn->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	//treecolumn->item.widgetdata.copy = _w_treeitem_copy;
	//treecolumn->item.widgetdata.equals = _w_treeitem_equals;
	/*tablecolumn->item.get_parent_widget = _w_treecolumn_get_parent_widget;
	tablecolumn->item.get_data = _w_treecolumn_get_data;
	tablecolumn->item.get_index = _w_treecolumn_get_index;
	tablecolumn->item.get_text = _w_treecolumn_get_text;
	tablecolumn->item.set_data = _w_treecolumn_set_data;
	tablecolumn->item.set_text = _w_treecolumn_set_text;*/
	/*
	 * table item
	 */
	struct _w_tableitem_class *tableitem = clazz->class_tableitem;
	tableitem->item.widgetdata.toolkit = gtk_toolkit;
	tableitem->get_bounds = _w_tableitem_get_bounds;
	tableitem->get_bounds_index = _w_tableitem_get_bounds_index;
	tableitem->get_checked = _w_tableitem_get_checked;
	tableitem->get_image = _w_tableitem_get_image;
	tableitem->set_checked = _w_tableitem_set_checked;
	tableitem->set_image = _w_tableitem_set_image;
	/*tableitem->item.get_parent_widget = _w_treeitem_get_parent_widget;
	tableitem->item.get_data = _w_treeitem_get_data;
	tableitem->item.get_index = _w_treeitem_get_index;
	tableitem->item.get_text = _w_treeitem_get_text;
	tableitem->item.set_data = _w_treeitem_set_data;
	tableitem->item.set_text = _w_treeitem_set_text;*/
	tableitem->item.widgetdata.close = _w_widgetdata_close;
	tableitem->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	//tableitem->item.widgetdata.copy = _w_treeitem_copy;
	//tableitem->item.widgetdata.equals = _w_treeitem_equals;

	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->post_event = _w_table_post_event;
	W_WIDGET_CLASS(clazz)->create = _w_table_create;
}
#endif
