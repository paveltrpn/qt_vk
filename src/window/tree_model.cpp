
#include <iostream>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QJsonParseError>
#include <QJsonArray>
#include <QCryptographicHash>

#include "tree_model.h"

namespace netpet {

QVariant TreeModel::data( const QModelIndex &index, int role ) const {
    if ( !index.isValid() ) return {};
    if ( index.row() > ( _model.size() - 1 ) ) return {};
    TreeModelItem *item = _model.at( index.row() ).get();
    switch ( role ) {
        case Qt::DisplayRole:
        case NameRole: {
            return QVariant::fromValue( item->name() );
        }
        case LevelRole:
            return QVariant::fromValue( item->level() );
        case IsOpenedRole:
            return QVariant::fromValue( item->isOpened() );
        case HasChildrenRole:
            return QVariant::fromValue( item->hasChildren() );
        case ParentTag:
            return QVariant::fromValue( item->parentTag() );
        case Uid:
            return QVariant::fromValue( item->uid() );
        default:
            return {};
    }
}

int TreeModel::rowCount( const QModelIndex &parent ) const {
    return _model.size();
}

QHash<int, QByteArray> TreeModel::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[NameRole] = "name";
    roles[LevelRole] = "level";
    roles[IsOpenedRole] = "isOpened";
    roles[HasChildrenRole] = "hasChildren";
    roles[ParentTag] = "parentTag";
    roles[Uid] = "uid";

    return roles;
}

void TreeModel::openItem( int numIndex ) {
    if ( numIndex > ( _model.size() - 1 ) ) return;
    if ( _model[numIndex]->isOpened() ) return;
    QModelIndex modelIndex = index( numIndex );
    // Выставляем флаг открытого элемента
    _model[numIndex]->_opened = true;
    // Оповещаем QML-код об изменении данных
    emit dataChanged( modelIndex, modelIndex );
    int i = numIndex + 1;
    // Оповещаем QML-код о том, что будут добавлены строки в модель
    beginInsertRows( QModelIndex(), i,
                     i + _model[numIndex]->_childList.size() - 1 );
    // Добавляем всех потомков элемента в модель после этого элемента
    // foreach ( std::shared_ptr<ProfilePlanItem> item, _model[numIndex]->_childList )
    // _model.insert( i++, item );

    std::for_each( _model[numIndex]->_childList.begin(),
                   _model[numIndex]->_childList.end(),
                   [this, &i]( auto item ) { _model.insert( i++, item ); } );

    //Оповещаем QML-код о том, что все строки добавлены
    endInsertRows();

    maxUnfoldLevel_ = getMaximumSublevel( _model );
}

void TreeModel::closeItem( int numIndex ) {
    if ( numIndex > ( _model.size() - 1 ) ) return;
    if ( !_model[numIndex]->isOpened() ) return;
    QModelIndex modelIndex = index( numIndex );
    // Сбрасываем флаг открытого элемента
    _model[numIndex]->_opened = false;
    // Оповещаем QML-код об изменении данных
    emit dataChanged( modelIndex, modelIndex );
    int i = numIndex + 1;
    // Ищем все элементы после текущего с большим level
    // Таким образом найдем всех прямых и косвенных потомков
    for ( ; i < _model.size() &&
            ( _model[i]->level() > _model[numIndex]->level() );
          ++i ) {
    }
    --i;
    // Оповещаем QML-код о том, что будут удалены строки из модели
    beginRemoveRows( QModelIndex(), numIndex + 1, i );
    // Удаляем все посчитанные элементы из мо-дели и сбрасываем у них флаг открытия
    while ( i > numIndex ) {
        _model[i]->_opened = false;
        _model.removeAt( i-- );
    }
    //Оповещаем QML-код о том, что все строки удалены
    endRemoveRows();

    maxUnfoldLevel_ = getMaximumSublevel( _model );
}

void TreeModel::openAll() {
    for ( auto i{ 0 }; i < _model.size(); ++i ) {
        if ( !_model[i]->_opened ) {
            openItem( i );
        }
    }
}

void TreeModel::closeAll() {
    for ( auto i{ 0 }; i < _model.size(); ++i ) {
        if ( _model[i]->_opened ) {
            closeItem( i );
        }
    }
}

