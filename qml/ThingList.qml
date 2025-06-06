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

    DelegateModel {
        id: visualModel
        delegate: DropArea {
            id: delegateRoot

            // В эту проперти попадает model.name
            required property string name
            required property string level
            required property string isOpened


            property int modelIndex

            width: listView.width
            height: control.itemHeight

            onEntered: function (drag) {
                var from = drag.source.visualIndex
                var to = thingTile.visualIndex
                visualModel.items.move(from, to)
            }

            // Отрабатывает в момент mouse release и именно здесь
            // нужно обновлять основную модель!
            onDropped: function (drop) {
                var from = modelIndex
                // какой то странынй каст...
                // var to = (drop.source as Example.ThingTile).visualIndex
                var to = drop.source.visualIndex
                LocalListModel.move(from, to)
            }

            property int visualIndex: DelegateModel.itemsIndex

            ThingItem {
                id: thingTile
                width: listView.width
                height: control.itemHeight
                dragParent: listView
                visualIndex: delegateRoot.visualIndex
                onPressed: delegateRoot.modelIndex = visualIndex
                text: delegateRoot.name
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
