#include "pch.h"
#include "EntityManager.h"
#include "Entitiy/Triangle2D.h"
#include "Common/Utils/utils_imgui.h"
#include <sstream>
#include "Common/Utils/utils_common.h"
#include "Core/Game.h"
#include "Renderer/ImGuiRenderer.h"
namespace GNF::Core
{
	constexpr static ImVec4 g_xBtnColor = ImVec4(0.2f,0.7f,0.3f,1.f);
	constexpr static ImVec4 g_xBtnColorHovered = ImVec4(0.3f, 0.8f, 0.2f, 1.f);
	constexpr static ImVec4 g_xBtnColorActive = ImVec4(0.2f, 0.7f, 0.2f, 1.f);

	constexpr static ImVec4 g_yBtnColor = ImVec4(220.f/255.f, 105.f/255.f, 29.f/255.f, 1.f);
	constexpr static ImVec4 g_yBtnColorHovered = ImVec4(230.f/255.f, 125.f/255.f, 23.f/255.f, 1.f);
	constexpr static ImVec4 g_yBtnColorActive = ImVec4(220.f / 255.f, 105.f / 255.f, 23.f / 255.f, 1.f);

	constexpr static ImVec4 g_zBtnColor = ImVec4(16.f / 255.f, 129.f / 255.f, 224.f / 255.f, 1.f);
	constexpr static ImVec4 g_zBtnColorHovered = ImVec4(30.f / 255.f, 135.f / 255.f, 240.f / 255.f, 1.f);
	constexpr static ImVec4 g_zBtnColorActive = ImVec4(16.f / 255.f, 129.f / 255.f, 240.f / 255.f, 1.f);

	static ImVec2 g_btnSize = ImVec2(20.f, 20.f);
	static ImVec2 g_inputSize = ImVec2(100.f, 20.f);

	EntityManager::~EntityManager()
	{
		int a = 5;
	}

	void EntityManager::Init()
	{
		Common::Logger::LogDebug("Entity Manager Init");
		m_transformIcon.reset(new GNF::GUI::Icon(L"Assets/transformIcon.png",Core::Game::GetInstance()->GetCurrentTextureManager(),32,32));
		m_objectIcon.reset(new GNF::GUI::Icon(L"Assets/objectIcon.png", Core::Game::GetInstance()->GetCurrentTextureManager(), 32, 32));
	}

