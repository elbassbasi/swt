/*
 * widget.c
 *
 *  Created on: 21 avr. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "widget.h"
#include "toolkit.h"
/*
 *
 */
GdkWindow* _gdk_window_get_device_position(GdkWindow *window, gint *x, gint *y,
		GdkModifierType *mask) {
#if GTK3
	GdkDisplay *display = 0;
	if (window != 0) {
		display = gdk_window_get_display(window);
	} else {
		window = gdk_get_default_root_window();
		display = gdk_window_get_display(window);
	}
	GdkDeviceManager *device_manager = gdk_display_get_device_manager(display);
	GdkDevice *pointer = gdk_device_manager_get_client_pointer(device_manager);
	return gdk_window_get_device_position(window, pointer, x, y, mask);
#else
		return gdk_window_get_pointer(window, x, y, mask);
#endif
}
int _w_widget_font_height(const PangoFontDescription *font, GtkWidget *widgetHandle) {
	PangoContext *context = gtk_widget_get_pango_context(widgetHandle);
	PangoLanguage *lang = pango_context_get_language(context);
	PangoFontMetrics *metrics = pango_context_get_metrics(context, font, lang);
	int ascent = pango_font_metrics_get_ascent(metrics);
	int descent = pango_font_metrics_get_descent(metrics);
	pango_font_metrics_unref(metrics);
	return PANGO_PIXELS(ascent + descent);
}
void _gtk_widget_get_preferred_size(GtkWidget *widget,
		GtkRequisition *requisition) {
#if GTK3
	gtk_widget_get_preferred_size(widget, requisition, 0);
#else
		gtk_widget_size_request (widget, requisition);
#endif
}
GtkWidget* _w_widget_h0(w_widget *widget, struct _w_widget_reserved *reserved) {
	return 0;
}
GtkWidget* _w_widget_h(w_widget *widget, struct _w_widget_reserved *reserved) {
	return _W_WIDGET(widget)->handle;
}
GtkWidget* _w_widget_hp(w_widget *widget, struct _w_widget_reserved *reserved) {
	return gtk_widget_get_parent(_W_WIDGET(widget)->handle);
}
GtkWidget* _w_widget_hpp(w_widget *widget,
		struct _w_widget_reserved *reserved) {
	return gtk_widget_get_parent(
			gtk_widget_get_parent(_W_WIDGET(widget)->handle));
}
GtkWidget* _w_widget_hppp(w_widget *widget,
		struct _w_widget_reserved *reserved) {
	return gtk_widget_get_parent(
			gtk_widget_get_parent(
					gtk_widget_get_parent(_W_WIDGET(widget)->handle)));
}
void _w_widget_check_open(w_widget *widget, _w_widget_reserved *reserved) {

}
void _w_widget_set_font_description(w_widget *control, GtkWidget *widget,
		PangoFontDescription *font, _w_widget_reserved *reserved) {
#if GTK3
	gtk_widget_override_font(widget, font);
	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	gtk_style_context_invalidate(context);
#endif
#if GTK2
#endif
}
void _w_widget_check_orientation(w_widget *widget, w_widget *parent,
		_w_widget_reserved *reserved) {
	_W_WIDGET(widget)->style &= ~W_MIRRORED;
	if ((_W_WIDGET(widget)->style & (W_LEFT_TO_RIGHT | W_RIGHT_TO_LEFT)) == 0) {
		if (_W_CONTROL(widget)->parent != 0) {
			if ((_W_WIDGET(_W_CONTROL(widget)->parent)->style & W_LEFT_TO_RIGHT)
					!= 0)
				_W_WIDGET(widget)->style |= W_LEFT_TO_RIGHT;
			if ((_W_WIDGET(_W_CONTROL(widget)->parent)->style & W_RIGHT_TO_LEFT)
					!= 0)
				_W_WIDGET(widget)->style |= W_RIGHT_TO_LEFT;
		}
	}
	_W_WIDGET(widget)->style = _w_widget_check_bits(_W_WIDGET(widget)->style,
			W_LEFT_TO_RIGHT, W_RIGHT_TO_LEFT, 0, 0, 0, 0);
}
wresult _w_widget_create_widget(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	wresult ret = reserved->create_handle(widget, index, reserved);
	if (ret < 0)
		return ret;
	reserved->set_orientation(widget, W_TRUE, reserved);
	reserved->hook_events(widget, reserved);
	return ret;
}
wresult _w_widget_create_handle(w_widget *widget, int index,
		_w_widget_reserved *reserved) {
	return W_TRUE;
}
void _w_widget_set_orientation(w_widget *widget, wbool create,
		_w_widget_reserved *reserved) {
}
void _w_widget_hook_events(w_widget *widget, _w_widget_reserved *reserved) {
}
int _w_widget_check_bits(int style, int int0, int int1, int int2, int int3,
		int int4, int int5) {
	int mask = int0 | int1 | int2 | int3 | int4 | int5;
	if ((style & mask) == 0)
		style |= int0;
	if ((style & int0) != 0)
		style = (style & ~mask) | int0;
	if ((style & int1) != 0)
		style = (style & ~mask) | int1;
	if ((style & int2) != 0)
		style = (style & ~mask) | int2;
	if ((style & int3) != 0)
		style = (style & ~mask) | int3;
	if ((style & int4) != 0)
		style = (style & ~mask) | int4;
	if ((style & int5) != 0)
		style = (style & ~mask) | int5;
	return style;
}
_w_widget_reserved* _w_widget_get_reserved(w_widget *widget) {
	struct _w_widget_class *clazz = W_WIDGET_GET_CLASS(widget);
	while (clazz->toolkit != (w_toolkit*) gtk_toolkit) {
		clazz = clazz->next_class;
	}
	return (struct _w_widget_reserved*) clazz->reserved[0];
}
void _w_widget_find_children(GtkWidget *widget, gpointer data) {
	_w_widget_find_child *i = (_w_widget_find_child*) data;
	if (i->index >= 0) {
		if (i->count == i->index) {
			i->widget = widget;
		}
		if (i->count == i->index - 1) {
			i->prev = widget;
		}
		if (i->count == i->index + 1) {
			i->next = widget;
		}
	} else {
		i->prev = i->widget;
		i->widget = widget;
	}
	i->count++;
}
void _w_widget_children_count(GtkWidget *widget, gpointer data) {
	((_w_widget_find_child*) data)->count++;
}
gboolean _w_widget_proc(GtkWidget *widget, int msg, void *args0, void *args1,
		void *args2) {
	w_widget *cc = (w_widget*) g_object_get_qdata(G_OBJECT(widget),
			gtk_toolkit->quark[0]);
	if (cc == 0)
		return FALSE;
	else {
		struct _w_event_platform e;
		e.event.type = W_EVENT_PLATFORM;
		e.event.widget = cc;
		e.event.platform_event = 0;
		e.event.data = 0;
		e.msg = msg;
		e.result = FALSE;
		e.widget = widget;
		e.args[0] = args0;
		e.args[1] = args1;
		e.args[2] = args2;
		_w_widget_send_event(cc, (w_event*) &e);
		return e.result;
	}
}
gboolean _w_widget_proc_2(GtkWidget *widget, void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, 0, 0, 0);
}
gboolean _w_widget_proc_3(GtkWidget *widget, void *args0, void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, args0, 0, 0);
}
gboolean _w_widget_proc_4(GtkWidget *widget, void *args0, void *args1,
		void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, args0, args1, 0);
}
gboolean _w_widget_proc_5(GtkWidget *widget, void *args0, void *args1,
		void *args2, void *msg) {
	return _w_widget_proc(widget, (intptr_t) msg, args0, args1, args2);
}

