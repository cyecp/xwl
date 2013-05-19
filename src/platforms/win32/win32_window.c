#include <xwl/xwl.h>
#include <xwl/xwl_internal.h>
#include <stdio.h>
#include <xwl/platforms/win32/win32.h>

static wchar_t xwl_windowClassName[] = L"xwl_window";

int VirtualKeyCodeToXWL( WPARAM wp, LPARAM lp )
{
	switch( wp )
	{
		case VK_LWIN :       return XWLK_LSYSTEM;
        case VK_RWIN :       return XWLK_RSYSTEM;
        case VK_APPS :       return XWLK_MENU;
        case VK_OEM_1 :      return XWLK_SEMICOLON;
        case VK_OEM_2 :      return XWLK_SLASH;
        case VK_OEM_PLUS :   return XWLK_EQUALS;
        case VK_OEM_MINUS :  return XWLK_MINUS;
        case VK_OEM_4 :      return XWLK_LBRACKET;
        case VK_OEM_6 :      return XWLK_RBRACKET;
        case VK_OEM_COMMA :  return XWLK_COMMA;
        case VK_OEM_PERIOD : return XWLK_PERIOD;
        case VK_OEM_7 :      return XWLK_QUOTE;
        case VK_OEM_5 :      return XWLK_BACKSLASH;
        case VK_OEM_3 :      return XWLK_TILDE;
        case VK_ESCAPE :     return XWLK_ESCAPE;
        case VK_SPACE :      return XWLK_SPACE;
        case VK_RETURN :     return XWLK_RETURN;
        case VK_BACK :       return XWLK_BACKSPACE;
        case VK_TAB :        return XWLK_TAB;
        case VK_PRIOR :      return XWLK_PAGEUP;
        case VK_NEXT :       return XWLK_PAGEDN;
        case VK_END :        return XWLK_END;
        case VK_HOME :       return XWLK_HOME;
        case VK_INSERT :     return XWLK_INSERT;
        case VK_DELETE :     return XWLK_DELETE;
        case VK_ADD :        return XWLK_ADD;
        case VK_SUBTRACT :   return XWLK_SUBTRACT;
        case VK_MULTIPLY :   return XWLK_MULTIPLY;
        case VK_DIVIDE :     return XWLK_DIVIDE;
        case VK_PAUSE :      return XWLK_PAUSE;
		case VK_F1 :         return XWLK_F1;
        case VK_F2 :         return XWLK_F2;
        case VK_F3 :         return XWLK_F3;
        case VK_F4 :         return XWLK_F4;
        case VK_F5 :         return XWLK_F5;
        case VK_F6 :         return XWLK_F6;
        case VK_F7 :         return XWLK_F7;
        case VK_F8 :         return XWLK_F8;
        case VK_F9 :         return XWLK_F9;
        case VK_F10 :        return XWLK_F10;
        case VK_F11 :        return XWLK_F11;
        case VK_F12 :        return XWLK_F12;
        case VK_F13 :        return XWLK_F13;
        case VK_F14 :        return XWLK_F14;
        case VK_F15 :        return XWLK_F15;
        case VK_LEFT :       return XWLK_LEFT;
        case VK_RIGHT :      return XWLK_RIGHT;
        case VK_UP :         return XWLK_UP;
        case VK_DOWN :       return XWLK_DOWN;
        case VK_NUMPAD0 :    return XWLK_NUMPAD0;
        case VK_NUMPAD1 :    return XWLK_NUMPAD1;
        case VK_NUMPAD2 :    return XWLK_NUMPAD2;
        case VK_NUMPAD3 :    return XWLK_NUMPAD3;
        case VK_NUMPAD4 :    return XWLK_NUMPAD4;
        case VK_NUMPAD5 :    return XWLK_NUMPAD5;
        case VK_NUMPAD6 :    return XWLK_NUMPAD6;
        case VK_NUMPAD7 :    return XWLK_NUMPAD7;
        case VK_NUMPAD8 :    return XWLK_NUMPAD8;
        case VK_NUMPAD9 :    return XWLK_NUMPAD9;
        case 'A' :           return XWLK_A;
        case 'Z' :           return XWLK_Z;
        case 'E' :           return XWLK_E;
        case 'R' :           return XWLK_R;
        case 'T' :           return XWLK_T;
        case 'Y' :           return XWLK_Y;
        case 'U' :           return XWLK_U;
        case 'I' :           return XWLK_I;
        case 'O' :           return XWLK_O;
        case 'P' :           return XWLK_P;
        case 'Q' :           return XWLK_Q;
        case 'S' :           return XWLK_S;
        case 'D' :           return XWLK_D;
        case 'F' :           return XWLK_F;
        case 'G' :           return XWLK_G;
        case 'H' :           return XWLK_H;
        case 'J' :           return XWLK_J;
        case 'K' :           return XWLK_K;
        case 'L' :           return XWLK_L;
        case 'M' :           return XWLK_M;
        case 'W' :           return XWLK_W;
        case 'X' :           return XWLK_X;
        case 'C' :           return XWLK_C;
        case 'V' :           return XWLK_V;
        case 'B' :           return XWLK_B;
        case 'N' :           return XWLK_N;
        case '0' :           return XWLK_0;
        case '1' :           return XWLK_1;
        case '2' :           return XWLK_2;
        case '3' :           return XWLK_3;
        case '4' :           return XWLK_4;
        case '5' :           return XWLK_5;
        case '6' :           return XWLK_6;
        case '7' :           return XWLK_7;
        case '8' :           return XWLK_8;
        case '9' :           return XWLK_9;
	}


	return 0;
	/*

	// this code is straight from SFML
    switch (VirtualKey)
    {
        // Check the scancode to distinguish between left and right shift
        case VK_SHIFT :
        {

            UINT scancode = (Flags & (0xFF << 16)) >> 16;
            return scancode == LShift ? Key::LShift : Key::RShift;
        }

        // Check the "extended" flag to distinguish between left and right alt
        case VK_MENU : return (HIWORD(Flags) & KF_EXTENDED) ? Key::RAlt : Key::LAlt;

        // Check the "extended" flag to distinguish between left and right control
        case VK_CONTROL : return (HIWORD(Flags) & KF_EXTENDED) ? Key::RControl : Key::LControl;
		*/
}


