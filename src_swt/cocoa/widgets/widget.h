/*
 * widget.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_WIDGETS_WIDGET_H_
#define SRC_COCOA_WIDGETS_WIDGET_H_
#ifdef __APPLE__
#include "../graphics/graphics.h"
#ifndef __OBJC__
typedef struct NSEvent NSEvent;
typedef struct NSSet NSSet;
typedef struct NSView NSView;
typedef struct NSGraphicsContext NSGraphicsContext;
typedef struct NSPasteboard NSPasteboard;
typedef struct NSImage NSImage;
typedef struct NSBitmapImageRep NSBitmapImageRep;
typedef struct NSMenu NSMenu;
typedef struct NSWindow NSWindow;
typedef struct NSTrackingArea NSTrackingArea;
typedef struct NSClipView NSClipView;
#define NSEventMaskFromType(type)  (1UL << type)
#endif

wresult _w_widget_send_msg(w_widget *widget, void *handle, int msg, void *d1,
		void *d2, void *d3);
int _w_widget_update_cursor(w_widget *widget);
w_widget* NSWindow_getWidget(NSView *window);
w_widget* SWTView_getWidget(NSView *view);
w_widget* SWTOutlineView_get_widget(NSView *view);
w_widget* NSScrollView_getWidget(NSView *view);
w_widget* NSTableView_getWidget(NSView *view);

/*
 *  NSEvent
 */
long NSEvent_CGEvent(NSEvent *nsEvent);
long NSEvent_buttonNumber(NSEvent *nsEvent);
NSString* NSEvent_characters(NSEvent *nsEvent);
NSString* NSEvent_charactersIgnoringModifiers(NSEvent *nsEvent);
long NSEvent_clickCount(NSEvent *nsEvent);
double NSEvent_deltaX(NSEvent *nsEvent);
double NSEvent_deltaY(NSEvent *nsEvent);
double NSEvent_doubleClickInterval();
NSEvent* NSEvent_enterExitEventWithType(long type, w_point* location, long flags, double time, NSUInteger wNum, NSGraphicsContext* context, NSInteger eNum, NSInteger tNum, void* data);
short NSEvent_keyCode(NSEvent *nsEvent);
void NSEvent_locationInWindow(NSEvent *nsEvent,w_point *result);
double NSEvent_magnification(NSEvent *nsEvent);
long NSEvent_modifierFlags(NSEvent *nsEvent);
void NSEvent_mouseLocation(w_point *result);
NSEvent* NSEvent_otherEventWithType(long type, w_point* location, long flags, double time, NSInteger wNum, NSGraphicsContext *context, short subtype, NSInteger d1, NSInteger d2);
long NSEvent_phase(NSEvent *nsEvent);
NSUInteger NSEvent_pressedMouseButtons();
float NSEvent_rotation(NSEvent *nsEvent);
double NSEvent_timestamp(NSEvent *nsEvent);
NSSet* NSEvent_touchesMatchingPhase(NSEvent *nsEvent,long phase, NSView* view);
long NSEvent_type(NSEvent *nsEvent);
NSWindow* NSEvent_window(NSEvent *nsEvent);

/* Global state flags */
#define STATE_HANDLE (1 << 0)
#define STATE_OBSCURED (1 << 1)
#define STATE_WIDGET_END (1)

typedef w_widget* (*ns_get_widget)(NSView* view);
typedef void (*ns_init_class)(Class clazz);
enum {
    _NS_CLASS_VIEW,
    _NS_CLASS_BUTTON,
    _NS_CLASS_OUTLINEVIEW,
    _NS_CLASS_LAST
};
void _w_toolkit_registre_class(int _id,Class clazz);
enum {
    _NS_UNKNOWN = 0,
    /*
     VIEW
     */
    _NS_resignFirstResponder,
    _NS_becomeFirstResponder,
    _NS_resetCursorRects,
    _NS_updateTrackingAreas,
    _NS_mouseDownCanMoveWindow,
    _NS_mouseDown,
    _NS_mouseUp,
    _NS_scrollWheel,
    _NS_rightMouseDown,
    _NS_rightMouseUp,
    _NS_rightMouseDragged,
    _NS_otherMouseDown,
    _NS_otherMouseUp,
    _NS_otherMouseDragged,
    _NS_mouseDragged,
    _NS_mouseMoved,
    _NS_mouseEntered,
    _NS_mouseExited,
    _NS_menuForEvent,
    _NS_keyDown,
    _NS_keyUp,
    _NS_flagsChanged,
    _NS_cursorUpdate,
    _NS_setNeedsDisplay,
    _NS_shouldDelayWindowOrderingForEvent,
    _NS_acceptsFirstMouse,
    _NS_changeColor,
    _NS_cancelOperation,
    _NS_touchesBeganWithEvent,
    _NS_touchesMovedWithEvent,
    _NS_touchesEndedWithEvent,
    _NS_touchesCancelledWithEvent,
    _NS_swipeWithEvent,
    _NS_rotateWithEvent,
    _NS_magnifyWithEvent,
    _NS_setNeedsDisplayInRect,
    _NS_drawRect,
    _NS_hitTest,
    _NS_VIEW_LAST,
    /*

     */
    _NS_WILL_MOVE_TO_WINDOW =_NS_hitTest,
    _NS_outlineViewSelectionDidChange,
    _NS_outlineViewColumnDidMove,
    _NS_outlineViewColumnDidResize,
    _NS_outlineViewSelectionIsChanging,
    _NS_outlineViewItemWillExpand,
    _NS_outlineViewItemDidExpand,
    _NS_outlineViewItemWillCollapse,
    _NS_outlineViewItemDidCollapse,
    /*
     window
     */
    _NS_WINDOW_DID_BECOME_KEY,
    _NS_WINDOW_DID_DEMINITURIZE,
    _NS_WINDOW_DID_MINITURIZE,
    _NS_WINDOW_DID_MOVE,
    _NS_WINDOW_DID_RESIZE,
    _NS_WINDOW_DID_RESIGN_KEY,
    _NS_WINDOW_SEND_EVENT,
    _NS_WINDOW_WILL_CLOSE,
    _NS_WINDOW_SHOULD_CLOSE,
    _NS_LAST

};
typedef struct _w_event_platform {
	w_event event;
	int msg;
	void* result;
	NSView *handle;
	void *args[3];
} _w_event_platform;

