/*
 * text.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "text.h"
#include "../widgets/toolkit.h"
#define SPACE_FOR_CURSOR 1
wresult _w_text_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	wresult result = _w_control_create(widget, parent, style, post_event);
	if (result > 0 && (_W_WIDGET(widget)->style & W_SEARCH) != 0) {
		/*
		 * Ensure that SWT.ICON_CANCEL and ICON_SEARCH are set.
		 * NOTE: ICON_CANCEL has the same value as H_SCROLL and CON_SEARCH has the same value as V_SCROLL
		 * so it is necessary to first clear these bits to avoid a scroll bar and then reset the
		 * bit using the original style upplied by the programmer.
		 *
		 * NOTE2: Default GtkSearchEntry shows both "find" icon and "clear" icon.
		 * "find" icon can be manually removed here while "clear" icon must be removed depending on text.
		 * See gtk_changed.
		 */
		_W_WIDGET(widget)->style |= W_ICON_SEARCH | W_ICON_CANCEL;
		GtkEntry *entry = GTK_ENTRY(_W_WIDGET(widget)->handle);

		if ((style & W_ICON_SEARCH) == 0) {
			_W_WIDGET(widget)->style &= ~W_ICON_SEARCH;
			gtk_entry_set_icon_from_icon_name(entry, GTK_ENTRY_ICON_PRIMARY, 0);
		} else {
			// Default GtkSearchEntry always shows inactive "find" icon
			// make it active and sensitive to be consistent with other platforms
			gtk_entry_set_icon_activatable(entry, GTK_ENTRY_ICON_PRIMARY, TRUE);
			gtk_entry_set_icon_sensitive(entry, GTK_ENTRY_ICON_PRIMARY,
			TRUE);
		}
		if ((style & W_ICON_CANCEL) == 0) {
			_W_WIDGET(widget)->style &= ~W_ICON_CANCEL;
		}
	}
	return result;
}
int _w_text_apply_theme_background(w_scrollable *scrollable,
		_w_scrollable_reserved *reserved) {
	return (_W_CONTROL(scrollable)->backgroundAlpha == 0
			|| (_W_WIDGET(scrollable)->style
					& (W_BORDER | W_HSCROLL | W_VSCROLL)) == 0) ? 1 : 0;
}

wresult _w_text_create_widget(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	wresult result = _w_control_create_widget(widget, index, reserved);
	if (result > 0) {
		_W_WIDGET(widget)->state |= STATE_TEXT_DOUBLE_CLICK;
	}
	return result;
}
wresult _w_text_append(w_text *text, const char *str, size_t length, int enc) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	//_w_text_clear_segments(text, W_TRUE);
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gint pos = -1;
		size_t newlength;
		char *s = _gtk_text_fix(str, length, &newlength, enc);
		if (s == 0)
			return W_ERROR_NO_MEMORY;
		gtk_editable_insert_text(GTK_EDITABLE(handle), s, newlength, &pos);
		_gtk_text_free(str, s);
		gtk_editable_set_position(GTK_EDITABLE(handle), -1);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		GtkTextIter position;
		gtk_text_buffer_get_end_iter(bufferHandle, &position);
		size_t newlength;
		char *s = _gtk_text_fix(str, length, &newlength, enc);
		if (s == 0)
			return W_ERROR_NO_MEMORY;
		gtk_text_buffer_insert(bufferHandle, &position, s, newlength);
		_gtk_text_free(str, s);
		gtk_text_buffer_place_cursor(bufferHandle, &position);
		GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle), mark, 0, TRUE, 0,
				0);
	}
	//_w_text_apply_segments(text);
	return W_TRUE;
}
void _w_text_apply_segments(w_text *text) {
	/*
	 * It is possible (but unlikely), that application code could have
	 * disposed the widget in the modify event. If this happens, return to
	 * cancel the operation.
	 */
	/*if (w_widget_is_ok(W_WIDGET(text))<=0) return;
	 w_event event;
	 String string = getText ();
	 event.text = string;
	 event.segments = segments;
	 sendEvent (SWT.Segments, event);
	 segments = event.segments;
	 if (segments == null) return;
	 int nSegments = segments.length;
	 if (nSegments == 0) return;

	 for (int i = 1, length = string == null ? 0 : string.length (); i < nSegments; i++) {
	 if (event.segments [i] < event.segments [i - 1] || event.segments [i] > length) {
	 error (SWT.ERROR_INVALID_ARGUMENT);
	 }
	 }
	 char[] segmentsChars = event.segmentsChars;
	 char [] separator = { getOrientation () == SWT.RIGHT_TO_LEFT ? RTL_MARK : LTR_MARK };
	 if ((style & SWT.SINGLE) != 0) {
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, INSERT_TEXT);
	 int limit = GTK.gtk_entry_get_max_length (handle);
	 if (limit != 0) GTK.gtk_entry_set_max_length (handle, translateOffset (limit));
	 int [] pos = new int [1];
	 for (int i = 0; i < nSegments; i++) {
	 pos [0] = segments [i] + i;
	 if (segmentsChars != null && segmentsChars.length > i) {
	 separator [0] = segmentsChars [i];
	 }
	 byte [] buffer = Converter.wcsToMbcs (separator, false);
	 long ptr = GTK.gtk_entry_get_text (handle);
	 pos [0] = (int)OS.g_utf16_offset_to_utf8_offset (ptr, pos [0]);
	 GTK.gtk_editable_insert_text (handle, buffer, buffer.length, pos);
	 }
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, INSERT_TEXT);
	 } else {
	 OS.g_signal_handlers_block_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 OS.g_signal_handlers_block_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEXT_BUFFER_INSERT_TEXT);
	 byte [] pos = new byte [ITER_SIZEOF];
	 byte [] startIter =  new byte [ITER_SIZEOF];
	 byte [] endIter =  new byte [ITER_SIZEOF];
	 for (int i = 0; i < nSegments; i++) {
	 GTK.gtk_text_buffer_get_bounds (bufferHandle, startIter, endIter);
	 long ptr = GTK.gtk_text_buffer_get_text (bufferHandle, startIter, endIter, true);
	 GTK.gtk_text_buffer_get_iter_at_offset (bufferHandle, pos, (int)OS.g_utf16_offset_to_utf8_offset (ptr, segments[i] + i));
	 OS.g_free (ptr);
	 if (segmentsChars != null && segmentsChars.length > i) {
	 separator [0] = segmentsChars [i];
	 }
	 byte [] buffer = Converter.wcsToMbcs (separator, false);
	 GTK.gtk_text_buffer_insert (bufferHandle, pos, buffer, buffer.length);
	 }
	 OS.g_signal_handlers_unblock_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 OS.g_signal_handlers_unblock_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, TEXT_BUFFER_INSERT_TEXT);
	 }*/
}

