#pragma once
#include "IShaderManager1.h"
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXMath.h> q
#include "ShBasicPixelShader.h"
#include "ShBasicVertexShader.h"
#include "ShColorCameraVertexShader.h"
#include "ShColorCameraPixelShader.h"
#include "Common/Bindable/VertexShaderBindable.h"
#include "Common/Bindable/PixelShaderBindable.h"
#include "ShBasicColoredPixelShader.h"
#include "ShBasicColoredVertexShader.h"


#include "ShCameraTexturePixelShader.h";
#include "ShCameraTextureVertexShader.h";

#include <map>
#include <memory>
namespace GNF::ShaderManagement 
{
	class ShaderManager : public IShaderManager1
	{
		public:
			ShaderManager(Device::IDeviceResources* resources)
			{
				pParentDevice = resources;
			}
			virtual Bindable::VertexShaderBindable* GetShaderBindable(CommonVertexShaders) override;
			virtual Bindable::PixelShaderBindable* GetShaderBindable(CommonPixelShaders) override;

			virtual const Bindable::PixelShaderBindable* GetBindedPixelShader() override;
			virtual const Bindable::VertexShaderBindable* GetBindedVertexShader() override;

			virtual void LoadAllShaderTypes() override;

			void CreateVertexShader(CommonVertexShaders shaderType);
			void CreatePixelShader(CommonPixelShaders shaderType);

			inline static std::map<CommonVertexShaders,std::pair<const void*,UINT>> VertexShaders = { 
				{BasicVertexShader,std::make_pair<const void*,UINT>(&g_BasicVertexShader,sizeof(g_BasicVertexShader))},
				{BasicColoredVertexShader,std::make_pair<const void*,UINT>(&g_BasicColoredVertexShader,sizeof(g_BasicColoredVertexShader))},
				{ColorCameraVertexShader,std::make_pair<const void*,UINT>(&g_ColorCameraVertexShader,sizeof(g_ColorCameraVertexShader))},
				{CameraTextureVertexShader,std::make_pair<const void*,UINT>(&g_CameraTextureVertexShader,sizeof(g_CameraTextureVertexShader))},
			};
			inline static std::map<CommonPixelShaders, std::pair<const void*, UINT>> PixelShaders = {
				{BasicPixelShader,std::make_pair<const void*,UINT>(&g_BasicPixelShader,sizeof(g_BasicPixelShader))},
				{BasicColoredPixelShader,std::make_pair<const void*,UINT>(&g_BasicColoredPixelShader,sizeof(g_BasicColoredPixelShader))},
				{ColorCameraPixelShader,std::make_pair<const void*,UINT>(&g_ColorCameraPixelShader,sizeof(g_ColorCameraPixelShader))},
				{CameraTexturePixelShader,std::make_pair<const void*,UINT>(&g_CameraTexturePixelShader,sizeof(g_CameraTexturePixelShader))},
			};

			inline static CommonVertexShaders AllVertexShaderTypes[] = { BasicVertexShader,BasicColoredVertexShader,ColorCameraVertexShader,CameraTextureVertexShader };
			inline static CommonPixelShaders AllPixelShaderTypes[] = { BasicPixelShader, BasicColoredPixelShader,ColorCameraPixelShader,CameraTexturePixelShader };

		private:
			void OnVertexShaderBinded(const Bindable::VertexShaderBindable&);
			void OnPixelShaderBinded(const Bindable::PixelShaderBindable&);

		private:
			std::map<CommonVertexShaders,std::unique_ptr<Bindable::VertexShaderBindable>> m_vertexShaders;
			std::map<CommonPixelShaders,std::unique_ptr<Bindable::PixelShaderBindable>> m_pixelShaders;
			Device::IDeviceResources* pParentDevice;
 			const Bindable::VertexShaderBindable* m_bindedVertexShader = nullptr;
			const Bindable::PixelShaderBindable* m_bindedPixelShader = nullptr;
	};
}
