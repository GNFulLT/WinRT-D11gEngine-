#include "config.h"

bool Config::can_be_serialize()
{
	return false;
}

bool Config::serialize(const String& key, const String& path)
{
	return false;
}

bool Config::serialize(const String& path)
{
	return false;
}
