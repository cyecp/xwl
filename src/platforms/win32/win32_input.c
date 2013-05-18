#include <xwl/xwl.h>
#include <xwl/xwl_internal.h>
#include <xwl/platforms/win32/win32.h>
//#include <stdio.h>


static unsigned int lshift;

int win32_input_startup( void )
{
#if 0
	Display * display = win32_current_display();
	if ( !display )
	{
		xwl_set_error( "Unable to open win32 Display!" );
		return 0;
	}

	currentInputMethod = XOpenIM( win32_current_display(), 0, 0, 0 );
	if ( !currentInputMethod )
	{
		xwl_set_error( "Unable to open X Input Method!" );
		return 0;
	}
#endif

#ifdef _WIN32
	// initialize key map
	lshift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
#endif

	return 1;
} // win32_input_startup

void win32_input_shutdown( void )
{
} // win32_input_shutdown

int win32_input_dispatch_events( void )
{
	MSG msg;

	while ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return 0;
} // win32_input_dispatch_events


void win32_input_post_window_creation( xwl_native_window_t * native_window )
{

} // win32_input_post_window_creation


void win32_input_register( xwl_input_provider_t * input )
{
	input->startup = win32_input_startup;
	input->shutdown = win32_input_shutdown;
	input->dispatch_events = win32_input_dispatch_events;
	input->post_window_creation = win32_input_post_window_creation;
} // win32_input_register
