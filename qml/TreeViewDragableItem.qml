import QtQuick 2.15
import QtQuick 2.14
import QtQuick.Controls 2.15
import QtQuick.Controls.impl 2.15
import NetPet 1.0
import "components"

Rectangle {
    id: root
    readonly property var _units: Constants.units
    readonly property var _fonts: Constants.fonts

    color: "transparent"

    required property Item dragParent
    signal pressed
    signal released
    signal clicked

    property bool hovered: false

    property alias text: nameTextElement.text
    property int itemLevel
    property bool itemIsOpened
    property bool itemHasChildren

    property int visualIndex: 0

    anchors {
        horizontalCenter: parent.horizontalCenter
        verticalCenter: parent.verticalCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: root
        drag.axis: Drag.YAxis
        onClicked: {
            root.clicked()
        }
        onPressed: root.pressed()
        onReleased: {
            parent.Drag.drop()
            root.released()
        }
        propagateComposedEvents: true
        hoverEnabled: true
        onEntered: {
            root.hovered = true
        }
        onExited: {
            root.hovered = false
        }
    }

    Drag.active: mouseArea.drag.active
    Drag.source: root
    Drag.hotSpot.x: root.width / 2
    Drag.hotSpot.y: root.height / 2

    // "Отбой" слева элементов-потомков
    Item {
        id: levelMarginElement
        width: (itemLevel > 1 ? 1 : itemLevel) * 24
        height: parent.height
        anchors {
            left: parent.left
            verticalCenter: root.verticalCenter
        }
    }

    Item {
        id: nodeOpenElement

        anchors {
            left: levelMarginElement.right
            top: parent.top
            bottom: parent.bottom
        }
        width: 24

        ColorImage {
            id: delegateStateIndicatorIcon
            anchors.centerIn: parent
            width: 24
            height: 24
            color: "white"
            state: "closedNode"
        }

        // Открыть/свернуть элемент списка. Активна на узле,
        // отключается на листе.
        MouseArea {
            id: nodeOpenClickable
            anchors.fill: parent
            onClicked: {
                if (itemIsOpened) {
                    LocalTreeModel.closeItem(visualIndex, true)
                } else {
                    LocalTreeModel.openItem(visualIndex, true)
                }
            }
        }

        states: [
            State {
                name: "leafNode"
                when: !itemHasChildren && (itemLevel === 2)
                PropertyChanges {
                    target: nodeOpenClickable
                    enabled: false
                }
                PropertyChanges {
                    target: delegateStateIndicatorIcon
                    visible: false
                }
            },

            State {
                when: !itemHasChildren && (itemLevel !== 2)
                PropertyChanges {
                    target: nodeOpenClickable
                    enabled: false
                }
                PropertyChanges {
                    target: delegateStateIndicatorIcon
                    visible: false
                }
            },

            State {
                name: "openedNode"
                when: (itemHasChildren) && (itemIsOpened)
                PropertyChanges {
                    target: nodeOpenClickable
                    enabled: true
                }
                PropertyChanges {
                    target: delegateStateIndicatorIcon
                    source: "icons/chevron_down.svg"
                    visible: true
                }
            },

            // Закрытый элемент ("Группа" или "Здание")
            State {
                name: "closedNode"
                when: (itemHasChildren) && (!itemIsOpened)
                PropertyChanges {
                    target: nodeOpenClickable
                    enabled: true
                }
                PropertyChanges {
                    target: delegateStateIndicatorIcon
                    source: "icons/chevron_right.svg"
                    visible: true
                }
            }
        ]
    }

    Rectangle {
        id: itemWrapper

        anchors {
            left: nodeOpenElement.right
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }

        color: Theme.colors["accent_focus_30"]
        state: "loneleyState"
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
                source: "icons/list.svg"
            }
        }

        Text {
            id: nameTextElement
            font: _fonts.text_body_accent
            elide: Text.ElideRight
            color: Theme.colors["main_contrast"]
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
                when: (itemLevel === 0)
                PropertyChanges {
                    target: itemWrapper
                    color: root.hovered ? Theme.colors["accent_focus_60"] : Theme.colors["accent_focus_30"]
                }
                PropertyChanges {
                    target: itemIcon
                    source: "icons/branch_hierarchy.svg"
                    color: root.hovered ? Theme.colors["main_contrast"] : "white"
                }
                PropertyChanges {
                    target: nameTextElement
                    font: _fonts.text_body_accent
                }
            },
            State {
                when: (itemLevel === 1)
                PropertyChanges {
                    target: itemWrapper
                    color: root.hovered ? Theme.colors["accent_focus_60"] : Theme.colors["accent_focus_30"]
                }
                PropertyChanges {
                    target: itemIcon
                    source: "icons/flag_01.svg"
                    color: root.hovered ? Theme.colors["main_contrast"] : "white"
                }
                PropertyChanges {
                    target: nameTextElement
                    font: _fonts.text_body_accent
                }
            },
            State {
                when: (itemLevel === 2)
                PropertyChanges {
                    target: itemWrapper
                    color: root.hovered ? Theme.colors["accent_focus_60"] : "transparent"
                }
                PropertyChanges {
                    target: itemIcon
                    source: "icons/cube_01.svg"
                    color: root.hovered ? Theme.colors["main_contrast"] : "white"
                }
                PropertyChanges {
                    target: nameTextElement
                    font: _fonts.label_accent
                }
            }
        ]
    }
    states: [
        State {
            when: mouseArea.drag.active
            ParentChange {
                target: root
                parent: root.dragParent
            }

            AnchorChanges {
                target: root
                anchors.horizontalCenter: undefined
                anchors.verticalCenter: undefined
            }
        }
    ]
}
