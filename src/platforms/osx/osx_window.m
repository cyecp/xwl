#import <xwl/platforms/osx/osx.h>
#import <xwl/platforms/osx/appdelegate.h>
#import <xwl/platforms/osx/xwlwindow.h>

NSApplication * application = 0;
NSAutoreleasePool *pool = 0;
xwlDelegate * appDelegate = 0;

void populateApplicationMenu( NSMenu * aMenu, NSString * applicationName )
{
	NSMenuItem * menuItem;
	
	menuItem = [aMenu addItemWithTitle:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"About", nil), applicationName]
								action:@selector(orderFrontStandardAboutPanel:)
						 keyEquivalent:@""];
	[menuItem setTarget:NSApp];
	
	[aMenu addItem:[NSMenuItem separatorItem]];
	
	menuItem = [aMenu addItemWithTitle:NSLocalizedString(@"Preferences...", nil)
								action:NULL
						 keyEquivalent:@","];
	
	[aMenu addItem:[NSMenuItem separatorItem]];
	
	menuItem = [aMenu addItemWithTitle:NSLocalizedString(@"Services", nil)
								action:NULL
						 keyEquivalent:@""];
	NSMenu * servicesMenu = [[NSMenu alloc] initWithTitle:@"Services"];
	[aMenu setSubmenu:servicesMenu forItem:menuItem];
	[NSApp setServicesMenu:servicesMenu];
	
	[aMenu addItem:[NSMenuItem separatorItem]];
	
	menuItem = [aMenu addItemWithTitle:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"Hide", nil), applicationName]
								action:@selector(hide:)
						 keyEquivalent:@"h"];
	[menuItem setTarget:NSApp];
	
	menuItem = [aMenu addItemWithTitle:NSLocalizedString(@"Hide Others", nil)
								action:@selector(hideOtherApplications:)
						 keyEquivalent:@"h"];
	[menuItem setKeyEquivalentModifierMask:NSCommandKeyMask | NSAlternateKeyMask];
	[menuItem setTarget:NSApp];
	
	menuItem = [aMenu addItemWithTitle:NSLocalizedString(@"Show All", nil)
								action:@selector(unhideAllApplications:)
						 keyEquivalent:@""];
	[menuItem setTarget:NSApp];
	
	[aMenu addItem:[NSMenuItem separatorItem]];
	
	menuItem = [aMenu addItemWithTitle:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"Quit", nil), applicationName]
								action:@selector(terminate:) keyEquivalent:@"q"];
	[menuItem setTarget:NSApp];
}

void attachMenu( const char * title )
{
	//NSLog( @"Application will finish launching?" );
	
	NSMenu * mainMenu = [[NSMenu alloc] initWithTitle: NSLocalizedString(@"MainMenu",nil)];
	NSMenuItem * menuItem;
	NSMenu * submenu;
	
	menuItem = [mainMenu addItemWithTitle: NSLocalizedString(@"Apple", nil) action: nil keyEquivalent:@""];
	submenu = [[NSMenu alloc] initWithTitle:NSLocalizedString(@"Apple",nil)];
	[NSApp performSelector:@selector(setAppleMenu:) withObject: submenu];
	// populate application menu
	
	NSString * applicationName = [[[NSString alloc] autorelease] initWithCString: title encoding:NSUTF8StringEncoding];
	populateApplicationMenu( submenu, applicationName );
	[applicationName release];
	[mainMenu setSubmenu: submenu forItem: menuItem];
	
	[NSApp setMainMenu: mainMenu];
	//[[NSDocumentController sharedDocumentController] noteNewRecentDocumentURL:[NSURL fileURLWithPath:@"/Developer/About Xcode Tools.pdf"]];
}

int cocoa_startup( xwl_api_provider_t * api )
{
	pool = [[NSAutoreleasePool alloc] init];
	appDelegate = [[[xwlDelegate alloc] init] autorelease];
	
	// straight from SFML-2.0
	ProcessSerialNumber psn;
	// Set the process as a normal application so it can get focus.
	
	if (!GetCurrentProcess(&psn)) {
		TransformProcessType(&psn, kProcessTransformToForegroundApplication);
		SetFrontProcess(&psn);
	}
	
	// Tell the application to stop bouncing in the Dock.
	//[[NSApplication sharedApplication] finishLaunching];
	// NOTE : This last call won't harm anything even if SFML window was
	// created with an external handle.
	// NOTE2: Removing this call in OSX 10.7 fixes "_createMenuRef called with existing principal MenuRef" Unhandled exception.
	
	
	application = [NSApplication sharedApplication];
	/*
	 float scaleFactor = 0;
	 NSWindow * window = [application mainWindow];
	 NSScreen * screen = [window screen];
	 if ( [screen respondsToSelector:@selector(backingScaleFactor)] )
	 {
	 scaleFactor = [screen backingScaleFactor];
	 }
	 
	 if ( scaleFactor < 1 )
	 scaleFactor = 1;
	 
	 NSLog( @"backingScaleFactor for main screen is %g\n", scaleFactor );
	 */
	
	// set application handler here
	[application setDelegate: appDelegate];
	
	//[NSApp activateIgnoringOtherApps:YES];
	
	// setup the app menu
	[application finishLaunching];
	
	return 1;
} // cocoa_startup


void cocoa_shutdown( void )
{
	[application setDelegate: nil ];
	
	[pool release];
}

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
	
	// setup a basic menu
	attachMenu( utf8_title );
	
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

void cocoa_get_screen_size( unsigned int screen_index, int * width, int * height )
{
	NSScreen * screen = [[ NSScreen screens] objectAtIndex: screen_index];
	NSRect frame = [screen frame];
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
	wapi->get_screen_size = cocoa_get_screen_size;
	wapi->get_screen_count = cocoa_get_screen_count;
} // cocoa_register