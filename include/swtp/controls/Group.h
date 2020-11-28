/*
 * Group.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_GROUP_H_
#define SWTP_CONTROLS_GROUP_H_
#include "../widgets/Composite.h"
/**
 * Instances of this class provide an etched border
 * with an optional title.
 * <p>
 * Shadow styles are hints and may not be honoured
 * by the platform.  To create a group with the
 * default shadow style for the platform, do not
 * specify a shadow style.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>SHADOW_ETCHED_IN, SHADOW_ETCHED_OUT, SHADOW_IN, SHADOW_OUT, SHADOW_NONE</dd>
 * <dt><b>Events:</b></dt>
 * <dd>(none)</dd>
 * </dl>
 * <p>
 * Note: Only one of the above styles may be specified.
 * </p>
 */
class SWTP_PUBLIC WGroup: public WComposite {
public:
	WGroup() {

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
	 * @see SWT#SHADOW_ETCHED_IN
	 * @see SWT#SHADOW_ETCHED_OUT
	 * @see SWT#SHADOW_IN
	 * @see SWT#SHADOW_OUT
	 * @see SWT#SHADOW_NONE
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WGroup(WComposite *parent, wuint64 style) {
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
	 * @see SWT#SHADOW_ETCHED_IN
	 * @see SWT#SHADOW_ETCHED_OUT
	 * @see SWT#SHADOW_IN
	 * @see SWT#SHADOW_OUT
	 * @see SWT#SHADOW_NONE
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
	 * Returns the receiver's text, which is the string that the
	 * is used as the <em>title</em>. If the text has not previously
	 * been set, returns an empty string.
	 *
	 * @return the text
	 */
	WString GetText() {
		WString str;
		w_group_get_text(W_GROUP(this), w_alloc_string_ref, &str.ref);
		return str;
	}
	char* GetTextChars() {
		char *buffer = 0;
		w_group_get_text(W_GROUP(this), w_alloc_buffer_new, &buffer);
		return buffer;
	}
	char* GetText(char *text, size_t length) {
		w_alloc_buffer buffer;
		buffer.buffer = text;
		buffer.size = length;
		w_group_get_text(W_GROUP(this), w_alloc_buffer_copy, &buffer);
		return (char*) text;
	}
	std::string GetTextStd() {
		std::string str;
		w_group_get_text(W_GROUP(this), w_alloc_std_string, &str);
		return std::move(str);
	}
	/**
	 * Sets the receiver's text, which is the string that will
	 * be displayed as the receiver's <em>title</em>, to the argument,
	 * which may not be null. The string may include the mnemonic character.
	 * </p>
	 * Mnemonics are indicated by an '&amp;' that causes the next
	 * character to be the mnemonic.  When the user presses a
	 * key sequence that matches the mnemonic, focus is assigned
	 * to the first child of the group. On most platforms, the
	 * mnemonic appears underlined but may be emphasised in a
	 * platform specific manner.  The mnemonic indicator character
	 * '&amp;' can be escaped by doubling it in the string, causing
	 * a single '&amp;' to be displayed.
	 * </p>
	 * @param string the new text
	 */
	bool SetText(const char *string) {
		return w_group_set_text(W_GROUP(this), string) > 0;
	}
private:
	void *handles[(sizeof(w_group) - sizeof(w_composite)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_GROUP_H_ */
