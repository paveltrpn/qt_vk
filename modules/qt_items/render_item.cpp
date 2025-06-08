
#include <QSGRenderNode>
#include <QSGRendererInterface>
#include <QQuickWindow>
#include <QVulkanInstance>

#include "render_item.h"
#include "render_node.h"

namespace tire {

RenderItem::RenderItem( QQuickItem *parent )
    : QQuickItem{ parent } {
    setFlag( QQuickItem::ItemHasContents );
    connect( this, &QQuickItem::windowChanged, this,
             &RenderItem::handleWindowChanged );
}

void RenderItem::handleWindowChanged( QQuickWindow *win ) {
    if ( win ) {
        connect( win, &QQuickWindow::beforeSynchronizing, this,
                 &RenderItem::sync, Qt::DirectConnection );
        connect( win, &QQuickWindow::sceneGraphInvalidated, this,
                 &RenderItem::cleanup, Qt::DirectConnection );

        // Ensure we start with cleared to black. The squircle's blend mode relies on this.
        win->setColor( Qt::black );
    }
}

void RenderItem::cleanup() {
}

void RenderItem::sync() {
    auto wnd = window();

    if ( !render_ ) {
        QSGRendererInterface *rif = wnd->rendererInterface();

        QVulkanInstance *inst =
            reinterpret_cast<QVulkanInstance *>( rif->getResource(
                wnd, QSGRendererInterface::VulkanInstanceResource ) );

        const auto physDev =
            *reinterpret_cast<VkPhysicalDevice *>( rif->getResource(
                wnd, QSGRendererInterface::PhysicalDeviceResource ) );

        const auto dev = *reinterpret_cast<VkDevice *>(
            rif->getResource( wnd, QSGRendererInterface::DeviceResource ) );

        VkRenderPass rp = *reinterpret_cast<VkRenderPass *>(
            rif->getResource( wnd, QSGRendererInterface::RenderPassResource ) );

        render_ = new RenderVK{ inst->vkInstance(), physDev, dev, rp };

        // Initializing resources is done before starting to record the
        // renderpass, regardless of wanting an underlay or overlay.
        connect(
            wnd, &QQuickWindow::beforeRendering, this,
            [this]() {
                //
                render_->frameStart();
            },
            Qt::DirectConnection );

        // Here we want an underlay and therefore connect to
        // beforeRenderPassRecording. Changing to afterRenderPassRecording
        // would render the squircle on top (overlay).
        connect(
            wnd, &QQuickWindow::beforeRenderPassRecording, this,
            [this, wnd]() {
                //
                QSGRendererInterface *rif = wnd->rendererInterface();

                wnd->beginExternalCommands();

                // Must query the command buffer _after_ beginExternalCommands(), this is
                // actually important when running on Vulkan because what we get here is a
                // new secondary command buffer, not the primary one.
                VkCommandBuffer cb =
                    *reinterpret_cast<VkCommandBuffer *>( rif->getResource(
                        wnd, QSGRendererInterface::CommandListResource ) );
                render_->mainPassRecordingStart( cb );
                wnd->endExternalCommands();
            },
            Qt::DirectConnection );
    }

    //render_->setViewportSize( window()->size() * window()->devicePixelRatio() );
}

}  // namespace tire
