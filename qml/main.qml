import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import Tire 1.0
import "components"

Item {
    id: mainWindow
    readonly property var _fonts: Constants.fonts
    readonly property var _units: Constants.units

    Render {
        id: renderer
        NumberAnimation on t {
            running: true
            from: 0
            to: 1 << 63
            loops: Animation.Infinite
        }
    }

    Item {
        id: background
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            // leftMargin: 500
        }

        Rectangle {
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 24

            color: "green"

            MouseArea {
                anchors.fill: parent
                property variant clickPos: "1,1"

                onPressed: mouse => {
                               clickPos = Qt.point(mouse.x, mouse.y)
                           }

                onPositionChanged: mouse => {
                                       var delta = Qt.point(
                                           mouse.x - clickPos.x,
                                           mouse.y - clickPos.y)
                                       //mainWindow.x += delta.x
                                       //mainWindow.y += delta.y

                                   }
            }
        }

        Rectangle {
            id: leftPanel

            height: reloadModelsButton.height + 32
            width: reloadModelsButton.width + closeButton.width + 32

            x: 100
            y: 100

            radius: 8

            color: Theme.colors["background"]

            NpButton {
                id: reloadModelsButton
                anchors {
                    left: leftPanel.left
                    leftMargin: 8
                    bottom: leftPanel.bottom
                    bottomMargin: 8
                }
                icon.source: "icons/exit_up.svg"
                onClicked: {
                    quickViewHandle.noop()
                }
            }

            NpButton {
                id: closeButton
                anchors {
                    right: leftPanel.right
                    rightMargin: 8
                    bottom: leftPanel.bottom
                    bottomMargin: 8
                }
                icon.source: "icons/power.svg"
                onClicked: {
                    // send QQmlEngine::quit()
                    Qt.quit()
                }
            }

            Item {
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
                height: 24

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
                                           leftPanel.x += delta.x
                                           leftPanel.y += delta.y
                                       }
                }
            }
        }
    }
}
