
#include <iostream>

#include <QApplication>
#include <QStyleHints>

#include "window/window.h"

int main( int argc, char *argv[] ) {
    QGuiApplication app( argc, argv );
    QGuiApplication::styleHints()->setStartDragDistance( 10 );

    std::cout << "Qt version: " << qVersion() << "\n";

    auto *mainWindow = new netpet::MainWindow( &app );
    Q_UNUSED( mainWindow )
    return app.exec();
}
