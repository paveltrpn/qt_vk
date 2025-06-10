
#pragma once

#include <QSGRendererInterface>
#include <QQuickItem>

#include <vulkan/vulkan.h>

#include "render/rendervk.h"

namespace tire {

struct RenderItem : QQuickItem {
    Q_OBJECT
    Q_PROPERTY( unsigned long long t READ t WRITE setT NOTIFY tChanged )
    QML_ELEMENT

public:
    RenderItem( QQuickItem *parent = nullptr );

    [[nodiscard]] unsigned long long t() const { return t_; }
    void setT( unsigned long long t );

    void noop() { log::info( "noop from render item" ); }

    void updateSurface() {
        if ( render_ ) render_->updateSurface();
    }

public slots:
    void sync();
    void cleanup();

private:
    void handleWindowChanged( QQuickWindow *win );

public slots:
    void beforeRendering();
    void beforeRenderPassRecording();

signals:
    void tChanged();

private:
    bool initialized_{ false };

    // Cached window and render interface that this item
    // assined to. Is this pointers valid trough all
    // window lifetime?
    QQuickWindow *window_{};
    QSGRendererInterface *renderInterface_{};

    RenderVK *render_{ nullptr };

    unsigned long long t_{};
};

}  // namespace tire
