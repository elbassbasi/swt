/*
 * menu.c
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */
#if defined( __WIN32__) || defined(__WIN64__)
#include "shell.h"
#include "menu.h"
#include "toolkit.h"
/*
 * private
 */
#define MENU_CHECK  1 << 31
#define MENU_HAS_ACCELERATOR  1 << 30
struct _w_menuitem_iterator {
	struct _w_iterator_class *clazz;
	w_menu *menu;
	HMENU parentItem;
	int count;
	int i;
};
wresult _w_menuitem_iterator_close(w_iterator *it) {
	return W_TRUE;
}
wresult _w_menuitem_iterator_reset(w_iterator *it) {
	((struct _w_menuitem_iterator*) it)->i = 0;
	return W_TRUE;
}
wresult _w_menuitem_iterator_next(w_iterator *it, void *obj) {

}
wresult _w_menuitem_iterator_remove(w_iterator *it) {
}
size_t _w_menuitem_iterator_get_count(w_iterator *it) {
	return ((struct _w_menuitem_iterator*) it)->count;
}
_w_iterator_class _w_menuitem_iterator_class = { //
		sizeof(struct _w_menuitem_iterator*), sizeof(_w_iterator_class), //
				_w_menuitem_iterator_close, //
				_w_menuitem_iterator_next, //
				_w_menuitem_iterator_reset, //
				_w_menuitem_iterator_remove, //
				_w_menuitem_iterator_get_count //
		};
int _w_menuitem_find_index(HMENU parent_menu, HMENU menu) {
	if (menu == 0)
		return -1;
	int count = GetMenuItemCount(parent_menu);
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_SUBMENU;
	for (int i = 0; i < count; i++) {
		info.hSubMenu = 0;
		if (!GetMenuItemInfoW(parent_menu, i, TRUE, &info)) {
			return -1;
		}
		if (info.hSubMenu == menu) {
			return i;
		}
	}
	return -1;
}
HMENU _w_menuitem_get_submenu(w_menuitem *item) {
	MENUITEMINFOW info;
	if (_W_MENUITEM(item)->index < 0) {
		return _W_MENUITEM(item)->parentItem;
	} else {
		info.cbSize = sizeof(info);
		info.fMask = MIIM_SUBMENU;
		info.hSubMenu = 0;
		if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
		_W_MENUITEM(item)->index, TRUE, &info)) {
			return info.hSubMenu;
		} else
			return 0;
	}
}
void _w_menuitem_set_text_0(struct UnicodeString *str, const char *string) {
	unicode_set(str, string, -1);
	if (str->str) {
		int l = str->length;
		WCHAR *s = str->str;
		for (int i = 0; i < l; i++) {
			if (s[i] == '\t')
				s[i] = ' ';
		}
	}
}
w_menu* _w_menu_get_top(HMENU menu) {
	HMENU m = menu;
	MENUINFO info;
	info.cbSize = sizeof(info);
	info.fMask = MIM_MENUDATA | MIM_HELPID;
	while (1) {
		if (!GetMenuInfo(m, &info)) {
			return 0;
		}
		if (info.dwContextHelpID == (DWORD) -1) {
			return (w_menu*) info.dwMenuData;
		}
		m = (HMENU) info.dwMenuData;
	}
	return 0;
}

wresult _w_menuitem_fill_accel(ACCEL *accel, struct _w_shell_accel_item *item) {
	accel->cmd = accel->key = accel->fVirt = 0;
	//if (accelerator == 0 || !getEnabled ()) return false;
	if ((item->accelerator & W_COMMAND) != 0)
		return W_FALSE;
	int fVirt = FVIRTKEY;
	int key = item->accelerator & W_KEY_MASK;
	int vKey = _w_untranslate_key(key);
	WCHAR tp[2];
	if (vKey != 0) {
		key = vKey;
	} else {
		switch (key) {
		/*
		 * Bug in Windows.  For some reason, VkKeyScan
		 * fails to map ESC to VK_ESCAPE and DEL to
		 * VK_DELETE.  The fix is to map these keys
		 * as a special case.
		 */
		case 27:
			key = VK_ESCAPE;
			break;
		case 127:
			key = VK_DELETE;
			break;
		default: {
			if (key == 0)
				return W_FALSE;
			vKey = VkKeyScanW((short) key);
			if (vKey == -1) {
				tp[0] = key;
				tp[1] = 0;
				CharUpperW(tp);
				if (key != tp[0]) {
					fVirt = 0;
				}
			} else {
				key = vKey & 0xFF;
			}
		}
		}
	}
	accel->key = (short) key;
	accel->cmd = (short) item->id;
	accel->fVirt = fVirt;
	if ((item->accelerator & W_ALT) != 0)
		accel->fVirt |= FALT;
	if ((item->accelerator & W_SHIFT) != 0)
		accel->fVirt |= FSHIFT;
	if ((item->accelerator & W_CTRL) != 0)
		accel->fVirt |= FCONTROL;
	return W_TRUE;
}
/*
 * menuitem
 */
