/*
 * control.h
 *
 *  Created on: 25 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_WIDGETS_CONTROL_H_
#define SWT_WIDGETS_CONTROL_H_
#include "menu.h"
#include "dnd.h"
#ifdef __cplusplus
extern "C" {
#endif
struct w_control {
	w_widget widget;
	void *handle[0x20];
};

#define W_CONTROL(x) ((w_control*)x)
typedef struct w_layout_data {
	wushort size;
	wushort sign;
} w_layout_data;
typedef struct w_accessible w_accessible;
struct _w_control_class {
	struct _w_widget_class widget;
	wresult (*get_graphics)(w_control *control, w_graphics *gc);
	wresult (*get_orientation)(w_control *control);
	wresult (*set_orientation)(w_control *control, int orientation);
	wresult (*get_text_direction)(w_control *control);
	wresult (*set_text_direction)(w_control *control, int textDirection);
	wresult (*print)(w_control *control, w_graphics *gc);
	wresult (*get_accessible)(w_control *control, w_accessible **accessible);
	wresult (*get_bounds)(w_control *control, w_point *location, w_size *size);
	wresult (*set_bounds)(w_control *control, w_point *location, w_size *size);
	wresult (*get_region)(w_control *control, w_region *region);
	wresult (*set_region)(w_control *control, w_region *region);
	wresult (*move_above)(w_control *control, w_control *_control);
	wresult (*move_below)(w_control *control, w_control *_control);
	wresult (*pack)(w_control *control, int changed);
	wresult (*to_control)(w_control *control, w_point *result, w_point *point);
	wresult (*to_display)(w_control *control, w_point *result, w_point *point);
	wresult (*drag_detect)(w_control *control, w_event_mouse *event);
	wresult (*get_drag_detect)(w_control *control);
	wresult (*set_drag_detect)(w_control *control, int dragDetect);
	wresult (*set_focus)(w_control *control);
	wresult (*force_focus)(w_control *control);
	wresult (*is_focus_control)(w_control *control);
	wresult (*get_border_width)(w_control *control);
	wresult (*get_cursor)(w_control *control, w_cursor **cursor);
	wresult (*set_cursor)(w_control *control, w_cursor *cursor);
	wresult (*get_enabled)(w_control *control);
	wresult (*set_enabled)(w_control *control, int enabled);
	wresult (*is_enabled)(w_control *control);
	wresult (*get_font)(w_control *control, w_font **font);
	wresult (*set_font)(w_control *control, w_font *font);
	wresult (*get_background)(w_control *control, w_color *background);
	wresult (*set_background)(w_control *control, w_color color);
	wresult (*get_foreground)(w_control *control, w_color *foreground);
	wresult (*set_foreground)(w_control *control, w_color color);
	wresult (*get_layout_data)(w_control *control, void **data);
	wresult (*new_layout_data)(w_control *control, void **data, size_t size);
	wresult (*request_layout)(w_control *control);
	wresult (*get_menu)(w_control *control, w_menu **menu);
	wresult (*set_menu)(w_control *control, w_menu *menu);
	wresult (*get_parent)(w_control *control, w_composite **parent);
	wresult (*set_parent)(w_control *control, w_composite *parent);
	wresult (*is_reparentable)(w_control *control);
	wresult (*get_shell)(w_control *control, w_shell **shell);
	wresult (*get_toolTip_text)(w_control *control, w_alloc alloc,
			void *user_data);
	wresult (*set_toolTip_text)(w_control *control, const char *text,
			size_t length);
	wresult (*get_touch_enabled)(w_control *control);
	wresult (*set_touch_enabled)(w_control *control, int enabled);
	wresult (*get_visible)(w_control *control);
	wresult (*set_visible)(w_control *control, int visible);
	wresult (*is_visible)(w_control *control);
	wresult (*redraw)(w_control *control, w_rect *rect, int all);
	wresult (*set_redraw)(w_control *control, int redraw);
	wresult (*set_capture)(w_control *control, int capture);
	wresult (*traverse)(w_control *control, int traversal, w_event_key *event);
	wresult (*update)(w_control *control);
	wresult (*set_timer)(w_control *control, wint64 ms, wushort id);
	wresult (*kill_timer)(w_control *control, wushort id);
	wresult (*set_tab)(w_control *control, int tab);
	wresult (*get_tab)(w_control *control);
	wresult (*create_dragsource_effect)(w_control *control,
			w_dragsource_effect *effect);
	wresult (*create_droptarget_effect)(w_control *control,
			w_droptarget_effect *effect);
	void *reserved[2];
};
struct _w_ccanvas_class {
	struct _w_control_class control;

};
SWT_PUBLIC wresult w_control_get_graphics(w_control *control, w_graphics *gc);
SWT_PUBLIC wresult w_control_get_orientation(w_control *control);
SWT_PUBLIC wresult w_control_set_orientation(w_control *control,
		int orientation);
SWT_PUBLIC wresult w_control_get_text_direction(w_control *control);
SWT_PUBLIC wresult w_control_set_text_direction(w_control *control,
		int textDirection);
SWT_PUBLIC wresult w_control_print(w_control *control, w_graphics *gc);
SWT_PUBLIC wresult w_control_compute_size(w_control *control, w_size *size,
		int wHint, int hHint);
SWT_PUBLIC wresult w_control_compute_size_0(w_control *control, w_size *size,
		int wHint, int hHint, int changed);
SWT_PUBLIC wresult w_control_get_accessible(w_control *control,
		w_accessible **accessible);
SWT_PUBLIC wresult w_control_get_bounds(w_control *control,w_point* location,w_size* size);
SWT_PUBLIC wresult w_control_set_bounds(w_control *control,w_point* location,w_size* size);
SWT_PUBLIC wresult w_control_set_bounds_0(w_control *control, int x, int y,
		int width, int heigth);
SWT_PUBLIC wresult w_control_get_region(w_control *control, w_region *region);
SWT_PUBLIC wresult w_control_set_region(w_control *control, w_region *region);
SWT_PUBLIC wresult w_control_move_above(w_control *control,
		w_control *_control);
SWT_PUBLIC wresult w_control_move_below(w_control *control,
		w_control *_control);
SWT_PUBLIC wresult w_control_pack(w_control *control, int changed);
SWT_PUBLIC wresult w_control_to_control(w_control *control, w_point *result,
		w_point *point);
SWT_PUBLIC wresult w_control_to_display(w_control *control, w_point *result,
		w_point *point);
SWT_PUBLIC wresult w_control_drag_detect(w_control *control,
		w_event_mouse *event);
SWT_PUBLIC wresult w_control_get_drag_detect(w_control *control);
SWT_PUBLIC wresult w_control_set_drag_detect(w_control *control,
		int dragDetect);
SWT_PUBLIC wresult w_control_set_focus(w_control *control);
SWT_PUBLIC wresult w_control_force_focus(w_control *control);
SWT_PUBLIC wresult w_control_is_focus_control(w_control *control);
SWT_PUBLIC wresult w_control_get_border_width(w_control *control);
SWT_PUBLIC wresult w_control_get_cursor(w_control *control, w_cursor **cursor);
SWT_PUBLIC wresult w_control_set_cursor(w_control *control, w_cursor *cursor);
SWT_PUBLIC wresult w_control_get_enabled(w_control *control);
SWT_PUBLIC wresult w_control_set_enabled(w_control *control, int enabled);
SWT_PUBLIC wresult w_control_is_enabled(w_control *control);
SWT_PUBLIC wresult w_control_get_font(w_control *control, w_font **font);
SWT_PUBLIC wresult w_control_set_font(w_control *control, w_font *font);
SWT_PUBLIC wresult w_control_get_background(w_control *control,
		w_color *background);
SWT_PUBLIC wresult w_control_set_background(w_control *control, w_color color);
SWT_PUBLIC wresult w_control_get_foreground(w_control *control,
		w_color *foreground);
SWT_PUBLIC wresult w_control_set_foreground(w_control *control, w_color color);
SWT_PUBLIC wresult w_control_get_layout_data(w_control *control, void **data);
SWT_PUBLIC wresult w_control_new_layout_data(w_control *control, void **data,
		size_t size);
SWT_PUBLIC wresult w_control_layout_set_data(w_control *control,
		const w_layout_data *data);
SWT_PUBLIC wresult w_control_layout_get_data(w_control *control,
		w_layout_data *data);
SWT_PUBLIC wresult w_control_request_layout(w_control *control);
SWT_PUBLIC wresult w_control_get_menu(w_control *control, w_menu **menu);
SWT_PUBLIC wresult w_control_set_menu(w_control *control, w_menu *menu);
SWT_PUBLIC wresult w_control_get_parent(w_control *control,
		w_composite **parent);
SWT_PUBLIC wresult w_control_set_parent(w_control *control,
		w_composite *parent);
SWT_PUBLIC wresult w_control_is_reparentable(w_control *control);
SWT_PUBLIC wresult w_control_get_shell(w_control *control, w_shell **shell);
SWT_PUBLIC wresult w_control_get_tooltip_text(w_control *control, w_alloc alloc,
		void *user_data);
SWT_PUBLIC wresult w_control_set_tooltip_text(w_control *control,
		const char *text, size_t length);
SWT_PUBLIC wresult w_control_get_touch_enabled(w_control *control);
SWT_PUBLIC wresult w_control_set_touch_enabled(w_control *control, int enabled);
SWT_PUBLIC wresult w_control_get_visible(w_control *control);
SWT_PUBLIC wresult w_control_set_visible(w_control *control, int visible);
SWT_PUBLIC wresult w_control_is_visible(w_control *control);
SWT_PUBLIC wresult w_control_redraw(w_control *control, w_rect *rect, int all);
SWT_PUBLIC wresult w_control_set_redraw(w_control *control, int redraw);
SWT_PUBLIC wresult w_control_set_capture(w_control *control, int capture);
SWT_PUBLIC wresult w_control_traverse(w_control *control, int traversal,
		w_event_key *event);
SWT_PUBLIC wresult w_control_update(w_control *control);
SWT_PUBLIC wresult w_control_set_timer(w_control *control, wint64 ms,
		wushort id);
SWT_PUBLIC wresult w_control_kill_timer(w_control *control, wushort id);
SWT_PUBLIC wresult w_control_set_tab(w_control *control, int tab);
SWT_PUBLIC wresult w_control_get_tab(w_control *control);

#define W_CONTROL_CLASS(x) ((struct _w_control_class*)x)
#define W_CONTROL_GET_CLASS(x) ((struct _w_control_class*)W_WIDGET_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_WIDGETS_CONTROL_H_ */
