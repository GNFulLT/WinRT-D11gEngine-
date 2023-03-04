#ifndef RENDER_DEVICE_1_H
#define RENDER_DEVICE_1_H

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_glfw.h"
#include "../../imgui/imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "../../core/typedefs.h"
#include <vector>
class RenderDevice_1
{
public:
    struct VulkanSync final
    {
        VkSemaphore semaphore_image_acquired;
        VkSemaphore semaphore_render_finished;
        VkFence fence_queue_submited;
    };

    struct VulkanSwapchain final
    {
        VkSurfaceKHR surface = nullptr;
        VkSurfaceFormatKHR  surface_format;
        VkPresentModeKHR present_mode;
        VkSwapchainKHR swapchain = nullptr;
        VkRenderPass render_pass;

        std::vector<VkImage> images;
        std::vector<VkImageView> image_views;
        std::vector<VkFramebuffer> frame_buffers;
        
        VkClearValue clear_value
        {
           { { 0.0f, 0.0f, 0.0f, 0.0f } }
        };

        uint32_t current_frame_index = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        uint32_t min_image_count = 0;
        uint32_t image_count = 0;
    };



    ~RenderDevice_1();
    void init(GLFWwindow* window);

    inline void beginW()
    {
        ImGui_ImplGlfw_NewFrame();
    }

    void ready_ui_data();

    void fill_command_buff();

    void reset_things();

    void set_next_image();

    void init_imgui();


    void make_frame();
private:
    _INLINE_ VkPresentInfoKHR* get_present_info()
    {
        static VkPresentInfoKHR info = {
            VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            nullptr,
            1,
            &g_sync.semaphore_render_finished,
            1,
            &g_swapchain.swapchain,
            &g_swapchain.current_frame_index,
            nullptr
        };
        
        return &info;
    }

    _INLINE_ int get_min_image_count(VkPresentModeKHR present_mode)
    {
        if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
            return 3;
        if (present_mode == VK_PRESENT_MODE_FIFO_KHR || present_mode == VK_PRESENT_MODE_FIFO_RELAXED_KHR)
            return 2;
        if (present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            return 1;
        return 1;
    }
    void create_swap_chain(int w,int h);
    void create_sync();
    void rebuild_swapchain();

    VulkanSync               g_sync;
    VkAllocationCallbacks*   g_Allocator = NULL;
    VkInstance               g_Instance = VK_NULL_HANDLE;
    VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice                 g_Device = VK_NULL_HANDLE;
    uint32_t                 g_QueueFamily = (uint32_t)-1;
    VkQueue                  g_Queue = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
    VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;
    VulkanSwapchain          g_swapchain;

    VkCommandPool            g_mainCommandPool;
    VkCommandBuffer          g_mainCommandBuffer;

    //ImGui_ImplVulkanH_Window g_MainWindowData;
    int                      g_MinImageCount = 2;
    bool                     g_SwapChainRebuild = false;
    GLFWwindow* window;
};

#endif // RENDER_DEVICE_H
