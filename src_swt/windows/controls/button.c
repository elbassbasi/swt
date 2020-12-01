/*
 * button.c
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "button.h"
#include "../widgets/toolkit.h"
#include "vsstyle.h"
#define STATE_BUTTON_GRAYED (1 << 31)
#define  MARGIN  4
int CHECK_WIDTH = 0;
int CHECK_HEIGHT = 0;
wresult _w_button_get_selection(w_button *button);
wresult _w_button_set_selection(w_button *button, wresult selected);
int _w_button_check_style(int style) {
	style = _w_widget_check_bits(style, W_PUSH, W_ARROW, W_CHECK, W_RADIO,
			W_TOGGLE, 0);
	if ((style & (W_PUSH | W_TOGGLE)) != 0) {
		return _w_widget_check_bits(style, W_CENTER, W_LEFT, W_RIGHT, 0, 0, 0);
	}
	if ((style & (W_CHECK | W_RADIO)) != 0) {
		return _w_widget_check_bits(style, W_LEFT, W_RIGHT, W_CENTER, 0, 0, 0);
	}
	if ((style & W_ARROW) != 0) {
		style |= W_NO_FOCUS;
		return _w_widget_check_bits(style, W_UP, W_DOWN, W_LEFT, W_RIGHT, 0, 0);
	}
	return style;
}
void _w_button_style(w_widget *widget, w_widget *parent, int style,
		DWORD *dwExStyle, DWORD *dwStyle) {
	_w_control_style(widget, parent, style, dwExStyle, dwStyle);
	if ((style & W_FLAT) != 0)
		*dwStyle |= BS_FLAT;
	if ((style & W_ARROW) != 0) {
		*dwStyle |= BS_OWNERDRAW;
		return;
	}
	if ((style & W_LEFT) != 0)
		*dwStyle |= BS_LEFT;
	if ((style & W_CENTER) != 0)
		*dwStyle |= BS_CENTER;
	if ((style & W_RIGHT) != 0)
		*dwStyle |= BS_RIGHT;
	if ((style & W_WRAP) != 0)
		*dwStyle |= BS_MULTILINE;
	if ((style & W_PUSH) != 0) {
		*dwStyle |= BS_PUSHBUTTON | WS_TABSTOP;
		return;
	}
	if ((style & W_CHECK) != 0) {
		*dwStyle |= BS_CHECKBOX | WS_TABSTOP;
		return;
	}
	if ((style & W_RADIO) != 0) {
		*dwStyle |= BS_RADIOBUTTON;
		return;
	}
	if ((style & W_TOGGLE) != 0) {
		*dwStyle |= BS_PUSHLIKE | BS_CHECKBOX | WS_TABSTOP;
		return;
	}
	/*	if ((style & W_COMMAND) != 0){
	 *dwStyle |= BS_COMMANDLINK | WS_TABSTOP;
	 return;
	 }*/
	*dwStyle |= BS_PUSHBUTTON | WS_TABSTOP;
}
void _w_button_update_selection(w_button *button, int flags) {
	if (flags != SendMessageW(_W_WIDGET(button)->handle, BM_GETCHECK, 0, 0)) {
		/*
		 * Feature in Windows. When BM_SETCHECK is used
		 * to set the checked state of a radio or check
		 * button, it sets the WS_TABSTOP style.  This
		 * is undocumented and unwanted.  The fix is
		 * to save and restore the window style bits.
		 */
		int bits = GetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE);
		if ((_W_WIDGET(button)->style & W_CHECK) != 0) {
			if (flags == BST_INDETERMINATE) {
				bits &= ~BS_CHECKBOX;
				bits |= BS_3STATE;
			} else {
				bits |= BS_CHECKBOX;
				bits &= ~BS_3STATE;
			}
			if (bits != GetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE)) {
				SetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE, bits);
			}
		}
		SendMessageW(_W_WIDGET(button)->handle, BM_SETCHECK, flags, 0);
		if (bits != GetWindowLong(_W_WIDGET(button)->handle, GWL_STYLE)) {
			SetWindowLongW(_W_WIDGET(button)->handle, GWL_STYLE, bits);
		}
	}
}
wresult _w_button_is_checked(w_button *button) {
	LRESULT flags = SendMessageW(_W_WIDGET(button)->handle, BM_GETCHECK, 0, 0);
	return flags != BST_UNCHECKED;
}
wresult _w_button_set_radio_selection(w_button *button, wresult value) {
	if ((_W_WIDGET(button)->style & W_RADIO) == 0)
		return W_FALSE;
	if (_w_button_get_selection(button) != value) {
		_w_button_set_selection(button, value);
		w_event event;
		memset(&event, 0, sizeof(event));
		event.type = W_EVENT_SELECTION;
		//event.platform_event = (struct w_event_platform*) 0;
		event.widget = W_WIDGET(button);
		_w_widget_send_event(W_WIDGET(button), &event);
	}
	return W_TRUE;
}
void _w_button_select_radio_0(w_button *button, UINT uCmd) {
	HWND h = _W_WIDGET(button)->handle;
	while ((h = GetWindow(h, uCmd)) != 0) {
		w_control *c = (w_control*) GetWindowLongPtrW(h, GWLP_USERDATA);
		if (c != 0) {
			if (w_widget_get_class(W_WIDGET(c))->class_id == _W_CLASS_BUTTON) {
				if (!_w_button_set_radio_selection(W_BUTTON(c), W_FALSE))
					break;
			} else
				break;
		}
	}
}

