
#include <iostream>

#include <QApplication>
#include <QStyleHints>
#include <QVulkanInstance>

#include "window.h"
#include "log/log.h"

int main( int argc, char *argv[] ) {
    QGuiApplication app( argc, argv );

    tire::log::info( "Qt version: {}", qVersion() );

    // Force use vulkan as backend renderer API.
    QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );

    // Instance to config and pass to qt.
    QVulkanInstance inst;

    // NOTE: have no effect.
    inst.setApiVersion(  {1, 3, 0}  );

    // inst.setLayers({"VK_LAYER_KHRONOS_validation"});

    // inst.setExtensions({"VK_KHR_surface"});

    if ( !inst.create() ) {
        qDebug() << "bad vk instance";
        return 1;
    }

    tire::MainWindow view{};
    view.setVulkanInstance( &inst );
    view.show();

    // Main loop starts here.
    return app.exec();
}
