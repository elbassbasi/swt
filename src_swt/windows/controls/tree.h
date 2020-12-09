/*
 * tree.h
 *
 *  Created on: 21 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_TREE_H_
#define SRC_WIN32_CONTROLS_TREE_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/composite.h"
struct _w_tree_columns {
	unsigned short alloc;
	unsigned char flags[0];
};
typedef struct _w_tree {
	struct _w_composite composite;
	HWND hwndParent;
	HWND hwndHeader;
	HTREEITEM hAnchor;
	int scrollWidth;
	unsigned explorerTheme :1;
	unsigned printClient :1;
	unsigned linesVisible :1;
	unsigned ignoreDrawForeground :1;
	unsigned ignoreDrawBackground :1;
	unsigned ignoreDrawSelection :1;
	unsigned ignoreDrawFocus :1;
	unsigned ignoreDrawHot :1;
	unsigned ignoreFullSelection :1;
	unsigned ignoreCustomDraw :1;
	unsigned enableEraseItem :1;
	unsigned enableMeasureItem :1;
	unsigned enableSelectionForeground :1;
	unsigned lockSelection :1;
	unsigned oldSelected :1;
	unsigned newSelected :1;
	unsigned ignoreSelect :1;
	unsigned ignoreExpand :1;
	unsigned ignoreColumnMove :1;
	unsigned ignoreResize : 1;
	unsigned hooksEraseItem : 1;
	struct _w_tree_columns *column_info;
	w_imagelist* imagelist;
	int sortColumn;
	int sortDirection;
	int selectionForeground;
} _w_tree;
typedef struct _w_treecolumn {
	w_widgetdata widgetdata;
	w_tree *tree;
	int index;
} _w_treecolumn;
typedef struct _w_treeitem {
	w_widgetdata widgetdata;
	w_tree *tree;
	HTREEITEM htreeitem;
} _w_treeitem;

typedef struct _w_tree_reserved {
	struct _w_composite_reserved composite;
	WNDPROC headerProc;
} _w_tree_reserved;
#define _W_TREECOLUMN(x) ((_w_treecolumn*)x)
#define _W_TREEITEM(x) ((_w_treeitem*)x)
#define _W_TREE(x) ((_w_tree*)x)
#define _W_TREE_RESERVED(x) ((struct _w_tree_reserved*)x)

#define TREE_INCREMENT 5
#define _W_TREE_BOUNDS_GET_TEXT (1 << 0)
#define _W_TREE_BOUNDS_GET_IMAGE (1 << 1)
#define _W_TREE_BOUNDS_FULLTEXT (1 << 2)
#define _W_TREE_BOUNDS_FULLIMAGE (1 << 3)
#define _W_TREE_BOUNDS_CLIP (1 << 4)
#define _W_TREE_INSET 3
#define _W_TREE_GRID_WIDTH 1
#define _W_TREE_EXPLORER_EXTRA 2
#define _W_TREE_INSET  3
#define _W_TREE_HEADER_MARGIN 12
#define _W_TREE_HEADER_EXTRA 3

#define EXPLORER_THEME FALSE

void _w_tree_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle);
HTREEITEM _tree_find_item(HWND handle, HTREEITEM hFirstItem, int index);
int _tree_get_item_count(HWND handle, HTREEITEM hFirstItem);
HTREEITEM _tree_find_previous(HWND handle, HTREEITEM parent, int index);
wresult _w_treeitem_insert_item_0(w_treeitem *item, w_treeitem *subitem,
		const char *text, HTREEITEM after);
wresult _w_tree_create_parent(w_tree *tree);
HWND _w_tree_top_handle(w_control *control);
int _w_tree_get_header_width(w_tree *tree);
void _w_tree_set_scroll_width_0(w_tree *tree, int width);
void _w_tree_set_scroll_width(w_tree *tree);
void _w_tree_update_scrollbar(w_tree *tree);
void _w_tree_call_window_proc(w_widget *widget, _w_event_platform *e,
		struct _w_widget_reserved *reserved);
void _w_treeitem_get_bounds_0(w_treeitem *item, RECT *rect, HDC hDC, int index,
		int flags);
/*
 * tree column
 */