void _w_button_select_radio(w_button *button) {
	/*
	 * This code is intentionally commented.  When two groups
	 * of radio buttons with the same parent are separated by
	 * another control, the correct behavior should be that
	 * the two groups act independently.  This is consistent
	 * with radio tool and menu items.  The commented code
	 * implements this behavior.
	 */
	_w_button_select_radio_0(button, GW_HWNDNEXT);
	_w_button_select_radio_0(button, GW_HWNDPREV);
	_w_button_set_selection(button, W_TRUE);
}
/*
 *
 */
int _w_button_get_alignment(w_button *button) {
}
wresult _w_button_get_grayed(w_button *button) {
	if ((_W_WIDGET(button)->style & W_CHECK) == 0)
		return W_FALSE;
	return _W_WIDGET(button)->state & STATE_BUTTON_GRAYED;
}
wresult _w_button_get_image(w_button *button, w_image *image) {
}
wresult _w_button_get_selection(w_button *button) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	return _w_button_is_checked(button);
}
wresult _w_button_get_text(w_button *button, w_alloc *text) {
	/*int l, _l = sizeof(win_toolkit->tmp_wchar)
			/ sizeof(win_toolkit->tmp_wchar[0]);
	WCHAR *tmp = win_toolkit->tmp_wchar;
	l = GetWindowTextW(_W_WIDGET(button)->handle, tmp, _l);
	if (l > _l) {
		tmp = (WCHAR*) malloc((l + 1) * sizeof(WCHAR));
		if (tmp == 0)
			return W_FALSE;
		GetWindowTextW(_W_WIDGET(button)->handle, tmp, l);
		tmp[l] = 0;
	}
	int ll = w_utf8_from_utf16(tmp, l, 0, 0);
	int newsz;
	char *str = 0;
	//newsz = text->realloc(text, ll, &str);
	if (str != 0) {
		w_utf8_from_utf16(tmp, l, str, newsz);
	}
	if (tmp != win_toolkit->tmp_wchar) {
		free(tmp);
	}
	if (str == 0)
		return W_ERROR_NO_MEMORY;
	return W_TRUE;*/
}
wresult _w_button_set_alignment(w_button *button, int alignment) {
}
wresult _w_button_set_grayed(w_button *button, wresult grayed) {
	if ((_W_WIDGET(button)->style & W_CHECK) == 0)
		return W_FALSE;
	LRESULT flags = SendMessageW(_W_WIDGET(button)->handle, BM_GETCHECK, 0, 0);
	if (grayed) {
		_W_WIDGET(button)->state |= STATE_BUTTON_GRAYED;
		if (flags == BST_CHECKED)
			_w_button_update_selection(button, BST_INDETERMINATE);
	} else {
		_W_WIDGET(button)->state &= ~STATE_BUTTON_GRAYED;
		if (flags == BST_INDETERMINATE)
			_w_button_update_selection(button, BST_CHECKED);
	}
	return W_TRUE;
}
wresult _w_button_set_image(w_button *button, w_image *image) {
}
wresult _w_button_set_selection(w_button *button, wresult selected) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	int flags = selected ? BST_CHECKED : BST_UNCHECKED;
	if ((_W_WIDGET(button)->style & W_CHECK) != 0) {
		if (selected && (_W_WIDGET(button)->state & STATE_BUTTON_GRAYED))
			flags = BST_INDETERMINATE;
	}
	_w_button_update_selection(button, flags);
	return W_TRUE;
}
wresult _w_button_set_text(w_button *button, const char *text) {
	if (text == 0)
		text = "";
	if ((_W_WIDGET(button)->style & W_ARROW) != 0)
		return W_TRUE;
	struct UnicodeString str;
	unicode_set(&str, text, -1);
	WINBOOL result = SetWindowTextW(_W_WIDGET(button)->handle, str.str);
	unicode_free(&str);
	if (result)
		return W_TRUE;
	else
		return W_ERROR_CANNOT_SET_TEXT;
}
/*
 *
 */
