
#include <xwl/xwl.h>
#include <stdio.h>
#include <string.h> // for memset
#include <wchar.h>


#if _WIN32

static wchar_t xwl_windowClassName[] = L"xwl_window";

#endif


#if LINUX
static Display *currentDisplay = 0;
static int currentScreen = 0;
static XIM currentInputMethod = 0;
static XComposeStatus currentKeyboardStatus;
#endif

#ifdef __cplusplus
extern "C" {
#endif

int xwl_renderer_startup( xwl_renderer_settings_t * settings, unsigned int * attribs );
void xwl_renderer_post( xwl_renderer_settings_t * settings );
void xwl_renderer_shutdown( xwl_renderer_settings_t * settings );
void xwl_renderer_activate( xwl_renderer_settings_t * settings );

#define XWL_MAX_WINDOW_HANDLES 4
static struct xwl_native_window_s xwl_windowHandles[ XWL_MAX_WINDOW_HANDLES ];

static xwl_event_callback xwl_callback;
const char * xwlerror;

const char * xwl_get_error()
{
    return xwlerror;
}

void xwl_set_error( const char * error )
{
	xwlerror = error;
}

#define XWL_MAX_EVENTS 256
unsigned int event_index = 0;
unsigned int event_read_ptr = 0;
unsigned int last_index = 0;
static xwl_event_t eventList[ XWL_MAX_EVENTS ];

xwl_native_window_t *xwl_get_unused_window()
{
	int i;
	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		if ( !xwl_windowHandles[i].handle.handle )
		{
			return &xwl_windowHandles[i];
		}
	}

	// no more window handles
	return 0;
}

void xwl_send_event( xwl_event_t * ev )
{
	// if there is a callback function set, dispatch to the callback
	if ( xwl_callback )
	{
		xwl_callback( ev );
	}
	else // otherwise, queue the event
	{
		last_index = event_index;
		memcpy( &eventList[ event_index++ ], ev, sizeof(xwl_event_t) );
		event_index = event_index % XWL_MAX_EVENTS;
	}
} // xwl_send_event

void xwl_setup_rendering( xwl_window_t * window, unsigned int * attribs )
{
#if _WIN32

#endif

#if __APPLE__
//	xwl_setup_osx_rendering( window, attribs );
#endif
}
	
void *xwl_rendering_context(xwl_window_t * window )
{
#if __APPLE__ && TARGET_OS_MAC
//	return xwl_osx_rendering_context( window );
    return 0;
#else
	return 0;
#endif
}
	
void xwl_activate( xwl_window_t * window )
{
	xwl_renderer_settings_t cfg;
	
	if ( !window )
	{
		return;
	}
	
	cfg.window = window;
	xwl_renderer_activate( &cfg );
}


