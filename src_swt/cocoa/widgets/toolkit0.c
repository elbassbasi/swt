//
//  toolkit.c
//  cswt
//
//  Created by elbassbasi on 7/21/20.
//
#ifdef __APPLE__
#include "toolkit.h"
#include <pthread.h>
_w_toolkit *mac_toolkit;
w_toolkit* w_toolkit_get_platform() {
    if (mac_toolkit == 0) {
        mac_toolkit = malloc(sizeof(struct _w_toolkit));
        if (mac_toolkit == 0) {
            fprintf(stderr,
                    "SWT Fatal Error : Do not alloc memory for toolkit\n");
            exit(EXIT_FAILURE);
            return 0;
        }
        memset(mac_toolkit, 0, sizeof(struct _w_toolkit));
        _w_toolkit_init(mac_toolkit);
        w_app_dispose_registre((w_disposable*) mac_toolkit);
    }
    return W_TOOLKIT(mac_toolkit);
}
void _w_toolkit_dispose(w_disposable *_toolkit) {
    NSAutoreleasePool_release(_W_TOOLKIT(_toolkit)->pool);
}
wresult _w_toolkit_check_widget(w_toolkit *toolkit, w_widget *widget) {
    if(NSThread_isMainThread()){
         return W_TRUE;
    }else{
    if (pthread_equal(pthread_self(), (pthread_t)
    _W_TOOLKIT(toolkit)->guithread.id)) {
        return W_TRUE;
    } else {
        return W_ERROR_THREAD_INVALID_ACCESS;
    }
    }
}
wresult _w_toolkit_check_widgetdata(w_toolkit *toolkit,
        w_widgetdata *widgetdata) {
    if(NSThread_isMainThread()){
         return W_TRUE;
    }else{
    if (pthread_equal(pthread_self(), (pthread_t)
    _W_TOOLKIT(toolkit)->guithread.id)) {
        return W_TRUE;
    } else {
        return W_ERROR_THREAD_INVALID_ACCESS;
    }
    }
}
struct _w_widget_class* _w_toolkit_get_class(w_toolkit *toolkit,
        wushort clazz_id) {
    if (clazz_id >= _W_CLASS_LAST)
        return 0;
    else
        return ((_w_toolkit*) toolkit)->classes[clazz_id];
}
struct w_theme* _w_toolkit_get_theme(w_toolkit *toolkit) {
}
wresult _w_toolkit_async_exec(w_toolkit *toolkit, w_thread_start function,
        void *args) {

}
wresult _w_toolkit_beep(w_toolkit *toolkit) {
}
wresult _w_toolkit_close(w_toolkit *toolkit) {
}
w_shell* _w_toolkit_get_active_shell(w_toolkit *toolkit) {
}
w_menu* _w_toolkit_get_menubar(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_bounds(w_toolkit *toolkit, w_rect *bounds) {
}
wresult _w_toolkit_get_client_area(w_toolkit *toolkit, w_rect *clientArea) {
    return _w_toolkit_get_bounds(toolkit, clientArea);
}
w_control* _w_toolkit_get_cursor_control(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_cursor_location(w_toolkit *toolkit, w_point *location) {
}
size_t _w_toolkit_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
        size_t length) {
}
int _w_toolkit_get_dismissal_alignment(w_toolkit *toolkit) {
}
int _w_toolkit_get_double_click_time(w_toolkit *toolkit) {
}
w_control* _w_toolkit_get_focus_control(w_toolkit *toolkit) {
}
wbool _w_toolkit_get_high_contrast(w_toolkit *toolkit) {
}
int _w_toolkit_get_icon_depth(w_toolkit *toolkit) {
}
size_t _w_toolkit_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
        size_t length) {
}
_w_shell* _w_shells_iterator_find_next(_w_shell *parent, _w_shell *current) {
    if (parent == 0)
        return current->next;
    else {
        _w_shell *s = current->next;
        while (s != 0) {
            if (_W_CONTROL(s)->parent == (w_composite*) parent)
                return s;
            s = s->next;
        }
        return 0;
    }
}
_w_shell* _w_shells_iterator_find_first(_w_shell *parent) {
    _w_shell *s = mac_toolkit->shells;
    while (s != 0) {
        if (_W_CONTROL(s)->parent == (w_composite*) parent)
            return s;
        s = s->next;
    }
    return 0;
}
wbool _w_shells_iterator_next(w_iterator *it, void *obj) {
    if (_W_SHELLS_ITERATOR(it)->current != 0) {
        *((_w_shell**) obj) = _W_SHELLS_ITERATOR(it)->current;
        _W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_next(
        _W_SHELLS_ITERATOR(it)->parent,
        _W_SHELLS_ITERATOR(it)->current);
        return W_TRUE;
    } else {
        return W_FALSE;
    }
}
wresult _w_shells_iterator_reset(w_iterator *it) {
    if (_W_SHELLS_ITERATOR(it)->parent == 0)
        _W_SHELLS_ITERATOR(it)->current = mac_toolkit->shells;
    else
        _W_SHELLS_ITERATOR(it)->current = _w_shells_iterator_find_first(
        _W_SHELLS_ITERATOR(it)->parent);
    return W_TRUE;
}
wresult _w_shells_iterator_close(w_iterator *it) {
    return W_TRUE;
}
wresult _w_shells_iterator_remove(w_iterator *it) {
    return W_ERROR_NOT_IMPLEMENTED;
}
size_t _w_shells_iterator_count(w_iterator *it) {
    if (_W_SHELLS_ITERATOR(it)->parent == 0)
        return mac_toolkit->shells_count;
    else {
        if (_W_SHELLS_ITERATOR(it)->count == -1) {
            _w_shell *s = mac_toolkit->shells;
            size_t count = 0;
            while (s != 0) {
                if (_W_CONTROL(s)->parent
                        == (w_composite*) _W_SHELLS_ITERATOR(it)->parent)
                    count++;
                s = s->next;
            }
            _W_SHELLS_ITERATOR(it)->count = count;
        }
        return _W_SHELLS_ITERATOR(it)->count;
    }
}
_w_iterator_class _w_shells_iterator_class = { //
        _w_shells_iterator_close, //
                _w_shells_iterator_next, //
                _w_shells_iterator_reset, //
                _w_shells_iterator_remove, //
                _w_shells_iterator_count };
