#include <xwl/xwl.h>
#include <stdio.h>
#include <xwl/platforms/win32/win32.h>

int win32_opengl_setup_pixelformat( HWND window )
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

	PIXELFORMATDESCRIPTOR pfd = {0};
	int pixelFormat = -1;
	HDC hdc;

	if ( !window )
	{
		xwl_set_error( "[win32] Unable to create pixel format; window handle is invalid!" );
		return -1;
	}

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.cColorBits = 24;
	pfd.cDepthBits = 24;
	pfd.cAlphaBits = 8;
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

	return pixelFormat;
}



void *win32_opengl_create_context( xwl_native_window_t * native_window, xwl_window_provider_t * wapi, unsigned int * attributes, void * other_context )
{
	//int modern_attributes[ 2 * XWL_ATTRIBUTE_COUNT ];
	//int attrib_id = 0;
	//memset( modern_attributes, 0, sizeof(int) * 2 * XWL_ATTRIBUTE_COUNT );
	HGLRC glrc;
	HDC hdc;
	HWND hwnd;

	hwnd = (HWND)native_window->handle.handle;
	hdc = GetDC( hwnd );

	// create a rendering context from it
	glrc = wglCreateContext( hdc );

	// make that context current
	wglMakeCurrent( hdc, glrc );


	return glrc;
} // win32_opengl_create_context

void win32_opengl_destroy_context( void * context, xwl_native_window_t * native_window, struct xwl_window_provider_s * wapi )
{
} // win32_opengl_destroy_context

void win32_opengl_activate_context( void * context, xwl_native_window_t * native_window )
{
	
} // win32_opengl_activate_context

void win32_opengl_swap_buffers( xwl_native_window_t * window )
{

} // win32_opengl_swap_buffers

int win32_opengl_pixel_format( unsigned int * attribs )
{

	return 0;
} // win32_opengl_pixel_format

void * win32_opengl_get_symbol( const char * symbol_name )
{
	//return (void*)glXGetProcAddress( (const GLubyte*)symbol_name );
	return 0;
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