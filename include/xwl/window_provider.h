#pragma once

#ifdef __cplusplus
extern "C" {
#endif



struct xwl_window_provider_s;
struct xwl_input_provider_s;
struct xwl_api_provider_s;
struct xwl_native_window_s;


// Window Provider functions
// startup the window provider
typedef int (*xwl_window_provider_startup)( struct xwl_api_provider_s * api );

// shutdown the window provider
typedef void (*xwl_window_provider_shutdown)( void );

// create a window; return the native window handle on success
// returns 0 on failure
typedef void *(*xwl_window_provider_create_window)( struct xwl_native_window_s * handle, const char * utf8_title, unsigned int * attribs, int pixel_format );

// destroy a window
typedef void (*xwl_window_provider_destroy_window) ( xwl_window_t * window );

// return the window width and height
typedef void (*xwl_window_provider_get_window_size)( xwl_window_t * window, int * width, int * height );

// return the screen width and height
typedef void (*xwl_window_provider_get_screen_size)( unsigned int screen_index, int * width, int * height );

// return the number of screens
typedef unsigned int (*xwl_window_provider_get_screen_count)();


// register functions for this provider
typedef void (*xwl_window_provider_register)( struct xwl_window_provider_s * wapi );


typedef struct xwl_window_provider_s
{
	xwl_window_provider_startup startup;
	xwl_window_provider_shutdown shutdown;
	
	xwl_window_provider_create_window create_window;
	xwl_window_provider_destroy_window destroy_window;
	
	xwl_window_provider_get_window_size get_window_size;
	xwl_window_provider_get_screen_size get_screen_size;
	xwl_window_provider_get_screen_count get_screen_count;
} xwl_window_provider_t;

// on linux, there are several options with regards to creating the window
// Other platforms always assume the default selection.
enum
{
	XWL_WINDOW_PROVIDER_INVALID,
	XWL_WINDOW_PROVIDER_DEFAULT,
	XWL_WINDOW_PROVIDER_EGL,
	XWL_WINDOW_PROVIDER_X11,
	XWL_WINDOW_PROVIDER_WAYLAND,
	XWL_WINDOW_PROVIDER_COCOA,
	XWL_WINDOW_PROVIDER_WIN32,
	XWL_WINDOW_PROVIDER_RASPBERRYPI
};

#ifdef __cplusplus
}; // extern "C"
#endif
