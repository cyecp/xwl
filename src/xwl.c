#include <xwl/xwl.h>
#include <xwl/xwl_internal.h>
#include <stdio.h>
#include <string.h> // for memset
#include <wchar.h>
#include <xlib.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

static struct xwl_native_window_s xwl_windowHandles[ XWL_MAX_WINDOW_HANDLES ];

static xwl_event_callback xwl_callback;
const char * xwlerror;
xlib_t api_lib;
int _use_findsymbol = 0;

const char * xwl_get_error( void )
{
    return xwlerror;
} // xwl_get_error

void xwl_set_error( const char * error )
{
	xwlerror = error;
} // xwl_set_error


xwl_native_window_t *_xwl_window_at_index( int index )
{
	if ( index > XWL_MAX_WINDOW_HANDLES-1 )
	{
		return 0;
	}

	return &xwl_windowHandles[ index ];
} // _xwl_window_at_index


#if _WIN32 || __linux__
int _xwl_open_driver_library( unsigned int api_provider )
{
	const char * library_name = "";
#if __linux__
#if RASPBERRYPI
	library_name = "libGLESv2.so";
#else
	library_name = "libGL.so";
#endif
	
	if ( api_provider == XWL_API_PROVIDER_EGL )
	{
		library_name = "libGLESv2.so";
	}
	
#elif _WIN32
	library_name = "OpenGL32.dll";
#endif
	
	if ( !xlib_open( &api_lib, library_name ) )
	{
		return 0;
	}
	
	return 1;
}


void _xwl_close_driver_library()
{
	// close the library
	xlib_close( &api_lib );
}

#endif



xwl_native_window_t *xwl_get_unused_window()
{
	int i;
	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		if ( !xwl_windowHandles[i].handle.handle )
		{
			return &xwl_windowHandles[i];
		}
	}

	// no more window handles
	return 0;
} // xwl_get_unused_window

void xwl_send_event( xwl_event_t * ev )
{
	// if there is a callback function set, dispatch to the callback
	if ( xwl_callback )
	{
		xwl_callback( ev );
	}
} // xwl_send_event


