/*
 * graphics.c
 *
 *  Created on: 11 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "graphics.h"
#include <gdiplus.h>
#include "../widgets/toolkit.h"

unsigned char LINE_DOT_ZERO[] = { 3, 3 };
unsigned char LINE_DASH_ZERO[] = { 18, 6 };
unsigned char LINE_DASHDOT_ZERO[] = { 9, 6, 3, 6 };
unsigned char LINE_DASHDOTDOT_ZERO[] = { 9, 3, 3, 3, 3, 3 };

int _cos(int angle, int length) {
	return (int) (cosf(angle * (M_PI / 180)) * length);
}

int _sin(int angle, int length) {
	return (int) (sinf(angle * (M_PI / 180)) * length);
}
wresult w_graphics_drawText(w_graphics *gc, GpGraphics *gdipGraphics,
		const char *string, size_t length, int x, int y, int flags,
		w_point *size);
void w_graphics_set_clipping_HRGN(w_graphics *gc, HRGN clipRgn);
void w_graphics_init(w_graphics *gc) {
	_W_GRAPHICS(gc)->handle = 0;
}
void w_graphics_dispose(w_graphics *gc) {
	if (gc != 0 && _W_GRAPHICS(gc)->handle != 0) {
		if (_W_GRAPHICS(gc)->state & W_GRAPHICS_GDIPRELEASEDC) {
			GdipReleaseDC(_W_GRAPHICS(gc)->gdipGraphics,
			_W_GRAPHICS(gc)->handle);
			_W_GRAPHICS(gc)->gdipGraphics = 0;
		}
		if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
			GdipDeleteGraphics(_W_GRAPHICS(gc)->gdipGraphics);
		}
		if (_W_GRAPHICS(gc)->state & W_GRAPHICS_RELEASEDC) {
			ReleaseDC(_W_GRAPHICS(gc)->hwnd, _W_GRAPHICS(gc)->handle);
		}
		if (_W_GRAPHICS(gc)->state & W_GRAPHICS_DELETEDC) {
			DeleteDC(_W_GRAPHICS(gc)->handle);
		}
		_W_GRAPHICS(gc)->handle = 0;
	}
}
void _w_graphics_init(w_graphics *gc, HDC handle) {
	memset(gc, 0, sizeof(struct _w_graphics));
	_W_GRAPHICS(gc)->handle = handle;
}
wresult w_graphics_create(w_graphics *gc) {
	w_graphics_dispose(gc);
	w_toolkit_get_platform();
	HDC hdc = GetDC(NULL);
	if (hdc == NULL)
		return W_ERROR_NO_HANDLES;
	_w_graphics_init(gc, hdc);
	return W_TRUE;
}
wresult w_graphics_is_ok(w_graphics *gc) {
	if (gc == 0)
		return W_FALSE;
	return _W_GRAPHICS(gc)->handle != 0;
}
GpMatrix* w_graphics_identity(w_graphics *gc) {
	GpMatrix *matrix = 0;
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
		int width = 0;
		int technology = GetDeviceCaps(_W_GRAPHICS(gc)->handle, TECHNOLOGY);
		if (technology == DT_RASPRINTER) {
			width = GetDeviceCaps(_W_GRAPHICS(gc)->handle, PHYSICALWIDTH);
		} else {
			if (_W_GRAPHICS(gc)->image != 0) {
				BITMAP bm;
				GetObjectW(_W_IMAGE(_W_GRAPHICS(gc)->image)->image, sizeof(bm),
						&bm);
				width = bm.bmWidth;
			} else {
				HWND hwnd = WindowFromDC(_W_GRAPHICS(gc)->handle);
				if (hwnd != 0) {
					RECT rect;
					GetClientRect(hwnd, &rect);
					width = rect.right - rect.left;
				} else {
					HBITMAP hBitmap = GetCurrentObject(_W_GRAPHICS(gc)->handle,
					OBJ_BITMAP);
					BITMAP bm;
					GetObjectW(hBitmap, sizeof(bm), &bm);
					width = bm.bmWidth;
				}
			}
		}
		POINT pt;
		GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
		GdipCreateMatrix2(-1, 0, 0, 1, width + 2 * pt.x, 0, &matrix);
		return matrix;
	}
	GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &matrix);
	return matrix;
}

wresult w_graphics_init_gdip(w_graphics *gc) {
	//data.device.checkGDIP();
	if (_W_GRAPHICS(gc)->gdipGraphics != 0)
		return W_TRUE;
	/*
	 * Feature in GDI+. The GDI+ clipping set with Graphics->SetClip()
	 * is always intersected with the GDI clipping at the time the
	 * GDI+ graphics is created.  This means that the clipping
	 * cannot be reset.  The fix is to clear the clipping before
	 * the GDI+ graphics is created and reset it afterwards.
	 */
	HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
	int result = GetClipRgn(_W_GRAPHICS(gc)->handle, hRgn);
	POINT pt;
	GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
	OffsetRgn(hRgn, pt.x, pt.y);
	SelectClipRgn(_W_GRAPHICS(gc)->handle, 0);

	/*
	 * Bug in GDI+.  GDI+ does not work when the HDC layout is RTL.  There
	 * are many issues like pixel corruption, but the most visible problem
	 * is that it does not have an effect when drawing to an bitmap.  The
	 * fix is to clear the bit before creating the GDI+ graphics and install
	 * a mirroring matrix ourselves.
	 */
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
		SetLayout(_W_GRAPHICS(gc)->handle,
				GetLayout(_W_GRAPHICS(gc)->handle) & ~LAYOUT_RTL);
	}
	GdipCreateFromHDC(_W_GRAPHICS(gc)->handle, &_W_GRAPHICS(gc)->gdipGraphics);
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		GdipSetPageUnit(_W_GRAPHICS(gc)->gdipGraphics, UnitPixel);
		GdipSetPixelOffsetMode(_W_GRAPHICS(gc)->gdipGraphics,
				PixelOffsetModeHalf);
		if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {

			GpMatrix *matrix = w_graphics_identity(gc);
			GdipSetWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, matrix);
			GdipDeleteMatrix(matrix);
		}
		if (result == 1)
			w_graphics_set_clipping_HRGN(gc, hRgn);
		DeleteObject(hRgn);
		_W_GRAPHICS(gc)->state = 0;
		if (_W_GRAPHICS(gc)->hPen != 0) {
			SelectObject(_W_GRAPHICS(gc)->handle, GetStockObject(NULL_PEN));
			DeleteObject(_W_GRAPHICS(gc)->hPen);
			_W_GRAPHICS(gc)->hPen = 0;
		}
		if (_W_GRAPHICS(gc)->hBrush != 0) {
			SelectObject(_W_GRAPHICS(gc)->handle, GetStockObject(NULL_BRUSH));
			DeleteObject(_W_GRAPHICS(gc)->hBrush);
			_W_GRAPHICS(gc)->hBrush = 0;
		}
		return W_TRUE;
	} else {
		DeleteObject(hRgn);
		return W_ERROR_NO_HANDLES;
	}
}
GpFont* w_graphics_create_gdip_font(HDC hDC, HFONT hFont, GpGraphics *graphics,
		long fontCollection, GpFontFamily **outFamily, HFONT *outFont) {
	GpStatus status;
	LOGFONTW logFont;
	GetObjectW(hFont, sizeof(logFont), &logFont);
	GpFont *font = 0;
	GdipCreateFontFromLogfontW(hDC, &logFont, &font);
	if (font == 0)
		return 0;
	GpFontFamily *family = 0;
	/*if (!Gdip.Font_IsAvailable(font)) {
	 Gdip.Font_delete(font);
	 LOGFONT logFont = OS.IsUnicode ? (LOGFONT)new LOGFONTW() : new LOGFONTA();
	 OS.GetObject(hFont, LOGFONT.sizeof, logFont);
	 int size = Math.abs(logFont.lfHeight);
	 int style = Gdip.FontStyleRegular;
	 if (logFont.lfWeight == 700) style |= Gdip.FontStyleBold;
	 if (logFont.lfItalic != 0) style |= Gdip.FontStyleItalic;
	 char[] chars;
	 if (OS.IsUnicode) {
	 chars = ((LOGFONTW)logFont).lfFaceName;
	 } else {
	 chars = new char[OS.LF_FACESIZE];
	 byte[] bytes = ((LOGFONTA)logFont).lfFaceName;
	 OS.MultiByteToWideChar (OS.CP_ACP, OS.MB_PRECOMPOSED, bytes, bytes.length, chars, chars.length);
	 }
	 int index = 0;
	 while (index < chars.length) {
	 if (chars [index] == 0) break;
	 index++;
	 }
	 String name = new String (chars, 0, index);
	 if (name.equalsIgnoreCase("Courier")) { //$NON-NLS-1$
	 name = "Courier New"; //$NON-NLS-1$
	 }
	 char[] buffer = new char[name.length() + 1];
	 name.getChars(0, name.length(), buffer, 0);
	 if (fontCollection != 0) {
	 family = Gdip.FontFamily_new(buffer, fontCollection);
	 if (!Gdip.FontFamily_IsAvailable(family)) {
	 Gdip.FontFamily_delete(family);
	 family = Gdip.FontFamily_new(buffer, 0);
	 if (!Gdip.FontFamily_IsAvailable(family)) {
	 Gdip.FontFamily_delete(family);
	 family = 0;
	 }
	 }
	 }
	 if (family != 0) {
	 font = Gdip.Font_new(family, size, style, Gdip.UnitPixel);
	 } else {
	 font = Gdip.Font_new(buffer, size, style, Gdip.UnitPixel, 0);
	 }
	 if (outFont != null && font != 0) {
	 long  hHeap = OS.GetProcessHeap();
	 long  pLogFont = OS.HeapAlloc(hHeap, OS.HEAP_ZERO_MEMORY, LOGFONTW.sizeof);
	 Gdip.Font_GetLogFontW(font, graphics, pLogFont);
	 outFont[0] = OS.CreateFontIndirectW(pLogFont);
	 OS.HeapFree(hHeap, 0, pLogFont);
	 }
	 }*/
	if (outFamily != 0 && font != 0) {
		if (family == 0) {
			GdipGetFamily(font, &family);
		}
		outFamily[0] = family;
	} else {
		if (family != 0)
			GdipDeleteFontFamily(family);
	}
	//if (font == 0) SWT.error(SWT.ERROR_NO_HANDLES);
	return font;
}
wresult w_graphics_check(w_graphics *gc, int mask) {
	if (gc == 0 || _W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	int state = _W_GRAPHICS(gc)->state;
	if ((state & mask) == mask)
		return 1;
	state = (state ^ mask) & mask;
	_W_GRAPHICS(gc)->state |= mask;
	GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
	if (gdipGraphics != 0) {
		GpPen *pen = _W_GRAPHICS(gc)->gdipPen;
		float width = _W_GRAPHICS(gc)->line.width;
		if ((state & W_GRAPHICS_FOREGROUND) != 0
				|| (pen == 0
						&& (state
								& (W_GRAPHICS_LINE_WIDTH | W_GRAPHICS_LINE_STYLE
										| W_GRAPHICS_LINE_MITERLIMIT
										| W_GRAPHICS_LINE_JOIN
										| W_GRAPHICS_LINE_CAP)) != 0)) {
			if (_W_GRAPHICS(gc)->gdipFgBrush != 0)
				GdipDeleteBrush(_W_GRAPHICS(gc)->gdipFgBrush);
			_W_GRAPHICS(gc)->gdipFgBrush = 0;
			GpSolidFill *brush = 0;
			w_pattern *pattern = _W_GRAPHICS(gc)->foregroundPattern;
			if (pattern != 0) {
				brush = _W_PATTERN(pattern)->handle;
				if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
					GpBrushType type = BrushTypeSolidColor;
					GpBrush *newbrush;
					if (brush != 0) {
						GdipGetBrushType(brush, &type);
						if (type == BrushTypeTextureFill) {
							newbrush = 0;
							GdipCloneBrush((GpBrush*) brush,
									(GpBrush**) &newbrush);
							if (newbrush == 0)
								return W_ERROR_NO_HANDLES;
							brush = newbrush;
							GdipScaleTextureTransform(brush, -1, 1,
									MatrixOrderPrepend);
							_W_GRAPHICS(gc)->gdipFgBrush = brush;
						}
					}
				}
			} else {
				int foreground = _W_GRAPHICS(gc)->foreground;
				int rgb = ((foreground >> 16) & 0xFF) | (foreground & 0xFF00)
						| ((foreground & 0xFF) << 16);
				long color = _W_GRAPHICS(gc)->alpha << 24 | rgb;
				GdipCreateSolidFill(color, &brush);
				_W_GRAPHICS(gc)->gdipFgBrush = brush;
			}
			if (pen != 0) {
				GdipSetPenBrushFill(pen, brush);
			} else {
				GdipCreatePen2(brush, width, UnitWorld, &pen);
				_W_GRAPHICS(gc)->gdipPen = pen;
			}
		}
		if ((state & W_GRAPHICS_LINE_WIDTH) != 0) {
			GdipSetPenWidth(pen, width);
			switch (_W_GRAPHICS(gc)->line.style) {
			case W_LINE_CUSTOM:
				state |= W_GRAPHICS_LINE_STYLE;
			}
		}
		if ((state & W_GRAPHICS_LINE_STYLE) != 0) {
			//float dashesOffset = 0;
			float *dashes = 0;
			int dashes_length;
			float width = _W_GRAPHICS(gc)->line.width;
			int is_custom_dash = W_FALSE;
			//float[] dashes = null;
			float dashOffset = 0;
			DashStyle dashStyle = DashStyleSolid;
			switch (_W_GRAPHICS(gc)->line.style) {
			case W_LINE_SOLID:
				break;
			case W_LINE_DOT:
				dashStyle = DashStyleDot;
				if (width == 0) {
					dashes_length = sizeof(LINE_DOT_ZERO)
							/ sizeof(LINE_DOT_ZERO[0]);
					dashes = (float*) LINE_DOT_ZERO;
				}
				break;
			case W_LINE_DASH:
				dashStyle = DashStyleDash;
				if (width == 0) {

					dashes_length = sizeof(LINE_DASH_ZERO)
							/ sizeof(LINE_DASH_ZERO[0]);
					dashes = (float*) LINE_DASH_ZERO;
				}
				break;
			case W_LINE_DASHDOT:
				dashStyle = DashStyleDashDot;
				if (width == 0) {
					dashes_length = sizeof(LINE_DASHDOT_ZERO)
							/ sizeof(LINE_DASHDOT_ZERO[0]);
					dashes = (float*) LINE_DASHDOT_ZERO;
				}
				break;
			case W_LINE_DASHDOTDOT:
				dashStyle = DashStyleDashDotDot;
				if (width == 0) {
					dashes_length = sizeof(LINE_DASHDOTDOT_ZERO)
							/ sizeof(LINE_DASHDOTDOT_ZERO[0]);
					dashes = (float*) LINE_DASHDOTDOT_ZERO;
				}
				break;
			case W_LINE_CUSTOM: {
				if (_W_GRAPHICS(gc)->line.dash != 0) {
					is_custom_dash = W_TRUE;
					int m = width;
					if (m < 1)
						m = 1;
					dashOffset = _W_GRAPHICS(gc)->line.dashOffset / m;
					dashes_length = _W_GRAPHICS(gc)->line.dash->length * 2;
					dashes = malloc(
					_W_GRAPHICS(gc)->line.dash->length * 2 * sizeof(float));
					for (int i = 0; i < _W_GRAPHICS(gc)->line.dash->length;
							i++) {
						float dash = _W_GRAPHICS(gc)->line.dash->dash[i] / m;
						dashes[i] = dash;
						dashes[i + _W_GRAPHICS(gc)->line.dash->length] = dash;
					}
				}
			}
			}
			if (dashes != 0) {
				REAL *_dashes;
				if (is_custom_dash) {
					_dashes = dashes;
				} else {
					REAL dashesArray[6];
					for (int i = 0; i < dashes_length; i++) {
						dashesArray[i] = ((unsigned char*) dashes)[i];
					}
					_dashes = dashesArray;
				}
				GdipSetPenDashArray(pen, _dashes, dashes_length);
				GdipSetPenDashStyle(pen, DashStyleCustom);
				GdipSetPenDashOffset(pen, dashOffset);
				if (is_custom_dash) {
					free(dashes);
				}
			} else {
				GdipSetPenDashStyle(pen, dashStyle);
			}
		}
		if ((state & W_GRAPHICS_LINE_MITERLIMIT) != 0) {
			GdipSetPenMiterLimit(pen,
			_W_GRAPHICS(gc)->line.miterLimit);
		}
		if ((state & W_GRAPHICS_LINE_JOIN) != 0) {
			LineJoin joinStyle = LineJoinMiter;
			switch (_W_GRAPHICS(gc)->line.join) {
			case W_JOIN_MITER:
				joinStyle = LineJoinMiter;
				break;
			case W_JOIN_BEVEL:
				joinStyle = LineJoinBevel;
				break;
			case W_JOIN_ROUND:
				joinStyle = LineJoinRound;
				break;
			}
			GdipSetPenLineJoin(pen, joinStyle);
		}
		if ((state & W_GRAPHICS_LINE_CAP) != 0) {
			LineCap capStyle = LineCapFlat;
			DashCap dashCap = DashCapFlat;
			switch (_W_GRAPHICS(gc)->line.cap) {
			case W_CAP_FLAT:
				capStyle = LineCapFlat;
				break;
			case W_CAP_ROUND:
				capStyle = LineCapRound;
				dashCap = DashCapRound;
				break;
			case W_CAP_SQUARE:
				capStyle = LineCapSquare;
				break;
			}
			GdipSetPenLineCap197819(pen, capStyle, capStyle, dashCap);
		}
		if ((state & W_GRAPHICS_BACKGROUND) != 0) {
			if (_W_GRAPHICS(gc)->gdipBgBrush != 0) {
				GdipDeleteBrush(_W_GRAPHICS(gc)->gdipBgBrush);
			}
			_W_GRAPHICS(gc)->gdipBgBrush = 0;
			w_pattern *pattern = _W_GRAPHICS(gc)->backgroundPattern;
			if (pattern != 0) {
				_W_GRAPHICS(gc)->gdipBrush = _W_PATTERN(pattern)->handle;
				if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
					GpBrushType type = BrushTypeSolidColor;
					GdipGetBrushType(_W_GRAPHICS(gc)->gdipBrush, &type);
					GpBrush *brush;
					if (type == BrushTypeTextureFill) {
						brush = 0;
						GdipCloneBrush(_W_GRAPHICS(gc)->gdipBrush, &brush);
						if (brush == 0)
							return W_ERROR_NO_HANDLES;
						GdipScaleTextureTransform(brush, -1, 1,
								MatrixOrderPrepend);
						_W_GRAPHICS(gc)->gdipBrush =
						_W_GRAPHICS(gc)->gdipBgBrush = brush;
					}
				}
			} else {
				GpSolidFill *brush = 0;
				int background = _W_GRAPHICS(gc)->background;
				int rgb = ((background >> 16) & 0xFF) | (background & 0xFF00)
						| ((background & 0xFF) << 16);
				int color = _W_GRAPHICS(gc)->alpha << 24 | rgb;
				GdipCreateSolidFill(color, &brush);
				_W_GRAPHICS(gc)->gdipBrush = _W_GRAPHICS(gc)->gdipBgBrush =
						brush;
			}
		}
		if ((state & W_GRAPHICS_FONT) != 0) {
			w_font *font = _W_GRAPHICS(gc)->font;
			if (font != 0) {
				SelectObject(_W_GRAPHICS(gc)->handle, _W_FONT(font)->handle);
				HFONT hFont = 0;
				GpFont *gdipFont = w_graphics_create_gdip_font(
				_W_GRAPHICS(gc)->handle, _W_FONT(font)->handle,
				_W_GRAPHICS(gc)->gdipGraphics, /*device.fontCollection*/
				0, 0, &hFont);
				if (hFont != 0)
					SelectObject(_W_GRAPHICS(gc)->handle, hFont);
				if (_W_GRAPHICS(gc)->hGDIFont != 0)
					DeleteObject(_W_GRAPHICS(gc)->hGDIFont);
				_W_GRAPHICS(gc)->hGDIFont = hFont;
				if (_W_GRAPHICS(gc)->gdipFont != 0)
					GdipDeleteFont(_W_GRAPHICS(gc)->gdipFont);
				_W_GRAPHICS(gc)->gdipFont = gdipFont;
			}
		}
		if ((state & W_GRAPHICS_DRAW_OFFSET) != 0) {
			_W_GRAPHICS(gc)->gdipXOffset = _W_GRAPHICS(gc)->gdipYOffset = 0;
			GpMatrix *matrix = 0;
			PointF point;
			point.X = point.Y = 1;
			GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &matrix);
			if (matrix != 0) {
				GdipGetWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, matrix);
				GdipTransformMatrixPoints(matrix, &point, 1);
				GdipDeleteMatrix(matrix);
			}
			float scaling = point.X;
			if (scaling < 0)
				scaling = -scaling;
			float penWidth = _W_GRAPHICS(gc)->line.width * scaling;
			if (penWidth == 0 || ((int) penWidth % 2) == 1) {
				_W_GRAPHICS(gc)->gdipXOffset = 0.5 / scaling;
			}
			scaling = point.Y;
			if (scaling < 0)
				scaling = -scaling;
			penWidth = _W_GRAPHICS(gc)->line.width * scaling;
			if (penWidth == 0 || ((int) penWidth % 2) == 1) {
				_W_GRAPHICS(gc)->gdipYOffset = 0.5 / scaling;
			}
		}
		return 1;
	}
	if ((state
			& (W_GRAPHICS_FOREGROUND | W_GRAPHICS_LINE_CAP
					| W_GRAPHICS_LINE_JOIN | W_GRAPHICS_LINE_STYLE
					| W_GRAPHICS_LINE_WIDTH)) != 0) {
		int color = _W_GRAPHICS(gc)->foreground;
		int width = (int) _W_GRAPHICS(gc)->line.width;
		DWORD *dashes = 0;
		int dashes_length = 0;
		int lineStyle = PS_SOLID;
		switch (_W_GRAPHICS(gc)->line.style) {
		case W_LINE_SOLID:
			break;
		case W_LINE_DASH:
			lineStyle = PS_DASH;
			break;
		case W_LINE_DOT:
			lineStyle = PS_DOT;
			break;
		case W_LINE_DASHDOT:
			lineStyle = PS_DASHDOT;
			break;
		case W_LINE_DASHDOTDOT:
			lineStyle = PS_DASHDOTDOT;
			break;
		case W_LINE_CUSTOM: {
			if (_W_GRAPHICS(gc)->line.dash != 0) {
				lineStyle = PS_USERSTYLE;
				dashes_length = _W_GRAPHICS(gc)->line.dash->length;
				dashes = malloc(dashes_length * sizeof(DWORD));
				for (int i = 0; i < dashes_length; i++) {
					dashes[i] = (int) _W_GRAPHICS(gc)->line.dash->dash[i];
				}
			}
			break;
		}
		}
		if ((state & W_GRAPHICS_LINE_STYLE) != 0) {
			SetBkMode(_W_GRAPHICS(gc)->handle,
			_W_GRAPHICS(gc)->line.style == W_LINE_SOLID ? OPAQUE : TRANSPARENT);
		}
		int joinStyle = 0;
		switch (_W_GRAPHICS(gc)->line.join) {
		case W_JOIN_MITER:
			joinStyle = PS_JOIN_MITER;
			break;
		case W_JOIN_ROUND:
			joinStyle = PS_JOIN_ROUND;
			break;
		case W_JOIN_BEVEL:
			joinStyle = PS_JOIN_BEVEL;
			break;
		}
		int capStyle = 0;
		switch (_W_GRAPHICS(gc)->line.cap) {
		case W_CAP_ROUND:
			capStyle = PS_ENDCAP_ROUND;
			break;
		case W_CAP_FLAT:
			capStyle = PS_ENDCAP_FLAT;
			break;
		case W_CAP_SQUARE:
			capStyle = PS_ENDCAP_SQUARE;
			break;
		}
		int style = lineStyle | joinStyle | capStyle;
		/*
		 * Feature in Windows.  Windows does not honour line styles other then
		 * PS_SOLID for pens wider than 1 pixel created with CreatePen().  The fix
		 * is to use ExtCreatePen() instead.
		 */
		HPEN newPen;
		LOGBRUSH logBrush;
		logBrush.lbStyle = BS_SOLID;
		logBrush.lbColor = color & 0x00FFFFFF;
		logBrush.lbHatch = 0;
		/* Feature in Windows. PS_GEOMETRIC pens cannot have zero width. */
		int m = width;
		if (m < 1)
			m = 1;
		newPen = ExtCreatePen(style | PS_GEOMETRIC, m, &logBrush,
				dashes != 0 ? dashes_length : 0, dashes);
		SelectObject(_W_GRAPHICS(gc)->handle, newPen);
		_W_GRAPHICS(gc)->state |= W_GRAPHICS_PEN;
		_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_NULL_PEN;
		if (_W_GRAPHICS(gc)->hPen != 0)
			DeleteObject(_W_GRAPHICS(gc)->hPen);
		_W_GRAPHICS(gc)->hPen = _W_GRAPHICS(gc)->hOldPen = newPen;
		if (dashes != 0)
			free(dashes);
	} else if ((state & W_GRAPHICS_PEN) != 0) {
		SelectObject(_W_GRAPHICS(gc)->handle, _W_GRAPHICS(gc)->hOldPen);
		_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_NULL_PEN;
	} else if ((state & W_GRAPHICS_NULL_PEN) != 0) {
		_W_GRAPHICS(gc)->hOldPen = (HPEN) SelectObject(_W_GRAPHICS(gc)->handle,
				GetStockObject(NULL_PEN));
		_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_PEN;
	}
	if ((state & W_GRAPHICS_BACKGROUND) != 0) {
		HBRUSH newBrush = CreateSolidBrush(
		_W_GRAPHICS(gc)->background & 0x00FFFFFF);
		SelectObject(_W_GRAPHICS(gc)->handle, newBrush);
		_W_GRAPHICS(gc)->state |= W_GRAPHICS_BRUSH;
		_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_NULL_BRUSH;
		if (_W_GRAPHICS(gc)->hBrush != 0)
			DeleteObject(_W_GRAPHICS(gc)->hBrush);
		_W_GRAPHICS(gc)->hOldBrush = _W_GRAPHICS(gc)->hBrush = newBrush;
	} else if ((state & W_GRAPHICS_BRUSH) != 0) {
		SelectObject(_W_GRAPHICS(gc)->handle, _W_GRAPHICS(gc)->hOldBrush);
		_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_NULL_BRUSH;
	} else if ((state & W_GRAPHICS_NULL_BRUSH) != 0) {
		_W_GRAPHICS(gc)->hOldBrush = (HBRUSH) SelectObject(
		_W_GRAPHICS(gc)->handle, GetStockObject(NULL_BRUSH));
		_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_BRUSH;
	}
	if ((state & W_GRAPHICS_BACKGROUND_TEXT) != 0) {
		SetBkColor(_W_GRAPHICS(gc)->handle,
		_W_GRAPHICS(gc)->background & 0x00FFFFFF);
	}
	if ((state & W_GRAPHICS_FOREGROUND_TEXT) != 0) {
		SetTextColor(_W_GRAPHICS(gc)->handle,
		_W_GRAPHICS(gc)->foreground & 0x00FFFFFF);
	}
	if ((state & W_GRAPHICS_FONT) != 0) {
		w_font *font = _W_GRAPHICS(gc)->font;
		if (font != 0) {
			SelectObject(_W_GRAPHICS(gc)->handle, _W_FONT(font)->handle);
		}
	}
	return 1;
}
wresult w_graphics_copy_area_to_image(w_graphics *gc,
		w_surface *surface, int x, int y){
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (image == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	 if (image.type != SWT.BITMAP || image.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);*/
	/* Copy the bitmap area */
	/*Rectangle rect = image.getBoundsInPixels();
	 long memHdc = OS.CreateCompatibleDC(handle);
	 long hOldBitmap = OS.SelectObject(memHdc, image.handle);
	 OS.BitBlt(memHdc, 0, 0, rect.width, rect.height, handle, x, y, OS.SRCCOPY);
	 OS.SelectObject(memHdc, hOldBitmap);
	 OS.DeleteDC(memHdc);*/
}
wresult w_graphics_copy_area(w_graphics *gc, int srcX, int srcY, int width,
		int height, int destX, int destY, wresult paint) {
	//if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	/*
	 * Feature in WinCE.  The function WindowFromDC is not part of the
	 * WinCE SDK.  The fix is to remember the HWND.
	 */
	if (_W_GRAPHICS(gc)->hwnd == 0) {
		//OS.BitBlt(handle, destX, destY, width, height, handle, srcX, srcY, OS.SRCCOPY);
	} else {
		/*RECT lprcClip = null;
		 long hrgn = OS.CreateRectRgn(0, 0, 0, 0);
		 if (OS.GetClipRgn(handle, hrgn) == 1) {
		 lprcClip = new RECT();
		 OS.GetRgnBox(hrgn, lprcClip);
		 }
		 OS.DeleteObject(hrgn);
		 RECT lprcScroll = new RECT();
		 OS.SetRect(lprcScroll, srcX, srcY, srcX + width, srcY + height);
		 int flags = paint ? OS.SW_INVALIDATE | OS.SW_ERASE : 0;
		 int res = OS.ScrollWindowEx(hwnd, destX - srcX, destY - srcY, lprcScroll, lprcClip, 0, null, flags);
		 */
		/*
		 * Feature in WinCE.  ScrollWindowEx does not accept combined
		 * vertical and horizontal scrolling.  The fix is to do a
		 * BitBlt and invalidate the appropriate source area.
		 */
		/*if (res == 0 && OS.IsWinCE) {
		 OS.BitBlt(handle, destX, destY, width, height, handle, srcX, srcY, OS.SRCCOPY);
		 if (paint) {
		 int deltaX = destX - srcX, deltaY = destY - srcY;
		 boolean disjoint = (destX + width < srcX) || (srcX + width < destX) || (destY + height < srcY) || (srcY + height < destY);
		 if (disjoint) {
		 OS.InvalidateRect(hwnd, lprcScroll, true);
		 } else {
		 if (deltaX != 0) {
		 int newX = destX - deltaX;
		 if (deltaX < 0) newX = destX + width;
		 OS.SetRect(lprcScroll, newX, srcY, newX + Math.abs(deltaX), srcY + height);
		 OS.InvalidateRect(hwnd, lprcScroll, true);
		 }
		 if (deltaY != 0) {
		 int newY = destY - deltaY;
		 if (deltaY < 0) newY = destY + height;
		 OS.SetRect(lprcScroll, srcX, newY, srcX + width, newY + Math.abs(deltaY));
		 OS.InvalidateRect(hwnd, lprcScroll, true);
		 }
		 }
		 }
		 }*/
	}
}
wresult w_graphics_draw_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_DRAW);
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
		return result;
	GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
	if (gdipGraphics != 0) {
		GdipTranslateWorldTransform(gdipGraphics,
		_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		if (width == height) {
			GdipDrawArc(gdipGraphics, _W_GRAPHICS(gc)->gdipPen, x, y, width,
					height, -startAngle, -arcAngle);
		} else {
			GpPath *path = 0;
			GdipCreatePath(FillModeAlternate, &path);
			if (path != 0) {
				GpMatrix *matrix = 0;
				GdipCreateMatrix2(width, 0, 0, height, x, y, &matrix);
				if (matrix != 0) {
					GdipAddPathArc(path, 0, 0, 1, 1, -startAngle, -arcAngle);
					GdipTransformPath(path, matrix);
					GdipDrawPath(_W_GRAPHICS(gc)->gdipGraphics,
					_W_GRAPHICS(gc)->gdipPen, path);
					GdipDeleteMatrix(matrix);
				}
				GdipDeletePath(path);
			}
		}
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
				-_W_GRAPHICS(gc)->gdipXOffset, -_W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		return result;
	}
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
		if (_W_GRAPHICS(gc)->line.width != 0
				&& ((int) _W_GRAPHICS(gc)->line.width) % 2 == 0)
			x--;
	}