const char * xwl_key_to_string( int key )
{
	switch( key )
	{
		case XWLK_A: return "XWLK_A";
		case XWLK_B: return "XWLK_B";
		case XWLK_C: return "XWLK_C";
		case XWLK_D: return "XWLK_D";
		case XWLK_E: return "XWLK_E";
		case XWLK_F: return "XWLK_F";
		case XWLK_G: return "XWLK_G";
		case XWLK_H: return "XWLK_H";
		case XWLK_I: return "XWLK_I";
		case XWLK_J: return "XWLK_J";
		case XWLK_K: return "XWLK_K";
		case XWLK_L: return "XWLK_L";
		case XWLK_M: return "XWLK_M";
		case XWLK_N: return "XWLK_N";
		case XWLK_O: return "XWLK_O";
		case XWLK_P: return "XWLK_P";
		case XWLK_Q: return "XWLK_Q";
		case XWLK_R: return "XWLK_R";
		case XWLK_S: return "XWLK_S";
		case XWLK_T: return "XWLK_T";
		case XWLK_U: return "XWLK_U";
		case XWLK_V: return "XWLK_V";
		case XWLK_W: return "XWLK_W";
		case XWLK_Y: return "XWLK_Y";
		case XWLK_X: return "XWLK_X";
		case XWLK_Z: return "XWLK_Z";
		case XWLK_LSYSTEM: return "XWLK_LSYSTEM";
		case XWLK_RSYSTEM: return "XWLK_RSYSTEM";
		case XWLK_MENU: return "XWLK_MENU";
		case XWLK_SEMICOLON: return "XWLK_SEMICOLON";
		case XWLK_SLASH: return "XWLK_SLASH";
		case XWLK_BACKSLASH: return "XWLK_BACKSLASH";
		case XWLK_EQUALS: return "XWLK_EQUALS";
		case XWLK_MINUS: return "XWLK_MINUS";
		case XWLK_LBRACKET: return "XWLK_LBRACKET";
		case XWLK_RBRACKET: return "XWLK_RBRACKET";
		case XWLK_COMMA: return "XWLK_COMMA";
		case XWLK_PERIOD: return "XWLK_PERIOD";
		case XWLK_QUOTE: return "XWLK_QUOTE";
		case XWLK_TILDE: return "XWLK_TILDE";
		case XWLK_ESCAPE: return "XWLK_ESCAPE";
		case XWLK_SPACE: return "XWLK_SPACE";
		case XWLK_RETURN: return "XWLK_RETURN";
		case XWLK_BACKSPACE: return "XWLK_BACKSPACE";
		case XWLK_TAB: return "XWLK_TAB";
		case XWLK_PAGEUP: return "XWLK_PAGEUP";
		case XWLK_PAGEDN: return "XWLK_PAGEDN";
		case XWLK_END: return "XWLK_END";
		case XWLK_HOME: return "XWLK_HOME";
		case XWLK_INSERT: return "XWLK_INSERT";
		case XWLK_DELETE: return "XWLK_DELETE";
		case XWLK_ADD: return "XWLK_ADD";
		case XWLK_SUBTRACT: return "XWLK_SUBTRACT";
		case XWLK_MULTIPLY: return "XWLK_MULTIPLY";
		case XWLK_DIVIDE: return "XWLK_DIVIDE";
		case XWLK_PAUSE: return "XWLK_PAUSE";
		case XWLK_F1: return "XWLK_F1";
		case XWLK_F2: return "XWLK_F2";
		case XWLK_F3: return "XWLK_F3";
		case XWLK_F4: return "XWLK_F4";
		case XWLK_F5: return "XWLK_F5";
		case XWLK_F6: return "XWLK_F6";
		case XWLK_F7: return "XWLK_F7";
		case XWLK_F8: return "XWLK_F8";
		case XWLK_F9: return "XWLK_F9";
		case XWLK_F10: return "XWLK_F10";
		case XWLK_F11: return "XWLK_F11";
		case XWLK_F12: return "XWLK_F12";
		case XWLK_F13: return "XWLK_F13";
		case XWLK_F14: return "XWLK_F14";
		case XWLK_F15: return "XWLK_F15";
		case XWLK_LEFT: return "XWLK_LEFT";
		case XWLK_RIGHT: return "XWLK_RIGHT";
		case XWLK_UP: return "XWLK_UP";
		case XWLK_DOWN: return "XWLK_DOWN";
		case XWLK_NUMPAD0: return "XWLK_NUMPAD0";
		case XWLK_NUMPAD1: return "XWLK_NUMPAD1";
		case XWLK_NUMPAD2: return "XWLK_NUMPAD2";
		case XWLK_NUMPAD3: return "XWLK_NUMPAD3";
		case XWLK_NUMPAD4: return "XWLK_NUMPAD4";
		case XWLK_NUMPAD5: return "XWLK_NUMPAD5";
		case XWLK_NUMPAD6: return "XWLK_NUMPAD6";
		case XWLK_NUMPAD7: return "XWLK_NUMPAD7";
		case XWLK_NUMPAD8: return "XWLK_NUMPAD8";
		case XWLK_NUMPAD9: return "XWLK_NUMPAD9";
		case XWLK_0: return "XWLK_0";
		case XWLK_1: return "XWLK_1";
		case XWLK_2: return "XWLK_2";
		case XWLK_3: return "XWLK_3";
		case XWLK_4: return "XWLK_4";
		case XWLK_5: return "XWLK_5";
		case XWLK_6: return "XWLK_6";
		case XWLK_7: return "XWLK_7";
		case XWLK_8: return "XWLK_8";
		case XWLK_9: return "XWLK_9";

		case XWLK_LSHIFT: return "XWLK_LSHIFT";
		case XWLK_RSHIFT: return "XWLK_RSHIFT";
		case XWLK_LCONTROL: return "XWLK_LCONTROL";
		case XWLK_RCONTROL: return "XWLK_RCONTROL";
		case XWLK_LALT: return "XWLK_LALT";
		case XWLK_RALT: return "XWLK_RALT";
		case XWLK_NUMLOCK: return "XWLK_NUMLOCK";


	}

	return "XWLK_INVALID";
}

const char * xwl_event_to_string( int event_type )
{
	switch( event_type )
	{
		case XWLE_INVALID: return "XWLE_INVALID";
		case XWLE_MOUSEMOVE: return "XWLE_MOUSEMOVE";
		case XWLE_MOUSEBUTTON_PRESSED: return "XWLE_MOUSEBUTTON_PRESSED";
		case XWLE_MOUSEBUTTON_RELEASED: return "XWLE_MOUSEBUTTON_RELEASED";
		case XWLE_MOUSEWHEEL: return "XWLE_MOUSEWHEEL";
		case XWLE_KEYPRESSED: return "XWLE_KEYPRESSED";
		case XWLE_KEYRELEASED: return "XWLE_KEYRELEASED";
//		case XWLE_JOYSTICK_MOVE: return "XWLE_JOYSTICK_MOVE";
//		case XWLE_JOYSTICKBUTTON_PRESSED: return "XWLE_JOYSTICKBUTTON_PRESSED";
//		case XWLE_JOYSTICKBUTTON_RELEASED: return "XWLE_JOYSTICKBUTTON_RELEASED";
		case XWLE_SIZE: return "XWLE_SIZE";
		case XWLE_CLOSED: return "XWLE_CLOSED";
		case XWLE_LOSTFOCUS: return "XWLE_LOSTFOCUS";
		case XWLE_GAINFOCUS: return "XWLE_GAINFOCUS";
		case XWLE_TEXT: return "XWLE_TEXT";
	}

	return "";
}

const char * xwl_mouse_to_string( int mb )
{
	switch( mb )
	{
		case XWLMB_LEFT: return "XWLMB_LEFT";
		case XWLMB_RIGHT: return "XWLMB_RIGHT";
		case XWLMB_MIDDLE: return "XWLMB_MIDDLE";
		case XWLMB_MOUSE4: return "XWLMB_MOUSE4";
		case XWLMB_MOUSE5: return "XWLMB_MOUSE5";

	}
	return "";
}


