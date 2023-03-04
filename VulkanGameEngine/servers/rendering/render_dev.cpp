#include "render_dev.h"
#include "render_dev.h"

void RenderDevice_1::init_imgui()
{
    //ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = g_Instance;
    init_info.PhysicalDevice = g_PhysicalDevice;
    init_info.Device = g_Device;
    init_info.QueueFamily = g_QueueFamily;
    init_info.Queue = g_Queue;
    init_info.PipelineCache = g_PipelineCache;
    init_info.DescriptorPool = g_DescriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = g_MinImageCount;
    init_info.ImageCount = g_swapchain.image_count;
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = g_Allocator;
    ImGui_ImplVulkan_Init(&init_info, g_swapchain.render_pass);

    {
        // Use any command queue
        VkCommandPool command_pool =    g_mainCommandPool;
        VkCommandBuffer command_buffer = g_mainCommandBuffer;

        auto err = vkResetCommandPool(g_Device, command_pool, 0);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);

        ImGui_ImplVulkan_CreateFontsTexture(command_buffer);

        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        err = vkEndCommandBuffer(command_buffer);
        err = vkQueueSubmit(g_Queue, 1, &end_info, VK_NULL_HANDLE);

        err = vkDeviceWaitIdle(g_Device);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
}

RenderDevice_1::~RenderDevice_1()
{

    if (g_Instance)
    {
        if (g_Device)
        {
            for (int i = 0; i < g_swapchain.image_count; i++)
            {
                vkDestroyFramebuffer(g_Device, g_swapchain.frame_buffers[i], g_Allocator);
                vkDestroyImageView(g_Device, g_swapchain.image_views[i], g_Allocator);
            }

            ImGui_ImplVulkan_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            //ImGui_ImplVulkanH_DestroyWindow(g_Instance, g_Device, &g_MainWindowData, g_Allocator);

            vkDestroyDescriptorPool(g_Device, g_DescriptorPool, g_Allocator);

            vkDestroyDevice(g_Device,g_Allocator);
        }


#ifdef IMGUI_VULKAN_DEBUG_REPORT
        // Remove the debug report callback
        auto vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkDestroyDebugReportCallbackEXT");
        vkDestroyDebugReportCallbackEXT(g_Instance, g_DebugReport, g_Allocator);
#endif // IMGUI_VULKAN_DEBUG_REPORT

        vkDestroyInstance(g_Instance,g_Allocator);
    }
}