void _w_menuitem_copy(w_widgetdata *from, w_widgetdata *to) {
	_W_MENUITEM(to)->widgetdata.clazz =
	_W_MENUITEM(from)->widgetdata.clazz;
	_W_MENUITEM(to)->menu = _W_MENUITEM(from)->menu;
	_W_MENUITEM(to)->parentItem = _W_MENUITEM(from)->parentItem;
	_W_MENUITEM(to)->index = _W_MENUITEM(from)->index;
	//_W_MENUITEM(to)->text = 0;
}
void _w_menuitem_close(w_menuitem *item) {
	//w_string_ref_dec(_W_MENUITEM(item)->text);
}
wresult _w_menuitem_insert(w_menuitem *parent, w_menuitem *item,
		const char *text, int style, int index) {
	struct UnicodeString str;
	MENUITEMINFOW info;
	MENUINFO _info;
	HMENU parentItem = _w_menuitem_get_submenu(parent), submenu = 0;
	if (parentItem == 0)
		return W_ERROR_INVALID_ARGUMENT;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID | MIIM_TYPE;
	info.fType = 0;
	info.wID = 0;
	int bits = W_CHECK | W_RADIO | W_PUSH | W_SEPARATOR | W_CASCADE;
	switch (style & bits) {
	case W_SEPARATOR:
		info.fType = MFT_SEPARATOR;
		break;
	case W_CHECK:
		info.wID = MENU_CHECK;
		break;
	case W_RADIO:
		info.fType = MFT_RADIOCHECK;
		break;
	case W_CASCADE:
		submenu = CreatePopupMenu();
		_info.cbSize = sizeof(_info);
		_info.fMask = MIM_MENUDATA | MIM_STYLE | MIM_HELPID;
		_info.dwMenuData = (ULONG_PTR) parentItem;
		_info.dwStyle = MNS_NOTIFYBYPOS;
		_info.dwContextHelpID = 0;
		SetMenuInfo(submenu, &_info);
		info.fMask = MIIM_ID | MIIM_TYPE | MIIM_SUBMENU;
		info.hSubMenu = submenu;
		break;
	}
	if (text == 0)
		text = " ";
	_w_menuitem_set_text_0(&str, text);
	info.dwTypeData = str.str;
	info.cch = str.length;
	if (index < 0)
		index = GetMenuItemCount(parentItem);
	WINBOOL succes = InsertMenuItemW(parentItem, index, TRUE, &info);
	unicode_free(&str);
	if (succes) {
		if (item != 0) {
			_W_MENUITEM(item)->widgetdata.clazz =
			_W_MENUITEM(parent)->widgetdata.clazz;
			_W_MENUITEM(item)->menu = _W_MENUITEM(parent)->menu;
			_W_MENUITEM(item)->parentItem = parentItem;
			_W_MENUITEM(item)->index = index;
			//_W_MENUITEM(item)->text = 0;
		}
		return W_TRUE;
	} else {
		if (submenu != 0)
			DestroyMenu(submenu);
		return W_ERROR_ITEM_NOT_ADDED;
	}
}
int _w_menuitem_get_accelerator(w_menuitem *item) {
}
void* _w_menuitem_get_data(w_item *item) {
	if (_W_MENUITEM(item)->index < 0)
		return 0;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_DATA;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		return (void*) info.dwItemData;
	} else
		return 0;
}
wresult _w_menuitem_get_enabled(w_menuitem *item) {
	if (_W_MENUITEM(item)->index < 0)
		return W_TRUE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		return (info.fState & (MFS_DISABLED | MFS_GRAYED)) == 0;
	} else
		return W_TRUE;
}
wresult _w_menuitem_get_item(w_menuitem *item, unsigned int index,
		w_menuitem *sub_item) {
	HMENU parentItem = _w_menuitem_get_submenu(item);
	if (parentItem == 0)
		return W_ERROR_INVALID_ARGUMENT;
	int count = GetMenuItemCount(parentItem);
	if (index < count) {
		_W_MENUITEM(sub_item)->widgetdata.clazz =
		_W_MENUITEM(item)->widgetdata.clazz;
		_W_MENUITEM(sub_item)->menu =
		_W_MENUITEM(item)->menu;
		_W_MENUITEM(sub_item)->parentItem = parentItem;
		_W_MENUITEM(sub_item)->index = index;
		//_W_MENUITEM(sub_item)->text = 0;
		return W_TRUE;
	}
	return W_ERROR_INVALID_RANGE;
}
int _w_menuitem_get_item_count(w_menuitem *item) {
	HMENU parentItem = _w_menuitem_get_submenu(item);
	if (parentItem == 0)
		return 0;
	return GetMenuItemCount(parentItem);
}
void _w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	HMENU parentItem = _w_menuitem_get_submenu(item);
	if (parentItem != 0) {
		((struct _w_menuitem_iterator*) items)->clazz =
				&_w_menuitem_iterator_class;
		((struct _w_menuitem_iterator*) items)->menu = _W_MENUITEM(item)->menu;
		((struct _w_menuitem_iterator*) items)->parentItem = parentItem;
		((struct _w_menuitem_iterator*) items)->i = 0;
		((struct _w_menuitem_iterator*) items)->count = GetMenuItemCount(
				parentItem);
	}
}
unsigned short _w_menuitem_get_id(w_menuitem *item) {
	if (_W_MENUITEM(item)->index < 0)
		return 0;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		return info.wID & 0xFFFF;
	} else
		return 0;
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
}
w_widget* _w_menuitem_get_menu(w_item *item) {
	return W_WIDGET(_W_MENUITEM(item)->menu);
}
wresult _w_menuitem_get_parent(w_menuitem *item, w_menuitem *parent) {
	MENUINFO _info;
	int index = 0;
	_info.cbSize = sizeof(_info);
	_info.fMask = MIM_MENUDATA | MIM_HELPID;
	_info.dwMenuData = 0;
	if (GetMenuInfo(_W_MENUITEM(item)->parentItem, &_info)) {
		if (_info.dwContextHelpID != -1) {
			index = _w_menuitem_find_index(_W_MENUITEM(item)->parentItem,
					(HMENU) _info.dwMenuData);
			if (index >= 0) {
				_W_MENUITEM(parent)->widgetdata.clazz =
				_W_MENUITEM(item)->widgetdata.clazz;
				_W_MENUITEM(parent)->menu = _W_MENUITEM(item)->menu;
				_W_MENUITEM(parent)->parentItem = (HMENU) _info.dwMenuData;
				_W_MENUITEM(parent)->index = index;
				//_W_MENUITEM(parent)->text = 0;
				return W_TRUE;
			}
		} else {
			if (_info.dwMenuData != 0) {
				_W_MENUITEM(parent)->widgetdata.clazz =
				_W_MENUITEM(item)->widgetdata.clazz;
				_W_MENUITEM(parent)->menu = _W_MENUITEM(item)->menu;
				_W_MENUITEM(parent)->parentItem = (HMENU) _info.dwMenuData;
				_W_MENUITEM(parent)->index = -1;
				//_W_MENUITEM(parent)->text = 0;
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}
wresult _w_menuitem_get_selection(w_menuitem *item) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		return (info.fState & MFS_CHECKED) != 0;
	} else
		return W_FALSE;
}
int _w_menuitem_get_style(w_menuitem *item) {
	HMENU parentItem = _w_menuitem_get_submenu(item);
	if (parentItem != 0)
		return W_CASCADE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_FTYPE | MIIM_ID;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		if (info.fType & MFT_SEPARATOR)
			return W_SEPARATOR;
		if (info.fType & MFT_RADIOCHECK)
			return W_RADIO;
		if ((info.wID & MENU_CHECK) != 0)
			return W_CHECK;
		return W_PUSH;
	}
	return 0;
}
void _w_menuitem_get_text_0(w_menuitem *item, struct UnicodeString *str,
		size_t added) {
	unicode_alloc_tmp(str);
	if (_W_MENUITEM(item)->index < 0) {
		return;
	}
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STRING;
	info.dwTypeData = str->str;
	info.cch = str->length;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		if (info.cch >= str->length) {
			int l = info.cch + 1;
			unicode_alloc(str, l + added);
			if (str->str != 0) {
				info.dwTypeData = str->str;
				info.cch = l;
				if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
				_W_MENUITEM(item)->index, TRUE, &info)) {
					return;
				}
			}
		} else
			return;
	}
	unicode_free(str);
	unicode_init(str);
}
wresult _w_menuitem_get_text(w_item *item, w_alloc *text) {
	if (_W_MENUITEM(item)->index < 0)
		return 0;
	//if (_W_MENUITEM(item)->text != 0)
	//	return _W_MENUITEM(item)->text;
	struct UnicodeString str;
	char *string;
	size_t l1, l2, sz;
	int ret = W_FALSE;
	_w_menuitem_get_text_0(W_MENUITEM(item), &str, 0);
	if (str.str != 0) {
		l1 = 0;
		l2 = str.length - 1;
		while (l1 < l2 && str.str[l1] != '\t') {
			l1++;
		}
		sz = w_utf8_from_utf16(str.str, l1, 0, 0);
		//sz = text->realloc(text, sz, &string);
		if (string != 0) {
			w_utf8_from_utf16(str.str, l1, string, sz);
			ret = W_TRUE;
		}
	}
	unicode_free(&str);
	return ret;
}
wresult _w_menuitem_is_enabled(w_menuitem *item) {
	if (_W_MENUITEM(item)->index < 0)
		return W_TRUE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		return (info.fState & MFS_ENABLED) != 0;
	} else
		return W_TRUE;
}
wresult _w_menuitem_remove(w_menuitem *item) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	if (DeleteMenu(_W_MENUITEM(item)->parentItem, _W_MENUITEM(item)->index,
	MF_BYPOSITION)) {
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_menuitem_remove_item(w_menuitem *item, int index) {
}
wresult _w_menuitem_set_accelerator(w_menuitem *item, int accelerator) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	struct UnicodeString str;
	int l1;
	WCHAR txt[30];
	MENUITEMINFOW info;
	int i = 0;
	wresult __ADD = W_FALSE;
	if (accelerator & W_CTRL) {
		txt[i++] = 'C';
		txt[i++] = 't';
		txt[i++] = 'r';
		txt[i++] = 'l';
		__ADD = W_TRUE;
	}
	if (accelerator & W_ALT) {
		if (__ADD)
			txt[i++] = '+';
		txt[i++] = 'A';
		txt[i++] = 'l';
		txt[i++] = 't';
		__ADD = W_TRUE;
	}
	if (accelerator & W_SHIFT) {
		if (__ADD)
			txt[i++] = '+';
		txt[i++] = 'S';
		txt[i++] = 'h';
		txt[i++] = 'i';
		txt[i++] = 'f';
		txt[i++] = 't';
		__ADD = W_TRUE;
	}
	if ((accelerator & 0xFFFF) == 0) {
		txt[i] = 0;
	} else {
		if (__ADD)
			txt[i++] = '+';
		if (accelerator & W_KEYCODE_BIT) {
			//_w_untranslate_key()

		} else {
			txt[i++] = (accelerator & 0xFFFF);
			txt[i] = 0;
		}
	}
	_w_menuitem_get_text_0(item, &str, i + 1);
	if (str.str != 0) {
		l1 = 0;
		while (1) {
			if (str.str[l1] == '\t')
				break;
			if (str.str[l1] == 0)
				break;
			l1++;
		}
		str.str[l1++] = '\t';
		lstrcpyW(&str.str[l1], txt);
		info.cbSize = sizeof(info);
		info.fMask = MIIM_STRING;
		info.dwTypeData = str.str;
		info.cch = l1 + i;
		if (SetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
		_W_MENUITEM(item)->index, TRUE, &info)) {
			unicode_free(&str);
			return W_TRUE;
		}

	}
	unicode_free(&str);
	return W_TRUE;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_DATA;
	info.dwItemData = (ULONG_PTR) data;
	if (SetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_menuitem_set_enabled(w_menuitem *item, wresult enabled) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	info.fState = 0;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		int bits = MFS_DISABLED | MFS_GRAYED;
		if (enabled) {
			if ((info.fState & bits) == 0)
				return W_TRUE;
			info.fState &= ~bits;
		} else {
			if ((info.fState & bits) == bits)
				return W_TRUE;
			info.fState |= bits;
		}
		if (SetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
		_W_MENUITEM(item)->index, TRUE, &info)) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_menuitem_set_id(w_menuitem *item, unsigned short id) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_ID;
	info.wID = 0;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		info.wID = (info.wID & 0xFFFF0000) | (id & 0xFFFF);
		if (SetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
		_W_MENUITEM(item)->index, TRUE, &info)) {
			return W_TRUE;
		}
	}
	return W_FALSE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
}
wresult _w_menuitem_set_selection(w_menuitem *item, wresult selected) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE | MIIM_ID;
	info.fState = 0;
	if (GetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
	_W_MENUITEM(item)->index, TRUE, &info)) {
		if ((info.wID & MENU_CHECK) || (info.fState & MFT_RADIOCHECK)) {
			info.fMask = MIIM_STATE;
			info.fState &= ~MFS_CHECKED;
			if (selected)
				info.fState |= MFS_CHECKED;
			if (SetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
			_W_MENUITEM(item)->index, TRUE, &info)) {
				return W_TRUE;
			}
		}
	}
	return W_FALSE;
}

