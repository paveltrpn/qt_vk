
#include <iostream>

#include <QFont>

#include "appearance.h"

namespace tire {

Appearance::Appearance( const QDir &path, QObject *parent )
    : QObject{ parent } {
    // Load color scheme.
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

    // Build fonts map
    fonts_["title_big"] = QFont{ "Roboto Mono", 32, QFont::ExtraBold };
    fonts_["title_accent"] = QFont{ "Roboto Mono", 24, QFont::ExtraBold };
    fonts_["subtitle_accent"] = QFont{ "Roboto Mono", 24, QFont::Medium };
    fonts_["button_accent"] = QFont{ "Roboto Mono", 20, QFont::ExtraBold };
    fonts_["button"] = QFont{ "Roboto Mono", 16, QFont::ExtraBold };
    fonts_["button_middle"] = QFont{ "Roboto Mono", 16, QFont::Medium };
    fonts_["button_middle_subtext"] =
        QFont{ "Roboto Mono", 14, QFont::ExtraBold };
    fonts_["text_body_accent"] = QFont{ "Roboto Mono", 12, QFont::ExtraBold };
    fonts_["text_body"] = QFont{ "Roboto Mono", 16, QFont::Medium };
    fonts_["label_accent"] = QFont{ "Roboto Mono", 14, QFont::ExtraBold };
    fonts_["label"] = QFont{ "Roboto Mono", 14, QFont::Medium };
    fonts_["subtext_accent"] = QFont{ "Roboto Mono", 12, QFont::ExtraBold };
    fonts_["subtext"] = QFont{ "Roboto Mono", 12, QFont::Medium };

    // Build gaps map
    fonts_["quarter"] = 4;
    fonts_["half"] = 8;
    fonts_["full"] = 16;

    // Build radius map
    radius_["quarter"] = 4;
    radius_["half"] = 8;
    radius_["full"] = 16;
}

}  // namespace tire
