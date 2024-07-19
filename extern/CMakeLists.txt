set(I_AM_DEV false)

if(I_AM_DEV)
    execute_process(
        COMMAND git submodule update --init wui
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND_ERROR_IS_FATAL ANY
    )

    set(WUI_OUT_DIR ${GDEXTENSION_LIB_PATH} CACHE INTERNAL "")
    set(WUI_SKIP_HELPER_CMAKE true CACHE INTERNAL "")
    set(WUI_SKIP_INCLUDE_FILES true CACHE INTERNAL "")

    add_subdirectory(wui)
    add_dependencies(${PROJECT_NAME} wui)

    # Manually link the library and its deps
    target_link_libraries(${PROJECT_NAME} PRIVATE wui)
    use_cjson(${PROJECT_NAME})

else()
    include(FetchContent) # cmake utility

    set(FETCHCONTENT_QUIET off) # Download progress should be shown

    # reduce network traffic as much as possible
    # this means even if you delete your entire build directory you will not trigger a re-download the library
    get_filename_component(external_deps "../external_deps"
        REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")

    set(FETCHCONTENT_BASE_DIR ${external_deps})

    FetchContent_Declare( # fetch wui from server
        wui

        # get release tag info and sha from:
        # https://gitlab.zweieuro.at/wui/web-user-interface-library/-/releases

        # To get the most recent release
        # URL https://gitlab.zweieuro.at/wui/core/web-user-interface-library/-/releases/permalink/latest/downloads/ReleaseLinux.tar.gz
        URL http://localhost:8080/build/Release/Release_0.0.31_linux64.tar.gz

        # To get a specific release Version M.m.p (Major, minor, patch)
        # URL https://gitlab.zweieuro.at/wui/web-user-interface-library/-/releases/M.m.p/downloads/ReleaseLinux.tar.gz
        # e.g.
        # URL https://gitlab.zweieuro.at/wui/web-user-interface-library/-/releases/0.0.29/downloads/ReleaseLinux.tar.gz

        # When starting your project its okay to use the "most recent" release. Once you have started development
        # you should use a specific release. This way you can control when you update your dependencies.
        # The url hash enforce that the file is not tampered with and is the one of your desired release.
        # the hash can be found for each release on gitlab.
        # This is the hash for 0.0.29
        # URL_HASH SHA1=607db9e7b5869af08bf7c46be0d0b350f5a2aaf6

        # for security its smart to put the hash here.
        # Its shown for each release on gitlab
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    FetchContent_MakeAvailable(wui) # unpack/load it

    wui_add_target(${PROJECT_NAME})
endif()

# Very important in order to find all library dependencies.
# If you want to change where the libraries are at
# then you need to change the rpath as well
# do not forget to adjust the wui_resources path when initializing the wui!
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/wui_resources")
set_target_properties(${PROJECT_NAME} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)