wresult _w_menuitem_set_text(w_item *item, const char *string) {
	if (_W_MENUITEM(item)->index < 0)
		return W_FALSE;
	struct UnicodeString str;
	MENUITEMINFOW info;
	//w_string_ref_dec(_W_MENUITEM(item)->text);
	//_W_MENUITEM(item)->text = 0;
	_w_menuitem_set_text_0(&str, string);
	if (str.str) {
		info.cbSize = sizeof(info);
		info.fMask = MIIM_STRING;
		info.dwTypeData = str.str;
		info.cch = str.length;
		if (SetMenuItemInfoW(_W_MENUITEM(item)->parentItem,
		_W_MENUITEM(item)->index, TRUE, &info)) {
			unicode_free(&str);
			return W_TRUE;
		}
	}
	unicode_free(&str);
	return W_FALSE;
}
/*
 * menu
 */
wresult _w_menu_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	if ((style & W_BAR) != 0) {
		_W_MENU(widget)->handle = CreateMenu();
		if (_W_MENU(widget)->handle == 0)
			return W_ERROR_NO_HANDLES;
	} else {
		_W_MENU(widget)->handle = CreatePopupMenu();
		if (_W_MENU(widget)->handle == 0)
			return W_ERROR_NO_HANDLES;
	}
	MENUINFO _info;
	_info.cbSize = sizeof(_info);
	_info.fMask = MIM_MENUDATA | MIM_STYLE | MIM_HELPID;
	_info.dwMenuData = (ULONG_PTR) widget;
	_info.dwStyle = MNS_NOTIFYBYPOS;
	_info.dwContextHelpID = -1;
	SetMenuInfo(_W_MENU(widget)->handle, &_info);
	_W_MENU(widget)->parent = W_CONTROL(parent);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_MENU(widget)->x = -1;
	_W_MENU(widget)->y = -1;
	return W_TRUE;
}
void _w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
	if ((_W_WIDGET(menu)->style & W_BAR) != 0) {
		if (_W_SHELL( _W_MENU(menu)->parent)->menubar != menu) {
			memset(bounds, 0, sizeof(w_rect));
			return;
		}
		HWND hwndShell = _W_WIDGET( _W_MENU(menu)->parent)->handle;
		MENUBARINFO info;
		info.cbSize = sizeof(info);
		if (GetMenuBarInfo(hwndShell, OBJID_MENU, 0, &info)) {
			bounds->x = info.rcBar.left;
			bounds->y = info.rcBar.top;
			bounds->width = info.rcBar.right - info.rcBar.left;
			bounds->height = info.rcBar.bottom - info.rcBar.top;
			return;
		}
	} else {
		int count = GetMenuItemCount(_W_MENU(menu)->handle);
		if (count != 0) {
			RECT rect1;
			if (GetMenuItemRect(0, _W_MENU(menu)->handle, 0, &rect1)) {
				RECT rect2;
				if (GetMenuItemRect(0, _W_MENU(menu)->handle, count - 1,
						&rect2)) {
					bounds->x = rect1.left - 2;
					bounds->y = rect1.top - 2;
					bounds->width = (rect2.right - rect2.left) + 4;
					bounds->height = (rect2.bottom - rect1.top) + 4;
					return;
				}
			}
		}
	}
	memset(bounds, 0, sizeof(w_rect));
}
wresult _w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	_W_MENUITEM(rootitem)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_MENU_GET_CLASS(menu)->class_menuitem);
	_W_MENUITEM(rootitem)->menu = menu;
	_W_MENUITEM(rootitem)->parentItem = _W_MENU(menu)->handle;
	_W_MENUITEM(rootitem)->index = -1;
	//_W_MENUITEM(rootitem)->text = 0;
	return W_TRUE;
}
int _w_menu_get_orientation(w_menu *menu) {
	return _W_WIDGET(menu)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT);
}
w_control* _w_menu_get_parent(w_menu *menu) {
	return _W_MENU(menu)->parent;
}
wresult _w_menu_get_visible(w_menu *menu) {
}
wresult _w_menu_is_visible(w_menu *menu) {
}
void _w_menu_set_location(w_menu *menu, w_point *location) {
	if ((_W_WIDGET(menu)->style & (W_BAR | W_DROP_DOWN)) != 0)
		return;
	_W_MENU(menu)->x = location->x;
	_W_MENU(menu)->y = location->y;
}
void _w_menu_set_orientation(w_menu *menu, int orientation) {
}
wresult _w_menu_set_visible(w_menu *menu, wresult visible) {
	if ((_W_WIDGET(menu)->style & (W_BAR | W_DROP_DOWN)) != 0)
		return W_FALSE;
	w_shell *shell;
	w_control_get_shell( _W_MENU(menu)->parent,&shell);
	if (shell == 0)
		return W_ERROR_INVALID_ARGUMENT;
	HWND hwndParent = _W_WIDGET(shell)->handle;
	if (visible) {
		int flags = TPM_LEFTBUTTON;
		if (GetKeyState(VK_LBUTTON) >= 0)
			flags |= TPM_RIGHTBUTTON;
		if ((_W_WIDGET(menu)->style & W_RIGHT_TO_LEFT) != 0)
			flags |= TPM_RIGHTALIGN;
		if ((_W_WIDGET(shell)->style & W_MIRRORED) != 0) {
			flags &= ~TPM_RIGHTALIGN;
			if ((_W_WIDGET(menu)->style & W_LEFT_TO_RIGHT) != 0)
				flags |= TPM_RIGHTALIGN;
		}
		int nX = _W_MENU(menu)->x, nY = _W_MENU(menu)->y;
		if (nX < 0) {
			int pos = GetMessagePos();
			//nX = GET_X_LPARAM(pos);
			//nY = GET_Y_LPARAM(pos);
		}
		/*
		 * Feature in Windows.  It is legal use TrackPopupMenu()
		 * to display an empty menu as long as menu items are added
		 * inside of WM_INITPOPUPMENU.  If no items are added, then
		 * TrackPopupMenu() fails and does not send an indication
		 * that the menu has been closed.  This is not strictly a
		 * bug but leads to unwanted behavior when application code
		 * assumes that every WM_INITPOPUPMENU will eventually result
		 * in a WM_MENUSELECT, wParam=MAKEWPARAM (0, 0xFFFF), lParam=0 to
		 * indicate that the menu has been closed.  The fix is to detect
		 * the case when TrackPopupMenu() fails and the number of items in
		 * the menu is zero and issue a fake WM_MENUSELECT.
		 */
		WINBOOL success = TrackPopupMenu(_W_MENU(menu)->handle, flags, nX, nY,
				0, hwndParent, 0);
		if (!success && GetMenuItemCount(_W_MENU(menu)->handle) == 0) {
			SendMessage(hwndParent, WM_MENUSELECT, MAKEWPARAM(0, 0xFFFF), 0);
		}
	} else {
		SendMessage(hwndParent, WM_CANCELMODE, 0, 0);
	}
	return W_TRUE;
}
int _MENU_WM_MENUCOMMAND(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	w_menu *menu = _w_menu_get_top((HMENU) e->lparam);
	if (menu == 0)
		return W_FALSE;
	_w_event_platform *ee = (_w_event_platform*) e;
	_w_menuitem item;
	w_event_item ei;
	MENUITEMINFOW info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID | MIIM_CHECKMARKS;
	WINBOOL success = GetMenuItemInfoW((HMENU) ee->lparam, ee->wparam,
	TRUE, &info);
	if (!success)
		return W_TRUE;
	//if ((info.fType & MFT_RADIOCHECK) != 0) {
	if ((info.fType & MFT_RADIOCHECK) != 0) {
		UINT last_state = info.fState;
		int i = ee->wparam - 1;
		WINBOOL stop = FALSE;
		while (i >= 0 && !stop) {
			info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID;
			success = GetMenuItemInfoW((HMENU) ee->lparam, i,
			TRUE, &info);
			if (success) {
				if ((info.fType & MFT_RADIOCHECK) != 0) {
					info.fMask = MIIM_STATE;
					info.fState &= ~MFS_CHECKED;
					success = SetMenuItemInfoW((HMENU) ee->lparam, i,
					TRUE, &info);
				} else
					stop = TRUE;
			} else
				stop = TRUE;
			i--;
		}
		i = ee->wparam + 1;
		stop = FALSE;
		while (i >= 0 && !stop) {
			info.fMask = MIIM_FTYPE | MIIM_STATE | MIIM_ID;
			success = GetMenuItemInfoW((HMENU) ee->lparam, i,
			TRUE, &info);
			if (success) {
				if ((info.fType & MFT_RADIOCHECK) != 0) {
					info.fMask = MIIM_STATE;
					info.fState &= ~MFS_CHECKED;
					success = SetMenuItemInfoW((HMENU) ee->lparam, i,
					TRUE, &info);
				} else
					stop = TRUE;
			} else
				stop = TRUE;
			i++;
		}
		info.fMask = MIIM_STATE;
		info.fState = last_state | MFS_CHECKED;
		SetMenuItemInfoW((HMENU) ee->lparam, ee->wparam,
		TRUE, &info);
	} else {
		if (info.wID & MENU_CHECK) {
			if ((info.fState & MFS_CHECKED) != 0) {
				info.fState &= ~MFS_CHECKED;
			} else {
				info.fState |= MFS_CHECKED;
			}
			info.fMask = MIIM_STATE;
			success = SetMenuItemInfoW((HMENU) ee->lparam, ee->wparam,
			TRUE, &info);
		}
	}
	//}
	ei.event.type = W_EVENT_ITEM_SELECTION;
	ei.event.platform_event = (struct w_event_platform*) ee;
	ei.event.widget = W_WIDGET(menu);
	ei.item = (w_item*) &item;
	_W_MENUITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_MENU_GET_CLASS(menu)->class_menuitem);
	_W_MENUITEM(&item)->menu = W_MENU(menu);
	_W_MENUITEM(&item)->parentItem = (HMENU) ee->lparam;
	_W_MENUITEM(&item)->index = ee->wparam;
	//_W_MENUITEM(&item)->text = 0;
	_w_widget_send_event(W_WIDGET(menu), (w_event*) &ei);
	ee->result = FALSE;
	return W_TRUE;
	/*item.peer =
	 &((swt::MenuPeer*) widget->GetHandle<Menu_t>().peer)->menuItemPeer;
	 item.parent = (swt::Menu*) widget;
	 item.parentItem = (HMENU) ((_w_event_platform*) e)->lparam;
	 item.index = ((_w_event_platform*) e)->wparam;
	 swt::ItemEvent ee;
	 ee.type = swt::SelectionItem;
	 ee.platformEvent = e;
	 ee.widget = widget;
	 ee.item = (swt::Item*) &item;
	 swt::WidgetPeer::SendEvent(widget, &ee);*/
}
int _MENU_WM_INITMENUPOPUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {

	/* Ignore WM_INITMENUPOPUP for an accelerator */
	//if (display.accelKeyHit) return null;
	/*
	 * If the high order word of LPARAM is non-zero,
	 * the menu is the system menu and we can ignore
	 * WPARAM.  Otherwise, use WPARAM to find the menu.
	 */
	/*Shell shell = getShell ();
	 Menu oldMenu = shell.activeMenu, newMenu = null;
	 if (OS.HIWORD (lParam) == 0) {
	 newMenu = menuShell ().findMenu (wParam);
	 if (newMenu != null) newMenu.update ();
	 }
	 Menu menu = newMenu;
	 while (menu != null && menu != oldMenu) {
	 menu = menu.getParentMenu ();
	 }
	 if (menu == null) {
	 menu = shell.activeMenu;
	 while (menu != null) {*/
	/*
	 * It is possible (but unlikely), that application
	 * code could have disposed the widget in the hide
	 * event.  If this happens, stop searching up the
	 * ancestor list because there is no longer a link
	 * to follow.
	 */
	/*menu.sendEvent (SWT.Hide);
	 if (menu.isDisposed ()) break;
	 menu = menu.getParentMenu ();
	 Menu ancestor = newMenu;
	 while (ancestor != null && ancestor != menu) {
	 ancestor = ancestor.getParentMenu ();
	 }
	 if (ancestor != null) break;
	 }
	 }*/

	/*
	 * The shell and the new menu may be disposed because of
	 * sending the hide event to the ancestor menus but setting
	 * a field to null in a disposed shell is not harmful.
	 */
	//if (newMenu != null && newMenu.isDisposed ()) newMenu = null;
	//shell.activeMenu = newMenu;
	/* Send the show event */
	/*if (newMenu != null && newMenu != oldMenu) {
	 newMenu.sendEvent (SWT.Show);
	 // widget could be disposed at this point
	 }*/
	return W_FALSE;

}
int _MENU_WM_UNINITMENUPOPUP(w_widget *widget, struct _w_event_platform *e,
		struct _w_widget_reserved *reserved) {
	/*Menu hiddenMenu = menuShell ().findMenu (wParam);
	 if (hiddenMenu != null) {
	 Shell shell = getShell ();
	 hiddenMenu.sendEvent (SWT.Hide);
	 if (hiddenMenu == shell.activeMenu) shell.activeMenu = null;
	 }*/
	return W_FALSE;
}

