//
//  shell_m.m
//  swt
//
//  Created by elbassbasi on 7/29/20.
//
#import "widget.h"
@interface SWTWindowDelegate : NSObject <NSWindowDelegate> {
@public w_widget* widget;
}
- (BOOL)windowShouldClose:(NSWindow *)sender;
- (void)windowWillClose:(NSNotification *)notification;
- (void)windowDidResize:(NSNotification *)notification;
@end
@implementation SWTWindowDelegate
- (BOOL)windowShouldClose:(NSWindow *)sender{
    return _w_widget_send_msg(widget, self, _NS_WINDOW_SHOULD_CLOSE, 0, 0, 0);
}
- (void)windowWillClose:(NSNotification *)notification{
    _w_widget_send_msg(widget, self, _NS_WINDOW_WILL_CLOSE, notification, 0, 0);
}
- (void)windowDidResize:(NSNotification *)notification{
    _w_widget_send_msg(widget, self, _NS_WINDOW_DID_RESIZE, notification, 0, 0);
}
@end
@interface SWTWindow : NSWindow {
@public w_widget* widget;
}
@end
@implementation SWTWindow
@end
@interface SWTPanel : NSPanel {
@public w_widget* widget;
}
@end
@implementation SWTPanel
@end
NSObject* SWTWindowDelegate_alloc(w_widget* widget){
    SWTWindowDelegate* delegate =[SWTWindowDelegate alloc];
    [delegate init];
    delegate->widget = widget;
    return delegate;
}
NSWindow* SWTWindow_alloc(w_widget* widget){
    SWTWindow* window =[SWTWindow alloc];
    window->widget = widget;
    return window;
}
NSWindow* SWTPanel_alloc(w_widget* widget){
    SWTPanel* panel =[SWTPanel alloc];
    panel->widget = widget;
    return panel;
}
void NSPanel_setBecomesKeyOnlyIfNeeded(NSPanel* panel,int becomesKeyOnlyIfNeeded) {
    [panel setBecomesKeyOnlyIfNeeded:becomesKeyOnlyIfNeeded];
}
void NSPanel_setFloatingPanel(NSPanel* panel,int floatingPanel) {
    [panel setFloatingPanel:floatingPanel];
}
void NSPanel_setWorksWhenModal(NSPanel* panel,int worksWhenModal) {
    [panel setWorksWhenModal:worksWhenModal];
}
double NSPanel_minFrameWidthWithTitle(NSString* aTitle, long aStyle) {
    return [NSPanel minFrameWidthWithTitle:aTitle styleMask:aStyle];
}
long NSPanel_windowNumberAtPoint(w_point* point, long windowNumber) {
    NSPoint _point = NSMakePoint(point->x, point->y);
    return [NSPanel windowNumberAtPoint:_point belowWindowWithWindowNumber:windowNumber];
}
/*
 * NSWindow
 */
void NSWindow_addChildWindow(NSWindow* window,NSWindow* childWin, long place) {
    [window addChildWindow:childWin ordered:place];
}

double NSWindow_alphaValue(NSWindow* window) {
    return [window alphaValue];
}

int NSWindow_areCursorRectsEnabled(NSWindow* window) {
    return [window areCursorRectsEnabled];
}

void NSWindow_becomeKeyWindow(NSWindow* window) {
    [window becomeKeyWindow];
}

int NSWindow_canBecomeKeyWindow(NSWindow* window) {
    return [window canBecomeKeyWindow];
}

void NSWindow_cascadeTopLeftFromPoint(NSWindow* window,w_point* result,w_point* topLeftPoint) {
    NSPoint _topLeftPoint = NSMakePoint(topLeftPoint->x, topLeftPoint->y);
    NSPoint _result = [window cascadeTopLeftFromPoint:_topLeftPoint];
    result->x = _result.x;
    result->y = _result.y;
}

void NSWindow_close(NSWindow* window) {
    [window close];
}

int NSWindow_collectionBehavior(NSWindow* window) {
    return window.collectionBehavior;
}

