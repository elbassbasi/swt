/*
 * graphics.h
 *
 *  Created on: 13 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_GRAPHICS_GRAPHICS_H_
#define SRC_WIN32_GRAPHICS_GRAPHICS_H_
#if defined( __WIN32__) || defined(__WIN64__)
#include "../runtime/core.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <gdiplus.h>
#include <mlang.h>
#include <usp10.h>
#define IMAGE_FLAGS_NODISPOSE_HBITMAP (1 << 1)
#define IMAGE_FLAGS_NODISPOSE_GPBITMAP (1 << 2)
#define IMAGE_FLAGS_ICON (1 << 3)
struct _w_image {
	HBITMAP image;
	GpBitmap *gimage;
	int flags;
};
HBITMAP _w_image_get_HBITMAP(w_image *image);
#define _W_IMAGE(x) ((struct _w_image*)x)
struct _w_cursor {
	HCURSOR cursor;
};
#define _W_CURSOR(x) ((struct _w_cursor*)x)
struct _w_imagelist {
	HIMAGELIST imagelist;
};
#define _W_IMAGELIST(x) ((struct _w_imagelist*)x)
struct _w_font {
	HFONT handle;
};
#define _W_FONT(x) ((struct _w_font*)x)
struct _w_region {
	HRGN handle;
};
#define _W_REGION(x) ((struct _w_region*)x)
struct _w_pattern {
	HBRUSH hbrush;
	GpBrush *handle;
};
#define _W_PATTERN(x) ((struct _w_pattern*)x)

//graphics

enum {
	W_GRAPHICS_DELETEDC = 1 << 0,
	W_GRAPHICS_RELEASEDC = 1 << 1,
	W_GRAPHICS_HIDEFOCUS = 1 << 2,
	W_GRAPHICS_GDIPRELEASEDC = 1 << 3,
	W_GRAPHICS_FOREGROUND = 1 << 5,
	W_GRAPHICS_BACKGROUND = 1 << 6,
	W_GRAPHICS_FONT = 1 << 7,
	W_GRAPHICS_LINE_STYLE = 1 << 8,
	W_GRAPHICS_LINE_WIDTH = 1 << 9,
	W_GRAPHICS_LINE_CAP = 1 << 10,
	W_GRAPHICS_LINE_JOIN = 1 << 11,
	W_GRAPHICS_LINE_MITERLIMIT = 1 << 12,
	W_GRAPHICS_FOREGROUND_TEXT = 1 << 13,
	W_GRAPHICS_BACKGROUND_TEXT = 1 << 14,
	W_GRAPHICS_BRUSH = 1 << 15,
	W_GRAPHICS_PEN = 1 << 16,
	W_GRAPHICS_NULL_BRUSH = 1 << 17,
	W_GRAPHICS_NULL_PEN = 1 << 18,
	W_GRAPHICS_DRAW_OFFSET = 1 << 19,
	W_GRAPHICS_MIRRORED = 1 << 20,
	W_GRAPHICS_UISF_HIDEACCEL = 1 << 21,
	W_GRAPHICS_UISF_HIDEFOCUS = 1 << 22,
	W_GRAPHICS_RIGHT_TO_LEFT = 1 << 23,
	W_GRAPHICS_LEFT_TO_RIGHT = 1 << 24,

	W_GRAPHICS_DRAW = W_GRAPHICS_FOREGROUND | W_GRAPHICS_LINE_STYLE
			| W_GRAPHICS_LINE_WIDTH | W_GRAPHICS_LINE_CAP | W_GRAPHICS_LINE_JOIN
			| W_GRAPHICS_LINE_MITERLIMIT | W_GRAPHICS_PEN
			| W_GRAPHICS_NULL_BRUSH | W_GRAPHICS_DRAW_OFFSET,
	W_GRAPHICS_FILL = W_GRAPHICS_BACKGROUND | W_GRAPHICS_BRUSH
			| W_GRAPHICS_NULL_PEN
};
typedef struct _w_graphics {
	HDC handle;
	HWND hwnd;
	w_font *font;
	struct _w_font native_font;
	int state;
	w_color foreground;
	w_color background;
	char alpha;
	w_line_attributes line;
	HPEN hPen;
	HPEN hOldPen;
	HBRUSH hBrush;
	HBRUSH hOldBrush;
	HFONT hGDIFont;
	GpGraphics *gdipGraphics;
	GpPen *gdipPen;
	GpBrush *gdipFgBrush;
	GpBrush *gdipBgBrush;
	GpBrush *gdipBrush;
	GpFont *gdipFont;
	float gdipXOffset, gdipYOffset;
	w_pattern *foregroundPattern;
	w_pattern *backgroundPattern;
	w_surface *image;
	HBITMAP oldbitmap;

}_w_graphics;

void _w_graphics_init(w_graphics *gc, HDC handle);
#define _W_GRAPHICS(x) ((_w_graphics*)x)

struct _w_textlayout_style {
	struct _w_textlayout_style *next;
	w_textstyle style;
	int start;
	int end;
};
struct _w_textlayout_styleitem {
	struct _w_textlayout_styleitem *next;
	struct _w_textlayout_styleitem *prev;
	struct _w_textlayout_style *style;
	int start;
	int length;
	unsigned lineBreak :1;
	unsigned softBreak :1;
	unsigned tab :1;
	unsigned glyphs_created :1;
	int x;
	int width;
	int ascentInPoints;
	int descentInPoints;
	int leadingInPoints;
	int underlinePos;
	int underlineThickness;
	int strikeoutPos;
	int strikeoutThickness;
	SCRIPT_CACHE psc;
	HFONT fallbackFont;
	SCRIPT_ANALYSIS analysis;
	int glyphCount;
	SCRIPT_LOGATTR *psla; //length
	WORD *glyphs; //maxGlyphs
	WORD *clusters; //maxGlyphs
	SCRIPT_VISATTR *visAttrs; //maxGlyphs
	int *advances; //glyphCount
	GOFFSET *goffsets; //glyphCount
	int *justify; //glyphCount
};

struct _w_textlayout_line {
	int lineOffset;
	int lineY;
	int lineWidth;
	int lineRunCount;
	struct _w_textlayout_styleitem *runs;
};
struct _w_textlayout_text {
	int length; //in utf16
	int utf8_length;
	int lineCount;
	wchar text[0];
};
struct _w_textlayout {
	IMLangFontLink2 *mLangFontLink2;
	w_font *font;
	struct _w_textlayout_text *text;
	struct _w_textlayout_style *styles;
	struct _w_textlayout_styleitem *allruns;
	struct _w_textlayout_line *lines;
	void *tmp_mem;
	int textDirection;
	int *tabs;
	int tabs_length;
	int lineSpacingInPoints;
	int ascentInPixels;
	int descentInPixels;
	int alignment;
	int wrapWidth;
	int orientation;
	int indent;
	int wrapIndent;
	unsigned justify :1;
	unsigned created :1;
};
#define _W_TEXTLAYOUT(x) ((struct _w_textlayout*)x)
#ifdef __cplusplus
}
#endif
#endif
#endif /* SRC_WIN32_GRAPHICS_GRAPHICS_H_ */
