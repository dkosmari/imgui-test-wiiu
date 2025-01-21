#include <iostream>
#include <iomanip>

#include <SDL.h>

#include "show.hpp"


using std::cout;
using std::endl;
using std::hex;
using std::dec;


void
show(const SDL_AudioDeviceEvent& e)
{
    const char* type = e.type == SDL_AUDIODEVICEADDED ? "added" : "removed";
    cout << "AudioDeviceEvent: type=" << type
         << ", which=" << e.which
         << ", iscapture=" << bool(e.iscapture)
         << endl;
}


void
show(const SDL_ControllerAxisEvent& e)
{
    cout << "ControllerAxisEvent: which=" << e.which
         << ", axis=" << unsigned(e.axis)
         << ", value=" << e.value
         << endl;
}


void
show(const SDL_ControllerButtonEvent& e)
{
    const char* state = e.state == SDL_PRESSED ? "pressed" : "released";

    auto b = static_cast<SDL_GameControllerButton>(e.button);
    const char* button_name = SDL_GameControllerGetStringForButton(b);

    cout << "ControllerButtonEvent: which=" << int(e.which)
         << ", button=" << button_name << " (" << unsigned(e.button) << ")"
         << ", state=" << state
         << endl;
}


void
show(const SDL_ControllerDeviceEvent& e)
{
    const char* type = "invalid";
    switch (e.type) {
    case SDL_CONTROLLERDEVICEADDED:
        type = "added";
        break;
    case SDL_CONTROLLERDEVICEREMOVED:
        type = "removed";
        break;
    case SDL_CONTROLLERDEVICEREMAPPED:
        type = "remapped";
        break;
    }
    cout << "ControllerDeviceEvent: type=" << type
         << ", which=" << e.which
         << endl;
}


void
show(const SDL_ControllerSensorEvent& e)
{
    const char* sensor = "invalid";
    switch (e.sensor) {
    case SDL_SENSOR_UNKNOWN:
        sensor = "unknown";
        break;
    case SDL_SENSOR_ACCEL:
        sensor = "accel";
        break;
    case SDL_SENSOR_GYRO:
        sensor = "gyro";
        break;
    case SDL_SENSOR_ACCEL_L:
        sensor = "accel-l";
        break;
    case SDL_SENSOR_GYRO_L:
        sensor = "gyro-l";
        break;
    case SDL_SENSOR_ACCEL_R:
        sensor = "accel-r";
        break;
    case SDL_SENSOR_GYRO_R:
        sensor = "gyro-r";
        break;
    }
    cout << "ControllerSensorEvent: which=" << e.which
         << ", sensor=" << sensor
         << endl;
}


void
show(const SDL_ControllerTouchpadEvent& e)
{
    const char* type = "invalid";
    switch (e.type) {
    case SDL_CONTROLLERTOUCHPADDOWN:
        type = "down";
        break;
    case SDL_CONTROLLERTOUCHPADMOTION:
        type = "motion";
        break;
    case SDL_CONTROLLERTOUCHPADUP:
        type = "up";
        break;
    }
    cout << "ControllerTouchpadEvent: type=" << type
         << ", which=" << e.which
         << ", touchpad=" << e.touchpad
         << ", finger=" << e.finger
         << ", x=" << e.x
         << ", y=" << e.y
         << ", pressure=" << e.pressure
         << endl;
}


void
show(const SDL_DisplayEvent& e)
{
    const char* event = "invalid";
    switch (e.event) {
    case SDL_DISPLAYEVENT_ORIENTATION:
        event = "orientation";
        break;
    case SDL_DISPLAYEVENT_CONNECTED:
        event = "connected";
        break;
    case SDL_DISPLAYEVENT_DISCONNECTED:
        event = "disconnected";
        break;
    }
    cout << "DisplayEvent: display=" << e.display
         << ", event=" << event
         << endl;
}


void
show(const SDL_DropEvent& e)
{
    const char* type;
    switch (e.type) {
    case SDL_DROPBEGIN:
        type = "drop-begin";
        break;
    case SDL_DROPFILE:
        type = "drop-file";
        break;
    case SDL_DROPTEXT:
        type = "drop-text";
        break;
    case SDL_DROPCOMPLETE:
        type = "drop-complete";
        break;
    default:
        type = "invalid";
    }

    cout << "DropEvent: type=" << type
         << ", file=";
    if (e.file)
        cout << "\"" << e.file << "\"";
    else
        cout << "<NULL>";
    cout << ", windowID=" << e.windowID
         << endl;
}


