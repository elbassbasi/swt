/*
 * composite.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>
wresult w_composite_create(w_composite *composite, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event) {
	return _w_widget_create(W_WIDGET(composite), toolkit, W_WIDGET(parent),
			style, _W_CLASS_COMPOSITE, post_event);
}
w_composite* w_composite_new(struct w_toolkit *toolkit, w_composite *parent,
                             wuint64 style, w_widget_post_event_proc post_event) {
	return W_COMPOSITE(
			_w_widget_new(toolkit, W_WIDGET(parent), style,_W_CLASS_COMPOSITE, post_event));
}
wresult w_composite_get_children(w_composite *composite, w_iterator *it) {
	w_iterator_close(it);
    wresult result = W_WIDGET_CHECK0(composite);
	if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->get_children(composite, it);
    }else return result;
}
wresult w_composite_get_layout(w_composite *composite,w_layout** layout) {
    wresult result = W_WIDGET_CHECK0(composite);
    if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->get_layout(composite,layout);
    } else{
        *layout = 0;
        return result;
    }
}
wresult w_composite_get_layout_deferred(w_composite *composite) {
    wresult result = W_WIDGET_CHECK0(composite);
    if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->get_layout_deferred(composite);
	}  else
           return result;
}
wresult w_composite_get_tab_list(w_composite *composite, w_iterator *it) {
	w_iterator_close(it);
    wresult result = W_WIDGET_CHECK0(composite);
    if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->get_tab_list(composite, it);
	} else
           return result;
}
wresult w_composite_is_layout_deferred(w_composite *composite) {
    wresult result = W_WIDGET_CHECK0(composite);
    if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->is_layout_deferred(composite);
	} else
		return result;
}
wresult w_composite_do_layout(w_composite *composite, int changed, int all) {
    wresult result = W_WIDGET_CHECK0(composite);
    if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->do_layout(composite, changed, all);
	} else
           return result;
}
wresult w_composite_set_layout(w_composite *composite, w_layout *layout) {
    wresult result = W_WIDGET_CHECK0(composite);
    if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->set_layout(composite, layout);
	} else
           return result;
}
wresult w_composite_set_layout_deferred(w_composite *composite, int defer) {
    wresult result = W_WIDGET_CHECK0(composite);
    if (result >= 0) {
		return W_COMPOSITE_GET_CLASS(composite)->set_layout_deferred(composite,
				defer);
	} else
           return result;
}
