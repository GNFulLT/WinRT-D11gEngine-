#ifndef PLUGIN_ENTRYP_H
#define PLUGIN_ENTRYP_H

#define GNFPLUGIN_ENTRYP_NAME _GNFPluginEntryPoint 

#if defined(__CYGWIN32__)
#define GNFPLUGIN_INTERFACE_API __stdcall
#define GNFPLUGIN_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#define GNFPLUGIN_INTERFACE_API __stdcall
#define GNFPLUGIN_INTERFACE_EXPORT __declspec(dllexport)
#elif defined(__MACH__) || defined(__ANDROID__) || defined(__linux__) || defined(LUMIN)
#define GNFPLUGIN_INTERFACE_API
#define GNFPLUGIN_INTERFACE_EXPORT __attribute__ ((visibility ("default")))
#else
#define GNFPLUGIN_INTERFACE_API
#define GNFPLUGIN_INTERFACE_EXPORT
#endif

#define GNFPLUGIN_POINT GNFPLUGIN_INTERFACE_EXPORT GNFPLUGIN_INTERFACE_API


#endif 