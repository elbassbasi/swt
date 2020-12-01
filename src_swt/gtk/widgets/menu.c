/*
 * menu.c
 *
 *  Created on: 28 juil. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "menu.h"
#include "shell.h"
#include "toolkit.h"
/*
 * private
 */
struct _w_menuitem_iterator {
	struct _w_iterator_class *clazz;
	w_menu *menu;
	GList *original;
	GList *list;
	GtkWidget *widget;
	int count;
};
#define _W_MENUITEM_ITERATOR(x) ((struct _w_menuitem_iterator*)x)
wresult _w_menuitem_iterator_close(w_iterator *it) {
	if (_W_MENUITEM_ITERATOR( it)->original != 0) {
		g_list_free(_W_MENUITEM_ITERATOR( it)->original);
	}
	return W_TRUE;
}
int _w_menuitem_iterator_reset(w_iterator *it) {
	_W_MENUITEM_ITERATOR( it)->list =
	_W_MENUITEM_ITERATOR( it)->original;
	return W_TRUE;
}
wresult _w_menuitem_iterator_next(w_iterator *it, void *obj) {
	if (_W_MENUITEM_ITERATOR( it)->list != 0) {
		_W_MENUITEM_ITERATOR( it)->widget =
				(GtkWidget*) _W_MENUITEM_ITERATOR( it)->list->data;
		_W_MENUITEM(obj)->widgetdata.clazz =
				W_WIDGETDATA_CLASS(
						W_MENU_GET_CLASS(_W_MENUITEM_ITERATOR( it)->menu)->class_menuitem);
		_W_MENUITEM( obj)->menu = _W_MENUITEM_ITERATOR( it)->menu;
		_W_MENUITEM( obj)->widget =
		_W_MENUITEM_ITERATOR( it)->widget;
		_W_MENUITEM_ITERATOR( it)->list =
		_W_MENUITEM_ITERATOR( it)->list->next;
		return W_TRUE;
	} else {
		_W_MENUITEM( obj)->widgetdata.clazz = 0;
		return W_FALSE;
	}
}
size_t _w_menuitem_iterator_get_count(w_iterator *it) {
	if (_W_MENUITEM_ITERATOR(it)->count < 0) {
		int count = 0;
		GList *list = _W_MENUITEM_ITERATOR(it)->original;
		while (list != 0) {
			count++;
			list = list->next;
		}
		_W_MENUITEM_ITERATOR(it)->count = count;
	}
	return _W_MENUITEM_ITERATOR(it)->count;
}
GtkWidget* _w_menuitem_get_submenu(w_menuitem *item) {
	if (GTK_IS_MENU_SHELL(_W_MENUITEM(item)->widget)) {
		return _W_MENUITEM(item)->widget;
	} else {
		return gtk_menu_item_get_submenu(
				GTK_MENU_ITEM(_W_MENUITEM(item)->widget));
	}
}
void _w_menuitem_remove_item_0(GtkWidget *widget, gpointer data) {
	GtkWidget *last_widget = _W_MENUITEM(((w_event_item*) data)->item)->widget;
	_W_MENUITEM(((w_event_item*) data)->item)->widget = widget;
	GtkWidget *submenu = _w_menuitem_get_submenu(
			(w_menuitem*) ((w_event_item*) data)->item);
	if (submenu != 0) {
		gtk_container_forall(GTK_CONTAINER(submenu), _w_menuitem_remove_item_0,
				data);
	}
	_w_widget_send_event(((w_event_item*) data)->event.widget, (w_event*) data);
	_W_MENUITEM(((w_event_item*) data)->item)->widget = last_widget;
#if GTK3
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(widget), NULL);
#else
    gtk_menu_item_remove_submenu(GTK_MENU_ITEM(ii.widget));
#endif
	gtk_container_remove(GTK_CONTAINER(gtk_widget_get_parent(widget)), widget);
}
int _w_menuitem_iterator_remove(w_iterator *it) {
	w_menuitem item;
	_W_MENUITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
			W_MENU_GET_CLASS(_W_MENUITEM_ITERATOR( it)->menu)->class_menuitem);
	_W_MENUITEM(&item)->menu = _W_MENUITEM_ITERATOR( it)->menu;
	_W_MENUITEM( &item)->widget = _W_MENUITEM_ITERATOR( it)->widget;
	return _w_menuitem_remove(&item);
}
struct _w_iterator_class _w_menuitem_iterator_class = { //
		_w_menuitem_iterator_close, //
		_w_menuitem_iterator_next, //
		_w_menuitem_iterator_reset, //
		_w_menuitem_iterator_remove, //
		_w_menuitem_iterator_get_count //
		};