const char * xwl_key_to_string( int key )
{
	switch( key )
	{
		case XWLK_A: return "XWLK_A";
		case XWLK_B: return "XWLK_B";
		case XWLK_C: return "XWLK_C";
		case XWLK_D: return "XWLK_D";
		case XWLK_E: return "XWLK_E";
		case XWLK_F: return "XWLK_F";
		case XWLK_G: return "XWLK_G";
		case XWLK_H: return "XWLK_H";
		case XWLK_I: return "XWLK_I";
		case XWLK_J: return "XWLK_J";
		case XWLK_K: return "XWLK_K";
		case XWLK_L: return "XWLK_L";
		case XWLK_M: return "XWLK_M";
		case XWLK_N: return "XWLK_N";
		case XWLK_O: return "XWLK_O";
		case XWLK_P: return "XWLK_P";
		case XWLK_Q: return "XWLK_Q";
		case XWLK_R: return "XWLK_R";
		case XWLK_S: return "XWLK_S";
		case XWLK_T: return "XWLK_T";
		case XWLK_U: return "XWLK_U";
		case XWLK_V: return "XWLK_V";
		case XWLK_W: return "XWLK_W";
		case XWLK_Y: return "XWLK_Y";
		case XWLK_X: return "XWLK_X";
		case XWLK_Z: return "XWLK_Z";
		case XWLK_LSYSTEM: return "XWLK_LSYSTEM";
		case XWLK_RSYSTEM: return "XWLK_RSYSTEM";
		case XWLK_MENU: return "XWLK_MENU";
		case XWLK_SEMICOLON: return "XWLK_SEMICOLON";
		case XWLK_SLASH: return "XWLK_SLASH";
		case XWLK_BACKSLASH: return "XWLK_BACKSLASH";
		case XWLK_EQUALS: return "XWLK_EQUALS";
		case XWLK_MINUS: return "XWLK_MINUS";
		case XWLK_LBRACKET: return "XWLK_LBRACKET";
		case XWLK_RBRACKET: return "XWLK_RBRACKET";
		case XWLK_COMMA: return "XWLK_COMMA";
		case XWLK_PERIOD: return "XWLK_PERIOD";
		case XWLK_QUOTE: return "XWLK_QUOTE";
		case XWLK_TILDE: return "XWLK_TILDE";
		case XWLK_ESCAPE: return "XWLK_ESCAPE";
		case XWLK_SPACE: return "XWLK_SPACE";
		case XWLK_RETURN: return "XWLK_RETURN";
		case XWLK_BACKSPACE: return "XWLK_BACKSPACE";
		case XWLK_TAB: return "XWLK_TAB";
		case XWLK_PAGEUP: return "XWLK_PAGEUP";
		case XWLK_PAGEDN: return "XWLK_PAGEDN";
		case XWLK_END: return "XWLK_END";
		case XWLK_HOME: return "XWLK_HOME";
		case XWLK_INSERT: return "XWLK_INSERT";
		case XWLK_DELETE: return "XWLK_DELETE";
		case XWLK_ADD: return "XWLK_ADD";
		case XWLK_SUBTRACT: return "XWLK_SUBTRACT";
		case XWLK_MULTIPLY: return "XWLK_MULTIPLY";
		case XWLK_DIVIDE: return "XWLK_DIVIDE";
		case XWLK_PAUSE: return "XWLK_PAUSE";
		case XWLK_F1: return "XWLK_F1";
		case XWLK_F2: return "XWLK_F2";
		case XWLK_F3: return "XWLK_F3";
		case XWLK_F4: return "XWLK_F4";
		case XWLK_F5: return "XWLK_F5";
		case XWLK_F6: return "XWLK_F6";
		case XWLK_F7: return "XWLK_F7";
		case XWLK_F8: return "XWLK_F8";
		case XWLK_F9: return "XWLK_F9";
		case XWLK_F10: return "XWLK_F10";
		case XWLK_F11: return "XWLK_F11";
		case XWLK_F12: return "XWLK_F12";
		case XWLK_F13: return "XWLK_F13";
		case XWLK_F14: return "XWLK_F14";
		case XWLK_F15: return "XWLK_F15";
		case XWLK_LEFT: return "XWLK_LEFT";
		case XWLK_RIGHT: return "XWLK_RIGHT";
		case XWLK_UP: return "XWLK_UP";
		case XWLK_DOWN: return "XWLK_DOWN";
		case XWLK_NUMPAD0: return "XWLK_NUMPAD0";
		case XWLK_NUMPAD1: return "XWLK_NUMPAD1";
		case XWLK_NUMPAD2: return "XWLK_NUMPAD2";
		case XWLK_NUMPAD3: return "XWLK_NUMPAD3";
		case XWLK_NUMPAD4: return "XWLK_NUMPAD4";
		case XWLK_NUMPAD5: return "XWLK_NUMPAD5";
		case XWLK_NUMPAD6: return "XWLK_NUMPAD6";
		case XWLK_NUMPAD7: return "XWLK_NUMPAD7";
		case XWLK_NUMPAD8: return "XWLK_NUMPAD8";
		case XWLK_NUMPAD9: return "XWLK_NUMPAD9";
		case XWLK_0: return "XWLK_0";
		case XWLK_1: return "XWLK_1";
		case XWLK_2: return "XWLK_2";
		case XWLK_3: return "XWLK_3";
		case XWLK_4: return "XWLK_4";
		case XWLK_5: return "XWLK_5";
		case XWLK_6: return "XWLK_6";
		case XWLK_7: return "XWLK_7";
		case XWLK_8: return "XWLK_8";
		case XWLK_9: return "XWLK_9";

		case XWLK_LSHIFT: return "XWLK_LSHIFT";
		case XWLK_RSHIFT: return "XWLK_RSHIFT";
		case XWLK_LCONTROL: return "XWLK_LCONTROL";
		case XWLK_RCONTROL: return "XWLK_RCONTROL";
		case XWLK_LALT: return "XWLK_LALT";
		case XWLK_RALT: return "XWLK_RALT";
		case XWLK_NUMLOCK: return "XWLK_NUMLOCK";
	}

	return "XWLK_INVALID";
}

