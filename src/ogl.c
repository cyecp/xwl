#include <xwl/xwl.h>
#include <xwl/xwl_ogl.h>
#include <stdio.h>

#if _WIN32
	#include <windows.h>
	#include <gl/gl.h>
	#pragma comment( lib, "opengl32.lib" )
#elif LINUX
	#include <GL/gl.h>
	#include <GL/glx.h>
	#include <stdlib.h> // for malloc/free
	#include <string.h>
#elif __APPLE__
	#include <OpenGL/OpenGL.h>
	#include <memory> // for malloc/free
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if _WIN32
int xwl_renderer_startup( xwl_renderer_settings_t * settings, unsigned int * attribs )
{
	HGLRC glrc;
	PIXELFORMATDESCRIPTOR pfd = {0};
	int pixelFormat = -1;
	HWND handle;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 24;
	pfd.cDepthBits = 24;
	pfd.cAlphaBits = 8;
	pfd.dwLayerMask = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;
	if ( !settings->window || !settings->window->handle )
		return 0;

	handle = (HWND)settings->window->handle;

	// get a DC to our window
	settings->window->dc = GetDC( handle );

	// choose the pixel format
	pixelFormat = ChoosePixelFormat( settings->window->dc, &pfd );

	if ( pixelFormat == 0 )
		return 0;

	// set the pixel format
	SetPixelFormat( settings->window->dc, pixelFormat, &pfd );

	// create a rendering context from it
	glrc = wglCreateContext( settings->window->dc );

	// make that context current
	wglMakeCurrent( settings->window->dc, glrc );

	return 1;
}

void xwl_renderer_post( xwl_renderer_settings_t * settings )
{
	if ( !settings->window )
		return;

	SwapBuffers( settings->window->dc );
}

void xwl_renderer_shutdown( xwl_renderer_settings_t * settings )
{
	HGLRC rc;

	if ( !settings->window )
		return;

	rc = wglGetCurrentContext();

	wglMakeCurrent( 0, 0 );

	wglDeleteContext( rc );

	ReleaseDC( (HWND)settings->window->handle, settings->window->dc );
}
	
void xwl_renderer_activate( xwl_renderer_settings_t * settings )
{
}
#endif


#if LINUX

//#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
//#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
#define GLX_CONTEXT_PROFILE_MASK_ARB 0x9126
#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB	0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002

typedef GLXContext (*GLXCREATECONTEXTATTRIBSARBPROC)( Display *, GLXFBConfig, GLXContext, Bool, const int * );
GLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = 0;

int xwl_linux_temp( xwl_renderer_settings_t * settings, unsigned int * attribs )
{


	// intervene

	// now try to get the new context creation pointer


	return 1;
}

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


int xwl_linux_calculate_output_attribs( unsigned int * attribs )
{
	unsigned int total_output = 0;
	int i;
	int total_attribs = 0;

	// count how many total attribs we'll need
	for( total_attribs = 0; attribs[total_attribs] != 0; ++total_attribs ) {}

	// preprocess input attribs
	for( i = 0; i < total_attribs; )
	{
		switch( attribs[i] )
		{
			case XWL_GL_PROFILE:
				++i;
				if ( attribs[i] == XWL_GLPROFILE_CORE3_2 )
				{
					total_output += 4;
				}
				else if ( attribs[i] == XWL_GLPROFILE_LEGACY )
				{
					total_output += 2;
				}
				++i;

				break;

			default: ++i; break;
		}
	}

	return total_output+1; // plus one for the 0 terminator
}

