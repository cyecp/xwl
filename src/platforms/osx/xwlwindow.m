#import <Cocoa/Cocoa.h>
#import <xwl/platforms/osx/xwlwindow.h>

@implementation xwlWindow
@synthesize xwlhandle;

-(void) dealloc
{
	[super dealloc];
}

/* this is required when using the styleMask: NSBorderlessWindowMask */
/* By default NSBorderlessWindowMask windows report that they cannot become the key (target of keyboard input) */
-(BOOL) canBecomeKeyWindow
{
	return YES;
}

/* By default NSBorderlessWindowMask windows report that they cannot become the main window without this override */
-(BOOL) canBecomeMainWindow
{
	return YES;
}
@end