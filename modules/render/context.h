
#pragma once

#include <vector>
#include <expected>

#include <vulkan/vulkan.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
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
             VkRenderPass rp );
    ~Context();

    Context( const Context& other ) = delete;
    Context( Context&& other ) = delete;
    Context& operator=( const Context& other ) = delete;
    Context& operator=( Context&& other ) = delete;

    [[nodiscard]] VkInstance instance() const { return instance_; };
    [[nodiscard]] VkDevice device() const { return device_; };
    [[nodiscard]] VkRenderPass renderPass() const { return renderPass_; };

    // [[nodiscard]] const VkSurfaceFormatKHR& surfaceFormat() const {
    // return surfaceFormat_;
    // };

    [[nodiscard]] uint32_t memoryRequirements(
        uint32_t typeFilter, VkMemoryPropertyFlags properties ) const;
    [[nodiscard]] VkFormat findSupportedFormat(
        const std::vector<VkFormat>& candidates, VkImageTiling tiling,
        VkFormatFeatureFlags features ) const;

    //[[nodiscard]] uint32_t framesCount() const { return framesCount_; };

    // [[nodiscard]] const VkQueue& graphicsQueue() const {
    // return graphicsQueue_;
    // }

    //[[nodiscard]] VkQueue presentQueue() const { return presentQueue_; };

    // [[nodiscard]] uint32_t graphicsFamily() const {
    // return graphicsFamilyQueueId_;
    // };

    [[nodiscard]] const VkExtent2D& currentExtent() const {
        return currentExtent_;
    };

private:
    // void makeCommandPool();

private:
    VkInstance instance_{ VK_NULL_HANDLE };
    VkPhysicalDevice pDevice_{ VK_NULL_HANDLE };
    VkDevice device_{ VK_NULL_HANDLE };
    VkExtent2D currentExtent_{ 1515, 1006 };
    VkRenderPass renderPass_{ VK_NULL_HANDLE };

    VkPhysicalDeviceProperties devProps_{};
};

}  // namespace tire::vk
