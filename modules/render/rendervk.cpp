
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

#include "rendervk.h"
#include "log/log.h"
static constexpr bool DEBUG_OUTPUT_RENDERVK_CPP{ true };
#include "algebra/matrix4.h"

namespace tire {

RenderVK::RenderVK( VkInstance instance, VkPhysicalDevice pDevice,
                    VkDevice device, VkRenderPass rp ) {
    try {
        context_ =
            std::make_unique<vk::Context>( instance, pDevice, device, rp );

        piplineMatrixReady_ =
            std::make_unique<vk::PiplineMatrixReady>( context_.get() );

        auto program = vk::Program{ context_.get() };
        program.fill(
            { { vk::vk_simple_box_VERTEX, vk::vertex_stage_suffix },
              { vk::vk_simple_box_FRAGMENT, vk::fragment_stage_suffix } } );

        piplineMatrixReady_->initShaderStages( program );
        piplineMatrixReady_->buildPipeline();

    } catch ( const std::runtime_error &e ) {
        throw std::runtime_error( e.what() );
    }
};

void RenderVK::frameStart(){

};

void RenderVK::mainPassRecordingStart( VkCommandBuffer cb ) {
    // Update global timer
    timer_.update();

    // =================================
    // Get transformation matricies
    auto offset = algebra::translate( 0.0f, 0.0f, -5.0f );
    offset.transposeSelf();

    const auto [width, height] = context_->currentExtent();
    const auto proj = algebra::vperspective<float>(
        50.0f, static_cast<float>( width ) / static_cast<float>( height ), 0.1f,
        25.0f );
    const auto viewMatrix = offset * proj;
    angle_ += timer_.floatFrameDuration() * 25.0f;
    algebra::vector3f ax{ 0.0f, 1.0f, 1.0f };
    ax.normalizeSelf();
    const auto modelMatrix = algebra::rotate( ax, angle_ );
    // =================================

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

}  // namespace tire