	bool EntityManager::ChangeNameOf(Entity::EntityID id, const char* newName)
	{
		if (auto entity = m_map.find(id); entity != m_map.end())
		{
			if (m_nameMap.find(newName) != m_nameMap.end() || std::strlen(newName) == 0)
			{
				Common::Logger::LogWarn("There is already named entity with that name or name length = 0");
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

	void EntityManager::PreRender()
	{
		m_iconCenterY = m_transformIcon->GetHeight() / 4;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_iconCenterY);
		bool nodeOpen = ImGui::TreeNode("##HTransform");
		ImGui::SameLine();
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - m_iconCenterY);
		m_transformIcon->Draw();
		ImGui::SameLine();
		ImGui::Text("Transform");
		if (nodeOpen)
		{
			ImGui::TreePop();
		}
		//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(0.f,ImGui::GetStyle().FramePadding.y));
			//ImGui::GetStyle().Inn
		ImGui::Text("Position");
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_xBtnColor);
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_xBtnColorHovered);
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_xBtnColorActive);

		g_btnSize.x = ImGui::CalcTextSize("Position").y * 5 / 4;
		g_btnSize.y = g_btnSize.x;
		g_inputSize.y = g_btnSize.y;
		/*
		if (ImGui::Button("X", g_btnSize))
		{

		}
		*/
		ImGui::PopStyleColor(3);
		//ImGui::PopStyleVar();
	}

	void EntityManager::RenderSGui()
	{
		auto imguiManager = Core::Game::GetInstance()->GetEngineManager<Renderer::ImGuiRenderer>();
		if (!imguiManager)
			return;
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderHovered, ImVec4(89.f/255.f, 89.f / 255.f, 89.f / 255.f,1.f));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Header, ImVec4(92.f / 255.f, 95.f / 255.f, 96.f / 255.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderActive, ImVec4(89.f / 255.f, 89.f / 255.f, 96.f / 255.f, 1.f));

		if(ImGui::Begin("Scene",0,ImGuiWindowFlags_::ImGuiWindowFlags_NoResize))
		{
			for (auto& entity : m_map)
			{
				bool isSelected =  m_selectedEntity == nullptr ? false :  entity.second.first->m_entityId == m_selectedEntity->first->m_entityId;
				if (ImGui::Selectable(entity.second.first->GetName().c_str(),isSelected))
				{
					m_selectedEntity = &entity.second;
					memset(m_entityPrevName, 0, m_charUsage);
					std::copy(m_selectedEntity->first->m_name.begin(), m_selectedEntity->first->m_name.end(), m_entityPrevName);
					m_charUsage = m_selectedEntity->first->m_name.size();
					SelectedEntityChanged(m_selectedEntity->first);
				}
			}
		}
		ImGui::End();

		ImGui::PopStyleColor(3);

		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderHovered, ImVec4(0, 0, 0, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Header, ImVec4(0, 0, 0, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_HeaderActive, ImVec4(0, 0, 0, 0.4f));
		if (ImGui::Begin("Properties", 0, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize))
		{
			if (m_selectedEntity == nullptr)
			{
				ImGui::Text("First Select an entity");
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBg, ImVec4(25.f / 255.f, 29.f / 255.f, 40.f / 255.f, 1));
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgHovered, ImVec4(30.f / 255.f, 35.f / 255.f, 45.f / 255.f, 1));
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_FrameBgActive, ImVec4(25.f / 255.f, 29.f / 255.f, 45.f / 255.f, 1));

				ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_CellPadding, ImVec2(4, 18));
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 9);
#pragma region Header
				if (ImGui::BeginTable("##PosTable", 3))
				{
					ImGui::TableSetupColumn("##Icon_Col", ImGuiTableColumnFlags_::ImGuiTableColumnFlags_WidthFixed);
					ImGui::TableSetupColumn("##Input_Col");
					ImGui::TableSetupColumn("##Check_Col", ImGuiTableColumnFlags_::ImGuiTableColumnFlags_WidthFixed);

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);

					m_objectIcon->Draw();

					ImGui::TableSetColumnIndex(1);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_objectIcon->GetHeight() / 8);
					ImGui::PushItemWidth(-FLT_MIN);
					if (m_selectedEntity->first->m_isNamed)
					{
						if (ImGui::InputText("##InputName", m_entityPrevName, ENTITY_NAME_MAX_LENGTH, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
						{
							//!: This is for deleting the spaces end of the array
							std::string finalString = Common::Utils::DeleteLastSpaces(m_entityPrevName);
							ChangeNameOf(m_selectedEntity->first->m_entityId, finalString.c_str());

						}
					}
					else
					{
						ImGui::InputText("##InputName", m_entityPrevName, ENTITY_NAME_MAX_LENGTH, ImGuiInputTextFlags_ReadOnly);

					}
					ImGui::PopItemWidth();

					ImGui::TableSetColumnIndex(2);
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_objectIcon->GetHeight() / 8);
					ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding,ImVec2(2,2));
					ImGui::Checkbox("", &m_selectedEntity->first->m_isNamed);
					ImGui::PopStyleVar();
					ImGui::EndTable();
				}
				
				ImGui::PopStyleVar();
#pragma endregion
				
