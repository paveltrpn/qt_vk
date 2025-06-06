import QtQuick 2.14
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15
import QtQml.Models 2.15
import NetPet 1.0
import "components"

Rectangle {
    id: control

    readonly property var _units: Constants.units
    readonly property var _fonts: Constants.fonts

    property alias model: visualModel.model

    property int itemHeight: 40
    property int itemsVisibleMax: 100

    color: "transparent"

    implicitHeight: isListEmpty ? 0 : listView.height - 8

    property bool isListEmpty: (listView.count == 0)
    property alias currentIndex: listView.currentIndex
    property bool isListUnfolded: false

    function performAllowedDragAction(from, to) {
        // Открытые элементы списка перемещать нельзя!
        if ((visualModel.model.get(from)["isOpened"] === true)
                || (visualModel.model.get(to)["isOpened"] === true)) {
            return false
        }

        let maxUnfoldLevel = visualModel.model.maxUnfoldLevel()
        // Допускаем редактирование порядка только в самом открытом подсписке
        if ((visualModel.model.get(from)["level"] < maxUnfoldLevel)
                || (visualModel.model.get(to)["level"] < maxUnfoldLevel)) {
            return false
        }

        // Ни при каких условиях, случайно или намеренно, не допускаем
        // перемещения, в которых участвуют ограничивающие элементы.
        // Вообще говоря они не отображаются и нет возможности их перетащить
        // но бывает кейс когда перетаскиваемый элемент меняется местом
        // с ограничителем, это выглядит как баг.
        if ((visualModel.model.get(from)["name"] === "__sentinel__")
                || (visualModel.model.get(to)["name"] === "__sentinel__")) {
            return false
        }

        // Тэг выбранного элемента
        let fromParentTag = visualModel.model.get(from)["parentTag"]

        // Тэг элементов следующих перед и после того, на который будем дропать выбраный
        let toPrevParentTag = visualModel.model.get(to - 1)["parentTag"]
        let toNextParentTag = visualModel.model.get(to + 1)["parentTag"]

        // Допускаем перемещение если:
        // 1) Перетаскиваемый элемент ограничен сверху и снизу элементами с тем же родителем что и он.
        // 2) Перетаскиваемый элемент ограничен сверху __sentinel__ begin и снизу элементом, с тем же родителем что и он.
        // 3) Перетаскиваемый элемент ограничен снизу __sentinel__ end и сверху элементом, с тем же родителем что и он.
        if (((fromParentTag === toPrevParentTag)
             && (fromParentTag === toNextParentTag))
                || ((fromParentTag === toPrevParentTag)
                    && (toNextParentTag === "__end__"))
                || ((fromParentTag === toNextParentTag)
                    && (toPrevParentTag === "__start__"))) {
            return true
        }

        return false
    }

    DelegateModel {
        id: visualModel
        delegate: DropArea {
            id: delegateRoot

            // В эти проперти попадают model roles
            required property string name
            required property int level
            required property bool isOpened
            required property bool hasChildren

            property int modelIndex

            width: listView.width
            height: thingItem.isSentinelItem ? 0 : control.itemHeight

            //height: control.itemHeight
            onEntered: function (drag) {
                let from = drag.source.visualIndex
                let to = thingItem.visualIndex

                if (performAllowedDragAction(from, to)) {
                    visualModel.items.move(from, to)
                }
            }

            // Отрабатывает в момент mouse release и именно здесь
            // нужно обновлять основную модель!
            onDropped: function (drop) {
                let from = modelIndex
                // какой то странынй каст...
                // var to = (drop.source as Example.thingItem).visualIndex
                let to = drop.source.visualIndex

                if (performAllowedDragAction(from, to)) {
                    let parent = visualModel.model.get(from)["parentTag"]
                    LocalTreeModel.move(from, to, parent)
                }
            }
            property int visualIndex: DelegateModel.itemsIndex

            TreeViewDragableItem {
                id: thingItem

                // Элементы с таким именем не отобрадаются в списке
                property bool isSentinelItem: delegateRoot.name === "__sentinel__"

                width: listView.width
                height: isSentinelItem ? 0 : control.itemHeight
                //height: control.itemHeight
                visible: !isSentinelItem

                dragParent: listView
                visualIndex: delegateRoot.visualIndex
                onPressed: delegateRoot.modelIndex = visualIndex
                text: delegateRoot.name
                itemLevel: delegateRoot.level
                itemIsOpened: delegateRoot.isOpened
                itemHasChildren: delegateRoot.hasChildren
            }
        }
    }

    ListView {
        id: listView

        property int visibleItemsCount: Math.min(control.itemsVisibleMax,
                                                 listView.count)

        anchors.left: parent.left
        anchors.right: parent.right
        height: (visibleItemsCount > 0) ? (visibleItemsCount * control.itemHeight
                                           + spacing * visibleItemsCount) : 0

        ScrollBar.vertical: ScrollBar {}

        spacing: 1
        clip: true

        displaced: Transition {
            NumberAnimation {
                properties: "x,y"
                easing.type: Easing.OutQuad
            }
        }

        model: visualModel
    }
}