#if IsWinCE
	/*
	 * Feature in WinCE.  The function Arc is not present in the
	 * WinCE SDK.  The fix is to emulate arc drawing by using
	 * Polyline.
	 */
		/* compute arc with a simple linear interpolation */
		if (arcAngle < 0) {
			startAngle += arcAngle;
			arcAngle = -arcAngle;
		}
		if (arcAngle > 360)
			arcAngle = 360;

		int cteX = 2 * x + width;
		int cteY = 2 * y + height;
		int points_length = arcAngle + 1;
		POINT points[arcAngle + 1];
		for (int i = 0; i <= arcAngle; i++) {
			points[i].x = (cos(startAngle + i, width) + cteX) >> 1;
			points[i].y = (cteY - sin(startAngle + i, height)) >> 1;
		}
		Polyline(data->handle, points, points_length);
#else
	int x1, y1, x2, y2, tmp;
	int isNegative;
	if (arcAngle >= 360 || arcAngle <= -360) {
		x1 = x2 = x + width;
		y1 = y2 = y + height / 2;
	} else {
		isNegative = arcAngle < 0;

		arcAngle = arcAngle + startAngle;
		if (isNegative) {
			// swap angles
			tmp = startAngle;
			startAngle = arcAngle;
			arcAngle = tmp;
		}
		x1 = _cos(startAngle, width) + x + width / 2;
		y1 = -1 * _sin(startAngle, height) + y + height / 2;

		x2 = _cos(arcAngle, width) + x + width / 2;
		y2 = -1 * _sin(arcAngle, height) + y + height / 2;
	}
	Arc(_W_GRAPHICS(gc)->handle, x, y, x + width + 1, y + height + 1, x1, y1,
			x2, y2);