wresult _w_button_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0) {
		return W_ERROR_NULL_ARGUMENT;
	}
	if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_COMPOSITE)
		return W_ERROR_INVALID_PARENT;
	style = _w_button_check_style(style);
	wresult result;

	DWORD dwExStyle = 0, dwStyle = 0;
	_w_button_style(widget, parent, style, &dwExStyle, &dwStyle);
	result = _w_control_create(W_CONTROL(widget), W_COMPOSITE(parent),
	WC_BUTTONA, dwExStyle, dwStyle);
	if (result > 0) {
		w_font *systemfont = w_toolkit_get_system_font(
				(w_toolkit*) win_toolkit);
		SendMessageW(_W_WIDGET(widget)->handle, WM_SETFONT,
				(WPARAM) _W_FONT(systemfont)->handle, 0);
		_W_WIDGET(widget)->style = style;
		_W_WIDGET(widget)->post_event = post_event;
	}
	return result;

}
/*
 *
 */

int _BUTTON_WM_GETDLGCODE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _CONTROL_WM_GETDLGCODE(widget, e, reserved);
	if (result != W_FALSE)
		return result;
	if ((_W_WIDGET(widget)->style & W_ARROW) != 0) {
		e->result = DLGC_STATIC;
	}
	return result;
}

int _BUTTON_WM_GETOBJECT(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Ensure that there is an accessible object created for this
	 * control because support for radio button position in group
	 * accessibility is implemented in the accessibility package.
	 */
	/*if ((style & SWT.RADIO) != 0) {
	 if (accessible == null) accessible = new_Accessible (this);
	 }*/
	return _CONTROL_WM_GETOBJECT(widget, e, reserved);
}

int _BUTTON_WM_KILLFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _WIDGET_WM_KILLFOCUS(widget, e, reserved);
	/*if ((style & SWT.PUSH) != 0 && getDefault()) {
	 menuShell().setDefaultButton(null, false);
	 }*/
	return result;
}

int _BUTTON_WM_LBUTTONDOWN(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (ignoreMouse)
	 return null;*/
	return _WIDGET_WM_LBUTTONDOWN(widget, e, reserved);
}

int _BUTTON_WM_LBUTTONUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*if (ignoreMouse)
	 return null;*/
	return _WIDGET_WM_LBUTTONUP(widget, e, reserved);
}

int _BUTTON_WM_SETFOCUS(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*
	 * Feature in Windows. When Windows sets focus to
	 * a radio button, it sets the WS_TABSTOP style.
	 * This is undocumented and unwanted.  The fix is
	 * to save and restore the window style bits.
	 */
	int bits = 0;
	if ((_W_WIDGET(widget)->style & W_RADIO) != 0) {
		bits = GetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE);
	}
	int result = _WIDGET_WM_SETFOCUS(widget, e, reserved);
	if ((_W_WIDGET(widget)->style & W_RADIO) != 0) {
		SetWindowLong(_W_WIDGET(widget)->handle, GWL_STYLE, bits);
	}
	if ((_W_WIDGET(widget)->style & W_PUSH) != 0) {
		w_shell *shell;
		w_control_get_shell(W_CONTROL(widget),&shell);
		w_shell_set_default_button(shell, W_BUTTON(widget));
	}
	return result;
}

