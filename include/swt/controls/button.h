/*
 * button.h
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_BUTTON_H_
#define SWT_CONTROLS_BUTTON_H_
#include "../widgets/shell.h"
#ifdef __cplusplus
extern "C" {
#endif
struct w_button {
	w_control control;
	void *handle[8];
};
#define W_BUTTON(x) ((w_button*)x)
struct _w_button_class {
	struct _w_control_class control;
	wresult (*get_alignment)(w_button *button);
	wresult (*get_grayed)(w_button *button);
	wresult (*get_image)(w_button *button, w_image *image);
	wresult (*get_selection)(w_button *button);
	wresult (*get_text)(w_button *button,w_alloc alloc,void* user_data);
	wresult (*set_alignment)(w_button *button, int alignment);
	wresult (*set_grayed)(w_button *button, wbool grayed);
	wresult (*set_image)(w_button *button, w_image *image);
	wresult (*set_selection)(w_button *button, wbool selected);
	wresult (*set_text)(w_button *button, const char *string,size_t length);
};
SWT_PUBLIC wresult w_button_create(w_button *button, w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_button* w_button_new(w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_button_get_alignment(w_button *button);
SWT_PUBLIC wresult w_button_get_grayed(w_button *button);
SWT_PUBLIC wresult w_button_get_image(w_button *button, w_image *image);
SWT_PUBLIC wresult w_button_get_selection(w_button *button);
SWT_PUBLIC wresult w_button_get_text(w_button *button,w_alloc alloc,void* user_data);
SWT_PUBLIC wresult w_button_set_alignment(w_button *button, int alignment);
SWT_PUBLIC wresult w_button_set_grayed(w_button *button, wbool grayed);
SWT_PUBLIC wresult w_button_set_image(w_button *button, w_image *image);
SWT_PUBLIC wresult w_button_set_selection(w_button *button, wbool selected);
SWT_PUBLIC wresult w_button_set_text(w_button *button, const char *string,size_t length);

#define W_BUTTON_CLASS(x) ((struct _w_button_class*)x)
#define W_BUTTON_GET_CLASS(x) ((struct _w_button_class*)W_WIDGET_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_BUTTON_H_ */
