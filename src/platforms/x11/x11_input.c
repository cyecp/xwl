#include <xwl/xwl.h>
#include <xwl/xwl_internal.h>
#include <xwl/platforms/x11/x11.h>
#include <stdio.h>

static XIM currentInputMethod = 0;
static XComposeStatus currentKeyboardStatus;



void ProcessEvent( XEvent event, xwl_native_window_t * window );

Bool CheckEvent( Display *display, XEvent *event, XPointer userdata )
{
	return event->xany.window == (Window)userdata;
}


int x11_input_startup( void )
{
	Display * display = x11_current_display();
	if ( !display )
	{
		xwl_set_error( "Unable to open X11 Display!" );
		return 0;
	}

	currentInputMethod = XOpenIM( x11_current_display(), 0, 0, 0 );
	if ( !currentInputMethod )
	{
		xwl_set_error( "Unable to open X Input Method!" );
		return 0;
	}

	return 1;
} // x11_input_startup

void x11_input_shutdown( void )
{
	if ( currentInputMethod )
	{
		XCloseIM( currentInputMethod );
		currentInputMethod = 0;
	}
} // x11_input_shutdown

int x11_input_dispatch_events( void )
{
	XEvent ev;
	int i;
	xwl_native_window_t *wh = 0;
	XEvent lastKeyReleaseEvent;

	if ( !x11_current_display() )
	{
		//fprintf( stderr, "x11 display is INVALID\n" );
		return 0;
	}

	for( i = 0; i < XWL_MAX_WINDOW_HANDLES; ++i )
	{
		wh = _xwl_window_at_index( i );
		if ( wh->handle.handle != 0 )
		{
			while (XCheckIfEvent(x11_current_display(), &ev, &CheckEvent, wh->handle.handle) )
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
						XQueryKeymap(x11_current_display(), keys);
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

	return 0;
} // x11_input_dispatch_events


void x11_input_post_window_creation( xwl_native_window_t * native_window )
{
	Window wind;
	if ( currentInputMethod )
	{
		wind = (Window)native_window->handle.handle;
		native_window->inputContext = XCreateIC( currentInputMethod, XNClientWindow, wind, XNFocusWindow, wind, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL );
		if ( !native_window->inputContext )
		{
			fprintf( stderr, "[xwl] Failed to create input context!\n" );
		}
	}
	else
	{
		fprintf( stderr, "currentInputMethod is INVALID\n" );
	}
} // x11_input_post_window_creation


void x11_input_register( xwl_input_provider_t * input )
{
	input->startup = x11_input_startup;
	input->shutdown = x11_input_shutdown;
	input->dispatch_events = x11_input_dispatch_events;
	input->post_window_creation = x11_input_post_window_creation;
} // x11_input_register



// this code taken straight from SFML-1.6
unsigned int x11_key_to_xwl( KeySym sym )
{
	// First convert to uppercase (to avoid dealing with two different keysyms for the same key)
	KeySym Lower, Key;
	XConvertCase(sym, &Lower, &Key);

	switch (Key)
	{
		case XK_Shift_L :	  return XWLK_LSHIFT;
		case XK_Shift_R :	  return XWLK_RSHIFT;
		case XK_Control_L :	return XWLK_LCONTROL;
		case XK_Control_R :	return XWLK_RCONTROL;
		case XK_Alt_L :		return XWLK_LALT;
		case XK_Alt_R :		return XWLK_RALT;
		case XK_Super_L :	  return XWLK_LSYSTEM;
		case XK_Super_R :	  return XWLK_RSYSTEM;
		case XK_Menu :		 return XWLK_MENU;
		case XK_Escape :	   return XWLK_ESCAPE;
		case XK_semicolon :	return XWLK_SEMICOLON;
		case XK_slash :		return XWLK_SLASH;
		case XK_equal :		return XWLK_EQUALS;
		case XK_minus :		return XWLK_MINUS;
		case XK_bracketleft :  return XWLK_LBRACKET;
		case XK_bracketright : return XWLK_RBRACKET;
		case XK_comma :		return XWLK_COMMA;
		case XK_period :	   return XWLK_PERIOD;
		case XK_apostrophe :   return XWLK_QUOTE;
		case XK_backslash :	return XWLK_BACKSLASH;
		case XK_grave :		return XWLK_TILDE;
		case XK_space :		return XWLK_SPACE;
		case XK_Return :	   return XWLK_RETURN;
		case XK_KP_Enter :	 return XWLK_RETURN;
		case XK_BackSpace :	return XWLK_BACKSPACE;
		case XK_Tab :		  return XWLK_TAB;
		case XK_Prior :		return XWLK_PAGEUP;
		case XK_Next :		 return XWLK_PAGEDN;
		case XK_End :		  return XWLK_END;
		case XK_Home :		 return XWLK_HOME;
		case XK_Insert :	   return XWLK_INSERT;
		case XK_Delete :	   return XWLK_DELETE;
		case XK_KP_Add :	   return XWLK_ADD;
		case XK_KP_Subtract :  return XWLK_SUBTRACT;
		case XK_KP_Multiply :  return XWLK_MULTIPLY;
		case XK_KP_Divide :	return XWLK_DIVIDE;
		case XK_Pause :		return XWLK_PAUSE;
		case XK_F1 :		   return XWLK_F1;
		case XK_F2 :		   return XWLK_F2;
		case XK_F3 :		   return XWLK_F3;
		case XK_F4 :		   return XWLK_F4;
		case XK_F5 :		   return XWLK_F5;
		case XK_F6 :		   return XWLK_F6;
		case XK_F7 :		   return XWLK_F7;
		case XK_F8 :		   return XWLK_F8;
		case XK_F9 :		   return XWLK_F9;
		case XK_F10 :		  return XWLK_F10;
		case XK_F11 :		  return XWLK_F11;
		case XK_F12 :		  return XWLK_F12;
		case XK_F13 :		  return XWLK_F13;
		case XK_F14 :		  return XWLK_F14;
		case XK_F15 :		  return XWLK_F15;
		case XK_Left :		 return XWLK_LEFT;
		case XK_Right :		return XWLK_RIGHT;
		case XK_Up :		   return XWLK_UP;
		case XK_Down :		 return XWLK_DOWN;
		case XK_KP_0 :		 return XWLK_NUMPAD0;
		case XK_KP_1 :		 return XWLK_NUMPAD1;
		case XK_KP_2 :		 return XWLK_NUMPAD2;
		case XK_KP_3 :		 return XWLK_NUMPAD3;
		case XK_KP_4 :		 return XWLK_NUMPAD4;
		case XK_KP_5 :		 return XWLK_NUMPAD5;
		case XK_KP_6 :		 return XWLK_NUMPAD6;
		case XK_KP_7 :		 return XWLK_NUMPAD7;
		case XK_KP_8 :		 return XWLK_NUMPAD8;
		case XK_KP_9 :		 return XWLK_NUMPAD9;
		case XK_A :			return XWLK_A;
		case XK_Z :			return XWLK_Z;
		case XK_E :			return XWLK_E;
		case XK_R :			return XWLK_R;
		case XK_T :			return XWLK_T;
		case XK_Y :			return XWLK_Y;
		case XK_U :			return XWLK_U;
		case XK_I :			return XWLK_I;
		case XK_O :			return XWLK_O;
		case XK_P :			return XWLK_P;
		case XK_Q :			return XWLK_Q;
		case XK_S :			return XWLK_S;
		case XK_D :			return XWLK_D;
		case XK_F :			return XWLK_F;
		case XK_G :			return XWLK_G;
		case XK_H :			return XWLK_H;
		case XK_J :			return XWLK_J;
		case XK_K :			return XWLK_K;
		case XK_L :			return XWLK_L;
		case XK_M :			return XWLK_M;
		case XK_W :			return XWLK_W;
		case XK_X :			return XWLK_X;
		case XK_C :			return XWLK_C;
		case XK_V :			return XWLK_V;
		case XK_B :			return XWLK_B;
		case XK_N :			return XWLK_N;
		case XK_0 :			return XWLK_0;
		case XK_1 :			return XWLK_1;
		case XK_2 :			return XWLK_2;
		case XK_3 :			return XWLK_3;
		case XK_4 :			return XWLK_4;
		case XK_5 :			return XWLK_5;
		case XK_6 :			return XWLK_6;
		case XK_7 :			return XWLK_7;
		case XK_8 :			return XWLK_8;
		case XK_9 :			return XWLK_9;
		case XK_Num_Lock:	   return XWLK_NUMLOCK;

		// this does not match with the caps lock button
		//case XK_Caps_Lock:		return XWLK_CAPSLOCK;
	}
	fprintf( stderr, "[xwl] Unknown Key: %u\n", (unsigned int)Key );
	return 0;
} // x11_key_to_xwl

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
            ev.key = x11_key_to_xwl( sym );
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
            ev.key = x11_key_to_xwl( sym );
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

} // ProcessEvent
