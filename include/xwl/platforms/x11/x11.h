#pragma once
#include <xwl/xwl.h>

// providers for this platform
void x11_window_register( xwl_window_provider_t * wapi );
void x11_opengl_register( xwl_api_provider_t * api );
void x11_input_register( xwl_input_provider_t * iapi );


// accessible to all providers which interact with X11
Display * x11_current_display();
int x11_current_screen();
void x11_set_current_screen( int screen );
void x11_close_display();

// the data returned from this function must be freed with XFree()
XVisualInfo * x11_fetch_visual( int pixel_format );

