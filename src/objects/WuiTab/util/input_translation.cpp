

#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

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

    char16_t getUnicodeFromKeyEvent(const Ref<InputEventKey>& keyEvent)
    {
        const uint16_t ret = static_cast<char16_t>(keyEvent->get_unicode());

        if (ret != 0)
            return ret;

        if (keyEvent->get_keycode() == KEY_ENTER)
            return 0x0D;

        if (keyEvent->get_keycode() == KEY_TAB)
            return 0x09;

        // godot::UtilityFunctions::print("Unknown unicode for key code for event", keyEvent);

        return 0;
    }

    wui::WuiKeyEvent KeyEventToWuiKeyEvent(const Ref<InputEventKey>& keyEvent)
    {
        uint16_t unicode = getUnicodeFromKeyEvent(keyEvent);

        wui::WuiKeyEvent ret = {
            .type = wui::wui_key_event_type_t::KEYEVENT_RAWKEYDOWN,  // overwritten
            .modifiers = get_wui_key_modifier_mask(keyEvent),
            .windows_key_code = getWindows_key_code(keyEvent),
            .native_key_code = static_cast<int>(unicode),
            .is_system_key = false,
            .character = unicode,
            .unmodified_character = unicode,
            .focus_on_editable_field = false,
        };
#if DEBUG_MODE
        printf("Windows key code: %d\n Modifier mask 0x%X, unicode: 0x%X\n\n",
               static_cast<int>(ret.windows_key_code), ret.modifiers, unicode);
#endif
        return ret;
    }
}