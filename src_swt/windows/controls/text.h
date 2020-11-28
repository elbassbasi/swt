/*
 * text.h
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CONTROLS_TEXT_H_
#define SRC_WIN32_CONTROLS_TEXT_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../widgets/scrollable.h"
typedef struct _w_text {
	struct _w_scrollable scrollable;
}_w_text;

struct _w_text_reserved {
	struct _w_scrollable_reserved scrollable;

};
#define _W_TEXT(x) ((_w_text*)x)
#define _W_TEXT_RESERVED(x) ((struct _w_text_reserved*)x)
/**
 * messages
 */
int _TEXT_WM_CHAR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_CLEAR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_CUT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_ERASEBKGND(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_GETDLGCODE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_GETOBJECT(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_IME_CHAR(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_LBUTTONDBLCLK(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_LBUTTONDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_PASTE(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_UNDO(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_CTLCOLORCHILD(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_COMMANDCHILD(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
int _TEXT_WM_KEYDOWN(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved);
void _w_text_class_init (struct _w_text_class* clazz);
#endif
#endif /* SRC_WIN32_CONTROLS_TEXT_H_ */
