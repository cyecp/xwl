#include <xwl/xwl.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <xwl/platforms/x11/x11.h>
#include <string.h> // for memset

// if this is set, then we'll try to setup GLFBConfig and use glXCreateContextAttribs to create the context
// as opposed to the legacy method.

#define XWL_USE_FBCONFIG 1

#if XWL_USE_FBCONFIG
	//#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
	//#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
	#define GLX_CONTEXT_PROFILE_MASK_ARB 0x9126
	#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB	0x00000001
	#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

	typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)( Display *, GLXFBConfig, GLXContext, Bool, const int * );
	GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 0;
#endif

typedef void (*GLXSWAPINTERVALEXTPROC)( int interval );
GLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = 0;

#if 0
Window xwl_linux_create_window( xwl_renderer_settings_t * settings, unsigned int * attribs )
{
    XSetWindowAttributes window_attribs;
    Window handle;
	Colormap colormap;
	int cwattrs;


	fprintf( stderr, "[xwl] Okay... trying to create a window\n" );

	if ( !attribs )
	{
		// create a simple test window used to create a GL context.
		GLint basic_attribs[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None };

		fprintf( stderr, "[xwl] Choosing visual...\n" );
		settings->visual = glXChooseVisual( settings->display, settings->screen, basic_attribs );

		if ( !settings->visual )
		{
			fprintf( stderr, "[xwl] Unable to get temporary visual!\n" );
			return 0;
		}

		fprintf( stdout, "[xwl] status is valid\n" );
		window_attribs.event_mask = StructureNotifyMask;
		window_attribs.colormap = XCreateColormap( settings->display, RootWindow(settings->display, settings->screen), settings->visual->visual, AllocNone);
        handle = XCreateWindow( settings->display, RootWindow(settings->display, settings->screen), 0, 0, 100, 100, 0, settings->visual->depth, InputOutput, settings->visual->visual, CWColormap | CWEventMask, &window_attribs );
		XMapWindow( settings->display, handle );

		if ( handle )
		{
			glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
			fprintf( stdout, "[xwl] function is: %p\n", glXCreateContextAttribsARB );

			// we have the function pointer, destroy this window.
			XDestroyWindow( settings->display, handle );
		}
		else
		{
			fprintf( stderr, "[xwl Error creating temporary window!\n" );
		}
	}

	return handle;
}
#endif





