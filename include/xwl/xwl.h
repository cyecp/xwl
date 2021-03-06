#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	
// platform includes
#if __linux__
	#if XWL_WITH_X11
		#include <X11/Xlib.h>
		#include <X11/keysym.h>
		#include <X11/extensions/Xrandr.h>
	#endif

	#define XWL_EXPORT
#elif _WIN32
	#ifdef XWL_DLL
		#define XWL_EXPORT __declspec(dllexport)
	#else
		#define XWL_EXPORT __declspec(dllimport)
	#endif

	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x0501
	#endif
    
	#include <windows.h>	
	#ifndef MAPVK_VK_TO_VSC
		#define MAPVK_VK_TO_VSC 0
	#endif
    
#elif __APPLE__
    #include <TargetConditionals.h>
	#define XWL_EXPORT
#endif
    
#define XWL_DEBUG 1
#if XWL_DEBUG
    #define xwlPrintf printf
#else
    #define xwlPrintf //
#endif
    
#define XWL_MAX_WINDOW_HANDLES 4
#define XWL_NOTSET 0xFFFFFFF0

enum
{
	XWL_NONE,
	
	// enum attributes
	XWL_API,
	XWL_API_PROVIDER,
	XWL_WINDOW_PROVIDER,
	
	// integer attributes
	XWL_API_MAJOR_VERSION,
	XWL_API_MINOR_VERSION,
    XWL_API_SAMPLES,
    XWL_API_SAMPLEBUFFERS,

	XWL_DEPTH_SIZE,
	XWL_COLOR_SIZE,
	XWL_ALPHA_SIZE,
	XWL_STENCIL_SIZE,
	XWL_WINDOW_WIDTH,
	XWL_WINDOW_HEIGHT,
	XWL_WINDOW_X,
	XWL_WINDOW_Y,
	
#if _WIN32
	XWL_WIN32_ICON,
	XWL_WIN32_ICONSM,
#endif
	
	// boolean attributes
	XWL_USE_FULLSCREEN,
	XWL_DISABLE_RESIZE,
	
	XWL_ATTRIBUTE_COUNT
};

enum
{
	XWL_API_INVALID,
	XWL_API_OPENGL,
	XWL_API_GLES2,
	XWL_API_GLES3
};

#include "keys.h"

typedef struct xwl_window_s
{
    void * handle;
    void * context;
    int pixel_format;
    unsigned int id;
} xwl_window_t;

    
#include <xwl/window_provider.h>
#include <xwl/api_provider.h>
#include <xwl/input_provider.h>
	
struct xwl_event_s;
typedef void (*xwl_event_callback)( struct xwl_event_s * );
	

typedef struct xwl_event_s
{
    // target window
    xwl_window_t *target;
        
    // key
    int key;
    int unicode;
        
    // size dimension
    int width;
    int height;
        
    // mouse pos
    int mx;
    int my;
        
    // joystick
    short joyid;
    short joyaxis;
    short joybutton;
    float joypos;
        
    // event types
    unsigned short type;
        
    // -1 is towards the user
    // 1 is away from the user
    short wheelDelta;
    short button;
    short keymods;

    struct xwl_window_s * window;
} xwl_event_t;


typedef struct xwl_native_window_s
{
    xwl_window_t handle;
        
#ifdef __linux__
    #if XWL_WITH_X11
	    XIC inputContext;
	    Atom atomClose;
	    XEvent lastKeyRelease;
   #endif
#endif

#if _WIN32
    HDC dc;
#endif
} xwl_native_window_t;


// returns 0 on failure
// returns 1 on success
XWL_EXPORT int xwl_startup( unsigned int window_provider, unsigned int api_provider, unsigned int input_provider );
    
// shutdown system
XWL_EXPORT void xwl_shutdown( void );
    
// returns 0 if no events are queued
// returns > 0 if one or more events were dispatched
XWL_EXPORT int xwl_dispatch_events();
    
// returns 0 on failure
// title is a UTF-8 encoded string
XWL_EXPORT xwl_window_t *xwl_create_window( const char * utf8_title, unsigned int * attribs );
    
// set the event callback
XWL_EXPORT void xwl_set_callback( xwl_event_callback cb );
    
XWL_EXPORT const char * xwl_key_to_string( int key );
XWL_EXPORT const char * xwl_event_to_string( int event_type );
XWL_EXPORT const char * xwl_mouse_to_string( int mouse );
    
XWL_EXPORT const char * xwl_get_error( void );
XWL_EXPORT void xwl_set_error( const char * error );
	
//	void * xwl_get_native_window( xwl_window_t * window );
//	void * xwl_get_api_context( xwl_window_t * window );
XWL_EXPORT void xwl_get_window_size( xwl_window_t * window, int * width, int * height );
XWL_EXPORT void xwl_get_window_render_size( xwl_window_t * window, int * width, int * height );
XWL_EXPORT void xwl_get_screen_size( unsigned int screen_index, int * width, int * height );
XWL_EXPORT unsigned int xwl_get_screen_count();
            
XWL_EXPORT void xwl_finish( void );
XWL_EXPORT void xwl_swap_buffers( xwl_window_t * window );
   
// must call this before xwl_startup otherwise findsymbol will return 0 for all symbols.
XWL_EXPORT void xwl_use_findsymbol( void );

XWL_EXPORT void * xwl_findsymbol( const char * symbol_name );
    
    
#ifdef __cplusplus
}; // extern "C"
#endif
