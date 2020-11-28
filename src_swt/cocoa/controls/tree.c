/*
 * tree.c
 *
 *  Created on: 21 ao�t 2019
 *      Author: azeddine El Bassbasi
 */
#if __APPLE__
#include "tree.h"
#include "../widgets/toolkit.h"
#define FIRST_COLUMN_MINIMUM_WIDTH 5
#define IMAGE_GAP 3
#define TEXT_GAP 2
#define CELL_GAP 1
/*
 * tree column
 */
int _w_treecolumn_get_alignment(w_treecolumn *column) {
}
int _w_treecolumn_get_id(w_treecolumn *column) {
}
int _w_treecolumn_get_image(w_treecolumn *column) {
}
wbool _w_treecolumn_get_moveable(w_treecolumn *column) {
}
wbool _w_treecolumn_get_resizable(w_treecolumn *column) {
}
w_string_ref* _w_treecolumn_get_tooltip_text(w_treecolumn *column) {
}
int _w_treecolumn_get_width(w_treecolumn *column) {
}
wresult _w_treecolumn_pack(w_treecolumn *column) {
}
wresult _w_treecolumn_set_alignment(w_treecolumn *column, int alignment) {
}
wresult _w_treecolumn_set_id(w_treecolumn *column, int id) {
}
wresult _w_treecolumn_set_image(w_treecolumn *column, int image) {
}
wresult _w_treecolumn_set_moveable(w_treecolumn *column, wbool moveable) {
}
wresult _w_treecolumn_set_resizable(w_treecolumn *column, wbool resizable) {
}
wresult _w_treecolumn_set_tooltip_text(w_treecolumn *column,
		const char *text) {
}
wresult _w_treecolumn_set_width(w_treecolumn *column, int width) {
}
w_widget* _w_treecolumn_get_parent_widget(w_item *column) {
}
void* _w_treecolumn_get_data(w_item *column) {
}
int _w_treecolumn_get_index(w_item *column) {
}
w_string_ref* _w_treecolumn_get_text(w_item *item) {
}
wresult _w_treecolumn_set_data(w_item *column, void *data) {
}
wresult _w_treecolumn_set_text(w_item *item, const char *string) {
}
/*
 * tree item
 */
