
#include <stdexcept>
#include "vulkan/vulkan_core.h"
#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vk_enum_string_helper.h>

#include "context.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_CONTEXT_CPP{ true };

#include "pipelines/pipeline.h"

namespace tire::vk {

Context::Context( VkInstance instance, VkPhysicalDevice pDevice,
                  VkDevice device, VkRenderPass rp )
    : instance_{ instance }
    , pDevice_{ pDevice }
    , device_{ device }
    , renderPass_{ rp } {
    vkGetPhysicalDeviceProperties( pDevice, &devProps_ );
    log::info( "vk::Device === name: {}", devProps_.deviceName );
}

uint32_t Context::memoryRequirements( uint32_t typeFilter,
                                      VkMemoryPropertyFlags properties ) const {
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties( pDevice_, &memProperties );

    for ( uint32_t i = 0; i < memProperties.memoryTypeCount; i++ ) {
        if ( ( typeFilter & ( 1 << i ) ) &&
             ( memProperties.memoryTypes[i].propertyFlags & properties ) ==
                 properties ) {
            return i;
        }
    }

    log::fatal( "failed to find suitable memory type!" );

    // Silence warning
    return {};
}

VkFormat Context::findSupportedFormat( const std::vector<VkFormat> &candidates,
                                       VkImageTiling tiling,
                                       VkFormatFeatureFlags features ) const {
    for ( VkFormat format : candidates ) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties( pDevice_, format, &props );

        if ( tiling == VK_IMAGE_TILING_LINEAR &&
             ( props.linearTilingFeatures & features ) == features ) {
            return format;
        } else if ( tiling == VK_IMAGE_TILING_OPTIMAL &&
                    ( props.optimalTilingFeatures & features ) == features ) {
            return format;
        }
    }

    throw std::runtime_error(
        std::format( "failed to find supported format!" ) );

    // Silence warning
    return {};
}
/*
void Context::makeCommandPool() {
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = graphicsFamilyQueueId_;

    if ( const auto err =
             vkCreateCommandPool( device_, &poolInfo, nullptr, &commandPool_ );
         err != VK_SUCCESS ) {
        throw std::runtime_error(
            std::format( "failed to create command pool woth code {}!",
                         string_VkResult( err ) ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::CommandPool === command pool created!" );
    }
}
*/

Context::~Context() {
}

}  // namespace tire::vk