#endif
	return result;
}
wresult w_graphics_draw_focus(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_HIDEFOCUS) != 0)
		return W_FALSE;
	//data.focusDrawn = true;
	HDC hdc = _W_GRAPHICS(gc)->handle;
	int state = 0;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		HRGN clipRgn = 0;
		GdipSetPixelOffsetMode(_W_GRAPHICS(gc)->gdipGraphics,
				PixelOffsetModeNone);
		GpRegion *rgn = 0;
		GdipCreateRegion(&rgn);
		if (rgn == 0) {
			return W_ERROR_NO_HANDLES;
		}
		GdipGetClip(_W_GRAPHICS(gc)->gdipGraphics, rgn);
		BOOL IsInfinite = FALSE;
		GdipIsInfiniteRegion(rgn, _W_GRAPHICS(gc)->gdipGraphics, &IsInfinite);
		if (!IsInfinite) {
			GdipGetRegionHRgn(rgn, _W_GRAPHICS(gc)->gdipGraphics, &clipRgn);
		}
		GdipDeleteRegion(rgn);
		GdipSetPixelOffsetMode(_W_GRAPHICS(gc)->gdipGraphics,
				PixelOffsetModeHalf);
		XFORM lpXform;
		GpMatrix *matrix = 0;
		GdipCreateMatrix2(1, 0, 0, 1, 0, 0, &matrix);
		if (matrix == 0)
			return W_ERROR_NO_HANDLES;
		GdipGetWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, matrix);
		BOOL IsMatrixIdentity = TRUE;
		GdipIsMatrixIdentity(matrix, &IsMatrixIdentity);
		if (!IsMatrixIdentity) {
			GdipGetMatrixElements(matrix, (REAL*) &lpXform);
		}
		GdipDeleteMatrix(matrix);
		GdipGetDC(_W_GRAPHICS(gc)->gdipGraphics, &hdc);
		state = SaveDC(hdc);
		if (!IsMatrixIdentity) {
			SetGraphicsMode(hdc, GM_ADVANCED);
			SetWorldTransform(hdc, &lpXform);
		}
		if (clipRgn != 0) {
			SelectClipRgn(hdc, clipRgn);
			DeleteObject(clipRgn);
		}
	}
	SetBkColor(hdc, 0xFFFFFF);
	SetTextColor(hdc, 0x000000);
	RECT r;
	SetRect(&r, rect->x, rect->y, rect->x + rect->width,
			rect->y + rect->height);
	DrawFocusRect(hdc, &r);
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		RestoreDC(hdc, state);
		GdipReleaseDC(_W_GRAPHICS(gc)->gdipGraphics, hdc);
	} else {
		_W_GRAPHICS(gc)->state &= ~(W_GRAPHICS_BACKGROUND_TEXT
				| W_GRAPHICS_FOREGROUND_TEXT);
	}
	return result;
}
wresult w_graphics_draw_image(w_graphics *gc, w_image *image,
		w_rect *src, w_rect *dest,int state) {
	wresult result = w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (!w_image_is_ok(image))
		return W_ERROR_INVALID_ARGUMENT;
	int srcX = src->x, srcY = src->y, srcWidth = src->width, srcHeight =
			src->height;
	int destX = dest->x, destY = dest->y, destWidth = dest->width, destHeight =
			dest->height;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		GpBitmap *img = 0;
		if (_W_IMAGE(image)->gimage != 0) {
			image = _W_IMAGE(image)->gimage;
		} else {
			GdipCreateBitmapFromHBITMAP(_W_IMAGE(image)->image, NULL, &img);
			_W_IMAGE(image)->gimage = img;
		}
		if (img != 0) {
			UINT imgWidth;
			UINT imgHeight;
			GdipGetImageWidth(img, &imgWidth);
			GdipGetImageHeight(img, &imgHeight);
			Rect rect;
			rect.X = destX;
			rect.Y = destY;
			rect.Width = destWidth;
			rect.Height = destHeight;
			/*
			 * Note that if the wrap mode is not WrapModeTileFlipXY, the scaled image
			 * is translucent around the borders.
			 */
			GpImageAttributes *attrib = 0;
			GdipCreateImageAttributes(&attrib);
			GdipSetImageAttributesWrapMode(attrib, WrapModeTileFlipXY, 0,
			FALSE);
			if (_W_GRAPHICS(gc)->alpha != 0xFF) {
				ColorMatrix matrix = { .m[0]= { 1, 0, 0, 0, 0 }, .m[1]= { 0, 1,
						0, 0, 0 }, .m[2]= { 0, 0, 1, 0, 0 }, .m[3]= { 0, 0, 0,
				_W_GRAPHICS(gc)->alpha / (float) 0xFF, 0 }, .m[4]= { 0, 0, 0, 0,
						1 } };
				GdipSetImageAttributesColorMatrix(attrib, ColorAdjustTypeBitmap,
				FALSE, &matrix, 0, ColorMatrixFlagsDefault);
			}
			GraphicsState gstate = 0;
			if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
				GdipSaveGraphics(_W_GRAPHICS(gc)->gdipGraphics, &gstate);
				GdipScaleWorldTransform(_W_GRAPHICS(gc)->gdipGraphics, -1, 1,
						MatrixOrderPrepend);
				GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
						-2 * destX - destWidth, 0, MatrixOrderPrepend);
			}
			GdipDrawImageRectRectI(_W_GRAPHICS(gc)->gdipGraphics, img, rect.X,
					rect.Y, rect.Width, rect.Height, srcX, srcY, srcWidth,
					srcHeight, UnitPixel, attrib, 0, 0);
			if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
				GdipRestoreGraphics(_W_GRAPHICS(gc)->gdipGraphics, gstate);
			}
			GdipDisposeImageAttributes(attrib);
			GdipDisposeImage(img);
		}
		return W_TRUE;
	}
	if (_W_IMAGE(image)->flags & IMAGE_ICON) {

	} else {
		if (_W_IMAGE(image)->image == 0) {
			GdipCreateHBITMAPFromBitmap(_W_IMAGE(image)->gimage,
					&_W_IMAGE(image)->image, 0);
		}
		if (_W_IMAGE(image)->image == 0)
			return W_FALSE;
		BITMAP bm;
		GetObjectW(_W_IMAGE(image)->image, sizeof(bm), &bm);
		int imgWidth = bm.bmWidth;
		int imgHeight = bm.bmHeight;
		if (srcWidth < 0 || srcWidth >= imgWidth)
			srcWidth = imgWidth;
		if (srcHeight < 0 || srcHeight >= imgHeight)
			srcHeight = imgHeight;
		if (destWidth < 0)
			destWidth = imgWidth;
		if (destHeight < 0)
			destHeight = imgHeight;
		//int mustRestore = W_FALSE;
		if (bm.bmBitsPixel == 32) {
			BLENDFUNCTION blend;
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 0xFF;
			blend.AlphaFormat = AC_SRC_ALPHA;
			HDC srcHdc = CreateCompatibleDC(_W_GRAPHICS(gc)->handle);
			HBITMAP oldSrcBitmap = SelectObject(srcHdc, _W_IMAGE(image)->image);
			AlphaBlend(_W_GRAPHICS(gc)->handle, destX, destY, destWidth,
					destHeight, srcHdc, srcX, srcY, srcWidth, srcHeight, blend);
			SelectObject(srcHdc, oldSrcBitmap);
			DeleteDC(srcHdc);
			return W_TRUE;
		} else {
			HDC srcHdc = CreateCompatibleDC(_W_GRAPHICS(gc)->handle);
			HBITMAP oldSrcBitmap = SelectObject(srcHdc, _W_IMAGE(image)->image);
			int rop2 = 0;
#if !IsWinCE
			rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
#else
				rop2 = SetROP2 (handle, R2_COPYPEN);
				SetROP2 (handle, rop2);
#endif
			int dwRop = rop2 == R2_XORPEN ? SRCINVERT : SRCCOPY;
			if (srcWidth != destWidth || srcHeight != destHeight) {
				int mode = 0;
#if !IsWinCE
				mode = SetStretchBltMode(_W_GRAPHICS(gc)->handle, COLORONCOLOR);
#endif
				StretchBlt(_W_GRAPHICS(gc)->handle, destX, destY, destWidth,
						destHeight, srcHdc, srcX, srcY, srcWidth, srcHeight,
						dwRop);
#if !IsWinCE
				SetStretchBltMode(_W_GRAPHICS(gc)->handle, mode);
#endif
			} else {
				BitBlt(_W_GRAPHICS(gc)->handle, destX, destY, destWidth,
						destHeight, srcHdc, srcX, srcY, dwRop);
			}
			SelectObject(srcHdc, oldSrcBitmap);
			DeleteDC(srcHdc);
		}
	}
	return result;
}
wresult w_graphics_draw_line(w_graphics *gc, w_point *pt1, w_point *pt2) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_DRAW);
	if (result < 0)
		return result;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
		_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		GdipDrawLineI(_W_GRAPHICS(gc)->gdipGraphics, _W_GRAPHICS(gc)->gdipPen,
				pt1->x, pt1->y, pt2->x, pt2->y);
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
				-_W_GRAPHICS(gc)->gdipXOffset, -_W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		return result;
	}
