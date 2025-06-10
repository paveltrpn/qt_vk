
#include <iostream>

#include <QApplication>
#include <QStyleHints>
#include <QVulkanInstance>

#include "window.h"
#include "log/log.h"

int main( int argc, char *argv[] ) {
    QGuiApplication app( argc, argv );

    tire::log::info( "Qt version: {}", qVersion() );

    QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );

    QVulkanInstance inst;

    // NOTE: have no effect.
    inst.setApiVersion( { 1, 3 } );

    if ( !inst.create() ) {
        qDebug() << "bad vk instance";
        return 1;
    }

    tire::MainWindow view{};
    view.setVulkanInstance( &inst );
    view.show();

    return app.exec();
}