#if LINUX && 0
	#include <stdio.h>
	#include <X11/Xlib.h>
	//#include <X11/extensions/Xrandr.h>
	#include <cstdlib>
	#include <X11/extensions/Xinerama.h>
#endif


#if LINUX && 0
        // If TwinView is enabled, Linux only sees 1 display and 1 screen.
        // the resolution ends up being a combination of both screen sizes, which is not what we want.

        // -lXinerama
        // http://stackoverflow.com/questions/836086/programmatically-determining-individual-screen-widths-heights-in-linux-w-xineram
        Display *d=XOpenDisplay(NULL);

        if (d)
        {
            int dummy1, dummy2;
            if (XineramaQueryExtension(d, &dummy1, &dummy2)) {
                if (XineramaIsActive(d)) {
                    int heads=0;
                    XineramaScreenInfo *p=XineramaQueryScreens(d, &heads);
                    if (heads>0) {
                        for (int x=0; x<heads; ++x)
                        {
                            /*
                            cout << "Head " << x+1 << " of " << heads << ": " <<
                                p[x].width << "x" << p[x].height << " at " <<
                                p[x].x_org << "," << p[x].y_org << endl;    */

                            // Only interested in the first display for now, we should support others.
                            mode.width = p[x].width;
                            mode.height = p[x].height;
                            fprintf( stderr, "[xwl] Width: %i, Height: %i\n", mode.width, mode.height );
                            break;
                        }
                    }
                    else
                    {
                        fprintf( stderr, "[xwl] XineramaQueryScreens says there aren't any\n" );
                    }


                    XFree(p);
                }
                else
                {
                    fprintf( stderr, "[xwl] Xinerama not active\n" );
                }

            }
            else
            {
                fprintf( stderr, "[xwl] No Xinerama extension\n" );
            }

            XCloseDisplay(d);
        }
#endif

#if _WIN32
static unsigned int lshift;

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




#endif



