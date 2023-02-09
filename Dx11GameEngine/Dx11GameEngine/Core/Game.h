#pragma once

#include <memory>
#include <stack>
#include "State/IState.h"
#include "../Common/Windowing/IWindow.h"
#include "GraphicEngine/GraphicEngine.h"
#include "Bindable/Buffer/VertexBufferBindable.h"
#include "Common/Statistic/Statistic.h"
#include "Common/Camera/Camera.h"
#include "Renderer/ImGuiRenderer.h"
#include "GUI/MenuBar.h"
#include "Scene/EntityManager.h"
#include "TextureManager.h"
#include "imgui/imgui.h"
#include "Scene/Scene.h"

//X: Change this
#include "GUI/EntityNode.h"

namespace GNF::Core
{
	struct Layout
	{
		float topAlignment = 0.7f;
		float bottomAlignment = 0.7f;
		float leftAlignment = 0.2f;
		float rightAlignment = 0.2f;
		float rightDownAlignment = 0.3f;
	};

	enum SwapState
	{
		Swap = 0,NeedResize
	};

	typedef boost::signals2::signal<void(int width,int height,bool isFullSize)> SizeChangedSignal;

	class Game 
	{
		//!: Statics
	public:
		~Game();
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;

		void ValidateLayout(ImGuiID id);
		
		inline static Game* GetInstance()
		{
			return g_instance.get();
		}

		inline auto AddWindowSizeChanged(const SizeChangedSignal::slot_type& slot) const noexcept
		{
			return m_sizeChangedSignal.connect(slot);
		}

		static Game* Build();
		static Game* Build(const char* gameName);

	/*	inline EntityManager* GetEntityManager() const
		{
			return m_entityManager.get();
		}*/

		inline TextureManager* GetTextureManager() const noexcept
		{
			return m_textureManager.get();
		}

		inline GNF::Common::Windowing::IWindow* GetWindow() const
		{
			return m_window.get();
		}
		inline GNF::Common::Camera::Camera* GetCamera() const
		{
			return m_camera.get();
		}
		void Init();
		void Run();
		void Destroy();
		//void CalculateFrameScreenBounds();
		void Render();
		
		Common::Windowing::Keyboard::IKeyboard* GetKeyboard();
		Common::Windowing::Mouse::IMouse* GetMouse();
		
		inline Core::GraphicEngine::GraphicEngine* GetGraphicEngine()
		{
			return &engine;
		}

		std::weak_ptr<Renderer::ImGuiRenderer> GetImGuiRenderer();

		void RenderSGui();
		void FrameSizeChanged();
	private:
		Game(const char* gameName);

		//!: Render statement for just once
		void PreRender();
	private:
		void OnWindowCreated(HWND hwnd, UINT width, UINT height,bool isFullScreen);
		void OnWindowResized(HWND hwnd, UINT width, UINT height, bool isFullScreen,UINT dpi);
	private:
		int m_fps = 0;
		float m_deltaTime = 0;
		std::stack<std::unique_ptr<State::IState>> m_states;
		std::unique_ptr<Common::Windowing::IWindow> m_window;
		GraphicEngine::GraphicEngine engine;
		
		std::unique_ptr<Bindable::Shader::PixelShaderBindable> m_pixelShader;
		std::unique_ptr<Bindable::Shader::VertexShaderBindable> m_vertexShader;
		
		
		//std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<TextureManager> m_textureManager;
		std::unique_ptr<GUI::MenuBar> m_menuBar;

		std::unique_ptr<Common::Statistic::Statistic> m_statistic;
		std::unique_ptr<Common::Camera::CameraPositioner::ICameraPositioner> m_fpsCameraPositioner;
		std::unique_ptr<Common::Camera::Camera> m_camera;
		std::shared_ptr<Renderer::ImGuiRenderer> m_imgui;
		std::unique_ptr<Scene::Scene> m_scene;
		//std::shared_ptr<Entity::IEntity> m_triangle;
		//std::shared_ptr<Entity::IEntity> m_triangle1;

		ImVec2 m_frameSize;
		Layout m_layout;
		bool m_layoutNeedValidate = true;
		bool m_newFrameSizeCalculated = false;
		bool m_newFrameSizeShouldBeCalculated = false;
		mutable SizeChangedSignal m_sizeChangedSignal;
		bool m_isFirstPreRender = false;
		bool m_waitForFrame = false;
		SwapState m_swapState = SwapState::Swap;
	private:
		inline static std::unique_ptr<Game> g_instance;
	private:
		friend class GraphicEngine::GraphicEngine;
		friend class  GNF::Common::Camera::Camera;

		//!: FOR IM GUI
	private:
		std::string m_currentCamera;
		int m_currentCameraIndex = 0;
	};
}