#ifndef WINDOW_SERVER_GLFW_H
#define WINDOW_SERVER_GLFW_H


#include "../../servers/window_server.h"
#include <GLFW/glfw3.h>

class WindowServerGLFW : public WindowServer
{
public:
	OBJECT_DEF(WindowServerGLFW, WindowServer)

	WindowServerGLFW() : WindowServer()
	{}

	WindowServerGLFW(const String& windowId) : WindowServer(windowId)
	{}

	virtual void handle_events() override;

	virtual WINDOW_SUPPORTER get_window_supporter() const noexcept override
	{
		return WINDOW_SUPPORTER_GLFW;
	}

 private:

};

#endif // WINDOW_SERVER_GLFW_H