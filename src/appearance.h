
#pragma once

#include <QObject>
#include <QJsonObject>
#include <QDir>
#include <QQuickImageProvider>

namespace tire {

// Application-wide image provide.
struct ImageProvider : QQuickImageProvider {
private:
};

// Colors and other theme related provider. Instance of
// this object available in qml.
struct Appearance : QObject {
    Q_OBJECT
    Q_PROPERTY( QJsonObject colors READ colors WRITE setColors MEMBER colors_
                    NOTIFY colorsChanged )

public:
    Appearance( const QDir &path, QObject *parent = nullptr );

    Q_INVOKABLE QJsonObject colors() { return colors_; };
    Q_INVOKABLE void setColors( QJsonObject &value ) { colors_ = value; };

signals:
    void colorsChanged();

private:
    QJsonObject colors_{};
};

}  // namespace tire