/*
 * menuitem
 */
GtkAccelGroup* _w_menuitem_get_accel_group(w_menuitem *item) {
	w_menu *menu = _W_MENUITEM(item)->menu;
	w_control *control = _W_MENU(menu)->parent;
	w_shell *shell;
	w_control_get_shell(control,&shell);
	return _w_shell_create_accel_group(shell);
}
void _w_menuitem_copy(w_widgetdata *from, w_widgetdata *to) {
	_W_MENUITEM(to)->widget = _W_MENUITEM(from)->widget;
	_W_MENUITEM(to)->menu = _W_MENUITEM(from)->menu;
	_W_MENUITEM(to)->widgetdata.clazz = _W_MENUITEM(from)->widgetdata.clazz;
}
wresult _w_menuitem_insert(w_menuitem *parent, w_menuitem *item,
		const char *text, int style, int index) {
	GtkWidget *menuItem, *sub_menu = 0;
	GtkWidget *_p = _w_menuitem_get_submenu(parent);
	if (_p == 0)
		return W_ERROR_NO_HANDLES;

	int bits = W_CHECK | W_RADIO | W_PUSH | W_SEPARATOR | W_CASCADE;
	switch (style & bits) {
	case W_SEPARATOR:
		menuItem = gtk_separator_menu_item_new();
		break;
	case W_CHECK:
		menuItem = gtk_check_menu_item_new_with_mnemonic("");
		break;
	case W_RADIO: {
		struct _w_widget_find_child ii;
		memset(&ii, 0, sizeof(ii));
		ii.index = index;
		gtk_container_forall(GTK_CONTAINER(_p),
				(GtkCallback) _w_widget_find_children, &ii);
		GSList *group = NULL;
		if (ii.widget != 0 && GTK_IS_RADIO_MENU_ITEM(ii.widget)) {
			group = gtk_radio_menu_item_get_group(
					GTK_RADIO_MENU_ITEM(ii.widget));
		}
		if (group == NULL
				&& index
						>= 0&& ii.prev != 0 && GTK_IS_RADIO_MENU_ITEM(ii.prev)) {
			group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(ii.prev));
		}
		menuItem = gtk_radio_menu_item_new_with_mnemonic(group, "");
	}
		break;
	case W_CASCADE: {
		menuItem = gtk_menu_item_new_with_mnemonic("");
		if (menuItem != 0) {
			sub_menu = gtk_menu_new();
			if (sub_menu == 0) {
				gtk_widget_destroy(menuItem);
				menuItem = 0;
			} else {
				g_object_set_qdata(G_OBJECT(sub_menu), gtk_toolkit->quark[0],
				_W_MENUITEM(parent)->menu);
				gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), sub_menu);
			}
		}
	}
		break;
	default:
		menuItem = gtk_image_menu_item_new_with_mnemonic("");
		break;
	}
	if (menuItem == 0) {
		return W_ERROR_NO_HANDLES;
	}
	g_object_set_qdata(G_OBJECT(menuItem), gtk_toolkit->quark[0],
	_W_MENUITEM(parent)->menu);
	if (GTK_IS_MENU_SHELL(_p)) {
		if (index < 0)
			gtk_menu_shell_append(GTK_MENU_SHELL(_p), menuItem);
		else
			gtk_menu_shell_insert(GTK_MENU_SHELL(_p), menuItem, index);
	}
	if (item != 0) {
		_W_MENUITEM(item)->widget = menuItem;
		_W_MENUITEM(item)->menu = _W_MENUITEM(parent)->menu;
		_W_MENUITEM(item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
				W_MENU_GET_CLASS(_W_MENUITEM(parent)->menu)->class_menuitem);
	}
	gtk_widget_show(menuItem);
	if (style != W_SEPARATOR)
		_w_menuitem_set_text_0(menuItem, text);
	struct _w_menu_reserved *reserved = _W_MENU_RESERVED(
			_w_widget_get_reserved(W_WIDGET(_W_MENUITEM(parent)->menu)));
	if (reserved->signal_activate_id == 0) {
		reserved->signal_activate_id = g_signal_lookup("activate",
				gtk_menu_item_get_type());
		reserved->signal_select_id = g_signal_lookup("select",
				gtk_menu_item_get_type());
	}
	_w_widget_connect(menuItem, SIGNAL_ACTIVATE, reserved->signal_activate_id,
	FALSE);
	_w_widget_connect(menuItem, SIGNAL_SELECT, reserved->signal_select_id,
	FALSE);
	_w_widget_connect(menuItem, SIGNAL_SHOW_HELP, 0, FALSE);
	return W_OK;
}
int _w_menuitem_get_accelerator(w_menuitem *item) {
	int accelerator = 0;
	if (GTK_IS_MENU_SHELL(_W_MENUITEM(item)->widget))
		return accelerator;
	GtkAccelLabel *label = GTK_ACCEL_LABEL(
			gtk_bin_get_child(GTK_BIN(_W_MENUITEM(item)->widget)));
	if (label != 0) {
		guint accelerator_key;
		GdkModifierType t;
		gtk_accel_label_get_accel(label, &accelerator_key, &t);
		if (t & GDK_MOD1_MASK) {
			accelerator |= W_ALT;
		}
		if (t & GDK_CONTROL_MASK) {
			accelerator |= W_CTRL;
		}
		if (t & GDK_SHIFT_MASK) {
			accelerator |= W_SHIFT;
		}
		accelerator |= (accelerator_key & 0xFFFF);
	}
	return accelerator;
}
void* _w_menuitem_get_data(w_item *item) {
	return g_object_get_qdata(G_OBJECT(_W_MENUITEM(item)->widget),
			gtk_toolkit->quark[1]);
}
wresult _w_menuitem_get_enabled(w_menuitem *item) {
	return gtk_widget_get_sensitive(_W_MENUITEM(item)->widget);
}
wresult _w_menuitem_get_item(w_menuitem *item, unsigned int index,
		w_menuitem *sub_item) {
	GtkWidget *_p, *c = 0;
	_p = _w_menuitem_get_submenu(item);
	if (_p == 0)
		return W_FALSE;
	if (GTK_IS_CONTAINER(_p)) {
		struct _w_widget_find_child find;
		memset(&find, 0, sizeof(find));
		find.index = index;
		gtk_container_forall(GTK_CONTAINER(_p), _w_widget_find_children, &find);
		c = find.widget;
	}
	if (c == 0) {
		return W_ERROR_CANNOT_GET_ITEM;
	} else {
		_W_MENUITEM(sub_item)->menu = _W_MENUITEM(item)->menu;
		_W_MENUITEM(sub_item)->widget = c;
		_W_MENUITEM(sub_item)->widgetdata.clazz =
		_W_MENUITEM(item)->widgetdata.clazz;
		return W_TRUE;
	}
}
int _w_menuitem_get_item_count(w_menuitem *item) {
	GtkWidget *_p = _w_menuitem_get_submenu(item);
	if (_p == 0)
		return 0;
	if (GTK_IS_CONTAINER(_p)) {
		struct _w_widget_find_child find;
		memset(&find, 0, sizeof(find));
		gtk_container_forall(GTK_CONTAINER(_p), _w_widget_children_count,
				&find);
		return find.count;
	}
	return 0;
}
void _w_menuitem_get_items(w_menuitem *item, w_iterator *items) {
	GtkWidget *_p = _w_menuitem_get_submenu(item);
	if (_p == 0)
		return;
	if (GTK_IS_CONTAINER(_p)) {
		_W_MENUITEM_ITERATOR( items)->clazz = &_w_menuitem_iterator_class;
		_W_MENUITEM_ITERATOR( items)->original = gtk_container_get_children(
				GTK_CONTAINER(_p));
		_W_MENUITEM_ITERATOR( items)->list =
		_W_MENUITEM_ITERATOR( items)->original;
		_W_MENUITEM_ITERATOR( items)->count = -1;
	}
}
unsigned short _w_menuitem_get_id(w_menuitem *item) {
	void *id = g_object_get_qdata(G_OBJECT(_W_MENUITEM(item)->widget),
			gtk_toolkit->quark[2]);
	return (intptr_t) id;
}
wresult _w_menuitem_get_image(w_menuitem *item, w_image *image) {
}
w_widget* _w_menuitem_get_menu(w_item *item) {
	return W_WIDGET(_W_MENUITEM(item)->menu);
}
wresult _w_menuitem_get_parent(w_menuitem *item, w_menuitem *parent) {
	if (GTK_IS_MENU_SHELL(_W_MENUITEM(item)->widget)) {
		return W_FALSE;
	} else {
		GtkWidget *_p = gtk_widget_get_parent(
				gtk_widget_get_parent(_W_MENUITEM(item)->widget));
		if (_p) {
			_W_MENUITEM(parent)->menu = _W_MENUITEM(item)->menu;
			_W_MENUITEM(parent)->widget = _p;
			_W_MENUITEM(parent)->widgetdata.clazz =
			_W_MENUITEM(item)->widgetdata.clazz;
			return W_TRUE;
		} else {
			_W_MENUITEM(parent)->widgetdata.clazz = 0;
			return W_FALSE;
		}
	}
}
wresult _w_menuitem_get_selection(w_menuitem *item) {
	if (GTK_IS_CHECK_MENU_ITEM(
			_W_MENUITEM(item)->widget) || GTK_IS_RADIO_MENU_ITEM(_W_MENUITEM(item)->widget)) {
		return gtk_check_menu_item_get_active(
				GTK_CHECK_MENU_ITEM(_W_MENUITEM(item)->widget));
	} else {
		return W_FALSE;
	}
}
int _w_menuitem_get_style(w_menuitem *item) {
	if (GTK_IS_SEPARATOR_MENU_ITEM(_W_MENUITEM(item)->widget)) {
		return W_SEPARATOR;
	}
	if (GTK_IS_RADIO_MENU_ITEM(_W_MENUITEM(item)->widget)) {
		return W_RADIO;
	}
	if (GTK_IS_CHECK_MENU_ITEM(_W_MENUITEM(item)->widget)) {
		return W_CHECK;
	}
	if (GTK_IS_MENU_SHELL(_W_MENUITEM(item))) {
		return W_CASCADE;
	}
	GtkWidget *widget = gtk_menu_item_get_submenu(
			GTK_MENU_ITEM(_W_MENUITEM(item)->widget));
	if (widget == 0)
		return W_PUSH;
	else
		return W_CASCADE;
}
wresult _w_menuitem_get_text(w_item *item, w_alloc text,void* user_data) {
	GtkLabel *label = GTK_LABEL(
			gtk_bin_get_child(GTK_BIN(_W_MENUITEM(item)->widget)));
	if (label != 0) {
		return _gtk_alloc_set_text(text,user_data, gtk_label_get_label(label), -1,0);
	}
	return 0;
}
wresult _w_menuitem_is_enabled(w_menuitem *item) {
	return gtk_widget_is_sensitive(_W_MENUITEM(item)->widget);
}
wresult _w_menuitem_remove(w_menuitem *item) {
	if (_W_MENUITEM(item)->widget != 0) {
		w_event_item ei;
		ei.event.type = W_EVENT_ITEM_DISPOSE;
		ei.event.platform_event = 0;
		ei.event.widget = W_WIDGET(_W_MENUITEM(item)->menu);
		ei.item = (w_item*) item;
		_w_menuitem_remove_item_0(_W_MENUITEM(item)->widget, &ei);
		return W_TRUE;
	}
	return W_ERROR_NO_HANDLES;
}
wresult _w_menuitem_remove_item(w_menuitem *item, int index) {
	w_menuitem sub_item;
	w_widgetdata_init(W_WIDGETDATA(&sub_item));
	if (_w_menuitem_get_item(item, index, &sub_item) > 0) {
		return _w_menuitem_remove(&sub_item);
	} else
		return W_ERROR_INVALID_RANGE;
}
wresult _w_menuitem_set_accelerator(w_menuitem *item, int accelerator) {
	if (GTK_IS_MENU_SHELL(_W_MENUITEM(item)->widget))
		return W_TRUE;
	GtkAccelLabel *label = GTK_ACCEL_LABEL(
			gtk_bin_get_child(GTK_BIN(_W_MENUITEM(item)->widget)));
	if (label != 0) {
		GdkModifierType t = 0;
		if (accelerator & W_ALT)
			t |= GDK_MOD1_MASK;
		if (accelerator & W_CTRL)
			t |= GDK_CONTROL_MASK;
		if (accelerator & W_SHIFT)
			t |= GDK_SHIFT_MASK;
		gtk_accel_label_set_accel(label, accelerator & 0xFFFF, t);

		GtkAccelGroup *accelgroup = _w_menuitem_get_accel_group(item);
		if (accelgroup == 0)
			return W_ERROR_NO_HANDLES;
		if ((accelerator & W_COMMAND) != 0)
			return W_FALSE;
		GdkModifierType mask = 0;
		if ((accelerator & W_ALT) != 0)
			mask |= GDK_MOD1_MASK;
		if ((accelerator & W_SHIFT) != 0)
			mask |= GDK_SHIFT_MASK;
		if ((accelerator & W_CTRL) != 0)
			mask |= GDK_CONTROL_MASK;
		int keysym = accelerator & W_KEY_MASK;
		int newKey = _w_untranslate_key(keysym);
		if (newKey != 0) {
			keysym = newKey;
		} else {
			switch (keysym) {
			case '\r':
				keysym = GDK_KEY_Return;
				break;
				//default: keysym =keysym /*Converter.wcsToMbcs ((char) keysym)*/;
			}
		}
		/* When accel_key is zero, it causes GTK warnings */
		if (keysym != 0) {
			//if (add) {
			gtk_widget_add_accelerator(_W_MENUITEM(item)->widget, "activate",
					accelgroup, keysym, mask, GTK_ACCEL_VISIBLE);
			/*} else {
			 GTK.gtk_widget_remove_accelerator (handle, accelGroup, keysym, mask);
			 }*/
		}

	}
	return W_TRUE;
}
wresult _w_menuitem_set_data(w_item *item, void *data) {
	g_object_set_qdata(G_OBJECT(_W_MENUITEM(item)->widget),
			gtk_toolkit->quark[1], data);
	return W_TRUE;
}
wresult _w_menuitem_set_enabled(w_menuitem *item, int enabled) {
	gtk_widget_set_sensitive(_W_MENUITEM(item)->widget, enabled);
	return W_TRUE;
}
wresult _w_menuitem_set_id(w_menuitem *item, unsigned short id) {
	intptr_t _id = (intptr_t) g_object_get_qdata(
			G_OBJECT(_W_MENUITEM(item)->widget), gtk_toolkit->quark[2]);
	_id |= (id & 0xFFFF);
	g_object_set_qdata(G_OBJECT(_W_MENUITEM(item)->widget),
			gtk_toolkit->quark[2], (void*) _id);
	return W_TRUE;
}
wresult _w_menuitem_set_image(w_menuitem *item, w_image *image) {
}
wresult _w_menuitem_set_selection(w_menuitem *item, int selected) {
	if (GTK_IS_CHECK_MENU_ITEM(_W_MENUITEM(item)->widget)) {
		gtk_check_menu_item_set_active(
				GTK_CHECK_MENU_ITEM(_W_MENUITEM(item)->widget), selected);
		return W_TRUE;
	}
	return W_FALSE;
}
wresult _w_menuitem_set_text_0(GtkWidget *widget, const char *text) {
	if (text == 0)
		text = "";
	GtkLabel *label = GTK_LABEL(gtk_bin_get_child(GTK_BIN(widget)));
	if (label != 0) {
		struct _mnemonics mn;
		int _l = -1;
		char *s = strchr(text, '\t');
		if (s != 0) {
			_l = (s - text);
		}
		_mnemonics_fix(&mn, text, _l, W_TRUE);
		if (mn.str == 0)
			return W_ERROR_NO_HANDLES;
		gtk_label_set_text_with_mnemonic(label, mn.str);
		_mnemonics_free(&mn, text);
		return W_TRUE;
	} else
		return W_FALSE;
}
wresult _w_menuitem_set_text(w_item *item, const char *string) {
	return _w_menuitem_set_text_0(_W_MENUITEM(item)->widget, string);
}
/*
 * menu
 */
