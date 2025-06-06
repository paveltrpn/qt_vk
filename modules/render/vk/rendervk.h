
#pragma once

#include "../render.h"
#include "context.h"
#include "pipelines/pipeline.h"
#include "commands/scene_render_command.h"

#include "../../log/log.h"

#define FRAMES_IN_FLIGHT_COUNT 3

namespace tire {
    struct RenderVK final : Render {
        RenderVK();

        ~RenderVK();

        void init() override;

        void clean() override;

        void preLoop() override;

        void postLoop() override;

    protected:

        void preFrame() override;

        void frame() override;

        void postFrame() override;

        void swapBuffers() override;

    private:
        std::unique_ptr<vk::Context> context_{};

        std::unique_ptr<vk::Pipeline> piplineMatrixReady_{};

        std::unique_ptr<vk::SceneRenderCommand> renderCommand_{};

        float angle_{};

        uint32_t currentFrame_{0};
    };

}