wresult _w_treeitem_clear(w_treeitem *item, int index, wbool all) {
}
wresult _w_treeitem_clear_all(w_treeitem *item, wbool all) {
}
void _w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds) {
}
wbool _w_treeitem_get_checked(w_treeitem *item) {
}
wbool _w_treeitem_get_expanded(w_treeitem *item) {
}
wbool _w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child) {
}
wbool _w_treeitem_get_grayed(w_treeitem *item) {
}
wbool _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem) {
}
int _w_treeitem_get_item_count(w_treeitem *item) {
}
void _w_treeitem_get_items(w_treeitem *item, w_iterator *items) {
}
int _w_treeitem_get_image(w_treeitem *item) {
}
wbool _w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child) {
}
wbool _w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next) {
}
w_tree* _w_treeitem_get_parent(w_treeitem *item) {
}
wbool _w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent) {
}
wbool _w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev) {
}
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		const char *text, int index) {
    NSOutlineView* view = _W_TREEITEM(item)->view;
    SWTTreeItem* parent = _W_TREEITEM(item)->handle;
    SWTTreeItem* newitem =SWTTreeItem_create(_W_TREEITEM(item)->view,parent, index, text);
    if (subitem != 0) {
        W_WIDGETDATA(subitem)->clazz =W_WIDGETDATA(item)->clazz;
        _W_TREEITEM(subitem)->handle = newitem;
        _W_TREEITEM(subitem)->tree =  _W_TREEITEM(item)->tree;
        _W_TREEITEM(subitem)->view = _W_TREEITEM(item)->view;
    }
    if (parent != 0) {
        NSOutlineView_reloadItem(view, parent, W_TRUE);
    } else {
        NSOutlineView_reloadData(view);
    }
  /*      int count;
    TreeItem [] items;
    if (parentItem != null) {
        count = parentItem.itemCount;
        items = parentItem.items;
    } else {
        count = this.itemCount;
        items = this.items;
    }
    if (index == -1) index = count;
    if (!(0 <= index && index <= count)) error (SWT.ERROR_INVALID_RANGE);
    if (count == items.length) {
        TreeItem [] newItems = new TreeItem [items.length + 4];
        System.arraycopy (items, 0, newItems, 0, items.length);
        items = newItems;
        if (parentItem != null) {
            parentItem.items = items;
        } else {
            this.items = items;
        }
    }
    
    System.arraycopy (items, index, items, index + 1, count++ - index);
    items [index] = item;
    item.items = new TreeItem [4];
    SWTTreeItem handle = (SWTTreeItem) new SWTTreeItem ().alloc ().init ();
    item.handle = handle;
    item.createJNIRef ();
    item.register ();
    if (parentItem != null) {
        parentItem.itemCount = count;
    } else {
        this.itemCount = count;
    }
    ignoreExpand = true;
    NSOutlineView widget = (NSOutlineView)view;
    if (getDrawing()) {
        TreeItem[] selectedItems = getSelection ();
        if (parentItem != null) {
            widget.reloadItem (parentItem.handle, true);
        } else {
            widget.reloadData ();
        }
        selectItems (selectedItems, true);
    } else {
        reloadPending = true;
    }
    if (parentItem != null && parentItem.itemCount == 1 && parentItem.expanded) {
        widget.expandItem (parentItem.handle);
    }
    ignoreExpand = false;*/

}
wresult _w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		const char *text, w_treeitem *after) {
}
wresult _w_treeitem_remove_all(w_treeitem *item) {
}
wresult _w_treeitem_set_checked(w_treeitem *item, wbool checked) {
}
wresult _w_treeitem_set_expanded(w_treeitem *item, wbool expanded) {
}
wresult _w_treeitem_set_grayed(w_treeitem *item, wbool grayed) {
}
wresult _w_treeitem_set_image(w_treeitem *item, int image) {
}
wresult _w_treeitem_set_has_children(w_treeitem *item) {
    SWTTreeItem_setHasChildren(_W_TREEITEM(item)->handle);
}
wresult _w_treeitem_set_item_count(w_treeitem *item, int count) {
}
w_widget* _w_treeitem_get_parent_widget(w_item *item) {
}
void* _w_treeitem_get_data(w_item *item) {
    return SWTTreeItem_getUserData(_W_TREEITEM(item)->handle);
}
int _w_treeitem_get_index(w_item *item) {
}
w_string_ref* _w_treeitem_get_text(w_item *item) {
}
wresult _w_treeitem_set_data(w_item *item, void *data) {
    SWTTreeItem_setUserData(_W_TREEITEM(item)->handle,data);
    return W_TRUE;
}
wresult _w_treeitem_set_text(w_item *item, const char *string) {
}
void _w_treeitem_copy(w_widgetdata *from, w_widgetdata *to) {

}
wbool _w_treeitem_equals(w_widgetdata *obj1, w_widgetdata *obj2) {

}
/*
 * tree
 */
