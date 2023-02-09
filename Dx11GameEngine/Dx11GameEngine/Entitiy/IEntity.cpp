#include "pch.h"
#include "IEntity.h"
#include "Core/Game.h"
#include "Core/GraphicEngine/GraphicEngine.h"
namespace GNF::Entity
{
	

	IEntity::IEntity(const std::string& name) : m_name(name)
	{

	}

	void IEntity::SetTexture(Texture::TextureID textureId)
	{
		
		if (Core::Game::GetInstance()->GetCurrentTextureManager()->IsTextureAvailable(textureId))
		{
			m_textureId = textureId;
		}
		else
		{
			Common::Logger::LogCritical("Tried to bind a texture that is not recognized by texture manager");
			m_textureId = 0;
		}
	}

	void IEntity::Draw()
	{
		Core::Game::GetInstance()->GetCamera()->Use(GetModelMatrix());
		Core::Game::GetInstance()->GetCurrentTextureManager()->BindTexture(m_textureId);
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();
		
		Core::GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Core::GraphicEngine::GraphicEngine::GetInstance()->GetD3DContext()->DrawIndexed(m_indices.size(), 0, 0);
	}

	void IEntity::ReSetVerticesIndices()
	{
		m_vertexBuffer.reset(Core::GraphicEngine::GraphicEngine::GetInstance()->CreateDefaultVertexBuff(m_vertices));
		m_indexBuffer.reset(Core::GraphicEngine::GraphicEngine::GetInstance()->CreateDefaultIndexBuff(m_indices));
	}
}