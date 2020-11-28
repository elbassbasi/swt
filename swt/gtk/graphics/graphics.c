/*
 * graphics.c
 *
 *  Created on: 15 sept. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "graphics.h"
#include "../widgets/toolkit.h"
unsigned char LINE_DOT[] = { 1, 1, 0 };
unsigned char LINE_DASH[] = { 3, 1, 0 };
unsigned char LINE_DASHDOT[] = { 3, 1, 1, 1, 0 };
unsigned char LINE_DASHDOTDOT[] = { 3, 1, 1, 1, 1, 1, 0 };
unsigned char LINE_DOT_ZERO[] = { 3, 3, 0 };
unsigned char LINE_DASH_ZERO[] = { 18, 6, 0 };
unsigned char LINE_DASHDOT_ZERO[] = { 9, 6, 3, 6, 0 };
unsigned char LINE_DASHDOTDOT_ZERO[] = { 9, 3, 3, 3, 3, 3, 0 };
void w_graphics_init(w_graphics *gc) {
#if USE_CAIRO
	_W_GRAPHICS(gc)->cairo = 0;
#else
#endif
}
#if USE_CAIRO
void _w_graphics_init(w_graphics *gc, cairo_t *drawable) {
	memset(gc, 0, sizeof(struct _w_graphics));
	_W_GRAPHICS(gc)->cairo = drawable;
	_W_GRAPHICS(gc)->alpha = 0xFF;
}
#else
void _w_graphics_init(w_graphics* gc,GdkWindow* drawable);
#endif
void w_graphics_dispose(w_graphics *gc) {
}
wbool w_graphics_is_ok(w_graphics *gc) {
#if USE_CAIRO
	return _W_GRAPHICS(gc)->cairo != 0;
#else
#endif
}
wresult _w_graphics_check(w_graphics *gc, int mask) {
#if USE_CAIRO
	if (_W_GRAPHICS(gc)->cairo == 0) {
		return W_FALSE;
	}
	int state = _W_GRAPHICS(gc)->state;
	if ((state & mask) == mask)
		return W_TRUE;
	state = (state ^ mask) & mask;
	_W_GRAPHICS(gc)->state |= mask;
	if ((state & (GRAPHICS_STATE_BACKGROUND | GRAPHICS_STATE_FOREGROUND))
			!= 0) {
		w_color color;
		w_pattern *pattern;
		if ((state & GRAPHICS_STATE_FOREGROUND) != 0) {
			color = _W_GRAPHICS(gc)->foreground;
			pattern = _W_GRAPHICS(gc)->foregroundPattern;
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_BACKGROUND;
		} else {
			color = _W_GRAPHICS(gc)->background;
			pattern = _W_GRAPHICS(gc)->backgroundPattern;
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_FOREGROUND;
		}
		if (pattern != 0) {
			/*if ((_W_GRAPHICS(gc)->state & GRAPHICS_MIRRORED) != 0
			 && _W_PATTERN(pattern)->surface != 0) {
			 long newPattern = cairo_pattern_create_for_surface(
			 _W_PATTERN(pattern)->surface);
			 if (newPattern == 0)
			 return W_ERROR_NO_HANDLES;
			 cairo_pattern_set_extend(newPattern, CAIRO_EXTEND_REPEAT);
			 cairo_matrix_t matrix = {-1, 0, 0, 1, 0, 0};
			 cairo_pattern_set_matrix(newPattern, &matrix);
			 cairo_set_source(_W_GRAPHICS(gc)->cairo, newPattern);
			 cairo_pattern_destroy(newPattern);
			 } else {*/
			cairo_set_source(_W_GRAPHICS(gc)->cairo,
			_W_PATTERN(pattern)->handle);
			//}
		} else {
			cairo_set_source_rgba(_W_GRAPHICS(gc)->cairo, W_RED(color) / 255.0,
			W_GREEN(color) / 255.0, W_BLUE(color) / 255.0,
			_W_GRAPHICS(gc)->alpha / (float) 0xFF);
		}
	}
	if ((state & GRAPHICS_STATE_FONT) != 0) {
		if (_W_GRAPHICS(gc)->layout != 0) {
			if (_W_GRAPHICS(gc)->font != 0)
				pango_layout_set_font_description(_W_GRAPHICS(gc)->layout,
				_W_FONT(_W_GRAPHICS(gc)->font)->handle);
		}
	}
	if ((state & GRAPHICS_STATE_LINE_CAP) != 0) {
		cairo_line_cap_t cap_style = CAIRO_LINE_CAP_BUTT;
		switch (_W_GRAPHICS(gc)->line.cap) {
		case W_CAP_ROUND:
			cap_style = CAIRO_LINE_CAP_ROUND;
			break;
		case W_CAP_FLAT:
			cap_style = CAIRO_LINE_CAP_BUTT;
			break;
		case W_CAP_SQUARE:
			cap_style = CAIRO_LINE_CAP_SQUARE;
			break;
		}
		cairo_set_line_cap(_W_GRAPHICS(gc)->cairo, cap_style);
	}
	if ((state & GRAPHICS_STATE_LINE_JOIN) != 0) {
		cairo_line_join_t join_style = CAIRO_LINE_JOIN_MITER;
		switch (_W_GRAPHICS(gc)->line.join) {
		case W_JOIN_MITER:
			join_style = CAIRO_LINE_JOIN_MITER;
			break;
		case W_JOIN_ROUND:
			join_style = CAIRO_LINE_JOIN_ROUND;
			break;
		case W_JOIN_BEVEL:
			join_style = CAIRO_LINE_JOIN_BEVEL;
			break;
		}
		cairo_set_line_join(_W_GRAPHICS(gc)->cairo, join_style);
	}
	if ((state & GRAPHICS_STATE_LINE_WIDTH) != 0) {
		cairo_set_line_width(_W_GRAPHICS(gc)->cairo,
		_W_GRAPHICS(gc)->line.width == 0 ? 1 : _W_GRAPHICS(gc)->line.width);
		switch (_W_GRAPHICS(gc)->line.style) {
		case W_LINE_DOT:
		case W_LINE_DASH:
		case W_LINE_DASHDOT:
		case W_LINE_DASHDOTDOT:
			state |= GRAPHICS_STATE_LINE_STYLE;
		}
	}
	if ((state & GRAPHICS_STATE_LINE_STYLE) != 0) {
		float dashesOffset = 0;
		unsigned char *dashes = 0;
		float width = _W_GRAPHICS(gc)->line.width;
		int is_custom_dash = W_FALSE;
		switch (_W_GRAPHICS(gc)->line.style) {
		case W_LINE_SOLID:
			break;
		case W_LINE_DASH:
			dashes = width != 0 ? LINE_DASH : LINE_DASH_ZERO;
			break;
		case W_LINE_DOT:
			dashes = width != 0 ? LINE_DOT : LINE_DOT_ZERO;
			break;
		case W_LINE_DASHDOT:
			dashes = width != 0 ? LINE_DASHDOT : LINE_DASHDOT_ZERO;
			break;
		case W_LINE_DASHDOTDOT:
			dashes = width != 0 ? LINE_DASHDOTDOT : LINE_DASHDOTDOT_ZERO;
			break;
		case W_LINE_CUSTOM:
			is_custom_dash = W_TRUE;
			//dashes = _W_GRAPHICS(gc)->lineDashes;
			break;
		}
		if (dashes != 0) {
			dashesOffset = _W_GRAPHICS(gc)->line.dashOffset;
			double cairoDashes[6];
			int i = 0;
			while (dashes[i] != 0) {
				cairoDashes[i] =
						width == 0
								|| _W_GRAPHICS(gc)->line.style
										== W_LINE_CUSTOM ?
								dashes[i] : dashes[i] * width;
				i++;
			}
			cairo_set_dash(_W_GRAPHICS(gc)->cairo, cairoDashes, i,
					dashesOffset);
		} else {
			if (is_custom_dash && _W_GRAPHICS(gc)->line.dash != 0) {
				dashesOffset = _W_GRAPHICS(gc)->line.dashOffset;
				double *cairoDashes = malloc(
				_W_GRAPHICS(gc)->line.dash->length * sizeof(double));
				if (cairoDashes != 0) {
					for (int i = 0; i < _W_GRAPHICS(gc)->line.dash->length;
							i++) {
						cairoDashes[i] =
								width == 0
										|| _W_GRAPHICS(gc)->line.style
												== W_LINE_CUSTOM ?
										dashes[i] : dashes[i] * width;
					}
					cairo_set_dash(_W_GRAPHICS(gc)->cairo, cairoDashes,
					_W_GRAPHICS(gc)->line.dash->length, dashesOffset);
					free(cairoDashes);
				} else {

				}
			} else
				cairo_set_dash(_W_GRAPHICS(gc)->cairo, 0, 0, 0);
		}
	}
	if ((state & GRAPHICS_STATE_LINE_MITERLIMIT) != 0) {
		cairo_set_miter_limit(_W_GRAPHICS(gc)->cairo,
		_W_GRAPHICS(gc)->line.miterLimit);
	}
	if ((state & GRAPHICS_STATE_DRAW_OFFSET) != 0) {
		_W_GRAPHICS(gc)->cairoXoffset = _W_GRAPHICS(gc)->cairoYoffset = 0;
		cairo_matrix_t matrix;
		cairo_get_matrix(_W_GRAPHICS(gc)->cairo, &matrix);
		double dx = 1;
		double dy = 1;
		cairo_user_to_device_distance(_W_GRAPHICS(gc)->cairo, &dx, &dy);
		double scaling = dx;
		if (scaling < 0)
			scaling = -scaling;
		double strokeWidth = _W_GRAPHICS(gc)->line.width * scaling;
		if (strokeWidth == 0 || ((int) strokeWidth % 2) == 1) {
			_W_GRAPHICS(gc)->cairoXoffset = 0.5 / scaling;
		}
		scaling = dy;
		if (scaling < 0)
			scaling = -scaling;
		strokeWidth = _W_GRAPHICS(gc)->line.width * scaling;
		if (strokeWidth == 0 || ((int) strokeWidth % 2) == 1) {
			_W_GRAPHICS(gc)->cairoYoffset = 0.5 / scaling;
		}
	}
	return W_TRUE;
