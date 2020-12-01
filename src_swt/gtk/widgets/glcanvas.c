/*
 * glcanvas.c
 *
 *  Created on: 1 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "glcanvas.h"
#include "toolkit.h"
#include "GL/glx.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include <gdk/gdkx.h>

#define USE_TEXTURE 0

wresult _w_glcanvas_init_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	if (data == 0)
		return W_ERROR_NULL_ARGUMENT;
	GError *error = NULL;
	//gtk_widget_set_has_window(_W_WIDGET(glcanvas)->handle, FALSE);
	gtk_widget_set_app_paintable(_W_WIDGET(glcanvas)->handle, TRUE);
	_W_GLCANVAS(glcanvas)->context = gdk_window_create_gl_context(
			gtk_widget_get_window(_W_WIDGET(glcanvas)->handle), &error);
	gdk_gl_context_realize(_W_GLCANVAS(glcanvas)->context, &error);
	GdkWindow *window = gtk_widget_get_window(_W_WIDGET(glcanvas)->handle);
	GdkVisual *gdkvisual = gdk_window_get_visual(window);
	GdkWindowAttr attrs;
	attrs.width = 1;
	attrs.height = 1;
	attrs.event_mask = GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK
			| GDK_FOCUS_CHANGE_MASK | GDK_POINTER_MOTION_MASK
			| GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK
			| GDK_ENTER_NOTIFY_MASK | GDK_LEAVE_NOTIFY_MASK | GDK_EXPOSURE_MASK
			| GDK_POINTER_MOTION_HINT_MASK;
	attrs.window_type = GDK_WINDOW_CHILD;
	attrs.visual = gdkvisual;
	_W_GLCANVAS(glcanvas)->event_window = gdk_window_new(window, &attrs,
			GDK_WA_VISUAL);
	gdk_window_set_user_data(_W_GLCANVAS(glcanvas)->event_window,
	_W_WIDGET(glcanvas)->handle);
	gdk_window_show(_W_GLCANVAS(glcanvas)->event_window);
	return W_TRUE;
}
wresult _w_glcanvas_get_gldata(w_glcanvas *glcanvas, w_gldata *data) {
	return W_TRUE;
}
wresult _w_glcanvas_is_current(w_glcanvas *glcanvas) {
	return gdk_gl_context_get_current() == _W_GLCANVAS(glcanvas)->context;
}
wresult _w_glcanvas_set_current(w_glcanvas *glcanvas) {
	GLenum status;
	if (gdk_gl_context_get_current() == _W_GLCANVAS(glcanvas)->context)
		return W_TRUE;
	gdk_gl_context_make_current(_W_GLCANVAS(glcanvas)->context);
	//gtk_widget_realize(_W_WIDGET(glcanvas)->handle);
	if (_W_GLCANVAS(glcanvas)->frame_buffer != 0) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &_W_GLCANVAS(glcanvas)->frame_buffer);
		_W_GLCANVAS(glcanvas)->frame_buffer = 0;
	}
#if USE_TEXTURE
	if (_W_GLCANVAS(glcanvas)->texture != 0) {
		glDeleteTextures(1, &_W_GLCANVAS(glcanvas)->texture);
		_W_GLCANVAS(glcanvas)->texture = 0;
	}
#else
	if (_W_GLCANVAS(glcanvas)->render_buffer != 0) {
		glDeleteRenderbuffers(1, &_W_GLCANVAS(glcanvas)->render_buffer);
		_W_GLCANVAS(glcanvas)->render_buffer = 0;
	}
#endif
	glGenFramebuffers(1, &_W_GLCANVAS(glcanvas)->frame_buffer);
#if USE_TEXTURE
	glGenTextures(1, &_W_GLCANVAS(glcanvas)->texture);
	glBindTexture(GL_TEXTURE_2D, _W_GLCANVAS(glcanvas)->texture);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);*/
#else
	glGenRenderbuffers (1, &_W_GLCANVAS(glcanvas)->render_buffer);
    glBindRenderbuffer (GL_RENDERBUFFER, _W_GLCANVAS(glcanvas)->render_buffer);
#endif
	int scale = gtk_widget_get_scale_factor(_W_WIDGET(glcanvas)->handle);
	int w = gtk_widget_get_allocated_width(_W_WIDGET(glcanvas)->handle) * scale;
	int h = gtk_widget_get_allocated_height(_W_WIDGET(glcanvas)->handle)
			* scale;
