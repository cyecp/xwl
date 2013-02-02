#include <xwl/xwl.h>

// #include <X11/Xlib.h>
// #include <X11/keysym.h>
// #include <X11/extensions/Xrandr.h>

int x11_input_startup( void )
{
	return 1;
}

void x11_input_shutdown( void )
{

}

int x11_input_dispatch_events( void )
{
	return 0;
}


void x11_input_register( xwl_input_provider_t * input )
{
	input->startup = x11_input_startup;
	input->shutdown = x11_input_shutdown;
	input->dispatch_events = x11_input_dispatch_events;
}