#define SEND_MOUSE_EVENT( t, b ) \
	ev.type = t;\
	ev.button = b;\
	xwl_send_event( &ev );

// Utility Windows Message callback function
LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
{
	short temp = 0;
	xwl_event_t ev = {0};
	xwl_window_t * wnd;
	wnd = (xwl_window_t*)GetWindowLongPtrA( hwnd, GWLP_USERDATA );

	if ( wnd )
	{
		ev.target = wnd;
		ev.mx = LOWORD(lp);
		ev.my = HIWORD(lp);

		// handle window messages
		switch( msg )
		{
		case WM_CHAR:
			// skip repeated messages
			if ( (lp & (1<<30)) == 0 )
			{
				ev.type = XWLE_TEXT;
				ev.unicode = (unsigned int)wp;
				xwl_send_event( &ev );
			}
			return 0;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			// skip repeated messages
			if ( (HIWORD(lp) & KF_REPEAT) == 0 )
			{
				ev.type = XWLE_KEYPRESSED;
				ev.key = VirtualKeyCodeToXWL( wp, lp );
				ev.keymods = 0;
				if ( HIWORD(GetAsyncKeyState(VK_MENU)) != 0 )
					ev.keymods |= XWLKM_ALT;
				if ( HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0 )
					ev.keymods |= XWLKM_CONTROL;
				if ( HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0 )
					ev.keymods |= XWLKM_SHIFT;
				xwl_send_event( &ev );

			}
			return 0;


		case WM_KEYUP:
		case WM_SYSKEYUP:
			ev.type = XWLE_KEYRELEASED;
			ev.key = VirtualKeyCodeToXWL( wp, lp );
			ev.keymods = 0;
			if ( HIWORD(GetAsyncKeyState(VK_MENU)) != 0 )
				ev.keymods |= XWLKM_ALT;
			if ( HIWORD(GetAsyncKeyState(VK_CONTROL)) != 0 )
				ev.keymods |= XWLKM_CONTROL;
			if ( HIWORD(GetAsyncKeyState(VK_SHIFT)) != 0 )
				ev.keymods |= XWLKM_SHIFT;
			xwl_send_event( &ev );

			return 0;

		case WM_MOUSEMOVE:
			ev.type = XWLE_MOUSEMOVE;
			{
				POINT pt = { ev.mx, ev.my };

				//ClientToScreen( hwnd, &pt );
				ev.mx = pt.x;
				ev.my = pt.y;
			}
			xwl_send_event( &ev );
			return 0;

		// -1 is towards the user
		// 1 is away from the user
		case WM_MOUSEWHEEL:
			ev.type = XWLE_MOUSEWHEEL;
			ev.wheelDelta = ((short)HIWORD(wp) > 0) ? 1 : -1;
			xwl_send_event( &ev );
			return 0;

		case WM_LBUTTONDOWN:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, XWLMB_LEFT );
			return 0;
		case WM_LBUTTONDBLCLK:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, XWLMB_LEFT );
			return 0;
		case WM_LBUTTONUP:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_RELEASED, XWLMB_LEFT );
			return 0;
		case WM_RBUTTONDOWN:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, XWLMB_RIGHT );
			return 0;
		case WM_RBUTTONDBLCLK:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, XWLMB_RIGHT );
			return 0;
		case WM_RBUTTONUP:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_RELEASED, XWLMB_RIGHT );
			return 0;
		case WM_MBUTTONDOWN:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, XWLMB_MIDDLE );
			return 0;
		case WM_MBUTTONDBLCLK:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, XWLMB_MIDDLE );
			return 0;
		case WM_MBUTTONUP:
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_RELEASED, XWLMB_MIDDLE );
			return 0;
		case WM_XBUTTONDOWN:
			if ( HIWORD(wp) == 1 )
				temp = XWLMB_MOUSE4;
			else
				temp = XWLMB_MOUSE5;

			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, temp );
			return 0;
		case WM_XBUTTONDBLCLK:
			if ( HIWORD(wp) == 1 )
				temp = XWLMB_MOUSE4;
			else
				temp = XWLMB_MOUSE5;
			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_PRESSED, temp );
			return 0;
		case WM_XBUTTONUP:
			if ( HIWORD(wp) == 1 )
				temp = XWLMB_MOUSE4;
			else
				temp = XWLMB_MOUSE5;

			SEND_MOUSE_EVENT( XWLE_MOUSEBUTTON_RELEASED, temp );
			return 0;

		case WM_SETFOCUS:
			ev.type = XWLE_GAINFOCUS;
			xwl_send_event( &ev );
			return 0;

		case WM_KILLFOCUS:
			ev.type = XWLE_LOSTFOCUS;
			xwl_send_event( &ev );
			return 0;

		case WM_DESTROY:
		case WM_CLOSE:
			ev.type = XWLE_CLOSED;
			xwl_send_event( &ev );
			return 0;
		case WM_SIZE:
			ev.type = XWLE_SIZE;
			ev.width = LOWORD(lp);
			ev.height = HIWORD(lp);
			xwl_send_event( &ev );
			return 0;
		}
	}

	return DefWindowProc( hwnd, msg, wp, lp );
}

