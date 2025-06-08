
#include <iostream>

#include <QApplication>
#include <QStyleHints>

#include "window.h"

int main( int argc, char *argv[] ) {
    QGuiApplication app( argc, argv );

    QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );

    std::cout << "Qt version: " << qVersion() << "\n";

    [[maybe_unused]] auto *mainWindow = new tire::MainWindow( &app );

    return app.exec();
}
