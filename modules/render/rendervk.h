
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "pipelines/pipeline.h"
#include "timer.h"

#define FRAMES_IN_FLIGHT_COUNT 3

namespace tire {

struct RenderVK final {
    RenderVK() = default;
    ~RenderVK() = default;

    void init( VkInstance instance, VkPhysicalDevice pDevice, VkDevice device,
               VkRenderPass rp );

    void mainPassRecordingStart( VkCommandBuffer cb );

    void createGraphicsPipeline();

private:
    std::unique_ptr<vk::Context> context_{};

    std::unique_ptr<vk::Pipeline> piplineMatrixReady_{};

    float angle_{};

    uint32_t currentFrame_{ 0 };

    Timer timer_{};

    // Background color value
    std::array<VkClearValue, 2> clearValues_{};
};

}  // namespace tire