#ifdef _WIN32
#define SEND_MOUSE_EVENT( t, b ) \
	ev.type = t;\
	ev.button = b;\
	xwl_send_event( &ev );

	// Utility Windows Message callback function
	LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wp, LPARAM lp )
	{
		i16 temp = 0;
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
				ev.wheelDelta = ((i16)HIWORD(wp) > 0) ? 1 : -1;
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
#endif

#if LINUX
/*
int xwl_xserver_handler( Display * display, XErrorEvent * event )
{
	char msg[80];
	XGetErrorText(display, event->error_code, msg, 80);
	fprintf(stderr, "[xwl.XError] Error code %s\n", msg);
	return 0;
}
*/
#endif
	
// include relevant headers for platforms

#if __APPLE__
	#include <xwl/platforms/osx/osx.h>
#endif

xwl_window_provider_t _window_provider;
xwl_api_provider_t _api_provider;
xwl_input_provider_t _input_provider;

// this list must sync up with the XWL_WINDOW_PROVIDER_* list
xwl_window_provider_register _window_providers[] = {
	0, // invalid
	0, // default
#if LINUX
	0, // EGL
	0, // X11
	0, // Wayland
#elif __APPLE__
	cocoa_register,
#elif WIN32
	0, // Win32
#elif XWL_RASPBERRYPI
	0, // Raspberry Pi
#endif
};

// this list must sync up with the XWL_API_PROVIDER_* list
xwl_api_provider_register _api_providers[] = {
	0, // invalid
	0, // default
#if LINUX
	0, // EGL
	0, // X11,
#elif __APPLE__
	cocoa_opengl_register,
#elif WIN32
	0, // Win32
#endif
};

// this list must sync up with the XWL_INPUT_PROVIDER_* list
xwl_input_provider_register _input_providers[] = {
	0, // invalid
	0, // default
#if LINUX
	0, // X11 (XKb)
#elif __APPLE__
	cocoa_input_register,
#elif WIN32
	0, // Win32
#endif
};

unsigned int _xwl_default_window_provider()
{
#if __APPLE__
	return XWL_WINDOW_PROVIDER_COCOA;
#elif LINUX
	return XWL_WINDOW_PROVIDER_X11;
#elif WIN32
	return XWL_WINDOW_PROVIDER_WIN32;
#endif

	return 0;
} // _xwl_default_window_provider

unsigned int _xwl_default_api_provider()
{
#if __APPLE__
	return XWL_API_PROVIDER_COCOA;
#elif XWL_RASPBERRYPI
	return XWL_API_PROVIDER_EGL;
#elif LINUX
	return XWL_API_PROVIDER_X11;
#elif WIN32
	return XWL_API_PROVIDER_WIN32;
#endif
	
	return 0;
} // _xwl_default_api_provider


unsigned int _xwl_default_input_provider()
{
#if __APPLE__
	return XWL_INPUT_PROVIDER_COCOA;
#elif LINUX
	return XWL_API_PROVIDER_X11;
#elif WIN32
	return XWL_API_PROVIDER_WIN32;
#endif
	
	return 0;
} // _xwl_default_input_provider

void xwl_get_window_size( xwl_window_t * window, int * width, int * height )
{
	_window_provider.get_window_size( window, width, height );
}

void xwl_get_screen_size( unsigned int screen_index, int * width, int * height )
{
	_window_provider.get_screen_size( screen_index, width, height );
}

unsigned int xwl_get_screen_count()
{
	return _window_provider.get_screen_count();
}
	
	
int _xwl_setup_window_provider( unsigned int window_provider )
{
	// choose default window provider for this platform: note this is not guaranteed to work!
    if ( window_provider == XWL_WINDOW_PROVIDER_DEFAULT )
	{
		window_provider = _xwl_default_window_provider();
	}
    
    xwl_window_provider_register wp_register = _window_providers[ window_provider ];
	if ( !wp_register )
	{
		return 0;
	}
    
	// register with the selected provider
    wp_register( &_window_provider );
	
	return 1;
} // _xwl_setup_window_provider

int _xwl_setup_api_provider( unsigned int api_provider )
{
	// choose default api provider
	if ( api_provider == XWL_API_PROVIDER_DEFAULT )
	{
		api_provider = _xwl_default_api_provider();
	}
	
	xwl_api_provider_register api_register = _api_providers[ api_provider ];
	if ( !api_register )
	{
		return 0;
	}
	
	api_register( &_api_provider );
	
	return 1;
} // _xwl_setup_api_provider

int _xwl_setup_input_provider( unsigned int input_provider )
{
	// choose default api provider
	if ( input_provider == XWL_INPUT_PROVIDER_DEFAULT )
	{
		input_provider = _xwl_default_input_provider();
	}
	
	xwl_input_provider_register input_register = _input_providers[ input_provider ];
	if ( !input_register )
	{
		return 0;
	}
	
	input_register( &_input_provider );
	
	return 1;
} // _xwl_setup_input_provider

int xwl_startup( unsigned int window_provider, unsigned int api_provider, unsigned int input_provider )
{
	int i;
	xwl_native_window_t * wh;
	int result = 0;

	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		wh = &xwl_windowHandles[i];
		memset( wh, 0, sizeof(xwl_native_window_t) );
	}
    
    memset( &_window_provider, 0, sizeof(xwl_window_provider_t) );

	if ( !_xwl_setup_window_provider( window_provider ) )
	{
		xwlerror = "No valid window provider found!";
		return 0;
	}
    	
	// perform startup
	result = _window_provider.startup( 0 );
        
	if ( !_xwl_setup_api_provider( api_provider ) )
	{
		xwl_set_error( "No valid API provider found!" );
		return 0;
	}
	
	if ( !_xwl_setup_input_provider( input_provider ) )
	{
		xwl_set_error ( "No valid Input provider found!" );
		return 0;
	}
    
#ifdef _WIN32
	// initialize key map
	lshift = MapVirtualKey(VK_LSHIFT, MAPVK_VK_TO_VSC);
#endif

#if LINUX
    {
        Bool detectable;
        currentDisplay = XOpenDisplay( 0 );

        if ( currentDisplay )
        {
            currentScreen = DefaultScreen( currentDisplay );
            currentInputMethod = XOpenIM( currentDisplay, 0, 0, 0 );
        }
        else
        {
            return 0;
        }

        // disable auto-repeats
        // "The standard behavior of the X server is to generate a KeyRelease event for every KeyPress event."
        // #include <X11/XKBlib.h>
        //XkbSetDetectableAutorepeat( currentDisplay, True, &detectable );

        // install error handler
        //XSetErrorHandler( xwl_xserver_handler );
    }


#endif


	return result;
}

	
	void xwl_swap_buffers( xwl_window_t * window )
	{
//		xwl_renderer_settings_t cfg;
#if LINUX
//		cfg.display = currentDisplay;
//		cfg.screen = currentScreen;
#endif
		
//		cfg.window = window;
		
		if ( window->handle != 0 )
		{
//			xwl_renderer_post( &cfg );
			_api_provider.swap_buffers( window->handle );
		}
	}
	
	void xwl_finish()
	{
		// loop through all windows and call finish
		xwl_native_window_t * wh = 0;
		xwl_renderer_settings_t cfg;
		int i;
		
#if LINUX
		cfg.display = currentDisplay;
		cfg.screen = currentScreen;
#endif
		
		for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
		{
			wh = &xwl_windowHandles[i];
			cfg.window = &wh->handle;
			
			
			if ( wh->handle.handle != 0 )
			{
				xwl_renderer_post( &cfg );
			}
		}
	}

#if LINUX
Bool CheckEvent( Display *display, XEvent *event, XPointer userdata )
{
    return event->xany.window == (Window)userdata;
}
#endif

void xwl_shutdown( void )
{
	int i;
	xwl_native_window_t * wh;

	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		wh = &xwl_windowHandles[i];

		if ( wh->handle.handle )
		{
#if LINUX
			XDestroyWindow( currentDisplay, (Window)wh->handle.handle );
#endif
			_window_provider.destroy_window( &wh->handle );
			wh->handle.handle = 0;
		}

		memset( wh, 0, sizeof(xwl_native_window_t) );
	}



	_window_provider.shutdown();
	_input_provider.shutdown();

#if LINUX
    if ( currentInputMethod )
    {
        XCloseIM( currentInputMethod );
        XCloseDisplay( currentDisplay );
        currentInputMethod = 0;
        currentDisplay = 0;
    }
