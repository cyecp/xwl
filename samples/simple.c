#include <stdio.h>
#include <xwl/xwl.h>
#include <string.h>

int running = 1;

i32 mx;
i32 my;


// temporary OpenGL tests
#if _WIN32
#include <windows.h>
#include <gl/gl.h>
#pragma comment( lib, "opengl32.lib" )
#elif LINUX
#include <GL/gl.h>
#include <GL/glx.h>
#elif __APPLE__
// for OSX 10.6
#include <OpenGL/OpenGL.h>
// for OSX 10.7+
#include <OpenGL/gl.h>
#endif


void callback( xwl_event_t * e )
{
    xwlPrintf( "event: %s\n", xwl_event_to_string(e->type) );

	if ( e->type == XWLE_GAINFOCUS )
	{
		//xwlPrintf( "Gain Focus\n" );
	}
	else if ( e->type == XWLE_LOSTFOCUS )
	{
		//xwlPrintf( "Lost Focus\n" );
	}
	else if ( e->type == XWLE_MOUSEWHEEL )
	{
		xwlPrintf( "\t-> wheel delta: %i\n", e->wheelDelta );
	}
	else if ( e->type == XWLE_KEYPRESSED || e->type == XWLE_KEYRELEASED )
	{
		xwlPrintf( "\t-> unicode: %i\n", e->unicode );
		xwlPrintf( "\t-> keymods: %i\n", e->keymods );
		xwlPrintf( "\t-> key: %i (%s)\n", e->key, xwl_key_to_string(e->key) );
	}
	else if ( e->type == XWLE_MOUSEBUTTON_PRESSED || e->type == XWLE_MOUSEBUTTON_RELEASED )
	{
		xwlPrintf( "\t-> button: %s\n", xwl_mouse_to_string(e->button) );
	}
	else if ( e->type == XWLE_MOUSEMOVE )
	{
		xwlPrintf( "\t-> pos: (%i, %i)\n", e->mx, e->my );
		mx = e->mx;
		my = e->my;
	}
	else if ( e->type == XWLE_SIZE )
	{
//		p.width = e->width;
//		p.height = e->height;
		xwlPrintf( "\t-> width: %i\n", e->width );
		xwlPrintf( "\t-> height: %i\n", e->height );
	}
	else if ( e->type == XWLE_CLOSED )
	{
		xwlPrintf( "Closed the window\n" );
		running = 0;
	}
	else if ( e->type == XWLE_TEXT )
	{
		xwlPrintf( "Text; %i\n", e->unicode );

	}


	if ( e->type == XWLE_KEYRELEASED && e->key == XWLK_ESCAPE )
	{
		running = 0;
	}
}

int main()
{
	xwl_window_t *w = 0;
	int window_width = 800;
	int window_height = 600;
	unsigned int screen_count = 0;
	unsigned int i;
	int screen_width;
	int screen_height;
    u32 attribs[] = {
        XWL_API, XWL_API_OPENGL,
//        XWL_API_MAJOR_VERSION, 3,
//        XWL_API_MINOR_VERSION, 2,
        XWL_WINDOW_WIDTH, window_width,
        XWL_WINDOW_HEIGHT, window_height,
//		XWL_USE_FULLSCREEN, 1,
        XWL_NONE,
    };
	
	
	if ( !xwl_startup( XWL_WINDOW_PROVIDER_DEFAULT, XWL_API_PROVIDER_DEFAULT, XWL_INPUT_PROVIDER_DEFAULT ) )
	{
		xwlPrintf( "xwl_startup failed: '%s'\n", xwl_get_error() );
		return -1;
	}
	
	screen_count = xwl_get_screen_count();
	xwlPrintf( "-> Total Screens: %i\n", screen_count );
	
	for( i = 0; i < screen_count; ++i )
	{

		xwl_get_screen_size( i, &screen_width, &screen_height );
		
		xwlPrintf( "-> screen %i, size: %i x %i\n", i, screen_width, screen_height );
	}
	
//	attribs[3] = screen_width;
//	attribs[5] = screen_height;
	
	
    xwlPrintf( "-> xwl_create_window...\n" );
	w = xwl_create_window( "Window Title Here \xc3\xb1 | \xe2\x82\xa1", attribs );
	if ( !w )
	{
		xwlPrintf( "ERROR: Unable to create window! [%s]\n", xwl_get_error() );
		return -1;
	}
	xwlPrintf( "-> Window created OK! (handle: %x)\n", (u32)w->handle );
	
	xwl_get_window_size( w, &window_width, &window_height );
	xwlPrintf( "-> Actual Window dimensions: %i x %i\n", window_width, window_height );
	

	
	// set event callback
	xwl_set_callback( callback );

#if 1
	xwlPrintf( "-> GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );
	xwlPrintf( "-> GL_RENDERER: %s\n", glGetString( GL_RENDERER ) );
	xwlPrintf( "-> GL_VERSION: %s\n", glGetString( GL_VERSION ) );
#endif

	while( running )
	{
		xwl_dispatch_events();
#if 1
		glClearColor(0.25, 0.25, 0.25, 1.0);
		glClear( GL_COLOR_BUFFER_BIT );
		glViewport( 0, 0, window_width, window_height );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glOrtho( 0, window_width, window_height, 0, -.01f, 256.0f );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();


		glPointSize( 4.0 );
		glColor3ub( 0, 255, 0 );

		glBegin( GL_POINTS );
			glVertex3i( mx, my, 0 );
		glEnd();
#endif

		xwl_swap_buffers( w );
	}

	xwl_shutdown();
	return 0;
} // main
