#include "viewport.h"

Viewport::Viewport() :
	m_msaa_settings(new ConfigProp<MSAA>(MSAA_DISABLED)),
	m_pos(new ConfigProp<core::UVec2>({0,0})),
	m_size(new ConfigProp<core::UVec2>({0,0}))
{
	
}

Config* Viewport::config_creation()
{
	assert(false);
	return nullptr;
}