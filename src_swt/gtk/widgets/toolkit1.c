/*
 * toolkit1.c
 *
 *  Created on: 21 mai 2020
 *      Author: Azeddine El Bassbasi
 */
#include "toolkit.h"
#ifdef __linux
#include "dnd.h"
/**
 * Disable warning deprecated
 */
#if defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
wuchar trimWidths[] = { 0, 4, 6, 5, 6, 0 };
wuchar trimHeights[] = { 0, 4, 6, 28, 29, 23 };
wbool _w_toolkit_set_warnings(_w_toolkit *toolkit, wbool warning) {
	return FALSE;
}
void* _w_toolkit_malloc(w_toolkit *toolkit, size_t size) {
	if (gtk_toolkit->mem_tmp_allocated + size
			< gtk_toolkit->mem_tmp_total_size) {
		gtk_toolkit->mem_tmp_allocated = size;
		return gtk_toolkit->mem_tmp;
	} else {
		return malloc(size);
	}
}
void* _w_toolkit_realloc(w_toolkit *toolkit, void *ptr, size_t size) {
	if (gtk_toolkit->mem_tmp == ptr) {
		if (size <= gtk_toolkit->mem_tmp_total_size) {
			gtk_toolkit->mem_tmp_allocated = size;
			return gtk_toolkit->mem_tmp;
		} else {
			void *newptr = malloc(size);
			if (newptr != 0) {
				memcpy(newptr, gtk_toolkit->mem_tmp,
						gtk_toolkit->mem_tmp_allocated);
				gtk_toolkit->mem_tmp_allocated = 0;
			}
			return newptr;
		}
	} else
		return realloc(ptr, size);
}
void _w_toolkit_free(w_toolkit *toolkit, void *ptr) {
	if (gtk_toolkit->mem_tmp == ptr) {
		gtk_toolkit->mem_tmp_allocated = 0;
	} else
		free(ptr);
}
void _w_toolkit_registre_free(w_widget *widget) {
	widget->handle = gtk_toolkit->widget_free; //widget->next =
	gtk_toolkit->widget_free = widget;
}
void _w_toolkit_add_shell(_w_shell *shell) {
	shell->next = 0;
	if (gtk_toolkit->shells == 0) {
		gtk_toolkit->shells = shell;
		shell->prev = shell; //last
	} else {
		_w_shell *last = gtk_toolkit->shells->prev;
		last->next = shell;
		shell->prev = last;
		gtk_toolkit->shells->prev = shell;
	}
	gtk_toolkit->shells_count++;
}
void _w_toolkit_remove_shell(_w_shell *shell) {
	if (shell == gtk_toolkit->shells) {
		gtk_toolkit->shells = shell->next;
		if (gtk_toolkit->shells != 0)
			gtk_toolkit->shells->prev = shell->prev; //last
	} else {
		if (shell->next == 0) {
			gtk_toolkit->shells->prev = shell->prev;
		} else {
			shell->next->prev = shell->prev;
		}
		shell->prev->next = shell->next;
	}
	gtk_toolkit->shells_count--;
}
void _w_toolkit_put_gdk_events() {

}
w_color _w_toolkit_GtkStyleContext(GtkStyleContext *context,
		GtkStateFlags flag) {
	GdkRGBA rgba;
	if (GTK_VERSION >= VERSION(3, 18, 0)) {
		gtk_style_context_save(context);
		gtk_style_context_set_state(context, flag);
		gtk_style_context_get_color(context, flag, &rgba);
		gtk_style_context_restore(context);
	} else {
		gtk_style_context_get_color(context, flag, &rgba);
	}
	return W_RGBA(rgba.red * 0xFF, rgba.green * 0xFF, rgba.blue * 0xFF,
			rgba.alpha * 0xFF);
}
w_color _w_toolkit_getBackgroundColor(GtkStyleContext *context,
		GtkStateFlags state) {
	/*
	 * Draw the context background to an offset screen surface and get the color
	 * in the middle of the surface.
	 */
	w_color color;
	gtk_style_context_save(context);
	gtk_style_context_set_state(context, state);
	cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, 1,
			1);
	cairo_t *cairo = cairo_create(surface);
	gtk_render_background(context, cairo, -50, -50, 100, 100);
	cairo_fill(cairo);
	cairo_surface_flush(surface);
	unsigned char *buffer = cairo_image_surface_get_data(surface);
	color = W_RGB(buffer[2], buffer[1], buffer[0]);
	cairo_surface_destroy(surface);
	cairo_destroy(cairo);
	gtk_style_context_restore(context);
	return color;
}
w_color toGdkRGBA(GdkRGBA rgba, double brightness) {
	// Copy RGB values into a new object
	GdkRGBA newRGBA;
	newRGBA.red = rgba.red;
	newRGBA.green = rgba.green;
	newRGBA.blue = rgba.blue;
	newRGBA.alpha = rgba.alpha;

	// Instantiate hue, saturation, and value doubles for HSV.
	// Perform the RGB to HSV conversion.
	double hue;
	double saturationHSV;
	double value;
	gtk_rgb_to_hsv(newRGBA.red, newRGBA.green, newRGBA.blue, &hue,
			&saturationHSV, &value);

	// Calculate luminosity
	double luminosity = ((2 - saturationHSV) * value) / 2;

	// Calculate saturation
	double saturationHSL = saturationHSV * value;
	saturationHSL /= (luminosity <= 1) ? luminosity : 2 - luminosity;

	// Calculate saturation and luminosity with brightness.
	saturationHSL = MAX(0, MIN(1, saturationHSL * brightness));
	luminosity = MAX(0, MIN(1, luminosity * brightness));

	// Convert from HSL (with brightness) back to HSV
	luminosity *= 2;
	saturationHSL *= luminosity <= 1 ? luminosity : 2 - luminosity;
	value = (luminosity + saturationHSL) / 2;
	saturationHSV = (2 * saturationHSL) / (luminosity + saturationHSL);

	// Convert from HSV back to RGB and return the GdkRGBA object
	gtk_hsv_to_rgb(hue, saturationHSV, value, &hue, &saturationHSV, &value);
	newRGBA.red = hue;
	newRGBA.green = saturationHSV;
	newRGBA.blue = value;
	return W_RGB(hue * 0xFF, saturationHSV * 0xFF, value * 0xFF);
}
gboolean _w_toolkit_async_exec_GSourceFunc(gpointer user_data) {
	struct threads_idle *funcs = (struct threads_idle*) user_data;
	funcs->func(funcs->data);
	if (funcs >= &gtk_toolkit->threads_idle[0]
			&& funcs < &gtk_toolkit->threads_idle[threads_idle_count]) {
		funcs->func = 0;
	} else {
		free(funcs);
	}
	return FALSE;
}
gboolean _w_toolkit_sync_exec_GSourceFunc(gpointer user_data) {
	struct threads_idle *funcs = (struct threads_idle*) user_data;
	funcs->func(funcs->data);
	if (funcs >= &gtk_toolkit->threads_idle[0]
			&& funcs < &gtk_toolkit->threads_idle[threads_idle_count]) {
		funcs->func = 0;
	} else {
		free(funcs);
	}
	funcs->signalled = 1;
	pthread_cond_broadcast(&gtk_toolkit->condition);
	return FALSE;
}
void GLogFunc_Empty(const gchar *log_domain, GLogLevelFlags log_level,
		const gchar *message, gpointer user_data) {

}
void _w_toolkit_set_modal_shell(struct _w_toolkit *toolkit, w_shell *shell) {
	_W_WIDGET(shell)->state |= STATE_MODAL_SHELL;
	_w_shell *it = toolkit->shells;
	while (it != 0) {
		_w_shell_update_modal(W_SHELL(it));
		it = it->next;
	}
}
void _w_toolkit_clear_modal(struct _w_toolkit *toolkit, w_shell *shell) {
	_W_WIDGET(shell)->state |= ~STATE_MODAL_SHELL;
	_w_shell *it = toolkit->shells;
	while (it != 0) {
		_w_shell_update_modal(W_SHELL(it));
		it = it->next;
	}
}
void _w_toolkit_init_trims(_w_toolkit *toolkit) {
	toolkit->trimWidths[TRIM_NONE] = 0;
	toolkit->trimHeights[TRIM_NONE] = 0;
	toolkit->trimWidths[TRIM_BORDER] = 4;
	toolkit->trimHeights[TRIM_BORDER] = 4;
	toolkit->trimWidths[TRIM_RESIZE] = 6;
	toolkit->trimHeights[TRIM_RESIZE] = 6;
	toolkit->trimWidths[TRIM_TITLE_BORDER] = 5;
	toolkit->trimHeights[TRIM_TITLE_BORDER] = 28;
	toolkit->trimWidths[TRIM_TITLE_RESIZE] = 6;
	toolkit->trimHeights[TRIM_TITLE_RESIZE] = 29;
	toolkit->trimWidths[TRIM_TITLE] = 0;
	toolkit->trimHeights[TRIM_TITLE] = 23;
}
void _w_toolkit_init_widget(_w_toolkit *toolkit) {
	toolkit->shellHandle = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	toolkit->tooltipShellHandle = gtk_window_new(GTK_WINDOW_POPUP);
	toolkit->fixedHandle = gtk_fixed_new();
	toolkit->buttonHandle = gtk_button_new();
#if GTK3
	//toolkit->arrowHandle = gtk_image_new_from_icon_name (GTK_NAMED_ICON_GO_DOWN, GTK_ICON_SIZE_MENU);
	toolkit->arrowHandle = gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_NONE);
