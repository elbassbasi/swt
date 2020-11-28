#include "core.h"
#import <objc/objc.h>
#import <objc/objc-class.h>
IMP oldCursorSetProc;
void* _w_toolkit_cursor_set_proc (void* id, void* sel);
void* _w_toolkit_oldcursor_set_proc_call (void* id, void* sel){
    ((void*(*)(void*,void*))oldCursorSetProc)(id,sel);
}
void NSCursor_init(){
    Class cls = objc_lookUpClass("NSCursor");
    SEL sel = sel_getUid("set");
    Method method = class_getInstanceMethod(cls, sel);
    if (method != 0) oldCursorSetProc = method_setImplementation(method, (IMP)_w_toolkit_cursor_set_proc);
}
/*
 * NSObject
 */
Class NSObject_class(NSObject* object){
    return object.class;
}
void NSObject_retain(NSObject* object){
    [object retain];
}
void NSObject_release(NSObject* object){
    [object release];
}
void NSObject_autorelease(NSObject* object){
    [object autorelease];
}
NSUInteger NSObject_retainCount(NSObject* object){
    return [object retainCount];
}
/*
 *
 */
NSString* NSString_stringWithUTF8String(const char* nullTerminatedCString) {
    return [NSString stringWithUTF8String:nullTerminatedCString];
}
NSString* NSString_stringWithCharacters(const wchar_t* str,size_t length) {
    return [NSString stringWithCharacters:(const unichar*)str length:length];
}
NSString* NSString_stringWithUTF8(const char* str,size_t length) {
    if(length == -1){
        return [NSString stringWithUTF8String:str];
    }else{
        NSString* _str = [NSString alloc];
        return [_str initWithBytes:str length:length encoding:NSUTF8StringEncoding];
    }
}
NSString* NSString_initWithBytes(const void *bytes,NSUInteger length, int encoding){
    NSString* str = [NSString alloc];
    return [str initWithBytes:bytes length:length encoding:encoding];
}
NSString* NSString_initWithBytes_utf8(const void *bytes,NSUInteger length){
    return NSString_initWithBytes(bytes,length,NSUTF8StringEncoding);
}
/*
 *
 */
int NSArray_containsObject(NSArray *array,NSObject* anObject) {
    return [array containsObject:anObject];
}

NSUInteger NSArray_count(NSArray *array) {
    return [array count];
}

NSUInteger NSArray_indexOfObjectIdenticalTo(NSArray *array,NSObject* anObject) {
    return [array indexOfObjectIdenticalTo:anObject];
}

NSObject* NSArray_objectAtIndex(NSArray *array,NSUInteger index) {
    return [array objectAtIndex:index];
}
