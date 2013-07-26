#include <xwl/platforms/osx/appdelegate.h>
#include <xwl/platforms/osx/xwlopenglview.h>
#import <xwl/platforms/osx/xwlwindow.h>

unsigned int NonLocalizedKeys(unsigned short keycode);
unsigned int LocalizedKeys(unichar ch);

@implementation xwlOpenGLView

@synthesize context = _context;

-(id)initWithFrame:(NSRect)frameRect
{
	self = [super initWithFrame: frameRect];
	
	if ( self == nil )
		return nil;
	
	return self;
}

-(void)lockFocus
{
	[super lockFocus];
	
	if ( [self.context view] != self )
	{
		[self.context setView: self];
	}
	
	[self.context makeCurrentContext];
	
	//NSLog( @"Lock Focus!" );
}

-(void)unlockFocus
{
	//NSLog( @"unlock focus" );
	[super unlockFocus];
}

-(void)dealloc
{
	self.context = nil;
	[super dealloc];
}


-(BOOL) isOpaque
{
	return YES;
}


-(BOOL) acceptsFirstResponder
{
	return YES;
}

-(void) noResponderFor: (SEL)eventSelector
{
	//	NSLog( @"noResponderFor event!" );
	[super noResponderFor:eventSelector];
}

-(void) windowResized:(NSNotification*) notification
{
	xwl_event_t ev = {0};
	xwlWindow * wnd = (xwlWindow*)[notification object];
	ev.type = XWLE_SIZE;
	
	ev.width = [[wnd contentView] frame].size.width;
	ev.height = [[wnd contentView] frame].size.height;
	ev.window = &wnd.xwlhandle->handle;
	
	xwlOpenGLView * view = (xwlOpenGLView*)[wnd contentView];
	[view.context update];
	
	xwl_send_event( &ev );
}

void dispatchMouseMoveEvent(NSEvent * theEvent)
{
	xwl_event_t ev = {0};
	unsigned short titleBarHeight;
	unsigned int fixedHeight;
	xwlWindow * wnd = (xwlWindow*)[theEvent window];
	NSPoint pt = [[theEvent window] mouseLocationOutsideOfEventStream];
	//NSPoint loc = [self convertPoint:[theEvent locationInWindow] fromView: nil];
	//printf( "x: %g, y: %g\n", loc.x, loc.y );
	
	// subtract the window height from the contentView height
	// this will give us the size of the title bar
	titleBarHeight = [wnd frame].size.height - [[wnd contentView] frame].size.height;
	
	fixedHeight = [wnd frame].size.height - titleBarHeight;
	
	ev.type = XWLE_MOUSEMOVE;
	ev.mx = pt.x;
	ev.my = fixedHeight - pt.y; // top left is origin
	ev.window = &wnd.xwlhandle->handle;
	if ( ev.mx >= 0 && ev.my >= 0 && (ev.mx <= [wnd frame].size.width) && (ev.my <= fixedHeight) )
	{
		xwl_send_event( &ev );
	}
}

-(void) mouseMoved:(NSEvent *) theEvent
{
	dispatchMouseMoveEvent( theEvent );
	
	//NSLog(@"mouseMoved");
}

-(void) mouseDown:(NSEvent *) theEvent
{
	xwl_event_t ev = {0};
	ev.type = XWLE_MOUSEBUTTON_PRESSED;
	ev.button = XWLMB_LEFT;
	ev.window = &((xwlWindow*)[theEvent window]).xwlhandle->handle;
	xwl_send_event( &ev );
}

-(void) mouseUp:(NSEvent *) theEvent
{
	xwl_event_t ev = {0};
	ev.type = XWLE_MOUSEBUTTON_RELEASED;
	ev.button = XWLMB_LEFT;
	ev.window = &((xwlWindow*)[theEvent window]).xwlhandle->handle;
	xwl_send_event( &ev );
}

-(void) mouseDragged:(NSEvent *) theEvent
{
	dispatchMouseMoveEvent( theEvent );
	
	//NSLog( @"mouseDragged" );
}


