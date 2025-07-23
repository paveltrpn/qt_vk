import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import Qt5Compat.GraphicalEffects
import Tire 1.0
import "components"

Item {
    id: mainWindow

    readonly property var _fonts: Appearence.fonts
    readonly property var _gaps: Appearence.gaps
    readonly property var _radius: Appearence.radius
    readonly property var _color: Appearence.colors

    // =====================================================
    // RenderItem instance that holds vk::Context and
    // vk::Render objects. Pointer to this object
    // available in main application.
    Render {
        id: mainRenderItem
        NumberAnimation on t {
            running: true
            from: 0
            to: 1 << 63
            loops: Animation.Infinite
        }
    }

    // =====================================================
    Item {
        id: background
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        Rectangle {
            id: mainWindowDragArea
            anchors {
                top: parent.top
                right: parent.right
            }
            width: 64
            height: 64

            color: _color["background_additional_40"]


            /*
            Image {
                id: mainWindowDragIcon
                anchors.centerIn: parent
                source: "icons/arrow_all_direction_01.svg"
                sourceSize: Qt.size(
                                mainWindowDragIconHiddenImg.sourceSize.width * 2,
                                mainWindowDragIconHiddenImg.sourceSize.height * 2)
                Image {
                    id: mainWindowDragIconHiddenImg
                    source: parent.source
                    width: 0
                    height: 0
                }
                ColorOverlay {
                    anchors.fill: mainWindowDragIcon
                    source: mainWindowDragIcon
                    color: "white"
                }
            }
            */
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
                                       mainQuickViewHandle.move(delta.x,
                                                                delta.y)
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

            color: _color["background"]

            Text {
                id: testText

                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }

                color: "white"
                font: _fonts["label_accent"]
                text: "test test FOO"
            }

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
                    mainQuickViewHandle.noop()
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
