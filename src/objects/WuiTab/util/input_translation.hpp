#include <godot_cpp/classes/input_event_key.hpp>

#include "input/webUiInput.hpp"
#include "input/webUiInputKeyboardCodes.hpp"

namespace godot
{
    // mouse
    wui::WuiMouseEventCoordinates Vec2ToWuiMouseEventCoordinates(const Vector2& point);
    wui::wui_mouse_button_type_t MouseButtonToWuiMouseButtonType(MouseButton mouseButton);
    wui::WuiKeyEvent KeyEventToWuiKeyEvent(const Ref<InputEventKey>& keyEvent);

    // keyboard

    extern bool is_capslock_pressed;
    uint32_t get_wui_key_modifier_mask(const Ref<InputEventKey>& keyEvent);
    wui::KeyboardCodesPOSIX getWindows_key_code(const Ref<InputEventKey>& keyEvent);
    wui::WuiMouseEventCoordinates Vec2ToWuiMouseEventCoordinates(const Vector2& point);
}