/*
 * scrollable.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_WIDGETS_SCROLLABLE_H_
#define SRC_COCOA_WIDGETS_SCROLLABLE_H_

#ifdef __APPLE__
#include "control.h"
#define STATE_SCROLLABLE_END (STATE_CONTROL_END + 0)
#ifndef __OBJC__
/*
 SWTScrollView
 */
typedef struct NSScrollView NSScrollView;
NSScrollView* NSScrollView_new(void* userdata);
int NSScrollView_isKindOfClass(NSView* view);
void NSScrollView_setDrawsBackground(NSScrollView* scrollView,int draw);
void NSScrollView_setHasHorizontalScroller(NSScrollView* scrollView,int scroller);
void NSScrollView_setHasVerticalScroller(NSScrollView* scrollView,int scroller);
void NSScrollView_setAutohidesScrollers(NSScrollView* scrollView,int autohide);
void NSScrollView_setBorderType(NSScrollView* scrollView,int border);
NSView* NSScrollView_contentView(NSScrollView* scrollView);
void NSScrollView_setDocumentView(NSScrollView* scrollView,NSView* view);
void NSScrollView_contentSizeForFrameSize(NSScrollView* scrollView,w_size* size,int hFlag,int vFlag,int type);
NSView* NSScrollView_documentView(NSScrollView* scrollView);
#endif
/*
 
 */
typedef struct _w_scrollable {
	struct _w_control control;
} _w_scrollable;

typedef struct _w_scrollbar {
	struct w_widgetdata widgetdata;
	w_scrollable *parent;
	int style;
} _w_scrollbar;

struct _w_scrollable_reserved {
	struct _w_control_reserved control;
    NSView* (*scroll_view)(w_widget* widget);

};
#define _W_SCROLLABLE(x) ((_w_scrollable*)x)
#define _W_SCROLLABLE_RESERVED(x) ((struct _w_scrollable_reserved*)x)
#define _W_SCROLLBAR(x) ((_w_scrollbar*)x)
/*
 * public function for scrollbar
 */
void _w_scrollbar_copy(w_widgetdata *from, w_widgetdata *to);
w_scrollable* _w_scrollbar_get_parent(w_scrollbar *scrollbar);
wresult _w_scrollbar_get_enabled(w_scrollbar *scrollbar);
wresult _w_scrollbar_is_enabled(w_scrollbar *scrollbar);
void _w_scrollbar_set_enabled(w_scrollbar *scrollbar, int enabled);
wresult _w_scrollbar_get_visible(w_scrollbar *scrollbar);
wresult _w_scrollbar_is_visible(w_scrollbar *scrollbar);
wresult _w_scrollbar_set_visible_0(w_scrollbar *scrollbar, int visible);
void _w_scrollbar_set_visible(w_scrollbar *scrollbar, int visible);
void _w_scrollbar_get_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values);
int _w_scrollbar_get_increment(w_scrollbar *scrollbar);
int _w_scrollbar_get_maximum(w_scrollbar *scrollbar);
int _w_scrollbar_get_minimum(w_scrollbar *scrollbar);
int _w_scrollbar_get_page_increment(w_scrollbar *scrollbar);
int _w_scrollbar_get_selection(w_scrollbar *scrollbar);
int _w_scrollbar_get_thumb(w_scrollbar *scrollbar);
void _w_scrollbar_set_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values);
void _w_scrollbar_set_increment(w_scrollbar *scrollbar, int value);
void _w_scrollbar_set_maximum(w_scrollbar *scrollbar, int value);
void _w_scrollbar_set_minimum(w_scrollbar *scrollbar, int minimum);
void _w_scrollbar_set_page_increment(w_scrollbar *scrollbar, int pageIncrement);
void _w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection);
void _w_scrollbar_set_thumb(w_scrollbar *scrollbar, int thumb);
void _w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size);
void _w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect);
void _w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar, w_rect *rect);
/*
 * public function for scrollable
 */
NSView* _w_scrollable_top_view(w_widget* widget);
NSView* _w_scrollable_get_view(w_widget* widget);
NSView* _w_scrollable_scroll_view(w_widget* widget);
void _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar);
int _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable);
void _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar);
/*
 * post event
 */
void _w_scrollable_class_init(struct _w_scrollable_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_SCROLLABLE_H_ */
