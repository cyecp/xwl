#include <xwl/xwl.h>
#import <CoreFoundation/CoreFoundation.h>

CFBundleRef _gl_bundle = 0;

int _xwl_open_driver_library( unsigned int api_provider )
{
#if TARGET_OS_IPHONE
	_gl_bundle = CFBundleGetBundleWithIdentifier( CFSTR( "com.apple.opengles" ) );
#else
	_gl_bundle = CFBundleGetBundleWithIdentifier( CFSTR( "com.apple.opengl" ) );
#endif

	return _gl_bundle != 0;
} // _xwl_open_driver_library

void _xwl_close_driver_library()
{
	if ( _gl_bundle != 0 )
	{
		CFRelease( _gl_bundle );
		_gl_bundle = 0;
	}
} // _xwl_close_driver_library

void * _xwl_apple_find_symbol( const char * name )
{
	if ( _gl_bundle )
	{
		CFStringRef symbol_name = CFStringCreateWithCString( kCFAllocatorDefault, name, kCFStringEncodingASCII );
		void * symbol = CFBundleGetFunctionPointerForName( _gl_bundle, symbol_name );
		CFRelease( symbol_name );
		return symbol;
	}
	return 0;
} // _xwl_apple_find_symbol