#else
	if ((state & (GRAPHICS_STATE_BACKGROUND | GRAPHICS_STATE_FOREGROUND)) != 0) {
		GdkColor foreground;
		if ((state & GRAPHICS_STATE_FOREGROUND) != 0) {
			graphics_set_color(&foreground, _W_GRAPHICS(gc)->foreground);
			//foreground = _W_GRAPHICS(gc)->foreground;
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_BACKGROUND;
		} else {
			graphics_set_color(&foreground, _W_GRAPHICS(gc)->background);
			//foreground = _W_GRAPHICS(gc)->background;
			_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_FOREGROUND;
		}
		gdk_gc_set_foreground(_W_GRAPHICS(gc)->gc, &foreground);
	}
	if ((state & GRAPHICS_STATE_BACKGROUND_BG) != 0) {
		GdkColor background;
		graphics_set_color(&background, _W_GRAPHICS(gc)->background);
		gdk_gc_set_background(_W_GRAPHICS(gc)->gc, &background);
	}
	if ((state & GRAPHICS_STATE_FONT) != 0) {
		if (_W_GRAPHICS(gc)->layout != 0) {
			PangoFontDescription* handle =
			_W_GRAPHICS(gc)->font->GetHandles<Font_t>()->handle;
			pango_layout_set_font_description(_W_GRAPHICS(gc)->layout, handle);
		}
	}
	if ((state & (GRAPHICS_STATE_LINE_CAP | GRAPHICS_STATE_LINE_JOIN | GRAPHICS_STATE_LINE_STYLE | GRAPHICS_STATE_LINE_WIDTH)) != 0) {
		GdkCapStyle cap_style = GDK_CAP_NOT_LAST;
		GdkJoinStyle join_style = GDK_JOIN_MITER;
		int width = (int) _W_GRAPHICS(gc)->line.width;
		GdkLineStyle line_style = GDK_LINE_SOLID;
		unsigned char* dashes = 0;
		bool is_custom_dash = false;
		switch (_W_GRAPHICS(gc)->line.cap) {
			case W_CAP_ROUND:
			cap_style = GDK_CAP_ROUND;
			break;
			case W_CAP_FLAT:
			cap_style = GDK_CAP_BUTT;
			break;
			case W_CAP_SQUARE:
			cap_style = GDK_CAP_PROJECTING;
			break;
		}
		switch (_W_GRAPHICS(gc)->line.join) {
			case W_JOIN_ROUND:
			join_style = GDK_JOIN_ROUND;
			break;
			case W_JOIN_MITER:
			join_style = GDK_JOIN_MITER;
			break;
			case W_JOIN_BEVEL:
			join_style = GDK_JOIN_BEVEL;
			break;
		}
		/*			switch (_W_GRAPHICS(gc)->line.style) {
		 case graphics_LINE_SOLID:
		 break;
		 case graphics_LINE_DASH:
		 dashes = width != 0 ? LINE_DASH : LINE_DASH_ZERO;
		 break;
		 case graphics_LINE_DOT:
		 dashes = width != 0 ? LINE_DOT : LINE_DOT_ZERO;
		 break;
		 case graphics_LINE_DASHDOT:
		 dashes = width != 0 ? LINE_DASHDOT : LINE_DASHDOT_ZERO;
		 break;
		 case graphics_LINE_DASHDOTDOT:
		 dashes = width != 0 ? LINE_DASHDOTDOT : LINE_DASHDOTDOT_ZERO;
		 break;
		 case graphics_LINE_CUSTOM:
		 is_custom_dash = true;
		 break;
		 }*/
		if (dashes != 0) {
			if ((state & GRAPHICS_STATE_LINE_STYLE) != 0) {
				signed char dash_list[10];
				int i = 0;
				while (dashes[i] != 0) {
					dash_list[i] = (
							width == 0
							|| _W_GRAPHICS(gc)->line.style
							== graphics_LINE_CUSTOM ?
							dashes[i] : dashes[i] * width);
					i++;
				}
				gdk_gc_set_dashes(_W_GRAPHICS(gc)->gc, 0, dash_list, i);
			}
			line_style = GDK_LINE_ON_OFF_DASH;
		} else {
			if (is_custom_dash) {
				if ((state & GRAPHICS_STATE_LINE_STYLE) != 0) {
					signed char *dash_list =
					new signed char[_W_GRAPHICS(gc)->line.dash->length];
					for (int i = 0; i < _W_GRAPHICS(gc)->line.dash->length; i++) {
						dash_list[i] = (
								width == 0
								|| _W_GRAPHICS(gc)->line.style
								== graphics_LINE_CUSTOM ?
								_W_GRAPHICS(gc)->line.dash->dash[i] :
								_W_GRAPHICS(gc)->line.dash->dash[i] * width);
					}
					gdk_gc_set_dashes(_W_GRAPHICS(gc)->gc, 0, dash_list,
							_W_GRAPHICS(gc)->line.dash->length);
					delete[] dash_list;
				}
				line_style = GDK_LINE_ON_OFF_DASH;
			}
			line_style = GDK_LINE_SOLID;
		}
		gdk_gc_set_line_attributes(_W_GRAPHICS(gc)->gc, width, line_style, cap_style,
				join_style);
	}
#endif
}

wresult w_graphics_copy_area_to_image(w_graphics *gc, w_surface *bitmap, int x,
		int y) {
}
wresult w_graphics_copy_area(w_graphics *gc, int srcX, int srcY, int width,
		int height, int destX, int destY, wbool paint) {

}
wresult w_graphics_draw_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (width < 0) {
		x = x + width;
		width = -width;
	}
	if (height < 0) {
		y = y + height;
		height = -height;
	}
	if (width == 0 || height == 0 || arcAngle == 0)
		return W_TRUE;
