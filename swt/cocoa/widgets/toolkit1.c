//
//  toolkit1.c
//  cswt
//
//  Created by elbassbasi on 7/21/20.
//
#ifdef __APPLE__
#include "toolkit.h"
#import <objc/runtime.h>
void* _w_toolkit_malloc(size_t size){
    return malloc(size);
}
void _w_toolkit_free(void* ptr){
    free(ptr);
}
void _w_toolkit_add_shell(_w_shell *shell) {
    shell->next = 0;
    if (mac_toolkit->shells == 0) {
        mac_toolkit->shells = shell;
        shell->prev = shell; //last
    } else {
        _w_shell *last = mac_toolkit->shells->prev;
        last->next = shell;
        shell->prev = last;
        mac_toolkit->shells->prev = shell;
    }
    mac_toolkit->shells_count++;
}
void _w_toolkit_remove_shell(_w_shell *shell) {
    if (shell == mac_toolkit->shells) {
        mac_toolkit->shells = shell->next;
        if (mac_toolkit->shells != 0)
            mac_toolkit->shells->prev = shell->prev; //last
    } else {
        if (shell->next == 0) {
            mac_toolkit->shells->prev = shell->prev;
        } else {
            shell->next->prev = shell->prev;
        }
        shell->prev->next = shell->next;
    }
    mac_toolkit->shells_count--;
}
void _w_toolkit_registre_free(w_widget *widget) {
    widget->handle = mac_toolkit->widget_free; //widget->next =
    mac_toolkit->widget_free = widget;
}
void _w_toolkit_registre_class(int _id,Class clazz){
    if(mac_toolkit->nsclasses[_id]==0){
        mac_toolkit->nsclasses[_id]=clazz;
    }
}
void* _w_toolkit_oldcursor_set_proc_call (NSObject* id, SEL sel);
void* _w_toolkit_cursor_set_proc (NSObject* id, SEL sel) {
    if (mac_toolkit->lockCursor) {
        if (mac_toolkit->currentControl != 0) {
            if(mac_toolkit->currentControl->widget.clazz != 0){
                _w_control_reserved* reserved =_W_CONTROL_RESERVED( _w_widget_get_reserved(W_WIDGET(mac_toolkit->currentControl)));
                w_cursor* cursor = reserved->find_cursor(W_WIDGET(mac_toolkit->currentControl),_W_WIDGET_RESERVED(reserved));
                if (cursor != 0 && _W_CURSOR(cursor)->handle != id) return 0;
            }
        }
    }
    _w_toolkit_oldcursor_set_proc_call (id, sel);
    return 0;
}
ns_get_widget ns_get_widgets[_NS_CLASS_LAST]={
    SWTView_get_widget,//NSView
    SWTButton_get_widget,//NSButton
    SWTOutlineView_get_widget,//NSOutlineView
    
};
void _w_toolkit_class_init(_w_toolkit *toolkit) {
    struct _w_toolkit_class *clazz = &toolkit->class_toolkit;
    ((w_disposable*) toolkit)->clazz = (struct w_disposable_class*) clazz;
    clazz->disposable.dispose = _w_toolkit_dispose;
    clazz->check_widget = _w_toolkit_check_widget;
    clazz->check_widgetdata = _w_toolkit_check_widgetdata;
    clazz->get_class = _w_toolkit_get_class;
    clazz->get_theme = _w_toolkit_get_theme;
    clazz->async_exec = _w_toolkit_async_exec;
    clazz->beep = _w_toolkit_beep;
    clazz->close = _w_toolkit_close;
    clazz->get_active_shell = _w_toolkit_get_active_shell;
    clazz->get_menubar = _w_toolkit_get_menubar;
    clazz->get_bounds = _w_toolkit_get_bounds;
    clazz->get_client_area = _w_toolkit_get_client_area;
    clazz->get_cursor_control = _w_toolkit_get_cursor_control;
    clazz->get_cursor_location = _w_toolkit_get_cursor_location;
    clazz->get_cursor_sizes = _w_toolkit_get_cursor_sizes;
    clazz->get_dismissal_alignment = _w_toolkit_get_dismissal_alignment;
    clazz->get_double_click_time = _w_toolkit_get_double_click_time;
    clazz->get_focus_control = _w_toolkit_get_focus_control;
    clazz->get_high_contrast = _w_toolkit_get_high_contrast;
    clazz->get_icon_depth = _w_toolkit_get_icon_depth;
    clazz->get_icon_sizes = _w_toolkit_get_icon_sizes;
    clazz->get_shells = _w_toolkit_get_shells;
    clazz->get_system_color = _w_toolkit_get_system_color;
    clazz->get_system_cursor = _w_toolkit_get_system_cursor;
    clazz->get_system_font = _w_toolkit_get_system_font;
    clazz->get_system_image = _w_toolkit_get_system_image;
    clazz->get_system_menu = _w_toolkit_get_system_menu;
    clazz->get_system_taskbar = _w_toolkit_get_system_taskbar;
    clazz->get_system_tray = _w_toolkit_get_system_tray;
    clazz->get_thread = _w_toolkit_get_thread;
    clazz->get_thread_id = _w_toolkit_get_thread_id;
    clazz->get_touch_enabled = _w_toolkit_get_touch_enabled;
    clazz->map_0 = _w_toolkit_map_0;
    clazz->map_1 = _w_toolkit_map_1;
    clazz->post = _w_toolkit_post;
    clazz->post_quit = _w_toolkit_post_quit;
    clazz->read_and_dispatch = _w_toolkit_read_and_dispatch;
    clazz->set_cursor_location = _w_toolkit_set_cursor_location;
    clazz->run = _w_toolkit_run;
    clazz->sleep = _w_toolkit_sleep;
    clazz->sync_exec = _w_toolkit_sync_exec;
    clazz->timer_exec = _w_toolkit_timer_exec;
    clazz->update = _w_toolkit_update;
    clazz->wake = _w_toolkit_wake;
}
typedef void (*__w_widget_init_class)(struct _w_widget_class *clazz);