#if IsWinCE
		int points[] = {x1, y1, x2, y2};
		Polyline(handle, points, points.length / 2);
#else
	MoveToEx(_W_GRAPHICS(gc)->handle, pt1->x, pt1->y, 0);
	LineTo(_W_GRAPHICS(gc)->handle, pt2->x, pt2->y);
#endif
	if (_W_GRAPHICS(gc)->line.width <= 1) {
		SetPixel(_W_GRAPHICS(gc)->handle, pt1->x, pt1->y,
		_W_GRAPHICS(gc)->foreground);
	}
	return W_TRUE;
}
wresult w_graphics_draw_oval(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_DRAW);
	if (result < 0)
		return result;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
		_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		GdipDrawEllipseI(_W_GRAPHICS(gc)->gdipGraphics,
		_W_GRAPHICS(gc)->gdipPen, rect->x, rect->y, rect->width, rect->height);
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
				-_W_GRAPHICS(gc)->gdipXOffset, -_W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		return result;
	}
	int x = rect->x;
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
		if (_W_GRAPHICS(gc)->line.width != 0
				&& ((int) _W_GRAPHICS(gc)->line.width) % 2 == 0)
			x--;
	}
	Ellipse(_W_GRAPHICS(gc)->handle, x, rect->y, x + rect->width + 1,
			rect->y + rect->height + 1);
	return result;
}
wresult w_graphics_draw_path(w_graphics *gc, w_path *path) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (path == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	 if (path.handle == 0) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 wresult result= w_graphics_init_gdip(gc);
	 checkGC(DRAW);
	 long gdipGraphics = data.gdipGraphics;
	 Gdip.Graphics_TranslateTransform(gdipGraphics, data.gdipXOffset, data.gdipYOffset, Gdip.MatrixOrderPrepend);
	 Gdip.Graphics_DrawPath(gdipGraphics, data.gdipPen, path.handle);
	 Gdip.Graphics_TranslateTransform(gdipGraphics, -data.gdipXOffset, -data.gdipYOffset, Gdip.MatrixOrderPrepend);*/
}
wresult w_graphics_draw_point(w_graphics *gc, w_point *pt) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.gdipGraphics != 0) {
	 checkGC(DRAW);
	 Gdip.Graphics_FillRectangle(data.gdipGraphics, getFgBrush(), x, y, 1, 1);
	 return;
	 }
	 OS.SetPixel (handle, x, y, data.foreground);*/
}
wresult w_graphics_draw_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_DRAW);
	if (result < 0)
		return result;
	GpGraphics *gdipGraphics = _W_GRAPHICS(gc)->gdipGraphics;
	/*if (gdipGraphics != 0) {
	 Gdip.Graphics_TranslateTransform(gdipGraphics, data.gdipXOffset, data.gdipYOffset, Gdip.MatrixOrderPrepend);
	 Gdip.Graphics_DrawPolygon(gdipGraphics, data.gdipPen, pointArray, pointArray.length / 2);
	 Gdip.Graphics_TranslateTransform(gdipGraphics, -data.gdipXOffset, -data.gdipYOffset, Gdip.MatrixOrderPrepend);
	 return;
	 }*/
	/*if ((data.style & SWT.MIRRORED) != 0) {
	 if (data.lineWidth != 0 && data.lineWidth % 2 == 0) {
	 for (int i = 0; i < pointArray.length; i+=2) {
	 pointArray[i]--;
	 }
	 }
	 }*/
	Polygon(_W_GRAPHICS(gc)->handle, (POINT*) pointArray, count);
	/*if ((data.style & SWT.MIRRORED) != 0) {
	 if (data.lineWidth != 0 && data.lineWidth % 2 == 0) {
	 for (int i = 0; i < pointArray.length; i+=2) {
	 pointArray[i]++;
	 }
	 }
	 }*/
	return result;
}
wresult w_graphics_draw_polyline(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_DRAW);
	if (result < 0)
		return result;
	//if (pointArray == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	/*long gdipGraphics = data.gdipGraphics;
	 if (gdipGraphics != 0) {
	 Gdip.Graphics_TranslateTransform(gdipGraphics, data.gdipXOffset, data.gdipYOffset, Gdip.MatrixOrderPrepend);
	 Gdip.Graphics_DrawLines(gdipGraphics, data.gdipPen, pointArray, pointArray.length / 2);
	 Gdip.Graphics_TranslateTransform(gdipGraphics, -data.gdipXOffset, -data.gdipYOffset, Gdip.MatrixOrderPrepend);
	 return;
	 }*/
	/*if ((data.style & SWT.MIRRORED) != 0) {
	 if (data.lineWidth != 0 && data.lineWidth % 2 == 0) {
	 for (int i = 0; i < pointArray.length; i+=2) {
	 pointArray[i]--;
	 }
	 }
	 }*/
	Polyline(_W_GRAPHICS(gc)->handle, (POINT*) pointArray, count);
	/*int length = pointArray.length;
	 if (length >= 2) {
	 if (data.lineWidth <= 1) {
	 OS.SetPixel (handle, pointArray[length - 2], pointArray[length - 1], data.foreground);
	 }
	 }
	 if ((data.style & SWT.MIRRORED) != 0) {
	 if (data.lineWidth != 0 && data.lineWidth % 2 == 0) {
	 for (int i = 0; i < pointArray.length; i+=2) {
	 pointArray[i]++;
	 }
	 }
	 }*/
	return result;
}

wresult w_graphics_draw_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_DRAW);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
		_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		GdipDrawRectangleI(_W_GRAPHICS(gc)->gdipGraphics,
		_W_GRAPHICS(gc)->gdipPen, x, y, width, height);
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
				-_W_GRAPHICS(gc)->gdipXOffset, -_W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		return W_TRUE;
	}
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
		/*
		 * Note that Rectangle() subtracts one pixel in MIRRORED mode when
		 * the pen was created with CreatePen() and its width is 0 or 1.
		 */
		if (_W_GRAPHICS(gc)->line.width > 1) {
			if ((((int) _W_GRAPHICS(gc)->line.width) % 2) == 1)
				x++;
		} else {
			if (_W_GRAPHICS(gc)->hPen != 0
					&& GetObjectW(_W_GRAPHICS(gc)->hPen, 0, 0)
							!= sizeof(LOGPEN)) {
				x++;
			}
		}
	}
	Rectangle(_W_GRAPHICS(gc)->handle, x, y, x + width + 1, y + height + 1);
	return result;
}
wresult w_graphics_draw_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_DRAW);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		int nx = x;
		int ny = y;
		int nw = width;
		int nh = height;
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

		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
		_W_GRAPHICS(gc)->gdipXOffset, _W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		if (naw == 0 || nah == 0) {
			GdipDrawRectangleI(_W_GRAPHICS(gc)->gdipGraphics,
			_W_GRAPHICS(gc)->gdipPen, x, y, width, height);
		} else {
			GpPath *path = 0;
			GdipCreatePath(FillModeAlternate, &path);
			if (path != 0) {
				if (nw > naw) {
					if (nh > nah) {
						GdipAddPathArc(path, nx + nw - naw, ny, naw, nah, 0,
								-90);
						GdipAddPathArc(path, nx, ny, naw, nah, -90, -90);
						GdipAddPathArc(path, nx, ny + nh - nah, naw, nah, -180,
								-90);
						GdipAddPathArc(path, nx + nw - naw, ny + nh - nah, naw,
								nah, -270, -90);
					} else {
						GdipAddPathArc(path, nx + nw - naw, ny, naw, nh, -270,
								-180);
						GdipAddPathArc(path, nx, ny, naw, nh, -90, -180);
					}
				} else {
					if (nh > nah) {
						GdipAddPathArc(path, nx, ny, nw, nah, 0, -180);
						GdipAddPathArc(path, nx, ny + nh - nah, nw, nah, -180,
								-180);
					} else {
						GdipAddPathArc(path, nx, ny, nw, nh, 0, 360);
					}
				}
				GdipClosePathFigure(path);
				GdipDrawPath(_W_GRAPHICS(gc)->gdipGraphics,
				_W_GRAPHICS(gc)->gdipPen, path);
				GdipDeletePath(path);
			} else
				result = W_ERROR_NO_HANDLES;
		}
		GdipTranslateWorldTransform(_W_GRAPHICS(gc)->gdipGraphics,
				-_W_GRAPHICS(gc)->gdipXOffset, -_W_GRAPHICS(gc)->gdipYOffset,
				MatrixOrderPrepend);
		return result;
	}
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
		if (_W_GRAPHICS(gc)->line.width != 0
				&& ((int) _W_GRAPHICS(gc)->line.width) % 2 == 0)
			x--;
	}
#if IsWinCE
		/*
		* Bug in WinCE PPC.  On certain devices, RoundRect does not draw
		* all the pixels.  The workaround is to draw a round rectangle
		* using lines and arcs.
		*/
		if (width == 0 || height == 0) return;
		if (arcWidth == 0 || arcHeight == 0) {
			drawRectangleInPixels(x, y, width, height);
			return;
		}
		if (width < 0) {
			x += width;
			width = -width;
		}
		if (height < 0) {
			y += height;
			height = -height;
		}
		if (arcWidth < 0) arcWidth = -arcWidth;
		if (arcHeight < 0) arcHeight = -arcHeight;
		if (arcWidth > width) arcWidth = width;
		if (arcHeight > height) arcHeight = height;

		if (arcWidth < width) {
			drawLineInPixels(x+arcWidth/2, y, x+width-arcWidth/2, y);
			drawLineInPixels(x+arcWidth/2, y+height, x+width-arcWidth/2, y+height);
		}
		if (arcHeight < height) {
			drawLineInPixels(x, y+arcHeight/2, x, y+height-arcHeight/2);
			drawLineInPixels(x+width, y+arcHeight/2, x+width, y+height-arcHeight/2);
		}
		if (arcWidth != 0 && arcHeight != 0) {
			drawArcInPixels(x, y, arcWidth, arcHeight, 90, 90);
			drawArcInPixels(x+width-arcWidth, y, arcWidth, arcHeight, 0, 90);
			drawArcInPixels(x+width-arcWidth, y+height-arcHeight, arcWidth, arcHeight, 0, -90);
			drawArcInPixels(x, y+height-arcHeight, arcWidth, arcHeight, 180, 90);
		}
#else
	RoundRect(_W_GRAPHICS(gc)->handle, x, y, x + width + 1, y + height + 1,
			arcWidth, arcHeight);
	return result;
#endif
}
wresult w_graphics_draw_text(w_graphics *gc, const char *string, size_t length,
		w_rect *rect, int flags) {
	wresult result = w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (string == 0)
		return W_FALSE;
	if (*string == 0)
		return W_FALSE;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		result = w_graphics_check(gc,
				W_GRAPHICS_FONT | W_GRAPHICS_FOREGROUND
						| ((flags & W_DRAW_TRANSPARENT) != 0 ?
								0 : W_GRAPHICS_BACKGROUND));
		if (result < 0)
			return result;
		return w_graphics_drawText(gc, _W_GRAPHICS(gc)->gdipGraphics, string,
				length, rect->x, rect->y, flags, 0);
	}
	struct UnicodeString str;
	unicode_set(&str, string, length);
	if (str.str == 0)
		return W_FALSE;
	RECT r;
	/*
	 * Feature in Windows.  For some reason DrawText(), the maximum
	 * value for the bottom and right coordinates for the RECT that
	 * is used to position the text is different on between Windows
	 * versions.  If this value is larger than the maximum, nothing
	 * is drawn.  On Windows 98, the limit is 0x7FFF.  On Windows CE,
	 * NT, and 2000 it is 0x6FFFFFF. And on XP, it is 0x7FFFFFFF.
	 * The fix is to use the the smaller limit for Windows 98 and the
	 * larger limit on the other Windows platforms.
	 */
	int limit, right, bottom;
#if IsWin95
	limit =0x7FFF;
#else
	limit = 0x6FFFFFF;
#endif
	if (rect->width >= 0)
		right = rect->x + rect->width;
	else
		right = limit;
	if (rect->height >= 0)
		bottom = rect->y + rect->height;
	else
		bottom = limit;

	SetRect(&r, rect->x, rect->y, right, bottom);
	int uFormat = DT_LEFT;
	if ((flags & W_DRAW_DELIMITER) == 0)
		uFormat |= DT_SINGLELINE;
	if ((flags & W_DRAW_TAB) != 0)
		uFormat |= DT_EXPANDTABS;
	if ((flags & W_DRAW_MNEMONIC) == 0)
		uFormat |= DT_NOPREFIX;
	if ((flags & W_DRAW_MNEMONIC) != 0
			&& (_W_GRAPHICS(gc)->state & W_GRAPHICS_UISF_HIDEACCEL)
					!= 0) {
		uFormat |= DT_HIDEPREFIX;
	}
	int rop2 = 0;
#if IsWinCE
		rop2 = SetROP2(_W_GRAPHICS(gc)->handle, R2_COPYPEN);
		SetROP2(_W_GRAPHICS(gc)->handle, rop2);
#else
	rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
