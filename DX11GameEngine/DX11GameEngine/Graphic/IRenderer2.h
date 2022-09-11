#pragma once

#include "IRenderer.h"

namespace GNF::Graphic
{
	class IRenderer2 : public IRenderer
	{
		public:
			virtual ~IRenderer2() {};
		
			virtual void Render() = 0;

			virtual void InitResources() = 0;
	};
}