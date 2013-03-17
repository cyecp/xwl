#include <xwl/platforms/osx/appdelegate.h>
#import <xwl/platforms/osx/xwlwindow.h>


// the implementation for xwlDelegate is provided by default and takes over as application delegate.
@implementation xwlDelegate

-(void) noResponderFor: (SEL)eventSelector
{
	NSLog( @"xwlDelegate: noResponderFor event!" );
	//[super noResponderFor:eventSelector];
}

-(void) windowWillMiniaturize:(NSNotification*)notification
{
//	NSLog( @"Window will miniaturize" );
}

-(void) windowDidMiniaturize:(NSNotification*)notification
{
	//NSLog( @"Window did miniaturize!" );
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
	//NSLog( @"Window Did Deminiaturize!" );
}

-(void)windowWillClose:(NSNotification *)notification
{
	NSLog( @"windowWillClose" );
}


-(BOOL) windowShouldClose:(id)windowIn
{
	xwl_event_t ev = {0};
	xwlWindow * wnd = (xwlWindow*)windowIn;
	ev.window = &wnd.xwlhandle->handle;
	ev.type = XWLE_CLOSED;
	xwl_send_event( &ev );
	return YES;
}
-(void) windowDidResignKey:(NSNotification*)notification
{
	xwl_event_t ev = {0};
	xwlWindow * wnd = (xwlWindow*)[notification object];
	ev.window = &wnd.xwlhandle->handle;
	ev.type = XWLE_LOSTFOCUS;
	xwl_send_event( &ev );
}

-(void) windowDidBecomeKey:(NSNotification*)notification
{
	xwl_event_t ev = {0};
	xwlWindow * wnd = (xwlWindow*)[notification object];
	ev.window = &wnd.xwlhandle->handle;
	ev.type = XWLE_GAINFOCUS;
	xwl_send_event( &ev );
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