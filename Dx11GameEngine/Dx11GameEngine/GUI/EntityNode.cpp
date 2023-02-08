#include "pch.h"
#include "EntityNode.h"
#include "Core/Game.h"
#include <boost/bind.hpp>
#include "Common/Utils/utils_imgui.h"

namespace GNF::GUI
{

	constexpr static const char* g_nodeText = "Entity Node";
	constexpr static const char* g_positionText = "Position";
	constexpr static const char* g_x = "X";
	constexpr static const char* g_y = "Y";
	constexpr static const char* g_z = "Z";
	constexpr static const char* g_scaleText = "Scale";
	constexpr static const float g_gapBtw = 10.f;
	constexpr static const float g_rightAlign = 10.f;

	EntityNode::EntityNode()
	{
	}


	void EntityNode::RenderSGui()
	{
		if (ImGui::TreeNode(g_nodeText))
		{
			if (auto entity = m_entity.lock())
			{

				auto& pos = entity->GetPositionChangable();
				auto& scale = entity->GetScaleChangable();

				if (entity->m_isNamed)
				{

					if(ImGui::InputText("##InputName", m_entityPrevName, ENTITY_NAME_MAX_LENGTH,ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
					{
						//!: This is for deleting the spaces end of the array
						std::stringstream ss;
						ss.str(m_entityPrevName);
						std::string entityPrevNameWithoutSpaces;
						ss >> entityPrevNameWithoutSpaces;
						std::string finalString = entityPrevNameWithoutSpaces.substr(0, entityPrevNameWithoutSpaces.find_last_not_of(" ") + 1);
						GNF::Core::Game::GetInstance()->GetEntityManager()->ChangeNameOf(entity->m_entityId, finalString.c_str());
						
					}
				}
				else
				{
					ImGui::InputText("##InputName", m_entityPrevName, ENTITY_NAME_MAX_LENGTH,ImGuiInputTextFlags_ReadOnly);
				}
				
				
				ImGui::SameLine();

				ImGui::Checkbox("##Check1", &entity->m_isNamed);

				Common::Utils::ImGui_Header2(g_positionText);
				Common::Utils::ImGui_LeftLabeledInputFloat(g_x, &pos.x,g_rightAlign,g_gapBtw,"##1");
				ImGui::Spacing();
				Common::Utils::ImGui_LeftLabeledInputFloat(g_y, &pos.y,g_rightAlign, g_gapBtw,"##2");
				Common::Utils::ImGui_LeftLabeledInputFloat(g_z, &pos.z, g_rightAlign, g_gapBtw,"##3");
				Common::Utils::ImGui_Header2(g_scaleText);
				Common::Utils::ImGui_LeftLabeledInputFloat(g_x, &scale.x, g_rightAlign, g_gapBtw,"##4");
				Common::Utils::ImGui_LeftLabeledInputFloat(g_y, &scale.y, g_rightAlign, g_gapBtw,"##5");
				Common::Utils::ImGui_LeftLabeledInputFloat(g_z, &scale.z, g_rightAlign, g_gapBtw,"##6");
			
			}
			else
			{
				ImGui::Text("Select an entity");
			}
			ImGui::TreePop();
		}
	}

	void EntityNode::SetEntity(const std::weak_ptr<Entity::IEntity>& entity) noexcept
	{
		if (auto et = entity.lock())
		{
			m_entity = entity;
			memset(m_entityPrevName, 0, m_charUsage);
			std::copy(et->m_name.begin(), et->m_name.end(), m_entityPrevName);
			m_charUsage = et->m_name.size();
		}
	}

	void EntityNode::Init()
	{
		auto entityManager = Core::Game::GetInstance()->GetEntityManager();
		m_connectionEntityChanged = entityManager->AddSlotEntityChanged(boost::bind(&EntityNode::SetEntity,
			this, _1));
	}
}