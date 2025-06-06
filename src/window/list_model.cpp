
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QJsonParseError>

#include "list_model.h"

namespace netpet {

QVariant ListModel::data( const QModelIndex &index, int role ) const {
    if ( !index.isValid() ) return {};
    if ( index.row() > ( _model.size() - 1 ) ) return {};
    ListModelItem *item = _model.at( index.row() ).get();
    switch ( role ) {
        case Qt::DisplayRole:
        case NameRole: {
            return QVariant::fromValue( item->name() );
        }
        case LevelRole:
            return QVariant::fromValue( item->level() );
        case IsOpenedRole:
            return QVariant::fromValue( item->isOpened() );
        default:
            return {};
    }
}

int ListModel::rowCount( const QModelIndex &parent ) const {
    return _model.size();
}

QHash<int, QByteArray> ListModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[NameRole] = "name";
    roles[LevelRole] = "level";
    roles[IsOpenedRole] = "isOpened";

    return roles;
}

void ListModel::populate( const std::vector<QString> &list ) {
    beginResetModel();
    _model.clear();

    for ( const auto &item : list ) {
        auto itemSharedObject =
            std::make_shared<ListModelItem>( ListModelItem{ item, 0, false } );
        _model.push_back( std::move( itemSharedObject ) );
    }

    endResetModel();
}

void ListModel::populate( const QString &name ) {
    auto workPath = []() -> QDir {
        auto wp = QDir{ QDir::currentPath() };
        wp.cdUp();
        return wp;
    };

    sourceFile_ = name;
    QFile file(
        QString{ "%1/%2/%3" }.arg( workPath().path(), "assets", sourceFile_ ) );
    file.open( QIODevice::ReadOnly | QIODevice::Text );
    const QByteArray data = file.readAll();

    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson( data, &parseError );
    if ( parseError.error != QJsonParseError::NoError ) {
        std::cout << std::format(
            "list model data json parse failed with message: {}",
            parseError.errorString().toStdString() );
        return;
    }

    const QJsonArray array = doc.object().value( "data" ).toArray();

    beginResetModel();
    _model.clear();

    for ( const auto &item : array.toVariantList() ) {
        const auto &name = item.toJsonObject().value( "name" ).toString();
        const auto level = item.toJsonObject().value( "level" ).toBool();
        const auto opened = item.toJsonObject().value( "isOpened" ).toBool();

        auto itemSharedObject = std::make_shared<ListModelItem>(
            ListModelItem{ name, level, opened } );
        _model.push_back( std::move( itemSharedObject ) );
    }

    endResetModel();
}

void ListModel::move( int from, int to ) {
    if ( from >= 0 && from < rowCount() && to >= 0 && to < rowCount() &&
         from != to ) {
        if ( from == to - 1 ) {  // Allow item moving to the bottom
            to = from++;
        }

        beginResetModel();
        //beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
        _model.move( from, to );  // update backing QList
        //endMoveRows();
        endResetModel();
    }
}

void ListModel::reload() {
    populate( sourceFile_ );
}

}  // namespace netpet
