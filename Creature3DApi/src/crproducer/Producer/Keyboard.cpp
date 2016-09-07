/* -*-c++-*- Producer - Copyright (C) 2001-2004  Don Burns
 *
 * This library is open source and may be redistributed and/or modified under
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * OpenSceneGraph Public License for more details.
 */

#include <CRProducer/Producer/Keyboard>

using namespace Producer;

KeyboardProxy proxy;

Keyboard *Keyboard::theKeyboard = 0L;

KeyboardProxy::KeyboardProxy() 
{ 
    Keyboard::getSingleton(); 
}

KeyboardProxy::~KeyboardProxy() 
{ 
    delete Keyboard::theKeyboard; 
	//std::cerr << "~KeyboardProxy()" << std::endl;
}

Keyboard::Keyboard()
{ 
    _keyModifier = 0;
    _initKeyboardKeyMap();
    _initKeyCharMap();
}

// Force singleton
void *Keyboard::operator new( size_t size )
{
    if( theKeyboard == 0L )
        theKeyboard = ::new Keyboard;
    return (void *)theKeyboard;
}

Keyboard *Keyboard::getSingleton()
{
    if( theKeyboard == 0L )
        theKeyboard = ::new Keyboard;
    return theKeyboard;
}

bool Keyboard::isSpecialKey( KeyCharacter keyChar )
{
    if( keyChar & 0xFF00 )
        return true;
    else
        return false;
}

void Keyboard::mapKey( KeyCombination keyCombo, KeyCharacter keyChar )
{
    _charmap[keyCombo()] = keyChar;
}
	
void Keyboard::clearKeyModifier()
{
	_keyModifier &= ~KeyMod_Alt;
	_keyModifier &= ~KeyMod_Control;
	//_keyModifier &= ~KeyMod_Shift;
	//_keyModifier &= ~KeyMod_CapsLock;
}

#ifdef _X11_IMPLEMENTATION
KeyCharacter Keyboard::xkeyEventToKeyChar(Display *dpy, XKeyEvent &ev)
{
    unsigned long mod = 0;
    KeySym ks = XKeycodeToKeysym( dpy, ev.keycode, 0 );

    static XComposeStatus state;
    unsigned char keybuf[32];
    XLookupString( &ev, (char *)keybuf, sizeof(keybuf), NULL, &state );

    if( ev.state & ShiftMask )
        mod |= KeyMod_Shift;
    if( ev.state & LockMask )
        mod |= KeyMod_CapsLock;
    if( ev.state & ControlMask )
        mod |= KeyMod_Control;
    if( ev.state & Mod1Mask )
        mod |= KeyMod_Alt;
    if( ev.state & Mod2Mask )
        mod |= KeyMod_NumLock;
    if( ev.state & Mod4Mask )
        mod |= KeyMod_Super;

    KeyCharacter lookup_key  = KeyCharacter(keybuf[0]);
    KeyCharacter charmap_key = KeyCharacter( _charmap[mod | _keymap[ks]] );

    if ( charmap_key & 0xFF00 ) 
    {
        return charmap_key;
    } 
    else 
    {
        return lookup_key;
    }
}
#endif

