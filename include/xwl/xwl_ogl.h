#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int xwl_renderer_startup( xwl_renderer_settings_t * settings, unsigned int * attribs );
void xwl_renderer_post( xwl_renderer_settings_t * settings );
void xwl_renderer_shutdown( xwl_renderer_settings_t * settings );
void xwl_renderer_activate( xwl_renderer_settings_t * settings );


#ifdef __cplusplus
}; // extern "C"
#endif