typedef struct _w_widget {
	void *v_table; //used in c++ as virtual table
	struct _w_widget_class *clazz;
	volatile int ref;
	wuint _id;
	wuint64 style;
	NSView *handle;
	w_widget_post_event_proc post_event;
	void *data[5];
	wuint state;
} _w_widget;
typedef struct _w_widget_reserved _w_widget_reserved;
typedef wresult (*_w_msg)(w_widget *widget, _w_event_platform *e,_w_widget_reserved* reserved);
typedef struct _w_widget_reserved {
	NSView* (*topHandle)(w_widget *control, _w_widget_reserved *reserved);
	wresult (*compute_size)(w_widget *widget, w_event_compute_size *e,
			_w_widget_reserved *reserved);
	wresult (*get_client_area)(w_widget *widget, w_event_client_area *e,
			_w_widget_reserved *reserved);
	wresult (*compute_trim)(w_widget *widget, w_event_compute_trim *e,
			_w_widget_reserved *reserved);
	void (*hook_events)(w_widget *widget, _w_widget_reserved *reserved);
	void (*check_open)(w_widget *widget, _w_widget_reserved *reserved);
	void (*check_orientation)(w_widget *widget, w_widget *parent,
			_w_widget_reserved *reserved);
	wresult (*create_widget)(w_widget *widget, _w_widget_reserved *reserved);
	wresult (*create_handle)(w_widget *widget, _w_widget_reserved *reserved);
	void (*set_orientation)(w_widget *widget, int create,
			_w_widget_reserved *reserved);
	wuint64 (*check_style)(w_widget *control, wuint64 style);
	wresult (*set_tab_group_focus)(w_widget *widget, int next,
			_w_widget_reserved *reserved);
	wresult (*set_tab_item_focus)(w_widget *widget, int next,
			_w_widget_reserved *reserved);
	_w_msg msgs[_NS_LAST];
} _w_widget_reserved;
#define _W_WIDGET(x) ((_w_widget*)x)
#define _W_WIDGET_RESERVED(x) ((struct _w_widget_reserved*)x)
_w_widget_reserved* _w_widget_get_reserved(w_widget *widget);
wuint64 _w_widget_check_bits(wuint64 style, wuint64 int0, wuint64 int1, wuint64 int2, wuint64 int3,wuint64 int4, wuint64 int5);
wresult _w_widget_create_widget (w_widget *widget, _w_widget_reserved *reserved);
/*
 * public function
 */
w_widget_post_event_proc _w_widget_get_post_event(w_widget *widget);
w_widget_post_event_proc _w_widget_set_post_event(w_widget *widget,
		w_widget_post_event_proc post_event);
int _w_widget_send_event(w_widget *widget, w_event *event);
void _w_widget_set_id(w_widget *widget, int id);
int _w_widget_get_id(w_widget *widget);
int _w_widget_get_style(w_widget *widget);
void* _w_widget_get_data(w_widget *widget, int index);
void* _w_widget_set_data(w_widget *widget, int index, void *data);
/*
 * resource data public function
 */

wresult _w_widgetdata_is_ok(w_widgetdata *obj);
void _w_widgetdata_close(w_widgetdata *obj);
wresult _w_widget_is_ok(w_widget *widget);
void __w_resource_dispose(w_widget *widget);
void _w_widget_dispose(w_widget *widget);
wresult _w_widget_post_event(struct w_widget *widget, struct w_event *ee);

void _w_widget_class_init(struct _w_widget_class *clazz);

#endif
#endif /* SRC_GTK_WIDGETS_WIDGET_H_ */
