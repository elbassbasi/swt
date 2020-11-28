/*
 * Table.h
 *
 *  Created on: 7 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_TABLE_H_
#define SWTP_CONTROLS_TABLE_H_
#include "../widgets/Composite.h"
class WTable;
/**
 * Instances of this class represent a column in a table widget.
 * <p><dl>
 * <dt><b>Styles:</b></dt>
 * <dd>LEFT, RIGHT, CENTER</dd>
 * <dt><b>Events:</b></dt>
 * <dd> Move, Resize, Selection</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles LEFT, RIGHT and CENTER may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTableColumn: public WItem {
public:
	WTableColumn() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Table</code>) and a style value
	 * describing its behavior and appearance. The item is added
	 * to the end of the items maintained by its parent.
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
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#CENTER
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WTable *parent, const char *text) {
		return Create(parent, text, -1);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Table</code>), a style value
	 * describing its behavior and appearance, and the index
	 * at which to place it in the items maintained by its parent.
	 * <p>
	 * The style value is either one of the style constants defined in
	 * class <code>SWT</code> which is applicable to instances of this
	 * class, or must be built by <em>bitwise OR</em>'ing together
	 * (that is, using the <code>int</code> "|" operator) two or more
	 * of those <code>SWT</code> style constants. The class description
	 * lists the style constants that are applicable to the class.
	 * Style bits are also inherited from superclasses.
	 * </p>
	 * <p>
	 * Note that due to a restriction on some platforms, the first column
	 * is always left aligned.
	 * </p>
	 * @param parent a composite control which will be the parent of the new instance (cannot be null)
	 * @param style the style of control to construct
	 * @param index the zero-relative index to store the receiver in its parent
	 *
	 * @see SWT#LEFT
	 * @see SWT#RIGHT
	 * @see SWT#CENTER
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WTable *parent, const char *text, int index) {
		return w_table_insert_column(W_TABLE(parent), W_TABLECOLUMN(this), text,
				index) > 0;
	}
	/**
	 * Returns a value which describes the position of the
	 * text or image in the receiver. The value will be one of
	 * <code>LEFT</code>, <code>RIGHT</code> or <code>CENTER</code>.
	 *
	 * @return the alignment
	 */
	int GetAlignment() {
		return w_tablecolumn_get_alignment(W_TABLECOLUMN(this));
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Table</code>.
	 *
	 * @return the receiver's parent
	 */
	WTable* GetParent() {
		return (WTable*) WItem::GetParentWidget();
	}
	/**
	 * Gets the moveable attribute. A column that is
	 * not moveable cannot be reordered by the user
	 * by dragging the header but may be reordered
	 * by the programmer.
	 *
	 * @return the moveable attribute
	 *
	 * @see Table#getColumnOrder()
	 * @see Table#setColumnOrder(int[])
	 * @see TableColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	bool GetMoveable() {
		return w_tablecolumn_get_moveable(W_TABLECOLUMN(this));
	}
	/**
	 * Gets the resizable attribute. A column that is
	 * not resizable cannot be dragged by the user but
	 * may be resized by the programmer.
	 *
	 * @return the resizable attribute
	 */
	bool GetResizable() {
		return w_tablecolumn_get_resizable(W_TABLECOLUMN(this));
	}
	/**
	 * Returns the receiver's tool tip text, or null if it has
	 * not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WString GetToolTipText() {
		w_string_ref *ref = 0;
		w_tablecolumn_get_tooltip_text(W_TABLECOLUMN(this),w_alloc_string_ref, &ref);
		return ref;
	}
	/**
	 * Gets the width of the receiver.
	 *
	 * @return the width
	 */
	int GetWidth() {
		return w_tablecolumn_get_width(W_TABLECOLUMN(this));
	}
	/**
	 * Causes the receiver to be resized to its preferred size.
	 * For a composite, this involves computing the preferred size
	 * from its layout, if there is one.
	 */
	bool Pack() {
		return w_tablecolumn_pack(W_TABLECOLUMN(this)) > 0;
	}
	/**
	 * Controls how text and images will be displayed in the receiver.
	 * The argument should be one of <code>LEFT</code>, <code>RIGHT</code>
	 * or <code>CENTER</code>.
	 * <p>
	 * Note that due to a restriction on some platforms, the first column
	 * is always left aligned.
	 * </p>
	 * @param alignment the new alignment
	 */
	bool SetAlignment(int alignment) {
		return w_tablecolumn_set_alignment(W_TABLECOLUMN(this), alignment) > 0;
	}
	bool SetImage(int image) {
		return w_tablecolumn_set_image(W_TABLECOLUMN(this), image) > 0;
	}
	/**
	 * Sets the moveable attribute.  A column that is
	 * moveable can be reordered by the user by dragging
	 * the header. A column that is not moveable cannot be
	 * dragged by the user but may be reordered
	 * by the programmer.
	 *
	 * @param moveable the moveable attribute
	 *
	 * @see Table#setColumnOrder(int[])
	 * @see Table#getColumnOrder()
	 * @see TableColumn#getMoveable()
	 * @see SWT#Move
	 */
	bool SetMoveable(bool moveable) {
		return w_tablecolumn_set_moveable(W_TABLECOLUMN(this), moveable) > 0;
	}
	/**
	 * Sets the resizable attribute.  A column that is
	 * resizable can be resized by the user dragging the
	 * edge of the header.  A column that is not resizable
	 * cannot be dragged by the user but may be resized
	 * by the programmer.
	 *
	 * @param resizable the resize attribute
	 */
	bool SetResizable(bool resizable) {
		return w_tablecolumn_set_resizable(W_TABLECOLUMN(this), resizable) > 0;
	}
	/**
	 * Sets the receiver's tool tip text to the argument, which
	 * may be null indicating that the default tool tip for the
	 * control will be shown. For a control that has a default
	 * tool tip, such as the Tree control on Windows, setting
	 * the tool tip text to an empty string replaces the default,
	 * causing no tool tip text to be shown.
	 * <p>
	 * The mnemonic indicator (character '&amp;') is not displayed in a tool tip.
	 * To display a single '&amp;' in the tool tip, the character '&amp;' can be
	 * escaped by doubling it in the string.
	 * </p>
	 *
	 * @param string the new tool tip text (or null)
	 */
	bool SetToolTipText(const char *string) {
		return w_tablecolumn_set_tooltip_text(W_TABLECOLUMN(this), string) > 0;
	}
	/**
	 * Sets the width of the receiver.
	 *
	 * @param width the new width
	 */
	bool SetWidth(int width) {
		return w_tablecolumn_set_width(W_TABLECOLUMN(this), width) > 0;
	}
};
/**
 * Instances of this class represent a selectable user interface object
 * that represents an item in a table.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTableItem: public WItem {
public:
	WTableItem() {

	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Table</code>) and a style value
	 * describing its behavior and appearance. The item is added
	 * to the end of the items maintained by its parent.
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
	 * @see SWT
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WTable *parent, const char *string) {
		return Create(parent, string, -1);
	}
	/**
	 * Constructs a new instance of this class given its parent
	 * (which must be a <code>Table</code>), a style value
	 * describing its behavior and appearance, and the index
	 * at which to place it in the items maintained by its parent.
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
	 * @param index the zero-relative index to store the receiver in its parent
	 *
	 * @see SWT
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WTable *parent, const char *string, int index) {
		return w_table_insert_item(W_TABLE(parent), W_TABLEITEM(this), string,
				index) > 0;
	}
	/**
	 * Returns a rectangle describing the size and location of the receiver's
	 * text relative to its parent.
	 *
	 * @return the bounding rectangle of the receiver's text
	 */
	WRect& GetBounds(WRect &bounds) {
		w_tableitem_get_bounds(W_TABLEITEM(this), (w_rect*) &bounds);
		return bounds;
	}
	/**
	 * Returns a rectangle describing the size and location of the receiver's
	 * text relative to its parent.
	 *
	 * @return the bounding rectangle of the receiver's text
	 */
	WRect& GetBounds(const WRect &bounds = WRect()) {
		return GetBounds((WRect&) bounds);
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent at a column in the table.
	 *
	 * @param index the index that specifies the column
	 * @return the receiver's bounding column rectangle
	 */
	WRect& GetBounds(int index, WRect &bounds) {
		w_tableitem_get_bounds_index(W_TABLEITEM(this), index,
				(w_rect*) &bounds);
		return bounds;
	}
	/**
	 * Returns a rectangle describing the receiver's size and location
	 * relative to its parent at a column in the table.
	 *
	 * @param index the index that specifies the column
	 * @return the receiver's bounding column rectangle
	 */
	WRect& GetBounds(int index, const WRect &bounds = WRect()) {
		return GetBounds(index, (WRect&) bounds);
	}
	/**
	 * Returns <code>true</code> if the receiver is checked,
	 * and false otherwise.  When the parent does not have
	 * the <code>CHECK</code> style, return false.
	 *
	 * @return the checked state of the checkbox
	 */
	bool GetChecked() {
		return w_tableitem_get_checked(W_TABLEITEM(this));
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Table</code>.
	 *
	 * @return the receiver's parent
	 */
	WTable* GetParent() {
		return (WTable*) WItem::GetParentWidget();
	}
	bool SetImage(int image) {
		return w_tableitem_set_image(W_TABLEITEM(this), image) > 0;
	}

};
/**
 * Instances of this class implement a selectable user interface
 * object that displays a list of images and strings and issues
 * notification when selected.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>TableItem</code>.
 * </p><p>
 * Style <code>VIRTUAL</code> is used to create a <code>Table</code> whose
 * <code>TableItem</code>s are to be populated by the client on an on-demand basis
 * instead of up-front.  This can provide significant performance improvements for
 * tables that are very large or for which <code>TableItem</code> population is
 * expensive (for example, retrieving values from an external source).
 *
 *
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not normally make sense to add <code>Control</code> children to
 * it, or set a layout on it, unless implementing something like a cell
 * editor.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SINGLE, MULTI, CHECK, FULL_SELECTION, HIDE_SELECTION, VIRTUAL, NO_SCROLL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection, DefaultSelection, SetData, MeasureItem, EraseItem, PaintItem</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles SINGLE, and MULTI may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */

class SWTP_PUBLIC WTable: public WComposite {
public:
	WTable() {

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
	 * @see SWT#CHECK
	 * @see SWT#FULL_SELECTION
	 * @see SWT#HIDE_SELECTION
	 * @see SWT#VIRTUAL
	 * @see SWT#NO_SCROLL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WTable(WComposite *parent, wuint64 style) {
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
	 * @see SWT#CHECK
	 * @see SWT#FULL_SELECTION
	 * @see SWT#HIDE_SELECTION
	 * @see SWT#VIRTUAL
	 * @see SWT#NO_SCROLL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WComposite *parent, wuint64 style) {
		return Create(0, parent, style);
	}
	bool Create(WToolkit *toolkit, WComposite *parent, wuint64 style) {
		return Create(toolkit, parent, style) > 0;
	}
	WResult Create(WComposite *parent, wuint64 style,
				WToolkit *toolkit);
	WTableItem& AppendItem(WTableItem& item,const char* text){
		return InsertItem(item, text, -1);
	}
	WTableColumn& AppendColumn(WTableColumn& column,const char* text){
		return InsertColumn(column, text, -1);
	}
	/**
	 * Clears the item at the given zero-relative index in the receiver.
	 * The text, icon and other attributes of the item are set to the default
	 * value.  If the table was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param index the index of the item to clear
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool Clear(int index) {
		return w_table_clear_index(W_TABLE(this), index) > 0;
	}
	/**
	 * Removes the items from the receiver which are between the given
	 * zero-relative start and end indices (inclusive).  The text, icon
	 * and other attributes of the items are set to their default values.
	 * If the table was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param start the start index of the item to clear
	 * @param end the end index of the item to clear
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool Clear(int start, int end) {
		return w_table_clear_range(W_TABLE(this), start, end) > 0;
	}
	/**
	 * Clears the items at the given zero-relative indices in the receiver.
	 * The text, icon and other attributes of the items are set to their default
	 * values.  If the table was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param indices the array of indices of the items
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool Clear(int *indices, size_t length) {
		return w_table_clear_indices(W_TABLE(this), indices, length) > 0;
	}
	/**
	 * Clears all the items in the receiver. The text, icon and other
	 * attributes of the items are set to their default values. If the
	 * table was created with the <code>SWT.VIRTUAL</code> style, these
	 * attributes are requested again as needed.
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	bool ClearAll() {
		return w_table_clear_all(W_TABLE(this)) > 0;
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
	bool Deselect(int *indices, size_t length) {
		return w_table_deselect_indices(W_TABLE(this), indices, length) > 0;
	}
	/**
	 * Deselects the item at the given zero-relative index in the receiver.
	 * If the item at the index was already deselected, it remains
	 * deselected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to deselect
	 */
	bool Deselect(int index) {
		return w_table_deselect_index(W_TABLE(this), index) > 0;
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
	bool Deselect(int start, int end) {
		return w_table_deselect_range(W_TABLE(this), start, end) > 0;
	}
	/**
	 * Deselects all selected items in the receiver.
	 */
	bool DeselectAll() {
		return w_table_deselect_all(W_TABLE(this)) > 0;
	}
	/**
	 * Returns the column at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 * Columns are returned in the order that they were created.
	 * If no <code>TableColumn</code>s were created by the programmer,
	 * this method will throw <code>ERROR_INVALID_RANGE</code> despite
	 * the fact that a single column of data may be visible in the table.
	 * This occurs when the programmer uses the table like a list, adding
	 * items but never creating a column.
	 *
	 * @param index the index of the column to return
	 * @return the column at the given index
	 *
	 * @see Table#getColumnOrder()
	 * @see Table#setColumnOrder(int[])
	 * @see TableColumn#getMoveable()
	 * @see TableColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	WTableColumn& GetColumn(int index, WTableColumn &column) {
		w_table_get_column(W_TABLE(this), index, W_TABLECOLUMN(&column));
		return column;
	}
	/**
	 * Returns the column at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 * Columns are returned in the order that they were created.
	 * If no <code>TableColumn</code>s were created by the programmer,
	 * this method will throw <code>ERROR_INVALID_RANGE</code> despite
	 * the fact that a single column of data may be visible in the table.
	 * This occurs when the programmer uses the table like a list, adding
	 * items but never creating a column.
	 *
	 * @param index the index of the column to return
	 * @return the column at the given index
	 *
	 * @see Table#getColumnOrder()
	 * @see Table#setColumnOrder(int[])
	 * @see TableColumn#getMoveable()
	 * @see TableColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	WTableColumn& GetColumn(int index, const WTableColumn &column =
			WTableColumn()) {
		return GetColumn(index, (WTableColumn&) column);
	}
	/**
	 * Returns the number of columns contained in the receiver.
	 * If no <code>TableColumn</code>s were created by the programmer,
	 * this value is zero, despite the fact that visually, one column
	 * of items may be visible. This occurs when the programmer uses
	 * the table like a list, adding items but never creating a column.
	 *
	 * @return the number of columns
	 */
	int GetColumnCount() {
		return w_table_get_column_count(W_TABLE(this));
	}
	/**
	 * Returns an array of <code>TableColumn</code>s which are the
	 * columns in the receiver.  Columns are returned in the order
	 * that they were created.  If no <code>TableColumn</code>s were
	 * created by the programmer, the array is empty, despite the fact
	 * that visually, one column of items may be visible. This occurs
	 * when the programmer uses the table like a list, adding items but
	 * never creating a column.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 *
	 * @see Table#getColumnOrder()
	 * @see Table#setColumnOrder(int[])
	 * @see TableColumn#getMoveable()
	 * @see TableColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	void GetColumns(WIterator<WTableColumn> &columns) {
		w_table_get_columns(W_TABLE(this), (w_iterator*) &columns);
	}
	/**
	 * Returns the width in pixels of a grid line.
	 *
	 * @return the width of a grid line in pixels
	 */
	int GetGridLineWidth() {
		return w_table_get_gridline_width(W_TABLE(this));
	}
	/**
	 * Returns the height of the receiver's header
	 *
	 * @return the height of the header or zero if the header is not visible
	 */
	int GetHeaderHeight() {
		return w_table_get_header_height(W_TABLE(this));
	}
	/**
	 * Returns <code>true</code> if the receiver's header is visible,
	 * and <code>false</code> otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, this method
	 * may still indicate that it is considered visible even though
	 * it may not actually be showing.
	 * </p>
	 *
	 * @return the receiver's header's visibility state
	 */
	bool GetHeaderVisible() {
		return w_table_get_header_visible(W_TABLE(this));
	}
	WImageList* GetImageList(){
		return (WImageList*)w_table_get_imagelist(W_TABLE(this));
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTableItem& GetItem(int index, WTableItem &item) {
		w_table_get_item(W_TABLE(this), index, W_TABLEITEM(&item));
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTableItem& GetItem(int index, const WTableItem &item = WTableItem()) {
		return GetItem(index, (WTableItem&) item);
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the SWT.FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WTableItem& GetItem(const WPoint &point, WTableItem &item) {
		w_table_get_item_from_point(W_TABLE(this), (w_point*) &point,
				W_TABLEITEM(&item));
		return item;
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the SWT.FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WTableItem& GetItem(const WPoint &point, const WTableItem &item =
			WTableItem()) {
		return GetItem(point, (WTableItem&) item);
	}
	/**
	 * Returns the item at the given point in the receiver
	 * or null if no such item exists. The point is in the
	 * coordinate system of the receiver.
	 * <p>
	 * The item that is returned represents an item that could be selected by the user.
	 * For example, if selection only occurs in items in the first column, then null is
	 * returned if the point is outside of the item.
	 * Note that the SWT.FULL_SELECTION style hint, which specifies the selection policy,
	 * determines the extent of the selection.
	 * </p>
	 *
	 * @param point the point used to locate the item
	 * @return the item at the given point, or null if the point is not in a selectable item
	 */
	WTableItem& GetItem(int x, int y, const WTableItem &item = WTableItem()) {
		return GetItem(WPoint(x, y), (WTableItem&) item);
	}
	/**
	 * Returns the number of items contained in the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_table_get_item_count(W_TABLE(this));
	}
	/**
	 * Returns the height of the area which would be used to
	 * display <em>one</em> of the items in the receiver.
	 *
	 * @return the height of one item
	 */
	int GetItemHeight() {
		return w_table_get_item_height(W_TABLE(this));
	}

	/**
	 * Returns a (possibly empty) array of <code>TableItem</code>s which
	 * are the items in the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 */
	void GetItems(WIterator<WTableItem> &items) {
		w_table_get_items(W_TABLE(this), (w_iterator*) &items);
	}
	/**
	 * Returns <code>true</code> if the receiver's lines are visible,
	 * and <code>false</code> otherwise. Note that some platforms draw
	 * grid lines while others may draw alternating row colors.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, this method
	 * may still indicate that it is considered visible even though
	 * it may not actually be showing.
	 * </p>
	 *
	 * @return the visibility state of the lines
	 */
	bool GetLinesVisible() {
		return w_table_get_lines_visible(W_TABLE(this));
	}
	/**
	 * Returns an array of <code>TableItem</code>s that are currently
	 * selected in the receiver. The order of the items is unspecified.
	 * An empty array indicates that no items are selected.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its selection, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 * @return an array representing the selection
	 */
	void GetSelection(WIterator<WTableItem> &items) {

	}
	/**
	 * Returns the number of selected items contained in the receiver.
	 *
	 * @return the number of selected items
	 */
	int GetSelectionCount() {
		return w_table_get_selection_count(W_TABLE(this));
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * selected in the receiver, or -1 if no item is selected.
	 *
	 * @return the index of the selected item
	 */
	int GetSelectionIndex() {
		return w_table_get_selection_count(W_TABLE(this));
	}
	/**
	 * Returns the column which shows the sort indicator for
	 * the receiver. The value may be null if no column shows
	 * the sort indicator.
	 *
	 * @return the sort indicator
	 *
	 * @see #setSortColumn(TableColumn)
	 */
	WTableColumn& GetSortColumn(WTableColumn &column) {
		w_table_get_sort_column(W_TABLE(this), W_TABLECOLUMN(&column));
		return column;
	}
	/**
	 * Returns the column which shows the sort indicator for
	 * the receiver. The value may be null if no column shows
	 * the sort indicator.
	 *
	 * @return the sort indicator
	 *
	 * @see #setSortColumn(TableColumn)
	 */
	WTableColumn& GetSortColumn(const WTableColumn &column = WTableColumn()) {
		return GetSortColumn((WTableColumn&) column);
	}
	/**
	 * Returns the direction of the sort indicator for the receiver.
	 * The value will be one of <code>UP</code>, <code>DOWN</code>
	 * or <code>NONE</code>.
	 *
	 * @return the sort direction
	 *
	 * @see #setSortDirection(int)
	 */
	int GetSortDirection() {
		return w_table_get_sort_direction(W_TABLE(this));
	}
	/**
	 * Returns the zero-relative index of the item which is currently
	 * at the top of the receiver. This index can change when items are
	 * scrolled or new items are added or removed.
	 *
	 * @return the index of the top item
	 */
	int GetTopIndex() {
		return w_table_get_top_index(W_TABLE(this));
	}
	bool _InsertColumn(WTableColumn& column,const char* text,int index){
		return w_table_insert_item(W_TABLE(this), W_TABLEITEM(&column), text, index) > 0;
	}
	WTableColumn& InsertColumn(WTableColumn& column,const char* text,int index){
		w_table_insert_item(W_TABLE(this), W_TABLEITEM(&column), text, index);
		return column;
	}
	bool _InsertItem(WTableItem& item,const char* text,int index){
		return w_table_insert_item(W_TABLE(this), W_TABLEITEM(&item), text, index) > 0;
	}
	WTableItem& InsertItem(WTableItem& item,const char* text,int index){
		w_table_insert_item(W_TABLE(this), W_TABLEITEM(&item), text, index);
		return item;
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
		return w_table_is_selected(W_TABLE(this), index);
	}
	/**
	 * Removes the items from the receiver's list at the given
	 * zero-relative indices.
	 *
	 * @param indices the array of indices of the items
	 */
	bool Remove(int *indices, size_t length) {
		return w_table_remove_indices(W_TABLE(this), indices, length) > 0;
	}
	/**
	 * Removes the item from the receiver at the given
	 * zero-relative index.
	 *
	 * @param index the index for the item
	 */
	bool Remove(int index) {
		return w_table_remove_index(W_TABLE(this), index) > 0;
	}
	/**
	 * Removes the items from the receiver which are
	 * between the given zero-relative start and end
	 * indices (inclusive).
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 */
	bool Remove(int start, int end) {
		return w_table_remove_range(W_TABLE(this), start, end) > 0;
	}
	/**
	 * Removes all of the items from the receiver.
	 */
	bool RemoveAll() {
		return w_table_remove_all(W_TABLE(this)) > 0;
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
	 * </p>
	 *
	 * @param indices the array of indices for the items to select
	 *
	 * @see Table#setSelection(int[])
	 */
	bool Select(int *indices, size_t length) {
		return w_table_select_indices(W_TABLE(this), indices, length) > 0;
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver.
	 * If the item at the index was already selected, it remains
	 * selected. Indices that are out of range are ignored.
	 *
	 * @param index the index of the item to select
	 */
	bool Select(int index) {
		return w_table_select_index(W_TABLE(this), index) > 0;
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
	 * </p>
	 *
	 * @param start the start of the range
	 * @param end the end of the range
	 *
	 * @see Table#setSelection(int,int)
	 */
	bool Select(int start, int end) {
		return w_table_select_range(W_TABLE(this), start, end) > 0;
	}
	/**
	 * Selects all of the items in the receiver.
	 * <p>
	 * If the receiver is single-select, do nothing.
	 * </p>
	 */
	bool SelectAll() {
		return w_table_select_all(W_TABLE(this)) > 0;
	}
	/**
	 * Marks the receiver's header as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param show the new visibility state
	 */
	bool SetHeaderVisible(bool show) {
		return w_table_set_header_visible(W_TABLE(this), show) > 0;
	}
	bool SetImageList(WImageList* imagelist){
		return w_table_set_imagelist(W_TABLE(this),W_IMAGELIST(imagelist)) > 0;
	}
	/**
	 * Marks the receiver's lines as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise. Note that some platforms draw grid lines
	 * while others may draw alternating row colors.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param show the new visibility state
	 */
	bool SetLinesVisible(bool show) {
		return w_table_set_lines_visible(W_TABLE(this), show) > 0;
	}
	/**
	 * Selects the items at the given zero-relative indices in the receiver.
	 * The current selection is cleared before the new items are selected.
	 * <p>
	 * Indices that are out of range and duplicate indices are ignored.
	 * If the receiver is single-select and multiple indices are specified,
	 * then all indices are ignored.
	 * </p>
	 *
	 * @param indices the indices of the items to select
	 *
	 * @see Table#deselectAll()
	 * @see Table#select(int[])
	 */
	bool SetSelection(int *indices, size_t length) {
		return w_table_set_selection_indices(W_TABLE(this), indices, length) > 0;
	}
	/**
	 * Selects the item at the given zero-relative index in the receiver.
	 * The current selection is first cleared, then the new item is selected.
	 *
	 * @param index the index of the item to select
	 *
	 * @see Table#deselectAll()
	 * @see Table#select(int)
	 */
	bool SetSelection(int index) {
		return w_table_set_selection_index(W_TABLE(this), index) > 0;
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
	 * </p>
	 *
	 * @param start the start index of the items to select
	 * @param end the end index of the items to select
	 *
	 * @see Table#deselectAll()
	 * @see Table#select(int,int)
	 */
	bool SetSelection(int start, int end) {
		return w_table_set_selection_range(W_TABLE(this), start, end) > 0;
	}
	/**
	 * Sets the column used by the sort indicator for the receiver. A null
	 * value will clear the sort indicator.  The current sort column is cleared
	 * before the new column is set.
	 *
	 * @param column the column used by the sort indicator or <code>null</code>
	 */
	bool SetSortColumn(WTableColumn &column) {
		return w_table_set_sort_column(W_TABLE(this), W_TABLECOLUMN(&column))
				> 0;
	}
	/**
	 * Sets the direction of the sort indicator for the receiver. The value
	 * can be one of <code>UP</code>, <code>DOWN</code> or <code>NONE</code>.
	 *
	 * @param direction the direction of the sort indicator
	 */
	bool SetSortDirection(int direction) {
		return w_table_set_sort_direction(W_TABLE(this), direction) > 0;
	}
	/**
	 * Sets the zero-relative index of the item which is currently
	 * at the top of the receiver. This index can change when items
	 * are scrolled or new items are added and removed.
	 *
	 * @param index the index of the top item
	 */
	bool SetTopIndex(int index) {
		return w_table_set_top_index(W_TABLE(this), index) > 0;
	}
	/**
	 * Shows the column.  If the column is already showing in the receiver,
	 * this method simply returns.  Otherwise, the columns are scrolled until
	 * the column is visible.
	 *
	 * @param column the column to be shown
	 */
	bool ShowColumn(WTableColumn &column) {
		return w_table_show_column(W_TABLE(this), W_TABLECOLUMN(&column)) > 0;
	}
	/**
	 * Shows the item.  If the item is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled until
	 * the item is visible.
	 *
	 * @param item the item to be shown
	 *
	 * @see Table#showSelection()
	 */
	bool ShowItem(WTableItem &item) {
		return w_table_show_item(W_TABLE(this), W_TABLEITEM(&item)) > 0;
	}
	/**
	 * Shows the selection.  If the selection is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled until
	 * the selection is visible.
	 *
	 * @see Table#showItem(TableItem)
	 */
	bool ShowSelection() {
		return w_table_show_selection(W_TABLE(this)) > 0;
	}
private:
	void *handles[(sizeof(w_table) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_TABLE_H_ */