void _w_toolkit_get_shells(w_toolkit *toolkit, w_iterator *shells) {
    _W_SHELLS_ITERATOR(shells)->base.clazz = &_w_shells_iterator_class;
    _W_SHELLS_ITERATOR(shells)->current = mac_toolkit->shells;
    _W_SHELLS_ITERATOR(shells)->parent = 0;
    _W_SHELLS_ITERATOR(shells)->count = -1;
}
void _w_toolkit_get_shells_from_parent(w_shell *shell, w_iterator *iterator) {
    _W_SHELLS_ITERATOR(iterator)->base.clazz = &_w_shells_iterator_class;
    _W_SHELLS_ITERATOR(iterator)->current = _w_shells_iterator_find_first(
    _W_SHELLS_ITERATOR(iterator)->parent);
    _W_SHELLS_ITERATOR(iterator)->parent = shell;
    _W_SHELLS_ITERATOR(iterator)->count = -1;
}
w_color _w_toolkit_get_system_color(w_toolkit *toolkit, wuint id) {

}
w_cursor* _w_toolkit_get_system_cursor(w_toolkit *toolkit, wuint id) {
    if(id <= W_CURSOR_HAND){
        return (w_cursor*)(&_W_TOOLKIT(toolkit)->cursors[id]);
    }else return 0;
}
w_font* _w_toolkit_get_system_font(w_toolkit *toolkit) {
}
wresult _w_toolkit_get_system_image(w_toolkit *toolkit, wuint id,
        w_image *image) {
}
w_menu* _w_toolkit_get_system_menu(w_toolkit *toolkit) {
}
struct w_taskbar* _w_toolkit_get_system_taskbar(w_toolkit *toolkit) {
}
struct w_tray* _w_toolkit_get_system_tray(w_toolkit *toolkit) {
}
w_thread* _w_toolkit_get_thread(w_toolkit *toolkit) {
}
wuint64 _w_toolkit_get_thread_id(w_toolkit *toolkit) {
}
wbool _w_toolkit_get_touch_enabled(w_toolkit *toolkit) {
}
wresult _w_toolkit_map_0(w_toolkit *toolkit, w_control *from, w_control *to,
        w_point *result, w_point *point) {
}
wresult _w_toolkit_map_1(w_toolkit *toolkit, w_control *from, w_control *to,
        w_rect *result, w_rect *rectangle) {
}
wbool _w_toolkit_post(w_toolkit *toolkit, w_event *event) {
}
wbool _w_toolkit_post_quit(w_toolkit *toolkit, int quit) {
        _W_TOOLKIT(toolkit)->exit_code = quit;
    _W_TOOLKIT(toolkit)->exit_loop = 1;
    return W_TRUE;
}
wbool _w_toolkit_read_and_dispatch(w_toolkit *toolkit) {
    /*if (sendEventCount == 0 && loopCount == poolCount - 1 && Callback.getEntryCount () == 0) removePool ();
    addPool ();
    runSkin ();
    runDeferredLayouts ();
    loopCount++;
    boolean events = false;
        events |= runSettings ();
        events |= runTimers ();
        events |= runContexts ();
        events |= runPopups ();*/
    NSEvent* event = NSApplication_nextEventMatchingMask(_W_TOOLKIT(toolkit)->application, NSAnyEventMask, 0, NSDefaultRunLoopMode, W_TRUE);
        if ((event != 0) && (_W_TOOLKIT(toolkit)->application != 0)) {
            //events = true;
            NSApplication_sendEvent(_W_TOOLKIT(toolkit)->application, event);
        }
        /*events |= runPaint ();
        events |= runDeferredEvents ();
        if (!events) {
            events = isDisposed () || runAsyncMessages (false);
        }
        removePool ();
        loopCount--;
        if (sendEventCount == 0 && loopCount == poolCount && Callback.getEntryCount () == 0) addPool ();
    return events;*/
    w_event _e;
    w_widget *widget = ((_w_toolkit*) toolkit)->widget_free, *next;
    while (widget != 0) {
        w_widget_post_event_proc _proc =
                (w_widget_post_event_proc) widget->post_event;
        next = (w_widget*) widget->handle; //widget = widget->next
        if (_proc != 0) {
            _e.type = W_EVENT_FREE_MEMORY;
            _e.platform_event = 0;
            _e.data = 0;
            _e.widget = widget;
            _proc(widget, &_e);
        } else {
            free(widget);
        }
        widget = next; //widget = widget->next
    }
    ((_w_toolkit*) toolkit)->widget_free = 0;
    return W_TRUE;
}
wresult _w_toolkit_set_cursor_location(w_toolkit *toolkit, w_point *location) {
}
wbool _w_toolkit_sleep(w_toolkit *toolkit) {
    /*if (getMessageCount () != 0) return true;
    sendPreExternalEventDispatchEvent ();*/
        //addPool();
        //allowTimers = runAsyncMessages = false;
    NSRunLoop* loop = NSRunLoop_currentRunLoop();
    NSRunLoop_runMode(loop, NSDefaultRunLoopMode, NSDate_distantFuture());
        //allowTimers = runAsyncMessages = true;
        //removePool();
    //sendPostExternalEventDispatchEvent ();
    return W_TRUE;
}
int _w_toolkit_run(w_toolkit *toolkit) {
    _W_TOOLKIT(toolkit)->exit_loop = 0;
    while (_W_TOOLKIT(toolkit)->exit_loop == 0) {
        _w_toolkit_sleep(toolkit);
        _w_toolkit_read_and_dispatch(toolkit);
    }
    return _W_TOOLKIT(toolkit)->exit_code;
}
wresult _w_toolkit_sync_exec(w_toolkit *toolkit, w_thread_start function,
        void *args) {
}
wresult _w_toolkit_timer_exec(w_toolkit *toolkit, wuint milliseconds,
        w_thread_start function, void *args) {
}
wresult _w_toolkit_update(w_toolkit *toolkit) {
}
wresult _w_toolkit_wake(w_toolkit *toolkit) {
}
void _w_toolkit_init(_w_toolkit *toolkit) {
    /**
     * initialize class
     */
    _w_toolkit_class_init(toolkit);
    _w_toolkit_subclass_init(toolkit);
    _W_TOOLKIT(toolkit)->pool =NSAutoreleasePool_New();
    _W_TOOLKIT(toolkit)->application=NSApplication_sharedApplication();
    NSApp_Init();
    _W_TOOLKIT(toolkit)->guithread.id=pthread_self();
    NSCursor_get_systems((NSCursor**)toolkit->cursors);
    NSCursor_init();
}


#endif
