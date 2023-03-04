#ifndef IMGUI_DRAW_H
#define IMGUI_DRAW_H


#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class ImGuiDraw
{
public:
	ImGuiDraw();
	~ImGuiDraw();
	void init();

	void begin();
	void end();

	void fillCmd(VkCommandBuffer buff);

	void show_demo();
private:
	VkDevice dev;
	VkDescriptorPool imguiPool;
};


#endif // IMGUI_DRAW_H