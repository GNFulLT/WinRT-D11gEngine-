#ifndef WINDOW_H
#define WINDOW_H

#include "viewport.h"
#include "window_id.h"

class Window : public Viewport
{
	OBJECT_DEF(Window,Viewport)
public:

	Window() : Window("GNF Window")
	{}
	
	Window(const String& windowId) : m_windowName(windowId)
	{}

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	_INLINE_ const String& get_window_name() const noexcept
	{
		return m_windowName;
	}

	_INLINE_ void set_window_name(const String& windowName)
	{
		m_windowName = windowName;
	}

private:
	WindowID m_id;
	String m_windowName;
};

#endif // WINDOW_H