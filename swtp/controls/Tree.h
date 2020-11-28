/*
 * Tree.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_TREE_H_
#define SWTP_CONTROLS_TREE_H_
#include "../widgets/Composite.h"
class WTree;
/**
 * Instances of this class represent a column in a tree widget.
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
class SWTP_PUBLIC WTreeColumn: public WItem {
public:
	/**
	 * Returns a value which describes the position of the
	 * text or image in the receiver. The value will be one of
	 * <code>LEFT</code>, <code>RIGHT</code> or <code>CENTER</code>.
	 *
	 * @return the alignment
	 */
	int GetAlignment() {
		return w_treecolumn_get_alignment(W_TREECOLUMN(this));

	}
	int GetImage() {
		return w_treecolumn_get_image(W_TREECOLUMN(this));
	}
	/**
	 * Gets the moveable attribute. A column that is
	 * not moveable cannot be reordered by the user
	 * by dragging the header but may be reordered
	 * by the programmer.
	 *
	 * @return the moveable attribute
	 *
	 * @see Tree#getColumnOrder()
	 * @see Tree#setColumnOrder(int[])
	 * @see TreeColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	bool GetMoveable() {
		return w_treecolumn_get_moveable(W_TREECOLUMN(this));
	}
	/**
	 * Returns an array of zero-relative integers that map
	 * the creation order of the receiver's items to the
	 * order in which they are currently being displayed.
	 * <p>
	 * Specifically, the indices of the returned array represent
	 * the current visual order of the items, and the contents
	 * of the array represent the creation order of the items.
	 * </p><p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the current visual order of the receiver's items
	 *
	 * @see Tree#setColumnOrder(int[])
	 * @see TreeColumn#getMoveable()
	 * @see TreeColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	int GetOrder() {
		return w_treecolumn_get_order(W_TREECOLUMN(this));
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Tree</code>.
	 *
	 * @return the receiver's parent
	 */
	WTree* GetParent() {
		return (WTree*) WItem::GetParentWidget();
	}
	/**
	 * Gets the resizable attribute. A column that is
	 * not resizable cannot be dragged by the user but
	 * may be resized by the programmer.
	 *
	 * @return the resizable attribute
	 */
	bool GetResizable() {
		return w_treecolumn_get_resizable(W_TREECOLUMN(this));
	}
	/**
	 * Returns the receiver's tool tip text, or null if it has
	 * not been set.
	 *
	 * @return the receiver's tool tip text
	 */
	WString GetToolTipText() {
		w_string_ref *ref = 0;
		w_treecolumn_get_tooltip_text(W_TREECOLUMN(this), w_alloc_string_ref,
				&ref);
		return ref;
	}
	/**
	 * Gets the width of the receiver.
	 *
	 * @return the width
	 */
	int GetWidth() {
		return w_treecolumn_get_width(W_TREECOLUMN(this));
	}
	/**
	 * Causes the receiver to be resized to its preferred size.
	 * For a composite, this involves computing the preferred size
	 * from its layout, if there is one.
	 */
	WTreeColumn& Pack() {
		w_treecolumn_pack(W_TREECOLUMN(this));
		return *this;
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
	WTreeColumn& SetAlignment(int alignment) {
		w_treecolumn_set_alignment(W_TREECOLUMN(this), alignment);
		return *this;
	}
	WTreeColumn& SetImage(int image) {
		w_treecolumn_set_image(W_TREECOLUMN(this), image);
		return *this;
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
	 * @see Tree#setColumnOrder(int[])
	 * @see Tree#getColumnOrder()
	 * @see TreeColumn#getMoveable()
	 * @see SWT#Move
	 */
	WTreeColumn& SetMoveable(bool moveable) {
		w_treecolumn_set_moveable(W_TREECOLUMN(this), moveable);
		return *this;
	}
	/**
	 * Sets the order that the items in the receiver should
	 * be displayed in to the given argument which is described
	 * in terms of the zero-relative ordering of when the items
	 * were added.
	 *
	 * @param order the new order to display the items
	 *
	 * @see Tree#getColumnOrder()
	 * @see TreeColumn#getMoveable()
	 * @see TreeColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	bool SetOrder(int order) {
		return w_treecolumn_set_order(W_TREECOLUMN(this), order) > 0;
	}
	/**
	 * Sets the resizable attribute.  A column that is
	 * not resizable cannot be dragged by the user but
	 * may be resized by the programmer.
	 *
	 * @param resizable the resize attribute
	 */
	WTreeColumn& SetResizable(bool resizable) {
		w_treecolumn_set_resizable(W_TREECOLUMN(this), resizable);
		return *this;
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
	WTreeColumn& SetToolTipText(const char *text) {
		w_treecolumn_set_tooltip_text(W_TREECOLUMN(this), text);
		return *this;
	}
	/**
	 * Sets the width of the receiver.
	 *
	 * @param width the new width
	 */
	WTreeColumn& SetWidth(int width) {
		w_treecolumn_set_width(W_TREECOLUMN(this), width);
		return *this;
	}
private:
	void *handles[(sizeof(w_treecolumn) - sizeof(w_item)) / sizeof(void*)];
};
/**
 * Instances of this class represent a selectable user interface object
 * that represents a hierarchy of tree items in a tree widget.
 *
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
class SWTP_PUBLIC WTreeItem: public WItem {
public:
	/**
	 * Clears the item at the given zero-relative index in the receiver.
	 * The text, icon and other attributes of the item are set to the default
	 * value.  If the tree was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param index the index of the item to clear
	 * @param all <code>true</code> if all child items of the indexed item should be
	 * cleared recursively, and <code>false</code> otherwise
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	void Clear(int index, bool all) {
		w_treeitem_clear(W_TREEITEM(this), index, all);
	}
	/**
	 * Clears all the items in the receiver. The text, icon and other
	 * attributes of the items are set to their default values. If the
	 * tree was created with the <code>SWT.VIRTUAL</code> style, these
	 * attributes are requested again as needed.
	 *
	 * @param all <code>true</code> if all child items should be cleared
	 * recursively, and <code>false</code> otherwise
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	void ClearAll(bool all) {
		w_treeitem_clear_all(W_TREEITEM(this), all);

	}
	/**
	 * Returns a rectangle describing the size and location of the receiver's
	 * text relative to its parent.
	 *
	 * @return the bounding rectangle of the receiver's text
	 */
	WRect& GetBounds(WRect &bounds) {
		w_treeitem_get_bounds(W_TREEITEM(this), (w_rect*) &bounds);
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
	 * Returns <code>true</code> if the receiver is checked,
	 * and false otherwise.  When the parent does not have
	 * the <code>CHECK style, return false.
	 * <p>
	 *
	 * @return the checked state
	 */
	bool GetChecked() {
		return w_treeitem_get_checked(W_TREEITEM(this));
	}
	/**
	 * Returns the application defined widget data associated
	 * with the receiver, or null if it has not been set. The
	 * <em>widget data</em> is a single, unnamed field that is
	 * stored with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @return the widget data
	 * @see #setData(Object)
	 */
	void* GetData() {
		return WItem::GetData();
	}
	/**
	 * Returns <code>true</code> if the receiver is expanded,
	 * and false otherwise.
	 * <p>
	 *
	 * @return the expanded state
	 */
	bool GetExpanded() {
		return w_treeitem_get_expanded(W_TREEITEM(this));
	}
	WTreeItem& GetFirstChild(WTreeItem &firstchild) {
		w_treeitem_get_first_child(W_TREEITEM(this), W_TREEITEM(&firstchild));
		return firstchild;
	}
	WTreeItem& GetFirstChild(const WTreeItem &firstchild = WTreeItem()) {
		return GetFirstChild((WTreeItem&) firstchild);
	}
	/**
	 * Returns <code>true</code> if the receiver is grayed,
	 * and false otherwise. When the parent does not have
	 * the <code>CHECK style, return false.
	 * <p>
	 *
	 * @return the grayed state of the checkbox
	 */
	bool GetGrayed() {
		return w_treeitem_get_grayed(W_TREEITEM(this));
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTreeItem& GetItem(int index, WTreeItem &item) {
		w_treeitem_get_item(W_TREEITEM(this), index, W_TREEITEM(&item));
		return item;
	}
	/**
	 * Returns the item at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 *
	 * @param index the index of the item to return
	 * @return the item at the given index
	 */
	WTreeItem& GetItem(int index, const WTreeItem &item = WTreeItem()) {
		return GetItem(index, (WTreeItem&) item);
	}
	/**
	 * Returns the number of items contained in the receiver
	 * that are direct item children of the receiver.
	 *
	 * @return the number of items
	 */
	int GetItemCount() {
		return w_treeitem_get_item_count(W_TREEITEM(this));
	}
	/**
	 * Returns a (possibly empty) array of <code>TreeItem</code>s which
	 * are the direct item children of the receiver.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the receiver's items
	 */
	void GetItems(WIterator<WTreeItem> &items) {
		w_treeitem_get_items(W_TREEITEM(this), (w_iterator*) &items);
	}
	int GetImage() {
		return w_treeitem_get_image(W_TREEITEM(this));
	}
	WTreeItem& GetLastChild(WTreeItem &lastchild) {
		w_treeitem_get_last_child(W_TREEITEM(this), W_TREEITEM(&lastchild));
		return lastchild;
	}
	WTreeItem& GetLastChild(const WTreeItem &lastchild = WTreeItem()) {
		return GetLastChild((WTreeItem&) lastchild);
	}
	WTreeItem& GetNextSibling(WTreeItem &nextsibling) {
		w_treeitem_get_next_sibling(W_TREEITEM(this), W_TREEITEM(&nextsibling));
		return nextsibling;
	}
	WTreeItem& GetNextSibling(const WTreeItem &nextsibling = WTreeItem()) {
		return GetNextSibling((WTreeItem&) nextsibling);
	}
	/**
	 * Returns the receiver's parent, which must be a <code>Tree</code>.
	 *
	 * @return the receiver's parent
	 */
	WTree* GetParent() {
		return (WTree*) WItem::GetParentWidget();
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem& GetParentItem(WTreeItem &parentitem) {
		w_treeitem_get_parent_item(W_TREEITEM(this), W_TREEITEM(&parentitem));
		return parentitem;
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem& GetParentItem(const WTreeItem &parentitem = WTreeItem()) {
		return GetParentItem((WTreeItem&) parentitem);
	}
	WTreeItem& GetPrevSibling(WTreeItem &prevsibling) {
		w_treeitem_get_prev_sibling(W_TREEITEM(this), W_TREEITEM(&prevsibling));
		return prevsibling;
	}
	WTreeItem& GetPrevSibling(const WTreeItem &prevsibling = WTreeItem()) {
		return GetPrevSibling((WTreeItem&) prevsibling);
	}
	/*
	 *
	 */
	WTreeItem& InsertItem(WTreeItem &item, const char *text, int index) {
		w_treeitem_insert_item(W_TREEITEM(this), W_TREEITEM(&item), text,
				index);
		return item;
	}
	WTreeItem& InsertItem(const char *text, int index, const WTreeItem &item =
			WTreeItem()) {
		return InsertItem((WTreeItem&) item, text, index);
	}
	WTreeItem& AppendItem(WTreeItem &item, const char *text) {
		return InsertItem(item, text, -1);
	}
	WTreeItem& AppendItem(const char *text, const WTreeItem &item =
			WTreeItem()) {
		return InsertItem((WTreeItem&) item, text, -1);
	}
	/**
	 * Removes all of the items from the receiver.
	 * <p>
	 */
	bool RemoveAll() {
		return w_treeitem_remove_all(W_TREEITEM(this)) > 0;
	}
	/**
	 * Sets the checked state of the receiver.
	 * <p>
	 *
	 * @param checked the new checked state
	 */
	bool SetChecked(bool checked) {
		return w_treeitem_set_checked(W_TREEITEM(this), checked) > 0;
	}
	/**
	 * Sets the application defined widget data associated
	 * with the receiver to be the argument. The <em>widget
	 * data</em> is a single, unnamed field that is stored
	 * with every widget.
	 * <p>
	 * Applications may put arbitrary objects in this field. If
	 * the object stored in the widget data needs to be notified
	 * when the widget is disposed of, it is the application's
	 * responsibility to hook the Dispose event on the widget and
	 * do so.
	 * </p>
	 *
	 * @param data the widget data
	 * @see #getData()
	 */
	bool SetData(void *data) {
		return WItem::SetData(data);
	}
	/**
	 * Sets the expanded state of the receiver.
	 * <p>
	 *
	 * @param expanded the new expanded state
	 */
	bool SetExpanded(bool expanded) {
		return w_treeitem_set_expanded(W_TREEITEM(this), expanded) > 0;
	}
	/**
	 * Sets the grayed state of the checkbox for this item.  This state change
	 * only applies if the Tree was created with the SWT.CHECK style.
	 *
	 * @param grayed the new grayed state of the checkbox
	 */
	bool SetGrayed(bool grayed) {
		return w_treeitem_set_grayed(W_TREEITEM(this), grayed) > 0;
	}
	bool SetHasChildren() {
		return w_treeitem_set_has_children(W_TREEITEM(this)) > 0;
	}
	/**
	 * Sets the image for multiple columns in the tree.
	 *
	 * @param images the array of new images
	 */
	bool SetImage(int image) {
		return w_treeitem_set_image(W_TREEITEM(this), image) > 0;
	}
	void SetItemCount(int count) {

	}
private:
	void *handles[(sizeof(w_treeitem) - sizeof(w_item)) / sizeof(void*)];
};
class WTreeAttr {
public:
	WFont *font;
	WColor background;
	WColor foreground;
};
class WTreeEvent: public WEvent {
public:
	union {
		int detail;
		struct {
			unsigned doit :1;
			unsigned selected :1;
			unsigned focused :1;
			unsigned background :1;
			unsigned forground :1;
			unsigned hot :1;
		};
	};
	WRect *rect;
	WTreeColumn *column;
	WTreeItem *item;
	WGraphics *gc;
	WValue *value;
	WTreeAttr *attr;
};

/**
 * Instances of this class provide a selectable user interface object
 * that displays a hierarchy of items and issues notification when an
 * item in the hierarchy is selected.
 * <p>
 * The item children that may be added to instances of this class
 * must be of type <code>TreeItem</code>.
 * </p><p>
 * Style <code>VIRTUAL</code> is used to create a <code>Tree</code> whose
 * <code>TreeItem</code>s are to be populated by the client on an on-demand basis
 * instead of up-front.  This can provide significant performance improvements for
 * trees that are very large or for which <code>TreeItem</code> population is
 * expensive (for example, retrieving values from an external source).
 * </p><p>
 * Here is an example of using a <code>Tree</code> with style <code>VIRTUAL</code>:
 * <code><pre>
 *  final Tree tree = new Tree(parent, SWT.VIRTUAL | SWT.BORDER);
 *  tree.setItemCount(20);
 *  tree.addListener(SWT.SetData, new Listener() {
 *      public void handleEvent(Event event) {
 *          TreeItem item = (TreeItem)event.item;
 *          TreeItem parentItem = item.getParentItem();
 *          String text = null;
 *          if (parentItem == null) {
 *              text = "node " + tree.indexOf(item);
 *          } else {
 *              text = parentItem.getText() + " - " + parentItem.indexOf(item);
 *          }
 *          item.setText(text);
 *          System.out.println(text);
 *          item.setItemCount(10);
 *      }
 *  });
 * </pre></code>
 * </p><p>
 * Note that although this class is a subclass of <code>Composite</code>,
 * it does not normally make sense to add <code>Control</code> children to
 * it, or set a layout on it, unless implementing something like a cell
 * editor.
 * </p><p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SINGLE, MULTI, CHECK, FULL_SELECTION, VIRTUAL, NO_SCROLL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection, DefaultSelection, Collapse, Expand, SetData, MeasureItem, EraseItem, PaintItem</dd>
 * </dl>
 * </p><p>
 * Note: Only one of the styles SINGLE and MULTI may be specified.
 * </p><p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WTree: public WComposite {
public:
	WTree() {

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
	 * @see SWT#VIRTUAL
	 * @see SWT#NO_SCROLL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WTree(WComposite *parent, wuint64 style) {
		Create(parent, style);
	}
	/**
	 * create a new instance of this class given its parent
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
	 * @see SWT#VIRTUAL
	 * @see SWT#NO_SCROLL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	bool Create(WComposite *parent, wuint64 style) {
		return Create(0, parent, style);
	}
	bool Create(WToolkit *toolkit, WComposite *parent, wuint64 style) {
		return Create(parent, style, toolkit) > 0;
	}
	WResult Create(WComposite *parent, wuint64 style, WToolkit *toolkit);
	/**
	 * Clears the item at the given zero-relative index in the receiver.
	 * The text, icon and other attributes of the item are set to the default
	 * value.  If the tree was created with the <code>SWT.VIRTUAL</code> style,
	 * these attributes are requested again as needed.
	 *
	 * @param index the index of the item to clear
	 * @param all <code>true</code> if all child items of the indexed item should be
	 * cleared recursively, and <code>false</code> otherwise
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	void Clear(int index, bool all) {
		GetRootItem().Clear(index, all);
	}
	/**
	 * Clears all the items in the receiver. The text, icon and other
	 * attributes of the items are set to their default values. If the
	 * tree was created with the <code>SWT.VIRTUAL</code> style, these
	 * attributes are requested again as needed.
	 *
	 * @param all <code>true</code> if all child items should be cleared
	 * recursively, and <code>false</code> otherwise
	 *
	 * @see SWT#VIRTUAL
	 * @see SWT#SetData
	 */
	void ClearAll(bool all) {
		GetRootItem().ClearAll(all);
	}
	/**
	 * Deselects an item in the receiver.  If the item was already
	 * deselected, it remains deselected.
	 *
	 * @param item the item to be deselected
	 */
	void Deselect(WTreeItem &item) {
		w_tree_deselect(W_TREE(this), W_TREEITEM(&item));
	}
	/**
	 * Deselects all selected items in the receiver.
	 */
	void DeselectAll() {
		w_tree_deselect_all(W_TREE(this));
	}
	/**
	 * Returns the width in pixels of a grid line.
	 *
	 * @return the width of a grid line in pixels
	 */
	int GetGridLineWidth() {
		return w_tree_get_gridline_width(W_TREE(this));
	}
	/**
	 * Returns the height of the receiver's header
	 *
	 * @return the height of the header or zero if the header is not visible
	 */
	int GetHeaderHeight() {
		return w_tree_get_header_height(W_TREE(this));
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
		return w_tree_get_header_visible(W_TREE(this));
	}
	/**
	 * Returns the column at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 * Columns are returned in the order that they were created.
	 * If no <code>TreeColumn</code>s were created by the programmer,
	 * this method will throw <code>ERROR_INVALID_RANGE</code> despite
	 * the fact that a single column of data may be visible in the tree.
	 * This occurs when the programmer uses the tree like a list, adding
	 * items but never creating a column.
	 *
	 * @param index the index of the column to return
	 * @return the column at the given index
	 *
	 * @see Tree#getColumnOrder()
	 * @see Tree#setColumnOrder(int[])
	 * @see TreeColumn#getMoveable()
	 * @see TreeColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	WTreeColumn& GetColumn(int index, WTreeColumn &column) {
		w_tree_get_column(W_TREE(this), index, W_TREECOLUMN(&column));
		return column;
	}
	/**
	 * Returns the column at the given, zero-relative index in the
	 * receiver. Throws an exception if the index is out of range.
	 * Columns are returned in the order that they were created.
	 * If no <code>TreeColumn</code>s were created by the programmer,
	 * this method will throw <code>ERROR_INVALID_RANGE</code> despite
	 * the fact that a single column of data may be visible in the tree.
	 * This occurs when the programmer uses the tree like a list, adding
	 * items but never creating a column.
	 *
	 * @param index the index of the column to return
	 * @return the column at the given index
	 *
	 * @see Tree#getColumnOrder()
	 * @see Tree#setColumnOrder(int[])
	 * @see TreeColumn#getMoveable()
	 * @see TreeColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	WTreeColumn& GetColumn(int index, const WTreeColumn &column =
			WTreeColumn()) {
		return GetColumn(index, (WTreeColumn&) column);
	}
	/**
	 * Returns the number of columns contained in the receiver.
	 * If no <code>TreeColumn</code>s were created by the programmer,
	 * this value is zero, despite the fact that visually, one column
	 * of items may be visible. This occurs when the programmer uses
	 * the tree like a list, adding items but never creating a column.
	 *
	 * @return the number of columns
	 */
	int GetColumnCount() {
		return w_tree_get_column_count(W_TREE(this));
	}

	/**
	 * Returns an array of <code>TreeColumn</code>s which are the
	 * columns in the receiver. Columns are returned in the order
	 * that they were created.  If no <code>TreeColumn</code>s were
	 * created by the programmer, the array is empty, despite the fact
	 * that visually, one column of items may be visible. This occurs
	 * when the programmer uses the tree like a list, adding items but
	 * never creating a column.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its list of items, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 *
	 * @return the items in the receiver
	 *
	 * @see Tree#getColumnOrder()
	 * @see Tree#setColumnOrder(int[])
	 * @see TreeColumn#getMoveable()
	 * @see TreeColumn#setMoveable(bool)
	 * @see SWT#Move
	 */
	void GetColumns(WIterator<WTreeColumn> &columns) {
		w_tree_get_columns(W_TREE(this), (w_iterator*) &columns);
	}
	WImageList* GetImageList() {
		return (WImageList*) w_tree_get_imagelist(W_TREE(this));
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
	WTreeItem& GetItem(WTreeItem &item, const WPoint &point) {
		w_tree_get_item(W_TREE(this), (w_point*) &point, W_TREEITEM(&item));
		return item;
	}
	WTreeItem& GetItem(WTreeItem &item, int x, int y) {
		return GetItem(item, WPoint(x, y));
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
	WTreeItem& GetItem(const WPoint &point, const WTreeItem &item =
			WTreeItem()) {
		return GetItem(point, (WTreeItem&) item);
	}
	/**
	 * Returns the height of the area which would be used to
	 * display <em>one</em> of the items in the tree.
	 *
	 * @return the height of one item
	 */
	int GetItemHeight() {
		return w_tree_get_item_height(W_TREE(this));
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
		return w_tree_get_lines_visible(W_TREE(this));
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem& GetParentItem(WTreeItem &item) {
		w_tree_get_parent_item(W_TREE(this), W_TREEITEM(&item));
		return item;
	}
	/**
	 * Returns the receiver's parent item, which must be a
	 * <code>TreeItem</code> or null when the receiver is a
	 * root.
	 *
	 * @return the receiver's parent item
	 */
	WTreeItem& GetParentItem(const WTreeItem &item = WTreeItem()) {
		return GetParentItem((WTreeItem&) item);
	}
	/**
	 * Returns the root item, which must be a
	 * <code>TreeItem</code>
	 *
	 * @return the root item
	 */
	WTreeItem& GetRootItem(WTreeItem &root) {
		w_tree_get_root_item(W_TREE(this), W_TREEITEM(&root));
		return root;
	}
	/**
	 * Returns the root item, which must be a
	 * <code>TreeItem</code>
	 *
	 * @return the root item
	 */
	WTreeItem& GetRootItem(const WTreeItem &root = WTreeItem()) {
		return GetRootItem((WTreeItem&) root);
	}
	/**
	 * Returns an array of <code>TreeItem</code>s that are currently
	 * selected in the receiver. The order of the items is unspecified.
	 * An empty array indicates that no items are selected.
	 * <p>
	 * Note: This is not the actual structure used by the receiver
	 * to maintain its selection, so modifying the array will
	 * not affect the receiver.
	 * </p>
	 * @return an array representing the selection
	 */
	void GetSelection(WIterator<WTreeItem> &selection) {
		w_tree_get_selection(W_TREE(this), (w_iterator*) &selection);
	}
	/**
	 * Returns the number of selected items contained in the receiver.
	 *
	 * @return the number of selected items
	 */
	int GetSelectionCount() {
		return w_tree_get_selection_count(W_TREE(this));
	}
	/**
	 * Returns the column which shows the sort indicator for
	 * the receiver. The value may be null if no column shows
	 * the sort indicator.
	 *
	 * @return the sort indicator
	 *
	 * @see #setSortColumn(TreeColumn)
	 */
	WTreeColumn& GetSortColumn(WTreeColumn &column) {
		w_tree_get_sort_direction(W_TREE(this));
		return column;
	}
	/**
	 * Returns the column which shows the sort indicator for
	 * the receiver. The value may be null if no column shows
	 * the sort indicator.
	 *
	 * @return the sort indicator
	 *
	 * @see #setSortColumn(TreeColumn)
	 */
	WTreeColumn& GetSortColumn(const WTreeColumn &column = WTreeColumn()) {
		return GetSortColumn((WTreeColumn&) column);
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
		return w_tree_get_sort_direction(W_TREE(this));
	}
	/**
	 * Returns the item which is currently at the top of the receiver.
	 * This item can change when items are expanded, collapsed, scrolled
	 * or new items are added or removed.
	 *
	 * @return the item at the top of the receiver
	 */
	WTreeItem& GetTopItem(WTreeItem &topitem) {
		w_tree_get_top_item(W_TREE(this), W_TREEITEM(&topitem));
		return topitem;
	}
	/**
	 * Returns the item which is currently at the top of the receiver.
	 * This item can change when items are expanded, collapsed, scrolled
	 * or new items are added or removed.
	 *
	 * @return the item at the top of the receiver
	 */
	WTreeItem& GetTopItem(const WTreeItem &topitem = WTreeItem()) {
		return GetTopItem((WTreeItem&) topitem);
	}
	WTreeColumn& InsertColumn(WTreeColumn &column, const char *text,
			int index) {
		w_tree_insert_column(W_TREE(this), W_TREECOLUMN(&column), text, index);
		return column;
	}
	WTreeColumn& InsertColumn(const char *text, int index,
			const WTreeColumn &column = WTreeColumn()) {
		return InsertColumn((WTreeColumn&) column, text, index);
	}
	WTreeColumn& AppendColumn(WTreeColumn &column, const char *text) {
		return InsertColumn(column, text, -1);
	}
	WTreeColumn& AppendColumn(const char *text, const WTreeColumn &column =
			WTreeColumn()) {
		return InsertColumn(text, -1, column);
	}

	/**
	 * Removes all of the items from the receiver.
	 */
	void RemoveAll() {
		w_tree_remove_all(W_TREE(this));
	}
	/**
	 * Display a mark indicating the point at which an item will be inserted.
	 * The drop insert item has a visual hint to show where a dragged item
	 * will be inserted when dropped on the tree.
	 *
	 * @param item the insert item.  Null will clear the insertion mark.
	 * @param before true places the insert mark above 'item'. false places
	 *	the insert mark below 'item'.
	 */
	void SetInsertMark(WTreeItem &item, bool before) {
		w_tree_set_insert_mark(W_TREE(this), W_TREEITEM(&item), before);
	}
	bool SetImageList(WImageList *imagelist) {
		return w_tree_set_imagelist(W_TREE(this), (w_imagelist*) imagelist) > 0;
	}
	/**
	 * Sets the height of the area which would be used to
	 * display <em>one</em> of the items in the tree.
	 *
	 * @param itemHeight the height of one item
	 */
	void SetItemHeight(int itemHeight) {
		w_tree_set_item_height(W_TREE(this), itemHeight);
	}
	/**
	 * Marks the receiver's lines as visible if the argument is <code>true</code>,
	 * and marks it invisible otherwise. Note that some platforms draw
	 * grid lines while others may draw alternating row colors.
	 * <p>
	 * If one of the receiver's ancestors is not visible or some
	 * other condition makes the receiver not visible, marking
	 * it visible may not actually cause it to be displayed.
	 * </p>
	 *
	 * @param show the new visibility state
	 */
	void SetLinesVisible(bool show) {
		w_tree_set_lines_visible(W_TREE(this), show);
	}
	/**
	 * Selects an item in the receiver.  If the item was already
	 * selected, it remains selected.
	 *
	 * @param item the item to be selected
	 */
	void Select(WTreeItem &item) {
		w_tree_select(W_TREE(this), W_TREEITEM(&item));
	}
	/**
	 * Selects all of the items in the receiver.
	 * <p>
	 * If the receiver is single-select, do nothing.
	 * </p>
	 */
	void SelectAll() {
		w_tree_select_all(W_TREE(this));
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
	void SetHeaderVisible(bool show) {
		w_tree_set_header_visible(W_TREE(this), show);
	}
	/**
	 * Sets the receiver's selection to the given item.
	 * The current selection is cleared before the new item is selected.
	 * <p>
	 * If the item is not in the receiver, then it is ignored.
	 * </p>
	 *
	 * @param item the item to select
	 */
	void SetSelection(WTreeItem &item) {
		w_tree_set_selection(W_TREE(this), W_TREEITEM(&item));
	}
	/**
	 * Sets the column used by the sort indicator for the receiver. A null
	 * value will clear the sort indicator.  The current sort column is cleared
	 * before the new column is set.
	 *
	 * @param column the column used by the sort indicator or <code>null</code>
	 */
	void SetSortColumn(WTreeColumn &column) {
		w_tree_set_sort_column(W_TREE(this), W_TREECOLUMN(&column));
	}
	/**
	 * Sets the direction of the sort indicator for the receiver. The value
	 * can be one of <code>UP</code>, <code>DOWN</code> or <code>NONE</code>.
	 *
	 * @param direction the direction of the sort indicator
	 */
	void SetSortDirection(int direction) {
		w_tree_set_sort_direction(W_TREE(this), direction);
	}
	/**
	 * Sets the item which is currently at the top of the receiver.
	 * This item can change when items are expanded, collapsed, scrolled
	 * or new items are added or removed.
	 *
	 * @param item the item to be shown
	 *
	 * @see Tree#getTopItem()
	 */
	void SetTopItem(WTreeItem &item) {
		w_tree_set_top_item(W_TREE(this), W_TREEITEM(&item));
	}
	/**
	 * Shows the column.  If the column is already showing in the receiver,
	 * this method simply returns.  Otherwise, the columns are scrolled until
	 * the column is visible.
	 *
	 * @param column the column to be shown
	 */
	void ShowColumn(WTreeColumn &column) {
		w_tree_show_column(W_TREE(this), W_TREECOLUMN(&column));
	}
	/**
	 * Shows the item.  If the item is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled
	 * and expanded until the item is visible.
	 *
	 * @param item the item to be shown
	 *
	 * @see Tree#showSelection()
	 */
	void ShowItem(WTreeItem &item) {
		w_tree_show_item(W_TREE(this), W_TREEITEM(&item));
	}
	/**
	 * Shows the selection.  If the selection is already showing in the receiver,
	 * this method simply returns.  Otherwise, the items are scrolled until
	 * the selection is visible.
	 *
	 * @see Tree#showItem(TreeItem)
	 */
	void ShowSelection() {
		w_tree_show_selection(W_TREE(this));
	}
	void Sort() {
		w_tree_sort(W_TREE(this));
	}
	static bool IsTree(WWidget *widget) {
		return widget->GetClassId() == _W_CLASS_TREE;
	}
protected:
	bool PostEvent(WEvent *e);
	virtual bool OnItemExpand(WTreeEvent &e);
	virtual bool OnItemCollapse(WTreeEvent &e);
	virtual bool OnItemSelection(WTreeEvent &e);
	virtual bool OnItemDefaultSelection(WTreeEvent &e);
	virtual bool OnItemGetValue(WTreeEvent &e);
	virtual bool OnItemSetValue(WTreeEvent &e);
	virtual bool OnItemGetAttr(WTreeEvent &e);
	virtual bool OnItemSetAttr(WTreeEvent &e);
	virtual bool OnItemMeasure(WTreeEvent &e);
	virtual bool OnItemErase(WTreeEvent &e);
	virtual bool OnItemPaint(WTreeEvent &e);
	virtual bool OnItemDispose(WTreeEvent &e);
private:
	void *handles[(sizeof(w_tree) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_TREE_H_ */