#if USE_CAIRO
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	if (width == height) {
		if (arcAngle >= 0) {
			cairo_arc_negative(_W_GRAPHICS(gc)->cairo, x + xOffset + width / 2.,
					y + yOffset + height / 2.0, width / 2.0,
					-startAngle * (float) M_PI / 180,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		} else {
			cairo_arc(_W_GRAPHICS(gc)->cairo, x + xOffset + width / 2.0,
					y + yOffset + height / 2.0, width / 2.0,
					-startAngle * (float) M_PI / 180,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		}
	} else {
		cairo_save(_W_GRAPHICS(gc)->cairo);
		cairo_translate(_W_GRAPHICS(gc)->cairo, x + xOffset + width / 2.0,
				y + yOffset + height / 2.0);
		cairo_scale(_W_GRAPHICS(gc)->cairo, width / 2.0, height / 2.0);
		if (arcAngle >= 0) {
			cairo_arc_negative(_W_GRAPHICS(gc)->cairo, 0, 0, 1,
					-startAngle * (float) M_PI / 180,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		} else {
			cairo_arc(_W_GRAPHICS(gc)->cairo, 0, 0, 1,
					-startAngle * (float) M_PI / 180,
					-(startAngle + arcAngle) * (float) M_PI / 180);
		}
		cairo_restore(_W_GRAPHICS(gc)->cairo);
	}
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_arc(t->drawable, t->gc, 0, x, y, width, height, startAngle * 64,
			arcAngle * 64);
#endif
}
wresult w_graphics_draw_focus(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
#if USE_CAIRO
#if GTK3
	GtkStyleContext *context = gtk_widget_get_style_context(
			gtk_toolkit->shellHandle);
	gtk_render_focus(context, _W_GRAPHICS(gc)->cairo, rect->x, rect->y,
			rect->width, rect->height);
#endif
#if GTK2
#endif
	return W_TRUE;
#endif
#if GTK2
#endif
}
GdkPixbuf* _w_graphics_get_pixbuf(w_graphics *gc, w_image *image, int state) {
	GdkPixbuf *pixbuf = _W_IMAGE(image)->pixbuf;
	if (state & W_DISABLED) {
		w_image dst;
		_W_IMAGE(&dst)->pixbuf = 0;
		w_image_to_disabled(image, w_graphics_get_background(gc),
				w_graphics_get_foreground(gc), &dst);
		if (_W_IMAGE(&dst)->pixbuf != 0)
			return _W_IMAGE(&dst)->pixbuf;
	}
	return pixbuf;
}
wresult w_graphics_draw_image(w_graphics *gc, w_image *image, w_rect *src,
		w_rect *dest, int state) {
#if USE_CAIRO
	if (_W_GRAPHICS(gc)->cairo == 0) {
		return W_ERROR_NO_HANDLES;
	}
	if (_W_IMAGE(image)->pixbuf == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GdkPixbuf *pixbuf = _w_graphics_get_pixbuf(gc, image, state);
	cairo_t *cr = _W_GRAPHICS(gc)->cairo;
	if (src->x == 0 && src->y == 0 && src->width < 0 && src->height < 0
			&& dest->width < 0 && dest->height < 0) {
		gdk_cairo_set_source_pixbuf(cr, pixbuf, dest->x, dest->y);
		cairo_paint(cr);
		if (pixbuf != _W_IMAGE(image)->pixbuf) {
			g_object_unref(pixbuf);
		}
		return W_TRUE;
	}
	int srcX = src->x, srcY = src->y, srcWidth = src->width, srcHeight =
			src->height;
	int destX = dest->x, destY = dest->y, destWidth = dest->width, destHeight =
			dest->height;
	int imgWidth, imgHeight;
	imgWidth = gdk_pixbuf_get_width(pixbuf);
	imgHeight = gdk_pixbuf_get_height(pixbuf);
	if (srcWidth < 0)
		srcWidth = imgWidth;
	if (srcHeight < 0)
		srcHeight = imgHeight;
	if (destWidth < 0)
		destWidth = imgWidth;
	if (destHeight < 0)
		destHeight = imgHeight;
	if (_W_GRAPHICS(gc)->alpha != 0) {
		//srcImage.createSurface();
		cairo_save(cr);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_scale(cr, -1., 1);
			cairo_translate(cr, -2 * destX - destWidth, 0);
		}
		cairo_rectangle(cr, destX, destY, destWidth, destHeight);
		cairo_clip(cr);
		if (srcWidth != destWidth || srcHeight != destHeight) {
			float scaleX = destWidth / (float) srcWidth;
			float scaleY = destHeight / (float) srcHeight;
			cairo_translate(cr, destX - (int) (srcX * scaleX),
					destY - (int) (srcY * scaleY));
			cairo_scale(cr, scaleX, scaleY);
		} else {
			cairo_translate(cr, destX - srcX, destY - srcY);
		}
		cairo_filter_t filter = CAIRO_FILTER_GOOD;
		switch (_W_GRAPHICS(gc)->interpolation) {
		case 0xFF: //W_DEFAULT
			filter = CAIRO_FILTER_GOOD;
			break;
		case W_NONE:
			filter = CAIRO_FILTER_NEAREST;
			break;
		case W_LOW:
			filter = CAIRO_FILTER_FAST;
			break;
		case W_HIGH:
			filter = CAIRO_FILTER_BEST;
			break;
		}
		cairo_surface_t *surface = gdk_cairo_surface_create_from_pixbuf(pixbuf,
				0, NULL);
		if (surface == 0) {
			if (pixbuf != _W_IMAGE(image)->pixbuf) {
				g_object_unref(pixbuf);
			}
			return W_ERROR_NO_HANDLES;
		}
		cairo_pattern_t *pattern = cairo_pattern_create_for_surface(surface);
		if (pattern == 0) {
			cairo_surface_destroy(surface);
			if (pixbuf != _W_IMAGE(image)->pixbuf) {
				g_object_unref(pixbuf);
			}
			return W_ERROR_NO_HANDLES;
		}
		if (srcWidth != destWidth || srcHeight != destHeight) {
			/*
			 * Bug in Cairo.  When drawing the image stretched with an interpolation
			 * algorithm, the edges of the image are faded.  This is not a bug, but
			 * it is not desired.  To avoid the faded edges, it should be possible to
			 * use cairo_pattern_set_extend() to set the pattern extend to either
			 * CAIRO_EXTEND_REFLECT or CAIRO_EXTEND_PAD, but these are not implemented
			 * in some versions of cairo (1.2.x) and have bugs in others (in 1.4.2 it
			 * draws with black edges).  The fix is to implement CAIRO_EXTEND_REFLECT
			 * by creating an image that is 3 times bigger than the original, drawing
			 * the original image in every quadrant (with an appropriate transform) and
			 * use this image as the pattern.
			 *
			 * NOTE: For some reason, it is necessary to use CAIRO_EXTEND_PAD with
			 * the image that was created or the edges are still faded.
			 *
			 * NOTE: CAIRO_EXTEND_PAD works on Cairo 1.8.x and greater.
			 */
			int version = cairo_version();
			if (version >= CAIRO_VERSION_ENCODE(1, 4, 0)
					&& version < CAIRO_VERSION_ENCODE(1, 8, 0)) {
				cairo_surface_t *surface = cairo_image_surface_create(
						CAIRO_FORMAT_ARGB32, imgWidth * 3, imgHeight * 3);
				cairo_t *cr = cairo_create(surface);
				cairo_set_source_surface(cr, surface, imgWidth, imgHeight);
				cairo_paint(cr);
				cairo_scale(cr, -1, -1);
				cairo_set_source_surface(cr, surface, -imgWidth, -imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth * 3,
						-imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth,
						-imgHeight * 3);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth * 3,
						-imgHeight * 3);
				cairo_paint(cr);
				cairo_scale(cr, 1, -1);
				cairo_set_source_surface(cr, surface, -imgWidth, imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth * 3, imgHeight);
				cairo_paint(cr);
				cairo_scale(cr, -1, -1);
				cairo_set_source_surface(cr, surface, imgWidth, -imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, imgWidth, -imgHeight * 3);
				cairo_paint(cr);
				cairo_destroy(cr);
				cairo_pattern_t *newPattern = cairo_pattern_create_for_surface(
						surface);
				cairo_surface_destroy(surface);
				if (newPattern == 0) {
					if (pixbuf != _W_IMAGE(image)->pixbuf) {
						g_object_unref(pixbuf);
					}
					return W_ERROR_NO_HANDLES;
				}
				cairo_pattern_destroy(pattern);
				pattern = newPattern;
				cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
				cairo_matrix_t matrix;
				cairo_matrix_init_translate(&matrix, imgWidth, imgHeight);
				cairo_pattern_set_matrix(pattern, &matrix);
			} else if (version >= CAIRO_VERSION_ENCODE(1, 8, 0)) {
				cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
			}
		} else {
			gdk_cairo_set_source_pixbuf(cr, pixbuf, srcX, srcY);
		}
		cairo_pattern_set_filter(pattern, filter);
		cairo_set_source(cr, pattern);
		if (_W_GRAPHICS(gc)->alpha != 0xFF) {
			cairo_paint_with_alpha(cr,
			_W_GRAPHICS(gc)->alpha / (float) 0xFF);
		} else {
			cairo_paint(cr);
		}
		cairo_restore(cr);
		cairo_pattern_destroy(pattern);
	}
	if (pixbuf != _W_IMAGE(image)->pixbuf) {
		g_object_unref(pixbuf);
	}
	return W_TRUE;

#else
#endif
}
wresult w_graphics_draw_surface(w_graphics *gc, w_surface *surface, w_rect *src,
		w_rect *dest) {
#if USE_CAIRO
	if (_W_GRAPHICS(gc)->cairo == 0) {
		return W_ERROR_NO_HANDLES;
	}
	if (_W_SURFACE(surface)->surface == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (src->x == 0 && src->y == 0 && src->width < 0 && src->height < 0
			&& dest->width < 0 && dest->height < 0) {
		cairo_set_source_surface(_W_GRAPHICS(gc)->cairo,
		_W_SURFACE(surface)->surface, dest->x, dest->y);
		cairo_paint(_W_GRAPHICS(gc)->cairo);
		return W_TRUE;
	}
	int srcX = src->x, srcY = src->y, srcWidth = src->width, srcHeight =
			src->height;
	int destX = dest->x, destY = dest->y, destWidth = dest->width, destHeight =
			dest->height;
	int imgWidth, imgHeight;
	imgWidth = cairo_image_surface_get_width(_W_SURFACE(surface)->surface);
	imgHeight = cairo_image_surface_get_height(_W_SURFACE(surface)->surface);
	if (srcWidth < 0)
		srcWidth = imgWidth;
	if (srcHeight < 0)
		srcHeight = imgHeight;
	if (destWidth < 0)
		destWidth = imgWidth;
	if (destHeight < 0)
		destHeight = imgHeight;
	if (_W_GRAPHICS(gc)->alpha != 0) {
		//srcImage.createSurface();
		cairo_save(_W_GRAPHICS(gc)->cairo);
		if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
			cairo_scale(_W_GRAPHICS(gc)->cairo, -1., 1);
			cairo_translate(_W_GRAPHICS(gc)->cairo, -2 * destX - destWidth, 0);
		}
		cairo_rectangle(_W_GRAPHICS(gc)->cairo, destX, destY, destWidth,
				destHeight);
		cairo_clip(_W_GRAPHICS(gc)->cairo);
		if (srcWidth != destWidth || srcHeight != destHeight) {
			float scaleX = destWidth / (float) srcWidth;
			float scaleY = destHeight / (float) srcHeight;
			cairo_translate(_W_GRAPHICS(gc)->cairo,
					destX - (int) (srcX * scaleX),
					destY - (int) (srcY * scaleY));
			cairo_scale(_W_GRAPHICS(gc)->cairo, scaleX, scaleY);
		} else {
			cairo_translate(_W_GRAPHICS(gc)->cairo, destX - srcX, destY - srcY);
		}
		cairo_filter_t filter = CAIRO_FILTER_GOOD;
		switch (_W_GRAPHICS(gc)->interpolation) {
		case 0xFF: //W_DEFAULT
			filter = CAIRO_FILTER_GOOD;
			break;
		case W_NONE:
			filter = CAIRO_FILTER_NEAREST;
			break;
		case W_LOW:
			filter = CAIRO_FILTER_FAST;
			break;
		case W_HIGH:
			filter = CAIRO_FILTER_BEST;
			break;
		}
		cairo_surface_t *surface = _W_SURFACE(surface)->surface;
		cairo_pattern_t *pattern = cairo_pattern_create_for_surface(surface);
		if (pattern == 0) {
			return W_ERROR_NO_HANDLES;
		}
		if (srcWidth != destWidth || srcHeight != destHeight) {
			/*
			 * Bug in Cairo.  When drawing the image stretched with an interpolation
			 * algorithm, the edges of the image are faded.  This is not a bug, but
			 * it is not desired.  To avoid the faded edges, it should be possible to
			 * use cairo_pattern_set_extend() to set the pattern extend to either
			 * CAIRO_EXTEND_REFLECT or CAIRO_EXTEND_PAD, but these are not implemented
			 * in some versions of cairo (1.2.x) and have bugs in others (in 1.4.2 it
			 * draws with black edges).  The fix is to implement CAIRO_EXTEND_REFLECT
			 * by creating an image that is 3 times bigger than the original, drawing
			 * the original image in every quadrant (with an appropriate transform) and
			 * use this image as the pattern.
			 *
			 * NOTE: For some reason, it is necessary to use CAIRO_EXTEND_PAD with
			 * the image that was created or the edges are still faded.
			 *
			 * NOTE: Cairo.CAIRO_EXTEND_PAD works on Cairo 1.8.x and greater.
			 */
			int version = cairo_version();
			if (version >= CAIRO_VERSION_ENCODE(1, 4, 0)
					&& version < CAIRO_VERSION_ENCODE(1, 8, 0)) {
				cairo_surface_t *surface = cairo_image_surface_create(
						CAIRO_FORMAT_ARGB32, imgWidth * 3, imgHeight * 3);
				cairo_t *cr = cairo_create(surface);
				cairo_set_source_surface(cr, surface, imgWidth, imgHeight);
				cairo_paint(cr);
				cairo_scale(cr, -1, -1);
				cairo_set_source_surface(cr, surface, -imgWidth, -imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth * 3,
						-imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth,
						-imgHeight * 3);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth * 3,
						-imgHeight * 3);
				cairo_paint(cr);
				cairo_scale(cr, 1, -1);
				cairo_set_source_surface(cr, surface, -imgWidth, imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, -imgWidth * 3, imgHeight);
				cairo_paint(cr);
				cairo_scale(cr, -1, -1);
				cairo_set_source_surface(cr, surface, imgWidth, -imgHeight);
				cairo_paint(cr);
				cairo_set_source_surface(cr, surface, imgWidth, -imgHeight * 3);
				cairo_paint(cr);
				cairo_destroy(cr);
				cairo_pattern_t *newPattern = cairo_pattern_create_for_surface(
						surface);
				cairo_surface_destroy(surface);
				if (newPattern == 0) {
					return W_ERROR_NO_HANDLES;
				}
				cairo_pattern_destroy(pattern);
				pattern = newPattern;
				cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
				cairo_matrix_t matrix;
				cairo_matrix_init_translate(&matrix, imgWidth, imgHeight);
				cairo_pattern_set_matrix(pattern, &matrix);
			} else if (version >= CAIRO_VERSION_ENCODE(1, 8, 0)) {
				cairo_pattern_set_extend(pattern, CAIRO_EXTEND_PAD);
			}
		} else {
			cairo_set_source_surface(_W_GRAPHICS(gc)->cairo,
			_W_SURFACE(surface)->surface, srcX, srcY);

		}
		cairo_pattern_set_filter(pattern, filter);
		cairo_set_source(_W_GRAPHICS(gc)->cairo, pattern);
		if (_W_GRAPHICS(gc)->alpha != 0xFF) {
			cairo_paint_with_alpha(_W_GRAPHICS(gc)->cairo,
			_W_GRAPHICS(gc)->alpha / (float) 0xFF);
		} else {
			cairo_paint(_W_GRAPHICS(gc)->cairo);
		}
		cairo_restore(_W_GRAPHICS(gc)->cairo);
		cairo_pattern_destroy(pattern);
	}
	return W_TRUE;

#else
#endif
}
wresult w_graphics_draw_line(w_graphics *gc, w_point *pt1, w_point *pt2) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
#if USE_CAIRO
	//double xOffset = data.cairoXoffset, yOffset = data.cairoYoffset;
	cairo_move_to(_W_GRAPHICS(gc)->cairo, pt1->x /*+ xOffset*/,
			pt1->y /*+ yOffset*/);
	cairo_line_to(_W_GRAPHICS(gc)->cairo, pt2->x /*+ xOffset*/,
			pt2->y /*+ yOffset*/);
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_line(_W_GRAPHICS(gc)->drawable, _W_GRAPHICS(gc)->gc, x1, y1, x2, y2);
#endif
}
wresult w_graphics_draw_oval(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
#if USE_CAIRO
	if (width < 0) {
		x = x + width;
		width = -width;
	}
	if (height < 0) {
		y = y + height;
		height = -height;
	}
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	if (width == height) {
		cairo_arc_negative(_W_GRAPHICS(gc)->cairo, x + xOffset + width / 2.0,
				y + yOffset + height / 2.0, width / 2.0, 0, -2 * M_PI);
	} else {
		cairo_save(_W_GRAPHICS(gc)->cairo);
		cairo_translate(_W_GRAPHICS(gc)->cairo, x + xOffset + width / 2.0,
				y + yOffset + height / 2.0);
		cairo_scale(_W_GRAPHICS(gc)->cairo, width / 2.0, height / 2.0);
		cairo_arc_negative(_W_GRAPHICS(gc)->cairo, 0, 0, 1, 0, -2 * M_PI);
		cairo_restore(_W_GRAPHICS(gc)->cairo);
	}
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_arc(data.drawable, handle, 0, x, y, width, height, 0, 23040);
#endif
}
wresult w_graphics_draw_path(w_graphics *gc, w_path *path) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
	cairo_save(cairo);
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	cairo_translate(cairo, xOffset, yOffset);
	cairo_path_t *copy = cairo_copy_path(_W_PATH(path)->handle);
	if (copy == 0)
		return W_ERROR_NO_HANDLES;
	cairo_append_path(cairo, copy);
	cairo_path_destroy(copy);
	cairo_stroke(cairo);
	cairo_restore(cairo);
	return W_TRUE;
}
wresult w_graphics_draw_point(w_graphics *gc, w_point *pt) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
#if USE_CAIRO
	cairo_rectangle(_W_GRAPHICS(gc)->cairo, pt->x, pt->y, 1, 1);
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_point(data.drawable, handle, x, y);
#endif
}
wresult w_graphics_draw_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
#if USE_CAIRO
	if (count == 0)
		return W_TRUE;
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	cairo_move_to(_W_GRAPHICS(gc)->cairo, pointArray[0].x + xOffset,
			pointArray[0].y + yOffset);
	for (int i = 1; i < count; i++) {
		cairo_line_to(_W_GRAPHICS(gc)->cairo, pointArray[i].x + xOffset,
				pointArray[i].y + yOffset);
	}
	cairo_close_path(_W_GRAPHICS(gc)->cairo);
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_polygon(data.drawable, handle, 0, pointArray, pointArray.length / 2);
#endif
}
wresult w_graphics_draw_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
#if USE_CAIRO
	if (count == 0)
		return W_TRUE;
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	int x = va_arg(args, int);
	int y = va_arg(args, int);
	cairo_move_to(_W_GRAPHICS(gc)->cairo, x + xOffset, y + yOffset);
	for (int i = 1; i < count; i++) {
		x = va_arg(args, int);
		y = va_arg(args, int);
		cairo_line_to(_W_GRAPHICS(gc)->cairo, x + xOffset, y + yOffset);
	}
	cairo_close_path(_W_GRAPHICS(gc)->cairo);
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_polygon(data.drawable, handle, 0, pointArray, pointArray.length / 2);
#endif
}
wresult w_graphics_draw_polyline(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
#if USE_CAIRO
	if (count == 0)
		return W_TRUE;
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	cairo_move_to(_W_GRAPHICS(gc)->cairo, pointArray[0].x + xOffset,
			pointArray[0].y + yOffset);
	for (int i = 1; i < count; i++) {
		cairo_line_to(_W_GRAPHICS(gc)->cairo, pointArray[i].x + xOffset,
				pointArray[i].y + yOffset);
	}
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_polygon(data.drawable, handle, 0, pointArray, pointArray.length / 2);
#endif
}
wresult w_graphics_draw_polylinev(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
#if USE_CAIRO
	if (count == 0)
		return W_TRUE;
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	int x = va_arg(args, int);
	int y = va_arg(args, int);
	cairo_move_to(_W_GRAPHICS(gc)->cairo, x + xOffset, y + yOffset);
	for (int i = 1; i < count; i++) {
		x = va_arg(args, int);
		y = va_arg(args, int);
		cairo_line_to(_W_GRAPHICS(gc)->cairo, x + xOffset, y + yOffset);
	}
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_polygon(data.drawable, handle, 0, pointArray, pointArray.length / 2);
#endif

}
wresult w_graphics_draw_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (width < 0) {
		x = x + width;
		width = -width;
	}
	if (height < 0) {
		y = y + height;
		height = -height;
	}
