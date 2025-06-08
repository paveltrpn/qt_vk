
#include <QSGRenderNode>
#include <QSGRendererInterface>
#include <QQuickWindow>

#include "render_item.h"
#include "render_node.h"

namespace tire {

RenderItem::RenderItem( QQuickItem *parent )
    : QQuickItem{ parent } {
    setFlag( QQuickItem::ItemHasContents );
}

QSGNode *RenderItem::updatePaintNode( QSGNode *node, UpdatePaintNodeData * ) {
    auto *n = static_cast<QSGRenderNode *>( node );

    QSGRendererInterface *ri = window()->rendererInterface();
    if ( !ri ) {
        return nullptr;
    }

    if ( !n ) {
        n = new RenderNode;
    }

    static_cast<RenderNode *>( n )->sync( this );

    if ( !n )
        qWarning( "QSGRendererInterface reports unknown graphics API %d",
                  ri->graphicsApi() );

    return n;
}

}  // namespace tire
