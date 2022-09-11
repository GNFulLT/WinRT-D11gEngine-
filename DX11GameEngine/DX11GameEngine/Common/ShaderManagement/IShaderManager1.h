#include "IShaderManager.h"

namespace GNF::ShaderManagement
{
	class IShaderManager1 : public IShaderManager
	{
		public:
			virtual ~IShaderManager1(){}
			virtual Bindable::VertexShaderBindable* GetShaderBindable(CommonVertexShaders) = 0;
			virtual Bindable::PixelShaderBindable* GetShaderBindable(CommonPixelShaders) = 0;

			virtual const Bindable::PixelShaderBindable* GetBindedPixelShader() = 0;
			virtual const Bindable::VertexShaderBindable* GetBindedVertexShader() = 0;

			virtual void LoadAllShaderTypes() = 0;

			virtual void CreateVertexShader(CommonVertexShaders shaderType) = 0;
			virtual void CreatePixelShader(CommonPixelShaders shaderType) = 0;

			virtual void OnVertexShaderBinded(const Bindable::VertexShaderBindable&) = 0;
			virtual void OnPixelShaderBinded(const Bindable::PixelShaderBindable&) = 0;
	};
}