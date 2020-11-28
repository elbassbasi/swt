/*
 * group.h
 *
 *  Created on: 26 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWT_CONTROLS_GROUP_H_
#define SWT_CONTROLS_GROUP_H_
#include "../widgets/composite.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct w_group {
	w_composite composite;
	void *handle[0x10];
} w_group;

#define W_GROUP(x) ((w_group*)x)

struct _w_group_class {
	struct _w_composite_class composite;
	wresult (*get_text)(w_group *group,w_alloc alloc,void* user_data);
	wresult (*set_text)(w_group *group, const char *string);
};
SWT_PUBLIC wresult w_group_create(w_group *group, struct w_toolkit *toolkit,
		w_composite *parent, wuint64 style, w_widget_post_event_proc post_event);
SWT_PUBLIC w_group* w_group_new(struct w_toolkit *toolkit, w_composite *parent, wuint64 style,
		w_widget_post_event_proc post_event);
SWT_PUBLIC wresult w_group_get_text(w_group *group,w_alloc alloc,void* user_data);
SWT_PUBLIC wresult w_group_set_text(w_group *group, const char *string);
#define W_GROUP_CLASS(x) ((struct _w_group_class*)x)
#define W_GROUP_GET_CLASS(x) ((struct _w_group_class*)W_WIDGET_GET_CLASS(x))
#ifdef __cplusplus
}
#endif
#endif /* SWT_CONTROLS_GROUP_H_ */