void _w_text_clear_segments(w_text *text, int applyText) {
	/*if (segments == null) return;
	 int nSegments = segments.length;
	 if (nSegments == 0) return;

	 if ((style & SWT.SINGLE) != 0) {
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 if (applyText) {
	 OS.g_signal_handlers_block_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, DELETE_TEXT);
	 long ptr = GTK.gtk_entry_get_text (handle);
	 int start, end;
	 for (int i = 0; i < nSegments; i++) {
	 start = (int)OS.g_utf16_offset_to_utf8_offset (ptr, segments[i]);
	 end = (int)OS.g_utf16_offset_to_utf8_offset (ptr, segments[i] + 1);
	 GTK.gtk_editable_delete_text (handle, start, end);
	 }
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, DELETE_TEXT);
	 }
	 int limit = GTK.gtk_entry_get_max_length (handle);
	 if (limit != 0) GTK.gtk_entry_set_max_length (handle, untranslateOffset (limit));
	 OS.g_signal_handlers_unblock_matched (handle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 } else if (applyText) {
	 OS.g_signal_handlers_block_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 OS.g_signal_handlers_block_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, DELETE_RANGE);
	 byte [] start = new byte [ITER_SIZEOF], end = new byte [ITER_SIZEOF];
	 byte [] startIter =  new byte [ITER_SIZEOF], endIter =  new byte [ITER_SIZEOF];
	 for (int i = 0; i < nSegments; i++) {
	 GTK.gtk_text_buffer_get_bounds (bufferHandle, startIter, endIter);
	 long ptr = GTK.gtk_text_buffer_get_text (bufferHandle, startIter, endIter, true);
	 GTK.gtk_text_buffer_get_iter_at_offset (bufferHandle, start, (int)OS.g_utf16_offset_to_utf8_offset (ptr, segments[i]));
	 GTK.gtk_text_buffer_get_iter_at_offset (bufferHandle, end, (int)OS.g_utf16_offset_to_utf8_offset (ptr, segments[i] + 1));
	 GTK.gtk_text_buffer_delete (bufferHandle, start, end);
	 OS.g_free (ptr);
	 }
	 OS.g_signal_handlers_unblock_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, DELETE_RANGE);
	 OS.g_signal_handlers_unblock_matched (bufferHandle, OS.G_SIGNAL_MATCH_DATA, 0, 0, 0, 0, CHANGED);
	 }
	 segments = null;*/
}

wresult _w_text_clear_selection(w_text *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEditable *editable = GTK_EDITABLE(_W_WIDGET(text)->handle);
		int position = gtk_editable_get_position(editable);
		gtk_editable_select_region(editable, position, position);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
		GtkTextIter position;
		GtkTextMark *insertMark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, insertMark);
		gtk_text_buffer_select_range(bufferHandle, &position, &position);
	}
	return W_TRUE;
}
wresult _w_text_copy(w_text *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEditable *editable = GTK_EDITABLE(_W_WIDGET(text)->handle);
		gtk_editable_copy_clipboard(editable);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
#if GTK3
		GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);
#endif
#if GTK4
#endif
		_w_text_clear_segments(text, TRUE);
		gtk_text_buffer_copy_clipboard(bufferHandle, clipboard);
		_w_text_apply_segments(text);
	}
	return W_TRUE;
}
wresult _w_text_cut(w_text *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEditable *editable = GTK_EDITABLE(_W_WIDGET(text)->handle);
		gtk_editable_cut_clipboard(editable);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
#if GTK3
		GtkClipboard *clipboard = gtk_clipboard_get(GDK_NONE);
#endif
#if GTK4
#endif
		_w_text_clear_segments(text, TRUE);
		gtk_text_buffer_cut_clipboard(bufferHandle, clipboard,
				gtk_text_view_get_editable(
						GTK_TEXT_VIEW(_W_WIDGET(text)->handle)));
		_w_text_apply_segments(text);
	}
	return W_TRUE;
}

