#include "plugin_server.h"

#include "../core/io/io_utils.h"
#include <boost/filesystem.hpp>
#include "../core/string/string_utils.h"
#include "../core/version.h"
#include "../plugin/plugin_native_event.h"

PluginServer::PluginServer()
{

}

bool PluginServer::is_plugin_version_supported(unsigned int major, unsigned int minor)
{
	if (major == 0)
		return false;
	if (major == 1 && minor < 1)
		return false;
	return true;
}

GNFPluginBool PluginServer::register_object(pGNFPluginConstName name, GNF_PLUGIN_LAYER_LEVEL layerLevel, GNFPluginVersion* vers, GNFPluginClassConstructor ctor, GNFPluginClassDestructor destructor, GNF_PLUGIN_BIND_TYPE bindType)
{
	//X TODO: NEED LOGGER
	bool versionSupported = is_plugin_version_supported(vers->version_major, vers->version_minor);

	if (!versionSupported)
		return (GNFPluginBool)GNFPLUGIN_ERROR_CODE_VERSION_NOT_COMPATIBLE;

	std::string pluginName(name);

	if (singleton->m_nativePreLoadedLibs	.find(pluginName) != singleton->m_nativePreLoadedLibs.end())
		return (GNFPluginBool)GNFPLUGIN_ERROR_CODE_PLUGIN_NAME_EXIST;

	

	g_initThisLib = true;
	g_currentPluginVersion = vers;
	g_currentPluginBindType = bindType;
	g_currentClassConstructor = ctor;
	g_currentClassDestructor = destructor;
	g_currentName = name;
	return 0;
}

void PluginServer::load_all_plugins()
{
	bool exist = is_path_exist(m_folderPath);

	if (!exist)
	{
		create_path(m_folderPath);
		return;
	}

	boost::filesystem::path path(string16_to_wstr(m_folderPath));

	if (boost::filesystem::is_directory(path))
	{
		boost::filesystem::directory_iterator end_itr;
		for (boost::filesystem::directory_iterator itr(path); itr != end_itr; ++itr)
		{
			if (boost::filesystem::is_regular_file(itr->status()))
			{
				auto filePath = itr->path().filename().wstring();
				load_specific_plugin(filePath);
			}
		}
	}
}

void PluginServer::init_all_plugins()
{
	for (const auto& plugin : m_nativePreLoadedLibs)
	{
		plugin.second->init();
	}
}

bool PluginServer::load_specific_plugin(const std::wstring& filename)
{
	bool exist = is_path_exist(m_folderPath);

	if (!exist)
	{
		create_path(m_folderPath);
		return false;
	}

	namespace fs = boost::filesystem;

	fs::path dir(string16_to_wstr(m_folderPath));
	fs::path file(filename);

	fs::path fullPath = dir / file;

	String err;

	auto lib = DynamicLibrary::load_if_exist(fullPath.string(), _STR_XDEF(GNFPLUGIN_ENTRYP_NAME),err);
	
	//X TODO : Need Logger
	if (lib == nullptr)
	{
		return false;
	}
	bool loaded = lib->is_loaded();
	bool isExist = lib->has_function(_STR_XDEF(GNFPLUGIN_ENTRYP_NAME));
	auto pFunc = lib->get_function<GNFPluginEntryP>(_STR_XDEF(GNFPLUGIN_ENTRYP_NAME));

	if (pFunc == nullptr)
	{
		delete lib;
		return false;
	}
	
	GNFPluginEntryPArgs arg;
	arg.editorVersion.version_major = GNF_VERSION_MAJOR;
	arg.editorVersion.version_minor = GNF_VERSION_MINOR;

	arg.fRegister = register_object;
	auto res = pFunc(&arg);

	if (g_initThisLib)
	{
		// Load This Plugin
		std::string suggestedName(g_currentName);
		GNFPlugin* plugin;
		switch (g_currentPluginBindType)
		{
		case GNF_PLUGIN_BIND_TYPE_EVENT:
			plugin = new GNFPluginNativeEvent(suggestedName, lib, g_currentClassConstructor, g_currentClassDestructor, res, *g_currentPluginVersion);
			break;
		default:
			break;
		}
		m_nativePreLoadedLibs.emplace(suggestedName,plugin);
		g_currentClassConstructor = nullptr;
		g_currentClassDestructor = nullptr;
		g_currentName = nullptr;
		g_currentPluginVersion = nullptr;
		g_initThisLib = false;
		return true;
	}
	else
	{
		delete lib;
		return false;
	}
	return true;
}

bool PluginServer::load_specific_plugin(const String16& filename)
{
	return load_specific_plugin(string16_to_wstr(filename));
}

void PluginServer::destroy()
{
	delete singleton;
}
