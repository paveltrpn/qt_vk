#pragma once

#include <memory>
#include <QObject>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>
#include <QtQml>

#include "theme_manager.h"
#include "tree_model.h"
#include "list_model.h"

namespace netpet {
struct MainWindow final : QObject {
    explicit MainWindow( QObject *parent = nullptr );

    ~MainWindow() { delete window_; }

    void registerTypes();

    static QDir workPath() {
        auto wp = QDir{ QDir::currentPath() };
        wp.cdUp();
        return wp;
    };

private:
    QSettings *settings_;

    ThemeManager *theme_;
    TreeModel *treeModel_;
    ListModel *listModel_;

    QQmlEngine *engine_;
    QQmlComponent *component_;
    QQuickWindow *window_;
};
}  // namespace netpet
