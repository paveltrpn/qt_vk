
#pragma once

#include <QQuickItem>

namespace tire {

struct RenderItem final : QQuickItem {
    Q_OBJECT
    QML_ELEMENT

    RenderItem( QQuickItem *parent = nullptr );
    QSGNode *updatePaintNode( QSGNode *node, UpdatePaintNodeData * ) override;

private:
};

}  // namespace tire
