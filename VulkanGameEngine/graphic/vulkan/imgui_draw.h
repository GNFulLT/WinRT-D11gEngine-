#ifndef IMGUI_DRAW_H
#define IMGUI_DRAW_H

#include "render_device_vulkan.h"

class ImGuiDraw
{
public:
	ImGuiDraw();
	~ImGuiDraw();
	void init(RenderDeviceVulkan* device);

	void begin();
	void end();
	
	void fillCmd(VkCommandBuffer buff);

	void show_demo();
private:
	VkDevice dev;
	VkDescriptorPool imguiPool;
};


#endif // IMGUI_DRAW_H