/*
 * button.c
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */
#include "button.h"
wresult wc_button_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	wresult result = wc_control_create(widget, parent, style,
                                        post_event, sizeof(struct wc_button_priv));
	if (result < 0)
		return result;
	return result;
}
void wc_button_draw(w_widget *widget, w_graphics *gc) {
	struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	w_rect r;
	w_size sz;
	w_control_get_bounds(W_CONTROL(widget), &r.pt,&r.sz);
	r.x = 0;
	r.y = 0;
	w_theme *theme = w_toolkit_get_theme(w_widget_get_toolkit(widget));
	w_themedata data;
	data.clazz = W_THEME_CLASS_BUTTON;
	data.clientArea = 0;
	data.state = priv->state;
	data.style = w_widget_get_style(widget);
	w_theme_draw_background(theme,&data, gc, &r);
	w_theme_draw_text(theme,&data, gc, &r, priv->text, -1,
			W_THEME_DRAW_HCENTER | W_THEME_DRAW_VCENTER);
}
int wc_button_paint(w_widget *widget, w_event_paint *e) {
	wc_button_draw(widget, e->gc);
	return W_TRUE;
}
int wc_button_mousedown(w_widget *widget, w_event_mouse *e) {
	struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	priv->state |= W_PRESSED;
	w_graphics gc;
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(widget), &gc);
	wc_button_draw(widget, &gc);
	w_graphics_dispose(&gc);
	return W_TRUE;
}
int wc_button_mouseup(w_widget *widget, w_event_mouse *e) {
	struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(widget));
	priv->state &= ~W_PRESSED;
	w_graphics gc;
	w_graphics_init(&gc);
	w_control_get_graphics(W_CONTROL(widget), &gc);
	wc_button_draw(widget, &gc);
	w_graphics_dispose(&gc);
	return W_TRUE;
}
int wc_button_compute_size(w_widget *widget, w_event_compute_size *e) {
    if(e->wHint != W_DEFAULT && e->wHint != W_DEFAULT){
        e->size->width = e->wHint;
        e->size->height = e->hHint;
        return W_TRUE;
    }
    struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(widget));
    w_graphics gc;
    w_themedata data;
    w_rect bounds,result;
    w_graphics_init(&gc);
    w_control_get_graphics(W_CONTROL(widget), &gc);
    w_theme *theme = w_toolkit_get_theme(w_widget_get_toolkit(widget));
    data.clazz = W_THEME_CLASS_BUTTON;
    data.clientArea = 0;
    data.state = priv->state;
    data.style = w_widget_get_style(widget);
    w_theme_measure_text(theme,&data,&gc,0,&result,priv->text,-1,W_THEME_DRAW_CENTER);
    w_graphics_dispose(&gc);
    if(e->wHint == W_DEFAULT) e->size->width = result.width;
    else e->size->width = e->wHint;
    if(e->hHint == W_DEFAULT) e->size->height = result.height;
    else e->size->height = e->hHint;
    return W_TRUE;
}
int wc_button_post_event(w_widget *widget, w_event *e) {
	switch (e->type) {
	case W_EVENT_PAINT:
		return wc_button_paint(widget, (w_event_paint*) e);
		break;
	case W_EVENT_MOUSEDOWN:
		return wc_button_mousedown(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_MOUSEUP:
		return wc_button_mouseup(widget, (w_event_mouse*) e);
		break;
	case W_EVENT_COMPUTE_SIZE:
	    return wc_button_compute_size(widget,(w_event_compute_size*)e);
        break;
	}
	return widget->clazz->next_class->post_event(widget, e);
}
int wc_button_get_alignment(w_button *button) {
}
wresult wc_button_get_grayed(w_button *button) {
    struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(button));
    return priv->state & W_GRAYED != 0;
}
wresult wc_button_get_image(w_button *button, w_image *image) {
}
wresult wc_button_get_selection(w_button *button) {
    struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(button));
    return priv->state & W_SELECTED != 0;
}
wresult wc_button_get_text(w_button *button, w_alloc *text) {

}
wresult wc_button_set_alignment(w_button *button, int alignment) {
}
wresult wc_button_set_grayed(w_button *button, int grayed) {
    struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(button));
    if(grayed){
        priv->state |= W_GRAYED;
    }else{
        priv->state &= ~ W_GRAYED;
    }
    return W_TRUE;
}
wresult wc_button_set_image(w_button *button, w_image *image) {
}
wresult wc_button_set_selection(w_button *button, int selected) {
    struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(button));
    if(selected){
        priv->state |= W_SELECTED;
    }else{
        priv->state &= ~ W_SELECTED;
    }
    return W_TRUE;
}
wresult wc_button_set_text(w_button *button, const char *string) {
	struct wc_button_priv *priv = wc_control_get_priv(W_CONTROL(button));
	if (priv->text != 0) {
		free(priv->text);
	}
	priv->text = strdup(string);
	w_control_redraw(W_CONTROL(button), 0, W_FALSE);
	return W_TRUE;
}
void wc_button_init_class(struct _w_widget_class *clazz) {
    wc_ccanvas_init_class_priv(clazz,sizeof(struct wc_button_priv));
	clazz->create = wc_button_create;
	clazz->post_event = wc_button_post_event;

	W_BUTTON_CLASS(clazz)->get_alignment = wc_button_get_alignment;
	W_BUTTON_CLASS(clazz)->get_alignment = wc_button_get_alignment;
	W_BUTTON_CLASS(clazz)->get_grayed = wc_button_get_grayed;
	W_BUTTON_CLASS(clazz)->get_image = wc_button_get_image;
	W_BUTTON_CLASS(clazz)->get_selection = wc_button_get_selection;
	W_BUTTON_CLASS(clazz)->get_text = wc_button_get_text;
	W_BUTTON_CLASS(clazz)->set_alignment = wc_button_set_alignment;
	W_BUTTON_CLASS(clazz)->set_grayed = wc_button_set_grayed;
	W_BUTTON_CLASS(clazz)->set_image = wc_button_set_image;
	W_BUTTON_CLASS(clazz)->set_selection = wc_button_set_selection;
	W_BUTTON_CLASS(clazz)->set_text = wc_button_set_text;

}


