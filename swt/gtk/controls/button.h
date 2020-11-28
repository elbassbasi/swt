/*
 * button.h
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_CONTROLS_BUTTON_H_
#define SRC_GTK_CONTROLS_BUTTON_H_
#ifdef __linux
#include "../widgets/control.h"
#define STATE_BUTTON_GRAYED (1 << (STATE_CONTROL_END + 1))
#define STATE_BUTTON_END (STATE_CONTROL_END + 1)
typedef struct _w_button {
	struct _w_control control;
} _w_button;

struct _w_button_reserved {
	struct _w_control_reserved control;
	guint signal_clicked_id;

};
#define _W_BUTTON(x) ((_w_button*)x)
#define _W_BUTTON_RESERVED(x) ((struct _w_button_reserved*)x)

int _w_button_check_style(int style);
void _w_button_select_radio_0(_w_fixed *t, struct _w_event_platform *e,
		wbool next);
void _w_button_select_radio(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
/*
 * public function
 */
int _w_button_get_alignment(w_button *button);
wbool _w_button_get_grayed(w_button *button);
wbool _w_button_get_image(w_button *button, w_image *image);
wbool _w_button_get_selection(w_button *button);
wresult _w_button_get_text(w_button *button, w_alloc alloc, void *user_data);
wresult _w_button_set_alignment(w_button *button, int alignment);
wresult _w_button_set_grayed(w_button *button, wbool grayed);
wresult _w_button_set_image(w_button *button, w_image *image);
wresult _w_button_set_selection(w_button *button, wbool selected);
wresult _w_button_set_text(w_button *button, const char *text, size_t length);
/*
 *
 */
gboolean _gtk_button_clicked(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved);
void _w_button_hook_events(w_widget *widget,
		struct _w_widget_reserved *reserved);
wresult _w_button_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event);
void _w_button_get_border(GtkBorder *gtkBorder, const char *border,
		GtkWidget *handle, int defaultBorder);
int _w_button_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved);
void _w_button_class_init(struct _w_button_class *clazz);
#endif
#endif /* SRC_GTK_CONTROLS_BUTTON_H_ */
