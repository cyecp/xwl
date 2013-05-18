#pragma once
#include <xwl/xwl.h>

// providers for this platform
void win32_window_register( xwl_window_provider_t * wapi );
void win32_opengl_register( xwl_api_provider_t * api );
void win32_input_register( xwl_input_provider_t * iapi );


int win32_opengl_setup_pixelformat( HWND window );
