#pragma once

#include <memory>
#include <QObject>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QtQml>
#include <QQuickView>

#include "theme_manager.h"
#include "render_item.h"

namespace tire {

struct MainWindow final : QQuickView {
    Q_OBJECT

public:
    explicit MainWindow( QQuickView *parent = nullptr );

    ~MainWindow() override = default;

    static QDir workPath() {
        auto wp = QDir{ QDir::currentPath() };
        wp.cdUp();
        return wp;
    };

    Q_INVOKABLE void noop() { log::info( "noop from main window" ); }

private:
    QQmlEngine *engine_;
    QQmlContext *context_;
    QSettings *settings_;
    ThemeManager *theme_;

    RenderItem *renderer_{};
};

}  // namespace tire
