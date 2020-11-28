//
//  toolkit.h
//  cswt
//
//  Created by elbassbasi on 7/21/20.
//

#ifndef SRC_COCOA_WIDGETS_TOOLKIT_H_
#define SRC_COCOA_WIDGETS_TOOLKIT_H_
#ifdef __APPLE__
#include "shell.h"
#include "taskbar.h"
#include "tooltip.h"
#include "tracker.h"
#include "tray.h"
#include "glcanvas.h"
#include "ccanvas.h"
#include "../controls/controls.h"
/*
NSApplication
*/
typedef struct NSApplication NSApplication;
typedef struct NSDockTile NSDockTile;
typedef struct NSDate NSDate;
typedef struct NSModalResponse NSModalResponse;
typedef struct NSRunLoop NSRunLoop;
typedef struct NSTimer NSTimer;
typedef struct NSCalendarDate NSCalendarDate;
typedef struct NSTimeZone NSTimeZone;

/*
 *  NSDate
 */
NSCalendarDate* NSDate_dateWithCalendarFormat(NSDate* date,NSString* format, NSTimeZone* aTimeZone);
NSDate* NSDate_dateWithTimeIntervalSinceNow(double secs);
NSDate* NSDate_distantFuture();

extern NSString* const NSDefaultRunLoopMode;
extern NSString* const NSRunLoopCommonModes;
/*
 * NSRunLoop
 */
void NSRunLoop_addTimer(NSRunLoop* runloop,NSTimer* timer, NSString* mode);
NSRunLoop* NSRunLoop_currentRunLoop();
int NSRunLoop_runMode(NSRunLoop* runloop,NSString* mode, NSDate* limitDate);
/*
 * NSApplication
 */
void NSApplication_activateIgnoringOtherApps(NSApplication *application,int flag);
NSImage* NSApplication_applicationIconImage(NSApplication *application);
void NSApplication_arrangeInFront(NSApplication *application,NSObject* sender);
void NSApplication_beginSheet(NSApplication *application,NSWindow* sheet, NSWindow* docWindow, NSObject* modalDelegate, long didEndSelector, void* contextInfo);
NSEvent* NSApplication_currentEvent(NSApplication *application);
NSDockTile* NSApplication_dockTile(NSApplication *application);
void NSApplication_endSheet(NSApplication *application,NSWindow* sheet, long returnCode);
void NSApplication_finishLaunching(NSApplication *application);
void NSApplication_hide(NSApplication *application,NSObject* sender);
void NSApplication_hideOtherApplications(NSApplication *application,NSObject* sender);
int NSApplication_isActive(NSApplication *application);
int NSApplication_isRunning(NSApplication *application);
NSWindow* NSApplication_keyWindow(NSApplication *application);
NSMenu* NSApplication_mainMenu(NSApplication *application);
NSEvent* NSApplication_nextEventMatchingMask(NSApplication *application,long mask, NSDate* expiration, NSString* mode, int deqFlag);
void NSApplication_orderFrontStandardAboutPanel(NSApplication *application,NSObject* sender);
void NSApplication_postEvent(NSApplication *application,NSEvent* event, int flag);
void NSApplication_replyToOpenOrPrint(NSApplication *application,long reply);
NSModalResponse* NSApplication_runModalForWindow(NSApplication *application,NSWindow* theWindow);
int NSApplication_sendAction(NSApplication *application,long theAction, NSObject* theTarget, NSObject* sender);
void NSApplication_sendEvent(NSApplication *application,NSEvent* theEvent);
int NSApplication_setActivationPolicy(NSApplication *application,long activationPolicy);
void NSApplication_setApplicationIconImage(NSApplication *application,NSImage* applicationIconImage);
void NSApplication_setDelegate(NSApplication *application,NSObject* delegate);
void NSApplication_setHelpMenu(NSApplication *application,NSMenu* helpMenu) ;
void NSApplication_setMainMenu(NSApplication *application,NSMenu* mainMenu) ;
void NSApplication_setServicesMenu(NSApplication *application,NSMenu* servicesMenu);
NSApplication* NSApplication_sharedApplication();
void NSApplication_stop(NSApplication *application,NSObject* sender);
void NSApplication_stopModal(NSApplication *application);
void NSApplication_terminate(NSApplication *application,NSObject* sender);
void NSApplication_unhideAllApplications(NSApplication *application,NSObject* sender);
NSWindow* NSApplication_windowWithWindowNumber(NSApplication *application,long windowNum);
NSArray* NSApplication_windows(NSApplication *application);
/*
 * NSScreen
 */
typedef struct NSDictionary NSDictionary;
double NSScreen_backingScaleFactor(NSScreen* screen);
int NSScreen_depth(NSScreen* screen);
NSDictionary* NSScreen_deviceDescription(NSScreen* screen);
void NSScreen_frame(NSScreen* screen,w_rect* result);
NSScreen* NSScreen_mainScreen();
NSArray* NSScreen_screens();
void NSScreen_visibleFrame(NSScreen* screen,w_rect* result);

/*
NSAutoreleasePool
 */
typedef struct NSAutoreleasePool {
}NSAutoreleasePool;
NSAutoreleasePool* NSAutoreleasePool_New();
void NSAutoreleasePool_release(NSAutoreleasePool* pool);
void NSApp_Init();
int NSThread_isMainThread();

/*
 
 */
#define _W_CLASS_DECL(x) struct _w_##x##_class class_##x;\
    struct _w_##x##_reserved class_##x##_reserved
#define _W_CLASS_DECL_ITEM(x) _W_CLASS_DECL(x);\
        struct _w_##x##item_class class_##x##item