#ifdef _WIN32_IMPLEMENTATION
KeyCharacter Keyboard::win32KeyEventToKeyChar( LPARAM lParam, WPARAM wParam )
{
    enum KeyState{
        KeyReleased = 0x80000000L,
        KeyRepeated = 0x40000000L,
        Alt         = 0x20000000L,
        CtrlRight   = 0x01000000L,
        KeyCode     = 0x00FF0000L,
        ShiftRight  = 0x00360000L,
        ShiftLeft   = 0x002A0000L,
        
    };

    // Handle shift
    if( wParam == 0x10 )
    {
        if( (lParam & KeyCode) == ShiftLeft ) 
            wParam = VK_LSHIFT;
        else if( (lParam & KeyCode) == ShiftRight)
            wParam = VK_RSHIFT;
    }

    // Handle Ctrl
    if( wParam == 0x11 )
    {
        if( lParam &  CtrlRight )
            wParam = VK_RCONTROL;
        else
            wParam = VK_LCONTROL;
    }

    // Handle ALT
    if( lParam & Alt )
        _keyModifier |= KeyMod_Alt;
    else if( wParam == 0x12 && (lParam & KeyReleased))
        _keyModifier &= ~KeyMod_Alt;

    // Special case for alt key
    if( wParam == VK_MENU )
    {
        if( lParam & 0x1000000 )
            wParam = VK_RMENU;
        else
            wParam = VK_LMENU;
    }

    KeyboardKey key = _keymap[wParam];
    
    // Check control keys
    switch( key )
    {
        case Key_Shift_R:
        case Key_Shift_L:
            if( lParam & KeyReleased )
                _keyModifier &= ~(KeyMod_Shift);
            else
                _keyModifier |= KeyMod_Shift;
            break;

   //     case Key_Caps_Lock:
			//if( lParam & (KeyRepeated|KeyReleased) )
			//	break;
			//else
			//{
			//	if( _keyModifier & KeyMod_CapsLock )
			//		_keyModifier &= ~KeyMod_CapsLock;
			//	else
			//		_keyModifier |= KeyMod_CapsLock;
			//}
			//break;
        
        case Key_Control_R:
        case Key_Control_L:
            if( lParam & KeyReleased )
                _keyModifier &= ~(KeyMod_Control);
            else
                _keyModifier |= KeyMod_Control;
            break;

        case Key_Num_Lock:
            // Under Windows we need not do anything with NumLock.
            // The OS thinks for us....
            break;

        case Key_Super_L:
        case Key_Super_R:
            /* Keys with Super modifier aren't mapped by default
            if( lParam & KeyReleased )
                _keyModifier &= ~(KeyMod_Super);
            else
                _keyModifier |= KeyMod_Super;
                */

            break;

        default:
            break;
    }

    return _charmap[_keyModifier | key ];
}
#endif

#if defined(_OSX_CGL_IMPLEMENTATION) ||  defined(_OSX_AGL_IMPLEMENTATION) 
KeyCharacter Keyboard::akeyEventToKeyChar(unsigned long key)
{
    if (_keymap.count(key)!=0) return KeyCharacter(_charmap[_keymap[key]]);
    return KeyCharacter(key);
}
#endif