void *x11_opengl_create_context( xwl_native_window_t * native_window, xwl_window_provider_t * wapi, unsigned int * attributes, void * other_context )
{
	XVisualInfo * visual = 0;
	GLXContext context = 0;
	int modern_attributes[ 2 * XWL_ATTRIBUTE_COUNT ];
	int attrib_id = 0;
	memset( modern_attributes, 0, sizeof(int) * 2 * XWL_ATTRIBUTE_COUNT );



#if XWL_USE_FBCONFIG
	// check glXCreateContextAttribs, if it's not valid, we need to get a pointer to it

	if ( !glXCreateContextAttribsARB )
	{
		// I don't think we even need a new window for this - it's working on my desktop ubuntu.
		// Let's verify this behavior before removing this code.
		glXCreateContextAttribsARB = (GLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
		/*
		// we need a dummy window to get this from OpenGL... so...
		xwl_native_window_t handle;
		GLint basic_attribs[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None };
		int basic_pixel_format;
		XVisualInfo * vi = glXChooseVisual( x11_current_display(), x11_current_screen(), basic_attribs );

		if ( wapi->create_window( &handle, "dummy", basic_attribs, basic_pixel_format ) )
		{
			// glXGetProcAddress here?
			wapi->destroy_window( &handle );
		}
		XFree( vi );
		*/


		if ( !glXCreateContextAttribsARB )
		{
			xwl_set_error( "glXCreateContextAttribsARB could not be found" );
			return 0;
		}

		glXSwapIntervalEXT = (GLXSWAPINTERVALEXTPROC) glXGetProcAddress((const GLubyte*)"glXSwapIntervalSGI");
		if ( !glXSwapIntervalEXT )
		{
			xwl_set_error( "glXSwapIntervalEXT could not be found" );
			return 0;
		}
	}

	// setup attributes that will be passed to glXChooseFBConfig
	{
		int default_depth = attributes[ XWL_DEPTH_SIZE ];
		int default_stencil = attributes[ XWL_STENCIL_SIZE ];

		// fill in attributes for frame buffer config
		modern_attributes[ attrib_id++ ] = GLX_DOUBLEBUFFER;
		modern_attributes[ attrib_id++ ] = True;
		modern_attributes[ attrib_id++ ] = GLX_X_RENDERABLE;
		modern_attributes[ attrib_id++ ] = True;
		modern_attributes[ attrib_id++ ] = GLX_RENDER_TYPE;
		modern_attributes[ attrib_id++ ] = GLX_RGBA_BIT;
		modern_attributes[ attrib_id++ ] = GLX_RED_SIZE;
		modern_attributes[ attrib_id++ ] = 8;
		modern_attributes[ attrib_id++ ] = GLX_GREEN_SIZE;
		modern_attributes[ attrib_id++ ] = 8;
		modern_attributes[ attrib_id++ ] = GLX_BLUE_SIZE;
		modern_attributes[ attrib_id++ ] = 8;
		modern_attributes[ attrib_id++ ] = GLX_ALPHA_SIZE;
		modern_attributes[ attrib_id++ ] = 8;

		if ( default_stencil > 0 )
		{
			modern_attributes[ attrib_id++ ] = GLX_STENCIL_SIZE;
			modern_attributes[ attrib_id++ ] = default_stencil;
		}

		if ( default_depth > 0 )
		{
			modern_attributes[ attrib_id++ ] = GLX_DEPTH_SIZE;
			modern_attributes[ attrib_id++ ] = default_depth;
		}
	}

	// fetch an fb config and create the modern context
	{

		GLXFBConfig * framebuffer_config = 0;
		int elements;

		framebuffer_config = glXChooseFBConfig( x11_current_display(), x11_current_screen(), modern_attributes, &elements );
		if ( !framebuffer_config )
		{
			xwl_set_error( "Failed to retrieve frame buffer configuration" );
			return 0;
		}

		/*
		// This should be used to fetch the pixel format
		visual = glXGetVisualFromFBConfig( x11_current_display(), framebuffer_config[0] );
		if ( !visual )
		{
			xwl_set_error( "Unable to get visual from frame buffer config" );
			return 0;
		}		
		*/

		attrib_id = 0;
		memset( modern_attributes, 0, sizeof(int) * 2 * XWL_ATTRIBUTE_COUNT );
		// fill in attributes for glXCreateContextAttrib
		if ( attributes[ XWL_API_MAJOR_VERSION ] == 3 && attributes[ XWL_API_MINOR_VERSION ] == 2 )
		{
			modern_attributes[ attrib_id++ ] = GLX_CONTEXT_PROFILE_MASK_ARB;
			modern_attributes[ attrib_id++ ] = GLX_CONTEXT_CORE_PROFILE_BIT_ARB;					
			modern_attributes[ attrib_id++ ] = GLX_CONTEXT_MAJOR_VERSION_ARB;
			modern_attributes[ attrib_id++ ] = 3;
			modern_attributes[ attrib_id++ ] = GLX_CONTEXT_MINOR_VERSION_ARB;
			modern_attributes[ attrib_id++ ] = 2;
		}
		else
		{
			modern_attributes[ attrib_id++ ] = GLX_CONTEXT_PROFILE_MASK_ARB;
			modern_attributes[ attrib_id++ ] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
		}

		context = glXCreateContextAttribsARB( x11_current_display(), *framebuffer_config, 0, 1, modern_attributes );
		XFree( framebuffer_config );
	}


	//glXSwapIntervalEXT(0);

#else
	fprintf( stdout, "[xwl] Fetching visual: %i...\n", native_window->handle.pixel_format );
	visual = x11_fetch_visual( native_window->handle.pixel_format );

	fprintf( stdout, "[xwl] Creating GLXContext...\n" );
	context = glXCreateContext( x11_current_display(), visual, (GLXContext)other_context, True /*Direct Rendering*/ );
	if ( context )
	{
		if ( !glXIsDirect( x11_current_display(), context ) )
		{
			fprintf( stderr, "[xwl] Direct rendering is NOT enabled!\n" );
			XFree( visual );
			return 0;
		}
	}
	else
	{
		fprintf( stderr, "[xwl] context is NULL\n" );
	}

	// free the visual
	XFree( visual );	
#endif

	return context;
} // x11_opengl_create_context

void x11_opengl_destroy_context( void * context, xwl_native_window_t * native_window, struct xwl_window_provider_s * wapi )
{
	glXMakeCurrent( x11_current_display(), None, 0 );
	glXDestroyContext( x11_current_display(), (GLXContext)native_window->handle.context );
} // x11_opengl_destroy_context

void x11_opengl_activate_context( void * context, xwl_native_window_t * native_window )
{
	Window window = (Window)native_window->handle.handle;
	Bool success = False;
	success = glXMakeCurrent( x11_current_display(), window, (GLXContext)context );
	if ( !success )
	{
		fprintf( stdout, "glXMakeCurrent failed on context %p\n", context );
	}
	
} // x11_opengl_activate_context

void x11_opengl_swap_buffers( xwl_native_window_t * window )
{
	if ( window && window->handle.handle )
	{
		glXSwapBuffers( x11_current_display(), (GLXDrawable)window->handle.handle );
	}
} // x11_opengl_swap_buffers

int x11_opengl_pixel_format( unsigned int * attribs )
{
	int attrib[] = {GLX_RGBA, GLX_DOUBLEBUFFER, // need double buffering
							GLX_RGBA,
							GLX_DEPTH_SIZE, 16,	// put in the depth size
														// that was passed to us
							GLX_RED_SIZE, 8,			// 8 bits pretty standard for our RGBA
							GLX_GREEN_SIZE, 8,
							GLX_BLUE_SIZE,8,
							GLX_ALPHA_SIZE, 8,
							None };

	int glx_major, glx_minor;

	if ( !glXQueryVersion( x11_current_display(), &glx_major, &glx_minor ) ||
		(glx_major == 1) && (glx_minor < 3) || (glx_major < 1) )
	{
		xwl_set_error( "Invalid GLX version!" );
		return 0;
	}

	XVisualInfo * visual = glXChooseVisual( x11_current_display(), x11_current_screen(), attrib );

	return visual->visualid; 
} // x11_opengl_pixel_format

void * x11_opengl_get_symbol( const char * symbol_name )
{
	return (void*)glXGetProcAddress( (const GLubyte*)symbol_name );
} // x11_opengl_get_symbol


void x11_opengl_register( xwl_api_provider_t * api )
{
	api->create_context = x11_opengl_create_context;
	api->destroy_context = x11_opengl_destroy_context;
	api->activate_context = x11_opengl_activate_context;
	api->swap_buffers = x11_opengl_swap_buffers;

	api->pixel_format = x11_opengl_pixel_format;
	api->get_symbol = x11_opengl_get_symbol;
}