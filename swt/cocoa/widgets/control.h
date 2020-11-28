/*
 * control.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_WIDGETS_CONTROL_H_
#define SRC_COCOA_WIDGETS_CONTROL_H_
#ifdef __APPLE__
#include "menu.h"
#define STATE_CANVAS (1 << (STATE_WIDGET_END + 1))
#define STATE_MENU (1 << (STATE_WIDGET_END + 2))
#define STATE_DRAG_DETECT (1 << (STATE_WIDGET_END + 3))
#define STATE_TIMER_START (1 << (STATE_WIDGET_END + 4))
#define STATE_THEME_BACKGROUND (1 << (STATE_WIDGET_END + 5))
#define STATE_CHECK_SUBWINDOW (1 << (STATE_WIDGET_END + 6))
#define STATE_LAYOUT_DATA_LOCALE (1 << (STATE_WIDGET_END + 7))
#define STATE_DRAW_REGION (1 << (STATE_WIDGET_END + 8))
#define STATE_ZERO_WIDTH  (1 << (STATE_WIDGET_END + 9))
#define STATE_ZERO_HEIGHT (1 << (STATE_WIDGET_END + 10))
#define STATE_HIDDEN (1 << (STATE_WIDGET_END + 11))
#define STATE_LAYOUT_NEEDED (1 << (STATE_WIDGET_END + 12))
#define STATE_LAYOUT_CHANGED (1 << (STATE_WIDGET_END + 13))
#define STATE_LAYOUT_CHILD (1 << (STATE_WIDGET_END + 14))
#define STATE_TAB_LIST (1 << (STATE_WIDGET_END + 15))
#define STATE_PARENT_BACKGROUND (1 << (STATE_WIDGET_END + 16))
#define STATE_CONTROL_END (STATE_WIDGET_END + 16)
/*
 
 */
/*
NSView
 */
