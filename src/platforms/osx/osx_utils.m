#import <xwl/platforms/osx/osx.h>
#import <xwl/platforms/osx/appdelegate.h>

unsigned int LocalizedKeys(unichar ch)
{
	switch (ch)
	{
		case 'a':
		case 'A':                   return XWLK_A;
			
		case 'b':
		case 'B':                   return XWLK_B;
			
		case 'c':
		case 'C':                   return XWLK_C;
			
		case 'd':
		case 'D':                   return XWLK_D;
			
		case 'e':
		case 'E':                   return XWLK_E;
			
		case 'f':
		case 'F':                   return XWLK_F;
			
		case 'g':
		case 'G':                   return XWLK_G;
			
		case 'h':
		case 'H':                   return XWLK_H;
			
		case 'i':
		case 'I':                   return XWLK_I;
			
		case 'j':
		case 'J':                   return XWLK_J;
			
		case 'k':
		case 'K':                   return XWLK_K;
			
		case 'l':
		case 'L':                   return XWLK_L;
			
		case 'm':
		case 'M':                   return XWLK_M;
			
		case 'n':
		case 'N':                   return XWLK_N;
			
		case 'o':
		case 'O':                   return XWLK_O;
			
		case 'p':
		case 'P':                   return XWLK_P;
			
		case 'q':
		case 'Q':                   return XWLK_Q;
			
		case 'r':
		case 'R':                   return XWLK_R;
			
		case 's':
		case 'S':                   return XWLK_S;
			
		case 't':
		case 'T':                   return XWLK_T;
			
		case 'u':
		case 'U':                   return XWLK_U;
			
		case 'v':
		case 'V':                   return XWLK_V;
			
		case 'w':
		case 'W':                   return XWLK_W;
			
		case 'x':
		case 'X':                   return XWLK_X;
			
		case 'y':
		case 'Y':                   return XWLK_Y;
			
		case 'z':
		case 'Z':                   return XWLK_Z;
			
			// The kew is not 'localized'.
		default:                    return XWLK_INVALID;
	}
} // LocalizedKeys


