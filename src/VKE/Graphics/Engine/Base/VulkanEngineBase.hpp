#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>

#include <VKE/Graphics/Context/VulkanContext.hpp>
#include <VKE/Graphics/Descriptor/VulkanVertexDescriptor.hpp>
#include <VKE/Graphics/Engine/VulkanEngineInterface.hpp>
#include <VKE/Graphics/Swapchain/VulkanSwapchain.hpp>

#include <memory>

namespace vke {

namespace priv {

class VKE_API VulkanEngineBase : public VulkanEngineInterface
{
    public:
        VulkanEngineBase();
        ~VulkanEngineBase() override;

        virtual void start();
        virtual void getDeviceFeatures() {};

    protected:
        bool _running = false;
        bool _prepared = false;
        bool _paused = false;
        u32 _selected_gpu_index = 0;

        maths::Vector2u _size = {800, 600};

        //TODO: better way to handle this
        VkPipelineLayout _pipeline_layout = VKE_NULLPTR;
        VkRenderPass _render_pass = VKE_NULLPTR;
        VkDevice _device = VKE_NULLPTR;

        /** smart pointers, aligned */
        std::unique_ptr<VulkanDevice> _vulkan_device;
        std::shared_ptr<VulkanContext> _context;
        std::unique_ptr<VulkanPipelines> _pipelines;
        std::unique_ptr<VulkanVertexDescriptor> _vertex_descriptor;
        std::unique_ptr<VulkanDescriptorSet> _descriptor_set;

        std::shared_ptr<Window> _window;

        void renderFrame();
        void render() override;

        virtual void drawObjects(VkCommandBuffer VKE_UNUSED &cmd) {};
        virtual void buildCommandBufferBeforeRenderPass(VkCommandBuffer VKE_UNUSED &cmd) {};
        virtual void buildCommandBufferAfterRenderPass(VkCommandBuffer VKE_UNUSED &cmd) {};
        void build_command_buffer();

    private:
        /** Vulkan instance */
        VkInstance _instance = VKE_NULLPTR;
        std::vector<const char *> _enabled_extensions;
        std::vector<const char *> _enabled_device_extensions;
        VkPhysicalDeviceFeatures _enabled_features = {};

        /** Vulkan extensions */

        /** Vulkan device */
        void *_device_create_next_chain = VKE_NULLPTR;
        VkPhysicalDevice _physical_device = VKE_NULLPTR;
        VkPhysicalDeviceProperties _physical_device_properties = {};
        VkPhysicalDeviceFeatures _physical_device_features = {};
        VkPhysicalDeviceMemoryProperties _physical_device_memory_properties = {};

        /** Vulkan queue */
        VkQueue _queue = VKE_NULLPTR;
        struct RenderSemaphores {
                /** swapchain image presentation */
                VkSemaphore _imageAvailableSemaphore = VKE_NULLPTR;
                /** command buffer submission & execution */
                VkSemaphore _renderFinishedSemaphore = VKE_NULLPTR;
        } _semaphores;
        struct RenderDepthStencil {
                VkImage _image = VKE_NULLPTR;
                VkDeviceMemory _memory = VKE_NULLPTR;
                VkImageView _image_view = VKE_NULLPTR;
        } _depth_stencil;

        /** Vulkan Graphics */
        u32 _current_buffer = 0;
        bool _signal_frame = false;

        VulkanSwapchain _swapchain;
        VkFormat _depth_format = VK_FORMAT_D16_UNORM_S8_UINT;
        VkSubmitInfo _submit_info = {};
        VkCommandPool _command_pool = VKE_NULLPTR;
        VkPipelineCache _pipeline_cache = VKE_NULLPTR;
        VkDescriptorPool _descriptor_pool = VKE_NULLPTR;
        VkPipelineStageFlags _submit_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

        std::vector<VkCommandBuffer> _command_buffer;
        std::vector<VkShaderModule> _shader_modules;
        std::vector<VkFramebuffer> _framebuffers;
        std::vector<VkFence> _wait_fences;

        /** initialization */
        void _create_window();
        void _create_semaphores();
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
        void _create_context();

        /** draw */
        void _acquire_frame();
        void _submit_frame();
        void _resize_window();
        void _set_viewports(VkCommandBuffer &cmd);

        /** destroy */
        void _destroy();
        void _destroy_fences();
        void _destroy_semaphores();
        void _destroy_framebuffer();
        void _destroy_depth_stencil();
        void _destroy_command_buffer();
};

}// namespace priv

}// namespace vke
