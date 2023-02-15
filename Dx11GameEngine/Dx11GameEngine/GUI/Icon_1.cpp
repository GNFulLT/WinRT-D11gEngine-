#include "pch.h"

#include "Icon_1.h"
#include <boost/filesystem.hpp>
#include <format>

namespace GNF::GUI
{
	Icon_1::Icon_1(const wchar_t* filePath, Core::TextureManager* manager, float width, float height) :
		Common::IResource(std::format("Icon {}", boost::filesystem::path(filePath).filename().string().c_str()).c_str()),
		m_filePath(filePath),m_initialWidth(width),m_initialHeight(height),m_manager(manager)
	{

	}

	void Icon_1::Init()
	{
		m_icon.reset(new Icon(m_filePath.c_str(), m_manager, m_initialWidth, m_initialHeight));
	}
}