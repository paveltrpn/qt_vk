
#include <format>
#include <iostream>

#include <QApplication>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QStringLiteral>
#include <QtQml>

#include "log/log.h"
#include "window.h"

namespace tire {

MainWindow::MainWindow( QQuickView *parent )
    : QQuickView{ parent }
    , engine_{ engine() }
    , context_{ rootContext() }
    , settings_{ new QSettings(
          workPath().path() + QDir::separator() + "settings.ini",
          QSettings::NativeFormat, this ) }
    , theme_{ new Appearance{ workPath(), this } }

{
    QSGRendererInterface *rif = this->rendererInterface();
    // We are not prepared for anything other than running with the RHI and its Vulkan backend.
    if ( rif->graphicsApi() != QSGRendererInterface::Vulkan ) {
        log::fatal(
            "MainWindow === qt render backend based not on Vulkan api!" );
    } else {
        log::info( "MainWindow === qt render backend assert success" );
    }

    log::info( "MainWindow === start... work path: {}",
               workPath().path().toStdString() );

    // Configure main window.
    setResizeMode( QQuickView::SizeRootObjectToView );

    // Actions that must be processed before main QML component created.
    qmlRegisterSingletonInstance( "Tire", 1, 0, "Appearence", theme_ );
    qmlRegisterType<RenderItem>( "Tire", 1, 0, "Render" );

    // Pass this pointer to qml.
    context_->setContextProperty( "mainQuickViewHandle", this );

    // Setup RenderItem update interval.
    update_.setInterval( 1 );

    // Load main QML component.
    connect(
        this, &QQuickView::statusChanged, this,
        [this]( QQuickView::Status status ) {
            switch ( status ) {
                case QQuickView::Error: {
                    log::fatal(
                        "MainWindow === main QML component "
                        "error!" );
                    break;
                }
                case QQuickView::Loading: {
                    log::info(
                        "MainWindow === main QML component loading... " );
                    break;
                }
                case QQuickView::Ready: {
                    log::info( "MainWindow === main QML component ready." );

                    // Get main renderer handle from QML.
                    renderItemHandle_ = rootObject()->findChild<RenderItem *>();
                    if ( !renderItemHandle_ ) {
                        log::warning(
                            "MainWindow === can't acquire renderer "
                            "handle!" );
                    }

                    // Start update timer.
                    update_.start();

                    // Call updateWindow to redraw qml item.
                    connect( &update_, &QTimer::timeout, renderItemHandle_,
                             &RenderItem::updateWindow );

                    connect(
                        renderItemHandle_, &RenderItem::renderInitialized, this,
                        []() {
                            //
                            log::info( "MainWindow === render initialized!" );
                        } );
                    break;
                }
                case QQuickView::Null: {
                    log::warning(
                        "MainWindow === main QML component is NULL." );
                    break;
                }
            }
        } );
    setSource( QUrl( workPath().path() + QDir::separator() + "qml/main.qml" ) );

    // Deal with with main window geometry.
    const auto restoredGeometry =
        settings_->value( "main_window_geometry", QRect( 300, 300, 640, 480 ) );
    setGeometry( restoredGeometry.toRect() );

    // Process quit actions.
    connect( engine_, &QQmlEngine::quit, this,
             [this]() { QApplication::quit(); } );
}

void MainWindow::closeEvent( QCloseEvent *ev ) {
    log::info( "quit" );

    // NOTE: renderer object is still alive here!
    renderItemHandle_->noop();

    // Save window geometry at close.
    settings_->setValue( "main_window_geometry", geometry() );
}

void MainWindow::keyPressEvent( QKeyEvent *ev ) {
    switch ( ev->key() ) {
        case Qt::Key_Escape: {
            QApplication::quit();
            break;
        }
        default: {
            break;
        }
    }
}

void MainWindow::keyReleaseEvent( QKeyEvent *ev ) {
}

void MainWindow::mouseMoveEvent( QMouseEvent *ev ) {
}

void MainWindow::mousePressEvent( QMouseEvent *ev ) {
}

void MainWindow::mouseReleaseEvent( QMouseEvent *ev ) {
}

void MainWindow::resizeEvent( QResizeEvent *ev ) {
    // qDebug() << ev->size();

    if ( renderItemHandle_ ) renderItemHandle_->updateSurface();
}

}  // namespace tire
