import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import NetPet 1.0
import "components"

ApplicationWindow {
    id: netPetWnd

    readonly property var _fonts: Constants.fonts
    readonly property var _units: Constants.units

    property bool isFrameless: false

    // width: 1480
    //height: 840
    minimumWidth: 320
    minimumHeight: 240

    title: "model-view"

    property var windowFlags: isFrameless ? (Qt.FramelessWindowHint | Qt.ClickFocus) : undefined

    // flags: windowFlags
    visible: true

    color: "transparent"

    property bool isListUnfolded: false

    Rectangle {
        id: background
        anchors.fill: parent
        radius: netPetWnd.isFrameless ? 16 : 0

        color: Theme.colors["background"]

        Rectangle {
            id: backgroundFrame

            anchors {
                fill: parent
                margins: 8
            }

            color: Theme.colors["background_additional"]
            radius: 16 - 6
        }

        Rectangle {
            id: topDragArea
            anchors {
                top: background.top
                left: background.left
                right: background.right
            }
            height: 8
            color: "transparent"

            MouseArea {
                id: topMouseRegion
                anchors.fill: parent
                property variant clickPos: "1,1"

                onPressed: mouse => {
                               clickPos = Qt.point(mouse.x, mouse.y)
                           }

                onPositionChanged: mouse => {
                                       var delta = Qt.point(
                                           mouse.x - clickPos.x,
                                           mouse.y - clickPos.y)
                                       netPetWnd.x += delta.x
                                       netPetWnd.y += delta.y
                                   }
            }
        }

        Rectangle {
            id: bottomDragArea
            anchors {
                bottom: background.bottom
                left: background.left
                right: background.right
            }
            height: 8
            color: "transparent"

            MouseArea {
                id: bottomMouseRegion
                anchors.fill: parent
                property variant clickPos: "1,1"

                onPressed: mouse => {
                               clickPos = Qt.point(mouse.x, mouse.y)
                           }

                onPositionChanged: mouse => {
                                       var delta = Qt.point(
                                           mouse.x - clickPos.x,
                                           mouse.y - clickPos.y)
                                       netPetWnd.x += delta.x
                                       netPetWnd.y += delta.y
                                   }
            }
        }

        RowLayout {
            id: mainLayout

            anchors {
                fill: parent
                topMargin: 16
                bottomMargin: 16
                leftMargin: 16
                rightMargin: 16
            }

            spacing: 8
            Rectangle {
                id: leftPanel

                height: background.height - 32
                radius: 8

                color: Theme.colors["background"]

                Layout.fillHeight: true
                Layout.preferredWidth: 95

                NpButton {
                    id: reloadModelsButton
                    anchors {
                        top: leftPanel.top
                        topMargin: 16
                        horizontalCenter: leftPanel.horizontalCenter
                    }
                    icon.source: "icons/exit_up.svg"
                    onClicked: {
                        LocalListModel.reload()
                        LocalTreeModel.reload()
                    }
                }

                NpButton {
                    id: closeButton
                    anchors {
                        bottom: leftPanel.bottom
                        bottomMargin: 16
                        horizontalCenter: leftPanel.horizontalCenter
                    }
                    icon.source: "icons/power.svg"
                    onClicked: {
                        // send QQmlEngine::quit()
                        Qt.quit()
                    }
                }
            }

            Connections {
                target: LocalTreeModel
                onDataChanged: function(topLeft, bottomRight, roles) {
                    netPetWnd.isListUnfolded = LocalTreeModel.isUnfolded()
                }
            }

            states: [
                State {
                    when: netPetWnd.isListUnfolded === false
                    PropertyChanges {
                        target: treeStaticModelFoldButton
                        icon.source: "icons/expand.svg"
                    }
                    PropertyChanges {
                        target: treeDragableModelFoldButton
                        icon.source: "icons/expand.svg"
                    }
                },
                State {
                    when: netPetWnd.isListUnfolded === true
                    PropertyChanges {
                        target: treeStaticModelFoldButton
                        icon.source: "icons/collapse.svg"
                    }
                    PropertyChanges {
                        target: treeDragableModelFoldButton
                        icon.source: "icons/collapse.svg"
                    }
                }
            ]

            Panel {
                id: treeStaticModelPanel

                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: treeStaticModelPanelHeader

                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 16
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    height: 40
                    radius: 8
                    color: Theme.colors["accent_focus_70"]

                    Text {
                        id: treeStaticModelPanelHeaderText
                        text: "Tree view (static)"
                        font: _fonts.title_accent
                        color: Theme.colors["main_contrast"]
                        anchors {
                            fill: parent
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }

                NpButton {
                    id: treeStaticModelFoldButton
                    anchors {
                        top: treeStaticModelPanelHeader.bottom
                        topMargin: 8
                        left: parent.left
                        leftMargin: 16
                        right: parent.right
                        rightMargin: 16
                    }

                    onClicked: {
                        if (netPetWnd.isListUnfolded) {
                            LocalTreeModel.closeAll()
                            netPetWnd.isListUnfolded = false
                        } else {
                            LocalTreeModel.openAll()
                            netPetWnd.isListUnfolded = true
                        }
                    }
                }

                Item {
                    id: treeStaticModelViewWrapper
                    clip: true
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: treeStaticModelFoldButton.bottom
                        topMargin: 8
                        bottom: parent.bottom
                    }

                    TreeModelView {
                        id: tModelView

                        model: LocalTreeModel

                        height: parent.height

                        anchors {
                            left: parent.left
                            leftMargin: 32
                            top: parent.top
                            right: parent.right
                            rightMargin: 32
                        }
                    }
                }
            }

            Panel {
                id: treeDragableModelPanel

                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: treeDragableModelPanelHeader

                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 16
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    height: 40
                    radius: 8
                    color: Theme.colors["accent_focus_70"]

                    Text {
                        id: treeDragableModelPanelHeaderText
                        text: "Tree view (dragable)"
                        font: _fonts.title_accent
                        color: Theme.colors["main_contrast"]
                        anchors {
                            fill: parent
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }

                NpButton {
                    id: treeDragableModelFoldButton
                    anchors {
                        top: treeDragableModelPanelHeader.bottom
                        topMargin: 8
                        left: parent.left
                        leftMargin: 16
                        right: parent.right
                        rightMargin: 16
                    }

                    onClicked: {
                        if (netPetWnd.isListUnfolded) {
                            LocalTreeModel.closeAll()
                            netPetWnd.isListUnfolded = false
                        } else {
                            LocalTreeModel.openAll()
                            netPetWnd.isListUnfolded = true
                        }
                    }
                }

                Item {
                    id: treeDragableviewWrapper
                    clip: true
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: treeDragableModelFoldButton.bottom
                        topMargin: 8
                        bottom: parent.bottom
                    }

                    TreeViewDragable {
                        id: treeDragableModelView

                        model: LocalTreeModel

                        height: parent.height

                        anchors {
                            left: parent.left
                            leftMargin: 32
                            top: parent.top
                            right: parent.right
                            rightMargin: 32
                        }
                    }
                }
            }

            Panel {
                id: treeAsListPanel

                Rectangle {
                    id: treeAsListPanelHeader

                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 16
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    height: 40
                    radius: 8
                    color: Theme.colors["accent_focus_70"]

                    Text {
                        id: treeAsListPanelHeaderText
                        text: "Tree as list"
                        font: _fonts.title_accent
                        color: Theme.colors["main_contrast"]
                        anchors {
                            fill: parent
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }

                ListModelView {
                    id: treeAsListDelegateModelView

                    model: LocalTreeModel

                    anchors {
                        left: parent.left
                        leftMargin: 32
                        top: treeAsListPanelHeader.bottom
                        topMargin: 8
                        right: parent.right
                        rightMargin: 32
                    }
                }

                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            Panel {
                id: listStaticModelPanel

                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: listStaticModelPanelHeader

                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 16
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    height: 40
                    radius: 8
                    color: Theme.colors["accent_focus_70"]

                    Text {
                        id: listStaticModelPanelHeaderText
                        text: "List view (static)"
                        font: _fonts.title_accent
                        color: Theme.colors["main_contrast"]
                        anchors {
                            fill: parent
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }

                ListModelView {
                    id: listStaticModelView

                    model: LocalListModel

                    anchors {
                        left: parent.left
                        leftMargin: 32
                        top: listStaticModelPanelHeader.bottom
                        topMargin: 8
                        right: parent.right
                        rightMargin: 32
                    }
                }
            }

            // Qt example
            /*
            Panel {
                id: vanillaExamplePanel

                Rectangle {
                    id: listVanillaExamplePanelHeader

                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 16
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    height: 40
                    radius: 8
                    color: Theme.colors["accent_focus_70"]

                    Text {
                        id: listVanillaExamplePanelHeaderText
                        text: "Qt example (dragable)"
                        font: _fonts.title_accent
                        color: Theme.colors["main_contrast"]
                        anchors {
                            fill: parent
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }

                VanillaExample {
                    id: listVanillaExampleDelegateModelView

                    model: LocalListModel

                    anchors {
                        left: parent.left
                        leftMargin: 32
                        top: listVanillaExamplePanelHeader.bottom
                        topMargin: 8
                        right: parent.right
                        rightMargin: 32
                    }
                }

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
            */

            Panel {
                id: thingListExamplePanel

                Rectangle {
                    id: listthingListExamplePanelHeader

                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: 16
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    height: 40
                    radius: 8
                    color: Theme.colors["accent_focus_70"]

                    Text {
                        id: listthingListExamplePanelHeaderText
                        text: "List (dragable)"
                        font: _fonts.title_accent
                        color: Theme.colors["main_contrast"]
                        anchors {
                            fill: parent
                        }
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                    }
                }

                ThingList {
                    id: listThingListExampleDelegateModelView

                    model: LocalListModel

                    anchors {
                        left: parent.left
                        leftMargin: 32
                        top: listthingListExamplePanelHeader.bottom
                        topMargin: 8
                        right: parent.right
                        rightMargin: 32
                    }
                }

                Layout.fillHeight: true
                Layout.fillWidth: true
            }
        }
    }
}
