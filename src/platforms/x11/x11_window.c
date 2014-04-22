#include <xwl/xwl.h>
#include <stdio.h>
#include <xwl/platforms/x11/x11.h>
#include <string.h>

int x11_window_startup( xwl_api_provider_t * api )
{
	//Bool detectable;
	if ( x11_current_display() )
	{
		x11_set_current_screen( DefaultScreen( x11_current_display() ) );
	}
	else
	{
		xwl_set_error( "Unable to open an X display" );
		return 0;
	}

	_Xdebug = 1;
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
	x11_close_display();
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

	// fprintf( stdout, "[xwl] Create a color map... \n");
	window_attribs.colormap = XCreateColormap( x11_current_display(), RootWindow(x11_current_display(), x11_current_screen()), visual->visual, AllocNone );

	cwmask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	// fprintf( stdout, "[xwl] Attempting to create a window\n" );
	// fprintf( stdout, "root: %p\n", (void*)RootWindow(x11_current_display(), x11_current_screen()) );
	// fprintf( stdout, "display: %p\n", x11_current_display() );
	// fprintf( stdout, "screen: %i\n", x11_current_screen() );
	// fprintf( stdout, "window_width: %i\n", window_width );
	// fprintf( stdout, "window_height: %i\n", window_height );
	// fprintf( stdout, "visual->visual: %p\n", visual->visual );
	// fprintf( stdout, "visual->depth: %i\n", visual->depth );
	// fprintf( stdout, "cwmask: %i\n", cwmask );
	native_window = XCreateWindow( x11_current_display(), RootWindow(x11_current_display(), x11_current_screen()), 0, 0, window_width, window_height, 0, visual->depth, InputOutput, visual->visual, cwmask, &window_attribs );
	// fprintf( stdout, "[xwl] native_window = %i\n", (int)native_window );

	// free the visual
	// fprintf( stdout, "[xwl] Freeing visual...\n" );
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

		// fprintf( stdout, "[xwl] converting window to fullscreen...\n" );
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
	Window native_window = (Window)window->handle;
	XWindowAttributes attributes;
	XGetWindowAttributes( x11_current_display(), native_window, &attributes );

	*width = attributes.width;
	*height = attributes.height;

} // x11_window_get_window_size

void x11_window_get_window_render_size( xwl_window_t * window, int * width, int * height )
{
	// according to the man page for XGetWindowAttributes and XGetGeometry
	// I could use XGetGeometry to retrieve the drawable dimensions for the window.

	// At the moment, I believe the correct values to return here the actual window dimensions so I will just call that.
	x11_window_get_window_size( window, width, height );
} // x11_window_get_window_render_size

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
	wapi->get_window_render_size = x11_window_get_window_render_size;
	wapi->get_screen_size = x11_window_get_screen_size;
	wapi->get_screen_count = x11_window_get_screen_count;
} // x11_window_register


#if 0

#if __linux__ && 0
	#include <stdio.h>
	#include <X11/Xlib.h>
	//#include <X11/extensions/Xrandr.h>
	#include <cstdlib>
	#include <X11/extensions/Xinerama.h>

		// If TwinView is enabled, Linux only sees 1 display and 1 screen.
		// the resolution ends up being a combination of both screen sizes, which is not what we want.

		// -lXinerama
		// http://stackoverflow.com/questions/836086/programmatically-determining-individual-screen-widths-heights-in-linux-w-xineram
		Display *d=XOpenDisplay(NULL);

		if (d)
		{
			int dummy1, dummy2;
			if (XineramaQueryExtension(d, &dummy1, &dummy2)) {
				if (XineramaIsActive(d)) {
					int heads=0;
					XineramaScreenInfo *p=XineramaQueryScreens(d, &heads);
					if (heads>0) {
						for (int x=0; x<heads; ++x)
						{
							/*
							cout << "Head " << x+1 << " of " << heads << ": " <<
								p[x].width << "x" << p[x].height << " at " <<
								p[x].x_org << "," << p[x].y_org << endl;	*/

							// Only interested in the first display for now, we should support others.
							mode.width = p[x].width;
							mode.height = p[x].height;
							fprintf( stderr, "[xwl] Width: %i, Height: %i\n", mode.width, mode.height );
							break;
						}
					}
					else
					{
						fprintf( stderr, "[xwl] XineramaQueryScreens says there aren't any\n" );
					}


					XFree(p);
				}
				else
				{
					fprintf( stderr, "[xwl] Xinerama not active\n" );
				}

			}
			else
			{
				fprintf( stderr, "[xwl] No Xinerama extension\n" );
			}

			XCloseDisplay(d);
		}
#endif
#endif