wresult _w_tree_deselect(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_deselect_all(w_tree *tree) {
}
int _w_tree_get_gridline_width(w_tree *tree) {
}
int _w_tree_get_header_height(w_tree *tree) {
}
wbool _w_tree_get_header_visible(w_tree *tree) {
}
wbool _w_tree_get_column(w_tree *tree, int index, w_treecolumn *column) {
}
int _w_tree_get_column_count(w_tree *tree) {
}
int _w_tree_get_column_order(w_tree *tree, int *orders, size_t length) {
}
void _w_tree_get_columns(w_tree *tree, w_iterator *columns) {
}
w_imagelist* _w_tree_get_imagelist(w_tree *tree) {
}
wbool _w_tree_get_item(w_tree *tree, w_point *point, w_treeitem *item) {
}
int _w_tree_get_item_height(w_tree *tree) {
}
wbool _w_tree_get_lines_visible(w_tree *tree) {
}
wbool _w_tree_get_parent_item(w_tree *tree, w_treeitem *item) {
}
wbool _w_tree_get_root_item(w_tree *tree, w_treeitem *root) {
    _w_widget_reserved* reserved = _w_widget_get_reserved(W_WIDGET(tree));
    _W_TREEITEM(root)->view =(NSOutlineView*) _W_CONTROL_RESERVED(reserved)->get_view(W_WIDGET(tree));
    _W_TREEITEM(root)->handle = 0;
    _W_TREEITEM(root)->tree = tree;
    _W_TREEITEM(root)->widgetdata.clazz =W_WIDGETDATA_CLASS( W_TREE_CLASS(_W_WIDGET(tree)->clazz)->class_treeitem);
    return W_TRUE;
}
void _w_tree_get_selection(w_tree *tree, w_iterator *selection) {
}
int _w_tree_get_selection_count(w_tree *tree) {
}
wbool _w_tree_get_sort_column(w_tree *tree, w_treecolumn *column) {
}
int _w_tree_get_sort_direction(w_tree *tree) {
}
wbool _w_tree_get_top_item(w_tree *tree, w_treeitem *topitem) {
}
wresult _w_tree_insert_first_column(w_tree *tree,
		struct _w_tree_reserved *reserved) {
}
wresult _w_tree_insert_column(w_tree *tree, w_treecolumn *column,
		const char *text, int index) {
	if (index == 0)
		return W_FALSE;
	if (text == 0)
		text = "";
}
wresult _w_tree_remove_all(w_tree *tree) {
}
wresult _w_tree_set_imagelist(w_tree *tree, w_imagelist *imagelist) {
	return W_TRUE;
}
wresult _w_tree_set_insert_mark(w_tree *tree, w_treeitem *item,
		wbool before) {
}
wresult _w_tree_set_item_height(w_tree *tree, int itemHeight) {
}
wresult _w_tree_set_lines_visible(w_tree *tree, wbool show) {
}
wresult _w_tree_select(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_select_all(w_tree *tree) {
}
wresult _w_tree_set_column_order(w_tree *tree, int *order, size_t length) {
}
wresult _w_tree_set_header_visible(w_tree *tree, wbool show) {
	return W_TRUE;
}
wresult _w_tree_set_selection(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_set_sort_column(w_tree *tree, w_treecolumn *column) {
}
wresult _w_tree_set_sort_direction(w_tree *tree, int direction) {
}
wresult _w_tree_set_top_item(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_show_column(w_tree *tree, w_treecolumn *column) {
}
wresult _w_tree_show_item(w_tree *tree, w_treeitem *item) {
}
wresult _w_tree_show_selection(w_tree *tree) {
}
wresult _w_tree_sort(w_tree *tree) {
}
wresult _w_tree_create_handle(w_widget *widget, _w_widget_reserved *reserved){
    wuint64 style = _W_WIDGET(widget)->style;
    NSScrollView* scrollWidget = NSScrollView_new(widget);
    NSScrollView_setHasHorizontalScroller(scrollWidget,(style & W_HSCROLL) != 0);
    NSScrollView_setHasVerticalScroller(scrollWidget,(style & W_VSCROLL) != 0);
    NSScrollView_setAutohidesScrollers(scrollWidget,W_TRUE);
    NSScrollView_setBorderType(scrollWidget,(style & W_BORDER) ? NSBezelBorder : NSNoBorder);

    NSOutlineView* outline = SWTOutlineView_new(widget);
    NSOutlineView_setAllowsMultipleSelection (outline,(style & W_MULTI) != 0);
    NSOutlineView_setAutoresizesOutlineColumn (outline,W_FALSE);
    NSOutlineView_setAutosaveExpandedItems (outline,W_TRUE);
    NSOutlineView_setColumnAutoresizingStyle (outline,NSTableViewNoColumnAutoresizing);
    NSOutlineView_setIntercellSpacing(outline,CELL_GAP,CELL_GAP);
    //outline.setDoubleAction (OS.sel_sendDoubleSelection);
    //if (!(style & W_BORDER)) outline.setFocusRingType (OS.NSFocusRingTypeNone);
    
    NSScrollView_setDocumentView(scrollWidget,(NSView*) outline);

    _W_TREE(widget)->headerView = NSTableHeaderView_new(widget);
    //NSTableView_setHeaderView((NSTableView*)outline,0);

    //NSString str = NSString.string();
    NSString* _id;
    char __id[10];
    if ((style & W_CHECK) != 0) {
        /*sprintf(__id, "id_%d",_W_TREE(widget)->NEXT_ID++);
        _id = NSString_stringWithUTF8String(__id);
        NSTableColumn *checkColumn = SWTTableColumn_new(_id);
        NSObject_release(NSOBJECT(_id));
        checkColumn.headerCell ().setTitle (str);
        outline.addTableColumn (checkColumn);
        outline.setOutlineTableColumn (checkColumn);
        checkColumn.setResizingMask (OS.NSTableColumnNoResizing);
        checkColumn.setEditable (false);
        long cls = NSButton.cellClass ();*//* use our custom cell class */
        /*buttonCell = new NSButtonCell (OS.class_createInstance (cls, 0));
        buttonCell.init ();
        checkColumn.setDataCell (buttonCell);
        buttonCell.setButtonType (OS.NSSwitchButton);
        buttonCell.setControlSize (OS.NSSmallControlSize);
        buttonCell.setImagePosition (OS.NSImageOnly);
        buttonCell.setAllowsMixedState (true);
        checkColumn.setWidth (getCheckColumnWidth ());*/
    }
    sprintf(__id, "id_%d",_W_TREE(widget)->NEXT_ID++);
    _id = NSString_stringWithUTF8String(__id);
    NSTableColumn *firstColumn = SWTTableColumn_new(_id);
    /*
    * Feature in Cocoa.  If a column's width is too small to show any content
    * then outlineView_objectValueForTableColumn_byItem is never invoked to
    * query for item values, which is a problem for VIRTUAL Trees.  The
    * workaround is to ensure that, for 0-column Trees, the internal first
    * column always has a minimal width that makes this call come in.
    */
    NSTableColumn_setMinWidth(firstColumn, FIRST_COLUMN_MINIMUM_WIDTH);
    //NSTableColumn_setWidth(firstColumn,0);
    NSTableColumn_setResizingMask (firstColumn,NSTableColumnNoResizing);
    NSCell_setTitle(NSTableColumn_headerCell(firstColumn),_id/*str*/);
    NSTableView_addTableColumn((NSTableView*)outline, firstColumn);
    NSOutlineView_setOutlineTableColumn(outline, firstColumn);
    NSCell* dataCell = SWTImageTextCell_new(_id);
    NSCell_setLineBreakMode(dataCell, NSLineBreakByTruncatingTail);
    NSTableColumn_setDataCell (firstColumn,dataCell);
    _W_WIDGET(widget)->handle = (NSView*)scrollWidget;
    return W_TRUE;
}
void _w_tree_dispose(w_widget *widget) {

}
wresult _ns_tree_outlineViewItemWillExpand(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
    w_treeitem item;
    w_event_tree event;
    memset(&event, 0, sizeof(event));
    event.event.type = W_EVENT_ITEM_EXPAND;
    event.event.widget = widget;
    event.item = &item;
    _W_TREEITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
    W_TREE_GET_CLASS(widget)->class_treeitem);
    _W_TREEITEM(&item)->tree = W_TREE(widget);
    _W_TREEITEM(&item)->handle = e->args[0];
    _W_TREEITEM(&item)->view = (NSOutlineView*) _W_CONTROL_RESERVED(reserved)->get_view(widget);
    _w_widget_send_event(widget, (w_event*) &event);
}
wresult _ns_tree_outlineViewSelectionDidChange(w_widget* widget,_w_event_platform* e,_w_widget_reserved* reserved) {
    //if (ignoreSelect) return;
    w_treeitem item;
    w_event_tree event;
    memset(&event, 0, sizeof(event));
    event.event.type = W_EVENT_ITEM_SELECTION;
    event.event.widget = widget;
    NSUInteger row = NSTableView_selectedRow((NSTableView*)e->handle);
    if(row == -1){
        event.item = 0;
    }else{
        SWTTreeItem* handle = (SWTTreeItem*) NSOutlineView_itemAtRow((NSOutlineView*)e->handle, row);
        event.item = &item;
        _W_TREEITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
        W_TREE_GET_CLASS(widget)->class_treeitem);
        _W_TREEITEM(&item)->tree = W_TREE(widget);
        _W_TREEITEM(&item)->handle = handle;
        _W_TREEITEM(&item)->view = (NSOutlineView*)_W_CONTROL_RESERVED(reserved)->get_view(widget);
    }
    _w_widget_send_event(widget, (w_event*) &event);
}
void _w_tree_class_init(struct _w_tree_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));

	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TREE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_tree_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_tree);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_tree);
	W_WIDGET_CLASS(clazz)->dispose = _w_tree_dispose;

	clazz->deselect = _w_tree_deselect;
	clazz->deselect_all = _w_tree_deselect_all;
	clazz->get_gridline_width = _w_tree_get_gridline_width;
	clazz->get_header_height = _w_tree_get_header_height;
	clazz->get_header_visible = _w_tree_get_header_visible;
	clazz->get_column = _w_tree_get_column;
	clazz->get_column_count = _w_tree_get_column_count;
	clazz->get_columns = _w_tree_get_columns;
	clazz->get_imagelist = _w_tree_get_imagelist;
	clazz->get_item = _w_tree_get_item;
	clazz->get_item_height = _w_tree_get_item_height;
	clazz->get_lines_visible = _w_tree_get_lines_visible;
	clazz->get_parent_item = _w_tree_get_parent_item;
	clazz->get_root_item = _w_tree_get_root_item;
	clazz->get_selection = _w_tree_get_selection;
	clazz->get_selection_count = _w_tree_get_selection_count;
	clazz->get_sort_column = _w_tree_get_sort_column;
	clazz->get_sort_direction = _w_tree_get_sort_direction;
	clazz->get_top_item = _w_tree_get_top_item;
	clazz->insert_column = _w_tree_insert_column;
	clazz->remove_all = _w_tree_remove_all;
	clazz->set_imagelist = _w_tree_set_imagelist;
	clazz->set_insert_mark = _w_tree_set_insert_mark;
	clazz->set_item_height = _w_tree_set_item_height;
	clazz->set_lines_visible = _w_tree_set_lines_visible;
	clazz->select = _w_tree_select;
	clazz->select_all = _w_tree_select_all;
	clazz->set_header_visible = _w_tree_set_header_visible;
	clazz->set_selection = _w_tree_set_selection;
	clazz->set_sort_column = _w_tree_set_sort_column;
	clazz->set_sort_direction = _w_tree_set_sort_direction;
	clazz->set_top_item = _w_tree_set_top_item;
	clazz->show_column = _w_tree_show_column;
	clazz->show_item = _w_tree_show_item;
	clazz->show_selection = _w_tree_show_selection;
	clazz->sort = _w_tree_sort;
	/*
	 * tree column
	 */
	struct _w_treecolumn_class *treecolumn = clazz->class_treecolumn;
	treecolumn->item.widgetdata.toolkit =mac_toolkit;
	treecolumn->get_alignment = _w_treecolumn_get_alignment;
	treecolumn->get_id = _w_treecolumn_get_id;
	treecolumn->get_image = _w_treecolumn_get_image;
	treecolumn->get_moveable = _w_treecolumn_get_moveable;
	treecolumn->get_resizable = _w_treecolumn_get_resizable;
	treecolumn->get_tooltip_text = _w_treecolumn_get_tooltip_text;
	treecolumn->get_width = _w_treecolumn_get_width;
	treecolumn->pack = _w_treecolumn_pack;
	treecolumn->set_alignment = _w_treecolumn_set_alignment;
	treecolumn->set_id = _w_treecolumn_set_id;
	treecolumn->set_image = _w_treecolumn_set_image;
	treecolumn->set_moveable = _w_treecolumn_set_moveable;
	treecolumn->set_resizable = _w_treecolumn_set_resizable;
	treecolumn->set_tooltip_text = _w_treecolumn_set_tooltip_text;
	treecolumn->set_width = _w_treecolumn_set_width;
	treecolumn->item.widgetdata.close = _w_widgetdata_close;
	treecolumn->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	//treecolumn->item.resourcedata.copy = _w_treeitem_copy;
	//treecolumn->item.resourcedata.equals = _w_treeitem_equals;
	treecolumn->item.get_parent_widget = _w_treecolumn_get_parent_widget;
	treecolumn->item.get_data = _w_treecolumn_get_data;
	treecolumn->item.get_index = _w_treecolumn_get_index;
	treecolumn->item.get_text = _w_treecolumn_get_text;
	treecolumn->item.set_data = _w_treecolumn_set_data;
	treecolumn->item.set_text = _w_treecolumn_set_text;
	/*
	 * tree item
	 */
	struct _w_treeitem_class *treeitem = clazz->class_treeitem;
	treeitem->item.widgetdata.toolkit = mac_toolkit;
	treeitem->clear = _w_treeitem_clear;
	treeitem->clear_all = _w_treeitem_clear_all;
	treeitem->get_bounds = _w_treeitem_get_bounds;
	treeitem->get_checked = _w_treeitem_get_checked;
	treeitem->get_expanded = _w_treeitem_get_expanded;
	treeitem->get_first_child = _w_treeitem_get_first_child;
	treeitem->get_grayed = _w_treeitem_get_grayed;
	treeitem->get_item = _w_treeitem_get_item;
	treeitem->get_item_count = _w_treeitem_get_item_count;
	treeitem->get_items = _w_treeitem_get_items;
	treeitem->get_image = _w_treeitem_get_image;
	treeitem->get_last_child = _w_treeitem_get_last_child;
	treeitem->get_next_sibling = _w_treeitem_get_next_sibling;
	treeitem->get_parent_item = _w_treeitem_get_parent_item;
	treeitem->get_prev_sibling = _w_treeitem_get_prev_sibling;
	treeitem->insert_item = _w_treeitem_insert_item;
	treeitem->insert_item_after = _w_treeitem_insert_item_after;
	treeitem->remove_all = _w_treeitem_remove_all;
	treeitem->set_checked = _w_treeitem_set_checked;
	treeitem->set_expanded = _w_treeitem_set_expanded;
	treeitem->set_grayed = _w_treeitem_set_grayed;
	treeitem->set_image = _w_treeitem_set_image;
	treeitem->set_has_children = _w_treeitem_set_has_children;
	treeitem->set_item_count = _w_treeitem_set_item_count;
	treeitem->item.get_parent_widget = _w_treeitem_get_parent_widget;
	treeitem->item.get_data = _w_treeitem_get_data;
	treeitem->item.get_index = _w_treeitem_get_index;
	treeitem->item.get_text = _w_treeitem_get_text;
	treeitem->item.set_data = _w_treeitem_set_data;
	treeitem->item.set_text = _w_treeitem_set_text;
	treeitem->item.widgetdata.close = _w_widgetdata_close;
	treeitem->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	treeitem->item.widgetdata.copy = _w_treeitem_copy;
	treeitem->item.widgetdata.equals = _w_treeitem_equals;
	/*
	 * reserved
	 */
	struct _w_tree_reserved *reserved = _W_TREE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
    _W_WIDGET_RESERVED(reserved)->create_handle = _w_tree_create_handle;
    _W_WIDGET_RESERVED(reserved)->check_style = _w_table_check_style;
        /*
     messages
     */
    _w_msg* msgs = _W_WIDGET_RESERVED(reserved)->msgs;
    msgs[_NS_outlineViewItemWillExpand]=_ns_tree_outlineViewItemWillExpand;
    msgs[_NS_outlineViewSelectionDidChange]=_ns_tree_outlineViewSelectionDidChange;
}
#endif
