//
//  control_m.m
//  swt
//
//  Created by elbassbasi on 7/29/20.
//
#import "widget.h"
@interface SWTView : NSView{
@public w_widget* widget;
}
@end
@implementation SWTView
-(void)mouseDown:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_mouseDown, event, 0, 0) ==0 ){
         [super mouseDown:event];
    }
}
-(void)mouseUp:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_mouseUp, event, 0, 0) ==0 ){
         [super mouseUp:event];
    }
}
-(void)scrollWheel:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_scrollWheel, event, 0, 0) ==0 ){
         [super scrollWheel:event];
    }
}
-(void)rightMouseDown:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_rightMouseDown, event, 0, 0) ==0 ){
         [super rightMouseDown:event];
    }
}
-(void)rightMouseUp:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_rightMouseUp, event, 0, 0) ==0 ){
         [super rightMouseUp:event];
    }
}
-(void)rightMouseDragged:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_rightMouseDragged, event, 0, 0) ==0 ){
         [super rightMouseDragged:event];
    }
}
-(void)otherMouseDown:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_otherMouseDown, event, 0, 0) ==0 ){
         [super otherMouseDown:event];
    }
}
-(void)otherMouseUp:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_otherMouseUp, event, 0, 0) ==0 ){
         [super otherMouseUp:event];
    }
}
-(void)otherMouseDragged:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_otherMouseDragged, event, 0, 0) ==0 ){
         [super otherMouseDragged:event];
    }
}
-(void)mouseDragged:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_mouseDragged, event, 0, 0) ==0 ){
         [super mouseDragged:event];
    }
}
-(void)mouseMoved:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_mouseMoved, event, 0, 0) ==0 ){
         [super mouseMoved:event];
    }
}
-(void)mouseEntered:(NSEvent *)event{
    _w_widget_update_cursor(widget);
    if(_w_widget_send_msg(widget, self, _NS_mouseEntered, event, 0, 0) ==0 ){
         [super mouseEntered:event];
    }
}
-(void)mouseExited:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_mouseExited, event, 0, 0) ==0 ){
         [super mouseExited:event];
    }
}
- (nullable NSMenu *)menuForEvent:(NSEvent *)event{
    NSMenu * menu = nil;
    if(_w_widget_send_msg(widget, self, _NS_menuForEvent, event, &menu, 0) == 0){
        return [super menuForEvent:event];
    }else return menu;
}
-(void)keyDown:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_keyDown, event, 0, 0) ==0 ){
         [super keyDown:event];
    }
}
-(void)keyUp:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_keyUp, event, 0, 0) ==0 ){
         [super keyUp:event];
    }
}
-(void)flagsChanged:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_flagsChanged, event, 0, 0) ==0 ){
         [super flagsChanged:event];
    }
}
-(void)cursorUpdate:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_cursorUpdate, event, 0, 0) ==0 ){
         [super cursorUpdate:event];
    }
}
-(void)setNeedsDisplay:(BOOL)display{
    _w_widget_send_msg(widget, self, _NS_setNeedsDisplay, 0, 0, 0);
}
-(BOOL)shouldDelayWindowOrderingForEvent:(NSEvent *)event{
    BOOL ret = false;
    if(_w_widget_send_msg(widget, self, _NS_shouldDelayWindowOrderingForEvent, event, &ret, 0) ==0 ){
         return [super shouldDelayWindowOrderingForEvent:event];
    }else return ret;
}
-(BOOL)acceptsFirstMouse:(NSEvent *)event{
    BOOL ret = false;
    if(_w_widget_send_msg(widget, self, _NS_acceptsFirstMouse, event, 0, 0) ==0 ){
         return [super acceptsFirstMouse:event];
    }else return ret;
}
-(void)changeColor:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_changeColor, event, 0, 0) ==0 ){
         [super changeColor:event];
    }
}
-(void)cancelOperation:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_cancelOperation, event, 0, 0) ==0 ){
         [super cancelOperation:event];
    }
}
-(void)touchesBeganWithEvent:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_touchesBeganWithEvent, event, 0, 0) ==0 ){
         [super touchesBeganWithEvent:event];
    }
}
-(void)touchesMovedWithEvent:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_touchesMovedWithEvent, event, 0, 0) ==0 ){
         [super touchesMovedWithEvent:event];
    }
}
-(void)touchesEndedWithEvent:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_touchesEndedWithEvent, event, 0, 0) ==0 ){
         [super touchesEndedWithEvent:event];
    }
}
-(void)touchesCancelledWithEvent:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_touchesCancelledWithEvent, event, 0, 0) ==0 ){
         [super touchesCancelledWithEvent:event];
    }
}
-(void)swipeWithEvent:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_swipeWithEvent, event, 0, 0) ==0 ){
         [super swipeWithEvent:event];
    }
}
-(void)rotateWithEvent:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_rotateWithEvent, event, 0, 0) ==0 ){
         [super rotateWithEvent:event];
    }
}
-(void)magnifyWithEvent:(NSEvent *)event{
    if(_w_widget_send_msg(widget, self, _NS_magnifyWithEvent, event, 0, 0) ==0 ){
         [super magnifyWithEvent:event];
    }
}
-(BOOL)resignFirstResponder{
    BOOL ret = false;
    if(_w_widget_send_msg(widget, self, _NS_resignFirstResponder, &ret, 0, 0) == 0){
        return [super resignFirstResponder];
    }else{
        return ret;
    }
}
-(BOOL)becomeFirstResponder{
    BOOL ret = false;
    if(_w_widget_send_msg(widget, self, _NS_becomeFirstResponder, 0, 0, 0)== 0){
         return [super becomeFirstResponder];
    }else {
        return ret;
    }
}
-(void)resetCursorRects{
    if(_w_widget_send_msg(widget, self, _NS_resetCursorRects, 0, 0, 0) ==0 ){
         [super resetCursorRects];
    }
}
-(void)updateTrackingAreas{
    if(_w_widget_send_msg(widget, self, _NS_updateTrackingAreas, 0, 0, 0) ==0 ){
         [super updateTrackingAreas];
    }
}
-(BOOL)mouseDownCanMoveWindow{
    BOOL ret = false;
    if(_w_widget_send_msg(widget, self, _NS_mouseDownCanMoveWindow, &ret, 0, 0) ==0 ){
         return [super mouseDownCanMoveWindow];
    }else return ret;
}
-(void)setNeedsDisplayInRect:(NSRect)invalidRect{
    if(_w_widget_send_msg(widget, self, _NS_setNeedsDisplayInRect, &invalidRect, 0, 0) ==0 ){
         [super setNeedsDisplayInRect:invalidRect];
    }
}
-(void)drawRect:(NSRect)dirtyRect{
    if(_w_widget_send_msg(widget, self, _NS_drawRect, &dirtyRect, 0, 0) ==0 ){
         [super drawRect:dirtyRect];
    }
}
- (nullable NSView *)hitTest:(NSPoint)point{
    NSView *view = 0;
    if(_w_widget_send_msg(widget, self, _NS_hitTest, &point, &view, 0) ==0 ){
         return [super hitTest:point];
    }else return view;
}
@end
w_widget* SWTView_get_widget(NSView* view){
    if( [view isKindOfClass:[SWTView class]])
    return ((SWTView*)view)->widget;
    else return 0;
}
NSView* SWTView_new(w_widget* widget){
    NSRect frame=NSMakeRect(0, 0, 0, 0);
    SWTView* canvasView = [[SWTView alloc] initWithFrame:frame];
    _w_toolkit_registre_class(_NS_CLASS_VIEW,canvasView.class);
    canvasView->widget = widget;
    return canvasView;
}

