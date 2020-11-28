/*
 * graphics.h
 *
 *  Created on: 15 sept. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_GRAPHICS_GRAPHICS_H_
#define SRC_COCOA_GRAPHICS_GRAPHICS_H_
#ifdef __APPLE__
#include "core.h"
#ifndef __OBJC__
typedef struct NSCursor NSCursor;
#endif
struct _w_image {
};
#define _W_IMAGE(x) ((struct _w_image*)x)
struct _w_bitmap {
};
#define _W_BITMAP(x) ((struct _w_bitmap*)x)
struct _w_imagelist {
};
#define _W_IMAGELIST(x) ((struct _w_imagelist*)x)

typedef struct _w_cursor {
    NSCursor* handle;
} _w_cursor;
#define _W_CURSOR(x) ((struct _w_cursor*)x)
void NSCursor_get_systems(NSCursor** cursors);
void NSCursor_set(NSCursor* cursor);
void NSCursor_init();
struct _w_font {
};
#define _W_FONT(x) ((struct _w_font*)x)
struct _w_fontdata {
};
#define _W_FONTMETRICS(x) ((struct _w_fontmetrics*)x)
struct _w_fontmetrics{
	int ascentInPoints;
	int descentInPoints;
	int averageCharWidthInPoints;
};
#define _W_FONTDATA(x) ((struct _w_fontdata*)x)

struct _w_region {
};
#define _W_REGION(x) ((struct _w_region*)x)
struct _w_pattern {
};
#define _W_PATTERN(x) ((struct _w_pattern*)x)

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
struct _w_graphics {
	int state;
	unsigned char alpha;
	unsigned char interpolation;
	w_color foreground;
	w_color background;
	w_pattern *foregroundPattern;
	w_pattern *backgroundPattern;
	w_font *font;
	w_line_attributes line;

};
#define _W_GRAPHICS(x) ((struct _w_graphics*)x)
wresult _w_graphics_check(w_graphics *gc, int mask);
typedef struct _w_textlayout_style_item {
	w_textstyle style;
	int start;
} _w_textlayout_style_item;
typedef struct _w_textlayout {
} _w_textlayout;
#define _W_TEXTLAYOUT(x) ((struct _w_textlayout*)x)
#endif
#endif /* SRC_SWTC_GTK_GRAPHICS_GRAPHICS_H_ */
