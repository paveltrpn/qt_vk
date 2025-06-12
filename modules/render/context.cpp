
#include "vulkan/vulkan.h"
#include <vulkan/vk_enum_string_helper.h>

#include "context.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_CONTEXT_CPP{ true };

#include "pipelines/pipeline.h"

namespace tire::vk {

Context::Context( VkInstance instance, VkPhysicalDevice pDevice,
                  VkDevice device, VkSurfaceKHR surface, VkRenderPass rp,
                  uint32_t gqfi )
    : instance_{ instance }
    , pDevice_{ pDevice }
    , device_{ device }
    , surface_{ surface }
    , renderPass_{ rp }
    , graphicsFamilyQueueId_{ gqfi } {
    uint32_t version{};
    vkEnumerateInstanceVersion( &version );
    log::info( "vk::Context === acquired api instance version is {}.{}",
               VK_API_VERSION_MAJOR( version ),
               VK_API_VERSION_MINOR( version ) );

    queryDeviceInfo();
    querySurface();
}

auto Context::queryDeviceInfo() -> void {
    // Collect physical devices and its properties
    vkGetPhysicalDeviceProperties( pDevice_, &pDeviceProperties_ );
    log::info( "vk::Device === name: {}", pDeviceProperties_.deviceName );

    vkGetPhysicalDeviceFeatures( pDevice_, &pDeviceFeatures_ );

    // Collect physical device queue families properies
    uint32_t queueFamilyCount{};
    vkGetPhysicalDeviceQueueFamilyProperties( pDevice_, &queueFamilyCount,
                                              nullptr );

    queueFamilyProperties_.resize( queueFamilyCount );
    vkGetPhysicalDeviceQueueFamilyProperties( pDevice_, &queueFamilyCount,
                                              queueFamilyProperties_.data() );
    log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
        "vk::Device === device queue family count: {}", queueFamilyCount );

    // Collect physical device extensions info
    uint32_t extensionCount{};
    if ( const auto err = vkEnumerateDeviceExtensionProperties(
             pDevice_, nullptr, &extensionCount, nullptr );
         err != VK_SUCCESS ) {
        log::fatal(
            "can't enumerate physical device extensions "
            "with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::Device === physical device extensions enumerated for "
            "device: {}, "
            "count: {}",
            pDeviceProperties_.deviceName, extensionCount );
    }

    pDeviceExtensions_.resize( extensionCount );
    if ( const auto err = vkEnumerateDeviceExtensionProperties(
             pDevice_, nullptr, &extensionCount, pDeviceExtensions_.data() );
         err != VK_SUCCESS ) {
        log::fatal(
            "can't acquire physical device extensions "
            "with code: {}\n",
            string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::Device === physical device extensions acquired for "
            "device: "
            "{}",
            pDeviceProperties_.deviceName );
    }

    // TODO: remove!!!
    printDriverInfo();
}

auto Context::querySurface() -> void {
    // Physical device surface capabilities.
    if ( const auto err = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
             pDevice_, surface_, &surfaceCapabilities_ );
         err != VK_SUCCESS ) {
        log::fatal( "failed to obtain surface capabilities with code {}!\n",
                    string_VkResult( err ) );
    } else {
        log::debug<DEBUG_OUTPUT_CONTEXT_CPP>(
            "vk::Device === physical device surface capabilities acquire "
            "success!" );
    }
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

Context::~Context() {
}

}  // namespace tire::vk
