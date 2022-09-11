#pragma once

#include <map>
#include "Window/WindowDesc.h"
#include "Window/IWindow32.h"
#include "Common/Timer/ITimerSystem.h"
#include "Window/MainWindow.h"
#include <memory>
#include "CameraSystem/CameraEntity.h"
#include "EntitySystem/IDrawableEntity.h"
namespace GNF::Game
{
	class GameMain
	{
		public:
			float GetFrameTime();
			static GameMain* BuildGame();
			static GameMain* BuildGame(const Window::WindowDesc&);
			void CalculateFPS();


			//! Create with new not unique ptr it will capsulate with unique ptr
			Entity::IEntity* CreateEntity(Entity::IEntity* entity)
			{
				m_entities.emplace_back(entity);

				if (auto drawable = entity->As<Entity::IDrawableEntity>())
				{
					m_drawables.push_back(drawable);
				}

				return entity;
			}

			Window::MainWindow* GetMainWindow()
			{
				return (Window::MainWindow*)pMainWindow.get();
			}

			static GameMain* GetInstance()
			{
				return g_gameMain.get();
			}
			int Run();
			
			//If there is no system with that type it will throw exception
			template<class T>
			T* GetSystem()
			{
				const type_info& typeInf = typeid(T);
				size_t hashCode = typeInf.hash_code();

				if (m_systemStorage.find(hashCode) == m_systemStorage.end())
					throw std::exception("There is no system like that");

				return (T*) m_systemStorage[hashCode].get();

			}

			//If there is no system with that return null
			template<class T>
			void TryGetSystem(T** system)
			{
				const type_info& typeInf = typeid(T);
				size_t hashCode = typeInf.hash_code();

				if (m_systemStorage.find(hashCode) == m_systemStorage.end())
				{
					system == nullptr;
					return;
				}
				
				(*system) = (Timer::ITimerSystem*)(m_systemStorage[hashCode].get());
				return;
			}

			Window::IWindow32* GetNativeWindow();

		private:
			std::map<size_t, std::unique_ptr<Common::ISystem>> m_systemStorage;

			Camera::CameraEntity* m_cameraEntity;

			std::vector<std::unique_ptr<Entity::IEntity>> m_entities;
			std::vector < Entity::IDrawableEntity*> m_drawables;
			GameMain* WithDefaultSystems();
			GameMain(const Window::WindowDesc&);
			inline static std::unique_ptr<GameMain> g_gameMain;
			float fps = 120;
			//! Members
		private:
			std::unique_ptr<Window::IWindow32> pMainWindow;
	};
}