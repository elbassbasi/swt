/*
 * textlayout.c
 *
 *  Created on: 17 févr. 2020
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "graphics.h"
#include "../widgets/toolkit.h"
void w_textlayout_init(w_textlayout *textlayout) {
	_W_TEXTLAYOUT(textlayout)->layout = 0;
}
void w_textlayout_dispose(w_textlayout *textlayout) {
	if (_W_TEXTLAYOUT(textlayout)->layout != 0) {
		if (_W_TEXTLAYOUT(textlayout)->layout != 0)
			g_object_unref(_W_TEXTLAYOUT(textlayout)->layout);
		if (_W_TEXTLAYOUT(textlayout)->context != 0)
			g_object_unref(_W_TEXTLAYOUT(textlayout)->context);
		_W_TEXTLAYOUT(textlayout)->layout = 0;
	}
}
wresult w_textlayout_isok(w_textlayout *textlayout) {
	if (textlayout == 0)
		return W_FALSE;
	return _W_TEXTLAYOUT(textlayout)->layout != 0;
}
/*
 *  Translate a client offset to an internal offset
 */
int _w_textlayout_translate_offset(w_textlayout *textlayout, int offset) {
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	const char *ptr = pango_layout_get_text(_W_TEXTLAYOUT(textlayout)->layout);
	int length = strlen(ptr);
	if (offset < 0 || offset > length)
		offset = length;
	if (offset < length) {
		while (offset >= 0 && ptr[offset] >= 128) {
			offset--;
		}
	}
	return offset;
}

/*
 *  Translate an internal offset to a client offset
 */
int _w_textlayout_untranslate_offset(w_textlayout *textlayout, int offset) {
	/*int length = text.length();
	 if (length == 0)
	 return offset;
	 if (invalidOffsets == null)
	 return offset;
	 int i = 0;
	 while (i < invalidOffsets.length && offset > invalidOffsets[i]) {
	 i++;
	 }*/
	return offset /*- i*/;
}
wresult w_textlayout_create(w_textlayout *textlayout) {
	w_textlayout_dispose(textlayout);
	PangoContext *context = gdk_pango_context_get();
	if (context == 0) {
		return W_ERROR_NO_HANDLES;
	}
	pango_context_set_language(context, gtk_get_default_language());
	pango_context_set_base_dir(context, PANGO_DIRECTION_LTR);
	PangoLayout *layout = pango_layout_new(context);
	if (layout == 0) {
		return W_ERROR_NO_HANDLES;
	}
	memset(textlayout, 0, sizeof(_w_textlayout));
	//pango_layout_set_font_description( device.systemFont.handle);
	pango_layout_set_wrap(layout, PANGO_WRAP_WORD_CHAR);
	pango_layout_set_tabs(layout, gtk_toolkit->empty_tab);
	pango_layout_set_auto_dir(layout, FALSE);
	_W_TEXTLAYOUT(textlayout)->layout = layout;
	_W_TEXTLAYOUT(textlayout)->context = context;
	_W_TEXTLAYOUT(textlayout)->wrapWidth = -1;
	_W_TEXTLAYOUT(textlayout)->ascentInPoints = -1;
	_W_TEXTLAYOUT(textlayout)->descentInPoints = -1;
	return W_TRUE;
}

