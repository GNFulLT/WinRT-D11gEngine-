#pragma once

#include <windows.h>
#include <boost/signals2.hpp>
namespace GNF::GUI
{
	class IFixedWindowChild
	{
	public:
		IFixedWindowChild(const char* name, int top,int left,float widthP,float heightP);
		~IFixedWindowChild() 
		{
			int a = 5;
		};
		void Init();
		void RenderSGui();
		void OnWindowSizeChanged(int newX,int newY,bool isFullScreen);
	private:
		const char* m_name;
		int m_top = 0;
		int m_left = 0;
		float m_widthP = 0.1f;
		float m_heightP = 0.1f;
		int m_width = 0;
		int m_height = 0;

		boost::signals2::connection sizeChangedConnection;
	};
}