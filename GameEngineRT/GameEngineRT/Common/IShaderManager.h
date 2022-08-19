#pragma once
#include "pch.h"
#include "Common/Bindable/IShaderBindable.h"



namespace GNF::Common
{
	enum CommonVertexShaders
	{
		BasicVertexShader = 0,
	};
	enum CommonPixelShaders
	{
		BasicPixelShader = 0,
	};

	class DeviceResources;
	class IShaderManager
	{
		public:
			virtual void LoadAllShaders(DeviceResources* resources) = 0;

			virtual void CreateVertexShader(DeviceResources* resources, CommonVertexShaders) = 0;
			virtual const void* GetVertexShader(CommonVertexShaders) const = 0;
			virtual UINT GetVertexShaderSize(CommonVertexShaders) const = 0;

			virtual void CreatePixelShader(DeviceResources* resources, CommonPixelShaders) = 0;
			virtual const void* GetPixelShader(CommonPixelShaders) const = 0;
			virtual UINT GetPixelShaderSize(CommonPixelShaders) const = 0;

			virtual Bindable::IShaderBindable* GetShaderBindable(CommonVertexShaders) = 0;
			virtual Bindable::IShaderBindable* GetShaderBindable(CommonPixelShaders) = 0;

			virtual const void* GetBindedPixelShader() const = 0;
			virtual UINT GetBindedPixelShaderSize() const = 0;

			virtual const void* GetBindedVertexShader() const = 0;
			virtual UINT GetBindedVertexShaderSize() const = 0;
	};
}