
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

namespace netpet {

MainWindow::MainWindow( QObject *parent )
    : QObject{ parent }
    , theme_{ new ThemeManager{ workPath(), this } }
    , treeModel_{ new TreeModel{ this } }
    , listModel_{ new ListModel{ this } }
    , engine_{ new QQmlEngine{ this } } {
    std::cout << std::format( "start... work path: {}\n",
                              workPath().path().toStdString() );

    settings_ =
        new QSettings( workPath().path() + QDir::separator() + "settings.ini",
                       QSettings::NativeFormat, this );

    connect( engine_, &QQmlEngine::quit, this, [this]() {
        settings_->setValue( "main_window_geometry", window_->geometry() );
        QApplication::quit();
    } );

    registerTypes();

    // engine_->addImportPath( );

    const auto mainQmlPath =
        workPath().path() + QDir::separator() + "qml/main.qml";
    component_ = new QQmlComponent( engine_, mainQmlPath,
                                    dynamic_cast<QObject *>( this ) );

    if ( component_->status() == QQmlComponent::Status::Error ) {
        std::cout << std::format( "component creation error: {}\n",
                                  component_->errorString().toStdString() );
    }

    const auto object = component_->create();

    window_ = dynamic_cast<QQuickWindow *>( object );
    if ( window_ == nullptr ) {
        std::cout << std::format( "MainWindow bad cast!\n" );
    }

    const auto restoredGeometry =
        settings_->value( "main_window_geometry", QRect( 300, 300, 640, 480 ) );
    window_->setGeometry( restoredGeometry.toRect() );

    std::vector<QString> firsts{ "top level one", "top level two",
                                 "top level three" };
    std::vector<QString> seconds{ "sec level one", "sec level two",
                                  "sec level three" };
    std::vector<QString> thirds{ "tre level one", "tre level two",
                                 "tre level three" };

    treeModel_->populate( "names_tree.json" );

    listModel_->populate( "names_list.json" );
}

void MainWindow::registerTypes() {
    qmlRegisterSingletonInstance( "NetPet", 1, 0, "Theme", theme_ );
    qmlRegisterSingletonInstance( "NetPet", 1, 0, "LocalTreeModel",
                                  treeModel_ );
    qmlRegisterSingletonInstance( "NetPet", 1, 0, "LocalListModel",
                                  listModel_ );
}

}  // namespace netpet
