#include "pch.h"
#include "ShaderManager.h"

#include "DeviceResources.h"
#include "Bindable/VertexShaderBindable.h"
#include "Bindable/PixelShaderBindable.h"

namespace GNF::Common
{
	Bindable::IShaderBindable* ShaderManager::GetShaderBindable(CommonVertexShaders vs)
	{
		return m_vertexShaders[vs].get();
	}
	Bindable::IShaderBindable* ShaderManager::GetShaderBindable(CommonPixelShaders ps)
	{
		return m_pixelShaders[ps].get();
	}

	void ShaderManager::LoadAllShaders(DeviceResources* resources)
	{

		for (int i = 0; i < sizeof(AllVertexShaderTypes) / sizeof(AllVertexShaderTypes[0]); i++)
		{
			CreateVertexShader(resources, AllVertexShaderTypes[i]);
		}

		for (int i = 0; i < sizeof(AllPixelShaderTypes) / sizeof(AllPixelShaderTypes[0]); i++)
		{
			CreatePixelShader(resources, AllPixelShaderTypes[i]);

		}
	}

	void ShaderManager::CreateVertexShader(DeviceResources* resources, CommonVertexShaders shaderType)
	{
		m_vertexShaders[shaderType].reset(new Bindable::VertexShaderBindable(shaderType, resources));
	}

	void ShaderManager::CreatePixelShader(DeviceResources* resources, CommonPixelShaders shaderType)
	{
		m_pixelShaders[shaderType].reset(new Bindable::PixelShaderBindable(shaderType, resources));
	}

	
	
	const void* ShaderManager::GetVertexShader(CommonVertexShaders id) const 
	{
		return VertexShaders[id].first;
	}

	UINT ShaderManager::GetVertexShaderSize(CommonVertexShaders id) const 
	{
		return VertexShaders[id].second;
	}
	

	const void* ShaderManager::GetPixelShader(CommonPixelShaders id) const 
	{
		return PixelShaders[id].first;
	}
	
	UINT ShaderManager::GetPixelShaderSize(CommonPixelShaders id) const 
	{
		return PixelShaders[id].second;
	}



}