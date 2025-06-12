
#pragma once

#include <QSGRendererInterface>
#include <QQuickItem>

#include <vulkan/vulkan.h>

#include "render/render.h"

namespace tire {

struct RenderItem : QQuickItem {
    Q_OBJECT
    Q_PROPERTY( unsigned long long t READ t WRITE setT NOTIFY tChanged )
    QML_ELEMENT

public:
    RenderItem( QQuickItem *parent = nullptr );

    [[nodiscard]] auto t() const -> unsigned long long { return t_; }
    auto setT( unsigned long long t ) -> void;
    auto noop() -> void { log::info( "noop from render item" ); }

    auto updateSurface() -> void {
        if ( context_ ) context_->querySurface();
    }

public slots:
    auto sync() -> void;
    auto cleanup() -> void;

protected:
    auto updatePaintNode( QSGNode *node, UpdatePaintNodeData * )
        -> QSGNode * override;

private:
    auto handleWindowChanged( QQuickWindow *win ) -> void;

public slots:
    auto beforeRendering() -> void;
    auto beforeRenderPassRecording() -> void;

signals:
    auto tChanged() -> void;

    auto contextinitialized() -> void;
    auto renderInitialized() -> void;

private:
    bool initialized_{ false };

    // Cached window and render interface that this item
    // assined to. Is this pointers valid through all
    // window lifetime?
    QQuickWindow *window_{};
    QSGRendererInterface *renderInterface_{};

    std::unique_ptr<vk::Context> context_{ nullptr };
    std::unique_ptr<vk::Render> render_{ nullptr };

    // Animation (i.e. frame update on main loop) depends on this.
    unsigned long long t_{};
};

}  // namespace tire
