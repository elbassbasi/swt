/*
 * Scale.h
 *
 *  Created on: 9 ao�t 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SWTP_CONTROLS_SCALE_H_
#define SWTP_CONTROLS_SCALE_H_
#include "../widgets/Control.h"
/**
 * Instances of the receiver represent a selectable user
 * interface object that present a range of continuous
 * numeric values.
 * <dl>
 * <dt><b>Styles:</b></dt>
 * <dd>HORIZONTAL, VERTICAL</dd>
 * <dt><b>Events:</b></dt>
 * <dd>Selection</dd>
 * </dl>
 * <p>
 * Note: Only one of the styles HORIZONTAL and VERTICAL may be specified.
 * </p><p>
 * <p>
 * IMPORTANT: This class is <em>not</em> intended to be subclassed.
 * </p>
 */
class SWTP_PUBLIC WScale: public WControl {
public:
	WScale() {

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
	 * @see SWT#HORIZONTAL
	 * @see SWT#VERTICAL
	 * @see Widget#checkSubclass
	 * @see Widget#getStyle
	 */
	WScale(WComposite *parent, wuint64 style) {
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
	 * @see SWT#HORIZONTAL
	 * @see SWT#VERTICAL
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
	 * Returns the amount that the receiver's value will be
	 * modified by when the up/down (or right/left) arrows
	 * are pressed.
	 *
	 * @return the increment
	 */
	int GetIncrement() {
		return w_scale_get_increment(W_SCALE(this));
	}
	/**
	 * Returns the maximum value which the receiver will allow.
	 *
	 * @return the maximum
	 */
	int GetMaximum() {
		return w_scale_get_maximum(W_SCALE(this));
	}
	/**
	 * Returns the minimum value which the receiver will allow.
	 *
	 * @return the minimum
	 */
	int GetMinimum() {
		return w_scale_get_minimum(W_SCALE(this));
	}
	/**
	 * Returns the amount that the receiver's value will be
	 * modified by when the page increment/decrement areas
	 * are selected.
	 *
	 * @return the page increment
	 */
	int GetPageIncrement() {
		return w_scale_get_page_increment(W_SCALE(this));
	}
	/**
	 * Returns the 'selection', which is the receiver's position.
	 *
	 * @return the selection
	 */
	int GetSelection() {
		return w_scale_get_selection(W_SCALE(this));
	}
	/**
	 * Sets the amount that the receiver's value will be
	 * modified by when the up/down (or right/left) arrows
	 * are pressed to the argument, which must be at least
	 * one.
	 *
	 * @param increment the new increment (must be greater than zero)
	 */
	bool SetIncrement(int increment) {
		return w_scale_set_increment(W_SCALE(this), increment) > 0;
	}
	/**
	 * Sets the maximum value that the receiver will allow.  This new
	 * value will be ignored if it is not greater than the receiver's current
	 * minimum value.  If the new maximum is applied then the receiver's
	 * selection value will be adjusted if necessary to fall within its new range.
	 *
	 * @param value the new maximum, which must be greater than the current minimum
	 */
	bool SetMaximum(int value) {
		return w_scale_set_maximum(W_SCALE(this), value) > 0;
	}
	/**
	 * Sets the minimum value that the receiver will allow.  This new
	 * value will be ignored if it is negative or is not less than the receiver's
	 * current maximum value.  If the new minimum is applied then the receiver's
	 * selection value will be adjusted if necessary to fall within its new range.
	 *
	 * @param value the new minimum, which must be nonnegative and less than the current maximum
	 */
	bool SetMinimum(int value) {
		return w_scale_set_minimum(W_SCALE(this), value) > 0;
	}
	/**
	 * Sets the amount that the receiver's value will be
	 * modified by when the page increment/decrement areas
	 * are selected to the argument, which must be at least
	 * one.
	 *
	 * @param pageIncrement the page increment (must be greater than zero)
	 */
	bool SetPageIncrement(int pageIncrement) {
		return w_scale_set_page_increment(W_SCALE(this), pageIncrement) > 0;
	}
	/**
	 * Sets the 'selection', which is the receiver's value,
	 * to the argument which must be greater than or equal to zero.
	 *
	 * @param value the new selection (must be zero or greater)
	 */
	bool SetSelection(int value) {
		return w_scale_set_selection(W_SCALE(this), value) > 0;
	}
private:
	void *handles[(sizeof(w_scale) - sizeof(w_control)) / sizeof(void*)];
};

#endif /* SWTP_CONTROLS_SCALE_H_ */
