#pragma once

#include <vulkan_backend/Backend.hpp>
#include <vulkan_backend/core/VulkanObject.hpp>

#include <string>

// clang-format off

namespace lumen {

class RenderPass;

class Pipeline : public VulkanObject<VkPipeline>
{
    public:
        Pipeline(Device &device, const RenderPass &render_pass, const std::string &vert_shader_path, const std::string &frag_shader_path);
        Pipeline(Device &device, const RenderPass &render_pass);
        ~Pipeline() noexcept override;

        VkPipelineLayout getLayout() const noexcept;

    private:
        void _set_shader_stages(const RenderPass &render_pass, const std::string &vert_content, const std::string &frag_content, bool is_source);

        VkPipelineLayout _pipelineLayout;
};

}// namespace lumen
