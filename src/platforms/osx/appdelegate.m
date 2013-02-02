#include <xwl/platforms/osx/appdelegate.h>



// the implementation for xwlDelegate is provided by default and takes over as application delegate.
@implementation xwlDelegate

-(void)keyDown:(NSEvent*)event
{
	//NSLog( @"xwlDelegate keyDown" );
}

-(void)keyUp:(NSEvent*)event
{
	//NSLog( @"xwlDelegate keyUp" );
}
-(void) noResponderFor: (SEL)eventSelector
{
	NSLog( @"xwlDelegate: noResponderFor event!" );
	//[super noResponderFor:eventSelector];
}

-(void) windowWillMiniaturize:(NSNotification*)notification
{
	//NSLog( @"Window will miniaturize" );
}

-(void) windowDidMiniaturize:(NSNotification*)notification
{
	//NSLog( @"Window did miniaturize!" );
}

-(BOOL) windowShouldClose:(id)windowIn
{
#if 0
	xwl_event_t ev = {0};
	//XWLWINDOW * wnd = (XWLWINDOW*)[notification object];
	//ev.window = &wnd.xwlhandle->handle;
	ev.type = XWLE_CLOSED;
	
	xwl_send_event( &ev );
#endif
	return YES;
}
-(void) windowDidResignKey:(NSNotification*)notification
{
#if 0
	xwl_event_t ev = {0};
	XWLWINDOW * wnd = (XWLWINDOW*)[notification object];
	ev.window = &wnd.xwlhandle->handle;
	ev.type = XWLE_LOSTFOCUS;
	xwl_send_event( &ev );
#endif
	//NSLog( @"Window did resign key %@", [notification object] );
}

-(void) windowDidBecomeKey:(NSNotification*)notification
{
#if 0
	xwl_event_t ev = {0};
	XWLWINDOW * wnd = (XWLWINDOW*)[notification object];
	ev.window = &wnd.xwlhandle->handle;
	ev.type = XWLE_GAINFOCUS;
	xwl_send_event( &ev );
#endif
	//NSLog( @"Window did become key %@", [notification object] );
}

-(void)windowDidBecomeMain:(NSNotification*)notification
{
	//NSLog( @"windowDidBecomeMain %@", [notification object] );
}

-(void)windowDidResignMain:(NSNotification*)notification
{
	//NSLog( @"windowDidResignMain %@", [notification object] );
}

-(void) windowWillClose
{
	//NSLog( @"Window will close!" );
}



- (void)windowDidEndSheet:(NSNotification *)notification
{
	//NSLog( @"Window did end sheet!" );
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
	//NSLog( @"Window Did Deminiaturize!" );
}


-(NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *)sender;
{
	//	NSLog( @"ApplicationShouldTerminate" );
	return YES;
}

-(void) applicationWillFinishLaunching:(NSNotification *)aNotification
{
	
}

-(void) applicationDidFinishLaunching:(NSNotification*)notification
{
	//	NSLog( @"Application finished launching?" );
}
@end



@implementation xwlWindow
@synthesize xwlhandle;
@synthesize render;

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