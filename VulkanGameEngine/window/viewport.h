#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../core/object/object.h"
#include "../plugin/pinclude/basic_shared_types.h"
#include "../core/typedefs.h"
#include "../config/config.h"
#include <memory>
#include <cassert>

class Viewport : public Object
{
	OBJECT_DEF(Viewport,Object)

public:
	virtual ~Viewport() = default;
	Viewport();

	enum MSAA {
		MSAA_DISABLED,
		MSAA_2X,
		MSAA_4X,
		MSAA_8X
	};
	

	_INLINE_ const UVec2& get_size() const noexcept
	{
		return *m_size->get_prop();
	}

	_INLINE_ const MSAA get_msaa_settings() const noexcept
	{
		return *m_msaa_settings->get_prop();
	}
protected:
	//X TODO : Add Merge for Configs
	virtual Config* config_creation();

protected:
	std::shared_ptr<ConfigProp<MSAA>> m_msaa_settings;
	std::shared_ptr<ConfigProp<UVec2>> m_size;
	std::shared_ptr<ConfigProp<UVec2>> m_pos;
};

#endif // VIEWPORT_H