const char * xwl_event_to_string( int event_type )
{
	switch( event_type )
	{
		case XWLE_INVALID: return "XWLE_INVALID";
		case XWLE_MOUSEMOVE: return "XWLE_MOUSEMOVE";
		case XWLE_MOUSEBUTTON_PRESSED: return "XWLE_MOUSEBUTTON_PRESSED";
		case XWLE_MOUSEBUTTON_RELEASED: return "XWLE_MOUSEBUTTON_RELEASED";
		case XWLE_MOUSEWHEEL: return "XWLE_MOUSEWHEEL";
		case XWLE_KEYPRESSED: return "XWLE_KEYPRESSED";
		case XWLE_KEYRELEASED: return "XWLE_KEYRELEASED";
//		case XWLE_JOYSTICK_MOVE: return "XWLE_JOYSTICK_MOVE";
//		case XWLE_JOYSTICKBUTTON_PRESSED: return "XWLE_JOYSTICKBUTTON_PRESSED";
//		case XWLE_JOYSTICKBUTTON_RELEASED: return "XWLE_JOYSTICKBUTTON_RELEASED";
		case XWLE_SIZE: return "XWLE_SIZE";
		case XWLE_CLOSED: return "XWLE_CLOSED";
		case XWLE_LOSTFOCUS: return "XWLE_LOSTFOCUS";
		case XWLE_GAINFOCUS: return "XWLE_GAINFOCUS";
		case XWLE_TEXT: return "XWLE_TEXT";
	}

	return "";
}

const char * xwl_mouse_to_string( int mb )
{
	switch( mb )
	{
		case XWLMB_LEFT: return "XWLMB_LEFT";
		case XWLMB_RIGHT: return "XWLMB_RIGHT";
		case XWLMB_MIDDLE: return "XWLMB_MIDDLE";
		case XWLMB_MOUSE4: return "XWLMB_MOUSE4";
		case XWLMB_MOUSE5: return "XWLMB_MOUSE5";

	}
	return "";
}




#if __linux__
/*
int xwl_xserver_handler( Display * display, XErrorEvent * event )
{
	char msg[80];
	XGetErrorText(display, event->error_code, msg, 80);
	fprintf(stderr, "[xwl.XError] Error code %s\n", msg);
	return 0;
}
*/
#endif
	
// include relevant headers for platforms

#if __APPLE__
	#include <xwl/platforms/osx/osx.h>
#elif __linux__

	#if RASPBERRYPI
		#include <xwl/platforms/rpi/rpi.h>
	#endif

	#if XWL_WITH_EGL
		#include <xwl/platforms/egl/egl.h>
	#endif

	#if XWL_WITH_X11
		#include <xwl/platforms/x11/x11.h>
	#endif

#elif _WIN32
	#include <xwl/platforms/win32/win32.h>
#endif

xwl_window_provider_t _window_provider;
xwl_api_provider_t _api_provider;
xwl_input_provider_t _input_provider;

// this list must sync up with the XWL_WINDOW_PROVIDER_* list
xwl_window_provider_register _window_providers[] = {
	0, // invalid
	0, // default
#if __linux__
	0, // EGL

	#if XWL_WITH_X11
		x11_window_register, // X11
	#else
		0, // X11
	#endif

	0, // Wayland
#else
	0, 0, 0,
#endif

#if __APPLE__
	cocoa_register,
#else
	0,
#endif

#if _WIN32
	win32_window_register, // Win32
#else
	0,
#endif

#if RASPBERRYPI
	rpi_window_register, // Raspberry Pi
#endif
};

