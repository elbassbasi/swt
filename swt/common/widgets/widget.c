/*
 * widget.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#include <swt.h>
int w_widget_check(w_widget *widget) {
	if (widget != 0 && widget->clazz != 0) {
		w_toolkit *toolkit = widget->clazz->toolkit;
		return W_TOOLKIT_GET_CLASS(toolkit)->check_widget(toolkit, widget);
	} else
		return W_ERROR_NO_HANDLES;
}

int w_widgetdata_check(w_widgetdata *obj) {
	if (obj != 0 && obj->clazz != 0) {
		w_toolkit *toolkit = obj->clazz->toolkit;
		return W_TOOLKIT_GET_CLASS(toolkit)->check_widgetdata(toolkit, obj);
	} else
		return W_ERROR_NO_HANDLES;
}
void w_widgetdata_init(w_widgetdata *obj) {
	obj->clazz = 0;
}
wbool w_widgetdata_is_ok(w_widgetdata *obj) {
	if (W_WIDGETDATA_CHECK(obj)) {
		return obj->clazz->is_ok(obj);
	} else
		return W_FALSE;
}
void w_widgetdata_close(w_widgetdata *obj) {
	if (W_WIDGETDATA_CHECK(obj)) {
		obj->clazz->close(obj);
		obj->clazz = 0;
	}
}
void w_widgetdata_copy(w_widgetdata *from, w_widgetdata *to) {
	w_widgetdata_close(to);
	if (W_WIDGETDATA_CHECK(from)) {
		return from->clazz->copy(from, to);
	}
}
wbool w_widgetdata_equals(w_widgetdata *obj1, w_widgetdata *obj2) {
	if (W_WIDGETDATA_CHECK(obj1)) {
		return obj1->clazz->equals(obj1, obj2);
	} else
		return W_FALSE;
}
void w_widgetdata_init_copy(w_widgetdata *from, w_widgetdata *to) {
	if (W_WIDGETDATA_CHECK(from)) {
		return from->clazz->copy(from, to);
	} else {
		to->clazz = 0;
	}
}
w_widget* w_item_get_parent_widget(w_item *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_ITEM_GET_CLASS(item)->get_parent_widget(item);
	} else
		return 0;
}
void* w_item_get_data(w_item *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_ITEM_GET_CLASS(item)->get_data(item);
	} else
		return 0;
}
int w_item_get_index(w_item *item) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_ITEM_GET_CLASS(item)->get_index(item);
	} else
		return -1;
}
wresult w_item_get_text(w_item *item,w_alloc alloc,void* user_data) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_ITEM_GET_CLASS(item)->get_text(item, alloc,user_data);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_item_set_data(w_item *item, void *data) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_ITEM_GET_CLASS(item)->set_data(item, data);
	} else
		return W_ERROR_NO_HANDLES;
}
wresult w_item_set_text(w_item *item, const char *string) {
	if (W_WIDGETDATA_CHECK(item)) {
		return W_ITEM_GET_CLASS(item)->set_text(item, string);
	} else
		return W_ERROR_NO_HANDLES;
}
void w_widget_init(w_widget *widget) {
	widget->clazz = 0;
	widget->ref = -1;
}
int w_widget_class_id(w_widget *widget) {
	if (widget != 0 && widget->clazz != 0)
		return widget->clazz->class_id;
	else
		return 0;
}
struct _w_widget_class* w_widget_get_class(w_widget *widget) {
	if (widget != 0)
		return widget->clazz;
	else
		return 0;
}
wresult w_widget_is_ok(w_widget *widget) {
	if (W_WIDGET_CHECK1(widget)) {
		return widget->clazz->is_ok(widget);
	} else
		return W_FALSE;
}
wresult w_widget_dispose(w_widget *widget) {
	if (W_WIDGET_CHECK(widget)) {
		widget->clazz->dispose(widget);
		widget->clazz = 0;
	}
}
void w_widget_free(w_widget *widget) {
	if (W_WIDGET_CHECK(widget)) {
		w_widget_post_event_proc post_event = widget->post_event;
		widget->clazz->dispose(widget);
		w_event e;
		e.type = W_EVENT_FREE_MEMORY;
		e.widget = widget;
		e.platform_event = 0;
		e.data = 0;
		if (post_event != 0) {
			post_event(widget, &e);
		} else
			free(widget);
	}
}
void w_widget_ref_create(w_widget *widget) {
	widget->ref = 0;
}
void w_widget_ref_inc(w_widget *widget) {
	if (widget->ref > 0) {
		watomic_fetch_add(&widget->ref,1);
	}
}
w_widget* w_widget_ref_dec(w_widget *widget) {
	if (widget->ref > 0) {
		watomic_fetch_sub(&widget->ref,1);
		if (widget->ref <= 0) {
			w_widget_post_event_proc post_event = widget->post_event;
			if (widget->clazz != 0) {
				widget->clazz->dispose(widget);
			}
			w_event e;
			e.type = W_EVENT_FREE_MEMORY;
			e.widget = widget;
			e.platform_event = 0;
			e.data = 0;
			if (post_event != 0) {
				post_event(widget, &e);
			} else
				free(widget);
		}
	}
	return widget;
}
struct w_toolkit* w_widget_get_toolkit(w_widget *widget) {
	if (W_WIDGET_CHECK1(widget)) {
		return widget->clazz->toolkit;
	} else
		return 0;
}
w_widget_post_event_proc w_widget_get_post_event(w_widget *widget) {
	if (W_WIDGET_CHECK1(widget)) {
		return widget->post_event;
	} else
		return 0;
}
w_widget_post_event_proc w_widget_set_post_event(w_widget *widget,
		w_widget_post_event_proc post_event) {
	if (W_WIDGET_CHECK(widget)) {
		w_widget_post_event_proc last = widget->post_event;
		widget->post_event = post_event;
		return last;
	} else
		return 0;
}
void w_widget_set_id(w_widget *widget, wuint id) {
	if (W_WIDGET_CHECK(widget)) {
		widget->id = id;
	}
}
wuint w_widget_get_id(w_widget *widget) {
	if (W_WIDGET_CHECK1(widget)) {
		return widget->id;
	} else
		return 0;
}
wuint64 w_widget_get_style(w_widget *widget) {
	if (W_WIDGET_CHECK1(widget)) {
		return widget->style;
	} else
		return 0;
}
void* w_widget_get_data(w_widget *widget, wuint index) {
	if (W_WIDGET_CHECK1(widget)
			&& index < sizeof(widget->data) / sizeof(widget->data[0])) {
		return widget->data[index];
	} else
		return 0;
}
void* w_widget_set_data(w_widget *widget, wuint index, void *data) {
	if (W_WIDGET_CHECK(widget)
			&& index < sizeof(widget->data) / sizeof(widget->data[0])) {
		void *last = widget->data[index];
		widget->data[index] = data;
		return last;
	} else
		return 0;
}
int w_widget_default_post_event(w_widget *widget, w_event *e) {
	if (W_WIDGET_CHECK(widget)) {
		return W_WIDGET_GET_CLASS(widget)->post_event(widget, e);
	} else
		return W_FALSE;
}
int w_widget_send_event(w_widget *widget, w_event *event) {
	if (W_WIDGET_CHECK(widget)) {
		if (widget->post_event != 0) {
			return widget->post_event(widget, event);
		} else {
			struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(widget);
			return clazz->post_event(widget, event);
		}
	} else
		return W_FALSE;
}
wresult _w_widget_create(w_widget *widget, w_toolkit *toolkit,
		w_widget *parent, wuint64 style, wuint class_id,
		w_widget_post_event_proc post_event) {
	struct _w_widget_class *clazz;
	w_event e;
	if (parent != 0 && !w_widget_is_ok(W_WIDGET(parent))) {
		return W_ERROR_INVALID_ARGUMENT;
	}
	w_widget_init(W_WIDGET(widget));
	if (toolkit == 0) {
		if (parent != 0) {
			toolkit = w_widget_get_toolkit(W_WIDGET(parent));
		} else {
			toolkit = w_toolkit_get_default();
		}
	}
	clazz = (struct _w_widget_class*) w_toolkit_get_class(toolkit, class_id);
	if (clazz == 0)
		return W_ERROR_INVALID_SUBCLASS;
	if (clazz->class_id == 0)
		clazz->init_class((struct _w_widget_class*) clazz);
	memset(&(widget->clazz), 0, clazz->object_used_size);
	widget->clazz = clazz;
	int ret = clazz->create(widget, parent, style, post_event);
	if (ret < 0) {
		widget->clazz = 0;
		if ((style & W_FREE_MEMORY) && post_event != 0) {
			e.type = W_EVENT_FREE_MEMORY;
			e.widget = widget;
			e.platform_event = 0;
			e.data = 0;
			post_event(widget, &e);
		}
	}
	return ret;
}
w_widget* _w_widget_new(w_toolkit *toolkit, w_widget *parent, wuint64 style,
		wuint class_id, w_widget_post_event_proc post_event) {
	struct _w_widget_class *clazz;
	w_toolkit *_t = toolkit;
	if (_t == 0) {
		if (parent != 0) {
			_t = w_widget_get_toolkit(parent);
			if (_t == 0) {
				return 0;
			}
		} else {
			_t = w_toolkit_get_default();
		}
	}
	clazz = (struct _w_widget_class*) w_toolkit_get_class(_t, class_id);
	if (clazz == 0)
		return 0;
	if (clazz->class_id == 0)
		clazz->init_class(clazz);
	w_widget *widget = (w_widget*) malloc(clazz->object_total_size);
	if (widget == 0)
		return 0;
	memset(&(widget->clazz), 0, clazz->object_used_size + sizeof(void*));
	widget->clazz = clazz;
	int ret = clazz->create(widget, parent, style, post_event);
	if (ret < 0) {
		free(widget);
		return 0;
	}
	return widget;
}
