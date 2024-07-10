

#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/os.hpp>

#include "input/webUiInputKeyboardCodes.hpp"
#include "input/webUiKeyEvent.hpp"
#include "input/webUiMouseEvent.hpp"
#include "input_translation.hpp"

namespace godot
{

    bool is_capslock_pressed = false;

    uint32_t get_wui_key_modifier_mask(const Ref<InputEventKey>& keyEvent)
    {
        uint32_t wuiModifiers = 0;

        if (keyEvent->is_shift_pressed() || is_capslock_pressed)
            wuiModifiers |= wui::wui_modifier_flags_t::EVENTFLAG_SHIFT_DOWN;
        if (keyEvent->is_ctrl_pressed())
            wuiModifiers |= wui::wui_modifier_flags_t::EVENTFLAG_CONTROL_DOWN;
        if (keyEvent->is_alt_pressed())
            wuiModifiers |= wui::wui_modifier_flags_t::EVENTFLAG_ALT_DOWN;

        return wuiModifiers;
    }

    wui::KeyboardCodesPOSIX getWindows_key_code_printable(const Ref<InputEventKey>& keyEvent)
    {
        int keycode = keyEvent->get_keycode();

        if (keycode >= KEY_0 && keycode <= KEY_9)
        {
            return static_cast<wui::KeyboardCodesPOSIX>(
                static_cast<int>(wui::KeyboardCodesPOSIX::VKEY_0) +
                (keycode - static_cast<int>(KEY_0)));
        }

        if (keycode >= KEY_A && keycode <= KEY_Z)
        {
            return static_cast<wui::KeyboardCodesPOSIX>(
                static_cast<int>(wui::KeyboardCodesPOSIX::VKEY_A) +
                (keycode - static_cast<int>(KEY_A)));
        }

        return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;
    }

    wui::KeyboardCodesPOSIX getWindows_key_code(const Ref<InputEventKey>& keyEvent)
    {
        int keycode = keyEvent->get_keycode();

        if ((keycode & KEY_SPECIAL) == false)
        {
            printf("printable key\n");
            return getWindows_key_code_printable(keyEvent);
        }

        // it is non printable
        // for these we don't need to handle all of them
        // most special characters are handled by the locale
        // so if someone presses shift + 1 it depends on the locale what will be printed
        // therefore we need not handle it

        switch (keyEvent->get_keycode())
        {
            case KEY_ESCAPE:
                return wui::KeyboardCodesPOSIX::VKEY_ESCAPE;
                break;
            case KEY_TAB:
                return wui::KeyboardCodesPOSIX::VKEY_TAB;
                break;
            case KEY_BACKTAB:
                return wui::KeyboardCodesPOSIX::VKEY_BACKTAB;
                break;
            case KEY_BACKSPACE:
                return wui::KeyboardCodesPOSIX::VKEY_BACK;
                break;
            case KEY_ENTER:
                return wui::KeyboardCodesPOSIX::VKEY_RETURN;
                break;
            case KEY_KP_ENTER:
                return wui::KeyboardCodesPOSIX::VKEY_RETURN;
                break;
            case KEY_INSERT:
                return wui::KeyboardCodesPOSIX::VKEY_INSERT;
                break;
            case KEY_DELETE:
                return wui::KeyboardCodesPOSIX::VKEY_DELETE;
                break;
            case KEY_PAUSE:
                return wui::KeyboardCodesPOSIX::VKEY_PAUSE;
                break;
            case KEY_PRINT:
                return wui::KeyboardCodesPOSIX::VKEY_PRINT;
                break;
            case KEY_SYSREQ:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
                break;
            case KEY_CLEAR:
                return wui::KeyboardCodesPOSIX::VKEY_CLEAR;
                break;
            case KEY_HOME:
                return wui::KeyboardCodesPOSIX::VKEY_HOME;
                break;
            case KEY_END:
                return wui::KeyboardCodesPOSIX::VKEY_END;
                break;
            case KEY_LEFT:
                return wui::KeyboardCodesPOSIX::VKEY_LEFT;
                break;
            case KEY_UP:
                return wui::KeyboardCodesPOSIX::VKEY_UP;
                break;
            case KEY_RIGHT:
                return wui::KeyboardCodesPOSIX::VKEY_RIGHT;
                break;
            case KEY_DOWN:
                return wui::KeyboardCodesPOSIX::VKEY_DOWN;
                break;
            case KEY_PAGEUP:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
                break;
            case KEY_PAGEDOWN:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
                break;
            case KEY_SHIFT:
                return wui::KeyboardCodesPOSIX::VKEY_SHIFT;
                break;
            case KEY_CTRL:
                return wui::KeyboardCodesPOSIX::VKEY_CONTROL;
                break;
            case KEY_META:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;  // ??
                break;
            case KEY_ALT:
                return wui::KeyboardCodesPOSIX::VKEY_ALTGR;
                break;
            case KEY_CAPSLOCK:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;
                break;
            case KEY_NUMLOCK:
                return wui::KeyboardCodesPOSIX::VKEY_NUMLOCK;
                break;
            case KEY_SCROLLLOCK:
                return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;
                break;

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
                break;
        }

        return wui::KeyboardCodesPOSIX::VKEY_UNKNOWN;
    }

    // MOUSE

    wui::WuiMouseEventCoordinates Vec2ToWuiMouseEventCoordinates(const Vector2& point)
    {
        return { x : static_cast<int>(roundf(point.x)), y : static_cast<int>(roundf(point.y)) };
    }

    wui::wui_mouse_button_type_t MouseButtonToWuiMouseButtonType(MouseButton mouseButton)
    {
        switch (mouseButton)
        {
            case MouseButton::MOUSE_BUTTON_LEFT:
                return wui::wui_mouse_button_type_t::MBT_LEFT;
            case MouseButton::MOUSE_BUTTON_MIDDLE:
                return wui::wui_mouse_button_type_t::MBT_MIDDLE;
            case MouseButton::MOUSE_BUTTON_WHEEL_UP:
            case MouseButton::MOUSE_BUTTON_WHEEL_DOWN:
            case MouseButton::MOUSE_BUTTON_WHEEL_LEFT:
            case MouseButton::MOUSE_BUTTON_WHEEL_RIGHT:
                return wui::wui_mouse_button_type_t::MBT_WHEEL;
            case MouseButton::MOUSE_BUTTON_RIGHT:
                return wui::wui_mouse_button_type_t::MBT_RIGHT;
            default:
                return wui::wui_mouse_button_type_t::MBT_NONE;
        }
    }

    wui::WuiKeyEvent KeyEventToWuiKeyEvent(const Ref<InputEventKey>& keyEvent)
    {
        wui::WuiKeyEvent ret = {
            .type = wui::wui_key_event_type_t::KEYEVENT_RAWKEYDOWN,  // overwritten
            .modifiers = get_wui_key_modifier_mask(keyEvent),
            .windows_key_code = getWindows_key_code(keyEvent),
            .native_key_code = static_cast<int>(keyEvent->get_unicode()),
            .is_system_key = false,
            .character = static_cast<char16_t>(keyEvent->get_unicode()),
            .unmodified_character = static_cast<char16_t>(keyEvent->get_unicode()),
            .focus_on_editable_field = false,
        };

        /*    printf("Windows key code: %d\n Modifier mask 0x%X, unicode: 0x%X\n",
                   static_cast<int>(ret.windows_key_code), ret.modifiers, keyEvent->get_unicode());
    */
        return ret;
    }
}