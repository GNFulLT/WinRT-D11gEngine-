
#define IMGUI_IMPL_VULKAN_NO_PROTOTYPES 

#include "imgui_draw.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_vulkan.h"
#include "../../servers/window_server.h"
#include "../../platform/GLFW/window_server_glfw.h"



ImGuiDraw::ImGuiDraw()
{

}

void ImGuiDraw::init(RenderDeviceVulkan* device)
{



	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	
	dev = device->m_device;
	auto err = vkCreateDescriptorPool(device->m_device, &pool_info, nullptr, &imguiPool);
	

	ImGui_ImplVulkan_LoadFunctions([](const char* function_name, void* vulkan_instance) {
		return vkGetInstanceProcAddr(*(reinterpret_cast<VkInstance*>(vulkan_instance)), function_name);
		}, &device->m_instance);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsLight();

	auto r =ImGui_ImplGlfw_InitForVulkan(((WindowServerGLFW*)WindowServer::get_singleton())->get_glfw_handle(),true);
	
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = device->m_instance;
	init_info.PhysicalDevice = device->m_physicalDevice->m_physical_device;
	init_info.Device = device->m_device;
	init_info.QueueFamily = 0;
	init_info.Queue = device->m_surfaceQueue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = imguiPool;
	init_info.Subpass = 0;
	init_info.MinImageCount = device->swapChainImages.size() - 1 ;
	init_info.ImageCount = device->swapChainImages.size();
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = VK_NULL_HANDLE;
	init_info.CheckVkResultFn = VK_NULL_HANDLE;

	ImGui_ImplVulkan_Init(&init_info, device->m_render_pass);
	device->reset_cmd_pool();

	auto cmd = device->get_cmd_buffer();
	VkCommandBufferBeginInfo begin_info = {};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	err = vkBeginCommandBuffer(cmd, &begin_info);
	ImGui_ImplVulkan_CreateFontsTexture(cmd);


	VkSubmitInfo inf = {};
	inf.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	inf.commandBufferCount = 1;
	inf.pCommandBuffers = &cmd;
	err = vkEndCommandBuffer(cmd);

	auto res = vkQueueSubmit(device->m_surfaceQueue, 1, &inf,nullptr);

	vkDeviceWaitIdle(device->m_device);

	ImGui_ImplVulkan_DestroyFontUploadObjects();
	//vkDestroyDescriptorPool(device->m_device, imguiPool, nullptr);
	

	device->reset_cmd_pool();
}
void ImGuiDraw::begin()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiDraw::show_demo()
{
	ImGui::ShowDemoWindow();
}

void ImGuiDraw::end()
{
	// Rendering
	ImGui::Render();
}
void ImGuiDraw::fillCmd(VkCommandBuffer buff)
{
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), buff);
}
ImGuiDraw::~ImGuiDraw()
{
	vkDestroyDescriptorPool(dev, imguiPool, nullptr);
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}