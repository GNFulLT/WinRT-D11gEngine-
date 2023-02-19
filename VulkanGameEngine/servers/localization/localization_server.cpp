#include "localization_server.h"

#include "core/string/string_utils.h"

void LocalizationServer::init()
{
}

void LocalizationServer::load_localization(const String16& path)
{
	throw FileNotFoundException("File not found");
}

const String32* LocalizationServer::get_str(const String& id) const noexcept
{
	if (const auto& finded = m_localizationMap.find(hash_string(id)); finded != m_localizationMap.end())
	{
		return &finded->second;
	}
	else
	{
		return &g_nullString;
	}
}


