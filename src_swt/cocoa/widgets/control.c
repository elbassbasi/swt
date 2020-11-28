/*
 * control.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "control.h"
#include "toolkit.h"
#include "menu.h"
wresult _w_control_create(w_widget *widget, w_widget *parent, wuint64 style,
        w_widget_post_event_proc post_event) {
    wresult ret;
    if (parent == 0)
        return W_ERROR_NULL_ARGUMENT;
    if (!w_widget_is_ok(parent))
        return W_ERROR_INVALID_ARGUMENT;
    if (w_widget_class_id(parent) < _W_CLASS_COMPOSITE)
        return W_ERROR_INVALID_ARGUMENT;
    _w_widget_reserved *preserved = _w_widget_get_reserved(parent);
    preserved->check_open(parent, preserved);
    _W_CONTROL(widget)->parent = W_COMPOSITE(parent);
    _W_WIDGET(widget)->post_event = post_event;
    _w_widget_reserved *reserved = _w_widget_get_reserved(widget);
    _W_WIDGET(widget)->style = reserved->check_style(widget, style);
    _W_WIDGET(widget)->state |= STATE_TAB_LIST;
    return reserved->create_widget(widget,reserved);
}
wuint64 _w_control_check_style(w_widget *control, wuint64 style){
    return style;
}
void _w_control_check_orientation(w_control *control,w_composite* parent,_w_control_reserved* reserved){
    
}
void _w_control_check_open(w_widget *widget, _w_widget_reserved *reserved){
    
}
void _w_control_check_background(w_control *control,_w_control_reserved* reserved){
    
}
void _w_control_check_buffered(w_control *control,_w_control_reserved* reserved){
    
}
void _w_control_set_default_font(w_control *control,_w_control_reserved* reserved){
    
}
void _w_control_set_zorder(w_control *control,_w_control_reserved* reserved){
    NSView* topView = reserved->top_view(W_WIDGET(control));
    _w_control_reserved* preserved = _W_CONTROL_GET_RESERVED(_W_CONTROL(control)->parent);
    NSView* contentView = preserved->content_view(W_WIDGET(_W_CONTROL(control)->parent),preserved);
    NSView_addSubview_0(contentView, topView, NSWindowBelow, 0);
}
void _w_control_set_relations(w_control *control,_w_control_reserved* reserved){
    
}
void _w_control_set_background_0(w_control *control,_w_control_reserved* reserved){
    
}
wresult _w_control_create_widget (w_widget* widget, _w_widget_reserved *reserved) {
    _w_control_reserved* creserved = _W_CONTROL_RESERVED(reserved);
    _W_WIDGET(widget)->state |= STATE_DRAG_DETECT;
    creserved->check_orientation(W_CONTROL(widget),_W_CONTROL(widget)->parent,creserved);
    wresult ret =_w_widget_create_widget(widget,reserved);
    if(ret <= 0) return ret;
    creserved->check_background(W_CONTROL(widget),creserved);
    creserved->check_buffered(W_CONTROL(widget),creserved);
    creserved->set_default_font(W_CONTROL(widget),creserved);
    creserved->set_zorder(W_CONTROL(widget),creserved);
    creserved->set_relations(W_CONTROL(widget),creserved);
    if ((_W_WIDGET(widget)->state & STATE_PARENT_BACKGROUND) != 0) {
        creserved->set_background(W_CONTROL(widget),creserved);
    }
    //display.clearPool ();
    return ret;
}
w_widget* NSView_get_widget(NSView* view){
    if(view == 0) return 0;
    Class clazz = NSObject_class(NSOBJECT(view));
    Class* classes = mac_toolkit->nsclasses;
    for(size_t i=0;i<_NS_CLASS_LAST;i++){
        if(classes[i]==clazz){
            return ns_get_widgets[i](view);
        }
    }
    return 0;
}
w_widget* NSView_get_widget_0(NSView* view,size_t* index){
    if(view == 0) return 0;
    Class clazz = NSObject_class(NSOBJECT(view));
    Class* classes = mac_toolkit->nsclasses;
    for(size_t i=0;i<_NS_CLASS_LAST;i++){
        if(classes[i]==clazz){
            *index = i;
            return ns_get_widgets[i](view);
        }
    }
    *index = -1;
    return 0;
}
NSView* _w_control_get_handle(w_widget* widget){
    return (NSView*)_W_WIDGET(widget)->handle;
}
NSView* _w_control_content_view(w_widget* widget,_w_control_reserved* reserved){
    return reserved->get_view(widget);
}
w_cursor* _w_control_find_cursor (w_widget* widget,_w_control_reserved* reserved){
        if (_W_CONTROL(widget)->cursor != 0) return _W_CONTROL(widget)->cursor;
    w_composite* parent;
    w_control_get_parent(W_CONTROL(widget),&parent);
    _w_widget_reserved* preserved = _w_widget_get_reserved(W_WIDGET(parent));
    return _W_CONTROL_RESERVED(preserved)->find_cursor(W_WIDGET(parent),preserved);
}
int _w_widget_update_cursor(w_widget* widget){
    _w_widget_reserved* reserved = _w_widget_get_reserved(widget);
    w_cursor *cursor=_W_CONTROL_RESERVED(reserved)->find_cursor(widget,reserved);
    if(cursor != 0 && _W_CURSOR(cursor)->handle!= 0 ){
        NSCursor_set(_W_CURSOR(cursor)->handle);
    }
}

/*
 
 */
