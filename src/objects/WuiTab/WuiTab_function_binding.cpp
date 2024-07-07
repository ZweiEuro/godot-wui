#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "WuiTab.hpp"

namespace godot
{
    // Method binding
    void WuiTab::_bind_methods()
    {
        // sizing
        ClassDB::bind_method(D_METHOD("resize_handler"), &WuiTab::resize_handler);

        // inspector props
        // initialPath
        ClassDB::bind_method(D_METHOD("get_initialPath"), &WuiTab::get_initialPath);
        ClassDB::bind_method(D_METHOD("set_initialPath", "path"), &WuiTab::set_initialPath);
        ADD_PROPERTY(
            PropertyInfo(
                Variant::STRING, "initialPath", PropertyHint::PROPERTY_HINT_NONE,
                "The initial path to load in the offscreen tab. Typically a URL or file path."),
            "set_initialPath", "get_initialPath");

        // initialDimensions come from the parent class

        // onLoadEndCallback
        ADD_SIGNAL(MethodInfo("load_end", PropertyInfo(Variant::STRING, "currentUrl"),
                              PropertyInfo(Variant::INT, "httpCode"),
                              PropertyInfo(Variant::DICTIONARY, "error")));

        // onEventReceived
        ADD_SIGNAL(MethodInfo("event_received", PropertyInfo(Variant::STRING, "eventName"),
                              PropertyInfo(Variant::DICTIONARY, "eventPayload")));

        // Sending events
        ClassDB::bind_method(D_METHOD("send_event", "eventName", "eventPayload"),
                             &WuiTab::send_event);

        // remove all unneeded props
        // UtilityFunctions::print("wui props", ClassDB::class_get_property_list("WuiTab"));
    }

    ///
    /// Connected handlers
    ///

    // send event handler
    void WuiTab::send_event(const String& eventName, const Dictionary& eventPayload)
    {
        if (wuiTab == nullptr)
            godot::UtilityFunctions::print("Error sending event: no tab");

        auto const jstr = JSON::stringify(eventPayload);

        auto cjson = cJSON_Parse(jstr.utf8().get_data());
        if (cjson == nullptr)
        {
            godot::UtilityFunctions::print("Error parsing json");
            return;
        }

        const auto ret = wuiTab->sendEvent(eventName.utf8().get_data(), cjson);

        if (ret != wui::WUI_OK)
            godot::UtilityFunctions::print("Error sending event: ", ret);

        cJSON_Delete(cjson);
    }

    // resize handler

    void WuiTab::resize_handler()
    {
        static size_t number_of_times_resized = 0;
        // UtilityFunctions::print("Resized\n", get_size(), number_of_times_resized);
        number_of_times_resized++;
        // ignore every second resize event
        // https://github.com/godotengine/godot/issues/92849

        if (number_of_times_resized % 2 == 0)
            return;

        const auto size = get_size();

        if (wuiTab != nullptr)
        {
            const auto ret = wuiTab->resize(size.x, size.y);

            if (ret != wui::WUI_OK)
                godot::UtilityFunctions::print("Error resizing tab: ", ret);

            queue_redraw();
        }
    }

    // Bound Setter / Getters

    String WuiTab::get_initialPath() const
    {
        return String(TabConfig.initialPath.c_str());
    }

    void WuiTab::set_initialPath(const String& path)
    {
        TabConfig.initialPath = path.utf8().get_data();

        if (wuiTab != nullptr)
        {
            const auto ret = wuiTab->loadUrl(TabConfig.initialPath);

            if (ret != wui::WUI_OK)
                godot::UtilityFunctions::print("Error loading initial path: ", ret);
        }
    }
}
