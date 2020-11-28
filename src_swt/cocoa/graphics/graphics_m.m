#include "core.h"
void NSCursor_get_systems(NSCursor** cursors){
    cursors[W_CURSOR_HAND] = [NSCursor pointingHandCursor];
    cursors[W_CURSOR_ARROW] = [NSCursor arrowCursor];
 //   cursors[W_CURSOR_WAIT] = [NSCursor busyButClickableCursor];
    cursors[W_CURSOR_CROSS] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_APPSTARTING] = [NSCursor arrowCursor];
    cursors[W_CURSOR_HELP] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZEALL] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZENESW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZENS] = [NSCursor resizeUpDownCursor];
    cursors[W_CURSOR_SIZENWSE] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZEWE] = [NSCursor resizeLeftRightCursor];
    cursors[W_CURSOR_SIZEN] = [NSCursor resizeUpCursor];
    cursors[W_CURSOR_SIZES] = [NSCursor resizeDownCursor];
    cursors[W_CURSOR_SIZEE] = [NSCursor resizeRightCursor];
    cursors[W_CURSOR_SIZEW] = [NSCursor resizeLeftCursor];
    cursors[W_CURSOR_SIZENE] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZESE] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZESW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_SIZENW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_UPARROW] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_IBEAM] = [NSCursor crosshairCursor];
    cursors[W_CURSOR_NO] = [NSCursor operationNotAllowedCursor];
}
void NSCursor_set(NSCursor* cursor){
    [cursor set];
}
