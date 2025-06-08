
#include <iostream>

#include <QApplication>
#include <QStyleHints>

#include "window.h"

int main( int argc, char *argv[] ) {
    QGuiApplication app( argc, argv );

    QQuickWindow::setGraphicsApi( QSGRendererInterface::Vulkan );

    std::cout << "Qt version: " << qVersion() << "\n";

    tire::MainWindow view;
    view.show();

    return app.exec();
}