int _BUTTON_WM_SIZE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int result = _CONTROL_WM_SIZE(widget, e, reserved);
	if (result != W_FALSE)
		return result;
	/*if ((style & (SWT.PUSH | SWT.TOGGLE)) != 0) {
	 if (imageList != null && text.length() != 0) {
	 BUTTON_IMAGELIST buttonImageList = new
	 BUTTON_IMAGELIST();
	 OS.SendMessage (handle, OS.BCM_GETIMAGELIST, 0, buttonImageList);
	 buttonImageList.uAlign = OS.BUTTON_IMAGELIST_ALIGN_LEFT;
	 buttonImageList.margin_left = computeLeftMargin();
	 buttonImageList.margin_right = MARGIN;
	 OS.SendMessage (handle, OS.BCM_SETIMAGELIST, 0, buttonImageList);
	 }
	 }*/
	return result;
}

int _BUTTON_WM_SYSCOLORCHANGE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*int result = _wm_control_SYSCOMMAND(widget, e, reserved);
	 if (result != null)
	 return result;
	 if (image2 != null)
	 _setImage(image);
	 return result;*/
	return W_FALSE;
}

int _BUTTON_WM_UPDATEUISTATE(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*LRESULT
	 result = super.WM_UPDATEUISTATE (wParam, lParam);
	 if (result != null)
	 return result;*/
	/*
	 * Feature in Windows.  When WM_UPDATEUISTATE is sent to
	 * a button, it sends WM_CTLCOLORBTN to get the foreground
	 * and background.  If drawing happens in WM_CTLCOLORBTN,
	 * it will overwrite the contents of the control.  The
	 * fix is draw the button without drawing the background
	 * and avoid the button window proc.
	 *
	 * NOTE:  This only happens for radio, check and toggle
	 * buttons.
	 */
	/*if ((style & (SWT.RADIO | SWT.CHECK | SWT.TOGGLE)) != 0) {
	 boolean redraw = findImageControl() != null;
	 if (!redraw) {
	 if ((state & THEME_BACKGROUND) != 0) {
	 if (OS.IsAppThemed()) {
	 redraw = findThemeControl() != null;
	 }
	 }
	 if (!redraw)
	 redraw = findBackgroundControl() != null;
	 }
	 if (redraw) {
	 OS.InvalidateRect(handle, null, false);
	 long code = OS.DefWindowProc (handle, OS.WM_UPDATEUISTATE, wParam, lParam);
	 return new LRESULT (code);
	 }
	 }*/
	/*
	 * Feature in Windows.  Push and toggle buttons draw directly
	 * in WM_UPDATEUISTATE rather than damaging and drawing later
	 * in WM_PAINT.  This means that clients who hook WM_PAINT
	 * expecting to get all the drawing will not.  The fix is to
	 * redraw the control when paint events are hooked.
	 */
	/*if ((style & (SWT.PUSH | SWT.TOGGLE)) != 0) {
	 if (hooks(SWT.Paint) || filters(SWT.Paint) || customDrawing()) {
	 OS.InvalidateRect(handle, null, true);
	 }
	 }
	 return result;*/
	return W_FALSE;
}

int _BUTTON_WM_COMMANDCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	int code = HIWORD(e->wparam);
	switch (code) {
	case BN_CLICKED:
	case BN_DOUBLECLICKED:
		if ((_W_WIDGET(widget)->style & (W_CHECK | W_TOGGLE)) != 0) {
			_w_button_set_selection(W_BUTTON(widget),
					!_w_button_get_selection(W_BUTTON(widget)));
		} else {
			if ((_W_WIDGET(widget)->style & W_RADIO) != 0) {
				w_composite *parent = _W_CONTROL(widget)->parent;
				if ((_W_WIDGET(parent)->style & W_NO_RADIO_GROUP) != 0) {
					_w_button_set_selection(W_BUTTON(widget),
							!_w_button_get_selection(W_BUTTON(widget)));
				} else {
					_w_button_select_radio(W_BUTTON(widget));
					//selectRadio();
				}
			}
		}
		w_event event;
		memset(&event, 0, sizeof(event));
		event.type = W_EVENT_SELECTION;
		event.platform_event = (struct w_event_platform*) e;
		event.widget = widget;
		_w_widget_send_event(widget, &event);
	}
	return W_FALSE;
}

