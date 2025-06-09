
#include <iostream>

#include <QApplication>
#include <QStyleHints>
#include <QVulkanInstance>

#include "window.h"

int main( int argc, char *argv[] ) {
    QGuiApplication app( argc, argv );

    std::cout << "Qt version: " << qVersion() << "\n";

    QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );

    QVulkanInstance inst;

    inst.setApiVersion( { 1, 3 } );

    if ( !inst.create() ) {
        qDebug() << "bad vk instance";
        return 1;
    }

    tire::MainWindow view;
    view.setVulkanInstance( &inst );
    view.show();

    return app.exec();
}
