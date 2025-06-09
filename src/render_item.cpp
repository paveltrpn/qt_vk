
#include <QSGRendererInterface>
#include <QQuickWindow>
#include <QVulkanInstance>

#include "render_item.h"

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

        win->setColor( Qt::black );
    } else {
        qDebug() << "RenderItem handleWindowChanged no win..";
    }
}

void RenderItem::cleanup() {
    qDebug() << "RenderItem cleanup..";
}

void RenderItem::setT( qreal t ) {
    if ( t == t_ ) return;
    t_ = t;
    emit tChanged();
    if ( window() ) {
        window()->update();
    }
}

void RenderItem::sync() {
    auto wnd = window();
    if ( !wnd ) {
        qDebug() << "RenderItem !wnd..";
    }

    if ( !render_ ) {
        render_ = new RenderVK{};

        // Initializing resources is done before starting to record the
        // renderpass, regardless of wanting an underlay or overlay.
        connect(
            wnd, &QQuickWindow::beforeRendering, this,
            [this, wnd]() {
                if ( !initialized_ ) {
                    const auto *rif = wnd->rendererInterface();

                    const auto inst =
                        reinterpret_cast<QVulkanInstance *>( rif->getResource(
                            wnd,
                            QSGRendererInterface::VulkanInstanceResource ) );
                    if ( !inst ) {
                        qDebug() << "RenderItem !inst..";
                    }

                    const auto physDev =
                        *reinterpret_cast<VkPhysicalDevice *>( rif->getResource(
                            wnd,
                            QSGRendererInterface::PhysicalDeviceResource ) );

                    const auto dev =
                        *reinterpret_cast<VkDevice *>( rif->getResource(
                            wnd, QSGRendererInterface::DeviceResource ) );

                    const auto rp =
                        *reinterpret_cast<VkRenderPass *>( rif->getResource(
                            wnd, QSGRendererInterface::RenderPassResource ) );

                    render_->init( inst->vkInstance(), physDev, dev, rp );

                    initialized_ = true;
                }
            },
            Qt::DirectConnection );

        // Here we want an underlay and therefore connect to
        // beforeRenderPassRecording. Changing to afterRenderPassRecording
        // would render the squircle on top (overlay).
        connect(
            wnd, &QQuickWindow::beforeRenderPassRecording, this,
            [this]() {
                auto w = this->window();
                const auto *rif = w->rendererInterface();

                w->beginExternalCommands();

                // Must query the command buffer _after_ beginExternalCommands(), this is
                // actually important when running on Vulkan because what we get here is a
                // new secondary command buffer, not the primary one.
                const auto cb =
                    *reinterpret_cast<VkCommandBuffer *>( rif->getResource(
                        w, QSGRendererInterface::CommandListResource ) );

                render_->mainPassRecordingStart( cb );

                w->endExternalCommands();
            },
            Qt::DirectConnection );
    }
    //render_->setViewportSize( window()->size() * window()->devicePixelRatio() );
}

}  // namespace tire
