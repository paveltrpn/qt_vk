#pragma once

#include <memory>
#include <QObject>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QtQml>
#include <QQuickView>

#include "theme_manager.h"
#include "qt_items/render_item.h"

namespace tire {

struct MainWindow final : QQuickView {
    explicit MainWindow( QQuickView *parent = nullptr );

    ~MainWindow() override = default;

    void registerTypes();

    static QDir workPath() {
        auto wp = QDir{ QDir::currentPath() };
        wp.cdUp();
        return wp;
    };

private:
    QSettings *settings_;

    ThemeManager *theme_;
};

}  // namespace tire
