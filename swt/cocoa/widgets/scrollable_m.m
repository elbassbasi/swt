//
//  scrollable_m.m
//  swt
//
//  Created by elbassbasi on 7/29/20.
//
#import "widget.h"
@interface SWTScrollView : NSScrollView{
@public void* widget;
}

@end

@implementation SWTScrollView


@end

SWTScrollView* NSScrollView_new(void* userdata){
    NSRect frame=NSMakeRect(0, 0, 0, 0);
    SWTScrollView* scrollView = [[SWTScrollView alloc] initWithFrame:frame];
    scrollView->widget = userdata;
    return scrollView;
}
w_widget* NSScrollView_getWidget(NSView* view){
    if( [view isKindOfClass:[SWTScrollView class]])
    return ((SWTScrollView*)view)->widget;
    else return 0;
}
int NSScrollView_isKindOfClass(NSView* view){
    return [view isKindOfClass:[NSScrollView class]];
}
void NSScrollView_setDrawsBackground(NSScrollView* scrollView,int draw){
    //[scrollView borderType]
}
void NSScrollView_setHasHorizontalScroller(NSScrollView* scrollView,int scroller){
    [scrollView setHasHorizontalScroller:scroller];
}
void NSScrollView_setHasVerticalScroller(NSScrollView* scrollView,int scroller){
   [scrollView setHasVerticalScroller:scroller];
}
void NSScrollView_setAutohidesScrollers(NSScrollView* scrollView,int autohide){
   [scrollView setAutohidesScrollers:autohide];
}
void NSScrollView_setBorderType(NSScrollView* scrollView,NSBorderType border){
    scrollView.borderType=border;
}
NSView* NSScrollView_contentView(NSScrollView* scrollView){
    return scrollView.contentView;
}
void NSScrollView_setDocumentView(NSScrollView* scrollView,NSView* view){
    [scrollView setDocumentView:view];
}
void NSScrollView_contentSizeForFrameSize(NSScrollView* scrollView,w_size* size,int hFlag,int vFlag,NSBorderType type){
    NSSize fSize = NSMakeSize(size->width, size->height);
    NSSize nSize = [NSScrollView contentSizeForFrameSize:fSize hasHorizontalScroller:hFlag hasVerticalScroller:vFlag borderType:type];
    size->width = nSize.width;
    size->height = nSize.height;
}
NSView* NSScrollView_documentView(NSScrollView* scrollView){
    return [scrollView documentView];
}