/*
 * post event proc
 */
int _w_menu_post_event(w_widget *widget, struct w_event *e) {
	return W_FALSE;
}
void _w_menu_class_init(struct _w_menu_class *clazz) {
	_w_widget_class_init(W_WIDGET_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_MENU;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_menu_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_menu);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_menu);
//clazz
	W_WIDGET_CLASS(clazz)->create = _w_menu_create;
	W_WIDGET_CLASS(clazz)->post_event = _w_menu_post_event;
	clazz->get_bounds = _w_menu_get_bounds;
	clazz->get_root = _w_menu_get_root;
	clazz->get_orientation = _w_menu_get_orientation;
	clazz->get_parent = _w_menu_get_parent;
	clazz->get_visible = _w_menu_get_visible;
	clazz->is_visible = _w_menu_is_visible;
	clazz->set_location = _w_menu_set_location;
	clazz->set_orientation = _w_menu_set_orientation;
	clazz->set_visible = _w_menu_set_visible;
//item
	struct _w_menuitem_class *item = clazz->class_menuitem;
	item->item.get_parent_widget = _w_menuitem_get_menu;
	item->item.get_data = _w_menuitem_get_data;
	item->item.get_text = _w_menuitem_get_text;
	item->item.set_data = _w_menuitem_set_data;
	item->item.set_text = _w_menuitem_set_text;
	item->insert = _w_menuitem_insert;
	item->get_accelerator = _w_menuitem_get_accelerator;
	item->get_enabled = _w_menuitem_get_enabled;
	item->get_item = _w_menuitem_get_item;
	item->get_item_count = _w_menuitem_get_item_count;
	item->get_items = _w_menuitem_get_items;
	item->get_id = _w_menuitem_get_id;
	item->get_image = _w_menuitem_get_image;
	item->get_parent = _w_menuitem_get_parent;
	item->get_selection = _w_menuitem_get_selection;
	item->get_style = _w_menuitem_get_style;
	item->is_enabled = _w_menuitem_is_enabled;
	item->remove = _w_menuitem_remove;
	item->remove_item = _w_menuitem_remove_item;
	item->set_accelerator = _w_menuitem_set_accelerator;
	item->set_enabled = _w_menuitem_set_enabled;
	item->set_id = _w_menuitem_set_id;
	item->set_image = _w_menuitem_set_image;
	item->set_selection = _w_menuitem_set_selection;
	item->item.widgetdata.close = _w_widgetdata_close;
	item->item.widgetdata.is_ok = _w_widgetdata_is_ok;
	item->item.widgetdata.copy = _w_menuitem_copy;
	item->item.widgetdata.toolkit = (w_toolkit*) win_toolkit;
}
#endif
