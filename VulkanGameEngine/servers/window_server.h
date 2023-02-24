#ifndef WINDOW_SERVER_H
#define WINDOW_SERVER_H

#include "../window/window.h"
#include "../config/config_utils.h"
#include "../graphic/graphic_api.h"

class CreationServer;


class WindowServer : public Window
{
	OBJECT_DEF(WindowServer,Window)

public:
	virtual ~WindowServer() = default;

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
		m_size.reset(new ConfigProp<UVec2>({640,480}));
	}
	
	enum MONITOR_SELECTION
	{
		MONITOR_SELECTION_PRIMARY
	};

	enum WINDOW_SUPPORTER
	{
		WINDOW_SUPPORTER_GLFW = 0
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

	virtual bool init() = 0;
	virtual void handle_events() = 0;

	virtual GRAPHIC_API get_default_graphic_api() const noexcept = 0;
	virtual bool is_graphic_api_supported(GRAPHIC_API api) const noexcept= 0;

	virtual WINDOW_SUPPORTER get_window_supporter() const noexcept = 0;

	virtual bool should_close() = 0;

	virtual void show() = 0;

	virtual void hide() = 0;

	// Could be overrided
	virtual void destroy();
	_INLINE_ static WindowServer* get_singleton()
	{
		return singleton;
	}
	//X TODO : Initial position should be in InitialConfigProp class
protected:
	std::shared_ptr<ConfigProp<MONITOR_SELECTION>> m_monitorSelection;
	std::shared_ptr<ConfigProp<WINDOW_MODE>> m_windowMode;
	std::shared_ptr<ConfigProp<WINDOW_INITIAL_POSITION>> m_windowInitialPosition;

protected:
	virtual Config* config_creation() override;
protected:
	friend class CreationServer;


	_INLINE_ static WindowServer* singleton;
};


#endif // WINDOW_SERVER_H
