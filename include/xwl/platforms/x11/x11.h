#pragma once
#include <xwl/xwl.h>

// providers for this platform
void x11_window_register( xwl_window_provider_t * wapi );
void x11_opengl_register( xwl_api_provider_t * api );
void x11_input_register( xwl_input_provider_t * iapi );