NSView* NSWindow_contentView(NSWindow* window) {
    return window.contentView;
}

void NSWindow_convertBaseToScreen(NSWindow* window,w_point* result,w_point* aPoint) {
    NSPoint _aPoint = NSMakePoint(aPoint->x, aPoint->y);
    NSPoint _result = [window convertBaseToScreen:_aPoint];
    result->x = _result.x;
    result->y = _result.y;
}

void NSWindow_convertScreenToBase(NSWindow* window,w_point* result,w_point* aPoint) {
    NSPoint _aPoint = NSMakePoint(aPoint->x, aPoint->y);
    NSPoint _result = [window convertScreenToBase:_aPoint];
    result->x = _result.x;
    result->y = _result.y;
}

NSButtonCell* NSWindow_defaultButtonCell(NSWindow* window) {
    return [window defaultButtonCell];
}

NSObject* NSWindow_delegate(NSWindow* window) {
    return [window delegate];
}

void NSWindow_deminiaturize(NSWindow* window,NSObject* sender) {
    [window deminiaturize:sender];
}

void NSWindow_disableCursorRects(NSWindow* window) {
   [window disableCursorRects];
}

void NSWindow_disableFlushWindow(NSWindow* window) {
    [window disableFlushWindow];
}

void NSWindow_display(NSWindow* window) {
    [window display];
}

void NSWindow_enableCursorRects(NSWindow* window) {
    [window enableCursorRects];
}

void NSWindow_enableFlushWindow(NSWindow* window) {
    [window enableFlushWindow];
}

void NSWindow_endEditingFor(NSWindow* window,NSObject* anObject) {
    [window endEditingFor:anObject];
}

NSText* NSWindow_fieldEditor(NSWindow* window,int createFlag, NSObject* anObject) {
    return [window fieldEditor:createFlag forObject:anObject];
}

NSResponder* NSWindow_firstResponder(NSWindow* window) {
    return [window firstResponder];
}

void NSWindow_flushWindowIfNeeded(NSWindow* window) {
    [window flushWindowIfNeeded];
}

void NSWindow_frame(NSWindow* window,w_rect* result) {
    NSRect _result =window.frame;
    result->x = _result.origin.x;
    result->y = _result.origin.y;
    result->width = _result.size.width;
    result->height = _result.size.height;
}

void NSWindow_frameRectForContentRect(NSWindow* window,w_rect* result,w_rect* contentRect) {
    NSRect _contentRect = NSMakeRect(contentRect->x, contentRect->y, contentRect->width, contentRect->height);
    NSRect _result =[window frameRectForContentRect:_contentRect];
    result->x = _result.origin.x;
    result->y = _result.origin.y;
    result->width = _result.size.width;
    result->height = _result.size.height;
}

NSGraphicsContext* NSWindow_graphicsContext(NSWindow* window) {
    return [window graphicsContext];
}

int NSWindow_hasShadow(NSWindow* window) {
    return [window hasShadow];
}

NSWindow* NSWindow_initWithContentRect(NSWindow* window,w_rect* contentRect, long aStyle, long bufferingType, int flag) {
    NSRect _contentRect;
    if(contentRect == 0){
        _contentRect.origin.x = 0;
        _contentRect.origin.y = 0;
        _contentRect.size.width = 0;
        _contentRect.size.height = 0;
    }else{
        _contentRect.origin.x = contentRect->x;
        _contentRect.origin.y = contentRect->y;
        _contentRect.size.width = contentRect->width;
        _contentRect.size.height = contentRect->height;
    }
    return [window initWithContentRect:_contentRect styleMask:aStyle backing:bufferingType defer:flag];
}