void TreeModel::populate( const std::vector<QString> &firsts,
                          const std::vector<QString> &seconds,
                          const std::vector<QString> &thirds ) {
    beginResetModel();
    _model.clear();

    for ( const auto &first : firsts ) {
        auto firstLevelItem = std::make_shared<TreeModelItem>(
            TreeModelItem{ {}, first, 0, false } );

        for ( const auto &second : seconds ) {
            auto secondLevelItem = std::make_shared<TreeModelItem>(
                TreeModelItem{ {}, second, 1, false } );

            for ( const auto &third : thirds ) {
                auto thirdLevelItem = std::make_shared<TreeModelItem>(
                    TreeModelItem{ {}, third, 2, false } );
                secondLevelItem->_childList.push_back(
                    std::move( thirdLevelItem ) );
            }
            firstLevelItem->_childList.push_back(
                std::move( secondLevelItem ) );
        }
        _model.push_back( std::move( firstLevelItem ) );
    }

    endResetModel();
}

void TreeModel::populate( const QString &name ) {
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
            "tree model data json parse failed with message: {}",
            parseError.errorString().toStdString() );
        return;
    }

    beginResetModel();
    _model.clear();

    // Add sentinel item
    _model.push_back( std::make_shared<TreeModelItem>(
        TreeModelItem{ {}, "__sentinel__", 0, false, "__start__", {} } ) );
    // =================

    auto hash = []( const QString &name, int level ) -> QByteArray {
        const auto salt =
            QString( "%1%2" ).arg( name, QString::number( level ) ).toLatin1();
        return QCryptographicHash::hash( salt, QCryptographicHash::Sha256 )
            .toHex();
    };

    // "names" level entry, level - 0
    const QJsonArray namesEntry = doc.object().value( "entry" ).toArray();
    for ( const auto &nameEntry : namesEntry.toVariantList() ) {
        QString rootHash{};

        auto nameSharedObject = std::make_shared<TreeModelItem>();
        {
            const auto thisName =
                nameEntry.toJsonObject().value( "name" ).toString();
            const auto thisLevel =
                nameEntry.toJsonObject().value( "level" ).toInt();

            rootHash = hash( thisName, thisLevel );

            nameSharedObject->setName( thisName );
            nameSharedObject->setLevel( thisLevel );
            nameSharedObject->setIsOpened(
                nameEntry.toJsonObject().value( "isOpened" ).toBool() );
            nameSharedObject->setParentTag( "__root__" );
            nameSharedObject->setUid( rootHash );
        }

        // "property" level entry, level - 1
        const auto propertiesEntry =
            nameEntry.toJsonObject().value( "entry" ).toArray();

        // Add sentinel item
        nameSharedObject->_childList.push_back( std::make_shared<TreeModelItem>(
            TreeModelItem{ {}, "__sentinel__", 1, false, "__start__", {} } ) );
        // =================

        for ( const auto &propertyEntry : propertiesEntry.toVariantList() ) {
            QString secHash{};

            auto propertySharedObject = std::make_shared<TreeModelItem>();
            {
                const auto thisName =
                    propertyEntry.toJsonObject().value( "name" ).toString();
                const auto thisLevel =
                    propertyEntry.toJsonObject().value( "level" ).toInt();

                secHash = hash( thisName, thisLevel );

                propertySharedObject->setName( thisName );
                propertySharedObject->setLevel( thisLevel );
                propertySharedObject->setIsOpened(
                    propertyEntry.toJsonObject().value( "isOpened" ).toBool() );
                propertySharedObject->setParentTag( rootHash );
                propertySharedObject->setUid( secHash );
            }

            // "part" level entry, level - 2
            const auto partsEntry =
                propertyEntry.toJsonObject().value( "entry" ).toArray();

            // Add sentinel item
            propertySharedObject->_childList.push_back(
                std::make_shared<TreeModelItem>( TreeModelItem{
                    {}, "__sentinel__", 2, false, "__start__", {} } ) );
            // =================

            for ( const auto &partEntry : partsEntry.toVariantList() ) {
                QString trdHash{};

                auto partSharedObject = std::make_shared<TreeModelItem>();
                {
                    const auto thisName =
                        partEntry.toJsonObject().value( "name" ).toString();
                    const auto thisLevel =
                        partEntry.toJsonObject().value( "level" ).toInt();

                    trdHash = hash( thisName, thisLevel );

                    partSharedObject->setName( thisName );
                    partSharedObject->setLevel( thisLevel );
                    partSharedObject->setIsOpened(
                        partEntry.toJsonObject().value( "isOpened" ).toBool() );
                    partSharedObject->setParentTag( secHash );
                    partSharedObject->setUid( trdHash );
                }

                propertySharedObject->_childList.push_back(
                    std::move( partSharedObject ) );
            }

            // Add sentinel item
            propertySharedObject->_childList.push_back(
                std::make_shared<TreeModelItem>( TreeModelItem{
                    {}, "__sentinel__", 2, false, "__end__", {} } ) );
            // =================

            nameSharedObject->_childList.push_back(
                std::move( propertySharedObject ) );
        }

        // Add sentinel item
        nameSharedObject->_childList.push_back( std::make_shared<TreeModelItem>(
            TreeModelItem{ {}, "__sentinel__", 1, false, "__end__", {} } ) );
        // =================

        _model.push_back( std::move( nameSharedObject ) );
    }

    // Add sentinel item
    _model.push_back( std::make_shared<TreeModelItem>(
        TreeModelItem{ {}, "__sentinel__", 0, false, "__end__", {} } ) );

    endResetModel();
}

