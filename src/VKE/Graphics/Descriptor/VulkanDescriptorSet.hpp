#pragma once

#include <VKE/Backend.hpp>
#include <VKE/Macros.hpp>
#include <VKE/Types.hpp>

namespace vke {

namespace priv {

class VKE_API VulkanDescriptorSet final
{
    public:
        explicit VulkanDescriptorSet(VkDevice device, u32 max_sets = 1);
        ~VulkanDescriptorSet();

        enum class Type { Image = 0, Buffer = 1 };
        struct Info {
                u32 _binding;
                u32 _index;
                Type _type;
                VkDescriptorImageInfo *_image_info = VKE_NULL_PTR;
                VkDescriptorBufferInfo *_buffer_info = VKE_NULL_PTR;
                VkDescriptorType _descriptor_type;
                VkShaderStageFlags _stage_flags;
        };

        void add(u32 binding, u32 index, VkDescriptorImageInfo *image_info, VkDescriptorType descriptor_type, VkShaderStageFlags stage_flags);
        void add(u32 binding, u32 index, VkDescriptorBufferInfo *buffer_info, VkDescriptorType descriptor_type, VkShaderStageFlags stage_flags);
        void generate(VkPipelineLayout *pipeline_layout);

        VkDescriptorSet &get(u32 index);
        u32 getSize() const;

    private:
        VkDevice _device = VKE_NULL_PTR;
        VkDescriptorPool _pool = VKE_NULL_PTR;
        VkDescriptorSetLayout _layout = VKE_NULL_PTR;

        std::vector<VkDescriptorSet> _sets;
        std::vector<Info> _infos;
};

}// namespace priv

}// namespace vke
