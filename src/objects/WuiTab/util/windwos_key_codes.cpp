#include "input/webUiInputKeyboardCodes.hpp"
#include "input/webUiKeyEvent.hpp"
#include "input_translation.hpp"

namespace godot
{

    wui::KeyboardCodesPOSIX getWindows_key_code_printable(const Ref<InputEventKey>& keyEvent)
    {
        int keycode = keyEvent->get_keycode();

        switch (keycode)
        {
            case KEY_A:
            case KEY_B:
            case KEY_C:
            case KEY_D:
            case KEY_E:
            case KEY_F:
            case KEY_G:
            case KEY_H:
            case KEY_I:
            case KEY_J:
            case KEY_K:
            case KEY_L:
            case KEY_M:
            case KEY_N:
            case KEY_O:
            case KEY_P:
            case KEY_Q:
            case KEY_R:
            case KEY_S:
            case KEY_T:
            case KEY_U:
            case KEY_V:
            case KEY_W:
            case KEY_X:
            case KEY_Y:
            case KEY_Z:
                return static_cast<wui::KeyboardCodesPOSIX>(
                    static_cast<int>(wui::KeyboardCodesPOSIX::VKEY_A) +
                    (keycode - static_cast<int>(KEY_A)));
            case KEY_0:
            case KEY_1:
            case KEY_2:
            case KEY_3:
            case KEY_4:
            case KEY_5:
            case KEY_6:
            case KEY_7:
            case KEY_8:
            case KEY_9:
                return static_cast<wui::KeyboardCodesPOSIX>(
                    static_cast<int>(wui::KeyboardCodesPOSIX::VKEY_0) +
                    (keycode - static_cast<int>(KEY_0)));
            case KEY_SPACE:
                return wui::KeyboardCodesPOSIX::VKEY_SPACE;

            default:
                printf("Unknown printable key code: %d\n", keycode);
                break;
        }

        return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;
    }

    wui::KeyboardCodesPOSIX getWindows_key_code_special(const Ref<InputEventKey>& keyEvent)
    {
        // it is non printable
        // for these we don't need to handle all of them
        // most special characters are handled by the locale
        // so if someone presses shift + 1 it depends on the locale what will be printed
        // therefore we need not handle it

        int keycode = keyEvent->get_keycode();

        switch (keyEvent->get_keycode())
        {
            case KEY_ESCAPE:
                return wui::KeyboardCodesPOSIX::VKEY_ESCAPE;
            case KEY_TAB:
                return wui::KeyboardCodesPOSIX::VKEY_TAB;
            case KEY_BACKTAB:
                return wui::KeyboardCodesPOSIX::VKEY_BACKTAB;
            case KEY_BACKSPACE:
                return wui::KeyboardCodesPOSIX::VKEY_BACK;
            case KEY_SPACE:
                return wui::KeyboardCodesPOSIX::VKEY_SPACE;
            case KEY_ENTER:
            case KEY_KP_ENTER:
                return wui::KeyboardCodesPOSIX::VKEY_RETURN;
            case KEY_INSERT:
                return wui::KeyboardCodesPOSIX::VKEY_INSERT;
            case KEY_DELETE:
                return wui::KeyboardCodesPOSIX::VKEY_DELETE;
            case KEY_PAUSE:
                return wui::KeyboardCodesPOSIX::VKEY_PAUSE;
            case KEY_PRINT:
                return wui::KeyboardCodesPOSIX::VKEY_PRINT;
            case KEY_SYSREQ:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
            case KEY_CLEAR:
                return wui::KeyboardCodesPOSIX::VKEY_CLEAR;
            case KEY_HOME:
                return wui::KeyboardCodesPOSIX::VKEY_HOME;
            case KEY_END:
                return wui::KeyboardCodesPOSIX::VKEY_END;
            case KEY_LEFT:
                return wui::KeyboardCodesPOSIX::VKEY_LEFT;
            case KEY_UP:
                return wui::KeyboardCodesPOSIX::VKEY_UP;
            case KEY_RIGHT:
                return wui::KeyboardCodesPOSIX::VKEY_RIGHT;
            case KEY_DOWN:
                return wui::KeyboardCodesPOSIX::VKEY_DOWN;
            case KEY_PAGEUP:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
            case KEY_PAGEDOWN:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
            case KEY_SHIFT:
                return wui::KeyboardCodesPOSIX::VKEY_SHIFT;
            case KEY_CTRL:
                return wui::KeyboardCodesPOSIX::VKEY_CONTROL;
            case KEY_META:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
            case KEY_ALT:
                return wui::KeyboardCodesPOSIX::VKEY_ALTGR;
            case KEY_CAPSLOCK:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;
            case KEY_NUMLOCK:
                return wui::KeyboardCodesPOSIX::VKEY_NUMLOCK;
            case KEY_SCROLLLOCK:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;

            case KEY_F1:  // F1 - F12 are consecutive
            case KEY_F2:
            case KEY_F3:
            case KEY_F4:
            case KEY_F5:
            case KEY_F6:
            case KEY_F7:
            case KEY_F8:
            case KEY_F9:
            case KEY_F10:
            case KEY_F11:
            case KEY_F12:
                return static_cast<wui::KeyboardCodesPOSIX>(
                    static_cast<int>(wui::KeyboardCodesPOSIX::VKEY_F1) +
                    (keycode - static_cast<int>(KEY_F1)));

            default:
                printf("Unknown non printable key code: %d\n", keycode);
                break;
        }

        return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;
    }

    wui::KeyboardCodesPOSIX getWindows_key_code(const Ref<InputEventKey>& keyEvent)
    {
        if (keyEvent->get_keycode() & KEY_SPECIAL)
            return getWindows_key_code_special(keyEvent);
        return getWindows_key_code_printable(keyEvent);
    }
}