std::optional<int> TreeModel::getPosition( const QString &id ) {
    for ( int i = 0; const auto &item : _model ) {
        if ( item->uid() == id ) {
            return i;
        }
        ++i;
    }
    return std::nullopt;
}

std::optional<std::shared_ptr<TreeModelItem>> TreeModel::find(
    const QList<std::shared_ptr<TreeModelItem>> &list, const QString &id ) {
    for ( const auto &item : list ) {
        if ( item->uid() == id ) {
            return item;
        } else {
            find( item->_childList, id );
        }
    }

    return std::nullopt;
}

int TreeModel::getMaximumSublevel(
    const QList<std::shared_ptr<TreeModelItem>> &list ) const {
    int max = 0;
    for ( const auto &item : list ) {
        if ( item->level() > max ) {
            max = item->level();
            getMaximumSublevel( item->_childList );
        }
    }
    return max;
}

void TreeModel::updateChildList( int from, const QString &parent ) {
    //
    if ( parent == "__root__" ) {
        return;
    }

    // Ищем родительский узел, дочерний список которого был изменён
    auto parentItem = find( _model, parent );

    if ( parentItem ) {
        auto parentPtr = parentItem.value();
        const auto childListSize = parentPtr->_childList.size();

        // Берём положение родительского узла в развёрнутом списке модели
        const auto parentPosition = getPosition( parent );

        // Очищаем дочерний (скрытый) подсписок
        parentPtr->_childList.clear();

        // Обновляем дочерний список узла с помощью элементов из
        // ращвёрнутого списка. Эти элементы и были переставлены.
        for ( size_t i = 0; i < childListSize; ++i ) {
            parentPtr->_childList.push_back(
                _model[( parentPosition.value() + 1 ) + i] );
        }
    }
}

bool TreeModel::isUnfolded() const {
    for ( const auto &item : _model ) {
        if ( item->isOpened() ) {
            return true;
        }
    }
    return false;
}

void TreeModel::move( int from, int to, const QString &parent ) {
    if ( from >= 0 && from < rowCount() && to >= 0 && to < rowCount() &&
         from != to ) {
        if ( from == to - 1 ) {  // Allow item moving to the bottom
            to = from++;
        }

        beginResetModel();

        // Перемещаем узлы в списке согласно изменениям
        _model.move( from, to );

        // Ищем дочерний список у элементов модели, который соответствует
        // изменённым элемента и обновляем его состояние.
        updateChildList( from, parent );

        endResetModel();
    }
}

void TreeModel::reload() {
    populate( sourceFile_ );
}

QVariantMap TreeModel::get( int row ) {
    QHash<int, QByteArray> names = roleNames();
    QHashIterator<int, QByteArray> i( names );
    QVariantMap res;
    while ( i.hasNext() ) {
        i.next();
        QModelIndex idx = index( row, 0 );
        QVariant data = idx.data( i.key() );
        res[i.value()] = data;
    }
    return res;
}

}  // namespace netpet