#define _W_TREECOLUMN_MOVEABLE 1
#define _W_TREECOLUMN_RESIZABLE 2
struct _w_tree_columns* _w_tree_columns_resize(w_tree *tree, int count);
int _w_treecolumn_get_alignment(w_treecolumn *column);
int _w_treecolumn_get_order(w_treecolumn *column);
int _w_treecolumn_get_image(w_treecolumn *column);
wresult _w_treecolumn_get_moveable_0(w_tree *tree, int index);
wresult _w_treecolumn_get_moveable(w_treecolumn *column);
wresult _w_treecolumn_get_resizable_0(w_tree *tree, int index);
wresult _w_treecolumn_get_resizable(w_treecolumn *column);
wresult _w_treecolumn_get_tooltip_text(w_treecolumn *column, w_alloc *text);
int _w_treecolumn_get_width(w_treecolumn *column);
wresult _w_treecolumn_pack(w_treecolumn *column);
wresult _w_treecolumn_set_alignment(w_treecolumn *column, int alignment);
wresult _w_treecolumn_set_image(w_treecolumn *column, int image);
wresult _w_treecolumn_set_moveable(w_treecolumn *column, wresult moveable);
wresult _w_treecolumn_set_resizable(w_treecolumn *column, wresult resizable);
wresult _w_treecolumn_set_tooltip_text(w_treecolumn *column,
		const char *text);
wresult _w_treecolumn_set_width(w_treecolumn *column, int width);
wresult _w_treecolumn_set_order(w_treecolumn *column, int order);
w_widget* _w_treecolumn_get_parent_widget(w_item *item);
void* _w_treecolumn_get_data(w_item *item);
int _w_treecolumn_get_index(w_item *item);
wresult _w_treecolumn_get_text(w_item *item, w_alloc *text);
wresult _w_treecolumn_set_data(w_item *item, void *data);
wresult _w_treecolumn_set_text(w_item *item, const char *string);
/*
 * tree item
 */
wresult _w_treeitem_clear(w_treeitem *item, int index, wresult all);
wresult _w_treeitem_clear_all(w_treeitem *item, wresult all);
void _w_treeitem_get_bounds_i(w_treeitem *item, int index, w_rect *bounds);
void _w_treeitem_get_bounds(w_treeitem *item, w_rect *bounds);
wresult _w_treeitem_get_checked(w_treeitem *item);
wresult _w_treeitem_get_expanded(w_treeitem *item);
wresult _w_treeitem_get_first_child(w_treeitem *item, w_treeitem *child);
wresult _w_treeitem_get_grayed(w_treeitem *item);
wresult _w_treeitem_get_item(w_treeitem *item, int index, w_treeitem *subitem);
int _w_treeitem_get_item_count(w_treeitem *item);
void _w_treeitem_get_items(w_treeitem *item, w_iterator *items);
int _w_treeitem_get_image(w_treeitem *item);
wresult _w_treeitem_get_last_child(w_treeitem *item, w_treeitem *child);
wresult _w_treeitem_get_next_sibling(w_treeitem *item, w_treeitem *next);
w_tree* _w_treeitem_get_parent(w_treeitem *item);
wresult _w_treeitem_get_parent_item(w_treeitem *item, w_treeitem *parent);
wresult _w_treeitem_get_prev_sibling(w_treeitem *item, w_treeitem *prev);
wresult _w_treeitem_insert_item(w_treeitem *item, w_treeitem *subitem,
		const char *text, int index);
wresult _w_treeitem_insert_item_after(w_treeitem *item, w_treeitem *subitem,
		const char *text, w_treeitem *after);
wresult _w_treeitem_remove_all(w_treeitem *item);
wresult _w_treeitem_set_checked(w_treeitem *item, wresult checked);
wresult _w_treeitem_set_expanded(w_treeitem *item, wresult expanded);
wresult _w_treeitem_set_grayed(w_treeitem *item, wresult grayed);
wresult _w_treeitem_set_has_children(w_treeitem *item);
wresult _w_treeitem_set_image(w_treeitem *item, int image);
w_widget* _w_treeitem_get_parent_widget(w_item *item);
void* _w_treeitem_get_data(w_item *item);
int _w_treeitem_get_index(w_item *item);
wresult _w_treeitem_get_text(w_item *item, w_alloc *text);
wresult _w_treeitem_set_data(w_item *item, void *data);
wresult _w_treeitem_set_text(w_item *item, const char *string);
void _w_treeitem_copy(w_widgetdata *from, w_widgetdata *to);
wresult _w_treeitem_equals(w_widgetdata *obj1, w_widgetdata *obj2);
/*
 * tree
 */
