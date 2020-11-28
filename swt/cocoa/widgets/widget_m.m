//
//  widget_m.m
//  swt
//
//  Created by elbassbasi on 7/29/20.
//
#import "widget.h"
long NSEvent_CGEvent(NSEvent *nsEvent) {
    return [nsEvent CGEvent];
}
long NSEvent_buttonNumber(NSEvent *nsEvent) {
    return [nsEvent buttonNumber];
}
NSString* NSEvent_characters(NSEvent *nsEvent) {
    return [nsEvent characters];
}
NSString* NSEvent_charactersIgnoringModifiers(NSEvent *nsEvent) {
    return [nsEvent charactersIgnoringModifiers];
}
long NSEvent_clickCount(NSEvent *nsEvent) {
    return [nsEvent clickCount];
}
double NSEvent_deltaX(NSEvent *nsEvent) {
    return [nsEvent deltaX];
}
double NSEvent_deltaY(NSEvent *nsEvent) {
    return [nsEvent deltaY];
}
double NSEvent_doubleClickInterval() {
    return [NSEvent doubleClickInterval];
}
NSEvent* NSEvent_enterExitEventWithType(long type, w_point* location, long flags, double time, NSUInteger wNum, NSGraphicsContext* context, NSInteger eNum, NSInteger tNum, void* data) {
    NSPoint _location = NSMakePoint(location->x, location->y);
    return [NSEvent enterExitEventWithType:type location:_location modifierFlags:flags timestamp:time windowNumber:wNum context:context eventNumber:eNum trackingNumber:tNum userData:data];
}
short NSEvent_keyCode(NSEvent *nsEvent) {
    return [nsEvent keyCode];
}
void NSEvent_locationInWindow(NSEvent *nsEvent,w_point *result) {
    NSPoint _result = [nsEvent locationInWindow];
    result->x = _result.x;
    result->y = _result.y;
}
double NSEvent_magnification(NSEvent *nsEvent) {
    return [nsEvent magnification];
}
long NSEvent_modifierFlags(NSEvent *nsEvent) {
    return [nsEvent modifierFlags];
}
void NSEvent_mouseLocation(w_point *result) {
    NSPoint _result = [NSEvent mouseLocation];
    result->x = _result.x;
    result->y = _result.y;
}
NSEvent* NSEvent_otherEventWithType(long type, w_point* location, long flags, double time, NSInteger wNum, NSGraphicsContext *context, short subtype, NSInteger d1, NSInteger d2) {
    NSPoint _location=NSMakePoint(location->x, location->y);
    return [NSEvent otherEventWithType:type location:_location modifierFlags:flags timestamp:time windowNumber:wNum context:context subtype:subtype data1:d1 data2:d2];
}
long NSEvent_phase(NSEvent *nsEvent) {
    return [nsEvent phase];
}
NSUInteger NSEvent_pressedMouseButtons() {
    return [NSEvent pressedMouseButtons];
}
float NSEvent_rotation(NSEvent *nsEvent) {
    return [nsEvent rotation];
}
double NSEvent_timestamp(NSEvent *nsEvent) {
    return [nsEvent timestamp];
}
NSSet* NSEvent_touchesMatchingPhase(NSEvent *nsEvent,long phase, NSView* view) {
    return [nsEvent touchesMatchingPhase:phase inView:view];
}
long NSEvent_type(NSEvent *nsEvent) {
    return [nsEvent type];
}
NSWindow* NSEvent_window(NSEvent *nsEvent) {
    return [nsEvent window];
}
