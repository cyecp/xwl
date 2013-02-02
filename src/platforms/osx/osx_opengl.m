#import <xwl/platforms/osx.h>
#import <xwl/platforms/osx/appdelegate.h>
#import <xwl/platforms/osx/xwlopenglview.h>
#import <xwl/platforms/osx/xwlwindow.h>

#import <string.h>

void *cocoa_opengl_create_context( void * native_window, xwl_window_provider_t * wapi, unsigned int * attributes, void * other_context )
{
	NSOpenGLPixelFormatAttribute * outattribs = 0;
	NSOpenGLPixelFormatAttribute * attrib_pointer = 0;
	NSLog( @"create opengl context" );
	NSOpenGLPixelFormat * format;
	NSOpenGLContext * context;
	unsigned int depth_size = 24;
	unsigned int color_size = 24;
	size_t attribute_size = sizeof(NSOpenGLPixelFormatAttribute) * XWL_ATTRIBUTE_COUNT * 2;
	attrib_pointer = outattribs = (NSOpenGLPixelFormatAttribute*)malloc( attribute_size );
	memset( outattribs, 0, attribute_size );
	
	if ( attributes[ XWL_DEPTH_SIZE ] )
	{
		depth_size = attributes[ XWL_DEPTH_SIZE ];
	}
	
	if ( attributes[ XWL_COLOR_SIZE ] )
	{
		color_size = attributes[ XWL_COLOR_SIZE ];
	}

	// request a core profile if we specified a version
	*outattribs++ = NSOpenGLPFAOpenGLProfile;
	if ( attributes[ XWL_API_MAJOR_VERSION ] == 3 && attributes[ XWL_API_MINOR_VERSION ] == 2 )
	{
		*outattribs++ = NSOpenGLProfileVersion3_2Core;
	}
	else // otherwise, return the legacy profile
	{
		*outattribs++ = NSOpenGLProfileVersionLegacy;
	}
	
	// add default attributes
	*outattribs++ = NSOpenGLPFADepthSize;
	*outattribs++ = depth_size;
	*outattribs++ = NSOpenGLPFAAccelerated;
	*outattribs++ = NSOpenGLPFADoubleBuffer;
	*outattribs++ = NSOpenGLPFAColorSize;
	*outattribs++ = color_size;
	*outattribs = 0;
	

	// create an opengl pixel format
	format = [[NSOpenGLPixelFormat alloc] initWithAttributes: attrib_pointer];
	free( attrib_pointer );
	
	if ( format == nil )
	{
		xwl_set_error( "Unable to create NSOpenGLPixelFormat!" );
		return 0;
	}
	
	// create opengl context
	context = [[NSOpenGLContext alloc] initWithFormat: format shareContext: (NSOpenGLContext*)other_context];
	[format release];
	
	if ( context == nil )
	{
		xwl_set_error( "Unable to create NSOpenGLContext!" );
		return 0;
	}
	
	return context;
} // cocoa_opengl_create_context

void cocoa_opengl_destroy_context( void * context, void * native_window, xwl_window_provider_t * wapi )
{
	NSLog( @"destroy opengl context" );
	[NSOpenGLContext clearCurrentContext];
} // cocoa_opengl_destroy_context


void cocoa_opengl_activate_context( void * context, void * native_window )
{
	xwlWindow * window = (xwlWindow*)native_window;
	xwlOpenGLView * view = nil;
	NSOpenGLContext * ctx = (NSOpenGLContext*)context;
	
	if ( window == nil )
	{
		return;
	}
	
	if ( ctx == nil )
	{
		return;
	}
	
	// create the custom view
	view = [[xwlOpenGLView alloc] initWithFrame: [[window contentView] frame] ];
	[view setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
	
	// set the context of that view
	[view setContext: ctx];
	
	// context is now 'current', so glGetString calls succeed.
	[ctx makeCurrentContext];
	
	// make sure it sends the windowResized event
	[[NSNotificationCenter defaultCenter] addObserver: view
											 selector:@selector(windowResized:) name:NSWindowDidResizeNotification
											   object: window];
	
	// make the view this window's first responder and add it as the content view
	[window setContentView: view];
	[window makeFirstResponder: [window contentView]];
	[view release];
} // cocoa_opengl_activate_context

void cocoa_opengl_swap_buffers( void * native_window )
{
	xwlWindow * window = (xwlWindow*)native_window;	
	xwlOpenGLView * view = [window contentView];
	[[view getContext] flushBuffer];
} // cocoa_opengl_swap_buffers

void cocoa_opengl_register( xwl_api_provider_t * api )
{
	api->create_context = cocoa_opengl_create_context;
	api->destroy_context = cocoa_opengl_destroy_context;
	api->activate_context = cocoa_opengl_activate_context;
	api->swap_buffers = cocoa_opengl_swap_buffers;
} // cocoa_opengl_register