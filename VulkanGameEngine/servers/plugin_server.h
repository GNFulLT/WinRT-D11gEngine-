#ifndef PLUGIN_SERVER
#define PLUGIN_SERVER

#include "../core/object/object.h"
#include "../core/typedefs.h"
#include <memory>
#include <unordered_map>
#include "../core/io/dynamic_library.h"
#include "../plugin/native_include/plugin_interface.h"
#include "../plugin/plugin.h"

class CreationServer;

struct PluginRegisterInfo
{
	GNFPluginClassConstructor ctor;
	GNFPluginClassDestructor dtor;
	GNFPluginExitFunc exitFunct;
};

class PluginServer : public Object
{
	OBJECT_DEF(PluginServer,Object)

public:
	PluginServer(const PluginServer&) = delete;
	PluginServer& operator=(const PluginServer&) = delete;
	

	_INLINE_ static PluginServer* get_singleton()
	{
		return singleton;
	}

	void destroy();


	void load_all_plugins();

	bool load_specific_plugin(const String16& filename);
	bool load_specific_plugin(const std::wstring& filename);
	
	void init_all_plugins();


	static bool is_plugin_version_supported(unsigned int major, unsigned int minor);
private:
	static GNFPluginBool register_object(pGNFPluginConstName name,GNF_PLUGIN_LAYER_LEVEL layerLevel,GNFPluginVersion* vers,GNFPluginClassConstructor ctor,GNFPluginClassDestructor destructor, GNF_PLUGIN_BIND_TYPE);

	PluginServer();
private:
	// String 16 support for paths

	String16 m_folderPath = u"./plugins";
	std::unordered_map<std::string, std::shared_ptr<GNFPlugin>> m_nativePreLoadedLibs;
private:
	friend class CreationServer;

	// Native Plugin Helpers
	_INLINE_ static bool g_initThisLib = false;
	_INLINE_ static GNF_PLUGIN_BIND_TYPE g_currentPluginBindType = GNF_PLUGIN_BIND_TYPE::GNF_PLUGIN_BIND_TYPE_EVENT;
	_INLINE_ static GNFPluginClassConstructor g_currentClassConstructor = nullptr;
	_INLINE_ static GNFPluginClassDestructor g_currentClassDestructor = nullptr;
	_INLINE_ static GNFPluginVersion* g_currentPluginVersion = nullptr;
	_INLINE_ static const char* g_currentName = nullptr;
	_INLINE_ static PluginServer* create_singleton()
	{
		singleton = new PluginServer();
		return get_singleton();
	}
	_INLINE_ static PluginServer* singleton;
};

#endif // PLUGIN_SERVER
