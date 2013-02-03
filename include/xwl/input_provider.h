#pragma once

#ifdef __cplusplus
extern "C" {
#endif


struct xwl_window_provider_s;
struct xwl_input_provider_s;
struct xwl_api_provider_s;
struct xwl_native_window_s;

// Input functions
typedef int (*xwl_input_provider_startup)( void );
typedef void (*xwl_input_provider_shutdown)( void );

// returns > 0 if one or more events were processed
typedef int (*xwl_input_provider_dispatch_events)( void );
	
typedef struct xwl_input_provider_s
{
	xwl_input_provider_startup startup;
	xwl_input_provider_shutdown shutdown;
	xwl_input_provider_dispatch_events dispatch_events;	
} xwl_input_provider_t;

// register functions for this provider
typedef void (*xwl_input_provider_register)( struct xwl_input_provider_s * api );

	
// this provides dispatch for events; can be different from how we get the native window on some systems
enum
{
	XWL_INPUT_PROVIDER_INVALID,
	XWL_INPUT_PROVIDER_DEFAULT,
	XWL_INPUT_PROVIDER_X11,
	XWL_INPUT_PROVIDER_COCOA,
	XWL_INPUT_PROVIDER_WIN32
};

#ifdef __cplusplus
}; // extern "C"
#endif
