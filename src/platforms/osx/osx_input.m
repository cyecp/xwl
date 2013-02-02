#import <xwl/platforms/osx/osx.h>
#import <Cocoa/Cocoa.h>

int cocoa_input_startup( void )
{
	return 1;
} // cocoa_input_startup


void cocoa_input_shutdown( void )
{
} // cocoa_input_shutdown

int cocoa_input_dispatch_events( void )
{
	NSAutoreleasePool * event_pool = [[NSAutoreleasePool alloc] init];
	NSEvent * event = [NSApp nextEventMatchingMask:NSAnyEventMask
										 untilDate: [NSDate distantPast]
											inMode: NSDefaultRunLoopMode
										   dequeue: YES];
	if ( event != nil )
	{
		// dispatch the event!
		[NSApp sendEvent: event ];
		[event_pool release];
		return 1;
	}

	[event_pool release];
	return 0;
} // cocoa_input_dispatch_events

void cocoa_input_register( xwl_input_provider_t * input )
{
	input->startup = cocoa_input_startup;
	input->shutdown = cocoa_input_shutdown;
	input->dispatch_events = cocoa_input_dispatch_events;
} // cocoa_input_register