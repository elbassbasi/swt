//
// Created by azeddine on 27/08/2020.
//
#include "shell.h"
#include "toolkit.h"
wresult wc_shell_create(w_widget *widget, w_widget *parent, int style,
                          w_widget_post_event_proc post_event) {
    wresult result = widget->clazz->next_class->create(widget, parent, style,
                                                        post_event);
    return result;
}
void wc_shell_init_class(struct _w_shell_class *clazz){
    struct _w_widget_class *shellclass = w_toolkit_get_class(
            ((c_toolkit*) ctoolkit)->system_toolkit,
            _W_CLASS_SHELL);
    if (shellclass->class_id == 0)
        shellclass->init_class((struct _w_widget_class*) shellclass);
    _init_class __init = W_WIDGET_CLASS(clazz)->init_class;
    memcpy(clazz, shellclass, sizeof(struct _w_shell_class));
    W_WIDGET_CLASS(clazz)->toolkit = ctoolkit;
    W_WIDGET_CLASS(clazz)->next_class = shellclass;
    memset(W_WIDGET_CLASS(clazz)->reserved, 0, sizeof(W_WIDGET_CLASS(clazz)->reserved));
    W_WIDGET_CLASS(clazz)->init_class = __init;
    W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_SHELL;

    W_WIDGET_CLASS(clazz)->create = wc_shell_create;
}