NSView* SWTView_new(w_widget* widget);
w_widget* SWTView_get_widget(NSView* view);
w_widget* NSView_get_widget(NSView* view);
w_widget* NSView_get_widget_0(NSView* view,size_t* index);
int NSView_acceptsFirstMouse(NSView* view,NSEvent* theEvent);
void NSView_addSubview(NSView* view,NSView* aView);
void NSView_addSubview_0(NSView* view,NSView* aView, int place, NSView* otherView);
long NSView_addToolTipRect(NSView* view,w_rect* aRect, NSObject* anObject, void * data);
void NSView_beginDocument(NSView* view);
void NSView_beginPageInRect(NSView* view,w_rect* aRect, w_point* location);
NSBitmapImageRep* NSView_bitmapImageRepForCachingDisplayInRect(NSView* view,w_rect* aRect);
void NSView_bounds(NSView* view,w_rect* bounds);
void NSView_cacheDisplayInRect(NSView* view,w_rect* aRect, NSBitmapImageRep* bitmapImageRep);
int NSView_canBecomeKeyView(NSView* view);
void NSView_convertPoint_fromView_(NSView* view,w_point* result,w_point* aPoint, NSView* aView);
void NSView_convertPoint_toView_(NSView* view,w_point* result,w_point* aPoint, NSView* aView);
void NSView_convertRect_fromView_(NSView* view,w_rect* result,w_rect* aRect, NSView* aView);
void NSView_convertRect_toView_(NSView* view,w_rect* result,w_rect* aRect, NSView* aView);
void NSView_discardCursorRects(NSView* view);
void NSView_display(NSView* view);
void NSView_displayIfNeeded(NSView* view);
void NSView_displayRectIgnoringOpacity(NSView* view,w_rect* aRect, NSGraphicsContext* context);
void NSView_dragImage(NSView* view,NSImage* anImage, w_point* viewLocation, w_size* initialOffset, NSEvent* event, NSPasteboard* pboard, NSObject* sourceObj, int slideFlag);
void NSView_drawRect(NSView* view,w_rect* dirtyRect);
void NSView_endDocument(NSView* view);
void NSView_endPage(NSView* view);
void NSView_frame(NSView* view,w_rect* result);
NSView* NSView_hitTest(NSView* view,w_point* aPoint);
NSView* NSView_initWithFrame(NSView* view,w_rect* frameRect);
int NSView_isDescendantOf(NSView* view,NSView* aView);
int NSView_isFlipped(NSView* view);
int NSView_isHidden(NSView* view);
int NSView_isHiddenOrHasHiddenAncestor(NSView* view);
int NSView_isOpaque(NSView* view);
void NSView_lockFocus(NSView* view);
int NSView_lockFocusIfCanDraw(NSView* view);
NSMenu* NSView_menuForEvent(NSView* view,NSEvent* event);
int NSView_mouse(NSView* view,w_point* aPoint, w_rect* aRect);
int NSView_mouseDownCanMoveWindow(NSView* view);
int NSView_needsPanelToBecomeKey(NSView* view);
void NSView_registerForDraggedTypes(NSView* view,NSArray* newTypes);
void NSView_removeFromSuperview(NSView* view);
void NSView_removeToolTip(NSView* view,long tag);
void NSView_removeTrackingArea(NSView* view,NSTrackingArea* trackingArea);
void NSView_resetCursorRects(NSView* view);
void NSView_scrollClipView(NSView* view,NSClipView* aClipView, w_point* aPoint);
void NSView_scrollPoint(NSView* view,w_point* aPoint);
void NSView_scrollRect(NSView* view,w_rect* aRect, w_size* delta);
void NSView_setAcceptsTouchEvents(NSView* view,int acceptsTouchEvents);
void NSView_setAutoresizesSubviews(NSView* view,int autoresizesSubviews);
void NSView_setAutoresizingMask(NSView* view,int autoresizingMask);
void NSView_setBoundsRotation(NSView* view,double boundsRotation);
void NSView_setBoundsSize(NSView* view,w_size* newSize);
void NSView_setFocusRingType(NSView* view,long focusRingType);
void NSView_setFrame(NSView* view,w_rect* frame);
void NSView_setFrame_0(NSView* view,w_point* location,w_size* size);
void NSView_setFrameOrigin(NSView* view,w_point* newOrigin);
void NSView_setFrameSize(NSView* view,w_size* newSize);
void NSView_setHidden(NSView* view,int hidden);
void NSView_setNeedsDisplay(NSView* view,int needsDisplay);
void NSView_setNeedsDisplayInRect(NSView* view,w_rect* invalidRect);
void NSView_setToolTip(NSView* view,const char* toolTip);
void NSView_setWantsRestingTouches(NSView* view,int wantsRestingTouches);
int NSView_shouldDelayWindowOrderingForEvent(NSView* view,NSEvent* theEvent);
NSArray* NSView_subviews(NSView* view);
NSView* NSView_superview(NSView* view);
NSArray* NSView_trackingAreas(NSView* view);
void NSView_unlockFocus(NSView* view);
void NSView_unregisterDraggedTypes(NSView* view);
void NSView_updateTrackingAreas(NSView* view);
void NSView_viewDidMoveToWindow(NSView* view);
void NSView_viewWillMoveToWindow(NSView* view,NSWindow* newWindow);
void NSView_visibleRect(NSView* view,w_rect* result);
NSWindow* NSView_window(NSView* view);

typedef struct _w_control {
	struct _w_widget widget;
	w_cursor *cursor;
	w_menu* menu;
    w_composite* parent;
} _w_control;
typedef struct _w_control_reserved _w_control_reserved;
struct _w_control_reserved {
	struct _w_widget_reserved widget;
    NSView* (*get_view)(w_widget* widget);
    NSView* (*top_view)(w_widget* widget);
    NSView* (*content_view)(w_widget* widget,_w_control_reserved* reserved);
    w_cursor* (*find_cursor) (w_widget* widget,_w_control_reserved* reserved);
    void (*mark_layout)(w_control *control, int flags,
            _w_control_reserved *reserved);
    void (*update_layout)(w_control *control, int flags,
            _w_control_reserved *reserved);
    void (*check_orientation) (w_control *control,w_composite* parent,_w_control_reserved* reserved);
    void (*check_background) (w_control *control,_w_control_reserved* reserved);
    void (*check_buffered) (w_control *control,_w_control_reserved* reserved);
    void (*set_default_font) (w_control *control,_w_control_reserved* reserved);
    void (*set_zorder) (w_control *control,_w_control_reserved* reserved);
    void (*set_relations) (w_control *control,_w_control_reserved* reserved);
    void (*set_background) (w_control *control,_w_control_reserved* reserved);
};
#define _W_CONTROL(x) ((_w_control*)x)
#define _W_CONTROL_RESERVED(x) ((struct _w_control_reserved*)x)
#define _W_CONTROL_GET_RESERVED(x) ((struct _w_control_reserved*)_w_widget_get_reserved(W_WIDGET(x)))

