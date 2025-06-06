
#include "theme_manager.h"
#include <iostream>

#include <QFile>
#include <QJsonParseError>

namespace netpet {

ThemeManager::ThemeManager( const QDir &path, QObject *parent )
    : QObject{ parent } {
    QFile file( path.path() + QDir::separator() + "assets/colors/colors.json" );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    const QByteArray data = file.readAll();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson( data, &parseError );
    if ( parseError.error != QJsonParseError::NoError ) {
        std::cout << std::format(
            "color theme json parse failed with message: {}",
            parseError.errorString().toStdString() );
        return;
    } else {
        // std::cout << doc.toJson( QJsonDocument::Indented ).toStdString();
        colors_ = doc.object();
    }
}

}  // namespace netpet
