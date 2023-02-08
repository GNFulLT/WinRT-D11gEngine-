#include "pch.h"
#include "EntityManager.h"
#include "Entitiy/Triangle2D.h"
#include "Common/Utils/utils_imgui.h"
#include <sstream>
#include "Common/Utils/utils_common.h"
namespace GNF::Core
{
	EntityManager::~EntityManager()
	{
		int a = 5;
	}

	void EntityManager::Init()
	{
		m_window.reset(new GUI::IFixedWindowChild("Helllo", 0, 0, 0.2f, 0.2f));
		m_window->Init();
	}

	bool EntityManager::ChangeNameOf(Entity::EntityID id, const char* newName)
	{
		if (auto entity = m_map.find(id); entity != m_map.end())
		{
			if (m_nameMap.find(newName) != m_nameMap.end())
			{
				Common::Logger::LogWarn("There is already named entity with that name");
				return false;
			}
			else
			{
				auto entity2 = m_nameMap.find(entity->second.first->m_name);
				m_nameMap.erase(entity2);
				entity->second.first->m_name = newName;
				m_nameMap.emplace(entity->second.first->m_name,entity->second.first);
				return true;
			}
		}
		else
		{
			throw std::runtime_error("An unknown error occured");
		}
		
	}

	void EntityManager::RenderSGui()
	{
		if(ImGui::Begin("Scene",0,ImGuiWindowFlags_::ImGuiWindowFlags_NoResize))
		{
			for (auto& entity : m_map)
			{
				bool isSelected =  m_selectedEntity == nullptr ? false :  entity.second.first->m_entityId == m_selectedEntity->first->m_entityId;
				if (ImGui::Selectable(entity.second.first->GetName().c_str(),isSelected))
				{
					m_selectedEntity = &entity.second;
					SelectedEntityChanged(m_selectedEntity->first);
				}
			}
		}
		ImGui::End();

		if (ImGui::Begin("Properties", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize))
		{
			ImGui::Text("First Select an entity");
		}
		ImGui::End();
	}

	std::weak_ptr<Entity::IEntity> EntityManager::CreateTriangle2D(const float edgeSize, DirectX::SimpleMath::Vector3 worldPos)
	{
		auto triangle = new Entity::Triangle2D(edgeSize, worldPos);
		auto uuid = RegisterEntity(triangle,nullptr);
		return GetEntityAsWeak(uuid);
	}

	Entity::EntityID EntityManager::RegisterEntity(Entity::IEntity* entity,bool* isSucceed)
	{
		//!: Entity already registered
		if (!entity->m_entityId == 0)
		{
			if(isSucceed != nullptr)
				*isSucceed = false;
			return 0;
		}
		auto hashedId = Common::Utils::CreateUniqueID();
		entity->SetId(hashedId);


		m_map.emplace(hashedId, std::pair<std::shared_ptr<Entity::IEntity>,bool>(entity,false) );

		auto name = FindNameForEntity(entity);
		
		
		m_nameMap.emplace(name, m_map.find(hashedId)._Ptr->_Myval.second.first);
		entity->m_name = name;

		std::string suggestedName = entity->m_name;

		if (isSucceed != nullptr)
			*isSucceed = true;
		return hashedId;
	}

	std::string EntityManager::FindNameForEntity(Entity::IEntity* entity)
	{
		std::stringstream suggestedName; 
		suggestedName << entity->m_name.c_str();
		if (m_nameMap.find(suggestedName.str()) != m_nameMap.end())
		{
			suggestedName << " (1)";
			int i = 2;
			while (m_nameMap.find(suggestedName.str()) != m_nameMap.end())
			{
				suggestedName.seekp(-2,::std::ios_base::end);
				suggestedName << i << ")";
				i++;
			}
		}

		return suggestedName.str();
	}
}