#endif
	int oldBkMode = SetBkMode(_W_GRAPHICS(gc)->handle,
			(flags & W_DRAW_TRANSPARENT) != 0 ? TRANSPARENT : OPAQUE);
	result = w_graphics_check(gc,
			W_GRAPHICS_FONT | W_GRAPHICS_FOREGROUND_TEXT
					| W_GRAPHICS_BACKGROUND_TEXT);
	if (result < 0)
		return result;
	if (rop2 != R2_XORPEN) {
		DrawTextW(_W_GRAPHICS(gc)->handle, str.str, str.length, &r, uFormat);
	} else {
		int foreground = GetTextColor(_W_GRAPHICS(gc)->handle);
		if ((flags & W_DRAW_TRANSPARENT) != 0) {
			DrawTextW(_W_GRAPHICS(gc)->handle, str.str, str.length, &r,
					uFormat | DT_CALCRECT);
			int width = r.right - r.left;
			int height = r.bottom - r.top;
			HBITMAP hBitmap = CreateCompatibleBitmap(_W_GRAPHICS(gc)->handle,
					width, height);
			if (hBitmap == 0) {
				unicode_free(&str);
				return W_FALSE;
			}
			HDC memDC = CreateCompatibleDC(_W_GRAPHICS(gc)->handle);
			HBITMAP hOldBitmap = SelectObject(memDC, hBitmap);
			PatBlt(memDC, 0, 0, width, height, BLACKNESS);
			SetBkMode(memDC, TRANSPARENT);
			SetTextColor(memDC, foreground);
			SelectObject(memDC,
					GetCurrentObject(_W_GRAPHICS(gc)->handle, OBJ_FONT));
			SetRect(&r, 0, 0, 0x7FFF, 0x7FFF);
			DrawTextW(memDC, str.str, str.length, &r, uFormat);
			BitBlt(_W_GRAPHICS(gc)->handle, rect->x, rect->y, width, height,
					memDC, 0, 0, SRCINVERT);
			SelectObject(memDC, hOldBitmap);
			DeleteDC(memDC);
			DeleteObject(hBitmap);
		} else {
			int background = GetBkColor(_W_GRAPHICS(gc)->handle);
			SetTextColor(_W_GRAPHICS(gc)->handle, foreground ^ background);
			DrawTextW(_W_GRAPHICS(gc)->handle, str.str, str.length, &r,
					uFormat);
			SetTextColor(_W_GRAPHICS(gc)->handle, foreground);
		}
	}
	SetBkMode(_W_GRAPHICS(gc)->handle, oldBkMode);
	return result;
}
wresult w_graphics_fill_arc(w_graphics *gc, w_rect *rect, int startAngle,
		int arcAngle) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 checkGC(FILL);
	 if (width < 0) {
	 x = x + width;
	 width = -width;
	 }
	 if (height < 0) {
	 y = y + height;
	 height = -height;
	 }
	 if (width == 0 || height == 0 || arcAngle == 0) return;
	 long gdipGraphics = data.gdipGraphics;
	 if (gdipGraphics != 0) {
	 if (width == height) {
	 Gdip.Graphics_FillPie(gdipGraphics, data.gdipBrush, x, y, width, height, -startAngle, -arcAngle);
	 } else {
	 int state = Gdip.Graphics_Save(gdipGraphics);
	 Gdip.Graphics_TranslateTransform(gdipGraphics, x, y, Gdip.MatrixOrderPrepend);
	 Gdip.Graphics_ScaleTransform(gdipGraphics, width, height, Gdip.MatrixOrderPrepend);
	 Gdip.Graphics_FillPie(gdipGraphics, data.gdipBrush, 0, 0, 1, 1, -startAngle, -arcAngle);
	 Gdip.Graphics_Restore(gdipGraphics, state);
	 }
	 return;
	 }

	 if ((data.style & SWT.MIRRORED) != 0) x--;*/
	/*
	 * Feature in WinCE.  The function Pie is not present in the
	 * WinCE SDK.  The fix is to emulate it by using Polygon.
	 */
#if IsWinCE
		/* compute arc with a simple linear interpolation */
		if (arcAngle < 0) {
			startAngle += arcAngle;
			arcAngle = -arcAngle;
		}
		boolean drawSegments = true;
		if (arcAngle >= 360) {
			arcAngle = 360;
			drawSegments = false;
		}
		int[] points = new int[(arcAngle + 1) * 2 + (drawSegments ? 4 : 0)];
		int cteX = 2 * x + width;
		int cteY = 2 * y + height;
		int index = (drawSegments ? 2 : 0);
		for (int i = 0; i <= arcAngle; i++) {
			points[index++] = (cos(startAngle + i, width) + cteX) >> 1;
			points[index++] = (cteY - sin(startAngle + i, height)) >> 1;
		}
		if (drawSegments) {
			points[0] = points[points.length - 2] = cteX >> 1;
			points[1] = points[points.length - 1] = cteY >> 1;
		}
		OS.Polygon(handle, points, points.length / 2);
#else
	/*int x1, y1, x2, y2,tmp;
	 boolean isNegative;
	 if (arcAngle >= 360 || arcAngle <= -360) {
	 x1 = x2 = x + width;
	 y1 = y2 = y + height / 2;
	 } else {
	 isNegative = arcAngle < 0;

	 arcAngle = arcAngle + startAngle;
	 if (isNegative) {
	 // swap angles
	 tmp = startAngle;
	 startAngle = arcAngle;
	 arcAngle = tmp;
	 }
	 x1 = cos(startAngle, width) + x + width/2;
	 y1 = -1 * sin(startAngle, height) + y + height/2;

	 x2 = cos(arcAngle, width) + x + width/2;
	 y2 = -1 * sin(arcAngle, height) + y + height/2;
	 }
	 OS.Pie(handle, x, y, x + width + 1, y + height + 1, x1, y1, x2, y2);*/
#endif
}
wresult w_graphics_fill_gradientrectangle(w_graphics *gc, w_rect *rect,
		int vertical) {
	wresult result = w_graphics_check(gc, 0);
	if (result < 0)
		return result;
	if (rect->width == 0 || rect->height == 0)
		return result;

	int backgroundRGB, foregroundRGB;
	backgroundRGB = w_graphics_get_background(gc);
	foregroundRGB = w_graphics_get_foreground(gc);

	int fromRGB, toRGB;
	fromRGB = foregroundRGB;
	toRGB = backgroundRGB;

	wresult swapColors = W_FALSE;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
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
	if (fromRGB == toRGB) {
		return w_graphics_fill_rectangle(gc, rect);
	}
	/*if (data.gdipGraphics != 0) {
	 wresult result= w_graphics_init_gdip(gc);
	 PointF p1= new PointF(), p2 = new PointF();
	 p1.X = x;
	 p1.Y = y;
	 if (vertical) {
	 p2.X = p1.X;
	 p2.Y = p1.Y + height;
	 } else {
	 p2.X = p1.X + width;
	 p2.Y = p1.Y;
	 }
	 int rgb = ((fromRGB.red & 0xFF) << 16) | ((fromRGB.green & 0xFF) << 8) | (fromRGB.blue & 0xFF);
	 long fromGpColor = Gdip.Color_new(data.alpha << 24 | rgb);
	 if (fromGpColor == 0) SWT.error(SWT.ERROR_NO_HANDLES);
	 rgb = ((toRGB.red & 0xFF) << 16) | ((toRGB.green & 0xFF) << 8) | (toRGB.blue & 0xFF);
	 long toGpColor = Gdip.Color_new(data.alpha << 24 | rgb);
	 if (toGpColor == 0) SWT.error(SWT.ERROR_NO_HANDLES);
	 long brush = Gdip.LinearGradientBrush_new(p1, p2, fromGpColor, toGpColor);
	 Gdip.Graphics_FillRectangle(data.gdipGraphics, brush, x, y, width, height);
	 Gdip.LinearGradientBrush_delete(brush);
	 Gdip.Color_delete(fromGpColor);
	 Gdip.Color_delete(toGpColor);
	 return;
	 }*/
	/* Use GradientFill if supported, only on Windows 98, 2000 and newer. */
	/*
	 * Bug in Windows: On Windows 2000 when the device is a printer,
	 * GradientFill swaps red and blue color components, causing the
	 * gradient to be printed in the wrong color. On Windows 98 when
	 * the device is a printer, GradientFill does not fill completely
	 * to the right edge of the rectangle. The fix is not to use
	 * GradientFill for printer devices.
	 */
	int rop2 = 0;
#if IsWinCE
		rop2 = SetROP2(handle, R2_COPYPEN);
		SetROP2(handle, rop2);
#else
	rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
#endif
	if (/*IsWinNT &&*/rop2 != R2_XORPEN
			&& GetDeviceCaps(_W_GRAPHICS(gc)->handle, TECHNOLOGY)
					!= DT_RASPRINTER) {

		GRADIENT_RECT gradientRect;
		gradientRect.UpperLeft = 0;
		gradientRect.LowerRight = 1;

		TRIVERTEX trivertex[2];
		trivertex[0].x = x;
		trivertex[0].y = y;
		trivertex[0].Red = (short) ((W_RED(fromRGB) << 8) | W_RED(fromRGB));
		trivertex[0].Green =
				(short) ((W_GREEN(fromRGB) << 8) | W_GREEN(fromRGB));
		trivertex[0].Blue = (short) ((W_BLUE(fromRGB) << 8) | W_BLUE(fromRGB));
		trivertex[0].Alpha = -1;

		trivertex[1].x = x + width;
		trivertex[1].y = y + height;
		trivertex[1].Red = (short) ((W_RED(toRGB) << 8) | W_RED(toRGB));
		trivertex[1].Green = (short) ((W_GREEN(toRGB) << 8) | W_GREEN(toRGB));
		trivertex[1].Blue = (short) ((W_BLUE(toRGB) << 8) | W_BLUE(toRGB));
		trivertex[1].Alpha = -1;

		WINBOOL success = GradientFill(_W_GRAPHICS(gc)->handle, trivertex, 2,
				&gradientRect, 1,
				vertical ? GRADIENT_FILL_RECT_V : GRADIENT_FILL_RECT_H);
		if (success)
			return result;
	}
	/*
	 int depth = GetDeviceCaps(_W_GRAPHICS(gc)->handle, BITSPIXEL);
	 int bitResolution = (depth >= 24) ? 8 : (depth >= 15) ? 5 : 0;
	 ImageData.fillGradientRectangle(this, data.device,
	 x, y, width, height, vertical, fromRGB, toRGB,
	 bitResolution, bitResolution, bitResolution);*/
	return result;
}
wresult w_graphics_fill_oval(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_FILL);
	if (result < 0)
		return result;
	/*if (data.gdipGraphics != 0) {
	 Gdip.Graphics_FillEllipse(data.gdipGraphics, data.gdipBrush, x, y, width, height);
	 return;
	 }
	 if ((data.style & SWT.MIRRORED) != 0) x--;*/
	Ellipse(_W_GRAPHICS(gc)->handle, rect->x, rect->y,
			rect->x + rect->width + 1, rect->y + rect->height + 1);
	return result;
}
wresult w_graphics_fill_path(w_graphics *gc, w_path *path) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (path == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	 if (path.handle == 0) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 wresult result= w_graphics_init_gdip(gc);
	 checkGC(FILL);
	 int mode = OS.GetPolyFillMode(handle) == OS.WINDING ? Gdip.FillModeWinding : Gdip.FillModeAlternate;
	 Gdip.GraphicsPath_SetFillMode(path.handle, mode);
	 Gdip.Graphics_FillPath(data.gdipGraphics, data.gdipBrush, path.handle);*/
}
wresult w_graphics_fill_polygon(w_graphics *gc, w_point *pointArray,
		int count) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_FILL);
	if (result < 0)
		return result;
	/*if (data.gdipGraphics != 0) {
	 int mode = OS.GetPolyFillMode(handle) == OS.WINDING ? Gdip.FillModeWinding : Gdip.FillModeAlternate;
	 Gdip.Graphics_FillPolygon(data.gdipGraphics, data.gdipBrush, pointArray, pointArray.length / 2, mode);
	 return;
	 }*/
	Polygon(_W_GRAPHICS(gc)->handle, (POINT*) pointArray, count);
	return result;
}
wresult w_graphics_fill_rectangle(w_graphics *gc, w_rect *rect) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_FILL);
	if (result < 0)
		return result;
	int x = rect->x, y = rect->y, width = rect->width, height = rect->height;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		if (width < 0) {
			x = x + width;
			width = -width;
		}
		if (height < 0) {
			y = y + height;
			height = -height;
		}
		GdipFillRectangle(_W_GRAPHICS(gc)->gdipGraphics,
		_W_GRAPHICS(gc)->gdipBrush, x, y, width, height);
		return result;
	}
	int rop2 = 0;
#if IsWinCE
		rop2 = SetROP2(data->handle, R2_COPYPEN);
		SetROP2(data->handle, rop2);
#else
	rop2 = GetROP2(_W_GRAPHICS(gc)->handle);
