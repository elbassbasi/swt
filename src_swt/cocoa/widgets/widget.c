/*
 * widget.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "widget.h"
#include "toolkit.h"
/*
 *
 */
wuint64 _w_widget_check_bits(wuint64 style, wuint64 int0, wuint64 int1, wuint64 int2, wuint64 int3,wuint64 int4, wuint64 int5) {
	int mask = int0 | int1 | int2 | int3 | int4 | int5;
	if ((style & mask) == 0)
		style |= int0;
	if ((style & int0) != 0)
		style = (style & ~mask) | int0;
	if ((style & int1) != 0)
		style = (style & ~mask) | int1;
	if ((style & int2) != 0)
		style = (style & ~mask) | int2;
	if ((style & int3) != 0)
		style = (style & ~mask) | int3;
	if ((style & int4) != 0)
		style = (style & ~mask) | int4;
	if ((style & int5) != 0)
		style = (style & ~mask) | int5;
	return style;
}
_w_widget_reserved* _w_widget_get_reserved(w_widget *widget) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(widget);
	while (clazz->toolkit != (struct w_toolkit*) mac_toolkit) {
		clazz = clazz->next_class;
	}
	return (struct _w_widget_reserved*) clazz->reserved[0];
}
wresult _w_widget_send_msg(w_widget* widget,void* handle,int msg,void* d1,void* d2,void* d3){
    _w_event_platform e;
    e.event.widget = widget;
    e.event.type= W_EVENT_PLATFORM;
    e.event.platform_event = 0;
    e.event.time = 0;
    e.event.data = 0;
    e.handle = handle;
    e.msg = msg;
    e.result =0;
    e.args[0] = d1;
    e.args[1] = d2;
    e.args[2] = d3;
    _w_widget_send_event(widget,(w_event*)&e);
    return e.result;
}
wresult _w_widget_create_widget (w_widget *widget, _w_widget_reserved *reserved){
    wresult ret = reserved->create_handle(widget,reserved);
    if(ret <= 0) return ret;
    //setOrientation();
    return ret;
}
/*
 * public function
 */
w_widget_post_event_proc _w_widget_get_post_event(w_widget *widget) {
	if (W_WIDGET_CLASS(widget) != 0) {
		return ((struct _w_widget*) widget)->post_event;
	} else
		return 0;
}
w_widget_post_event_proc _w_widget_set_post_event(w_widget *widget,
		w_widget_post_event_proc post_event) {
	if (W_WIDGET_CLASS(widget) != 0) {
		w_widget_post_event_proc last = ((struct _w_widget*) widget)->post_event;
		((struct _w_widget*) widget)->post_event = post_event;
		return last;
	} else
		return 0;

}
int _w_widget_send_event(w_widget *widget, w_event *event) {
	if (((struct _w_widget*) widget)->post_event != 0) {
		return ((struct _w_widget*) widget)->post_event(widget, event);
	} else {
		return w_widget_default_post_event(widget, event);
	}
}

void _w_widget_set_id(w_widget *widget, int id) {
	if (W_WIDGET_CLASS(widget) != 0)
		((_w_widget*) widget)->_id = id;
}
int _w_widget_get_id(w_widget *widget) {
	if (W_WIDGET_CLASS(widget) != 0)
		return ((_w_widget*) widget)->_id;
	else
		return 0;
}
int _w_widget_get_style(w_widget *widget) {
	if (W_WIDGET_CLASS(widget) != 0)
		return ((_w_widget*) widget)->style;
	else
		return 0;
}
void* _w_widget_get_data(w_widget *widget, int index) {
	if (W_WIDGET_CLASS(widget) != 0 && index >= 0
			&& index
					< (sizeof(((_w_widget*) widget)->data)
							/ sizeof(((_w_widget*) widget)->data[0]))) {
		return ((_w_widget*) widget)->data[index];
	} else
		return 0;
}
void* _w_widget_set_data(w_widget *widget, int index, void *data) {
	if (W_WIDGET_CLASS(widget) != 0 && index >= 0
			&& index
					< (sizeof(((_w_widget*) widget)->data)
							/ sizeof(((_w_widget*) widget)->data[0]))) {
		void *last = ((_w_widget*) widget)->data[index];
		((_w_widget*) widget)->data[index] = data;
		return last;
	} else
		return 0;
}
/*
 * resource data public function
 */

wresult _w_widgetdata_is_ok(w_widgetdata *obj) {
	return W_TRUE;
}
void _w_widgetdata_close(w_widgetdata *obj) {

}
wresult _w_widget_is_ok(w_widget *widget) {
	return W_TRUE;
}
void __w_resource_dispose(w_widget *widget) {

}
void _w_widget_dispose(w_widget *widget) {
	struct _w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(widget));
}
/*
 *
 */
wresult _w_widget_post_event(w_widget *widget, w_event *ee) {
	_w_widget_reserved *reserved;
	switch (ee->type) {
	case W_EVENT_PLATFORM: {
		reserved = _w_widget_get_reserved(widget);
        return reserved->msgs[((_w_event_platform*)ee)->msg](widget,(_w_event_platform*)ee,reserved);
	}
		break;
	case W_EVENT_COMPUTE_SIZE:
		reserved = _w_widget_get_reserved(widget);
		return reserved->compute_size(widget, (w_event_compute_size*) ee,
				reserved);
		break;
	case W_EVENT_CLIENT_AREA:
		reserved = _w_widget_get_reserved(widget);
		return reserved->get_client_area(widget,
				(struct w_event_client_area*) ee, reserved);
		break;
	case W_EVENT_COMPUTE_TRIM:
		reserved = _w_widget_get_reserved(widget);
		return reserved->compute_trim(widget, (w_event_compute_trim*) ee,
				reserved);
		break;
    case W_EVENT_LAYOUTDETECT:{
        w_layout* layout;
        w_composite_get_layout(W_COMPOSITE(widget),&layout);
		w_layout_do_layout(layout,
				W_COMPOSITE(widget),W_TRUE);
		return W_TRUE;
        }
		break;
	}
	return W_FALSE;
}
int _w_msg_null (w_widget* widget,_w_event_platform* e,_w_widget_reserved *reserved){
    return W_FALSE;
}
void _w_widget_class_init(struct _w_widget_class *clazz) {
	//resource
	clazz->is_ok = _w_widget_is_ok;
	clazz->dispose = _w_widget_dispose;
	clazz->toolkit = (w_toolkit*) mac_toolkit;
	//widget
	clazz->post_event = _w_widget_post_event;
	/*
	 * reserved
	 */
	//reserved
	_w_widget_reserved *reserved = W_WIDGET_CLASS(clazz)->reserved[0];
    reserved->create_widget = _w_widget_create_widget;

	/*
	 * signals
	 */
    for (int i=0; i<_NS_LAST; i++) {
        reserved->msgs[i]=_w_msg_null;
    }
}
#endif