wresult _w_menu_create(w_widget *widget, w_widget *parent, wuint64 style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_NULL_ARGUMENT;
	_W_WIDGET(widget)->state |= STATE_HANDLE;
	GtkWidget *vboxHandle = 0;
	w_shell *_p;
	w_control_get_shell(W_CONTROL(parent),&_p);
	if (_p == 0)
		return W_ERROR_INVALID_ARGUMENT;
	if ((style & W_BAR) != 0) {
		if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_SHELL)
			return W_ERROR_INVALID_ARGUMENT;
		_W_WIDGET(widget)->handle = gtk_menu_bar_new();
		if (_W_WIDGET(widget)->handle == 0)
			goto _err;
		vboxHandle = _W_SHELL_VBOX(_p);
		gtk_container_add(GTK_CONTAINER(vboxHandle), _W_WIDGET(widget)->handle);
		gtk_box_set_child_packing(GTK_BOX(vboxHandle),
		_W_WIDGET(widget)->handle, FALSE,
		TRUE, 0, GTK_PACK_START);
	} else {
		if (w_widget_class_id(W_WIDGET(parent)) < _W_CLASS_CONTROL)
			return W_ERROR_INVALID_ARGUMENT;
		_W_WIDGET(widget)->handle = gtk_menu_new();
		if (_W_WIDGET(widget)->handle == 0)
			goto _err;
	}
	_W_MENU(widget)->parent = W_CONTROL(parent);
	_W_WIDGET(widget)->style = style;
	_W_WIDGET(widget)->post_event = post_event;
	_W_MENU(widget)->x = -1;
	g_object_set_qdata(G_OBJECT(_W_WIDGET(widget)->handle),
			gtk_toolkit->quark[0], widget);
	//struct _w_widget_reserved *reserved = _w_widget_reserved(widget);
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_SHOW, 0, FALSE);
	_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_HIDE, 0, FALSE);
	//_w_widget_connect(_W_WIDGET(widget)->handle, SIGNAL_SHOW_HELP, FALSE);

	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}

