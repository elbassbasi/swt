/*
 * tabfolder.h
 *
 *  Created on: 25 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_TABFOLDER_H_
#define SWT_CONTROLS_TABFOLDER_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_tabitem {
	w_item item;
	void *handle[6];
} w_tabitem;
#define W_TABITEM(x) ((w_tabitem*)x)
typedef struct w_tabfolder {
	w_composite composite;
	void *handle[10];
} w_tabfolder;

typedef struct w_event_tabitem {
	w_event event;
	w_tabitem *item;
	w_control *control;
} w_event_tabitem;
#define W_TABFOLDER(x) ((w_tabfolder*)x)

struct _w_tabitem_class {
	struct _w_item_class item;
	wresult (*get_bounds)(w_tabitem *item, w_rect *rect);
	int (*get_image)(w_tabitem *item);
	wresult (*remove)(w_tabitem *item,int destroy_Control);
	wresult (*set_image)(w_tabitem *item, int image);
	wresult (*pack)(w_tabitem *item,w_control* control);
};
struct _w_tabfolder_class {
	struct _w_composite_class composite;
	struct _w_tabitem_class *class_tabfolderitem;
	w_imagelist* (*get_imagelist)(w_tabfolder *tabfolder);
	wresult (*get_item)(w_tabfolder *tabfolder, int index, w_tabitem *item);
	wresult (*get_item_p)(w_tabfolder *tabfolder, w_point *point,
			w_tabitem *item);
	int (*get_item_count)(w_tabfolder *tabfolder);
	void (*get_items)(w_tabfolder *tabfolder, w_iterator *items);
	wresult (*get_selection)(w_tabfolder *tabfolder, w_tabitem *item);
	wresult (*insert_item)(w_tabfolder *tabfolder, w_tabitem *item,
			const char *text, int index);
	wresult (*set_imagelist)(w_tabfolder *tabfolder, w_imagelist *imagelist);
	wresult (*set_selection)(w_tabfolder *tabfolder, int index);
};
/*
 * tabitem
 */
SWT_PUBLIC w_control* w_tabitem_get_control(w_tabitem *item);
SWT_PUBLIC wresult w_tabitem_get_bounds(w_tabitem *item, w_rect *rect);
SWT_PUBLIC int w_tabitem_get_image(w_tabitem *item);
SWT_PUBLIC wresult w_tabitem_remove(w_tabitem *item,int destroy_Control);
SWT_PUBLIC wresult w_tabitem_set_image(w_tabitem *item, int image);
SWT_PUBLIC wresult w_tabitem_set_control(w_tabitem *item, w_control *control);
SWT_PUBLIC wresult w_tabitem_pack(w_tabitem *item,w_control* control);
/*
 * tabfolder
 */
SWT_PUBLIC wresult w_tabfolder_create(w_tabfolder *tabfolder,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_tabfolder* w_tabfolder_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_imagelist* w_tabfolder_get_imagelist(w_tabfolder *tabfolder);
SWT_PUBLIC wresult w_tabfolder_get_item(w_tabfolder *tabfolder, int index,
		w_tabitem *item);
SWT_PUBLIC wresult w_tabfolder_get_item_p(w_tabfolder *tabfolder,
		w_point *point, w_tabitem *item);
SWT_PUBLIC int w_tabfolder_get_item_count(w_tabfolder *tabfolder);
SWT_PUBLIC void w_tabfolder_get_items(w_tabfolder *tabfolder,
		w_iterator *items);
SWT_PUBLIC wresult w_tabfolder_get_selection(w_tabfolder *tabfolder,
		w_tabitem *item);
SWT_PUBLIC wresult w_tabfolder_insert_item(w_tabfolder *tabfolder,
		w_tabitem *item, const char *text, int index);
SWT_PUBLIC wresult w_tabfolder_set_imagelist(w_tabfolder *tabfolder, w_imagelist *imagelist);
SWT_PUBLIC wresult w_tabfolder_set_selection(w_tabfolder *tabfolder,
		int index);
#define W_TABFOLDER_CLASS(x) ((struct _w_tabfolder_class*)x)
#define W_TABFOLDER_GET_CLASS(x) ((struct _w_tabfolder_class*)W_WIDGET_GET_CLASS(x))
#define W_TABITEM_CLASS(x) ((struct _w_tabitem_class*)x)
#define W_TABITEM_GET_CLASS(x) ((struct _w_tabitem_class*)W_WIDGETDATA_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_TABFOLDER_H_ */