int win32_window_startup( xwl_api_provider_t * api )
{
	return 1;
} // win32_window_startup

void win32_window_shutdown( void )
{

} // win32_window_shutdown


void * win32_window_create_window( xwl_native_window_t * handle, const char * utf8_title, unsigned int * attributes, int pixel_format )
{
	int style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	RECT clientrect;
	wchar_t windowName[ 128 ];
	//memset( windowName, 0, 128 );
	HWND native_handle;
	WNDCLASSEX wcx_info;
	WNDCLASSEX wcx;

	int window_width;
	int window_height;
	int use_fullscreen;
	int window_x, window_y;

	window_width = attributes[ XWL_WINDOW_WIDTH ];
	window_height = attributes[ XWL_WINDOW_HEIGHT ];
	use_fullscreen = attributes[ XWL_USE_FULLSCREEN ];
	window_x = attributes[ XWL_WINDOW_X ];
	window_y = attributes[ XWL_WINDOW_Y ];

	// see if the window class already exists...
	if ( GetClassInfoEx( 0, xwl_windowClassName, &wcx_info ) == 0 )
	{
		// class does not exist; create it 
		wcx.cbSize = sizeof( WNDCLASSEX );
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = 0;
		wcx.hbrBackground = 0;
		wcx.hCursor = LoadCursor( 0, IDC_ARROW );
		wcx.hIcon = (HICON)attributes[ XWL_WIN32_ICON ];
		wcx.hIconSm = (HICON)attributes[ XWL_WIN32_ICONSM ];
		wcx.hInstance = (HINSTANCE)GetModuleHandle(0);
		wcx.lpfnWndProc = WndProc; // long pointer-to-function WindowProc
		wcx.lpszClassName = xwl_windowClassName;
		wcx.lpszMenuName = 0;
		wcx.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;

		RegisterClassEx( &wcx );
	}

	if ( utf8_title == 0 )
	{
		utf8_title = "Untitled Window";
	}

	if ( !use_fullscreen )
	{
		RECT r = { 0, 0, window_width, window_height };
		style |= WS_MINIMIZEBOX | WS_CAPTION | WS_BORDER | WS_SYSMENU;
		if ( !(attributes[XWL_DISABLE_RESIZE]) )
		{
			style |= WS_OVERLAPPEDWINDOW;
		}
		
		AdjustWindowRect( &r, style, 0 );
		window_width = (r.right - r.left);
		window_height = (r.bottom - r.top);

	}
	else
	{
		style |= WS_POPUP;
	}


	MultiByteToWideChar( CP_UTF8, 0, utf8_title, -1, windowName, 128 );

	native_handle = CreateWindowW( xwl_windowClassName, windowName, style, window_x, window_y, window_width, window_height, 0, 0, GetModuleHandle(0), 0 );
	if ( !native_handle )
	{
		return 0;
	}

	clientrect.left = 0;
	clientrect.right = window_width;
	clientrect.top = 0;
	clientrect.bottom = window_height;

	// ATI driver bug? from irrlicht
	MoveWindow( native_handle, clientrect.left, clientrect.top, clientrect.right, clientrect.bottom, 1 );

	// if visible
	ShowWindow( native_handle, SW_SHOW );
	SetForegroundWindow( native_handle );
	UpdateWindow( native_handle );

	// store native window handle data
	SetWindowLongPtrA( native_handle, GWLP_USERDATA, (LONG)&handle->handle);

	//
	//GetClientRect( native_handle, &clientrect );

	// TODO: This is a hack because Windows requires the pixel format to come after window creation
	// and the current order of processes in xwl has pixel format being queried before.
	// Perhaps a solution is to offer both a pre- and post- window callback.
	handle->handle.pixel_format = win32_opengl_setup_pixelformat( handle, native_handle );

	return (void*)native_handle;
} // win32_window_create_window


