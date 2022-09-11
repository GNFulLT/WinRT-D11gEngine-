#pragma once

#include <Windows.h>
namespace GNF::Texture
{
	class ITextureManager
	{
		public:
			virtual UINT CreateTexture(const WCHAR* path) = 0;
			virtual void BindTexture(UINT id) = 0;
			virtual ~ITextureManager() {}
	};
}