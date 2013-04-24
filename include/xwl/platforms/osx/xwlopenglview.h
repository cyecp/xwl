#pragma once


#include <xwl/platforms/osx/osx.h>

#if __OBJC__
#import <Cocoa/Cocoa.h>
@interface xwlOpenGLView : NSView
{
@public
	NSOpenGLContext * _context;
//	CVDisplayLinkRef display_link;
	int current_virtual_screen;
}

@property (nonatomic, retain) NSOpenGLContext * context;

-(id)initWithFrame:(NSRect)frameRect;
-(void)dealloc;
-(BOOL) isOpaque;
@end
#endif