wresult _w_control_get_graphics(w_control *control, w_graphics *gc){}
wresult _w_control_get_orientation(w_control *control){}
wresult _w_control_set_orientation(w_control *control, int orientation){}
wresult _w_control_get_text_direction(w_control *control){}
wresult _w_control_set_text_direction(w_control *control, int textDirection){}
wresult _w_control_print(w_control *control, w_graphics *gc){}
wresult _w_control_get_accessible(w_control *control, w_accessible **accessible){}
wresult _w_control_get_bounds(w_control *control,w_point* location,w_size* size){}
wresult _w_control_set_bounds(w_control *control,w_point* location,w_size* size){
    /*
    * Bug in Cocoa. On Mac 10.8, a text control loses and gains focus
    * when its bounds changes.  The fix is to ignore these events.
    */
    w_control* oldIgnoreFocusControl = mac_toolkit->ignoreFocusControl;
    mac_toolkit->ignoreFocusControl = control;
    _w_control_reserved* reserved = _W_CONTROL_GET_RESERVED(control);
    NSView* topView = reserved->top_view(W_WIDGET(control));
    if (location!=0 && size!=0) {
        NSView_setFrame_0(topView, location,size);
    } else if (location!=0) {
        NSView_setFrameOrigin(topView, location);
    } else if (size!=0) {
        NSView_setFrameSize(topView, size);
    }
    mac_toolkit->ignoreFocusControl = oldIgnoreFocusControl;
}
wresult _w_control_get_region(w_control *control, w_region *region){}
wresult _w_control_set_region(w_control *control, w_region *region){}
wresult _w_control_move_above(w_control *control, w_control *_control){}
wresult _w_control_move_below(w_control *control, w_control *_control){}
wresult _w_control_pack(w_control *control, wbool changed){}
wresult _w_control_to_control(w_control *control, w_point *result, w_point *point){}
wresult _w_control_to_display(w_control *control, w_point *result, w_point *point){}
wresult _w_control_drag_detect(w_control *control, w_event_mouse *event){}
wresult _w_control_get_drag_detect(w_control *control){}
wresult _w_control_set_drag_detect(w_control *control, int dragDetect){}
wresult _w_control_set_focus(w_control *control){}
wresult _w_control_force_focus(w_control *control){}
wresult _w_control_is_focus_control(w_control *control){}
wresult _w_control_get_border_width(w_control *control){}
wresult _w_control_get_cursor(w_control *control, w_cursor **cursor){}
wresult _w_control_set_cursor(w_control *control, w_cursor *cursor){}
wresult _w_control_get_enabled(w_control *control){}
wresult _w_control_set_enabled(w_control *control, int enabled){}
wresult _w_control_is_enabled(w_control *control){}
wresult _w_control_get_font(w_control *control, w_font **font){}
wresult _w_control_set_font(w_control *control, w_font *font){}
wresult _w_control_get_background(w_control *control, w_color *background){}
wresult _w_control_set_background(w_control *control, w_color color){}
wresult _w_control_get_foreground(w_control *control, w_color *foreground){}
wresult _w_control_set_foreground(w_control *control, w_color color){}
wresult _w_control_get_layout_data(w_control *control, void **data){}
wresult _w_control_new_layout_data(w_control *control, void **data, size_t size){}
wresult _w_control_request_layout(w_control *control){}
wresult _w_control_get_menu(w_control *control, w_menu **menu){}
wresult _w_control_set_menu(w_control *control, w_menu *menu){}
wresult _w_control_get_parent(w_control *control, w_composite **parent){
    *parent = _W_CONTROL(control)->parent;
    return W_TRUE;
}
wresult _w_control_set_parent(w_control *control, w_composite *parent){}
wresult _w_control_is_reparentable(w_control *control){}
wresult _w_control_get_shell(w_control *control, w_shell **shell){
    w_composite* parent =_W_CONTROL(control)->parent;
    w_control_get_shell(W_CONTROL(parent), shell);
    return W_TRUE;
}
wresult _w_control_get_toolTip_text(w_control *control,w_alloc alloc,void* user_data){}
wresult _w_control_set_toolTip_text(w_control *control, const char *text,size_t length){}
wresult _w_control_get_touch_enabled(w_control *control){}
wresult _w_control_set_touch_enabled(w_control *control, int enabled){}
wresult _w_control_get_visible(w_control *control){
    return (_W_WIDGET(control)->state & STATE_HIDDEN) == 0;
}
wresult _w_control_set_visible(w_control *control, wbool visible){}
wresult _w_control_is_visible(w_control *control){}
wresult _w_control_redraw(w_control *control, w_rect *rect, int all){}
wresult _w_control_set_redraw(w_control *control, int redraw){}
wresult _w_control_set_capture(w_control *control, int capture){}
wresult _w_control_traverse(w_control *control, int traversal, w_event_key *event){}
wresult _w_control_update(w_control *control){}
wresult _w_control_set_timer(w_control *control, wint64 ms, wushort id){}
wresult _w_control_kill_timer(w_control *control, wushort id){}
wresult _w_control_set_tab(w_control *control, wbool tab){}
wresult _w_control_get_tab(w_control *control){}
wresult _w_control_create_dragsource_effect(w_control *control,
        w_dragsource_effect *effect){}