int NSView_acceptsFirstMouse(NSView* view,NSEvent* theEvent) {
    return  [view acceptsFirstMouse:theEvent];
}

void NSView_addSubview(NSView* view,NSView* aView) {
     [view addSubview:aView];
}

void NSView_addSubview_0(NSView* view,NSView* aView, int place, NSView* otherView) {
     [view addSubview:aView positioned:place relativeTo:otherView];
}

long NSView_addToolTipRect(NSView* view,w_rect* aRect, NSObject* anObject, void * data) {
    NSRect rect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    return [view addToolTipRect:rect owner:anObject userData:data];
}

void NSView_beginDocument(NSView* view) {
    [view beginDocument];
}

void NSView_beginPageInRect(NSView* view,w_rect* aRect, w_point* location) {
    NSRect rect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    NSPoint loc = NSMakePoint(location->x, location->y);
    [view beginPageInRect:rect atPlacement:loc];
}

NSBitmapImageRep* NSView_bitmapImageRepForCachingDisplayInRect(NSView* view,w_rect* aRect) {
    NSRect rect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    return [view bitmapImageRepForCachingDisplayInRect:rect];
}

void NSView_bounds(NSView* view,w_rect* bounds) {
    NSRect frame = view.bounds;
    bounds->x = frame.origin.x;
    bounds->y = frame.origin.y;
    bounds->width = frame.size.width;
    bounds->height = frame.size.height;
}