extern ns_get_widget ns_get_widgets[_NS_CLASS_LAST];
typedef struct _w_toolkit  {
    w_toolkit toolkit;
    int version;
    int exit_code;
    NSApplication *application;
    NSAutoreleasePool * pool;
    unsigned lockCursor : 1;
    unsigned exit_loop : 1;
    w_control* currentControl;
    w_control* ignoreFocusControl;
    w_widget* widget_free;
    _w_shell *shells;
    int shells_count;
    Class nsclasses[_NS_CLASS_LAST];
    SEL sels[_NS_VIEW_LAST];
    /*
     * threads
     */
    //struct threads_idle threads_idle[0x10];
    //pthread_mutex_t mutex;
    w_thread guithread;
    /*
     cursors
     */
    _w_cursor cursors[W_CURSOR_HAND+1];
    
        /*
         * classes
         */
        struct _w_widget_class *classes[_W_CLASS_LAST];
        struct _w_toolkit_class class_toolkit;
        struct _w_scrollbar_class class_scrollbar;
        struct _w_tabitem_class class_tabfolderitem;
        struct _w_treecolumn_class class_treecolumn; //
        _W_CLASS_DECL(shell); //
        _W_CLASS_DECL(button); //
        _W_CLASS_DECL(canvas); //
        _W_CLASS_DECL(ccanvas); //
        _W_CLASS_DECL_ITEM(menu); //
        _W_CLASS_DECL(browser); //
        _W_CLASS_DECL_ITEM(tree); //
        _W_CLASS_DECL(tabfolder); //
        _W_CLASS_DECL(composite); //
        _W_CLASS_DECL(sash); //
        _W_CLASS_DECL(glcanvas); //
        _W_CLASS_DECL_ITEM(combo); //
        _W_CLASS_DECL(coolbar); //
        struct _w_coolitem_class class_coolbaritem; //
        _W_CLASS_DECL(datetime); //
        _W_CLASS_DECL(expandbar); //
        struct _w_expanditem_class class_expandbaritem; //
        _W_CLASS_DECL(group); //
        _W_CLASS_DECL(label); //
        _W_CLASS_DECL(link); //
        _W_CLASS_DECL(list); //
        struct _w_listitem_class class_listitem; //
        _W_CLASS_DECL(progressbar); //
        _W_CLASS_DECL(scale); //
        _W_CLASS_DECL(slider); //
        _W_CLASS_DECL(spinner); //
        _W_CLASS_DECL_ITEM(table); //
        struct _w_tablecolumn_class class_tablecolumn; //
        _W_CLASS_DECL(taskbar); //
        struct _w_taskitem_class class_taskbaritem; //
        _W_CLASS_DECL(text); //
        _W_CLASS_DECL(toolbar); //
        struct _w_toolitem_class class_toolbaritem; //
        _W_CLASS_DECL(tooltip); //
        _W_CLASS_DECL(tracker); //
        _W_CLASS_DECL(tray); //
        struct _w_trayitem_class class_trayitem; //
        struct _w_control_class class_controlcanvas;
        struct _w_ccanvas_reserved class_controlcanvas_reserved;
        /*
         * temporary memory
         */
        char mem_tmp[0x200];
}_w_toolkit;
extern _w_toolkit *mac_toolkit;
#define _W_TOOLKIT(x) ((_w_toolkit*)x)
#define VERSION(major,minor,bugfix) ((major << 16) + (minor << 8) + bugfix)
#define MAC_VERSION (mac_toolkit->version)
void _w_toolkit_init(_w_toolkit *toolkit);
void _w_toolkit_class_init(_w_toolkit *toolkit);
void _w_toolkit_subclass_init(_w_toolkit *toolkit);
void* _w_toolkit_malloc(size_t size);
void _w_toolkit_free(void* ptr);
void _w_toolkit_add_shell(_w_shell *shell);
void _w_toolkit_remove_shell(_w_shell *shell);
typedef struct _w_shells_iterator {
    w_basic_iterator base;
    _w_shell *parent;
    _w_shell *current;
    int count;
} _w_shells_iterator;
#define _W_SHELLS_ITERATOR(it) ((_w_shells_iterator*)it)
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator);

/**
 * public function
 */
void _w_toolkit_dispose(w_disposable *_toolkit);
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget);
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
        w_widgetdata *widgetdata);
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
        wushort clazz_id);
struct w_theme* _w_toolkit_get_theme(w_toolkit *toolkit);
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
        void *args);
wresult _w_toolkit_beep(w_toolkit *toolkit);
wresult _w_toolkit_close(w_toolkit *toolkit);
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit);
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit);
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds);
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea);
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit);
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location);
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
        size_t length);
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit);
int _w_toolkit_get_double_click_time(w_toolkit *toolkit);
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit);
wbool _w_toolkit_get_high_contrast(w_toolkit *toolkit);
int _w_toolkit_get_icon_depth(w_toolkit *toolkit);
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
        size_t length);
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells);
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id);
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id);
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit);
wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
        w_image *image);
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit);
struct w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit);
struct w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit);
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit);
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit);
wbool _w_toolkit_get_touch_enabled(w_toolkit *toolkit);
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
        w_point *result, w_point *point);
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
        w_rect *result, w_rect *rectangle);
wbool _w_toolkit_post(w_toolkit *toolkit, w_event *event);
wbool _w_toolkit_post_quit(w_toolkit *toolkit, int quit);
wbool _w_toolkit_read_and_dispatch(w_toolkit *toolkit);
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *point);
int _w_toolkit_run(w_toolkit *toolkit);
wbool _w_toolkit_sleep(w_toolkit *toolkit);
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
        void *args);
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
        w_thread_start function, void *args);
wresult _w_toolkit_update(w_toolkit *toolkit);
wresult _w_toolkit_wake(w_toolkit *toolkit);
#endif
#endif /* toolkit_h */