#define _W_TOOLKIT_CLASS_INIT(x,X) ((struct _w_widget_class *)&toolkit->class_##x)->init_class =(__w_widget_init_class) _w_##x##_class_init; \
    ((struct _w_widget_class *)&toolkit->class_##x)->reserved[0] = &toolkit->class_##x##_reserved;\
    toolkit->classes[_W_CLASS_##X] =(struct _w_widget_class *) &toolkit->class_##x
#define _W_TOOLKIT_CLASS_ITEM_INIT(x,X) _W_TOOLKIT_CLASS_INIT(x,X);\
        toolkit->class_##x.class_##x##item = &toolkit->class_##x##item
void _w_toolkit_subclass_init(_w_toolkit *toolkit) {
    _W_TOOLKIT_CLASS_INIT(shell, SHELL);
    _W_TOOLKIT_CLASS_INIT(composite, COMPOSITE);
    _W_TOOLKIT_CLASS_INIT(canvas, CANVAS);
    _W_TOOLKIT_CLASS_INIT(sash, SASH);
    _W_TOOLKIT_CLASS_ITEM_INIT(tree, TREE);
    toolkit->class_tree.class_treecolumn = &toolkit->class_treecolumn;
     _W_TOOLKIT_CLASS_INIT(button, BUTTON);
     _W_TOOLKIT_CLASS_INIT(glcanvas, GLCANVAS);
     _W_TOOLKIT_CLASS_ITEM_INIT(menu, MENU);
     _W_TOOLKIT_CLASS_INIT(browser, BROWSER);
     _W_TOOLKIT_CLASS_ITEM_INIT(table, TABLE); //
     toolkit->class_table.class_tablecolumn = &toolkit->class_tablecolumn;
     _W_TOOLKIT_CLASS_ITEM_INIT(tabfolder, TABFOLDER);
     _W_TOOLKIT_CLASS_ITEM_INIT(combo, COMBO); //
     _W_TOOLKIT_CLASS_ITEM_INIT(coolbar, COOLBAR); //
     _W_TOOLKIT_CLASS_INIT(datetime, DATETIME); //
     _W_TOOLKIT_CLASS_ITEM_INIT(expandbar, EXPANDBAR); //
     _W_TOOLKIT_CLASS_INIT(group, GROUP); //
     _W_TOOLKIT_CLASS_INIT(label, LABEL); //
     _W_TOOLKIT_CLASS_INIT(link, LINK); //
     _W_TOOLKIT_CLASS_ITEM_INIT(list, LIST); //
     _W_TOOLKIT_CLASS_INIT(progressbar, PROGRESSBAR); //
     _W_TOOLKIT_CLASS_INIT(scale, SCALE); //
     _W_TOOLKIT_CLASS_INIT(slider, SLIDER); //
     _W_TOOLKIT_CLASS_INIT(spinner, SPINNER); //
     _W_TOOLKIT_CLASS_ITEM_INIT(taskbar, TASKBAR); //
     _W_TOOLKIT_CLASS_INIT(text, TEXT); //
     _W_TOOLKIT_CLASS_ITEM_INIT(toolbar, TOOLBAR); //
     _W_TOOLKIT_CLASS_INIT(tooltip, TOOLTIP); //
     _W_TOOLKIT_CLASS_INIT(tracker, TRACKER); //
     _W_TOOLKIT_CLASS_ITEM_INIT(tray, TRAY); //
     _W_TOOLKIT_CLASS_INIT(ccanvas, CCANVAS); //
}
#endif

