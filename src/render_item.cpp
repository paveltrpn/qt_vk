
#include <QQuickWindow>
#include <QVulkanInstance>
#include <rhi/qrhi.h>

#include "render_item.h"

namespace tire {

RenderItem::RenderItem( QQuickItem *parent )
    : QQuickItem{ parent } {
    setFlag( QQuickItem::ItemHasContents );
    connect( this, &QQuickItem::windowChanged, this,
             &RenderItem::handleWindowChanged );
}

auto RenderItem::updatePaintNode( QSGNode *node, UpdatePaintNodeData * )
    -> QSGNode * {
    window()->update();  // ensure getting to beforeRendering() at some point
    return node;
};

void RenderItem::handleWindowChanged( QQuickWindow *win ) {
    if ( win ) {
        connect( win, &QQuickWindow::beforeSynchronizing, this,
                 &RenderItem::sync, Qt::DirectConnection );
        connect( win, &QQuickWindow::sceneGraphInvalidated, this,
                 &RenderItem::cleanup, Qt::DirectConnection );

        win->setColor( "#0c0c1c" );
    }
}

void RenderItem::cleanup() {
    log::info( "RenderItem === cleanup.." );
}

void RenderItem::setT( unsigned long long t ) {
    if ( window_ ) {
        window_->update();
    }
}

void RenderItem::beforeRendering() {
    if ( !initialized_ ) {
        // Acquire vulkan resurces, initialized by Qt and provided
        // by QRhi interface and window.
        // This resources accumulated in vk::Context object
        // and used by vk::Render.

        const auto rhiHandle =
            reinterpret_cast<QRhi *>( renderInterface_->getResource(
                window_, QSGRendererInterface::RhiResource ) );

        qDebug() << rhiHandle->driverInfo();

        const auto nh = static_cast<const QRhiVulkanNativeHandles *>(
            rhiHandle->nativeHandles() );

        // Vulkan instance from QRhi.
        const auto inst = nh->inst;

        // Vulkan instance from window.
        // const auto inst =
        // reinterpret_cast<QVulkanInstance *>( renderInterface_->getResource(
        // window_, QSGRendererInterface::VulkanInstanceResource ) );

        // VkSurfaceKHR assiciated with Qt window.
        const auto sface = inst->surfaceForWindow( window_ );

        // Chosen physical device.
        const auto physDev = *reinterpret_cast<VkPhysicalDevice *>(
            renderInterface_->getResource(
                window_, QSGRendererInterface::PhysicalDeviceResource ) );

        // Acquired logical device.
        const auto dev =
            *reinterpret_cast<VkDevice *>( renderInterface_->getResource(
                window_, QSGRendererInterface::DeviceResource ) );

        // Render pass.
        const auto rp =
            *reinterpret_cast<VkRenderPass *>( renderInterface_->getResource(
                window_, QSGRendererInterface::RenderPassResource ) );

        // Graphics queue family index used by the scenegraph.
        const auto gqfi =
            *reinterpret_cast<uint32_t *>( renderInterface_->getResource(
                window_,
                QSGRendererInterface::GraphicsQueueFamilyIndexResource ) );

        // Graphics queue index.
        const auto gqi =
            *reinterpret_cast<uint32_t *>( renderInterface_->getResource(
                window_, QSGRendererInterface::GraphicsQueueIndexResource ) );

        context_ = std::make_unique<vk::Context>( inst->vkInstance(), physDev,
                                                  dev, sface, rp, gqfi, gqi );

        emit contextinitialized();

        render_->init( context_.get() );
        initialized_ = true;
        emit renderInitialized();
    }
}

void RenderItem::beforeRenderPassRecording() {
    window_->beginExternalCommands();

    // Must query the command buffer _after_ beginExternalCommands(), this is
    // actually important when running on Vulkan because what we get here is a
    // new secondary command buffer, not the primary one.
    // This object has limited validity, and is only valid while the scene
    // graph is preparing the next frame.
    const auto cb =
        *reinterpret_cast<VkCommandBuffer *>( renderInterface_->getResource(
            window_, QSGRendererInterface::CommandListResource ) );

    render_->frame( cb );

    window_->endExternalCommands();
}

void RenderItem::sync() {
    if ( !render_ ) {
        window_ = window();
        if ( !window_ ) {
            qDebug() << "RenderItem === bad qquickitem window...";
        }

        renderInterface_ = window_->rendererInterface();
        if ( !renderInterface_ ) {
            qDebug() << "RenderItem === bad qquickitem render interface...";
        }

        render_ = std::make_unique<vk::Render>();

        // Initializing resources is done before starting to record the
        // renderpass, regardless of wanting an underlay or overlay.
        // Most of vulkan resources already allocated by Qt at this point.
        connect( window_, &QQuickWindow::beforeRendering, this,
                 &RenderItem::beforeRendering, Qt::DirectConnection );

        // Here we want an underlay and therefore connect to
        // beforeRenderPassRecording. Changing to afterRenderPassRecording
        // would render on top (overlay).
        connect( window_, &QQuickWindow::beforeRenderPassRecording, this,
                 &RenderItem::beforeRenderPassRecording, Qt::DirectConnection );
    }
}

}  // namespace tire
