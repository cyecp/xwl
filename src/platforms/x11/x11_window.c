#include <xwl/xwl.h>
#include <stdio.h>
// #include <X11/Xlib.h>
// #include <X11/keysym.h>
// #include <X11/extensions/Xrandr.h>
//#include <X11/XKBlib.h>

static Display *currentDisplay = 0;
static int currentScreen = 0;
static XIM currentInputMethod = 0;
static XComposeStatus currentKeyboardStatus;

#include <string.h>

Display * x11_current_display()
{
	return currentDisplay;
}

int x11_current_screen()
{
	return currentScreen;
}

XVisualInfo * x11_fetch_visual( int pixel_format )
{
	// Visual Info
	XVisualInfo * visual = 0;
	XVisualInfo visualTemplate;
	int visual_count;
	visualTemplate.visualid = pixel_format;

	// retrieve the visual
	fprintf( stdout, "[xwl] Retrieving visual (%i)... \n", pixel_format );
	visual = XGetVisualInfo( currentDisplay, VisualIDMask, &visualTemplate, &visual_count );
	if ( !visual )
	{
		xwl_set_error( "Unable to find visual!" );
		return 0;
	}

	fprintf( stdout, "[xwl] Found visual. visual_count = %i\n", visual_count );

	return visual;
} // x11_fetch_visual






//////////////////


int x11_window_startup( xwl_api_provider_t * api )
{
	fprintf( stdout, "x11_window_startup\n" );

	//Bool detectable;
	currentDisplay = XOpenDisplay( 0 );

	if ( currentDisplay )
	{
		currentScreen = DefaultScreen( currentDisplay );
		currentInputMethod = XOpenIM( currentDisplay, 0, 0, 0 );
	}
	else
	{
		xwl_set_error( "Unable to open an X display" );
		return 0;
	}

	// disable auto-repeats
	// "The standard behavior of the X server is to generate a KeyRelease event for every KeyPress event."
	// 
	//XkbSetDetectableAutorepeat( currentDisplay, True, &detectable );

	// install error handler
	//XSetErrorHandler( xwl_xserver_handler );

	return 1;
} // x11_window_startup

void x11_window_shutdown()
{
	if ( currentInputMethod )
	{
		XCloseIM( currentInputMethod );
		XCloseDisplay( currentDisplay );
		currentInputMethod = 0;
		currentDisplay = 0;
	}
} // x11_window_shutdown



void * x11_window_create_window( xwl_native_window_t * handle, const char * utf8_title, unsigned int * attributes, int pixel_format )
{
	// using X11, 'pixel_format' here refers to the visualID we want to create the window with.
	Window native_window = 0;
	XSetWindowAttributes window_attribs;
	int window_width;
	int window_height;

	XVisualInfo * visual = x11_fetch_visual( pixel_format );
	if ( !visual )
	{
		xwl_set_error( "Unable to fetch visual" );
		return 0;
	}

	// fill in atributes
	window_width = attributes[ XWL_WINDOW_WIDTH ];
	window_height = attributes[ XWL_WINDOW_HEIGHT ];

	// we now have the visual; move on to creating the window now...
	window_attribs.event_mask = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;

	fprintf( stdout, "[xwl] Create a color map... \n");
	window_attribs.colormap = XCreateColormap( currentDisplay, RootWindow(currentDisplay, currentScreen), visual->visual, AllocNone );


	fprintf( stdout, "[xwl] Attempting to create a window\n" );
	native_window = XCreateWindow( currentDisplay, RootWindow(currentDisplay, currentScreen), 0, 0, window_width, window_height, 0, visual->depth, InputOutput, visual->visual, CWColormap | CWEventMask, &window_attribs );


	// free the visual
	fprintf( stdout, "[xwl] Freeing visual...\n" );
	XFree( visual );





	//set window name
	// XStoreName is NOT Unicode aware! So basically, don't use anything except "Host Portable Character Encoding", (essentially, ASCII), or the result will be implementation-dependent.
	XStoreName( currentDisplay, native_window, utf8_title );

	// to work around XStoreName's shortcomings, we use XChangeProperty with _NEW_WM_NAME
	XChangeProperty( currentDisplay, native_window,
					XInternAtom(currentDisplay, "_NET_WM_NAME", False),
					XInternAtom(currentDisplay, "UTF8_STRING", False),
					8, PropModeReplace, (unsigned char*)utf8_title, strlen(utf8_title) );

	// show the window
	XMapWindow( currentDisplay, native_window );

	XFlush( currentDisplay );


	// standard init
	//myAtomClose = XInternAtom( currentDisplay, "WM_DELETE_WINDOW", 0 );
	//XSetWMProtocols( currentDisplay, handle, &myAtomClose, 1 );



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
