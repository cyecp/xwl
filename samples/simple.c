#include <stdio.h>
#include <xwl/xwl.h>
#include <string.h>

int running = 1;

i32 mx;
i32 my;
xwl_windowparams_t p;

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
		p.width = e->width;
		p.height = e->height;
		xwlPrintf( "\t-> width: %i\n", p.width );
		xwlPrintf( "\t-> height: %i\n", p.height );
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
		running = 0;
}

int main()
{
	xwl_window_t *w = 0;
	xwl_event_t event;
	u32 attribs[] = { XWL_GL_DOUBLEBUFFER, XWL_GL_PROFILE, XWL_GLPROFILE_LEGACY, 0 };
    p.flags = XWL_OPENGL;

#if 0
	p.width = 1920;
	p.height = 1080;
	p.flags |= XWL_FULLSCREEN;
#else
	p.width = 800;
	p.height = 600;
	p.flags |= XWL_WINDOWED;
#endif

	xwl_startup();

    xwlPrintf( "-> xwl_create_window...\n" );
	w = xwl_create_window( &p, "Window Title Here \xc3\xb1 | \xe2\x82\xa1", attribs );

	if ( !w )
	{
		xwlPrintf( "ERROR: Unable to create window! [%s]\n", xwl_get_error() );
		return -1;
	}
	xwlPrintf( "-> Window created OK! (handle: %x)\n", (u32)w->handle );
	xwlPrintf( "-> Actual Window dimensions: %i x %i\n", p.width, p.height );

	// set event callback
	xwl_set_callback( callback );

#if 1
	xwlPrintf( "-> GL_VENDOR: %s\n", glGetString( GL_VENDOR ) );
	xwlPrintf( "-> GL_RENDERER: %s\n", glGetString( GL_RENDERER ) );
	xwlPrintf( "-> GL_VERSION: %s\n", glGetString( GL_VERSION ) );
#endif


	memset( &event, 0, sizeof(xwl_event_t) );
	while( running )
	{
		xwl_pollevent( &event );
		//callback( &event );

		glClearColor(0.25, 0.25, 0.25, 1.0);
		glClear( GL_COLOR_BUFFER_BIT );
		glViewport( 0, 0, p.width, p.height );

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();

		glOrtho( 0, p.width, p.height, 0, -.01f, 256.0f );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();


		glPointSize( 4.0 );
		glColor3ub( 0, 255, 0 );

		glBegin( GL_POINTS );
			glVertex3i( mx, my, 0 );
		glEnd();

		xwl_finish();
	}

	xwl_shutdown();
	return 0;
} // main
