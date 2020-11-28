/*
 * table.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_TABLE_H_
#define SRC_COCOA_CONTROLS_TABLE_H_
#ifdef __APPLE__
#include "../widgets/composite.h"
typedef struct NSTableView{
    
}NSTableView;
typedef struct NSTableHeaderView{
    
}NSTableHeaderView;
typedef struct NSTableColumn{
    
}NSTableColumn;
typedef struct NSCell{
    
}NSCell;
void NSTableView_setHeaderView(NSTableView* tableView,NSTableHeaderView *headerView);
void NSTableView_addTableColumn(NSTableView* tableView,NSTableColumn* column);
NSUInteger NSTableView_selectedRow(NSTableView* tableView);
/*
 
 */
NSTableHeaderView* NSTableHeaderView_new(w_widget* widget);

NSTableColumn* SWTTableColumn_new(NSString* identifier);
void NSTableColumn_setMinWidth(NSTableColumn* tablecolumn,int minWidth);
void NSTableColumn_setWidth(NSTableColumn* tablecolumn,int width);
void NSTableColumn_setResizingMask(NSTableColumn* tablecolumn,long ops);
void NSTableColumn_setDataCell(NSTableColumn* tablecolumn,NSCell* cell);
NSCell* NSTableColumn_headerCell(NSTableColumn* tablecolumn);
NSCell* SWTImageTextCell_new(NSString* identifier);
void NSCell_setTitle(NSCell* cell,NSString* str);
void NSCell_setLineBreakMode(NSCell* cell,int lineBreak);
typedef struct _w_table_base {
	struct _w_composite composite;
} _w_table_base;

struct _w_table_base_reserved {
	struct _w_composite_reserved composite;

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
wuint64 _w_table_check_style(w_widget* widget,wuint64 style);
void _w_table_class_init(struct _w_table_class *clazz);

#endif
#endif /* SRC_GTK_CONTROLS_TABLE_H_ */