////////////////////////////////////////////////////////
unsigned int NonLocalizedKeys(unsigned short keycode)
{
	// (Some) 0x code based on http://forums.macrumors.com/showthread.php?t=780577
	// Some sf::Key are present twice.
	switch (keycode)
	{
			// These cases should not be used but anyway...
		case 0x00:                      return XWLK_A;
		case 0x0b:                      return XWLK_B;
		case 0x08:                      return XWLK_C;
		case 0x02:                      return XWLK_D;
		case 0x0e:                      return XWLK_E;
		case 0x03:                      return XWLK_F;
		case 0x05:                      return XWLK_G;
		case 0x04:                      return XWLK_H;
		case 0x22:                      return XWLK_I;
		case 0x26:                      return XWLK_J;
		case 0x28:                      return XWLK_K;
		case 0x25:                      return XWLK_L;
		case 0x2e:                      return XWLK_M;
		case 0x2d:                      return XWLK_N;
		case 0x1f:                      return XWLK_O;
		case 0x23:                      return XWLK_P;
		case 0x0c:                      return XWLK_Q;
		case 0x0f:                      return XWLK_R;
		case 0x01:                      return XWLK_S;
		case 0x11:                      return XWLK_T;
		case 0x20:                      return XWLK_U;
		case 0x09:                      return XWLK_V;
		case 0x0d:                      return XWLK_W;
		case 0x07:                      return XWLK_X;
		case 0x10:                      return XWLK_Y;
		case 0x06:                      return XWLK_Z;
			
			// These cases should not be used but anyway...
		case 0x1d:                      return XWLK_0;
		case 0x12:                      return XWLK_1;
		case 0x13:                      return XWLK_2;
		case 0x14:                      return XWLK_3;
		case 0x15:                      return XWLK_4;
		case 0x17:                      return XWLK_5;
		case 0x16:                      return XWLK_6;
		case 0x1a:                      return XWLK_7;
		case 0x1c:                      return XWLK_8;
		case 0x19:                      return XWLK_9;
			
		case 0x35:                      return XWLK_ESCAPE;
			
			// Modifier keys : never happen with keyDown/keyUp methods (?)
		case 0x3b:                      return XWLK_LCONTROL;
		case 0x38:                      return XWLK_LSHIFT;
		case 0x3a:                      return XWLK_LALT;
		case 0x37:                      return XWLK_LSYSTEM;
		case 0x3e:                      return XWLK_RCONTROL;
		case 0x3c:                      return XWLK_RSHIFT;
		case 0x3d:                      return XWLK_RALT;
		case 0x36:                      return XWLK_RSYSTEM;
			
		case NSMenuFunctionKey:         return XWLK_MENU;
			
		case 0x21:                      return XWLK_LBRACKET;
		case 0x1e:                      return XWLK_RBRACKET;
		case 0x29:                      return XWLK_SEMICOLON;
		case 0x2b:                      return XWLK_COMMA;
		case 0x2f:                      return XWLK_PERIOD;
		case 0x27:                      return XWLK_QUOTE;
		case 0x2c:                      return XWLK_SLASH;
		case 0x2a:                      return XWLK_BACKSLASH;
			
			//#warning XWLK_TILDE might be in conflict with some other key.
			// 0x0a is for "Non-US Backslash" according to HID Calibrator,
			// a sample provided by Apple.
			//case 0x0a:                      return XWLK_TILDE;
		case 0x1b:						return XWLK_MINUS;
		case 0x18:                      return XWLK_EQUALS;
		case 0x32:                      return XWLK_TILDE;
		case 0x31:                      return XWLK_SPACE;
		case 0x24:                      return XWLK_RETURN;
		case 0x33:                      return XWLK_BACKSPACE;
		case 0x30:                      return XWLK_TAB;
			
			// Duplicates (see next §).
		case 0x74:                      return XWLK_PAGEUP;
		case 0x79:                      return XWLK_PAGEDN;
		case 0x77:                      return XWLK_END;
		case 0x73:                      return XWLK_HOME;
			
		case NSPageUpFunctionKey:       return XWLK_PAGEUP;
		case NSPageDownFunctionKey:     return XWLK_PAGEDN;
		case NSEndFunctionKey:          return XWLK_END;
		case NSHomeFunctionKey:         return XWLK_HOME;
			
		case NSInsertFunctionKey:       return XWLK_INSERT;
		case NSDeleteFunctionKey:       return XWLK_DELETE;
			
		case 0x45:                      return XWLK_ADD;
		case 0x4e:                      return XWLK_SUBTRACT;
		case 0x43:                      return XWLK_MULTIPLY;
		case 0x4b:                      return XWLK_DIVIDE;
			
			// Duplicates (see next §).
		case 0x7b:                      return XWLK_LEFT;
		case 0x7c:                      return XWLK_RIGHT;
		case 0x7e:                      return XWLK_UP;
		case 0x7d:                      return XWLK_DOWN;
			
		case NSLeftArrowFunctionKey:    return XWLK_LEFT;
		case NSRightArrowFunctionKey:   return XWLK_RIGHT;
		case NSUpArrowFunctionKey:      return XWLK_UP;
		case NSDownArrowFunctionKey:    return XWLK_DOWN;
			
		case 0x52:                      return XWLK_NUMPAD0;
		case 0x53:                      return XWLK_NUMPAD1;
		case 0x54:                      return XWLK_NUMPAD2;
		case 0x55:                      return XWLK_NUMPAD3;
		case 0x56:                      return XWLK_NUMPAD4;
		case 0x57:                      return XWLK_NUMPAD5;
		case 0x58:                      return XWLK_NUMPAD6;
		case 0x59:                      return XWLK_NUMPAD7;
		case 0x5b:                      return XWLK_NUMPAD8;
		case 0x5c:                      return XWLK_NUMPAD9;
			
			// Duplicates (see next §).
		case 0x7a:                      return XWLK_F1;
		case 0x78:                      return XWLK_F2;
		case 0x63:                      return XWLK_F3;
		case 0x76:                      return XWLK_F4;
		case 0x60:                      return XWLK_F5;
		case 0x61:                      return XWLK_F6;
		case 0x62:                      return XWLK_F7;
		case 0x64:                      return XWLK_F8;
		case 0x65:                      return XWLK_F9;
		case 0x6d:                      return XWLK_F10;
		case 0x67:                      return XWLK_F11;
		case 0x6f:                      return XWLK_F12;
		case 0x69:                      return XWLK_F13;
		case 0x6b:                      return XWLK_F14;
		case 0x71:                      return XWLK_F15;
			
		case NSF1FunctionKey:           return XWLK_F1;
		case NSF2FunctionKey:           return XWLK_F2;
		case NSF3FunctionKey:           return XWLK_F3;
		case NSF4FunctionKey:           return XWLK_F4;
		case NSF5FunctionKey:           return XWLK_F5;
		case NSF6FunctionKey:           return XWLK_F6;
		case NSF7FunctionKey:           return XWLK_F7;
		case NSF8FunctionKey:           return XWLK_F8;
		case NSF9FunctionKey:           return XWLK_F9;
		case NSF10FunctionKey:          return XWLK_F10;
		case NSF11FunctionKey:          return XWLK_F11;
		case NSF12FunctionKey:          return XWLK_F12;
		case NSF13FunctionKey:          return XWLK_F13;
		case NSF14FunctionKey:          return XWLK_F14;
		case NSF15FunctionKey:          return XWLK_F15;
			
		case NSPauseFunctionKey:        return XWLK_PAUSE;
			
			//#warning keycode 0x1b is not bound to any key.
			// This key is ' on CH-FR, ) on FR and - on US layouts.
			
			// An unknown key.
		default:                        return 0;
	}
} // NonLocalizedKeys