#endif
	int dwRop = rop2 == R2_XORPEN ? PATINVERT : PATCOPY;
	PatBlt(_W_GRAPHICS(gc)->handle, x, y, width, height, dwRop);
	return result;
}
wresult w_graphics_fill_roundrectangle(w_graphics *gc, w_rect *rect,
		int arcWidth, int arcHeight) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 checkGC(FILL);
	 if (data.gdipGraphics != 0) {
	 fillRoundRectangleGdip(data.gdipGraphics, data.gdipBrush, x, y, width, height, arcWidth, arcHeight);
	 return;
	 }
	 if ((data.style & SWT.MIRRORED) != 0) x--;
	 OS.RoundRect(handle, x,y,x+width+1,y+height+1,arcWidth, arcHeight);*/
}
int w_graphics_get_advance_width(w_graphics *gc, int ch) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 checkGC(FONT);
	 if (OS.IsWinCE) {
	 SIZE size = new SIZE();
	 OS.GetTextExtentPoint32W(handle, new char[]{ch}, 1, size);
	 return size.cx;
	 }
	 int tch = ch;
	 if (ch > 0x7F) {
	 TCHAR buffer = new TCHAR(getCodePage(), ch, false);
	 tch = buffer.tcharAt(0);
	 }
	 int[] width = new int[1];
	 OS.GetCharWidth(handle, tch, tch, width);
	 return width[0];*/
}
wresult w_graphics_get_advanced(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.gdipGraphics != 0;*/
}
int w_graphics_get_alpha(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.alpha;*/
}
int w_graphics_get_antialias(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.gdipGraphics == 0) return SWT.DEFAULT;
	 int mode = Gdip.Graphics_GetSmoothingMode(data.gdipGraphics);
	 switch (mode) {
	 case Gdip.SmoothingModeDefault: return SWT.DEFAULT;
	 case Gdip.SmoothingModeHighSpeed:
	 case Gdip.SmoothingModeNone: return SWT.OFF;
	 case Gdip.SmoothingModeAntiAlias:
	 case Gdip.SmoothingModeAntiAlias8x8:
	 case Gdip.SmoothingModeHighQuality: return SWT.ON;
	 }
	 return SWT.DEFAULT;*/
}
w_color w_graphics_get_background(w_graphics *gc) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_FILL);
	if (result < 0)
		return 0xFF000000;
	return _W_GRAPHICS(gc)->background;
}
w_pattern* w_graphics_get_background_pattern(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.backgroundPattern;*/
}
int w_graphics_get_char_width(w_graphics *gc, int ch) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 checkGC(FONT);*/

	/* GetCharABCWidths only succeeds on truetype fonts */
	/*if (!OS.IsWinCE) {
	 int tch = ch;
	 if (ch > 0x7F) {
	 TCHAR buffer = new TCHAR(getCodePage(), ch, false);
	 tch = buffer.tcharAt (0);
	 }
	 int[] width = new int[3];
	 if (OS.GetCharABCWidths(handle, tch, tch, width)) {
	 return width[1];
	 }
	 }*/

	/* It wasn't a truetype font */
	/*TEXTMETRIC lptm = OS.IsUnicode ? (TEXTMETRIC)new TEXTMETRICW() : new TEXTMETRICA();
	 OS.GetTextMetrics(handle, lptm);
	 SIZE size = new SIZE();
	 OS.GetTextExtentPoint32W(handle, new char[]{ch}, 1, size);
	 return size.cx - lptm.tmOverhang;*/
}
wresult w_graphics_get_clipping_rect(w_graphics *gc, w_rect *rect) {
	if (gc == 0)
		return W_ERROR_NULL_ARGUMENT;
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		Rect r;
		GdipSetPixelOffsetMode(_W_GRAPHICS(gc)->gdipGraphics,
				PixelOffsetModeNone);
		GdipGetVisibleClipBoundsI(_W_GRAPHICS(gc)->gdipGraphics, &r);
		GdipSetPixelOffsetMode(_W_GRAPHICS(gc)->gdipGraphics,
				PixelOffsetModeHalf);
		rect->x = r.X;
		rect->y = r.Y;
		rect->width = r.Width;
		rect->height = r.Height;
		return W_TRUE;
	}
	RECT r;
	GetClipBox(_W_GRAPHICS(gc)->handle, &r);
	rect->x = r.left;
	rect->y = r.top;
	rect->width = r.right - r.left;
	rect->height = r.bottom - r.top;
	return W_TRUE;
}
wresult w_graphics_get_clipping_region(w_graphics *gc,
		w_region *region) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (region == null) SWT.error (SWT.ERROR_NULL_ARGUMENT);
	 if (region.isDisposed()) SWT.error (SWT.ERROR_INVALID_ARGUMENT);
	 long  gdipGraphics = data.gdipGraphics;
	 if (gdipGraphics != 0) {
	 long  rgn = Gdip.Region_new();
	 Gdip.Graphics_GetClip(data.gdipGraphics, rgn);
	 if (Gdip.Region_IsInfinite(rgn, gdipGraphics)) {
	 Rect rect = new Rect();
	 Gdip.Graphics_SetPixelOffsetMode(gdipGraphics, Gdip.PixelOffsetModeNone);
	 Gdip.Graphics_GetVisibleClipBounds(gdipGraphics, rect);
	 Gdip.Graphics_SetPixelOffsetMode(gdipGraphics, Gdip.PixelOffsetModeHalf);
	 OS.SetRectRgn(region.handle, rect.X, rect.Y, rect.X + rect.Width, rect.Y + rect.Height);
	 } else {
	 long  matrix = Gdip.Matrix_new(1, 0, 0, 1, 0, 0);
	 long  identity = Gdip.Matrix_new(1, 0, 0, 1, 0, 0);
	 Gdip.Graphics_GetTransform(gdipGraphics, matrix);
	 Gdip.Graphics_SetTransform(gdipGraphics, identity);
	 long  hRgn = Gdip.Region_GetHRGN(rgn, data.gdipGraphics);
	 Gdip.Graphics_SetTransform(gdipGraphics, matrix);
	 Gdip.Matrix_delete(identity);
	 Gdip.Matrix_delete(matrix);
	 if (!OS.IsWinCE) {
	 POINT pt = new POINT ();
	 OS.GetWindowOrgEx (handle, pt);
	 OS.OffsetRgn (hRgn, pt.x, pt.y);
	 }
	 OS.CombineRgn(region.handle, hRgn, 0, OS.RGN_COPY);
	 OS.DeleteObject(hRgn);
	 }
	 Gdip.Region_delete(rgn);
	 return;
	 }
	 POINT pt = new POINT ();
	 if (!OS.IsWinCE) OS.GetWindowOrgEx (handle, pt);
	 int result = OS.GetClipRgn (handle, region.handle);
	 if (result != 1) {
	 RECT rect = new RECT();
	 OS.GetClipBox(handle, rect);
	 OS.SetRectRgn(region.handle, rect.left, rect.top, rect.right, rect.bottom);
	 } else {
	 OS.OffsetRgn (region.handle, pt.x, pt.y);
	 }
	 if (!OS.IsWinCE) {
	 long  metaRgn = OS.CreateRectRgn (0, 0, 0, 0);
	 if (OS.GetMetaRgn (handle, metaRgn) != 0) {
	 OS.OffsetRgn (metaRgn, pt.x, pt.y);
	 OS.CombineRgn (region.handle, metaRgn, region.handle, OS.RGN_AND);
	 }
	 OS.DeleteObject(metaRgn);
	 long  hwnd = data.hwnd;
	 if (hwnd != 0 && data.ps != null) {
	 long  sysRgn = OS.CreateRectRgn (0, 0, 0, 0);
	 if (OS.GetRandomRgn (handle, sysRgn, OS.SYSRGN) == 1) {
	 if (OS.WIN32_VERSION >= OS.VERSION(4, 10)) {
	 if ((OS.GetLayout(handle) & OS.LAYOUT_RTL) != 0) {
	 int nBytes = OS.GetRegionData (sysRgn, 0, null);
	 int [] lpRgnData = new int [nBytes / 4];
	 OS.GetRegionData (sysRgn, nBytes, lpRgnData);
	 long  newSysRgn = OS.ExtCreateRegion(new float [] {-1, 0, 0, 1, 0, 0}, nBytes, lpRgnData);
	 OS.DeleteObject(sysRgn);
	 sysRgn = newSysRgn;
	 }
	 }
	 if (OS.IsWinNT) {
	 OS.MapWindowPoints(0, hwnd, pt, 1);
	 OS.OffsetRgn(sysRgn, pt.x, pt.y);
	 }
	 OS.CombineRgn (region.handle, sysRgn, region.handle, OS.RGN_AND);
	 }
	 OS.DeleteObject(sysRgn);
	 }
	 }*/
}
int w_graphics_get_fill_rule(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (OS.IsWinCE) return SWT.FILL_EVEN_ODD;
	 return OS.GetPolyFillMode(handle) == OS.WINDING ? SWT.FILL_WINDING : SWT.FILL_EVEN_ODD;*/
}
w_font* w_graphics_get_font(w_graphics *gc) {
	wresult result = w_graphics_check(gc, 0);
	if (result < 0)
		return 0;
	return _W_GRAPHICS(gc)->font;
}
wresult w_graphics_get_font_metrics(w_graphics *gc,
		w_fontmetrics *fontMetrics) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_FONT);
	if (result < 0) {
		w_fontmetrics_init(fontMetrics);
		return result;
	}
	GetTextMetricsW(_W_GRAPHICS(gc)->handle, (TEXTMETRICW*) fontMetrics);
	return W_TRUE;
}
w_color w_graphics_get_foreground(w_graphics *gc) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_FILL);
	if (result < 0)
		return 0xFF000000;
	return _W_GRAPHICS(gc)->foreground;
}
w_pattern* w_graphics_get_foreground_pattern(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.foregroundPattern;*/
}
int w_graphics_get_interpolation(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.gdipGraphics == 0) return SWT.DEFAULT;
	 int mode = Gdip.Graphics_GetInterpolationMode(data.gdipGraphics);
	 switch (mode) {
	 case Gdip.InterpolationModeDefault: return SWT.DEFAULT;
	 case Gdip.InterpolationModeNearestNeighbor: return SWT.NONE;
	 case Gdip.InterpolationModeBilinear:
	 case Gdip.InterpolationModeLowQuality: return SWT.LOW;
	 case Gdip.InterpolationModeBicubic:
	 case Gdip.InterpolationModeHighQualityBilinear:
	 case Gdip.InterpolationModeHighQualityBicubic:
	 case Gdip.InterpolationModeHighQuality: return SWT.HIGH;
	 }
	 return SWT.DEFAULT;*/
}
w_line_attributes* w_graphics_get_line_attributes(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 float[] dashes = null;
	 if (data.lineDashes != null) {
	 dashes = new float[data.lineDashes.length];
	 System.arraycopy(data.lineDashes, 0, dashes, 0, dashes.length);
	 }
	 return new LineAttributes(data.lineWidth, data.lineCap, data.lineJoin, data.lineStyle, dashes, data.lineDashesOffset, data.lineMiterLimit);*/
}
int w_graphics_get_line_cap(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.lineCap;*/
}
w_line_dash* w_graphics_get_line_dash(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.lineDashes == null) return null;
	 int[] lineDashes = new int[data.lineDashes.length];
	 for (int i = 0; i < lineDashes.length; i++) {
	 lineDashes[i] = (int)data.lineDashes[i];
	 }
	 return lineDashes;*/
}
int w_graphics_get_line_join(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.lineJoin;*/
}
int w_graphics_get_line_style(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.lineStyle;*/
}
float w_graphics_get_line_width(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return (int)data.lineWidth;*/
}
int w_graphics_get_style(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 return data.style;*/
}
int w_graphics_get_text_antialias(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.gdipGraphics == 0) return SWT.DEFAULT;
	 int mode = Gdip.Graphics_GetTextRenderingHint(data.gdipGraphics);
	 switch (mode) {
	 case Gdip.TextRenderingHintSystemDefault: return SWT.DEFAULT;
	 case Gdip.TextRenderingHintSingleBitPerPixel:
	 case Gdip.TextRenderingHintSingleBitPerPixelGridFit: return SWT.OFF;
	 case Gdip.TextRenderingHintAntiAlias:
	 case Gdip.TextRenderingHintAntiAliasGridFit:
	 case Gdip.TextRenderingHintClearTypeGridFit: return SWT.ON;
	 }
	 return SWT.DEFAULT;*/
}
wresult w_graphics_get_transform(w_graphics *gc,
		w_transform *Transform) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (transform == null) SWT.error(SWT.ERROR_NULL_ARGUMENT);
	 if (transform.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 long gdipGraphics = data.gdipGraphics;
	 if (gdipGraphics != 0) {
	 Gdip.Graphics_GetTransform(gdipGraphics, transform.handle);
	 long identity = identity();
	 Gdip.Matrix_Invert(identity);
	 Gdip.Matrix_Multiply(transform.handle, identity, Gdip.MatrixOrderAppend);
	 Gdip.Matrix_delete(identity);
	 } else {
	 transform.setElements(1, 0, 0, 1, 0, 0);
	 }*/
}
wresult w_graphics_get_XOR_mode(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 int rop2 = 0;
	 if (OS.IsWinCE) {
	 rop2 = OS.SetROP2 (handle, OS.R2_COPYPEN);
	 OS.SetROP2 (handle, rop2);
	 } else {
	 rop2 = OS.GetROP2(handle);
	 }
	 return rop2 == OS.R2_XORPEN;*/
}
wresult w_graphics_is_clipped(w_graphics *gc) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 long gdipGraphics = data.gdipGraphics;
	 if (gdipGraphics != 0) {
	 long rgn = Gdip.Region_new();
	 Gdip.Graphics_GetClip(data.gdipGraphics, rgn);
	 boolean isInfinite = Gdip.Region_IsInfinite(rgn, gdipGraphics);
	 Gdip.Region_delete(rgn);
	 return !isInfinite;
	 }
	 long region = OS.CreateRectRgn(0, 0, 0, 0);
	 int result = OS.GetClipRgn(handle, region);
	 OS.DeleteObject(region);
	 return result > 0;*/
}
wresult w_graphics_set_advanced(w_graphics *gc, int advanced) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (advanced && data.gdipGraphics != 0) return;
	 if (advanced) {
	 wresult result= w_graphics_init_gdip(gc);
	 if(result < 0) return result;
	 } else {
	 disposeGdip();
	 data.alpha = 0xFF;
	 data.backgroundPattern = data.foregroundPattern = null;
	 data.state = 0;
	 setClipping(0);
	 if ((data.style & SWT.MIRRORED) != 0) {
	 OS.SetLayout(handle, OS.GetLayout(handle) | OS.LAYOUT_RTL);
	 }
	 }*/
}
wresult w_graphics_set_antialias(w_graphics *gc, int antialias) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->gdipGraphics == 0 && antialias == W_DEFAULT)
		return W_TRUE;
	int mode = 0;
	switch (antialias) {
	case W_DEFAULT:
		mode = SmoothingModeDefault;
		break;
	case W_OFF:
		mode = SmoothingModeNone;
		break;
	case W_ON:
		mode = SmoothingModeAntiAlias;
		break;
	default:
		return W_ERROR_INVALID_ARGUMENT;
	}
	wresult result = w_graphics_init_gdip(gc);
	if (result < 0)
		return result;
	GdipSetSmoothingMode(_W_GRAPHICS(gc)->gdipGraphics, mode);
	return W_TRUE;
}
wresult w_graphics_set_alpha(w_graphics *gc, int alpha) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->gdipGraphics == 0 && (alpha & 0xFF) == 0xFF)
		return W_TRUE;

	wresult result = w_graphics_init_gdip(gc);
	if (result < 0)
		return result;
	_W_GRAPHICS(gc)->alpha = alpha & 0xFF;
	_W_GRAPHICS(gc)->state &= ~(W_GRAPHICS_BACKGROUND | W_GRAPHICS_FOREGROUND);
	return result;
}
wresult w_graphics_set_background(w_graphics *gc, w_color color) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_GRAPHICS(gc)->background = color;
	_W_GRAPHICS(gc)->backgroundPattern = 0;
	_W_GRAPHICS(gc)->state &= ~(W_GRAPHICS_BACKGROUND
			| W_GRAPHICS_BACKGROUND_TEXT);
	return W_TRUE;
}
wresult w_graphics_set_background_pattern(w_graphics *gc, w_pattern *pattern) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (pattern != null && pattern.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 if (data.gdipGraphics == 0 && pattern == null) return;
	 wresult result= w_graphics_init_gdip(gc);
	 if (data.backgroundPattern == pattern) return;
	 data.backgroundPattern = pattern;
	 data.state &= ~BACKGROUND;*/
}
void w_graphics_set_clipping_HRGN(w_graphics *gc, HRGN clipRgn) {
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		if (clipRgn != 0) {
			GpRegion *region = 0;
			GdipCreateRegionHrgn(clipRgn, &region);
			if (region != 0) {
				GdipSetClipRegion(_W_GRAPHICS(gc)->gdipGraphics, region,
						CombineModeReplace);
				GdipDeleteRegion(region);
			}
		} else {
			GdipResetClip(_W_GRAPHICS(gc)->gdipGraphics);
		}
	} else {
		POINT pt;
		if (clipRgn != 0) {
			GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
			OffsetRgn(clipRgn, -pt.x, -pt.y);
		}
		SelectClipRgn(_W_GRAPHICS(gc)->handle, clipRgn);
		if (clipRgn != 0) {
			OffsetRgn(clipRgn, pt.x, pt.y);
		}
	}
}
wresult w_graphics_set_clipping_path(w_graphics *gc, w_path *path) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (path != null && path.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 setClipping(0);
	 if (path != null) {
	 wresult result= w_graphics_init_gdip(gc);
	 int mode = OS.GetPolyFillMode(handle) == OS.WINDING ? Gdip.FillModeWinding : Gdip.FillModeAlternate;
	 Gdip.GraphicsPath_SetFillMode(path.handle, mode);
	 Gdip.Graphics_SetClipPath(data.gdipGraphics, path.handle);
	 }*/
}
wresult w_graphics_set_clipping_rect(w_graphics *gc, w_rect *rect) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (rect == 0)
		return w_graphics_set_clipping_region(gc, 0);
	w_region region;
	_W_REGION(&region)->handle = CreateRectRgn(rect->x, rect->y,
			rect->x + rect->width, rect->y + rect->height);
	wresult result = w_graphics_set_clipping_region(gc, &region);
	DeleteObject( _W_REGION(&region)->handle);
	return result;
}
wresult w_graphics_set_clipping_region(w_graphics *gc, w_region *region) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	HRGN hRgn;
	if (region == 0)
		hRgn = 0;
	else {
		hRgn = _W_REGION(region)->handle;
		//if(hRgn == 0) return W_ERROR_INVALID_ARGUMENT;
	}
	if (_W_GRAPHICS(gc)->gdipGraphics != 0) {
		if (hRgn != 0) {
			GdipSetClipHrgn(_W_GRAPHICS(gc)->gdipGraphics, hRgn,
					CombineModeReplace);
		} else {
			GdipResetClip(_W_GRAPHICS(gc)->gdipGraphics);
		}
	} else {
#if IsWinCE
		SelectClipRgn(_W_GRAPHICS(gc)->handle, hRgn);
#else
		POINT pt;
		if (hRgn != 0) {
			GetWindowOrgEx(_W_GRAPHICS(gc)->handle, &pt);
			OffsetRgn(hRgn, -pt.x, -pt.y);
		}
		SelectClipRgn(_W_GRAPHICS(gc)->handle, hRgn);
		if (hRgn != 0) {
			OffsetRgn(hRgn, pt.x, pt.y);
		}
#endif
	}
	return W_TRUE;
}
wresult w_graphics_set_fill_rule(w_graphics *gc, int rule) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (OS.IsWinCE) return;
	 int mode = OS.ALTERNATE;
	 switch (rule) {
	 case SWT.FILL_WINDING: mode = OS.WINDING; break;
	 case SWT.FILL_EVEN_ODD: mode = OS.ALTERNATE; break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 OS.SetPolyFillMode(handle, mode);*/
}
wresult w_graphics_set_font(w_graphics *gc, w_font *font) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (font != 0 && _W_FONT(font)->handle == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if (font == 0) {
		if (win_toolkit == 0) {
			w_toolkit_get_platform();
		}
		font = (w_font*) &win_toolkit->systemfont;
	}
	_W_GRAPHICS(gc)->font = font;
	_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_FONT;
	return W_TRUE;
}
wresult w_graphics_set_foreground(w_graphics *gc, w_color color) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	_W_GRAPHICS(gc)->foreground = color;
	_W_GRAPHICS(gc)->foregroundPattern = 0;
	_W_GRAPHICS(gc)->state &= ~(W_GRAPHICS_FOREGROUND
			| W_GRAPHICS_FOREGROUND_TEXT);
	return W_TRUE;
}
wresult w_graphics_set_foreground_pattern(w_graphics *gc, w_pattern *pattern) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (pattern != null && pattern.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 if (data.gdipGraphics == 0 && pattern == null) return;
	 wresult result= w_graphics_init_gdip(gc);
	 if (data.foregroundPattern == pattern) return;
	 data.foregroundPattern = pattern;
	 data.state &= ~FOREGROUND;*/
}
wresult w_graphics_set_interpolation(w_graphics *gc, int interpolation) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.gdipGraphics == 0 && interpolation == SWT.DEFAULT) return;
	 int mode = 0;
	 switch (interpolation) {
	 case SWT.DEFAULT: mode = Gdip.InterpolationModeDefault; break;
	 case SWT.NONE: mode = Gdip.InterpolationModeNearestNeighbor; break;
	 case SWT.LOW: mode = Gdip.InterpolationModeLowQuality; break;
	 case SWT.HIGH: mode = Gdip.InterpolationModeHighQuality; break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 wresult result= w_graphics_init_gdip(gc);
	 Gdip.Graphics_SetInterpolationMode(data.gdipGraphics, mode);*/
}
wresult w_graphics_set_line_attributes(w_graphics *gc,
		w_line_attributes *attributes) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
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
	 wresult result= w_graphics_init_gdip(gc);
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
	if (_W_GRAPHICS(gc)->handle == 0)
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
	_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_LINE_CAP;
	return W_TRUE;
}
wresult w_graphics_set_line_dash(w_graphics *gc, int *dashes, int length) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	w_line_dash *lineDashes = _W_GRAPHICS(gc)->line.dash;
	if (dashes != 0 && length > 0) {
		wresult changed = _W_GRAPHICS(gc)->line.style != W_LINE_CUSTOM
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
	_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_join(w_graphics *gc, int join) {
	if (_W_GRAPHICS(gc)->handle == 0)
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
	_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_LINE_JOIN;
	return W_TRUE;
}
wresult w_graphics_set_line_style(w_graphics *gc, int lineStyle) {
	if (_W_GRAPHICS(gc)->handle == 0)
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
	_W_GRAPHICS(gc)->state &= ~W_GRAPHICS_LINE_STYLE;
	return W_TRUE;
}
wresult w_graphics_set_line_width(w_graphics *gc, float lineWidth) {
	if (_W_GRAPHICS(gc)->handle == 0)
		return W_ERROR_NO_HANDLES;
	if (_W_GRAPHICS(gc)->line.width == lineWidth)
		return W_TRUE;
	_W_GRAPHICS(gc)->line.width = lineWidth;
	_W_GRAPHICS(gc)->state &= ~(W_GRAPHICS_LINE_WIDTH | W_GRAPHICS_DRAW_OFFSET);
	return W_TRUE;
}
wresult w_graphics_set_XOR_mode(w_graphics *gc, wresult _xor) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 OS.SetROP2(handle, xor ? OS.R2_XORPEN : OS.R2_COPYPEN);*/
}
wresult w_graphics_set_text_antialias(w_graphics *gc, int antialias) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (data.gdipGraphics == 0 && antialias == SWT.DEFAULT) return;
	 int textMode = 0;
	 switch (antialias) {
	 case SWT.DEFAULT:
	 textMode = Gdip.TextRenderingHintSystemDefault;
	 break;
	 case SWT.OFF:
	 textMode = Gdip.TextRenderingHintSingleBitPerPixelGridFit;
	 break;
	 case SWT.ON:
	 int[] type = new int[1];
	 OS.SystemParametersInfo(OS.SPI_GETFONTSMOOTHINGTYPE, 0, type, 0);
	 if (type[0] == OS.FE_FONTSMOOTHINGCLEARTYPE) {
	 textMode = Gdip.TextRenderingHintClearTypeGridFit;
	 } else {
	 textMode = Gdip.TextRenderingHintAntiAliasGridFit;
	 }
	 break;
	 default:
	 SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 }
	 wresult result= w_graphics_init_gdip(gc);
	 Gdip.Graphics_SetTextRenderingHint(data.gdipGraphics, textMode);*/
}
wresult w_graphics_set_transform(w_graphics *gc, w_transform *Transform) {
	/*if (handle == 0) SWT.error(SWT.ERROR_GRAPHIC_DISPOSED);
	 if (transform != null && transform.isDisposed()) SWT.error(SWT.ERROR_INVALID_ARGUMENT);
	 if (data.gdipGraphics == 0 && transform == null) return;
	 wresult result= w_graphics_init_gdip(gc);
	 long identity = identity();
	 if (transform != null) {
	 Gdip.Matrix_Multiply(identity, transform.handle, Gdip.MatrixOrderPrepend);
	 }
	 Gdip.Graphics_SetTransform(data.gdipGraphics, identity);
	 Gdip.Matrix_delete(identity);
	 data.state &= ~DRAW_OFFSET;*/
}
wresult w_graphics_text_extent(w_graphics *gc, const char *string, int length,
		w_size *size, int flags) {
	wresult result = w_graphics_check(gc, W_GRAPHICS_FONT);
	if (result < 0)
		return result;
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	struct UnicodeString str;
	unicode_set(&str, string, length);
	if (str.str == 0)
		return W_ERROR_NO_MEMORY;
	/*long gdipGraphics = data.gdipGraphics;
	 if (gdipGraphics != 0) {
	 Point size = new Point(0, 0);
	 drawText(gdipGraphics, string, 0, 0, flags, size);
	 return size;
	 }*/
	if (str.length == 0) {
		SIZE _size;
		WCHAR c = ' ';
		GetTextExtentPoint32W(_W_GRAPHICS(gc)->handle, &c, 1, &_size);
		size->width = _size.cx;
		size->height = _size.cy;
	} else {
		RECT rect;
		int uFormat = DT_LEFT | DT_CALCRECT;
		if ((flags & W_DRAW_DELIMITER) == 0)
			uFormat |= DT_SINGLELINE;
		if ((flags & W_DRAW_TAB) != 0)
			uFormat |= DT_EXPANDTABS;
		if ((flags & W_DRAW_MNEMONIC) == 0)
			uFormat |= DT_NOPREFIX;
		DrawTextW(_W_GRAPHICS(gc)->handle, str.str, str.length, &rect, uFormat);
		size->width = rect.right;
		size->height = rect.bottom;
	}
	unicode_free(&str);
	return W_TRUE;
}
wresult w_graphics_use_GDIP_0(HDC hdc, struct UnicodeString *buffer,
		WORD *glyphs) {
	GetGlyphIndicesW(hdc, buffer->str, buffer->length, glyphs,
	GGI_MARK_NONEXISTING_GLYPHS);
	for (int i = 0; i < buffer->length; i++) {
		if (glyphs[i] == -1) {
			switch (buffer->str[i]) {
			case '\t':
			case '\n':
			case '\r':
				break;
			default:
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}
wresult w_graphics_use_GDIP_tmp(HDC hdc, struct UnicodeString *buffer) {
	WORD glyphs[buffer->length];
	return w_graphics_use_GDIP_0(hdc, buffer, glyphs);
}
wresult w_graphics_use_GDIP(HDC hdc, struct UnicodeString *buffer) {
	wresult result;
	if (buffer->length < 0x100) {
		result = w_graphics_use_GDIP_tmp(hdc, buffer);
	} else {
		WORD *glyphs = malloc(buffer->length * sizeof(WORD));
		if (glyphs != 0) {
			result = w_graphics_use_GDIP_0(hdc, buffer, glyphs);
			free(glyphs);
		} else
			result = W_FALSE;
	}
	return result;
}
RectF* w_graphics_drawText_0(w_graphics *gc, GpGraphics *gdipGraphics,
		struct UnicodeString *buffer, int start, int length, int x, int y,
		int flags, int mnemonicIndex, TEXTMETRICW *lptm, boolean draw,
		RectF *r);
wresult w_graphics_drawTextGDIP(w_graphics *gc, GpGraphics *gdipGraphics,
		struct UnicodeString *buffer, int x, int y, int flags, wresult draw,
		w_point *size);
GpBrush* w_graphics_getFgBrush(w_graphics *gc) {
	//return _W_GRAPHICS(gc)->foregroundPattern != 0 ? _W_GRAPHICS(gc)->foregroundPattern.handle : _W_GRAPHICS(gc)->gdipFgBrush;
	return _W_GRAPHICS(gc)->gdipFgBrush;
}
wresult w_graphics_drawText(w_graphics *gc, GpGraphics *gdipGraphics,
		const char *string, size_t length, int x, int y, int flags,
		w_point *size) {
	struct UnicodeString str;
	RectF r;
	unicode_set(&str, string, length);
	if (str.str == 0)
		return W_ERROR_NO_MEMORY;
	length = str.length;
	HDC hdc = 0;
	GdipGetDC(gdipGraphics, &hdc);
	HFONT hFont = _W_GRAPHICS(gc)->hGDIFont;
	if (hFont == 0 && _W_GRAPHICS(gc)->font != 0)
		hFont = _W_FONT(_W_GRAPHICS(gc)->font)->handle;
	HFONT oldFont = 0;
	if (hFont != 0)
		oldFont = SelectObject(hdc, hFont);
	TEXTMETRICW lptm;
	GetTextMetricsW(hdc, &lptm);
	wresult gdip = w_graphics_use_GDIP(hdc, &str);
	if (hFont != 0)
		SelectObject(hdc, oldFont);
	GdipReleaseDC(gdipGraphics, hdc);
	if (gdip) {
		return w_graphics_drawTextGDIP(gc, gdipGraphics, &str, x, y, flags,
				size == 0, size);
	}
	int i = 0, start = 0, end = 0, drawX = x, drawY = y, width = 0,
			mnemonicIndex = -1;
	if ((flags & (W_DRAW_DELIMITER | W_DRAW_TAB | W_DRAW_MNEMONIC)) != 0) {
		int tabWidth = lptm.tmAveCharWidth * 8;
		while (i < length) {
			char c = str.str[end++] = str.str[i++];
			switch (c) {
			case '\t':
				if ((flags & W_DRAW_TAB) != 0) {
					int l = end - start - 1;
					RectF *bounds = w_graphics_drawText_0(gc, gdipGraphics,
							&str, start, l, drawX, drawY, flags, mnemonicIndex,
							&lptm, size == 0, &r);
					drawX += ceil(bounds->Width);
					drawX = x + (((drawX - x) / tabWidth) + 1) * tabWidth;
					mnemonicIndex = -1;
					start = end;
				}
				break;
			case '&':
				if ((flags & W_DRAW_MNEMONIC) != 0) {
					if (i == length) {
						end--;
						continue;
					}
					if (str.str[i] == '&') {
						i++;
						continue;
					}
					end--;
					mnemonicIndex = end - start;
				}
				break;
			case '\r':
			case '\n':
				if ((flags & W_DRAW_DELIMITER) != 0) {
					int l = end - start - 1;
					if (c == '\r' && end != length && str.str[end] == '\n') {
						end++;
						i++;
					}
					RectF *bounds = w_graphics_drawText_0(gc, gdipGraphics,
							&str, start, l, drawX, drawY, flags, mnemonicIndex,
							&lptm, size == 0, &r);
					drawY += ceil(bounds->Height);
					width = w_max(width, drawX + (int) ceil(bounds->Width));
					drawX = x;
					mnemonicIndex = -1;
					start = end;
				}
				break;
			}
		}
		length = end;
	}
	RectF *bounds = w_graphics_drawText_0(gc, gdipGraphics, &str, start,
			length - start, drawX, drawY, flags, mnemonicIndex, &lptm,
			size == 0, &r);
	if (size != 0) {
		drawY += ceil(bounds->Height);
		width = w_max(width, drawX + (int) ceil(bounds->Width));
		size->x = width;
		size->y = drawY;
	}
}

RectF* w_graphics_drawText_0(w_graphics *gc, GpGraphics *gdipGraphics,
		struct UnicodeString *buffer, int start, int length, int x, int y,
		int flags, int mnemonicIndex, TEXTMETRICW *lptm, boolean draw,
		RectF *r) {
	wresult drawMnemonic = draw && mnemonicIndex != -1
			&& (_W_GRAPHICS(gc)->state & W_GRAPHICS_UISF_HIDEACCEL) == 0;
	wresult needsBounds = !draw || drawMnemonic
			|| (flags & W_DRAW_TRANSPARENT) == 0
			|| (_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0
			|| (flags & W_DRAW_DELIMITER) != 0;
	RectF *bounds;
	GraphicsState gstate;
	if (length <= 0) {
		bounds = 0;
		if (needsBounds) {
			bounds = r;
			memset(r, 0, sizeof(RectF));
			bounds->Height = lptm->tmHeight;
		}
		return bounds;
	}
	int nGlyphs = (length * 3 / 2) + 16;
	GCP_RESULTSW result;
	memset(&result, 0, sizeof(result));
	result.lStructSize = sizeof(GCP_RESULTS);
	result.nGlyphs = nGlyphs;
	HANDLE hHeap = GetProcessHeap();
	size_t mem_size = nGlyphs * 18;
	void *mem = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, mem_size), *mem_i;
	if (mem != 0) {
		mem_i = mem;
		result.lpDx = mem_i;
		mem_i += nGlyphs * sizeof(int);
		result.lpGlyphs = mem_i;
		mem_i += nGlyphs * sizeof(WCHAR);
		int dwFlags = GCP_GLYPHSHAPE | GCP_REORDER | GCP_LIGATE;
		if (drawMnemonic) {
			result.lpOrder = mem_i;
			mem_i += nGlyphs * sizeof(int);
		}
		HDC hdc;
		GdipGetDC(gdipGraphics, &hdc);
		HFONT hFont = _W_GRAPHICS(gc)->hGDIFont;
		if (hFont == 0 && _W_GRAPHICS(gc)->font != 0)
			hFont = _W_FONT(_W_GRAPHICS(gc)->font)->handle;
		HFONT oldFont = 0;
		if (hFont != 0)
			oldFont = SelectObject(hdc, hFont);
		if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
			SetLayout(hdc, GetLayout(hdc) | LAYOUT_RTL);
		}
		GetCharacterPlacementW(hdc, &buffer->str[start], length - start, 0,
				&result, dwFlags);
		if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
			SetLayout(hdc, GetLayout(hdc) & ~LAYOUT_RTL);
		}
		if (hFont != 0)
			SelectObject(hdc, oldFont);
		GdipReleaseDC(gdipGraphics, hdc);
		nGlyphs = result.nGlyphs;
		int drawX = x, drawY = y + lptm->tmAscent;
		int *dx = result.lpDx;
		PointF *points = (PointF*) mem_i;
		mem_i += nGlyphs * sizeof(PointF);
		for (int i = 0, j = 0; i < nGlyphs; i++) {
			points[j].X = drawX;
			points[j].Y = drawY;
			j++;
			drawX += dx[i];
		}
		bounds = 0;
		if (needsBounds) {
			bounds = r;
			GdipMeasureDriverString(gdipGraphics, result.lpGlyphs, nGlyphs,
			_W_GRAPHICS(gc)->gdipFont, points, 0, 0, bounds);
		}
		if (draw) {
			if ((flags & W_DRAW_TRANSPARENT) == 0) {
				GdipFillRectangleI(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush, x,
						y, (int) ceil(bounds->Width),
						(int) ceil(bounds->Height));
			}
			int gstate = 0;
			GpBrush *brush = w_graphics_getFgBrush(gc);
			if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
				GpBrushType type;
				GdipGetBrushType(brush, &type);
				switch (type) {
				case BrushTypeLinearGradient:
					GdipScaleLineTransform(brush, -1, 1, MatrixOrderPrepend);
					GdipTranslateLineTransform(brush, -2 * x - bounds->Width, 0,
							MatrixOrderPrepend);
					break;
				case BrushTypeTextureFill:
					GdipScaleTextureTransform(brush, -1, 1, MatrixOrderPrepend);
					GdipTranslateLineTransform(brush, -2 * x - bounds->Width, 0,
							MatrixOrderPrepend);
					break;
				}
				GdipSaveGraphics(gdipGraphics, &gstate);
				GdipScaleWorldTransform(gdipGraphics, -1, 1,
						MatrixOrderPrepend);
				GdipTranslateWorldTransform(gdipGraphics,
						-2 * x - bounds->Width, 0, MatrixOrderPrepend);
			}
			GdipDrawDriverString(gdipGraphics, result.lpGlyphs, result.nGlyphs,
			_W_GRAPHICS(gc)->gdipFont, brush, points, 0, 0);
			if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
				GpBrushType type;
				GdipGetBrushType(brush, &type);
				switch (type) {
				case BrushTypeLinearGradient:
					GdipResetLineTransform(brush);
					break;
				case BrushTypeTextureFill:
					GdipResetTextureTransform(brush);
					break;
				}
				GdipRestoreGraphics(gdipGraphics, gstate);
			}
			if (drawMnemonic) {
				GpPen *pen = 0;
				GdipCreatePen2(brush, 1, UnitWorld, &pen);
				if (pen != 0) {
					int order = result.lpOrder[mnemonicIndex];
					int mnemonicLeft, mnemonicRight;
					if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
						mnemonicLeft = (int) ceil(bounds->Width)
								- (int) points[order].X + 2 * x;
						mnemonicRight = mnemonicLeft - dx[order];
					} else {
						mnemonicLeft = (int) points[order].X;
						mnemonicRight = mnemonicLeft + dx[order];
					}
					int mnemonicY = y + lptm->tmAscent + 2;
					SmoothingMode smoothingMode;
					GdipGetSmoothingMode(gdipGraphics, &smoothingMode);
					GdipSetSmoothingMode(gdipGraphics, SmoothingModeNone);
					GdipDrawLineI(gdipGraphics, pen, mnemonicLeft, mnemonicY,
							mnemonicRight, mnemonicY);
					GdipSetSmoothingMode(gdipGraphics, smoothingMode);
					GdipDeletePen(pen);
				}
			}
		}
		HeapFree(hHeap, 0, mem);
	}
	return bounds;
}
float w_graphics_measureSpace(GpGraphics *gdipGraphics, GpFont *font,
		INT format) {
	RectF bounds, pt;
	WCHAR ch[2];
	ch[0] = ' ';
	ch[1] = 0;
	INT i1 = 0, i2 = 0;
	GdipMeasureString(gdipGraphics, ch, 1, font, &pt, &format, &bounds, &i1,
			&i2);
	return bounds.Width;
}

