
#pragma once

#include <iostream>
#include <memory>
#include <QString>
#include <QList>
#include <QAbstractListModel>
#include <QJsonArray>

namespace netpet {

struct ListModelItem final {
    QString name() const { return _name; };
    int level() const { return _level; };
    bool isOpened() const { return _opened; };

    void setName( const QString &value ) { _name = value; };
    void setLevel( int value ) { _level = value; };
    void setIsOpened( bool value ) { _opened = value; }

    QString _name{};
    int _level{ 0 };
    bool _opened{ false };
};

struct ListModel : QAbstractListModel {
    Q_OBJECT
public:
    enum ProfilePlanModelRoles {
        NameRole = Qt::UserRole + 1,
        LevelRole,
        IsOpenedRole,
    };

    explicit ListModel( QObject *parent = nullptr )
        : QAbstractListModel{ parent } {}

    QVariant data( const QModelIndex &index,
                   int role = Qt::DisplayRole ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE void move( int from, int to );

    void populate( const std::vector<QString> &list );
    void populate( const QString &name );

    Q_INVOKABLE void reload();

private:
    // srvice
    QList<std::shared_ptr<ListModelItem>> _model{};

    QString sourceFile_{};
};

}  // namespace netpet
