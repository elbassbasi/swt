/*
 * textlayout.c
 *
 *  Created on: 3 mars 2020
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
#include "../widgets/toolkit.h"
#define MERGE_MAX 512
#define TOO_MANY_RUNS 1024
#define LTR_MARK 0x200E
#define RTL_MARK 0x200F
GUID _CLSID_CMultiLanguage = { 0x275c23e2, 0x3747, 0x11d0, { 0x9f, 0xea, 0x00,
		0xaa, 0x00, 0x3f, 0x86, 0x46 } };
GUID _IID_IMLangFontLink2 = { 0xDCCFC162, 0x2B38, 0x11d2, { 0xB7, 0xEC, 0x00,
		0xC0, 0x4F, 0x8F, 0x5D, 0x9A } };
wresult w_textlayout_compute_runs(w_textlayout *textlayout, HDC hDC);
void w_textlayout_free_runs(w_textlayout *textlayout);
int w_textlayout_get_line_indent(w_textlayout *textlayout, int lineIndex);
void w_textstyle_init(w_textstyle *style) {
	memset(style, 0, sizeof(w_textstyle));
	style->metrics.width = -1;
	style->metrics.ascent = -1;
	style->metrics.descent = -1;
}
void w_textlayout_init(w_textlayout *textlayout) {
	_W_TEXTLAYOUT(textlayout)->created = FALSE;
}

boolean isAdherentBorder(w_textstyle *_this, w_textstyle *style) {
	if (_this == style)
		return TRUE;
	if (style == 0)
		return FALSE;
	/*if (_this->borderStyle != style->borderStyle)
		return FALSE;
	if (_this->borderColor != 0) {
		if (_this->borderColor != style->borderColor)
			return FALSE;
	} else {
		if (style->borderColor != 0)
			return FALSE;
		if (_this->foreground != 0) {
			if (_this->foreground != style->foreground)
				return FALSE;
		} else if (style->foreground != 0)
			return FALSE;
	}*/
	return TRUE;
}

boolean isAdherentUnderline(w_textstyle *_this, w_textstyle *style) {
	if (_this == style)
		return TRUE;
	if (style == 0)
		return FALSE;
	if (_this->underline != style->underline)
		return FALSE;
	if (_this->underlineStyle != style->underlineStyle)
		return FALSE;
	if (_this->underlineColor != 0) {
		if (_this->underlineColor != style->underlineColor)
			return FALSE;
	} else {
		if (style->underlineColor != 0)
			return FALSE;
		if (_this->foreground != 0) {
			if (_this->foreground != style->foreground)
				return FALSE;
		} else if (style->foreground != 0)
			return FALSE;
	}
	return TRUE;
}

boolean isAdherentStrikeout(w_textstyle *_this, w_textstyle *style) {
	if (_this == style)
		return TRUE;
	if (style == 0)
		return FALSE;
	if (_this->strikeout != style->strikeout)
		return FALSE;
	if (_this->strikeoutColor != 0) {
		if (_this->strikeoutColor != style->strikeoutColor)
			return FALSE;
	} else {
		if (style->strikeoutColor != 0)
			return FALSE;
		if (_this->foreground != 0) {
			if (_this->foreground != style->foreground)
				return FALSE;
		} else if (style->foreground != 0)
			return FALSE;
	}
	return TRUE;
}
void w_textlayout_dispose(w_textlayout *textlayout) {
	if (textlayout == 0)
		return;
	if (_W_TEXTLAYOUT(textlayout)->created == FALSE)
		return;
	w_textlayout_free_runs(textlayout);
	if (_W_TEXTLAYOUT(textlayout)->mLangFontLink2 != 0)
		_W_TEXTLAYOUT(textlayout)->mLangFontLink2->lpVtbl->Release(
		_W_TEXTLAYOUT(textlayout)->mLangFontLink2);
	if (_W_TEXTLAYOUT(textlayout)->text != 0) {
		free(_W_TEXTLAYOUT(textlayout)->text);
	}
	struct _w_textlayout_style *style = _W_TEXTLAYOUT(textlayout)->styles,
			*next;
	while (style != 0) {
		next = style->next;
		free(style);
		style = next;
	}
	_W_TEXTLAYOUT(textlayout)->created = FALSE;
}
wresult w_textlayout_isok(w_textlayout *textlayout) {
	if (textlayout == 0)
		return W_FALSE;
	if (_W_TEXTLAYOUT(textlayout)->created == FALSE)
		return W_FALSE;
	return W_TRUE;
}
wresult w_textlayout_create(w_textlayout *textlayout) {
	if (textlayout == 0)
		return W_FALSE;
	if (_W_TEXTLAYOUT(textlayout)->created == TRUE)
		w_textlayout_dispose(textlayout);
	memset(textlayout, 0, sizeof(struct _w_textlayout));
	_W_TEXTLAYOUT(textlayout)->wrapWidth = -1;
	_W_TEXTLAYOUT(textlayout)->ascentInPixels = -1;
	_W_TEXTLAYOUT(textlayout)->descentInPixels = -1;
	_W_TEXTLAYOUT(textlayout)->orientation = W_LEFT_TO_RIGHT;
	_W_TEXTLAYOUT(textlayout)->textDirection = W_LEFT_TO_RIGHT;
	_W_TEXTLAYOUT(textlayout)->created = TRUE;
	return W_TRUE;
}

/*
 *  Resolves text direction. If the nominal direction is LTR or RTL, no
 *  resolution is needed; if the nominal direction is "auto", have BidiUtil
 *  resolve it according to the first strong bidi character.
 */
int _w_textlayout_resolve_text_direction(w_textlayout *textlayout) {
	/*return _W_TEXTLAYOUT(textlayout)->textDirection == W_AUTO_TEXT_DIRECTION ?
	 BidiUtil.resolveTextDirection(text) :
	 _W_TEXTLAYOUT(textlayout)->textDirection;*/
	return W_LEFT_TO_RIGHT;
}
HFONT _w_textlayout_get_item_font(w_textlayout *textlayout,
		struct _w_textlayout_styleitem *item) {
	if (item->fallbackFont != 0)
		return item->fallbackFont;
	if (item->style != 0 && item->style->style.font != 0
			&& _W_FONT(item->style->style.font)->handle != 0) {
		return _W_FONT(item->style->style.font)->handle;
	}
	if (_W_TEXTLAYOUT(textlayout)->font != 0
			&& _W_FONT(_W_TEXTLAYOUT(textlayout)->font)->handle != 0) {
		return _W_FONT(_W_TEXTLAYOUT(textlayout)->font)->handle;
	}
	w_font *system_font = w_toolkit_get_system_font(0);
	return _W_FONT(system_font)->handle;
}
void w_textlayout_get_partial_selection(w_textlayout *textlayout,
		struct _w_textlayout_styleitem *run, int selectionStart,
		int selectionEnd, RECT *rect) {
	int end = run->start + run->length - 1;
	int selStart = w_max(selectionStart, run->start) - run->start;
	int selEnd = w_min(selectionEnd, end) - run->start;
	int cChars = run->length;
	int gGlyphs = run->glyphCount;
	int piX;
	int x = rect->left;
	int *advances = run->justify != 0 ? run->justify : run->advances;
	ScriptCPtoX(selStart, FALSE, cChars, gGlyphs, run->clusters, run->visAttrs,
			advances, &run->analysis, &piX);
	int runX =
			(_W_TEXTLAYOUT(textlayout)->orientation & W_RIGHT_TO_LEFT) != 0 ?
					run->width - piX : piX;
	rect->left = x + runX;
	ScriptCPtoX(selEnd, TRUE, cChars, gGlyphs, run->clusters, run->visAttrs,
			advances, &run->analysis, &piX);
	runX = (_W_TEXTLAYOUT(textlayout)->orientation & W_RIGHT_TO_LEFT) != 0 ?
			run->width - piX : piX;
	rect->right = x + runX;
}

void w_textlayout_draw_run_background(w_textlayout *textlayout, HDC hdc,
		struct _w_textlayout_styleitem *run, RECT *rect, int selectionStart,
		int selectionEnd, HBRUSH selBrush, boolean hasSelection) {
	int end = run->start + run->length - 1;
	boolean fullSelection = hasSelection && selectionStart <= run->start
			&& selectionEnd >= end;
	if (fullSelection) {
		SelectObject(hdc, selBrush);
		PatBlt(hdc, rect->left, rect->top, rect->right - rect->left,
				rect->bottom - rect->top, PATCOPY);
	} else {
		if (run->style != 0 && run->style->style.background != 0) {
			int bg = run->style->style.background;
			HBRUSH hBrush = CreateSolidBrush(bg & 0x00FFFFFF);
			HBRUSH oldBrush = SelectObject(hdc, hBrush);
			PatBlt(hdc, rect->left, rect->top, rect->right - rect->left,
					rect->bottom - rect->top, PATCOPY);
			SelectObject(hdc, oldBrush);
			DeleteObject(hBrush);
		}
		boolean partialSelection = hasSelection
				&& !(selectionStart > end || run->start > selectionEnd);
		if (partialSelection) {
			w_textlayout_get_partial_selection(textlayout, run, selectionStart,
					selectionEnd, rect);
			SelectObject(hdc, selBrush);
			PatBlt(hdc, rect->left, rect->top, rect->right - rect->left,
					rect->bottom - rect->top, PATCOPY);
		}
	}
}
RECT* w_textlayout_draw_run_text(w_textlayout *textlayout, HDC hdc,
		struct _w_textlayout_styleitem *run, RECT *rect, int baselineInPixels,
		int color, int selectionColor, int selectionStart, int selectionEnd) {
	//printf("drawruntext %d %d %s\n",run->start,run->length,_W_TEXTLAYOUT(textlayout)->text->text[run->start]);
	int end = run->start + run->length - 1;
	boolean hasSelection = selectionStart <= selectionEnd
			&& selectionStart != -1 && selectionEnd != -1;
	boolean fullSelection = hasSelection && selectionStart <= run->start
			&& selectionEnd >= end;
	boolean partialSelection = hasSelection && !fullSelection
			&& !(selectionStart > end || run->start > selectionEnd);
	int offset =
			(_W_TEXTLAYOUT(textlayout)->orientation & W_RIGHT_TO_LEFT) != 0 ?
					-1 : 0;
	int x = rect->left + offset;
	int y = rect->top + (baselineInPixels - run->ascentInPoints);
	HFONT hFont = _w_textlayout_get_item_font(textlayout, run);
	SelectObject(hdc, hFont);
	if (fullSelection) {
		color = selectionColor;
	} else {
		if (run->style != 0 && run->style->style.foreground != 0) {
			color = run->style->style.foreground;
		}
	}
	SetTextColor(hdc, color & 0x00FFFFFF);
	ScriptTextOut(hdc, &run->psc, x, y, 0, NULL, &run->analysis, 0, 0,
			run->glyphs, run->glyphCount, run->advances, run->justify,
			run->goffsets);
	if (partialSelection) {
		w_textlayout_get_partial_selection(textlayout, run, selectionStart,
				selectionEnd, rect);
		SetTextColor(hdc, selectionColor & 0x00FFFFFF);
		ScriptTextOut(hdc, &run->psc, x, y, ETO_CLIPPED, rect, &run->analysis,
				0, 0, run->glyphs, run->glyphCount, run->advances, run->justify,
				run->goffsets);
	}
	return fullSelection || partialSelection ? rect : 0;
}
RECT* w_textlayout_add_cliprect(w_textlayout *textlayout,
		struct _w_textlayout_styleitem *run, RECT *clipRect, RECT *rect,
		RECT *tmp, int selectionStart, int selectionEnd) {
	if (rect != 0) {
		if (clipRect == 0) {
			clipRect = tmp;
			SetRect(clipRect, -1, rect->top, -1, rect->bottom);
		}
		boolean isRTL = (_W_TEXTLAYOUT(textlayout)->orientation
				& W_RIGHT_TO_LEFT) != 0;
		if (run->start <= selectionStart
				&& selectionStart <= run->start + run->length) {
			if (run->analysis.fRTL ^ isRTL) {
				clipRect->right = rect->left;
			} else {
				clipRect->left = rect->left;
			}
		}
		if (run->start <= selectionEnd
				&& selectionEnd <= run->start + run->length) {
			if (run->analysis.fRTL ^ isRTL) {
				clipRect->left = rect->right;
			} else {
				clipRect->right = rect->right;
			}
		}
	}
	return clipRect;
}
int* computePolyline(int left, int top, int right, int bottom, int *length) {
	int height = bottom - top; // can be any number
	int width = 2 * height; // must be even
	int peaks = ceilf((float) (right - left) / (float) width);
	if (peaks == 0 && right - left > 2) {
		peaks = 1;
	}
	*length = ((2 * peaks) + 1) * 2;
	if (*length < 0)
		return 0;

	int *coordinates = malloc(*length * sizeof(int));
	if (coordinates == 0)
		return 0;
	for (int i = 0; i < peaks; i++) {
		int index = 4 * i;
		coordinates[index] = left + (width * i);
		coordinates[index + 1] = bottom;
		coordinates[index + 2] = coordinates[index] + width / 2;
		coordinates[index + 3] = top;
	}
	coordinates[*length - 2] = left + (width * peaks);
	coordinates[*length - 1] = bottom;
	return coordinates;
}

