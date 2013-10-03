#import <xwl/platforms/osx/osx.h>
#import <xwl/platforms/osx/appdelegate.h>
#import <xwl/platforms/osx/xwlopenglview.h>
#import <xwl/platforms/osx/xwlwindow.h>

#import <CoreVideo/CoreVideo.h>

void *cocoa_api_create_context( xwl_native_window_t * native_window, xwl_window_provider_t * wapi, unsigned int * attributes, void * other_context )
{
	NSOpenGLPixelFormatAttribute * outattribs = 0;
	NSOpenGLPixelFormatAttribute * attrib_pointer = 0;
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
//	global_format = format;
	
	free( attrib_pointer );
	
	if ( format == nil )
	{
		xwl_set_error( "Unable to create NSOpenGLPixelFormat!" );
		return 0;
	}
	
	// create opengl context
	context = [[NSOpenGLContext alloc] initWithFormat: format shareContext: (NSOpenGLContext*)other_context];
	
	GLint opacity = 1; // 1: opaque, 0: transparent
	GLint waitforvsync = 1; // 1: on; 0: off
	
	// make the window opaque
	[context setValues:&opacity forParameter:NSOpenGLCPSurfaceOpacity];
	
	// enable v-sync
	[context setValues:&waitforvsync forParameter:NSOpenGLCPSwapInterval];
	
	[format release];
	
	if ( context == nil )
	{
		xwl_set_error( "Unable to create NSOpenGLContext!" );
		return 0;
	}
	
	return context;
} // cocoa_api_create_context

void cocoa_api_destroy_context( void * context, xwl_native_window_t * native_window, xwl_window_provider_t * wapi )
{
	[NSOpenGLContext clearCurrentContext];
} // cocoa_api_destroy_context

#if 0
CVReturn display_link_callback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{

	NSLog( @"display_link_callback thread: %@", [NSThread currentThread] );
	return 0;
}
#endif

void cocoa_api_activate_context( void * context, xwl_native_window_t * native_window )
{
	xwlWindow * window = (xwlWindow*)native_window->handle.handle;
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
	
	// make sure we use the highest resolution possible on "Retina" screens.
	[view setWantsBestResolutionOpenGLSurface:YES];
	
	// make sure it sends the windowResized event
	[[NSNotificationCenter defaultCenter] addObserver: view
											 selector:@selector(windowResized:) name:NSWindowDidResizeNotification
											   object: window];
	
	
	int swap_interval = 1;
	[ctx setValues:&swap_interval forParameter:NSOpenGLCPSwapInterval];
	
	int is_opaque = 1; // 0: transparent
	[ctx setValues:&is_opaque forParameter:NSOpenGLCPSurfaceOpacity];
//	CVDisplayLinkCreateWithActiveCGDisplays( &view->display_link );
	
//	CVDisplayLinkSetOutputCallback( view->display_link, &display_link_callback, view );
	

//	CGLContextObj cglContext = [ctx CGLContextObj];
//	CGLPixelFormatObj cglPixelFormat = [global_format CGLPixelFormatObj];
//	CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext( view->display_link, cglContext, cglPixelFormat );
	
//	CVDisplayLinkStart( view->display_link );
	
	// make the view this window's first responder and add it as the content view
	[window setContentView: view];
	[window makeFirstResponder: [window contentView]];
	[view release];
} // cocoa_api_activate_context

void cocoa_api_swap_buffers( xwl_native_window_t * native_window )
{
	xwlWindow * window = (xwlWindow*)native_window->handle.handle;
	xwlOpenGLView * view = [window contentView];
	[view.context flushBuffer];
} // cocoa_api_swap_buffers

int cocoa_api_pixel_format( unsigned int * attribs )
{
	return 0;
} // cocoa_api_pixel_format

void * cocoa_api_get_symbol( const char * symbol_name )
{
	return 0;
} // cocoa_api_get_symbol

void cocoa_api_register( xwl_api_provider_t * api )
{
	api->create_context = cocoa_api_create_context;
	api->destroy_context = cocoa_api_destroy_context;
	api->activate_context = cocoa_api_activate_context;
	api->swap_buffers = cocoa_api_swap_buffers;
	api->pixel_format = cocoa_api_pixel_format;
	api->get_symbol = cocoa_api_get_symbol;
} // cocoa_api_register