void RenderDevice_1::init(GLFWwindow* window)
{
    glfwVulkanSupported();

    this->window = window;
    uint32_t extensions_count = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
    VkResult err;

    // Create Vulkan Instance
    {
        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        create_info.enabledExtensionCount = extensions_count;
        create_info.ppEnabledExtensionNames = extensions;
#ifdef IMGUI_VULKAN_DEBUG_REPORT
        // Enabling validation layers
        const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
        create_info.enabledLayerCount = 1;
        create_info.ppEnabledLayerNames = layers;

        // Enable debug report extension (we need additional storage, so we duplicate the user array to add our new extension to it)
        const char** extensions_ext = (const char**)malloc(sizeof(const char*) * (extensions_count + 1));
        memcpy(extensions_ext, extensions, extensions_count * sizeof(const char*));
        extensions_ext[extensions_count] = "VK_EXT_debug_report";
        create_info.enabledExtensionCount = extensions_count + 1;
        create_info.ppEnabledExtensionNames = extensions_ext;

        // Create Vulkan Instance
        err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);
        check_vk_result(err);
        free(extensions_ext);

        // Get the function pointer (required for any extensions)
        auto vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(g_Instance, "vkCreateDebugReportCallbackEXT");
        IM_ASSERT(vkCreateDebugReportCallbackEXT != NULL);

        // Setup the debug report callback
        VkDebugReportCallbackCreateInfoEXT debug_report_ci = {};
        debug_report_ci.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
        debug_report_ci.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
        debug_report_ci.pfnCallback = debug_report;
        debug_report_ci.pUserData = NULL;
        err = vkCreateDebugReportCallbackEXT(g_Instance, &debug_report_ci, g_Allocator, &g_DebugReport);
        check_vk_result(err);
#else
        // Create Vulkan Instance without any debug feature
        err = vkCreateInstance(&create_info, g_Allocator, &g_Instance);

        IM_UNUSED(g_DebugReport);
#endif
    }

    // Select GPU
    {
        uint32_t gpu_count;
        err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, NULL);

        VkPhysicalDevice* gpus = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpu_count);
        err = vkEnumeratePhysicalDevices(g_Instance, &gpu_count, gpus);

        // If a number >1 of GPUs got reported, find discrete GPU if present, or use first one available. This covers
        // most common cases (multi-gpu/integrated+dedicated graphics). Handling more complicated setups (multiple
        // dedicated GPUs) is out of scope of this sample.
        int use_gpu = 0;
        for (int i = 0; i < (int)gpu_count; i++)
        {
            VkPhysicalDeviceProperties properties;
            vkGetPhysicalDeviceProperties(gpus[i], &properties);
            if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                use_gpu = i;
                break;
            }
        }

        g_PhysicalDevice = gpus[use_gpu];
        free(gpus);
    }

    // Select graphics queue family
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, NULL);
        VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
        vkGetPhysicalDeviceQueueFamilyProperties(g_PhysicalDevice, &count, queues);
        for (uint32_t i = 0; i < count; i++)
            if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                g_QueueFamily = i;
                break;
            }
        free(queues);
    }

    // Create Logical Device (with 1 queue)
    {
        int device_extension_count = 1;
        const char* device_extensions[] = { "VK_KHR_swapchain" };
        const float queue_priority[] = { 1.0f };
        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = g_QueueFamily;
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = queue_priority;
        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = device_extension_count;
        create_info.ppEnabledExtensionNames = device_extensions;
        err = vkCreateDevice(g_PhysicalDevice, &create_info, g_Allocator, &g_Device);
        vkGetDeviceQueue(g_Device, g_QueueFamily, 0, &g_Queue);
    }

    // Create Descriptor Pool
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
        err = vkCreateDescriptorPool(g_Device, &pool_info, g_Allocator, &g_DescriptorPool);
    }

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);


    // Create Window Surface
    VkSurfaceKHR surface;
    err = glfwCreateWindowSurface(g_Instance, window, g_Allocator, &surface);

    //ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;


    g_swapchain.surface = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(g_PhysicalDevice, g_QueueFamily, g_swapchain.surface, &res);
    if (res != VK_TRUE)
    {
        fprintf(stderr, "Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

    g_swapchain.surface_format = ImGui_ImplVulkanH_SelectSurfaceFormat(g_PhysicalDevice, g_swapchain.surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    // Select Present Mode
#ifdef IMGUI_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
    g_swapchain.present_mode = ImGui_ImplVulkanH_SelectPresentMode(g_PhysicalDevice, g_swapchain.surface, &present_modes[0], IM_ARRAYSIZE(present_modes));
    //printf("[vulkan] Selected PresentMode = %d\n", wd->PresentMode);

    // Create SwapChain, RenderPass, Framebuffer, etc.
    //ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, wd, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
    create_swap_chain(width, height);
    create_sync();
    init_imgui();
}

void RenderDevice_1::create_sync()
{
    {
        VkFenceCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        vkCreateFence(g_Device, &info, g_Allocator, &g_sync.fence_queue_submited);
    }
    {   
        VkSemaphoreCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(g_Device, &info, g_Allocator, &g_sync.semaphore_image_acquired);
        vkCreateSemaphore(g_Device, &info, g_Allocator, &g_sync.semaphore_render_finished);
    }
    {
        VkCommandPoolCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        info.queueFamilyIndex = g_QueueFamily;
        vkCreateCommandPool(g_Device, &info, g_Allocator, &g_mainCommandPool);
    }
    {
        VkCommandBufferAllocateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        info.commandPool = g_mainCommandPool;
        info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        info.commandBufferCount = 1;
        vkAllocateCommandBuffers(g_Device, &info, &g_mainCommandBuffer);
        
    }
}

void RenderDevice_1::create_swap_chain(int w, int h)
{
    {
        g_swapchain.min_image_count = get_min_image_count(g_swapchain.present_mode);

        VkSwapchainCreateInfoKHR info = {};
        info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        info.surface = g_swapchain.surface;
        info.minImageCount = g_swapchain.min_image_count;
        info.imageFormat = g_swapchain.surface_format.format;
        info.imageColorSpace = g_swapchain.surface_format.colorSpace;
        info.imageArrayLayers = 1;
        info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;           // graphics family == present family
        info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        info.presentMode = g_swapchain.present_mode;
        info.clipped = VK_TRUE;
        info.oldSwapchain = VK_NULL_HANDLE;
        VkSurfaceCapabilitiesKHR cap;
        auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(g_PhysicalDevice, g_swapchain.surface, &cap);

        // X TODO: THROW EXCEPTION
        if (info.minImageCount < cap.minImageCount)
            info.minImageCount = cap.minImageCount;
        else if (cap.maxImageCount != 0 && info.minImageCount > cap.maxImageCount)
            info.minImageCount = cap.maxImageCount;
        if (cap.currentExtent.width == 0xffffffff)
        {
            info.imageExtent.width = g_swapchain.width = w;
            info.imageExtent.height = g_swapchain.height = h;
        }
        else
        {
            info.imageExtent.width = g_swapchain.width = cap.currentExtent.width;
            info.imageExtent.height = g_swapchain.height = cap.currentExtent.height;
        }

        err = vkCreateSwapchainKHR(g_Device, &info, g_Allocator, &g_swapchain.swapchain);

        err = vkGetSwapchainImagesKHR(g_Device, g_swapchain.swapchain, &g_swapchain.image_count, nullptr);
        g_swapchain.images = std::vector<VkImage>(g_swapchain.image_count);
        err = vkGetSwapchainImagesKHR(g_Device, g_swapchain.swapchain, &g_swapchain.image_count, g_swapchain.images.data());

    }
    // RENDER PASS
    {
    VkAttachmentDescription attachment = {};
    attachment.format = g_swapchain.surface_format.format;
    attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;   // Maybe dont care
    attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    VkAttachmentReference color_attachment = {};
    color_attachment.attachment = 0;
    color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;
    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    VkRenderPassCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.attachmentCount = 1;
    info.pAttachments = &attachment;
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 1;
    info.pDependencies = &dependency;
    auto err = vkCreateRenderPass(g_Device, &info, g_Allocator, &g_swapchain.render_pass);
    }


    // Create The Image Views
    {
        g_swapchain.image_views = std::vector<VkImageView>(g_swapchain.image_count);

        VkImageViewCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.format = g_swapchain.surface_format.format;
        info.components.r = VK_COMPONENT_SWIZZLE_R;
        info.components.g = VK_COMPONENT_SWIZZLE_G;
        info.components.b = VK_COMPONENT_SWIZZLE_B;
        info.components.a = VK_COMPONENT_SWIZZLE_A;
        VkImageSubresourceRange image_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        info.subresourceRange = image_range;
        for (uint32_t i = 0; i < g_swapchain.image_count; i++)
        {
            info.image = g_swapchain.images[i];
            vkCreateImageView(g_Device, &info, g_Allocator, &g_swapchain.image_views[i]);
        }
    }

    // Create Framebuffer
    {
        g_swapchain.frame_buffers = std::vector<VkFramebuffer>(g_swapchain.image_count);
        VkImageView attachment[1];
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = g_swapchain.render_pass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = g_swapchain.width;
        info.height = g_swapchain.height;
        info.layers = 1;
        for (uint32_t i = 0; i < g_swapchain.image_count; i++)
        {
            attachment[0] = g_swapchain.image_views[i];
            vkCreateFramebuffer(g_Device, &info, g_Allocator, &g_swapchain.frame_buffers[i]);
        }
    }
}

void RenderDevice_1::ready_ui_data()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    ImGui::Render();
}


void RenderDevice_1::set_next_image()
{
    auto err = vkAcquireNextImageKHR(g_Device, g_swapchain.swapchain, UINT64_MAX, g_sync.semaphore_image_acquired, VK_NULL_HANDLE, &g_swapchain.current_frame_index);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        g_SwapChainRebuild = true;
    }
}

