
#pragma once

#include <QQuickItem>
#include <vulkan/vulkan.h>

#include "render/rendervk.h"

namespace tire {

struct RenderItem : QQuickItem {
    Q_OBJECT
    Q_PROPERTY( qreal t READ t WRITE setT NOTIFY tChanged )
    QML_ELEMENT

public:
    RenderItem( QQuickItem *parent = nullptr );

    void sync();
    void cleanup();
    qreal t() const { return t_; }
    void setT( qreal t );

signals:
    void tChanged();

private:
    void handleWindowChanged( QQuickWindow *win );

private:
    bool initialized_{ false };
    RenderVK *render_{ nullptr };
    qreal t_{};
};

}  // namespace tire
