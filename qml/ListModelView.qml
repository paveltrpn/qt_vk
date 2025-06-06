import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15

import NetPet 1.0
import "components"

Rectangle {
    id: control

    readonly property var _units: Constants.units
    readonly property var _fonts: Constants.fonts

    property alias model: listView.model

    property int itemHeight: 40
    property int itemsVisibleMax: 100

    color: "transparent"

    implicitHeight: isListEmpty ? 0 : listView.height - 8

    property bool isListEmpty: (listView.count == 0)
    property alias currentIndex: listView.currentIndex
    property bool isListUnfolded: false

    ListView {
        id: listView

        property int visibleItemsCount: Math.min(control.itemsVisibleMax,
                                                 listView.count)

        anchors.left: parent.left
        anchors.right: parent.right
        height: (visibleItemsCount > 0) ? (visibleItemsCount * control.itemHeight
                                           + spacing * visibleItemsCount) : 0

        ScrollBar.vertical: ScrollBar {}

        delegate: listDelegate

        spacing: 1
        clip: true

        displaced: Transition {
            NumberAnimation {
                properties: "x,y"
                easing.type: Easing.OutQuad
            }
        }

        //Компонент делегата
        Component {
            id: listDelegate
            Item {
                id: wrapper
                height: itemHeight
                anchors {
                    left: parent.left
                    right: parent.right
                    // fix: item делегата выходит за скроллбар справа
                    rightMargin: 2
                }

                property bool hovered: false

                // Управляет ховером на делегате
                MouseArea {
                    id: hoverMouseArea
                    anchors.fill: parent
                    propagateComposedEvents: true
                    hoverEnabled: true
                    onEntered: {
                        wrapper.hovered = true
                    }
                    onExited: {
                        wrapper.hovered = false
                    }
                }

                //Область для отображения данных элемента
                Rectangle {
                    id: itemWrapper
                    state: "loneleyState"
                    anchors {
                        left: parent.left
                        top: parent.top
                        bottom: parent.bottom
                        right: parent.right
                    }
                    radius: 8

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

                    // Стиль и иконка делегата в зависимоти от его положения в иерархии и состояния ховера
                    states: [
                        State {
                            name: "loneleyState"
                            PropertyChanges {
                                target: itemWrapper
                                color: wrapper.hovered ? Theme.colors["accent_focus_60"] : Theme.colors["accent_focus_30"]
                            }
                            PropertyChanges {
                                target: itemIcon
                                source: "icons/list.svg"
                                color: wrapper.hovered ? Theme.colors["main_contrast"] : "white"
                            }
                            PropertyChanges {
                                target: nameTextElement
                                font: _fonts.text_body_accent
                            }
                        }
                    ]
                }
            }
        }
    }
}
