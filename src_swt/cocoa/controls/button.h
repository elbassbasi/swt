/*
 * button.h
 *
 *  Created on: 3 août 2019
 *      Author: azeddine El Bassbasi
 */

#ifndef SRC_COCOA_CONTROLS_BUTTON_H_
#define SRC_COCOA_CONTROLS_BUTTON_H_
#ifdef __APPLE__
#include "../widgets/control.h"
#define STATE_BUTTON_GRAYED (1 << (STATE_CONTROL_END + 1))
#define STATE_BUTTON_END (STATE_CONTROL_END + 1)
typedef struct NSButton NSButton;
typedef struct NSAttributedString NSAttributedString;
NSButton* SWTButton_new(w_widget* widget);
w_widget* SWTButton_get_widget(NSView* view);
NSAttributedString* NSButton_attributedTitle(NSButton* button);
long NSButton_bezelStyle(NSButton* button);
void NSButton_setAllowsMixedState(NSButton* button,int allowsMixedState);
void NSButton_setAttributedTitle(NSButton* button,NSAttributedString* attributedTitle);
void NSButton_setBezelStyle(NSButton* button,long bezelStyle);
void NSButton_setBordered(NSButton* button,int bordered);
void NSButton_setButtonType(NSButton* button,long aType);
void NSButton_setImage(NSButton* button,NSImage* image);
void NSButton_setImagePosition(NSButton* button,long imagePosition);
void NSButton_setKeyEquivalent(NSButton* button,NSString* keyEquivalent);
void NSButton_setState(NSButton* button,long state);
void NSButton_setTitle(NSButton* button,NSString* title);
long NSButton_state(NSButton* button);
typedef struct _w_button {
	struct _w_control control;
} _w_button;

struct _w_button_reserved {
	struct _w_control_reserved control;

};
#define _W_BUTTON(x) ((_w_button*)x)
#define _W_BUTTON_RESERVED(x) ((struct _w_button_reserved*)x)

int _w_button_check_style(int style);
/*
 * public function
 */
int _w_button_get_alignment(w_button *button);
wresult _w_button_get_grayed(w_button *button);
wresult _w_button_get_image(w_button *button, w_image *image);
wresult _w_button_get_selection(w_button *button);
wresult _w_button_get_text(w_button *button, w_alloc alloc,void* user_data);
wresult _w_button_set_alignment(w_button *button, int alignment);
wresult _w_button_set_grayed(w_button *button, int grayed);
wresult _w_button_set_image(w_button *button, w_image *image);
wresult _w_button_set_selection(w_button *button, int selected);
wresult _w_button_set_text(w_button *button, const char *text);
/*
 *
 */
void _w_button_class_init(struct _w_button_class *clazz);
#endif
#endif /* SRC_GTK_CONTROLS_BUTTON_H_ */