wresult _w_text_get_caret_line_number(w_text *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return 0;
	GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
			GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
	GtkTextIter position;
	GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
	gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, mark);
	return gtk_text_iter_get_line(&position);
}
wresult _w_text_get_caret_location(w_text *text, w_point *location) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEntry *entry = GTK_ENTRY(_W_WIDGET(text)->handle);
		int index = gtk_editable_get_position(GTK_EDITABLE(entry));
		index = gtk_entry_text_index_to_layout_index(entry, index);
		gint offset_x = 0, offset_y = 0;
		gtk_entry_get_layout_offsets(entry, &offset_x, &offset_y);
		PangoLayout *layout = gtk_entry_get_layout(entry);
		PangoRectangle pos;
		pango_layout_index_to_pos(layout, index, &pos);
		int border = W_CONTROL_GET_CLASS(text)->get_border_width(
				W_CONTROL(text));
		location->x = offset_x + PANGO_PIXELS(pos.x) - border;
		location->y = offset_y + PANGO_PIXELS(pos.y);
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter position;
		GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, mark);
		GdkRectangle rect;
		gtk_text_view_get_iter_location(textview, &position, &rect);
		gint x = 0;
		gint y = 0;
		gtk_text_view_buffer_to_window_coords(textview, GTK_TEXT_WINDOW_TEXT,
				rect.x, rect.y, &x, &y);
		location->x = x;
		location->y = y;
	}
	return W_TRUE;
}
wresult _w_text_get_caret_position(w_text *text, int enc) {
	int result;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		return gtk_editable_get_position(GTK_EDITABLE(_W_WIDGET(text)->handle));
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter position, zero;
		GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
		gtk_text_buffer_get_iter_at_mark(bufferHandle, &position, mark);
		return gtk_text_iter_get_offset(&position);
	}
}
wresult _w_text_get_char_count(w_text *text, int enc) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		const gchar *str = gtk_entry_get_text(
				GTK_ENTRY(_W_WIDGET(text)->handle));
		return strlen(str);
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter startIter, endIter;
		gtk_text_buffer_get_bounds(bufferHandle, &startIter, &endIter);
		return gtk_text_iter_get_offset(&endIter);
	}
}
wresult _w_text_get_doubleclick_enabled(w_text *text) {
	return (_W_WIDGET(text)->state & STATE_TEXT_DOUBLE_CLICK) != 0;
}
wresult _w_text_get_echo_char(w_text *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		GtkEntry *entry = GTK_ENTRY(_W_WIDGET(text)->handle);
		if (!gtk_entry_get_visibility(entry)) {
			return gtk_entry_get_invisible_char(entry);
		}
	}
	return '\0';
}
wresult _w_text_get_editable(w_text *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		return gtk_editable_get_editable(GTK_EDITABLE(_W_WIDGET(text)->handle));
	}
	return gtk_text_view_get_editable(GTK_TEXT_VIEW(_W_WIDGET(text)->handle));
}
wresult _w_text_get_line_count(w_text *text) {
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0)
		return 1;
	GtkTextView *textview = GTK_TEXT_VIEW(_W_WIDGET(text)->handle);
	GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
	return gtk_text_buffer_get_line_count(bufferHandle);
}
wresult _w_text_get_line_delimiter(w_text *text) {
	return '\n';
}
wresult _w_text_get_line_height(w_text *text) {
	_w_control_reserved *reserved = _W_CONTROL_GET_RESERVED(text);
	const PangoFontDescription *desc = _w_control_get_get_font_description(
			W_CONTROL(text), reserved);
	_w_widget_font_height(desc, _W_WIDGET(text)->handle);
	return W_TRUE;
}
wresult _w_text_get_message(w_text *text, w_alloc alloc, void *user_data,
		int enc) {
	return _gtk_alloc_set_text(alloc, user_data, _W_TEXT(text)->message, -1,
			enc);
}
wresult _w_text_get_position(w_text *text, w_point *point, int enc) {
	int position = -1;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gint index;
		gint trailing;
		PangoLayout *layout = gtk_entry_get_layout(GTK_ENTRY(handle));
		pango_layout_xy_to_index(layout, point->x * PANGO_SCALE,
				point->y * PANGO_SCALE, &index, &trailing);
		const gchar *ptr = pango_layout_get_text(layout);
		if (enc == W_ENCODING_UNICODE) {
			//position = (int)g_utf16_pointer_to_offset (ptr, &ptr[index]);
		} else {
			position = index + trailing;
		}
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter p, zero;
		gtk_text_view_get_iter_at_location(textview, &p, point->x, point->y);
		gtk_text_buffer_get_iter_at_offset(bufferHandle, &zero, 0);
		gchar *ptr = gtk_text_buffer_get_text(bufferHandle, &zero, &p, TRUE);
		position = gtk_text_iter_get_offset(&p);
		if (enc == W_ENCODING_UNICODE) {
			//position = g_utf8_offset_to_utf16_offset(ptr, position);
		}
		g_free(ptr);
	}
	return position;
}
wresult _w_text_get_selection(w_text *text, w_range *range, int enc) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gint start;
		gint end;
		gtk_editable_get_selection_bounds(GTK_EDITABLE(handle), &start, &end);
		const gchar *ptr = gtk_entry_get_text(GTK_ENTRY(handle));
		if (enc == W_ENCODING_UNICODE) {
			//start = g_utf8_offset_to_utf16_offset(ptr, start);
			//end = g_utf8_offset_to_utf16_offset(ptr, end);
		}
		range->start = start;
		range->end = end;
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter startIter, endIter, zero;
		gtk_text_buffer_get_selection_bounds(bufferHandle, &startIter,
				&endIter);
		gtk_text_buffer_get_iter_at_offset(bufferHandle, &zero, 0);
		gchar *ptr = gtk_text_buffer_get_text(bufferHandle, &zero, &endIter,
		TRUE);
		range->start = gtk_text_iter_get_offset(&startIter);
		range->end = gtk_text_iter_get_offset(&endIter);
		if (enc == W_ENCODING_UNICODE) {
			//range->start = g_utf8_offset_to_utf16_offset(ptr, range->start);
			//range->end = g_utf8_offset_to_utf16_offset(ptr, range->end);
		}
		g_free(ptr);
	}
	return W_TRUE;
}
wresult _w_text_get_selection_count(w_text *text, int enc) {
	w_range selection;
	_w_text_get_selection(text, &selection, enc);
	return abs(selection.end - selection.start);
}
wresult _w_text_get_selection_text(w_text *text, w_alloc _text, void *user_data,
		int enc) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	gint start;
	gint end;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gtk_editable_get_selection_bounds(GTK_EDITABLE(handle), &start, &end);
		const gchar *ptr = gtk_entry_get_text(GTK_ENTRY(handle));
		_gtk_alloc_set_text(_text, user_data, &ptr[start], abs(end - start),
				enc);
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(textview);
		GtkTextIter startIter, endIter, zero;
		gtk_text_buffer_get_selection_bounds(bufferHandle, &startIter,
				&endIter);
		gtk_text_buffer_get_iter_at_offset(bufferHandle, &zero, 0);
		gchar *ptr = gtk_text_buffer_get_text(bufferHandle, &zero, &endIter,
		TRUE);
		start = gtk_text_iter_get_offset(&startIter);
		end = gtk_text_iter_get_offset(&endIter);
		_gtk_alloc_set_text(_text, user_data, &ptr[start], abs(end - start),
				enc);
		g_free(ptr);
	}
	return W_TRUE;
}
int _w_text_get_tab_width(w_text *text) {
	PangoLayout *layout = gtk_widget_create_pango_layout(
	_W_WIDGET(text)->handle, " ");
	gint width;
	gint height;
	pango_layout_get_size(layout, &width, &height);
	g_object_unref(layout);
	return width;
}
wresult _w_text_get_tabs(w_text *text) {
	GtkWidget *handle = _W_WIDGET(text)->handle;
	gint start;
	gint end;
	PangoTabArray *tabs;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		tabs = gtk_entry_get_tabs(GTK_ENTRY(handle));
	} else {
		GtkTextView *textview = GTK_TEXT_VIEW(handle);
		tabs = gtk_text_view_get_tabs(textview);
	}
	gint tabWidth;
	PangoTabAlign align;
	pango_tab_array_get_tab(tabs, 0, &align, &tabWidth);
	gint width = _w_text_get_tab_width(text);
	return tabWidth / width;
}
wresult _w_text_get_text(w_text *text, w_alloc _text, void *user_data,
		int enc) {
	gchar *address;
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		address = (gchar*) gtk_entry_get_text(GTK_ENTRY(handle));
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		GtkTextIter start;
		GtkTextIter end;
		gtk_text_buffer_get_bounds(bufferHandle, &start, &end);
		address = gtk_text_buffer_get_text(bufferHandle, &start, &end, TRUE);
	}
	if (address == 0) {
		return _gtk_alloc_set_text(_text, user_data, address, -1, enc);
	} else {
		/*int length = strlen(address);
		 byte [] buffer = new byte [length];
		 OS.memmove(buffer, address, length);
		 if ((style & SWT.MULTI) != 0)
		 OS.g_free(address);

		 char [] result = Converter.mbcsToWcs (null, buffer);
		 Arrays.fill(buffer, (byte) 0);
		 if (segments != null) {
		 result = deprocessText(result, 0, -1);
		 }
		 return result;*/
		wresult ret = _gtk_alloc_set_text(_text, user_data, address, -1, enc);
		if ((_W_WIDGET(text)->style & W_MULTI) != 0)
			g_free(address);
		return ret;
	}
}
wresult _w_text_get_text_range(w_text *text, w_range *range) {
}
wresult _w_text_get_text_limit(w_text *text) {
}
wresult _w_text_get_top_index(w_text *text) {
}
wresult _w_text_get_top_pixel(w_text *text) {
}
wresult _w_text_insert(w_text *text, const char *string) {
}
wresult _w_text_paste(w_text *text) {
}
wresult _w_text_select_all(w_text *text) {
}
wresult _w_text_set_doubleclick_enabled(w_text *text, int doubleClick) {
}
wresult _w_text_set_echochar(w_text *text, char echo) {
}
wresult _w_text_set_editable(w_text *text, int editable) {
}
wresult _w_text_set_message(w_text *text, const char *message, size_t length,
		int enc) {
	size_t newlength;
	_W_TEXT(text)->message = _gtk_text_fix(message, length, &newlength,
			enc | _GTK_TEXT_FIX_REQUIRED_NULL | _GTK_TEXT_FIX_COPY);
	return _W_TEXT(text)->message != 0 ? W_TRUE : W_FALSE;
}
wresult _w_text_set_selection(w_text *text, int start) {
}
wresult _w_text_set_selection_range(w_text *text, w_range *selection) {
}
void _w_text_set_tab_stops(w_text *text, int tabs) {
	int tabWidth = _w_text_get_tab_width(text);
	tabWidth = tabWidth * tabs;
	PangoTabArray *tabArray = pango_tab_array_new(1, FALSE);
	pango_tab_array_set_tab(tabArray, 0, PANGO_TAB_LEFT, tabWidth);
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		gtk_entry_set_tabs(GTK_ENTRY(_W_WIDGET(text)->handle), tabArray);
	} else {
		gtk_text_view_set_tabs(GTK_TEXT_VIEW(_W_WIDGET(text)->handle),
				tabArray);
	}
	pango_tab_array_free(tabArray);
}
wresult _w_text_set_tabs(w_text *text, int tabs) {
	if (tabs < 0)
		return W_FALSE;
	_w_text_set_tab_stops(text, tabs);
	return W_TRUE;
}
wresult _w_text_set_text(w_text *text, const char *string, size_t length,
		int enc) {
	wresult result = W_ERROR_NO_MEMORY;
	//clearSegments (false);
	GtkWidget *handle = _W_WIDGET(text)->handle;
	if ((_W_WIDGET(text)->style & W_SINGLE) != 0) {
		size_t newlength;
		char *s = _gtk_text_fix(string, length, &newlength,
				enc | _GTK_TEXT_FIX_REQUIRED_NULL);
		if (s != 0) {
			g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_CHANGED);
			g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_DELETE_TEXT);
			g_signal_handlers_block_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_INSERT_TEXT);
			gtk_entry_set_text(GTK_ENTRY(handle), s);
			g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_CHANGED);
			g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_DELETE_TEXT);
			g_signal_handlers_unblock_matched(handle, G_SIGNAL_MATCH_DATA, 0, 0,
					0, 0, (void*) SIGNAL_INSERT_TEXT);
			result = W_TRUE;
		}
		_gtk_text_free(string, s);
	} else {
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(handle));
		size_t newlength;
		char *s = _gtk_text_fix(string, length, &newlength, enc);
		if (s != 0) {
			g_signal_handlers_block_matched(bufferHandle, G_SIGNAL_MATCH_DATA,
					0, 0, 0, 0, (void*) SIGNAL_CHANGED);
			g_signal_handlers_block_matched(bufferHandle, G_SIGNAL_MATCH_DATA,
					0, 0, 0, 0, (void*) SIGNAL_DELETE_RANGE);
			g_signal_handlers_block_matched(bufferHandle, G_SIGNAL_MATCH_DATA,
					0, 0, 0, 0, (void*) SIGNAL_TEXT_BUFFER_INSERT_TEXT);
			gtk_text_buffer_set_text(bufferHandle, s, newlength);
			g_signal_handlers_unblock_matched(bufferHandle, G_SIGNAL_MATCH_DATA,
					0, 0, 0, 0, (void*) SIGNAL_CHANGED);
			g_signal_handlers_unblock_matched(bufferHandle, G_SIGNAL_MATCH_DATA,
					0, 0, 0, 0, (void*) SIGNAL_DELETE_RANGE);
			g_signal_handlers_unblock_matched(bufferHandle, G_SIGNAL_MATCH_DATA,
					0, 0, 0, 0, (void*) SIGNAL_TEXT_BUFFER_INSERT_TEXT);
			result = W_TRUE;
		}
		_gtk_text_free(string, s);
		if (result > 0) {
			GtkTextIter position;
			gtk_text_buffer_get_iter_at_offset(bufferHandle, &position, 0);
			gtk_text_buffer_place_cursor(bufferHandle, &position);
			GtkTextMark *mark = gtk_text_buffer_get_insert(bufferHandle);
			gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(handle), mark, 0, TRUE,
					0, 0);
		}
	}
	if (result > 0) {
		w_event e;
		e.type = W_EVENT_MODIFY;
		e.widget = W_WIDGET(text);
		e.platform_event = 0;
		e.time = 0;
		e.data = 0;
		_w_widget_send_event(W_WIDGET(text), &e);
		if ((_W_WIDGET(text)->style & W_SEARCH) != 0) {
			if ((_W_WIDGET(text)->style & W_ICON_CANCEL) != 0) {
				gtk_entry_set_icon_sensitive(GTK_ENTRY(handle),
						GTK_ENTRY_ICON_SECONDARY, TRUE);
			}
		}
	}
	//applySegments ();
	return result;
}
wresult _w_text_set_text_limit(w_text *text, int limit) {
}
wresult _w_text_set_top_index(w_text *text, int index) {
}
wresult _w_text_show_selection(w_text *text) {
}
wuint64 _w_text_check_style(w_widget *control, wuint64 style) {
	if ((style & W_SEARCH) != 0) {
		style |= W_SINGLE | W_BORDER;
		style &= ~W_PASSWORD;
		/*
		 * NOTE: ICON_CANCEL has the same value as H_SCROLL and
		 * ICON_SEARCH has the same value as V_SCROLL so they are
		 * cleared because W_SINGLE is set.
		 */
	}
	if ((style & W_SINGLE) != 0 && (style & W_MULTI) != 0) {
		style &= ~W_MULTI;
	}
	style = _w_widget_check_bits(style, W_LEFT, W_CENTER, W_RIGHT, 0, 0, 0);
	if ((style & W_SINGLE) != 0)
		style &= ~(W_HSCROLL | W_VSCROLL | W_WRAP);
	if ((style & W_WRAP) != 0) {
		style |= W_MULTI;
		style &= ~W_HSCROLL;
	}
	if ((style & W_MULTI) != 0)
		style &= ~W_PASSWORD;
	if ((style & (W_SINGLE | W_MULTI)) != 0)
		return style;
	if ((style & (W_HSCROLL | W_VSCROLL)) != 0)
		return style | W_MULTI;
	return style | W_SINGLE;
}