void _w_menu_get_bounds(w_menu *menu, w_rect *bounds) {
}
wresult _w_menu_get_root(w_menu *menu, w_menuitem *rootitem) {
	if (rootitem == 0)
		return W_ERROR_NULL_ARGUMENT;
	_W_MENUITEM(rootitem)->menu = menu;
	_W_MENUITEM(rootitem)->widget = _W_WIDGET(menu)->handle;
	W_WIDGETDATA(rootitem)->clazz = W_WIDGETDATA_CLASS(
			W_MENU_GET_CLASS(menu)->class_menuitem);
	return W_TRUE;
}
void _w_menu_dispose(w_widget *widget) {
	/*w_menuitem rootitem;
	_w_menu_get_root(W_MENU(widget), &rootitem);
	_w_menuitem_remove(&rootitem);*/
	_w_widget_dispose(widget);
}
int _w_menu_get_orientation(w_menu *menu) {
}
w_control* _w_menu_get_parent(w_menu *menu) {
	return _W_MENU(menu)->parent;
}
wresult _w_menu_get_visible(w_menu *menu) {
}
wresult _w_menu_is_visible(w_menu *menu) {
}
void _w_menu_set_location(w_menu *menu, w_point *location) {
	if ((_W_WIDGET(menu)->style & (W_BAR | W_DROP_DOWN)) != 0) return;
	_W_MENU(menu)->x = location->x;
	_W_MENU(menu)->y = location->y;
	//_W_MENU(menu)->hasLocation = true;
}
void _w_menu_set_orientation_0(w_menu *menu, int orientation) {
}
void _w_menu_set_orientation(w_menu *menu, int orientation) {
}
void _w_menu_position_func(GtkMenu *menu, gint *x, gint *y, gboolean *push_in,
		gpointer user_data) {
	*push_in = TRUE;
	*x = _W_MENU(menu)->x;
	*y = _W_MENU(menu)->y;
}
wresult _w_menu_set_visible(w_menu *menu, int visible) {
	if ((_W_WIDGET(menu)->style & (W_BAR | W_DROP_DOWN)) != 0)
		return W_FALSE;
	if (visible) {
		//sendEvent (SWT.Show);
		//if (getItemCount () != 0) {
		if ((GTK_VERSION >= VERSION(2, 8, 0))) {
			/*
			 * Feature in GTK. ON_TOP shells will send out
			 * SWT.Deactivate whenever a context menu is shown.
			 * The fix is to prevent the menu from taking focus
			 * when it is being shown in an ON_TOP shell.
			 */
			/*if ((parent._getShell ().style & SWT.ON_TOP) != 0) {
			 OS.gtk_menu_shell_set_take_focus (handle, false);
			 }*/
		}
		GtkMenuPositionFunc func =
				(_W_MENU(menu)->x >= 0) ? _w_menu_position_func : 0;
		if ((GTK_VERSION >= VERSION(2, 10, 0))) {
			/*
			 * Popup-menu to the status icon should be aligned to
			 * Tray rather than to cursor position. There is a
			 * possibility (unlikely) that TrayItem might have
			 * been disposed in the listener, for which case
			 * the menu should be shown in the cursor position.
			 */
			/*TrayItem item = display.currentTrayItem;
			 if (item != null && !item.isDisposed()) {
			 data = item.handle;
			 address = OS.gtk_status_icon_position_menu_func ();
			 }*/
		}
		/*
		 * Bug in GTK.  The timestamp passed into gtk_menu_popup is used
		 * to perform an X pointer grab.  It cannot be zero, else the grab
		 * will fail.  The fix is to ensure that the timestamp of the last
		 * event processed is used.
		 */
		gtk_menu_popup(GTK_MENU(_W_WIDGET(menu)->handle), 0, 0, func, menu, 0,
				gdk_event_get_time (gdk_event_peek()));
		/*} else {
		 sendEvent (SWT.Hide);
		 }*/
	} else {
		gtk_menu_popdown(GTK_MENU(_W_WIDGET(menu)->handle));
	}
	return W_TRUE;
}
/*
 * post event
 */
