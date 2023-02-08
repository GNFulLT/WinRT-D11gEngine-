#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4099) // type name first seen using 'class' now seen using 'struct'
#	pragma warning(disable:4244) // 'argument': conversion from 'float' to 'int', possible loss of data
#	pragma warning(disable:4267) // conversion from 'size_t' to 'int', possible loss of data
#	pragma warning(disable:4305) // 'argument': truncation from 'double' to 'float'
#endif // _MSC_VER

#if defined(__clang__)
#	pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#endif // __clang__

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <imgui/imgui.cpp>
#include <imgui/imgui_demo.cpp>
#include <imgui/imgui_draw.cpp>
#include <imgui/imgui_tables.cpp>
#include <imgui/imgui_widgets.cpp>

//! For DirectX11 and Win32
#include <imgui/imgui_impl_dx11.cpp>
#include <imgui/imgui_impl_win32.cpp>
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif // _MSC_VER

