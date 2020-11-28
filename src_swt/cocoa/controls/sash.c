/*
 * sash.c
 *
 *  Created on: 8 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "sash.h"
#include "../widgets/toolkit.h"
#define INCREMENT 1
#define  PAGE_INCREMENT 9
int _w_sash_check_style(int style) {
    style |= W_SMOOTH;
	style = _w_widget_check_bits(style, W_HORIZONTAL, W_VERTICAL, 0, 0, 0, 0);
	return style;
}
wresult _w_sash_set_cursor(w_control *control, w_cursor *cursor) {
	if (cursor == 0) {
		w_toolkit *toolkit = w_widget_get_toolkit(W_WIDGET(control));
		int cursor_id;
		if (_W_WIDGET(control)->style & W_HORIZONTAL) {
			cursor_id = W_CURSOR_SIZENS;
		} else {
			cursor_id = W_CURSOR_SIZEWE;
		}
		cursor = w_toolkit_get_system_cursor(toolkit, cursor_id);
	}
	return _w_control_set_cursor(control, cursor);
}
wresult _w_sash_create_handle(w_widget *widget, _w_widget_reserved *reserved){
    _W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
    _W_WIDGET(widget)->handle = (NSView*)SWTView_new(widget);
    return W_TRUE;
}
w_cursor* _w_sash_find_cursor(w_widget* widget,_w_control_reserved* reserved){
    w_cursor* cursor = _w_control_find_cursor(widget, reserved);
    if (cursor == 0)    {
        int cursorType = (_W_WIDGET(widget)->style & W_HORIZONTAL) != 0 ? W_CURSOR_SIZENS : W_CURSOR_SIZEWE;
        cursor = w_toolkit_get_system_cursor(W_TOOLKIT(mac_toolkit), cursorType);
    }
    return cursor;
}
wresult _w_sash_becomeFirstResponder (w_widget *widget, _w_event_platform *e,_w_widget_reserved* reserved) {
    //boolean result = super.becomeFirstResponder(id, sel);
    w_rect r;
    NSView_frame(_W_WIDGET(widget)->handle, &r);
    _W_SASH(widget)->lastX = (int)r.x;
    _W_SASH(widget)->lastY = (int)r.y;
    //return result;
    return W_FALSE;
}
wresult _w_sash_mouseDown(w_widget *widget, _w_event_platform *e,_w_widget_reserved* reserved) {
    //TODO use sendMouseEvent
    /*super.mouseDown(id, sel, theEvent);
    if (isDisposed()) return;*/
    NSEvent* nsEvent = (NSEvent*)e->args[0];
    if (NSEvent_clickCount(nsEvent) != 1) return W_FALSE;
    w_point location,point;
    NSEvent_locationInWindow(nsEvent, &location);
    NSView_convertPoint_fromView_(_W_WIDGET(widget)->handle, &location, 0, &point);
    _W_SASH(widget)->startX = point.x;
    _W_SASH(widget)->startY = point.y;
    w_rect frame;
    NSView_frame(_W_WIDGET(widget)->handle, &frame);
    w_event_sash event;
    event.event.type = W_EVENT_SELECTION;
    event.event.widget = widget;
    event.event.data = 0;
    event.event.platform_event = (w_event_platform*) e;
    event.event.time = 0;
    event.bounds.x = frame.x;
    event.bounds.y = frame.y;
    event.bounds.width = frame.width;
    event.bounds.height = frame.height;
    
    int doit = _w_widget_send_event(widget, (w_event*) &event);
    //if (isDisposed ()) return 0;
    if (doit) {
        _W_WIDGET(widget)->state |= STATE_SASH_DRAGGING;
        _W_SASH(widget)->lastX = event.bounds.x;
        _W_SASH(widget)->lastY = event.bounds.y;
        _w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt, 0);
    }
}

/*boolean _w_sash_mouseEvent (long id, long sel, long theEvent, int type) {
    super.mouseEvent (id, sel, theEvent, type);
    return new NSEvent (theEvent).type () != OS.NSLeftMouseDown;
}*/

wresult _w_sash_mouseDragged(w_widget *widget, _w_event_platform *e,_w_widget_reserved* reserved) {
    //TODO use sendMouseEvent
    /*super.mouseDragged(id, sel, theEvent);
    if (isDisposed()) return;*/
    if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)) return W_FALSE;
    NSEvent* nsEvent = (NSEvent*)e->args[0];
    w_point location,point;
    NSEvent_locationInWindow(nsEvent, &location);
    NSView_convertPoint_fromView_(_W_WIDGET(widget)->handle, &location, 0, &point);
    w_rect frame;
    NSView_frame(_W_WIDGET(widget)->handle, &frame);
    /*NSRect parentFrame = parent.topView().frame();
    int newX = lastX, newY = lastY;
    if ((style & SWT.VERTICAL) != 0) {
        newX = Math.min (Math.max (0, (int)(point.x + frame.x - startX)), (int)(parentFrame.width - frame.width));
    } else {
        newY = Math.min (Math.max (0, (int)(point.y + frame.y - startY)), (int)(parentFrame.height - frame.height));
    }
    if (newX == lastX && newY == lastY) return;
    Event event = new Event ();
    event.x = newX;
    event.y = newY;
    event.width = (int)frame.width;
    event.height = (int)frame.height;
    sendSelectionEvent (SWT.Selection, event, true);
    if (isDisposed ()) return;
    if (event.doit) {
        lastX = event.x;
        lastY = event.y;
        setBounds (event.x, event.y, (int)frame.width, (int)frame.height);
    }*/
}

wresult _w_sash_mouseUp(w_widget *widget, _w_event_platform *e,_w_widget_reserved* reserved) {
    //TODO use sendMouseEvent
    /*super.mouseUp(id, sel, theEvent);
    if (isDisposed()) return;*/
    if (!(_W_WIDGET(widget)->state & STATE_SASH_DRAGGING)) return W_FALSE;
    _W_WIDGET(widget)->state &= ~STATE_SASH_DRAGGING;
    
    w_rect frame;
    NSView_frame(_W_WIDGET(widget)->handle, &frame);
    w_event_sash event;
    event.event.type = W_EVENT_SELECTION;
    event.event.widget = widget;
    event.event.data = 0;
    event.event.platform_event = (w_event_platform*) e;
    event.event.time = 0;
    event.bounds.x = _W_SASH(widget)->lastX;
    event.bounds.y = _W_SASH(widget)->lastY;
    event.bounds.width = frame.width;
    event.bounds.height = frame.height;
    
    int doit = _w_widget_send_event(widget, (w_event*) &event);
    //if (isDisposed ()) return 0;
    if (doit) {
        event.bounds.width = frame.width;
        event.bounds.height = frame.height;
        _w_control_set_bounds(W_CONTROL(widget), &event.bounds.pt,&event.bounds.sz);
    }
}

void _w_sash_class_init(struct _w_sash_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SASH;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_sash_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_sash);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_sash);

	struct _w_sash_reserved *reserved = _W_SASH_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
    _W_WIDGET_RESERVED(reserved)->create_handle = _w_sash_create_handle;
    _W_CONTROL_RESERVED(reserved)->find_cursor = _w_sash_find_cursor;
    /*
     *  messages
     */
    _w_msg* msgs = _W_WIDGET_RESERVED(reserved)->msgs;
    msgs[_NS_becomeFirstResponder] = _w_sash_becomeFirstResponder;
    msgs[_NS_mouseDown] = _w_sash_mouseDown;
    msgs[_NS_mouseDragged] = _w_sash_mouseDragged;
    msgs[_NS_mouseUp] = _w_sash_mouseUp;
}

#endif
