#include "plugin_native.h"

GNFPLUGIN_TYPE GNFPluginNative::get_plugin_type()
{
	return g_plugin_type;
}

#include "native_include/plugin_excepts.h"

GNFPluginBool error_native_event_opener(GNFPluginPackagedArgs packaged, GNFPluginNativeEventConstructorArgs* converter)
{
	return (GNFPluginBool)GNFPLUGIN_ERROR_CODE_OPEN_PACKAGED_TYPE_NOT_COMPATIBLE_BIND_TYPE;
}
