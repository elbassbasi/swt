/*
 * ccanvas.h
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */

#ifndef SRC_CUSTOM_WIDGETS_CCANVAS_H_
#define SRC_CUSTOM_WIDGETS_CCANVAS_H_
#include <swt.h>
#ifdef CSWTBUILD
#define CSWT_PUBLIC DLL_EXPORT
#else
#define CSWT_PUBLIC DLL_IMPORT
#endif
typedef struct wc_reserved {
    void* reserved;
    int state;
}wc_reserved;
#define WC_USED_INTERNAL_MEMORY (1 << 0)
#define WC_CLASS_RESERVED(clazz) ((wc_reserved*)((struct _w_widget_class*)clazz)->reserved)
typedef void (*_init_class)(struct _w_widget_class *clazz);
wresult wc_control_create(w_widget *widget, w_widget *parent, wuint64 style,
                           w_widget_post_event_proc post_event, size_t sizepriv);
void* wc_control_get_priv(w_control *c);
void* wc_control_alloc_priv(w_control *c, size_t size);
void wc_ccanvas_init_class(struct _w_widget_class *clazz);
void wc_ccanvas_init_class_priv(struct _w_widget_class *clazz,size_t sizeof_priv);

#endif /* SRC_CUSTOM_WIDGETS_CCANVAS_H_ */
