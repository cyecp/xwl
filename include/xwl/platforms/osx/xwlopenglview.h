#pragma once


#include <xwl/platforms/osx/osx.h>

#if __OBJC__
#import <Cocoa/Cocoa.h>
@interface xwlOpenGLView : NSView
{
@public
	NSOpenGLContext * _context;
}

@property (nonatomic, retain) NSOpenGLContext * context;

-(id)initWithFrame:(NSRect)frameRect;
-(void)dealloc;
-(BOOL) isOpaque;
@end
#endif
