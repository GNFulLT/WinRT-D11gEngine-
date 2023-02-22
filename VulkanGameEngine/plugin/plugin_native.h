#ifndef PLUGIN_NATIVE_H
#define PLUGIN_NATIVE_H

#include "plugin.h"

#include "../core/io/dynamic_library.h"
#include "native_include/plugin_interface.h"

static GNFPluginBool error_native_event_opener(GNFPluginPackagedArgs packaged, GNFPluginNativeEventConstructorArgs** converter);

class GNFPluginNative : public GNFPlugin
{
	OBJECT_DEF(GNFPlugin, GNFPlugin)

public:

	GNFPluginNative(const String& name,DynamicLibrary* lib, GNFPluginClassConstructor ctor, GNFPluginClassDestructor dtor, GNFPluginExitFunc exitFunc,GNFPluginVersion vers) :
		GNFPlugin(name,0x1000 * vers.version_major + 0x100 * vers.version_major),p_ctor(ctor),
		p_dtor(dtor),p_exitFunc(exitFunc)
	{
		m_lib.reset(lib);
	}

	virtual ~GNFPluginNative()
	{
		p_exitFunc();
		m_lib.reset();
	}
	
	virtual void init() = 0;

	virtual void destroy() = 0;

	GNFPLUGIN_TYPE get_plugin_type() override;
protected:
	constexpr static GNFPLUGIN_TYPE g_plugin_type = GNFPLUGIN_NATIVE;
	void* m_resource;
	std::unique_ptr<DynamicLibrary> m_lib;
	GNFPluginClassConstructor p_ctor;
	GNFPluginClassDestructor p_dtor;
	GNFPluginExitFunc p_exitFunc;
};


#endif // PLUGIN_NATIVE_H