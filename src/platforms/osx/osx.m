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

void cocoa_wp_startup( xwl_api_provider_t * api )
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


void cocoa_wp_shutdown( void )
{
	NSLog( @"cocoa shutdown" );
	[pool release];
	[application setDelegate: nil ];
	
	[appDelegate release];
	appDelegate = 0;
	
	[application release];
}


void cocoa_wp_register( xwl_window_provider_t * wapi )
{
    wapi->startup = cocoa_wp_startup;
    wapi->shutdown = cocoa_wp_shutdown;
}