wresult _w_text_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	GtkWidget *fixedHandle, *handle = 0, *scrolledHandle;
	GtkTextBuffer *bufferHandle;
	GtkStyleContext *context;
	wuint64 style = _W_WIDGET(widget)->style;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_MENU;
	if ((style & W_READ_ONLY) != 0) {
		if (_w_scrollable_apply_theme_background(W_SCROLLABLE(widget),
				_W_SCROLLABLE_RESERVED(reserved)) == 1) {
			_W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
		}
	}
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	if ((style & W_SINGLE) != 0) {
		handle = gtk_entry_new();
		if (handle == 0)
			goto _err;
		gtk_container_add(GTK_CONTAINER(fixedHandle), handle);
		gtk_editable_set_editable(GTK_EDITABLE(handle),
				(style & W_READ_ONLY) == 0);
		/*
		 * We need to handle borders differently in GTK3.20+. GtkEntry without frame will have a blank background color.
		 * So let's set border via css and override the background in this case to be COLOR_LIST_BACKGROUND.
		 */
		if (GTK_VERSION >= VERSION(3, 20, 0)) {
			if ((style & W_BORDER) == 0) {
				gtk_entry_set_has_frame(GTK_ENTRY(handle), FALSE);
				context = gtk_widget_get_style_context(handle);
				//String background = display.gtk_rgba_to_css_string(display.COLOR_LIST_BACKGROUND_RGBA);
				//gtk_css_provider_load_from_css(context, "entry {border: solid; background: " + background + ";}");
				gtk_style_context_invalidate(context);
			}
		} else {
			gtk_entry_set_has_frame(GTK_ENTRY(handle), (style & W_BORDER) != 0);
		}
		gtk_entry_set_visibility(GTK_ENTRY(handle), (style & W_PASSWORD) == 0);
		float alignment = 0.0f;
		if ((style & W_CENTER) != 0)
			alignment = 0.5f;
		if ((style & W_RIGHT) != 0)
			alignment = 1.0f;
		if (alignment > 0.0f) {
			gtk_entry_set_alignment(GTK_ENTRY(handle), alignment);
		}
	} else {
		scrolledHandle = gtk_scrolled_window_new(0, 0);
		if (scrolledHandle == 0)
			goto _err;
		handle = gtk_text_view_new();
		if (handle == 0)
			goto _err;
		bufferHandle = gtk_text_view_get_buffer(GTK_TEXT_VIEW(handle));
		if (bufferHandle == 0)
			goto _err;
		g_object_set_qdata(G_OBJECT(scrolledHandle), gtk_toolkit->quark[0],
				widget);
		gtk_container_add(GTK_CONTAINER(fixedHandle), scrolledHandle);
		gtk_container_add(GTK_CONTAINER(scrolledHandle), handle);
		gtk_text_view_set_editable(GTK_TEXT_VIEW(handle),
				(style & W_READ_ONLY) == 0);
		if ((style & W_WRAP) != 0)
			gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(handle),
					GTK_WRAP_WORD_CHAR);
		GtkPolicyType hsp = (style & W_HSCROLL) != 0 ? /*GTK_POLICY_ALWAYS*/
		GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
		GtkPolicyType vsp = (style & W_VSCROLL) != 0 ? /*GTK_POLICY_ALWAYS*/
		GTK_POLICY_AUTOMATIC : GTK_POLICY_NEVER;
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledHandle), hsp,
				vsp);
		if ((style & W_BORDER) != 0) {
			gtk_scrolled_window_set_shadow_type(
					GTK_SCROLLED_WINDOW(scrolledHandle), GTK_SHADOW_ETCHED_IN);
		}
		GtkJustification just = GTK_JUSTIFY_LEFT;
		if ((style & W_CENTER) != 0)
			just = GTK_JUSTIFY_CENTER;
		if ((style & W_RIGHT) != 0)
			just = GTK_JUSTIFY_RIGHT;
		gtk_text_view_set_justification(GTK_TEXT_VIEW(handle), just);
	}