void RenderDevice_1::reset_things()
{
     vkWaitForFences(g_Device, 1, &g_sync.fence_queue_submited, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking

     vkResetFences(g_Device, 1, &g_sync.fence_queue_submited);    
    
     vkResetCommandPool(g_Device, g_mainCommandPool, 0);
     VkCommandBufferBeginInfo info = {};
     info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
     info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
     vkBeginCommandBuffer(g_mainCommandBuffer, &info);
}

void RenderDevice_1::fill_command_buff()
{
    VkRenderPassBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    info.renderPass = g_swapchain.render_pass;
    info.framebuffer = g_swapchain.frame_buffers[g_swapchain.current_frame_index];
    info.renderArea.extent.width = g_swapchain.width;
    info.renderArea.extent.height = g_swapchain.height;
    info.clearValueCount = 1;
    info.pClearValues = &g_swapchain.clear_value;
    vkCmdBeginRenderPass(g_mainCommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), g_mainCommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(g_mainCommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &g_sync.semaphore_image_acquired;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &g_mainCommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &g_sync.semaphore_render_finished;

        vkEndCommandBuffer(g_mainCommandBuffer);
        vkQueueSubmit(g_Queue, 1, &info, g_sync.fence_queue_submited);
    }

    // Swap Buff

    if (g_SwapChainRebuild)
        return;
    {
        
        auto err = vkQueuePresentKHR(g_Queue, get_present_info());
        if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
        {
            g_SwapChainRebuild = true;
            return;
        }
    }
}

void RenderDevice_1::rebuild_swapchain()
{
    VkSwapchainCreateInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    info.surface = g_swapchain.surface;
    info.minImageCount = g_swapchain.min_image_count;
    info.imageFormat = g_swapchain.surface_format.format;
    info.imageColorSpace = g_swapchain.surface_format.colorSpace;
    info.imageArrayLayers = 1;
    info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;           // graphics family == present family
    info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = g_swapchain.present_mode;
    info.clipped = VK_TRUE;
    info.oldSwapchain = g_swapchain.swapchain;
    info.imageExtent.width = g_swapchain.width;
    info.imageExtent.height = g_swapchain.height;

    vkCreateSwapchainKHR(g_Device, &info, g_Allocator, &g_swapchain.swapchain);

    vkGetSwapchainImagesKHR(g_Device, g_swapchain.swapchain, &g_swapchain.image_count, g_swapchain.images.data());
    // Should destroy prev view frame buff
    {
        VkImageViewCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        info.format =g_swapchain.surface_format.format;
        info.components.r = VK_COMPONENT_SWIZZLE_R;
        info.components.g = VK_COMPONENT_SWIZZLE_G;
        info.components.b = VK_COMPONENT_SWIZZLE_B;
        info.components.a = VK_COMPONENT_SWIZZLE_A;
        VkImageSubresourceRange image_range = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
        info.subresourceRange = image_range;
        for (uint32_t i = 0; i < g_swapchain.image_count; i++)
        {
            vkDestroyImageView(g_Device, g_swapchain.image_views[i], g_Allocator);
            info.image = g_swapchain.images[i];
            vkCreateImageView(g_Device, &info, g_Allocator, &g_swapchain.image_views[i]);
        }
    }

    // Create Framebuffer
    {
        g_swapchain.frame_buffers = std::vector<VkFramebuffer>(g_swapchain.image_count);
        VkImageView attachment[1];
        VkFramebufferCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        info.renderPass = g_swapchain.render_pass;
        info.attachmentCount = 1;
        info.pAttachments = attachment;
        info.width = g_swapchain.width;
        info.height = g_swapchain.height;
        info.layers = 1;
        for (uint32_t i = 0; i < g_swapchain.image_count; i++)
        {
            vkDestroyFramebuffer(g_Device, g_swapchain.frame_buffers[i], g_Allocator);
            attachment[0] = g_swapchain.image_views[i];
            vkCreateFramebuffer(g_Device, &info, g_Allocator, &g_swapchain.frame_buffers[i]);
        }
    }
}

void RenderDevice_1::make_frame()
{
    if (g_SwapChainRebuild)
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        if (width > 0 && height > 0)
        {
            g_swapchain.width = width;
            g_swapchain.height = height;

            rebuild_swapchain();


            //ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
            //ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
            //g_MainWindowData.FrameIndex = 0;
            g_SwapChainRebuild = false;
        }
    }

    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    ImGui::ShowDemoWindow();


    ImGui::Render();


    VkResult err;

    err = vkAcquireNextImageKHR(g_Device, g_swapchain.swapchain, UINT64_MAX, g_sync.semaphore_image_acquired, VK_NULL_HANDLE, &g_swapchain.current_frame_index);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        g_SwapChainRebuild = true;
        return;
    }

    {
        err = vkWaitForFences(g_Device, 1, &g_sync.fence_queue_submited, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking

        err = vkResetFences(g_Device, 1, &g_sync.fence_queue_submited);
    }
    {
        err = vkResetCommandPool(g_Device, g_mainCommandPool, 0);
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(g_mainCommandBuffer, &info);
    }
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = g_swapchain.render_pass;
        info.framebuffer = g_swapchain.frame_buffers[g_swapchain.current_frame_index];
        info.renderArea.extent.width = g_swapchain.width;
        info.renderArea.extent.height = g_swapchain.height;
        info.clearValueCount = 1;
        info.pClearValues = &g_swapchain.clear_value;
        vkCmdBeginRenderPass(g_mainCommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), g_mainCommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(g_mainCommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &g_sync.semaphore_image_acquired;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &g_mainCommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &g_sync.semaphore_render_finished;

        err = vkEndCommandBuffer(g_mainCommandBuffer);
        err = vkQueueSubmit(g_Queue, 1, &info, g_sync.fence_queue_submited);
    }

    if (g_SwapChainRebuild)
        return;

    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &g_sync.semaphore_render_finished;
    info.swapchainCount = 1;
    info.pSwapchains = &g_swapchain.swapchain;
    info.pImageIndices = &g_swapchain.current_frame_index;
    err = vkQueuePresentKHR(g_Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        g_SwapChainRebuild = true;
        return;
    }

}