wresult _w_control_create_droptarget_effect(w_control *control,
        w_droptarget_effect *effect){}
void _w_control_mark_layout(w_control *control, int flags,
                            _w_control_reserved *reserved){
    
}
void _w_control_update_layout(w_control *control, int flags,
                              _w_control_reserved *reserved){
    
}
/*
 * signals
 */

int _w_control_post_event(struct w_widget *widget, struct w_event *ee) {
	return _w_widget_post_event(widget, ee);

}
void _w_control_class_init(struct _w_control_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	//clazz
    W_WIDGET_CLASS(clazz)->create = _w_control_create;
	clazz->get_graphics = _w_control_get_graphics;
	clazz->get_orientation = _w_control_get_orientation;
	clazz->set_orientation = _w_control_set_orientation;
	clazz->get_text_direction = _w_control_get_text_direction;
	clazz->set_text_direction = _w_control_set_text_direction;
	clazz->print = _w_control_print;
	clazz->get_accessible = _w_control_get_accessible;
	clazz->get_bounds = _w_control_get_bounds;
	clazz->set_bounds = _w_control_set_bounds;
	clazz->get_region = _w_control_get_region;
	clazz->set_region = _w_control_set_region;
	clazz->move_above = _w_control_move_above;
	clazz->move_below = _w_control_move_below;
	clazz->pack = _w_control_pack;
	clazz->to_control = _w_control_to_control;
	clazz->to_display = _w_control_to_display;
	clazz->drag_detect = _w_control_drag_detect;
	clazz->get_drag_detect = _w_control_get_drag_detect;
	clazz->set_drag_detect = _w_control_set_drag_detect;
	clazz->set_focus = _w_control_set_focus;
	clazz->force_focus = _w_control_force_focus;
	clazz->is_focus_control = _w_control_is_focus_control;
	clazz->get_border_width = _w_control_get_border_width;
	clazz->get_cursor = _w_control_get_cursor;
	clazz->set_cursor = _w_control_set_cursor;
	clazz->get_enabled = _w_control_get_enabled;
	clazz->set_enabled = _w_control_set_enabled;
	clazz->is_enabled = _w_control_is_enabled;
	clazz->get_font = _w_control_get_font;
	clazz->set_font = _w_control_set_font;
	clazz->get_background = _w_control_get_background;
	clazz->set_background = _w_control_set_background;
	clazz->get_foreground = _w_control_get_foreground;
	clazz->set_foreground = _w_control_set_foreground;
	clazz->get_layout_data = _w_control_get_layout_data;
	clazz->new_layout_data = _w_control_new_layout_data;
	clazz->request_layout = _w_control_request_layout;
	clazz->get_menu = _w_control_get_menu;
	clazz->set_menu = _w_control_set_menu;
	clazz->get_parent = _w_control_get_parent;
	clazz->set_parent = _w_control_set_parent;
	clazz->is_reparentable = _w_control_is_reparentable;
	clazz->get_shell = _w_control_get_shell;
	clazz->get_toolTip_text = _w_control_get_toolTip_text;
	clazz->set_toolTip_text = _w_control_set_toolTip_text;
	clazz->get_touch_enabled = _w_control_get_touch_enabled;
	clazz->set_touch_enabled = _w_control_set_touch_enabled;
	clazz->get_visible = _w_control_get_visible;
	clazz->set_visible = _w_control_set_visible;
	clazz->is_visible = _w_control_is_visible;
	clazz->redraw = _w_control_redraw;
	clazz->set_redraw = _w_control_set_redraw;
	clazz->set_capture = _w_control_set_capture;
	clazz->traverse = _w_control_traverse;
	clazz->update = _w_control_update;
	clazz->set_timer = _w_control_set_timer;
	clazz->kill_timer = _w_control_kill_timer;
	/*
	 *  reserved
	 */
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
    _W_WIDGET_RESERVED(reserved)->create_widget = _w_control_create_widget;
    _W_WIDGET_RESERVED(reserved)->check_style = _w_control_check_style;
    _W_WIDGET_RESERVED(reserved)->check_open = _w_control_check_open;
    reserved->top_view = _w_control_get_handle;
    reserved->get_view = _w_control_get_handle;
    reserved->content_view = _w_control_content_view;
    reserved->find_cursor = _w_control_find_cursor;
    reserved->mark_layout = _w_control_mark_layout;
    reserved->update_layout = _w_control_update_layout;
    reserved->check_orientation= _w_control_check_orientation;
    reserved->check_background= _w_control_check_background;
    reserved->check_buffered= _w_control_check_buffered;
    reserved->set_default_font= _w_control_set_default_font;
    reserved->set_zorder= _w_control_set_zorder;
    reserved->set_relations= _w_control_set_relations;
    reserved->set_background= _w_control_set_background_0;
}
#endif

