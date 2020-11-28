//
// Created by azeddine on 27/08/2020.
//
#include "canvas.h"
#include "toolkit.h"
void wc_canvas_init_class(struct _w_canvas_class *clazz){
    struct _w_widget_class* canvasclass = w_toolkit_get_class(
            ((c_toolkit*) ctoolkit)->system_toolkit,
            _W_CLASS_CANVAS);
    if (canvasclass->class_id == 0)
        canvasclass->init_class((struct _w_widget_class*) canvasclass);
    _init_class __init = W_WIDGET_CLASS(clazz)->init_class;
    memcpy(clazz, canvasclass, sizeof(struct _w_composite_class));
    W_WIDGET_CLASS(clazz)->toolkit = ctoolkit;
    W_WIDGET_CLASS(clazz)->next_class = canvasclass;
    memset(W_WIDGET_CLASS(clazz)->reserved, 0, sizeof(W_WIDGET_CLASS(clazz)->reserved));
    W_WIDGET_CLASS(clazz)->init_class = __init;
    W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_CANVAS;
}
