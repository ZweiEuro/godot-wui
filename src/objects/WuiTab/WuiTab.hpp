#pragma once

#include <classes/WebUiTabWrapper.hpp>

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/input_event.hpp>

namespace godot
{
    class WuiTab : public Control
    {
        GDCLASS(WuiTab, Control)

    public:
        WuiTab();
        ~WuiTab();

    private:
        wui::TabConfig TabConfig;

        wui::WebUiTabWrapper* wuiTab = nullptr;

        godot::Ref<godot::ImageTexture> texture = nullptr;

    protected:
        static void _bind_methods();

    public:
        void _draw() override;
        void _ready() override;
        bool _has_point(const Vector2& point) const override;
        void resize_handler();
        void _gui_input(const Ref<InputEvent>& event) override;

        void send_event(const String& eventName, const Dictionary& eventPayload);

        // Bound methods
    protected:
        String get_initialPath() const;
        void set_initialPath(const String& path);
    };

}
