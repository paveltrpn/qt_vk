#pragma once

#include <memory>
#include <QObject>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QtQml>

#include "theme_manager.h"
#include "qt_items/render_item.h"

namespace tire {

struct MainWindow final : QObject {
    explicit MainWindow( QObject *parent = nullptr );

    ~MainWindow() override { delete window_; }

    void registerTypes();

    static QDir workPath() {
        auto wp = QDir{ QDir::currentPath() };
        wp.cdUp();
        return wp;
    };

private:
    QSettings *settings_;

    ThemeManager *theme_;
    std::unique_ptr<RenderItem> renderItem_;

    QQmlEngine *engine_;
    QQmlComponent *component_;
    QQuickWindow *window_;
};

}  // namespace tire
