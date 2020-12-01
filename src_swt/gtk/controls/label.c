/*
 * label.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "label.h"
#include "../widgets/toolkit.h"
struct _w_label_handles {
	GtkWidget *handle;
	GtkWidget *box;
	GtkWidget *label;
	GtkWidget *image;
};
void _w_label_get_handles_callback(GtkWidget *widget, gpointer data) {
	if (GTK_IS_BOX(widget)) {
		((struct _w_label_handles*) data)->box = widget;
		return;
	}
	if (GTK_IS_IMAGE(widget)) {
		((struct _w_label_handles*) data)->image = widget;
		return;
	}
	if (GTK_IS_LABEL(widget)) {
		((struct _w_label_handles*) data)->label = widget;
		return;
	}
}
void _w_label_get_handles(struct _w_label_handles *handles) {
	if (GTK_IS_CONTAINER(handles->handle)) {
		gtk_container_forall(GTK_CONTAINER(handles->handle),
				_w_label_get_handles_callback, handles);
	}
	if (handles->box != 0) {
		gtk_container_forall(GTK_CONTAINER(handles->box),
				_w_label_get_handles_callback, handles);
	}
}
wresult _w_label_get_image(w_label *label, w_image *image) {
}
wresult _w_label_get_text(w_label *label, w_alloc string,void* user_data) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_TRUE;
	struct _w_label_handles handles;
	memset(&handles, 0, sizeof(handles));
	handles.handle = _W_WIDGET(label)->handle;
	_w_label_get_handles(&handles);
	if (handles.label != 0) {
		const char *text = gtk_label_get_text(GTK_LABEL(handles.label));
		_gtk_alloc_set_text(string,user_data, text, -1,W_ENCODING_UTF8);
	}
	return W_TRUE;
}
wresult _w_label_set_image(w_label *label, w_image *image) {
}
wresult _w_label_set_text(w_label *label, const char *text) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return W_TRUE;
	struct _mnemonics mn;
	struct _w_label_handles handles;
	_mnemonics_fix(&mn, text, -1, W_TRUE);
	memset(&handles, 0, sizeof(handles));
	handles.handle = _W_WIDGET(label)->handle;
	_w_label_get_handles(&handles);
	if (handles.label != 0) {
		gtk_label_set_text_with_mnemonic(GTK_LABEL(handles.label), mn.str);
		gtk_widget_show(handles.label);
	}
	//_setAlignment (style);
	_mnemonics_free(&mn, text);
	return W_TRUE;
}
int _w_label_check_style(int style) {
	style |= W_NO_FOCUS;
	if ((style & W_SEPARATOR) != 0) {
		style = _w_widget_check_bits(style, W_VERTICAL, W_HORIZONTAL, 0, 0, 0,
				0);
		return _w_widget_check_bits(style, W_SHADOW_OUT, W_SHADOW_IN,
				W_SHADOW_NONE, 0, 0, 0);
	}
	return _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
}
GtkWidget* _w_label_fixed_handle(w_widget *widget,
		struct _w_widget_reserved *reserved) {
	if (_W_WIDGET(widget)->style & W_BORDER) {
		return _w_widget_hpp(widget, reserved);
	} else {
		return _w_widget_hp(widget, reserved);
	}
}
void gtk_label_set_align(GtkWidget *labelHandle, float xalign, float yalign) {
	gtk_label_set_xalign(GTK_LABEL(labelHandle), xalign);
	gtk_label_set_yalign(GTK_LABEL(labelHandle), yalign);
}

void _w_label_set_alignment_0(w_label *label) {
	struct _w_widget_handles handles;
	memset(&handles, 0, sizeof(handles));
	handles.handle = _W_WIDGET(label)->handle;
	_w_widget_get_handles(&handles);
	if ((_W_WIDGET(label)->style & W_LEFT) != 0) {
		if (GTK_VERSION >= VERSION(3, 16, 0)) {
			gtk_widget_set_align(handles.label, GTK_ALIGN_START,
					GTK_ALIGN_START); //Aligns widget
			gtk_label_set_align(handles.label, 0.0f, 0.0f); //Aligns text inside the widget.
			gtk_widget_set_align(handles.image, GTK_ALIGN_START,
					GTK_ALIGN_CENTER);
		} else {
			gtk_misc_set_alignment(GTK_MISC(handles.label), 0.0f, 0.0f);
			gtk_misc_set_alignment(GTK_MISC(handles.image), 0.0f, 0.5f);
		}
		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_LEFT);
		return;
	}
	if ((_W_WIDGET(label)->style & W_CENTER) != 0) {
		if (GTK_VERSION >= VERSION(3, 16, 0)) {
			gtk_widget_set_align(handles.label, GTK_ALIGN_CENTER,
					GTK_ALIGN_START); //Aligns widget
			gtk_label_set_align(handles.label, 0.5f, 0.0f); //Aligns text inside the widget.
			gtk_widget_set_align(handles.image, GTK_ALIGN_CENTER,
					GTK_ALIGN_CENTER);
		} else {
			gtk_misc_set_alignment(GTK_MISC(handles.label), 0.5f, 0.0f);
			gtk_misc_set_alignment(GTK_MISC(handles.image), 0.5f, 0.5f);
		}

		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_CENTER);
		return;
	}
	if ((_W_WIDGET(label)->style & W_RIGHT) != 0) {
		if (GTK_VERSION >= VERSION(3, 16, 0)) {
			gtk_widget_set_align(handles.label, GTK_ALIGN_END, GTK_ALIGN_START); //Aligns widget.
			gtk_label_set_align(handles.label, 1.0f, 0.0f); //Aligns text inside the widget.
			gtk_widget_set_align(handles.image, GTK_ALIGN_END,
					GTK_ALIGN_CENTER);
		} else {
			gtk_misc_set_alignment(GTK_MISC(handles.label), 1.0f, 0.0f);
			gtk_misc_set_alignment(GTK_MISC(handles.image), 1.0f, 0.5f);
		}
		gtk_label_set_justify(GTK_LABEL(handles.label), GTK_JUSTIFY_RIGHT);
		return;
	}
}
void _w_label_set_alignment(w_label *label, int alignment) {
	if ((_W_WIDGET(label)->style & W_SEPARATOR) != 0)
		return;
	if ((alignment & (W_LEFT | W_RIGHT | W_CENTER)) == 0)
		return;
	_W_WIDGET(label)->style &= ~(W_LEFT | W_RIGHT | W_CENTER);
	_W_WIDGET(label)->style |= alignment & (W_LEFT | W_RIGHT | W_CENTER);
	_w_label_set_alignment_0(label);
}

wresult _w_label_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle, *boxHandle = 0, *frameHandle = 0, *labelHandle = 0,
			*imageHandle = 0, *handle = 0;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_THEME_BACKGROUND;
	_W_WIDGET(widget)->style = _w_label_check_style(style);
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) != 0) {
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			handle = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
			if (handle != 0 && GTK_VERSION >= VERSION(3, 20, 0)) {
				gtk_widget_set_valign(handle, GTK_ALIGN_CENTER);
			}
		} else {
			handle = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
			if (handle != 0 && GTK_VERSION >= VERSION(3, 20, 0)) {
				gtk_widget_set_halign(handle, GTK_ALIGN_CENTER);
			}
		}
		if (handle == 0)
			goto _err;
	} else {
		handle = gtk_event_box_new();
		if (handle == 0)
			goto _err;
		boxHandle = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		if (boxHandle == 0)
			goto _err;
		labelHandle = gtk_label_new_with_mnemonic("");
		if (labelHandle == 0)
			goto _err;
		imageHandle = gtk_image_new();
		if (imageHandle == 0)
			goto _err;
		gtk_container_add(GTK_CONTAINER(handle), boxHandle);
		gtk_container_add(GTK_CONTAINER(boxHandle), labelHandle);
		gtk_container_add(GTK_CONTAINER(boxHandle), imageHandle);
		gtk_box_set_child_packing(GTK_BOX(boxHandle), labelHandle, TRUE, TRUE,
				0, GTK_PACK_START);
		gtk_box_set_child_packing(GTK_BOX(boxHandle), imageHandle, TRUE, TRUE,
				0, GTK_PACK_START);
	}
	if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
		frameHandle = gtk_frame_new("");
		if (frameHandle == 0)
			goto _err;
		gtk_container_add(GTK_CONTAINER(fixedHandle), frameHandle);
		gtk_container_add(GTK_CONTAINER(frameHandle), handle);
		gtk_frame_set_shadow_type(GTK_FRAME(frameHandle), GTK_SHADOW_ETCHED_IN);
	} else {
		gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
	}
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	if (labelHandle != 0)
		g_object_set_qdata(G_OBJECT(labelHandle), gtk_toolkit->quark[0],
				widget);
	if (boxHandle != 0)
		g_object_set_qdata(G_OBJECT(boxHandle), gtk_toolkit->quark[0], widget);
	if (frameHandle != 0)
		g_object_set_qdata(G_OBJECT(frameHandle), gtk_toolkit->quark[0],
				widget);
	if (imageHandle != 0)
		g_object_set_qdata(G_OBJECT(imageHandle), gtk_toolkit->quark[0],
				widget);
	_W_WIDGET(widget)->handle = handle;
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) == 0) {
		if ((_W_WIDGET(widget)->style & W_WRAP) != 0) {
			gtk_label_set_line_wrap(GTK_LABEL(labelHandle), TRUE);
			gtk_label_set_line_wrap_mode(GTK_LABEL(labelHandle),
					PANGO_WRAP_WORD_CHAR);
		}
		// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
		// reset to default font to get the usual behavior
#if GTK3
		//setFontDescription(defaultFont().handle);
#endif
		_w_label_set_alignment_0(W_LABEL(widget));
	}
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent = W_COMPOSITE(parent);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
int _w_label_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint, hHint = e->hHint;
	int tmp[2];
	if ((_W_WIDGET(widget)->style & W_SEPARATOR) != 0) {
		if ((_W_WIDGET(widget)->style & W_HORIZONTAL) != 0) {
			if (wHint == W_DEFAULT)
				wHint = DEFAULT_WIDTH;
		} else {
			if (hHint == W_DEFAULT)
				hHint = DEFAULT_HEIGHT;
		}
	}
	struct _w_widget_handles handles;
	GtkWidget *frameHandle;
	memset(&handles, 0, sizeof(handles));
	handles.handle = _W_WIDGET(widget)->handle;
	_w_widget_get_handles(&handles);
	frameHandle = gtk_widget_get_parent(_W_WIDGET(widget)->handle);
	if (!GTK_IS_FRAME(frameHandle)) {
		frameHandle = 0;
	}
	/*
	 * Feature in GTK. GTK has a predetermined maximum width for wrapping text.
	 * The fix is to use pango layout directly instead of the label size request
	 * to calculate its preferred size.
	 */
	int fixWrap = handles.label != 0
			&& (_W_WIDGET(widget)->style & W_WRAP) != 0
			&& gtk_widget_get_visible(handles.label);
	if (fixWrap || frameHandle != 0) {
		_W_CONTROL_RESERVED(reserved)->force_resize(W_CONTROL(widget),
				_W_CONTROL_RESERVED(reserved));
	}
	if (fixWrap) {
		PangoLayout *labelLayout = gtk_label_get_layout(
				GTK_LABEL(handles.label));
		int pangoWidth = pango_layout_get_width(labelLayout);
		if (e->wHint != W_DEFAULT) {
			pango_layout_set_width(labelLayout, wHint * PANGO_SCALE);
		} else {
			pango_layout_set_width(labelLayout, -1);
		}
		int w, h;
		pango_layout_get_pixel_size(labelLayout, &w, &h);
		pango_layout_set_width(labelLayout, pangoWidth);
		if (frameHandle != 0) {
			int labelWidth, labelHeight;
			gtk_widget_get_size_request(handles.label, &labelWidth,
					&labelHeight);
			gtk_widget_set_size_request(handles.label, 1, 1);
			tmp[0] = e->wHint;
			tmp[1] = e->hHint;
			e->wHint = -1;
			e->hHint = -1;
			_w_control_compute_native_size(widget, frameHandle, e, reserved);
			e->wHint = tmp[0];
			e->hHint = tmp[1];
			gtk_widget_set_size_request(handles.label, labelWidth, labelHeight);
			e->size->width = e->size->width - 1;
			e->size->height = e->size->height - 1;
		} else {
			e->size->width = 0;
			e->size->height = 0;
		}
		e->size->width += wHint == W_DEFAULT ? w : wHint;
		e->size->height += hHint == W_DEFAULT ? h : hHint;
	} else {
		if (frameHandle != 0) {
			int reqWidth, reqHeight;
			gtk_widget_get_size_request(_W_WIDGET(widget)->handle, &reqWidth,
					&reqHeight);
			gtk_widget_set_size_request(_W_WIDGET(widget)->handle, wHint,
					hHint);
			tmp[0] = e->wHint;
			tmp[1] = e->hHint;
			e->wHint = -1;
			e->hHint = -1;
			_w_control_compute_native_size(widget, frameHandle, e, reserved);
			e->wHint = tmp[0];
			e->hHint = tmp[1];
			gtk_widget_set_size_request(_W_WIDGET(widget)->handle, reqWidth,
					reqHeight);
		} else {
			tmp[0] = e->wHint;
			tmp[1] = e->hHint;
			e->wHint = wHint;
			e->hHint = hHint;
			_w_control_compute_native_size(widget, _W_WIDGET(widget)->handle, e,
					reserved);
			e->wHint = tmp[0];
			e->hHint = tmp[1];
		}
	}
	/*
	 * Feature in GTK.  Instead of using the font height to determine
	 * the preferred height of the widget, GTK uses the text metrics.
	 * The fix is to ensure that the preferred height is at least as
	 * tall as the font height.
	 *
	 * NOTE: This work around does not fix the case when there are
	 * muliple lines of text.
	 */
	if (hHint == W_DEFAULT && handles.label != 0) {
		PangoLayout *layout = gtk_label_get_layout(GTK_LABEL(handles.label));
		const PangoFontDescription *font = pango_layout_get_font_description(
				layout);
		PangoContext *context = pango_layout_get_context(layout);
		PangoLanguage *lang = pango_context_get_language(context);
		/* PangoFontDescription *font = _w_control_getFontDescription(
		 W_CONTROL(widget), reserved);*/
		PangoFontMetrics *metrics = pango_context_get_metrics(context, font,
				lang);
		int ascent = pango_font_metrics_get_ascent(metrics);
		int descent = pango_font_metrics_get_descent(metrics);
		pango_font_metrics_unref(metrics);
		//int fontHeight = ascent + descent;
		int fontHeight = 0;
#if GTK3
		int bufferBottom;
		int bufferTop;
		g_object_get(handles.label, "margin-bottom", &bufferBottom, NULL);
		g_object_get(handles.label, "margin-top", &bufferTop, NULL);
		fontHeight += bufferBottom + bufferTop;
#else
			int [] bufferYpad = new int[1];
			OS.g_object_get (handles.label, OS.ypad, bufferYpad, 0);
			fontHeight += 2 * bufferYpad [0];
#endif
		if (frameHandle != 0) {
			w_point thickness;
			_w_control_get_thickness(frameHandle, &thickness);
			fontHeight += 2 * thickness.y;
			fontHeight += 2
					* gtk_container_get_border_width(
							GTK_CONTAINER(frameHandle));
		}
		e->size->height = w_max(e->size->height, fontHeight);
	}
	return W_TRUE;
}
void _w_label_class_init(struct _w_label_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_LABEL;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_label_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_label);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_label);
	/*
	 * public function
	 */
	clazz->get_image = _w_label_get_image;
	clazz->get_text = _w_label_get_text;
	clazz->set_image = _w_label_set_image;
	clazz->set_text = _w_label_set_text;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_label_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_label_fixed_handle;
	reserved->fixedHandle = _w_label_fixed_handle;
	reserved->eventHandle = _w_label_fixed_handle;
	reserved->widget.compute_size = _w_label_compute_size;
}
#endif