/*
 * internal function
 */
NSView* _w_control_get_handle(w_widget* widget);
w_cursor* _w_control_find_cursor (w_widget* widget,_w_control_reserved* reserved);
/*
 *  function
 */
wresult _w_control_get_graphics(w_control *control, w_graphics *gc);
wresult _w_control_get_orientation(w_control *control);
wresult _w_control_set_orientation(w_control *control, int orientation);
wresult _w_control_get_text_direction(w_control *control);
wresult _w_control_set_text_direction(w_control *control, int textDirection);
wresult _w_control_print(w_control *control, w_graphics *gc);
wresult _w_control_get_accessible(w_control *control, w_accessible **accessible);
wresult _w_control_get_bounds(w_control *control,w_point* location,w_size* size);
wresult _w_control_set_bounds(w_control *control,w_point* location,w_size* size);
wresult _w_control_get_region(w_control *control, w_region *region);
wresult _w_control_set_region(w_control *control, w_region *region);
wresult _w_control_move_above(w_control *control, w_control *_control);
wresult _w_control_move_below(w_control *control, w_control *_control);
wresult _w_control_pack(w_control *control, wbool changed);
wresult _w_control_to_control(w_control *control, w_point *result, w_point *point);
wresult _w_control_to_display(w_control *control, w_point *result, w_point *point);
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event);
wresult _w_control_get_drag_detect(w_control *control);
wresult _w_control_set_drag_detect(w_control *control, int dragDetect);
wresult _w_control_set_focus(w_control *control);
wresult _w_control_force_focus(w_control *control);
wresult _w_control_is_focus_control(w_control *control);
wresult _w_control_get_border_width(w_control *control);
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor);
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor);
wresult _w_control_get_enabled(w_control *control);
wresult _w_control_set_enabled(w_control *control, int enabled);
wresult _w_control_is_enabled(w_control *control);
wresult _w_control_get_font(w_control *control, w_font **font);
wresult _w_control_set_font(w_control *control, w_font *font);
wresult _w_control_get_background(w_control *control, w_color *background);
wresult _w_control_set_background(w_control *control, w_color color);
wresult _w_control_get_foreground(w_control *control, w_color *foreground);
wresult _w_control_set_foreground(w_control *control, w_color color);
wresult _w_control_get_layout_data(w_control *control, void **data);
wresult _w_control_new_layout_data(w_control *control, void **data, size_t size);
wresult _w_control_request_layout(w_control *control);
wresult _w_control_get_menu(w_control *control, w_menu **menu);
wresult _w_control_set_menu(w_control *control, w_menu *menu);
wresult _w_control_get_parent(w_control *control, w_composite **parent);
wresult _w_control_set_parent(w_control *control, w_composite *parent);
wresult _w_control_is_reparentable(w_control *control);
wresult _w_control_get_shell(w_control *control, w_shell **shell);
wresult _w_control_get_toolTip_text(w_control *control,w_alloc alloc,void* user_data);
wresult _w_control_set_toolTip_text(w_control *control, const char *text,size_t length);
wresult _w_control_get_touch_enabled(w_control *control);
wresult _w_control_set_touch_enabled(w_control *control, int enabled);
wresult _w_control_get_visible(w_control *control);
wresult _w_control_set_visible(w_control *control, wbool visible);
wresult _w_control_is_visible(w_control *control);
wresult _w_control_redraw(w_control *control, w_rect *rect, int all);
wresult _w_control_set_redraw(w_control *control, int redraw);
wresult _w_control_set_capture(w_control *control, int capture);
wresult _w_control_traverse(w_control *control, int traversal, w_event_key *event);
wresult _w_control_update(w_control *control);
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id);
wresult _w_control_kill_timer(w_control *control, wushort id);
wresult _w_control_set_tab(w_control *control, wbool tab);
wresult _w_control_get_tab(w_control *control);
wresult _w_control_create_dragsource_effect(w_control *control,
        w_dragsource_effect *effect);
wresult _w_control_create_droptarget_effect(w_control *control,
        w_droptarget_effect *effect);
/*
 * signals
 */
int _w_control_post_event(w_widget *widget, w_event *e);
void _w_control_class_init(struct _w_control_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_CONTROL_H_ */
