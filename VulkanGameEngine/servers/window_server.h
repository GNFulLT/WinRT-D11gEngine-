#ifndef WINDOW_SERVER_H
#define WINDOW_SERVER_H

#include "../window/window.h"
#include "configuration_server.h"

class WindowServer : public Window
{
	OBJECT_DEF(WindowServer,Window)

public:
	WindowServer() : Window()
	{
				
	}

	WindowServer(const String& windowId) : Window(windowId)
	{

	}

	enum WINDOW_SUPPORTER
	{
		WINDOW_SUPPORTER_GLFW = 0;
	};

	enum WINDOW_MODE
	{
		WINDOW_MODE_WINDOWED = 0,
		WINDOW_MODE_MINIMZED,
		WINDOW_MODE_MAXIMIZED,
		WINDOW_MODE_FULLSCREEN
	};

	enum WINDOW_INITIAL_POSITION {
		WINDOW_INITIAL_POSITION_CENTER = 0
	}


	virtual void handle_events() = 0;
	virtual WINDOW_SUPPORTER get_window_supporter() const noexcept = 0;
private:
	
};


#endif // WINDOW_SERVER_H