void Keyboard::_initKeyboardKeyMap()
{
#ifdef _X11_IMPLEMENTATION
    _keymap[XK_Escape       ] = Key_Escape;
    _keymap[XK_F1           ] = Key_F1;
    _keymap[XK_F2           ] = Key_F2;
    _keymap[XK_F3           ] = Key_F3;
    _keymap[XK_F4           ] = Key_F4;
    _keymap[XK_F5           ] = Key_F5;
    _keymap[XK_F6           ] = Key_F6;
    _keymap[XK_F7           ] = Key_F7;
    _keymap[XK_F8           ] = Key_F8;
    _keymap[XK_F9           ] = Key_F9;
    _keymap[XK_F10          ] = Key_F10;
    _keymap[XK_F11          ] = Key_F11;
    _keymap[XK_F12          ] = Key_F12;
    _keymap[XK_quoteleft    ] = Key_quoteleft;
    _keymap[XK_1            ] = Key_1;
    _keymap[XK_2            ] = Key_2;
    _keymap[XK_3            ] = Key_3;
    _keymap[XK_4            ] = Key_4;
    _keymap[XK_5            ] = Key_5;
    _keymap[XK_6            ] = Key_6;
    _keymap[XK_7            ] = Key_7;
    _keymap[XK_8            ] = Key_8;
    _keymap[XK_9            ] = Key_9;
    _keymap[XK_0            ] = Key_0;
    _keymap[XK_minus        ] = Key_minus;
    _keymap[XK_equal        ] = Key_equal;
    _keymap[XK_BackSpace    ] = Key_BackSpace;
    _keymap[XK_Tab          ] = Key_Tab;
    _keymap[XK_a            ] = Key_A;
    _keymap[XK_b            ] = Key_B;
    _keymap[XK_c            ] = Key_C;
    _keymap[XK_d            ] = Key_D;
    _keymap[XK_e            ] = Key_E;
    _keymap[XK_f            ] = Key_F;
    _keymap[XK_g            ] = Key_G;
    _keymap[XK_h            ] = Key_H;
    _keymap[XK_i            ] = Key_I;
    _keymap[XK_j            ] = Key_J;
    _keymap[XK_k            ] = Key_K;
    _keymap[XK_l            ] = Key_L;
    _keymap[XK_m            ] = Key_M;
    _keymap[XK_n            ] = Key_N;
    _keymap[XK_o            ] = Key_O;
    _keymap[XK_p            ] = Key_P;
    _keymap[XK_q            ] = Key_Q;
    _keymap[XK_r            ] = Key_R;
    _keymap[XK_s            ] = Key_S;
    _keymap[XK_t            ] = Key_T;
    _keymap[XK_u            ] = Key_U;
    _keymap[XK_v            ] = Key_V;
    _keymap[XK_w            ] = Key_W;
    _keymap[XK_x            ] = Key_X;
    _keymap[XK_y            ] = Key_Y;
    _keymap[XK_z            ] = Key_Z;
    _keymap[XK_bracketleft  ] = Key_bracketleft;
    _keymap[XK_bracketright ] = Key_bracketright;
    _keymap[XK_backslash    ] = Key_backslash;
    _keymap[XK_Caps_Lock    ] = Key_Caps_Lock;
    _keymap[XK_semicolon    ] = Key_semicolon;
    _keymap[XK_apostrophe   ] = Key_apostrophe;
    _keymap[XK_Return       ] = Key_Return;
    _keymap[XK_Shift_L      ] = Key_Shift_L;
    _keymap[XK_comma        ] = Key_comma;
    _keymap[XK_period       ] = Key_period;
    _keymap[XK_slash        ] = Key_slash;
    _keymap[XK_Shift_R      ] = Key_Shift_R;
    _keymap[XK_Control_L    ] = Key_Control_L;
    _keymap[XK_Super_L      ] = Key_Super_L;
    _keymap[XK_space        ] = Key_space;
    _keymap[XK_Alt_L        ] = Key_Alt_L;
    _keymap[XK_Alt_R        ] = Key_Alt_R;
    _keymap[XK_Super_R      ] = Key_Super_R;
    _keymap[XK_Menu         ] = Key_Menu;
    _keymap[XK_Control_R    ] = Key_Control_R;
    _keymap[XK_Print        ] = Key_Print;
    _keymap[XK_Scroll_Lock  ] = Key_Scroll_Lock;
    _keymap[XK_Pause        ] = Key_Pause;
    _keymap[XK_Home         ] = Key_Home;
    _keymap[XK_Page_Up      ] = Key_Page_Up;
    _keymap[XK_End          ] = Key_End;
    _keymap[XK_Page_Down    ] = Key_Page_Down;
    _keymap[XK_Delete       ] = Key_Delete;
    _keymap[XK_Insert       ] = Key_Insert;
    _keymap[XK_Left         ] = Key_Left;
    _keymap[XK_Up           ] = Key_Up;
    _keymap[XK_Right        ] = Key_Right;
    _keymap[XK_Down         ] = Key_Down;
    _keymap[XK_Num_Lock     ] = Key_Num_Lock;
    _keymap[XK_KP_Divide    ] = Key_KP_Divide;
    _keymap[XK_KP_Multiply  ] = Key_KP_Multiply;
    _keymap[XK_KP_Subtract  ] = Key_KP_Subtract;
    _keymap[XK_KP_Add       ] = Key_KP_Add;
    _keymap[XK_KP_Home      ] = Key_KP_Home;
    _keymap[XK_KP_Up        ] = Key_KP_Up;
    _keymap[XK_KP_Page_Up   ] = Key_KP_Page_Up;
    _keymap[XK_KP_Left      ] = Key_KP_Left;
    _keymap[XK_KP_Begin     ] = Key_KP_Begin;
    _keymap[XK_KP_Right     ] = Key_KP_Right;
    _keymap[XK_KP_End       ] = Key_KP_End;
    _keymap[XK_KP_Down      ] = Key_KP_Down;
    _keymap[XK_KP_Page_Down ] = Key_KP_Page_Down;
    _keymap[XK_KP_Insert    ] = Key_KP_Insert;
    _keymap[XK_KP_Delete    ] = Key_KP_Delete;
    _keymap[XK_KP_Enter     ] = Key_KP_Enter;

#endif

#ifdef _WIN32_IMPLEMENTATION
    _keymap[VK_ESCAPE       ] = Key_Escape;
    _keymap[VK_F1           ] = Key_F1;
    _keymap[VK_F2           ] = Key_F2;
    _keymap[VK_F3           ] = Key_F3;
    _keymap[VK_F4           ] = Key_F4;
    _keymap[VK_F5           ] = Key_F5;
    _keymap[VK_F6           ] = Key_F6;
    _keymap[VK_F7           ] = Key_F7;
    _keymap[VK_F8           ] = Key_F8;
    _keymap[VK_F9           ] = Key_F9;
    _keymap[VK_F10          ] = Key_F10;
    _keymap[VK_F11          ] = Key_F11;
    _keymap[VK_F12          ] = Key_F12;

    // No, really!  Windows does not predefined these.
    _keymap[0xC0            ] = Key_quoteleft;
    _keymap[0X31            ] = Key_1;
    _keymap[0X32            ] = Key_2;
    _keymap[0X33            ] = Key_3;
    _keymap[0X34            ] = Key_4;
    _keymap[0X35            ] = Key_5;
    _keymap[0X36            ] = Key_6;
    _keymap[0X37            ] = Key_7;
    _keymap[0X38            ] = Key_8;
    _keymap[0X39            ] = Key_9;
    _keymap[0X30            ] = Key_0;
    _keymap[0xBD            ] = Key_minus;
    _keymap[0xBB            ] = Key_equal;
    _keymap[VK_BACK         ] = Key_BackSpace;
    _keymap[VK_TAB          ] = Key_Tab;
    _keymap[0x41            ] = Key_A;
    _keymap[0x42            ] = Key_B;
    _keymap[0x43            ] = Key_C;
    _keymap[0x44            ] = Key_D;
    _keymap[0x45            ] = Key_E;
    _keymap[0x46            ] = Key_F;
    _keymap[0x47            ] = Key_G;
    _keymap[0x48            ] = Key_H;
    _keymap[0x49            ] = Key_I;
    _keymap[0x4A            ] = Key_J;
    _keymap[0x4B            ] = Key_K;
    _keymap[0x4C            ] = Key_L;
    _keymap[0x4D            ] = Key_M;
    _keymap[0x4E            ] = Key_N;
    _keymap[0x4F            ] = Key_O;
    _keymap[0x50            ] = Key_P;
    _keymap[0x51            ] = Key_Q;
    _keymap[0x52            ] = Key_R;
    _keymap[0x53            ] = Key_S;
    _keymap[0x54            ] = Key_T;
    _keymap[0x55            ] = Key_U;
    _keymap[0x56            ] = Key_V;
    _keymap[0x57            ] = Key_W;
    _keymap[0x58            ] = Key_X;
    _keymap[0x59            ] = Key_Y;
    _keymap[0x5A            ] = Key_Z;
    _keymap[0xDB            ] = Key_bracketleft;
    _keymap[0xDD            ] = Key_bracketright;
    _keymap[0xDC            ] = Key_backslash;
    _keymap[VK_CAPITAL      ] = Key_Caps_Lock;
    _keymap[0xBA            ] = Key_semicolon;
    _keymap[0xDE            ] = Key_apostrophe;
    _keymap[VK_RETURN       ] = Key_Return;
    _keymap[VK_LSHIFT       ] = Key_Shift_L;
    _keymap[0xBC            ] = Key_comma;
    _keymap[0xBE            ] = Key_period;
    _keymap[0xBF            ] = Key_slash;
    _keymap[VK_RSHIFT       ] = Key_Shift_R;
    _keymap[VK_LCONTROL     ] = Key_Control_L;
    _keymap[VK_LWIN         ] = Key_Super_L;
    _keymap[VK_SPACE        ] = Key_space;
// ALT never really gets to the app on Win32
    _keymap[VK_LMENU        ] = Key_Alt_L;
    _keymap[VK_RMENU        ] = Key_Alt_R;
    _keymap[VK_RWIN         ] = Key_Super_R;
    _keymap[VK_APPS         ] = Key_Menu;
    _keymap[VK_RCONTROL     ] = Key_Control_R;
    _keymap[VK_PRINT        ] = Key_Print;
    _keymap[VK_SCROLL       ] = Key_Scroll_Lock;
    _keymap[VK_PAUSE        ] = Key_Pause;
    _keymap[VK_HOME         ] = Key_Home;
    _keymap[VK_PRIOR        ] = Key_Page_Up;
    _keymap[VK_END          ] = Key_End;
    _keymap[VK_NEXT         ] = Key_Page_Down;
    _keymap[VK_DELETE       ] = Key_Delete;
    _keymap[VK_INSERT       ] = Key_Insert;
    _keymap[VK_LEFT         ] = Key_Left;
    _keymap[VK_UP           ] = Key_Up;
    _keymap[VK_RIGHT        ] = Key_Right;
    _keymap[VK_DOWN         ] = Key_Down;
    _keymap[VK_NUMLOCK      ] = Key_Num_Lock;
    _keymap[VK_DIVIDE       ] = Key_KP_Divide;
    _keymap[VK_MULTIPLY     ] = Key_KP_Multiply;
    _keymap[VK_SUBTRACT     ] = Key_KP_Subtract;
    _keymap[VK_ADD          ] = Key_KP_Add;
    _keymap[VK_CLEAR        ] = Key_KP_Begin;
    _keymap[VK_DECIMAL      ] = Key_KP_Delete;
    

    _keymap[VK_NUMPAD0      ] = Key_0;
    _keymap[VK_NUMPAD1      ] = Key_1;
    _keymap[VK_NUMPAD2      ] = Key_2;
    _keymap[VK_NUMPAD3      ] = Key_3;
    _keymap[VK_NUMPAD4      ] = Key_4;
    _keymap[VK_NUMPAD5      ] = Key_5;
    _keymap[VK_NUMPAD6      ] = Key_6;
    _keymap[VK_NUMPAD7      ] = Key_7;
    _keymap[VK_NUMPAD8      ] = Key_8;
    _keymap[VK_NUMPAD9      ] = Key_9;
#endif

#if defined(_OSX_CGL_IMPLEMENTATION) ||  defined(_OSX_AGL_IMPLEMENTATION) 
    _keymap[kEscapeCharCode] = Key_Escape;
    _keymap[kHomeCharCode] = Key_Home;
    _keymap[kEnterCharCode] = Key_KP_Enter;
    _keymap[kEndCharCode] = Key_End;
    _keymap[kReturnCharCode] = Key_Return;
    _keymap[kPageUpCharCode] = Key_Page_Up;
    _keymap[kPageDownCharCode] = Key_Page_Down;
    _keymap[kLeftArrowCharCode] = Key_Left;
    _keymap[kRightArrowCharCode] = Key_Right;
    _keymap[kUpArrowCharCode] = Key_Up;
    _keymap[kDownArrowCharCode] = Key_Down;
    _keymap[kBackspaceCharCode] = Key_BackSpace;
    _keymap[kTabCharCode] = Key_Tab;
    _keymap[kSpaceCharCode] = Key_space;
    _keymap[kDeleteCharCode] = Key_Delete;

/*    
   _keymap[kHelpCharCode] = KeyChar_Help;
   kLineFeedCharCode = 10,
   kVerticalTabCharCode = 11,
   kPageUpCharCode = 11,
   kFormFeedCharCode = 12,
   kPageDownCharCode = 12,
   kReturnCharCode = 13,
   kFunctionKeyCharCode = 16,
   kCommandCharCode = 17,
   kCheckCharCode = 18,
   kDiamondCharCode = 19,
   kAppleLogoCharCode = 20,
   kClearCharCode = 27,
   kSpaceCharCode = 32,
   kDeleteCharCode = 127,
   kBulletCharCode = 165,
   kNonBreakingSpaceCharCode = 202
*/   
   #endif
}

