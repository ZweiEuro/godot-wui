#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/os.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "objects/WuiTab/WuiTab.hpp"
#include "register_types.hpp"
#include "settings/settings.hpp"

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level)
{
    switch (p_level)
    {
        default:
        case MODULE_INITIALIZATION_LEVEL_CORE:
        case MODULE_INITIALIZATION_LEVEL_SERVERS:
        case MODULE_INITIALIZATION_LEVEL_MAX:
        case MODULE_INITIALIZATION_LEVEL_EDITOR:
            break;

        case MODULE_INITIALIZATION_LEVEL_SCENE:
            UtilityFunctions::print("Init Wui library\n");
            // Add our settings
            initialize_library_settings();

            auto config = get_wui_config();

            if (config.wuiResourcesPath.starts_with("res://"))
            {
                config.wuiResourcesPath = ProjectSettings::get_singleton()
                                              ->globalize_path(config.wuiResourcesPath.c_str())
                                              .utf8()
                                              .get_data();
            }

            try
            {
                WUI_ERROR_CHECK(wui::WuiInit(config));
            }
            catch (const std::exception& e)
            {
                UtilityFunctions::print("Error initializing Wui library\n", e.what());
                return;
            }

            ClassDB::register_class<WuiTab>();
            // ClassDB::register_class<Test>();
    }
}

void uninitialize_example_module(ModuleInitializationLevel p_level)
{
    wui::WuiDeInit();
}

extern "C"
{
    // Initialization.
    GDExtensionBool GDE_EXPORT example_library_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization)
    {
        GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_example_module);
        init_obj.register_terminator(uninitialize_example_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
