
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
    , engine_{ engine() }
    , context_{ rootContext() }
    , theme_{ new ThemeManager{ workPath(), this } }

{
    std::cout << std::format( "start... work path: {}\n",
                              workPath().path().toStdString() );

    // NOTE:
    setFlag( Qt::FramelessWindowHint, true );

    settings_ =
        new QSettings( workPath().path() + QDir::separator() + "settings.ini",
                       QSettings::NativeFormat, this );

    connect( engine_, &QQmlEngine::quit, this, [this]() {
        log::info( "quit" );
        renderer_->noop();
        settings_->setValue( "main_window_geometry", geometry() );
        QApplication::quit();
    } );

    setResizeMode( QQuickView::SizeRootObjectToView );

    qmlRegisterSingletonInstance( "Tire", 1, 0, "Theme", theme_ );
    qmlRegisterType<RenderItem>( "Tire", 1, 0, "Render" );

    // Pass this pointer to qml.
    context_->setContextProperty( "quickViewHandle", this );

    const auto mainQmlPath =
        workPath().path() + QDir::separator() + "qml/main.qml";
    setSource( QUrl( mainQmlPath ) );

    const auto restoredGeometry =
        settings_->value( "main_window_geometry", QRect( 300, 300, 640, 480 ) );

    setGeometry( restoredGeometry.toRect() );

    renderer_ =
        dynamic_cast<RenderItem *>( context_->objectForName( "renderer" ) );
}

}  // namespace tire
