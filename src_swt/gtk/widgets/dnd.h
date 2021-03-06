/*
 * dnd.h
 *
 *  Created on: 4 oct. 2020
 *      Author: azeddine
 */

#ifndef SRC_GTK_WIDGETS_DND_H_
#define SRC_GTK_WIDGETS_DND_H_
#ifdef __linux
#include "control.h"
struct _w_transfer_data {
	GdkAtom type;
	int length;
	int format;
	guchar *pValue;
	int result;
};
wresult _w_transfers_registre();
#define _W_TRANSFER(x) ((_w_transfer*)x)
typedef struct _w_clipboard {
	int clipboards;
	GtkClipboard *GTKCLIPBOARD;

} _w_clipboard;
#define _W_CLIPBOARD(x) ((_w_clipboard*)x)

#define STATE_DRAGSOURCE_TRANSFER_LOCAL (1 << (STATE_WIDGET_END + 1))
#define STATE_DRAGSOURCE_MOVEDATA (1 << (STATE_WIDGET_END + 2))

typedef struct _w_dragsource {
	_w_widget widget;
	w_control *control;
	GtkTargetList *targetList;
	w_dragsource_effect effect;
	size_t transfer_length;
	w_transfer *transfer[0];
} _w_dragsource;
#define _W_DRAGSOURCE(x) ((_w_dragsource*)x)
void _w_dragsource_class_init(struct _w_dragsource_class *clazz);
#define STATE_DROPTARGET_TRANSFER_LOCAL (1 << (STATE_WIDGET_END + 1))
#define STATE_DROPTARGET_HAS_EVENT (1 << (STATE_WIDGET_END + 1))
typedef struct _w_droptarget {
	_w_widget widget;
	w_control *control;
	guint drag_motion_handler;
	guint drag_leave_handler;
	guint drag_data_received_handler;
	guint drag_drop_handler;
	int selectedOperation;
	GdkAtom selectedDataType;
	int keyOperation;
	int dragOverEvent_x;
	int dragOverEvent_y;
	int dragOverEvent_operations;
	int dragOverEvent_time;
	wuint64 dragOverStart;
	GList* targets;
	w_droptarget_effect effect;
	size_t transfer_length;
	w_transfer *transfer[0];
} _w_droptarget;
typedef struct _w_droptarget_iterator {
	w_basic_iterator iterator;
	w_droptarget* target;
	GList *targets;
	GList *i;
	size_t count;
	w_transfer_data current_data;
	w_transfer_data temp_data;
} _w_droptarget_iterator;
#define _W_DROPTARGET(x) ((_w_droptarget*)x)
w_droptarget_effect* _w_droptarget_get_effect(w_droptarget *droptarget);
void _w_droptarget_class_init(struct _w_droptarget_class *clazz);
#endif
#endif /* SRC_GTK_WIDGETS_DND_H_ */