NSWindow* NSWindow_initWithContentRect_0(NSWindow* window,w_rect* contentRect, long aStyle, long bufferingType, int flag, NSScreen* screen) {
    NSRect _contentRect;
    if(contentRect == 0){
        _contentRect.origin.x = 0;
        _contentRect.origin.y = 0;
        _contentRect.size.width = 0;
        _contentRect.size.height = 0;
    }else{
        _contentRect.origin.x = contentRect->x;
        _contentRect.origin.y = contentRect->y;
        _contentRect.size.width = contentRect->width;
        _contentRect.size.height = contentRect->height;
    }
    return [window initWithContentRect:_contentRect styleMask:aStyle backing:bufferingType defer:flag screen:screen];
}

void NSWindow_invalidateShadow(NSWindow* window) {
    [window invalidateShadow];
}

int NSWindow_isDocumentEdited(NSWindow* window) {
    return [window isDocumentEdited];
}

int NSWindow_isKeyWindow(NSWindow* window) {
    return [window isKeyWindow];
}

int NSWindow_isMainWindow(NSWindow* window) {
    return [window isMainWindow];
}

int NSWindow_isMiniaturized(NSWindow* window) {
    return [window isMiniaturized];
}

int NSWindow_isSheet(NSWindow* window) {
    return [window isSheet];
}

int NSWindow_isVisible(NSWindow* window) {
    return [window isVisible];
}

int NSWindow_isZoomed(NSWindow* window) {
    return [window isZoomed];
}

int NSWindow_makeFirstResponder(NSWindow* window,NSResponder* aResponder) {
    return [window makeFirstResponder:aResponder];
}

void NSWindow_makeKeyAndOrderFront(NSWindow* window,NSObject* sender) {
   [window makeKeyAndOrderFront:sender];
}

double NSWindow_minFrameWidthWithTitle(NSString* aTitle, long aStyle) {
    return [NSWindow minFrameWidthWithTitle:aTitle styleMask:aStyle];
}

void NSWindow_minSize(NSWindow* window,w_size* result) {
    NSSize _result = window.minSize;
    result->width = _result.width;
    result->height = _result.height;
}

void NSWindow_miniaturize(NSWindow* window,NSObject* sender) {
    [window miniaturize:sender];
}
void NSWindow_mouseLocationOutsideOfEventStream(NSWindow* window,w_point* result) {
    NSPoint _result = window.mouseLocationOutsideOfEventStream;
    result->x = _result.x;
    result->y = _result.y;
}

void NSWindow_orderBack(NSWindow* window,NSObject* sender) {
    [window orderBack:sender];
}

void NSWindow_orderFront(NSWindow* window,NSObject* sender) {
    [window orderFront:sender];
}

void NSWindow_orderFrontRegardless(NSWindow* window) {
    [window orderFrontRegardless];
}

void NSWindow_orderOut(NSWindow* window,NSObject* sender) {
    [window orderOut:sender];
}

void NSWindow_orderWindow(NSWindow* window,long place, long otherWin) {
    [window orderWindow:place relativeTo:otherWin];
}

NSWindow* NSWindow_parentWindow(NSWindow* window) {
    return [window parentWindow];
}

void NSWindow_removeChildWindow(NSWindow* window,NSWindow* childWin) {
    [window removeChildWindow:childWin];
}

NSScreen* NSWindow_screen(NSWindow* window) {
    return [window screen];
}

void NSWindow_sendEvent(NSWindow* window,NSEvent* theEvent) {
    [window sendEvent:theEvent];
}
void NSWindow_setAllowsAutomaticWindowTabbing(int tabbing) {
    [NSWindow setAllowsAutomaticWindowTabbing:tabbing];
}
void NSWindow_setAcceptsMouseMovedEvents(NSWindow* window,int acceptsMouseMovedEvents) {
    [window setAcceptsMouseMovedEvents:acceptsMouseMovedEvents];
}

void NSWindow_setAlphaValue(NSWindow* window,double alphaValue) {
    [window setAlphaValue:alphaValue];
}

void NSWindow_setBackgroundColor(NSWindow* window,NSColor* backgroundColor) {
    [window setBackgroundColor:backgroundColor];
}

