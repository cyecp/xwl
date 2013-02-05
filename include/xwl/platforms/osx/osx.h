#pragma once
#include <xwl/xwl.h>

// providers for this platform
void cocoa_register( xwl_window_provider_t * wapi );
void cocoa_api_register( xwl_api_provider_t * api );
void cocoa_input_register( xwl_input_provider_t * iapi );