void
show(const SDL_JoyAxisEvent& e)
{
    cout << "JoyAxisEvent: which=" << e.which
         << ", axis=" << unsigned(e.axis)
         << ", value=" << e.value
         << endl;
}


void
show(const SDL_JoyBallEvent& e)
{
    cout << "JoyBallEvent: which=" << e.which
         << ", ball=" << unsigned(e.ball)
         << ", xrel=" << e.xrel
         << ", yrel=" << e.yrel
         << endl;
}


void
show(const SDL_JoyBatteryEvent& e)
{
    const char* level;
    switch (e.level) {
    case SDL_JOYSTICK_POWER_EMPTY:
        level = "empty";
        break;
    case SDL_JOYSTICK_POWER_LOW:
        level = "low";
        break;
    case SDL_JOYSTICK_POWER_MEDIUM:
        level = "medium";
        break;
    case SDL_JOYSTICK_POWER_FULL:
        level = "full";
        break;
    case SDL_JOYSTICK_POWER_WIRED:
        level = "wired";
        break;
    default:
        level = "invalid";
    }
    cout << "JoyBatteryEvent: which=" << e.which
         << ", level=" << level
         << endl;
}


void
show(const SDL_JoyButtonEvent& e)
{
    const char* state = e.state == SDL_PRESSED ? "pressed" : "released";
    cout << "JoyButtonEvent: which=" << e.which
         << ", button=" << unsigned(e.button)
         << ", state=" << state
         << endl;
}


void
show(const SDL_JoyDeviceEvent& e)
{
    const char* type = e.type == SDL_JOYDEVICEADDED ? "added" : "removed";
    cout << "JoyDeviceEvent: type=" << type
         << ", which=" << e.which << endl;
}


void
show(const SDL_JoyHatEvent& e)
{
    const char* value = "invalid";
    switch (e.value) {
    case SDL_HAT_CENTERED:
        value = "centered";
        break;
    case SDL_HAT_UP:
        value = "up";
        break;
    case SDL_HAT_RIGHT:
        value = "right";
        break;
    case SDL_HAT_DOWN:
        value = "down";
        break;
    case SDL_HAT_LEFT:
        value = "left";
        break;
    case SDL_HAT_RIGHTUP:
        value = "rightup";
        break;
    case SDL_HAT_RIGHTDOWN:
        value = "rightdown";
        break;
    case SDL_HAT_LEFTUP:
        value = "leftup";
        break;
    case SDL_HAT_LEFTDOWN:
        value = "leftdown";
        break;
    }
    cout << "JoyHatEvent: which=" << e.which
         << ", hat=" << unsigned(e.hat)
         << ", value=" << value
         << endl;
}


#define CASE(x)                                    \
    case x:                                        \
        name = #x;                                 \
        break

