//
// Created by azeddine on 27/08/2020.
//
#include "composite.h"
#include "toolkit.h"
void wc_composite_init_class(struct _w_composite_class *clazz){
    struct _w_widget_class *compositeclass = w_toolkit_get_class(
            ((c_toolkit*) ctoolkit)->system_toolkit,
            _W_CLASS_COMPOSITE);
    if (compositeclass->class_id == 0)
        compositeclass->init_class((struct _w_widget_class*) compositeclass);
    _init_class __init = W_WIDGET_CLASS(clazz)->init_class;
    memcpy(clazz, compositeclass, sizeof(struct _w_composite_class));
    W_WIDGET_CLASS(clazz)->toolkit = ctoolkit;
    W_WIDGET_CLASS(clazz)->next_class = compositeclass;
    memset(W_WIDGET_CLASS(clazz)->reserved, 0, sizeof(W_WIDGET_CLASS(clazz)->reserved));
    W_WIDGET_CLASS(clazz)->init_class = __init;
    W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMPOSITE;
}
void wc_composite_init_class_priv(struct _w_composite_class *clazz,size_t priv_size){
    struct _w_widget_class *next = W_WIDGET_CLASS(clazz)->next_class;
    if((next->object_used_size + priv_size) < next->object_total_size){
        WC_CLASS_RESERVED(clazz)->state |= WC_USED_INTERNAL_MEMORY;
        W_WIDGET_CLASS(clazz)->object_used_size += priv_size;
    }else{
        W_WIDGET_CLASS(clazz)->object_used_size += sizeof(void*);
    }
}