#endif

}

#ifdef LINUX

// this code taken straight from SFML-1.6
unsigned int X11KeyToXWL( KeySym sym )
{
    // First convert to uppercase (to avoid dealing with two different keysyms for the same key)
    KeySym Lower, Key;
    XConvertCase(sym, &Lower, &Key);

    switch (Key)
    {
        case XK_Shift_L :      return XWLK_LSHIFT;
        case XK_Shift_R :      return XWLK_RSHIFT;
        case XK_Control_L :    return XWLK_LCONTROL;
        case XK_Control_R :    return XWLK_RCONTROL;
        case XK_Alt_L :        return XWLK_LALT;
        case XK_Alt_R :        return XWLK_RALT;
        case XK_Super_L :      return XWLK_LSYSTEM;
        case XK_Super_R :      return XWLK_RSYSTEM;
        case XK_Menu :         return XWLK_MENU;
        case XK_Escape :       return XWLK_ESCAPE;
        case XK_semicolon :    return XWLK_SEMICOLON;
        case XK_slash :        return XWLK_SLASH;
        case XK_equal :        return XWLK_EQUALS;
        case XK_minus :        return XWLK_MINUS;
        case XK_bracketleft :  return XWLK_LBRACKET;
        case XK_bracketright : return XWLK_RBRACKET;
        case XK_comma :        return XWLK_COMMA;
        case XK_period :       return XWLK_PERIOD;
        case XK_apostrophe :   return XWLK_QUOTE;
        case XK_backslash :    return XWLK_BACKSLASH;
        case XK_grave :        return XWLK_TILDE;
        case XK_space :        return XWLK_SPACE;
        case XK_Return :       return XWLK_RETURN;
        case XK_KP_Enter :     return XWLK_RETURN;
        case XK_BackSpace :    return XWLK_BACKSPACE;
        case XK_Tab :          return XWLK_TAB;
        case XK_Prior :        return XWLK_PAGEUP;
        case XK_Next :         return XWLK_PAGEDN;
        case XK_End :          return XWLK_END;
        case XK_Home :         return XWLK_HOME;
        case XK_Insert :       return XWLK_INSERT;
        case XK_Delete :       return XWLK_DELETE;
        case XK_KP_Add :       return XWLK_ADD;
        case XK_KP_Subtract :  return XWLK_SUBTRACT;
        case XK_KP_Multiply :  return XWLK_MULTIPLY;
        case XK_KP_Divide :    return XWLK_DIVIDE;
        case XK_Pause :        return XWLK_PAUSE;
        case XK_F1 :           return XWLK_F1;
        case XK_F2 :           return XWLK_F2;
        case XK_F3 :           return XWLK_F3;
        case XK_F4 :           return XWLK_F4;
        case XK_F5 :           return XWLK_F5;
        case XK_F6 :           return XWLK_F6;
        case XK_F7 :           return XWLK_F7;
        case XK_F8 :           return XWLK_F8;
        case XK_F9 :           return XWLK_F9;
        case XK_F10 :          return XWLK_F10;
        case XK_F11 :          return XWLK_F11;
        case XK_F12 :          return XWLK_F12;
        case XK_F13 :          return XWLK_F13;
        case XK_F14 :          return XWLK_F14;
        case XK_F15 :          return XWLK_F15;
        case XK_Left :         return XWLK_LEFT;
        case XK_Right :        return XWLK_RIGHT;
        case XK_Up :           return XWLK_UP;
        case XK_Down :         return XWLK_DOWN;
        case XK_KP_0 :         return XWLK_NUMPAD0;
        case XK_KP_1 :         return XWLK_NUMPAD1;
        case XK_KP_2 :         return XWLK_NUMPAD2;
        case XK_KP_3 :         return XWLK_NUMPAD3;
        case XK_KP_4 :         return XWLK_NUMPAD4;
        case XK_KP_5 :         return XWLK_NUMPAD5;
        case XK_KP_6 :         return XWLK_NUMPAD6;
        case XK_KP_7 :         return XWLK_NUMPAD7;
        case XK_KP_8 :         return XWLK_NUMPAD8;
        case XK_KP_9 :         return XWLK_NUMPAD9;
        case XK_A :            return XWLK_A;
        case XK_Z :            return XWLK_Z;
        case XK_E :            return XWLK_E;
        case XK_R :            return XWLK_R;
        case XK_T :            return XWLK_T;
        case XK_Y :            return XWLK_Y;
        case XK_U :            return XWLK_U;
        case XK_I :            return XWLK_I;
        case XK_O :            return XWLK_O;
        case XK_P :            return XWLK_P;
        case XK_Q :            return XWLK_Q;
        case XK_S :            return XWLK_S;
        case XK_D :            return XWLK_D;
        case XK_F :            return XWLK_F;
        case XK_G :            return XWLK_G;
        case XK_H :            return XWLK_H;
        case XK_J :            return XWLK_J;
        case XK_K :            return XWLK_K;
        case XK_L :            return XWLK_L;
        case XK_M :            return XWLK_M;
        case XK_W :            return XWLK_W;
        case XK_X :            return XWLK_X;
        case XK_C :            return XWLK_C;
        case XK_V :            return XWLK_V;
        case XK_B :            return XWLK_B;
        case XK_N :            return XWLK_N;
        case XK_0 :            return XWLK_0;
        case XK_1 :            return XWLK_1;
        case XK_2 :            return XWLK_2;
        case XK_3 :            return XWLK_3;
        case XK_4 :            return XWLK_4;
        case XK_5 :            return XWLK_5;
        case XK_6 :            return XWLK_6;
        case XK_7 :            return XWLK_7;
        case XK_8 :            return XWLK_8;
        case XK_9 :            return XWLK_9;
        case XK_Num_Lock:	   return XWLK_NUMLOCK;

        // this does not match with the caps lock button
        //case XK_Caps_Lock:		return XWLK_CAPSLOCK;
    }
    fprintf( stderr, "[xwl] Unknown Key: %u\n", (unsigned int)Key );
    return 0;
}