void NSView_cacheDisplayInRect(NSView* view,w_rect* aRect, NSBitmapImageRep* bitmapImageRep) {
    NSRect rect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    [view cacheDisplayInRect:rect toBitmapImageRep:bitmapImageRep];
}

int NSView_canBecomeKeyView(NSView* view) {
    return [view canBecomeKeyView];
}

void NSView_convertPoint_fromView_(NSView* view,w_point* result,w_point* aPoint, NSView* aView) {
    NSPoint loc = NSMakePoint(aPoint->x, aPoint->y);
    NSPoint ret = [view convertPoint:loc fromView:aView];
    result->x = ret.x;
    result->y = ret.y;
}

void NSView_convertPoint_toView_(NSView* view,w_point* result,w_point* aPoint, NSView* aView) {
    NSPoint loc = NSMakePoint(aPoint->x, aPoint->y);
    NSPoint ret = [view convertPoint:loc toView:aView];
    result->x = ret.x;
    result->y = ret.y;
}

void NSView_convertRect_fromView_(NSView* view,w_rect* result,w_rect* aRect, NSView* aView) {
    NSRect rect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    NSRect ret = [view convertRect:rect fromView:aView];
    result->x = ret.origin.x;
    result->y = ret.origin.y;
    result->width = ret.size.width;
    result->height = ret.size.height;
}

void NSView_convertRect_toView_(NSView* view,w_rect* result,w_rect* aRect, NSView* aView) {
    NSRect rect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    NSRect ret = [view convertRect:rect toView:aView];
    result->x = ret.origin.x;
    result->y = ret.origin.y;
    result->width = ret.size.width;
    result->height = ret.size.height;
}

void NSView_discardCursorRects(NSView* view) {
    [view discardCursorRects];
}

void NSView_display(NSView* view) {
    [view display];
}

void NSView_displayIfNeeded(NSView* view) {
    [view displayIfNeeded];
}

void NSView_displayRectIgnoringOpacity(NSView* view,w_rect* aRect, NSGraphicsContext* context) {
    NSRect rect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    [view displayRectIgnoringOpacity:rect inContext:context];
}

void NSView_dragImage(NSView* view,NSImage* anImage, w_point* viewLocation, w_size* initialOffset, NSEvent* event, NSPasteboard* pboard, NSObject* sourceObj, int slideFlag) {
    NSPoint _viewLocation = NSMakePoint(viewLocation->x, viewLocation->y);
    NSSize _initialOffset = NSMakeSize(initialOffset->width, initialOffset->height);
    [view dragImage:anImage at:_viewLocation offset:_initialOffset event:event pasteboard:pboard source:sourceObj slideBack:slideFlag];
}