std::ostream&
operator <<(std::ostream& out, SDL_Scancode code)
{
    const char* name;
    switch (code) {
        CASE(SDL_SCANCODE_UNKNOWN);

        CASE(SDL_SCANCODE_A);
        CASE(SDL_SCANCODE_B);
        CASE(SDL_SCANCODE_C);
        CASE(SDL_SCANCODE_E);
        CASE(SDL_SCANCODE_F);
        CASE(SDL_SCANCODE_G);
        CASE(SDL_SCANCODE_H);
        CASE(SDL_SCANCODE_I);
        CASE(SDL_SCANCODE_J);
        CASE(SDL_SCANCODE_K);
        CASE(SDL_SCANCODE_L);
        CASE(SDL_SCANCODE_M);
        CASE(SDL_SCANCODE_N);
        CASE(SDL_SCANCODE_O);
        CASE(SDL_SCANCODE_P);
        CASE(SDL_SCANCODE_Q);
        CASE(SDL_SCANCODE_R);
        CASE(SDL_SCANCODE_S);
        CASE(SDL_SCANCODE_T);
        CASE(SDL_SCANCODE_U);
        CASE(SDL_SCANCODE_V);
        CASE(SDL_SCANCODE_W);
        CASE(SDL_SCANCODE_X);
        CASE(SDL_SCANCODE_Y);
        CASE(SDL_SCANCODE_Z);

        CASE(SDL_SCANCODE_1);
        CASE(SDL_SCANCODE_2);
        CASE(SDL_SCANCODE_3);
        CASE(SDL_SCANCODE_4);
        CASE(SDL_SCANCODE_5);
        CASE(SDL_SCANCODE_6);
        CASE(SDL_SCANCODE_7);
        CASE(SDL_SCANCODE_8);
        CASE(SDL_SCANCODE_9);
        CASE(SDL_SCANCODE_0);

        CASE(SDL_SCANCODE_RETURN);
        CASE(SDL_SCANCODE_ESCAPE);
        CASE(SDL_SCANCODE_BACKSPACE);
        CASE(SDL_SCANCODE_TAB);
        CASE(SDL_SCANCODE_SPACE);

        CASE(SDL_SCANCODE_MINUS);
        CASE(SDL_SCANCODE_EQUALS);
        CASE(SDL_SCANCODE_LEFTBRACKET);
        CASE(SDL_SCANCODE_RIGHTBRACKET);
        CASE(SDL_SCANCODE_BACKSLASH);
        CASE(SDL_SCANCODE_NONUSHASH);
        CASE(SDL_SCANCODE_SEMICOLON);
        CASE(SDL_SCANCODE_APOSTROPHE);
        CASE(SDL_SCANCODE_GRAVE);
        CASE(SDL_SCANCODE_COMMA);
        CASE(SDL_SCANCODE_PERIOD);
        CASE(SDL_SCANCODE_SLASH);

        CASE(SDL_SCANCODE_CAPSLOCK);

        CASE(SDL_SCANCODE_F1);
        CASE(SDL_SCANCODE_F2);
        CASE(SDL_SCANCODE_F3);
        CASE(SDL_SCANCODE_F4);
        CASE(SDL_SCANCODE_F5);
        CASE(SDL_SCANCODE_F6);
        CASE(SDL_SCANCODE_F7);
        CASE(SDL_SCANCODE_F8);
        CASE(SDL_SCANCODE_F9);
        CASE(SDL_SCANCODE_F10);
        CASE(SDL_SCANCODE_F11);
        CASE(SDL_SCANCODE_F12);

        CASE(SDL_SCANCODE_PRINTSCREEN);
        CASE(SDL_SCANCODE_SCROLLLOCK);
        CASE(SDL_SCANCODE_PAUSE);
        CASE(SDL_SCANCODE_INSERT);
        CASE(SDL_SCANCODE_HOME);
        CASE(SDL_SCANCODE_PAGEUP);
        CASE(SDL_SCANCODE_DELETE);
        CASE(SDL_SCANCODE_END);
        CASE(SDL_SCANCODE_PAGEDOWN);
        CASE(SDL_SCANCODE_RIGHT);
        CASE(SDL_SCANCODE_LEFT);
        CASE(SDL_SCANCODE_DOWN);
        CASE(SDL_SCANCODE_UP);

        CASE(SDL_SCANCODE_NUMLOCKCLEAR);

        CASE(SDL_SCANCODE_KP_DIVIDE);
        CASE(SDL_SCANCODE_KP_MULTIPLY);
        CASE(SDL_SCANCODE_KP_MINUS);
        CASE(SDL_SCANCODE_KP_PLUS);
        CASE(SDL_SCANCODE_KP_ENTER);
        CASE(SDL_SCANCODE_KP_1);
        CASE(SDL_SCANCODE_KP_2);
        CASE(SDL_SCANCODE_KP_3);
        CASE(SDL_SCANCODE_KP_4);
        CASE(SDL_SCANCODE_KP_5);
        CASE(SDL_SCANCODE_KP_6);
        CASE(SDL_SCANCODE_KP_7);
        CASE(SDL_SCANCODE_KP_8);
        CASE(SDL_SCANCODE_KP_9);
        CASE(SDL_SCANCODE_KP_0);
        CASE(SDL_SCANCODE_KP_PERIOD);

        CASE(SDL_SCANCODE_NONUSBACKSLASH);
        CASE(SDL_SCANCODE_APPLICATION);
        CASE(SDL_SCANCODE_KP_EQUALS);
        CASE(SDL_SCANCODE_F13);
        CASE(SDL_SCANCODE_F14);
        CASE(SDL_SCANCODE_F15);
        CASE(SDL_SCANCODE_F16);
        CASE(SDL_SCANCODE_F17);
        CASE(SDL_SCANCODE_F18);
        CASE(SDL_SCANCODE_F19);
        CASE(SDL_SCANCODE_F20);
        CASE(SDL_SCANCODE_F21);
        CASE(SDL_SCANCODE_F22);
        CASE(SDL_SCANCODE_F23);
        CASE(SDL_SCANCODE_F24);
        CASE(SDL_SCANCODE_EXECUTE);
        CASE(SDL_SCANCODE_HELP);
        CASE(SDL_SCANCODE_MENU);
        CASE(SDL_SCANCODE_SELECT);
        CASE(SDL_SCANCODE_STOP);
        CASE(SDL_SCANCODE_AGAIN);
        CASE(SDL_SCANCODE_UNDO);
        CASE(SDL_SCANCODE_CUT);
        CASE(SDL_SCANCODE_COPY);
        CASE(SDL_SCANCODE_PASTE);
        CASE(SDL_SCANCODE_FIND);
        CASE(SDL_SCANCODE_MUTE);
        CASE(SDL_SCANCODE_VOLUMEUP);
        CASE(SDL_SCANCODE_VOLUMEDOWN);

        CASE(SDL_SCANCODE_KP_COMMA);
        CASE(SDL_SCANCODE_KP_EQUALSAS400);

        CASE(SDL_SCANCODE_INTERNATIONAL1);
        CASE(SDL_SCANCODE_INTERNATIONAL2);
        CASE(SDL_SCANCODE_INTERNATIONAL3);
        CASE(SDL_SCANCODE_INTERNATIONAL4);
        CASE(SDL_SCANCODE_INTERNATIONAL5);
        CASE(SDL_SCANCODE_INTERNATIONAL6);
        CASE(SDL_SCANCODE_INTERNATIONAL7);
        CASE(SDL_SCANCODE_INTERNATIONAL8);
        CASE(SDL_SCANCODE_INTERNATIONAL9);
        CASE(SDL_SCANCODE_LANG1);
        CASE(SDL_SCANCODE_LANG2);
        CASE(SDL_SCANCODE_LANG3);
        CASE(SDL_SCANCODE_LANG4);
        CASE(SDL_SCANCODE_LANG5);
        CASE(SDL_SCANCODE_LANG6);
        CASE(SDL_SCANCODE_LANG7);
        CASE(SDL_SCANCODE_LANG8);
        CASE(SDL_SCANCODE_LANG9);

        CASE(SDL_SCANCODE_ALTERASE);
        CASE(SDL_SCANCODE_SYSREQ);
        CASE(SDL_SCANCODE_CANCEL);
        CASE(SDL_SCANCODE_CLEAR);
        CASE(SDL_SCANCODE_PRIOR);
        CASE(SDL_SCANCODE_RETURN2);
        CASE(SDL_SCANCODE_SEPARATOR);
        CASE(SDL_SCANCODE_OUT);
        CASE(SDL_SCANCODE_OPER);
        CASE(SDL_SCANCODE_CLEARAGAIN);
        CASE(SDL_SCANCODE_CRSEL);
        CASE(SDL_SCANCODE_EXSEL);

        CASE(SDL_SCANCODE_KP_00);
        CASE(SDL_SCANCODE_KP_000);
        CASE(SDL_SCANCODE_THOUSANDSSEPARATOR);
        CASE(SDL_SCANCODE_DECIMALSEPARATOR);
        CASE(SDL_SCANCODE_CURRENCYUNIT);
        CASE(SDL_SCANCODE_CURRENCYSUBUNIT);
        CASE(SDL_SCANCODE_KP_LEFTPAREN);
        CASE(SDL_SCANCODE_KP_RIGHTPAREN);
        CASE(SDL_SCANCODE_KP_LEFTBRACE);
        CASE(SDL_SCANCODE_KP_RIGHTBRACE);
        CASE(SDL_SCANCODE_KP_TAB);
        CASE(SDL_SCANCODE_KP_BACKSPACE);
        CASE(SDL_SCANCODE_KP_A);
        CASE(SDL_SCANCODE_KP_B);
        CASE(SDL_SCANCODE_KP_C);
        CASE(SDL_SCANCODE_KP_D);
        CASE(SDL_SCANCODE_KP_E);
        CASE(SDL_SCANCODE_KP_F);
        CASE(SDL_SCANCODE_KP_XOR);
        CASE(SDL_SCANCODE_KP_POWER);
        CASE(SDL_SCANCODE_KP_PERCENT);
        CASE(SDL_SCANCODE_KP_LESS);
        CASE(SDL_SCANCODE_KP_GREATER);
        CASE(SDL_SCANCODE_KP_AMPERSAND);
        CASE(SDL_SCANCODE_KP_DBLAMPERSAND);
        CASE(SDL_SCANCODE_KP_VERTICALBAR);
        CASE(SDL_SCANCODE_KP_DBLVERTICALBAR);
        CASE(SDL_SCANCODE_KP_COLON);
        CASE(SDL_SCANCODE_KP_HASH);
        CASE(SDL_SCANCODE_KP_SPACE);
        CASE(SDL_SCANCODE_KP_AT);
        CASE(SDL_SCANCODE_KP_EXCLAM);
        CASE(SDL_SCANCODE_KP_MEMSTORE);
        CASE(SDL_SCANCODE_KP_MEMRECALL);
        CASE(SDL_SCANCODE_KP_MEMCLEAR);
        CASE(SDL_SCANCODE_KP_MEMADD);
        CASE(SDL_SCANCODE_KP_MEMSUBTRACT);
        CASE(SDL_SCANCODE_KP_MEMMULTIPLY);
        CASE(SDL_SCANCODE_KP_MEMDIVIDE);
        CASE(SDL_SCANCODE_KP_PLUSMINUS);
        CASE(SDL_SCANCODE_KP_CLEAR);
        CASE(SDL_SCANCODE_KP_CLEARENTRY);
        CASE(SDL_SCANCODE_KP_BINARY);
        CASE(SDL_SCANCODE_KP_OCTAL);
        CASE(SDL_SCANCODE_KP_DECIMAL);
        CASE(SDL_SCANCODE_KP_HEXADECIMAL);

        CASE(SDL_SCANCODE_LCTRL);
        CASE(SDL_SCANCODE_LSHIFT);
        CASE(SDL_SCANCODE_LALT);
        CASE(SDL_SCANCODE_LGUI);
        CASE(SDL_SCANCODE_RCTRL);
        CASE(SDL_SCANCODE_RSHIFT);
        CASE(SDL_SCANCODE_RALT);
        CASE(SDL_SCANCODE_RGUI);

        CASE(SDL_SCANCODE_MODE);

        CASE(SDL_SCANCODE_AUDIONEXT);
        CASE(SDL_SCANCODE_AUDIOPREV);
        CASE(SDL_SCANCODE_AUDIOSTOP);
        CASE(SDL_SCANCODE_AUDIOPLAY);
        CASE(SDL_SCANCODE_AUDIOMUTE);
        CASE(SDL_SCANCODE_MEDIASELECT);
        CASE(SDL_SCANCODE_WWW);
        CASE(SDL_SCANCODE_MAIL);
        CASE(SDL_SCANCODE_CALCULATOR);
        CASE(SDL_SCANCODE_COMPUTER);
        CASE(SDL_SCANCODE_AC_SEARCH);
        CASE(SDL_SCANCODE_AC_HOME);
        CASE(SDL_SCANCODE_AC_BACK);
        CASE(SDL_SCANCODE_AC_FORWARD);
        CASE(SDL_SCANCODE_AC_STOP);
        CASE(SDL_SCANCODE_AC_REFRESH);
        CASE(SDL_SCANCODE_AC_BOOKMARKS);

        CASE(SDL_SCANCODE_BRIGHTNESSDOWN);
        CASE(SDL_SCANCODE_BRIGHTNESSUP);
        CASE(SDL_SCANCODE_DISPLAYSWITCH);
        CASE(SDL_SCANCODE_KBDILLUMTOGGLE);
        CASE(SDL_SCANCODE_KBDILLUMDOWN);
        CASE(SDL_SCANCODE_KBDILLUMUP);
        CASE(SDL_SCANCODE_EJECT);
        CASE(SDL_SCANCODE_SLEEP);

        CASE(SDL_SCANCODE_APP1);
        CASE(SDL_SCANCODE_APP2);

        CASE(SDL_SCANCODE_AUDIOREWIND);
        CASE(SDL_SCANCODE_AUDIOFASTFORWARD);

        CASE(SDL_SCANCODE_SOFTLEFT);
        CASE(SDL_SCANCODE_SOFTRIGHT);
        CASE(SDL_SCANCODE_CALL);
        CASE(SDL_SCANCODE_ENDCALL);

    default:
        name = "invalid";
    }

    return out << name;
}

