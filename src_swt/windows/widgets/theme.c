/*
 * theme.c
 *
 *  Created on: 6 nov. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "widget.h"
#include "toolkit.h"
#include "vsstyle.h"
#include "vssym32.h"
typedef void (*_w_theme_part_id)(w_themedata *data, int *ids, int part);
HTHEME _w_theme_load_htheme(const char *clazz) {
	struct UnicodeString str;
	unicode_set(&str, clazz, -1);
	return OpenThemeData(NULL, str.str);
}
HTHEME _w_theme_get_htheme(int index, const char *clazz) {
	if (win_toolkit->themes[index] == 0) {
		win_toolkit->themes[index] = _w_theme_load_htheme(clazz);
	}
	return win_toolkit->themes[index];
}
void _w_theme_htheme_close(HTHEME hteme) {
}
void _w_themedata_close(w_themedata *data) {

}
void _w_themedata_compute_trim(w_themedata *data, w_graphics *gc,
		w_rect *result) {
	if (data->clientArea != 0) {
		memcpy(result, data->clientArea, sizeof(w_rect));
	} else {
		memset(result, 0, sizeof(w_rect));
	}
}
void _w_themedata_draw_focus(w_themedata *data, w_graphics *gc,
		w_rect *bounds) {

}
void _w_themedata_draw_image(w_themedata *data, w_graphics *gc, w_rect *bounds,
		w_image *image, int flags) {
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		w_rect src;
		src.x = 0;
		src.y = 0;
		w_image_get_size(image, &src.sz);
		w_graphics_draw_image(gc, image, &src, bounds,flags);
	}
}
void _w_themedata_draw_text(w_themedata *data, w_graphics *gc, w_rect *bounds,
		const char *text, size_t length, int flags) {
	_w_theme_part_id part_id =0;// data->clazz->reserved[0];
	size_t charsength = w_utf8_to_utf16(text, length, 0, 0);
	WCHAR *chars = _w_toolkit_malloc((charsength + 1) * sizeof(WCHAR));
	if (chars != 0) {
		w_utf8_to_utf16(text, length, chars, charsength + 1);
		int textFlags = DT_SINGLELINE;
		if ((flags & W_THEME_DRAW_LEFT) != 0)
			textFlags |= DT_LEFT;
		if ((flags & W_THEME_DRAW_HCENTER) != 0)
			textFlags |= DT_CENTER;
		if ((flags & W_THEME_DRAW_RIGHT) != 0)
			textFlags |= DT_RIGHT;
		if ((flags & W_THEME_DRAW_TOP) != 0)
			textFlags |= DT_TOP;
		if ((flags & W_THEME_DRAW_BOTTOM) != 0)
			textFlags |= DT_BOTTOM;
		if ((flags & W_THEME_DRAW_VCENTER) != 0)
			textFlags |= DT_VCENTER;
		RECT rect;
		rect.left = bounds->x;
		rect.right = bounds->x + bounds->width;
		rect.top = bounds->y;
		rect.bottom = bounds->y + bounds->height;
		HFONT oldFont = (HFONT) SelectObject(_W_GRAPHICS(gc)->handle,
				win_toolkit->systemfont.handle);
		if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
			HTHEME hTheme = (HTHEME)0;// data->reserved[0];
			int part[2];
			part_id(data, part, W_THEME_WIDGET_WHOLE);
			int iPartId = part[0];
			int iStateId = part[1];
			DrawThemeText(hTheme, _W_GRAPHICS(gc)->handle, iPartId, iStateId,
					chars, charsength, textFlags, 0, &rect);
		} else {
			RECT r;
			memcpy(&r,&rect,sizeof(RECT));
			DrawTextW(_W_GRAPHICS(gc)->handle, chars, charsength, &r,
					textFlags /*| DT_CALCRECT*/);
			/*int width = r.right - r.left;
			int height = r.bottom - r.top;
			HBITMAP hBitmap = CreateCompatibleBitmap(_W_GRAPHICS(gc)->handle,
					width, height);
			if (hBitmap != 0) {
				HDC memDC = CreateCompatibleDC(_W_GRAPHICS(gc)->handle);
				HBITMAP hOldBitmap = SelectObject(memDC, hBitmap);
				PatBlt(memDC, 0, 0, width, height, BLACKNESS);
				SetBkMode(memDC, TRANSPARENT);
				SelectObject(memDC,
						GetCurrentObject(_W_GRAPHICS(gc)->handle, OBJ_FONT));
				DrawTextW(memDC, chars, charsength, &r, textFlags);
				BitBlt(_W_GRAPHICS(gc)->handle, 0, 0, width,
						height, memDC, 0, 0, SRCINVERT);
				SelectObject(memDC, hOldBitmap);
				DeleteDC(memDC);
				DeleteObject(hBitmap);
			}*/
		}
		SelectObject(_W_GRAPHICS(gc)->handle, oldFont);
		_w_toolkit_free(chars,(charsength + 1) * sizeof(WCHAR));
	}
}
void _w_themedata_get_bounds(w_themedata *data, int part, w_rect *result,
		w_rect *bounds) {
	memcpy(result, bounds, sizeof(w_rect));
}
int _w_themedata_get_selection(w_themedata *data, w_point *offset,
		w_rect *bounds) {
	return 0;
}
int _w_themedata_hit_background(w_themedata *data, w_point *position,
		w_rect *bounds) {
	return -1;
}
void _w_themedata_measure_text(w_themedata *data, w_graphics *gc,
		w_rect *result, w_rect *bounds, const char *text, size_t length,
		int flags) {
	if (!(W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed())) {
		memset(result, 0, sizeof(w_rect));
		return;
	}
	HTHEME hTheme = (HTHEME)0;// data->reserved[0];
	size_t charsength = w_utf8_to_utf16(text, length, 0, 0);
	WCHAR *chars = _w_toolkit_malloc((charsength + 1) * sizeof(WCHAR));
	if (chars == 0) {
		memset(result, 0, sizeof(w_rect));
		return;
	}
	w_utf8_to_utf16(text, length, chars, charsength + 1);
	int textFlags = 0; //OS.DT_VCENTER | OS.DT_SINGLELINE | OS.DT_CALCRECT;
	if ((data->style & W_LEFT) != 0)
		textFlags |= DT_LEFT;
	if ((data->style & W_CENTER) != 0)
		textFlags |= DT_CENTER;
	if ((data->style & W_RIGHT) != 0)
		textFlags |= DT_RIGHT;
	RECT extent, rect;
	if (bounds != 0) {
		rect.left = bounds->x;
		rect.right = bounds->x + bounds->width;
		rect.top = bounds->y;
		rect.bottom = bounds->y + bounds->height;
	}
	_w_theme_part_id _w_theme_part_id_fn =0;// data->clazz->reserved[0];
	int part[2];
	_w_theme_part_id_fn(data, part, W_THEME_WIDGET_WHOLE);
	int iPartId = part[0];
	int iStateId = part[1];
	GetThemeTextExtent(hTheme, _W_GRAPHICS(gc)->handle, iPartId, iStateId,
			chars, charsength, textFlags, &rect, &extent);
	result->x = extent.left;
	result->y = extent.top;
	result->width = extent.right - extent.left;
	result->height = extent.bottom - extent.top;
}
void _w_theme_button_part_id(w_themedata *data, int *ids, int part) {
	int state = data->state;
	int style = data->style;
	int iPartId = 0, iStateId = 0;
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		if ((style & W_PUSH) != 0) {
			iPartId = BP_PUSHBUTTON;
			iStateId = PBS_NORMAL;
			if ((state & W_THEME_DEFAULTED) != 0
					&& (state & W_THEME_ACTIVE) != 0)
				iStateId = PBS_DEFAULTED;
			if ((state & W_THEME_HOT) != 0)
				iStateId = PBS_HOT;
			if ((state & W_THEME_PRESSED) != 0)
				iStateId = PBS_PRESSED;
			if ((state & W_THEME_DISABLED) != 0)
				iStateId = PBS_DISABLED;
		}
		if ((style & W_RADIO) != 0) {
			iPartId = BP_RADIOBUTTON;
		}
		if ((style & W_CHECK) != 0) {
			iPartId = BP_CHECKBOX;
		}
		if ((style & (W_CHECK | W_RADIO)) != 0) {
			if ((state & W_THEME_SELECTED) != 0) {
				iStateId = CBS_CHECKEDNORMAL;
				if ((state & W_THEME_HOT) != 0)
					iStateId = CBS_CHECKEDHOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId = CBS_CHECKEDPRESSED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId = CBS_CHECKEDDISABLED;
			} else {
				iStateId = CBS_UNCHECKEDNORMAL;
				if ((state & W_THEME_HOT) != 0)
					iStateId = CBS_UNCHECKEDHOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId = CBS_UNCHECKEDPRESSED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId = CBS_UNCHECKEDDISABLED;
			}
		}
	} else {
		if ((style & W_PUSH) != 0) {
			iPartId = DFC_BUTTON;
			iStateId = DFCS_BUTTONPUSH;
			/*if ((state & W_THEME_DEFAULTED) != 0
			 && (state & W_THEME_ACTIVE) != 0)
			 iStateId = PBS_DEFAULTED;*/
			if ((state & W_THEME_HOT) != 0)
				iStateId |= DFCS_HOT;
			if ((state & W_THEME_PRESSED) != 0)
				iStateId |= DFCS_PUSHED;
			if ((state & W_THEME_DISABLED) != 0)
				iStateId |= DFCS_INACTIVE;
		}
		if ((style & W_RADIO) != 0) {
			iPartId = DFC_BUTTON;
			iStateId = DFCS_BUTTONRADIO;
		}
		if ((style & W_CHECK) != 0) {
			iPartId = DFC_BUTTON;
			iStateId = DFCS_BUTTONCHECK;
		}
		if ((style & (W_CHECK | W_RADIO)) != 0) {
			if ((state & W_THEME_SELECTED) != 0) {
				iStateId |= DFCS_CHECKED;
				if ((state & W_THEME_HOT) != 0)
					iStateId |= DFCS_HOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId |= DFCS_PUSHED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId |= DFCS_INACTIVE;
			} else {
				//iStateId |= CBS_UNCHECKEDNORMAL;
				if ((state & W_THEME_HOT) != 0)
					iStateId |= DFCS_HOT;
				if ((state & W_THEME_PRESSED) != 0)
					iStateId |= DFCS_PUSHED;
				if ((state & W_THEME_DISABLED) != 0)
					iStateId |= DFCS_INACTIVE;
			}
		}
	}
	ids[0] = iPartId;
	ids[1] = iStateId;
}
void _w_theme_button_draw(w_themedata *data, w_graphics *gc, w_rect *bounds) {
	RECT rect;
	int part[2];
	rect.left = bounds->x;
	rect.right = bounds->x + bounds->width;
	rect.top = bounds->y;
	rect.bottom = bounds->y + bounds->height;
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		HTHEME hTheme = (HTHEME)0;// data->reserved[0];
		if (hTheme != 0) {
			_w_theme_button_part_id(data, part, W_THEME_WIDGET_WHOLE);
			if ((data->style & (W_CHECK | W_RADIO)) != 0) {
				SIZE size;
				GetThemePartSize(hTheme, _W_GRAPHICS(gc)->handle, part[0],
						part[1], &rect, TS_DRAW, &size);
				rect.right = rect.left + size.cx;
				DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0],
						part[1], &rect, 0);
				rect.left = rect.right + 3;
				rect.right = rect.left + bounds->width - size.cx - 3;
			} else {
				DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0],
						part[1], &rect, 0);
			}
			if (data->clientArea != 0) {
				RECT contentRect;
				GetThemeBackgroundContentRect(hTheme, _W_GRAPHICS(gc)->handle,
						part[0], part[1], &rect, &contentRect);
				data->clientArea->x = contentRect.left;
				data->clientArea->y = contentRect.top;
				data->clientArea->width = contentRect.right - contentRect.left;
				data->clientArea->height = contentRect.bottom - contentRect.top;
			}
		}
	} else {
		_w_theme_button_part_id(data, part, W_THEME_WIDGET_WHOLE);
		DrawFrameControl(_W_GRAPHICS(gc)->handle, &rect, part[0], part[1]);
	}
}
void _w_theme_combo_part_id(w_themedata *data, int *ids, int part) {
	int state = data->state;
	int style = data->style;
	int iPartId = 0, iStateId = 0;
	switch (part) {
	case W_THEME_WIDGET_WHOLE:
		iPartId = EP_EDITTEXT;
		iStateId = ETS_NORMAL;
		if ((state & W_THEME_DISABLED) != 0)
			iStateId = ETS_DISABLED;
		break;
	case W_THEME_COMBO_ARROW:
		iPartId = CP_DROPDOWNBUTTON;
		iStateId = CBXS_NORMAL;
		if ((state & W_THEME_DISABLED) != 0)
			iStateId = CBXS_DISABLED;
		if ((state & W_THEME_HOT) != 0)
			iStateId = CBXS_HOT;
		if ((state & W_THEME_PRESSED) != 0)
			iStateId = CBXS_PRESSED;
		break;
	}
	ids[0] = iPartId;
	ids[1] = iStateId;
}
void _w_theme_combo_draw(w_themedata *data, w_graphics *gc, w_rect *bounds) {
	if (W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()) {
		HTHEME hTheme = _w_theme_get_htheme(_W_THEME_CLASS_TEXT, "EDIT");
		RECT rect, contentRect;
		rect.left = bounds->x;
		rect.right = bounds->x + bounds->width;
		rect.top = bounds->y;
		rect.bottom = bounds->y + bounds->height;
		int part[2];
		_w_theme_combo_part_id(data, part, W_THEME_WIDGET_WHOLE);
		DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0], part[1],
				&rect, NULL);
		GetThemeBackgroundContentRect(hTheme, _W_GRAPHICS(gc)->handle, part[0],
				part[1], &rect, &contentRect);
		w_rect *clientArea = data->clientArea;
		if (clientArea != 0) {
			clientArea->x = contentRect.left;
			clientArea->y = contentRect.top;
			clientArea->width = contentRect.right - contentRect.left;
			clientArea->height = contentRect.bottom - contentRect.top;
		}
		hTheme = _w_theme_get_htheme(_W_THEME_CLASS_COMBO, "COMBOBOX");
		int width = GetThemeSysSize(hTheme, SM_CXVSCROLL);
		rect.left = contentRect.right - width;
		rect.top = contentRect.top;
		rect.right = contentRect.right;
		rect.bottom = contentRect.bottom;
		_w_theme_combo_part_id(data, part, W_THEME_COMBO_ARROW);
		DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0], part[1],
				&rect, NULL);
		if (clientArea != 0) {
			clientArea->width -= width;
		}
	}
}
int _w_theme_combo_hit_background(w_themedata *data, w_point *position,
		w_rect *bounds) {
	if (!(W_COMCTL32_VERSION >= VERSION(6, 0) && IsAppThemed()))
		return W_THEME_WIDGET_NOWHERE;
	if (!w_rect_contains(bounds, position->x, position->y))
		return W_THEME_WIDGET_NOWHERE;
	HTHEME hTheme = _w_theme_get_htheme(_W_THEME_CLASS_TEXT, "EDIT");
	int part[2];
	_w_theme_combo_part_id(data, part, W_THEME_WIDGET_WHOLE);
	int iPartId = part[0];
	int iStateId = part[1];
	RECT rect, contentRect;
	rect.left = bounds->x;
	rect.right = bounds->x + bounds->width;
	rect.top = bounds->y;
	rect.bottom = bounds->y + bounds->height;
	GetThemeBackgroundContentRect(hTheme, 0, iPartId, iStateId, &rect,
			&contentRect);
	hTheme = _w_theme_get_htheme(_W_THEME_CLASS_COMBO, "COMBOBOX");
	int width = GetThemeSysSize(hTheme, SM_CXVSCROLL);
	w_rect arrowRect;
	arrowRect.x = contentRect.right - width;
	arrowRect.y = contentRect.top;
	arrowRect.width = contentRect.bottom - contentRect.top;
	arrowRect.height = width;
	if (w_rect_contains(&arrowRect, position->x, position->y))
		return W_THEME_COMBO_ARROW;
	return W_THEME_WIDGET_WHOLE;
}
void _w_theme_progressbar_part_id(w_themedata *data, int *ids, int part) {
	int iPartId = 0, iStateId = 0;
	if ((data->style & W_VERTICAL) != 0) {
		iPartId = PP_BARVERT;
	} else {
		iPartId = PP_BAR;
	}
	ids[0] = iPartId;
	ids[1] = iStateId;
}
void _w_theme_progressbar_draw(w_themedata *data, w_graphics *gc,
		w_rect *bounds) {
	HTHEME hTheme = (HTHEME)0;// data->reserved[0];
	RECT rect;
	rect.left = bounds->x;
	rect.right = rect.left + bounds->width;
	rect.top = bounds->y;
	rect.bottom = rect.top + bounds->height;
	int chunkSize;
	GetThemeInt(hTheme, 0, 0, TMT_PROGRESSCHUNKSIZE, &chunkSize);
	int spaceSize;
	GetThemeInt(hTheme, 0, 0, TMT_PROGRESSSPACESIZE, &spaceSize);
	RECT content;
	int part[2];
	_w_theme_progressbar_part_id(data, part, W_THEME_WIDGET_WHOLE);
	if ((data->style & W_VERTICAL) != 0) {
		GetThemeBackgroundContentRect(hTheme, _W_GRAPHICS(gc)->handle, part[0],
				part[1], &rect, &content);
		DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0], part[1],
				&rect, NULL);
		int top = content.bottom
				- (((content.bottom - content.top)
						* (data->range.selection - data->range.minimum))
						/ (data->range.maximum - data->range.minimum));
		content.top = content.bottom - chunkSize;
		while (content.top >= top) {
			DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, PP_CHUNKVERT,
					0, &content, NULL);
			content.bottom -= chunkSize + spaceSize;
			content.top = content.bottom - chunkSize;
		}
		if (data->range.selection != 0) {
			DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, PP_CHUNKVERT,
					0, &content, NULL);
		}
	} else {
		GetThemeBackgroundContentRect(hTheme, _W_GRAPHICS(gc)->handle, part[0],
				part[1], &rect, &content);
		DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0], part[1],
				&rect, NULL);
		int div = data->range.maximum - data->range.minimum;
		if (div == 0)
			div = 1;
		int right = content.left
				+ (((content.right - content.left)
						* (data->range.selection - data->range.minimum)) / div);
		content.right = content.left + chunkSize;
		while (content.right <= right) {
			DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, PP_CHUNK, 0,
					&content, NULL);
			content.left += chunkSize + spaceSize;
			content.right = content.left + chunkSize;
		}
		if (data->range.selection != 0) {
			DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, PP_CHUNK, 0,
					&content, NULL);
		}
	}
}
int _w_theme_progressbar_hit_background(w_themedata *data, w_point *position,
		w_rect *bounds) {
	return w_rect_contains(bounds, position->x, position->y) ?
			W_THEME_WIDGET_WHOLE : W_THEME_WIDGET_NOWHERE;
}
void _w_theme_tabfolder_part_id(w_themedata *data, int *ids, int part) {
	int iPartId = TABP_PANE, iStateId = TIS_NORMAL;
	if ((data->state & W_THEME_DISABLED) != 0) {
		iStateId = TIS_DISABLED;
	} else {
		if ((data->state & W_THEME_HOT) != 0)
			iStateId = TIS_HOT;
		if ((data->state & W_THEME_SELECTED) != 0)
			iStateId = TIS_SELECTED;
	}
	ids[0] = iPartId;
	ids[1] = iStateId;
}
void _w_theme_tabfolder_draw(w_themedata *data, w_graphics *gc,
		w_rect *bounds) {
	HTHEME hTheme = (HTHEME)0;// data->reserved[0];
	RECT rect;
	rect.left = bounds->x;
	rect.right = bounds->x + bounds->width;
	rect.top = bounds->y;
	if ((data->style & W_BOTTOM) != 0) {
		rect.bottom = bounds->y + bounds->height - data->tab.tabsHeight;
	} else {
		rect.top += data->tab.tabsHeight;
		rect.bottom = bounds->y + bounds->height;
	}
	int part[2];
	_w_theme_tabfolder_part_id(data, part, W_THEME_WIDGET_WHOLE);
	DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0], part[1],
			&rect, NULL);
	if (data->tab.tabsArea != 0) {
		data->tab.tabsArea->x = bounds->x;
		data->tab.tabsArea->y = bounds->y;
		data->tab.tabsArea->width = bounds->width;
		data->tab.tabsArea->height = data->tab.tabsHeight;
		if ((data->style & W_BOTTOM) != 0) {
			data->tab.tabsArea->y += bounds->height - data->tab.tabsHeight;
		}
	}
}
void _w_theme_tabitem_part_id(w_themedata *data, int *ids, int part) {
	int iPartId = TABP_TABITEM, iStateId = TIS_NORMAL;
	if ((data->style & W_LEFT) != 0 && (data->style & W_RIGHT) != 0) {
		iPartId = TABP_TABITEMLEFTEDGE;
	} else if ((data->style & W_LEFT) != 0) {
		iPartId = TABP_TABITEMLEFTEDGE;
	} else if ((data->style & W_RIGHT) != 0) {
	}
	if ((data->state & W_THEME_HOT) != 0)
		iStateId = TIS_HOT;
	if ((data->state & W_THEME_FOCUSED) != 0)
		iStateId = TIS_FOCUSED;
	if ((data->state & W_THEME_SELECTED) != 0)
		iStateId = TIS_SELECTED;
	if ((data->state & W_THEME_DISABLED) != 0)
		iStateId = TIS_DISABLED;
	ids[0] = iPartId;
	ids[1] = iStateId;
}
#define TABITEM_INSET  2
#define TABITEM_INSET2 6
void _w_theme_tabitem_draw(w_themedata *data, w_graphics *gc, w_rect *bounds) {
	HTHEME hTheme = (HTHEME)0;// data->reserved[0];
	int state = data->state;
	int x = bounds->x, y = bounds->y, width = bounds->width, height =
			bounds->height;
	if ((data->tabitem.position & W_LEFT) != 0) {
		x += TABITEM_INSET;
		width -= TABITEM_INSET;
	}
	y += TABITEM_INSET;
	height -= TABITEM_INSET;
	if ((state & W_THEME_SELECTED) != 0) {
		x -= TABITEM_INSET;
		y -= TABITEM_INSET;
		width += TABITEM_INSET * 2;
		height += TABITEM_INSET * 2;
	}
	RECT rect;
	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;
	int part[2];
	_w_theme_tabitem_part_id(data, part, W_THEME_WIDGET_WHOLE);
	DrawThemeBackground(hTheme, _W_GRAPHICS(gc)->handle, part[0], part[1],
			&rect, NULL);
	w_rect *clientArea = data->clientArea;
	if (clientArea != 0) {
		RECT contentRect;
		GetThemeBackgroundContentRect(hTheme, _W_GRAPHICS(gc)->handle, part[0],
				part[1], &rect, &contentRect);
		clientArea->x = contentRect.left;
		clientArea->y = contentRect.top;
		clientArea->width = contentRect.right - contentRect.left;
		clientArea->height = contentRect.bottom - contentRect.top;
	}
}
void _w_theme_close_htheme(HTHEME htheme) {
}
void _w_theme_dispose(w_theme *theme) {
}
wresult _w_theme_open_data(w_theme *theme, int clazz_id, w_themedata *data) {
	HTHEME htheme;
	struct _w_themedata_class *clazz;
/*	switch (clazz_id) {
	case W_THEME_CLASS_BUTTON:
		clazz = &class_button;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_BUTTON, "BUTTON");
		break;
	case W_THEME_CLASS_COMBO:
		clazz = &class_combo;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_EDIT, "EDIT");
		break;
	case W_THEME_CLASS_GROUP:
		clazz = &class_group;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_BUTTON, "BUTTON");
		break;
	case W_THEME_CLASS_PROGRESSBAR:
		clazz = &class_progressbar;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_PROGRESSBAR, "PROGRESS");
		break;
	case W_THEME_CLASS_SCALE:
		clazz = &class_scale;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_SCALE, "TRACKBAR");
		break;
	case W_THEME_CLASS_SCROLLBAR:
		clazz = &class_scrollbar;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_SCROLLBAR, "SCROLLBAR");
		break;
	case W_THEME_CLASS_TABFOLDER:
		clazz = &class_tabfolder;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_TAB, "TAB");
		break;
	case W_THEME_CLASS_TABITEM:
		clazz = &class_tabitem;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_TAB, "TAB");
		break;
	case W_THEME_CLASS_TOOLBAR:
		clazz = &class_toolbar;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_TOOLBAR, "TOOLBAR");
		break;
	case W_THEME_CLASS_TOOLITEM:
		clazz = &class_toolitem;
		htheme = _w_theme_get_htheme(_W_THEME_CLASS_TOOLBAR, "TOOLBAR");
		break;
	default:
		return W_FALSE;
		break;
	}
	if (htheme != 0) {
		memset(data, 0, sizeof(w_themedata));
		data->clazz = clazz;
		data->reserved[0] = htheme;
		return W_TRUE;
	} else
		return W_FALSE;*/
}
_w_theme_class win32_theme = { //
		"windows", //
				_w_theme_dispose, //
				_w_theme_open_data, //
		};
#endif
