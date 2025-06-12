
#include <memory>
#include <print>
#include <format>

#include "context.h"
#include "pipelines/pipeline.h"
#include "pipelines/program.h"
#include "pipelines/shader_source.h"

#define VK_USE_PLATFORM_XLIB_KHR
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#include "render.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };
#include "algebra/matrix4.h"

namespace tire::vk {

void Render::init( vk::Context *context ) {
    try {
        context_ = context;

        piplineMatrixReady_ =
            std::make_unique<vk::PiplineMatrixReady>( context_ );

        auto program = vk::Program{ context_ };
        program.fill(
            { { vk::vk_simple_box_VERTEX, vk::vertex_stage_suffix },
              { vk::vk_simple_box_FRAGMENT, vk::fragment_stage_suffix } } );

        piplineMatrixReady_->initShaderStages( program );
        piplineMatrixReady_->buildPipeline();

        clearValues_[0].color = { 0.0f, 0.0f, 1.0f, 0.0f };
        clearValues_[1].depthStencil = { .depth = 0.0f, .stencil = 0 };

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
}

void Render::frame( VkCommandBuffer cb ) {
    // Update global timer
    timer_.update();

    // =================================
    // Get transformation matricies
    auto offset = algebra::translate( 0.0f, 0.0f, -5.0f );
    offset.transposeSelf();

    const auto [width, height] = context_->currentExtent();
    // NOTE: Choose right projection matrix!!!
    const auto proj = algebra::perspective<float>(
        50.0f, static_cast<float>( width ) / static_cast<float>( height ), 0.1f,
        100.0f );
    const auto viewMatrix = offset * proj;
    angle_ += timer_.floatFrameDuration() * 25.0f;
    algebra::vector3f ax{ 0.0f, 1.0f, 1.0f };
    ax.normalizeSelf();
    const auto modelMatrix = algebra::rotate( ax, angle_ );
    // =================================

    const VkViewport viewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>( context_->currentExtent().width ),
        .height = static_cast<float>( context_->currentExtent().height ),
        .minDepth = 0.0f,
        .maxDepth = 1.0f };
    vkCmdSetViewport( cb, 0, 1, &viewport );

    const VkRect2D scissor{ { .x = 0, .y = 0 },
                            { .width = context_->currentExtent().width,
                              .height = context_->currentExtent().height } };
    vkCmdSetScissor( cb, 0, 1, &scissor );

    vkCmdBindPipeline( cb, VK_PIPELINE_BIND_POINT_GRAPHICS,
                       piplineMatrixReady_->pipeline() );

    vkCmdPushConstants( cb, piplineMatrixReady_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, 0,
                        sizeof( algebra::matrix4f ), &viewMatrix );

    vkCmdPushConstants( cb, piplineMatrixReady_->layout(),
                        VK_SHADER_STAGE_VERTEX_BIT, sizeof( algebra::matrix4f ),
                        sizeof( algebra::matrix4f ), &modelMatrix );

    vkCmdDraw( cb, 36, 3, 0, 0 );
};

}  // namespace tire::vk