void NSView_drawRect(NSView* view,w_rect* dirtyRect) {
    NSRect _dirtyRect = NSMakeRect(dirtyRect->x, dirtyRect->y,dirtyRect->width,dirtyRect->height);
     [view drawRect:_dirtyRect];
}

void NSView_endDocument(NSView* view) {
    [view endDocument];
}

void NSView_endPage(NSView* view) {
    [view endPage];
}

void NSView_frame(NSView* view,w_rect* result) {
    NSRect frame = view.frame;
    result->x = frame.origin.x;
    result->y = frame.origin.y;
    result->width = frame.size.width;
    result->height = frame.size.height;
}

NSView* NSView_hitTest(NSView* view,w_point* aPoint) {
    NSPoint _aPoint = NSMakePoint(aPoint->x, aPoint->y);
    return [view hitTest:_aPoint];
}

NSView* NSView_initWithFrame(NSView* view,w_rect* frameRect) {
    NSRect _frameRect;
    if(frameRect != 0){
    _frameRect = NSMakeRect(frameRect->x, frameRect->y,frameRect->width,frameRect->height);
    }else{
    _frameRect = NSMakeRect(0,0,0,0);
    }
    return [view initWithFrame:_frameRect];
}

int NSView_isDescendantOf(NSView* view,NSView* aView) {
    return [view isDescendantOf:aView];
}

int NSView_isFlipped(NSView* view) {
    return [view isFlipped];
}

int NSView_isHidden(NSView* view) {
    return [view isHidden];
}

int NSView_isHiddenOrHasHiddenAncestor(NSView* view) {
    return [view isHiddenOrHasHiddenAncestor];
}

int NSView_isOpaque(NSView* view) {
    return [view isOpaque];
}

void NSView_lockFocus(NSView* view) {
    [view lockFocus];
}

int NSView_lockFocusIfCanDraw(NSView* view) {
    return [view lockFocusIfCanDraw];
}

NSMenu* NSView_menuForEvent(NSView* view,NSEvent* event) {
    return [view menuForEvent:event];
}

int NSView_mouse(NSView* view,w_point* aPoint, w_rect* aRect) {
    NSRect _aRect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    NSPoint _aPoint=NSMakePoint(aPoint->x, aPoint->y);
    return [view mouse:_aPoint inRect:_aRect];
}

int NSView_mouseDownCanMoveWindow(NSView* view) {
    return [view mouseDownCanMoveWindow];
}

int NSView_needsPanelToBecomeKey(NSView* view) {
    return [view needsPanelToBecomeKey];
}

void NSView_registerForDraggedTypes(NSView* view,NSArray* newTypes) {
    [view registerForDraggedTypes:newTypes];
}

void NSView_removeFromSuperview(NSView* view) {
    [view removeFromSuperview];
}

void NSView_removeToolTip(NSView* view,long tag) {
    [view removeToolTip:tag];
}

void NSView_removeTrackingArea(NSView* view,NSTrackingArea* trackingArea) {
    [view removeTrackingArea:trackingArea];
}

void NSView_resetCursorRects(NSView* view) {
    [view resetCursorRects];
}

void NSView_scrollClipView(NSView* view,NSClipView* aClipView, w_point* aPoint) {
    NSPoint _aPoint = NSMakePoint(aPoint->x, aPoint->y);
    [view scrollClipView:aClipView toPoint:_aPoint];
}

void NSView_scrollPoint(NSView* view,w_point* aPoint) {
    NSPoint _aPoint = NSMakePoint(aPoint->x, aPoint->y);
    [view scrollPoint:_aPoint];
}

void NSView_scrollRect(NSView* view,w_rect* aRect, w_size* delta) {
    NSRect _aRect = NSMakeRect(aRect->x, aRect->y,aRect->width,aRect->height);
    NSSize _delta=NSMakeSize(delta->width, delta->height);
    [view scrollRect:_aRect by:_delta];
}