-(void)rightMouseDown:(NSEvent *) theEvent
{
	xwl_event_t ev = {0};
	ev.type = XWLE_MOUSEBUTTON_PRESSED;
	ev.button = XWLMB_RIGHT;
	ev.window = &((xwlWindow*)[theEvent window]).xwlhandle->handle;
	xwl_send_event( &ev );
}

-(void) rightMouseUp:(NSEvent *) theEvent
{
	xwl_event_t ev = {0};
	ev.type = XWLE_MOUSEBUTTON_RELEASED;
	ev.button = XWLMB_RIGHT;
	ev.window = &((xwlWindow*)[theEvent window]).xwlhandle->handle;
	xwl_send_event( &ev );
}

-(void) rightMouseDragged:(NSEvent *) theEvent
{
	dispatchMouseMoveEvent( theEvent );
	//NSLog( @"rightMouseDragged" );
}

-(void) otherMouseDown:(NSEvent *) theEvent
{
	xwl_event_t ev = {0};
	ev.type = XWLE_MOUSEBUTTON_PRESSED;
	ev.button = XWLMB_MIDDLE;
	ev.window = &((xwlWindow*)[theEvent window]).xwlhandle->handle;
	xwl_send_event( &ev );
}

-(void) otherMouseUp:(NSEvent *) theEvent
{
	xwl_event_t ev = {0};
	ev.type = XWLE_MOUSEBUTTON_RELEASED;
	ev.button = XWLMB_MIDDLE;
	xwl_send_event( &ev );
}


-(void) otherMouseDragged:(NSEvent *) theEvent
{
	dispatchMouseMoveEvent( theEvent );
	//NSLog( @"otherMouseDragged" );
}

-(void) scrollWheel:(NSEvent *) theEvent
{
	xwl_event_t ev = {0};
	ev.type = XWLE_MOUSEWHEEL;
	// -1 is towards the user
	// 1 is away from the user
	ev.wheelDelta = ([theEvent deltaY] > 0) ? 1 : -1;
	ev.window = &((xwlWindow*)[theEvent window]).xwlhandle->handle;
	xwl_send_event( &ev );
	//NSLog( @"scrollWheel %g %g %g", [theEvent deltaX], [theEvent deltaY], [theEvent deltaZ] );
}

-(void) setKeymods:(xwl_event_t*) ev fromModifierFlags:(NSUInteger) modifierFlags
{
	if ( modifierFlags & NSAlternateKeyMask )
	{
		ev->keymods |= XWLKM_ALT;
		//NSLog( @"alt" );
	}
	else
	{
		ev->keymods &= ~XWLKM_ALT;
	}
	
	if ( modifierFlags & NSControlKeyMask)
	{
		ev->keymods |= XWLKM_CONTROL;
		//NSLog( @"control" );
	}
	else
	{
		ev->keymods &= ~XWLKM_CONTROL;
	}
	
	if ( modifierFlags & NSShiftKeyMask )
	{
		ev->keymods |= XWLKM_SHIFT;
		//NSLog( @"shift" );
	}
	else
	{
		ev->keymods &= ~XWLKM_SHIFT;
	}
	
	if ( modifierFlags & NSCommandKeyMask )
	{
		ev->keymods |= XWLKM_SYSTEM;
		//NSLog( @"system" );
	}
	else
	{
		ev->keymods &= ~XWLKM_SYSTEM;
	}
}

