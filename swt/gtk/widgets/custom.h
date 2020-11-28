/*
 * fixed.h
 *
 *  Created on: 26 juil. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_GTK_WIDGETS_CUSTOM_H_
#define SRC_GTK_WIDGETS_CUSTOM_H_

#ifdef __linux
#include "../graphics/graphics.h"
/**
 * fixed widget
 */
extern GType _w_fixed_type;
typedef struct _w_fixed {
	GtkContainer container;
	GtkAdjustment *hadjustment;
	GtkAdjustment *vadjustment;
	struct _w_fixed *next;
	struct _w_fixed *prev;
	struct _w_fixed *first;
	/* Accessibility */
	AtkObject *accessible;
	GtkWidget *child;
	guint count :28;
	guint hscroll_policy :1;
	guint vscroll_policy :1;
	guint use_as_child :1;
	guint ignore_fixed :1;
	GtkAllocation alloc;
} _w_fixed;
#define _W_TYPE_FIXED             (_w_fixed_type)
#define _W_FIXED(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), _W_TYPE_FIXED, _w_fixed))
#define _W_IS_FIXED(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), _W_TYPE_FIXED))
GtkWidget* _w_fixed_new();
void _w_fixed_move(GtkWidget *child, gint x, gint y);
void _w_fixed_resize(GtkWidget *child, gint width, gint height);
_w_fixed* _w_fixed_get(GtkWidget *widget);
void _w_fixed_add_remove_child(_w_fixed *fixed, GtkWidget *child);
void _w_fixed_restack(GtkWidget *fixed, GtkWidget *child, GtkWidget *sibling,
		gboolean above);
/**
 * image widget
 */
extern GType _w_image_widget_type;
typedef struct _w_image_widget {
	GtkImage image;
	w_widget *parent;
	w_imagelist* (*get_image_list)(w_widget *widget);
	int index;
} _w_image_widget;

#define _W_TYPE_IMAGE_WIDGET             (_w_image_widget_type)
#define _W_IMAGE_WIDGET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), _W_TYPE_IMAGE_WIDGET, _w_image_widget))
#define _W_IS_IMAGE_WIDGET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), _W_TYPE_IMAGE_WIDGET))

_w_image_widget* _w_image_widget_new();
#endif
#endif /* SRC_GTK_WIDGETS_CUSTOM_H_ */
