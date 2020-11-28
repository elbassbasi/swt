//
//  core.h
//  swt
//
//  Created by elbassbasi on 7/23/20.
//

#ifndef SRC_COCOA_GRAPHICS_CORE_H_
#define SRC_COCOA_GRAPHICS_CORE_H_
#ifdef __APPLE__
#include <swt.h>
#ifdef __OBJC__
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#else
#import <objc/NSObjCRuntime.h>
#import <objc/NSObject.h>

typedef struct NSObject NSObject;
typedef struct NSString NSString;
typedef struct NSArray NSArray;
enum {
    NSAlertFirstButtonReturn = 1000,//
    NSAlertSecondButtonReturn = 1001,//
    NSAlertThirdButtonReturn = 1002,//
    NSAlphaFirstBitmapFormat = 1,//
    NSAlphaNonpremultipliedBitmapFormat = 2,//
    NSAlternateKeyMask = 524288,//
    NSAnyEventMask = -1L,//
    NSAppKitDefined = 13,//
    NSApplicationActivateIgnoringOtherApps = 2,//
    NSApplicationActivationPolicyRegular = 0,//
    NSApplicationDefined = 15,//
    NSApplicationDelegateReplySuccess = 0,//
    NSAtTop = 2,//
    NSBackgroundStyleRaised = 2,//
    NSBackingStoreBuffered = 2,//
    NSBackspaceCharacter = 8,//
    NSBevelLineJoinStyle = 2,//
    NSBezelBorder = 2,//
    NSBoldFontMask = 2,//
    NSBorderlessWindowMask = 0,//
    NSBottomTabsBezelBorder = 2,//
    NSBoxCustom = 4,//
    NSBoxSeparator = 2,//
    NSButtLineCapStyle = 0,//
    NSCancelButton = 0,//
    NSCarriageReturnCharacter = 13,//
    NSClockAndCalendarDatePickerStyle = 1,//
    NSClosableWindowMask = 2,//
    NSClosePathBezierPathElement = 3,//
    NSCommandKeyMask = 1048576,//
    NSCompositeClear = 0,//
    NSCompositeCopy = 1,//
    NSCompositeSourceAtop = 5,//
    NSCompositeSourceOver = 2,//
    NSContentsCellMask = 1,//
    NSControlKeyMask = 262144,//
    NSCriticalAlertStyle = 2,//
    NSCurveToBezierPathElement = 2,//
    NSDeleteCharacter = 127,//
    NSDeviceIndependentModifierFlagsMask = 4294901760L,//
    NSDocModalWindowMask = 64,//
    NSDragOperationCopy = 1,//
    NSDragOperationDelete = 32,//
    NSDragOperationEvery = -1L,//
    NSDragOperationLink = 2,//
    NSDragOperationMove = 16,//
    NSDragOperationNone = 0,//
    NSEnterCharacter = 3,//
    NSEvenOddWindingRule = 1,//
    NSEventPhaseBegan = 1,//
    NSEventPhaseCancelled = 16,//
    NSEventPhaseEnded = 8,//
    NSEventTypeBeginGesture = 19,//
    NSEventTypeEndGesture = 20,//
    NSEventTypeGesture = 29,//
    NSEventTypeMagnify = 30,//
    NSEventTypeRotate = 18,//
    NSEventTypeSwipe = 31,//
    NSFileHandlingPanelOKButton = 1,//
    NSFlagsChanged = 12,//
    NSFocusRingTypeNone = 1,//
    NSFocusRingTypeExterior = 2,//
    NSFontPanelAllEffectsModeMask = 1048320,//
    NSFontPanelAllModesMask = -1,//
    NSFullScreenWindowMask = 16384,//
    NSHelpFunctionKey = 63302,//
    NSHelpKeyMask = 4194304,//
    NSHourMinuteDatePickerElementFlag = 12,//
    NSHourMinuteSecondDatePickerElementFlag = 14,//
    NSImageAbove = 5,//
    NSImageAlignCenter = 0,//
    NSImageAlignLeft = 4,//
    NSImageAlignRight = 8,//
    NSImageCacheNever = 3,//
    NSImageInterpolationDefault = 0,//
    NSImageInterpolationHigh = 3,//
    NSImageInterpolationLow = 2,//
    NSImageInterpolationNone = 1,//
    NSImageLeft = 2,//
    NSImageOnly = 1,//
    NSImageOverlaps = 6,//
    NSInformationalAlertStyle = 1,//
    NSItalicFontMask = 1,//
    NSKeyDown = 10,//
    NSKeyUp = 11,//
    NSLandscapeOrientation = 1,//
    NSLeftMouseDown = 1,//
    NSLeftMouseDragged = 6,//
    NSLeftMouseDraggedMask = 64,//
    NSLeftMouseUp = 2,//
    NSLeftMouseUpMask = 4,//
    NSLeftTabStopType = 0,//
    NSLineBreakByClipping = 2,//
    NSLineBreakByTruncatingMiddle = 5,//
    NSLineBreakByTruncatingTail = 4,//
    NSLineBreakByWordWrapping = 0,//
    NSLineToBezierPathElement = 1,//
    NSMiniControlSize = 2,//
    NSMiniaturizableWindowMask = 4,//
    NSMiterLineJoinStyle = 0,//
    NSMixedState = -1,//
    NSMomentaryLightButton = 0,//
    NSMouseEntered = 8,//
    NSMouseExited = 9,//
    NSMouseMoved = 5,//
    NSMoveToBezierPathElement = 0,//
    NSNewlineCharacter = 10,//
    NSNoBorder = 0,//
    NSNoImage = 0,//
    NSNoTitle = 0,//
    NSNonZeroWindingRule = 0,//
    NSNonactivatingPanelMask = 128,//
    NSOffState = 0,//
    NSOnState = 1,//
    NSOpenGLCPSurfaceOrder = 235,//
    NSOpenGLPFAAccumSize = 14,//
    NSOpenGLPFAAlphaSize = 11,//
    NSOpenGLPFAColorSize = 8,//
    NSOpenGLPFADepthSize = 12,//
    NSOpenGLPFADoubleBuffer = 5,//
    NSOpenGLPFASampleBuffers = 55,//
    NSOpenGLPFASamples = 56,//
    NSOpenGLPFAStencilSize = 13,//
    NSOpenGLPFAStereo = 6,//
    NSOtherMouseDown = 25,//
    NSOtherMouseDragged = 27,//
    NSOtherMouseUp = 26,//
    NSPageDownFunctionKey = 63277,//
    NSPageUpFunctionKey = 63276,//
    NSPortraitOrientation = 0,//
    NSPrintPanelShowsPageSetupAccessory = 256,//
    NSPrintPanelShowsPrintSelection = 32,//
    NSProgressIndicatorPreferredThickness = 14,//
    NSPushOnPushOffButton = 1,//
    NSRGBColorSpaceModel = 1,//
    NSRadioButton = 4,//
    NSRegularControlSize = 0,//
    NSRegularSquareBezelStyle = 2,//
    NSResizableWindowMask = 8,//
    NSRightMouseDown = 3,//
    NSRightMouseDragged = 7,//
    NSRightMouseUp = 4,//
    NSRoundLineCapStyle = 1,//
    NSRoundLineJoinStyle = 1,//
    NSRoundedBezelStyle = 1,//
    NSRoundedDisclosureBezelStyle = 14,//
    NSScaleNone = 2,//
    NSScrollElasticityNone = 1,//
    NSScrollWheel = 22,//
    NSScrollerDecrementLine = 4,//
    NSScrollerDecrementPage = 1,//
    NSScrollerIncrementLine = 5,//
    NSScrollerIncrementPage = 3,//
    NSScrollerKnob = 2,//
    NSScrollerKnobSlot = 6,//
    NSScrollerStyleLegacy = 0,//
    NSScrollerStyleOverlay = 1,//
    NSShadowlessSquareBezelStyle = 6,//
    NSShiftKeyMask = 131072,//
    NSSmallControlSize = 1,//
    NSSquareLineCapStyle = 2,//
    NSStatusWindowLevel = 25,//
    NSStringDrawingUsesLineFragmentOrigin = 1,//
    NSSubmenuWindowLevel = 3,//
    NSSwitchButton = 3,//
    NSSystemDefined = 14,//
    NSTabCharacter = 9,//
    NSTableColumnNoResizing = 0,//
    NSTableColumnUserResizingMask = 2,//
    NSTableViewDropAbove = 1,//
    NSTableViewDropOn = 0,//
    NSTableViewGridNone = 0,//
    NSTableViewNoColumnAutoresizing = 0,//
    NSTableViewSolidVerticalGridLineMask = 1,//
    NSTerminateCancel = 0,//
    NSTerminateNow = 1,//
    NSTextAlignmentCenter = 2,//
    NSTextAlignmentJustified = 3,//
    NSTextAlignmentLeft = 0,//
    NSTextAlignmentRight = 1,//
    NSTextFieldAndStepperDatePickerStyle = 0,//
    NSTextFieldDatePickerStyle = 2,//
    NSTitledWindowMask = 1,//
    NSToolbarDisplayModeIconOnly = 2,//
    NSTouchPhaseAny = -1L,//
    NSTouchPhaseBegan = 1,//
    NSTouchPhaseCancelled = 16,//
    NSTouchPhaseEnded = 8,//
    NSTouchPhaseMoved = 2,//
    NSTouchPhaseStationary = 4,//
    NSUnderlineStyleDouble = 9,//
    NSUnderlineStyleNone = 0,//
    NSUnderlineStyleSingle = 1,//
    NSUnderlineStyleThick = 2,//
    NSUtilityWindowMask = 16,//
    NSViewHeightSizable = 16,//
    NSViewMaxXMargin = 4,//
    NSViewMaxYMargin = 32,//
    NSViewMinXMargin = 1,//
    NSViewMinYMargin = 8,//
    NSViewWidthSizable = 2,//
    NSWarningAlertStyle = 0,//
    NSWindowAbove = 1,//
    NSWindowBelow = -1,//
    NSWindowCollectionBehaviorFullScreenAuxiliary = 256,//
    NSWindowCollectionBehaviorFullScreenPrimary = 128,//
    NSWindowCollectionBehaviorMoveToActiveSpace = 2,//
    NSWritingDirectionLeftToRight = 0,//
    NSWritingDirectionRightToLeft = 1,//
    NSYearMonthDatePickerElementFlag = 192,//
    NSYearMonthDayDatePickerElementFlag = 224,//
    kCFRunLoopBeforeWaiting = 32,//
    kCFStringEncodingUTF8 = 134217984,//
    kCGBlendModeDifference = 10,//
    kCGBlendModeNormal = 0,//
    kCGEventLeftMouseDown = 1,//
    kCGEventLeftMouseUp = 2,//
    kCGEventMouseMoved = 5,//
    kCGEventOtherMouseDown = 25,//
    kCGEventOtherMouseUp = 26,//
    kCGEventRightMouseDown = 3,//
    kCGEventRightMouseUp = 4,//
    kCGEventSourceStateHIDSystemState = 1,//
    kCGHIDEventTap = 0,//
    kCGImageAlphaFirst = 4,//
    kCGImageAlphaLast = 3,//
    kCGImageAlphaNoneSkipFirst = 6,//
    kCGImageAlphaNoneSkipLast = 5,//
    kCGImageAlphaOnly = 7,//
    kCGKeyboardEventKeyboardType = 10,//
    kCGLineCapButt = 0,//
    kCGLineCapRound = 1,//
    kCGLineCapSquare = 2,//
    kCGLineJoinBevel = 2,//
    kCGLineJoinMiter = 0,//
    kCGLineJoinRound = 1,//
    kCGPathElementAddCurveToPoint = 3,//
    kCGPathElementAddLineToPoint = 1,//
    kCGPathElementAddQuadCurveToPoint = 2,//
    kCGPathElementCloseSubpath = 4,//
    kCGPathElementMoveToPoint = 0,//
    kCGScrollEventUnitLine = 1,//
    kCTFontManagerScopeProcess = 1,//
    NSAllApplicationsDirectory = 100,//
    NSAllDomainsMask = 65535,//
    NSOrderedSame = 0,//
    NSURLCredentialPersistenceForSession = 1,//
    NSURLErrorBadURL = -1000,//
    NSURLErrorSecureConnectionFailed = -1200,//
    NSURLErrorServerCertificateNotYetValid = -1204,//
    NSURLRequestReloadIgnoringLocalCacheData = 1,//
    NSUTF8StringEncoding = 4,//
};
#if defined(__LP64__) && __LP64__
typedef double CGFloat;
#else
typedef float CGFloat;
#endif
typedef struct _NSPoint {
    CGFloat x;
    CGFloat y;
} NSPoint;
typedef struct _NSSize {
    CGFloat width;        /* should never be negative */
    CGFloat height;        /* should never be negative */
} NSSize;
typedef struct _NSRect {
    NSPoint origin;
    NSSize size;
} NSRect;
#endif
/*
 * NSObject
 */
#define NSOBJECT(x) ((NSObject*)x)
Class NSObject_class(NSObject* object);
void NSObject_retain(NSObject* object);
void NSObject_release(NSObject* object);
void NSObject_autorelease(NSObject* object);
NSUInteger NSObject_retainCount(NSObject* object);
/*
 *  NSString
 */
NSString* NSString_stringWithUTF8String(const char* nullTerminatedCString);
NSString* NSString_stringWithUTF8(const char* str,size_t length);
NSString* NSString_initWithBytes(const void *bytes,NSUInteger length, int encoding);
NSString* NSString_initWithBytes_utf8(const void *bytes,NSUInteger length);
/*
 *  NSArray
 */
int NSArray_containsObject(NSArray *array,NSObject* anObject);
NSUInteger NSArray_count(NSArray *array);
NSUInteger NSArray_indexOfObjectIdenticalTo(NSArray *array,NSObject* anObject);
NSObject* NSArray_objectAtIndex(NSArray *array,NSUInteger index);
#endif
#endif /* SRC_COCOA_GRAPHICS_CORE_H_ */