#if USE_CAIRO
	cairo_rectangle(_W_GRAPHICS(gc)->cairo, x + _W_GRAPHICS(gc)->cairoXoffset,
			y + _W_GRAPHICS(gc)->cairoYoffset, width, height);
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	OS.gdk_draw_rectangle(data.drawable, handle, 0, x, y, width, height);
#endif
}
wresult w_graphics_draw_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_DRAW);
	if (result < 0)
		return result;
	int nx = rect->x;
	int ny = rect->y;
	int nw = rect->width;
	int nh = rect->height;
	int naw = arcWidth;
	int nah = arcHeight;
	if (nw < 0) {
		nw = 0 - nw;
		nx = nx - nw;
	}
	if (nh < 0) {
		nh = 0 - nh;
		ny = ny - nh;
	}
	if (naw < 0)
		naw = 0 - naw;
	if (nah < 0)
		nah = 0 - nah;
#if USE_CAIRO
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	if (naw == 0 || nah == 0) {
		cairo_rectangle(_W_GRAPHICS(gc)->cairo, rect->x + xOffset,
				rect->y + yOffset, rect->width, rect->height);
	} else {
		float naw2 = naw / 2.0;
		float nah2 = nah / 2 / 2;
		float fw = nw / naw2;
		float fh = nh / nah2;
		cairo_save(_W_GRAPHICS(gc)->cairo);
		cairo_translate(_W_GRAPHICS(gc)->cairo, nx + xOffset, ny + yOffset);
		cairo_scale(_W_GRAPHICS(gc)->cairo, naw2, nah2);
		cairo_move_to(_W_GRAPHICS(gc)->cairo, fw - 1, 0);
		cairo_arc(_W_GRAPHICS(gc)->cairo, fw - 1, 1, 1, M_PI + M_PI / 2.0,
		M_PI * 2.0);
		cairo_arc(_W_GRAPHICS(gc)->cairo, fw - 1, fh - 1, 1, 0, M_PI / 2.0);
		cairo_arc(_W_GRAPHICS(gc)->cairo, 1, fh - 1, 1, M_PI / 2, M_PI);
		cairo_arc(_W_GRAPHICS(gc)->cairo, 1, 1, 1, M_PI, 270.0 * M_PI / 180.0);
		cairo_close_path(_W_GRAPHICS(gc)->cairo);
		cairo_restore(_W_GRAPHICS(gc)->cairo);
	}
	cairo_stroke(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	int naw2 = naw / 2;
	int nah2 = nah / 2;
	long /*int*/drawable = data.drawable;
	if (nw > naw) {
		if (nh > nah) {
			OS.gdk_draw_arc(drawable, handle, 0, nx, ny, naw, nah, 5760, 5760);
			OS.gdk_draw_line(drawable, handle, nx + naw2, ny, nx + nw - naw2, ny);
			OS.gdk_draw_arc(drawable, handle, 0, nx + nw - naw, ny, naw, nah, 0, 5760);
			OS.gdk_draw_line(drawable, handle, nx + nw, ny + nah2, nx + nw, ny + nh - nah2);
			OS.gdk_draw_arc(drawable, handle, 0, nx + nw - naw, ny + nh - nah, naw, nah, 17280, 5760);
			OS.gdk_draw_line(drawable,handle, nx + naw2, ny + nh, nx + nw - naw2, ny + nh);
			OS.gdk_draw_arc(drawable, handle, 0, nx, ny + nh - nah, naw, nah, 11520, 5760);
			OS.gdk_draw_line(drawable, handle, nx, ny + nah2, nx, ny + nh - nah2);
		} else {
			OS.gdk_draw_arc(drawable, handle, 0, nx, ny, naw, nh, 5760, 11520);
			OS.gdk_draw_line(drawable, handle, nx + naw2, ny, nx + nw - naw2, ny);
			OS.gdk_draw_arc(drawable, handle, 0, nx + nw - naw, ny, naw, nh, 17280, 11520);
			OS.gdk_draw_line(drawable,handle, nx + naw2, ny + nh, nx + nw - naw2, ny + nh);
		}
	} else {
		if (nh > nah) {
			OS.gdk_draw_arc(drawable, handle, 0, nx, ny, nw, nah, 0, 11520);
			OS.gdk_draw_line(drawable, handle, nx + nw, ny + nah2, nx + nw, ny + nh - nah2);
			OS.gdk_draw_arc(drawable, handle, 0, nx, ny + nh - nah, nw, nah, 11520, 11520);
			OS.gdk_draw_line(drawable,handle, nx, ny + nah2, nx, ny + nh - nah2);
		} else {
			OS.gdk_draw_arc(drawable, handle, 0, nx, ny, nw, nh, 0, 23040);
		}
	}
#endif
}
int _w_graphics_fix_mnemonic(struct _mnemonics *mnemonics, const char *text,
		unsigned int text_length, int flags) {
	char ch;
	int j = 0;
	int mnemonic = -1;
	unsigned int i, length;
	char *buffer;
	for (i = 0; ((ch = text[i]) != 0 && i < text_length); i++) {
		if (ch == '&') {
			mnemonic = 1;
		}
	}
	if (mnemonic == -1 || !(flags & W_DRAW_MNEMONIC)) {
		mnemonics->str = (char*) text;
		mnemonics->length = i;
		return mnemonic;
	}
	length = i;
	if (i >= sizeof(gtk_toolkit->mem_tmp)) {
		mnemonics->str = malloc(length);
	} else {
		mnemonics->str = gtk_toolkit->mem_tmp;
	}
	mnemonics->length = length;
	if (mnemonics->str == 0)
		return mnemonic;
	buffer = mnemonics->str;
	i = 0;
	while (i < length) {
		if ((buffer[j++] = text[i++]) == '&') {
			if (i == length) {
				continue;
			}
			if (buffer[i] == '&') {
				i++;
				continue;
			}
			if (mnemonic == -1)
				mnemonic = j;
			j--;
		}
	}
	buffer[j++] = 0;
	return mnemonic;
}
wresult _w_graphics_create_layout(w_graphics *gc) {
	PangoContext *context = gdk_pango_context_get();
	if (context == 0)
		return W_ERROR_NO_HANDLES;
	PangoLayout *layout = pango_layout_new(context);
	if (layout == 0)
		return W_ERROR_NO_HANDLES;
	_W_GRAPHICS(gc)->layout = layout;
	_W_GRAPHICS(gc)->context = context;
	pango_context_set_language(context, gtk_get_default_language());
	pango_context_set_base_dir(context,
			(_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0 ?
					PANGO_DIRECTION_RTL : PANGO_DIRECTION_LTR);
	pango_layout_set_auto_dir(layout, FALSE);
	return W_TRUE;
}
wresult _w_graphics_set_text(w_graphics *gc, const char *string, size_t length,
		int flags) {
	if (_W_GRAPHICS(gc)->layout == 0) {
		wresult result = _w_graphics_create_layout(gc);
		if (result < 0)
			return result;
	}
	struct _mnemonics mn;
	int mnemonic = _w_graphics_fix_mnemonic(&mn, string, length, flags);
	if ((flags & W_DRAW_MNEMONIC) && mnemonic != -1) {
		PangoAttrList *attr_list = pango_attr_list_new();
		PangoAttribute *attribute = pango_attr_underline_new(
				PANGO_UNDERLINE_LOW);
		attribute->start_index = mnemonic - 1;
		attribute->end_index = mnemonic;
		pango_attr_list_insert(attr_list, attribute);
		pango_layout_set_attributes(_W_GRAPHICS(gc)->layout, attr_list);
		pango_attr_list_unref(attr_list);
	} else {
		pango_layout_set_attributes(_W_GRAPHICS(gc)->layout, 0);
	}
	pango_layout_set_text(_W_GRAPHICS(gc)->layout, mn.str, mn.length);
	pango_layout_set_single_paragraph_mode(_W_GRAPHICS(gc)->layout,
			(flags & W_DRAW_DELIMITER) == 0);
	pango_layout_set_tabs(_W_GRAPHICS(gc)->layout,
			(flags & W_DRAW_TAB) != 0 ? 0 : gtk_toolkit->empty_tab);
	_mnemonics_free(&mn, string);
	return W_TRUE;
}
wresult w_graphics_draw_text(w_graphics *gc, const char *string, size_t length,
		w_rect *rect, int flags) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (string == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (*string == 0)
		return W_OK;
	result = _w_graphics_set_text(gc, string, length, flags);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, stringWidth, stringHeight;
	if (rect->width >= 0) {
		pango_layout_set_width(_W_GRAPHICS(gc)->layout, rect->width);
	}
	if (rect->height >= 0) {
		pango_layout_set_height(_W_GRAPHICS(gc)->layout, rect->height);
	}
	pango_layout_get_pixel_size(_W_GRAPHICS(gc)->layout, &stringWidth,
			&stringHeight);
	if (rect->width >= 0) {
		PangoAlignment alignment = PANGO_ALIGN_LEFT;
		int diff = rect->width - stringWidth;
		if (diff < 0)
			diff = 0;
		if (flags & W_DRAW_HCENTER) {
			alignment = PANGO_ALIGN_CENTER;
			x = rect->x + diff / 2;
		}
		if (flags & W_DRAW_RIGHT) {
			alignment = PANGO_ALIGN_RIGHT;
			x = rect->x + diff;
		}
		pango_layout_set_alignment(_W_GRAPHICS(gc)->layout, alignment);
	}
	if (rect->height >= 0) {
		if (rect->height >= stringHeight) {
			int diff = rect->height - stringWidth;
			if (flags & W_DRAW_VCENTER) {
				y = rect->y + diff / 2;
			}
			if (flags & W_DRAW_BOTTOM) {
				y = rect->y + diff;
			}
		}
	}
	_w_graphics_check(gc,
			GRAPHICS_STATE_FONT | GRAPHICS_STATE_FOREGROUND
					| GRAPHICS_STATE_BACKGROUND);
#if USE_CAIRO
	if ((flags & W_DRAW_TRANSPARENT) == 0) {
		cairo_rectangle(_W_GRAPHICS(gc)->cairo, x, y, stringWidth,
				stringHeight);
		cairo_fill(_W_GRAPHICS(gc)->cairo);
	}
	if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
		cairo_save(_W_GRAPHICS(gc)->cairo);
		cairo_scale(_W_GRAPHICS(gc)->cairo, -1.0, 1);
		cairo_translate(_W_GRAPHICS(gc)->cairo, -2 * x - stringWidth, 0);
	}
	cairo_move_to(_W_GRAPHICS(gc)->cairo, x, y);
	pango_cairo_show_layout(_W_GRAPHICS(gc)->cairo, _W_GRAPHICS(gc)->layout);
	if ((_W_GRAPHICS(gc)->state & GRAPHICS_STATE_MIRRORED) != 0) {
		cairo_restore(_W_GRAPHICS(gc)->cairo);
	}
	cairo_new_path(_W_GRAPHICS(gc)->cairo);
	return TRUE;
#else
	graphics_check(t, GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_FONT | GRAPHICS_STATE_BACKGROUND_BG);
	GdkColor background;
	if ((flags & graphics_DRAW_TRANSPARENT) == 0)
	graphics_set_color(&background, t->background);
	//background = data.background;
	if (!t->xorMode) {
		gdk_draw_layout_with_colors(_W_GRAPHICS(gc)->drawable, _W_GRAPHICS(gc)->gc, x, y, _W_GRAPHICS(gc)->layout, 0,
				&background);
	} else {
		pango_layout_get_pixel_size(_W_GRAPHICS(gc)->layout, &stringWidth, &stringHeight);
		GdkPixmap* pixmap = gdk_pixmap_new(gdk_get_default_root_window(),
				stringWidth, stringHeight, -1);
		if (pixmap == 0)
		return;
		//SWT.error(SWT.ERROR_NO_HANDLES);
		GdkGC* gdkGC = gdk_gc_new(pixmap);
		if (gdkGC == 0)
		return;
		//SWT.error(SWT.ERROR_NO_HANDLES);
		GdkColor black, foreground;
		memset(&black, 0, sizeof(black));
		gdk_gc_set_foreground(gdkGC, &black);
		gdk_draw_rectangle(pixmap, gdkGC, 1, 0, 0, stringWidth,
				stringHeight);
		graphics_set_color(&foreground, _W_GRAPHICS(gc)->foreground);
		gdk_gc_set_foreground(gdkGC, &foreground);
		graphics_set_color(&foreground, _W_GRAPHICS(gc)->background);
		gdk_draw_layout_with_colors(pixmap, gdkGC, 0, 0, _W_GRAPHICS(gc)->layout, 0,
				&foreground);
		g_object_unref(gdkGC);
		gdk_draw_drawable(_W_GRAPHICS(gc)->drawable, _W_GRAPHICS(gc)->gc, pixmap, 0, 0, x, y,
				stringWidth, stringHeight);
		g_object_unref(pixmap);
	}
	_mnemonics_free(&mn, string);
#endif
}
wresult w_graphics_fill_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (width < 0) {
		x = x + width;
		width = -width;
	}
	if (height < 0) {
		y = y + height;
		height = -height;
	}
	if (width == 0 || height == 0 || arcAngle == 0)
		return W_TRUE;
