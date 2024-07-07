#include <string_view>

#include <webUi.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "settings/settings.hpp"

#define PREFIX "wui/"

namespace godot
{

    void add_custom_project_setting(const String name, const Variant& default_value,
                                    const PropertyHint hint = PropertyHint::PROPERTY_HINT_NONE,
                                    const String hint_string = "")
    {
        std::string_view setting_prefix{ PREFIX };

        const godot::String fullString{ setting_prefix.data() + name };

        auto ProjectSettings = ProjectSettings::get_singleton();
        if (ProjectSettings == NULL)
        {
            UtilityFunctions::print("EditorSettings is null\n");
            return;
        }

        if (!ProjectSettings->has_setting(fullString))
            ProjectSettings->set_setting(fullString, default_value);

        ProjectSettings->set_initial_value(fullString, default_value);

        auto setting_info = Dictionary();

        setting_info[Variant("name")] = fullString;
        setting_info[Variant("type")] = default_value.get_type();
        setting_info[Variant("description")] = "descr";
        setting_info[Variant("tip")] = "tooltip";

        setting_info[Variant("hint")] = hint;
        setting_info[Variant("hint_string")] = hint_string;

        ProjectSettings->add_property_info(setting_info);
        ProjectSettings->set_as_basic(fullString, true);
        ProjectSettings->set_restart_if_changed(fullString, true);
    }

    // to get if you are in an editor: is_editor_hint

    void initialize_library_settings()
    {
        wui::WuiVersion version = {};
        WUI_ERROR_CHECK(wui::WuiGetVersion(version));
        UtilityFunctions::print("WUI Version ", version.commit.c_str());

        // Add our settings

        // in editor
        // Dev tools mode:
        add_custom_project_setting("Editor/devtoolsMode", 0, PropertyHint::PROPERTY_HINT_ENUM,
                                   "DISABLED:0,ENABLED:1");

        // debugger port
        add_custom_project_setting("Editor/remoteDebuggingPort", 8001,
                                   PropertyHint::PROPERTY_HINT_RANGE, "1025,65535,1");

        // in project
        // Dev tools mode:
        add_custom_project_setting("Project/devtoolsMode", 0, PropertyHint::PROPERTY_HINT_ENUM,
                                   "DISABLED:0,ENABLED:1");

        // debugger port
        add_custom_project_setting("Project/remoteDebuggingPort", 8002,
                                   PropertyHint::PROPERTY_HINT_RANGE, "1025,65535,1");

        // locale string
        add_custom_project_setting("General/locale", "en_US", PropertyHint::PROPERTY_HINT_LOCALE_ID);

        // cache directory
        add_custom_project_setting("General/cacheDirectoryPath", "", PropertyHint::PROPERTY_HINT_DIR);

        // resource path
        add_custom_project_setting("General/wui_resources path", "res://bin/wui_resources",
                                   PropertyHint::PROPERTY_HINT_DIR);
    }

    Variant get_library_setting(const String name)
    {
        std::string_view setting_prefix{ PREFIX };
        auto ProjectSettings = ProjectSettings::get_singleton();
        if (ProjectSettings == NULL)
        {
            UtilityFunctions::print("EditorSettings is null\n");
            assert(false);  // This should never happen
        }

        return ProjectSettings->get_setting(setting_prefix.data() + name);
    }

    wui::WuiConfig get_wui_config()
    {
        wui::WuiConfig config = {};

        auto ProjectSettings = ProjectSettings::get_singleton();
        if (ProjectSettings == NULL)
        {
            UtilityFunctions::print("EditorSettings is null\n");
            return config;
        }

        if (Engine::get_singleton()->is_editor_hint())
        {
            config.devtoolsMode = static_cast<wui::WuiConfig::DevToolsMode>(
                get_library_setting("Editor/devtoolsMode").operator int());

            config.remoteDebuggingPort =
                get_library_setting("Editor/remoteDebuggingPort").operator int();
        }
        else
        {
            config.devtoolsMode = static_cast<wui::WuiConfig::DevToolsMode>(
                get_library_setting("Project/devtoolsMode").operator int());

            config.remoteDebuggingPort =
                get_library_setting("Project/remoteDebuggingPort").operator int();
        }

        config.locale = get_library_setting("General/locale").operator String().utf8().get_data();

        config.cacheDirectoryPath =
            get_library_setting("General/cacheDirectoryPath").operator String().utf8().get_data();

        config.wuiResourcesPath =
            get_library_setting("General/wui_resources path").operator String().utf8().get_data();

        if (config.wuiResourcesPath.starts_with("res://"))
        {
            config.wuiResourcesPath = ProjectSettings::get_singleton()
                                          ->globalize_path(config.wuiResourcesPath.c_str())
                                          .utf8()
                                          .get_data();
        }

        return config;
    }
};