RECT* w_textlayout_draw_underline(w_textlayout *textlayout, HDC hdc, int x,
		int baselineInPixels, int lineUnderlinePos, int lineBottom,
		struct _w_textlayout_styleitem *run,
		struct _w_textlayout_styleitem *line_start,
		struct _w_textlayout_styleitem *nextRun, int color, int selectionColor,
		RECT *clipRect, RECT *pRect, int selectionStart, int selectionEnd,
		w_rect *drawClip) {
	struct w_textstyle *style = 0, *nextRunStyle;
	if (run->style != 0)
		style = &run->style->style;
	struct _w_textlayout_styleitem *item;
	RECT tmp;
	if (style == 0)
		return 0;
	if (!style->underline)
		return 0;
	clipRect = w_textlayout_add_cliprect(textlayout, run, clipRect, pRect, &tmp,
			selectionStart, selectionEnd);
	boolean lastRunVisible = drawClip != 0
			&& (x + run->x + run->width) > (drawClip->x + drawClip->width);
	nextRunStyle = 0;
	if (nextRun != 0 && nextRun->style != 0)
		nextRunStyle = &nextRun->style->style;
	if (nextRun == 0 || lastRunVisible
			|| !isAdherentUnderline(style, nextRunStyle)) {
		int left = run->x;
		int start = run->start;
		int end = run->start + run->length - 1;
		item = run;
		while (item != line_start) {
			if (item->prev->style == 0)
				break;
			if (isAdherentUnderline(style, &item->prev->style->style))
				break;
			left = item->prev->x;
			start = w_min(start, item->prev->start);
			end = w_max(end, item->prev->start + item->prev->length - 1);

		}
		boolean hasSelection = selectionStart <= selectionEnd
				&& selectionStart != -1 && selectionEnd != -1;
		boolean fullSelection = hasSelection && selectionStart <= start
				&& end <= selectionEnd;
		if (style->underline != 0) {
			color = style->underlineColor;
			clipRect = 0;
		} else {
			if (fullSelection) {
				color = selectionColor;
				clipRect = 0;
			} else {
				if (style->foreground != 0) {
					color = style->foreground;
				}
			}
		}
		RECT rect;
		int riseInPixels = style->rise;
		SetRect(&rect, x + left,
				baselineInPixels - lineUnderlinePos - riseInPixels,
				x + run->x + run->width,
				baselineInPixels - lineUnderlinePos + run->underlineThickness
						- riseInPixels);
		if (clipRect != 0) {
			if (clipRect->left == -1)
				clipRect->left = 0;
			if (clipRect->right == -1)
				clipRect->right = 0x7ffff;
			SetRect(clipRect, w_max(rect.left, clipRect->left), rect.top,
					w_min(rect.right, clipRect->right), rect.bottom);
		}
		switch (style->underlineStyle) {
		case W_UNDERLINE_SQUIGGLE:
		case W_UNDERLINE_ERROR: {
			int squigglyThickness = 1;
			int squigglyHeight = 2 * squigglyThickness;
			int squigglyY = w_min(rect.top - squigglyHeight / 2,
					lineBottom - squigglyHeight - 1);
			int l;
			int *points = computePolyline(rect.left, squigglyY, rect.right,
					squigglyY + squigglyHeight, &l);
			if (points != 0) {
				HPEN pen = CreatePen(PS_SOLID, squigglyThickness,
						color & 0x00FFFFFF);
				HPEN oldPen = SelectObject(hdc, pen);
				int state = SaveDC(hdc);
				IntersectClipRect(hdc, rect.left, squigglyY, rect.right + 1,
						squigglyY + squigglyHeight + 1);
				Polyline(hdc, (LPPOINT) points, l / 2);
				int length = l;
				if (length >= 2 && squigglyThickness <= 1) {
					SetPixel(hdc, points[length - 2], points[length - 1],
							color);
				}
				SelectObject(hdc, oldPen);
				DeleteObject(pen);
				RestoreDC(hdc, state);
				if (clipRect != 0) {
					pen = CreatePen(PS_SOLID, squigglyThickness,
							selectionColor & 0x00FFFFFF);
					oldPen = SelectObject(hdc, pen);
					state = SaveDC(hdc);
					IntersectClipRect(hdc, clipRect->left, squigglyY,
							clipRect->right + 1,
							squigglyY + squigglyHeight + 1);
					Polyline(hdc, (LPPOINT) points, l / 2);
					if (length >= 2 && squigglyThickness <= 1) {
						SetPixel(hdc, points[length - 2], points[length - 1],
								selectionColor);
					}
					SelectObject(hdc, oldPen);
					DeleteObject(pen);
					RestoreDC(hdc, state);
				}
				free(points);
			}
			break;
		}
		case W_UNDERLINE_SINGLE:
		case W_UNDERLINE_DOUBLE:
		case W_UNDERLINE_LINK: {
			//case W_UNDERLINE_IME_THICK:
			/*if (style.underlineStyle == UNDERLINE_IME_THICK) {
			 rect.top -= run.underlineThickness;
			 if (clipRect != null) clipRect.top -= run.underlineThickness;
			 }*/
			int bottom =
					style->underlineStyle == W_UNDERLINE_DOUBLE ?
							rect.bottom + run->underlineThickness * 2 :
							rect.bottom;
			if (bottom > lineBottom) {
				OffsetRect(&rect, 0, lineBottom - bottom);
				if (clipRect != 0)
					OffsetRect(clipRect, 0, lineBottom - bottom);
			}
			HBRUSH brush = CreateSolidBrush(color & 0x00FFFFFF);
			FillRect(hdc, &rect, brush);
			if (style->underlineStyle == W_UNDERLINE_DOUBLE) {
				SetRect(&rect, rect.left,
						rect.top + run->underlineThickness * 2, rect.right,
						rect.bottom + run->underlineThickness * 2);
				FillRect(hdc, &rect, brush);
			}
			DeleteObject(brush);
			if (clipRect != 0) {
				HBRUSH selBrush = CreateSolidBrush(selectionColor & 0x00FFFFFF);
				FillRect(hdc, clipRect, selBrush);
				if (style->underlineStyle == W_UNDERLINE_DOUBLE) {
					SetRect(clipRect, clipRect->left, rect.top, clipRect->right,
							rect.bottom);
					FillRect(hdc, clipRect, selBrush);
				}
				DeleteObject(selBrush);
			}
		}
			break;
			/*case W_UNDERLINE_IME_DASH:
			 case W_UNDERLINE_IME_DOT: {
			 int penStyle = style.underlineStyle == UNDERLINE_IME_DASH ? OS.PS_DASH : OS.PS_DOT;
			 long  pen = OS.CreatePen(penStyle, 1, color);
			 long  oldPen = OS.SelectObject(hdc, pen);
			 int descentInPixels = DPIUtil.autoScaleUp(run.descentInPoints);
			 OS.SetRect(rect, rect.left, baselineInPixels + descentInPixels, rect.right, baselineInPixels + descentInPixels + run.underlineThickness);
			 OS.MoveToEx(hdc, rect.left, rect.top, 0);
			 OS.LineTo(hdc, rect.right, rect.top);
			 OS.SelectObject(hdc, oldPen);
			 OS.DeleteObject(pen);
			 if (clipRect != null) {
			 pen = OS.CreatePen(penStyle, 1, selectionColor);
			 oldPen = OS.SelectObject(hdc, pen);
			 OS.SetRect(clipRect, clipRect.left, rect.top, clipRect.right, rect.bottom);
			 OS.MoveToEx(hdc, clipRect.left, clipRect.top, 0);
			 OS.LineTo(hdc, clipRect.right, clipRect.top);
			 OS.SelectObject(hdc, oldPen);
			 OS.DeleteObject(pen);
			 }
			 break;
			 }*/
		}
		return 0;
	}
	return clipRect;
}

