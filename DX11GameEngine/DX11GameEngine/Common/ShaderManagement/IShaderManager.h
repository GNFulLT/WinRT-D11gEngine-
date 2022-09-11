#pragma once
#include "Common/Bindable/IShaderBindable.h"
#include "Common/Device/IDeviceResources.h"
#include "Common/Bindable/PixelShaderBindable.h"
#include "Common/Bindable/VertexShaderBindable.h"
#include "ShaderTypes.h"

using namespace GNF::Common;

namespace GNF::ShaderManagement
{


	class IShaderManager
	{
		public:
			virtual ~IShaderManager()
			{

			}
			virtual void LoadAllShaderTypes() = 0;

			virtual Bindable::VertexShaderBindable* GetShaderBindable(CommonVertexShaders) = 0;
			virtual Bindable::PixelShaderBindable* GetShaderBindable(CommonPixelShaders) = 0;

			virtual const Bindable::PixelShaderBindable* GetBindedPixelShader() = 0;
			virtual const Bindable::VertexShaderBindable* GetBindedVertexShader() = 0;
	};
}