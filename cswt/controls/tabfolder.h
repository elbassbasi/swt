//
// Created by azeddine on 27/08/2020.
//

#ifndef SRC_CUSTOM_CONTROLS_TABFOLDER_H
#define SRC_CUSTOM_CONTROLS_TABFOLDER_H
#include "../widgets/composite.h"
enum {
		WC_TABFOLDER_REDRAW = 1 << 1, //
		WC_TABFOLDER_REDRAW_TABS = 1 << 2, //
		WC_TABFOLDER_UPDATE_TAB_HEIGHT = 1 << 3, //
		WC_TABFOLDER_PART_BODY = -1,//
		WC_TABFOLDER_PART_HEADER = -2,//
		WC_TABFOLDER_PART_BORDER = -3,//
		WC_TABFOLDER_PART_BACKGROUND = -4,//
		WC_TABFOLDER_PART_MAX_BUTTON = -5,//
		WC_TABFOLDER_PART_MIN_BUTTON = -6,//
		WC_TABFOLDER_PART_CHEVRON_BUTTON = -7,//
		WC_TABFOLDER_PART_CLOSE_BUTTON = -8,//
		WC_TABFOLDER_MINIMUM_SIZE = 1 << 24,//
};
typedef struct wc_tabitem {
	struct wc_tabitem* next;
	struct wc_tabitem* prev;
    char* text;
    void* userdata;
	w_font *font;
	int state;
	int index;
	int image;
	int shortenedTextWidth;
	int closeImageState;
	unsigned showing :1;
	unsigned showClose :1;
	w_rect bounds;
}wc_tabitem;
void wc_tabitem_get_close_rect(w_tabfolder *tabfolder,wc_tabitem* item,w_graphics* gc,w_rect* rect);
int wc_tabfolder_get_right_item_edge(w_tabfolder *tabfolder, w_graphics *gc);
void wc_tabfolder_set_selection_0(w_tabfolder *tabfolder,wc_tabitem* selection,wbool notify);
typedef struct _wc_tabitem {
    struct _w_widgetdata_class *clazz;
    w_tabfolder* parent;
    wc_tabitem* item;
    int index;
}_wc_tabitem;
#define _WC_TABITEM(x) ((_wc_tabitem*)x)

typedef struct wc_tabfolder_renderer {
	void (*init)(w_tabfolder *tabfolder);
	void (*dispose)(w_tabfolder *tabfolder);
	void (*compute_size)(w_tabfolder *tabfolder, wc_tabitem *item, int part,
			int state, w_graphics *gc, w_size *result, int wHint, int hHint);
	void (*compute_trim)(w_tabfolder *tabfolder, wc_tabitem *item, int part,
			int state, w_rect *result, const w_rect *r);
	void (*draw)(w_tabfolder *tabfolder, wc_tabitem *item, int part, int state,
			w_rect* bounds,  w_graphics *gc);
}wc_tabfolder_renderer;

typedef struct wc_tabfolder_priv {
	wc_tabitem* items;
	int items_count;
    w_imagelist* imagelist;
    wc_tabitem* selectedItem;
    wc_tabitem* firstItem;
    w_control* selectedcontrol;
    wc_tabfolder_renderer* renderer;
	//CTabItem *priorityItem;
	w_color selectionBackground;
	w_color selectionForeground;
	w_image selectionBgImage;
	//int selectedIndex;
	//int firstIndex;
	w_size oldSize;
	//int priorityIndex;
	//int priorityLength;
	int tabHeight;
	size_t length;
	size_t alloc;
	int updateFlags;
	int fixedTabHeight;
	int marginWidth;
	int marginHeight;
	// when disposing CTabFolder, don't try to layout the items or
	// change the selection as each child is destroyed.
	unsigned inDispose :1;
	unsigned single :1;
	unsigned simple :1;
	unsigned platform : 1;
	unsigned borderVisible :1;
	unsigned showMin :1;
	unsigned minimized :1;
	unsigned showMax :1;
	unsigned showUnselectedClose :1;
	unsigned showUnselectedImage :1;
	unsigned showChevron :1;
	unsigned maximized :1;
	unsigned mru :1;
	unsigned ignoreResize :1;
	unsigned fontChanged :1;
}wc_tabfolder_priv;

void wc_tabfolder_init_class(struct _w_tabfolder_class *clazz);
#endif //SRC_CUSTOM_CONTROLS_TABFOLDER_H