void ProcessEvent( XEvent event, xwl_native_window_t * window )
{
    xwl_event_t ev = {0};
    int length;
    unsigned int *end;
    char keybuffer[16];
    char buffer[32];
    unsigned int unicode[2];
    KeySym sym;
    Status returnedStatus;

    ev.target = &window->handle;
    switch( event.type )
    {
        case DestroyNotify:
            break;
        case FocusIn:
            ev.type = XWLE_GAINFOCUS;
            xwl_send_event( &ev );
            break;

        case FocusOut:
            ev.type = XWLE_LOSTFOCUS;
            xwl_send_event( &ev );
            break;

        case ConfigureNotify:
            ev.type = XWLE_SIZE;
            ev.width = event.xconfigure.width;
            ev.height = event.xconfigure.height;
            xwl_send_event( &ev );
            break;

        case ClientMessage:
            break;

        case KeyPress:
            XLookupString( &event.xkey, buffer, 32, &sym, &currentKeyboardStatus );
            ev.type = XWLE_KEYPRESSED;
            ev.key = X11KeyToXWL( sym );
            xwl_send_event( &ev );

            // generate a text event with unicode value
            if ( !XFilterEvent( &event, None ) )
            {
                #ifdef X_HAVE_UTF8_STRING
                if ( window->inputContext )
                {
                    length = Xutf8LookupString( window->inputContext, &event.xkey, keybuffer, 16, 0, &returnedStatus );

                    if ( length > 0 )
                    {
                        if ( length == 1 ) // straight to ASCII
                        {
                            ev.unicode = (int)keybuffer[0];
                        }
                        else
                        {
                            fprintf( stderr, "[xwl] Do UTF-8 -> UTF-32 Conversion!\n" );
                            fprintf( stderr, "[xwl] Length is: %i bytes.\n", length );
                            fprintf( stderr, "[xwl] %c\n", keybuffer[0] );
                            //end = Unicode::
                        }

                        // send a text event
                        ev.type = XWLE_TEXT;
                        xwl_send_event( &ev );
                    }
                }

                #endif
            }

            break;

        case KeyRelease:
            XLookupString( &event.xkey, buffer, 32, &sym, 0 );
            ev.type = XWLE_KEYRELEASED;
            ev.key = X11KeyToXWL( sym );
            xwl_send_event( &ev );
            break;

        case ButtonPress:
            // support for 5 mouse buttons
            if ( event.xbutton.button > 0 && event.xbutton.button < 4 || (event.xbutton.button == 8) || (event.xbutton.button == 9) )
            {
                ev.type = XWLE_MOUSEBUTTON_PRESSED;
                switch( event.xbutton.button )
                {
                    case Button1: ev.button = XWLMB_LEFT; break;
                    case Button2: ev.button = XWLMB_MIDDLE; break;
                    case Button3: ev.button = XWLMB_RIGHT; break;
                    case 8: ev.button = XWLMB_MOUSE4; break;
                    case 9: ev.button = XWLMB_MOUSE5; break;
                }
                xwl_send_event( &ev );
            }
            break;

        case ButtonRelease:
            // support for 5 mouse buttons
            if ( event.xbutton.button > 0 && event.xbutton.button < 4 || (event.xbutton.button == 8) || (event.xbutton.button == 9) )
            {
                ev.type = XWLE_MOUSEBUTTON_RELEASED;
                switch( event.xbutton.button )
                {
                    case Button1: ev.button = XWLMB_LEFT; break;
                    case Button2: ev.button = XWLMB_MIDDLE; break;
                    case Button3: ev.button = XWLMB_RIGHT; break;
                    case 8: ev.button = XWLMB_MOUSE4; break;
                    case 9: ev.button = XWLMB_MOUSE5; break;
                }
                xwl_send_event( &ev );
            }
            else if ( event.xbutton.button == Button4 || event.xbutton.button == Button5 )
            {
                // -1 is towards the user
                // 1 is away from the user
                ev.type = XWLE_MOUSEWHEEL;
                ev.wheelDelta = (event.xbutton.button == 4) ? 1 : -1;
                xwl_send_event( &ev );
            }
            break;

        case MotionNotify:
            ev.type = XWLE_MOUSEMOVE;
            ev.mx = event.xmotion.x;
            ev.my = event.xmotion.y;
            xwl_send_event( &ev );
            break;

        case EnterNotify:
            break;

        case LeaveNotify:
            break;
    }

}
#endif

