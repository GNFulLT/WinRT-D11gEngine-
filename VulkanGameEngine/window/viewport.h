#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "../core/object/object.h"
#include "../core/vec2.h"
#include "../core/typedefs.h"

class Viewport : public Object
{
	OBJECT_DEF(Viewport,Object)

public:
	enum MSAA {
		MSAA_DISABLED,
		MSAA_2X,
		MSAA_4X,
		MSAA_8X
	};
	
	_INLINE_ const UVec2& get_size() const noexcept
	{
		return m_size;
	}

	_INLINE_ const MSAA get_msaa_settings() const noexcept
	{
		return m_msaa_settings;
	}

private:
	MSAA m_msaa_settings = MSAA_DISABLED;
	UVec2 m_size;
	UVec2 m_pos;
};

#endif // VIEWPORT_H