void win32_window_destroy_window( xwl_window_t * window )
{
} // win32_window_destroy_window

void win32_window_get_window_size( xwl_window_t * window, int * width, int * height )
{
	HWND native_window = (HWND)window->handle;
	RECT rect;

	if ( native_window )
	{
		GetClientRect( native_window, &rect );
		*width = (rect.right - rect.left);
		*height = (rect.bottom - rect.top);
	}
} // win32_window_get_window_size

void win32_window_get_window_render_size( xwl_window_t * window, int * width, int * height )
{
	// http://stackoverflow.com/questions/4631292/how-detect-current-screen-resolution

	win32_window_get_window_size( window, width, height );
} // win32_window_get_window_render_size



struct monitor_size_s
{
	unsigned short num_monitors_iterated;
	unsigned short target_monitor_id;
	int * width;
	int * height;
};

BOOL CALLBACK monitor_screen_size(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFO monitorinfo;
	struct monitor_size_s * monitorsize = (struct monitor_size_s*)dwData;

	// found the monitor requested, no longer need to continue
	if (monitorsize->target_monitor_id == monitorsize->num_monitors_iterated)
	{
		// if the name of the monitor is ever required; use MONITORINFOEX to retrieve it
		monitorinfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo( hMonitor, &monitorinfo );
		*monitorsize->width = (monitorinfo.rcMonitor.right - monitorinfo.rcMonitor.left);
		*monitorsize->height = (monitorinfo.rcMonitor.bottom - monitorinfo.rcMonitor.top);

		return FALSE;
	}
	
	monitorsize->num_monitors_iterated++;

    return TRUE;
} // monitor_enum_proc


void win32_window_get_screen_size( unsigned int screen_index, int * width, int * height )
{
	// http://stackoverflow.com/questions/4631292/how-detect-current-screen-resolution

	struct monitor_size_s monitorsize;
	monitorsize.width = width;
	monitorsize.height = height;
	monitorsize.target_monitor_id = screen_index;
	monitorsize.num_monitors_iterated = 0;

	EnumDisplayMonitors(0, 0, monitor_screen_size, (LPARAM)&monitorsize);
} // win32_window_get_screen_size

BOOL CALLBACK monitor_enum_proc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    unsigned int * num_monitors = (unsigned int*)dwData;
    (*num_monitors)++;
    return TRUE;
} // monitor_enum_proc

unsigned int win32_window_get_screen_count()
{
	// http://stackoverflow.com/questions/7767036/how-do-i-get-the-number-of-displays-in-windows
	unsigned int screen_count = 0;
	EnumDisplayMonitors(0, 0, monitor_enum_proc, (LPARAM)&screen_count);

	return screen_count;
} // win32_window_get_screen_count


void win32_window_register( xwl_window_provider_t * wapi )
{
	wapi->startup = win32_window_startup;
	wapi->shutdown = win32_window_shutdown;
	wapi->create_window = win32_window_create_window;
	wapi->destroy_window = win32_window_destroy_window;
	wapi->get_window_size = win32_window_get_window_size;
	wapi->get_window_render_size = win32_window_get_window_render_size;
	wapi->get_screen_size = win32_window_get_screen_size;
	wapi->get_screen_count = win32_window_get_screen_count;
} // win32_window_register