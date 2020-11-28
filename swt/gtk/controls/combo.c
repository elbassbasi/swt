/*
 * combo.c
 *
 *  Created on: 11 oct. 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __linux
#include "combo.h"
#include "../widgets/toolkit.h"
wresult _w_combo_insert(w_combo *combo, w_comboitem *item, const char *string,
		int index) {
	if (string == 0)
		return W_ERROR_NULL_ARGUMENT;
	GtkTreeIter iter;
	/*if (!(0 <= index && index <= items.length)) {
	 error (SWT.ERROR_INVALID_RANGE);
	 }*/
#if GTK3
	//gtk_combo_box_text_insert(_W_WIDGET(combo)->handle, index, NULL, string);
	GtkTreeModel *model = gtk_combo_box_get_model(
			GTK_COMBO_BOX(_W_WIDGET(combo)->handle));
	gtk_list_store_insert(GTK_LIST_STORE(model), &iter, index);
	gtk_list_store_set(GTK_LIST_STORE(model), &iter, 0, string, 1, 0, -1);
#else
		gtk_combo_box_insert_text (handle, index, buffer);
#endif
	/*if ((style & SWT.RIGHT_TO_LEFT) != 0 && popupHandle != 0) {
	 OS.gtk_container_forall (popupHandle, display.setDirectionProc, OS.GTK_TEXT_DIR_RTL);
	 }*/
	return W_TRUE;
}
wresult _w_combo_clear_selection(w_combo *combo) {
}
wresult _w_combo_copy(w_combo *combo) {
}
wresult _w_combo_cut(w_combo *combo) {
}
wresult _w_combo_deselect(w_combo *combo, int index) {
}
wresult _w_combo_deselect_all(w_combo *combo) {
}
wresult _w_combo_get_item(w_combo *combo, int index, w_comboitem *item) {
}
int _w_combo_get_item_count(w_combo *combo) {
}
int _w_combo_get_item_height(w_combo *combo) {
}
void _w_combo_get_items(w_combo *combo, w_iterator *items) {
}
wbool _w_combo_get_list_visible(w_combo *combo) {
}
wresult _w_combo_set_list_visible(w_combo *combo, wbool visible) {
}
wbool _w_combo_get_selection(w_combo *combo, w_point *pt) {
}
int _w_combo_get_selection_index(w_combo *combo) {
	return gtk_combo_box_get_active(GTK_COMBO_BOX(_W_WIDGET(combo)->handle));
}
wresult _w_combo_get_text(w_combo *combo, w_alloc text,void* user_data) {
	GtkWidget *entryHandle = gtk_bin_get_child(
			GTK_BIN(_W_WIDGET(combo)->handle));
	const gchar *str;
	if (GTK_IS_ENTRY(entryHandle)) {
		str = gtk_entry_get_text(GTK_ENTRY(entryHandle));
		return _gtk_alloc_set_text(text,user_data, str, -1,W_ENCODING_UTF8);
	} else {
		GtkTreeModel *model = gtk_combo_box_get_model(
				GTK_COMBO_BOX(_W_WIDGET(combo)->handle));
		GtkTreeIter iter;
		GValue value;
		if (gtk_combo_box_get_active_iter(
				GTK_COMBO_BOX(_W_WIDGET(combo)->handle), &iter)) {
			gtk_tree_model_get(model, &iter, 0, &str, -1);
		}else{
			str = 0;
		}
		return _gtk_alloc_set_text(text,user_data, str, -1,W_ENCODING_UTF8);
	}
}
int _w_combo_get_text_height(w_combo *combo) {
}
int _w_combo_get_text_limit(w_combo *combo) {
}
int _w_combo_get_visible_item_count(w_combo *combo) {
}
int _w_combo_indexof(w_combo *combo, const char *string, int start) {
}
wresult _w_combo_paste(w_combo *combo) {
}
wresult _w_combo_remove_index(w_combo *combo, int index) {
}
wresult _w_combo_remove_range(w_combo *combo, int start, int end) {
}
wresult _w_combo_remove_string(w_combo *combo, const char *string) {
}
wresult _w_combo_remove_all(w_combo *combo) {
}
wresult _w_combo_select(w_combo *combo, int index) {
}
wresult _w_combo_set_item(w_combo *combo, int index, const char *string) {
}
wresult _w_combo_set_selection(w_combo *combo, w_point *selection) {
}
wresult _w_combo_set_text(w_combo *combo, const char *string) {
}
wresult _w_combo_set_text_limit(w_combo *combo, int limit) {
}
wresult _w_combo_set_visible_item_count(w_combo *combo, int count) {
}
int _w_combo_check_style(int style) {
	/*
	 * Feature in Windows.  It is not possible to create
	 * a combo box that has a border using Windows style
	 * bits.  All combo boxes draw their own border and
	 * do not use the standard Windows border styles.
	 * Therefore, no matter what style bits are specified,
	 * clear the BORDER bits so that the SWT style will
	 * match the Windows widget.
	 *
	 * The Windows behavior is currently implemented on
	 * all platforms.
	 */
	style &= ~W_BORDER;

	/*
	 * Even though it is legal to create this widget
	 * with scroll bars, they serve no useful purpose
	 * because they do not automatically scroll the
	 * widget's client area.  The fix is to clear
	 * the SWT style.
	 */
	style &= ~(W_HSCROLL | W_VSCROLL);
	style = _w_widget_check_bits(style, W_DROP_DOWN, W_SIMPLE, 0, 0, 0, 0);
	if ((style & W_SIMPLE) != 0)
		return style & ~W_READ_ONLY;
	return style;
}
wresult _w_combo_create(w_widget *widget, w_widget *parent, int style,
		w_widget_post_event_proc post_event) {
	if (parent == 0)
		return W_ERROR_INVALID_ARGUMENT;
	GtkWidget *fixedHandle, *cellHandle = 0, *entryHandle = 0;
	GtkCellRenderer *textRenderer = 0;
	GtkTreeModel *model = 0;
	_W_WIDGET(widget)->state |= STATE_HANDLE | STATE_MENU;
	_W_WIDGET(widget)->style = _w_combo_check_style(style);
	fixedHandle = _w_fixed_new();
	if (fixedHandle == 0)
		goto _err;
	gtk_widget_set_has_window(fixedHandle, TRUE);
	//GList *oldList = gtk_window_list_toplevels();
	model = GTK_TREE_MODEL(
			gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_POINTER));
	if (model == 0)
		goto _err;
	if ((_W_WIDGET(widget)->style & W_READ_ONLY) != 0) {
#if GTK3
		_W_WIDGET(widget)->handle = gtk_combo_box_new_with_model(
				GTK_TREE_MODEL(model));
#else
		_W_WIDGET(widget)->handle = gtk_combo_box_new_text ();
#endif
		if (_W_WIDGET(widget)->handle == 0)
			goto _err;
		cellHandle = gtk_bin_get_child(GTK_BIN(_W_WIDGET(widget)->handle));
		if (cellHandle == 0)
			goto _err;
		// Setting wrap width has the side effect of removing the whitespace on top in popup bug#438992
		gtk_combo_box_set_wrap_width(GTK_COMBO_BOX(_W_WIDGET(widget)->handle),
				1);
	} else {
#if GTK3
		_W_WIDGET(widget)->handle = gtk_combo_box_new_with_model_and_entry(
				GTK_TREE_MODEL(model));
#else
			handle = gtk_combo_box_entry_new_text ();
#endif
		if (_W_WIDGET(widget)->handle == 0)
			goto _err;
		gtk_combo_box_set_entry_text_column(
				GTK_COMBO_BOX(_W_WIDGET(widget)->handle), 0);
		entryHandle = gtk_bin_get_child(GTK_BIN(_W_WIDGET(widget)->handle));
		if (entryHandle == 0)
			goto _err;
#if GTK3
		//imContext = imContextLast();
#endif
	}
	//popupHandle = findPopupHandle(oldList);
	gtk_container_add(GTK_CONTAINER(fixedHandle), _W_WIDGET(widget)->handle);
	textRenderer = gtk_cell_renderer_text_new();
	if (textRenderer == 0)
		goto _err;
	/*
	 * Feature in GTK. In order to make a read only combo box the same
	 * height as an editable combo box the ypad must be set to 0. In
	 * versions 2.4.x of GTK, a pad of 0 will clip some letters. The
	 * fix is to set the pad to 1.
	 */
	int pad = 0;
	g_object_set(G_OBJECT(textRenderer), "ypad", pad, NULL);
	/*
	 * Feature in GTK.  In version 2.4.9 of GTK, a warning is issued
	 * when a call to gtk_cell_layout_clear() is made. The fix is to hide
	 * the warning.
	 */
	//boolean warnings = display.getWarnings();
	//display.setWarnings(false);
	gtk_cell_layout_clear(GTK_CELL_LAYOUT(_W_WIDGET(widget)->handle));
	//display.setWarnings(warnings);
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(_W_WIDGET(widget)->handle),
			textRenderer, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(_W_WIDGET(widget)->handle),
			textRenderer, "text", 0, NULL);
	/*
	 * Feature in GTK. Toggle button creation differs between GTK versions. The
	 * fix is to call size_request() to force the creation of the button
	 * for those versions of GTK that defer the creation.
	 */
	/*menuHandle = findMenuHandle();
	 if (menuHandle != 0)
	 g_object_ref(menuHandle);
	 buttonHandle = findButtonHandle();
	 if (buttonHandle != 0)
	 g_object_ref(buttonHandle);*/
	/*
	 * Feature in GTK. By default, read only combo boxes
	 * process the RETURN key rather than allowing the
	 * default button to process the key. The fix is to
	 * clear the GTK_RECEIVES_DEFAULT flag.
	 */
	/*if ((_W_WIDGET(widget)->style & W_READ_ONLY) != 0 && buttonHandle != 0) {
	 gtk_widget_set_receives_default(buttonHandle, false);
	 }*/
	// In GTK 3 font description is inherited from parent widget which is not how SWT has always worked,
	// reset to default font to get the usual behavior
