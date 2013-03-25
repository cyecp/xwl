#include <xwl/xwl.h>
#include <stdio.h>
#include <EGL/egl.h>

static EGLDisplay display;
static EGLConfig config;
static EGLSurface surface;

EGLint egl_check_error()
{
	EGLint error = eglGetError();
	if ( error != EGL_SUCCESS )
	{
		fprintf( stdout, "eglGetError returned %i\n", error );
	}

	return error;
}

void *egl_api_create_context( xwl_native_window_t * native_window, xwl_window_provider_t * wapi, unsigned int * attributes, void * other_context )
{
	EGLint api_type = EGL_OPENGL_ES_API;
	EGLContext context;
	EGLBoolean result;
	int major_api_version = 2;

	EGLint context_attribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	EGLint value;


// Until EGL works with plain old OpenGL, disable this.
#if 0
	if ( attributes[ XWL_API_MAJOR_VERSION ] )
	{
		major_api_version = attributes[ XWL_API_MAJOR_VERSION ];
		context_attribs[ 1 ] = major_api_version;
	}

	if ( attributes[ XWL_API ] == XWL_API_OPENGL )
	{
		api_type = EGL_OPENGL_API;
	}

#endif

	egl_check_error();

	result = eglBindAPI( api_type );
	if ( result == EGL_FALSE )
	{
		xwl_set_error( "eglBindAPI failed" );
		return 0;
	}

	// so far, this will not succeed with EGL_OPENGL_BIT set in the attribs.
	context = eglCreateContext( display, config, EGL_NO_CONTEXT, context_attribs );
	if ( context == EGL_NO_CONTEXT )
	{
		xwl_set_error( "eglCreateContext failed" );
		return 0;
	}

	eglQueryContext( display, context, EGL_CONTEXT_CLIENT_VERSION, &value);
	if ( value != major_api_version )
	{
		xwl_set_error( "Context returned a different version than requested!" );
		eglDestroyContext( display, context );
		return 0;
	}

	surface = eglCreateWindowSurface( display, config, (EGLNativeWindowType)native_window->handle.handle, 0 );
	if ( surface == EGL_NO_SURFACE )
	{
		xwl_set_error( "eglCreateWindowSurface failed" );
		return 0;
	}

	fprintf( stdout, "egl_api_create_context: %p\n", context );
	return context;
} // egl_api_create_context

void egl_api_destroy_context( void * context, xwl_native_window_t * native_window, struct xwl_window_provider_s * wapi )
{
	EGLBoolean result;
	eglMakeCurrent( display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );

	result = eglDestroySurface( display, surface );
	if ( result != EGL_FALSE )
	{
		fprintf( stdout, "Destroyed Window surface\n" );
	}

	result = eglDestroyContext( display, context );
	if ( result != EGL_FALSE )
	{
		fprintf( stdout, "Destroyed Context\n" );
	}
} // egl_api_destroy_context

void egl_api_activate_context( void * context, xwl_native_window_t * native_window )
{
	EGLBoolean result;
	fprintf( stdout, "egl_api_activate_context\n" );

	result = eglMakeCurrent( display, surface, surface, (EGLContext)context );
	if ( result == EGL_FALSE )
	{
		fprintf( stderr, "Error making egl context active!\n" );
	}
} // egl_api_activate_context

void egl_api_swap_buffers( xwl_native_window_t * window )
{
	eglSwapBuffers( display, surface );
} // egl_api_swap_buffers

int egl_api_pixel_format( unsigned int * attribs )
{
	// this should be placed into a startup function
	int major;
	int minor;

	EGLBoolean result;
	EGLint num_configs;
	EGLint visual_id;
	const char * eglstring;

	EGLint eglattribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		//EGL_CONFIG_CAVEAT, EGL_NONE,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_DEPTH_SIZE, 16,
		EGL_NONE
	};

	egl_check_error();

	display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	if ( display == EGL_NO_DISPLAY )
	{
		xwl_set_error( "eglGetDisplay failed" );
		return -1;
	}

	// initialize egl
	result = eglInitialize( display, &major, &minor );
	if ( result == EGL_FALSE )
	{
		xwl_set_error( "eglInitialize failed" );
		return -1;
	}

	fprintf( stdout, "Initialized EGL %i.%i\n", major, minor );

	// query configurations
	result = eglChooseConfig( display, eglattribs, &config, 1, &num_configs );
	if ( result == EGL_FALSE )
	{
		xwl_set_error( "eglChooseConfig unable to find config for attributes" );
		return -1;
	}

	result = eglGetConfigAttrib( display, config, EGL_NATIVE_VISUAL_ID, &visual_id );
	if ( result == EGL_FALSE )
	{
		xwl_set_error( "eglGetConfigAttrib failed when acquiring EGL_NATIVE_VISUAL_ID" );
		return -1;
	}

	fprintf( stdout, "EGL config chosen, visual: %i\n", visual_id );

	eglstring = (const char*) eglQueryString( display, EGL_VENDOR );
	fprintf( stdout, "EGL_VENDOR: %s\n", eglstring );

	eglstring = (const char*) eglQueryString( display, EGL_VERSION );
	fprintf( stdout, "EGL_VERSION: %s\n", eglstring );

	eglstring = (const char*) eglQueryString( display, EGL_CLIENT_APIS );
	fprintf( stdout, "EGL_CLIENT_APIS: %s\n", eglstring );

	eglstring = (const char*) eglQueryString( display, EGL_EXTENSIONS );
	fprintf( stdout, "EGL_EXTENSIONS: %s\n", eglstring );	

	return visual_id;
} // egl_api_pixel_format

void * egl_api_get_symbol( const char * symbol_name )
{
	//return 0;
	// VERIFY THIS
	// Watch the output of this, we may need to force this to always return 0,
	// because eglGetProcAddress may not be suitable for Core Profile functions in the future.
	return eglGetProcAddress( symbol_name );
} // egl_api_get_symbol

void egl_api_register( xwl_api_provider_t * api )
{
	api->create_context = egl_api_create_context;
	api->destroy_context = egl_api_destroy_context;
	api->activate_context = egl_api_activate_context;
	api->swap_buffers = egl_api_swap_buffers;
	api->pixel_format = egl_api_pixel_format;
	api->get_symbol = egl_api_get_symbol;
} // egl_api_register