// this list must sync up with the XWL_API_PROVIDER_* list
xwl_api_provider_register _api_providers[] = {
	0, // invalid
	0, // default
#if __linux__

	#if XWL_WITH_EGL
		egl_api_register,
	#else
		0, // EGL
	#endif

	#if XWL_WITH_X11
		x11_opengl_register, // X11,
	#else
		0,
	#endif
#else
	0, 0,
#endif

#if __APPLE__
	cocoa_api_register,
#else
	0,
#endif

#if _WIN32
	win32_opengl_register, // Win32
#else
	0,
#endif
};

// this list must sync up with the XWL_INPUT_PROVIDER_* list
xwl_input_provider_register _input_providers[] = {
	0, // invalid
	0, // default
#if __linux__

	#if XWL_WITH_X11
		x11_input_register, // X11
	#else
		0, // X11
	#endif

#else
	0,
#endif

#if __APPLE__
	cocoa_input_register,
#else
	0,
#endif

#if _WIN32
	win32_input_register, // Win32
#else
	0,
#endif
};

//#define USE_PROVIDER( name ) fprintf( stdout, "use provider: '%s'\n", #name ); return name
#define USE_PROVIDER( name ) return name

unsigned int _xwl_default_window_provider()
{
#if __APPLE__
	USE_PROVIDER( XWL_WINDOW_PROVIDER_COCOA );
#elif RASPBERRYPI
	USE_PROVIDER( XWL_WINDOW_PROVIDER_RASPBERRYPI );
#elif __linux__

	#if XWL_WITH_X11
		USE_PROVIDER( XWL_WINDOW_PROVIDER_X11 );
	#endif
		
#elif _WIN32
	USE_PROVIDER( XWL_WINDOW_PROVIDER_WIN32 );
#endif

	return 0;
} // _xwl_default_window_provider

unsigned int _xwl_default_api_provider()
{
#if __APPLE__
	USE_PROVIDER( XWL_API_PROVIDER_COCOA );
#elif RASPBERRYPI
	USE_PROVIDER( XWL_API_PROVIDER_EGL );
#elif __linux__

	#if XWL_WITH_X11
		USE_PROVIDER( XWL_API_PROVIDER_X11 );
	#endif

#elif _WIN32
	USE_PROVIDER( XWL_API_PROVIDER_WIN32 );
#endif
	
	return 0;
} // _xwl_default_api_provider


unsigned int _xwl_default_input_provider()
{
#if __APPLE__
	USE_PROVIDER( XWL_INPUT_PROVIDER_COCOA );
#elif __linux__

	#if XWL_WITH_X11
		USE_PROVIDER( XWL_INPUT_PROVIDER_X11 );
	#endif

#elif _WIN32
	USE_PROVIDER( XWL_INPUT_PROVIDER_WIN32 );
#endif
	
	return 0;
} // _xwl_default_input_provider

void xwl_get_window_size( xwl_window_t * window, int * width, int * height )
{
	assert( _window_provider.get_window_size != 0 );
	_window_provider.get_window_size( window, width, height );
} // xwl_get_window_size

void xwl_get_window_render_size( xwl_window_t * window, int * width, int * height )
{
	assert( _window_provider.get_window_render_size != 0 );
	_window_provider.get_window_render_size( window, width, height );
} // xwl_get_window_render_size

void xwl_get_screen_size( unsigned int screen_index, int * width, int * height )
{
	assert( _window_provider.get_screen_size != 0 );
	_window_provider.get_screen_size( screen_index, width, height );
} // xwl_get_screen_size

unsigned int xwl_get_screen_count()
{
	assert( _window_provider.get_screen_count != 0 );
	return _window_provider.get_screen_count();
} // xwl_get_screen_count