int _w_menu_post_event(w_widget *widget, w_event *e) {
	if (e->type == W_EVENT_PLATFORM) {
		if (gdk_pointer_is_grabbed()) {
			gdk_pointer_ungrab(0);
		}
		struct _w_event_platform *ee = (struct _w_event_platform*) e;
		w_menuitem item;
		switch (ee->msg) {
		case SIGNAL_ACTIVATE: {
			w_event_item ei;
			ei.event.type = W_EVENT_ITEM_SELECTION;
			ei.event.platform_event = (struct w_event_platform*) ee;
			ei.event.widget = widget;
			ei.item = (w_item*) &item;
			_W_MENUITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					W_MENU_GET_CLASS(widget)->class_menuitem);
			_W_MENUITEM(&item)->menu = W_MENU(widget);
			_W_MENUITEM(&item)->widget = ee->widget;
			_w_widget_send_event(widget, (w_event*) &ei);
			ee->result = FALSE;
			return W_TRUE;
		}
			break;
		case SIGNAL_SELECT: {
			w_event_item ei;
			ei.event.type = W_EVENT_ITEM_ARM;
			ei.event.platform_event = (struct w_event_platform*) ee;
			ei.event.widget = widget;
			ei.item = (w_item*) &item;
			_W_MENUITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					W_MENU_GET_CLASS(widget)->class_menuitem);
			_W_MENUITEM(&item)->menu = W_MENU(widget);
			_W_MENUITEM(&item)->widget = ee->widget;
			_w_widget_send_event(widget, (w_event*) &ei);
			ee->result = FALSE;
			return W_TRUE;
		}
			break;
		case SIGNAL_SHOW_HELP: {
			w_event_item ei;
			ei.event.type = W_EVENT_ITEM_HELP;
			ei.event.platform_event = (struct w_event_platform*) ee;
			ei.event.widget = widget;
			ei.item = (w_item*) &item;
			_W_MENUITEM(&item)->widgetdata.clazz = W_WIDGETDATA_CLASS(
					W_MENU_GET_CLASS(widget)->class_menuitem);
			_W_MENUITEM(&item)->menu = W_MENU(widget);
			_W_MENUITEM(&item)->widget = ee->widget;
			_w_widget_send_event(widget, (w_event*) &ei);
			ee->result = FALSE;
			return W_TRUE;
		}
			break;
		case SIGNAL_SHOW: {
			w_event ei;
			ei.type = W_EVENT_SHOW;
			ei.platform_event = (struct w_event_platform*) ee;
			ei.widget = e->widget;
			_w_widget_send_event(widget, (w_event*) &ei);
			ee->result = FALSE;
			return W_TRUE;
		}
			break;
		case SIGNAL_HIDE: {
			w_event ei;
			ei.type = W_EVENT_HIDE;
			ei.platform_event = (struct w_event_platform*) ee;
			ei.widget = e->widget;
			_w_widget_send_event(widget, (w_event*) &ei);
			ee->result = FALSE;
			return W_TRUE;
		}
			break;
		default:
			break;
		}

	} else
		return W_TRUE;
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
	W_WIDGET_CLASS(clazz)->dispose = _w_menu_dispose;
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
	item->item.widgetdata.toolkit = W_TOOLKIT(gtk_toolkit);
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
}
#endif
