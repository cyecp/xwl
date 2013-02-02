#pragma once

#ifdef __cplusplus
extern "C" {
#endif


struct xwl_window_provider_s;
struct xwl_input_provider_s;
struct xwl_api_provider_s;
struct xwl_native_window_s;

// API Provider functions
typedef void *(*xwl_api_provider_create_context)( void * native_window, struct xwl_window_provider_s * wapi, unsigned int * attributes, void * other_context );
typedef void (*xwl_api_provider_destroy_context)( void * context, void * native_window, struct xwl_window_provider_s * wapi );
typedef void (*xwl_api_provider_activate_context)( void * context, void * native_window );
typedef void (*xwl_api_provider_swap_buffers)( void * native_window );

typedef struct xwl_api_provider_s
{
	xwl_api_provider_create_context create_context;
	xwl_api_provider_destroy_context destroy_context;
	xwl_api_provider_activate_context activate_context;
	xwl_api_provider_swap_buffers swap_buffers;
} xwl_api_provider_t;

// register functions for this provider
typedef void (*xwl_api_provider_register)( struct xwl_api_provider_s * api );

// on linux, there are several options with regards to setting up the graphics context
// Other platforms assume the default selection.
enum
{
	XWL_API_PROVIDER_INVALID,
	XWL_API_PROVIDER_DEFAULT,
	XWL_API_PROVIDER_EGL,
	XWL_API_PROVIDER_X11,
	XWL_API_PROVIDER_COCOA,
	XWL_API_PROVIDER_WIN32,
};


#ifdef __cplusplus
}; // extern "C"
#endif
