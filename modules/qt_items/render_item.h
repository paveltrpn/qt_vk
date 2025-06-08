
#pragma once

#include <QQuickItem>
#include <vulkan/vulkan.h>

#include "render/rendervk.h"

namespace tire {

struct RenderItem final : QQuickItem {
    Q_OBJECT
    QML_ELEMENT

public:
    RenderItem( QQuickItem *parent = nullptr );
    void sync();
    void cleanup(){};

    void handleWindowChanged( QQuickWindow *win );

private:
    RenderVK *render_;
};

}  // namespace tire