#undef CASE


void
show(const SDL_KeyboardEvent& e)
{
    const char* type = e.type == SDL_KEYDOWN ? "down" : "up";
    const char* state = e.state == SDL_PRESSED ? "pressed" : "released";
    cout << "KeyboardEvent: type=" << type
         << ", windowID=" << e.windowID
         << ", state=" << state
         << ", scancode=" << e.keysym.scancode
        // TODO: print out SDL_Keycode and modifier
         << endl;
}


void
show(const SDL_MouseButtonEvent& e)
{
    const char* state = e.state == SDL_PRESSED ? "pressed" : "released";
    cout << "MouseButtonEvent: windowID=" << e.windowID
         << ", which=";
    if (e.which == SDL_TOUCH_MOUSEID)
        cout << "touch";
    else
        cout << e.which;
    cout << ", button=" << unsigned(e.button)
         << ", state=" << state
         << ", clicks=" << unsigned(e.clicks)
         << ", x=" << e.x
         << ", y=" << e.y
         << endl;
}


void
show(const SDL_MouseMotionEvent& e)
{
    cout << "MouseMotionEvent: windowID=" << e.windowID
         << ", which=" << e.which
         << ", state=" << hex << e.state << dec
         << ", x=" << e.x
         << ", y=" << e.y
         << ", xrel=" << e.xrel
         << ", yrel=" << e.yrel
         << endl;
}


