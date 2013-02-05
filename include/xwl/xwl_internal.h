#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	// access the internal window handle by index
	xwl_native_window_t *_xwl_window_at_index( int index );
	
	//
	// open/close api driver libraries
	// returns 0 on failure, 1 on success
	int _xwl_open_driver_library( unsigned int api_provider );
	void _xwl_close_driver_library( void );
	
#if __APPLE__
	void * _xwl_apple_find_symbol( const char * symbol_name );
#endif
	
#ifdef __cplusplus
}; // extern "C"
#endif