#if USE_TEXTURE
	if (gdk_gl_context_get_use_es(_W_GLCANVAS(glcanvas)->context))
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, NULL);
#else
	glRenderbufferStorage (GL_RENDERBUFFER, GL_RGB8, w, h);
#endif
	glBindFramebuffer(GL_FRAMEBUFFER, _W_GLCANVAS(glcanvas)->frame_buffer);
#if USE_TEXTURE
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
	GL_TEXTURE_2D, _W_GLCANVAS(glcanvas)->texture, 0);
#else
    glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                               GL_RENDERBUFFER, _W_GLCANVAS(glcanvas)->render_buffer);
#endif
	glEnable(GL_CULL_FACE);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		return FALSE;
	}
	//glDisable(GL_DEPTH_TEST);

	return W_TRUE;
}

wresult _w_glcanvas_swap_buffers(w_glcanvas *glcanvas) {
	int scale = gtk_widget_get_scale_factor(_W_WIDGET(glcanvas)->handle);
	int w = gtk_widget_get_allocated_width(_W_WIDGET(glcanvas)->handle) * scale;
	int h = gtk_widget_get_allocated_height(_W_WIDGET(glcanvas)->handle)
			* scale;
	GdkWindow *window = _W_GLCANVAS(glcanvas)->event_window; //gtk_widget_get_window(_W_WIDGET(glcanvas)->handle);
	cairo_t *cr = gdk_cairo_create(window);
#if USE_TEXTURE
	gdk_cairo_draw_from_gl(cr,
			gtk_widget_get_window(_W_WIDGET(glcanvas)->handle),
			_W_GLCANVAS(glcanvas)->frame_buffer,
			GL_TEXTURE, scale, 0, 0, w, h);
#else
	gdk_cairo_draw_from_gl(cr,
			gtk_widget_get_window(_W_WIDGET(glcanvas)->handle),
			_W_GLCANVAS(glcanvas)->render_buffer,
			GL_RENDERBUFFER, scale, 0, 0, w, h);
#endif
	cairo_destroy(cr);
	gdk_gl_context_clear_current();
	return W_TRUE;
}
void _w_glcanvas_resize_handle(w_control *control, w_size *size,
		struct _w_control_reserved *reserved) {
	_w_composite_resize_handle(control, size, reserved);
	if (gtk_widget_get_realized(_W_WIDGET(control)->handle)) {
		if (_W_GLCANVAS(control)->event_window != NULL) {
			GtkAllocation allocation;
			gtk_widget_get_allocation(_W_WIDGET(control)->handle, &allocation);
			gdk_window_move_resize(_W_GLCANVAS(control)->event_window,
					allocation.x, allocation.y, allocation.width,
					allocation.height);
			glViewport(0, 0, allocation.width, allocation.height);
		}
	}
}
/*
 * signals
 */
gboolean _gtk_glcanvas_destroy(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	if (_W_GLCANVAS(widget)->event_window != 0) {
		gdk_window_destroy(_W_GLCANVAS(widget)->event_window);
		_W_GLCANVAS(widget)->event_window = 0;
	}
	return _gtk_composite_destroy(widget, e, reserved);
}
gboolean _gtk_glcanvas_realize(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
gboolean _gtk_glcanvas_unrealize(struct w_widget *widget,
		struct _w_event_platform *e, struct _w_widget_reserved *reserved) {
	return W_FALSE;
}
void _w_glcanvas_class_init(struct _w_glcanvas_class *clazz) {
	_w_canvas_class_init(W_CANVAS_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_GLCANVAS;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_glcanvas_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_glcanvas);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_glcanvas);
	/*
	 *
	 */
	clazz->init_gldata = _w_glcanvas_init_gldata;
	clazz->get_gldata = _w_glcanvas_get_gldata;
	clazz->is_current = _w_glcanvas_is_current;
	clazz->set_current = _w_glcanvas_set_current;
	clazz->swap_buffers = _w_glcanvas_swap_buffers;
	/*
	 *
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->resize_handle = _w_glcanvas_resize_handle;
	//signal
	_gtk_signal *signals = reserved->widget.signals;
	signals[SIGNAL_DESTROY] = _gtk_glcanvas_destroy;
	signals[SIGNAL_REALIZE] = _gtk_glcanvas_realize;
	signals[SIGNAL_UNREALIZE] = _gtk_glcanvas_unrealize;
}
#endif
