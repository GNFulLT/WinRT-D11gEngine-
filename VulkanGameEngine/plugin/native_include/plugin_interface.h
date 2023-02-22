#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "plugin_server_types.h"
#include "plugin_common_types.h"
#include "plugin_defs.h"
#include "plugin_excepts.h"

#ifdef __cplusplus
extern "C" {
#endif
	typedef const char* pGNFPluginConstName;

	// PARAMS FOR CREATION
	typedef struct GNFPluginVersion
	{
		uint32_t version_major = 0;
		uint32_t version_minor = 0;
	} GNFPluginVersion;

	typedef enum
	{
		GNF_PLUGIN_BIND_TYPE_EVENT = 0,

	}GNF_PLUGIN_BIND_TYPE;

	typedef enum
	{
		GNF_PLUGIN_LAYER_LEVEL_SERVER_LAYER,
		GNF_PLUGIN_LAYER_LEVEL_CORE_LAYER,
	} GNF_PLUGIN_LAYER_LEVEL;


	typedef struct GNFPluginConstructorArgs
	{
		GNFPluginVersion version;
	} GNFPluginConstructorArgs;

	// PLUGIN (TYPEDEFS / CLASSES)
	typedef void* pGNFPluginObject;
	typedef pGNFPluginObject(*GNFPluginClassConstructor)(const GNFPluginConstructorArgs*, GNFPluginPackagedArgs, GNFPluginOpenPackage*);
	typedef void (*GNFPluginClassDestructor)(pGNFPluginObject);
	typedef GNFPluginBool(*GNFPluginRegisterFunc)(pGNFPluginConstName, GNF_PLUGIN_LAYER_LEVEL, GNFPluginVersion*, GNFPluginClassConstructor, GNFPluginClassDestructor, GNF_PLUGIN_BIND_TYPE);
	typedef GNFPluginBool(*GNFPluginExitFunc)();
	//typedef GNFPluginExitFunc(*GNFPluginInitFunc)();

	// GNF_PLUGIN_LAYER_LEVEL_SERVERS Specific methods. If your selected layer is servers, you can hook the methods in init
	//X TODO : Needed new interface that is specific to layers
	// Setters for changing system config. These are called generally in scope_change(). Scopes are -> scope_expose() -> scope_change() -> scope_init(). You can change
	// Initial values in Servre LAYER
	typedef void (*GNFPluginConfig_Int_Setter)(int);
	typedef void (*GNFPluginConfig_String_Setter)(char*, uint32_t);
	typedef void (*GNFPluginConfig_UVec2_Setter)(UVec2*);


	typedef const void* const GNFPluginConfig;
	typedef const void* const GNFPluginConfigProp;

	//typedef GNFPluginSetConfigProp

	// Use this to change any server config in scope_change()
	typedef enum
	{
		GNFPLUGIN_CONFIG_PROP_TYPE_INT =0,
		GNFPLUGIN_CONFIG_PROP_TYPE_STRING,
		GNFPLUGIN_CONFIG_PROP_TYPE_UVEC2
	} GNFPLUGIN_CONFIG_PROP_TYPE;

	typedef GNFPluginConfig(*GNFPluginGetConfigFunc)(const char*);
	typedef GNFPluginConfigProp(*GNFPluginGetConfigProp)(GNFPluginConfig, GNFPLUGIN_CONFIG_PROP_TYPE, const char*,void*);
	typedef void(*GNFPluginSetConfigProp)(GNFPluginConfigProp, GNFPLUGIN_CONFIG_PROP_TYPE,void*);
	typedef struct GNFPluginServerChangeScopeArgs
	{
		GNFPluginGetConfigFunc fGetConfig;
		GNFPluginGetConfigProp sGetConfigProps;
		GNFPluginSetConfigProp sSetConfigProps;
	}GNFPluginServerChangeScopeArgs;

	typedef void(*GNFPluginScopeChangePoint)(pGNFPluginObject,const GNFPluginServerChangeScopeArgs*);

	// Begins of the plugin. This method is the first called method and will be called for initalization
	typedef struct GNFPluginEntryPArgs
	{
		GNFPluginVersion editorVersion;
		GNFPluginRegisterFunc fRegister;
	} GNFPluginEntryPArgs;

	typedef GNFPluginExitFunc(*GNFPluginEntryP)(const GNFPluginEntryPArgs*);

#ifdef __cplusplus
}
#endif

#define GNFPLUGIN_ENTRYP_ARG const GNFPluginEntryPArgs*
#define GNFPLUGIN_SERVERLAYER_CHANGESCOPE_POINT_ARG const GNFPluginServerChangeScopeArgs* 
#endif