/*
 * toolkit1.c
 *
 *  Created on: 9 mai 2020
 *      Author: azeddine El Bassbasi
 */
#include "toolkit.h"
#if defined( __WIN32__) || defined(__WIN64__)
#ifdef __cplusplus
extern "C" {
#endif
_w_toolkit *win_toolkit = 0;
const char *WindowClass = "SWT_Window"; //$NON-NLS-1$
const char *WindowShadowClass = "SWT_WindowShadow"; //$NON-NLS-1$
const char *WindowOwnDCName = "SWT_WindowOwnDC"; //$NON-NLS-1$
typedef struct _DLLVERSIONINFO {
	DWORD cbSize;
	DWORD dwMajorVersion;
	DWORD dwMinorVersion;
	DWORD dwBuildNumber;
	DWORD dwPlatformID;
} DLLVERSIONINFO;
typedef VOID WINAPI (*DllGetVersion)(DLLVERSIONINFO *pcdvi);
int _w_toolkit_get_dll_version(const char *dll) {
	DLLVERSIONINFO dvi;
	dvi.cbSize = sizeof(DLLVERSIONINFO);
	dvi.dwMajorVersion = 4;
	dvi.dwMinorVersion = 0;
	WCHAR tmp[0x100];
	w_utf8_to_utf16(dll, -1, tmp, sizeof(tmp) / sizeof(tmp[0]));
	HMODULE hModule = LoadLibraryW(tmp);
	if (hModule != 0) {
		DllGetVersion dllGetVersion = GetProcAddress(hModule, "DllGetVersion");
		if (dllGetVersion != 0) {
			dllGetVersion(&dvi);
		}
		FreeLibrary(hModule);
	}
	return VERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
}
void _w_toolkit_init_dll(_w_toolkit *toolkit) {
	/* enabling visual style */
	/*ACTCTXW actCtx;
	 ULONG_PTR cookie;
	 actCtx.cbSize = sizeof(actCtx);
	 actCtx.dwFlags =
	 ACTCTX_FLAG_HMODULE_VALID | ACTCTX_FLAG_RESOURCE_NAME_VALID;
	 actCtx.lpSource = NULL;
	 actCtx.wProcessorArchitecture = 0;
	 actCtx.wLangId = 0;
	 actCtx.lpAssemblyDirectory = NULL;
	 actCtx.lpResourceName = MAKEINTRESOURCEW(MY_MANIFEST);
	 actCtx.lpApplicationName = NULL;
	 actCtx.hModule = hinst;
	 HANDLE hactCtx = CreateActCtxW(&actCtx);
	 if (hactCtx != INVALID_HANDLE_VALUE) {
	 ActivateActCtx(hactCtx, &cookie);
	 }*/
	//GetStartupInfoW(&toolkit->lpStartUpInfo);
	INITCOMMONCONTROLSEX picce;
	picce.dwSize = sizeof(picce);
	picce.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES
			| ICC_NATIVEFNTCTL_CLASS;
	InitCommonControlsEx(&picce);
	OleInitialize(NULL);
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput input;
	GdiplusStartupOutput output;
	memset(&input, 0, sizeof(input));
	memset(&output, 0, sizeof(output));
	input.GdiplusVersion = 1;
	GdiplusStartup(&gdiplusToken, &input, &output);
	OSVERSIONINFOA info;
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);
	if (GetVersionExA(&info)) {
		toolkit->win32_version = VERSION(info.dwMajorVersion,
				info.dwMinorVersion);
	}
	toolkit->comctrl32_version = _w_toolkit_get_dll_version("comctl32.dll");
	/* Get the DBCS flag */
	int dbcsEnabled = GetSystemMetrics (SM_DBCSENABLED) != 0;
	int immEnabled = GetSystemMetrics (SM_IMMENABLED) != 0;
	win_toolkit->IsDBLocale = dbcsEnabled || immEnabled;

}
void* _w_toolkit_malloc(size_t size) {
	if ((win_toolkit->tmp_length + size) < win_toolkit->tmp_alloc) {
		int i = win_toolkit->tmp_length;
		win_toolkit->tmp_length += size;
		return &win_toolkit->tmp[i];
	} else {
		return malloc(size);
	}
}
void* _w_toolkit_malloc_all(size_t *size) {
	*size = win_toolkit->tmp_alloc - win_toolkit->tmp_length;
	int i = win_toolkit->tmp_length;
	win_toolkit->tmp_length += *size;
	return &win_toolkit->tmp[i];
}
void _w_toolkit_free(void *ptr, size_t size) {
	wintptr diff = ptr - (void*) win_toolkit->tmp;
	if (diff >= 0 && diff < win_toolkit->tmp_alloc) {
		win_toolkit->tmp_length -= size;
	} else
		free(ptr);
}
void _w_toolkit_add_shell(_w_shell *shell) {
	shell->next = 0;
	if (win_toolkit->shells == 0) {
		win_toolkit->shells = shell;
		shell->prev = shell; //last
	} else {
		_w_shell *last = win_toolkit->shells->prev;
		last->next = shell;
		shell->prev = last;
		win_toolkit->shells->prev = shell;
	}
	win_toolkit->shells_count++;
}
void _w_toolkit_remove_shell(_w_shell *shell) {
	if (shell == win_toolkit->shells) {
		win_toolkit->shells = shell->next;
		if (win_toolkit->shells != 0)
			win_toolkit->shells->prev = shell->prev; //last
	} else {
		if (shell->next == 0) {
			win_toolkit->shells->prev = shell->prev;
		} else {
			shell->next->prev = shell->prev;
		}
		shell->prev->next = shell->next;
	}
	win_toolkit->shells_count--;
}
wresult _w_toolkit_messagebox_open(w_toolkit *toolkit,
		w_messagebox *messagebox) {
	if (messagebox->parent != 0) {
		if (w_widget_is_ok(W_WIDGET(messagebox->parent)) < 0)
			return W_ERROR_INVALID_ARGUMENT;
	}
	/* Compute the MessageBox style */
	int buttonBits = 0;
	wuint64 style = messagebox->style;
	if ((style & W_OK) == W_OK)
		buttonBits = MB_OK;
	if ((style & (W_OK | W_CANCEL)) == (W_OK | W_CANCEL))
		buttonBits = MB_OKCANCEL;
	if ((style & (W_YES | W_NO)) == (W_YES | W_NO))
		buttonBits = MB_YESNO;
	if ((style & (W_YES | W_NO | W_CANCEL)) == (W_YES | W_NO | W_CANCEL))
		buttonBits = MB_YESNOCANCEL;
	if ((style & (W_RETRY | W_CANCEL)) == (W_RETRY | W_CANCEL))
		buttonBits = MB_RETRYCANCEL;
	if ((style & (W_ABORT | W_RETRY | W_IGNORE))
			== (W_ABORT | W_RETRY | W_IGNORE))
		buttonBits = MB_ABORTRETRYIGNORE;
	if (buttonBits == 0)
		buttonBits = MB_OK;

	int iconBits = 0;
	if ((style & W_ICON_ERROR) != 0)
		iconBits = MB_ICONERROR;
	if ((style & W_ICON_INFORMATION) != 0)
		iconBits = MB_ICONINFORMATION;
	if ((style & W_ICON_QUESTION) != 0)
		iconBits = MB_ICONQUESTION;
	if ((style & W_ICON_WARNING) != 0)
		iconBits = MB_ICONWARNING;
	if ((style & W_ICON_WORKING) != 0)
		iconBits = MB_ICONINFORMATION;

	/* Only MB_APPLMODAL is supported on WinCE */
	int modalBits = 0;
	/*if (IsWinCE) {
	 if ((style & (W_PRIMARY_MODAL | W_APPLICATION_MODAL | W_SYSTEM_MODAL)) != 0) {
	 modalBits = MB_APPLMODAL;
	 }
	 } else {*/
	if ((style & W_PRIMARY_MODAL) != 0)
		modalBits = MB_APPLMODAL;
	if ((style & W_APPLICATION_MODAL) != 0)
		modalBits = MB_TASKMODAL;
	if ((style & W_SYSTEM_MODAL) != 0)
		modalBits = MB_SYSTEMMODAL;
	//}

	int bits = buttonBits | iconBits | modalBits;
	if ((style & W_RIGHT_TO_LEFT) != 0)
		bits |= MB_RTLREADING | MB_RIGHT;
	if ((style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)) == 0) {
		if (messagebox->parent != 0
				&& (_W_WIDGET(messagebox->parent)->style & W_MIRRORED) != 0) {
			bits |= MB_RTLREADING | MB_RIGHT;
		}
	}

	/*
	 * Feature in Windows.  System modal is not supported
	 * on Windows 95 and NT.  The fix is to convert system
	 * modal to task modal.
	 */
	if ((bits & MB_SYSTEMMODAL) != 0) {
		bits |= MB_TASKMODAL;
		bits &= ~MB_SYSTEMMODAL;
		/* Force a system modal message box to the front */
		bits |= MB_TOPMOST;
	}

	/*
	 * Feature in Windows.  In order for MB_TASKMODAL to work,
	 * the parent HWND of the MessageBox () call must be NULL.
	 * If the parent is not NULL, MB_TASKMODAL behaves the
	 * same as MB_APPLMODAL.  The fix to set the parent HWND
	 * anyway and not rely on MB_MODAL to work by making the
	 * parent be temporarily modal.
	 */
	HWND hwndOwner = messagebox->parent != 0 ?
	_W_WIDGET(messagebox->parent)->handle :
												0;
	/*Display display = parent != null ? parent.getDisplay (): Display.getCurrent ();
	 Dialog oldModal = null;
	 if ((bits & MB_TASKMODAL) != 0) {
	 oldModal = display.getModalDialog ();
	 display.setModalDialog (this);
	 }*/

	/* Use the character encoding for the default locale */
	struct UnicodeString message;
	struct UnicodeString title;
	unicode_set(&message, messagebox->message, -1);
	unicode_set(&title, messagebox->title, -1);
	int code = MessageBoxW(hwndOwner, message.str, title.str, bits);

	/* Clear the temporarily dialog modal parent */
	if ((bits & MB_TASKMODAL) != 0) {
		//display.setModalDialog (oldModal);
	}

	/*
	 * This code is intentionally commented.  On some
	 * platforms, the owner window is repainted right
	 * away when a dialog window exits.  This behavior
	 * is currently unspecified.
	 */
