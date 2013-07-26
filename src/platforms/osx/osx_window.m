#import <xwl/platforms/osx/osx.h>
#import <xwl/platforms/osx/appdelegate.h>
#import <xwl/platforms/osx/xwlwindow.h>

NSApplication * application = 0;
NSAutoreleasePool *pool = 0;
xwlDelegate * appDelegate = 0;

int cocoa_startup( xwl_api_provider_t * api )
{
	pool = [[NSAutoreleasePool alloc] init];	
	return 1;
} // cocoa_startup


void cocoa_shutdown( void )
{
	[pool release];
} // cocoa_shutdown

void *cocoa_create_window( xwl_native_window_t * wh, const char * utf8_title, unsigned int * attributes, int pixel_format )
{
	xwlWindow *window;
	NSRect frame;
	NSPoint origin;
	int windowMask;
	NSRect mainScreenFrame;	
	CGFloat window_width, window_height;
	CGFloat window_x = 0, window_y = 0;
	
	window_width = attributes[ XWL_WINDOW_WIDTH ];
	window_height = attributes[ XWL_WINDOW_HEIGHT ];

	if ( attributes[ XWL_WINDOW_X ] != XWL_NOTSET )
	{
		window_x = attributes[ XWL_WINDOW_X ];
	}
	if ( attributes[ XWL_WINDOW_Y ] != XWL_NOTSET )
	{
		window_y = attributes[ XWL_WINDOW_Y ];
	}
	
	// is this window using fullscreen?
	if ( attributes[ XWL_USE_FULLSCREEN ] )
	{
		windowMask = NSBorderlessWindowMask;
	}
	else
	{
		windowMask = NSTitledWindowMask | NSClosableWindowMask | NSMiniaturizableWindowMask | NSResizableWindowMask;
	}
	
	if ( attributes[ XWL_DISABLE_RESIZE ] )
	{
		// remove this flag
		windowMask &= ~NSResizableWindowMask;
	}
	
	frame = NSMakeRect( window_x, window_y, window_width, window_height );
	
	// get this screen's dimensions
	mainScreenFrame = [[NSScreen mainScreen] frame];
//	NSLog( @"[xwl] screen size: %g %g", mainScreenFrame.size.width, mainScreenFrame.size.height );
	
	window = [[xwlWindow alloc] initWithContentRect: frame styleMask: windowMask backing:NSBackingStoreBuffered defer:NO];
	if ( !window )
	{
		xwl_set_error( "Unable to create cocoa window!" );
		return 0;
	}

	//NSLog( @"created window on thread: %@", [NSThread currentThread]);
	
	// uncomment the next line to see a red color when the window is initially created
	//[handle setBackgroundColor: [NSColor redColor]];
	[window setBackgroundColor: [NSColor blackColor]];
	[window makeKeyAndOrderFront: nil];

	[window setTitle: [NSString stringWithUTF8String: utf8_title] ];
	[window setAcceptsMouseMovedEvents: YES];
	[window setReleasedWhenClosed: YES];
	
	[window setDelegate: appDelegate ];

	// try to center the window
	if ( attributes[ XWL_WINDOW_X ] == XWL_NOTSET && attributes[ XWL_WINDOW_Y ] == XWL_NOTSET )
	{
		origin = NSMakePoint( (mainScreenFrame.size.width/2) - (window_width/2), (mainScreenFrame.size.height/2) - (window_height/2) );
		[window center];
		[window setFrameOrigin: origin];
	}
	
	if ( attributes[ XWL_USE_FULLSCREEN ] )
	{
		[window setLevel: CGShieldingWindowLevel() ];
	}
	
	// keep track of the internal handle	
	window.xwlhandle = wh;
	[window retain];
	return window;
} // cocoa_create_window

void cocoa_destroy_window( xwl_window_t * handle )
{
} // cocoa_destroy_window

void cocoa_get_window_size( xwl_window_t * window, int * width, int * height )
{
	xwlWindow * xwlwindow = (xwlWindow*)window->handle;
	NSRect frame = [[xwlwindow contentView] bounds];
	*width = frame.size.width;
	*height = frame.size.height;
} // cocoa_get_window_size

void cocoa_get_window_render_size( xwl_window_t * window, int * width, int * height )
{
	xwlWindow * xwlwindow = (xwlWindow*)window->handle;
	NSRect render_frame = [[xwlwindow contentView] convertRectToBacking:[[xwlwindow contentView] bounds]];
	*width = render_frame.size.width;
	*height = render_frame.size.height;
} // cocoa_get_window_render_size

void cocoa_get_screen_size( unsigned int screen_index, int * width, int * height )
{
	NSScreen * screen = [[NSScreen screens] objectAtIndex:screen_index];
	NSRect frame = [screen convertRectToBacking:[screen frame]];
	*width = frame.size.width;
	*height = frame.size.height;
} // cocoa_get_screen_size

unsigned int cocoa_get_screen_count()
{
	return [[NSScreen screens] count];
} // cocoa_get_screen_count

void cocoa_register( xwl_window_provider_t * wapi )
{
    wapi->startup = cocoa_startup;
    wapi->shutdown = cocoa_shutdown;
	
	wapi->create_window = cocoa_create_window;
	wapi->destroy_window = cocoa_destroy_window;
	
	wapi->get_window_size = cocoa_get_window_size;
	wapi->get_window_render_size = cocoa_get_window_render_size;
	wapi->get_screen_size = cocoa_get_screen_size;
	wapi->get_screen_count = cocoa_get_screen_count;
} // cocoa_register