void NSWindow_setCollectionBehavior(NSWindow* window,long collectionBehavior) {
    [window setCollectionBehavior:collectionBehavior];
}

void NSWindow_setContentView(NSWindow* window,NSView* contentView) {
    [window setContentView:contentView];
}

void NSWindow_setDefaultButtonCell(NSWindow* window,NSButtonCell* defaultButtonCell) {
    [window setDefaultButtonCell:defaultButtonCell];
}
NSObject* NSWindow_getDelegate(NSWindow* window) {
    return window.delegate;
}
void NSWindow_setDelegate(NSWindow* window,NSObject* delegate) {
    [window setDelegate:(id<NSWindowDelegate>)delegate];
}

void NSWindow_setDocumentEdited(NSWindow* window,int documentEdited) {
    [window setDocumentEdited:documentEdited];
}

void NSWindow_setFrame(NSWindow* window,w_rect* frameRect, int flag) {
        NSRect _frameRect=NSMakeRect(frameRect->x, frameRect->y, frameRect->width, frameRect->height);
    [window setFrame:_frameRect display:flag];
}

void NSWindow_setFrame_0(NSWindow* window,w_rect* frameRect, int displayFlag, int animateFlag) {
    NSRect _frameRect=NSMakeRect(frameRect->x, frameRect->y, frameRect->width, frameRect->height);
    [window setFrame:_frameRect display:displayFlag animate:animateFlag];
}

void NSWindow_setHasShadow(NSWindow* window,int hasShadow) {
    [window setHasShadow:hasShadow];
}

void NSWindow_setHidesOnDeactivate(NSWindow* window,int hidesOnDeactivate) {
    [window setHidesOnDeactivate:hidesOnDeactivate];
}

void NSWindow_setLevel(NSWindow* window,long level) {
    [window setLevel:level];
}

void NSWindow_setMinSize(NSWindow* window,w_size* minSize) {
    NSSize _minSize = NSMakeSize(minSize->width, minSize->height);
    [window setMinSize:_minSize];
}

void NSWindow_setMovable(NSWindow* window,int movable) {
    [window setMovable:movable];
}

void NSWindow_setOpaque(NSWindow* window,int opaque) {
    [window setOpaque:opaque];
}

void NSWindow_setReleasedWhenClosed(NSWindow* window,int releasedWhenClosed) {
    [window setReleasedWhenClosed:releasedWhenClosed];
}

void NSWindow_setRepresentedFilename(NSWindow* window,NSString* representedFilename) {
    [window setRepresentedFilename:representedFilename];
}

void NSWindow_setRepresentedURL(NSWindow* window,NSURL* representedURL) {
    [window setRepresentedURL:representedURL];
}

void NSWindow_setShowsResizeIndicator(NSWindow* window,int showsResizeIndicator) {
    [window setShowsResizeIndicator:showsResizeIndicator];
}

void NSWindow_setShowsToolbarButton(NSWindow* window,int showsToolbarButton) {
    [window setShowsToolbarButton:showsToolbarButton];
}

void NSWindow_setTitle(NSWindow* window,NSString* title) {
    [window setTitle:title];
}

void NSWindow_setToolbar(NSWindow* window,NSToolbar* toolbar) {
    [window setToolbar:toolbar];
}

long NSWindow_styleMask(NSWindow* window) {
    return [window styleMask];
}

void NSWindow_toggleFullScreen(NSWindow* window,NSObject* sender) {
    [window toggleFullScreen:sender];
}

NSToolbar* NSWindow_toolbar(NSWindow* window) {
    return window.toolbar;
}

long NSWindow_windowNumber(NSWindow* window) {
    return window.windowNumber;
}

long NSWindow_windowNumberAtPoint(w_point* point, long windowNumber) {
    NSPoint _point = NSMakePoint(point->x, point->y);
    return [NSWindow windowNumberAtPoint:_point belowWindowWithWindowNumber:windowNumber];
}

void NSWindow_zoom(NSWindow* window,NSObject* sender) {
    [window zoom:sender];
}