// Bug 477950: In order to support GTK2 and GTK3 colors simultaneously, this method's parameters
// were modified to accept SWT Color objects instead of GdkColor structs.
void _w_textlayout_draw_with_cairo(w_textlayout *textlayout, w_graphics *gc,
		int x, int y, int start, int end, int fullSelection, w_color fg,
		w_color bg) {
	cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	cairo_save(cairo);
	if (!fullSelection) {
		cairo_move_to(cairo, x, y);
		pango_cairo_show_layout(cairo, layout);
	}
	int ranges[] = { start, end };
	cairo_region_t *rgn = gdk_pango_layout_get_clip_region(layout, x, y, ranges,
			1);
	if (rgn != 0) {
		gdk_cairo_region(cairo, rgn);
		cairo_clip(cairo);
#if GTK3
		cairo_set_source_rgba(cairo, W_RED(bg), W_GREEN(bg), W_BLUE(bg),
				W_ALPHA(bg));
#else
			cairo_set_source_rgba(cairo, (bg.handle.red & 0xFFFF) / (float)0xFFFF, (bg.handle.green & 0xFFFF) / (float)0xFFFF, (bg.handle.blue & 0xFFFF) / (float)0xFFFF, data.alpha / (float)0xFF);
#endif
		cairo_paint(cairo);
		cairo_region_destroy(rgn);
	}
#if GTK3
	cairo_set_source_rgba(cairo, W_RED(fg), W_GREEN(fg), W_BLUE(fg),
			W_ALPHA(fg));
#else
		Cairo.cairo_set_source_rgba(cairo, (fg.handle.red & 0xFFFF) / (float)0xFFFF, (fg.handle.green & 0xFFFF) / (float)0xFFFF, (fg.handle.blue & 0xFFFF) / (float)0xFFFF, data.alpha / (float)0xFF);
#endif
	cairo_move_to(cairo, x, y);
	pango_layout_set_attributes(layout, _W_TEXTLAYOUT(textlayout)->selAttrList);
	pango_cairo_show_layout(cairo, layout);
	pango_layout_set_attributes(layout, _W_TEXTLAYOUT(textlayout)->attrList);
	cairo_restore(cairo);
}
wresult w_textlayout_draw(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, int selectionStart, int selectionEnd,
		w_color selectionForeground, w_color selectionBackground, int flags) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (!w_textlayout_isok(textlayout))
		return W_ERROR_INVALID_ARGUMENT;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	_w_graphics_check(gc, GRAPHICS_STATE_FOREGROUND);
	int length = pango_layout_get_character_count(layout);
	x += MIN(_W_TEXTLAYOUT(textlayout)->indent,
			_W_TEXTLAYOUT(textlayout)-> wrapIndent);
	wresult hasSelection = selectionStart <= selectionEnd && selectionStart != -1
			&& selectionEnd != -1;
	cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
	if ((flags & (W_FULL_SELECTION | W_DELIMITER_SELECTION)) != 0
			&& (hasSelection || (flags & W_LAST_LINE_SELECTION) != 0)) {
		int nAttrs;
		PangoLogAttr *attrs = 0;
		PangoRectangle rect;
		int lineCount = pango_layout_get_line_count(layout);
		const char *ptr = pango_layout_get_text(layout);
		PangoLayoutIter *iter = pango_layout_get_iter(layout);
		if (selectionBackground == 0) {
			selectionBackground = w_toolkit_get_system_color(0,
					W_COLOR_LIST_SELECTION);
		}
		cairo_save(cairo);
#if GTK3
		cairo_set_source_rgba(cairo, W_RED(selectionBackground),
				W_GREEN(selectionBackground), W_BLUE(selectionBackground),
				W_ALPHA(selectionBackground));
#else
			GdkColor color = selectionBackground.handle;
			cairo_set_source_rgba(cairo, (color.red & 0xFFFF) / (float)0xFFFF, (color.green & 0xFFFF) / (float)0xFFFF, (color.blue & 0xFFFF) / (float)0xFFFF, data.alpha / (float)0xFF);
#endif
		int lineIndex = 0;
		do {
			int lineEnd;
			pango_layout_iter_get_line_extents(iter, NULL, &rect);
			if (pango_layout_iter_next_line(iter)) {
				int bytePos = pango_layout_iter_get_index(iter);
				//lineEnd = g_utf16_pointer_to_offset(ptr, ptr + bytePos);
			} else {
				//lineEnd = g_utf16_strlen(ptr, -1);
			}
			int extent = FALSE;
			if (lineIndex == lineCount - 1
					&& (flags & W_LAST_LINE_SELECTION) != 0) {
				extent = TRUE;
			} else {
				if (attrs == 0)
					pango_layout_get_log_attrs(layout, &attrs, &nAttrs);
				if (!attrs->is_line_break) {
					if (selectionStart <= lineEnd && lineEnd <= selectionEnd)
						extent = TRUE;
				} else {
					if (selectionStart <= lineEnd && lineEnd < selectionEnd
							&& (flags & W_FULL_SELECTION) != 0) {
						extent = TRUE;
					}
				}
			}
			if (extent) {
				int lineX = x + PANGO_PIXELS(rect.x) + PANGO_PIXELS(rect.width);
				int lineY = y + PANGO_PIXELS(rect.y);
				int height = PANGO_PIXELS(rect.height);
				if (_W_TEXTLAYOUT(textlayout)->ascentInPoints != -1
						&& _W_TEXTLAYOUT(textlayout)->descentInPoints != -1) {
					height =
							MAX(height,
									_W_TEXTLAYOUT(textlayout)->ascentInPoints + _W_TEXTLAYOUT(textlayout)->descentInPoints);
				}
				int width =
						(flags & W_FULL_SELECTION) != 0 ? 0x7fff : height / 3;
				cairo_rectangle(cairo, lineX, lineY, width, height);
				cairo_fill(cairo);
			}
			lineIndex++;
		} while (lineIndex < lineCount);
		pango_layout_iter_free(iter);
		if (attrs != 0)
			g_free(attrs);
		cairo_restore(cairo);
	}
	if (length == 0)
		return W_FALSE;
	if (!hasSelection) {
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_save(cairo);
			cairo_scale(cairo, -1, 1);
			//cairo_translate(cairo, -2 * x - width(), 0);
		}
		cairo_move_to(cairo, x, y);
		pango_cairo_show_layout(cairo, layout);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_restore(cairo);
		}
	} else {
		selectionStart = w_min(w_max(0, selectionStart), length - 1);
		selectionEnd = w_min(w_max(0, selectionEnd), length - 1);
		length = g_utf8_strlen(pango_layout_get_text(layout), -1);
		/*selectionStart = translateOffset(selectionStart);
		 selectionEnd = translateOffset(selectionEnd);
		 if (selectionForeground == null) selectionForeground = device.getSystemColor(SWT.COLOR_LIST_SELECTION_TEXT);
		 if (selectionBackground == null) selectionBackground = device.getSystemColor(SWT.COLOR_LIST_SELECTION);*/
		int fullSelection = selectionStart == 0 && selectionEnd == length - 1;
		if (fullSelection) {
			const char *ptr = pango_layout_get_text(layout);
			/*if ((data.style & SWT.MIRRORED) != 0) {
			 cairo_save(cairo);
			 cairo_scale(cairo, -1,  1);
			 cairo_translate(cairo, -2 * x - width(), 0);
			 }*/
			_w_textlayout_draw_with_cairo(textlayout, gc, x, y, 0, strlen(ptr),
					fullSelection, selectionForeground, selectionBackground);
			/*if ((data.style & SWT.MIRRORED) != 0) {
			 cairo_restore(cairo);
			 }*/
		} else {
			const char *ptr = pango_layout_get_text(layout);
			int _strlen = strlen(ptr);
			int byteSelStart = 0; //(g_utf16_offset_to_pointer(ptr, selectionStart) - ptr);
			int byteSelEnd = _strlen; //(g_utf16_offset_to_pointer(ptr, selectionEnd + 1) - ptr);
			byteSelStart = w_min(byteSelStart, _strlen);
			byteSelEnd = w_min(byteSelEnd, _strlen);
			/*if ((data.style & SWT.MIRRORED) != 0) {
			 cairo_save(cairo);
			 cairo_scale(cairo, -1,  1);
			 cairo_translate(cairo, -2 * x - width(), 0);
			 }*/
			_w_textlayout_draw_with_cairo(textlayout, gc, x, y, byteSelStart,
					byteSelEnd, fullSelection, selectionForeground,
					selectionBackground);
			/*if ((data.style & SWT.MIRRORED) != 0) {
			 cairo_restore(cairo);
			 }*/
		}
	}
	cairo_new_path(cairo);
	return W_TRUE;
}
int w_textlayout_get_alignment(w_textlayout *textlayout) {
	if (!w_textlayout_isok(textlayout))
		return W_LEFT;
	int align = pango_layout_get_alignment(_W_TEXTLAYOUT(textlayout)->layout);
	int rtl = pango_context_get_base_dir(_W_TEXTLAYOUT(textlayout)->context)
			== PANGO_DIRECTION_RTL;
	switch (align) {
	case PANGO_ALIGN_LEFT:
		return rtl ? W_RIGHT : W_LEFT;
	case PANGO_ALIGN_RIGHT:
		return rtl ? W_LEFT : W_RIGHT;
	}
	return W_CENTER;
}
int w_textlayout_get_ascent(w_textlayout *textlayout) {
	if (!w_textlayout_isok(textlayout))
		return 0;
	return _W_TEXTLAYOUT(textlayout)->ascentInPoints;
}
void w_textlayout_get_bounds(w_textlayout *textlayout, int start, int end,
		w_rect *rect, int enc) {
}
int w_textlayout_get_descent(w_textlayout *textlayout) {
}
w_font* w_textlayout_get_font(w_textlayout *textlayout) {
}
int w_textlayout_get_indent(w_textlayout *textlayout) {
}
wresult w_textlayout_get_justify(w_textlayout *textlayout) {
}
int w_textlayout_get_level(w_textlayout *textlayout, int offset, int enc) {
}
void w_textlayout_get_line_bounds(w_textlayout *textlayout, int lineIndex,
		w_rect *rect) {
}
int w_textlayout_get_line_count(w_textlayout *textlayout) {
}
int w_textlayout_get_line_index(w_textlayout *textlayout, int offset, int enc) {
}
wresult w_textlayout_get_line_metrics(w_textlayout *textlayout, int lineIndex,
		w_fontmetrics *fontmetrics) {
}
int w_textlayout_get_line_offset(w_textlayout *textlayout, int lineIndex,
		int enc) {
}
wresult w_textlayout_get_location(w_textlayout *textlayout, int offset,
		int trailing, w_point *pt, int enc) {
}
int w_textlayout_get_next_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
}
int w_textlayout_get_offset(w_textlayout *textlayout, w_point *point,
		int *trailing, int trailing_count, int enc) {
}
int w_textlayout_get_orientation(w_textlayout *textlayout) {
}
int w_textlayout_get_previous_offset(w_textlayout *textlayout, int offset,
		int movement, int enc) {
}
typedef struct w_iterator_textlayout_ranges {
	w_basic_iterator base;
	w_textlayout *textlayout;
	PangoAttrIterator *iter;
	gboolean next;
	size_t count;
} w_iterator_textlayout_ranges;
wresult w_iterator_textlayout_ranges_close(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	pango_attr_iterator_destroy(it->iter);
	return W_TRUE;
}
wresult w_iterator_textlayout_ranges_next(w_iterator *_it, void *obj) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	gboolean ret = it->next;
	PangoAttrColor *color;
	PangoAttrInt *attrInt;
	if (ret) {
		w_textstyle_range *range = (w_textstyle_range*) obj;
		pango_attr_iterator_range(it->iter, &range->range.start,
				&range->range.end);
		color = (PangoAttrColor*) pango_attr_iterator_get(it->iter,
				PANGO_ATTR_FOREGROUND);
		if (color != 0) {
			range->style.foreground = W_RGB(color->color.red / 0x100,
					color->color.green / 0x100, color->color.blue / 0x100);
		} else {
			range->style.foreground = 0;
		}
		color = (PangoAttrColor*) pango_attr_iterator_get(it->iter,
				PANGO_ATTR_BACKGROUND);
		if (color != 0) {
			range->style.background = W_RGB(color->color.red / 0x100,
					color->color.green / 0x100, color->color.blue / 0x100);
		} else {
			range->style.background = 0;
		}
		it->next = pango_attr_iterator_next(it->iter);
	}
	return ret;
}
wresult w_iterator_textlayout_ranges_reset(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	pango_attr_iterator_destroy(it->iter);
	it->iter = pango_attr_list_get_iterator(
	_W_TEXTLAYOUT(it->textlayout)->attrList);
	return W_TRUE;
}
wresult w_iterator_textlayout_ranges_remove(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;
	return W_ERROR_NOT_IMPLEMENTED;
}
size_t w_iterator_textlayout_ranges_get_count(w_iterator *_it) {
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) _it;

}
struct _w_iterator_class w_iterator_textlayout_ranges_class = { //
		w_iterator_textlayout_ranges_close, //
				w_iterator_textlayout_ranges_next, //
				w_iterator_textlayout_ranges_reset, //
				w_iterator_textlayout_ranges_remove, //
				w_iterator_textlayout_ranges_get_count, //
		};
