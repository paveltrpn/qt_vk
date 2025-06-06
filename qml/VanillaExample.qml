import QtQuick 2.15
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

    Component {
        id: dragDelegate

        MouseArea {
            id: dragArea

            property bool held: false
            property bool hovered: false

            // Индекс элемента на котором произошел клик
            property int visualIndex: model.index

            width: listView.width
            height: control.itemHeight

            drag.target: held ? itemWrapper : undefined
            drag.axis: Drag.YAxis

            hoverEnabled: true

            onPressed: {
                // console.log(" onPressed ")
                held = true
            }
            onReleased: {
                // console.log(" onReleased ")
                held = false
            }
            onEntered: {
                hovered = true
            }
            onExited: {
                hovered = false
            }

            // property var visualIndex: visualModel.model.index

            // A DropArea is an invisible item which receives events when other items are dragged over it.
            DropArea {
                anchors {
                    fill: parent
                    margins: 10
                }

                onEntered: function (drag) {
                    // let keys = Object.keys(drag.source)
                    // for (let i in Object.keys(drag.source)){
                        // console.log( keys[i] + " = " + drag.source[keys[i]])
                    // }

                    // let obj = visualModel.items
                    // console.log(JSON.stringify(obj))
                    // let modelKeys = Object.keys(obj)
                    // for (let i in Object.keys(obj)){
                        // console.log( modelKeys[i] + " = " + obj[modelKeys[i]])
                    // }

                    // let obj = listView
                    // let modelKeys = Object.keys(obj)
                    // for (let i in Object.keys(obj)){
                        // console.log( modelKeys[i] + " = " + obj[modelKeys[i]])
                    // }

                    console.log("teken id " + dragArea.visualIndex + " hovered " + drag.source.DelegateModel.itemsIndex)

                    visualModel.items.move(
                                drag.source.DelegateModel.itemsIndex,
                                dragArea.visualIndex)
                }

                onDropped: function (drop) {
                    // let obj = drop
                    // let modelKeys = Object.keys(obj)
                    // for (let i in Object.keys(obj)){
                        // console.log( modelKeys[i] + " = " + obj[modelKeys[i]])
                    // }
                    // console.log("drop.source in " + drop.source.DelegateModel.itemsIndex)
                }
            }

            Rectangle {
                id: itemWrapper

                state: "loneleyState"
                color: Theme.colors["accent_focus_30"]

                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2

                radius: 8
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: dragArea.width
                height: control.itemHeight

                Item {
                    id: itemIconWrapper
                    height: parent.height
                    width: 40

                    anchors {
                        left: parent.left
                        leftMargin: _units.gaps.leftQuarter
                        top: parent.top
                        bottom: parent.bottom
                    }

                    ColorImage {
                        id: itemIcon
                        anchors.centerIn: parent
                        width: 24
                        height: 24
                        color: "white"
                        state: "loneleyState"
                        source: "icons/list.svg"
                    }
                }

                Text {
                    id: nameTextElement
                    text: model.name
                    font: _fonts.text_body_accent
                    elide: Text.ElideRight
                    color: Theme.colors["main_contrast"]
                    state: "loneleyState"
                    anchors {
                        left: itemIconWrapper.right
                        leftMargin: _units.gaps.leftHalf
                        right: parent.right
                        rightMargin: _units.gaps.rightHalf
                        verticalCenter: parent.verticalCenter
                    }

                    verticalAlignment: Text.AlignVCenter
                }

                states: [
                    State {
                        name: "loneleyState"
                        PropertyChanges {
                            target: itemWrapper
                            color: dragArea.hovered ? Theme.colors["accent_focus_60"] : Theme.colors["accent_focus_30"]
                        }
                        PropertyChanges {
                            target: itemIcon
                            color: dragArea.hovered ? Theme.colors["main_contrast"] : "white"
                        }
                        PropertyChanges {
                            target: nameTextElement
                            font: _fonts.text_body_accent
                        }
                    },
                    State {
                        when: dragArea.held
                        PropertyChanges {
                            target: itemWrapper
                            color: Theme.colors["accent_focus_60"]
                        }
                        ParentChange {
                            target: itemWrapper
                            parent: control
                        }
                        AnchorChanges {
                            target: itemWrapper
                            anchors {
                                horizontalCenter: undefined
                                verticalCenter: undefined
                            }
                        }
                    }
                ]
            }
        }
    }

    DelegateModel {
        id: visualModel
        delegate: dragDelegate

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

        model: visualModel

        spacing: 1
        // cacheBuffer: 50
    }
}