#if USE_CAIRO
	if (width == height) {
		if (arcAngle >= 0) {
			cairo_arc_negative(_W_GRAPHICS(gc)->cairo, x + width / 2.0,
					y + height / 2.0, width / 2.0, -startAngle * M_PI / 180,
					-(startAngle + arcAngle) * M_PI / 180);
		} else {
			cairo_arc(_W_GRAPHICS(gc)->cairo, x + width / 2.0, y + height / 2.0,
					width / 2.0, -startAngle * M_PI / 180,
					-(startAngle + arcAngle) * M_PI / 180);
		}
		cairo_line_to(_W_GRAPHICS(gc)->cairo, x + width / 2.0,
				y + height / 2.0);
	} else {
		cairo_save(_W_GRAPHICS(gc)->cairo);
		cairo_translate(_W_GRAPHICS(gc)->cairo, x + width / 2.0,
				y + height / 2.0);
		cairo_scale(_W_GRAPHICS(gc)->cairo, width / 2.0, height / 2.0);
		if (arcAngle >= 0) {
			cairo_arc_negative(_W_GRAPHICS(gc)->cairo, 0, 0, 1,
					-startAngle * M_PI / 180,
					-(startAngle + arcAngle) * M_PI / 180);
		} else {
			cairo_arc(_W_GRAPHICS(gc)->cairo, 0, 0, 1, -startAngle * M_PI / 180,
					-(startAngle + arcAngle) * M_PI / 180);
		}
		cairo_line_to(_W_GRAPHICS(gc)->cairo, 0, 0);
		cairo_restore(_W_GRAPHICS(gc)->cairo);
	}
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	OS.gdk_draw_arc(data.drawable, handle, 1, x, y, width, height, startAngle * 64, arcAngle * 64);
#endif
}
wresult w_graphics_fill_gradientrectangle(w_graphics *gc, w_rect *rect,
		wbool vertical) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if ((width == 0) || (height == 0))
		return W_TRUE;

	/* Rewrite this to use GdkPixbuf */

	w_color backgroundRGB, foregroundRGB;
	backgroundRGB = _W_GRAPHICS(gc)->background;
	foregroundRGB = _W_GRAPHICS(gc)->foreground;

	w_color fromRGB, toRGB;
	fromRGB = foregroundRGB;
	toRGB = backgroundRGB;
	wbool swapColors = W_FALSE;
	if (width < 0) {
		x += width;
		width = -width;
		if (!vertical)
			swapColors = W_TRUE;
	}
	if (height < 0) {
		y += height;
		height = -height;
		if (vertical)
			swapColors = W_TRUE;
	}
	if (swapColors) {
		fromRGB = backgroundRGB;
		toRGB = foregroundRGB;
	}
	/*if (fromRGB.equals(toRGB)) {
	 fillRectangleInPixels(x, y, width, height);
	 return;
	 }*/
#if USE_CAIRO
	cairo_pattern_t *pattern;
	if (vertical) {
		pattern = cairo_pattern_create_linear(0.0, 0.0, 0.0, 1.0);
	} else {
		pattern = cairo_pattern_create_linear(0.0, 0.0, 1.0, 0.0);
	}
	cairo_pattern_add_color_stop_rgba(pattern, 0, W_RED(fromRGB) / 255.0,
	W_GREEN(fromRGB) / 255.0, W_BLUE(fromRGB) / 255.0,
	_W_GRAPHICS(gc)->alpha / 255.0);
	cairo_pattern_add_color_stop_rgba(pattern, 1, W_RED(toRGB) / 255.0,
	W_GREEN(toRGB) / 255.0, W_BLUE(toRGB) / 255.0,
	_W_GRAPHICS(gc)->alpha / 255.0);
	cairo_save(_W_GRAPHICS(gc)->cairo);
	cairo_translate(_W_GRAPHICS(gc)->cairo, x, y);
	cairo_scale(_W_GRAPHICS(gc)->cairo, width, height);
	cairo_rectangle(_W_GRAPHICS(gc)->cairo, 0, 0, 1, 1);
	cairo_set_source(_W_GRAPHICS(gc)->cairo, pattern);
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	cairo_restore(_W_GRAPHICS(gc)->cairo);
	cairo_pattern_destroy(pattern);
	return W_TRUE;
#else
	ImageData.fillGradientRectangle(this, data.device,
			DPIUtil.autoScaleDown(drawable, x), DPIUtil.autoScaleDown(drawable, y),
			DPIUtil.autoScaleDown(drawable, width), DPIUtil.autoScaleDown(drawable, height),
			vertical, fromRGB, toRGB, 8, 8, 8);
#endif
}
wresult w_graphics_fill_oval(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (width < 0) {
		x = x + width;
		width = -width;
	}
	if (height < 0) {
		y = y + height;
		height = -height;
	}
#if USE_CAIRO
	if (width == height) {
		cairo_arc_negative(_W_GRAPHICS(gc)->cairo, x + width / 2.0,
				y + height / 2.0, width / 2.0, 0, 2 * M_PI);
	} else {
		cairo_save(_W_GRAPHICS(gc)->cairo);
		cairo_translate(_W_GRAPHICS(gc)->cairo, x + width / 2.0,
				y + height / 2.0);
		cairo_scale(_W_GRAPHICS(gc)->cairo, width / 2.0, height / 2.0);
		cairo_arc_negative(_W_GRAPHICS(gc)->cairo, 0, 0, 1, 0, 2 * M_PI);
		cairo_restore(_W_GRAPHICS(gc)->cairo);
	}
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_arc(data.drawable, handle, 1, x, y, width, height, 0, 23040);
#endif
}
wresult w_graphics_fill_path(w_graphics *gc, w_path *path) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	if (path == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_PATH(path)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
	cairo_path_t *copy = cairo_copy_path(_W_PATH(path)->handle);
	if (copy == 0)
		return W_ERROR_NO_HANDLES;
	cairo_append_path(cairo, copy);
	cairo_path_destroy(copy);
	cairo_fill(cairo);
	return W_TRUE;
}
wresult w_graphics_fill_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
#if USE_CAIRO
	if (count == 0)
		return W_TRUE;
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	cairo_move_to(_W_GRAPHICS(gc)->cairo, pointArray[0].x + xOffset,
			pointArray[0].y + yOffset);
	for (int i = 1; i < count; i++) {
		cairo_line_to(_W_GRAPHICS(gc)->cairo, pointArray[i].x + xOffset,
				pointArray[i].y + yOffset);
	}
	cairo_close_path(_W_GRAPHICS(gc)->cairo);
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_polygon(data.drawable, handle, 0, pointArray, pointArray.length / 2);
#endif
}
wresult w_graphics_fill_polygonv(w_graphics *gc, int count, va_list args) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
#if USE_CAIRO
	if (count == 0)
		return W_TRUE;
	double xOffset = _W_GRAPHICS(gc)->cairoXoffset, yOffset =
	_W_GRAPHICS(gc)->cairoYoffset;
	int x = va_arg(args, int);
	int y = va_arg(args, int);
	cairo_move_to(_W_GRAPHICS(gc)->cairo, x + xOffset, y + yOffset);
	for (int i = 1; i < count; i++) {
		int x = va_arg(args, int);
		int y = va_arg(args, int);
		cairo_line_to(_W_GRAPHICS(gc)->cairo, x + xOffset, y + yOffset);
	}
	cairo_close_path(_W_GRAPHICS(gc)->cairo);
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	gdk_draw_polygon(data.drawable, handle, 0, pointArray, pointArray.length / 2);
#endif
}
wresult w_graphics_fill_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (width < 0) {
		x = x + width;
		width = -width;
	}
	if (height < 0) {
		y = y + height;
		height = -height;
	}
