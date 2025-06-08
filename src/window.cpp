
#include <format>
#include <iostream>

#include <QApplication>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QStringLiteral>
#include <QtQml>

#include "window.h"

namespace tire {

MainWindow::MainWindow( QQuickView *parent )
    : QQuickView{ parent }
    , theme_{ new ThemeManager{ workPath(), this } }

{
    std::cout << std::format( "start... work path: {}\n",
                              workPath().path().toStdString() );

    settings_ =
        new QSettings( workPath().path() + QDir::separator() + "settings.ini",
                       QSettings::NativeFormat, this );

    setResizeMode( QQuickView::SizeRootObjectToView );

    qmlRegisterSingletonInstance( "Tire", 1, 0, "Theme", theme_ );
    qmlRegisterType<RenderItem>( "Tire", 1, 0, "Render" );

    const auto mainQmlPath =
        workPath().path() + QDir::separator() + "qml/main.qml";
    setSource( QUrl( mainQmlPath ) );

    const auto restoredGeometry =
        settings_->value( "main_window_geometry", QRect( 300, 300, 640, 480 ) );

    setGeometry( restoredGeometry.toRect() );

    registerTypes();
}

void MainWindow::registerTypes() {
}

}  // namespace tire
