#include <VKE/Error.hpp>
#include <VKE/Graphics/Descriptor/VulkanDescriptorSet.hpp>
#include <VKE/Maths/Matrix.hpp>

/**
* public
*/

vke::priv::VulkanDescriptorSet::VulkanDescriptorSet(VkDevice device, u32 max_sets) : _device(device)
{
    _device = device;
    _sets.resize(max_sets);
}

vke::priv::VulkanDescriptorSet::~VulkanDescriptorSet()
{
    for (auto &set : _sets) {
        VKE_SAFE_CLEAN(set, vkFreeDescriptorSets(_device, _pool, 1, &set));
    }
    VKE_SAFE_CLEAN(_pool, vkDestroyDescriptorPool(_device, _pool, VKE_NULLPTR));
    VKE_SAFE_CLEAN(_layout, vkDestroyDescriptorSetLayout(_device, _layout, VKE_NULLPTR));
}

void vke::priv::VulkanDescriptorSet::add(u32 binding, u32 index, VkDescriptorImageInfo *image_info, VkDescriptorType descriptor_type, VkShaderStageFlags stage_flags)
{
    // clang-format off
    _infos.push_back(
        {
            ._binding = binding,
            ._index = index,
            ._type = Type::Image,
            ._image_info = image_info,
            ._buffer_info = VKE_NULLPTR,
            ._descriptor_type = descriptor_type,
            ._stage_flags = stage_flags
        }
    );
    // clang-format on
}

void vke::priv::VulkanDescriptorSet::add(u32 binding, u32 index, VkDescriptorBufferInfo *buffer_info, VkDescriptorType descriptor_type, VkShaderStageFlags stage_flags)
{
    // clang-format off
    _infos.push_back(
        {
            ._binding = binding,
            ._index = index,
            ._type = Type::Buffer,
            ._image_info = VKE_NULLPTR,
            ._buffer_info = buffer_info,
            ._descriptor_type = descriptor_type,
            ._stage_flags = stage_flags
        }
    );
    // clang-format on
}

/**
 * helpers
 */

static inline VkWriteDescriptorSet _create_write_set(VkDescriptorSet set, VkDescriptorType type, VkDescriptorImageInfo *image_info, VkDescriptorBufferInfo *buffer_info,
    u32 binding, u32 count = 1)
{
    return {
        .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
        .pNext = {},
        .dstSet = set,
        .dstBinding = binding,
        .dstArrayElement = {},
        .descriptorCount = count,
        .descriptorType = type,
        .pImageInfo = image_info,
        .pBufferInfo = buffer_info,
        .pTexelBufferView = {},
    };
}

void vke::priv::VulkanDescriptorSet::generate(VkPipelineLayout *pipeline_layout)
{
    std::vector<VkDescriptorPoolSize> pool_sizes;
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    std::vector<std::vector<VkWriteDescriptorSet>> write_sets(_sets.size());

    for (auto &info : _infos) {
        pool_sizes.push_back({info._descriptor_type, 1});

        if (info._index == 0) {
            bindings.push_back({info._binding, info._descriptor_type, 1, info._stage_flags, {}});
        }

        if (info._type == Type::Image) {
            write_sets.at(info._index).push_back({_create_write_set(VKE_NULLPTR, info._descriptor_type, info._image_info, VKE_NULLPTR, info._binding)});
            continue;
        }
        write_sets.at(info._index).push_back({_create_write_set(VKE_NULLPTR, info._descriptor_type, VKE_NULLPTR, info._buffer_info, info._binding)});
    }

    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = static_cast<u32>(_sets.size());
    pool_info.poolSizeCount = static_cast<u32>(pool_sizes.size());
    pool_info.pPoolSizes = pool_sizes.data();

    VKE_ASSERT(vkCreateDescriptorPool(_device, &pool_info, VKE_NULLPTR, &_pool));

    VkDescriptorSetLayoutCreateInfo layout_info = {};
    layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_info.bindingCount = static_cast<u32>(bindings.size());
    layout_info.pBindings = bindings.data();

    VKE_ASSERT(vkCreateDescriptorSetLayout(_device, &layout_info, VKE_NULLPTR, &_layout));

    const VkPushConstantRange push_constant_range = {
        .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
        .offset = 0,
        .size = sizeof(maths::Matrix4f),
    };

    VkPipelineLayoutCreateInfo pipeline_info = {};
    pipeline_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_info.setLayoutCount = 1;
    pipeline_info.pSetLayouts = &_layout;
    pipeline_info.pushConstantRangeCount = 1;
    pipeline_info.pPushConstantRanges = &push_constant_range;

    VKE_ASSERT(vkCreatePipelineLayout(_device, &pipeline_info, VKE_NULLPTR, pipeline_layout));

    for (u32 i = 0; i < _sets.size(); ++i) {

        VkDescriptorSetAllocateInfo alloc_info = {};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.descriptorPool = _pool;
        alloc_info.descriptorSetCount = 1;
        alloc_info.pSetLayouts = &_layout;

        VKE_ASSERT(vkAllocateDescriptorSets(_device, &alloc_info, &_sets[i]));

        for (u32 j = 0; j < write_sets[i].size(); ++j) {
            write_sets[i][j].dstSet = _sets[i];
        };
        vkUpdateDescriptorSets(_device, static_cast<u32>(write_sets[i].size()), write_sets[i].data(), 0, VKE_NULLPTR);
    }
}

/**
 * getters
 */

VkDescriptorSet &vke::priv::VulkanDescriptorSet::get(u32 index)
{
    if (index >= _sets.size()) {
        throw exception::OutOfRange("vke::priv::VulkanDescriptorSet::get", "Index out of bounds for descriptor sets.");
    }

    return _sets[index];
}

u32 vke::priv::VulkanDescriptorSet::getSize() const
{
    return static_cast<u32>(_sets.size());
}

/**
* private
*/
