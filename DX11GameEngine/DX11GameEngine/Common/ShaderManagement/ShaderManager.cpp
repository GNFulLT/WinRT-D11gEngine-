#include "ShaderManager.h"

#include "Common/Event/EventHandler.h"

namespace GNF::ShaderManagement
{
	Bindable::VertexShaderBindable* ShaderManager::GetShaderBindable(CommonVertexShaders vs)
	{
		return m_vertexShaders[vs].get();
	}
	Bindable::PixelShaderBindable* ShaderManager::GetShaderBindable(CommonPixelShaders ps)
	{
		return m_pixelShaders[ps].get();
	}

	//Use this for creating shader resources for specific device resource.
	void ShaderManager::LoadAllShaderTypes()
	{

		for (int i = 0; i < sizeof(AllVertexShaderTypes) / sizeof(AllVertexShaderTypes[0]); i++)
		{
			CreateVertexShader(AllVertexShaderTypes[i]);
		}

		for (int i = 0; i < sizeof(AllPixelShaderTypes) / sizeof(AllPixelShaderTypes[0]); i++)
		{
			CreatePixelShader(AllPixelShaderTypes[i]);

		}
	}

	void ShaderManager::CreateVertexShader(CommonVertexShaders shaderType)
	{
		m_vertexShaders[shaderType].reset(new Bindable::VertexShaderBindable(VertexShaders[shaderType], shaderType, pParentDevice));
		m_vertexShaders[shaderType]->Binded += Event::EventHandler::Bind(&ShaderManager::OnVertexShaderBinded, this);
	}

	void ShaderManager::CreatePixelShader(CommonPixelShaders shaderType)
	{
		m_pixelShaders[shaderType].reset(new Bindable::PixelShaderBindable(PixelShaders[shaderType], shaderType, pParentDevice));
		m_pixelShaders[shaderType]->Binded += Event::EventHandler::Bind(&ShaderManager::OnPixelShaderBinded, this);
	}

	void ShaderManager::OnVertexShaderBinded(const Bindable::VertexShaderBindable& bindedVertexShader)
	{
		m_bindedVertexShader = &bindedVertexShader;
	}
	void ShaderManager::OnPixelShaderBinded(const Bindable::PixelShaderBindable& bindedPixelShader)
	{
		m_bindedPixelShader = &bindedPixelShader;
	}

	const Bindable::PixelShaderBindable* ShaderManager::GetBindedPixelShader()
	{
		return m_bindedPixelShader;
	}
	const Bindable::VertexShaderBindable* ShaderManager::GetBindedVertexShader()
	{
		return m_bindedVertexShader;
	}
	
}
