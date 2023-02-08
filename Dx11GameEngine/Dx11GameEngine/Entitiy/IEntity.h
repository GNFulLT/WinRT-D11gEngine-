#pragma once

#include <DirectXTK/SimpleMath.h>
#include <memory>
#include "Core/Bindable/IBindable.h"
#include <vector>
#include "Core/VertexBuffer/OnlyVertexBuffer.h"
#include <string>
#include "Texture/TextureInfo.h"
#define ENTITY_NAME_MAX_LENGTH 100

namespace GNF::Core
{

	class EntityManager;
}
namespace GNF::GUI
{
	class EntityNode;
}
namespace GNF::Entity
{
	typedef unsigned long long EntityID;

	inline constexpr static DirectX::SimpleMath::Vector3 DEFAULT_WORLD_POS = DirectX::SimpleMath::Vector3(0, 0, 0);
	class IEntity 
	{
	public:
		IEntity(const std::string& name);
		virtual ~IEntity() = default;

		//!: This is for creation. Give Indices and Vertexes in this method. If you created with EntityManager the entity. Dont use it
		virtual void OnCreated() = 0;

		//!: Update Method Per Frame
		virtual void Update(float const deltaTime) = 0;

		//!: 
		virtual void Destroy() = 0;

	public:
		void SetTexture(Texture::TextureID textureId);
		
		inline void RemoveTexture()
		{
			m_textureId = 0;
		}

		EntityID GetID()
		{
			return m_entityId;
		}
		virtual void Draw();
		void ReSetVerticesIndices();
		
		inline const DirectX::SimpleMath::Vector3& GetScale()
		{
			return m_scale;
		}

		inline const DirectX::SimpleMath::Vector3& GetPosition()
		{
			return m_pos;
		}

		inline const DirectX::SimpleMath::Quaternion& GetRotation()
		{
			return m_rotationQuat;
		}
	protected:
		inline void SetVertices(const std::vector<GNF::Core::VertexBuffer::OnlyVertexBuffer>& vertices)
		{
			this->m_vertices = vertices;
		}

		inline void SetIndices(const std::vector<UINT>& indices)
		{
			this->m_indices = indices;
		}

		const std::string& GetName()
		{
			return m_isNamed ? m_name : m_defaultName;
		}
		
		//!: For Entity Manager
	private:

		inline DirectX::SimpleMath::Vector3& GetPositionChangable()
		{
			return m_pos;
		}

		inline DirectX::SimpleMath::Vector3& GetScaleChangable()
		{
			return m_scale;
		}

		inline DirectX::SimpleMath::Quaternion& GetRotationChangable()
		{
			return m_rotationQuat;
		}

		void SetId(EntityID entityId)
		{
			m_defaultName = std::to_string(entityId);
			m_entityId = entityId;
		}

		inline DirectX::SimpleMath::Matrix GetModelMatrix()
		{
			return DirectX::SimpleMath::Matrix::CreateTranslation(m_pos) * DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotationQuat) * DirectX::SimpleMath::Matrix::CreateScale(m_scale);
		}

		std::string m_name;
		std::string m_defaultName;
		bool m_isNamed = false;
		EntityID m_entityId;
		Texture::TextureID m_textureId = 0;
		friend class GNF::Core::EntityManager;
		friend class GNF::GUI::EntityNode;
	private:
		std::vector<GNF::Core::VertexBuffer::OnlyVertexBuffer> m_vertices;
		
		std::vector<UINT> m_indices;


		//!: DirectX Stuff
	private:
		DirectX::SimpleMath::Vector3 m_scale = DirectX::SimpleMath::Vector3(1.f, 1.f, 1.f);
		DirectX::SimpleMath::Quaternion m_rotationQuat;
		DirectX::SimpleMath::Vector3 m_pos = DirectX::SimpleMath::Vector3(0.f, 0.f, 0.f);

		std::unique_ptr<Core::Bindable::IBindable> m_vertexBuffer;
		std::unique_ptr<Core::Bindable::IBindable> m_indexBuffer;
	};
}