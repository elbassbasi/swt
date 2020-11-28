/*
 * shell.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_WIDGETS_SHELL_H_
#define SRC_COCOA_WIDGETS_SHELL_H_

#ifdef __APPLE__
#include "canvas.h"
#define STATE_FOREIGN_HANDLE (1 << (STATE_CANVAS_END + 2))
/*
 SWTWindow
 */
typedef struct NSWindow NSWindow;
typedef struct NSButtonCell NSButtonCell;
typedef struct NSText NSText;
typedef struct NSResponder NSResponder;
typedef struct NSURL NSURL;
typedef struct NSToolbar NSToolbar;
typedef struct NSColor NSColor;
NSObject* SWTWindowDelegate_alloc(w_widget* widget);
NSWindow* SWTWindow_alloc(w_widget* widget);
NSWindow* SWTPanel_alloc(w_widget* widget);
typedef struct NSPanel NSPanel;
void NSPanel_setBecomesKeyOnlyIfNeeded(NSPanel* panel,int becomesKeyOnlyIfNeeded);
void NSPanel_setFloatingPanel(NSPanel* panel,int floatingPanel);
void NSPanel_setWorksWhenModal(NSPanel* panel,int worksWhenModal);
double NSPanel_minFrameWidthWithTitle(NSString* aTitle, long aStyle);
long NSPanel_windowNumberAtPoint(w_point* point, long windowNumber);
/*
 * NSWindow
 */
