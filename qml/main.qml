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

    // RenderItem instance that holds vk::Context and
    // vk::Render objects. Pointer to this object
    // available in main application.
    // Can be accesed as "renderItemHandle.doSomeAction()"
    // or bu id property.
    Render {}

    Connections {
        target: renderItemHandle

        function onContextinitialized() {}

        function onRenderInitialized() {
            mainUIComponentLoader.sourceComponent = mainUIComponent
        }
    }

    // Main ui component loading must be
    // deffered after render item fully initialized.
    Loader {
        id: mainUIComponentLoader
        anchors {
            // left: parent.left
            // right: parent.right
            // top: parent.top
            // bottom: parent.bottom
        }
    }

    // Component that holds ui itself.
    Component {
        id: mainUIComponent

        Item {
            id: background
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
            Rectangle {
                id: dummyz
                anchors {
                    //right: parent.right
                    //bottom: parent.bottom
                    centerIn: parent
                }
                width: 100
                height: 100
                color: "blue"
            }

            DragAbleItem {
                id: leftPanel

                height: reloadModelsButton.height + 32
                width: reloadModelsButton.width + closeButton.width + 32

                x: 100
                y: 100

                radius: _radius.half
                color: _color.background

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
                        mainWindowHandle.noop()
                        renderItemHandle.noop()
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
                        // Send QQmlEngine::quit()
                        Qt.quit()
                    }
                }
            }

            DragAbleItem {
                id: vulkanInfoWidget

                height: 128
                width: 258

                x: 400
                y: 100
                z: parent.z

                radius: _radius.half
                color: _color.background

                Text {
                    id: vulkanInfoWidgetTitle

                    anchors {
                        top: parent.top
                        topMargin: _gaps.half
                        left: parent.left
                        leftMargin: _gaps.half
                        right: parent.right
                    }

                    height: 32
                    color: "white"
                    font: _fonts.label_accent
                    text: "Vulkan info"
                }

                Text {
                    id: device

                    anchors {
                        top: vulkanInfoWidgetTitle.bottom
                        topMargin: _gaps.half
                        left: parent.left
                        leftMargin: _gaps.half
                        right: parent.right
                    }

                    height: 32
                    color: "white"
                    font: _fonts.label
                    text: renderItemHandle.infoRenderDevice()
                }
            }
        }
    }
}
