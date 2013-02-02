#pragma once


#include <xwl/platforms/osx.h>

#if __OBJC__
#import <Cocoa/Cocoa.h>
@interface MyOpenGLView : NSView
{
@public
	NSOpenGLContext * ctx;
}

@property (nonatomic, retain) NSOpenGLContext * context;

-(id)initWithFrame:(NSRect)frameRect;
-(void)dealloc;
-(NSOpenGLContext*) getContext;
-(void)setContext:(NSOpenGLContext*)context;
-(BOOL) isOpaque;
@end




@interface xwlWindow : NSWindow
{
@public
	xwl_window_handle_t * xwlhandle;
	MyOpenGLView * render;
}

@property (nonatomic) xwl_window_handle_t *xwlhandle;
@property (nonatomic, retain) MyOpenGLView *render;

-(BOOL) canBecomeKeyWindow;
-(BOOL) canBecomeMainWindow;

@end



@interface xwlDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>

@end

#endif