void NSView_setAcceptsTouchEvents(NSView* view,int acceptsTouchEvents) {
    [view setAcceptsTouchEvents:acceptsTouchEvents];
}

void NSView_setAutoresizesSubviews(NSView* view,int autoresizesSubviews) {
    [view setAutoresizesSubviews:autoresizesSubviews];
}

void NSView_setAutoresizingMask(NSView* view,int autoresizingMask) {
    [view setAutoresizingMask:autoresizingMask];
}

void NSView_setBoundsRotation(NSView* view,double boundsRotation) {
    [view setBoundsRotation:boundsRotation];
}

void NSView_setBoundsSize(NSView* view,w_size* newSize) {
   NSSize _newSize=NSMakeSize(newSize->width, newSize->height);
    [view setBoundsSize:_newSize];
}

void NSView_setFocusRingType(NSView* view,long focusRingType) {
    [view setFocusRingType:focusRingType];
}

void NSView_setFrame(NSView* view,w_rect* frame) {
    NSRect _frame = NSMakeRect(frame->x, frame->y,frame->width,frame->height);
    [view setFrame:_frame];
}
void NSView_setFrame_0(NSView* view,w_point* location,w_size* size) {
    NSRect _frame = NSMakeRect(location->x, location->y,size->width,size->height);
    [view setFrame:_frame];
}

void NSView_setFrameOrigin(NSView* view,w_point* newOrigin) {
    NSPoint _newOrigin = NSMakePoint(newOrigin->x, newOrigin->y);
    [view setFrameOrigin:_newOrigin];
}

void NSView_setFrameSize(NSView* view,w_size* newSize) {
    NSSize _newSize=NSMakeSize(newSize->width, newSize->height);
     [view setFrameSize:_newSize];
}

void NSView_setHidden(NSView* view,int hidden) {
    [view setHidden:hidden];
}

void NSView_setNeedsDisplay(NSView* view,int needsDisplay) {
   [view setNeedsDisplay:needsDisplay];
}

void NSView_setNeedsDisplayInRect(NSView* view,w_rect* invalidRect) {
    NSRect _invalidRect = NSMakeRect(invalidRect->x, invalidRect->y,invalidRect->width,invalidRect->height);
    [view setNeedsDisplayInRect:_invalidRect];
}

void NSView_setToolTip(NSView* view,const char* toolTip) {
    NSString *_toolTip=[NSString stringWithUTF8String:toolTip];
    [view setToolTip:_toolTip];
}

void NSView_setWantsRestingTouches(NSView* view,int wantsRestingTouches) {
    [view setWantsRestingTouches:wantsRestingTouches];
}

int NSView_shouldDelayWindowOrderingForEvent(NSView* view,NSEvent* theEvent) {
    return [view shouldDelayWindowOrderingForEvent:theEvent];
}

NSArray* NSView_subviews(NSView* view) {
    return [view subviews];
}

NSView* NSView_superview(NSView* view) {
    return [view superview];
}

NSArray* NSView_trackingAreas(NSView* view) {
    return [view trackingAreas];;
}

void NSView_unlockFocus(NSView* view) {
    [view unlockFocus];
}

void NSView_unregisterDraggedTypes(NSView* view) {
    [view unregisterDraggedTypes];
}

void NSView_updateTrackingAreas(NSView* view) {
    [view updateTrackingAreas];
}

void NSView_viewDidMoveToWindow(NSView* view) {
    [view viewDidMoveToWindow];
}

void NSView_viewWillMoveToWindow(NSView* view,NSWindow* newWindow) {
    [view viewWillMoveToWindow:newWindow];
}

void NSView_visibleRect(NSView* view,w_rect* result) {
    NSRect visibleRect = view.visibleRect;
    result->x = visibleRect.origin.x;
    result->y = visibleRect.origin.y;
    result->width = visibleRect.size.width;
    result->height = visibleRect.size.height;
}

NSWindow* NSView_window(NSView* view) {
    return [view window];;
}
