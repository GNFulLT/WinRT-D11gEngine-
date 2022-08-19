#pragma once
#include "IShaderManager.h"
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h> 
#include "ShBasicPixelShader.h"
#include "ShBasicVertexShader.h"
#include "Bindable/VertexShaderBindable.h"
#include "Bindable/PixelShaderBindable.h"
#include "ShaderManager.fwd.h"


namespace GNF::Common 
{
	class ShaderManager : public IShaderManager
	{
		public:
			ShaderManager() = default;
			virtual void CreateVertexShader(DeviceResources* resources, CommonVertexShaders);
			virtual void CreatePixelShader(DeviceResources* resources, CommonPixelShaders);

			virtual const void* GetVertexShader(CommonVertexShaders) const;
			virtual UINT GetVertexShaderSize(CommonVertexShaders) const;
			virtual const void* GetPixelShader(CommonPixelShaders) const;
			virtual UINT GetPixelShaderSize(CommonPixelShaders) const;

			virtual const void* GetBindedVertexShader() const;
			virtual UINT GetBindedVertexShaderSize() const;


			virtual const void* GetBindedPixelShader() const;
			virtual UINT GetBindedPixelShaderSize() const;

			virtual void LoadAllShaders(DeviceResources* resources);


			inline static std::map<CommonVertexShaders,std::pair<const void*,UINT>> VertexShaders = { 
				{BasicVertexShader,std::make_pair<const void*,UINT>(&g_BasicVertexShader,sizeof(g_BasicVertexShader))},
 };
			inline static std::map<CommonPixelShaders, std::pair<const void*, UINT>> PixelShaders = {
				{BasicPixelShader,std::make_pair<const void*,UINT>(&g_BasicPixelShader,sizeof(g_BasicPixelShader))},
			};

			virtual Bindable::IShaderBindable* GetShaderBindable(CommonVertexShaders);
			virtual Bindable::IShaderBindable* GetShaderBindable(CommonPixelShaders);

			inline static CommonVertexShaders AllVertexShaderTypes[] = { BasicVertexShader };
			inline static CommonPixelShaders AllPixelShaderTypes[] = { BasicPixelShader };

		private:
			void OnVertexShaderBinded(const Bindable::VertexShaderBindable&);
			void OnPixelShaderBinded(const Bindable::PixelShaderBindable&);

		private:
			std::map<CommonVertexShaders,std::unique_ptr<Bindable::VertexShaderBindable>> m_vertexShaders;
			std::map<CommonPixelShaders,std::unique_ptr<Bindable::PixelShaderBindable>> m_pixelShaders;

			const Bindable::VertexShaderBindable* m_bindedVertexShader = nullptr;
			const Bindable::PixelShaderBindable* m_bindedPixelShader = nullptr;
	};
}
