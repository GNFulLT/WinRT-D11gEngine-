#include "pch.h"
#include "ShaderManager.h"

#include "DeviceResources.h"

#include "Event/EventHandler.h"

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

	//Use this for creating shader resources for specific device resource.
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
		m_vertexShaders[shaderType]->Binded += Event::EventHandler::Bind(&ShaderManager::OnVertexShaderBinded, this);
	}

	void ShaderManager::CreatePixelShader(DeviceResources* resources, CommonPixelShaders shaderType)
	{
		m_pixelShaders[shaderType].reset(new Bindable::PixelShaderBindable(shaderType, resources));
		m_pixelShaders[shaderType]->Binded += Event::EventHandler::Bind(&ShaderManager::OnPixelShaderBinded, this);
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

	void ShaderManager::OnVertexShaderBinded(const Bindable::VertexShaderBindable& bindedVertexShader)
	{
		m_bindedVertexShader = &bindedVertexShader;
	}
	void ShaderManager::OnPixelShaderBinded(const Bindable::PixelShaderBindable& bindedPixelShader)
	{
		m_bindedPixelShader = &bindedPixelShader;
	}
	
	const void* ShaderManager::GetBindedVertexShader() const
	{
		return m_bindedVertexShader->GetShader();
	}
	UINT ShaderManager::GetBindedVertexShaderSize() const
	{
		return m_bindedVertexShader->GetShaderSize();
	}


	const void* ShaderManager::GetBindedPixelShader() const
	{
		return m_bindedVertexShader->GetShader();
	}
	UINT ShaderManager::GetBindedPixelShaderSize() const
	{
		return m_bindedPixelShader->GetShaderSize();
	}
}