#if GTK3
	//imContext = imContextLast();
	if ((style & W_SINGLE) != 0) {
		gtk_entry_set_width_chars(GTK_ENTRY(handle), 6);
	}
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
	w_font *font = _w_control_default_font(W_CONTROL(widget));
	_w_control_set_font_description(W_CONTROL(widget), _W_FONT(font)->handle,
			_W_CONTROL_RESERVED(reserved));
#endif
	g_object_set_qdata(G_OBJECT(handle), gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);

	_W_WIDGET(widget)->handle = handle;
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
GtkWidget* _w_text_scrolledHandle(w_widget *control,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(control)->style & W_SINGLE) != 0) {
		return 0;
	} else {
		return gtk_widget_get_parent(_W_WIDGET(control)->handle);
	}
}
GtkWidget* _w_text_fixedHandle(w_widget *control,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(control)->style & W_SINGLE) != 0) {
		return gtk_widget_get_parent(_W_WIDGET(control)->handle);
	} else {
		return gtk_widget_get_parent(
				gtk_widget_get_parent(_W_WIDGET(control)->handle));
	}
}
int _w_text_compute_trim(w_widget *widget, w_event_compute_trim *e,
		_w_widget_reserved *reserved) {
	w_event_compute_size ee;
	w_size widthNative;
	_w_scrollable_compute_trim(widget, e, reserved);
	int xborder = 0, yborder = 0;
	if ((_W_WIDGET(widget)->style & W_SINGLE) != 0) {
#if GTK3
		GtkBorder tmp;
		GtkStyleContext *context = gtk_widget_get_style_context(
		_W_WIDGET(widget)->handle);
		if (GTK_VERSION < VERSION(3, 18, 0)) {
			gtk_style_context_get_padding(context, GTK_STATE_FLAG_NORMAL, &tmp);
		} else {
			gtk_style_context_get_padding(context,
					gtk_widget_get_state_flags(_W_WIDGET(widget)->handle),
					&tmp);
		}
		e->result->x -= tmp.left;
		e->result->y -= tmp.top;
		e->result->width += tmp.left + tmp.right;
		if (tmp.bottom == 0 && tmp.top == 0) {
			memset(&ee, 0, sizeof(ee));
			ee.wHint = e->result->width;
			ee.hHint = W_DEFAULT;
			ee.size = &widthNative;
			_w_control_compute_native_size(widget, _W_WIDGET(widget)->handle,
					&ee, reserved);
			e->result->height = widthNative.height;
		} else {
			e->result->height += tmp.top + tmp.bottom;
		}
		if ((_W_WIDGET(widget)->style & W_BORDER) != 0) {
			if (GTK_VERSION < VERSION(3, 18, 0)) {
				gtk_style_context_get_border(context, GTK_STATE_FLAG_NORMAL,
						&tmp);
			} else {
				gtk_style_context_get_border(context,
						gtk_widget_get_state_flags(_W_WIDGET(widget)->handle),
						&tmp);
			}
			e->result->x -= tmp.left;
			e->result->y -= tmp.top;
			e->result->width += tmp.left + tmp.right;
			e->result->height += tmp.top + tmp.bottom;
		}
		GdkRectangle icon_area;
		gtk_entry_get_icon_area(GTK_ENTRY(_W_WIDGET(widget)->handle),
				GTK_ENTRY_ICON_PRIMARY, &icon_area);
		e->result->x -= icon_area.width;
		e->result->width += icon_area.width;
		gtk_entry_get_icon_area(GTK_ENTRY(_W_WIDGET(widget)->handle),
				GTK_ENTRY_ICON_SECONDARY, &icon_area);
		e->result->width += icon_area.width;
#endif
#if GTK2
#endif
	} else {
		int borderWidth = gtk_container_get_border_width(
				GTK_CONTAINER(_W_WIDGET(widget)->handle));
		xborder += borderWidth;
		yborder += borderWidth;
	}
	int property;
	gtk_widget_style_get(_W_WIDGET(widget)->handle, "interior-focus", &property,
	NULL);
	if (property == 0) {
		gtk_widget_style_get(_W_WIDGET(widget)->handle, "focus-line-width",
				&property, NULL);
		xborder += property;
		yborder += property;
	}
	e->result->x -= xborder;
	e->result->y -= yborder;
	e->result->width += 2 * xborder;
	e->result->height += 2 * yborder;
	e->result->width += SPACE_FOR_CURSOR;
	return W_TRUE;
}

