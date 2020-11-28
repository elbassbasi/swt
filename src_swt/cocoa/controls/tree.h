/*
 * tree.h
 *
 *  Created on: 21 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_TREE_H_
#define SRC_COCOA_CONTROLS_TREE_H_
#ifdef __APPLE__
#include "table.h"
typedef struct NSOutlineView{
    
}NSOutlineView;
typedef struct SWTTreeItem{
    
}SWTTreeItem;

NSOutlineView* SWTOutlineView_new(void* userdata);
void NSOutlineView_setAllowsMultipleSelection(NSOutlineView* outlineView,int op);
void NSOutlineView_setAutoresizesOutlineColumn(NSOutlineView* outlineView,int op);
void NSOutlineView_setAutosaveExpandedItems(NSOutlineView* outlineView,int op);
void NSOutlineView_setDataSource(NSOutlineView* outlineView,int op);
void NSOutlineView_setDelegate(NSOutlineView* outlineView,int op);
void NSOutlineView_setColumnAutoresizingStyle(NSOutlineView* outlineView,long op);
void NSOutlineView_setIntercellSpacing(NSOutlineView* outlineView,int width,int heigth);
void NSOutlineView_reloadItem(NSOutlineView* outlineView,SWTTreeItem* obj,int reloadChildren);
void NSOutlineView_reloadData(NSOutlineView* outlineView);
void NSOutlineView_setOutlineTableColumn(NSOutlineView* tableView,NSTableColumn* column);
NSObject* NSOutlineView_itemAtRow(NSOutlineView* tableView,NSUInteger index);

/*
 */

SWTTreeItem *SWTTreeItem_new();
SWTTreeItem *SWTTreeItem_create(NSOutlineView* outline,SWTTreeItem* parent,int index,const char* text);
void SWTTreeItem_setText(SWTTreeItem *item,const char* text);
void SWTTreeItem_setHasChildren(SWTTreeItem *item);
void SWTTreeItem_setUserData(SWTTreeItem *item,void* userdata);
void* SWTTreeItem_getUserData(SWTTreeItem *item);

#define COLUMN_INFO_CHECK (1 << 1)
#define COLUMN_INFO_HAS_CHILDREN (1 << 2)
typedef struct _w_tree {
	struct _w_composite composite;
    NSTableHeaderView* headerView;
    int NEXT_ID;
} _w_tree;
typedef struct _w_treecolumn {
	struct w_widgetdata widgetdata;
	w_tree *tree;
	int index;
} _w_treecolumn;
typedef struct _w_treeitem {
	struct w_widgetdata widgetdata;
	w_tree *tree;
    SWTTreeItem* handle;
    NSOutlineView* view;
} _w_treeitem;
enum {
	COLUMN_INFO = 0, COLUMN_IMAGE, COLUMN_TEXT, COLUMN_USER_DATA, COLUMN_COUNT
};
typedef struct _w_column_reserved {
	struct _w_composite_reserved composite;
} _w_column_reserved;
typedef struct _w_tree_reserved {
	struct _w_column_reserved column;
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
