# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("VulkanGameEngine")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# For checking user downloaded deps
if(WIN32)
	set(PYTHON_EXECUTABLE "python")
else()
	set(PYTHON_EXECUTABLE "python3")
endif()

if((NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/.bootstrap.json") OR ("${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt" IS_NEWER_THAN "${CMAKE_CURRENT_SOURCE_DIR}/deps/.bootstrap.json"))
	execute_process(
		COMMAND ${PYTHON_EXECUTABLE} "bootstrap.py"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		RESULT_VARIABLE bootstrap_result
	)
	if (NOT (${bootstrap_result} EQUAL 0))
		message(FATAL_ERROR "Cannot run bootstrap.py automatically")
	endif()
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/deps/src/boost/build" OR "${CMAKE_CURRENT_SOURCE_DIR}/deps/src/boost/build/lib" OR "${CMAKE_CURRENT_SOURCE_DIR}/deps/src/boost/build/include")
	message(FATAL_ERROR "Please build boost library to ./build dir of boost dir firstly")
endif()

include_directories(./VulkanGameEngine)
include_directories(./deps/src/glfw/include)
include_directories(./deps/src/boost/build/include/boost-1_82)
# Include sub-projects.

add_subdirectory ("VulkanGameEngine")
# For Libs including dependencies main directory
add_subdirectory(./deps/src/glfw)


link_directories(./deps/src/boost/build/lib)

if(WIN32)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# For Include and use headers
message(STATUS "Include directories: ${CMAKE_INCLUDE_PATH}")
