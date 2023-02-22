#ifndef PLUGIN_COMMON_TYPES_H
#define PLUGIN_COMMON_TYPES_H

#include <stdint.h>

#include "plugin_native_event_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	// Helpers 
	typedef uint_fast8_t GNFPluginReturnCode;
	//X TODO : Change this
	typedef GNFPluginReturnCode GNFPluginBool;

	typedef void* GNFPluginPackagedArgs;
	


	typedef  GNFPluginBool(*GNFPluginNativeEvent_OpenPackage)(GNFPluginPackagedArgs, GNFPluginNativeEventConstructorArgs**);


	typedef struct GNFPluginOpenPackage
	{
		GNFPluginNativeEvent_OpenPackage fNativeEventOpener;
	}GNFPluginOpenPackage;



	typedef struct UVec2
	{
		unsigned int x;
		unsigned int y;
	}UVec2;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // PLUGIN_COMMON_TYPES_H