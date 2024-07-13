
#include <godot_cpp/classes/display_server.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "WuiTab.hpp"
#include "inttypes.h"
#include "util/input_translation.hpp"

namespace godot
{

    bool WuiTab::_has_point(const Vector2& point) const
    {
        if (wuiTab == nullptr)
        {
            godot::UtilityFunctions::print("no tab, can't check point");
            return false;
        }

        const auto ret = wuiTab->coordinateNotEmpty(Vec2ToWuiMouseEventCoordinates(point));
        return ret;
    }

    void WuiTab::_gui_input(const Ref<InputEvent>& event)
    {
        // TODO: mouse enter and mouse leave events could be handled

        // apparently the mouse events are relative to the origin of the tab itself,
        // thankfully this means we do not need to handle anything else in terms of translation

        auto mouseMotion = Object::cast_to<InputEventMouseMotion>(event.ptr());
        if (mouseMotion != nullptr)
        {
            wuiTab->sendMouseMoveEvent(Vec2ToWuiMouseEventCoordinates(mouseMotion->get_position()));
            accept_event();
            return;
        }

        auto mouseButton = Object::cast_to<InputEventMouseButton>(event.ptr());
        if (mouseButton != nullptr)
        {
            const auto wui_mouse_button_type = MouseButtonToWuiMouseButtonType(
                mouseButton->get_button_index());

            switch (wui_mouse_button_type)
            {
                case wui::wui_mouse_button_type_t::MBT_RIGHT:
                case wui::wui_mouse_button_type_t::MBT_LEFT:
                case wui::wui_mouse_button_type_t::MBT_MIDDLE:
                {
                    int clicks = 1;
                    if (mouseButton->is_double_click())
                        clicks = 2;

                    wuiTab->sendMouseClickEvent(
                        Vec2ToWuiMouseEventCoordinates(mouseButton->get_position()),
                        wui_mouse_button_type, mouseButton->is_released(), clicks);
                    break;
                }
                case wui::wui_mouse_button_type_t::MBT_WHEEL:

                    // Note: this is not a good solution, for for now its good enough
                    // TODO: Check if shift is being held and then change it to a X scroll
                    // get some reliable way to get how far to scroll ?

                    switch (mouseButton->get_button_index())
                    {
                        case MouseButton::MOUSE_BUTTON_WHEEL_UP:
                            wuiTab->sendMouseWheelEvent(
                                Vec2ToWuiMouseEventCoordinates(mouseButton->get_position()), 0, 10);
                            break;
                        case MouseButton::MOUSE_BUTTON_WHEEL_DOWN:
                            wuiTab->sendMouseWheelEvent(
                                Vec2ToWuiMouseEventCoordinates(mouseButton->get_position()), 0, -10);
                            break;
                        case MouseButton::MOUSE_BUTTON_WHEEL_LEFT:
                            wuiTab->sendMouseWheelEvent(
                                Vec2ToWuiMouseEventCoordinates(mouseButton->get_position()), -10, 0);
                            break;
                        case MouseButton::MOUSE_BUTTON_WHEEL_RIGHT:
                            wuiTab->sendMouseWheelEvent(
                                Vec2ToWuiMouseEventCoordinates(mouseButton->get_position()), 10, 0);
                            break;
                        default:
                            godot::UtilityFunctions::print("Unknown mouse wheel event");
                            break;
                    }

                    break;

                case wui::wui_mouse_button_type_t::MBT_NONE:
                    godot::UtilityFunctions::print("No mouse button associated with event");
                    break;
            }

            accept_event();
            return;
        }

        auto keyboardEvent = Object::cast_to<InputEventKey>(event.ptr());
        if (keyboardEvent != nullptr)
        {
#if DEBUG_MODE
            printf("=====================================\n");
#endif
            if (keyboardEvent->get_keycode() == KEY_CAPSLOCK)
                is_capslock_pressed = keyboardEvent->is_pressed();

            if (wuiTab->getCurrentTextInputMode() ==
                wui::wui_text_input_mode_t::WUI_TEXT_INPUT_MODE_NONE)
            {
                // skip keyboard events if we are not in text input mode
                UtilityFunctions::print("skipping keyboard event");
                return;
            }

            // TODO: clean this up, technically more complex situations are possible
            // but a cleaner parser is required to get to CEF events.

#if DEBUG_MODE
            UtilityFunctions::print("keyboard event", keyboardEvent);
#endif
            auto wuiEvent = KeyEventToWuiKeyEvent(keyboardEvent);

            if (keyboardEvent->is_pressed())
            {
#if DEBUG_MODE
                printf("DOWN\n");
#endif
                if (keyboardEvent->is_echo())
                {
                    wuiEvent.type = wui::wui_key_event_type_t::KEYEVENT_CHAR;
                    wuiTab->sendKeyEvent(wuiEvent);
                }
                else
                {
                    wuiEvent.type = wui::wui_key_event_type_t::KEYEVENT_RAWKEYDOWN;
                    wuiTab->sendKeyEvent(wuiEvent);

                    wuiEvent.type = wui::wui_key_event_type_t::KEYEVENT_KEYDOWN;
                    wuiTab->sendKeyEvent(wuiEvent);

                    wuiEvent.type = wui::wui_key_event_type_t::KEYEVENT_CHAR;
                    wuiTab->sendKeyEvent(wuiEvent);
                }
            }
            else
            {
#if DEBUG_MODE
                printf("UP\n");
#endif
                wuiEvent.type = wui::wui_key_event_type_t::KEYEVENT_KEYUP;
                wuiTab->sendKeyEvent(wuiEvent);
            }

            accept_event();
#if DEBUG_MODE
            printf("=====================================\n");
#endif
            return;
        }
    }
}