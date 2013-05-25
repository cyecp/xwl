#include <xwl/xwl.h>
#include <xwl/xwl_internal.h>
#include <xwl/platforms/win32/win32.h>


pfnChoosePixelFormatARB _win32ChoosePixelFormatARB = 0;
pfnCreateContextAttribsARB _win32CreateContextAttribsARB = 0;