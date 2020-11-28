/*
 * tree.h
 *
 *  Created on: 21 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_CONTROLS_TREE_H_
#define SRC_GTK_CONTROLS_TREE_H_
#ifdef __linux
#include "table.h"
#define COLUMN_INFO_CHECK (1 << 1)
#define COLUMN_INFO_GRAYED (1 << 2)
#define COLUMN_INFO_HAS_CHILDREN (1 << 3)
typedef struct _w_tree {
	struct _w_composite composite;
	int drawState;
	int drawFlags;
	unsigned ignoreSize : 1;
	unsigned rowActivated : 1;
} _w_tree;
typedef struct _w_treecolumn {
	struct w_widgetdata widgetdata;
	w_tree *tree;
	int index;
} _w_treecolumn;
typedef struct _w_treeitem {
	struct w_widgetdata widgetdata;
	w_tree *tree;
	GtkTreeIter iter;
} _w_treeitem;
enum {
	COLUMN_INFO = 0, COLUMN_IMAGE, COLUMN_TEXT, COLUMN_USER_DATA, COLUMN_COUNT
};
struct _w_cell_renderer {
	union {
		GtkCellRenderer renderer;
		GtkCellRendererText text_renderer;
		GtkCellRendererToggle toggle_renderer;
		GtkCellRendererPixbuf pixbuf_renderer;
	};
	w_widget *widget;
	_w_treecolumn column;
	union {
		_w_treeitem item;
	};
};
struct _w_cell_renderer_class {
	union {
		GtkCellRendererClass cellClass;
		GtkCellRendererTextClass textClass;
		GtkCellRendererToggleClass toggleClass;
		GtkCellRendererPixbufClass pixbufClass;
	};
	GParamSpec *param;
#if GTK3
	void (*default_get_preferred_width)(GtkCellRenderer *cell,
			GtkWidget *widget, gint *minimum_size, gint *natural_size);
#else
	void (*default_get_size)(GtkCellRenderer *cell, GtkWidget *widget,
			const GdkRectangle *cell_area, gint *x_offset, gint *y_offset,
			gint *width, gint *height);
#endif
	void (*default_render)(GtkCellRenderer *cell, cairo_t *cr,
			GtkWidget *widget, const GdkRectangle *background_area,
			const GdkRectangle *cell_area, GtkCellRendererState flags);
};
GtkCellRenderer* _w_text_renderer_new(w_widget* widget);
GtkCellRenderer* _w_pixbuf_renderer_new(w_widget* widget);
GtkCellRenderer* _w_toggle_renderer_new(w_widget* widget);
typedef struct _w_column_reserved {
	struct _w_composite_reserved composite;
	void (*renderer_render)(w_widget *widget, GtkCellRenderer *cell,
			cairo_t *cr, GtkWidget *gtkwidget,
			const GdkRectangle *background_area, const GdkRectangle *cell_area,
			GtkCellRendererState flags);
#if GTK3
	void (*renderer_get_preferred_width)(w_widget *widget,
			GtkCellRenderer *cell, GtkWidget *gtkwidget, gint *minimum_size,
			gint *natural_size);
#else
	void (*rendererGetSizeProc)(w_widget* widget, GtkCellRenderer *cell,
			GtkWidget *widget, const GdkRectangle *cell_area, gint *x_offset,
			gint *y_offset, gint *width, gint *height);
#endif
} _w_column_reserved;
typedef struct _w_tree_reserved {
	struct _w_column_reserved column;
	int signal_changed_id;
	int signal_row_activated_id;
	int signal_test_expand_row_id;
	int signal_test_collapse_row_id;
	int signal_expand_collapse_cursor_row_id;
	int signal_row_has_child_toggled_id;
	int signal_start_interactive_search_id;
	int signal_row_inserted_id;
	int signal_row_deleted_id;
} _w_tree_reserved;
#define _W_TREECOLUMN(x) ((_w_treecolumn*)x)
#define _W_TREEITEM(x) ((_w_treeitem*)x)
#define _W_TREE(x) ((_w_tree*)x)
#define _W_TREE_RESERVED(x) ((struct _w_tree_reserved*)x)
/*
 *
 */
void _w_tree_class_init(struct _w_tree_class *clazz);
#endif
#endif /* SRC_GTK_CONTROLS_TREE_H_ */
