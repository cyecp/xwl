#include <xwl/xwl.h>
#include <stdio.h>

#include <xwl/platforms/x11/x11.h>
// #include <X11/Xlib.h>
// #include <X11/keysym.h>
// #include <X11/extensions/Xrandr.h>
//#include <X11/XKBlib.h>


static XIM currentInputMethod = 0;
static XComposeStatus currentKeyboardStatus;

#include <string.h>

//////////////////


int x11_window_startup( xwl_api_provider_t * api )
{
	fprintf( stdout, "x11_window_startup\n" );

	//Bool detectable;
	

	if ( x11_current_display() )
	{
		x11_set_current_screen( DefaultScreen( x11_current_display() ) );
		currentInputMethod = XOpenIM( x11_current_display(), 0, 0, 0 );
	}
	else
	{
		xwl_set_error( "Unable to open an X display" );
		return 0;
	}

	// disable auto-repeats
	// "The standard behavior of the X server is to generate a KeyRelease event for every KeyPress event."
	// 
	//XkbSetDetectableAutorepeat( x11_current_display(), True, &detectable );

	// install error handler
	//XSetErrorHandler( xwl_xserver_handler );

	return 1;
} // x11_window_startup

void x11_window_shutdown()
{
	if ( currentInputMethod )
	{
		XCloseIM( currentInputMethod );

		currentInputMethod = 0;
		x11_close_display();
	}
} // x11_window_shutdown



void * x11_window_create_window( xwl_native_window_t * handle, const char * utf8_title, unsigned int * attributes, int pixel_format )
{
	// using X11, 'pixel_format' here refers to the visualID we want to create the window with.
	Window native_window = 0;
	XSetWindowAttributes window_attribs;
	int window_width;
	int window_height;
	int cwmask;

	XVisualInfo * visual = x11_fetch_visual( pixel_format );
	if ( !visual )
	{
		xwl_set_error( "Unable to fetch visual" );
		return 0;
	}

	// fill in atributes
	window_width = attributes[ XWL_WINDOW_WIDTH ];
	window_height = attributes[ XWL_WINDOW_HEIGHT ];

	window_attribs.background_pixel = 0;
	window_attribs.border_pixel = 0;

	// we now have the visual; move on to creating the window now...
	window_attribs.event_mask = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;

	fprintf( stdout, "[xwl] Create a color map... \n");
	window_attribs.colormap = XCreateColormap( x11_current_display(), RootWindow(x11_current_display(), x11_current_screen()), visual->visual, AllocNone );

	cwmask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	fprintf( stdout, "[xwl] Attempting to create a window\n" );
	native_window = XCreateWindow( x11_current_display(), RootWindow(x11_current_display(), x11_current_screen()), 0, 0, window_width, window_height, 0, visual->depth, InputOutput, visual->visual, cwmask, &window_attribs );
	fprintf( stdout, "[xwl] native_window = %i\n", (int)native_window );

	// free the visual
	fprintf( stdout, "[xwl] Freeing visual...\n" );
	XFree( visual );





	//set window name
	// XStoreName is NOT Unicode aware! So basically, don't use anything except "Host Portable Character Encoding", (essentially, ASCII), or the result will be implementation-dependent.
	XStoreName( x11_current_display(), native_window, utf8_title );

	// to work around XStoreName's shortcomings, we use XChangeProperty with _NEW_WM_NAME
	XChangeProperty( x11_current_display(), native_window,
					XInternAtom(x11_current_display(), "_NET_WM_NAME", False),
					XInternAtom(x11_current_display(), "UTF8_STRING", False),
					8, PropModeReplace, (unsigned char*)utf8_title, strlen(utf8_title) );

	// send a message to the xserver to convert this window to fullscreen
	if ( attributes[ XWL_USE_FULLSCREEN ] )
	{
		Atom wm_state = XInternAtom( x11_current_display(), "_NET_WM_STATE", False );
		Atom fullscreen = XInternAtom( x11_current_display(), "_NET_WM_STATE_FULLSCREEN", False );

		XEvent ev;
		memset( &ev, 0, sizeof(XEvent) );
		ev.type = ClientMessage;
		ev.xclient.window = native_window;
		ev.xclient.message_type = wm_state;
		ev.xclient.format = 32;
		ev.xclient.data.l[0] = 1;
		ev.xclient.data.l[1] = fullscreen;
		ev.xclient.data.l[2] = 0;

		fprintf( stdout, "[xwl] converting window to fullscreen...\n" );
		XSendEvent( x11_current_display(), DefaultRootWindow(x11_current_display()), False,
			SubstructureRedirectMask | SubstructureNotifyMask, &ev );
	}

	// show the window
	XMapWindow( x11_current_display(), native_window );

	XFlush( x11_current_display() );

	// standard init
	//myAtomClose = XInternAtom( x11_current_display(), "WM_DELETE_WINDOW", 0 );
	//XSetWMProtocols( x11_current_display(), handle, &myAtomClose, 1 );

	handle->handle.handle = (void*)native_window;
	//wh->handle.userdata = params->userdata;

	if ( currentInputMethod )
	{
		handle->inputContext = XCreateIC( currentInputMethod, XNClientWindow, native_window, XNFocusWindow, native_window, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL );
		if ( !handle->inputContext )
		{
			xwlPrintf( "[xwl::X11] Failed to create input context!\n" );
		}
	}

	return (void*)native_window;
} // x11_window_create_window


void x11_window_destroy_window( xwl_window_t * window )
{
	if ( window && window->handle )
	{
		XDestroyWindow( x11_current_display(), (Window)window->handle );
	}
} // x11_window_destroy_window

void x11_window_get_window_size( xwl_window_t * window, int * width, int * height )
{

} // x11_window_get_window_size

void x11_window_get_screen_size( unsigned int screen_index, int * width, int * height )
{

} // x11_window_get_screen_size

unsigned int x11_window_get_screen_count()
{
	return 0;
} // x11_window_get_screen_count



void x11_window_register( xwl_window_provider_t * wapi )
{
	wapi->startup = x11_window_startup;
	wapi->shutdown = x11_window_shutdown;
	wapi->create_window = x11_window_create_window;
	wapi->destroy_window = x11_window_destroy_window;
	wapi->get_window_size = x11_window_get_window_size;
	wapi->get_screen_size = x11_window_get_screen_size;
	wapi->get_screen_count = x11_window_get_screen_count;
} // x11_window_register
