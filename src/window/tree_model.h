
#pragma once

#include <memory>
#include <QString>
#include <QList>
#include <QAbstractListModel>

namespace netpet {

struct TreeModelItem final {
    bool hasChildren() const { return !_childList.empty(); }
    QString name() const { return _name; };
    int level() const { return _level; };
    bool isOpened() const { return _opened; };
    QString parentTag() const { return parentTag_; };
    QString uid() const { return uid_; };

    void setName( const QString &value ) { _name = value; };
    void setLevel( int value ) { _level = value; };
    void setIsOpened( bool value ) { _opened = value; }
    void setParentTag( const QString &value ) { parentTag_ = value; };
    void setUid( const QString &value ) { uid_ = value; };

    QList<std::shared_ptr<TreeModelItem>> _childList{};
    QString _name{};
    int _level{ 0 };
    bool _opened{ false };
    QString parentTag_{};
    QString uid_{};
};

/*
 * Свёрнутый список:
 *
 * root1 -> childList -> ...
 * root2 -> childList -> ...
 * ...
 * rootN -> childList -> ...
 *
 * Список с раскрытым подузлом:
 * ( Элементы из дочернего списка вставляются после своего родителя.
 * При удалении после родителя удаляются n = childList.size() элементов)
 *
 * root1 -> childList -> ...
 * root2 -> childList -> ...
 *   childList[0] -> childList -> ...
 *   childList[1] -> childList -> ...
 *   ...
 *   childList[n] -> childList -> ...
 * ...
 * rootN -> childList -> ...
 */

struct TreeModel : QAbstractListModel {
    Q_OBJECT
public:
    enum ProfilePlanModelRoles {
        NameRole = Qt::UserRole + 1,
        LevelRole,
        IsOpenedRole,
        HasChildrenRole,
        ParentTag,
        Uid,
    };

    explicit TreeModel( QObject *parent = nullptr )
        : QAbstractListModel{ parent } {}

    QVariant data( const QModelIndex &index,
                   int role = Qt::DisplayRole ) const override;
    int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
    QHash<int, QByteArray> roleNames() const override;

    void populate( const std::vector<QString> &firsts,
                   const std::vector<QString> &seconds,
                   const std::vector<QString> &thirds );
    void populate( const QString &name );

    Q_INVOKABLE QVariantMap get( int row );

    Q_INVOKABLE void move( int from, int to, const QString &parent );

    Q_INVOKABLE void openAll();
    Q_INVOKABLE void closeAll();
    Q_INVOKABLE void openItem( int numIndex );
    Q_INVOKABLE void closeItem( int numIndex );
    Q_INVOKABLE bool isUnfolded() const;

    Q_INVOKABLE void reload();

    // Возвращает глубину, соотвеветствующую самому глубокому открытому
    // в данный момент подузлу во всём списке
    Q_INVOKABLE int getMaximumSublevel(
        const QList<std::shared_ptr<TreeModelItem>> &list ) const;

    // Геттер для qml
    Q_INVOKABLE int maxUnfoldLevel() const { return maxUnfoldLevel_; };

private:
    // Когда произошло событие перемещения элемента в развёрнутом подузле списка, мы должны
    // выполнить теже самые действие в списке дочерних элементов родительского узла.
    void updateChildList( int from, const QString &parent );

    // Рекурсивно ищем элемент в модели по его hash
    std::optional<std::shared_ptr<TreeModelItem>> find(
        const QList<std::shared_ptr<TreeModelItem>> &list, const QString &id );

    // Ищем в _model как в линейном списке, т.е. в найденные элементы
    // могут попасть те элементы модели, которые
    // в даннный момент развёрнуты. Возвращаем индекс в списке
    std::optional<int> getPosition( const QString &id );

private:
    // srvice
    QList<std::shared_ptr<TreeModelItem>> _model{};

    int maxUnfoldLevel_{};

    QString sourceFile_{};
};

}  // namespace netpet
