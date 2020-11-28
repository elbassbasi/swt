/*
 * ccanvas.c
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */
#include "ccanvas.h"
#include "toolkit.h"
wresult wc_control_create(w_widget *widget, w_widget *parent, wuint64 style,
                           w_widget_post_event_proc post_event, size_t sizepriv) {
	wresult result = widget->clazz->next_class->create(widget, parent, style, post_event);
	if(result > 0){
	    void* ptr = wc_control_alloc_priv(W_CONTROL(widget), sizepriv);
	    if(ptr == 0){
	        w_widget_dispose(widget);
	        return W_ERROR_NO_MEMORY;
	    }
	    memset(ptr,0,sizepriv);
	}
	return result;
}
int wc_canvas_post_event(w_widget *widget, w_event *e) {
	return widget->clazz->next_class->post_event(widget, e);
}
void* wc_control_get_priv(w_control *c) {
    struct _w_widget_class *clazz = c->widget.clazz;
    void* ptr = ((void*) c) + clazz->next_class->object_used_size;
    if(WC_CLASS_RESERVED(clazz)->state & WC_USED_INTERNAL_MEMORY){
        return ptr;
    }else{
        return  *((void**)ptr);
    }
}
void* wc_control_alloc_priv(w_control *c, size_t size){
    struct _w_widget_class *clazz = c->widget.clazz;
    void* ptr = ((void*) c) + clazz->next_class->object_used_size;
    if(WC_CLASS_RESERVED(clazz)->state & WC_USED_INTERNAL_MEMORY){
        return ptr;
    }else{
        void** priv = (void**)ptr;
        *priv = malloc(size);
        return *priv;
    }
}
void wc_ccanvas_init_class(struct _w_widget_class *clazz) {
	struct _w_widget_class *ccanvasclass = w_toolkit_get_class(
			((c_toolkit*) ctoolkit)->system_toolkit,
			_W_CLASS_CCANVAS);
	if (ccanvasclass->class_id == 0)
		ccanvasclass->init_class((struct _w_widget_class*) ccanvasclass);
	_init_class __init = clazz->init_class;
	memcpy(clazz, ccanvasclass, sizeof(struct _w_ccanvas_class));
	clazz->toolkit = W_TOOLKIT(ctoolkit);
	clazz->next_class = ccanvasclass;
	memset(clazz->reserved, 0, sizeof(clazz->reserved));
	clazz->init_class = __init;
}
void wc_ccanvas_init_class_priv(struct _w_widget_class *clazz,size_t sizeof_priv){
    wc_ccanvas_init_class(clazz);
    struct _w_widget_class *next = clazz->next_class;
    if((next->object_used_size + sizeof_priv) > next->object_total_size){
        WC_CLASS_RESERVED(clazz)->state |= WC_USED_INTERNAL_MEMORY;
        clazz->object_used_size += sizeof_priv;
    }else{
        clazz->object_used_size += sizeof(void*);
    }
}


