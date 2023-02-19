# CMakeList.txt : CMake project for VulkanGameEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (VulkanGameEngine "VulkanGameEngine.cpp" "VulkanGameEngine.h" "Core/engine.h" "core/string/locales.h" "core/typedefs.h" "core/string/unicode_char_range.h" "core/string/unicode_char_utils.h" "core/string/unicode_char_utils.cpp" "core/string/string_types.h" "core/templates/safe_num.h" "core/object/object.h" "core/object/object_id.h" "servers/localization/localization_server.h" "core/string/string_utils.h" "core/string/string_utils.cpp" "exception/file_not_found_ex.h" "servers/localization/localization_server.cpp" "servers/window_server.h" "servers/configuration_server.h" "config/config.h" "core/scope.h" "servers/configuration_server.cpp" "config/config_prop.h" "config/config_prop_base.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VulkanGameEngine PROPERTY CXX_STANDARD 20)
endif()

if(Boost_FOUND)   
    include_directories(${Boost_INCLUDE_DIRS})
    add_executable(VulkanGameEngine VulkanGameEngine.cc)   
    target_link_libraries(VulkanGameEngine ${Boost_LIBRARIES})
endif()

# TODO: Add tests and install targets if needed.
