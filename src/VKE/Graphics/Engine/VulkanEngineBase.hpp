#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>

#include <VKE/Graphics/Context/VulkanContext.hpp>
#include <VKE/Graphics/Descriptor/VulkanDescriptorSet.hpp>
#include <VKE/Graphics/Descriptor/VulkanVertexDescriptor.hpp>
#include <VKE/Graphics/Device/VulkanDevice.hpp>
#include <VKE/Graphics/Engine/VulkanEngineInterface.hpp>
#include <VKE/Graphics/Pipelines/VulkanPipelines.hpp>
#include <VKE/Graphics/Swapchain/VulkanSwapchain.hpp>

#include <memory>

namespace vke {

namespace priv {

class VKE_API VulkanEngineBase : public VulkanEngineInterface
{
    public:
        VulkanEngineBase();
        ~VulkanEngineBase() override;

        void start();
        void renderLoop();
        void stop();

    protected:
        bool _running = false;
        bool _paused = false;
        f32 _frame_time = 0.0f;
        maths::Vector2u _size = {800, 600};
        std::unique_ptr<Window> _window;
        std::unique_ptr<VulkanPipelines> _pipelines;

        void renderFrame();

        void initialize() override;
        void update() override;
        void render() override;

        virtual void buildObjects(VkCommandBuffer VKE_UNUSED &cmd) {};
        virtual void buildCommandBufferBeforeRenderPass() {};
        virtual void buildCommandBufferAfterRenderPass() {};

        void waitForCurrentFrame();

    private:
        /** Vulkan instance */
        VkInstance _instance = VKE_NULL_PTR;
        std::vector<const char *> _enabled_extensions;
        std::vector<const char *> _enabled_device_extensions;
        VkPhysicalDeviceFeatures _enabled_features = {};

        /** Vulkan extensions */

        /** Vulkan device */
        std::unique_ptr<VulkanDevice> _vulkan_device;
        void *_device_create_next_chain = VKE_NULL_PTR;
        VkDevice _device = VKE_NULL_PTR;
        VkPhysicalDevice _physical_device = VKE_NULL_PTR;
        VkPhysicalDeviceProperties _physical_device_properties = {};
        VkPhysicalDeviceFeatures _physical_device_features = {};
        VkPhysicalDeviceMemoryProperties _physical_device_memory_properties = {};

        /** Vulkan queue */
        VkQueue _queue = VKE_NULL_PTR;
        struct RenderSemaphores {
                /** swapchain image presentation */
                VkSemaphore _presentation = VKE_NULL_PTR;
                /** command buffer submission & execution */
                VkSemaphore _rendering = VKE_NULL_PTR;
        } _semaphores;
        struct RenderDepthStencil {
                VkImage _image = VKE_NULL_PTR;
                VkDeviceMemory _memory = VKE_NULL_PTR;
                VkImageView _image_view = VKE_NULL_PTR;
        } _depth_stencil;

        /** Vulkan Graphics */
        u32 _current_buffer = 0;
        bool _signal_frame = false;

        VulkanSwapchain _swapchain;
        VkFormat _depth_format = VK_FORMAT_D16_UNORM_S8_UINT;
        VkSubmitInfo _submit_info = {};
        VkRenderPass _render_pass = VKE_NULL_PTR;
        VkCommandPool _command_pool = VKE_NULL_PTR;
        VkPipelineCache _pipeline_cache = VKE_NULL_PTR;
        VkPipelineLayout _pipeline_layout = VKE_NULL_PTR;
        VkDescriptorPool _descriptor_pool = VKE_NULL_PTR;
        std::unique_ptr<VulkanContext> _context;
        std::unique_ptr<VulkanDescriptorSet> _descriptor_set;
        std::unique_ptr<VulkanVertexDescriptor> _vertex_descriptor;

        std::vector<VkCommandBuffer> _command_buffer;
        std::vector<VkShaderModule> _shader_modules;
        std::vector<VkFramebuffer> _framebuffers;
        std::vector<VkFence> _wait_fences;

        /** initialization */
        void _create_window();
        void _create_vulkan_instance();
        void _get_physical_device();

        /** start */
        void _create_command_pool();
        void _create_command_buffer();
        void _create_synchronization_objects();
        void _create_depth_stencil();
        void _create_render_pass();
        void _create_pipeline_cache();
        void _create_framebuffer();

        /** draw */
        void _acquire_frame();
        void _submit_frame();

        /** build command */
        void _build_command_buffer();
        void _set_viewports(VkCommandBuffer &cmd);

        /** destroy */
        void _destroy();
        void _destroy_command_buffer();
        void _destroy_surface();
};

}// namespace priv

}// namespace vke
