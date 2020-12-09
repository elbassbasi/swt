/*
 * button.c
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */
#ifdef __APPLE__
#include "button.h"
#include "../widgets/toolkit.h"
#define  INNER_BORDER  1
#define  DEFAULT_BORDER  1
/*
 * private function
 */
int _w_button_check_style(int style) {
	style = _w_widget_check_bits(style, W_PUSH, W_ARROW, W_CHECK, W_RADIO,
			W_TOGGLE, 0);
	if ((style & (W_PUSH | W_TOGGLE)) != 0) {
		return _w_widget_check_bits(style, W_CENTER, W_LEFT, W_RIGHT, 0, 0, 0);
	}
	if ((style & (W_CHECK | W_RADIO)) != 0) {
		return _w_widget_check_bits(style, W_LEFT, W_RIGHT, W_CENTER, 0, 0, 0);
	}
	if ((style & W_ARROW) != 0) {
		style |= W_NO_FOCUS;
		return _w_widget_check_bits(style, W_UP, W_DOWN, W_LEFT, W_RIGHT, 0, 0);
	}
	return style;
}
/*
 * public function
 */
int _w_button_get_alignment(w_button *button) {
	if ((_W_WIDGET(button)->style & W_ARROW) != 0) {
		if ((_W_WIDGET(button)->style & W_UP) != 0)
			return W_UP;
		if ((_W_WIDGET(button)->style & W_DOWN) != 0)
			return W_DOWN;
		if ((_W_WIDGET(button)->style & W_LEFT) != 0)
			return W_LEFT;
		if ((_W_WIDGET(button)->style & W_RIGHT) != 0)
			return W_RIGHT;
		return W_UP;
	}
	if ((_W_WIDGET(button)->style & W_LEFT) != 0)
		return W_LEFT;
	if ((_W_WIDGET(button)->style & W_CENTER) != 0)
		return W_CENTER;
	if ((_W_WIDGET(button)->style & W_RIGHT) != 0)
		return W_RIGHT;
	return W_LEFT;
}
wresult _w_button_get_grayed(w_button *button) {
	return _W_WIDGET(button)->state & STATE_BUTTON_GRAYED;
}
wresult _w_button_get_image(w_button *button, w_image *image) {
}
wresult _w_button_get_selection(w_button *button) {}
wresult _w_button_get_text(w_button *button, w_alloc alloc,void* user_data) {
}
wresult _w_button_set_alignment(w_button *button, int alignment) {
	return W_TRUE;
}
wresult _w_button_set_grayed(w_button *button, int grayed) {
	if ((_W_WIDGET(button)->style & W_CHECK) == 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_button_set_image(w_button *button, w_image *image) {
	if ((_W_WIDGET(button)->style & W_ARROW) != 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_button_set_selection(w_button *button, int selected) {
	if ((_W_WIDGET(button)->style & (W_CHECK | W_RADIO | W_TOGGLE)) == 0)
		return W_FALSE;
	return W_TRUE;
}
wresult _w_button_set_text(w_button *button, const char *text) {
	if (text == 0)
		return W_ERROR_NULL_ARGUMENT;
	return W_TRUE;
}
/*
 *
 */
wresult _w_button_create_handle(w_widget *widget, _w_widget_reserved *reserved){
    wuint64 style = _W_WIDGET(widget)->style;
    if ((style & W_PUSH) == 0) _W_WIDGET(widget)->state |= STATE_THEME_BACKGROUND;
        NSButton* button = SWTButton_new(widget);
        /*NSButtonCell cell = (NSButtonCell)new SWTButtonCell ().alloc ().init ();
        widget.setCell (cell);
        cell.release ();*/
        if ((style & (W_PUSH | W_TOGGLE)) != 0 && (style & W_FLAT) == 0) {
            /*NSView superview = parent.view;
            while (superview != null) {
                if (superview.isKindOfClass(OS.class_NSTableView)) {
                    style |= SWT.FLAT;
                    break;
                }
                superview = superview.superview();
            }*/
        }
        int type = NSMomentaryLightButton;
        if ((style & W_PUSH) != 0) {
            int BezelStyle;
            if ((style & W_FLAT) != 0) {
                BezelStyle= NSShadowlessSquareBezelStyle;
            } else {
               BezelStyle =((style & W_WRAP) != 0) ? NSRegularSquareBezelStyle : NSRoundedBezelStyle;
            }
            NSButton_setBezelStyle(button, BezelStyle);
        } else if ((style & W_CHECK) != 0) {
            type = NSSwitchButton;
        } else if ((style & W_RADIO) != 0) {
            type = NSRadioButton;
            //radioParent = (SWTView) new SWTView().alloc().init();
        } else if ((style & W_TOGGLE) != 0) {
            type = NSPushOnPushOffButton;
            int BezelStyle;
            if ((style & W_FLAT) != 0) {
                BezelStyle= NSShadowlessSquareBezelStyle;
            } else {
               BezelStyle =((style & W_WRAP) != 0) ? NSRegularSquareBezelStyle : NSRoundedBezelStyle;
            }
            NSButton_setBezelStyle(button, BezelStyle);
        } else if ((style & W_ARROW) != 0) {
            NSButton_setBezelStyle(button, NSShadowlessSquareBezelStyle);
        }
    NSButton_setButtonType(button, type);
        /*widget.setButtonType(type);
        widget.setTitle(NSString.string());
        widget.setImagePosition(OS.NSImageLeft);
        widget.setTarget(widget);
        widget.setAction(OS.sel_sendSelection);
        view = widget;
        _setAlignment(style);*/
    _W_WIDGET(widget)->handle =(NSView*) button;
}
int _w_button_compute_size(w_widget *widget, struct w_event_compute_size *e,
		struct _w_widget_reserved *reserved) {
	int wHint = e->wHint;
	int hHint = e->hHint;
	if (wHint != W_DEFAULT && wHint < 0)
		wHint = 0;
	if (hHint != W_DEFAULT && hHint < 0)
		hHint = 0;
    e->size->width = 70;
    e->size->height = 40;
    /*if ((style & SWT.ARROW) != 0) {
        // TODO use some OS metric instead of hardcoded values
        int width = wHint != SWT.DEFAULT ? wHint : 14;
        int height = hHint != SWT.DEFAULT ? hHint : 14;
        return new Point (width, height);
    }
    NSSize size = null;
    NSCell cell = ((NSButton)view).cell ();
    if ((style & SWT.WRAP) != 0 && wHint != SWT.DEFAULT) {
        NSRect rect = new NSRect ();
        rect.width = wHint;
        rect.height = hHint != SWT.DEFAULT ? hHint : MAX_SIZE;
        size = cell.cellSizeForBounds (rect);
    } else {
        size = cell.cellSize ();
    }
    int width = (int)Math.ceil (size.width);
    int height = (int)Math.ceil (size.height);
    if (wHint != SWT.DEFAULT) width = wHint;
    if (hHint != SWT.DEFAULT) height = hHint;
    return new Point (width, height);*/
	return W_TRUE;
}
int _w_button_post_event(w_widget *widget, w_event *ee) {
	return _w_control_post_event(widget, ee);
}

void _w_button_class_init(struct _w_button_class *clazz) {
	_w_control_class_init(W_CONTROL_CLASS(clazz));
	W_WIDGET_CLASS(clazz)->class_id = _W_CLASS_BUTTON;
	W_WIDGET_CLASS(clazz)->class_size = sizeof(struct _w_button_class);
	W_WIDGET_CLASS(clazz)->object_total_size = sizeof(w_button);
	W_WIDGET_CLASS(clazz)->object_used_size = sizeof(_w_button);
	/*
	 * public function
	 */
	clazz->get_alignment = _w_button_get_alignment;
	clazz->get_grayed = _w_button_get_grayed;
	clazz->get_image = _w_button_get_image;
	clazz->get_selection = _w_button_get_selection;
	clazz->get_text = _w_button_get_text;
	clazz->set_alignment = _w_button_set_alignment;
	clazz->set_grayed = _w_button_set_grayed;
	clazz->set_image = _w_button_set_image;
	clazz->set_selection = _w_button_set_selection;
	clazz->set_text = _w_button_set_text;
	/*
	 * reserved
	 */
	struct _w_control_reserved *reserved = _W_CONTROL_RESERVED(
			W_WIDGET_CLASS(clazz)->reserved[0]);
	reserved->widget.compute_size = _w_button_compute_size;
    _W_WIDGET_RESERVED(reserved)->create_handle = _w_button_create_handle;
	/*
	 * signals
	 */

}
#endif

