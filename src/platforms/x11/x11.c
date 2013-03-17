#include <xwl/xwl.h>
#include <xwl/platforms/x11/x11.h>
#include <stdio.h>

static Display * _xdisplay = 0;
static int currentScreen = 0;

Display * x11_current_display()
{
	if ( !_xdisplay )
	{
		_xdisplay = XOpenDisplay( 0 );
	}

	return _xdisplay;
} // x11_current_display

void x11_close_display()
{
	if ( _xdisplay )
	{
		XCloseDisplay( _xdisplay );
		_xdisplay = 0;
	}
} // x11_close_display

int x11_current_screen()
{
	return currentScreen;
} // x11_current_screen

void x11_set_current_screen( int screen )
{
	currentScreen = screen;
} // x11_set_current_screen

XVisualInfo * x11_fetch_visual( int pixel_format )
{
	// Visual Info
	XVisualInfo * visual = 0;
	XVisualInfo visualTemplate;
	int visual_count;
	visualTemplate.visualid = pixel_format;

	// retrieve the visual
	fprintf( stdout, "[xwl] Retrieving visual (%i)... \n", pixel_format );
	visual = XGetVisualInfo( x11_current_display(), VisualIDMask, &visualTemplate, &visual_count );
	if ( !visual )
	{
		xwl_set_error( "Unable to find visual!" );
		return 0;
	}

	fprintf( stdout, "[xwl] Found visual. visual_count = %i\n", visual_count );

	return visual;
} // x11_fetch_visual