int _xwl_setup_window_provider( unsigned int window_provider )
{
	xwl_window_provider_register wp_register;

	// choose default window provider for this platform: note this is not guaranteed to work!
    if ( window_provider == XWL_WINDOW_PROVIDER_DEFAULT )
	{
		window_provider = _xwl_default_window_provider();
	}
    
	wp_register = _window_providers[ window_provider ];
	if ( !wp_register )
	{
		return 0;
	}
    
	// register with the selected provider
    wp_register( &_window_provider );
	
	return 1;
} // _xwl_setup_window_provider

int _xwl_setup_api_provider( unsigned int api_provider )
{
	xwl_api_provider_register api_register;

	// choose default api provider
	if ( api_provider == XWL_API_PROVIDER_DEFAULT )
	{
		api_provider = _xwl_default_api_provider();
	}
	
	api_register = _api_providers[ api_provider ];
	if ( !api_register )
	{
		return 0;
	}

	api_register( &_api_provider );
	
	return 1;
} // _xwl_setup_api_provider

int _xwl_setup_input_provider( unsigned int input_provider )
{
	xwl_input_provider_register input_register;

	// choose default api provider
	if ( input_provider == XWL_INPUT_PROVIDER_DEFAULT )
	{
		input_provider = _xwl_default_input_provider();
	}
	
	input_register = _input_providers[ input_provider ];
	if ( !input_register )
	{	
		return 0;
	}
	
	input_register( &_input_provider );
	
	return 1;
} // _xwl_setup_input_provider

int xwl_startup( unsigned int window_provider, unsigned int api_provider, unsigned int input_provider )
{
	int i;
	xwl_native_window_t * wh;
	int result = 0;

	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		wh = &xwl_windowHandles[i];
		memset( wh, 0, sizeof(xwl_native_window_t) );
		wh->handle.id = i;
	}
    
    memset( &_window_provider, 0, sizeof(xwl_window_provider_t) );

	if ( !_xwl_setup_window_provider( window_provider ) )
	{
		xwlerror = "No valid window provider found!";
		return 0;
	}
    
	
	// perform startup
	assert( _window_provider.startup != 0 );
	result = _window_provider.startup( 0 );
	if ( !result )
	{
		if ( !xwl_get_error() )
		{
			xwl_set_error( "Window provider startup failed!" );
		}
		return 0;
	}

	if ( !_xwl_setup_api_provider( api_provider ) )
	{
		xwl_set_error( "No valid API provider found!" );
		return 0;
	}
	
	if ( !_xwl_setup_input_provider( input_provider ) )
	{
		xwl_set_error( "No valid Input provider found!" );
		return 0;
	}

	assert( _input_provider.startup != 0 );
	result = _input_provider.startup();
	if ( !result )
	{
		if ( !xwl_get_error() )
		{
			xwl_set_error( "Input provider startup failed!" );
		}
		return 0;
	}
    

	// open a handle to the correct API library	
	if ( _use_findsymbol && !_xwl_open_driver_library( api_provider ) )
	{
		xwl_set_error( "Unable to link with API library" );
		return 0;
	}


	return result;
} // xwl_startup

void xwl_use_findsymbol( void )
{
	_use_findsymbol = 1;
}

void * xwl_findsymbol( const char * symbol_name )
{
	void * func = 0;
	
	if ( _use_findsymbol )
	{
		// first try to get the symbol with the API specific implementation.
		assert( _api_provider.get_symbol != 0 );
		func = _api_provider.get_symbol( symbol_name );

		// if that fails, try to get it from the dynamic library
		if ( !func )
		{
	#if _WIN32 || __linux__
			func = xlib_find_symbol( &api_lib, symbol_name );
	#elif __APPLE__
			func = _xwl_apple_find_symbol( symbol_name );
	#endif
		}
	}

	return func;
} // xwl_findsymbol

void xwl_swap_buffers( xwl_window_t * window )
{
	xwl_native_window_t * wh;
	//fprintf( stdout, "[xwl] swap buffers on window: %i\n", window->id );
	wh = &xwl_windowHandles[ window->id ];
	if ( wh && wh->handle.handle != 0 )
	{
		assert( _api_provider.swap_buffers != 0 );
		_api_provider.swap_buffers( wh );
	}
} // xwl_swap_buffers

