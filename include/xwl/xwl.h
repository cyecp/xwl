#pragma once

#ifdef __cplusplus
extern "C" {
#endif
    
    // platform includes
#if LINUX
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/Xrandr.h>
#elif _WIN32
#   ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501
#endif
    
#include <windows.h>
#ifndef MAPVK_VK_TO_VSC
    #define MAPVK_VK_TO_VSC 0
#endif
    
#elif __APPLE__
    #include <TargetConditionals.h>
#endif
    
#define XWL_MAX_TOUCHES 5
#define XWL_MAX_WINDOW_ATTRIBS 10
    
#ifndef Z_TYPES
	typedef signed char i8;
	typedef unsigned char u8;
	typedef unsigned int u32;
	typedef unsigned short u16;
	typedef short i16;
	typedef int i32;
#endif
    
#define XWL_DEBUG 1
#if XWL_DEBUG
    #define xwlPrintf printf
#else
    #define xwlPrintf //
#endif
    
    
#include "constants.h"

typedef struct xwl_window_s
{
    void * handle;
} xwl_window_t;

struct xwl_api_provider_s;
struct xwl_window_provider_s;

typedef void (*xwl_window_provider_startup)( struct xwl_api_provider_s * api );
typedef void (*xwl_window_provider_shutdown)( void );

typedef xwl_window_t (*xwl_window_provider_create_window)( const char * utf8_title, unsigned int * attribs );
typedef void (*xwl_window_provier_destroy_window) ( xwl_window_t window );

typedef void (*xwl_window_provider_register)( struct xwl_window_provider_s * wapi );


typedef struct xwl_window_provider_s
{
    xwl_window_provider_startup startup;
    xwl_window_provider_shutdown shutdown;
	
	xwl_window_provider_create_window create_window;
	xwl_window_provier_destroy_window destroy_window;
} xwl_window_provider_t;

typedef struct xwl_api_provider_s
{
    
} xwl_api_provider_t;
    
    
    
    struct xwl_event_s;
    typedef void (*xwl_event_callback)( struct xwl_event_s * );
	
#if 0
    typedef struct xwl_window_s
    {
        void * handle;
        void * userdata;
        xwl_event_callback event_callback;
        
#if _WIN32
        HDC dc;
#endif
        
#if LINUX
        void * context;
#endif
        
#if __APPLE__
        void * view;
#endif
    } xwl_window_t;
#endif
    typedef struct xwl_windowparams_s
    {
        u32 width;
        u32 height;
        u32 flags;
        u32 x;
        u32 y;
        void * userdata;
        char * title;
        
#if _WIN32
        HICON hIcon;
        HICON hIconSm;
#endif
    } xwl_windowparams_t;
	
    typedef struct
    {
        float x;
        float y;
        
    } xwl_touch_t;
    
    typedef struct xwl_event_s
    {
        // target window
        xwl_window_t *target;
        
        // key
        i32 key;
        i32 unicode;
        
        // size dimension
        i32 width;
        i32 height;
        
        // mouse pos
        i32 mx;
        i32 my;
        
        // joystick
        i16 joyid;
        i16 joyaxis;
        i16 joybutton;
        float joypos;
        
        // event types
        u16 type;
        
        // -1 is towards the user
        // 1 is away from the user
        i16 wheelDelta;
        i16 button;
        i16 keymods;
        
        xwl_touch_t touches[ XWL_MAX_TOUCHES ];
        struct xwl_window_s * window;
    } xwl_event_t;
    
	
    // returns 0 on failure
    // returns 1 on success
    i32 xwl_startup( unsigned int window_provider, unsigned int api_provider );
    
    // shutdown system
    void xwl_shutdown( void );
    
    // returns 0 if no events are queued
    // returns 1 if an event is removed from the queue
    i32 xwl_pollevent( xwl_event_t *event );
    
    // returns 0 on failure
    // title is a UTF-8 encoded string
    xwl_window_t *xwl_create_window( const char * utf8_title, u32 * attribs );
    
    // set the event callback
    void xwl_set_callback( xwl_event_callback cb );
    
    const char * xwl_key_to_string( i32 key );
    const char * xwl_event_to_string( i32 event_type );
    const char * xwl_mouse_to_string( i32 mouse );
    
    const char * xwl_get_error( void );
	void xwl_set_error( const char * error );
    
    typedef struct xwl_window_handle_s
    {
        xwl_window_t handle;
        
#ifdef LINUX
        XIC inputContext;
        Atom atomClose;
        XEvent lastKeyRelease;
#endif
        
    } xwl_window_handle_t;
    
    xwl_window_handle_t *xwl_get_unused_window( void );
    void xwl_send_event( xwl_event_t * ev );
    void xwl_setup_rendering( xwl_window_t * window, u32 * attribs );
    void xwl_finish( void );
    void xwl_activate( xwl_window_t * window ); // activate this window's rendering context
    void *xwl_rendering_context( xwl_window_t * window );
    
    // -- platform specifics
    typedef struct xwl_renderer_settings_s
    {
        xwl_window_t * window;
        
#if LINUX
        XVisualInfo * visual;
        Display * display;
        i32 screen;
#endif
        
    } xwl_renderer_settings_t;
    
    
#ifdef __cplusplus
}; // extern "C"
#endif
