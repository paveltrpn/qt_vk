
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <span>

#include "rendervk.h"

namespace tire {

void RenderVK::preLoop() {
    log::notice( "render loop starts here..." );
};

void RenderVK::preFrame() {
    // Update global timer
    timer_.update();
};

void RenderVK::frame() {
    const auto [iaSem, rfSem, ifFnc] =
        context_->getFrameSyncSet( currentFrame_ );

    // NOTE: omit return code check
    vkWaitForFences( context_->device(), 1, &ifFnc, VK_TRUE, UINT64_MAX );

    // NOTE: omit return code check
    vkResetFences( context_->device(), 1, &ifFnc );

    // NOTE: omit return code check
    // May return VK_SUBOPTIMAL_KHR or even VK_ERROR_OUT_OF_DATE_KHR
    // if current surface properties are no longer matched
    // exactly or swap chain has become incompatible
    // with the surface and can no longer be used for rendering
    uint32_t imageIndex{};
    vkAcquireNextImageKHR( context_->device(), context_->swapchain(),
                           UINT64_MAX, iaSem, VK_NULL_HANDLE, &imageIndex );

    // NOTE: currentFrame_->imageIndex
    const auto currentFramebuffer = context_->framebuffer( currentFrame_ );

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

    renderCommand_->reset();
    renderCommand_->prepare( currentFramebuffer, viewMatrix, modelMatrix );
    renderCommand_->submit( iaSem, rfSem, ifFnc );

    context_->present( rfSem, &currentFrame_ );

    currentFrame_ = ( currentFrame_ + 1 ) % context_->framesCount();
};

void RenderVK::postFrame(){

};

void RenderVK::swapBuffers(){

};

void RenderVK::postLoop() {
    log::info( "vk::Render === wait device idle..." );

    // we should wait for the logical device to finish operations
    // before exiting mainLoop and destroying the window
    vkDeviceWaitIdle( context_->device() );
};

}  // namespace tire
