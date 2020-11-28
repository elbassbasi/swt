/*
 * toolkit.c
 *
 *  Created on: 24 mai 2020
 *      Author: Azeddine El Bassbasi
 */
#include "toolkit.h"
#include "shell.h"
#include "../controls/tabfolder.h"
#include "../controls/tree.h"

c_toolkit *ctoolkit = 0;
w_toolkit* w_toolkit_get_custom() {
	if (ctoolkit == 0) {
		ctoolkit = malloc(sizeof(c_toolkit));
		if (ctoolkit == 0)
			return 0;
		memset(ctoolkit, 0, sizeof(c_toolkit));
		w_toolkit_custom_init((struct c_toolkit*) ctoolkit);
		w_app_dispose_registre((w_disposable*) ctoolkit);
	}
	return W_TOOLKIT(ctoolkit);
}
themes_creates_function w_toolkit_custom_find_theme_0(const char *name);
themes_creates_function w_toolkit_custom_find_theme(const char *name) {
	return w_toolkit_custom_find_theme_0(name);
}
w_theme* w_toolkit_custom_create_theme(const char *name) {
	themes_creates_function fn = w_toolkit_custom_find_theme(name);
	if (fn != 0)
		return fn();
	else
		return 0;
}
void w_toolkit_custom_init(struct c_toolkit *toolkit) {
	toolkit->toolkit.disposable.next = (w_disposable*) w_toolkit_get_platform();
	toolkit->toolkit.disposable.clazz =
			(struct w_disposable_class*) &toolkit->class_toolkit;
	toolkit->system_toolkit = W_TOOLKIT(toolkit->toolkit.disposable.next);
	w_toolkit_custom_init_clazz(toolkit);
	toolkit->theme = w_toolkit_get_theme(w_toolkit_get_platform());
}
void w_toolkit_custom_dispose(w_disposable *toolkit) {
	free(toolkit);
}
wresult w_toolkit_custom_check_widget(w_toolkit *toolkit, w_widget *widget) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->check_widget(systoolkit,widget);
}
wresult w_toolkit_custom_check_widgetdata(w_toolkit *toolkit,
		w_widgetdata *widgetdata) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->check_widgetdata(systoolkit,widgetdata);
}
void* w_toolkit_custom_malloc(w_toolkit *toolkit, size_t size) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->malloc(systoolkit,size);
}
void w_toolkit_custom_free(w_toolkit *toolkit, void *ptr) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->free(systoolkit,ptr);
}
struct _w_widget_class* w_toolkit_custom_get_class(w_toolkit *toolkit,
		wushort clazz_id) {
	if (clazz_id >= _W_CLASS_LAST)
		return 0;
	struct c_toolkit *ctoolkit = (struct c_toolkit*) toolkit;
	return ctoolkit->classes[clazz_id];
}
w_theme* w_toolkit_custom_get_theme(w_toolkit *toolkit) {
	struct c_toolkit *ctoolkit = (struct c_toolkit*) toolkit;
	if (ctoolkit->theme != 0) {
		return ctoolkit->theme;
	} else {
		w_toolkit *systoolkit = ctoolkit->system_toolkit;
		return W_TOOLKIT_GET_CLASS(systoolkit)->get_theme(systoolkit);
	}
}
wresult w_toolkit_custom_async_exec(w_toolkit *toolkit,
		w_thread_start function, void *args) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->async_exec(systoolkit,function,args);
}
wresult w_toolkit_custom_beep(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->beep(systoolkit);
}
wresult w_toolkit_custom_close(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->close(systoolkit);
}
w_shell* w_toolkit_custom_get_active_shell(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_active_shell(systoolkit);
}
w_menu* w_toolkit_custom_get_menubar(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_menubar(systoolkit);
}
wresult w_toolkit_custom_get_bounds(w_toolkit *toolkit, w_rect *bounds) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_bounds(systoolkit,bounds);
}
wresult w_toolkit_custom_get_client_area(w_toolkit *toolkit,
		w_rect *clientArea) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_client_area(systoolkit,clientArea);
}
w_control* w_toolkit_custom_get_cursor_control(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_cursor_control(systoolkit);
}
wresult w_toolkit_custom_get_cursor_location(w_toolkit *toolkit,
		w_point *location) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_cursor_location(systoolkit,location);
}
size_t w_toolkit_custom_get_cursor_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_cursor_sizes(systoolkit,sizes,length);
}
int w_toolkit_custom_get_dismissal_alignment(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_dismissal_alignment(systoolkit);
}
int w_toolkit_custom_get_double_click_time(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_double_click_time(systoolkit);
}
w_control* w_toolkit_custom_get_focus_control(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_focus_control(systoolkit);
}
wresult w_toolkit_custom_get_high_contrast(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_high_contrast(systoolkit);
}
int w_toolkit_custom_get_icon_depth(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_icon_depth(systoolkit);
}
size_t w_toolkit_custom_get_icon_sizes(w_toolkit *toolkit, w_size *sizes,
		size_t length) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_icon_sizes(systoolkit,sizes,length);
}
void w_toolkit_custom_get_shells(w_toolkit *toolkit, w_iterator *shells) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_shells(systoolkit,shells);
}
w_color w_toolkit_custom_get_system_color(w_toolkit *toolkit, wuint id) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_system_color(systoolkit,id);
}
w_cursor* w_toolkit_custom_get_system_cursor(w_toolkit *toolkit, wuint id) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_system_cursor(systoolkit,id);
}
w_font* w_toolkit_custom_get_system_font(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_system_font(systoolkit);
}
wresult w_toolkit_custom_get_system_image(w_toolkit *toolkit, wuint id,
		w_image *image) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_system_image(systoolkit,id,image);
}
w_menu* w_toolkit_custom_get_system_menu(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_system_menu(systoolkit);
}
struct w_taskbar* w_toolkit_custom_get_system_taskbar(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_system_taskbar(systoolkit);
}
struct w_tray* w_toolkit_custom_get_system_tray(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_system_tray(systoolkit);
}
w_thread* w_toolkit_custom_get_thread(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_thread(systoolkit);
}
wuint64 w_toolkit_custom_get_thread_id(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_thread_id(systoolkit);
}
wresult w_toolkit_custom_get_touch_enabled(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->get_touch_enabled(systoolkit);
}
wresult w_toolkit_custom_map_0(w_toolkit *toolkit, w_control *from,
		w_control *to, w_point *result, w_point *point) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->map_0(systoolkit,from,to,result,point);
}
wresult w_toolkit_custom_map_1(w_toolkit *toolkit, w_control *from,
		w_control *to, w_rect *result, w_rect *rectangle) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->map_1(systoolkit,from,to,result,rectangle);
}
wresult w_toolkit_custom_post(w_toolkit *toolkit, w_event *event) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->post(systoolkit,event);
}
wresult w_toolkit_custom_post_quit(w_toolkit *toolkit, int quit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->post_quit(systoolkit,quit);
}
wresult w_toolkit_custom_read_and_dispatch(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->read_and_dispatch(systoolkit);
}
wresult w_toolkit_custom_set_cursor_location(w_toolkit *toolkit,
		w_point *point) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->set_cursor_location(systoolkit,point);
}
wresult w_toolkit_custom_set_theme(w_toolkit *toolkit, w_theme *theme) {
	struct c_toolkit *ctoolkit = (struct c_toolkit*) toolkit;
	ctoolkit->theme = theme;
	return W_TRUE;
}
int w_toolkit_custom_run(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->run(systoolkit);
}
wresult w_toolkit_custom_sleep(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->sleep(systoolkit);
}
wresult w_toolkit_custom_sync_exec(w_toolkit *toolkit, w_thread_start function,
		void *args) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->sync_exec(systoolkit,function,args);
}
wresult w_toolkit_custom_timer_exec(w_toolkit *toolkit, wuint milliseconds,
		w_thread_start function, void *args) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->timer_exec(systoolkit,milliseconds,function,args);
}
wresult w_toolkit_custom_update(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->update(systoolkit);
}
wresult w_toolkit_custom_wake(w_toolkit *toolkit) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->wake(systoolkit);
}
/**
 *
 */
