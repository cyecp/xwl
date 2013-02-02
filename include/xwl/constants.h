#pragma once


// keys
enum
{
	XWLK_INVALID,
	XWLK_A,
	XWLK_B,
	XWLK_C,
	XWLK_D,
	XWLK_E,
	XWLK_F,
	XWLK_G,
	XWLK_H,
	XWLK_I,
	XWLK_J,
	XWLK_K,
	XWLK_L,
	XWLK_M,
	XWLK_N,
	XWLK_O,
	XWLK_P,
	XWLK_Q,
	XWLK_R,
	XWLK_S,
	XWLK_T,
	XWLK_U,
	XWLK_V,
	XWLK_W,
	XWLK_Y,
	XWLK_X,
	XWLK_Z,
	XWLK_LSYSTEM,
	XWLK_RSYSTEM,
	XWLK_MENU,
	XWLK_SEMICOLON,
	XWLK_SLASH,
	XWLK_BACKSLASH,
	XWLK_EQUALS,
	XWLK_MINUS,
	XWLK_LBRACKET,
	XWLK_RBRACKET,
	XWLK_COMMA,
	XWLK_PERIOD,
	XWLK_QUOTE,
	XWLK_TILDE,
	XWLK_ESCAPE,
	XWLK_SPACE,
	XWLK_RETURN,
	XWLK_BACKSPACE,
	XWLK_TAB,
	XWLK_PAGEUP,
	XWLK_PAGEDN,
	XWLK_END,
	XWLK_HOME,
	XWLK_INSERT,
	XWLK_DELETE,
	XWLK_ADD,
	XWLK_SUBTRACT,
	XWLK_MULTIPLY,
	XWLK_DIVIDE,
	XWLK_PAUSE,
	XWLK_F1,
	XWLK_F2,
	XWLK_F3,
	XWLK_F4,
	XWLK_F5,
	XWLK_F6,
	XWLK_F7,
	XWLK_F8,
	XWLK_F9,
	XWLK_F10,
	XWLK_F11,
	XWLK_F12,
	XWLK_F13,
	XWLK_F14,
	XWLK_F15,
	XWLK_LEFT,
	XWLK_RIGHT,
	XWLK_UP,
	XWLK_DOWN,
	XWLK_NUMPAD0,
	XWLK_NUMPAD1,
	XWLK_NUMPAD2,
	XWLK_NUMPAD3,
	XWLK_NUMPAD4,
	XWLK_NUMPAD5,
	XWLK_NUMPAD6,
	XWLK_NUMPAD7,
	XWLK_NUMPAD8,
	XWLK_NUMPAD9,
	XWLK_0,
	XWLK_1,
	XWLK_2,
	XWLK_3,
	XWLK_4,
	XWLK_5,
	XWLK_6,
	XWLK_7,
	XWLK_8,
	XWLK_9,
	XWLK_LSHIFT,
	XWLK_RSHIFT,
	XWLK_LCONTROL,
	XWLK_RCONTROL,
	XWLK_LALT,
	XWLK_RALT,
	XWLK_NUMLOCK,
	XWLK_CAPSLOCK,

	XWLMB_LEFT,
	XWLMB_RIGHT,
	XWLMB_MIDDLE,
	XWLMB_MOUSE4,
	XWLMB_MOUSE5,
	XWLMB_MOUSE6,
	XWLMB_MOUSE7
};

// key mods
enum
{
	XWLKM_INVALID,

	XWLKM_ALT = 1,
	XWLKM_SHIFT = 2,
	XWLKM_CONTROL = 4
};

// event constants
enum
{
	XWLE_INVALID,
	XWLE_MOUSEMOVE,
	XWLE_MOUSEBUTTON_PRESSED,
	XWLE_MOUSEBUTTON_RELEASED,
	XWLE_MOUSEWHEEL,
	XWLE_KEYPRESSED,
	XWLE_KEYRELEASED,
	XWLE_SIZE,
	XWLE_CLOSED,
	XWLE_LOSTFOCUS,
	XWLE_GAINFOCUS,
	XWLE_TEXT
};

enum
{
    // enum attributes
	XWL_API,
    XWL_API_PROVIDER,
    XWL_WINDOW_PROVIDER,
    
    // integer attributes
	XWL_API_MAJOR_VERSION,
	XWL_API_MINOR_VERSION,
	XWL_DEPTH_SIZE,
	XWL_COLOR_SIZE,
	XWL_ALPHA_SIZE,
	XWL_STENCIL_SIZE,
    XWL_WINDOW_WIDTH,
    XWL_WINDOW_HEIGHT,
    
#if _WIN32
	XWL_WIN32_ICON = 8,
	XWL_WIN32_ICONSM = 16,
#endif
    
    // boolean attributes
	XWL_USE_FULLSCREEN,
	XWL_DISABLE_RESIZE,
   

};

enum
{
	XWL_API_OPENGL,
	XWL_API_GLES1,
	XWL_API_GLES2,
	XWL_API_GLES3
};


// on linux, there are several options with regards to creating the window
// Other platforms always assume the default selection.
enum
{
    XWL_WINDOW_PROVIDER_DEFAULT,
    XWL_WINDOW_PROVIDER_EGL,
    XWL_WINDOW_PROVIDER_X11,
    XWL_WINDOW_PROVIDER_WAYLAND,
    XWL_WINDOW_PROVIDER_COCOA,
    XWL_WINDOW_PROVIDER_WIN32,
    XWL_WINDOW_PROVIDER_RASPBERRYPI
};


// on linux, there are several options with regards to setting up the graphics context
// Other platforms assume the default selection.
enum
{
    XWL_API_PROVIDER_DEFAULT,
    XWL_API_PROVIDER_EGL,
    XWL_API_PROVIDER_X11,
};
