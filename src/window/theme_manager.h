#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QJsonObject>
#include <QDir>

namespace netpet
{
struct ThemeManager final : QObject {
    Q_OBJECT
    Q_PROPERTY( QJsonObject colors READ colors WRITE setColors MEMBER colors_ NOTIFY colorsChanged )

public:
    ThemeManager( const QDir &path, QObject *parent = nullptr );

    Q_INVOKABLE QJsonObject colors() { return colors_; };
    Q_INVOKABLE void setColors( QJsonObject &value ) { colors_ = value; };

signals:
    void colorsChanged();

private:
    QJsonObject colors_{};
};
} // namespace netpet
