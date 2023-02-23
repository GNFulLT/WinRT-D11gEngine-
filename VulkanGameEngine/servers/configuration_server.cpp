#include "configuration_server.h"
#include "../core/io/io_utils.h"
#include "../core/folders.h"

#include <boost/property_tree/json_parser.hpp>

void ConfigurationServer::scope_init_begins()
{
	m_init_scope_started = true;
}

void ConfigurationServer::scope_init_ends()
{
	m_init_scope_started = false;
	m_init_scope_finished = true;
}

void ConfigurationServer::scope_expose_begins()
{
	m_expose_scope_started = true;
}

void ConfigurationServer::scope_expose_ends()
{
	m_expose_scope_started = false;
	m_expose_scope_finished = true;
}

void ConfigurationServer::scope_change_begins()
{
	m_change_scope_started = true;
}

void ConfigurationServer::scope_change_ends()
{
	m_change_scope_started = false;
	m_change_scope_finished = true;
}

bool ConfigurationServer::read_init_configuration_file(const String& fileName)
{
	auto configPath = String(CONFIG_FOLDER_FULL_PATH);
	if (!is_path_exist(configPath))
	{
		return false;
	}
	if (!is_path_exist(configPath+fileName))
	{
		return false;
	}
	if (!is_file(fileName))
	{
		return false;
	}
	const auto exts = get_file_extension(fileName);
	namespace pt = boost::property_tree;
	if (exts == ".json")
	{
		pt::ptree* tree = new pt::ptree;
		try
		{
			pt::read_json(configPath + fileName,*tree);
			m_filePTreeContainer.emplace(fileName, tree);
			return true;
		}
		catch (_UNUSED_ std::exception& ex)
		{
			delete tree;
			//X TODO: NEED LOGGER
			return false;
		}
	}
	else if (exts == ".xml")
	{
		//X TODO: Need Support 
		return false;

	}
	else if (exts == ".yaml")
	{
		//X TODO: Need Support 
		return false;

	}
	else
	{
		//X TODO: Need Support 
		return false;
	}
	return true;
}

void ConfigurationServer::destroy()
{
	delete singleton;
}