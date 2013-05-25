#include <xwl/xwl.h>
#include <stdio.h>
#include <xwl/platforms/win32/win32.h>
#include <gl/GL.h>
#include <xwl/platforms/win32/wglext.h>


int win32_load_symbols( HWND window )
{
	// this function takes a native window handle and creates an OpenGL pixelformat
	// from it. The sole purpose to retrieve some WGL functions that are used to
	// create the actual window later.
	PIXELFORMATDESCRIPTOR pfd = {0};
	int pixelFormat = -1;
	HDC hdc;
	HGLRC hrc;

	if ( !window )
	{
		xwl_set_error( "[win32] Unable to create pixel format; window handle is invalid!" );
		return -1;
	}

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	//pfd.cAlphaBits = 8;
	pfd.cStencilBits = 8;
	pfd.dwLayerMask = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.nVersion = 1;
	pfd.iPixelType = PFD_TYPE_RGBA;

	// get a DC to our window
	hdc = GetDC( window );

	// choose the pixel format
	pixelFormat = ChoosePixelFormat( hdc, &pfd );
	if ( pixelFormat == 0 )
	{
		xwl_set_error( "[win32] Unable to choose a valid pixel format" );
		return -1;
	}

	// set the pixel format
	SetPixelFormat( hdc, pixelFormat, &pfd );


	hrc = wglCreateContext( hdc );
	if ( !hrc )
	{
		xwl_set_error( "[win32] Unable to create temporary rendering context!\n" );
		return -1;
	}

	// active the context
	wglMakeCurrent( hdc, hrc );

	// load symbols here
	_win32ChoosePixelFormatARB = (pfnChoosePixelFormatARB)wglGetProcAddress( "wglChoosePixelFormatARB" );
	_win32CreateContextAttribsARB = (pfnCreateContextAttribsARB)wglGetProcAddress( "wglCreateContextAttribsARB" );

	// deactivate context
	wglMakeCurrent( hdc, NULL );


	// delete context
	wglDeleteContext( hrc );

	// release dc
	ReleaseDC( window, hdc );

	return 0;
} // win32_load_symbols

int win32_opengl_setup_pixelformat( xwl_native_window_t * native_window, HWND window, unsigned int * attributes )
{
#if 0
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
#endif

	//PIXELFORMATDESCRIPTOR pfd;
	int attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, attributes[ XWL_DEPTH_SIZE ],
		WGL_STENCIL_BITS_ARB, attributes[ XWL_STENCIL_SIZE ],
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		//WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		//WGL_SAMPLES_ARB, 4,
		0
	};

	UINT max_formats = 1;
	int pixel_formats[ 1 ];
	UINT pixel_format_count = 0;
	HGLRC hrc = 0;

	native_window->dc = GetDC( window );
	if (FALSE == _win32ChoosePixelFormatARB( native_window->dc, attribs, 0, max_formats, pixel_formats, &pixel_format_count ))
	{
		xwl_set_error( "[win32] Unable to fetch compatible pixel format\n" );
		return -1;
	}

	if (FALSE == SetPixelFormat( native_window->dc, pixel_formats[0], 0 ))
	{
		xwl_set_error( "[win32] Unable to set pixel format\n" );
		return -1;
	}
	
	return pixel_formats[0];
}



void *win32_opengl_create_context( xwl_native_window_t * native_window, xwl_window_provider_t * wapi, unsigned int * attributes, void * other_context )
{
#if 0
	HGLRC glrc;
	HDC hdc;
	HWND hwnd;

	hwnd = (HWND)native_window->handle.handle;
	hdc = native_window->dc; //GetDC( hwnd );

	// create a rendering context from it
	glrc = wglCreateContext( hdc );

	//ReleaseDC( hwnd, hdc );

	return glrc;
#endif

	int context_attribs[ 2 * XWL_ATTRIBUTE_COUNT ];
	int attrib_id = 0;
	memset( context_attribs, 0, sizeof(int) * 2 * XWL_ATTRIBUTE_COUNT );
	if ( attributes[ XWL_API_MAJOR_VERSION ] == 3 && attributes[ XWL_API_MINOR_VERSION ] == 2 )
	{
		context_attribs[ attrib_id++ ] = WGL_CONTEXT_FLAGS_ARB;
		context_attribs[ attrib_id++ ] = WGL_CONTEXT_CORE_PROFILE_BIT_ARB;					
		context_attribs[ attrib_id++ ] = WGL_CONTEXT_MAJOR_VERSION_ARB;
		context_attribs[ attrib_id++ ] = 3;
		context_attribs[ attrib_id++ ] = WGL_CONTEXT_MINOR_VERSION_ARB;
		context_attribs[ attrib_id++ ] = 2;
	}
	else
	{
		context_attribs[ attrib_id++ ] = WGL_CONTEXT_FLAGS_ARB;
		context_attribs[ attrib_id++ ] = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
	}

	return _win32CreateContextAttribsARB( native_window->dc, 0, context_attribs );
} // win32_opengl_create_context

void win32_opengl_destroy_context( void * context, xwl_native_window_t * native_window, struct xwl_window_provider_s * wapi )
{
	HGLRC glrc = (HGLRC)context;
	HWND hwnd = (HWND)native_window->handle.handle;
	
	wglMakeCurrent( native_window->dc, NULL );

	ReleaseDC( hwnd, native_window->dc );
	native_window->dc = 0;

	wglDeleteContext( glrc );
} // win32_opengl_destroy_context

void win32_opengl_activate_context( void * context, xwl_native_window_t * native_window )
{
	HGLRC glrc = (HGLRC)context;
	HWND hwnd = (HWND)native_window->handle.handle;

	// make that context current
	wglMakeCurrent( native_window->dc, glrc );
} // win32_opengl_activate_context

void win32_opengl_swap_buffers( xwl_native_window_t * native_window )
{
	HWND hwnd = (HWND)native_window->handle.handle;

	SwapBuffers( native_window->dc );
} // win32_opengl_swap_buffers

int win32_opengl_pixel_format( unsigned int * attribs )
{
	return 0;
} // win32_opengl_pixel_format

void * win32_opengl_get_symbol( const char * symbol_name )
{
	return wglGetProcAddress( symbol_name );
} // win32_opengl_get_symbol


void win32_opengl_register( xwl_api_provider_t * api )
{
	api->create_context = win32_opengl_create_context;
	api->destroy_context = win32_opengl_destroy_context;
	api->activate_context = win32_opengl_activate_context;
	api->swap_buffers = win32_opengl_swap_buffers;

	api->pixel_format = win32_opengl_pixel_format;
	api->get_symbol = win32_opengl_get_symbol;
}