wresult w_graphics_drawTextGDIP(w_graphics *gc, GpGraphics *gdipGraphics,
		struct UnicodeString *buffer, int x, int y, int flags, wresult draw,
		w_point *size) {
	wresult needsBounds = !draw || (flags & W_DRAW_TRANSPARENT) == 0;
	wresult reset_buffer = W_FALSE;
	if (buffer->length == 0) {
		if (draw)
			return W_TRUE;
		buffer->str = buffer->temp;
		buffer->length = 1;
		buffer->str[0] = ' ';
		buffer->str[1] = 0;
		reset_buffer = W_TRUE;
	}
	RectF pt, bounds;
	GpStringFormat *genericTypographic = 0, *format = 0;
	INT formatFlags;
	REAL tabs[1];
	GdipStringFormatGetGenericTypographic(&genericTypographic);
	GdipCloneStringFormat(genericTypographic, &format);
	GdipGetStringFormatFlags(format, &formatFlags);
	formatFlags |= StringFormatFlagsMeasureTrailingSpaces;
	if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0)
		formatFlags |= StringFormatFlagsDirectionRightToLeft;
	GdipSetStringFormatFlags(format, formatFlags);
	if ((flags & W_DRAW_TAB) != 0) {
		tabs[0] = w_graphics_measureSpace(gdipGraphics,
		_W_GRAPHICS(gc)->gdipFont, formatFlags) * 8;
	} else {
		tabs[0] = 0;
	}
	GdipSetStringFormatTabStops(format, 0, 1, tabs);
	int hotkeyPrefix =
			(flags & W_DRAW_MNEMONIC) != 0 ?
					HotkeyPrefixShow : HotkeyPrefixNone;
	//if ((flags & W_DRAW_MNEMONIC) != 0 && (_W_GRAPHICS(gc)->uiState & UISF_HIDEACCEL) != 0) hotkeyPrefix = HotkeyPrefixHide;
	GdipSetStringFormatHotkeyPrefix(format, hotkeyPrefix);
	if (needsBounds) {
		GdipMeasureString(gdipGraphics, buffer->str, buffer->length,
		_W_GRAPHICS(gc)->gdipFont, &pt, format, &bounds, NULL, NULL);
	}
	if (draw) {
		if ((flags & W_DRAW_TRANSPARENT) == 0) {
			GdipFillRectangleI(gdipGraphics, _W_GRAPHICS(gc)->gdipBrush, x, y,
					(int) ceil(bounds.Width), (int) ceil(bounds.Height));
		}
		GraphicsState gstate = 0;
		GpBrushType brushType;
		GpBrush *brush = w_graphics_getFgBrush(gc);
		if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
			GdipGetBrushType(brush, &brushType);
			switch (brushType) {
			case BrushTypeLinearGradient:
				GdipScaleLineTransform(brush, -1, 1, MatrixOrderPrepend);
				GdipTranslateLineTransform(brush, -2 * x, 0,
						MatrixOrderPrepend);
				break;
			case BrushTypeTextureFill:
				GdipScaleTextureTransform(brush, -1, 1, MatrixOrderPrepend);
				GdipTranslateTextureTransform(brush, -2 * x, 0,
						MatrixOrderPrepend);
				break;
			}
			GdipSaveGraphics(gdipGraphics, &gstate);
			GdipScaleWorldTransform(gdipGraphics, -1, 1, MatrixOrderPrepend);
			GdipTranslateWorldTransform(gdipGraphics, -2 * x, 0,
					MatrixOrderPrepend);
		}
		pt.X = x;
		pt.Y = y;
		GdipDrawString(gdipGraphics, buffer->str, buffer->length,
		_W_GRAPHICS(gc)->gdipFont, &pt, format, brush);
		if ((_W_GRAPHICS(gc)->state & W_GRAPHICS_MIRRORED) != 0) {
			GdipGetBrushType(brush, &brushType);
			switch (brushType) {
			case BrushTypeLinearGradient:
				GdipResetLineTransform(brush);
				break;
			case BrushTypeTextureFill:
				GdipResetTextureTransform(brush);
				break;
			}
			GdipRestoreGraphics(gdipGraphics, gstate);
		}
	}
	GdipDeleteStringFormat(format);
	if (reset_buffer) {
		bounds.Width = 0;
		buffer->str[0] = 0;
		buffer->length = 0;
	}
	if (size != 0) {
		size->x = (int) ceil(bounds.Width);
		size->y = (int) ceil(bounds.Height);
	}
	return W_TRUE;
}

#endif
