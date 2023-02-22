#ifndef PLUGIN_NATIVE_EVENT_TYPES_H
#define PLUGIN_NATIVE_EVENT_TYPES_H


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "plugin_common_types.h"
#include "plugin_events.h"
#define GNFPLUGIN_SERVERLAYER_CHANGESCOPE_POINT_NAME _GNFPluginServerLayerChangeScopePoint


#ifdef __cplusplus
extern "C" {
#endif


	typedef struct GNFPluginNativeEventConstructorArgs
	{
		GNFPLUGIN_EVENT_FLAG flag;
	} GNFPluginNativeEventConstructorArgs;


	

 
#ifdef __cplusplus
}
#endif
#endif //PLUGIN_NATIVE_EVENT_TYPES_H