int xwl_renderer_startup( xwl_renderer_settings_t * settings, unsigned int * attribs )
{
	XWindowAttributes att;
	Window window = (Window)settings->window;
	int elements;
	int i;
	GLXFBConfig * fbc = 0;
	unsigned int total_attribs = 0;
	GLint * modern_attribs = 0;
	int current_attrib = 0;

	total_attribs = xwl_linux_calculate_output_attribs( attribs );

	fprintf( stderr, "[xwl] total attribs needed: %i\n", total_attribs );
	modern_attribs = (GLint*)malloc( total_attribs * sizeof(GLint) );
	if ( !modern_attribs )
	{
		fprintf( stderr, "[xwl] Failed to allocate %i attribs!\n", total_attribs );
		return 0;
	}
	memset( modern_attribs, 0, total_attribs*sizeof(GLint) );

	for( i = 0; i < total_attribs-1; )
	{
		switch( attribs[i] )
		{
			case XWL_GL_PROFILE:
				++i;
				if ( attribs[i] == XWL_GLPROFILE_CORE3_2 )
				{
					modern_attribs[ current_attrib++ ] = GLX_CONTEXT_MAJOR_VERSION_ARB;
					modern_attribs[ current_attrib++ ] = 3;
					modern_attribs[ current_attrib++ ] = GLX_CONTEXT_MINOR_VERSION_ARB;
					modern_attribs[ current_attrib++ ] = 2;
				}
				else if ( attribs[i] == XWL_GLPROFILE_LEGACY )
				{
					modern_attribs[ current_attrib++ ] = GLX_CONTEXT_PROFILE_MASK_ARB;
					modern_attribs[ current_attrib++ ] = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
				}
				++i;

				break;

			//case XWL_GL_DEPTHSIZE:
			//	break;

			default: ++i; break;
		}
	}

    fbc = glXChooseFBConfig( settings->display, settings->screen, 0, &elements );
    if ( !fbc )
    {
    	fprintf( stderr, "[xwl] Failed to retrieve frame buffer configuration\n" );
    	return 0;
    }

    //settings->visual = glXChooseVisual( settings->display, settings->screen, attrib );
    settings->visual = glXGetVisualFromFBConfig( settings->display, fbc[0] );
    if ( !settings->visual )
    {
        fprintf( stderr, "[xwl] Unable to get visual!\n" );
        return 0;
    }

    // see if we can create a context with the new function
    if ( glXCreateContextAttribsARB )
    {
		modern_attribs[ current_attrib ] = 0;
    	settings->window->context = glXCreateContextAttribsARB( settings->display, *fbc, 0, 1, modern_attribs );
    }
    else // otherwise, resort to the old way
    {
		settings->window->context = glXCreateContext( settings->display, settings->visual, 0, 1 );
	}

	// free the memory
	free( modern_attribs );
	modern_attribs = 0;	

    if ( !settings->window->context )
    {
        fprintf( stderr, "[xwl] Unable to create context!\n" );
        return 0;
    }

    if ( !glXIsDirect( settings->display, settings->window->context ) )
    {
        fprintf( stderr, "[xwl] Direct rendering is NOT enabled!\n" );
        return 0;
    }

	return 1;
}

void xwl_renderer_post( xwl_renderer_settings_t * settings )
{
    if ( !settings || !settings->display || !settings->window )
        return;

    glXSwapBuffers( settings->display, (GLXDrawable)settings->window->handle );
}

void xwl_renderer_shutdown( xwl_renderer_settings_t * settings )
{
    glXMakeCurrent( settings->display, None, 0 );
    glXDestroyContext( settings->display, settings->window->context );
}
	
void xwl_renderer_activate( xwl_renderer_settings_t * settings )
{
}
#endif




#if __APPLE__
void xwl_pollevent_osx( xwl_event_t * event );
void xwl_setup_osx_rendering( xwl_window_t * window, unsigned int * attribs );
void xwl_osx_finish( xwl_window_t * window );
void xwl_osx_activate( xwl_window_t * window );

int xwl_renderer_startup( xwl_renderer_settings_t * settings, unsigned int * attribs )
{
	// setup an opengl view
	xwl_setup_osx_rendering( settings->window, attribs );

	return 1;
}

void xwl_renderer_post( xwl_renderer_settings_t * settings )
{
	xwl_osx_finish( settings->window );
}

void xwl_renderer_shutdown( xwl_renderer_settings_t * settings )
{

}

void xwl_renderer_activate( xwl_renderer_settings_t * settings )
{
	xwl_osx_activate( settings->window );
}
#endif

#ifdef __cplusplus
}; // extern "C"
#endif
