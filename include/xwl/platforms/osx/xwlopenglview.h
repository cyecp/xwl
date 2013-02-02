#pragma once


#include <xwl/platforms/osx.h>

#if __OBJC__
#import <Cocoa/Cocoa.h>
@interface xwlOpenGLView : NSView
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
#endif
