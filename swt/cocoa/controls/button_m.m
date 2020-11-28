//
//  button_m.m
//  swt
//
//  Created by elbassbasi on 7/29/20.
//
#import "../widgets/widget.h"
@interface SWTButton : NSButton {
 @public w_widget* widget;
}
@end
@implementation SWTButton

@end
/*
 * NSButton
 */
NSButton* SWTButton_new(w_widget* widget){
    SWTButton* button =  [[SWTButton alloc] init];
    _w_toolkit_registre_class(_NS_CLASS_BUTTON,button.class);
    button->widget = widget;
    return button;
}
w_widget* SWTButton_get_widget(NSView* view){
    if([view isKindOfClass:SWTButton.class]){
        return ((SWTButton*)view)->widget;
    }
    return 0;
}
NSAttributedString* NSButton_attributedTitle(NSButton* button) {
    return [button attributedTitle];
}

long NSButton_bezelStyle(NSButton* button) {
    return [button bezelStyle];
}

void NSButton_setAllowsMixedState(NSButton* button,int allowsMixedState) {
    [button setAllowsMixedState:allowsMixedState];
}

void NSButton_setAttributedTitle(NSButton* button,NSAttributedString* attributedTitle) {
    [button setAttributedTitle:attributedTitle];
}

void NSButton_setBezelStyle(NSButton* button,long bezelStyle) {
    [button setBezelStyle:bezelStyle];
}

void NSButton_setBordered(NSButton* button,int bordered) {
    [button setBordered:bordered];
}

void NSButton_setButtonType(NSButton* button,long aType) {
    [button setButtonType:aType];
}

void NSButton_setImage(NSButton* button,NSImage* image) {
    [button setImage:image];
}

void NSButton_setImagePosition(NSButton* button,long imagePosition) {
    [button setImagePosition:imagePosition];
}

void NSButton_setKeyEquivalent(NSButton* button,NSString* keyEquivalent) {
    [button setKeyEquivalent:keyEquivalent];
}

void NSButton_setState(NSButton* button,long state) {
    [button setState:state];
}

void NSButton_setTitle(NSButton* button,NSString* title) {
    [button setTitle:title];
}

long NSButton_state(NSButton* button) {
    return [button state];
}

Class* NSButton_cellClass() {
    return [NSButton cellClass];
}

void NSButton_setCellClass(Class* factoryId) {
    [NSButton setCellClass:factoryId];
}
/*
 * NSCellButton
 */
/*@interface SWTCellButton : NSCellButton {
    w_widget* widget;
}
@end
@implementation SWTCellButton

@end
void NSCellButton_drawBezelWithFrame(NSCellButton* cellButton,w_rect* frame, NSView* controlView) {
    [cellButton drawbez];
}

void NSCellButton_drawImage(NSCellButton* cellButton,NSImage image, NSRect frame, NSView controlView) {
    OS.objc_msgSend(this.id, OS.sel_drawImage_withFrame_inView_, image != null ? image.id : 0, frame, controlView != null ? controlView.id : 0);
}

NSRect NSCellButton_drawTitle(NSCellButton* cellButton,NSAttributedString title, NSRect frame, NSView controlView) {
    NSRect result = new NSRect();
    OS.objc_msgSend_stret(result, this.id, OS.sel_drawTitle_withFrame_inView_, title != null ? title.id : 0, frame, controlView != null ? controlView.id : 0);
    return result;
}

void NSCellButton_setBackgroundColor(NSCellButton* cellButton,NSColor backgroundColor) {
    OS.objc_msgSend(this.id, OS.sel_setBackgroundColor_, backgroundColor != null ? backgroundColor.id : 0);
}

void NSCellButton_setButtonType(NSCellButton* cellButton,long aType) {
    OS.objc_msgSend(this.id, OS.sel_setButtonType_, aType);
}

void NSCellButton_setHighlightsBy(NSCellButton* cellButton,long highlightsBy) {
    OS.objc_msgSend(this.id, OS.sel_setHighlightsBy_, highlightsBy);
}

void NSCellButton_setImagePosition(NSCellButton* cellButton,long imagePosition) {
    [cellButton setimage];
}

NSString* NSCellButton_title(NSCellButton* cellButton) {
    return [cellButton title];
}*/