//	if (hwndOwner != 0) UpdateWindow (hwndOwner);
	/* Compute and return the result */
	if (code != 0) {
		int type = bits & 0x0F;
		if (type == MB_OK)
			return W_OK;
		if (type == MB_OKCANCEL) {
			return (code == IDOK) ? W_OK : W_CANCEL;
		}
		if (type == MB_YESNO) {
			return (code == IDYES) ? W_YES : W_NO;
		}
		if (type == MB_YESNOCANCEL) {
			if (code == IDYES)
				return W_YES;
			if (code == IDNO)
				return W_NO;
			return W_CANCEL;
		}
		if (type == MB_RETRYCANCEL) {
			return (code == IDRETRY) ? W_RETRY : W_CANCEL;
		}
		if (type == MB_ABORTRETRYIGNORE) {
			if (code == IDRETRY)
				return W_RETRY;
			if (code == IDABORT)
				return W_ABORT;
			return W_IGNORE;
		}
	}
	return W_CANCEL;
}
void _w_toolkit_registre_class(_w_toolkit *toolkit) {
	if (toolkit->is_class_registred == 1)
		return;
	struct UnicodeString c;
	WNDCLASSEXW wincl;
	ATOM atom;
	wincl.hInstance = hinst;
	wincl.lpfnWndProc = _w_widget_window_proc;
	wincl.style = CS_DBLCLKS;
	wincl.cbSize = sizeof(WNDCLASSEXW);
	wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.lpszMenuName = NULL;
	wincl.cbClsExtra = 0;
	wincl.cbWndExtra = 0;
	wincl.hbrBackground = (HBRUSH) (COLOR_3DFACE + 1);
	unicode_set(&c, WindowClass, -1);
	wincl.lpszClassName = c.str;
	atom = RegisterClassExW(&wincl);
	unicode_free(&c);
	if (atom == 0) {
		fprintf(stderr, "Error registre Atom %s", WindowClass);
		exit(0);
		return;
	}
	unicode_set(&c, WindowShadowClass, -1);
	wincl.lpszClassName = c.str;
	atom = RegisterClassExW(&wincl);
	unicode_free(&c);
	if (atom == 0) {
		fprintf(stderr, "Error registre Atom %s", WindowShadowClass);
		exit(0);
		return;
	}
	unicode_set(&c, WindowOwnDCName, -1);
	wincl.style |= CS_OWNDC;
	wincl.lpszClassName = c.str;
	atom = RegisterClassExW(&wincl);
	unicode_free(&c);
	if (atom == 0) {
		fprintf(stderr, "Error registre Atom %s", WindowOwnDCName);
		exit(0);
		return;
	}
	toolkit->is_class_registred = 1;
}
void _w_toolkit_class_init(_w_toolkit *toolkit) {
	struct _w_toolkit_class *clazz = &toolkit->class_toolkit;
	((w_disposable*) toolkit)->clazz = (struct w_disposable_class*) clazz;
	clazz->check_widget = _w_toolkit_check_widget;
	clazz->check_widgetdata = _w_toolkit_check_widgetdata;
	clazz->get_class = _w_toolkit_get_class;
	clazz->get_theme = _w_toolkit_get_theme;
	clazz->async_exec = _w_toolkit_async_exec;
	clazz->beep = _w_toolkit_beep;
	clazz->close = _w_toolkit_close;
	clazz->get_active_shell = _w_toolkit_get_active_shell;
	clazz->get_menubar = _w_toolkit_get_menubar;
	clazz->get_bounds = _w_toolkit_get_bounds;
	clazz->get_client_area = _w_toolkit_get_client_area;
	clazz->get_cursor_control = _w_toolkit_get_cursor_control;
	clazz->get_cursor_location = _w_toolkit_get_cursor_location;
	clazz->get_cursor_sizes = _w_toolkit_get_cursor_sizes;
	clazz->get_dismissal_alignment = _w_toolkit_get_dismissal_alignment;
	clazz->get_double_click_time = _w_toolkit_get_double_click_time;
	clazz->get_focus_control = _w_toolkit_get_focus_control;
	clazz->get_high_contrast = _w_toolkit_get_high_contrast;
	clazz->get_icon_depth = _w_toolkit_get_icon_depth;
	clazz->get_icon_sizes = _w_toolkit_get_icon_sizes;
	clazz->get_shells = _w_toolkit_get_shells;
	clazz->get_system_color = _w_toolkit_get_system_color;
	clazz->get_system_cursor = _w_toolkit_get_system_cursor;
	clazz->get_system_font = _w_toolkit_get_system_font;
	clazz->get_system_image = _w_toolkit_get_system_image;
	clazz->get_system_menu = _w_toolkit_get_system_menu;
	clazz->get_system_taskbar = _w_toolkit_get_system_taskbar;
	clazz->get_system_tray = _w_toolkit_get_system_tray;
	clazz->get_theme = _w_toolkit_get_theme;
	clazz->get_thread = _w_toolkit_get_thread;
	clazz->get_thread_id = _w_toolkit_get_thread_id;
	clazz->get_touch_enabled = _w_toolkit_get_touch_enabled;
	clazz->map_0 = _w_toolkit_map_0;
	clazz->map_1 = _w_toolkit_map_1;
	clazz->post = _w_toolkit_post;
	clazz->post_quit = _w_toolkit_post_quit;
	clazz->read_and_dispatch = _w_toolkit_read_and_dispatch;
	clazz->set_cursor_location = _w_toolkit_set_cursor_location;
	clazz->run = _w_toolkit_run;
	clazz->sleep = _w_toolkit_sleep;
	clazz->sync_exec = _w_toolkit_sync_exec;
	clazz->timer_exec = _w_toolkit_timer_exec;
	clazz->update = _w_toolkit_update;
	clazz->wake = _w_toolkit_wake;

	clazz->messagebox_open = _w_toolkit_messagebox_open;
}
void _w_toolkit_init_messages(struct _w_toolkit *toolkit) {
	toolkit->wm_msg[WM_ACTIVATE] = _WM_ACTIVATE;
	toolkit->wm_msg[WM_CAPTURECHANGED] = _WM_CAPTURECHANGED;
	toolkit->wm_msg[WM_CHANGEUISTATE] = _WM_CHANGEUISTATE;
	toolkit->wm_msg[WM_CHAR] = _WM_CHAR;
	toolkit->wm_msg[WM_CLEAR] = _WM_CLEAR;
	toolkit->wm_msg[WM_CLOSE] = _WM_CLOSE;
	toolkit->wm_msg[WM_COMMAND] = _WM_COMMAND;
	toolkit->wm_msg[WM_CONTEXTMENU] = _WM_CONTEXTMENU;
	toolkit->wm_msg[WM_CTLCOLORMSGBOX] = _WM_CTLCOLOR;
	toolkit->wm_msg[WM_CTLCOLOREDIT] = _WM_CTLCOLOR;
	toolkit->wm_msg[WM_CTLCOLORLISTBOX] = _WM_CTLCOLOR;
	toolkit->wm_msg[WM_CTLCOLORBTN] = _WM_CTLCOLOR;
	toolkit->wm_msg[WM_CTLCOLORDLG] = _WM_CTLCOLOR;
	toolkit->wm_msg[WM_CTLCOLORSCROLLBAR] = _WM_CTLCOLOR;
	toolkit->wm_msg[WM_CTLCOLORSTATIC] = _WM_CTLCOLOR;
	toolkit->wm_msg[WM_CUT] = _WM_CUT;
	toolkit->wm_msg[WM_DESTROY] = _WM_DESTROY;
	toolkit->wm_msg[WM_DRAWITEM] = _WM_DRAWITEM;
	toolkit->wm_msg[WM_ENDSESSION] = _WM_ENDSESSION;
	toolkit->wm_msg[WM_ENTERIDLE] = _WM_ENTERIDLE;
	toolkit->wm_msg[WM_ERASEBKGND] = _WM_ERASEBKGND;
	//toolkit->wm_msg[WM_GESTURE] = W_WM_GESTURE;
	toolkit->wm_msg[WM_GETDLGCODE] = _WM_GETDLGCODE;
	toolkit->wm_msg[WM_GETFONT] = _WM_GETFONT;
	toolkit->wm_msg[WM_GETOBJECT] = _WM_GETOBJECT;
	toolkit->wm_msg[WM_GETMINMAXINFO] = _WM_GETMINMAXINFO;
	toolkit->wm_msg[WM_HELP] = _WM_HELP;
	toolkit->wm_msg[WM_HSCROLL] = _WM_HSCROLL;
	toolkit->wm_msg[WM_IME_CHAR] = _WM_IME_CHAR;
	toolkit->wm_msg[WM_IME_COMPOSITION] = _WM_IME_COMPOSITION;
	//toolkit->wm_msg[WM_IME_COMPOSITION_START] = W_WM_IME_COMPOSITION_START;
	toolkit->wm_msg[WM_IME_ENDCOMPOSITION] = _WM_IME_ENDCOMPOSITION;
	toolkit->wm_msg[WM_INITMENUPOPUP] = _WM_INITMENUPOPUP;
	toolkit->wm_msg[WM_INPUTLANGCHANGE] = _WM_INPUTLANGCHANGE;
	toolkit->wm_msg[WM_HOTKEY] = _WM_HOTKEY;
	toolkit->wm_msg[WM_KEYDOWN] = _WM_KEYDOWN;
	toolkit->wm_msg[WM_KEYUP] = _WM_KEYUP;
	toolkit->wm_msg[WM_KILLFOCUS] = _WM_KILLFOCUS;
	toolkit->wm_msg[WM_LBUTTONDBLCLK] = _WM_LBUTTONDBLCLK;
	toolkit->wm_msg[WM_LBUTTONDOWN] = _WM_LBUTTONDOWN;
	toolkit->wm_msg[WM_LBUTTONUP] = _WM_LBUTTONUP;
	toolkit->wm_msg[WM_MBUTTONDBLCLK] = _WM_MBUTTONDBLCLK;
	toolkit->wm_msg[WM_MBUTTONDOWN] = _WM_MBUTTONDOWN;
	toolkit->wm_msg[WM_MBUTTONUP] = _WM_MBUTTONUP;
	toolkit->wm_msg[WM_MEASUREITEM] = _WM_MEASUREITEM;
	toolkit->wm_msg[WM_MENUCHAR] = _WM_MENUCHAR;
	toolkit->wm_msg[WM_MENUSELECT] = _WM_MENUSELECT;
	toolkit->wm_msg[WM_MOUSEACTIVATE] = _WM_MOUSEACTIVATE;
	toolkit->wm_msg[WM_MOUSEHOVER] = _WM_MOUSEHOVER;
	toolkit->wm_msg[WM_MOUSELEAVE] = _WM_MOUSELEAVE;
	toolkit->wm_msg[WM_MOUSEMOVE] = _WM_MOUSEMOVE;
	toolkit->wm_msg[WM_MOUSEWHEEL] = _WM_MOUSEWHEEL;
	toolkit->wm_msg[WM_MOUSEHWHEEL] = _WM_MOUSEHWHEEL;
	toolkit->wm_msg[WM_MOVE] = _WM_MOVE;
	toolkit->wm_msg[WM_NCACTIVATE] = _WM_NCACTIVATE;
	toolkit->wm_msg[WM_NCCALCSIZE] = _WM_NCCALCSIZE;
	toolkit->wm_msg[WM_NCHITTEST] = _WM_NCHITTEST;
	toolkit->wm_msg[WM_NCLBUTTONDOWN] = _WM_NCLBUTTONDOWN;
	toolkit->wm_msg[WM_NCPAINT] = _WM_NCPAINT;
	toolkit->wm_msg[WM_NOTIFY] = _WM_NOTIFY;
	toolkit->wm_msg[WM_PAINT] = _WM_PAINT;
	toolkit->wm_msg[WM_PALETTECHANGED] = _WM_PALETTECHANGED;
	toolkit->wm_msg[WM_PARENTNOTIFY] = _WM_PARENTNOTIFY;
	toolkit->wm_msg[WM_PASTE] = _WM_PASTE;
	toolkit->wm_msg[WM_PRINT] = _WM_PRINT;
	toolkit->wm_msg[WM_PRINTCLIENT] = _WM_PRINTCLIENT;
	toolkit->wm_msg[WM_QUERYENDSESSION] = _WM_QUERYENDSESSION;
	toolkit->wm_msg[WM_QUERYNEWPALETTE] = _WM_QUERYNEWPALETTE;
	toolkit->wm_msg[WM_QUERYOPEN] = _WM_QUERYOPEN;
	toolkit->wm_msg[WM_RBUTTONDBLCLK] = _WM_RBUTTONDBLCLK;
	toolkit->wm_msg[WM_RBUTTONDOWN] = _WM_RBUTTONDOWN;
	toolkit->wm_msg[WM_RBUTTONUP] = _WM_RBUTTONUP;
	toolkit->wm_msg[WM_SETCURSOR] = _WM_SETCURSOR;
	toolkit->wm_msg[WM_SETFOCUS] = _WM_SETFOCUS;
	toolkit->wm_msg[WM_SETFONT] = _WM_SETFONT;
	toolkit->wm_msg[WM_SETTINGCHANGE] = _WM_SETTINGCHANGE;
	toolkit->wm_msg[WM_SETREDRAW] = _WM_SETREDRAW;
	toolkit->wm_msg[WM_SHOWWINDOW] = _WM_SHOWWINDOW;
	toolkit->wm_msg[WM_SIZE] = _WM_SIZE;
	toolkit->wm_msg[WM_SYSCHAR] = _WM_SYSCHAR;
	toolkit->wm_msg[WM_SYSCOLORCHANGE] = _WM_SYSCOLORCHANGE;
	toolkit->wm_msg[WM_SYSCOMMAND] = _WM_SYSCOMMAND;
	toolkit->wm_msg[WM_SYSKEYDOWN] = _WM_SYSKEYDOWN;
	toolkit->wm_msg[WM_SYSKEYUP] = _WM_SYSKEYUP;
	//toolkit->wm_msg[WM_TABLET_FLICK] = W_WM_TABLET_FLICK;
	toolkit->wm_msg[WM_TIMER] = _WM_TIMER;
	toolkit->wm_msg[WM_TOUCH] = _WM_TOUCH;
	toolkit->wm_msg[WM_UNDO] = _WM_UNDO;
	toolkit->wm_msg[WM_UNINITMENUPOPUP] = _WM_UNINITMENUPOPUP;
	toolkit->wm_msg[WM_UPDATEUISTATE] = _WM_UPDATEUISTATE;
	toolkit->wm_msg[WM_VSCROLL] = _WM_VSCROLL;
	toolkit->wm_msg[WM_WINDOWPOSCHANGED] = _WM_WINDOWPOSCHANGED;
	toolkit->wm_msg[WM_WINDOWPOSCHANGING] = _WM_WINDOWPOSCHANGING;
	toolkit->wm_msg[WM_XBUTTONDBLCLK] = _WM_XBUTTONDBLCLK;
	toolkit->wm_msg[WM_XBUTTONDOWN] = _WM_XBUTTONDOWN;
	toolkit->wm_msg[WM_XBUTTONUP] = _WM_XBUTTONUP;
	//toolkit->wm_msg[WM_DPICHANGED] = W_WM_DPICHANGED;
	toolkit->wm_msg[WM_MENUCOMMAND] = _WM_MENUCOMMAND;

}
typedef void (*_w_widget_init_class)(struct _w_widget_class *clazz);
#define _W_TOOLKIT_CLASS_INIT(x,X) ((struct _w_widget_class *)&toolkit->class_##x)->init_class =(_w_widget_init_class) _w_##x##_class_init; \
	((struct _w_widget_class *)&toolkit->class_##x)->reserved[0] = &toolkit->class_##x##_reserved;\
	toolkit->classes[_W_CLASS_##X] =(struct _w_widget_class *) &toolkit->class_##x
#define _W_TOOLKIT_CLASS_ITEM_INIT(x,X) _W_TOOLKIT_CLASS_INIT(x,X);\
		toolkit->class_##x.class_##x##item = &toolkit->class_##x##item

void _w_toolkit_widget_class_init(_w_toolkit *toolkit) {
	//widget class init
	_W_TOOLKIT_CLASS_INIT(shell, SHELL);
	_W_TOOLKIT_CLASS_INIT(composite, COMPOSITE);
	_W_TOOLKIT_CLASS_INIT(canvas, CANVAS);
	_W_TOOLKIT_CLASS_INIT(ccanvas, CCANVAS); //
	_W_TOOLKIT_CLASS_INIT(glcanvas, GLCANVAS);
	_W_TOOLKIT_CLASS_INIT(button, BUTTON);
	_W_TOOLKIT_CLASS_INIT(sash, SASH);
	_W_TOOLKIT_CLASS_INIT(browser, BROWSER);
	_W_TOOLKIT_CLASS_ITEM_INIT(menu, MENU);
	_W_TOOLKIT_CLASS_ITEM_INIT(tree, TREE);
	toolkit->class_tree.class_treecolumn = &toolkit->class_treecolumn;
	_W_TOOLKIT_CLASS_ITEM_INIT(table, TABLE); //
	toolkit->class_table.class_tablecolumn = &toolkit->class_tablecolumn;
	_W_TOOLKIT_CLASS_ITEM_INIT(tabfolder, TABFOLDER);
	_W_TOOLKIT_CLASS_ITEM_INIT(combo, COMBO); //
	_W_TOOLKIT_CLASS_ITEM_INIT(coolbar, COOLBAR); //
	_W_TOOLKIT_CLASS_INIT(datetime, DATETIME); //
	_W_TOOLKIT_CLASS_ITEM_INIT(expandbar, EXPANDBAR); //
	_W_TOOLKIT_CLASS_INIT(group, GROUP); //
	_W_TOOLKIT_CLASS_INIT(label, LABEL); //
	_W_TOOLKIT_CLASS_INIT(link, LINK); //
	_W_TOOLKIT_CLASS_ITEM_INIT(list, LIST); //
	_W_TOOLKIT_CLASS_INIT(progressbar, PROGRESSBAR); //
	_W_TOOLKIT_CLASS_INIT(scale, SCALE); //
	_W_TOOLKIT_CLASS_INIT(slider, SLIDER); //
	_W_TOOLKIT_CLASS_INIT(spinner, SPINNER); //
	_W_TOOLKIT_CLASS_ITEM_INIT(taskbar, TASKBAR); //
	_W_TOOLKIT_CLASS_INIT(text, TEXT); //
	_W_TOOLKIT_CLASS_ITEM_INIT(toolbar, TOOLBAR); //
	_W_TOOLKIT_CLASS_INIT(tooltip, TOOLTIP); //
	_W_TOOLKIT_CLASS_INIT(tracker, TRACKER); //
	_W_TOOLKIT_CLASS_ITEM_INIT(tray, TRAY); //

}

#ifdef __cplusplus
}
#endif
#endif
