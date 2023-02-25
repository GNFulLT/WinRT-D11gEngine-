#ifndef WINDOW_SERVER_GLFW_H
#define WINDOW_SERVER_GLFW_H


#include "../../servers/window_server.h"
#include <GLFW/glfw3.h>

class WindowServerGLFW : public WindowServer
{
public:
	OBJECT_DEF(WindowServerGLFW, WindowServer)

	WindowServerGLFW() : WindowServer()
	{
		assert(false);
	}

	WindowServerGLFW(const String& windowId) : WindowServer(windowId)
	{
	}

	~WindowServerGLFW();
	virtual void handle_events() override;
	virtual bool init() override;

	virtual bool should_close() override;

	virtual void show() override;
	virtual void hide() override;

	virtual WINDOW_SUPPORTER get_window_supporter() const noexcept override;
	virtual void* get_native_handle() override;
	virtual GRAPHIC_API get_default_graphic_api() const noexcept override;
	virtual bool is_graphic_api_supported(GRAPHIC_API api) const noexcept override;
protected:

private:
	 friend class CreationServer;
	 GLFWwindow* m_window = nullptr;
 
};

#endif // WINDOW_SERVER_GLFW_H