#if USE_CAIRO
	cairo_rectangle(_W_GRAPHICS(gc)->cairo, x, y, width, height);
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
	OS.gdk_draw_rectangle(data.drawable, handle, 1, x, y, width, height);
#endif
}
wresult w_graphics_fill_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = _w_graphics_check(gc, GRAPHICS_STATE_FILL);
	if (result < 0)
		return result;
	int nx = rect->x;
	int ny = rect->y;
	int nw = rect->width;
	int nh = rect->height;
	int naw = arcWidth;
	int nah = arcHeight;
	if (nw < 0) {
		nw = 0 - nw;
		nx = nx - nw;
	}
	if (nh < 0) {
		nh = 0 - nh;
		ny = ny - nh;
	}
	if (naw < 0)
		naw = 0 - naw;
	if (nah < 0)
		nah = 0 - nah;
#if USE_CAIRO
	if (naw == 0 || nah == 0) {
		cairo_rectangle(_W_GRAPHICS(gc)->cairo, rect->x, rect->y, rect->width,
				rect->height);
	} else {
		float naw2 = naw / 2.0;
		float nah2 = nah / 2.0;
		float fw = nw / naw2;
		float fh = nh / nah2;
		cairo_save(_W_GRAPHICS(gc)->cairo);
		cairo_translate(_W_GRAPHICS(gc)->cairo, nx, ny);
		cairo_scale(_W_GRAPHICS(gc)->cairo, naw2, nah2);
		cairo_move_to(_W_GRAPHICS(gc)->cairo, fw - 1, 0);
		cairo_arc(_W_GRAPHICS(gc)->cairo, fw - 1, 1, 1, M_PI + M_PI / 2.0,
		M_PI * 2.0);
		cairo_arc(_W_GRAPHICS(gc)->cairo, fw - 1, fh - 1, 1, 0, M_PI / 2.0);
		cairo_arc(_W_GRAPHICS(gc)->cairo, 1, fh - 1, 1, M_PI / 2, M_PI);
		cairo_arc(_W_GRAPHICS(gc)->cairo, 1, 1, 1, M_PI, 270.0 * M_PI / 180.0);
		cairo_close_path(_W_GRAPHICS(gc)->cairo);
		cairo_restore(_W_GRAPHICS(gc)->cairo);
	}
	cairo_fill(_W_GRAPHICS(gc)->cairo);
	return W_TRUE;
#else
#endif
}
int w_graphics_get_advance_width(w_graphics *gc, int ch) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	//BOGUS
	/* return stringExtentInPixels(new String(new char[]{ch})).x;*/
}
wbool w_graphics_get_advanced(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return W_FALSE;
	// return data.cairo != 0;
}
int w_graphics_get_alpha(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0xFF;
	return _W_GRAPHICS(gc)->alpha;
}
int w_graphics_get_antialias(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.cairo == 0) return SWT.DEFAULT;
	 int antialias = Cairo.cairo_get_antialias(data.cairo);
	 switch (antialias) {
	 case Cairo.CAIRO_ANTIALIAS_DEFAULT: return SWT.DEFAULT;
	 case Cairo.CAIRO_ANTIALIAS_NONE: return SWT.OFF;
	 case Cairo.CAIRO_ANTIALIAS_GRAY:
	 case Cairo.CAIRO_ANTIALIAS_SUBPIXEL: return SWT.ON;
	 }
	 return SWT.DEFAULT;*/
}
w_color w_graphics_get_background(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return W_COLOR_WHITE;
	return _W_GRAPHICS(gc)->background;
}
w_pattern* w_graphics_get_background_pattern(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	return _W_GRAPHICS(gc)->backgroundPattern;
}
int w_graphics_get_char_width(w_graphics *gc, int ch) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 //BOGUS
	 return stringExtentInPixels(new String(new char[]{ch})).x;*/
}
void w_graphics_getSize(w_graphics *gc, int *width, int *height) {
	/*if (_W_GRAPHICS(gc)->width != -1 && _W_GRAPHICS(gc)->height != -1) {
	 *width = _W_GRAPHICS(gc)->width;
	 *height = _W_GRAPHICS(gc)->height;
	 return;
	 }*/
	/*if (data.drawable != 0) {
	 if (OS.GTK_VERSION >= OS.VERSION(2, 24, 0)) {
	 width[0] = OS.gdk_window_get_width(data.drawable);
	 height[0] = OS.gdk_window_get_height(data.drawable);
	 } else {
	 OS.gdk_drawable_get_size(data.drawable, width, height);
	 }
	 return;
	 }*/
#if USE_CAIRO
	cairo_surface_t *surface = cairo_get_target(_W_GRAPHICS(gc)->cairo);
	switch (cairo_surface_get_type(surface)) {
	case CAIRO_SURFACE_TYPE_IMAGE:
		*width = cairo_image_surface_get_width(surface);
		*height = cairo_image_surface_get_height(surface);
		break;
		/*case CAIRO_SURFACE_TYPE_XLIB:
		 *width = cairo_xlib_surface_get_width(surface);
		 *height = cairo_xlib_surface_get_height(surface);
		 break;*/
	}
#endif
}
wresult w_graphics_get_clipping_rect(w_graphics *gc, w_rect *rect) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	GdkRectangle r;
	//Calculate visible bounds in device space
	rect->x = 0, rect->y = 0, rect->width = 0, rect->height = 0;
	//w_graphics_getSize(gc, &rect->width, &rect->height);
	gdk_cairo_get_clip_rectangle(_W_GRAPHICS(gc)->cairo, &r);
	// Intersect visible bounds with clipping in device space and then convert then to user space
	cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
	cairo_region_t *clipRgn = 0 /*_W_GRAPHICS(gc)->clipRgn*/;
	cairo_region_t *damageRgn = 0 /*_W_GRAPHICS(gc)->damageRgn*/;
	if (clipRgn != 0 || damageRgn != 0 || cairo != 0) {
		cairo_region_t *rgn = cairo_region_create();
		/*r.x = r.y = 0;
		 r.width = rect->width;
		 r.height = rect->height;*/
		cairo_region_union_rectangle(rgn, &r);
		if (damageRgn != 0) {
			cairo_region_intersect(rgn, damageRgn);
		}
		// Intersect visible bounds with clipping
		if (clipRgn != 0) {
			// Convert clipping to device space if needed
			/*if (data.clippingTransform != 0 && GTK_VERSION < VERSION (3, 14, 0)) {
			 clipRgn = convertRgn(clipRgn, data.clippingTransform);
			 GDK.gdk_region_intersect(rgn, clipRgn);
			 GDK.gdk_region_destroy(clipRgn);
			 } else {
			 GDK.gdk_region_intersect(rgn, clipRgn);
			 }*/
		}
		// Convert to user space
		if (cairo != 0 && GTK_VERSION < VERSION(3, 14, 0)) {
			/*cairo_matrix_t matrix;
			 cairo_get_matrix(cairo, &matrix);
			 cairo_matrix_invert(&matrix);
			 clipRgn = convertRgn(rgn, &matrix);
			 cairo_region_destroy(rgn);
			 rgn = clipRgn;*/
		}
		cairo_region_get_extents(rgn, &r);
		//gdk_region_get_clipbox(rgn, rect);
		cairo_region_destroy(rgn);
		rect->x = r.x;
		rect->y = r.y;
		rect->width = r.width;
		rect->height = r.height;
	}
	return W_TRUE;
}
wresult w_graphics_get_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (region == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_REGION(region)->handle == 0) {
		_W_REGION(region)->handle = cairo_region_create();
	} else {
		cairo_region_subtract(_W_REGION(region)->handle,
		_W_REGION(region)->handle);
	}
	cairo_region_t *clipping = _W_REGION(region)->handle;
	if (_W_GRAPHICS(gc)->clipRgn == 0) {
		GdkRectangle rect;
		gdk_cairo_get_clip_rectangle(_W_GRAPHICS(gc)->cairo, &rect);
		/*getSize(width, height);
		 rect.width = width[0];
		 rect.height = height[0];*/
		cairo_region_union_rectangle(clipping, &rect);
	} else {
		// Convert clipping to device space if needed
		/*if (data.clippingTransform != null && GTK.GTK_VERSION < OS.VERSION (3, 14, 0)) {
		 long rgn = convertRgn(clipRgn, data.clippingTransform);
		 GDK.gdk_region_union(clipping, rgn);
		 GDK.gdk_region_destroy(rgn);
		 } else {
		 GDK.gdk_region_union(clipping, clipRgn);
		 }*/
	}
	/*if (data.damageRgn != 0) {
	 GDK.gdk_region_intersect(clipping, data.damageRgn);
	 }*/
	// Convert to user space
	if (GTK_VERSION < VERSION(3, 14, 0)) {
		/*double[] matrix = new double[6];
		 Cairo.cairo_get_matrix(cairo, matrix);
		 Cairo.cairo_matrix_invert(matrix);
		 long rgn = convertRgn(clipping, matrix);
		 GDK.gdk_region_subtract(clipping, clipping);
		 GDK.gdk_region_union(clipping, rgn);
		 GDK.gdk_region_destroy(rgn);*/
	}
	return W_TRUE;
}
int w_graphics_get_fill_rule(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 long  cairo = data.cairo;
	 if (cairo == 0) return SWT.FILL_EVEN_ODD;
	 return Cairo.cairo_get_fill_rule(cairo) == Cairo.CAIRO_FILL_RULE_WINDING ? SWT.FILL_WINDING : SWT.FILL_EVEN_ODD;*/

}
w_font* w_graphics_get_font(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	return _W_GRAPHICS(gc)->font;
}
wresult w_graphics_get_font_metrics(w_graphics *gc, w_fontmetrics *fm) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	if (_W_GRAPHICS(gc)->context == 0) {
		wresult result = _w_graphics_create_layout(gc);
		if (result < 0)
			return result;
	}
	result = _w_graphics_check(gc, GRAPHICS_STATE_FONT);
	if (result < 0)
		return 0;
	w_font *font = _W_GRAPHICS(gc)->font;
	PangoContext *context = _W_GRAPHICS(gc)->context;
	PangoLanguage *lang = pango_context_get_language(context);
	PangoFontMetrics *metrics = pango_context_get_metrics(context,
	_W_FONT(font)->handle, lang);

	int ascent = pango_font_metrics_get_ascent(metrics);
	int descent = pango_font_metrics_get_descent(metrics);
	int ascentInPoints = ascent;
	_W_FONTMETRICS(fm)->ascentInPoints = ascentInPoints;
	int heightInPoints = ascent + descent;
	_W_FONTMETRICS(fm)->descentInPoints = heightInPoints - ascentInPoints;
	_W_FONTMETRICS(fm)->averageCharWidthInPoints =
			pango_font_metrics_get_approximate_char_width(metrics);
	pango_font_metrics_unref(metrics);
	return W_TRUE;
}
w_color w_graphics_get_foreground(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	return _W_GRAPHICS(gc)->foreground;
}
w_pattern* w_graphics_get_foreground_pattern(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.foregroundPattern;*/
}
int w_graphics_get_interpolation(w_graphics *gc) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return W_DEFAULT;
	if (_W_GRAPHICS(gc)->interpolation == 0xFF)
		return W_DEFAULT;
	else
		_W_GRAPHICS(gc)->interpolation;
}
w_line_attributes* w_graphics_get_line_attributes(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 float[] dashes = null;
	 if (data.lineDashes != null) {
	 dashes = new float[data.lineDashes.length];
	 System.arraycopy(data.lineDashes, 0, dashes, 0, dashes.length);
	 }
	 return new LineAttributes(data.lineWidth, data.lineCap, data.lineJoin, data.lineStyle, dashes, data.lineDashesOffset, data.lineMiterLimit);
	 */
}
int w_graphics_get_line_cap(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.lineCap;*/
}
w_line_dash* w_graphics_get_line_dash(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.lineDashes == null) return null;
	 int[] lineDashes = new int[data.lineDashes.length];
	 for (int i = 0; i < lineDashes.length; i++) {
	 lineDashes[i] = (int)data.lineDashes[i];
	 }
	 return lineDashes;*/
}
int w_graphics_get_line_join(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.lineJoin;*/
}
int w_graphics_get_line_style(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.lineStyle;*/
}
float w_graphics_get_line_width(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return (int)data.lineWidth;*/
}
int w_graphics_get_style(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.style;*/
}
int w_graphics_get_text_antialias(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.cairo == 0) return SWT.DEFAULT;
	 int antialias = Cairo.CAIRO_ANTIALIAS_DEFAULT;
	 if (data.context != 0) {
	 long  options = OS.pango_cairo_context_get_font_options(data.context);
	 if (options != 0) antialias = Cairo.cairo_font_options_get_antialias(options);
	 }
	 switch (antialias) {
	 case Cairo.CAIRO_ANTIALIAS_DEFAULT: return SWT.DEFAULT;
	 case Cairo.CAIRO_ANTIALIAS_NONE: return SWT.OFF;
	 case Cairo.CAIRO_ANTIALIAS_GRAY:
	 case Cairo.CAIRO_ANTIALIAS_SUBPIXEL: return SWT.ON;
	 }
	 return SWT.DEFAULT;*/
}
wresult w_graphics_get_transform(w_graphics *gc, w_transform *Transform) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (transform == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	 if (transform.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 long  cairo = data.cairo;
	 if (cairo != 0) {
	 /*
	 * The client wants to know the relative transformation they set for their widgets.
	 * They do not want to know about the global coordinates of their widget, which is contained in Cairo.cairo_get_matrix().
	 * So we return whatever the client specified with setTransform.
	 *
	 if (GTK.GTK_VERSION >= OS.VERSION (3, 14, 0)) {
	 if (currentTransform != null) {
	 transform.handle = currentTransform.clone();
	 } else {
	 transform.handle = new double[] { 1.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
	 }
	 } else {
	 Cairo.cairo_get_matrix(cairo, transform.handle);
	 double[] identity = identity();
	 Cairo.cairo_matrix_invert(identity);
	 Cairo.cairo_matrix_multiply(transform.handle, transform.handle, identity);
	 }
	 } else {
	 transform.setElements(1, 0, 0, 1, 0, 0);
	 }*/
}
wbool w_graphics_get_xor_mode(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.xorMode;
	 */
}
wbool w_graphics_is_clipped(w_graphics *gc) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.clipRgn != 0;*/
}
wresult w_graphics_set_advanced(w_graphics *gc, wbool advanced) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (!advanced) {
	 setAlpha(0xFF);
	 setAntialias(SWT.DEFAULT);
	 setBackgroundPattern(null);
	 resetClipping();
	 setForegroundPattern(null);
	 setInterpolation(SWT.DEFAULT);
	 setTextAntialias(SWT.DEFAULT);
	 setTransform(null);
	 }*/
}
wresult w_graphics_set_antialias(w_graphics *gc, int antialias) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.cairo == 0 && antialias == SWT.DEFAULT) return;
	 int mode = 0;
	 switch (antialias) {
	 case SWT.DEFAULT: mode = Cairo.CAIRO_ANTIALIAS_DEFAULT; break;
	 case SWT.OFF: mode = Cairo.CAIRO_ANTIALIAS_NONE; break;
	 case SWT.ON: mode = Cairo.CAIRO_ANTIALIAS_GRAY;
	 break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 initCairo();
	 long cairo = data.cairo;
	 Cairo.cairo_set_antialias(cairo, mode);*/
}
wresult w_graphics_set_alpha(w_graphics *gc, int alpha) {

	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_GRAPHIC_DISPOSED;
	//if (data.cairo == 0 && (alpha & 0xff) == 0xff) return;
	if ((_W_GRAPHICS(gc)->alpha & 0xff) == (alpha & 0xff))
		return W_TRUE;
	//initCairo();
	_W_GRAPHICS(gc)->alpha = alpha & 0xff;
	_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_BACKGROUND
			| GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_BACKGROUND_BG);
	return W_TRUE;
}
wresult w_graphics_set_background(w_graphics *gc, w_color color) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	_W_GRAPHICS(gc)->background = color & 0x00FFFFFF;
	_W_GRAPHICS(gc)->backgroundPattern = 0;
	_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_BACKGROUND
			| GRAPHICS_STATE_BACKGROUND_BG);
	return W_TRUE;
}
wresult w_graphics_set_background_pattern(w_graphics *gc, w_pattern *pattern) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (pattern != null && pattern.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 if (data.cairo == 0 && pattern == null) return;
	 initCairo();
	 if (data.backgroundPattern == pattern) return;
	 data.backgroundPattern = pattern;
	 data.state &= ~BACKGROUND;*/
}

