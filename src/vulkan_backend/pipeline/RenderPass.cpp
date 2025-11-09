#include "vulkan_backend/pipeline/RenderPass.hpp"
#include "vulkan_backend/core/Device.hpp"
#include "vulkan_backend/surface/SwapChain.hpp"
#include "vulkan_backend/utils/Result.hpp"
#include <vulkan/vulkan_core.h>

/**
 * static helpers
 */
// clang-format off

namespace {

static inline VkAttachmentDescription __create_color_attachment(VkFormat format) noexcept
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    return colorAttachment;
}

static inline VkAttachmentReference __create_color_attachment_ref() noexcept
{
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    return colorAttachmentRef;
}

static inline VkSubpassDescription __create_subpass(const VkAttachmentReference *colorAttachmentRef) noexcept
{
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = colorAttachmentRef;
    return subpass;
}

static inline VkSubpassDependency __create_dependency() noexcept
{
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    return dependency;
}

static inline VkRenderPassCreateInfo __create_render_pass_info(
    const VkAttachmentDescription *colorAttachment,
    const VkSubpassDescription *subpass,
    const VkSubpassDependency *dependency
) noexcept
{
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = dependency;
    return renderPassInfo;
}

}// namespace

/**
* public
*/

lumen::RenderPass::RenderPass(Device &device, const SwapChain &swapChain)
    : VulkanObject(device, VK_NULL_HANDLE)
{
    VkAttachmentDescription colorAttachment = __create_color_attachment(swapChain.getImageFormat());
    VkAttachmentReference colorAttachmentRef = __create_color_attachment_ref();
    VkSubpassDescription subpass = __create_subpass(&colorAttachmentRef);
    VkSubpassDependency dependency = __create_dependency();
    VkRenderPassCreateInfo renderPassInfo = __create_render_pass_info(&colorAttachment, &subpass, &dependency);

    vk_check(vkCreateRenderPass(_device.logicalDevice(), &renderPassInfo, nullptr, &_handle), "failed to create render pass!");
}

lumen::RenderPass::~RenderPass() noexcept
{
    if (_handle != VK_NULL_HANDLE) {
        vkDestroyRenderPass(_device.logicalDevice(), _handle, nullptr);
    }
}
