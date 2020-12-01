/*
 * table.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_CONTROLS_TABLE_H_
#define SRC_GTK_CONTROLS_TABLE_H_
#ifdef __linux
#include "../widgets/composite.h"
typedef struct _w_table_base {
	struct _w_composite composite;
	GtkCellRenderer* pixbuf_renderer;
	w_imagelist *imagelist;
} _w_table_base;

struct _w_table_base_reserved {
	struct _w_composite_reserved composite;
	int signal_toggled_id;

};
typedef struct _w_table {
	struct _w_table_base table_base;
} _w_table;

struct _w_table_reserved {
	struct _w_table_base_reserved table_base;

};
#define _W_TABLE(x) ((_w_table*)x)
#define _W_TABLE_RESERVED(x) ((struct _w_table_reserved*)x)
#define _W_TABLEBASE(x) ((_w_table_base*)x)
#define _W_TABLEBASE_RESERVED(x) ((struct _w_table_base_reserved*)x)
int _w_table_check_style(int style);
/*
 * private
 */
wresult _w_table_insert_first_column(w_table *table,
		struct _w_table_reserved *reserved);
wresult _w_table_set_imagelist(w_table *table, w_imagelist *imagelist);
void _w_treecolumn_cell_data(GtkTreeViewColumn *tree_column,
		GtkCellRenderer *cell, GtkTreeModel *tree_model, GtkTreeIter *iter,
		gpointer data);
wresult _w_table_create_0(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event, int is_tree);
void _w_tree_connect_signal(w_widget *widget,
		struct _w_control_reserved *reserved);
void _w_table_class_init(struct _w_table_class *clazz);

#endif
#endif /* SRC_GTK_CONTROLS_TABLE_H_ */
