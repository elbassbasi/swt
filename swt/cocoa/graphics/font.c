/*
 * font.c
 *
 *  Created on: 19 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "graphics.h"
/*
 * fontdata
 */
void w_fontdata_init(w_fontdata *fontdata) {
	memset(fontdata, 0, sizeof(struct _w_fontdata));
}
void w_fontdata_dispose(w_fontdata *fontdata) {
}
wresult w_fontdata_create(w_fontdata *fontdata, const char *name, int height,
		int style) {
	w_fontdata_dispose(fontdata);
	return w_fontdata_set(fontdata, name, height, style);
}
int w_fontdata_get_height(w_fontdata *fontdata) {
}
wresult w_fontdata_get_locale(w_fontdata *fontdata,w_alloc alloc,void* user_data) {
}
size_t w_fontdata_copy_locale(w_fontdata *fontdata, char *local, size_t size) {
}
wresult w_fontdata_get_name(w_fontdata *fontdata,w_alloc alloc,void* user_data) {
}
size_t w_fontdata_copy_name(w_fontdata *fontdata, char *name, size_t size) {
}
int w_fontdata_get_style(w_fontdata *fontdata) {
}
wresult w_fontdata_set(w_fontdata *fontdata, const char *name, int height,
		int style) {
	w_fontdata_set_style(fontdata, style);
	w_fontdata_set_height(fontdata, height);
	w_fontdata_set_name(fontdata, name);
	return W_TRUE;
}
wresult w_fontdata_set_height(w_fontdata *fontdata, int height) {
	return W_TRUE;
}
wresult w_fontdata_set_locale(w_fontdata *fontdata, const char *locale) {
}
wresult w_fontdata_set_name(w_fontdata *fontdata, const char *name) {
	return W_TRUE;
}
wresult w_fontdata_set_style(w_fontdata *fontdata, int style) {
}

/*
 * font
 */
void w_font_init(w_font *font) {
}
void w_font_dispose(w_font *font) {
}
wresult w_font_create(w_font *font, const char *name, int style, int size) {
}
wresult w_font_create_from_fontdata(w_font *font, w_fontdata *fontdata) {
}
wresult w_font_get_fontdata(w_font *font, w_fontdata *fontdata) {
}
/*
 * font metrics
 */
void w_fontmetrics_init(w_fontmetrics *metrics) {
	_W_FONTMETRICS(metrics)->ascentInPoints = 0;
	_W_FONTMETRICS(metrics)->averageCharWidthInPoints = 0;
	_W_FONTMETRICS(metrics)->descentInPoints = 0;
}
void w_fontmetrics_dispose(w_fontmetrics *metrics) {
}
int w_fontmetrics_get_ascent(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->ascentInPoints;
}
int w_fontmetrics_get_average_charwidth(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->averageCharWidthInPoints;
}
int w_fontmetrics_get_descent(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->descentInPoints;
}
int w_fontmetrics_get_height(w_fontmetrics *metrics) {
	return _W_FONTMETRICS(metrics)->ascentInPoints
			+ _W_FONTMETRICS(metrics)->descentInPoints;
}
int w_fontmetrics_get_leading(w_fontmetrics *metrics) {
	return 0; // Pango has no concept of "leading"
}

#endif
