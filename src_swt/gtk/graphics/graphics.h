/*
 * graphics.h
 *
 *  Created on: 15 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_SWTC_GTK_GRAPHICS_GRAPHICS_H_
#define SRC_SWTC_GTK_GRAPHICS_GRAPHICS_H_
#ifdef __linux
#include <swt.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pango/pango.h>
#include <cairo/cairo.h>
#include <gio/gio.h>
#define GTK4 (GTK_MAJOR_VERSION == 4)
#define GTK3 (GTK_MAJOR_VERSION == 3)
#define GTK2 (GTK_MAJOR_VERSION == 2)
#define GTK1 (GTK_MAJOR_VERSION == 1)
#define USE_CAIRO 1
#define VERSION(major,minor,micro) ((major << 16) + (minor << 8) + micro)
#define H_GTK_VERSION VERSION(GTK_MAJOR_VERSION,GTK_MINOR_VERSION,GTK_MICRO_VERSION)
#define H_GDK_VERSION VERSION(GDK_MAJOR_VERSION,GDK_MINOR_VERSION,GDK_MICRO_VERSION)
#define IMAGE_REF (1 << 1)
#define _GTK_TEXT_FIX_REQUIRED_NULL (1 << 16)
#define _GTK_TEXT_FIX_CALCUL_LENGTH (1 << 17)
#define _GTK_TEXT_FIX_COPY (1 << 18)
typedef cairo_rectangle_int_t GdkRectangle;
typedef struct _mnemonics {
	char *str;
	size_t length;
} _mnemonics;
int _mnemonics_fix_0(struct _mnemonics *mnemonics, const char *text,
		size_t text_length, int replace, int *process);
void _mnemonics_fix(_mnemonics *mnemonics, const char *text,
		size_t text_length, int replace);
void _mnemonics_free(_mnemonics *mnemonics, const char *text);
int _mnemonics_from_gtk_0(const char *text, size_t text_length, char *out,
		size_t out_length, int *process);
char* _gtk_text_fix(const char *text, size_t text_length,size_t *newlength,int enc);
void _gtk_text_free(const char *text,char* alloc);
wresult _gtk_alloc_set_text(w_alloc string,void* user_data, const char *text,
		size_t text_length,int enc);
GInputStream * _w_input_stream_new(w_stream* stream);
GOutputStream* _w_output_stream_new(w_stream *stream);
struct _w_image {
	GdkPixbuf *pixbuf;
	int flags;
};
#define _W_IMAGE(x) ((struct _w_image*)x)

cairo_surface_t* _w_image_create_surface(GdkPixbuf *pixbuf);
cairo_pattern_t* _w_image_create_pattern(GdkPixbuf *pixbuf,
		cairo_surface_t **surface);

struct _w_surface {
	cairo_surface_t *surface;
};
#define _W_SURFACE(x) ((struct _w_surface*)x)
struct _w_imagelist_images {
	int alloc;
	int count;
	int style;
	int width;
	int height;
	void *reserved;
	GdkPixbuf *images[0];
};
struct _w_imagelist {
	struct _w_imagelist_images *images;
};
#define _W_IMAGELIST(x) ((struct _w_imagelist*)x)
typedef struct _w_cursor {
	GdkCursor *handle;
} _w_cursor;
#define _W_CURSOR(x) ((struct _w_cursor*)x)
struct _w_font {
	PangoFontDescription *handle;
};
#define _W_FONT(x) ((struct _w_font*)x)
struct _w_fontdata {
	PangoStyle style :2;
	PangoStretch stretch :4;
	PangoWeight weight :10;
	wushort size;
	char family[0];
};
#define _W_FONTMETRICS(x) ((struct _w_fontmetrics*)x)
struct _w_fontmetrics {
	int ascentInPoints;
	int descentInPoints;
	int averageCharWidthInPoints;
};
#define _W_FONTDATA(x) ((struct _w_fontdata*)x)

struct _w_region {
	cairo_region_t *handle;
};
#define _W_REGION(x) ((struct _w_region*)x)
void _gdk_region_get_rectangles(cairo_region_t *region,
		GdkRectangle **rectangles, int *n_rectangles);
struct _w_pattern {
	cairo_pattern_t *handle;
};
#define _W_PATTERN(x) ((struct _w_pattern*)x)

struct _w_transform {
	cairo_matrix_t *handle;
};
#define _W_TRANSFORM(x) ((struct _w_transform*)x)

struct _w_path {
	cairo_t *handle;
	unsigned moved :1;
	unsigned closed :1;
};
#define _W_PATH(x) ((struct _w_path*)x)

#define GRAPHICS_STATE_CAIRO  (1 << 0)
#define GRAPHICS_STATE_FOREGROUND  (1 << 1)
#define GRAPHICS_STATE_BACKGROUND  (1 << 2)
#define GRAPHICS_STATE_FONT  (1 << 3)
#define GRAPHICS_STATE_LINE_STYLE  (1 << 4)
#define GRAPHICS_STATE_LINE_CAP  (1 << 5)
#define GRAPHICS_STATE_LINE_JOIN  (1 << 6)
#define GRAPHICS_STATE_LINE_WIDTH  (1 << 7)
#define GRAPHICS_STATE_LINE_MITERLIMIT  (1 << 8)
#define GRAPHICS_STATE_BACKGROUND_BG  (1 << 9)
#define GRAPHICS_STATE_DRAW_OFFSET  (1 << 10)
#define GRAPHICS_STATE_DRAW  (GRAPHICS_STATE_FOREGROUND | GRAPHICS_STATE_LINE_WIDTH | \
GRAPHICS_STATE_LINE_STYLE | GRAPHICS_STATE_LINE_CAP | GRAPHICS_STATE_LINE_JOIN\
		| GRAPHICS_STATE_LINE_MITERLIMIT | GRAPHICS_STATE_DRAW_OFFSET)
#define GRAPHICS_STATE_FILL  GRAPHICS_STATE_BACKGROUND
#define GRAPHICS_STATE_MIRRORED  (1 << 11)
typedef struct _w_graphics {
	union {
#if USE_CAIRO
		struct {
			cairo_t *cairo;
			PangoContext *context;
			PangoLayout *layout;
			double cairoXoffset, cairoYoffset;
		};
#else
#endif
	};
	int state;
	unsigned char alpha;
	unsigned char interpolation;
	w_color foreground;
	w_color background;
	w_pattern *foregroundPattern;
	w_pattern *backgroundPattern;
	w_font *font;
	w_line_attributes line;
	cairo_region_t *clipRgn;
	cairo_region_t *damageRgn;
	GdkRectangle clipping;

} _w_graphics;
#define _W_GRAPHICS(x) ((_w_graphics*)x)
#if USE_CAIRO
void _w_graphics_init(w_graphics *gc, cairo_t *drawable);
#else
void _w_graphics_init(w_graphics* gc,GdkWindow* drawable);
#endif
wresult _w_graphics_check(w_graphics *gc, int mask);
typedef struct _w_textlayout {
	PangoLayout *layout;
	PangoContext *context;
	PangoAttrList *attrList;
	PangoAttrList *selAttrList;
	w_font *font;
	char *text;
	int ascentInPoints;
	int descentInPoints;
	int indent;
	int wrapIndent;
	int wrapWidth;
	int *tabs;
	int *invalidOffsets;
} _w_textlayout;
#define _W_TEXTLAYOUT(x) ((struct _w_textlayout*)x)
#endif
#endif /* SRC_SWTC_GTK_GRAPHICS_GRAPHICS_H_ */