#pragma region Transform
				ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(4,8));

				bool nodeOpen = ImGui::TreeNodeEx("##HTransform", ImGuiTreeNodeFlags_FramePadding);
				ImGui::PopStyleVar();
				ImGui::SameLine();
				//ImGui::SetCursorPosY(ImGui::GetCursorPosY() + m_iconCenterY/4);
				m_transformIcon->Draw();
				//ImGui::SetCursorPosY(ImGui::GetCursorPosY() - m_iconCenterY/4);
				ImGui::SameLine();
				imguiManager->SetFont(Renderer::FONT_ENTITY_HEADER);
				ImGui::Text("Transform");
				imguiManager->SetFontDefault();
				
				
				if(nodeOpen)
				{
					auto fullWidth = ImGui::GetContentRegionAvail().x;
						
					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().IndentSpacing);
					auto leftS = ImGui::GetCursorPosX();				
					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

					//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(0.f,ImGui::GetStyle().FramePadding.y));
					//ImGui::GetStyle().Inn
					ImGui::Text("Position");
					ImGui::SameLine();
					
		
					ImGui::SetCursorPosX(leftS + fullWidth - g_inputSize.x *5/ 8 - g_btnSize.x);
					
					if(ImGui::BeginTable("##PosTable", 2, ImGuiTableFlags_::ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_::ImGuiTableFlags_NoPadOuterX))
					{
						
						ImGui::TableSetupColumn("##Btn_Col", ImGuiTableColumnFlags_::ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("##Input_Col");

						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_xBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_xBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_xBtnColorActive);

						if (ImGui::Button("X", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosX", &m_selectedEntity->first->GetPositionChangable().x);
						ImGui::PopItemWidth();

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_yBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_yBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_yBtnColorActive);
						if (ImGui::Button("Y", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosY", &m_selectedEntity->first->GetPositionChangable().y);
						ImGui::PopItemWidth();

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_zBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_zBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_zBtnColorActive);
						if (ImGui::Button("Z", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosZ", &m_selectedEntity->first->GetPositionChangable().z);
						ImGui::PopItemWidth();

						ImGui::EndTable();
					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().IndentSpacing);

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);


					ImGui::Text("Scale");
					ImGui::SameLine();


					ImGui::SetCursorPosX(leftS + fullWidth - g_inputSize.x * 5 / 8 - g_btnSize.x);

					if (ImGui::BeginTable("##ScaleTable", 2, ImGuiTableFlags_::ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_::ImGuiTableFlags_NoPadOuterX))
					{

						ImGui::TableSetupColumn("##Btn_Col", ImGuiTableColumnFlags_::ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("##Input_Col");

						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_xBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_xBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_xBtnColorActive);

						if (ImGui::Button("X", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosX", &m_selectedEntity->first->GetScaleChangable().x);
						ImGui::PopItemWidth();

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_yBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_yBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_yBtnColorActive);
						if (ImGui::Button("Y", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosY", &m_selectedEntity->first->GetScaleChangable().y);
						ImGui::PopItemWidth();

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_zBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_zBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_zBtnColorActive);
						if (ImGui::Button("Z", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosZ", &m_selectedEntity->first->GetScaleChangable().z);
						ImGui::PopItemWidth();

						ImGui::EndTable();
					}

					ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ImGui::GetStyle().IndentSpacing);

					ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);


					ImGui::Text("Rotation");
					ImGui::SameLine();


					ImGui::SetCursorPosX(leftS + fullWidth - g_inputSize.x * 5 / 8 - g_btnSize.x);

					if (ImGui::BeginTable("##RotateTable", 2, ImGuiTableFlags_::ImGuiTableFlags_NoPadInnerX | ImGuiTableFlags_::ImGuiTableFlags_NoPadOuterX))
					{

						ImGui::TableSetupColumn("##Btn_Col", ImGuiTableColumnFlags_::ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("##Input_Col");

						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_xBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_xBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_xBtnColorActive);

						if (ImGui::Button("X", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosX", &m_selectedEntity->first->GetRotationChangable().x);
						ImGui::PopItemWidth();

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_yBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_yBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_yBtnColorActive);
						if (ImGui::Button("Y", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosY", &m_selectedEntity->first->GetRotationChangable().y);
						ImGui::PopItemWidth();

						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0);

						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Button, g_zBtnColor);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonHovered, g_zBtnColorHovered);
						ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_ButtonActive, g_zBtnColorActive);
						if (ImGui::Button("Z", ImVec2(20, 0.0f)))
						{

						}

						ImGui::PopStyleColor(3);

						ImGui::TableSetColumnIndex(1);
						ImGui::PushItemWidth(-FLT_MIN);
						ImGui::InputFloat("##InputPosZ", &m_selectedEntity->first->GetRotationChangable().z);
						ImGui::PopItemWidth();

						ImGui::EndTable();
					}
					//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					//ImGui::SameLine();

					//ImGui::PushStyleVar(ImGuiStyleVar_::ImGuiStyleVar_FramePadding, ImVec2(g_inputSize.x/4, g_inputSize.y/8));
					
					//ImGui::InputFloat("##InputPosX", &m_selectedEntity->first->GetPositionChangable().x);
					//ImGui::PopStyleVar();
					ImGui::TreePop();
				}

				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Separator();

#pragma endregion
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20.f);
				ImGui::PopStyleColor(3);

			}

		}
		ImGui::End();
		ImGui::PopStyleColor(3);
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