void _w_signal_lookup() {
	if (gtk_toolkit->closures[SIGNAL_BUTTON_PRESS_EVENT].signal_id != 0)
		return;
	gtk_toolkit->closures[SIGNAL_BUTTON_PRESS_EVENT].signal_id =
			g_signal_lookup("button-press-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_BUTTON_RELEASE_EVENT].signal_id =
			g_signal_lookup("button-release-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_CONFIGURE_EVENT].signal_id = g_signal_lookup(
			"configure-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DELETE_EVENT].signal_id = g_signal_lookup(
			"delete-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_ENTER_NOTIFY_EVENT].signal_id =
			g_signal_lookup("enter-notify-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_EVENT].signal_id = g_signal_lookup("event",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_EVENT_AFTER].signal_id = g_signal_lookup(
			"event-after", GTK_TYPE_WIDGET);
#if GTK3
	gtk_toolkit->closures[SIGNAL_EXPOSE_EVENT].signal_id = g_signal_lookup(
			"draw", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_EXPOSE_EVENT_INVERSE].signal_id =
			gtk_toolkit->closures[SIGNAL_EXPOSE_EVENT].signal_id;
#else
	gtk_toolkit->closures[EXPOSE_EVENT].signal_id = g_signal_lookup( "expose-event",GTK_TYPE_WIDGET);
	gtk_toolkit->closures[EXPOSE_EVENT_INVERSE].signal_id = g_signal_lookup( "expose-event",GTK_TYPE_WIDGET);
#endif
	gtk_toolkit->closures[SIGNAL_FOCUS].signal_id = g_signal_lookup("focus",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_FOCUS_IN_EVENT].signal_id = g_signal_lookup(
			"focus-in-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_FOCUS_OUT_EVENT].signal_id = g_signal_lookup(
			"focus-out-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_GRAB_FOCUS].signal_id = g_signal_lookup(
			"grab-focus", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_HIDE].signal_id = g_signal_lookup("hide",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_KEY_PRESS_EVENT].signal_id = g_signal_lookup(
			"key-press-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_KEY_RELEASE_EVENT].signal_id = g_signal_lookup(
			"key-release-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_LEAVE_NOTIFY_EVENT].signal_id =
			g_signal_lookup("leave-notify-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_MAP].signal_id = g_signal_lookup("map",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_MAP_EVENT].signal_id = g_signal_lookup(
			"map-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_MNEMONIC_ACTIVATE].signal_id = g_signal_lookup(
			"mnemonic-activate", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_MOTION_NOTIFY_EVENT].signal_id =
			g_signal_lookup("motion-notify-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_POPUP_MENU].signal_id = g_signal_lookup(
			"popup-menu", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_REALIZE].signal_id = g_signal_lookup("realize",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_SCROLL_EVENT].signal_id = g_signal_lookup(
			"scroll-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_SHOW].signal_id = g_signal_lookup("show",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_SHOW_HELP].signal_id = g_signal_lookup(
			"show-help", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_SIZE_ALLOCATE].signal_id = g_signal_lookup(
			"size-allocate", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_STYLE_SET].signal_id = g_signal_lookup(
			"style-set", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_UNMAP].signal_id = g_signal_lookup("unmap",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_UNMAP_EVENT].signal_id = g_signal_lookup(
			"unmap-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_UNREALIZE].signal_id = g_signal_lookup(
			"unrealize", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_WINDOW_STATE_EVENT].signal_id =
			g_signal_lookup("window-state-event", GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DESTROY].signal_id = g_signal_lookup("destroy",
	GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DRAG_DATA_GET].signal_id = g_signal_lookup(
			"drag_data_get",
			GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DRAG_END].signal_id = g_signal_lookup(
			"drag_end",
			GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DRAG_DATA_DELETE].signal_id = g_signal_lookup(
			"drag_data_delete",
			GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DRAG_MOTION].signal_id = g_signal_lookup(
			"drag_motion",
			GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DRAG_LEAVE].signal_id = g_signal_lookup(
			"drag_leave",
			GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DRAG_DATA_RECEIVED].signal_id =
			g_signal_lookup("drag_data_received",
			GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_DRAG_DROP].signal_id = g_signal_lookup(
			"drag_drop",
			GTK_TYPE_WIDGET);
	gtk_toolkit->closures[SIGNAL_CHANGE_VALUE].signal_id = g_signal_lookup(
			"change-value",
			GTK_TYPE_SCROLLBAR);
	gtk_toolkit->closures[SIGNAL_VALUE_CHANGED].signal_id = g_signal_lookup(
			"value-changed",
			GTK_TYPE_ADJUSTMENT);
}
void _w_widget_connect(GtkWidget *widget, wushort signal, wushort signal_id,
		gboolean after) {
	if (gtk_toolkit->closures[signal].widget_closures == 0) {
		GCallback callback;
		if (signal >= SIGNAL_5_ARGS) {
			callback = (GCallback) _w_widget_proc_5;
		} else if (signal >= SIGNAL_4_ARGS) {
			callback = (GCallback) _w_widget_proc_4;
		} else if (signal >= SIGNAL_3_ARGS) {
			callback = (GCallback) _w_widget_proc_3;
		} else if (signal >= SIGNAL_2_ARGS) {
			callback = (GCallback) _w_widget_proc_2;
		}
		gtk_toolkit->closures[signal].widget_closures = g_cclosure_new(callback,
				(gpointer) (long) signal, 0);
		if (gtk_toolkit->closures[signal].widget_closures == 0)
			return;
	}
	if (signal_id == 0) {
		_w_signal_lookup();
		signal_id = gtk_toolkit->closures[signal].signal_id;
	}
	g_signal_connect_closure_by_id(widget, signal_id, 0,
			gtk_toolkit->closures[signal].widget_closures, after);
}
int _w_widget_set_input_state(int state) {
	int statemask = 0;
	if ((state & GDK_MOD1_MASK) != 0)
		statemask |= W_ALT;
	if ((state & GDK_SHIFT_MASK) != 0)
		statemask |= W_SHIFT;
	if ((state & GDK_CONTROL_MASK) != 0)
		statemask |= W_CTRL;
	if ((state & GDK_BUTTON1_MASK) != 0)
		statemask |= W_BUTTON1;
	if ((state & GDK_BUTTON2_MASK) != 0)
		statemask |= W_BUTTON2;
	if ((state & GDK_BUTTON3_MASK) != 0)
		statemask |= W_BUTTON3;
	return statemask;
}
int KeyTable[][2] = {

/* Keyboard and Mouse Masks */
{ GDK_KEY_Alt_L, W_ALT }, //
		{ GDK_KEY_Alt_R, W_ALT }, //
		{ GDK_KEY_Meta_L, W_ALT }, //
		{ GDK_KEY_Meta_R, W_ALT }, //
		{ GDK_KEY_Shift_L, W_SHIFT }, //
		{ GDK_KEY_Shift_R, W_SHIFT }, //
		{ GDK_KEY_Control_L, W_CTRL }, //
		{ GDK_KEY_Control_R, W_CTRL }, //
		{ GDK_KEY_ISO_Level3_Shift, W_ALT }, //
//		{OS.GDK_KEY_????,		W_COMMAND},
//		{OS.GDK_KEY_????,		W_COMMAND},

		/* Non-Numeric Keypad Keys */
		{ GDK_KEY_Up, W_ARROW_UP }, //
		{ GDK_KEY_KP_Up, W_ARROW_UP }, //
		{ GDK_KEY_Down, W_ARROW_DOWN }, //
		{ GDK_KEY_KP_Down, W_ARROW_DOWN }, //
		{ GDK_KEY_Left, W_ARROW_LEFT }, //
		{ GDK_KEY_KP_Left, W_ARROW_LEFT }, //
		{ GDK_KEY_Right, W_ARROW_RIGHT }, //
		{ GDK_KEY_KP_Right, W_ARROW_RIGHT }, //
		{ GDK_KEY_Page_Up, W_PAGE_UP }, //
		{ GDK_KEY_KP_Page_Up, W_PAGE_UP }, //
		{ GDK_KEY_Page_Down, W_PAGE_DOWN }, //
		{ GDK_KEY_KP_Page_Down, W_PAGE_DOWN }, //
		{ GDK_KEY_Home, W_HOME }, //
		{ GDK_KEY_KP_Home, W_HOME }, //
		{ GDK_KEY_End, W_END }, //
		{ GDK_KEY_KP_End, W_END }, //
		{ GDK_KEY_Insert, W_INSERT }, //
		{ GDK_KEY_KP_Insert, W_INSERT }, //

		/* Virtual and Ascii Keys */
		{ GDK_KEY_BackSpace, W_BS }, //
		{ GDK_KEY_Return, W_CR }, //
		{ GDK_KEY_Delete, W_DEL }, //
		{ GDK_KEY_KP_Delete, W_DEL }, //
		{ GDK_KEY_Escape, W_ESC }, //
		{ GDK_KEY_Linefeed, W_LF }, //
		{ GDK_KEY_Tab, W_TAB }, //
		{ GDK_KEY_ISO_Left_Tab, W_TAB }, //

		/* Functions Keys */
		{ GDK_KEY_F1, W_F1 }, //
		{ GDK_KEY_F2, W_F2 }, //
		{ GDK_KEY_F3, W_F3 }, //
		{ GDK_KEY_F4, W_F4 }, //
		{ GDK_KEY_F5, W_F5 }, //
		{ GDK_KEY_F6, W_F6 }, //
		{ GDK_KEY_F7, W_F7 }, //
		{ GDK_KEY_F8, W_F8 }, //
		{ GDK_KEY_F9, W_F9 }, //
		{ GDK_KEY_F10, W_F10 }, //
		{ GDK_KEY_F11, W_F11 }, //
		{ GDK_KEY_F12, W_F12 }, //
		{ GDK_KEY_F13, W_F13 }, //
		{ GDK_KEY_F14, W_F14 }, //
		{ GDK_KEY_F15, W_F15 }, //
		{ GDK_KEY_F16, W_F16 }, //
		{ GDK_KEY_F17, W_F17 }, //
		{ GDK_KEY_F18, W_F18 }, //
		{ GDK_KEY_F19, W_F19 }, //
		{ GDK_KEY_F20, W_F20 }, //

		/* Numeric Keypad Keys */
		{ GDK_KEY_KP_Multiply, W_KEYPAD_MULTIPLY }, //
		{ GDK_KEY_KP_Add, W_KEYPAD_ADD }, //
		{ GDK_KEY_KP_Enter, W_KEYPAD_CR }, //
		{ GDK_KEY_KP_Subtract, W_KEYPAD_SUBTRACT }, //
		{ GDK_KEY_KP_Decimal, W_KEYPAD_DECIMAL }, //
		{ GDK_KEY_KP_Divide, W_KEYPAD_DIVIDE }, //
		{ GDK_KEY_KP_0, W_KEYPAD_0 }, //
		{ GDK_KEY_KP_1, W_KEYPAD_1 }, //
		{ GDK_KEY_KP_2, W_KEYPAD_2 }, //
		{ GDK_KEY_KP_3, W_KEYPAD_3 }, //
		{ GDK_KEY_KP_4, W_KEYPAD_4 }, //
		{ GDK_KEY_KP_5, W_KEYPAD_5 }, //
		{ GDK_KEY_KP_6, W_KEYPAD_6 }, //
		{ GDK_KEY_KP_7, W_KEYPAD_7 }, //
		{ GDK_KEY_KP_8, W_KEYPAD_8 }, //
		{ GDK_KEY_KP_9, W_KEYPAD_9 }, //
		{ GDK_KEY_KP_Equal, W_KEYPAD_EQUAL }, //

		/* Other keys */
		{ GDK_KEY_Caps_Lock, W_CAPS_LOCK }, //
		{ GDK_KEY_Num_Lock, W_NUM_LOCK }, //
		{ GDK_KEY_Scroll_Lock, W_SCROLL_LOCK }, //
		{ GDK_KEY_Pause, W_PAUSE }, //
		{ GDK_KEY_Break, W_BREAK }, //
		{ GDK_KEY_Print, W_PRINT_SCREEN }, //
		{ GDK_KEY_Help, W_HELP }, //

		};
int _w_translate_key(int key) {
	for (int i = 0; i < (int) (sizeof(KeyTable) / sizeof(KeyTable[0])); i++) {
		if (KeyTable[i][0] == key)
			return KeyTable[i][1];
	}
	return 0;
}

int _w_untranslate_key(int key) {
	for (int i = 0; i < (int) (sizeof(KeyTable) / sizeof(KeyTable[0])); i++) {
		if (KeyTable[i][1] == key)
			return KeyTable[i][0];
	}
	return 0;
}

void _w_widget_set_location_state(w_event_key *event, GdkEventKey *keyEvent) {
	switch (keyEvent->keyval) {
	case GDK_KEY_Alt_L:
	case GDK_KEY_Shift_L:
	case GDK_KEY_Control_L:
		event->keylocation = W_LEFT;
		break;
	case GDK_KEY_Alt_R:
	case GDK_KEY_Shift_R:
	case GDK_KEY_Control_R:
		event->keylocation = W_RIGHT;
		break;
	case GDK_KEY_KP_0:
	case GDK_KEY_KP_1:
	case GDK_KEY_KP_2:
	case GDK_KEY_KP_3:
	case GDK_KEY_KP_4:
	case GDK_KEY_KP_5:
	case GDK_KEY_KP_6:
	case GDK_KEY_KP_7:
	case GDK_KEY_KP_8:
	case GDK_KEY_KP_9:
	case GDK_KEY_KP_Add:
	case GDK_KEY_KP_Decimal:
	case GDK_KEY_KP_Delete:
	case GDK_KEY_KP_Divide:
	case GDK_KEY_KP_Down:
	case GDK_KEY_KP_End:
	case GDK_KEY_KP_Enter:
	case GDK_KEY_KP_Equal:
	case GDK_KEY_KP_Home:
	case GDK_KEY_KP_Insert:
	case GDK_KEY_KP_Left:
	case GDK_KEY_KP_Multiply:
	case GDK_KEY_KP_Page_Down:
	case GDK_KEY_KP_Page_Up:
	case GDK_KEY_KP_Right:
	case GDK_KEY_KP_Subtract:
	case GDK_KEY_KP_Up:
	case GDK_KEY_Num_Lock:
		event->keylocation = W_KEYPAD;
		break;
	}
}
gboolean _w_widget_set_key_state(w_event_key *event, GdkEventKey *keyEvent,
		int unicode_length) {
	if (keyEvent->string != 0 && unicode_length > 1)
		return FALSE;
	gboolean isNull = FALSE;
	event->keycode = _w_translate_key(keyEvent->keyval);
	switch (keyEvent->keyval) {
	case GDK_KEY_BackSpace:
		event->character = W_BS;
		break;
	case GDK_KEY_Linefeed:
		event->character = W_LF;
		break;
	case GDK_KEY_KP_Enter:
	case GDK_KEY_Return:
		event->character = W_CR;
		break;
	case GDK_KEY_KP_Delete:
	case GDK_KEY_Delete:
		event->character = W_DEL;
		break;
	case GDK_KEY_Escape:
		event->character = W_ESC;
		break;
	case GDK_KEY_Tab:
	case GDK_KEY_ISO_Left_Tab:
		event->character = W_TAB;
		break;
	default: {
		if (event->keycode == 0) {
			guint keyval = 0;
			gint effective_group, level;
			GdkModifierType consumed_modifiers;
			if (gdk_keymap_translate_keyboard_state(gdk_keymap_get_default(),
					keyEvent->hardware_keycode, (GdkModifierType) 0,
					gtk_toolkit->latinKeyGroup, &keyval, &effective_group,
					&level, &consumed_modifiers)) {
				event->keycode = (int) gdk_keyval_to_unicode(keyval);
			}
		}
		int key = keyEvent->keyval;
		if ((keyEvent->state & GDK_CONTROL_MASK) != 0
				&& (0 <= key && key <= 0x7F)) {
			if ('a' <= key && key <= 'z')
				key -= 'a' - 'A';
			if (64 <= key && key <= 95)
				key -= 64;
			event->character = key;
			isNull = keyEvent->keyval == '@' && key == 0;
		} else {
			if (keyEvent->string != 0) {
				event->character = g_utf8_get_char(keyEvent->string);
			} else {
				event->character = gdk_keyval_to_unicode(key);
			}
		}
	}
	}
	_w_widget_set_location_state(event, keyEvent);
	if (event->keycode == 0 && event->character == 0) {
		if (!isNull)
			return FALSE;
	}
	event->statemask = _w_widget_set_input_state(keyEvent->state);
	return TRUE;
}

gboolean _w_widget_send_IM_key_event(w_widget *widget, _w_event_platform *e,
		int type, GdkEventKey *keyEvent, const char *chars, int length) {
	GdkEventKey *ptr = 0;
	int index = 0, count = 0;
	GdkModifierType state = 0;
	w_event_key event;
	if (keyEvent == 0) {
		keyEvent = (GdkEventKey*) gtk_get_current_event();
		ptr = keyEvent;
		if (keyEvent != 0) {
			switch (keyEvent->type) {
			case GDK_KEY_PRESS:
			case GDK_KEY_RELEASE:
				state = keyEvent->state;
				break;
			default:
				keyEvent = 0;
				break;
			}
		}
	}
	if (keyEvent == 0) {
		gtk_get_current_event_state(&state);
	}
	gunichar c;
	while (index < length) {
		c = g_utf8_get_char(&chars[index]);
		if (c == 0)
			break;
		event.event.type = type;
		event.event.time = keyEvent->time;
		event.event.platform_event = (w_event_platform*) e;
		event.event.widget = widget;
		event.event.time = keyEvent->time;
		if (keyEvent != 0 && keyEvent->length <= 1) {
			_w_widget_set_key_state(&event, keyEvent, length);
		} else {
			event.statemask = _w_widget_set_input_state(state);
		}
		event.character = c;
		int doit = _w_widget_send_event(widget, (w_event*) &event);

		/*
		 * It is possible (but unlikely), that application
		 * code could have disposed the widget in the key
		 * events.  If this happens, end the processing of
		 * the key by returning null.
		 */
		//if (event.doit) chars [count++] = chars [index];
		if (doit)
			count++;
		index += g_utf8_skip[*(const guchar*) (&chars[index])];
	}
	if (ptr != 0)
		gdk_event_free((GdkEvent*) ptr);
	if (count == 0)
		return FALSE;
	return TRUE;
}
gboolean _w_widget_send_key_event(w_widget *widget, _w_event_platform *e) {
	GdkEventKey *keyEvent = (GdkEventKey*) e->args[0];
	w_event_key event;
	int length = keyEvent->length;
	int unicode_length = w_utf8_to_utf16(keyEvent->string, length, 0, 0);
	if (e->msg == SIGNAL_KEY_PRESS_EVENT) {
		event.event.type = W_EVENT_KEYDOWN;
	} else {
		event.event.type = W_EVENT_KEYUP;
	}
	if (keyEvent->string == 0 || unicode_length <= 1) {
		event.event.time = keyEvent->time;
		event.event.platform_event = (struct w_event_platform*) e;
		event.event.widget = widget;
		event.event.time = keyEvent->time;
		if (!_w_widget_set_key_state(&event, keyEvent, unicode_length))
			return TRUE;
		return _w_widget_send_event(widget, (w_event*) &event);
		// widget could be disposed at this point

		/*
		 * It is possible (but unlikely), that application
		 * code could have disposed the widget in the key
		 * events.  If this happens, end the processing of
		 * the key by returning false.
		 */
		//if (isDisposed ()) return false;
		//return event.doit;
	}
	return _w_widget_send_IM_key_event(widget, e, event.event.type, keyEvent,
			keyEvent->string, unicode_length);
}

void _w_widget_get_handles_callback(GtkWidget *widget, gpointer data) {
	if (GTK_IS_BOX(widget)) {
		((_w_widget_handles*) data)->box = widget;
		return;
	}
	if (GTK_IS_IMAGE(widget)) {
		((_w_widget_handles*) data)->image = widget;
		return;
	}
	if (GTK_IS_LABEL(widget)) {
		((_w_widget_handles*) data)->label = widget;
		return;
	}
	if (GTK_IS_ARROW(widget)) {
		((_w_widget_handles*) data)->arrow = widget;
		return;
	}
}
void _w_widget_get_handles(_w_widget_handles *handles) {
	if (GTK_IS_CONTAINER(handles->handle)) {
		gtk_container_forall(GTK_CONTAINER(handles->handle),
				_w_widget_get_handles_callback, handles);
	}
	if (handles->box != 0) {
		gtk_container_forall(GTK_CONTAINER(handles->box),
				_w_widget_get_handles_callback, handles);
	}
}
/*
 * resource data public function
 */

wbool _w_widgetdata_is_ok(w_widgetdata *obj) {
	return W_TRUE;
}
void _w_widgetdata_close(w_widgetdata *obj) {

}
wbool _w_widget_is_ok(w_widget *widget) {
	return W_TRUE;
}
void __w_resource_dispose(w_widget *widget) {

}
void _w_widget_dispose(w_widget *widget) {
	_w_widget_reserved *reserved = _w_widget_get_reserved(W_WIDGET(widget));
	GtkWidget *topHandle = reserved->topHandle(W_WIDGET(widget), reserved);
	gtk_widget_destroy(topHandle);
}
int _w_widget_send_event(w_widget *widget, w_event *event) {
	if (_W_WIDGET(widget)->post_event != 0) {
		return _W_WIDGET(widget)->post_event(widget, event);
	} else {
		return w_widget_default_post_event(widget, event);
	}
}
/*
 *
 */
int _w_widget_post_event(struct w_widget *widget, struct w_event *ee) {
	struct _w_widget_reserved *reserved;
	switch (ee->type) {
	case W_EVENT_PLATFORM: {
		reserved = _w_widget_get_reserved(widget);
		_w_event_platform *e = (_w_event_platform*) ee;
		e->result = reserved->signals[e->msg](widget, e, reserved);
		return W_TRUE;
	}
		break;
	case W_EVENT_COMPUTE_SIZE:
		reserved = _w_widget_get_reserved(widget);
		return reserved->compute_size(widget, (struct w_event_compute_size*) ee,
				reserved);
		break;
	case W_EVENT_CLIENT_AREA:
		reserved = _w_widget_get_reserved(widget);
		return reserved->get_client_area(widget,
				(struct w_event_client_area*) ee, reserved);
		break;
	case W_EVENT_COMPUTE_TRIM:
		reserved = _w_widget_get_reserved(widget);
		return reserved->compute_trim(widget, (struct w_event_compute_trim*) ee,
				reserved);
		break;
	case W_EVENT_LAYOUTDETECT: {
		w_layout *layout;
		w_composite_get_layout(W_COMPOSITE(widget), &layout);
		w_layout_do_layout(layout, W_COMPOSITE(widget), W_FALSE);
		return W_TRUE;
	}
		break;
	}
	return W_FALSE;
}
gboolean _gtk_signal_null(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	return FALSE;
}
gboolean _gtk_widget_expose_inverse(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (GTK_IS_CONTAINER(e->widget)) {
		return reserved->signals[SIGNAL_DRAW](widget, e, reserved);
	}
	return FALSE;
}
gboolean _gtk_widget_expose(w_widget *widget, _w_event_platform *e,
		_w_widget_reserved *reserved) {
	if (!GTK_IS_CONTAINER(e->widget)) {
		return reserved->signals[SIGNAL_DRAW](widget, e, reserved);
	}
	return FALSE;
}
void _w_widget_class_init(struct _w_widget_class *clazz) {
	//resource
	clazz->is_ok = _w_widget_is_ok;
	clazz->dispose = _w_widget_dispose;
	clazz->toolkit = (w_toolkit*) gtk_toolkit;
	clazz->post_event = _w_widget_post_event;
	/*
	 * reserved
	 */
	struct _w_widget_reserved *reserved = W_WIDGET_CLASS(clazz)->reserved[0];
	if (reserved != 0) {
		reserved->topHandle = _w_widget_h;
		reserved->check_open = _w_widget_check_open;
		reserved->check_orientation = _w_widget_check_orientation;
		reserved->create_widget = _w_widget_create_widget;
		reserved->create_handle = _w_widget_create_handle;
		reserved->set_orientation = _w_widget_set_orientation;
		reserved->hook_events = _w_widget_hook_events;
		/*
		 * signals
		 */
		for (int i = 0;
				i < sizeof(reserved->signals) / sizeof(reserved->signals[0]);
				i++) {
			reserved->signals[i] = _gtk_signal_null;
		}
		reserved->compute_size = _gtk_signal_null;
		reserved->get_client_area = _gtk_signal_null;
		reserved->compute_trim = _gtk_signal_null;
		reserved->signals[SIGNAL_EXPOSE_EVENT] = _gtk_widget_expose;
		reserved->signals[SIGNAL_EXPOSE_EVENT_INVERSE] =
				_gtk_widget_expose_inverse;
	}
}
#endif