int _BUTTON_WM_NOTIFYCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*	switch (hdr.code) {
	 case OS.NM_CUSTOMDRAW:
	 // this message will not appear for owner-draw buttons (currently the ARROW button).

	 NMCUSTOMDRAW nmcd = new NMCUSTOMDRAW ();
	 OS.MoveMemory (nmcd, lParam, NMCUSTOMDRAW.sizeof);

	 switch (nmcd.dwDrawStage) {
	 case OS.CDDS_PREPAINT: {
	 // buttons are ignoring SetBkColor, SetBkMode and SetTextColor
	 if (customBackgroundDrawing()) {
	 int pixel = buttonBackground;
	 if ((nmcd.uItemState & OS.CDIS_SELECTED) != 0) {
	 pixel = getDifferentColor(buttonBackground);
	 } else if ((nmcd.uItemState & OS.CDIS_HOT) != 0) {
	 pixel = getSlightlyDifferentColor(buttonBackground);
	 }
	 if ((style & SWT.TOGGLE) != 0 && isChecked()) {
	 pixel = getDifferentColor(buttonBackground);
	 }
	 RECT rect = new RECT ();
	 OS.SetRect (rect, nmcd.left+2, nmcd.top+2, nmcd.right-2, nmcd.bottom-2);
	 long brush = OS.CreateSolidBrush(pixel);
	 OS.FillRect(nmcd.hdc, rect, brush);
	 OS.DeleteObject(brush);
	 }
	 if (customForegroundDrawing()) {*/
	/*
	 * Check-box/Radio buttons are native widget which honors
	 * the Win OS zoom level for both 'Square' and 'Text' part
	 * [Note: By-design SWT doesn't control native auto-scaling]
	 * Hence, custom fore-ground draw logic should auto-scale
	 * text-padding as per OS Native DPI level to fix bug 506371
	 */
	/*int radioOrCheckTextPadding = DPIUtil.autoScaleUpUsingNativeDPI(16);
	 int border = isRadioOrCheck() ? 0 : 3;
	 int left = nmcd.left + border;
	 int right = nmcd.right - border;
	 if (image != null) {
	 GCData data = new GCData();
	 data.device = display;
	 GC gc = GC.win32_new (nmcd.hdc, data);

	 int margin = computeLeftMargin();
	 int imageWidth = image.getBoundsInPixels().width;
	 left += (imageWidth + (isRadioOrCheck() ? 2 * MARGIN : MARGIN)); // for SWT.RIGHT_TO_LEFT right and left are inverted

	 int x = margin + (isRadioOrCheck() ? radioOrCheckTextPadding : 3);
	 int y = Math.max (0, (nmcd.bottom - image.getBoundsInPixels().height) / 2);
	 gc.drawImage (image, DPIUtil.autoScaleDown(x), DPIUtil.autoScaleDown(y));
	 gc.dispose ();
	 }

	 left += isRadioOrCheck() ? radioOrCheckTextPadding : 0;
	 RECT textRect = new RECT ();
	 OS.SetRect (textRect, left, nmcd.top + border, right, nmcd.bottom - border);

	 // draw text
	 TCHAR buffer = new TCHAR (getCodePage (), text, false);
	 int flags = 0;
	 if ((style & SWT.WRAP) != 0) {
	 flags |= OS.DT_WORDBREAK;
	 if (!isRadioOrCheck() && image != null) {
	 textRect.right -= MARGIN;
	 }
	 } else {
	 flags |= OS.DT_SINGLELINE; // TODO: this always draws the prefix
	 }
	 OS.DrawText(nmcd.hdc, buffer, buffer.length(), textRect, flags | OS.DT_CALCRECT);
	 OS.OffsetRect(textRect, 0, Math.max(0, (nmcd.bottom - textRect.bottom - border) / 2));
	 if (image != null) {
	 // The default button with an image doesn't respect the text alignment. So we do the same for styled buttons.
	 flags |= OS.DT_LEFT;
	 if (!isRadioOrCheck()) {
	 OS.OffsetRect(textRect, Math.max(MARGIN, (right - textRect.right) / 2 + 1), 0);
	 }
	 } else if ((style & SWT.LEFT) != 0) {
	 flags |= OS.DT_LEFT;
	 } else if ((style & SWT.RIGHT) != 0) {
	 flags |= OS.DT_RIGHT;
	 OS.OffsetRect(textRect, right - textRect.right, 0);
	 } else {
	 flags |= OS.DT_CENTER;
	 OS.OffsetRect(textRect, (right - textRect.right) / 2, 0);
	 }
	 OS.SetBkMode(nmcd.hdc, OS.TRANSPARENT);
	 OS.SetTextColor(nmcd.hdc, foreground);
	 OS.DrawText(nmcd.hdc, buffer, buffer.length(), textRect, flags);

	 // draw focus rect
	 if ((nmcd.uItemState & OS.CDIS_FOCUS) != 0) {
	 RECT focusRect = new RECT ();
	 if (isRadioOrCheck()) {
	 if (text.length() > 0) {
	 OS.SetRect(focusRect, textRect.left-1, textRect.top, Math.min(nmcd.right, textRect.right+1), Math.min(nmcd.bottom, textRect.bottom+1));
	 } else {*/
	/*
	 * With custom foreground, draw focus rectangle for CheckBox
	 * and Radio buttons considering the native text padding
	 * value(which is DPI aware). See bug 508141 for details.
	 */
	/*OS.SetRect (focusRect, nmcd.left+1+radioOrCheckTextPadding, nmcd.top, nmcd.right-2, nmcd.bottom-1);
	 }
	 } else {
	 OS.SetRect (focusRect, nmcd.left+2, nmcd.top+3, nmcd.right-2, nmcd.bottom-3);
	 }
	 OS.DrawFocusRect(nmcd.hdc, focusRect);
	 }
	 return new LRESULT (OS.CDRF_SKIPDEFAULT);
	 }
	 return new LRESULT (OS.CDRF_DODEFAULT);
	 }
	 }
	 break;
	 }
	 return super.wmNotifyChild(hdr, wParam, lParam);*/
	return W_FALSE;
}

int _BUTTON_WM_DRAWCHILD(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	if ((_W_WIDGET(widget)->style & W_ARROW) == 0)
		return W_FALSE;
	DRAWITEMSTRUCT *item = (DRAWITEMSTRUCT*) e->lparam;
	if (IsAppThemed()) {
		int iStateId = ABS_LEFTNORMAL;
		switch (_W_WIDGET(widget)->style & (W_UP | W_DOWN | W_LEFT | W_RIGHT)) {
		case W_UP:
			iStateId = ABS_UPNORMAL;
			break;
		case W_DOWN:
			iStateId = ABS_DOWNNORMAL;
			break;
		case W_LEFT:
			iStateId = ABS_LEFTNORMAL;
			break;
		case W_RIGHT:
			iStateId = ABS_RIGHTNORMAL;
			break;
		}
		/*
		 * Feature in Windows.  DrawThemeBackground() does not mirror the drawing.
		 * The fix is switch left to right and right to left.
		 */
		if ((_W_WIDGET(widget)->style & W_MIRRORED) != 0) {
			if ((_W_WIDGET(widget)->style & (W_LEFT | W_RIGHT)) != 0) {
				iStateId =
						iStateId == ABS_RIGHTNORMAL ?
								ABS_LEFTNORMAL : ABS_RIGHTNORMAL;
			}
		}
		/*
		 * NOTE: The normal, hot, pressed and disabled state is
		 * computed relying on the fact that the increment between
		 * the direction states is invariant (always separated by 4).
		 */
		if (!IsWindowEnabled(_W_WIDGET(widget)->handle))
			iStateId += ABS_UPDISABLED - ABS_UPNORMAL;
		if ((item->itemState & ODS_SELECTED) != 0)
			iStateId += ABS_UPPRESSED - ABS_UPNORMAL;
		HTHEME hTheme = _w_theme_get_htheme(_W_THEME_CLASS_SCROLLBAR,
				"SCROLLBAR");
		DrawThemeBackground(hTheme, item->hDC, SBP_ARROWBTN, iStateId,
				&item->rcItem, NULL);
	} else {
		int uState = DFCS_SCROLLLEFT;
		switch (_W_WIDGET(widget)->style & (W_UP | W_DOWN | W_LEFT | W_RIGHT)) {
		case W_UP:
			uState = DFCS_SCROLLUP;
			break;
		case W_DOWN:
			uState = DFCS_SCROLLDOWN;
			break;
		case W_LEFT:
			uState = DFCS_SCROLLLEFT;
			break;
		case W_RIGHT:
			uState = DFCS_SCROLLRIGHT;
			break;
		}
		if (!IsWindowEnabled(_W_WIDGET(widget)->handle))
			uState |= DFCS_INACTIVE;
		if ((_W_WIDGET(widget)->style & W_FLAT) == W_FLAT)
			uState |= DFCS_FLAT;
		if ((item->itemState & ODS_SELECTED) != 0)
			uState |= DFCS_PUSHED;
		DrawFrameControl(item->hDC, &item->rcItem, DFC_SCROLL, uState);
	}
	return W_FALSE;
}
#define OBM_CHECKBOXES 0x7ff7
void _w_button_init_check_widths() {
	if (CHECK_WIDTH != 0 || CHECK_HEIGHT != 0)
		return;
	HBITMAP hBitmap = LoadBitmapW(NULL, (LPCWSTR) OBM_CHECKBOXES);
	if (hBitmap == 0) {
#if IsWinCE
		CHECK_WIDTH = GetSystemMetrics (OS.IsWinCE ? SM_CXSMICON : SM_CXVSCROLL);
		CHECK_HEIGHT = GetSystemMetrics (OS.IsWinCE ? SM_CYSMICON : SM_CYVSCROLL);
#else
		CHECK_WIDTH = GetSystemMetrics(SM_CXVSCROLL);
		CHECK_HEIGHT = GetSystemMetrics(SM_CYVSCROLL);
#endif
	} else {
		BITMAP bitmap;
		GetObjectW(hBitmap, sizeof(BITMAP), &bitmap);
		DeleteObject(hBitmap);
		CHECK_WIDTH = bitmap.bmWidth / 4;
		CHECK_HEIGHT = bitmap.bmHeight / 3;
	}
}
int _w_button_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	int width = 0, height = 0;
	int border = _w_control_get_border_width(W_CONTROL(widget));
	if ((_W_WIDGET(widget)->style & W_ARROW) != 0) {
		if ((_W_WIDGET(widget)->style & (W_UP | W_DOWN)) != 0) {
			width += GetSystemMetrics(SM_CXVSCROLL);
			height += GetSystemMetrics(SM_CYVSCROLL);
		} else {
			width += GetSystemMetrics(SM_CXHSCROLL);
			height += GetSystemMetrics(SM_CYHSCROLL);
		}
	} else {
		if ((_W_WIDGET(widget)->style & W_COMMAND) != 0) {
			SIZE size;
			if (wHint != W_DEFAULT) {
				size.cx = wHint;
				SendMessageW(_W_WIDGET(widget)->handle, BCM_GETIDEALSIZE, 0,
						(LPARAM) &size);
				width = size.cx;
				height = size.cy;
			} else {
				SendMessageW(_W_WIDGET(widget)->handle, BCM_GETIDEALSIZE, 0,
						(LPARAM) &size);
				width = size.cy;
				height = size.cy;
				size.cy = 0;
				while (size.cy != height) {
					size.cx = width++;
					size.cy = 0;
					SendMessageW(_W_WIDGET(widget)->handle, BCM_GETIDEALSIZE,
							0, (LPARAM) &size);
				}
			}
		} else {
			int extra = 0;
			//boolean hasImage = image != null;
			boolean hasText = TRUE;
			/*if (COMCTL32_MAJOR < 6) {
			 if ((style & SWT.PUSH) == 0) {
			 int bits = OS.GetWindowLong (handle, OS.GWL_STYLE);
			 hasImage = (bits & (OS.BS_BITMAP | OS.BS_ICON)) != 0;
			 if (hasImage) hasText = false;
			 }
			 }*/
			/*if (hasImage) {
			 if (image != null) {
			 Rectangle rect = image.getBoundsInPixels ();
			 width = rect.width;
			 if (hasText && text.length () != 0) {
			 width += MARGIN * 2;
			 }
			 height = rect.height;
			 extra = MARGIN * 2;
			 }
			 }*/
			if (hasText) {
				RECT rect;
				HFONT oldFont = 0;
				HDC hDC = GetDC(_W_WIDGET(widget)->handle);
				HFONT newFont = (HFONT) SendMessageW(_W_WIDGET(widget)->handle,
				WM_GETFONT, 0, 0);
				if (newFont != 0)
					oldFont = SelectObject(hDC, newFont);
				TEXTMETRICW lptm;
				GetTextMetricsW(hDC, &lptm);
				int length = GetWindowTextLengthW(_W_WIDGET(widget)->handle);
				if (length == 0) {
					height = w_max(height, lptm.tmHeight);
				} else {
					WCHAR *buffer;
					buffer = _w_toolkit_malloc((length + 1) * sizeof(WCHAR));
					if (buffer != 0) {
						GetWindowTextW(_W_WIDGET(widget)->handle, buffer,
								length + 1);
						extra = w_max(MARGIN * 2, lptm.tmAveCharWidth);
						int flags = DT_CALCRECT | DT_SINGLELINE;
						if ((_W_WIDGET(widget)->style & W_WRAP) != 0
								&& wHint != W_DEFAULT) {
							flags = DT_CALCRECT | DT_WORDBREAK;
							rect.right = wHint - width - 2 * border;
							if ((_W_WIDGET(widget)->style & (W_CHECK | W_RADIO))
									!= 0) {
								_w_button_init_check_widths();
								rect.right -= CHECK_WIDTH + 3;
							} else {
								rect.right -= 6;
							}
							if (W_COMCTL32_VERSION < VERSION(6, 0)
									|| !IsAppThemed()) {
								rect.right -= 2;
								if ((_W_WIDGET(widget)->style
										& (W_CHECK | W_RADIO)) != 0) {
									rect.right -= 2;
								}
							}
						}
						DrawTextW(hDC, buffer, -1, &rect, flags);
						_w_toolkit_free(buffer,(length + 1) * sizeof(WCHAR));
					}
					width += rect.right - rect.left;
					height = w_max(height, rect.bottom - rect.top);
				}
				if (newFont != 0)
					SelectObject(hDC, oldFont);
				ReleaseDC(_W_WIDGET(widget)->handle, hDC);
			}
			if ((_W_WIDGET(widget)->style & (W_CHECK | W_RADIO)) != 0) {
				_w_button_init_check_widths();
				width += CHECK_WIDTH + extra;
				height = w_max(height, CHECK_HEIGHT + 3);
			}
			if ((_W_WIDGET(widget)->style & (W_PUSH | W_TOGGLE)) != 0) {
				width += 12;
				height += 10;
			}
		}
	}
	if (wHint != W_DEFAULT)
		width = wHint;
	if (hHint != W_DEFAULT)
		height = hHint;
	width += border * 2;
	height += border * 2;
	e->size->width = width;
	e->size->height = height;
	return W_TRUE;
}
void _w_button_class_init(struct _w_button_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BUTTON;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_button_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_button);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_button);
	/*
	 * public function
	 */
	clazz->get_alignment = _w_button_get_alignment;
	clazz->get_grayed = _w_button_get_grayed;
	clazz->get_image = _w_button_get_image;
	clazz->get_selection = _w_button_get_selection;
	clazz->get_text = _w_button_get_text;
	clazz->set_alignment = _w_button_set_alignment;
	clazz->set_grayed = _w_button_set_grayed;
	clazz->set_image = _w_button_set_image;
	clazz->set_selection = _w_button_set_selection;
	clazz->set_text = _w_button_set_text;
	W_WIDGET_CLASS(clazz)->create = _w_button_create;
	//reserved
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->compute_size = _w_button_compute_size;
	//messages
	struct _w_control_reserved *msg = reserved;
	msg->messages[_WM_GETDLGCODE] = _BUTTON_WM_GETDLGCODE;
	msg->messages[_WM_GETOBJECT] = _BUTTON_WM_GETOBJECT;
	msg->messages[_WM_KILLFOCUS] = _BUTTON_WM_KILLFOCUS;
	msg->messages[_WM_LBUTTONDOWN] = _BUTTON_WM_LBUTTONDOWN;
	msg->messages[_WM_LBUTTONUP] = _BUTTON_WM_LBUTTONUP;
	msg->messages[_WM_SETFOCUS] = _BUTTON_WM_SETFOCUS;
	msg->messages[_WM_SIZE] = _BUTTON_WM_SIZE;
	msg->messages[_WM_SYSCOLORCHANGE] = _BUTTON_WM_SYSCOLORCHANGE;
	msg->messages[_WM_UPDATEUISTATE] = _BUTTON_WM_UPDATEUISTATE;
	msg->messages[_WM_COMMANDCHILD] = _BUTTON_WM_COMMANDCHILD;
	msg->messages[_WM_NOTIFYCHILD] = _BUTTON_WM_NOTIFYCHILD;
	msg->messages[_WM_DRAWCHILD] = _BUTTON_WM_DRAWCHILD;
}
#endif

