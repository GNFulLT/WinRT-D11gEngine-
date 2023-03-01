#ifndef WINDOW_SERVER_H
#define WINDOW_SERVER_H

#include "../window/window.h"
#include "../config/config_utils.h"
#include "../graphic/graphic_api.h"
#include <GLFW/glfw3.h>

class CreationServer;


class WindowServer : public Window
{
	OBJECT_DEF(WindowServer,Window)

public:
	~WindowServer()
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}

	WindowServer() : Window()
	{
		assert(false);
	}

	WindowServer(const String& windowId) : Window(windowId),
		m_monitorSelection(new ConfigProp<MONITOR_SELECTION>(MONITOR_SELECTION_PRIMARY)),
		m_windowMode(new ConfigProp<WINDOW_MODE>(WINDOW_MODE_WINDOWED)),
		m_windowInitialPosition(new ConfigProp<WINDOW_INITIAL_POSITION>(WINDOW_INITIAL_POSITION_CENTER))
	{
		// Default Window Size is 640 480
		//X TODO : IT SHOULD BE WORKING SIZE OF MONITOR

		m_size.reset(new ConfigProp<UVec2>({640,480}));
	}
	
	enum MONITOR_SELECTION
	{
		MONITOR_SELECTION_PRIMARY
	};

	enum WINDOW_MODE
	{
		WINDOW_MODE_WINDOWED = 0,
		WINDOW_MODE_MINIMIZED,
		WINDOW_MODE_MAXIMIZED,
		WINDOW_MODE_FULLSCREEN
	};

	enum WINDOW_INITIAL_POSITION {
		WINDOW_INITIAL_POSITION_CENTER = 0
	};

	bool init();
	_INLINE_ void handle_events()
	{
		glfwPollEvents();
	}

	_INLINE_ bool should_close() const
	{
		assert(m_window != nullptr);

		return glfwWindowShouldClose(m_window);
	}

	_INLINE_ void show()
	{
		assert(m_window != nullptr);


		glfwShowWindow(m_window);
	}

	_INLINE_ void hide()
	{
		assert(m_window != nullptr);

		glfwHideWindow(m_window);
		m_windowMode->set_prop(WINDOW_MODE_MINIMIZED);
	}
	// It returns by pixel
	
	_INLINE_ void get_framebuffer_size(int* width,int* height) const noexcept
	{
		glfwGetFramebufferSize(m_window, width, height);
	}

	// Could be overrided
	void destroy();
	_INLINE_ static WindowServer* get_singleton()
	{
		return singleton;
	}

	_INLINE_ GLFWwindow* get_window()
	{
		return m_window;
	}

	//X TODO : Initial position should be in InitialConfigProp class
protected:
	std::shared_ptr<ConfigProp<MONITOR_SELECTION>> m_monitorSelection;
	std::shared_ptr<ConfigProp<WINDOW_MODE>> m_windowMode;
	std::shared_ptr<ConfigProp<WINDOW_INITIAL_POSITION>> m_windowInitialPosition;

private:
	GLFWwindow* m_window = nullptr;
	GLFWmonitor* m_monitor = nullptr;
protected:
	virtual Config* config_creation() override;
protected:
	friend class CreationServer;

	static WindowServer* create_singleton();

	_INLINE_ static WindowServer* singleton;
};


#endif // WINDOW_SERVER_H