int xwl_dispatch_events()
{
	int result;
	
	result = _input_provider.dispatch_events();
#ifdef _WIN32
	MSG msg;

	while ( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
#endif

#if LINUX
    XEvent ev;
    int i;
    xwl_native_window_t *wh = 0;
    XEvent lastKeyReleaseEvent;

    for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
    {
        wh = &xwl_windowHandles[i];
        if ( wh->handle.handle != 0 )
        {
            while (XCheckIfEvent( currentDisplay, &ev, &CheckEvent, wh->handle.handle) )
            {
                if ((ev.type == KeyPress) || (ev.type == KeyRelease) )
                {
                    if ( ev.xkey.keycode < 256 )
                    {
                        // To detect if it is a repeated key event, we check the current state of the key.
                        // - If the state is "down", KeyReleased events must obviously be discarded.
                        // - KeyPress events are a little bit harder to handle: they depend on the EnableKeyRepeat state,
                        //   and we need to properly forward the first one.
                        char keys[32];
                        XQueryKeymap(currentDisplay, keys);
                        if (keys[ev.xkey.keycode >> 3] & (1 << (ev.xkey.keycode % 8)))
                        {
                            // KeyRelease event + key down = repeated event --> discard
                            if (ev.type == KeyRelease)
                            {
                                lastKeyReleaseEvent = ev;
                                continue;
                            }

                            // KeyPress event + key repeat disabled + matching KeyRelease event = repeated event --> discard
                            if ((ev.type == KeyPress) &&
                                (lastKeyReleaseEvent.xkey.keycode == ev.xkey.keycode) &&
                                (lastKeyReleaseEvent.xkey.time == ev.xkey.time))
                            {
                                continue;
                            }
                        }
                    }
                }

                ProcessEvent( ev, wh );
            }
        }
    }

#endif

	return result;
}

#if LINUX
	Window xwl_linux_create_window( xwl_renderer_settings_t * settings, unsigned int * attribs );
#endif

xwl_window_t *xwl_create_window( const char * title, unsigned int * attribs )
{
	xwl_native_window_t * wh = 0;
	unsigned int i;
//    xwl_renderer_settings_t cfg;
	unsigned int attributes[ XWL_ATTRIBUTE_COUNT * 2 ] = {0};
	int current_attrib = -1;
	if ( title == 0 )
	{
		title = "xwl window";
	}
	
	// see if we can get an unused window handle
	wh = xwl_get_unused_window();
	if ( !wh )
	{
		xwl_set_error( "Failed to find an unused window handle!" );
		return 0;
	}
	
	attributes[ XWL_WINDOW_X ] = XWL_NOTSET;
	attributes[ XWL_WINDOW_Y ] = XWL_NOTSET;
	
	// translate all attribs into a single array we can check uniformly.
	for( i = 0; *attribs && i < XWL_ATTRIBUTE_COUNT; ++i )
	{
		if ( current_attrib == -1 )
		{
			current_attrib = *attribs;
		}
		else
		{
			attributes[ current_attrib ] = *attribs;
			current_attrib = -1;
		}
		++attribs;
	}
	
	// create the native window
	wh->handle.handle = _window_provider.create_window( wh, title, attributes );
	if ( !wh->handle.handle )
	{
		return 0;
	}


	// create a context
	void * context = _api_provider.create_context( wh->handle.handle, &_window_provider, attributes, 0 );
	if ( !context )
	{
		return 0;
	}

	// activate the context with this window
	_api_provider.activate_context( context, wh->handle.handle );


#ifdef _WIN32
	int style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	RECT clientrect;
	wchar_t windowName[ 128 ];
	//memset( windowName, 0, 128 );
	HWND handle;
	WNDCLASSEX wcx_info;
	WNDCLASSEX wcx;

	// see if the window class already exists...
	if ( GetClassInfoEx( 0, xwl_windowClassName, &wcx_info ) == 0 )
	{
		// class does not exist; create it 
		wcx.cbSize = sizeof( WNDCLASSEX );
		wcx.cbClsExtra = 0;
		wcx.cbWndExtra = 0;
		wcx.hbrBackground = 0;
		wcx.hCursor = LoadCursor( 0, IDC_ARROW );
		if ( params->flags & XWL_WIN32_ICON )
		{
			wcx.hIcon = params->hIcon;
		}
		else
		{
			wcx.hIcon = 0;
		}

		if ( params->flags & XWL_WIN32_ICONSM )
		{
			wcx.hIconSm = params->hIconSm;
		}
		else
		{
			wcx.hIconSm = 0;
		}

		wcx.hInstance = (HINSTANCE)GetModuleHandle(0);
		wcx.lpfnWndProc = WndProc; // long pointer-to-function WindowProc
		wcx.lpszClassName = xwl_windowClassName;
		wcx.lpszMenuName = 0;
		wcx.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;

		RegisterClassEx( &wcx );
	}


	if ( title == 0 )
	{
		title = "Untitled Window";
	}

	if ( !(params->flags & XWL_FULLSCREEN) )
	{
		RECT r = { 0, 0, params->width, params->height };
		style |= WS_MINIMIZEBOX | WS_CAPTION | WS_BORDER | WS_SYSMENU;
		if ( !(params->flags & XWL_NORESIZE) )
		{
			style |= WS_OVERLAPPEDWINDOW;
		}
		
		AdjustWindowRect( &r, style, 0 );
		params->width = (r.right - r.left);
		params->height = (r.bottom - r.top);

	}
	else
	{
		style |= WS_POPUP;
	}


	MultiByteToWideChar( CP_UTF8, 0, title, -1, windowName, 128 );

	handle = CreateWindowW( xwl_windowClassName, windowName, style, params->x, params->y, params->width, params->height, 0, 0, GetModuleHandle(0), 0 );

	if ( !handle )
	{
		return 0;
	}

	clientrect.left = 0;
	clientrect.right = params->width;
	clientrect.top = 0;
	clientrect.bottom = params->height;
	// ATI driver bug? from irrlicht
	MoveWindow( handle, clientrect.left, clientrect.top, clientrect.right, clientrect.bottom, 1 );

	// if visible
	ShowWindow( handle, SW_SHOW );
	SetForegroundWindow( handle );
	UpdateWindow( handle );


	wh = xwl_get_unused_window();

	if ( !wh )
	{
		// no more window handles
		return 0;
	}

	wh->handle.dc = 0;
	wh->handle.handle = handle;
	wh->handle.userdata = params->userdata;
	SetWindowLongPtrA( handle, GWLP_USERDATA, (LONG)wh);


	//GetWindowRect( handle, &clientrect );
	//printf( "WindowRect: %i %i %i %i\n", clientrect.left, clientrect.top, clientrect.bottom, clientrect.right );
	GetClientRect( handle, &clientrect );
	//printf( "ClientRect: %i %i %i %i\n", clientrect.left, clientrect.top, clientrect.bottom, clientrect.right );
	params->width = clientrect.right;
	params->height = clientrect.bottom;

	if ( (params->flags & XWL_OPENGL) )
	{
	    cfg.window = &wh->handle;
		xwl_renderer_startup( &cfg, attribs );
	}
#endif

#if LINUX && 0
    XSetWindowAttributes window_attribs;
    XVisualInfo * info;
    Window handle;
	Colormap colormap;
	int cwattrs;

	if ( title == 0 )
		title = "Untitled Window";

    wh = xwl_get_unused_window();

    if ( !wh )
    {
        xwlPrintf( "[xwl] Could not find unused window\n" );
        return 0;
    }

    if ( !(params->flags & XWL_OPENGL) )
    {
        xwlPrintf( "[xwl::X11] Must create an OpenGL window on Linux!\n" );
        return 0;
    }



    window_attribs.event_mask = FocusChangeMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;
    cfg.display = currentDisplay;
    cfg.screen = currentScreen;
    cfg.window = &wh->handle;

    cwattrs = CWEventMask;


    // to start, we need to create a temporary window to get a context and then link glXCreateContextAttribsARB.
    xwl_linux_create_window( &cfg, 0 );

    fprintf( stdout, "[xwl] renderer_startup...\n" );
    xwl_renderer_startup( &cfg, attribs );

    fprintf( stdout, "[xwl] Create a color map... \n");
    window_attribs.colormap = XCreateColormap( currentDisplay, RootWindow(currentDisplay, currentScreen), cfg.visual->visual, AllocNone );

    fprintf( stdout, "[xwl] Attempting to create a window" );
    handle = XCreateWindow( currentDisplay, RootWindow(currentDisplay, currentScreen), 0, 0, params->width, params->height, 0, cfg.visual->depth, InputOutput, cfg.visual->visual, CWColormap | CWEventMask, &window_attribs );
    if ( handle == 0 )
    {
        xwlPrintf( "[xwl::X11] XCreateWindow failed\n" );
        return 0;
    }

    glXMakeCurrent( currentDisplay, handle, cfg.window->context );

    //set window name
    // XStoreName is NOT Unicode aware! So basically, don't use anything except "Host Portable Character Encoding", (essentially, ASCII), or the result will be implementation-dependent.
    XStoreName( currentDisplay, handle, title );

    // to work around XStoreName's shortcomings, we use XChangeProperty with _NEW_WM_NAME
    XChangeProperty( currentDisplay, handle,
                    XInternAtom(currentDisplay, "_NET_WM_NAME", False),
                    XInternAtom(currentDisplay, "UTF8_STRING", False),
                    8, PropModeReplace, (unsigned char*)title, strlen(title) );

    // show the window
    XMapWindow( currentDisplay, handle );

    XFlush( currentDisplay );


    // standard init
    //myAtomClose = XInternAtom( currentDisplay, "WM_DELETE_WINDOW", 0 );
    //XSetWMProtocols( currentDisplay, handle, &myAtomClose, 1 );



    wh->handle.handle = (void*)handle;
    wh->handle.userdata = params->userdata;

    if ( currentInputMethod )
    {
        wh->inputContext = XCreateIC( currentInputMethod, XNClientWindow, handle, XNFocusWindow, handle, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL );
        if ( !wh->inputContext )
        {
            xwlPrintf( "[xwl::X11] Failed to create input context!\n" );
        }
    }

#endif

#if __APPLE__
//	wh = xwl_create_osx_window( params, title );
//	cfg.window = &wh->handle;

//    if ( (params->flags & XWL_OPENGL) )
//    {
//        xwl_renderer_startup( &cfg, attribs );
//    }

#endif

	if ( !wh )
		return 0;

	return &wh->handle;
} // xwl_create_window

void xwl_set_callback( xwl_event_callback cb )
{
	xwl_callback = cb;
}

#ifdef __cplusplus
}; // extern "C"
#endif
