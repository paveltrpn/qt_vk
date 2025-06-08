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
        }
    }
}