int w_toolkit_custom_register_type(w_toolkit *toolkit, const char *formatName) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->register_type(systoolkit,formatName);
}
wresult w_toolkit_custom_open_clipboard(w_toolkit *toolkit,
		w_clipboard *clipboard,int clipboards) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->open_clipboard(systoolkit,clipboard,clipboards);
}
/*
 * dialogs
 */
int w_toolkit_custom_messagebox_open(w_toolkit *toolkit,
		w_messagebox *messagebox) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->messagebox_open(systoolkit,messagebox);
}
wresult w_toolkit_custom_dialog_color_open(w_toolkit *toolkit,
		w_dialog_color *dialog_color) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->dialog_color_open(systoolkit,dialog_color);
}
wresult w_toolkit_custom_dialog_font_open(w_toolkit *toolkit,
		w_dialog_font *dialog_font) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->dialog_font_open(systoolkit,dialog_font);
}
wresult w_toolkit_custom_dialog_directory_open(w_toolkit *toolkit,
		w_dialog_directory *dialog_directory) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->dialog_directory_open(systoolkit,dialog_directory);
}
wresult w_toolkit_custom_dialog_file_open(w_toolkit *toolkit,
		w_dialog_file *dialog_file) {
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	return W_TOOLKIT_GET_CLASS(systoolkit)->dialog_file_open(systoolkit,dialog_file);
}
void wc_button_init_class(struct _w_widget_class *clazz);
void w_toolkit_custom_init_clazz(struct c_toolkit *toolkit) {
	struct _w_toolkit_class *clazz = &toolkit->class_toolkit;
	w_toolkit *systoolkit = ((struct c_toolkit*) toolkit)->system_toolkit;
	for (int i = 0; i < _W_CLASS_LAST; i++) {
		toolkit->classes[i] = W_TOOLKIT_GET_CLASS(systoolkit)->get_class(
				systoolkit, i);
	}
	clazz->disposable.dispose = w_toolkit_custom_dispose;
	clazz->check_widget = w_toolkit_custom_check_widget;
	clazz->check_widgetdata = w_toolkit_custom_check_widgetdata;
	clazz->malloc = w_toolkit_custom_malloc;
	clazz->free = w_toolkit_custom_free;
	clazz->get_class = w_toolkit_custom_get_class;
	clazz->get_theme = w_toolkit_custom_get_theme;
	clazz->async_exec = w_toolkit_custom_async_exec;
	clazz->beep = w_toolkit_custom_beep;
	clazz->close = w_toolkit_custom_close;
	clazz->get_active_shell = w_toolkit_custom_get_active_shell;
	clazz->get_menubar = w_toolkit_custom_get_menubar;
	clazz->get_bounds = w_toolkit_custom_get_bounds;
	clazz->get_client_area = w_toolkit_custom_get_client_area;
	clazz->get_cursor_control = w_toolkit_custom_get_cursor_control;
	clazz->get_cursor_location = w_toolkit_custom_get_cursor_location;
	clazz->get_cursor_sizes = w_toolkit_custom_get_cursor_sizes;
	clazz->get_dismissal_alignment = w_toolkit_custom_get_dismissal_alignment;
	clazz->get_double_click_time = w_toolkit_custom_get_double_click_time;
	clazz->get_focus_control = w_toolkit_custom_get_focus_control;
	clazz->get_high_contrast = w_toolkit_custom_get_high_contrast;
	clazz->get_icon_depth = w_toolkit_custom_get_icon_depth;
	clazz->get_icon_sizes = w_toolkit_custom_get_icon_sizes;
	clazz->get_shells = w_toolkit_custom_get_shells;
	clazz->get_system_color = w_toolkit_custom_get_system_color;
	clazz->get_system_cursor = w_toolkit_custom_get_system_cursor;
	clazz->get_system_font = w_toolkit_custom_get_system_font;
	clazz->get_system_image = w_toolkit_custom_get_system_image;
	clazz->get_system_menu = w_toolkit_custom_get_system_menu;
	clazz->get_system_taskbar = w_toolkit_custom_get_system_taskbar;
	clazz->get_system_tray = w_toolkit_custom_get_system_tray;
	clazz->get_thread = w_toolkit_custom_get_thread;
	clazz->get_thread_id = w_toolkit_custom_get_thread_id;
	clazz->get_touch_enabled = w_toolkit_custom_get_touch_enabled;
	clazz->map_0 = w_toolkit_custom_map_0;
	clazz->map_1 = w_toolkit_custom_map_1;
	clazz->post = w_toolkit_custom_post;
	clazz->post_quit = w_toolkit_custom_post_quit;
	clazz->read_and_dispatch = w_toolkit_custom_read_and_dispatch;
	clazz->set_cursor_location = w_toolkit_custom_set_cursor_location;
	clazz->set_theme = w_toolkit_custom_set_theme;
	clazz->run = w_toolkit_custom_run;
	clazz->sleep = w_toolkit_custom_sleep;
	clazz->sync_exec = w_toolkit_custom_sync_exec;
	clazz->timer_exec = w_toolkit_custom_timer_exec;
	clazz->update = w_toolkit_custom_update;
	clazz->wake = w_toolkit_custom_wake;
	/**
	 *
	 */
	clazz->register_type = w_toolkit_custom_register_type;
	clazz->open_clipboard = w_toolkit_custom_open_clipboard;
	/*
	 * dialogs
	 */
	clazz->messagebox_open = w_toolkit_custom_messagebox_open;
	clazz->dialog_color_open = w_toolkit_custom_dialog_color_open;
	clazz->dialog_font_open = w_toolkit_custom_dialog_font_open;
	clazz->dialog_directory_open = w_toolkit_custom_dialog_directory_open;
	clazz->dialog_file_open = w_toolkit_custom_dialog_file_open;
	/*
	 * shell
	 */
	toolkit->classes[_W_CLASS_SHELL] =
			(struct _w_widget_class*) &toolkit->class_shell;
	W_WIDGET_CLASS(&toolkit->class_shell)->init_class =
			(w_widget_init_class) wc_shell_init_class;
	/*
	 * composite
	 */
	toolkit->classes[_W_CLASS_COMPOSITE] =
			(struct _w_widget_class*) &toolkit->class_composite;
	W_WIDGET_CLASS(&toolkit->class_composite)->init_class =
			(w_widget_init_class) wc_composite_init_class;
	/*
	 * canvas
	 */
	toolkit->classes[_W_CLASS_CANVAS] =
			(struct _w_widget_class*) &toolkit->class_canvas;
	W_WIDGET_CLASS(&toolkit->class_canvas)->init_class =
			(w_widget_init_class) wc_canvas_init_class;
	/*
	 * button
	 */
	toolkit->classes[_W_CLASS_BUTTON] =
			(struct _w_widget_class*) &toolkit->class_button;
	W_WIDGET_CLASS(&toolkit->class_button)->init_class =
			(w_widget_init_class) wc_button_init_class;
	/*
	 * tabfolder
	 */
	toolkit->classes[_W_CLASS_TABFOLDER] =
			(struct _w_widget_class*) &toolkit->class_tabfolder;
	toolkit->class_tabfolder.class_tabfolderitem = &toolkit->class_tabitem;
	W_WIDGET_CLASS(&toolkit->class_tabfolder)->init_class =
			(w_widget_init_class) wc_tabfolder_init_class;
	/*
	 * tree
	 */
	/*toolkit->classes[_W_CLASS_TREE] =
	 (struct _w_widget_class*) &toolkit->class_tree;
	 toolkit->class_tree.class_treeitem = &toolkit->class_treeitem;
	 toolkit->class_tree.class_treecolumn = &toolkit->class_treecolumn;
	 W_WIDGET_CLASS(&toolkit->class_tree)->init_class = (w_widget_init_class)wc_tree_init_class;*/
}

