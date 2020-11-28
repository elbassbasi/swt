/*
 * tray.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_WIDGETS_TRAY_H_
#define SWT_WIDGETS_TRAY_H_
#include "widget.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_tray {
	w_widget widget;
} w_tray;

#define W_TRAY(x) ((w_tray*)x)
typedef struct w_trayitem {
	w_widget widget;
	void *handle[6];
} w_trayitem;
#define W_TRAYITEM(x) ((w_trayitem*)x)
struct _w_trayitem_class {
	struct _w_widget_class widget;
	w_tooltip* (*get_tooltip)(w_trayitem *trayitem);
	wresult (*get_tooltip_text)(w_trayitem *trayitem, w_alloc alloc,
			void *user_data);
	wbool (*get_visible)(w_trayitem *trayitem);
	wresult (*set_image)(w_trayitem *trayitem, w_image *image);
	wresult (*set_tooltip)(w_trayitem *trayitem, w_tooltip *toolTip);
	wresult (*set_tooltip_text)(w_trayitem *trayitem, const char *string);
	wresult (*set_visible)(w_trayitem *trayitem, wbool visible);
};
struct _w_tray_class {
	struct _w_widget_class widget;
	struct _w_trayitem_class *class_trayitem;
	wbool (*get_item)(w_tray *tray, int index, w_trayitem **item);
	int (*get_item_count)(w_tray *tray);
	void (*get_items)(w_tray *tray, w_iterator *items);
};
SWT_PUBLIC w_tooltip* w_trayitem_get_tooltip(w_trayitem *trayitem);
SWT_PUBLIC wresult w_trayitem_get_tooltip_text(w_trayitem *trayitem,
		w_alloc alloc, void *user_data);
SWT_PUBLIC wbool w_trayitem_get_visible(w_trayitem *trayitem);
SWT_PUBLIC wresult w_trayitem_set_image(w_trayitem *trayitem, w_image *image);
SWT_PUBLIC wresult w_trayitem_set_tooltip(w_trayitem *trayitem,
		w_tooltip *toolTip);
SWT_PUBLIC wresult w_trayitem_set_tooltip_text(w_trayitem *trayitem,
		const char *string);
SWT_PUBLIC wresult w_trayitem_set_visible(w_trayitem *trayitem, wbool visible);
/*
 *
 */
SWT_PUBLIC wbool w_tray_get_item(w_tray *tray, int index, w_trayitem **item);
SWT_PUBLIC int w_tray_get_item_count(w_tray *tray);
SWT_PUBLIC void w_tray_get_items(w_tray *tray, w_iterator *items);

#define W_TRAY_CLASS(x) ((struct _w_tray_class*)x)
#define W_TRAY_GET_CLASS(x) ((struct _w_tray_class*)W_WIDGET_GET_CLASS(x))
#define W_TRAYITEM_CLASS(x) ((struct _w_trayitem_class*)x)
#define W_TRAYITEM_GET_CLASS(x) ((struct _w_trayitem_class*)W_WIDGETDATA_GET_CLASS(x))

#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_TRAY_H_ */