void
show(const SDL_MouseWheelEvent& e)
{
    const char* direction = e.direction == SDL_MOUSEWHEEL_NORMAL ? "normal" : "flipped";
    cout << "MouseWheelEvent: windowID=" << e.windowID
         << ", which=" << e.which
         << ", direction=" << direction
         << ", x=" << e.x
         << ", y=" << e.y
         << ", preciseX=" << e.preciseX
         << ", preciseY=" << e.preciseY
         << ", mouseX=" << e.mouseX
         << ", mouseY=" << e.mouseY
         << endl;
}


void
show(const SDL_QuitEvent&)
{
    cout << "QuitEvent" << endl;
}


void
show(const SDL_TextEditingEvent& e)
{
    cout << "TextEditingEvent: windowID=" << e.windowID
         << ", text=\"" << e.text << "\""
         << ", start=" << e.start
         << ", length=" << e.length
         << endl;
}


void
show(const SDL_TextEditingExtEvent& e)
{
    cout << "TextEditingExtEvent: windowID=" << e.windowID
         << ", text=\"" << e.text << "\""
         << ", start=" << e.start
         << ", length=" << e.length
         << endl;
}


void
show(const SDL_TextInputEvent& e)
{
    cout << "TextInputEvent: windowID=" << e.windowID
         << ", text=\"" << e.text << "\""
         << endl;
}


