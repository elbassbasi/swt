/*
 * font.c
 *
 *  Created on: 28 févr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
void w_fontdata_init(w_fontdata *fontdata) {
	memset(fontdata, 0, sizeof(LOGFONTW));
}
void w_fontdata_dispose(w_fontdata *fontdata) {
}
int w_fontdata_get_height(w_fontdata *fontdata) {
	return ((LOGFONTW*) fontdata)->lfHeight;
}
wresult w_fontdata_get_locale(w_fontdata *fontdata,w_alloc locale,void* user_data){

}
wresult w_fontdata_get_name(w_fontdata *fontdata,w_alloc name,void* user_data){

}
int w_fontdata_get_style(w_fontdata *fontdata) {
	int style = W_NORMAL;
	if (((LOGFONTW*) fontdata)->lfWeight == 700)
		style |= W_BOLD;
	if (((LOGFONTW*) fontdata)->lfItalic != 0)
		style |= W_ITALIC;
	return style;
}
wresult w_fontdata_set(w_fontdata *fontdata, const char *name, int height,
		int style) {
	w_fontdata_set_name(fontdata, name);
	w_fontdata_set_height(fontdata, height);
	w_fontdata_set_style(fontdata, style);
	return W_TRUE;
}
wresult w_fontdata_set_height(w_fontdata *fontdata, int height) {
	((LOGFONTW*) fontdata)->lfHeight = height;
	return W_TRUE;
}
wresult w_fontdata_set_locale(w_fontdata *fontdata, const char *locale) {
	return W_FALSE;
}
wresult w_fontdata_set_name(w_fontdata *fontdata, const char *name) {
	w_utf8_to_utf16(name, -1, ((LOGFONTW*) fontdata)->lfFaceName,
	LF_FACESIZE);
	((LOGFONTW*) fontdata)->lfFaceName[LF_FACESIZE - 1] = 0;
	return W_TRUE;
}
wresult w_fontdata_set_style(w_fontdata *fontdata, int style) {
	if ((style & W_BOLD) == W_BOLD) {
		((LOGFONTW*) fontdata)->lfWeight = 700;
	} else {
		((LOGFONTW*) fontdata)->lfWeight = 0;
	}
	if ((style & W_ITALIC) == W_ITALIC) {
		((LOGFONTW*) fontdata)->lfItalic = 1;
	} else {
		((LOGFONTW*) fontdata)->lfItalic = 0;
	}
	return W_TRUE;
}

/*
 * font
 */
void w_font_init(w_font *font) {
	_W_FONT(font)->handle = 0;
}
void w_font_dispose(w_font *font) {
}
wresult w_font_isok(w_font *font) {
	if(font != 0 && _W_FONT(font)->handle != 0){
		return W_TRUE;
	}else return W_FALSE;
}
wresult w_font_create(w_font *font, const char *name, int style, int size) {
	LOGFONTW logfont;
	memset(&logfont, 0, sizeof(LOGFONTW));
	w_fontdata_set((w_fontdata*) &logfont, name, size, style);
	return w_font_create_from_fontdata(font, (w_fontdata*) &logfont);
}
wresult w_font_create_from_fontdata(w_font *font, w_fontdata *fontdata) {
	_W_FONT(font)->handle = CreateFontIndirectW((LOGFONTW*) fontdata);
	if (_W_FONT(font)->handle == 0) {
		return W_ERROR_NO_HANDLES;
	} else
		return W_TRUE;
}
wresult w_font_get_fontdata(w_font *font, w_fontdata *fontdata) {
	GetObjectW(_W_FONT(font)->handle, sizeof(LOGFONTW), fontdata);
	return W_TRUE;
}
/*
 * font metrics
 */
void w_fontmetrics_init(w_fontmetrics *metrics) {
	memset(metrics,0,sizeof(TEXTMETRICW));
}
void w_fontmetrics_dispose(w_fontmetrics *metrics) {
}
int w_fontmetrics_get_ascent(w_fontmetrics *metrics) {
}
int w_fontmetrics_get_average_charwidth(w_fontmetrics *metrics) {
}
int w_fontmetrics_get_descent(w_fontmetrics *metrics) {
}
int w_fontmetrics_get_height(w_fontmetrics *metrics) {
}
int w_fontmetrics_get_leading(w_fontmetrics *metrics) {
}

#endif
