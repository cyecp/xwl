#pragma once


#include <xwl/platforms/osx/osx.h>


@interface xwlWindow : NSWindow
{
@public
	xwl_native_window_t * xwlhandle;
}

@property (nonatomic) xwl_native_window_t *xwlhandle;

-(BOOL) canBecomeKeyWindow;
-(BOOL) canBecomeMainWindow;

@end