void
show(const SDL_TouchFingerEvent& e)
{
    const char* type = "invalid";
    switch (e.type) {
    case SDL_FINGERMOTION:
        type = "motion";
        break;
    case SDL_FINGERDOWN:
        type = "down";
        break;
    case SDL_FINGERUP:
        type = "up";
        break;
    }
    cout << "TouchFingerEvent: type=" << type
         << ", touchID=" << e.touchId
         << ", fingerID=" << e.fingerId
         << ", x=" << e.x
         << ", y=" << e.y
         << ", dx=" << e.dx
         << ", dy=" << e.dy
         << ", pressure=" << e.pressure
         << ", windowID=" << e.windowID
         << endl;
}


void
show(const SDL_WindowEvent& e)
{
    const char* event;
    switch (e.event) {
    case SDL_WINDOWEVENT_SHOWN:
        event = "shown";
        break;
    case SDL_WINDOWEVENT_HIDDEN:
        event = "hidden";
        break;
    case SDL_WINDOWEVENT_EXPOSED:
        event = "exposed";
        break;
    case SDL_WINDOWEVENT_MOVED:
        event = "moved";
        break;
    case SDL_WINDOWEVENT_RESIZED:
        event = "resized";
        break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        event = "size-changed";
        break;
    case SDL_WINDOWEVENT_MINIMIZED:
        event = "minimized";
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        event = "maximized";
        break;
    case SDL_WINDOWEVENT_RESTORED:
        event = "restored";
        break;
    case SDL_WINDOWEVENT_ENTER:
        event = "enter";
        break;
    case SDL_WINDOWEVENT_LEAVE:
        event = "leave";
        break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
        event = "focus-gained";
        break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
        event = "focus-lost";
        break;
    case SDL_WINDOWEVENT_CLOSE:
        event = "close";
        break;
    case SDL_WINDOWEVENT_TAKE_FOCUS:
        event = "take-focus";
        break;
    case SDL_WINDOWEVENT_HIT_TEST:
        event = "hit-test";
        break;
    case SDL_WINDOWEVENT_ICCPROF_CHANGED:
        event = "iccprof-changed";
        break;
    case SDL_WINDOWEVENT_DISPLAY_CHANGED:
        event = "display-changed";
        break;
    default:
        event = "invalid";
    }
    cout << "WindowEvent: windowID=" << e.windowID
         << ", event=" << event
         << endl;
}