int _w_text_compute_size(w_widget *widget, w_event_compute_size *e,
		_w_widget_reserved *reserved) {
	int w, h;
	w_event_compute_trim ee;
	w_rect rect, result;
	if ((_W_WIDGET(widget)->style & W_SINGLE) != 0) {
		gtk_widget_realize(_W_WIDGET(widget)->handle);
		PangoLayout *layout = gtk_entry_get_layout(
				GTK_ENTRY(_W_WIDGET(widget)->handle));
		pango_layout_get_pixel_size(layout, &w, &h);
	} else {
		GtkTextIter start, end;
		GtkTextBuffer *bufferHandle = gtk_text_view_get_buffer(
				GTK_TEXT_VIEW(_W_WIDGET(widget)->handle));
		gtk_text_buffer_get_bounds(bufferHandle, &start, &end);
		gchar *text = gtk_text_buffer_get_text(bufferHandle, &start, &end,
		TRUE);
		PangoLayout *layout = gtk_widget_create_pango_layout(
		_W_WIDGET(widget)->handle, text);
		g_free(text);
		pango_layout_set_width(layout, e->wHint * PANGO_SCALE);
		pango_layout_get_pixel_size(layout, &w, &h);
		g_object_unref(layout);
	}
	int width = w;
	int height = h;
	if ((_W_WIDGET(widget)->style & W_SINGLE) != 0
			&& _W_TEXT(widget)->message != 0) {
		PangoLayout *layout = gtk_widget_create_pango_layout(
		_W_WIDGET(widget)->handle, _W_TEXT(widget)->message);
		pango_layout_get_pixel_size(layout, &w, &h);
		g_object_unref(layout);
		width = w_max(width, w);
	}
	if (width == 0)
		width = DEFAULT_WIDTH;
	if (height == 0)
		height = DEFAULT_HEIGHT;
	width = e->wHint == W_DEFAULT ? width : e->wHint;
	height = e->hHint == W_DEFAULT ? height : e->hHint;
	memset(&ee, 0, sizeof(ee));
	ee.rect = &rect;
	ee.result = &result;
	rect.x = 0;
	rect.y = 0;
	rect.width = width;
	rect.height = height;
	memset(&result, 0, sizeof(result));
	_w_text_compute_trim(widget, &ee, reserved);
	e->size->width = result.width;
	e->size->height = result.height;
	return W_TRUE;
}
void _w_text_class_init(struct _w_text_class *clazz) {
	_w_scrollable_class_init(W_SCROLLABLE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_TEXT;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_text_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_text);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_text);
	/*
	 * public function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_text_create;
	clazz->append = _w_text_append;
	clazz->clear_selection = _w_text_clear_selection;
	clazz->copy = _w_text_copy;
	clazz->cut = _w_text_cut;
	clazz->get_caret_line_number = _w_text_get_caret_line_number;
	clazz->get_caret_location = _w_text_get_caret_location;
	clazz->get_caret_position = _w_text_get_caret_position;
	clazz->get_char_count = _w_text_get_char_count;
	clazz->get_doubleclick_enabled = _w_text_get_doubleclick_enabled;
	clazz->get_echo_char = _w_text_get_echo_char;
	clazz->get_editable = _w_text_get_editable;
	clazz->get_line_count = _w_text_get_line_count;
	clazz->get_line_delimiter = _w_text_get_line_delimiter;
	clazz->get_line_height = _w_text_get_line_height;
	clazz->get_message = _w_text_get_message;
	clazz->get_position = _w_text_get_position;
	clazz->get_selection = _w_text_get_selection;
	clazz->get_selection_count = _w_text_get_selection_count;
	clazz->get_selection_text = _w_text_get_selection_text;
	clazz->get_tabs = _w_text_get_tabs;
	clazz->get_text = _w_text_get_text;
	clazz->get_text_range = _w_text_get_text_range;
	clazz->get_text_limit = _w_text_get_text_limit;
	clazz->get_top_index = _w_text_get_top_index;
	clazz->get_top_pixel = _w_text_get_top_pixel;
	clazz->insert = _w_text_insert;
	clazz->paste = _w_text_paste;
	clazz->select_all = _w_text_select_all;
	clazz->set_doubleclick_enabled = _w_text_set_doubleclick_enabled;
	clazz->set_echochar = _w_text_set_echochar;
	clazz->set_editable = _w_text_set_editable;
	clazz->set_message = _w_text_set_message;
	clazz->set_selection = _w_text_set_selection;
	clazz->set_selection_range = _w_text_set_selection_range;
	clazz->set_tabs = _w_text_set_tabs;
	clazz->set_text = _w_text_set_text;
	clazz->set_text_limit = _w_text_set_text_limit;
	clazz->set_top_index = _w_text_set_top_index;
	clazz->show_selection = _w_text_show_selection;
	//reserved
	_w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->fixedHandle = _w_text_fixedHandle;
	_W_WIDGET_RESERVED(reserved)->create_handle = _w_text_create_handle;
	_W_WIDGET_RESERVED(reserved)->check_style = _w_text_check_style;
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_text_fixedHandle;
	_W_WIDGET_RESERVED(reserved)->create_widget = _w_text_create_widget;
	_W_SCROLLABLE_RESERVED(reserved)->scrolledHandle = _w_text_scrolledHandle;
	_W_SCROLLABLE_RESERVED(reserved)->apply_theme_background =
			_w_text_apply_theme_background;
	reserved->widget.compute_size = _w_text_compute_size;
	reserved->widget.compute_trim = _w_text_compute_trim;
}
#endif
