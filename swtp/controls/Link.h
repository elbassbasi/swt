/*
 * Link.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_LINK_H_
#define SWTP_CONTROLS_LINK_H_
#include "../widgets/Control.h"
/**
 * Instances of this class represent a selectable
 * user interface object that displays a text with
 * links.
 * <p>
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>(none)</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WLink: public WControl {
public:
	WLink() {

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
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WLink(WComposite *parent, wuint64 style) {
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
	 * Returns the receiver's text, which will be an empty
	 * string if it has never been set.
	 *
	 * @return the receiver's text
	 */
	WString GetText() {
		w_string_ref *ref = 0;
		w_link_get_text(W_LINK(this),w_alloc_string_ref, &ref);
		return ref;
	}
	/**
	 * Sets the receiver's text.
	 * <p>
	 * The string can contain both regular text and hyperlinks.  A hyperlink
	 * is delimited by an anchor tag, &lt;A&gt; and &lt;/A&gt;.  Within an
	 * anchor, a single HREF attribute is supported.  When a hyperlink is
	 * selected, the text field of the selection event contains either the
	 * text of the hyperlink or the value of its HREF, if one was specified.
	 * In the rare case of identical hyperlinks within the same string, the
	 * HREF attribute can be used to distinguish between them.  The string may
	 * include the mnemonic character and line delimiters. The only delimiter
	 * the HREF attribute supports is the quotation mark (").
	 * </p>
	 * <p>
	 * Mnemonics are indicated by an '&amp;' that causes the next
	 * character to be the mnemonic. The receiver can have a
	 * mnemonic in the text preceding each link. When the user presses a
	 * key sequence that matches the mnemonic, focus is assigned
	 * to the link that follows the text. Mnemonics in links and in
	 * the trailing text are ignored. On most platforms,
	 * the mnemonic appears underlined but may be emphasised in a
	 * platform specific manner.  The mnemonic indicator character
	 * '&amp;' can be escaped by doubling it in the string, causing
	 * a single '&amp;' to be displayed.
	 * </p>
	 *
	 * @param string the new text
	 */
	bool SetText(const char *string) {
		return w_link_set_text(W_LINK(this), string) > 0;
	}
private:
	void *handles[(sizeof(w_link) - sizeof(w_control)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_LINK_H_ */
