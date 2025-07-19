#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>

namespace vke {

namespace priv {

class VKE_API VulkanVertexDescriptor final
{
    public:
        VulkanVertexDescriptor();
        ~VulkanVertexDescriptor() = default;

        VkPipelineVertexInputStateCreateInfo &getState();

    private:
        VkPipelineVertexInputStateCreateInfo _input_state = {};
        std::vector<VkVertexInputBindingDescription> _input_binding;
        std::vector<VkVertexInputAttributeDescription> _input_attributes;

        void _generate_descriptions();
};

}// namespace priv

}// namespace vke
