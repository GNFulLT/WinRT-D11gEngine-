#pragma once

#include "IRenderer2.h"

namespace GNF::Graphic
{
	class IRenderer3 : public IRenderer2
	{
		public:
			virtual ~IRenderer3() {};
		
			virtual void Render() = 0;

			virtual void InitResources() = 0;

			virtual void UpdateThings() = 0;
	};
}