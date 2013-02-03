#include <xwl/xwl.h>
#include <string.h>

#if RASPBERRYPI
	#include <bcm_host.h>
#else
	#error This should only be compiled if RASPBERRYPI is defined as a value greater than zero.
#endif

// Most of the code below has come from the hellow_triangle example for the Pi.
// The Dispmanx code will have to be replaced with OpenWF, if and when 
// elinux.org is correct about it replacing Dispmanx.

// really only one of these on the pi.
static EGL_DISPMANX_WINDOW_T native_window;
static unsigned int window_width;
static unsigned int window_height;

//VCHI_INSTANCE_T vchi_instance;

int rpi_window_startup( xwl_api_provider_t * api )
{
	int success = 0;

	fprintf( stdout, "rpi_window_startup\n" );

	// init the broadcom device
	bcm_host_init();


	// initialize vcos and vchi
//	vcos_init();

	success = graphics_get_display_size( 0 /* LCD */, &window_width, &window_height );
	if ( success < 0 )
	{
		xwl_set_error( "Unable to get the display size!" );
		return 0;
	}

	return 1;
} // rpi_window_startup

void rpi_window_shutdown()
{
} // rpi_window_shutdown


void * rpi_window_create_window( xwl_native_window_t * handle, const char * utf8_title, unsigned int * attributes, int pixel_format )
{
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;

	dst_rect.x = 0;
	dst_rect.y = 0;

	dst_rect.width = window_width;
	dst_rect.height = window_height;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = window_width << 16;
	src_rect.height = window_height << 16;

	dispman_display = vc_dispmanx_display_open( 0 /* LCD */ );
	dispman_update = vc_dispmanx_update_start( 0 );

	dispman_element = vc_dispmanx_element_add( dispman_update, dispman_display,
		0, /* layer */, &dst_rect, 0 /* src */,
		&src_rect, DISPMANX_PROTECTION_NONE, 0 /* alpha */,
		0, /* clamp */, 0, /* transform */ );

	native_window.element = dispman_element;
	native_window.width = window_width;
	native_window.height = window_height;
	vc_dispmanx_update_submit_sync( dispman_update );

	return (void*)&native_window;
} // rpi_window_create_window


void rpi_window_destroy_window( xwl_window_t * window )
{

} // rpi_window_destroy_window

void rpi_window_get_window_size( xwl_window_t * window, int * width, int * height )
{

} // rpi_window_get_window_size

void rpi_window_get_screen_size( unsigned int screen_index, int * width, int * height )
{

} // rpi_window_get_screen_size

unsigned int rpi_window_get_screen_count()
{
	return 0;
} // rpi_window_get_screen_count



void rpi_window_register( xwl_window_provider_t * wapi )
{
	wapi->startup = rpi_window_startup;
	wapi->shutdown = rpi_window_shutdown;
	wapi->create_window = rpi_window_create_window;
	wapi->destroy_window = rpi_window_destroy_window;
	wapi->get_window_size = rpi_window_get_window_size;
	wapi->get_screen_size = rpi_window_get_screen_size;
	wapi->get_screen_count = rpi_window_get_screen_count;
} // rpi_window_register
