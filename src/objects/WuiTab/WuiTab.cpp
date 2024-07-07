#include <godot_cpp/classes/font.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/json.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "WuiTab.hpp"

namespace godot
{

    WuiTab::WuiTab()
    {
        godot::UtilityFunctions::print("created offscreen tab");

        // Initialize any variables here.
    }

    WuiTab::~WuiTab()
    {
        // Add your cleanup here.
        if (wuiTab != nullptr)
        {
            delete wuiTab;
            wuiTab = nullptr;
        }
    }

    void WuiTab::_ready()
    {
        godot::UtilityFunctions::print("Ready WUI tab\n", get_size(), get_initialPath());

        // INPUT

        set_focus_mode(FOCUS_ALL);  // accept keyboard input

        // RENDERING

        // get sizes
        const auto size = get_size();
        const auto width = static_cast<size_t>(size.x);
        const auto height = static_cast<size_t>(size.y);

        // Create the offscreen tab
        TabConfig.initialDimensions = { height : height, width : width };

        // SIGNALING
        TabConfig.initialPath = get_initialPath().utf8().get_data();

        TabConfig.onLoadEndCallback = [this](const wui::wui_tab_id_t tab_id, std::string currentUrl,
                                             int httpCode, const wui::TabConfig::TabLoadError& error) {
            // godot::UtilityFunctions::print("OnLoadEndCallback: ", currentUrl.c_str(), " httpCode
            // ", httpCode, " errorCode ", error.errorCode, " error Message ",
            // error.errorMessage.c_str());

            Dictionary errorDict;
            errorDict["errorCode"] = error.errorCode;
            errorDict["errorMessage"] = error.errorMessage.c_str();
            call_deferred("emit_signal", "load_end", currentUrl.c_str(), httpCode, errorDict);
        };

        TabConfig.onPaintCallback = [this](const wui::wui_tab_id_t tab_id,
                                           const wui::WuiTabPixelBuffer& buffer) {
            call_deferred("queue_redraw");
        };

        auto eventReceiver = [this](const wui::wui_tab_id_t tab_id, const std::string& eventName,
                                    const cJSON* eventPayload, cJSON* successRetObj,
                                    std::string& exception) {
            // godot::UtilityFunctions::print("Event received: ", eventName.c_str());

            // convert the event payload to a dictionary
            auto json_str = cJSON_PrintUnformatted(eventPayload);

            auto result = JSON::parse_string(json_str);

            if (result.get_type() != Variant::Type::DICTIONARY)
            {
                godot::UtilityFunctions::print("Error parsing JSON: ", json_str);
                std::free(json_str);
                return -1;
            }

            std::free(json_str);
            call_deferred("emit_signal", "event_received", eventName.c_str(), result);
            return 0;
        };

        try
        {
            wuiTab = new wui::WebUiTabWrapper(TabConfig);
            wuiTab->registerEventListener(eventReceiver);
        }
        catch (const std::exception& e)
        {
            wuiTab = nullptr;
            godot::UtilityFunctions::print(
                "Error creating offscreen tab: your settings are invalid", e.what());
            return;
        }

        this->connect("resized", Callable(this, "resize_handler"));
        // this->connect("validate_property", Callable(this, "validate_property_handler"));
    }

    void WuiTab::_draw()
    {
        static wui::WuiTabPixelBuffer buffer;

        if (wuiTab == nullptr)
        {
            auto font = ThemeDB::get_singleton()->get_fallback_font();
            draw_string(font, Vector2(0, 0), "WUI init Error, check console",
                        HORIZONTAL_ALIGNMENT_CENTER, 500, 22);

            return;
        }

        // check if the size is the same that we should have

        const auto size = get_size();
        const auto width = static_cast<int32_t>(size.x);
        const auto height = static_cast<int32_t>(size.y);

        const auto ret = wuiTab->copyPixelBuffer(buffer);
        if (ret != wui::WUI_OK)
        {
            godot::UtilityFunctions::print("Error copying pixel buffer from wui: ", ret);
            queue_redraw();
            return;
        }

        if (width != (int32_t)buffer.width || height != (int32_t)buffer.height)
        {
            godot::UtilityFunctions::print("Buffer size mismatch: ", width, " by ", height, " vs ",
                                           buffer.width, " by ", buffer.height);
            queue_redraw();
            return;
        }

        // check that the image has the same dims

        if (buffer.byteSize > 0 && buffer.currentPixelBuffer != nullptr)
        {
            auto byte_array = PackedByteArray();
            byte_array.resize(buffer.byteSize);
            auto raw_ptr = byte_array.ptrw();
            memcpy(raw_ptr, buffer.currentPixelBuffer, buffer.byteSize);
            // swap red and blue channels so we get RGBA from BGRA
            for (size_t i = 0; i < buffer.byteSize; i += 4)
                std::swap(raw_ptr[i], raw_ptr[i + 2]);

            auto image = Image::create_from_data(width, height, false, Image::FORMAT_RGBA8,
                                                 byte_array);

            if (texture.is_null() || texture->get_width() != width || texture->get_height() != height)
                texture = ImageTexture::create_from_image(image);
            else
                texture->update(image);

            draw_texture(texture, Vector2(0, 0));
        }
    }

}