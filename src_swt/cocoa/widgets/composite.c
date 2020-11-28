/*
 * composite.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "composite.h"
#include "toolkit.h"
wresult _w_composite_create_handle(w_widget *widget, _w_widget_reserved *reserved){
    _W_WIDGET(widget)->state |= STATE_CANVAS;
    wbool scrolled = (_W_WIDGET(widget)->style & (W_VSCROLL | W_HSCROLL)) != 0;
    if (!scrolled)  _W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
    NSScrollView* scrollView = 0;
        if (scrolled /*|| _W_CONTROL_RESERVED(reserved)->has_border()*/) {
            scrollView = NSScrollView_new(widget);
            NSScrollView_setDrawsBackground(scrollView,W_FALSE);
            if ((_W_WIDGET(widget)->style & W_HSCROLL) != 0) NSScrollView_setHasHorizontalScroller(scrollView,W_TRUE);
            if ((_W_WIDGET(widget)->style & W_VSCROLL) != 0) NSScrollView_setHasVerticalScroller(scrollView,W_TRUE);
            NSScrollView_setBorderType(scrollView,(_W_WIDGET(widget)->style & W_BORDER) != 0 ? NSBezelBorder : NSNoBorder);
        }
    NSView* view = SWTView_new(widget);
    NSView_initWithFrame(view, 0);
    NSView_setFocusRingType(view,NSFocusRingTypeExterior);
    if (scrollView != 0) {
        NSView* contentView =NSScrollView_contentView(scrollView);
        NSView_setAutoresizesSubviews(contentView, W_TRUE);
        NSView_setAutoresizingMask((NSView*)view, NSViewWidthSizable | NSViewHeightSizable);
        NSScrollView_setDocumentView(scrollView, view);
        _W_WIDGET(widget)->handle = (NSView*) scrollView;
    }else{
        _W_WIDGET(widget)->handle = view;
    }
}
wresult _w_composite_create_0(w_widget *widget,int style,NSScrollView** scrollView,NSView** view) {
        wbool scrolled = (style & (W_VSCROLL | W_HSCROLL)) != 0;
        if (!scrolled)  _W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
        if (scrolled ||  (style & W_BORDER) != 0 ) {
            NSScrollView* scrollWidget = NSScrollView_new(widget);
            NSScrollView_setDrawsBackground(scrollWidget,W_FALSE);
            if ((style & W_HSCROLL) != 0) NSScrollView_setHasHorizontalScroller(scrollWidget,W_TRUE);
            if ((style & W_VSCROLL) != 0) NSScrollView_setHasVerticalScroller(scrollWidget,W_TRUE);
            NSScrollView_setBorderType(scrollWidget,(style & W_BORDER) != 0 ? NSBezelBorder : NSNoBorder);
            *scrollView = scrollWidget;
        }
        NSView* _widget = SWTView_new(widget);
        NSView_setFocusRingType((NSView*)_widget,NSFocusRingTypeExterior);
        *view = _widget;
        if (*scrollView != 0) {
            NSView* contentView =NSScrollView_contentView(*scrollView);
            NSView_setAutoresizesSubviews(contentView, W_TRUE);
            NSView_setAutoresizingMask((NSView*)*view, NSViewWidthSizable | NSViewHeightSizable);
            NSScrollView_setDocumentView(*scrollView, (NSView*)_widget);
        }
    return W_TRUE;
}
wresult _w_composite_add_child(w_composite* composite,w_widget* child){
    _w_widget_reserved* reserved = _w_widget_get_reserved(W_WIDGET(composite));
    NSView* view = _W_CONTROL_RESERVED(reserved)->get_view(W_WIDGET(composite));
    NSView_addSubview(view, _W_WIDGET(child)->handle);
}
wresult _w_composite_set_bounds(w_control *control,w_point* location,w_size* size) {
    wresult ret = _w_control_set_bounds(control, location,size);
    if (size != 0) {
        w_event _e;
        _e.type = W_EVENT_LAYOUTDETECT;
        _e.platform_event = 0;
        _e.widget = W_WIDGET(control);
        _e.data = 0;
        _w_widget_send_event(W_WIDGET(control), &_e);
        return W_TRUE;
    }
    return ret;
}
/*
 * public function
 */
