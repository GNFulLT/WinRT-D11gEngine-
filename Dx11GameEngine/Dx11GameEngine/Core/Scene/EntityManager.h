#pragma once

#include "Entitiy/IEntity.h"
#include <unordered_map>
#include <memory>
#include "Common/Logger.h"
#include <vector>
#include <boost/signals2.hpp>
#include "GUI/Icon.h"
#include "Core/TextureManager.h"
#include "Common/IResource.h"
/*
namespace std
{
	template<>
	struct hash<boost::uuids::uuid>
	{
		std::size_t operator()(const boost::uuids::uuid& id) const
		{
			return boost::hash_value(id);
		}
	};
}
*/

namespace GNF::Core
{
	typedef boost::signals2::signal<void(const std::weak_ptr<Entity::IEntity>&)> EntityChangedSignal;

	class EntityManager : public Common::IResource
	{

	public:
		EntityManager() : IResource("EntityManager")
		{}
		~EntityManager();

		void PreRender();

		bool ChangeNameOf(Entity::EntityID, const char* newName);

		inline auto AddSlotEntityChanged(const EntityChangedSignal::slot_type& slot) const noexcept
		{
			return SelectedEntityChanged.connect(slot);
		}

		void Init() override;
		void RenderSGui();
		std::weak_ptr<Entity::IEntity> CreateTriangle2D(const float edgeSize,DirectX::SimpleMath::Vector3 worldPos = Entity::DEFAULT_WORLD_POS);
		inline std::weak_ptr<Entity::IEntity> GetFocusedEntity() const noexcept
		{
			if (m_selectedEntity == nullptr)
				return std::weak_ptr<Entity::IEntity>();
			return m_selectedEntity->first;
		}
	public:
		Entity::EntityID RegisterEntity(Entity::IEntity* entity, bool* isSucceed);
		

		inline std::weak_ptr<Entity::IEntity> GetEntityAsWeak(Entity::EntityID entityId)
		{	
			if (auto entity = m_map.find(entityId); entity != m_map.end())
			{
				return entity._Ptr->_Myval.second.first;
			}
			else
			{
				Common::Logger::LogCritical("Wanted a entity that is not recognized by Entity Manager");
				return std::weak_ptr<Entity::IEntity>();
			}
		}

		inline void UnregisterEntityIfExist(Entity::EntityID id)
		{
			//! Delete If Exist
			if (auto it = m_map.find(id); it != m_map.end())
			{
				m_map.erase(it);
			}
		}
	private:

		std::string FindNameForEntity(Entity::IEntity* entity);

		std::unordered_map<Entity::EntityID, std::pair<std::shared_ptr<Entity::IEntity>,bool>> m_map;

		std::unordered_map<std::string,std::shared_ptr<Entity::IEntity>> m_nameMap;

		
		std::pair<std::shared_ptr<Entity::IEntity>, bool>* m_selectedEntity = nullptr;

		std::unique_ptr<GUI::Icon> m_transformIcon;
		std::unique_ptr<GUI::Icon> m_objectIcon;

		char m_entityPrevName[ENTITY_NAME_MAX_LENGTH] = { 0 };
		uint_fast8_t m_charUsage = 0;

		//!: Const Values
	private:
		float m_iconCenterY;
	//!: Signals
	private:
		mutable EntityChangedSignal SelectedEntityChanged;
	};
}