
#pragma once

#include <print>
#include <vector>
#include <expected>

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define CONFIG_APPLAICATION_NAME "application_name"
#define CONFIG_ENGINE_NAME "engine_name"
#define CONFIG_ENABLE_VALIDATION_LAYERS false
#define CONFIG_ENABLE_ADDITIONAL_VALIDATION_LAYERS false
#define CONFIG_ENABLE_API_DUMP_VALIDATION_LAYRES false
#define CONFIG_VERBOSE_VULKAN_OUTPUT false
#define CONFIG_FRAMES_COUNT 3
#define CONFIG_ENABLE_RAYTRACING_APPLICATIONS false

namespace tire::vk {

struct Pipeline;

struct Context final {
    Context( VkInstance instance, VkPhysicalDevice pDevice, VkDevice device,
             VkSurfaceKHR surface, VkRenderPass rp, uint32_t gqfi );
    ~Context();

    Context( const Context& other ) = delete;
    Context( Context&& other ) = delete;
    Context& operator=( const Context& other ) = delete;
    Context& operator=( Context&& other ) = delete;

    auto queryDeviceInfo() -> void;
    auto querySurface() -> void;

    [[nodiscard]] auto instance() const -> VkInstance { return instance_; };
    [[nodiscard]] auto device() const -> VkDevice { return device_; };
    [[nodiscard]] auto renderPass() const -> VkRenderPass {
        return renderPass_;
    };

    // [[nodiscard]] const VkSurfaceFormatKHR& surfaceFormat() const {
    // return surfaceFormat_;
    // };

    [[nodiscard]] auto memoryRequirements(
        uint32_t typeFilter, VkMemoryPropertyFlags properties ) const
        -> uint32_t;

    [[nodiscard]] auto findSupportedFormat(
        const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features ) const -> VkFormat;

    [[nodiscard]] auto framesCount() const -> uint32_t { return framesCount_; };

    // [[nodiscard]] const VkQueue& graphicsQueue() const {
    // return graphicsQueue_;
    // }

    //[[nodiscard]] VkQueue presentQueue() const { return presentQueue_; };

    [[nodiscard]] auto graphicsFamily() const -> uint32_t {
        return graphicsFamilyQueueId_;
    };

    [[nodiscard]] auto currentExtent() const -> const VkExtent2D& {
        return surfaceCapabilities_.currentExtent;
    };

    // NOTE: temporary!
    auto printDriverInfo() -> void {
        std::println( "{} {} {} {} {} {}",
                      VK_VERSION_MAJOR( pDeviceProperties_.driverVersion ),
                      VK_VERSION_MINOR( pDeviceProperties_.driverVersion ),
                      VK_VERSION_PATCH( pDeviceProperties_.driverVersion ),
                      VK_VERSION_MAJOR( pDeviceProperties_.apiVersion ),
                      VK_VERSION_MINOR( pDeviceProperties_.apiVersion ),
                      VK_VERSION_PATCH( pDeviceProperties_.apiVersion ) );
    }

private:
    // Instance
    VkInstance instance_{ VK_NULL_HANDLE };

    // Physical device
    VkPhysicalDevice pDevice_{ VK_NULL_HANDLE };
    VkPhysicalDeviceProperties pDeviceProperties_{};
    VkPhysicalDeviceFeatures pDeviceFeatures_{};
    std::vector<VkExtensionProperties> pDeviceExtensions_{};
    std::vector<VkQueueFamilyProperties> queueFamilyProperties_{};

    // Logical device
    VkDevice device_{ VK_NULL_HANDLE };

    // Surface
    VkSurfaceKHR surface_{ VK_NULL_HANDLE };
    VkSurfaceCapabilitiesKHR surfaceCapabilities_{};

    // Render pass
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    uint32_t graphicsFamilyQueueId_{};

    uint32_t framesCount_{ CONFIG_FRAMES_COUNT };
};

}  // namespace tire::vk
