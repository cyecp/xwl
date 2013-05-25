#pragma once
#include <xwl/xwl.h>


// providers for this platform
void win32_window_register( xwl_window_provider_t * wapi );
void win32_opengl_register( xwl_api_provider_t * api );
void win32_input_register( xwl_input_provider_t * iapi );


int win32_opengl_setup_pixelformat( xwl_native_window_t * native_window, HWND window, unsigned int * attribs );
int win32_load_symbols( HWND window );


typedef BOOL (__stdcall *pfnChoosePixelFormatARB)(HDC hdc,
                                const int *piAttribIList,
                                const FLOAT *pfAttribFList,
                                UINT nMaxFormats,
                                int *piFormats,
                                UINT *nNumFormats);

typedef HGLRC (__stdcall *pfnCreateContextAttribsARB)(HDC hDC, HGLRC hshareContext, const int *attribList);

extern pfnChoosePixelFormatARB _win32ChoosePixelFormatARB;
extern pfnCreateContextAttribsARB _win32CreateContextAttribsARB;