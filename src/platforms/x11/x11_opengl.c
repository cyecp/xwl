#include <xwl/xwl.h>

#include <stdio.h>

#include <X11/Xlib.h>
// #include <X11/keysym.h>
// #include <X11/extensions/Xrandr.h>

#include <GL/glx.h>

Display * x11_current_display();
int x11_current_screen();
XVisualInfo * x11_fetch_visual( int pixel_format );

void *x11_opengl_create_context( xwl_native_window_t * native_window, xwl_window_provider_t * wapi, unsigned int * attributes, void * other_context )
{
	XVisualInfo * visual = 0;
	fprintf( stdout, "[xwl] Fetching visual: %i...\n", native_window->handle.pixel_format );

	visual = x11_fetch_visual( native_window->handle.pixel_format );

	fprintf( stdout, "[xwl] Creating GLXContext...\n" );
	GLXContext context = glXCreateContext( x11_current_display(), visual, (GLXContext)other_context, True /*Direct Rendering*/ );
	if ( context )
	{
		if ( !glXIsDirect( x11_current_display(), context ) )
		{
			fprintf( stderr, "[xwl] Direct rendering is NOT enabled!\n" );
			return 0;
		}
	}
	else
	{
		fprintf( stderr, "[xwl] context is NULL\n" );
	}

	// free the visual
	XFree( visual );

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
                            GLX_DEPTH_SIZE, 16,    // put in the depth size
                                                        // that was passed to us
                            GLX_RED_SIZE, 8,            // 8 bits pretty standard for our RGBA
                            GLX_GREEN_SIZE, 8,
                            GLX_BLUE_SIZE,8,
                            GLX_ALPHA_SIZE, 8,
                            None };

	XVisualInfo * visual = glXChooseVisual( x11_current_display(), x11_current_screen(), attrib );

	fprintf( stdout, "visual id: %zu\n", visual->visualid );

	return visual->visualid; 
} // x11_opengl_pixel_format

void x11_opengl_register( xwl_api_provider_t * api )
{
	api->create_context = x11_opengl_create_context;
	api->destroy_context = x11_opengl_destroy_context;
	api->activate_context = x11_opengl_activate_context;
	api->swap_buffers = x11_opengl_swap_buffers;

	api->pixel_format = x11_opengl_pixel_format;
}