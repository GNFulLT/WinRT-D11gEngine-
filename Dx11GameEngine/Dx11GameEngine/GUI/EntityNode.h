#pragma once

#include "Entitiy/IEntity.h"
#include <memory>
#include <boost/signals2.hpp>
namespace GNF::GUI
{
	class EntityNode
	{
	public:
		EntityNode();
		void RenderSGui();
		void Init();

		void SetEntity(const std::weak_ptr<Entity::IEntity>& entity) noexcept;

	private:
		std::weak_ptr<Entity::IEntity> m_entity;
		char m_entityPrevName[ENTITY_NAME_MAX_LENGTH] = { 0 };
		uint_fast8_t m_charUsage = 0;
		boost::signals2::connection m_connectionEntityChanged;
	};
}