void xwl_finish()
{
	// loop through all windows and call finish
	xwl_native_window_t * wh = 0;
	int i;
	
	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		wh = &xwl_windowHandles[i];
		if ( wh->handle.handle != 0 )
		{
			xwl_swap_buffers( &wh->handle );
		}
	}
} // xwl_finish


void xwl_shutdown( void )
{
	int i;
	xwl_native_window_t * wh;
	
	assert( _input_provider.pre_window_destroy != 0 );
	assert( _api_provider.destroy_context != 0 );
	assert( _window_provider.destroy_window != 0 );

	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		wh = &xwl_windowHandles[i];

		if ( wh->handle.handle )
		{
			_input_provider.pre_window_destroy( wh );
			//_api_provider.destroy_context( &wh->handle );
			_window_provider.destroy_window( &wh->handle );
			wh->handle.handle = 0;
		}

		memset( wh, 0, sizeof(xwl_native_window_t) );
	}



	if ( _use_findsymbol )
	{
		_xwl_close_driver_library();
	}

	assert( _window_provider.shutdown != 0 );
	_window_provider.shutdown();
	
	assert( _input_provider.shutdown != 0 );
	_input_provider.shutdown();
} // xwl_shutdown

int xwl_dispatch_events()
{
	int result = 0;
	
	assert( _input_provider.dispatch_events != 0 );
	result = _input_provider.dispatch_events();

	return result;
}

xwl_window_t *xwl_create_window( const char * title, unsigned int * attribs )
{
	xwl_native_window_t * wh = 0;
	unsigned int i;
	unsigned int attributes[ XWL_ATTRIBUTE_COUNT * 2 ] = {0};
	int current_attrib = -1;
	int pixel_format;
	void * context;

	if ( title == 0 )
	{
		title = "xwl window";
	}
	
	// see if we can get an unused window handle
	wh = xwl_get_unused_window();
	if ( !wh )
	{
		xwl_set_error( "Failed to find an unused window handle!" );
		return 0;
	}
	
	attributes[ XWL_WINDOW_X ] = XWL_NOTSET;
	attributes[ XWL_WINDOW_Y ] = XWL_NOTSET;
	
	// translate all attribs into a single array we can check uniformly.
	for( i = 0; *attribs && i < XWL_ATTRIBUTE_COUNT; ++i )
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
	

	// give the API first crack at creating a pixel format
	assert( _api_provider.pixel_format != 0 );
	pixel_format = _api_provider.pixel_format( attributes );
	if ( pixel_format < 0 )
	{
		return 0;
	}

	// cache the pixel format
	wh->handle.pixel_format = pixel_format;

	// create the native window
	assert( _window_provider.create_window != 0 );
	wh->handle.handle = _window_provider.create_window( wh, title, attributes, pixel_format );
	if ( !wh->handle.handle )
	{
		xwl_set_error( "window creation failed" );
		return 0;
	}

	// tell the input system we've created a window
	assert( _input_provider.post_window_creation != 0 );
	_input_provider.post_window_creation( wh );

	// create a context
	assert( _api_provider.create_context != 0 );
	context = _api_provider.create_context( wh, &_window_provider, attributes, 0 );
	if ( !context )
	{
		if ( !xwl_get_error() )
		{
			xwl_set_error( "context creation failed" );
		}
		return 0;
	}

	// activate the context with this window
	assert( _api_provider.activate_context != 0 );
	_api_provider.activate_context( context, wh );

	if ( !wh )
	{
		return 0;
	}

	return &wh->handle;
} // xwl_create_window

void xwl_set_callback( xwl_event_callback cb )
{
	xwl_callback = cb;
} // xwl_set_callback

#ifdef __cplusplus
}; // extern "C"
#endif