-(void) keyDown:(NSEvent *) event
{
	xwl_event_t ev = {0};
	xwlWindow * wnd = (xwlWindow*)[event window];
	NSString *string = [event charactersIgnoringModifiers];
	
	// send text event with unicode
	if ( [[event characters] length] && ![event isARepeat] )
	{
		unichar code = [[event characters] characterAtIndex:0];
		
		// Codes from 0xF700 to 0xF8FF are non text keys (see NSEvent.h)
		// 0x35 is the Escape key
		// not escape, or any other non-text keys
		if ( [event keyCode] != 0x35 && (code < 0xF700 || code > 0xF8FF))
		{
			NSString * str;
			NSText *text = [wnd fieldEditor:YES forObject:wnd];
			[text interpretKeyEvents:[NSArray arrayWithObject:event]];
			
			str = [text string];
			if ( [str length] > 0 )
			{
				// send text event
				ev.type = XWLE_TEXT;
				ev.unicode = [str characterAtIndex:0];
				xwl_send_event( &ev );
				
				// we must reset this here, otherwise we don't get different character codes
				[text setString:@""];
			}
		}
	}
	
	if ( ![event isARepeat] )
	{
		memset( &ev, 0, sizeof(xwl_event_t) );
		ev.type = XWLE_KEYPRESSED;
		ev.window = &wnd.xwlhandle->handle;
		
		if ([string length] > 0)
		{
			ev.key = LocalizedKeys([string characterAtIndex:0]);
		}
		
		// the key is not a localized one...
		if (ev.key == XWLK_INVALID)
		{
			ev.key = NonLocalizedKeys([event keyCode]);
			
			if ( ev.key == XWLK_BACKSPACE )
			{
				ev.unicode = 8;
				ev.type = XWLE_TEXT;
				xwl_send_event( &ev );
			}
		}
		
		if (ev.key == XWLK_INVALID)
		{
			NSLog( @"Unknown key code: %i", [event keyCode] );
		}
		
		// setup modifier flags
		[self setKeymods:&ev fromModifierFlags:[event modifierFlags]];
		
		ev.type = XWLE_KEYPRESSED;
		// send the KeyPressed event
		xwl_send_event( &ev );
	}

	//NSLog( @"xwlOpenGLView keyDown" );
	//[super keyDown: event];
}

-(BOOL)resignFirstResponder
{
	return NO;
}

-(void) keyUp:(NSEvent *) event
{
	xwl_event_t ev = {0};
	xwlWindow * wnd = (xwlWindow*)[event window];
	NSString *string = [event charactersIgnoringModifiers];
	
	if ( ![event isARepeat] )
	{
		memset( &ev, 0, sizeof(xwl_event_t) );
		ev.type = XWLE_KEYRELEASED;
		ev.window = &wnd.xwlhandle->handle;
		
		if ([string length] > 0)
		{
			ev.key = LocalizedKeys([string characterAtIndex:0]);
		}
		
		// the key is not a localized one...
		if (ev.key == XWLK_INVALID)
		{
			ev.key = NonLocalizedKeys([event keyCode]);
		}
		
		if (ev.key == XWLK_INVALID)
		{
			NSLog( @"Unknown key code: %i", [event keyCode] );
		}
		
		// send the KeyPressed event
		xwl_send_event( &ev );
	}
	else
	{
		//[super keyUp: event];
	}
	
	//NSLog( @"xwlOpenGLView keyUp" );
	[super keyUp: event];
}

-(void)flagsChanged:(NSEvent *)event
{
/*
	xwl_event_t ev;
	ev.type = XWLE_KEYPRESSED;
	[self setKeymods:&ev fromModifierFlags:[event modifierFlags]];
	*/
/*
//	 TODO: generate key events for these
		NSUInteger modifierFlags = [event modifierFlags];
		if ( modifierFlags & NSAlternateKeyMask )
			NSLog( @"alt" );
		if ( modifierFlags & NSControlKeyMask)
			NSLog( @"control" );
		if ( modifierFlags & NSShiftKeyMask )
			NSLog( @"shift" );
		if ( modifierFlags & NSCommandKeyMask )
			NSLog( @"system" );
	
	NSLog( @"Flags Changed!" );
	*/
}

-(void)mouseEntered:(NSEvent *)theEvent
{
	//NSLog( @"mouseEntered");
}

-(void)mouseExited:(NSEvent *)theEvent
{
	//NSLog( @"mouseExited" );
}

-(void)viewDidChangeBackingProperties
{
	NSLog(@"xwl: handle viewDidChangeBackingProperties");
}

-(void)update
{
	int new_viertual_screen;

	// handle GPU switches
	[super update];

	new_viertual_screen = [[self openGLContext] currentVirtualScreen];
	if ( current_virtual_screen != new_viertual_screen )
	{
		// a switch has occurred - requery GPU for capabilities
		current_virtual_screen = new_viertual_screen;
	}
}

@end