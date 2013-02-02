#pragma once
#include <xwl/xwl.h>

// providers for this platform
void cocoa_register( xwl_window_provider_t * wapi );
void cocoa_opengl_register( xwl_api_provider_t * api );