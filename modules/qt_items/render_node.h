
#pragma once

#include <QSGRenderNode>
#include <QQuickItem>

namespace tire {

struct RenderNode final : QSGRenderNode {
    ~RenderNode();

    void render( const RenderState *state ) override;
    void releaseResources() override;
    QSGRenderNode::RenderingFlags flags() const override;
    QRectF rect() const override;

    void sync( QQuickItem *item );

private:
};

}  // namespace tire
