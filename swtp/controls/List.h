/*
 * List.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_LIST_H_
#define SWTP_CONTROLS_LIST_H_
#include "../widgets/Scrollable.h"
class WListItem: public WItem {
public:
private:
};
/**
 * Instances of this class represent a selectable user interface
 * object that displays a list of strings and issues notification
 * when a string is selected.  A list may be single or multi select.
 * <p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SINGLE, MULTI</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection, DefaultSelection</dd>
 * </dl>
 * <p>
 * Note: Only one of SINGLE and MULTI may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WList: public WScrollable {
public:
	WList() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see SWT#SINGLE
	 * @see SWT#MULTI
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WList(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * and a style value describing its behavior and appearance.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 *
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 *
	 * @see SWT#SINGLE
	 * @see SWT#MULTI
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WComposite *parent, wuint64 style) {
		return Create(0, parent, style);
	}
	bool Create(WToolkit *toolkit, WComposite *parent, wuint64 style) {
		return Create(parent, style, toolkit) > 0;
	}
	WResult Create(WComposite *parent, wuint64 style,
				WToolkit *toolkit);
	/**
	 * Adds the argument to the end of the receiver's list.
	 *
	 * @param string the new item
	 *
	 * @see #add(String,int)
	 */
	bool Add(const char *string) {
		return Add(string, -1);
	}
	/**
	 * Adds the argument to the receiver's list at the given
	 * zero-relative index.
	 * <p>
	 * Note: To add an item at the end of the list, use the
	 * result of calling <code>getItemCount()</code> as the
	 * index or use <code>add(String)</code>.
	 * </p>
	 *
	 * @param string the new item
	 * @param index the index for the item
	 *
	 * @see #add(String)
	 */
	bool Add(const char *string, int index) {
		return w_list_insert(W_LIST(this), 0, string, index) > 0;
	}
	/**
	 * Deselects the items at the given zero-relative indices in the receiver.
	 * If the item at the given zero-relative index in the receiver
	 * is selected, it is deselected.  If the item at the index
	 * was not selected, it remains deselected. Indices that are out
	 * of range and duplicate indices are ignored.
	 *
	 * @param indices the array of indices for the items to deselect
	 */
	void Deselect(int *indices, size_t length) {
		w_list_deselect_indices(W_LIST(this), indices, length);
	}
	/**
	 * Deselects the item at the given zero-relative index in the receiver.
	 * If the item at the index was already deselected, it remains
	 * deselected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to deselect
	 */
	void Deselect(int index) {
		w_list_deselect_index(W_LIST(this), index);
	}
	/**
	 * Deselects the items at the given zero-relative indices in the receiver.
	 * If the item at the given zero-relative index in the receiver
	 * is selected, it is deselected.  If the item at the index
	 * was not selected, it remains deselected.  The range of the
	 * indices is inclusive. Indices that are out of range are ignored.
	 *
	 * @param start the start index of the items to deselect
	 * @param end the end index of the items to deselect
	 */
	void Deselect(int start, int end) {
		w_list_deselect_range(W_LIST(this), start, end);
	}
	/**
	 * Deselects all selected items in the receiver.
	 */
	void DeselectAll() {
		w_list_deselect_all(W_LIST(this));
	}
	/**
	 * Returns the zero-relative index of the item which currently
	 * has the focus in the receiver, or -1 if no item has focus.
	 *
	 * @return the index of the selected item
	 */
	int GetFocusIndex() {
		return w_list_get_focus_index(W_LIST(this));
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WListItem& GetItem(int index, WListItem &item) {
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WListItem& GetItem(int index, const WListItem &item = WListItem()) {
		return GetItem(index, (WListItem&) item);
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_list_get_item_count(W_LIST(this));
	}
	/**
	 * Returns the height of the area which would be used to
	 * display <em>one</em> of the items in the list.
	 *
	 * @return the height of one item
	 */
	int GetItemHeight() {
		return w_list_get_item_count(W_LIST(this));
	}
	/**
	 * Returns a (possibly empty) array of <code>String</code>s which
	 * are the items in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver's list
	 */
	void GetItems(WIterator<WListItem> &items) {
		w_list_get_items(W_LIST(this), (w_iterator*) &items);
	}
	/**
	 * Returns an array of <code>String</code>s that are currently
	 * selected in the receiver.  The order of the items is unspecified.
	 * An empty array indicates that no items are selected.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its selection, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 * @return an array representing the selection
	 */
	void GetSelection(WIterator<WListItem> &selections) {
		w_list_get_selection(W_LIST(this), (w_iterator*) &selections);
	}
	/**
	 * Returns the number of selected items contained in the receiver.
	 *
	 * @return the number of selected items
	 */
	int GetSelectionCount() {
		return w_list_get_selection_count(W_LIST(this));
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * selected in the receiver, or -1 if no item is selected.
	 *
	 * @return the index of the selected item or -1
	 */
	int GetSelectionIndex() {
		return w_list_get_selection_index(W_LIST(this));
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * at the top of the receiver. This index can change when items are
	 * scrolled or new items are added or removed.
	 *
	 * @return the index of the top item
	 */
	int GetTopIndex() {
		return w_list_get_top_index(W_LIST(this));
	}
	/**
	 * Gets the index of an item.
	 * <p>
	 * The list is searched starting at 0 until an
	 * item is found that is equal to the search item.
	 * If no item is found, -1 is returned.  Indexing
	 * is zero based.
	 *
	 * @param string the search item
	 * @return the index of the item
	 */
	int IndexOf(const char *string) {
		return IndexOf(string, 0);
	}
	/**
	 * Searches the receiver's list starting at the given,
	 * zero-relative index until an item is found that is equal
	 * to the argument, and returns the index of that item. If
	 * no item is found or the starting index is out of range,
	 * returns -1.
	 *
	 * @param string the search item
	 * @param start the zero-relative index at which to start the search
	 * @return the index of the item
	 */
	int IndexOf(const char *string, int start) {
		return w_list_index_of(W_LIST(this), string, start);
	}
	/**
	 * Returns <code>true</code> if the item is selected,
	 * and <code>false</code> otherwise.  Indices out of
	 * range are ignored.
	 *
	 * @param index the index of the item
	 * @return the selection state of the item at the index
	 */
	bool IsSelected(int index) {
		return w_list_is_selected(W_LIST(this), index);
	}
	/**
	 * Removes the items from the receiver at the given
	 * zero-relative indices.
	 *
	 * @param indices the array of indices of the items
	 */
	void Remove(int *indices, size_t length) {
		w_list_remove_indices(W_LIST(this), indices, length);
	}
	/**
	 * Removes the item from the receiver at the given
	 * zero-relative index.
	 *
	 * @param index the index for the item
	 */
	void Remove(int index) {
		w_list_remove_index(W_LIST(this), index);
	}
	/**
	 * Removes the items from the receiver which are
	 * between the given zero-relative start and end
	 * indices (inclusive).
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 */
	void Remove(int start, int end) {
		w_list_remove_range(W_LIST(this), start, end);
	}
	/**
	 * Searches the receiver's list starting at the first item
	 * until an item is found that is equal to the argument,
	 * and removes that item from the list.
	 *
	 * @param string the item to remove
	 */
	void Remove(const char *string) {
		w_list_remove_string(W_LIST(this), string);
	}
	/**
	 * Removes all of the items from the receiver.
	 * <p>
	 */
	void RemoveAll() {
		w_list_remove_all(W_LIST(this));
	}
	/**
	 * Selects the items at the given zero-relative indices in the receiver.
	 * The current selection is not cleared before the new items are selected.
	 * <p>
	 * If the item at a given index is not selected, it is selected.
	 * If the item at a given index was already selected, it remains selected.
	 * Indices that are out of range and duplicate indices are ignored.
	 * If the receiver is single-select and multiple indices are specified,
	 * then all indices are ignored.
	 *
	 * @param indices the array of indices for the items to select
	 *
	 * @see List#setSelection(int[])
	 */
	void Select(int *indices, size_t length) {
		w_list_select_indices(W_LIST(this), indices, length);
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver's
	 * list.  If the item at the index was already selected, it remains
	 * selected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to select
	 */
	void Select(int index) {
		w_list_select_index(W_LIST(this), index);
	}
	/**
	 * Selects the items in the range specified by the given zero-relative
	 * indices in the receiver. The range of indices is inclusive.
	 * The current selection is not cleared before the new items are selected.
	 * <p>
	 * If an item in the given range is not selected, it is selected.
	 * If an item in the given range was already selected, it remains selected.
	 * Indices that are out of range are ignored and no items will be selected
	 * if start is greater than end.
	 * If the receiver is single-select and there is more than one item in the
	 * given range, then all indices are ignored.
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 *
	 * @see List#setSelection(int,int)
	 */
	void Select(int start, int end) {
		w_list_select_range(W_LIST(this), start, end);
	}
	/**
	 * Selects all of the items in the receiver.
	 * <p>
	 * If the receiver is single-select, do nothing.
	 */
	void SelectAll() {
		w_list_select_all(W_LIST(this));
	}
	/**
	 * Sets the text of the item in the receiver's list at the given
	 * zero-relative index to the string argument.
	 *
	 * @param index the index for the item
	 * @param string the new text for the item
	 */
	void SetItem(int index, const char *string) {
		w_list_set_item(W_LIST(this), index, string);
	}
	/**
	 * Sets the receiver's items to be the given array of items.
	 *
	 * @param items the array of items
	 */
	void SetItems(const char **items, size_t length) {
		w_list_set_items(W_LIST(this), items, length);
	}
	/**
	 * Selects the items at the given zero-relative indices in the receiver.
	 * The current selection is cleared before the new items are selected.
	 * <p>
	 * Indices that are out of range and duplicate indices are ignored.
	 * If the receiver is single-select and multiple indices are specified,
	 * then all indices are ignored.
	 *
	 * @param indices the indices of the items to select
	 *
	 * @see List#deselectAll()
	 * @see List#select(int[])
	 */
	void SetSelection(int *indices, size_t length) {
		w_list_set_selection_indices(W_LIST(this), indices, length);
	}
	/**
	 * Sets the receiver's selection to be the given array of items.
	 * The current selection is cleared before the new items are selected.
	 * <p>
	 * Items that are not in the receiver are ignored.
	 * If the receiver is single-select and multiple items are specified,
	 * then all items are ignored.
	 *
	 * @param items the array of items
	 *
	 * @see List#deselectAll()
	 * @see List#select(int[])
	 * @see List#setSelection(int[])
	 */
	void SetSelection(const char **items, size_t length) {
		w_list_set_selection_strings(W_LIST(this), items, length);
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver.
	 * If the item at the index was already selected, it remains selected.
	 * The current selection is first cleared, then the new item is selected.
	 * Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to select
	 *
	 * @see List#deselectAll()
	 * @see List#select(int)
	 */
	void SetSelection(int index) {
		w_list_set_selection_index(W_LIST(this), index);
	}
	/**
	 * Selects the items in the range specified by the given zero-relative
	 * indices in the receiver. The range of indices is inclusive.
	 * The current selection is cleared before the new items are selected.
	 * <p>
	 * Indices that are out of range are ignored and no items will be selected
	 * if start is greater than end.
	 * If the receiver is single-select and there is more than one item in the
	 * given range, then all indices are ignored.
	 *
	 * @param start the start index of the items to select
	 * @param end the end index of the items to select
	 *
	 * @see List#deselectAll()
	 * @see List#select(int,int)
	 */
	void SetSelection(int start, int end) {
		w_list_set_selection_range(W_LIST(this), start, end);
	}
	/**
	 * Sets the zero-relative index of the item which is currently
	 * at the top of the receiver. This index can change when items
	 * are scrolled or new items are added and removed.
	 *
	 * @param index the index of the top item
	 */
	void SetTopIndex(int index) {
		w_list_set_top_index(W_LIST(this), index);
	}
	/**
	 * Shows the selection.  If the selection is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled until
	 * the selection is visible.
	 */
	void ShowSelection() {
		w_list_show_selection(W_LIST(this));
	}
private:
	void *handles[(sizeof(w_list) - sizeof(w_scrollable)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_LIST_H_ */