#else //GTK2
		toolkit->arrowHandle = gtk_arrow_new(GTK_ARROW_DOWN, GTK_SHADOW_NONE);
#endif
	toolkit->checkButtonHandle = gtk_check_button_new();
	toolkit->frameHandle = gtk_check_button_new();
	toolkit->entryHandle = gtk_entry_new();
	toolkit->radioButtonHandle = gtk_radio_button_new(0);
	toolkit->notebookHandle = gtk_notebook_new();
	toolkit->progressHandle = gtk_progress_bar_new();
	toolkit->toolbarHandle = gtk_toolbar_new();
	toolkit->treeHandle = gtk_tree_view_new_with_model(0);
#if GTK3
	toolkit->separatorHandle = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
#else
		toolkit->separatorHandle = gtk_vseparator_new();
#endif
	toolkit->labelHandle = gtk_label_new(NULL);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->labelHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->frameHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->entryHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->separatorHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->arrowHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->toolbarHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->progressHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->checkButtonHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->radioButtonHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->buttonHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle), toolkit->treeHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->fixedHandle),
			toolkit->notebookHandle);
	gtk_container_add(GTK_CONTAINER(toolkit->shellHandle),
			toolkit->fixedHandle);
	gtk_widget_realize(toolkit->separatorHandle);
	gtk_widget_realize(toolkit->labelHandle);
	gtk_widget_realize(toolkit->frameHandle);
	gtk_widget_realize(toolkit->entryHandle);
	gtk_widget_realize(toolkit->arrowHandle);
	gtk_widget_realize(toolkit->buttonHandle);
	gtk_widget_realize(toolkit->treeHandle);
	gtk_widget_realize(toolkit->notebookHandle);
	gtk_widget_realize(toolkit->checkButtonHandle);
	gtk_widget_realize(toolkit->radioButtonHandle);
	gtk_widget_realize(toolkit->progressHandle);
	gtk_widget_realize(toolkit->toolbarHandle);
	gtk_widget_realize(toolkit->shellHandle);
