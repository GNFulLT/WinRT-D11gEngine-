#ifndef GRAPHIC_API_H
#define GRAPHIC_API_H

#include <unordered_map>
#include "../core/serialize/serializable_enum.h"

enum GRAPHIC_API
{
	GRAPHIC_API_VULKAN = 0, 
	GRAPHIC_API_DIRECTX_12,
	GRAPHIC_API_DIRECTX_11
};


#endif // GRAPHIC_API_H