#if GTK3
	//setFontDescription(defaultFont().handle);
#endif
	_W_WIDGET(widget)->post_event = post_event;
	_W_CONTROL(widget)->parent =W_COMPOSITE(parent);
	g_object_set_qdata(G_OBJECT(_W_WIDGET(widget)->handle),
			gtk_toolkit->quark[0], widget);
	g_object_set_qdata(G_OBJECT(fixedHandle), gtk_toolkit->quark[0], widget);
	_w_composite_add_child(W_COMPOSITE(parent), fixedHandle);
	gtk_widget_show_all(fixedHandle);
	return W_TRUE;
	_err: return W_ERROR_NO_HANDLES;
}
int _w_combo_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
#if GTK3
	_w_control_compute_native_size(widget, _W_WIDGET(widget)->handle, e,
			reserved);
	return TRUE;
#else
#endif
}
void _w_combo_class_init(struct _w_combo_class *clazz) {
	_w_composite_class_init(W_COMPOSITE_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_COMBO;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_combo_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_combo);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_combo);
	/*
	 * public function
	 */
	clazz->insert = _w_combo_insert;
	clazz->clear_selection = _w_combo_clear_selection;
	clazz->copy = _w_combo_copy;
	clazz->cut = _w_combo_cut;
	clazz->deselect = _w_combo_deselect;
	clazz->deselect_all = _w_combo_deselect_all;
	clazz->get_item = _w_combo_get_item;
	clazz->get_item_count = _w_combo_get_item_count;
	clazz->get_item_height = _w_combo_get_item_height;
	clazz->get_items = _w_combo_get_items;
	clazz->get_list_visible = _w_combo_get_list_visible;
	clazz->set_list_visible = _w_combo_set_list_visible;
	clazz->get_selection = _w_combo_get_selection;
	clazz->get_selection_index = _w_combo_get_selection_index;
	clazz->get_text = _w_combo_get_text;
	clazz->get_text_height = _w_combo_get_text_height;
	clazz->get_text_limit = _w_combo_get_text_limit;
	clazz->get_visible_item_count = _w_combo_get_visible_item_count;
	clazz->indexof = _w_combo_indexof;
	clazz->paste = _w_combo_paste;
	clazz->remove_index = _w_combo_remove_index;
	clazz->remove_range = _w_combo_remove_range;
	clazz->remove_string = _w_combo_remove_string;
	clazz->remove_all = _w_combo_remove_all;
	clazz->select = _w_combo_select;
	clazz->set_item = _w_combo_set_item;
	clazz->set_selection = _w_combo_set_selection;
	clazz->set_text = _w_combo_set_text;
	clazz->set_text_limit = _w_combo_set_text_limit;
	clazz->set_visible_item_count = _w_combo_set_visible_item_count;
	/*
	 * overriding function
	 */
	W_WIDGET_CLASS(clazz)->create = _w_combo_create;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	_W_WIDGET_RESERVED(reserved)->topHandle = _w_widget_hp;
	reserved->widget.compute_size = _w_combo_compute_size;
	reserved->fixedHandle = _w_widget_hp;
}
#endif