#if GTK3
	gtk_widget_set_name(toolkit->tooltipShellHandle, "gtk-tooltip");
#else
	gtk_widget_set_name(toolkit->tooltipShellHandle,"gtk-tooltips");
#endif
	gtk_widget_realize(toolkit->tooltipShellHandle);
}
void _w_toolkit_class_init(_w_toolkit *toolkit) {
	struct _w_toolkit_class *clazz = &toolkit->class_toolkit;
	((w_disposable*) toolkit)->clazz = (struct w_disposable_class*) clazz;
	clazz->disposable.dispose = _w_toolkit_dispose;
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
	clazz->malloc = _w_toolkit_malloc;
	clazz->realloc = _w_toolkit_realloc;
	clazz->free = _w_toolkit_free;

	/*
	 *
	 */
	clazz->messagebox_open = _w_toolkit_messagebox_open;
	clazz->open_clipboard = _w_toolkit_open_clipboard;
}
typedef void (*__w_widget_init_class)(struct _w_widget_class *clazz);

#define _W_TOOLKIT_CLASS_INIT(x,X) ((struct _w_widget_class *)&toolkit->class_##x)->init_class =(__w_widget_init_class) _w_##x##_class_init; \
	((struct _w_widget_class *)&toolkit->class_##x)->reserved[0] = &toolkit->class_##x##_reserved;\
	toolkit->classes[_W_CLASS_##X] =(struct _w_widget_class *) &toolkit->class_##x
#define _W_TOOLKIT_CLASS_ITEM_INIT(x,X) _W_TOOLKIT_CLASS_INIT(x,X);\
		toolkit->class_##x.class_##x##item = &toolkit->class_##x##item
void _w_toolkit_subclass_init(_w_toolkit *toolkit) {
	/*
	 * shell
	 */
	W_WIDGET_CLASS(&toolkit->class_shell)->init_class = (__w_widget_init_class) _w_shell_class_init;
	W_WIDGET_CLASS(&toolkit->class_shell)->reserved[0] = &toolkit->class_shell_reserved;
	toolkit->classes[_W_CLASS_SHELL] =W_WIDGET_CLASS( &toolkit->class_shell);
	/*
	 * canvas
	 */
	W_WIDGET_CLASS(&toolkit->class_canvas)->init_class = (__w_widget_init_class) _w_canvas_class_init;
	W_WIDGET_CLASS(&toolkit->class_canvas)->reserved[0] = &toolkit->class_canvas_reserved;
	toolkit->classes[_W_CLASS_CANVAS] =W_WIDGET_CLASS( &toolkit->class_canvas);
	/*
	 * composite
	 */
	W_WIDGET_CLASS(&toolkit->class_composite)->init_class = (__w_widget_init_class) _w_composite_class_init;
	W_WIDGET_CLASS(&toolkit->class_composite)->reserved[0] = &toolkit->class_composite_reserved;
	toolkit->classes[_W_CLASS_COMPOSITE] =W_WIDGET_CLASS( &toolkit->class_composite);
	/*
	 * dragsource
	 */
	W_WIDGET_CLASS(&toolkit->class_dragsource)->init_class = (__w_widget_init_class) _w_dragsource_class_init;
	toolkit->classes[_W_CLASS_DRAGSOURCE] =W_WIDGET_CLASS( &toolkit->class_dragsource);
	/*
	 * droptarget
	 */
	W_WIDGET_CLASS(&toolkit->class_droptarget)->init_class = (__w_widget_init_class) _w_droptarget_class_init;
	toolkit->classes[_W_CLASS_DROPTARGET] =W_WIDGET_CLASS( &toolkit->class_droptarget);

	/*
	 *
	 */
	_W_TOOLKIT_CLASS_INIT(sash, SASH);
	_W_TOOLKIT_CLASS_ITEM_INIT(tree, TREE);
	toolkit->class_tree.class_treecolumn = &toolkit->class_treecolumn;
	_W_TOOLKIT_CLASS_INIT(button, BUTTON);
	_W_TOOLKIT_CLASS_INIT(glcanvas, GLCANVAS);
	_W_TOOLKIT_CLASS_ITEM_INIT(menu, MENU);
	_W_TOOLKIT_CLASS_INIT(browser, BROWSER);
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
	_W_TOOLKIT_CLASS_INIT(ccanvas, CCANVAS); //
}
#endif