wresult _w_composite_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
        _W_WIDGET(widget)->state |= STATE_CANVAS;
    NSScrollView* scrollView=0;
    NSView* view=0;
    wresult ret =_w_composite_create_0(widget, style, &scrollView, &view);
    if(ret < 0) return ret;
    _W_WIDGET(widget)->post_event = post_event;
    _W_WIDGET(widget)->style = style;
    if(scrollView!=0){
        _W_WIDGET(widget)->handle = (NSView*)scrollView;
    }else{
        _W_WIDGET(widget)->handle = (NSView*)view;
    }
    return ret;
}
wbool _w_composite_iterator_next(w_iterator *it, void *obj) {
    _w_composite_iterator* _it =(_w_composite_iterator*) it;
    *((w_control**) obj) = 0;
    if(_it->i >= _it->count)
        return W_FALSE;
    NSView* view = (NSView*)NSArray_objectAtIndex(_it->views, _it->i);
    _it->i++;
    if(view == 0)
        return W_FALSE;
    *((w_control**) obj) =(w_control*) NSView_get_widget(view);
    if(*((w_control**) obj) == 0) return W_FALSE;
    return W_TRUE;
}
wresult _w_composite_iterator_reset(w_iterator *it) {
    _W_COMPOSITE_ITERATOR(it)->i = 0;
	return W_TRUE;
}
wresult _w_composite_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_composite_iterator_remove(w_iterator *it) {
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_composite_get_count(w_iterator *it) {
    return NSArray_count(_W_COMPOSITE_ITERATOR(it)->views);
}
_w_iterator_class _w_composite_iterator_class = { //
		_w_composite_iterator_close, //
				_w_composite_iterator_next, //
				_w_composite_iterator_reset, //
				_w_composite_iterator_remove, //
				_w_composite_get_count };
wresult _w_composite_get_children(w_composite *composite, w_iterator *it) {
    _w_widget_reserved* reserved = _w_widget_get_reserved(W_WIDGET(composite));
    NSView* view = _W_CONTROL_RESERVED(reserved)->get_view(W_WIDGET(composite));
    it->base.clazz = &_w_composite_iterator_class;
    _W_COMPOSITE_ITERATOR(it)->composite = composite;
    _W_COMPOSITE_ITERATOR(it)->views = NSView_subviews(view);
    _W_COMPOSITE_ITERATOR(it)->i = 0;
    _W_COMPOSITE_ITERATOR(it)->count = NSArray_count(_W_COMPOSITE_ITERATOR(it)->views);
}
wresult _w_composite_get_layout(w_composite *composite,w_layout** layout) {
	*layout = _W_COMPOSITE(composite)->layout;
    return W_TRUE;
}
int _w_composite_get_layout_deferred(w_composite *composite) {
	return _W_COMPOSITE(composite)->layoutCount > 0;
}
void _w_composite_get_tab_list(w_composite *composite, w_iterator *it) {
}
int _w_composite_is_layout_deferred(w_composite *composite) {
}
void _w_composite_set_layout(w_composite *composite, w_layout *layout) {
	_W_COMPOSITE(composite)->layout = layout;
}
void _w_composite_set_layout_deferred(w_composite *composite, int defer) {
	if (!defer) {
		_W_COMPOSITE(composite)->layoutCount--;
		if (_W_COMPOSITE(composite)->layoutCount == 0) {
			/*if ((_W_WIDGET(composite)->state & STATE_LAYOUT_CHILD) != 0
			 || (_W_WIDGET(composite)->state & STATE_LAYOUT_NEEDED)
			 != 0) {*/
			//updateLayout(true);
			/*	}*/
		}
	} else {
		_W_COMPOSITE(composite)->layoutCount++;
	}
}
void _w_composite_minimum_size(w_composite *composite, w_size *size, int wHint,
		int hHint, wbool changed) {
	w_iterator children;
	w_iterator_init(&children);
	w_composite_get_children(composite, &children);
	/*
	 * Since getClientArea can be overridden by subclasses, we cannot
	 * call getClientAreaInPixels directly.
	 */
	w_rect clientArea, rect;
	w_scrollable_get_client_area(W_SCROLLABLE(composite), &clientArea);
	int width = 0, height = 0;
	w_control *child = 0;
	while (w_iterator_next(&children, &child)) {
		if (child != 0) {
            w_control_get_bounds(child, &rect.pt,&rect.sz);
			width = w_max(width, rect.x - clientArea.x + rect.width);
			height = w_max(height, rect.y - clientArea.y + rect.height);
		}
	}
	size->width = width;
	size->height = height;
}
w_composite* _w_composite_find_deferred_control(w_composite *composite,
        _w_composite_reserved *reserved) {
    if ( _W_COMPOSITE(composite)->layoutCount > 0) {
        return composite;
    } else {
        w_composite *parent = _W_CONTROL(composite)->parent;
        _w_composite_reserved *preserved = _W_COMPOSITE_RESERVED(
                _w_widget_get_reserved(W_WIDGET(parent)));
        return preserved->find_deferred_control(parent, reserved);
    }
}
void _w_composite_mark_layout(w_control *_this, int flags,
        _w_control_reserved *reserved) {
    //if (layout != null) {
        _W_WIDGET(_this)->state |= STATE_LAYOUT_NEEDED;
        if (flags & W_CHANGED) _W_WIDGET(_this)->state |= STATE_LAYOUT_CHANGED;
    //}
    if (flags & W_ALL) {
        NSView* view = _W_CONTROL_RESERVED(reserved)->get_view(W_WIDGET(_this));
        NSArray *views = NSView_subviews(view);
        size_t count = NSArray_count(views);
        w_control *child;
        for (size_t i=0;i<count;i++) {
            view =(NSView*) NSArray_objectAtIndex(views, i);
            child = (w_control*)NSView_get_widget(view);
            if(child != 0){
            _w_control_reserved* creserved = _W_CONTROL_GET_RESERVED(child);
            creserved->mark_layout(child,flags,creserved);
            }
        }
    }
}
void _w_composite_update_layout(w_control *_this, int flags,
        _w_control_reserved *reserved) {
    w_composite *parent =
    _W_COMPOSITE_RESERVED(reserved)->find_deferred_control(W_COMPOSITE(_this),
            _W_COMPOSITE_RESERVED(reserved));
    if (parent != 0) {
        _W_WIDGET(parent)->state |= STATE_LAYOUT_CHILD;
        return;
    }
    if ((_W_WIDGET(_this)->state & STATE_LAYOUT_NEEDED) != 0) {
        wbool changed = (_W_WIDGET(_this)->state & STATE_LAYOUT_CHANGED) != 0;
        _W_WIDGET(_this)->state &=
                ~(STATE_LAYOUT_NEEDED | STATE_LAYOUT_CHANGED);
        //display.runSkin();
        //layout.layout(_this, changed);
        w_event _e;
        _e.type = W_EVENT_LAYOUTDETECT;
        _e.platform_event = 0;
        _e.widget = W_WIDGET(_this);
        _e.data = 0;
        _w_widget_send_event(W_WIDGET(_this), &_e);
    }
    if (flags & W_ALL) {
        _W_WIDGET(_this)->state &= ~STATE_LAYOUT_CHILD;
        NSView* view = _W_CONTROL_RESERVED(reserved)->get_view(W_WIDGET(_this));
        NSArray *views = NSView_subviews(view);
        size_t count = NSArray_count(views);
        w_control *child;
        for (size_t i=0;i<count;i++) {
            view =(NSView*) NSArray_objectAtIndex(views, i);
            child = (w_control*)NSView_get_widget(view);
            if(child != 0){
            _w_control_reserved* creserved = _W_CONTROL_GET_RESERVED(child);
            creserved->update_layout(child,flags,creserved);
            }
        }
    }
}
wresult _w_composite_compute_size(w_widget *widget, w_event_compute_size *e,
        _w_widget_reserved *reserved) {
    if (_W_COMPOSITE(widget)->layout != 0) {
        if ((e->wHint == W_DEFAULT) || (e->hHint == W_DEFAULT)) {
            //changed |= (_W_WIDGET(widget)->state & STATE_LAYOUT_CHANGED) != 0;
            w_layout_compute_size(_W_COMPOSITE(widget)->layout, W_COMPOSITE(widget), e->size, e->wHint, e->hHint, W_TRUE/*changed*/);
            _W_WIDGET(widget)->state &= ~STATE_LAYOUT_CHANGED;
        } else {
            e->size->width = e->wHint;
            e->size->height = e->hHint;
        }
    } else {
        /*size = minimumSize (wHint, hHint, changed);
        if (size.x == 0) size.x = DEFAULT_WIDTH;
        if (size.y == 0) size.y = DEFAULT_HEIGHT;*/
    }
    if (e->wHint != W_DEFAULT) e->size->width = e->wHint;
    if (e->hHint != W_DEFAULT) e->size->height = e->hHint;
    //Rectangle trim = computeTrim (0, 0, size.x, size.y);
    return W_TRUE;
}
/*
 * signals
 */
void _w_composite_class_init(struct _w_composite_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_composite_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_composite);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_composite);
	/*
	 * composite class
	 */
    W_CONTROL_CLASS(clazz)->set_bounds = _w_composite_set_bounds;
	clazz->get_children = _w_composite_get_children;
	clazz->get_layout = _w_composite_get_layout;
	clazz->get_layout_deferred = _w_composite_get_layout_deferred;
	clazz->get_tab_list = _w_composite_get_tab_list;
	clazz->is_layout_deferred = _w_composite_is_layout_deferred;
	clazz->set_layout = _w_composite_set_layout;
	clazz->set_layout_deferred = _w_composite_set_layout_deferred;
	/*
	 * reserved
	 */
	struct _w_composite_reserved *reserved = _W_COMPOSITE_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
    _W_WIDGET_RESERVED(reserved)->create_handle = _w_composite_create_handle;
    _W_WIDGET_RESERVED(reserved)->compute_size =
            _w_composite_compute_size;
    _W_CONTROL_RESERVED(reserved)->mark_layout = _w_composite_mark_layout;
    _W_CONTROL_RESERVED(reserved)->update_layout = _w_composite_update_layout;
    reserved->find_deferred_control = _w_composite_find_deferred_control;
}
#endif