wresult _w_tree_deselect(w_tree *tree, w_treeitem *item);
wresult _w_tree_deselect_all(w_tree *tree);
int _w_tree_get_gridline_width(w_tree *tree);
int _w_tree_get_header_height(w_tree *tree);
wresult _w_tree_get_header_visible(w_tree *tree);
wresult _w_tree_get_column(w_tree *tree, int index, w_treecolumn *column);
int _w_tree_get_column_count(w_tree *tree);
int _w_tree_get_column_order(w_tree *tree, int *orders, size_t length);
void _w_tree_get_columns(w_tree *tree, w_iterator *columns);
w_imagelist* _w_tree_get_imagelist(w_tree *tree);
wresult _w_tree_get_item(w_tree *tree, w_point *point, w_treeitem *item);
int _w_tree_get_item_height(w_tree *tree);
wresult _w_tree_get_lines_visible(w_tree *tree);
wresult _w_tree_get_parent_item(w_tree *tree, w_treeitem *item);
wresult _w_tree_get_root_item(w_tree *tree, w_treeitem *root);
void _w_tree_get_selection(w_tree *tree, w_iterator *selection);
int _w_tree_get_selection_count(w_tree *tree);
wresult _w_tree_get_sort_column(w_tree *tree, w_treecolumn *column);
int _w_tree_get_sort_direction(w_tree *tree);
wresult _w_tree_get_top_item(w_tree *tree, w_treeitem *topitem);
wresult _w_tree_insert_column(w_tree *tree, w_treecolumn *column,
		const char *text, int index);
wresult _w_tree_remove_all(w_tree *tree) ;
wresult _w_tree_set_imagelist(w_tree *tree, w_imagelist *imagelist);
wresult _w_tree_set_insert_mark(w_tree *tree, w_treeitem *item,
		wresult before);
wresult _w_tree_set_item_height(w_tree *tree, int itemHeight);
wresult _w_tree_set_lines_visible(w_tree *tree, wresult show);
wresult _w_tree_select(w_tree *tree, w_treeitem *item);
wresult _w_tree_select_all(w_tree *tree);
wresult _w_tree_set_column_order(w_tree *tree, int *order, size_t length);
wresult _w_tree_set_header_visible(w_tree *tree, wresult show);
wresult _w_tree_set_selection(w_tree *tree, w_treeitem *item);
wresult _w_tree_set_sort_column(w_tree *tree, w_treecolumn *column);
wresult _w_tree_set_sort_direction(w_tree *tree, int direction);
wresult _w_tree_set_top_item(w_tree *tree, w_treeitem *item) ;
wresult _w_tree_show_column(w_tree *tree, w_treecolumn *column);
wresult _w_tree_show_item(w_tree *tree, w_treeitem *item);
wresult _w_tree_show_selection(w_tree *tree);
wresult _w_tree_sort(w_tree *tree);
wresult _w_tree_set_bounds(w_control *control, w_point *location,
		w_size *size);
wresult _w_tree_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event);
void _w_tree_dispose(w_widget *resource);
wresult _w_tree_is_item_selected(w_widget *widget, NMTVCUSTOMDRAW *nmcd);
HWND _w_tree_scrolled_handle(w_widget *widget);
wresult _w_tree_insert_column(w_tree *tree, w_treecolumn *column,
		const char *text, int index);
int _TREE_WM_NOTIFY_HEADER0(w_widget *widget, struct _w_event_platform *e,
		NMHEADERW *hdr);
int _TREE_WM_CHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_ERASEBKGND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_GETOBJECT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_HSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_KEYDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_LBUTTONDBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_LBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_MOUSEMOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_MOUSEWHEEL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_MOVE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_RBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_PAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_SETCURSOR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_SETFONT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_SETREDRAW(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_SYSCOLORCHANGE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_VSCROLL(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_TIMER(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_CTLCOLORCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_GETDISPINFO(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_CDDS_PREPAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_CDDS_ITEMPREPAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_CDDS_ITEMPOSTPAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_CDDS_POSTPAINT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_CUSTOMDRAW(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_DBLCLK(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_ITEMCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_SELCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_SELCHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_ITEMEXPANDED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_ITEMEXPANDING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_BEGINDRAG(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFYCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_HEADER(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_TOOLTIP0(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _TREE_WM_NOTIFY_TOOLTIP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _w_tree_post_event(w_widget *widget, struct w_event *ee);
int _TREE_WM_NOTIFY(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);

/*
 *
 */
void _w_tree_class_init(struct _w_tree_class *clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_TREE_H_ */