/**
 * Intersects given clipping with original clipping of this context, so
 * that resulting clip does not allow to paint outside of the GC bounds.
 */
wresult w_graphics_limitClipping(w_graphics *gc, cairo_region_t *gcClipping) {
	cairo_region_t *clippingRegion = cairo_region_create();
	/*if (currentTransform != null) {
	 // we want to apply GC clipping stored in init() as is, so we invert user transformations that may distort it
	 double[] invertedCurrentTransform = currentTransform.clone();
	 Cairo.cairo_matrix_invert(invertedCurrentTransform);
	 int[] clippingWithoutUserTransform = transformRectangle(invertedCurrentTransform, clipping);
	 clippingRegion.add(clippingWithoutUserTransform);
	 GDK.gdk_region_intersect(gcClipping, clippingRegion.handle);
	 } else {*/
	cairo_region_union_rectangle(clippingRegion, &_W_GRAPHICS(gc)->clipping);
	//}
	cairo_region_intersect(gcClipping, clippingRegion);
	cairo_region_destroy(clippingRegion);
	return W_TRUE;
}
wresult w_graphics_set_cairo_clip(w_graphics *gc, cairo_region_t *damageRgn,
		cairo_region_t *clipRgn) {
#if GTK3
	cairo_reset_clip(_W_GRAPHICS(gc)->cairo);
#else
	gdk_cairo_reset_clip(cairo, data.drawable);
#endif
	if (damageRgn != 0) {
		cairo_matrix_t matrix;
		cairo_get_matrix(_W_GRAPHICS(gc)->cairo, &matrix);
		cairo_matrix_t identity;
		cairo_matrix_init_identity(&identity);
		cairo_set_matrix(_W_GRAPHICS(gc)->cairo, &identity);
		gdk_cairo_region(_W_GRAPHICS(gc)->cairo, damageRgn);
		cairo_clip(_W_GRAPHICS(gc)->cairo);
		cairo_set_matrix(_W_GRAPHICS(gc)->cairo, &matrix);
	}
	if (clipRgn != 0) {
		cairo_region_t *clipRgnCopy = cairo_region_create();
		cairo_region_union(clipRgnCopy, clipRgn);

		/*
		 * Bug 531667: widgets paint over other widgets
		 *
		 * The Cairo handle is shared by all widgets, but GC.setClipping allows global clipping changes.
		 * So we intersect whatever the client sets with the initial GC clipping.
		 */
		if (GTK_VERSION >= VERSION(3, 14, 0)) {
			w_graphics_limitClipping(gc, clipRgnCopy);
		}

		gdk_cairo_region(_W_GRAPHICS(gc)->cairo, clipRgnCopy);
		cairo_clip(_W_GRAPHICS(gc)->cairo);
		cairo_region_destroy(clipRgnCopy);
	}
	return W_TRUE;
}
wresult w_graphics_set_clipping(w_graphics *gc, cairo_region_t *clipRgn) {
	if (clipRgn == 0) {
		if (_W_GRAPHICS(gc)->clipRgn != 0) {
			cairo_region_destroy(_W_GRAPHICS(gc)->clipRgn);
			_W_GRAPHICS(gc)->clipRgn = 0;
		}
		//_W_GRAPHICS(gc)->clippingTransform = 0;
		w_graphics_set_cairo_clip(gc, _W_GRAPHICS(gc)->damageRgn, 0);
	} else {
		if (_W_GRAPHICS(gc)->clipRgn == 0)
			_W_GRAPHICS(gc)->clipRgn = cairo_region_create();
		cairo_region_subtract(_W_GRAPHICS(gc)->clipRgn,
		_W_GRAPHICS(gc)->clipRgn);
		cairo_region_union(_W_GRAPHICS(gc)->clipRgn, clipRgn);
		if (GTK_VERSION < VERSION(3, 14, 0)) {
			/*if (_W_GRAPHICS(gc)->clippingTransform == null) data.clippingTransform = new double[6];
			 Cairo.cairo_get_matrix(_W_GRAPHICS(gc)->cairo, data.clippingTransform);*/
		}
		w_graphics_set_cairo_clip(gc, _W_GRAPHICS(gc)->damageRgn, clipRgn);
	}
	return W_TRUE;
}
void w_graphics_reset_clipping(w_graphics *gc) {
	if (GTK_VERSION >= VERSION(3, 14, 0)) {
		/*
		 * Bug 531667: widgets paint over other widgets
		 *
		 * The Cairo handle is shared by all widgets, and GC.setClipping(0) allows painting outside the current GC area.
		 * So if we reset any custom clipping we still want to restrict GC operations with the initial GC clipping.
		 */
		cairo_region_t *clipRgn = cairo_region_create();
		cairo_region_union_rectangle(clipRgn, &_W_GRAPHICS(gc)->clipping);
		w_graphics_set_clipping(gc, clipRgn);
		cairo_region_destroy(clipRgn);
	} else {
		w_graphics_set_clipping(gc, 0);
	}
}
wresult w_graphics_set_clipping_path(w_graphics *gc, w_path *path) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (path != null && path.isDisposed()) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 resetClipping();
	 if (path != null) {
	 initCairo();
	 long cairo = data.cairo;
	 long copy = Cairo.cairo_copy_path(path.handle);
	 if (copy == 0) SWT.error(SWT.ERROR_NO_HANDLES);
	 Cairo.cairo_append_path(cairo, copy);
	 Cairo.cairo_path_destroy(copy);
	 Cairo.cairo_clip(cairo);
	 }*/
}
wresult w_graphics_set_clipping_rect(w_graphics *gc, w_rect *r) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (r == 0)
		return W_ERROR_NULL_ARGUMENT;
	GdkRectangle rect;
	rect.x = r->x;
	rect.y = r->y;
	rect.width = r->width;
	rect.height = r->height;
	if (rect.width < 0) {
		rect.x = rect.x + rect.width;
		rect.width = -rect.width;
	}
	if (rect.height < 0) {
		rect.y = rect.y + rect.height;
		rect.height = -rect.height;
	}

	cairo_region_t *clipRgn = cairo_region_create();
	cairo_region_union_rectangle(clipRgn, &rect);
	w_graphics_set_clipping(gc, clipRgn);
	cairo_region_destroy(clipRgn);
	return W_TRUE;
}
wresult w_graphics_set_clipping_region(w_graphics *gc, w_region *region) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (region == 0) {
		w_graphics_reset_clipping(gc);
	} else {
		if (_W_REGION(region)->handle == 0)
			return W_ERROR_INVALID_ARGUMENT;

		w_graphics_set_clipping(gc, _W_REGION(region)->handle);
	}
	return W_TRUE;
}
wresult w_graphics_set_fill_rule(w_graphics *gc, int rule) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 int cairo_mode = Cairo.CAIRO_FILL_RULE_EVEN_ODD;
	 switch (rule) {
	 case SWT.FILL_WINDING:
	 cairo_mode = Cairo.CAIRO_FILL_RULE_WINDING; break;
	 case SWT.FILL_EVEN_ODD:
	 cairo_mode = Cairo.CAIRO_FILL_RULE_EVEN_ODD; break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 initCairo();
	 Cairo.cairo_set_fill_rule(data.cairo, cairo_mode);*/
}
wresult w_graphics_set_font(w_graphics *gc, w_font *font) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (font != 0 && _W_FONT(font)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	_W_GRAPHICS(gc)->font = font != 0 ? font : w_toolkit_get_system_font(0);
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_FONT;
	//_W_GRAPHICS(gc)->stringWidth = _W_GRAPHICS(gc)->stringHeight = -1;
	return W_TRUE;
}
wresult w_graphics_set_foreground(w_graphics *gc, w_color color) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	_W_GRAPHICS(gc)->foreground = color & 0x00FFFFFF;
	_W_GRAPHICS(gc)->foregroundPattern = 0;
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_FOREGROUND;
	return W_TRUE;
}
wresult w_graphics_set_foreground_pattern(w_graphics *gc, w_pattern *pattern) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (pattern != null && pattern.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 if (data.cairo == 0 && pattern == null) return;
	 initCairo();
	 if (data.foregroundPattern == pattern) return;
	 data.foregroundPattern = pattern;
	 data.state &= ~FOREGROUND;
	 */
}
wresult w_graphics_set_interpolation(w_graphics *gc, int interpolation) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.cairo == 0 && interpolation == SWT.DEFAULT) return;
	 switch (interpolation) {
	 case SWT.DEFAULT:
	 case SWT.NONE:
	 case SWT.LOW:
	 case SWT.HIGH:
	 break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 initCairo();
	 data.interpolation = interpolation;*/
}
wresult w_graphics_set_line_attributes(w_graphics *gc,
		w_line_attributes *attributes) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (attributes == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	 int mask = 0;
	 float lineWidth = attributes.width;
	 if (lineWidth != data.lineWidth) {
	 mask |= LINE_WIDTH | DRAW_OFFSET;
	 }
	 int lineStyle = attributes.style;
	 if (lineStyle != data.lineStyle) {
	 mask |= LINE_STYLE;
	 switch (lineStyle) {
	 case SWT.LINE_SOLID:
	 case SWT.LINE_DASH:
	 case SWT.LINE_DOT:
	 case SWT.LINE_DASHDOT:
	 case SWT.LINE_DASHDOTDOT:
	 break;
	 case SWT.LINE_CUSTOM:
	 if (attributes.dash == null) lineStyle = SWT.LINE_SOLID;
	 break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 }
	 int join = attributes.join;
	 if (join != data.lineJoin) {
	 mask |= LINE_JOIN;
	 switch (join) {
	 case SWT.JOIN_MITER:
	 case SWT.JOIN_ROUND:
	 case SWT.JOIN_BEVEL:
	 break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 }
	 int cap = attributes.cap;
	 if (cap != data.lineCap) {
	 mask |= LINE_CAP;
	 switch (cap) {
	 case SWT.CAP_FLAT:
	 case SWT.CAP_ROUND:
	 case SWT.CAP_SQUARE:
	 break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 }
	 float[] dashes = attributes.dash;
	 float[] lineDashes = data.lineDashes;
	 if (dashes != null && dashes.length > 0) {
	 boolean changed = lineDashes == null || lineDashes.length != dashes.length;
	 for (int i = 0; i < dashes.length; i++) {
	 float dash = dashes[i];
	 if (dash <= 0) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 if (!changed && lineDashes[i] != dash) changed = true;
	 }
	 if (changed) {
	 float[] newDashes = new float[dashes.length];
	 System.arraycopy(dashes, 0, newDashes, 0, dashes.length);
	 dashes = newDashes;
	 mask |= LINE_STYLE;
	 } else {
	 dashes = lineDashes;
	 }
	 } else {
	 if (lineDashes != null && lineDashes.length > 0) {
	 mask |= LINE_STYLE;
	 } else {
	 dashes = lineDashes;
	 }
	 }
	 float dashOffset = attributes.dashOffset;
	 if (dashOffset != data.lineDashesOffset) {
	 mask |= LINE_STYLE;
	 }
	 float miterLimit = attributes.miterLimit;
	 if (miterLimit != data.lineMiterLimit) {
	 mask |= LINE_MITERLIMIT;
	 }
	 initCairo();
	 if (mask == 0) return;
	 data.lineWidth = lineWidth;
	 data.lineStyle = lineStyle;
	 data.lineCap = cap;
	 data.lineJoin = join;
	 data.lineDashes = dashes;
	 data.lineDashesOffset = dashOffset;
	 data.lineMiterLimit = miterLimit;
	 data.state &= ~mask;*/
}
wresult w_graphics_set_line_cap(w_graphics *gc, int cap) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.cap == cap)
		return W_TRUE;
	switch (cap) {
	case W_CAP_ROUND:
	case W_CAP_FLAT:
	case W_CAP_SQUARE:
		break;
	default:
		return W_TRUE;
	}
	_W_GRAPHICS(gc)->line.cap = cap;
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_CAP;
	return W_TRUE;
}
wresult w_graphics_set_line_dash(w_graphics *gc, int *dashes, int length) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	w_line_dash *lineDashes = _W_GRAPHICS(gc)->line.dash;
	if (dashes != 0 && length > 0) {
		wbool changed = _W_GRAPHICS(gc)->line.style != W_LINE_CUSTOM
				|| lineDashes == 0 || lineDashes->length != length;
		if (lineDashes != 0) {
			for (int i = 0; i < length; i++) {
				int dash = dashes[i];
				if (dash <= 0)
					dash = 0;	//SWT.error(SWT.ERROR_INVALID_ARGUMENT);
				if (!changed && lineDashes->dash[i] != dash)
					changed = W_TRUE;
			}
		}
		if (!changed)
			return W_TRUE;
		lineDashes = (w_line_dash*) malloc(
				sizeof(w_line_dash) + length * sizeof(float));
		lineDashes->length = length;
		for (int i = 0; i < length; i++) {
			lineDashes->dash[i] = dashes[i];
		}
		if (_W_GRAPHICS(gc)->line.dash != 0) {
			free(_W_GRAPHICS(gc)->line.dash);
		}
		_W_GRAPHICS(gc)->line.dash = lineDashes;
		_W_GRAPHICS(gc)->line.style = W_LINE_CUSTOM;
	} else {
		if (_W_GRAPHICS(gc)->line.style == W_LINE_SOLID
				&& (lineDashes == 0 || lineDashes->length == 0))
			return W_TRUE;
		if (_W_GRAPHICS(gc)->line.dash != 0) {
			free(_W_GRAPHICS(gc)->line.dash);
		}
		_W_GRAPHICS(gc)->line.dash = 0;
		_W_GRAPHICS(gc)->line.style = W_LINE_SOLID;
	}
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_join(w_graphics *gc, int join) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.join == join)
		return W_TRUE;
	switch (join) {
	case W_JOIN_MITER:
	case W_JOIN_ROUND:
	case W_JOIN_BEVEL:
		break;
	default:
		return W_ERROR_INVALID_ARGUMENT;
	}
	_W_GRAPHICS(gc)->line.join = join;
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_JOIN;
	return W_TRUE;
}
wresult w_graphics_set_line_style(w_graphics *gc, int lineStyle) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.style == lineStyle)
		return W_TRUE;
	switch (lineStyle) {
	case W_LINE_SOLID:
	case W_LINE_DASH:
	case W_LINE_DOT:
	case W_LINE_DASHDOT:
	case W_LINE_DASHDOTDOT:
		break;
	case W_LINE_CUSTOM:
		if (_W_GRAPHICS(gc)->line.dash == 0)
			lineStyle = W_LINE_SOLID;
		break;
	default:
		return W_ERROR_INVALID_ARGUMENT;
	}
	_W_GRAPHICS(gc)->line.style = lineStyle;
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_width(w_graphics *gc, float lineWidth) {
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.width == lineWidth)
		return W_TRUE;
	_W_GRAPHICS(gc)->line.width = lineWidth;
	_W_GRAPHICS(gc)->state &= ~(GRAPHICS_STATE_LINE_WIDTH
			| GRAPHICS_STATE_DRAW_OFFSET);
	return W_TRUE;
}
wresult w_graphics_set_xor_mode(w_graphics *gc, wbool _xor) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (Cairo.cairo_version() >= Cairo.CAIRO_VERSION_ENCODE(1, 10, 0)) {
	 Cairo.cairo_set_operator(handle, xor ? Cairo.CAIRO_OPERATOR_DIFFERENCE : Cairo.CAIRO_OPERATOR_OVER);
	 }
	 data.xorMode = xor;*/
}
wresult w_graphics_set_text_antialias(w_graphics *gc, int antialias) {
	/*
	 if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.cairo == 0 && antialias == SWT.DEFAULT) return;
	 int mode = 0;
	 switch (antialias) {
	 case SWT.DEFAULT: mode = Cairo.CAIRO_ANTIALIAS_DEFAULT; break;
	 case SWT.OFF: mode = Cairo.CAIRO_ANTIALIAS_NONE; break;
	 case SWT.ON: mode = Cairo.CAIRO_ANTIALIAS_GRAY;
	 break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 initCairo();
	 long options = Cairo.cairo_font_options_create();
	 Cairo.cairo_font_options_set_antialias(options, mode);
	 if (data.context == 0) createLayout();
	 OS.pango_cairo_context_set_font_options(data.context, options);
	 Cairo.cairo_font_options_destroy(options);*/
}
wresult w_graphics_set_transform(w_graphics *gc, w_transform *transform) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (!w_transform_is_ok(transform))
		return W_ERROR_INVALID_ARGUMENT;
	cairo_t *cairo = _W_GRAPHICS(gc)->cairo;
	if (GTK_VERSION >= VERSION(3, 14, 0)) {
		cairo_matrix_t matrix;
		// Re-set the original Cairo transformation matrix: it contains a translation relative to the parent widget.
		/*if (currentTransform != null) {
		 cairo_set_matrix(cairo, cairoTransformationMatrix);
		 currentTransform = null;
		 }*/
		// Apply user transform on top of the current transformation matrix (and remember it)
		if (transform != 0) {
			//currentTransform = transform.handle.clone();
			cairo_matrix_t transformMatrix;
			cairo_matrix_init(&transformMatrix, 1, 0, 0, 1, 0, 0);
			cairo_matrix_multiply(&transformMatrix,
			_W_TRANSFORM(transform)->handle, &transformMatrix);
			cairo_transform(cairo, &transformMatrix);
		}
	} else {
		cairo_matrix_t identity;
		cairo_matrix_init(&identity, 1, 0, 0, 1, 0, 0);
		if (transform != 0) {
			cairo_matrix_multiply(&identity, _W_TRANSFORM(transform)->handle,
					&identity);
		}
		cairo_set_matrix(cairo, &identity);
	}
	_W_GRAPHICS(gc)->state &= ~GRAPHICS_STATE_DRAW_OFFSET;
	return result;
}
wresult w_graphics_text_extent(w_graphics *gc, const char *string, int length,
		w_size *size, int flags) {
	wresult result = _w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (_W_GRAPHICS(gc)->cairo == 0)
		return W_ERROR_NO_HANDLES;
	if (string == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (*string == 0)
		return W_OK;
	result = _w_graphics_set_text(gc, string, length, flags);
	if (result < 0)
		return result;
	_w_graphics_check(gc, GRAPHICS_STATE_FONT);
	pango_layout_get_pixel_size(_W_GRAPHICS(gc)->layout, &size->width,
			&size->height);
	return W_TRUE;
}
#endif
