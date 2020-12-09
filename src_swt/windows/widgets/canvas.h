/*
 * canvas.h
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_WIDGETS_CANVAS_H_
#define SRC_WIN32_WIDGETS_CANVAS_H_

#if defined( __WIN32__) || defined(__WIN64__)
#include "composite.h"
typedef struct _w_caret {
	struct _w_caret_class *clazz;
	w_canvas *parent;
	w_rect bounds;
	unsigned isVisible :1;
	unsigned isShowing :1;
	int blinkRate;
	w_image image;
	w_font *font;
} _w_caret;
#define _W_CARET(x) ((_w_caret*)x)
typedef struct _w_ime {
	struct _w_ime_class *clazz;
	w_canvas *parent;
	int caretOffset;
	int startOffset;
	int commitCount;
	char *text;
	char *preeditString;
} _w_ime;
#define _W_IME(x) ((_w_ime*)x)
typedef struct _w_canvas {
	struct _w_composite composite;
	w_caret *caret;
	w_ime *ime;
} _w_canvas;
typedef struct _w_canvas_reserved _w_canvas_reserved;
struct _w_canvas_reserved {
	struct _w_composite_reserved composite;

};
#define _W_CANVAS(x) ((_w_canvas*)x)
#define _W_CANVAS_RESERVED(x) ((_w_canvas_reserved*)x)
const char* _w_canvas_window_class(w_control *control,
		_w_control_reserved *reserved);
w_caret* _w_canvas_get_caret(w_canvas *canvas);
wresult _w_canvas_set_caret(w_canvas *canvas, w_caret *caret);
int _CANVAS_WM_CHAR(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_IME_COMPOSITION(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_IME_COMPOSITION_START(w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved);
int _CANVAS_WM_IME_ENDCOMPOSITION(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_INPUTLANGCHANGE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_KEYDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_LBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_WINDOWPOSCHANGED(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
int _CANVAS_WM_WINDOWPOSCHANGING(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved);
void _w_canvas_class_init(struct _w_canvas_class *clazz);
#endif
#endif /* SRC_WIN32_WIDGETS_CANVAS_H_ */
