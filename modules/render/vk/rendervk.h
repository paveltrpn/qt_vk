
#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#include "pipelines/pipeline.h"
#include "buffers/vertex_buffer.h"
#include "command_pool.h"
#include "commands/scene_render_command.h"

#include "../render.h"

#define FRAMES_IN_FLIGHT_COUNT 3

namespace tire {

struct RenderVK final : Render {
    RenderVK();
    ~RenderVK() override = default;

    void displayRenderInfo() override{};
    void setSwapInterval( int interval ) override;

private:
    void preLoop() override;
    void preFrame() override;
    void frame() override;
    void postFrame() override;
    void swapBuffers() override;
    void postLoop() override;

    void createGraphicsPipeline();
    void createSyncObjects();

private:
    std::unique_ptr<vk::Context> context_{};

    std::unique_ptr<vk::Pipeline> piplineMatrixReady_{};

    std::unique_ptr<vk::SceneRenderCommand> renderCommand_{};

    float angle_{};

    uint32_t currentFrame_{ 0 };
};

}  // namespace tire
