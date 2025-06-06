
#include "rendervk.h"
#include "pipelines/program.h"
#include "pipelines/shader_source.h"

namespace tire {
    RenderVK::RenderVK() = default;

    RenderVK::~RenderVK() = default;

    void RenderVK::init() {
        try {
            context_ = std::make_unique<vk::Context>(window_);
            context_->init();

            piplineMatrixReady_ =
                    std::make_unique<vk::PiplineMatrixReady>(context_.get());

            auto program = vk::Program{context_.get()};
            program.fill(
                    {{vk::vk_simple_box_VERTEX,   vk::vertex_stage_suffix},
                     {vk::vk_simple_box_FRAGMENT, vk::fragment_stage_suffix}});

            piplineMatrixReady_->initShaderStages(program);
            piplineMatrixReady_->buildPipeline();

            context_->makeFrames(piplineMatrixReady_.get());

            renderCommand_ = std::make_unique<vk::SceneRenderCommand>(context_.get(),
                                                                      piplineMatrixReady_.get());

            ready_ = true;
        } catch (const std::runtime_error &e) {
            throw std::runtime_error(e.what());
        }
    }

    void RenderVK::clean() {
        ready_ = false;
    }
}