RECT* w_textlayout_draw_strikeout(w_textlayout *textlayout, HDC hdc, int x,
		int baselineInPixels, struct _w_textlayout_styleitem *run,
		struct _w_textlayout_styleitem *line_start,
		struct _w_textlayout_styleitem *nextRun, int color, int selectionColor,
		RECT *clipRect, RECT *pRect, int selectionStart, int selectionEnd,
		w_rect *drawClip) {
	struct w_textstyle *style = 0, *nextRunStyle;
	if (run->style != 0)
		style = &run->style->style;
	struct _w_textlayout_styleitem *item;
	RECT tmp;
	if (style == 0)
		return 0;
	if (!style->strikeout)
		return 0;
	clipRect = w_textlayout_add_cliprect(textlayout, run, clipRect, pRect, &tmp,
			selectionStart, selectionEnd);
	boolean lastRunVisible = drawClip != 0
			&& (x + run->x + run->width) > (drawClip->x + drawClip->width);
	nextRunStyle = 0;
	if (nextRun != 0 && nextRun->style != 0)
		nextRunStyle = &nextRun->style->style;
	if (nextRun == 0 || lastRunVisible
			|| !isAdherentStrikeout(style, nextRunStyle)) {
		int left = run->x;
		int start = run->start;
		int end = run->start + run->length - 1;
		item = run;
		while (item != line_start) {
			if (item->prev->style == 0)
				break;
			if (isAdherentStrikeout(style, &item->prev->style->style))
				break;
			left = item->prev->x;
			start = w_min(start, item->prev->start);
			end = w_max(end, item->prev->start + item->prev->length - 1);

		}
		boolean hasSelection = selectionStart <= selectionEnd
				&& selectionStart != -1 && selectionEnd != -1;
		boolean fullSelection = hasSelection && selectionStart <= start
				&& end <= selectionEnd;
		if (style->strikeoutColor != 0) {
			color = style->strikeoutColor;
			clipRect = 0;
		} else {
			if (fullSelection) {
				color = selectionColor;
				clipRect = 0;
			} else {
				if (style->foreground != 0) {
					color = style->foreground;
				}
			}
		}
		RECT rect;
		int riseInPixels = style->rise;
		SetRect(&rect, x + left,
				baselineInPixels - run->strikeoutPos - riseInPixels,
				x + run->x + run->width,
				baselineInPixels - run->strikeoutPos + run->strikeoutThickness
						- riseInPixels);
		HBRUSH brush = CreateSolidBrush(color & 0x00FFFFFF);
		FillRect(hdc, &rect, brush);
		DeleteObject(brush);
		if (clipRect != 0) {
			HBRUSH selBrush = CreateSolidBrush(selectionColor & 0x00FFFFFF);
			if (clipRect->left == -1)
				clipRect->left = 0;
			if (clipRect->right == -1)
				clipRect->right = 0x7ffff;
			SetRect(clipRect, w_max(rect.left, clipRect->left), rect.top,
					w_min(rect.right, clipRect->right), rect.bottom);
			FillRect(hdc, clipRect, selBrush);
			DeleteObject(selBrush);
		}
		return 0;
	}
	return clipRect;
}
void w_textlayout_draw_border(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, int start, int end, int style, w_color color){

}
RECT* w_textlayout_draw_border_0(w_textlayout *textlayout, HDC hdc, int x, int y,
		int lineHeight, struct _w_textlayout_styleitem *run,
		struct _w_textlayout_styleitem *line_start,
		struct _w_textlayout_styleitem *nextRun, int index, int color,
		int selectionColor, RECT *clipRect, RECT *pRect, int selectionStart,
		int selectionEnd, w_rect *drawClip) {
	struct w_textstyle *style = 0, *nextRunStyle;
	if (run->style != 0)
		style = &run->style->style;
	struct _w_textlayout_styleitem *item;
	RECT tmp;
	if (style == 0)
		return 0;
	/*if (style->borderStyle == W_NONE)
		return 0;*/
	clipRect = w_textlayout_add_cliprect(textlayout, run, clipRect, pRect, &tmp,
			selectionStart, selectionEnd);
	boolean lastRunVisible = drawClip != 0
			&& (x + run->x + run->width) > (drawClip->x + drawClip->width);
	nextRunStyle = 0;
	if (nextRun != 0 && nextRun->style != 0)
		nextRunStyle = &nextRun->style->style;
	if (nextRun == 0 || lastRunVisible
			|| !isAdherentBorder(style, nextRunStyle)) {
		int left = run->x;
		int start = run->start;
		int end = run->start + run->length - 1;
		item = run;
		while (item != line_start) {
			if (item->prev->style == 0)
				break;
			if (isAdherentBorder(style, &item->prev->style->style))
				break;
			left = item->prev->x;
			start = w_min(start, item->prev->start);
			end = w_max(end, item->prev->start + item->prev->length - 1);

		}
		boolean hasSelection = selectionStart <= selectionEnd
				&& selectionStart != -1 && selectionEnd != -1;
		boolean fullSelection = hasSelection && selectionStart <= start
				&& end <= selectionEnd;
		/*if (style->borderColor != 0) {
			color = style->borderColor;
			clipRect = 0;
		} else {*/
			if (fullSelection) {
				color = selectionColor;
				clipRect = 0;
			} else {
				if (style->foreground != 0) {
					color = style->foreground;
				}
			}
		//}
		int lineWidth = 1;
		int pattern = 1;
		int lineStyle = PS_SOLID;
		/*switch (style->borderStyle) {
		case W_BORDER_SOLID:
			break;
		case W_BORDER_DASH: {
			lineStyle = PS_DASH;
			pattern = 4;
			break;
		}
		case W_BORDER_DOT: {
			lineStyle = PS_DOT;
			pattern = 2;
			break;
		}
		}*/
		HBRUSH oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = (int) color;
		logBrush.lbHatch = 0;
		HPEN newPen = ExtCreatePen(lineStyle | PS_GEOMETRIC, lineWidth,
				&logBrush, 0, 0);
		HPEN oldPen = SelectObject(hdc, newPen);
		RECT drawRect;
		SetRect(&drawRect, x + left, y, x + run->x + run->width,
				y + lineHeight);
		if (drawClip != 0) {
			if (drawRect.left < drawClip->x) {
				int remainder = drawRect.left % pattern;
				drawRect.left = drawClip->x / pattern * pattern + remainder
						- pattern;
			}
			if (drawRect.right > drawClip->x + drawClip->width) {
				int remainder = drawRect.right % pattern;
				drawRect.right = (drawClip->x + drawClip->width) / pattern
						* pattern + remainder + pattern;
			}
		}
		Rectangle(hdc, drawRect.left, drawRect.top, drawRect.right,
				drawRect.bottom);
		SelectObject(hdc, oldPen);
		DeleteObject(newPen);
		if (clipRect != 0) {
			int state = SaveDC(hdc);
			if (clipRect->left == -1)
				clipRect->left = 0;
			if (clipRect->right == -1)
				clipRect->right = 0x7ffff;
			IntersectClipRect(hdc, clipRect->left, clipRect->top,
					clipRect->right, clipRect->bottom);
			logBrush.lbColor = (int) selectionColor;
			HPEN selPen = ExtCreatePen(lineStyle | PS_GEOMETRIC, lineWidth,
					&logBrush, 0, 0);
			oldPen = SelectObject(hdc, selPen);
			Rectangle(hdc, drawRect.left, drawRect.top, drawRect.right,
					drawRect.bottom);
			RestoreDC(hdc, state);
			SelectObject(hdc, oldPen);
			DeleteObject(selPen);
		}
		SelectObject(hdc, oldBrush);
		return 0;
	}
	return clipRect;
}
wresult w_textlayout_draw(w_textlayout *textlayout, w_graphics *gc, int x,
		int y, int selectionStart, int selectionEnd,
		w_color selectionForeground, w_color selectionBackground, int flags) {
	if (!w_textlayout_isok(textlayout))
		return W_ERROR_NO_HANDLES;
	if (!w_graphics_is_ok(gc))
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_TEXTLAYOUT(textlayout)->text == 0)
		return W_TRUE;
	int length = _W_TEXTLAYOUT(textlayout)->text->length;
	if (length == 0 && flags == 0)
		return W_TRUE;
	struct _w_textlayout_styleitem *run, *nextRun;
	HDC hdc = _W_GRAPHICS(gc)->handle;
	printf("compute start    \n");
	wresult result = w_textlayout_compute_runs(textlayout, hdc);
	printf("compute end %d\n", result);
	if (result < 0)
		return result;
	w_rect clip;
	w_graphics_get_clipping_rect(gc, &clip);
	GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
	int foreground = _W_GRAPHICS(gc)->foreground;
	int linkColor = GetSysColor(COLOR_HOTLIGHT);
	char alpha = _W_GRAPHICS(gc)->alpha;
	wresult gdip = gdipGraphics != 0;
	w_color gdipForeground = 0;
	GpBrush *gdipLinkColor = 0;
	int state = 0;
	if (gdip) {
		/*gc.checkGC(GC.FOREGROUND);
		 gdipForeground = gc.getFgBrush();*/
	} else {
		state = SaveDC(hdc);
		/*if (( _W_GRAPHICS(gc)->state & STATE_MIRRORED) != 0) {
		 SetLayout(hdc, GetLayout(hdc) | LAYOUT_RTL);
		 }*/
	}
	wresult hasSelection = selectionStart <= selectionEnd && selectionStart != -1
			&& selectionEnd != -1;
	GpBrush *gdipSelBackground = 0, *gdipSelForeground = 0;
	HFONT lastHFont = 0;
	GpFont *gdipFont = 0;
	HBRUSH selBackground = 0;
	int selForeground = 0;
	if (hasSelection
			|| ((flags & W_LAST_LINE_SELECTION) != 0
					&& (flags & (W_FULL_SELECTION | W_DELIMITER_SELECTION)) != 0)) {
		int fgSel =
				selectionForeground != 0 ?
						selectionForeground : GetSysColor(COLOR_HIGHLIGHTTEXT);
		int bgSel =
				selectionBackground != 0 ?
						selectionBackground : GetSysColor(COLOR_HIGHLIGHT);
		if (gdip) {
			/*gdipSelBackground = createGdipBrush(bgSel, alpha);
			 gdipSelForeground = createGdipBrush(fgSel, alpha);*/
		} else {
			selBackground = CreateSolidBrush(bgSel);
			selForeground = fgSel;
		}
		if (hasSelection) {
			selectionStart = w_min(w_max(0, selectionStart), length - 1);
			selectionEnd = w_min(w_max(0, selectionEnd), length - 1);
		}
	}
	RECT rect;
	SetBkMode(hdc, TRANSPARENT);
	struct _w_textlayout_line *lines = _W_TEXTLAYOUT(textlayout)->lines,
			*lineRuns;
	int lineCount = _W_TEXTLAYOUT(textlayout)->text->lineCount;
	run = _W_TEXTLAYOUT(textlayout)->allruns;
	printf("runs ");
	while (run != 0) {
		printf("%d ", run->start);
		run = run->next;
	}
	printf("\nlinecount %d\n", lineCount);
	for (int line = 0; line < lineCount; line++) {
		int drawX = x + w_textlayout_get_line_indent(textlayout, line);
		int drawY = y + lines[line].lineY;
		lineRuns = &lines[line];
		printf("drawline %d  %d\n", line, lineRuns->runs->start);
		int lineHeight = lines[line + 1].lineY - lines[line].lineY
				- _W_TEXTLAYOUT(textlayout)->lineSpacingInPoints;

		//Draw last line selection
		if ((flags & (W_FULL_SELECTION | W_DELIMITER_SELECTION)) != 0
				&& (hasSelection || (flags & W_LAST_LINE_SELECTION) != 0)) {
			/*boolean extents = false;
			 if (line == runs.length - 1
			 && (flags & SWT.LAST_LINE_SELECTION) != 0) {
			 extents = true;
			 } else {
			 StyleItem run = lineRuns[lineRuns.length - 1];
			 if (run.lineBreak && !run.softBreak) {
			 if (selectionStart <= run.start
			 && run.start <= selectionEnd)
			 extents = true;
			 } else {
			 int endOffset = run.start + run.length - 1;
			 if (selectionStart <= endOffset && endOffset < selectionEnd
			 && (flags & SWT.FULL_SELECTION) != 0) {
			 extents = true;
			 }
			 }
			 }
			 if (extents) {
			 int width;
			 if ((flags & SWT.FULL_SELECTION) != 0) {
			 width = OS.IsWin95 ? 0x7FFF : 0x6FFFFFF;
			 } else {
			 width = lineHeight / 3;
			 }
			 if (gdip) {
			 Gdip.Graphics_FillRectangle(gdipGraphics, gdipSelBackground,
			 drawX + lineWidth[line], drawY, width, lineHeight);
			 } else {
			 OS.SelectObject(hdc, selBackground);
			 OS.PatBlt(hdc, drawX + lineWidth[line], drawY, width, lineHeight, OS.PATCOPY);
			 }
			 }*/
		}
		if (drawX > clip.x + clip.width)
			continue;
		if (drawX + lines[line].lineWidth < clip.x)
			continue;

		//Draw the background of the runs in the line
		int alignmentX = drawX;
		run = lineRuns->runs;
		for (int i = 0; i < lineRuns->lineRunCount; i++) {
			if (run->length == 0)
				continue;
			if (drawX > clip.x + clip.width)
				break;
			if (drawX + run->width >= clip.x) {
				if (!run->lineBreak || run->softBreak) {
					SetRect(&rect, drawX, drawY, drawX + run->width,
							drawY + lineHeight);
					if (gdip) {
						/*drawRunBackgroundGDIP(run, gdipGraphics, rect,
						 selectionStart, selectionEnd, alpha,
						 gdipSelBackground, hasSelection);*/
					} else {
						w_textlayout_draw_run_background(textlayout, hdc, run,
								&rect, selectionStart, selectionEnd,
								selBackground, hasSelection);
					}
				}
			}
			drawX += run->width;
			run = run->next;
		}

		//Draw the text, underline, strikeout, and border of the runs in the line
		int baselineInPixels = w_max(0,
		_W_TEXTLAYOUT(textlayout)->ascentInPixels);
		int lineUnderlinePos = 0;
		run = lineRuns->runs;
		for (int i = 0; i < lineRuns->lineRunCount; i++) {
			baselineInPixels = w_max(baselineInPixels, run->ascentInPoints);
			lineUnderlinePos = w_min(lineUnderlinePos, run->underlinePos);
			run = run->next;
			if (run == 0)
				break;
		}
		RECT *borderClip = 0, *underlineClip = 0, *strikeoutClip = 0,
				*pRect = 0;
		drawX = alignmentX;
		run = lineRuns->runs;
		for (int i = 0; i < lineRuns->lineRunCount; i++) {
			struct _w_textlayout_style *style = run->style;
			boolean hasAdorners = style != 0
					&& (style->style.underline || style->style.strikeout
							/*|| style->style.borderStyle != W_NONE*/);
			if (run->length == 0)
				continue;
			if (drawX > clip.x + clip.width)
				break;
			if (drawX + run->width >= clip.x) {
				boolean skipTab = run->tab && !hasAdorners;
				if (!skipTab && (!run->lineBreak || run->softBreak)
						&& !(style != 0
								&& (style->style.metrics.width != (wushort) -1))) {
					SetRect(&rect, drawX, drawY, drawX + run->width,
							drawY + lineHeight);
					if (gdip) {
						/*long hFont = getItemFont(run);
						 if (hFont != lastHFont) {
						 lastHFont = hFont;
						 if (gdipFont != 0)
						 Gdip.Font_delete(gdipFont);
						 long oldFont = OS.SelectObject(hdc, hFont);
						 gdipFont = Gdip.Font_new(hdc, hFont);
						 OS.SelectObject(hdc, oldFont);
						 if (gdipFont == 0)
						 SWT.error(SWT.ERROR_NO_HANDLES);
						 if (!Gdip.Font_IsAvailable(gdipFont)) {
						 Gdip.Font_delete(gdipFont);
						 gdipFont = 0;
						 }
						 }
						 long gdipFg = gdipForeground;
						 if (style != null && style.underline
						 && style.underlineStyle == SWT.UNDERLINE_LINK) {
						 if (gdipLinkColor == 0)
						 gdipLinkColor = createGdipBrush(linkColor,
						 alpha);
						 gdipFg = gdipLinkColor;
						 }
						 if (gdipFont != 0 && !run.analysis.fNoGlyphIndex) {
						 pRect = drawRunTextGDIP(gdipGraphics, run, rect,
						 gdipFont, baselineInPixels, gdipFg,
						 gdipSelForeground, selectionStart,
						 selectionEnd, alpha);
						 } else {
						 int fg =
						 style != null && style.underline
						 && style.underlineStyle
						 == SWT.UNDERLINE_LINK ?
						 linkColor : foreground;
						 pRect = drawRunTextGDIPRaster(gdipGraphics, run,
						 rect, baselineInPixels, fg, selForeground,
						 selectionStart, selectionEnd);
						 }
						 underlineClip = drawUnderlineGDIP(gdipGraphics, x,
						 drawY + baselineInPixels, lineUnderlinePos,
						 drawY + lineHeight, lineRuns, i, gdipFg,
						 gdipSelForeground, underlineClip, pRect,
						 selectionStart, selectionEnd, alpha, clip);
						 strikeoutClip = drawStrikeoutGDIP(gdipGraphics, x,
						 drawY + baselineInPixels, lineRuns, i, gdipFg,
						 gdipSelForeground, strikeoutClip, pRect,
						 selectionStart, selectionEnd, alpha, clip);
						 borderClip = drawBorderGDIP(gdipGraphics, x, drawY,
						 lineHeight, lineRuns, i, gdipFg,
						 gdipSelForeground, borderClip, pRect,
						 selectionStart, selectionEnd, alpha, clip);*/
					} else {
						int fg =
								style != 0 && style->style.underline
										&& style->style.underlineStyle
												== W_UNDERLINE_LINK ?
										linkColor : foreground;
						nextRun = run->next;
						if (i + 1 >= lineRuns->lineRunCount)
							nextRun = 0;
						pRect = w_textlayout_draw_run_text(textlayout, hdc, run,
								&rect, baselineInPixels, fg, selForeground,
								selectionStart, selectionEnd);
						underlineClip = w_textlayout_draw_underline(textlayout,
								hdc, x, drawY + baselineInPixels,
								lineUnderlinePos, drawY + lineHeight, run,
								lineRuns->runs, nextRun, fg, selForeground,
								underlineClip, pRect, selectionStart,
								selectionEnd, &clip);
						strikeoutClip = w_textlayout_draw_strikeout(textlayout,
								hdc, x, drawY + baselineInPixels, run,
								lineRuns->runs, nextRun, fg, selForeground,
								strikeoutClip, pRect, selectionStart,
								selectionEnd, &clip);
						/*borderClip = drawBorder(hdc, x, drawY, lineHeight,
						 lineRuns, i, fg, selForeground, borderClip,
						 pRect, selectionStart, selectionEnd, clip);*/
					}
				}
			}
			drawX += run->width;
			run = run->next;
		}
	}

	if (gdipSelBackground != 0)
		GdipDeleteBrush(gdipSelBackground);
	if (gdipSelForeground != 0)
		GdipDeleteBrush(gdipSelForeground);
	if (gdipLinkColor != 0)
		GdipDeleteBrush(gdipLinkColor);
	if (gdipFont != 0)
		GdipDeleteFont(gdipFont);
	if (state != 0)
		RestoreDC(hdc, state);
	if (selBackground != 0)
		DeleteObject(selBackground);
	return W_TRUE;
}
int w_textlayout_get_alignment(w_textlayout *textlayout) {
}
int w_textlayout_get_ascent(w_textlayout *textlayout) {
}
void w_textlayout_get_bounds(w_textlayout *textlayout, int start,
		int end, w_rect *rect, int enc) {
}
int w_textlayout_get_descent(w_textlayout *textlayout) {
}
w_font* w_textlayout_get_font(w_textlayout *textlayout) {
}
int w_textlayout_get_indent(w_textlayout *textlayout) {
}
wresult w_textlayout_get_justify(w_textlayout *textlayout) {
}
int w_textlayout_get_level(w_textlayout *textlayout, int offset,
		int enc) {
}
void w_textlayout_get_line_bounds(w_textlayout *textlayout, int lineIndex,
		w_rect *rect) {
}
int w_textlayout_get_line_count(w_textlayout *textlayout) {
}
int w_textlayout_get_line_index(w_textlayout *textlayout, int offset,
		int enc) {
}
wresult w_textlayout_get_line_metrics(w_textlayout *textlayout, int lineIndex,
		w_fontmetrics *fontmetrics) {
}
int w_textlayout_get_line_offset(w_textlayout *textlayout,
		int lineIndex, int enc) {
}
wresult w_textlayout_get_location(w_textlayout *textlayout,
		int offset, int trailing, w_point *pt, int enc) {
}
int w_textlayout_get_next_offset(w_textlayout *textlayout,
		int offset, int movement, int enc) {
}
int w_textlayout_get_offset(w_textlayout *textlayout, w_point *point,
		int *trailing, int trailing_count, int enc) {
}
int w_textlayout_get_orientation(w_textlayout *textlayout) {
}
int w_textlayout_get_previous_offset(w_textlayout *textlayout,
		int offset, int movement, int enc) {
}
wresult w_textlayout_get_ranges(w_textlayout *textlayout,
		w_iterator *ranges) {
}
int w_textlayout_get_spacing(w_textlayout *textlayout) {
}
wresult w_textlayout_get_style(w_textlayout *textlayout, int offset,
		w_textstyle *style, int enc) {
}
int w_textlayout_get_styles(w_textlayout *textlayout, w_textstyle *style,
		int count) {
}
int w_textlayout_get_tabs(w_textlayout *textlayout, int *tabs, int count) {
}
wresult w_textlayout_get_text(w_textlayout *textlayout, w_alloc text,
		void *user_data) {

}
int w_textlayout_get_text_direction(w_textlayout *textlayout) {
}
int w_textlayout_get_width(w_textlayout *textlayout) {
}
int w_textlayout_get_wrap_indent(w_textlayout *textlayout) {
}
wresult w_textlayout_set_alignment(w_textlayout *textlayout, int alignment) {
}
wresult w_textlayout_set_ascent(w_textlayout *textlayout, int ascent) {
}
wresult w_textlayout_set_descent(w_textlayout *textlayout, int descent) {
}
wresult w_textlayout_set_font(w_textlayout *textlayout, w_font *font) {
}
wresult w_textlayout_set_indent(w_textlayout *textlayout, int indent) {
}
wresult w_textlayout_set_justify(w_textlayout *textlayout,
		int justify) {
}
wresult w_textlayout_set_orientation(w_textlayout *textlayout, int orientation) {
}
wresult w_textlayout_set_spacing(w_textlayout *textlayout, int spacing) {
}
void w_textlayout_link_style(w_textlayout *textlayout,
		struct _w_textlayout_style *item, struct _w_textlayout_style *prev) {
	struct _w_textlayout_style *tmp;
	if (prev == 0) {
		tmp = _W_TEXTLAYOUT(textlayout)->styles;
		_W_TEXTLAYOUT(textlayout)->styles = item;
		item->next = tmp;
	} else {
		tmp = prev->next;
		prev->next = item;
		item->next = tmp;
	}
}
wresult w_textlayout_set_style(w_textlayout *textlayout,
		w_textstyle *style, int start, int end, int enc) {
	if (textlayout == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_TEXTLAYOUT(textlayout)->created == FALSE)
		return W_ERROR_NO_HANDLES;
	int length = _W_TEXTLAYOUT(textlayout)->text->length;
	if (length == 0)
		return W_ERROR_INVALID_RANGE;
	start = w_min(w_max(0, start), length);
	end = w_min(w_max(0, end), length);

	struct _w_textlayout_style *item = _W_TEXTLAYOUT(textlayout)->styles;
	struct _w_textlayout_style *prev = 0, *tmp, *begin = 0;
	while (item != 0) {
		if (begin == 0 && item->end >= start) {
			begin = prev;
		}
		if (item->end >= end) {

		}
		prev = item;
		item = item->next;
	}
	tmp = malloc(sizeof(struct _w_textlayout_style));
	if (tmp == 0)
		return W_ERROR_NO_MEMORY;
	tmp->start = start;
	tmp->end = end;
	memcpy(&tmp->style, style, sizeof(w_textstyle));
	w_textlayout_link_style(textlayout, tmp, prev);
	return W_TRUE;
}
wresult w_textlayout_set_tabs(w_textlayout *textlayout, int *tabs, int count) {
}
wresult w_textlayout_set_text(w_textlayout *textlayout, const char *text,
		int count) {
	return w_textlayout_set_text_segments(textlayout, text, count, 0, 0, 0);
}
wresult w_textlayout_set_text_segments(w_textlayout *textlayout,
		const char *text, int len, int *segments, wuint *segmentschars,
		int nsegments) {
	if (textlayout == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_TEXTLAYOUT(textlayout)->created == FALSE)
		return W_ERROR_NO_HANDLES;
	if (_W_TEXTLAYOUT(textlayout)->text != 0) {
		free(_W_TEXTLAYOUT(textlayout)->text);
	}
	wchar *_text;
	int length = 0, m;
	const char *in = text;
	while ((len < 0 || (m = text + len - in) > 0) && *in) {
		wuint wc = w_utf8_get_char_extended(in, len < 0 ? 6 : m);
		if (wc < 0x10000) {
			length += 1;
		} else {
			length += 2;
		}
		in = w_utf8_next(in);
	}
	_W_TEXTLAYOUT(textlayout)->text = malloc(
			sizeof(struct _w_textlayout_text)
					+ (length + nsegments + 1) * sizeof(wchar));
	if (_W_TEXTLAYOUT(textlayout)->text == 0)
		return W_ERROR_NO_MEMORY;
	_W_TEXTLAYOUT(textlayout)->text->length = length + nsegments;
	_text = _W_TEXTLAYOUT(textlayout)->text->text;
	if (nsegments == 0 || segments == 0) {
		length = 0;
		const char *in = text;
		while ((len < 0 || (m = text + len - in) > 0) && *in) {
			wint wc = w_utf8_get_char_extended(in, len < 0 ? 6 : m);
			if (wc < 0x10000) {
				_text[length++] = wc;
			} else {
				_text[length++] = (wc - 0x10000) / 0x400 + 0xd800;
				_text[length++] = (wc - 0x10000) % 0x400 + 0xdc00;
			}
			in = w_utf8_next(in);
		}
		_text[length] = 0;
	} else {
		int charCount = 0, segmentCount = 0;
		wint defaultSeparator =
				(_w_textlayout_resolve_text_direction(textlayout)
						& W_RIGHT_TO_LEFT) != 0 ? 0x200F : 0x200E;
		wchar *newChars = _W_TEXTLAYOUT(textlayout)->text->text;
		const char *oldChars = text;

		while (charCount < len) {
			if (segmentCount < nsegments
					&& charCount == segments[segmentCount]) {
				wchar separator =
						segmentschars != 0 ?
								segmentschars[segmentCount] : defaultSeparator;
				newChars[charCount + segmentCount++] = separator;
			} else {
				wint wc = w_utf8_get_next_char(oldChars, &oldChars);
				if (wc < 0x10000) {
					newChars[charCount + segmentCount] = wc;
				} else {
					newChars[charCount + segmentCount] = (wc - 0x10000) / 0x400
							+ 0xd800;
					charCount++;
					newChars[charCount + segmentCount] = (wc - 0x10000) % 0x400
							+ 0xdc00;
				}
				charCount++;
			}
		}
		while (segmentCount < nsegments) {
			segments[segmentCount] = charCount;
			wchar separator =
					segmentschars != 0 ?
							segmentschars[segmentCount] : defaultSeparator;
			newChars[charCount + segmentCount++] = separator;
		}
	}
	return W_TRUE;
}
wresult w_textlayout_set_text_direction(w_textlayout *textlayout,
		int textDirection) {
}
wresult w_textlayout_set_width(w_textlayout *textlayout, int width) {
	if (width < -1 || width == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (_W_TEXTLAYOUT(textlayout)->wrapWidth == width)
		return W_TRUE;
	w_textlayout_free_runs(textlayout);
	_W_TEXTLAYOUT(textlayout)->wrapWidth = width;
	return W_TRUE;
}
wresult w_textlayout_set_wrap_indent(w_textlayout *textlayout, int wrapIndent) {
}
/*
 *
 */
struct _w_textlayout_styleitem* w_textlayout_styleitem_create(
		w_textlayout *textlayout, struct _w_textlayout_styleitem *prev,
		int length) {
	int maxGlyphs = (length * 3 / 2) + 16;
	size_t sz = sizeof(struct _w_textlayout_styleitem)
			+ length * sizeof(SCRIPT_LOGATTR)
			+ maxGlyphs * (2 * sizeof(WORD) + sizeof(SCRIPT_VISATTR))
			+ maxGlyphs * (sizeof(int) + sizeof(GOFFSET));
	if (_W_TEXTLAYOUT(textlayout)->justify) {
		sz += maxGlyphs * sizeof(int);
	}
	struct _w_textlayout_styleitem *item = malloc(sz);
	if (item == 0) {
		printf("malloc failed\n");
		return 0;
	}
	memset(item, 0, sizeof(struct _w_textlayout_styleitem)/*sz*/);
	if (prev == 0) {
		_W_TEXTLAYOUT(textlayout)->allruns = item;
		item->prev = 0;
		item->next = 0;
	} else {
		item->next = prev->next;
		item->prev = prev;
		prev->next = item;
		if (item->next != 0) {
			item->next->prev = item;
		}
	}
	void *_mem = (void*) item + sizeof(struct _w_textlayout_styleitem)
			+ length * sizeof(SCRIPT_LOGATTR);
	item->glyphs = _mem;
	_mem += maxGlyphs * sizeof(WORD);
	item->clusters = _mem;
	_mem += maxGlyphs * sizeof(WORD);
	item->visAttrs = _mem;
	_mem += maxGlyphs * sizeof(SCRIPT_VISATTR);
	item->advances = _mem;
	_mem += maxGlyphs * sizeof(int);
	item->goffsets = _mem;
	_mem += maxGlyphs * sizeof(GOFFSET);
	if (_W_TEXTLAYOUT(textlayout)->justify) {
		item->justify = _mem;
	}
	return item;
}
struct _w_textlayout_styleitem* w_textlayout_styleitem_compact(
		w_textlayout *textlayout, struct _w_textlayout_styleitem *run) {
	struct _w_textlayout_styleitem *newrun, *next, *prev;
	void *_mem;
	int glyphCount = run->glyphCount;
	size_t sz = sizeof(struct _w_textlayout_styleitem)
			+ run->length * sizeof(SCRIPT_LOGATTR)
			+ glyphCount * (2 * sizeof(WORD) + sizeof(SCRIPT_VISATTR))
			+ glyphCount * (sizeof(int) + sizeof(GOFFSET));
	if (_W_TEXTLAYOUT(textlayout)->justify) {
		sz += glyphCount * sizeof(int);
	}
	_mem = (void*) run + sizeof(struct _w_textlayout_styleitem)
			+ run->length * sizeof(SCRIPT_LOGATTR) + glyphCount * sizeof(WORD);
	memcpy(_mem, run->clusters, run->glyphCount * sizeof(WORD));	//clusters
	run->clusters = _mem;
	_mem += glyphCount * sizeof(WORD);
	memcpy(_mem, run->visAttrs, run->glyphCount * sizeof(SCRIPT_VISATTR));//visAttrs
	run->visAttrs = _mem;
	next = run->next, prev = run->prev;
	newrun = realloc(run, sz);
	if (newrun == 0) {
		printf("realloc failed\n");
		return run;
	}
	if (newrun != run) {
		printf("realloc failed 2\n");
		newrun->next = next;
		newrun->prev = prev;
		if (next != 0)
			next->prev = newrun;
		if (prev != 0)
			prev->next = newrun;
	}
	_mem = (void*) newrun + sizeof(struct _w_textlayout_styleitem)
			+ newrun->length * sizeof(SCRIPT_LOGATTR);
	newrun->glyphs = _mem;
	_mem += glyphCount * sizeof(WORD);
	newrun->clusters = _mem;
	_mem += glyphCount * sizeof(WORD);
	newrun->visAttrs = _mem;
	_mem += glyphCount * sizeof(SCRIPT_VISATTR);
	newrun->advances = _mem;
	_mem += glyphCount * sizeof(int);
	newrun->goffsets = _mem;
	_mem += glyphCount * sizeof(GOFFSET);
	if (_W_TEXTLAYOUT(textlayout)->justify) {
		newrun->justify = _mem;
	}
	return newrun;
}

/*
 *  Merge styles ranges and script items
 */
void print_SCRIPT_ANALYSIS(SCRIPT_ANALYSIS *analysis) {
	printf("%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			analysis->eScript, analysis->s.uBidiLevel, analysis->fRTL,
			analysis->fLayoutRTL, analysis->fLinkBefore, analysis->fLinkAfter,
			analysis->fLogicalOrder, analysis->fNoGlyphIndex,
			analysis->s.fOverrideDirection, analysis->s.fInhibitSymSwap,
			analysis->s.fCharShape, analysis->s.fDigitSubstitute,
			analysis->s.fInhibitLigate, analysis->s.fDisplayZWG,
			analysis->s.fArabicNumContext, analysis->s.fGcpClusters,
			analysis->s.fReserved, analysis->s.fEngineReserved);
}
wresult w_textlayout_shape(w_textlayout *textlayout, HDC hdc,
		struct _w_textlayout_styleitem *run);
wresult w_textlayout_merge(w_textlayout *textlayout, SCRIPT_ITEM *items,
		int itemCount, HDC srcHdc) {
	WCHAR *segmentsText = _W_TEXTLAYOUT(textlayout)->text->text;
	int start = 0, end = _W_TEXTLAYOUT(textlayout)->text->length, itemIndex = 0,
			styleIndex = 0;
	int itemLimit = -1;
	int nextItemIndex = 0;
	boolean linkBefore = W_FALSE;
	boolean merge = itemCount > TOO_MANY_RUNS;
	const SCRIPT_PROPERTIES *sp, **ppSp;
	int piNumScripts;
	ScriptGetProperties(&ppSp, &piNumScripts);
	struct _w_textlayout_styleitem *_item, *prev = 0;
	struct _w_textlayout_style *style = _W_TEXTLAYOUT(textlayout)->styles,
			*_style;
	int _start, _lineBreak, _tab;
	SCRIPT_ANALYSIS _analysis;
	while (start < end) {
		_start = start;
		//printf("analysis origin:");
		//print_SCRIPT_ANALYSIS(&items[itemIndex].a);
		memcpy(&_analysis, &items[itemIndex].a, sizeof(SCRIPT_ANALYSIS));
		if (linkBefore) {
			_analysis.fLinkBefore = TRUE;
			linkBefore = FALSE;
		}
		wchar ch = segmentsText[start];
		_lineBreak = FALSE;
		_tab = FALSE;
		switch (ch) {
		case '\r':
		case '\n':
			_lineBreak = TRUE;
			break;
		case '\t':
			_tab = TRUE;
			break;
		}
		if (itemLimit == -1) {
			nextItemIndex = itemIndex + 1;
			itemLimit = items[nextItemIndex].iCharPos;
			if (nextItemIndex < itemCount && ch == '\r'
					&& segmentsText[itemLimit] == '\n') {
				nextItemIndex = itemIndex + 2;
				itemLimit = items[nextItemIndex].iCharPos;
			}
			if (nextItemIndex < itemCount && merge) {
				if (!_lineBreak) {
					sp = ppSp[_analysis.eScript];
					if (!sp->fComplex || _tab) {
						for (int i = 0; i < MERGE_MAX; i++) {
							if (nextItemIndex == itemCount)
								break;
							wchar c = segmentsText[itemLimit];
							if (c == '\n' || c == '\r')
								break;
							if (c == '\t' && c != _tab)
								break;
							sp = ppSp[items[nextItemIndex].a.eScript];
							if (!_tab && sp->fComplex)
								break;
							nextItemIndex++;
							itemLimit = items[nextItemIndex].iCharPos;
						}
					}
				}
			}
		}
		int styleLimit = -1;
		if (style != 0) {
			if (_start >= style->end) {
				style = style->next;
			}
			if (style != 0) {
				if (_start >= style->start) {
					_style = style;
					styleLimit = style->end;
				} else {
					_style = 0;
					styleLimit = style->start;
				}
			} else
				_style = 0;
		} else
			_style = 0;
		if (styleLimit == -1)
			styleLimit = end;
		if (styleLimit <= itemLimit) {
			styleIndex++;
			start = styleLimit;
			if (start < itemLimit && 0 < start && start < end) {
				wchar pChar = segmentsText[start - 1];
				wchar tChar = segmentsText[start];
				if (IsCharAlphaW(pChar) && IsCharAlphaW(tChar)) {
					_analysis.fLinkAfter = TRUE;
					linkBefore = TRUE;
				}
			}
		}
		if (itemLimit <= styleLimit) {
			itemIndex = nextItemIndex;
			start = itemLimit;
			itemLimit = -1;
		}

		_item = w_textlayout_styleitem_create(textlayout, prev, start - _start);
		if (_item == 0)
			return W_ERROR_NO_MEMORY;
		_item->start = _start;
		_item->style = _style;
		_item->lineBreak = _lineBreak;
		_item->tab = _tab;
		memcpy(&_item->analysis, &_analysis, sizeof(_analysis));
		_item->length = start - _start;
		prev = _item;
		SelectObject(srcHdc, _w_textlayout_get_item_font(textlayout, _item));
		w_textlayout_shape(textlayout, srcHdc, _item);
		printf("analysis       :");
		print_SCRIPT_ANALYSIS(&_item->analysis);
	}
	/*_item = w_textlayout_styleitem_create(textlayout, prev, 0);
	 _item->start = end;
	 _item->length = 0;
	 memcpy(&_item->analysis, &items[itemCount].a, sizeof(SCRIPT_ANALYSIS));*/
	return W_TRUE;
}
/*
 *  Itemize the receiver text
 */
wresult w_textlayout_itemize(w_textlayout *textlayout, HDC srcHdc) {
	if (_W_TEXTLAYOUT(textlayout)->text == 0)
		return W_FALSE;
	WCHAR *segmentsText = _W_TEXTLAYOUT(textlayout)->text->text;
	int length = _W_TEXTLAYOUT(textlayout)->text->length;
	SCRIPT_CONTROL scriptControl;
	SCRIPT_STATE scriptState;
	memset(&scriptState, 0, sizeof(scriptState));
	memset(&scriptControl, 0, sizeof(scriptControl));
	int MAX_ITEM = length + 1;

	if ((_w_textlayout_resolve_text_direction(textlayout) & W_RIGHT_TO_LEFT)
			!= 0) {
		scriptState.uBidiLevel = 1;
		scriptState.fArabicNumContext = TRUE;
	}

	/*
	 * In the version of Usp10.h that SWT is compiled the fReserved field is declared
	 * as a bitfield size 8. In newer versions of the Uniscribe, the first bit of fReserved
	 * was used to implement the fMergeNeutralItems feature which can be used to increase
	 * performance by reducing the number of SCRIPT_ITEM returned by ScriptItemize.
	 *
	 * Note: This code is wrong on a big endian machine.
	 *
	 * Note: This code is intentionally commented because it causes bug#377472.
	 */
//	if (!OS.IsWinCE && OS.WIN32_VERSION >= OS.VERSION (6, 0)) {
//		scriptControl.fReserved = 0x1;
//	}
	_W_TEXTLAYOUT(textlayout)->tmp_mem = malloc(MAX_ITEM * 2 * sizeof(void*));
	if (_W_TEXTLAYOUT(textlayout)->tmp_mem == 0) {
		return W_ERROR_NO_MEMORY;
	}
	int pcItems;
	SCRIPT_ITEM *pItems;
	pItems = (SCRIPT_ITEM*) _W_TEXTLAYOUT(textlayout)->tmp_mem;
	ScriptApplyDigitSubstitution(NULL, &scriptControl, &scriptState);

	HRESULT hr = ScriptItemize(segmentsText, length, MAX_ITEM, &scriptControl,
			&scriptState, pItems, &pcItems);
	if (hr != S_OK) {
		free(_W_TEXTLAYOUT(textlayout)->tmp_mem);
		_W_TEXTLAYOUT(textlayout)->tmp_mem = 0;
		if (hr == E_OUTOFMEMORY) {
			return W_ERROR_NO_MEMORY;
		} else
			return W_ERROR_UNSPECIFIED;
	}
	wresult result = w_textlayout_merge(textlayout, pItems, pcItems, srcHdc);
	if (result < 0)
		w_textlayout_free_runs(textlayout);
	return result;
}
wresult w_textlayout_shape_0(w_textlayout *textlayout, HDC hdc,
		struct _w_textlayout_styleitem *run, wchar *chars, int chars_length,
		const SCRIPT_PROPERTIES *sp) {
	int maxGlyphs = (run->length * 3 / 2) + 16;
	boolean useCMAPcheck = !sp->fComplex && !run->analysis.fNoGlyphIndex;
	if (useCMAPcheck) {
		if (ScriptGetCMap(hdc, &run->psc, chars, chars_length, 0,
				run->glyphs) != S_OK) {
			if (run->psc != 0) {
				ScriptFreeCache(&run->psc);
				run->glyphCount = 0;
			}
			return W_FALSE;
		}
	}
	HRESULT hr = ScriptShape(hdc, &run->psc, chars, chars_length, maxGlyphs,
			&run->analysis, run->glyphs, run->clusters, run->visAttrs,
			&run->glyphCount);
	if (useCMAPcheck)
		return W_TRUE;

	if (hr != USP_E_SCRIPT_NOT_IN_FONT) {
		if (run->analysis.fNoGlyphIndex)
			return W_TRUE;
		SCRIPT_FONTPROPERTIES fp;
		fp.cBytes = sizeof(SCRIPT_FONTPROPERTIES);
		ScriptGetFontProperties(hdc, &run->psc, &fp);
		int i;
		for (i = 0; i < run->glyphCount; i++) {
			if (run->glyphs[i] == fp.wgDefault)
				break;
		}
		if (i == run->glyphCount)
			return W_TRUE;
	}
	if (run->psc != 0) {
		ScriptFreeCache(&run->psc);
		run->glyphCount = 0;
		run->psc = 0;
	}
	run->glyphCount = 0;
	return W_FALSE;
}
void w_textlayout_break_run(w_textlayout *textlayout,
		struct _w_textlayout_styleitem *run) {
	if (run->psla != 0)
		return;
	WCHAR *segmentsText = _W_TEXTLAYOUT(textlayout)->text->text;
	wchar *chars = &segmentsText[run->start];
	int chars_length = run->length;
	run->psla = (void*) run + sizeof(struct _w_textlayout_styleitem);
	ScriptBreak(chars, chars_length, &run->analysis, run->psla);
}
int CALLBACK _w_textlayout_metaFileEnumProc(HDC hDC, HANDLETABLE *table,
CONST ENHMETARECORD *record, int nObj, LPARAM lpData) {
	EMREXTCREATEFONTINDIRECTW *emr = (EMREXTCREATEFONTINDIRECTW*) lpData;
	MoveMemory(&emr->emr, record, sizeof(EMR));
	switch (emr->emr.iType) {
	case EMR_EXTCREATEFONTINDIRECTW:
		MoveMemory(emr, record, sizeof(EMREXTCREATEFONTINDIRECTW));
		break;
	case EMR_EXTTEXTOUTW:
		return 0;
	}
	return 1;
}
/*
 * Generate glyphs for one Run.
 */
wresult w_textlayout_shape(w_textlayout *textlayout, HDC hdc,
		struct _w_textlayout_styleitem *run) {
	if (run->lineBreak)
		return W_TRUE;
	if (run->glyphs_created != 0)
		return W_TRUE;
	run->glyphs_created = TRUE;
	WCHAR *segmentsText = _W_TEXTLAYOUT(textlayout)->text->text;
	int buffer;
	wchar *chars = &segmentsText[run->start];
	int chars_length = run->length;

	int maxGlyphs = (run->length * 3 / 2) + 16;
	run->psc = 0;
	short script = run->analysis.eScript;
	const SCRIPT_PROPERTIES *sp, **ppSp;
	int piNumScripts;
	ScriptGetProperties(&ppSp, &piNumScripts);
	sp = ppSp[script];
	wresult shapeSucceed = w_textlayout_shape_0(textlayout, hdc, run, chars,
			chars_length, sp);
	if (!shapeSucceed) {
		if (sp->fPrivateUseArea) {
			run->analysis.fNoGlyphIndex = TRUE;
			shapeSucceed = w_textlayout_shape_0(textlayout, hdc, run, chars,
					chars_length, sp);
		}
	}
	if (!shapeSucceed) {
		HFONT hFont = GetCurrentObject(hdc, OBJ_FONT);
		HFONT newFont = 0;
		/*
		 * Bug in Uniscribe. In some version of Uniscribe, ScriptStringAnalyse crashes
		 * when the character array is too long. The fix is to limit the size of character
		 * array to two. Note, limiting the array to only one character would cause surrogate
		 * pairs to stop working.
		 */
		wchar sampleChars[3];		// = new char[Math.min(chars.length, 2)];
		int sampleChars_length = w_min(chars_length, 2);
		w_textlayout_break_run(textlayout, run);
		int count = 0;
		for (int i = 0; i < chars_length; i++) {
			if (!run->psla[i].fWhiteSpace) {
				sampleChars[count++] = chars[i];
				if (count == sampleChars_length)
					break;
			}
		}
		if (count > 0) {
			sampleChars[count] = 0;
			SCRIPT_STRING_ANALYSIS ssa;
			HDC metaFileDc = CreateEnhMetaFileW(hdc, NULL, NULL, NULL);
			HFONT oldMetaFont = SelectObject(metaFileDc, hFont);
			int flags = SSA_METAFILE | SSA_FALLBACK | SSA_GLYPHS | SSA_LINK;
			if (ScriptStringAnalyse(metaFileDc, &sampleChars, count, 0, -1,
					flags, 0,
					NULL, NULL, 0, 0, 0, &ssa) == S_OK) {
				ScriptStringOut(&ssa, 0, 0, 0, NULL, 0, 0, FALSE);
				ScriptStringFree(&ssa);
			}
			SelectObject(metaFileDc, oldMetaFont);
			HENHMETAFILE metaFile = CloseEnhMetaFile(metaFileDc);
			EMREXTCREATEFONTINDIRECTW emr;
			EnumEnhMetaFile(0, metaFile, _w_textlayout_metaFileEnumProc,
					(LPVOID) &emr,
					NULL);
			DeleteEnhMetaFile(metaFile);
			newFont = CreateFontIndirectW(&emr.elfw.elfLogFont);
		} else {
			/*
			 * The run is composed only by white spaces, this happens when a run is split
			 * by a visual style. The font fallback for the script can not be determined
			 * using only white spaces. The solution is to use the font of the previous
			 * or next run of the same script.
			 */
			struct _w_textlayout_styleitem *pRun = run->prev;
			if (pRun != 0) {
				if (pRun->analysis.eScript == run->analysis.eScript) {
					HFONT pFont = _w_textlayout_get_item_font(textlayout, pRun);
					LOGFONTW logFont;
					GetObjectW(pFont, sizeof(LOGFONTW), &logFont);
					newFont = CreateFontIndirectW(&logFont);
				}
			}
			if (newFont == 0) {
				struct _w_textlayout_styleitem *nRun = run->next;
				if (nRun != 0) {
					if (nRun->analysis.eScript == run->analysis.eScript) {
						SelectObject(hdc,
								_w_textlayout_get_item_font(textlayout, nRun));
						w_textlayout_shape(textlayout, hdc, nRun);
						HFONT nFont = _w_textlayout_get_item_font(textlayout,
								nRun);
						LOGFONTW logFont;
						GetObjectW(nFont, sizeof(LOGFONTW), &logFont);
						newFont = CreateFontIndirectW(&logFont);
					}
				}
			}
		}
		if (newFont != 0) {
			SelectObject(hdc, newFont);
			shapeSucceed = w_textlayout_shape_0(textlayout, hdc, run, chars,
					chars_length, sp);
			if (shapeSucceed) {
				run->fallbackFont = newFont;
			}
		}
		if (!shapeSucceed) {
			if (!sp->fComplex) {
				run->analysis.fNoGlyphIndex = TRUE;
				shapeSucceed = w_textlayout_shape_0(textlayout, hdc, run, chars,
						chars_length, sp);
				if (shapeSucceed) {
					run->fallbackFont = newFont;
				} else {
					run->analysis.fNoGlyphIndex = FALSE;
				}
			}
		}
		if (!shapeSucceed) {
			if (_W_TEXTLAYOUT(textlayout)->mLangFontLink2 == 0) {
				void *ppv = 0;
				if (CoCreateInstance(&_CLSID_CMultiLanguage, 0,
						CLSCTX_INPROC_SERVER, &_IID_IMLangFontLink2,
						&ppv) == S_OK) {
					_W_TEXTLAYOUT(textlayout)->mLangFontLink2 = ppv;
				} else
					return W_ERROR_NO_MEMORY;
			}
			if (_W_TEXTLAYOUT(textlayout)->mLangFontLink2 != 0) {
				IMLangFontLink2 *mLangFontLink2 =
				_W_TEXTLAYOUT(textlayout)->mLangFontLink2;
				HFONT hNewFont;
				DWORD dwCodePages;
				__LONG32 cchCodePages;
				/* GetStrCodePages() */
				mLangFontLink2->lpVtbl->GetStrCodePages(mLangFontLink2,
						&segmentsText[run->start], run->length, 0, &dwCodePages,
						&cchCodePages);
				/* MapFont() */
				if (mLangFontLink2->lpVtbl->MapFont(mLangFontLink2, hdc,
						dwCodePages, segmentsText[run->start],
						&hNewFont) == S_OK) {
					LOGFONTW logFont;
					GetObjectW(hNewFont, sizeof(LOGFONTW), &logFont);
					/* ReleaseFont() */
					mLangFontLink2->lpVtbl->ReleaseFont(mLangFontLink2,
							hNewFont);
					HFONT mLangFont = CreateFontIndirectW(&logFont);
					HFONT oldFont = SelectObject(hdc, mLangFont);
					shapeSucceed = w_textlayout_shape_0(textlayout, hdc, run,
							chars, chars_length, sp);
					if (shapeSucceed) {
						run->fallbackFont = mLangFont;
					} else {
						SelectObject(hdc, oldFont);
						DeleteObject(mLangFont);
					}
				}
			}
		}
		if (!shapeSucceed)
			SelectObject(hdc, hFont);
		if (newFont != 0 && newFont != run->fallbackFont)
			DeleteObject(newFont);
	}

	if (!shapeSucceed) {
		/*
		 * Shape Failed.
		 * Give up and shape the run with the default font.
		 * Missing glyphs typically will be represent as black boxes in the text.
		 */
		ScriptShape(hdc, &run->psc, chars, chars_length, maxGlyphs,
				&run->analysis, run->glyphs, run->clusters, run->visAttrs,
				&buffer);
		run->glyphCount = buffer;
	}
	run = w_textlayout_styleitem_compact(textlayout, run);
	ABC abc;
	ScriptPlace(hdc, &run->psc, run->glyphs, run->glyphCount, run->visAttrs,
			&run->analysis, run->advances, run->goffsets, &abc);
	run->width = abc.abcA + abc.abcB + abc.abcC;
	struct _w_textlayout_style *style = run->style;
	if (style != 0) {
		OUTLINETEXTMETRICW *lotm = 0, lotmt;
		if (style->style.underline || style->style.strikeout) {
			if (GetOutlineTextMetricsW(hdc, sizeof(OUTLINETEXTMETRICW), &lotmt)
					!= 0) {
				lotm = &lotmt;
			}
		}
		TEXTMETRICW *lptm = 0, lptmt;
		if (lotm != 0) {
			lptm = &lotm->otmTextMetrics;
		} else {
			GetTextMetricsW(hdc, &lptmt);
			lptm = &lptmt;
		}
		/*
		 *  Bug in Windows, on a Japanese machine, Uniscribe returns glyphcount
		 *  equals zero for FFFC (possibly other unicode code points), the fix
		 *  is to make sure the glyph is at least one pixel wide.
		 */
		if (style->style.metrics.width != (wushort) -1) {
			run->width = style->style.metrics.width * w_max(1, run->glyphCount);
			run->leadingInPoints = 0;
		} else {
			run->leadingInPoints = lptm->tmInternalLeading;
		}
		if (style->style.metrics.ascent != (wushort) -1) {
			run->ascentInPoints = style->style.metrics.ascent;
		} else {
			run->ascentInPoints = lptm->tmAscent;
		}
		if (style->style.metrics.descent != (wushort) -1) {
			run->descentInPoints = style->style.metrics.descent;
		} else {
			run->descentInPoints = lptm->tmDescent;
		}
		if (lotm != 0) {
			run->underlinePos = lotm->otmsUnderscorePosition;
			run->underlineThickness = w_max(1, lotm->otmsUnderscoreSize);
			run->strikeoutPos = lotm->otmsStrikeoutPosition;
			run->strikeoutThickness = w_max(1, lotm->otmsStrikeoutSize);
		} else {
			run->underlinePos = 1;
			run->underlineThickness = 1;
			run->strikeoutPos = run->ascentInPoints / 2;
			run->strikeoutThickness = 1;
		}
		run->ascentInPoints += style->style.rise;
		run->descentInPoints -= style->style.rise;
	} else {
		TEXTMETRICW lptm;
		GetTextMetricsW(hdc, &lptm);
		run->ascentInPoints = lptm.tmAscent;
		run->descentInPoints = lptm.tmDescent;
		run->leadingInPoints = lptm.tmInternalLeading;
	}
	return W_TRUE;
}

/*
 *  Reorder the run
 */
struct _w_textlayout_styleitem* w_textlayout_reorder(w_textlayout *textlayout,
		struct _w_textlayout_styleitem *runs, int length, boolean terminate,
		struct _w_textlayout_styleitem *lastRun) {
	if (length <= 1)
		return lastRun;
	BYTE *bidiLevels = _W_TEXTLAYOUT(textlayout)->tmp_mem
			+ (length + 1) * sizeof(int);
	struct _w_textlayout_styleitem *item = runs, *first, *last, **result;
	for (int i = 0; i < length; i++) {
		bidiLevels[i] = (byte) (item->analysis.s.uBidiLevel & 0x1F);
		item = item->next;
	}
	/*
	 * Feature in Windows.  If the orientation is RTL Uniscribe will
	 * resolve the level of line breaks to 1, this can cause the line
	 * break to be reorder to the middle of the line. The fix is to set
	 * the level to zero to prevent it to be reordered.
	 */
	if (lastRun->lineBreak && !lastRun->softBreak) {
		bidiLevels[length - 1] = 0;
	}
	int *log2vis = _W_TEXTLAYOUT(textlayout)->tmp_mem;
	ScriptLayout(length, bidiLevels, NULL, log2vis);
	result = _W_TEXTLAYOUT(textlayout)->tmp_mem + (length + 1) * sizeof(int);
	item = runs;
	for (int i = 0; i < length; i++) {
		result[log2vis[i]] = item;
		item = item->next;
	}
	if ((_W_TEXTLAYOUT(textlayout)->orientation & W_RIGHT_TO_LEFT) != 0) {
		int l = length;
		if (terminate)
			l--;
		for (int i = 0; i < l / 2; i++) {
			first = result[i];
			result[i] = result[l - i - 1];
			result[l - i - 1] = first;
		}
	}
	length--;
	first = runs->prev;
	last = lastRun->next;
	for (int i = 1; i < length/*length - 1*/; i++) {
		result[i]->next = result[i + 1];
		result[i]->prev = result[i - 1];
	}
	/*
	 * 0
	 */
	if (first != 0) {
		first->next = result[0];
	} else {
		_W_TEXTLAYOUT(textlayout)->allruns = result[0];
	}
	result[0]->next = result[1];
	result[0]->prev = first;
	/*
	 * length
	 */
	if (last != 0) {
		last->prev = result[length];
	} else {
		//nothing
	}
	result[length]->next = last;
	result[length]->prev = result[length - 1];
	return result[0];
}

int w_textlayout_get_line_indent(w_textlayout *textlayout, int lineIndex) {
	int lineIndent = _W_TEXTLAYOUT(textlayout)->wrapIndent;
	if (lineIndex == 0) {
		lineIndent = _W_TEXTLAYOUT(textlayout)->indent;
	} else {/*
	 StyleItem[] previousLine = runs[lineIndex - 1];
	 StyleItem previousRun = previousLine[previousLine.length - 1];
	 if (previousRun.lineBreak && !previousRun.softBreak) {
	 lineIndent = indent;
	 }*/
	}
	/*if (_W_TEXTLAYOUT(textlayout)->wrapWidth != -1) {
	 boolean partialLine = true;
	 if (_W_TEXTLAYOUT(textlayout)->justify) {
	 StyleItem[] lineRun = runs[lineIndex];
	 if (lineRun[lineRun.length - 1].softBreak) {
	 partialLine = false;
	 }
	 }
	 if (partialLine) {
	 int lineWidth = this.lineWidth[lineIndex] + lineIndent;
	 switch (alignment) {
	 case SWT.CENTER: lineIndent += (wrapWidth - lineWidth) / 2; break;
	 case SWT.RIGHT: lineIndent += wrapWidth - lineWidth; break;
	 }
	 }
	 }*/
	return lineIndent;
}
wresult w_textlayout_compute_runs(w_textlayout *textlayout, HDC hDC) {
	wresult result = W_TRUE;
	HDC srcHdc = hDC;
	if (srcHdc == 0)
		srcHdc = CreateCompatibleDC(NULL);
	result = w_textlayout_itemize(textlayout, srcHdc);
	if (result < 0) {
		w_textlayout_free_runs(textlayout);
		if (hDC == 0)
			DeleteDC(srcHdc);
		return result;
	}
	struct _w_textlayout_styleitem *run, *pRun, *fRun, *item, *newRun,
			*lineStartRun;
	int lineWidth = _W_TEXTLAYOUT(textlayout)->indent, lineCount = 1;
	//int i = 0,lineStart = 0;
	run = _W_TEXTLAYOUT(textlayout)->allruns;
	lineStartRun = run;
	const SCRIPT_PROPERTIES *properties, **ppSp;
	int piNumScripts;
	ScriptGetProperties(&ppSp, &piNumScripts);
	while (run != 0) {
		if (_W_TEXTLAYOUT(textlayout)->tabs != 0 && run->tab) {
			int tabsLength = _W_TEXTLAYOUT(textlayout)->tabs_length, j;
			int *tabs = _W_TEXTLAYOUT(textlayout)->tabs;
			for (j = 0; j < tabsLength; j++) {
				if (tabs[j] > lineWidth) {
					run->width = tabs[j] - lineWidth;
					break;
				}
			}
			if (j == tabsLength) {
				int tabX = tabs[tabsLength - 1];
				int lastTabWidth =
						tabsLength > 1 ?
								tabs[tabsLength - 1] - tabs[tabsLength - 2] :
								tabs[0];
				if (lastTabWidth > 0) {
					while (tabX <= lineWidth)
						tabX += lastTabWidth;
					run->width = tabX - lineWidth;
				}
			}
			int length = run->length;
			if (length > 1) {
				int stop = j + length - 1;
				if (stop < tabsLength) {
					run->width += tabs[stop] - tabs[j];
				} else {
					if (j < tabsLength) {
						run->width += tabs[tabsLength - 1] - tabs[j];
						length -= (tabsLength - 1) - j;
					}
					int lastTabWidth =
							tabsLength > 1 ?
									tabs[tabsLength - 1]
											- tabs[tabsLength - 2] :
									tabs[0];
					run->width += lastTabWidth * (length - 1);
				}
			}
		}
		if (_W_TEXTLAYOUT(textlayout)->wrapWidth != -1
				&& lineWidth + run->width > _W_TEXTLAYOUT(textlayout)->wrapWidth
				&& !run->tab && !run->lineBreak) {
			int start = 0;
			int *piDx = _W_TEXTLAYOUT(textlayout)->tmp_mem;
			if (run->style != 0
					&& (run->style->style.metrics.width != (wushort) -1)) {
				piDx[0] = run->width;
			} else {
				ScriptGetLogicalWidths(&run->analysis, run->length,
						run->glyphCount, run->advances, run->clusters,
						run->visAttrs, piDx);
			}
			int width = 0, maxWidth = _W_TEXTLAYOUT(textlayout)->wrapWidth
					- lineWidth;
			while (width + piDx[start] < maxWidth) {
				width += piDx[start++];
			}
			int firstStart = start;
			//int firstIndice = i;
			fRun = run;
			//wresult stop= W_FALSE;
			while (run->start >= lineStartRun->start) {
				w_textlayout_break_run(textlayout, run);
				while (start >= 0) {
					if (run->psla[start].fSoftBreak
							|| run->psla[start].fWhiteSpace)
						break;
					start--;
				}

				/*
				 *  Bug in Windows. For some reason Uniscribe sets the fSoftBreak flag for the first letter
				 *  after a letter with an accent. This cause a break line to be set in the middle of a word.
				 *  The fix is to detect the case and ignore fSoftBreak forcing the algorithm keep searching.
				 */
				if (start == 0 && run != lineStartRun && !run->tab) {
					if (run->psla[start].fSoftBreak
							&& !run->psla[start].fWhiteSpace) {
						int langID = ppSp[run->analysis.eScript]->langid;
						pRun = run->prev;
						if (ppSp[pRun->analysis.eScript]->langid
								== langID|| langID == LANG_NEUTRAL || ppSp[pRun->analysis.eScript]->langid == LANG_NEUTRAL) {
							w_textlayout_break_run(textlayout, pRun);
							if (!pRun->psla[pRun->length - 1].fWhiteSpace)
								start = -1;
						}
					}
				}
				if (start >= 0 || run == lineStartRun)
					break;
				run = run->prev;
				start = run->length - 1;
			}
			boolean wrapEntireRun = start == 0 && run != lineStartRun
					&& !run->tab;
			if (wrapEntireRun) {
				w_textlayout_break_run(textlayout, run);
				wrapEntireRun = !run->psla[start].fWhiteSpace;
			}
			if (wrapEntireRun) {
				run = run->prev;
				start = run->length;
			} else if (start <= 0 && run == lineStartRun) {
				/*
				 * No soft-break or line-feed found. Avoid breaking a run at
				 * the first character (firstStart == 0) unless it's the
				 * only run available (firstIndice == lineStart). See bug 408833.
				 */
				if (firstStart == 0
						&& /*firstIndice > lineStart*/fRun != lineStartRun) {
					/*i = firstIndice - 1;
					 run = allRuns[i];*/
					run = fRun->prev;
					start = run->length;
				} else {
					/*i = firstIndice;
					 run = allRuns[i];*/
					run = fRun;
					start = w_max(1, firstStart);
				}
			}
			w_textlayout_break_run(textlayout, run);
			while (start < run->length) {
				if (!run->psla[start].fWhiteSpace)
					break;
				start++;
			}
			if (0 < start && start < run->length) {
				int newRun_start = run->start + start;
				int newRun_length = run->length - start;
				struct _w_textlayout_style *newRun_style = run->style;
				if (run->psc != 0) {
					ScriptFreeCache(&run->psc);
				}
				if (run->fallbackFont != 0) {
					DeleteObject(run->fallbackFont);
				}
				run->psc = 0;
				run->fallbackFont = 0;
				run->psla = 0;
				run->glyphs = 0;
				run->clusters = 0;
				run->visAttrs = 0;
				run->advances = 0;
				run->goffsets = 0;
				run->justify = 0;
				run->length = start;
				SelectObject(srcHdc,
						_w_textlayout_get_item_font(textlayout, run));
				run->analysis.fNoGlyphIndex = FALSE;
				w_textlayout_shape(textlayout, srcHdc, run);
				newRun = w_textlayout_styleitem_create(textlayout, run,
						run->length - start);
				newRun->start = newRun_start;
				newRun->length = newRun_length;
				newRun->style = newRun_style;
				memcpy(&newRun->analysis, &run->analysis,
						sizeof(SCRIPT_ANALYSIS));
				SelectObject(srcHdc,
						_w_textlayout_get_item_font(textlayout, newRun));
				newRun->analysis.fNoGlyphIndex = FALSE;
				w_textlayout_shape(textlayout, srcHdc, newRun);
			}
			/*if (i != allRuns.length - 2) {
			 run.softBreak = run.lineBreak = true;
			 }*/
			if (run->next != 0) {
				run->softBreak = run->lineBreak = TRUE;
			}
		}
		lineWidth += run->width;
		if (run->lineBreak) {
			lineStartRun = run->next;
			lineWidth = run->softBreak ?
			_W_TEXTLAYOUT(textlayout)->wrapIndent :
											_W_TEXTLAYOUT(textlayout)->indent;
			lineCount++;
		}
		run = run->next;
	}
	_W_TEXTLAYOUT(textlayout)->lines = malloc(
			(lineCount + 1) * sizeof(struct _w_textlayout_line));
	if (_W_TEXTLAYOUT(textlayout)->lines == 0) {
		w_textlayout_free_runs(textlayout);
		return W_ERROR_NO_MEMORY;
	}
	_W_TEXTLAYOUT(textlayout)->text->lineCount = lineCount;
	struct _w_textlayout_line *lines = _W_TEXTLAYOUT(textlayout)->lines;
	memset(lines, 0, (lineCount + 1) * sizeof(struct _w_textlayout_line));
	run = _W_TEXTLAYOUT(textlayout)->allruns;
	lineWidth = 0;
	int lineRunCount = 0, line = 0;
	int ascentInPoints = w_max(0, _W_TEXTLAYOUT(textlayout)->ascentInPixels);
	int descentInPoints = w_max(0, _W_TEXTLAYOUT(textlayout)->descentInPixels);
	fRun = run;
	while (run != 0) {
		lineRunCount++;
		lineWidth += run->width;
		ascentInPoints = w_max(ascentInPoints, run->ascentInPoints);
		descentInPoints = w_max(descentInPoints, run->descentInPoints);
		if (run->lineBreak || run->next == 0) {
			/* Update the run metrics if the last run is a hard break. */
			if (lineRunCount == 1 && (run->next == 0 || !run->softBreak)) {
				TEXTMETRICW lptm;
				SelectObject(srcHdc,
						_w_textlayout_get_item_font(textlayout, run));
				GetTextMetricsW(srcHdc, &lptm);
				run->ascentInPoints = lptm.tmAscent;
				run->descentInPoints = lptm.tmDescent;
				ascentInPoints = w_max(ascentInPoints, run->ascentInPoints);
				descentInPoints = w_max(descentInPoints, run->descentInPoints);
			}

			lines[line].runs = fRun;
			lines[line].lineRunCount = lineRunCount;

			if (_W_TEXTLAYOUT(textlayout)->justify
					&& _W_TEXTLAYOUT(textlayout)->wrapWidth != -1
					&& run->softBreak && lineWidth > 0) {
				int lineIndent = _W_TEXTLAYOUT(textlayout)->wrapIndent;
				if (line == 0) {
					lineIndent = _W_TEXTLAYOUT(textlayout)->indent;
				} else {
					pRun = lines[line].runs->prev;
					if (pRun->lineBreak && !pRun->softBreak) {
						lineIndent = _W_TEXTLAYOUT(textlayout)->indent;
					}
				}
				lineWidth += lineIndent;
				int newLineWidth = 0;
				item = fRun;
				for (int j = 0; j < lineRunCount; j++) {
					int iDx = item->width * _W_TEXTLAYOUT(textlayout)->wrapWidth
							/ lineWidth;
					if (iDx != item->width) {
						ScriptJustify(item->visAttrs, item->advances,
								item->glyphCount, iDx - item->width, 2,
								item->justify);
						item->width = iDx;
					}
					newLineWidth += item->width;
					item = item->next;
				}
				lineWidth = newLineWidth;
			}
			lines[line].lineWidth = lineWidth;

			//StyleItem lastRun = runs[line][lineRunCount - 1];
			pRun = run;
			int lastOffset = pRun->start + pRun->length;
			pRun = w_textlayout_reorder(textlayout, lines[line].runs,
					lines[line].lineRunCount, run->next == 0, pRun);
			//lastRun = runs[line][lineRunCount - 1];
			if (run->softBreak && run != pRun) {
				run->softBreak = run->lineBreak = FALSE;
				pRun->softBreak = pRun->lineBreak = TRUE;
			}

			lineWidth = w_textlayout_get_line_indent(textlayout, line);
			item = fRun;
			for (int j = 0; j < lineRunCount; j++) {
				item->x = lineWidth;
				lineWidth += item->width;
				item = item->next;
				if (item == 0)
					break;
			}
			line++;
			lines[line].lineY = lines[line - 1].lineY + ascentInPoints
					+ descentInPoints
					+ _W_TEXTLAYOUT(textlayout)->lineSpacingInPoints;
			lines[line].lineOffset = lastOffset;
			lineRunCount = lineWidth = 0;
			ascentInPoints = w_max(0,
			_W_TEXTLAYOUT(textlayout)->ascentInPixels);
			descentInPoints = w_max(0,
			_W_TEXTLAYOUT(textlayout)->descentInPixels);
			fRun = run->next;
		}
		run = run->next;
	}

	free(_W_TEXTLAYOUT(textlayout)->tmp_mem);
	_W_TEXTLAYOUT(textlayout)->tmp_mem = 0;
	if (hDC == 0)
		DeleteDC(srcHdc);
	return result;
}
void w_textlayout_free_runs(w_textlayout *textlayout) {
	if (_W_TEXTLAYOUT(textlayout)->allruns == 0)
		return;
	struct _w_textlayout_styleitem *run = _W_TEXTLAYOUT(textlayout)->allruns,
			*tmp;
	if ( _W_TEXTLAYOUT(textlayout)->lines != 0) {
		free(_W_TEXTLAYOUT(textlayout)->lines);
	}
	while (run) {
		if (run->psc != 0) {
			ScriptFreeCache(&run->psc);
		}
		if (run->fallbackFont != 0) {
			DeleteObject(run->fallbackFont);
		}
		tmp = run;
		run = run->next;
		free(tmp);
	}
	if (_W_TEXTLAYOUT(textlayout)->tmp_mem != 0) {
		free(_W_TEXTLAYOUT(textlayout)->tmp_mem);
		_W_TEXTLAYOUT(textlayout)->tmp_mem = 0;
	}
	_W_TEXTLAYOUT(textlayout)->allruns = 0;
}
#endif
