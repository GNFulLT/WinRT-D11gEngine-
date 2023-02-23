# CMakeList.txt : CMake project for VulkanGameEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (VulkanGameEngine "VulkanGameEngine.cpp" "VulkanGameEngine.h" "Core/engine.h" "core/string/locales.h" "core/typedefs.h" "core/string/unicode_char_range.h" "core/string/unicode_char_utils.h" "core/string/unicode_char_utils.cpp" "core/string/string_types.h" "core/templates/safe_num.h" "core/object/object.h" "core/object/object_id.h" "servers/localization/localization_server.h" "core/string/string_utils.h" "core/string/string_utils.cpp" "exception/file_not_found_ex.h" "servers/localization/localization_server.cpp" "servers/window_server.h" "servers/configuration_server.h" "config/config.h" "core/scope.h" "servers/configuration_server.cpp" "config/config_prop.h" "config/config_prop_base.h" "window/window.h" "window/viewport.h"  "window/window_id.h"  "core/templates/node.h" "platform/GLFW/window_server_glfw.h" "servers/creation_server.h" "servers/creation_server.cpp" "config/config_utils.h" "config/config_utils.cpp" "platform/GLFW/window_server_glfw.cpp" "window/viewport.cpp" "servers/window_server.cpp" "core/version.h" "plugin/plugin.h" "plugin/pinclude/basic_shared_types.h" "core/io/logger.h" "core/io/logger.cpp" "servers/logger_server.h" "servers/logger_server.cpp" "servers/event_bus_server.h" "servers/event_bus_server.cpp" "core/vec2.h" "core/folders.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VulkanGameEngine PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(VulkanGameEngine glfw)
target_link_libraries(VulkanGameEngine ${BOOST_CORRECT_LIBDIR})
target_link_libraries(VulkanGameEngine ${Boost_LIBRARIES})


# TODO: Add tests and install targets if needed.