wresult w_textlayout_get_ranges(w_textlayout *textlayout, w_iterator *ranges) {
	w_iterator_close(ranges);
	if (textlayout == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NULL_ARGUMENT;
	w_iterator_textlayout_ranges *it = (w_iterator_textlayout_ranges*) ranges;
	it->base.clazz = &w_iterator_textlayout_ranges_class;
	it->textlayout = textlayout;
	it->iter = pango_attr_list_get_iterator(
	_W_TEXTLAYOUT(textlayout)->attrList);
	it->next = TRUE;
	it->count = -1;
	return W_TRUE;
}
int w_textlayout_get_spacing(w_textlayout *textlayout) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	return pango_layout_get_spacing(layout);
}
wresult w_textlayout_get_style(w_textlayout *textlayout, int offset,
		w_textstyle *style, int enc) {
	/*checkLayout();
	 int length = text.length();
	 if (!(0 <= offset && offset < length)) SWT.error(SWT.ERROR_INVALID_RANGE);
	 for (int i=1; i<stylesCount; i++) {
	 StyleItem item = styles[i];
	 if (item.start > offset) {
	 return styles[i - 1].style;
	 }
	 }
	 return null;*/
}
int w_textlayout_get_tabs(w_textlayout *textlayout, int *tabs, int count) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	//return tabs;
}
wresult w_textlayout_get_text(w_textlayout *textlayout, w_alloc text,
		void *user_data) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	pango_layout_get_text(layout);
	return w_alloc_set_text(text, user_data, pango_layout_get_text(layout), -1);
}
int w_textlayout_get_text_direction(w_textlayout *textlayout) {
	return w_textlayout_get_orientation(textlayout);
}
int w_textlayout_get_width(w_textlayout *textlayout) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	return _W_TEXTLAYOUT(textlayout)->wrapWidth;
}
int w_textlayout_get_wrap_indent(w_textlayout *textlayout) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	return _W_TEXTLAYOUT(textlayout)->wrapIndent;
}
wresult w_textlayout_set_alignment(w_textlayout *textlayout, int alignment) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	int mask = W_LEFT | W_CENTER | W_RIGHT;
	alignment &= mask;
	if (alignment == 0)
		return W_TRUE;
	if ((alignment & W_LEFT) != 0)
		alignment = W_LEFT;
	if ((alignment & W_RIGHT) != 0)
		alignment = W_RIGHT;
	gboolean rtl = pango_context_get_base_dir(
	_W_TEXTLAYOUT(textlayout)->context) == PANGO_DIRECTION_RTL;
	PangoAlignment align = PANGO_ALIGN_CENTER;
	switch (alignment) {
	case W_LEFT:
		align = rtl ? PANGO_ALIGN_RIGHT : PANGO_ALIGN_LEFT;
		break;
	case W_RIGHT:
		align = rtl ? PANGO_ALIGN_LEFT : PANGO_ALIGN_RIGHT;
		break;
	}
	pango_layout_set_alignment(layout, align);
	return W_TRUE;
}
wresult w_textlayout_set_ascent(w_textlayout *textlayout, int ascent) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	if (ascent < -1)
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_TEXTLAYOUT(textlayout)->ascentInPoints == ascent)
		return W_TRUE;
	//freeRuns();
	_W_TEXTLAYOUT(textlayout)->ascentInPoints = ascent;
	return W_TRUE;
}
wresult w_textlayout_set_descent(w_textlayout *textlayout, int descent) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	if (descent < -1)
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_TEXTLAYOUT(textlayout)->descentInPoints == descent)
		return W_TRUE;
	//freeRuns();
	_W_TEXTLAYOUT(textlayout)->descentInPoints = descent;
	return W_TRUE;
}
wresult w_textlayout_set_font(w_textlayout *textlayout, w_font *font) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	if (font != 0 && _W_FONT(font)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	w_font *oldFont = _W_TEXTLAYOUT(textlayout)->font;
	if (oldFont == font)
		return W_TRUE;
	//freeRuns();
	_W_TEXTLAYOUT(textlayout)->font = font;
	if (oldFont != 0 && _W_FONT(oldFont)->handle == _W_FONT(font)->handle)
		return W_TRUE;
	PangoFontDescription *_font;
	if (font == 0) {
		w_font *sysFont = w_toolkit_get_system_font(w_toolkit_get_default());
		_font = _W_FONT(sysFont)->handle;
	} else {
		_font = _W_FONT(font)->handle;
	}
	pango_layout_set_font_description(layout, _font);
	return W_TRUE;
}
wresult w_textlayout_set_indent(w_textlayout *textlayout, int indent) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	if (indent < 0)
		return W_TRUE;
	if (_W_TEXTLAYOUT(textlayout)->indent == indent)
		return W_TRUE;
	_W_TEXTLAYOUT(textlayout)->indent = indent;
	pango_layout_set_indent(layout,
			(indent - _W_TEXTLAYOUT(textlayout)->wrapIndent) * PANGO_SCALE);
	if (_W_TEXTLAYOUT(textlayout)->wrapWidth != -1) {
		int margin = MIN(indent, _W_TEXTLAYOUT(textlayout)->wrapIndent);
		pango_layout_set_width(layout,
				(_W_TEXTLAYOUT(textlayout)->wrapWidth - margin) * PANGO_SCALE);
	}
	return W_TRUE;
}
wresult w_textlayout_set_justify(w_textlayout *textlayout, int justify) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	pango_layout_set_justify(layout, justify);
	return W_TRUE;
}
wresult w_textlayout_set_orientation(w_textlayout *textlayout,
		int orientation) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	int mask = W_RIGHT_TO_LEFT | W_LEFT_TO_RIGHT;
	orientation &= mask;
	if (orientation == 0)
		return W_TRUE;
	if ((orientation & W_LEFT_TO_RIGHT) != 0)
		orientation = W_LEFT_TO_RIGHT;
	PangoDirection baseDir =
			orientation == W_RIGHT_TO_LEFT ?
					PANGO_DIRECTION_RTL : PANGO_DIRECTION_LTR;
	if (pango_context_get_base_dir(_W_TEXTLAYOUT(textlayout)->context)
			== baseDir)
		return W_TRUE;
	//freeRuns();
	pango_context_set_base_dir(_W_TEXTLAYOUT(textlayout)->context, baseDir);
	pango_layout_context_changed(layout);
	PangoAlignment align = pango_layout_get_alignment(layout);
	if (align != PANGO_ALIGN_CENTER) {
		align = align == PANGO_ALIGN_LEFT ?
				PANGO_ALIGN_RIGHT : PANGO_ALIGN_LEFT;
		pango_layout_set_alignment(layout, align);
	}
	return W_TRUE;
}
wresult w_textlayout_set_spacing(w_textlayout *textlayout, int spacing) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	if (spacing < 0)
		return W_ERROR_INVALID_ARGUMENT;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	pango_layout_set_spacing(layout, spacing * PANGO_SCALE);
	return W_TRUE;
}
wresult w_textlayout_set_style_0(w_textlayout *textlayout, w_textstyle *style,
		int byteStart, int byteEnd) {
	if (style == 0)
		return W_FALSE;
	if ((style->flags & W_TEXTSTYLE_MASK_ALL) == 0)
		return W_FALSE;
	PangoAttribute *attr;
	if (_W_TEXTLAYOUT(textlayout)->attrList == 0) {
		_W_TEXTLAYOUT(textlayout)->attrList = pango_attr_list_new();
		_W_TEXTLAYOUT(textlayout)->selAttrList = pango_attr_list_new();
		pango_layout_set_attributes(_W_TEXTLAYOUT(textlayout)->layout,
		_W_TEXTLAYOUT(textlayout)->attrList);
	}
	PangoAttrList *attrList = _W_TEXTLAYOUT(textlayout)->attrList;
	PangoAttrList *selAttrList =
	_W_TEXTLAYOUT(textlayout)->selAttrList;
	w_font *font = style->font;
	if ((style->flags & W_TEXTSTYLE_MASK_FONT) != 0 && font != 0
			&& _W_FONT(font)->handle != 0
			&& font != _W_TEXTLAYOUT(textlayout)->font) {
		PangoAttribute *attr = pango_attr_font_desc_new(_W_FONT(font)->handle);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
		pango_attr_list_insert(selAttrList, attr);
	}
	if ((style->flags & W_TEXTSTYLE_MASK_UNDERLINE) != 0 && style->underline) {
		PangoUnderline underlineStyle = PANGO_UNDERLINE_NONE;
		switch (style->underlineStyle) {
		case W_UNDERLINE_SINGLE:
			underlineStyle = PANGO_UNDERLINE_SINGLE;
			break;
		case W_UNDERLINE_DOUBLE:
			underlineStyle = PANGO_UNDERLINE_DOUBLE;
			break;
		case W_UNDERLINE_SQUIGGLE:
		case W_UNDERLINE_ERROR:
			underlineStyle = PANGO_UNDERLINE_ERROR;
			break;
		case W_UNDERLINE_LINK: {
			if (style->foreground == 0) {
				// Bug 497071: use COLOR_LINK_FOREGROUND for StyledText links
				PangoAttribute *attr;
#if GTK3
				w_color linkRGBA = w_toolkit_get_system_color(
						w_toolkit_get_default(), W_COLOR_LINK_FOREGROUND);
				// Manual conversion since PangoAttrColor is a special case.
				// It uses GdkColor style colors but is supported on GTK3.
				attr = pango_attr_foreground_new(W_RED(linkRGBA),
						W_GREEN(linkRGBA), W_BLUE(linkRGBA));
#endif
#if GTK2
#endif
				attr->start_index = byteStart;
				attr->end_index = byteEnd;
				pango_attr_list_insert(attrList, attr);
			}
			underlineStyle = PANGO_UNDERLINE_SINGLE;
			break;
		}
		}
		PangoAttribute *attr = pango_attr_underline_new(underlineStyle);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
		pango_attr_list_insert(selAttrList, pango_attribute_copy(attr));
		if (style->underlineColor != 0) {
#if GTK3
			attr = pango_attr_underline_color_new(
			W_RED(style->underlineColor) * 0x100,
			W_GREEN(style->underlineColor) * 0x100,
			W_BLUE(style->underlineColor) * 0x100);
#endif
#if GTK2
#endif
			if (attr != 0) {
				attr->start_index = byteStart;
				attr->end_index = byteEnd;
				pango_attr_list_insert(attrList, attr);
				pango_attr_list_insert(selAttrList, pango_attribute_copy(attr));
			}
		}
	}
	if ((style->flags & W_TEXTSTYLE_MASK_STRIKEOUT) != 0 && style->strikeout) {
		attr = pango_attr_strikethrough_new(TRUE);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
		pango_attr_list_insert(selAttrList, pango_attribute_copy(attr));
		if (style->strikeoutColor != 0) {
#if GTK3
			attr = pango_attr_strikethrough_color_new(
			W_RED(style->strikeoutColor) * 0x100,
			W_GREEN(style->strikeoutColor) * 0x100,
			W_BLUE(style->strikeoutColor) * 0x100);
#endif
#if GTK2
#endif
			if (attr != 0) {
				attr->start_index = byteStart;
				attr->end_index = byteEnd;
				pango_attr_list_insert(attrList, attr);
				pango_attr_list_insert(selAttrList, pango_attribute_copy(attr));
			}
		}
	}
	w_color foreground = style->foreground;
	if ((style->flags & W_TEXTSTYLE_MASK_FOREGROUND) != 0 && foreground != 0) {
#if GTK3
		attr = pango_attr_foreground_new(W_RED(style->foreground) * 0x100,
		W_GREEN(style->foreground) * 0x100,
		W_BLUE(style->foreground) * 0x100);
#endif
#if GTK2
#endif
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
	}
	w_color background = style->background;
	if ((style->flags & W_TEXTSTYLE_MASK_BACKGROUND) != 0 && background != 0) {
#if GTK3
		attr = pango_attr_background_new(W_RED(style->background) * 0x100,
		W_GREEN(style->background) * 0x100,
		W_BLUE(style->background) * 0x100);
#endif
#if GTK2
#endif
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
	}
	w_glyphmetrics *metrics = &style->metrics;
	if ((style->flags & W_TEXTSTYLE_MASK_METRICS) != 0
			&& metrics->width != (wushort) -1 && metrics->ascent != (wushort) -1
			&& metrics->descent != (wushort) -1) {
		int width = metrics->width == -1 ? 0 : metrics->width;
		int ascent = metrics->ascent == -1 ? 0 : metrics->ascent;
		int descent = metrics->descent == -1 ? 0 : metrics->descent;
		PangoRectangle rect;
		rect.x = 0;
		rect.y = -ascent * PANGO_SCALE;
		rect.height = ascent + descent * PANGO_SCALE;
		rect.width = width * PANGO_SCALE;
		PangoAttribute *attr = pango_attr_shape_new(&rect, &rect);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
		pango_attr_list_insert(selAttrList, pango_attribute_copy(attr));
	}
	int rise = style->rise;
	if ((style->flags & W_TEXTSTYLE_MASK_RISE) != 0 && rise != 0) {
		PangoAttribute *attr = pango_attr_rise_new(rise * PANGO_SCALE);
		attr->start_index = byteStart;
		attr->end_index = byteEnd;
		pango_attr_list_insert(attrList, attr);
		pango_attr_list_insert(selAttrList, pango_attribute_copy(attr));
	}
	return W_TRUE;
}
wresult w_textlayout_set_style(w_textlayout *textlayout, w_textstyle *style,
		int start, int end, int enc) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	const char *ptr = pango_layout_get_text(_W_TEXTLAYOUT(textlayout)->layout);
	//start = _w_textlayout_translate_offset(textlayout,start);
	//end = _w_textlayout_translate_offset(textlayout,end);
	return w_textlayout_set_style_0(textlayout, style, start, end);
}
wresult w_textlayout_set_tabs(w_textlayout *textlayout, int *tabs, int count) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	if (tabs == 0) {
		pango_layout_set_tabs(layout, gtk_toolkit->empty_tab);
	} else {
		PangoTabArray *tabArray = pango_tab_array_new(count, TRUE);
		if (tabArray != 0) {
			for (int i = 0; i < count; i++) {
				pango_tab_array_set_tab(tabArray, i, PANGO_TAB_LEFT, tabs[i]);
			}
			pango_layout_set_tabs(layout, tabArray);
			pango_tab_array_free(tabArray);
		}
	}
	/*
	 * Bug in Pango. A change in the tab stop array is not automatically reflected in the
	 * pango layout object because the call pango_layout_set_tabs() does not free the
	 * lines cache. The fix to use pango_layout_context_changed() to free the lines cache.
	 */
	pango_layout_context_changed(layout);
	return W_TRUE;
}
wresult w_textlayout_set_text(w_textlayout *textlayout, const char *text,
		int count) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	if (count < 0)
		count = -1;
	pango_layout_set_text(layout, text, count);
	return W_TRUE;
}
wresult w_textlayout_set_text_direction(w_textlayout *textlayout,
		int textDirection) {
	return W_TRUE;
}
wresult w_textlayout_set_width(w_textlayout *textlayout, int width) {
	_W_TEXTLAYOUT(textlayout)->wrapWidth = width;
	if (_W_TEXTLAYOUT(textlayout)->wrapWidth == -1) {
		pango_layout_set_width(_W_TEXTLAYOUT(textlayout)->layout, -1);
		int rtl = pango_context_get_base_dir(
		_W_TEXTLAYOUT(textlayout)->context) == PANGO_DIRECTION_RTL;
		pango_layout_set_alignment(_W_TEXTLAYOUT(textlayout)->layout,
				rtl ? PANGO_ALIGN_RIGHT : PANGO_ALIGN_LEFT);
	} else {
		int margin = w_min(_W_TEXTLAYOUT(textlayout)->indent,
		_W_TEXTLAYOUT(textlayout)->wrapIndent);
		pango_layout_set_width(_W_TEXTLAYOUT(textlayout)->layout,
				(_W_TEXTLAYOUT(textlayout)->wrapWidth - margin) * PANGO_SCALE);
	}
	return W_TRUE;
}
wresult w_textlayout_set_wrap_indent(w_textlayout *textlayout, int wrapIndent) {
	if (textlayout == 0 || _W_TEXTLAYOUT(textlayout)->layout == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = _W_TEXTLAYOUT(textlayout)->layout;
	if (wrapIndent < 0)
		return W_ERROR_NO_INDEX;
	if (_W_TEXTLAYOUT(textlayout)->wrapIndent == wrapIndent)
		return W_TRUE;
	_W_TEXTLAYOUT(textlayout)->wrapIndent = wrapIndent;
	pango_layout_set_indent(layout,
			(_W_TEXTLAYOUT(textlayout)->indent - wrapIndent) * PANGO_SCALE);
	if (_W_TEXTLAYOUT(textlayout)->wrapWidth != -1) {
		int margin = MIN(_W_TEXTLAYOUT(textlayout)->indent, wrapIndent);
		pango_layout_set_width(layout,
				(_W_TEXTLAYOUT(textlayout)->wrapWidth - margin) * PANGO_SCALE);
	}
	return W_TRUE;
}
#endif
