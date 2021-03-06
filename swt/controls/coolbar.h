/*
 * coolbar.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_COOLBAR_H_
#define SWT_CONTROLS_COOLBAR_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_coolbar {
	w_composite composite;
	void *handle[0x10];
} w_coolbar;

#define W_COOLBAR(x) ((w_coolbar*)x)
typedef struct w_coolitem {
	w_item item;
	void *handle[6];
} w_coolitem;
#define W_COOLITEM(x) ((w_coolitem*)x)
struct _w_coolitem_class {
	struct _w_item_class item;
	wresult (*compute_size)(w_coolitem *coolitem, w_size *result, int wHint,
			int hHint);
	wresult (*get_bounds)(w_coolitem *coolitem, w_rect *bounds);
	w_control* (*get_control)(w_coolitem *coolitem);
	wresult (*get_minimum_size)(w_coolitem *coolitem, w_size *size);
	int (*get_order)(w_coolitem *coolitem);
	wresult (*get_preferred_size)(w_coolitem *coolitem, w_size *size);
	wresult (*get_size)(w_coolitem *coolitem, w_size *size);
	int (*get_wrap_indice)(w_coolitem *coolitem);
	wresult (*set_control)(w_coolitem *coolitem,w_control* control);
	wresult (*set_layout)(w_coolitem *coolitem, int order, int wrapindices,
			w_size *sizes);
	wresult (*set_minimum_size)(w_coolitem *coolitem, w_size *size);
	wresult (*set_order)(w_coolitem *coolitem, int order);
	wresult (*set_preferred_size)(w_coolitem *coolitem, w_size *size);
	wresult (*set_size)(w_coolitem *coolitem, w_size *size);
	wresult (*set_wrap_indice)(w_coolitem *coolitem, int wrap_indice);
};
struct _w_coolbar_class {
	struct _w_composite_class composite;
	struct _w_coolitem_class *class_coolbaritem;
	wresult (*get_item)(w_coolbar *coolbar, int index, w_coolitem *item);
	int (*get_item_count)(w_coolbar *coolbar);
	void (*get_items)(w_coolbar *coolbar, w_iterator *items);
	wresult (*get_locked)(w_coolbar *coolbar);
	wresult (*insert_item)(w_coolbar *coolbar, w_coolitem *item,int style, int index);
	wresult (*set_locked)(w_coolbar *coolbar, int locked);
};
/*
 *
 */
SWT_PUBLIC wresult w_coolbar_create(w_coolbar *coolbar,
		struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC w_coolbar* w_coolbar_new(struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_coolbar_get_item(w_coolbar *coolbar, int index,
		w_coolitem *item);
SWT_PUBLIC int w_coolbar_get_item_count(w_coolbar *coolbar);
SWT_PUBLIC void w_coolbar_get_items(w_coolbar *coolbar, w_iterator *items);
SWT_PUBLIC wresult w_coolbar_get_locked(w_coolbar *coolbar);
SWT_PUBLIC wresult w_coolbar_insert_item(w_coolbar *coolbar, w_coolitem *item,int style,
		int index);
SWT_PUBLIC wresult w_coolbar_set_locked(w_coolbar *coolbar, int locked);
/*
 *
 */
SWT_PUBLIC wresult w_coolitem_compute_size(w_coolitem *coolitem,
		w_size *result, int wHint, int hHint);
SWT_PUBLIC wresult w_coolitem_get_bounds(w_coolitem *coolitem, w_rect *bounds);
SWT_PUBLIC w_control* w_coolitem_get_control(w_coolitem *coolitem);
SWT_PUBLIC wresult w_coolitem_get_minimum_size(w_coolitem *coolitem,
		w_size *size);
SWT_PUBLIC int w_coolitem_get_order(w_coolitem *coolitem);
SWT_PUBLIC wresult w_coolitem_get_preferred_size(w_coolitem *coolitem,
		w_size *size);
SWT_PUBLIC wresult w_coolitem_get_size(w_coolitem *coolitem, w_size *size);
SWT_PUBLIC int w_coolitem_get_wrap_indice(w_coolitem *coolitem);
SWT_PUBLIC wresult w_coolitem_set_control(w_coolitem *coolitem,
		w_control *control);
SWT_PUBLIC wresult w_coolitem_set_layout(w_coolitem *coolitem, int order,
		int wrapindices, w_size *sizes);
SWT_PUBLIC wresult w_coolitem_set_minimum_size(w_coolitem *coolitem,
		w_size *size);
SWT_PUBLIC wresult w_coolitem_set_order(w_coolitem *coolitem, int order);
SWT_PUBLIC wresult w_coolitem_set_preferred_size(w_coolitem *coolitem,
		w_size *size);
SWT_PUBLIC wresult w_coolitem_set_size(w_coolitem *coolitem, w_size *size);
SWT_PUBLIC wresult w_coolitem_set_wrap_indice(w_coolitem *coolitem,
		int wrap_indice);

#define W_COOLBAR_CLASS(x) ((struct _w_coolbar_class*)x)
#define W_COOLBAR_GET_CLASS(x) ((struct _w_coolbar_class*)W_WIDGET_GET_CLASS(x))
#define W_COOLITEM_CLASS(x) ((struct _w_coolitem_class*)x)
#define W_COOLITEM_GET_CLASS(x) ((struct _w_coolitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_COOLBAR_H_ */
