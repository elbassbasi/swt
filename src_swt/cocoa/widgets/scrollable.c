/*
 * scrollable.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "scrollable.h"
#include "toolkit.h"
/*
 * internal function
 */
NSView* _w_scrollable_top_view(w_widget* widget){
    return _W_WIDGET(widget)->handle;
}
NSView* _w_scrollable_get_view(w_widget* widget){
    if(NSScrollView_isKindOfClass(_W_WIDGET(widget)->handle)){
        return NSScrollView_documentView((NSScrollView*)_W_WIDGET(widget)->handle);
    }else{
        return _W_WIDGET(widget)->handle;
    }
}
NSView* _w_scrollable_scroll_view(w_widget* widget){
    if(NSScrollView_isKindOfClass(_W_WIDGET(widget)->handle)){
        return _W_WIDGET(widget)->handle;
    }else{
        return 0;
    }
}
/*
 * public function for scrollbar
 */
void _w_scrollbar_copy(w_widgetdata *from, w_widgetdata *to) {

}
w_scrollable* _w_scrollbar_get_parent(w_scrollbar *scrollbar) {
	return _W_SCROLLBAR(scrollbar)->parent;
}
wresult _w_scrollbar_get_enabled(w_scrollbar *scrollbar) {
}
wresult _w_scrollbar_is_enabled(w_scrollbar *scrollbar) {
}
void _w_scrollbar_set_enabled(w_scrollbar *scrollbar, int enabled) {
}
wresult _w_scrollbar_get_visible(w_scrollbar *scrollbar) {
}
wresult _w_scrollbar_is_visible(w_scrollbar *scrollbar) {
}
void _w_scrollbar_set_visible(w_scrollbar *scrollbar, int visible) {
}
void _w_scrollbar_get_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values) {
}
int _w_scrollbar_get_increment(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_maximum(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_minimum(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_page_increment(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_selection(w_scrollbar *scrollbar) {
}
int _w_scrollbar_get_thumb(w_scrollbar *scrollbar) {
}
void _w_scrollbar_set_values(w_scrollbar *scrollbar,
		struct w_scrollbar_value *values) {
}
void _w_scrollbar_set_increment(w_scrollbar *scrollbar, int value) {
	if (value < 1)
		return;
}
void _w_scrollbar_set_maximum(w_scrollbar *scrollbar, int value) {
}
void _w_scrollbar_set_minimum(w_scrollbar *scrollbar, int minimum) {
}
void _w_scrollbar_set_page_increment(w_scrollbar *scrollbar, int pageIncrement) {
}
void _w_scrollbar_set_selection(w_scrollbar *scrollbar, int selection) {
}
void _w_scrollbar_set_thumb(w_scrollbar *scrollbar, int thumb) {
}
void _w_scrollbar_get_size(w_scrollbar *scrollbar, w_size *size) {
}
void _w_scrollbar_get_thumb_bounds(w_scrollbar *scrollbar, w_rect *rect) {
}
void _w_scrollbar_get_thumb_track_bounds(w_scrollbar *scrollbar, w_rect *rect) {
}
/*
 * public function for scrollable
 */
void _w_scrollable_get_horizontal_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
}
int _w_scrollable_get_scrollbars_mode(w_scrollable *scrollable) {
	return W_NONE;
}
void _w_scrollable_get_vertical_bar(w_scrollable *scrollable,
		w_scrollbar *scrollbar) {
}
wresult _w_scrollable_get_client_area(w_widget *widget,
        w_event_client_area *e, _w_widget_reserved *reserved) {
    NSScrollView* scrollView =(NSScrollView*) _W_SCROLLABLE_RESERVED(reserved)->scroll_view(widget);
    if (scrollView != 0) {
        /*NSSize size = scrollView.contentSize();
        NSClipView contentView = scrollView.contentView();
        NSRect bounds = contentView.bounds();
        return new Rectangle((int)bounds.x, (int)bounds.y, (int)size.width, (int)size.height);*/
    } else {
        NSView* view = _W_CONTROL_RESERVED(reserved)->get_view(widget);
        NSView_bounds(view, e->rect);
        e->rect->x = 0;
        e->rect->y = 0;
    }
    return W_TRUE;
}
/*
 * signals
 */
void _w_scrollable_class_init(struct _w_scrollable_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	/*
	 * scrollable class
	 */
	clazz->get_horizontal_bar = _w_scrollable_get_horizontal_bar;
	clazz->get_scrollbars_mode = _w_scrollable_get_scrollbars_mode;
	clazz->get_vertical_bar = _w_scrollable_get_vertical_bar;
	/*
	 * scrollbar class
	 */
	clazz->class_scrollbar = &mac_toolkit->class_scrollbar;
	if (mac_toolkit->class_scrollbar.get_values == 0) {
		struct _w_scrollbar_class *scrollbar = &mac_toolkit->class_scrollbar;
		scrollbar->widgetdata.toolkit = (w_toolkit*) mac_toolkit;
		scrollbar->widgetdata.is_ok = _w_widgetdata_is_ok;
		scrollbar->widgetdata.close = _w_widgetdata_close;
		scrollbar->widgetdata.copy = _w_scrollbar_copy;
		scrollbar->get_parent = _w_scrollbar_get_parent;
		scrollbar->get_enabled = _w_scrollbar_get_enabled;
		scrollbar->is_enabled = _w_scrollbar_is_enabled;
		scrollbar->set_enabled = _w_scrollbar_set_enabled;
		scrollbar->get_visible = _w_scrollbar_get_visible;
		scrollbar->is_visible = _w_scrollbar_is_visible;
		scrollbar->set_visible = _w_scrollbar_set_visible;
		scrollbar->get_values = _w_scrollbar_get_values;
		scrollbar->get_increment = _w_scrollbar_get_increment;
		scrollbar->get_maximum = _w_scrollbar_get_maximum;
		scrollbar->get_minimum = _w_scrollbar_get_minimum;
		scrollbar->get_page_increment = _w_scrollbar_get_page_increment;
		scrollbar->get_selection = _w_scrollbar_get_selection;
		scrollbar->get_thumb = _w_scrollbar_get_thumb;
		scrollbar->set_values = _w_scrollbar_set_values;
		scrollbar->set_increment = _w_scrollbar_set_increment;
		scrollbar->set_maximum = _w_scrollbar_set_maximum;
		scrollbar->set_minimum = _w_scrollbar_set_minimum;
		scrollbar->set_page_increment = _w_scrollbar_set_page_increment;
		scrollbar->set_selection = _w_scrollbar_set_selection;
		scrollbar->set_thumb = _w_scrollbar_set_thumb;
		scrollbar->get_size = _w_scrollbar_get_size;
		scrollbar->get_thumb_bounds = _w_scrollbar_get_thumb_bounds;
		scrollbar->get_thumb_track_bounds = _w_scrollbar_get_thumb_track_bounds;
	}
	/*
	 * reserved
	 */
	struct _w_scrollable_reserved *reserved = _W_SCROLLABLE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
    _W_WIDGET_RESERVED(reserved)->get_client_area = _w_scrollable_get_client_area;
    _W_CONTROL_RESERVED(reserved)->get_view = _w_scrollable_get_view;
    _W_CONTROL_RESERVED(reserved)->top_view = _w_scrollable_top_view;
    _W_SCROLLABLE_RESERVED(reserved)->scroll_view =_w_scrollable_scroll_view;
}
#endif