typedef struct NSScreen NSScreen;
void NSWindow_addChildWindow(NSWindow* window,NSWindow* childWin, long place);
double NSWindow_alphaValue(NSWindow* window);
int NSWindow_areCursorRectsEnabled(NSWindow* window);
void NSWindow_becomeKeyWindow(NSWindow* window);
int NSWindow_canBecomeKeyWindow(NSWindow* window);
void NSWindow_cascadeTopLeftFromPoint(NSWindow* window,w_point* result,w_point* topLeftPoint);
void NSWindow_close(NSWindow* window);
int NSWindow_collectionBehavior(NSWindow* window);
NSView* NSWindow_contentView(NSWindow* window);
void NSWindow_convertBaseToScreen(NSWindow* window,w_point* result,w_point* aPoint);
void NSWindow_convertScreenToBase(NSWindow* window,w_point* result,w_point* aPoint);
NSButtonCell* NSWindow_defaultButtonCell(NSWindow* window);
NSObject* NSWindow_delegate(NSWindow* window);
void NSWindow_deminiaturize(NSWindow* window,NSObject* sender);
void NSWindow_disableCursorRects(NSWindow* window);
void NSWindow_disableFlushWindow(NSWindow* window);
void NSWindow_display(NSWindow* window);
void NSWindow_enableCursorRects(NSWindow* window);
void NSWindow_enableFlushWindow(NSWindow* window);
void NSWindow_endEditingFor(NSWindow* window,NSObject* anObject);
NSText* NSWindow_fieldEditor(NSWindow* window,int createFlag, NSObject* anObject);
NSResponder* NSWindow_firstResponder(NSWindow* window);
void NSWindow_flushWindowIfNeeded(NSWindow* window);
void NSWindow_frame(NSWindow* window,w_rect* result);
void NSWindow_frameRectForContentRect(NSWindow* window,w_rect* result,w_rect* contentRect);
NSGraphicsContext* NSWindow_graphicsContext(NSWindow* window);
int NSWindow_hasShadow(NSWindow* window);
NSWindow* NSWindow_initWithContentRect(NSWindow* window,w_rect* contentRect, long aStyle, long bufferingType, int flag);
NSWindow* NSWindow_initWithContentRect_0(NSWindow* window,w_rect* contentRect, long aStyle, long bufferingType, int flag, NSScreen* screen);
void NSWindow_invalidateShadow(NSWindow* window);
int NSWindow_isDocumentEdited(NSWindow* window);
int NSWindow_isKeyWindow(NSWindow* window);
int NSWindow_isMainWindow(NSWindow* window);
int NSWindow_isMiniaturized(NSWindow* window);
int NSWindow_isSheet(NSWindow* window);
int NSWindow_isVisible(NSWindow* window);
int NSWindow_isZoomed(NSWindow* window);
int NSWindow_makeFirstResponder(NSWindow* window,NSResponder* aResponder);
void NSWindow_makeKeyAndOrderFront(NSWindow* window,NSObject* sender);
double NSWindow_minFrameWidthWithTitle(NSString* aTitle, long aStyle);
void NSWindow_minSize(NSWindow* window,w_size* result);
void NSWindow_miniaturize(NSWindow* window,NSObject* sender);
void NSWindow_mouseLocationOutsideOfEventStream(NSWindow* window,w_point* result);
void NSWindow_orderBack(NSWindow* window,NSObject* sender);
void NSWindow_orderFront(NSWindow* window,NSObject* sender);
void NSWindow_orderFrontRegardless(NSWindow* window);
void NSWindow_orderOut(NSWindow* window,NSObject* sender);
void NSWindow_orderWindow(NSWindow* window,long place, long otherWin);
NSWindow* NSWindow_parentWindow(NSWindow* window);
void NSWindow_removeChildWindow(NSWindow* window,NSWindow* childWin);
NSScreen* NSWindow_screen(NSWindow* window);
void NSWindow_sendEvent(NSWindow* window,NSEvent* theEvent);
void NSWindow_setAllowsAutomaticWindowTabbing(int tabbing);
void NSWindow_setAcceptsMouseMovedEvents(NSWindow* window,int acceptsMouseMovedEvents);
void NSWindow_setAlphaValue(NSWindow* window,double alphaValue);
void NSWindow_setBackgroundColor(NSWindow* window,NSColor* backgroundColor);
void NSWindow_setCollectionBehavior(NSWindow* window,long collectionBehavior);
void NSWindow_setContentView(NSWindow* window,NSView* contentView);
void NSWindow_setDefaultButtonCell(NSWindow* window,NSButtonCell* defaultButtonCell);
NSObject* NSWindow_getDelegate(NSWindow* window);
void NSWindow_setDelegate(NSWindow* window,NSObject* delegate);
void NSWindow_setDocumentEdited(NSWindow* window,int documentEdited);
void NSWindow_setFrame(NSWindow* window,w_rect* frameRect, int flag);
void NSWindow_setFrame_0(NSWindow* window,w_rect* frameRect, int displayFlag, int animateFlag);
void NSWindow_setHasShadow(NSWindow* window,int hasShadow);
void NSWindow_setHidesOnDeactivate(NSWindow* window,int hidesOnDeactivate);
void NSWindow_setLevel(NSWindow* window,long level);
void NSWindow_setMinSize(NSWindow* window,w_size* minSize);
void NSWindow_setMovable(NSWindow* window,int movable);
void NSWindow_setOpaque(NSWindow* window,int opaque);
void NSWindow_setReleasedWhenClosed(NSWindow* window,int releasedWhenClosed);
void NSWindow_setRepresentedFilename(NSWindow* window,NSString* representedFilename);
void NSWindow_setRepresentedURL(NSWindow* window,NSURL* representedURL);
void NSWindow_setShowsResizeIndicator(NSWindow* window,int showsResizeIndicator);
void NSWindow_setShowsToolbarButton(NSWindow* window,int showsToolbarButton);
void NSWindow_setTitle(NSWindow* window,NSString* title);
void NSWindow_setToolbar(NSWindow* window,NSToolbar* toolbar);
long NSWindow_styleMask(NSWindow* window);
void NSWindow_toggleFullScreen(NSWindow* window,NSObject* sender);
NSToolbar* NSWindow_toolbar(NSWindow* window);
long NSWindow_windowNumber(NSWindow* window);
long NSWindow_windowNumberAtPoint(w_point* point, long windowNumber);
void NSWindow_zoom(NSWindow* window,NSObject* sender);
typedef struct _w_shell {
	struct _w_canvas canvas;
	struct _w_shell *next;
    struct _w_shell *prev;
	struct w_menu *menubar;
    NSWindow* window;
    unsigned minimized : 1;
    unsigned center :1;
    unsigned opened :1;
    unsigned moved : 1;
    unsigned resized:1;
    unsigned fullScreen:1;
} _w_shell;

struct _w_shell_reserved {
	struct _w_canvas_reserved canvas;

};
#define _W_SHELL(x) ((_w_shell*)x)
#define _W_SHELL_RESERVED(x) ((struct _w_shell_reserved*)x)
/*
 *
 */
wresult _w_shell_fix_resize (w_shell* shell);
wresult _w_shell_close_widget (w_shell *shell,_w_event_platform *e);
wresult _w_shell_set_window_visible (w_shell* shell,wbool visible, wbool key);
NSWindow* _w_shell_parent_window (w_shell *shell);
wresult _w_shell_set_full_screen(w_shell *shell, wbool fullScreen);
void _w_shell_class_init(struct _w_shell_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_SHELL_H_ */
