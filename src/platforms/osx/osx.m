#import <xwl/platforms/osx.h>
#import <xwl/platforms/osx/appdelegate.h>

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
	
	NSString * applicationName = [[[NSString alloc] retain] initWithCString: title encoding:NSUTF8StringEncoding];
	populateApplicationMenu( submenu, applicationName );
	[applicationName release];
	[mainMenu setSubmenu: submenu forItem: menuItem];
	//NSLog( @"Controller: %@", [window windowController] );
	
	//[JJMenuPopulator populateMainMenu];
	
	[NSApp setMainMenu: mainMenu];
	//[[NSDocumentController sharedDocumentController] noteNewRecentDocumentURL:[NSURL fileURLWithPath:@"/Developer/About Xcode Tools.pdf"]];
}

void cocoa_startup( xwl_api_provider_t * api )
{
	NSLog( @"cocoa startup" );
	
	pool = [[NSAutoreleasePool alloc] init];
	appDelegate = [[xwlDelegate alloc] init];
	
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
}


void cocoa_shutdown( void )
{
	NSLog( @"cocoa shutdown" );
	[pool release];
	[application setDelegate: nil ];
	
	[appDelegate release];
	appDelegate = 0;
	
	[application release];
}

xwl_window_t cocoa_create_window( const char * utf8_title, unsigned int * attribs )
{
	xwl_window_t handle;
	handle.handle = 0;
	NSWindow * window;
	NSRect frame;
	NSPoint origin;
	int windowMask;
	NSRect mainScreenFrame;
	
	
	CGFloat window_width, window_height;
	
	// translate all attribs into a single array we can check uniformly.
	unsigned int attributes[ XWL_ATTRIBUTE_COUNT * 2 ] = {0};
	
//	_xwl_translate_attributes( attribs, attributes );
	
	int current_attrib = -1;
	for( unsigned int i = 0; *attribs && i < XWL_ATTRIBUTE_COUNT; ++i )
	{
		if ( current_attrib == -1 )
		{
			current_attrib = *attribs;
		}
		else
		{
			attributes[ current_attrib ] = *attribs;
			current_attrib = -1;
		}
		++attribs;
	}
	
	
	
	
	window_width = attributes[ XWL_WINDOW_WIDTH ];
	window_height = attributes[ XWL_WINDOW_HEIGHT ];
	
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
	
	frame = NSMakeRect( 0, 0, window_width, window_height );
	
	// get this screen's dimensions
	mainScreenFrame = [[NSScreen mainScreen] frame];
	NSLog( @"[xwl] screen size: %g %g", mainScreenFrame.size.width, mainScreenFrame.size.height );
	
	window = [[[xwlWindow alloc] initWithContentRect: frame styleMask: windowMask backing:NSBackingStoreBuffered defer:NO] autorelease];
	if ( !window )
	{
		xwl_set_error( "Unable to create cocoa window!" );
		handle.handle = 0;
		return handle;
	}
	
	// uncomment the next line to see a red color when the window is initially created
	//[handle setBackgroundColor: [NSColor redColor]];
	[window setBackgroundColor: [NSColor blackColor]];
	[window makeKeyAndOrderFront: nil];
	[window makeMainWindow];
	[window makeKeyWindow];
	[window orderFront: nil];
	
	[window setTitle: [NSString stringWithUTF8String: utf8_title] ];
	[window setAcceptsMouseMovedEvents: YES];
	[window setReleasedWhenClosed: NO];
	
	[window setDelegate: appDelegate ];
	
	// try to center the window
	origin = NSMakePoint( (mainScreenFrame.size.width/2) - (window_width/2), (mainScreenFrame.size.height/2) - (window_height/2) );
	
	[window center];
	[window setFrameOrigin: origin];
	
	if ( attributes[ XWL_USE_FULLSCREEN ] )
	{
		[window setLevel: CGShieldingWindowLevel() ];
	}
	
	
#if 0

	// stuff this window into an unused handle
	if ( handle )
	{
		wh = xwl_get_unused_window();
		wh->handle.handle = handle;
		handle.xwlhandle = wh;
	}
#endif
	
	return handle;
}

void cocoa_destroy_window( xwl_window_t handle )
{
	
}


void cocoa_register( xwl_window_provider_t * wapi )
{
    wapi->startup = cocoa_startup;
    wapi->shutdown = cocoa_shutdown;
	
	wapi->create_window = cocoa_create_window;
	wapi->destroy_window = cocoa_destroy_window;
}