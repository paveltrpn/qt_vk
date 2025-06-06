
#include <memory>
#include <print>
#include <format>

#include "context.h"
#include "pipelines/pipeline.h"
#include "commands/scene_render_command.h"
#include "pipelines/program.h"
#include "pipelines/shader_source.h"

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "rendervk.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };
#include "algebra/matrix4.h"

namespace tire {

RenderVK::RenderVK()
    : Render{} {
    try {
        context_ = std::make_unique<vk::Context>( display_, window_ );
        context_->init();

        piplineMatrixReady_ =
            std::make_unique<vk::PiplineMatrixReady>( context_.get() );

        auto program = vk::Program{ context_.get() };
        program.fill(
            { { vk::vk_simple_box_VERTEX, vk::vertex_stage_suffix },
              { vk::vk_simple_box_FRAGMENT, vk::fragment_stage_suffix } } );

        piplineMatrixReady_->initShaderStages( program );
        piplineMatrixReady_->buildPipeline();

        context_->makeFrames( piplineMatrixReady_.get() );

        renderCommand_ = std::make_unique<vk::SceneRenderCommand>(
            context_.get(), piplineMatrixReady_.get() );
    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

void RenderVK::setSwapInterval( int interval ){

};

}  // namespace tire
