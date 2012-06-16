#pragma once

#include <xwl/xwl.h>

#ifdef __cplusplus
extern "C" {
#endif

void xwl_osx_startup( void );
void xwl_osx_shutdown( void );
void xwl_osx_activate( xwl_window_t * window );

xwl_window_handle_t *xwl_create_osx_window( xwl_windowparams_t * params, const char * title );
int xwl_pollevent_osx( xwl_event_t * event );
void xwl_setup_osx_rendering( xwl_window_t * window, u32 * attribs );
void xwl_osx_finish( xwl_window_t * window );
void *xwl_osx_rendering_context( xwl_window_t * window );

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
{}
@end

#endif

#ifdef __cplusplus
}; // extern "C"
#endif