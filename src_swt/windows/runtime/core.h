/*
 * gtk.h
 *
 *  Created on: 28 févr. 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_WIN32_CORE_WIN32_H_
#define SRC_WIN32_CORE_WIN32_H_
#if defined( __WIN32__) || defined(__WIN64__)
#ifdef __cplusplus
extern "C" {
#endif
#include <swt.h>
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <uxtheme.h>
#include "resource.h"

struct UnicodeString {
	WCHAR *str;
	int length;
	WCHAR temp[0x100];
};
typedef struct _w_thread_cleanup {
	void (*routine)(void*);
	void *arg;
	struct _w_thread_cleanup *prev;
} _w_thread_cleanup;
typedef struct _w_thread {
	void *vtbl;      //used in c++
	w_thread_start start_proc;
	void *args;
	HANDLE handle;
	DWORD threadId;
	_w_thread_cleanup *cleanup;
} _w_thread;
#define VERSION(major,minor) (major << 16 | minor)
extern HINSTANCE hinst;
void unicode_init(struct UnicodeString *string);
void unicode_set(struct UnicodeString *string, const char *str, int length);
void unicode_free(struct UnicodeString *string);
void unicode_alloc(struct UnicodeString *string, size_t size);
void unicode_alloc_tmp(struct UnicodeString *string);
w_string_ref* unicode_to_ref(WCHAR *text, int length);
#define WM_NOTIFY_CHILD (WM_USER + 1)
#define WM_CTLCOLOR_CHILD (WM_USER + 2)
#define WM_COMMAND_CHILD (WM_USER + 3)
#define WM_SCROLL_CHILD (WM_USER + 4)
#define WM_ASYNCEXEC (WM_USER + 5)
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#define WM_TOUCH 0x240
#endif
#ifdef __cplusplus
}
#endif
#endif /* SRC_WIN32_CORE_WIN32_H_ */
