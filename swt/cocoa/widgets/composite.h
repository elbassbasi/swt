/*
 * composite.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_WIDGETS_COMPOSITE_H_
#define SRC_COCOA_WIDGETS_COMPOSITE_H_

#ifdef __APPLE__
#include "scrollable.h"
#define STATE_COMPOSITE_END (STATE_SCROLLABLE_END + 0)
/*
 SWTCanvasView
 */
/*
 
 */
typedef struct _w_composite {
	struct _w_scrollable scrollable;
	w_layout *layout;
	wushort layoutCount;
} _w_composite;

typedef struct _w_composite_reserved {
	struct _w_scrollable_reserved scrollable;
    w_composite* (*find_deferred_control)(w_composite *composite,
            struct _w_composite_reserved *reserved);
}_w_composite_reserved;
#define _W_COMPOSITE(x) ((_w_composite*)x)
#define _W_COMPOSITE_RESERVED(x) ((struct _w_composite_reserved*)x)
#define _W_COMPOSITE_GET_RESERVED(x) ((struct _w_composite_reserved*)_w_widget_get_reserved(W_WIDGET(x)))

typedef struct _w_composite_iterator {
	w_basic_iterator base;
	w_composite *composite;
    NSArray* views;
    size_t i;
    size_t count;
}_w_composite_iterator;
#define _W_COMPOSITE_ITERATOR(it) ((_w_composite_iterator*)it)
wresult _w_composite_create_0(w_widget *widget,int style,NSScrollView** scrollView,NSView** view);
wresult _w_composite_add_child(w_composite* composite,w_widget* child);
wresult _w_composite_create_handle(w_widget *widget, _w_widget_reserved *reserved);
/**
 *
 */
wresult _w_composite_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event);
wresult _w_composite_get_children(w_composite *composite, w_iterator *it);
wresult _w_composite_get_layout(w_composite *composite,w_layout** layout);
int _w_composite_get_layout_deferred(w_composite *composite);
void _w_composite_get_tab_list(w_composite *composite, w_iterator *it);
int _w_composite_is_layout_deferred(w_composite *composite);
void _w_composite_set_layout(w_composite *composite, w_layout *layout);
void _w_composite_set_layout_deferred(w_composite *composite, int defer);
/*
 * signals
 */
void _w_composite_class_init(struct _w_composite_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_COMPOSITE_H_ */