void Keyboard::_initKeyCharMap()
{
    struct KeyTable{
        KeyboardKey key;
        KeyCharacter keyChar;
        KeyCharacter shiftedKeyChar;
        KeyCharacter numLockedKeyChar;
    }_keyTable[] = 
    {

    //          KeyboardKey       Normal KeyChar       Shifted KeyChar     Numlocked KeyChar
    //          ----------        --------------       ---------------     -----------------
    {          Key_Unknown,      KeyChar_Unknown,      KeyChar_Unknown,      KeyChar_Unknown },
    {           Key_Escape,       KeyChar_Escape,       KeyChar_Escape,       KeyChar_Escape },
    {               Key_F1,           KeyChar_F1,           KeyChar_F1,           KeyChar_F1 },
    {               Key_F2,           KeyChar_F2,           KeyChar_F2,           KeyChar_F2 },
    {               Key_F3,           KeyChar_F3,           KeyChar_F3,           KeyChar_F3 },
    {               Key_F4,           KeyChar_F4,           KeyChar_F4,           KeyChar_F4 },
    {               Key_F5,           KeyChar_F5,           KeyChar_F5,           KeyChar_F5 },
    {               Key_F6,           KeyChar_F6,           KeyChar_F6,           KeyChar_F6 },
    {               Key_F7,           KeyChar_F7,           KeyChar_F7,           KeyChar_F7 },
    {               Key_F8,           KeyChar_F8,           KeyChar_F8,           KeyChar_F8 },
    {               Key_F9,           KeyChar_F9,           KeyChar_F9,           KeyChar_F9 },
    {              Key_F10,          KeyChar_F10,          KeyChar_F10,          KeyChar_F10 },
    {              Key_F11,          KeyChar_F11,          KeyChar_F11,          KeyChar_F11 },
    {              Key_F12,          KeyChar_F12,          KeyChar_F12,          KeyChar_F12 },
    {        Key_quoteleft,    KeyChar_quoteleft,   KeyChar_asciitilde,    KeyChar_quoteleft },
    {                Key_1,            KeyChar_1,       KeyChar_exclam,            KeyChar_1 },
    {                Key_2,            KeyChar_2,           KeyChar_at,            KeyChar_2 },
    {                Key_3,            KeyChar_3,   KeyChar_numbersign,            KeyChar_3 },
    {                Key_4,            KeyChar_4,       KeyChar_dollar,            KeyChar_4 },
    {                Key_5,            KeyChar_5,      KeyChar_percent,            KeyChar_5 },
    {                Key_6,            KeyChar_6,  KeyChar_asciicircum,            KeyChar_6 },
    {                Key_7,            KeyChar_7,    KeyChar_ampersand,            KeyChar_7 },
    {                Key_8,            KeyChar_8,     KeyChar_asterisk,            KeyChar_8 },
    {                Key_9,            KeyChar_9,    KeyChar_parenleft,            KeyChar_9 },
    {                Key_0,            KeyChar_0,   KeyChar_parenright,            KeyChar_0 },
    {            Key_minus,        KeyChar_minus,   KeyChar_underscore,        KeyChar_minus },
    {            Key_equal,        KeyChar_equal,         KeyChar_plus,        KeyChar_equal },
    {        Key_BackSpace,    KeyChar_BackSpace,    KeyChar_BackSpace,    KeyChar_BackSpace },
    {              Key_Tab,          KeyChar_Tab,          KeyChar_Tab,          KeyChar_Tab },
    {                Key_A,            KeyChar_a,            KeyChar_A,            KeyChar_a },
    {                Key_B,            KeyChar_b,            KeyChar_B,            KeyChar_b },
    {                Key_C,            KeyChar_c,            KeyChar_C,            KeyChar_c },
    {                Key_D,            KeyChar_d,            KeyChar_D,            KeyChar_d },
    {                Key_E,            KeyChar_e,            KeyChar_E,            KeyChar_e },
    {                Key_F,            KeyChar_f,            KeyChar_F,            KeyChar_f },
    {                Key_G,            KeyChar_g,            KeyChar_G,            KeyChar_g },
    {                Key_H,            KeyChar_h,            KeyChar_H,            KeyChar_h },
    {                Key_I,            KeyChar_i,            KeyChar_I,            KeyChar_i },
    {                Key_J,            KeyChar_j,            KeyChar_J,            KeyChar_j },
    {                Key_K,            KeyChar_k,            KeyChar_K,            KeyChar_k },
    {                Key_L,            KeyChar_l,            KeyChar_L,            KeyChar_l },
    {                Key_M,            KeyChar_m,            KeyChar_M,            KeyChar_m },
    {                Key_N,            KeyChar_n,            KeyChar_N,            KeyChar_n },
    {                Key_O,            KeyChar_o,            KeyChar_O,            KeyChar_o },
    {                Key_P,            KeyChar_p,            KeyChar_P,            KeyChar_p },
    {                Key_Q,            KeyChar_q,            KeyChar_Q,            KeyChar_q },
    {                Key_R,            KeyChar_r,            KeyChar_R,            KeyChar_r },
    {                Key_S,            KeyChar_s,            KeyChar_S,            KeyChar_s },
    {                Key_T,            KeyChar_t,            KeyChar_T,            KeyChar_t },
    {                Key_U,            KeyChar_u,            KeyChar_U,            KeyChar_u },
    {                Key_V,            KeyChar_v,            KeyChar_V,            KeyChar_v },
    {                Key_W,            KeyChar_w,            KeyChar_W,            KeyChar_w },
    {                Key_X,            KeyChar_x,            KeyChar_X,            KeyChar_x },
    {                Key_Y,            KeyChar_y,            KeyChar_Y,            KeyChar_y },
    {                Key_Z,            KeyChar_z,            KeyChar_Z,            KeyChar_z },
    {      Key_bracketleft,  KeyChar_bracketleft,    KeyChar_braceleft,  KeyChar_bracketleft },
    {     Key_bracketright, KeyChar_bracketright,   KeyChar_braceright, KeyChar_bracketright },
    {        Key_backslash,    KeyChar_backslash,          KeyChar_bar,    KeyChar_backslash },
    {        Key_Caps_Lock,    KeyChar_Caps_Lock,    KeyChar_Caps_Lock,    KeyChar_Caps_Lock },
    {        Key_semicolon,    KeyChar_semicolon,        KeyChar_colon,    KeyChar_semicolon },
    {       Key_apostrophe,   KeyChar_apostrophe,     KeyChar_quotedbl,   KeyChar_apostrophe },
    {           Key_Return,       KeyChar_Return,       KeyChar_Return,       KeyChar_Return },
    {          Key_Shift_L,      KeyChar_Shift_L,      KeyChar_Shift_L,      KeyChar_Shift_L },
    {            Key_comma,        KeyChar_comma,         KeyChar_less,        KeyChar_comma },
    {           Key_period,       KeyChar_period,      KeyChar_greater,       KeyChar_period },
    {            Key_slash,        KeyChar_slash,     KeyChar_question,        KeyChar_slash },
    {          Key_Shift_R,      KeyChar_Shift_R,      KeyChar_Shift_R,      KeyChar_Shift_R },
    {        Key_Control_L,    KeyChar_Control_L,    KeyChar_Control_L,    KeyChar_Control_L },
    {          Key_Super_L,      KeyChar_Super_L,      KeyChar_Super_L,      KeyChar_Super_L },
    {            Key_space,        KeyChar_space,        KeyChar_space,        KeyChar_space },
    {            Key_Alt_L,        KeyChar_Alt_L,        KeyChar_Alt_L,        KeyChar_Alt_L },
    {            Key_Alt_R,        KeyChar_Alt_R,        KeyChar_Alt_R,        KeyChar_Alt_R },
    {          Key_Super_R,      KeyChar_Super_R,      KeyChar_Super_R,      KeyChar_Super_R },
    {             Key_Menu,         KeyChar_Menu,         KeyChar_Menu,         KeyChar_Menu },
    {        Key_Control_R,    KeyChar_Control_R,    KeyChar_Control_R,    KeyChar_Control_R },
    {            Key_Print,      KeyChar_Sys_Req,      KeyChar_Sys_Req,      KeyChar_Sys_Req },
    {      Key_Scroll_Lock,  KeyChar_Scroll_Lock,  KeyChar_Scroll_Lock,  KeyChar_Scroll_Lock },
    {            Key_Pause,        KeyChar_Break,        KeyChar_Break,        KeyChar_Break },
    {             Key_Home,         KeyChar_Home,         KeyChar_Home,         KeyChar_Home },
    {          Key_Page_Up,      KeyChar_Page_Up,      KeyChar_Page_Up,      KeyChar_Page_Up },
    {              Key_End,          KeyChar_End,          KeyChar_End,          KeyChar_End },
    {        Key_Page_Down,    KeyChar_Page_Down,    KeyChar_Page_Down,    KeyChar_Page_Down },
    {           Key_Delete,       KeyChar_Delete,       KeyChar_Delete,       KeyChar_Delete },
    {           Key_Insert,       KeyChar_Insert,       KeyChar_Insert,       KeyChar_Insert },
    {             Key_Left,         KeyChar_Left,         KeyChar_Left,         KeyChar_Left },
    {               Key_Up,           KeyChar_Up,           KeyChar_Up,           KeyChar_Up },
    {            Key_Right,        KeyChar_Right,        KeyChar_Right,        KeyChar_Right },
    {             Key_Down,         KeyChar_Down,         KeyChar_Down,         KeyChar_Down },
    {         Key_Num_Lock,     KeyChar_Num_Lock,     KeyChar_Num_Lock,     KeyChar_Num_Lock },
    {        Key_KP_Divide,    KeyChar_KP_Divide,    KeyChar_KP_Divide,    KeyChar_KP_Divide },
    {      Key_KP_Multiply,  KeyChar_KP_Multiply,  KeyChar_KP_Multiply,  KeyChar_KP_Multiply },
    {      Key_KP_Subtract,  KeyChar_KP_Subtract,  KeyChar_KP_Subtract,  KeyChar_KP_Subtract },
    {           Key_KP_Add,       KeyChar_KP_Add,       KeyChar_KP_Add,       KeyChar_KP_Add },
    {          Key_KP_Home,      KeyChar_KP_Home,      KeyChar_KP_Home,            KeyChar_7 },
    {            Key_KP_Up,        KeyChar_KP_Up,        KeyChar_KP_Up,            KeyChar_8 },
    {       Key_KP_Page_Up,   KeyChar_KP_Page_Up,   KeyChar_KP_Page_Up,            KeyChar_9 },
    {          Key_KP_Left,      KeyChar_KP_Left,      KeyChar_KP_Left,            KeyChar_4 },
    {         Key_KP_Begin,     KeyChar_KP_Begin,     KeyChar_KP_Begin,            KeyChar_5 },
    {         Key_KP_Right,     KeyChar_KP_Right,     KeyChar_KP_Right,            KeyChar_6 },
    {           Key_KP_End,       KeyChar_KP_End,       KeyChar_KP_End,            KeyChar_1 },
    {          Key_KP_Down,      KeyChar_KP_Down,      KeyChar_KP_Down,            KeyChar_2 },
    {     Key_KP_Page_Down, KeyChar_KP_Page_Down, KeyChar_KP_Page_Down,            KeyChar_3 },
    {        Key_KP_Insert,    KeyChar_KP_Insert,    KeyChar_KP_Insert,            KeyChar_0 },
    {        Key_KP_Delete,    KeyChar_KP_Delete,    KeyChar_KP_Delete,       KeyChar_period },
    {         Key_KP_Enter,     KeyChar_KP_Enter,     KeyChar_KP_Enter,     KeyChar_KP_Enter },
    {         Key_LAST_KEY,      KeyChar_Unknown,      KeyChar_Unknown,      KeyChar_Unknown },

    };

    _charmap.clear();

    int n = sizeof(_keyTable)/sizeof(KeyTable);
    for( int j = 0; j < 0x20; j++ )
    {
        unsigned long mod = j<<16;
        for( int i = 0; i < n; i++ )
        {
            KeyCharacter keyChar = KeyChar_Unknown;
            // Key is affected by shift
            if( _keyTable[i].keyChar != _keyTable[i].shiftedKeyChar )
            {
                unsigned long mask = mod & (KeyMod_Shift | KeyMod_CapsLock);

                keyChar = (mask == 0 || mask == (KeyMod_Shift | KeyMod_CapsLock)) ?
                                    _keyTable[i].keyChar :
                                    _keyTable[i].shiftedKeyChar;
                
            }
            // Key is affected by Num lock
            else if( _keyTable[i].keyChar != _keyTable[i].numLockedKeyChar )
            {
                if( mod & KeyMod_NumLock )
                    keyChar = _keyTable[i].numLockedKeyChar;
                else
                    keyChar = _keyTable[i].keyChar;
            }
            else
                keyChar = _keyTable[i].keyChar;

            _charmap[mod | _keyTable[i].key] = keyChar;
        }
    }

    //_charmap[KeyMod_Control | Key_A           ] = KeyCharacter(0x0007); // Ascii control characters
    //_charmap[KeyMod_Control | Key_B           ] = KeyCharacter(0x0008);
    //_charmap[KeyMod_Control | Key_F           ] = KeyCharacter(0x000C);
    //_charmap[KeyMod_Control | Key_L           ] = KeyCharacter(0x000A);
    //_charmap[KeyMod_Control | Key_R           ] = KeyCharacter(0x000D);
    //_charmap[KeyMod_Control | Key_T           ] = KeyCharacter(0x0009);
    //_charmap[KeyMod_Control | Key_V           ] = KeyCharacter(0x000B);
}
