#pragma once


#include <xwl/platforms/osx.h>

#if __OBJC__
#import <Cocoa/Cocoa.h>

@class xwlOpenGLView;

@interface xwlWindow : NSWindow
{
@public
	xwl_native_window_t * xwlhandle;
	xwlOpenGLView * render;
}

@property (nonatomic) xwl_native_window_t *xwlhandle;
@property (nonatomic, retain) xwlOpenGLView *render;

-(BOOL) canBecomeKeyWindow;
-(BOOL) canBecomeMainWindow;

@end



@interface xwlDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@end

#endif