#define CASE(x)                                    \
    case x:                                        \
        name = #x;                                 \
        break

void
show(const SDL_Event& e)
{
    const char* name = nullptr;

    switch (e.type) {

        case SDL_QUIT:
            show(e.quit);
            break;

        CASE(SDL_APP_TERMINATING);
        CASE(SDL_APP_LOWMEMORY);
        CASE(SDL_APP_WILLENTERBACKGROUND);
        CASE(SDL_APP_DIDENTERBACKGROUND);
        CASE(SDL_APP_WILLENTERFOREGROUND);
        CASE(SDL_APP_DIDENTERFOREGROUND);

        CASE(SDL_LOCALECHANGED);

        case SDL_DISPLAYEVENT:
            show(e.display);
            break;
        case SDL_WINDOWEVENT:
            show(e.window);
            break;

        CASE(SDL_SYSWMEVENT);

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            show(e.key);
            break;

        case SDL_TEXTEDITING:
            show(e.edit);
            break;

        case SDL_TEXTEDITING_EXT:
            show(e.editExt);
            break;

        case SDL_TEXTINPUT:
            show(e.text);
            break;

        CASE(SDL_KEYMAPCHANGED);

        case SDL_MOUSEMOTION:
            show(e.motion);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            show(e.button);
            break;

        case SDL_MOUSEWHEEL:
            show(e.wheel);
            break;

        case SDL_JOYAXISMOTION:
            show(e.jaxis);
            break;

        case SDL_JOYBALLMOTION:
            show(e.jball);
            break;

        case SDL_JOYHATMOTION:
            show(e.jhat);
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            show(e.jbutton);
            break;

        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
            show(e.jdevice);
            break;

        case SDL_JOYBATTERYUPDATED:
            show(e.jbattery);
            break;

        case SDL_CONTROLLERAXISMOTION:
            show(e.caxis);
            break;

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            show(e.cbutton);
            break;

        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED:
            show(e.cdevice);
            break;

        case SDL_CONTROLLERTOUCHPADDOWN:
        case SDL_CONTROLLERTOUCHPADMOTION:
        case SDL_CONTROLLERTOUCHPADUP:
            show(e.ctouchpad);
            break;

        case SDL_CONTROLLERSENSORUPDATE:
            show(e.csensor);
            break;

        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
        case SDL_FINGERMOTION:
            show(e.tfinger);
            break;

        CASE(SDL_DOLLARGESTURE);
        CASE(SDL_DOLLARRECORD);
        CASE(SDL_MULTIGESTURE);
        CASE(SDL_CLIPBOARDUPDATE);

        case SDL_DROPFILE:
        case SDL_DROPTEXT:
        case SDL_DROPBEGIN:
        case SDL_DROPCOMPLETE:
            show(e.drop);
            break;

        case SDL_AUDIODEVICEADDED:
        case SDL_AUDIODEVICEREMOVED:
            show(e.adevice);
            break;

        CASE(SDL_SENSORUPDATE);
        CASE(SDL_RENDER_TARGETS_RESET);
        CASE(SDL_RENDER_DEVICE_RESET);
        CASE(SDL_POLLSENTINEL);

        default:
            name = "unknown";
    }
    if (name)
        cout << "Event: " << name << endl;
}

#undef CASE
