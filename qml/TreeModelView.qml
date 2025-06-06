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

    // implicitHeight: isListEmpty ? 0 : listView.height - 8

    property bool isListEmpty: (listView.count == 0)
    property alias currentIndex: listView.currentIndex
    property bool isListUnfolded: false

    ListView {
        id: listView

        property int visibleItemsCount: Math.min(control.itemsVisibleMax,
                                                 listView.count)

        anchors.left: parent.left
        anchors.right: parent.right
        // height: (visibleItemsCount > 0) ? (visibleItemsCount * control.itemHeight
                                           // + spacing * visibleItemsCount) : 0
        height: parent.height

        ScrollBar.vertical: ScrollBar {}

        delegate: treeDelegate

        spacing: 1
        clip: true

        //Компонент делегата
        Component {
            id: treeDelegate
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

                // "Отбой" слева элементов-потомков
                Item {
                    id: levelMarginElement
                    width: (model.level > 1 ? 1 : model.level) * 24
                    height: parent.height
                    anchors {
                        left: parent.left
                        verticalCenter: wrapper.verticalCenter
                    }
                }

                // Область с иконкой открытых/закрытых потомков.
                // На листьях не виден.
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
                            if (model.isOpened) {
                                LocalTreeModel.closeItem(
                                            index, true)
                            } else {
                                LocalTreeModel.openItem(
                                            index, true)
                            }
                        }
                    }

                    states: [
                        State {
                            name: "leafNode"
                            when: !model.hasChildren && (model.level === 2)
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
                            when: !model.hasChildren && (model.level !== 2)
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
                            when: (model.hasChildren) && (model.isOpened)
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

                        State {
                            name: "closedNode"
                            when: (model.hasChildren) && (!model.isOpened)
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
                //Область для отображения данных элемента
                Rectangle {
                    id: itemWrapper
                    anchors {
                        left: nodeOpenElement.right
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
                            state: "closedNode"
                        }
                    }

                    Text {
                        id: nameTextElement
                        text: model.name
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
                            when: (model.level === 0)
                            PropertyChanges {
                                target: itemWrapper
                                color: wrapper.hovered ? Theme.colors["accent_focus_60"] : Theme.colors["accent_focus_30"]
                            }
                            PropertyChanges {
                                target: itemIcon
                                source: "icons/branch_hierarchy.svg"
                                color: wrapper.hovered ? Theme.colors["main_contrast"] : "white"
                            }
                            PropertyChanges {
                                target: nameTextElement
                                font: _fonts.text_body_accent
                            }
                        },
                        State {
                            when: (model.level === 1)
                            PropertyChanges {
                                target: itemWrapper
                                color: wrapper.hovered ? Theme.colors["accent_focus_60"] : Theme.colors["accent_focus_30"]
                            }
                            PropertyChanges {
                                target: itemIcon
                                source: "icons/flag_01.svg"
                                color: wrapper.hovered ? Theme.colors["main_contrast"] : "white"
                            }
                             PropertyChanges {
                              target: nameTextElement
                              font: _fonts.text_body_accent
                             }
                        },
                        State {
                            when: (model.level === 2)
                            PropertyChanges {
                                target: itemWrapper
                                color: wrapper.hovered ? Theme.colors["accent_focus_60"] : "transparent"
                            }
                            PropertyChanges {
                                target: itemIcon
                                source: "icons/cube_01.svg"
                                color: wrapper.hovered ? Theme.colors["main_contrast"] : "white"
                            }
                            PropertyChanges {
                                target: nameTextElement
                                font: _fonts.label_accent
                            }
                        }
                    ]
                }
            }
        }
    }
}
