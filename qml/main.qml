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
    // or by id property.
    Render {}

    Connections {
        target: renderItemHandle

        function onContextinitialized() {}

        function onRenderInitialized() {
            mainUIComponentLoader.sourceComponent = mainUIComponent
        }
    }

    // Main ui component loading must be deffered,
    // it appears after render item fully initialized.
    Loader {
        id: mainUIComponentLoader
    }

    // Component that holds ui itself.
    Component {
        id: mainUIComponent

        Item {
            id: background

            // HACK: Item used to set Z (bigger?) value.
            // Also it can be some pivot point to another anchored items. because
            // by now mainWindow do not know its size and anchors to it not working.
            Rectangle {
                id: dummyz
                anchors {
                    // right: parent.right
                    // bottom: parent.bottom
                    centerIn: parent
                }
                width: 10
                height: 10
                color: "blue"
            }

            DragAbleItem {
                id: buttonsPanel

                height: reloadModelsButton.height + 16
                width: reloadModelsButton.width + closeButton.width + 32

                x: 32
                y: 32

                radius: _radius.half
                color: _color.background

                NpButton {
                    id: reloadModelsButton
                    anchors {
                        left: buttonsPanel.left
                        leftMargin: 8
                        verticalCenter: parent.verticalCenter
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
                        right: buttonsPanel.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
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

                width: 258
                height: vulkanInfoWidgetTitle.height + vulkanInfoWidgetTitle.anchors.topMargin
                        + deviceText.height + deviceText.anchors.topMargin
                        + driverText.height + driverText.anchors.topMargin
                        + apiText.height + apiText.anchors.topMargin + _gaps.half

                x: 32
                y: 128
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

                    padding: 0

                    color: "white"
                    font: _fonts.label_accent
                    text: "Vulkan info"
                }

                Text {
                    id: deviceText

                    anchors {
                        top: vulkanInfoWidgetTitle.bottom
                        topMargin: _gaps.quarter
                        left: parent.left
                        leftMargin: _gaps.half
                        right: parent.right
                    }

                    padding: 0

                    color: "white"
                    font: _fonts.label
                    text: "dev: " + renderItemHandle.infoRenderDevice(
                              )["device"]
                    elide: Text.ElideRight
                }

                Text {
                    id: driverText

                    anchors {
                        top: deviceText.bottom
                        topMargin: _gaps.quarter
                        left: parent.left
                        leftMargin: _gaps.half
                        right: parent.right
                    }

                    padding: 0

                    color: "white"
                    font: _fonts.label
                    text: "drv: " + renderItemHandle.infoRenderDevice(
                              )["driverVersion"]
                }

                Text {
                    id: apiText

                    anchors {
                        top: driverText.bottom
                        topMargin: _gaps.quarter
                        left: parent.left
                        leftMargin: _gaps.half
                        right: parent.right
                    }

                    padding: 0

                    color: "white"
                    font: _fonts.label
                    text: "api: " + renderItemHandle.infoRenderDevice(
                              )["apiVersion"]
                }
            }
        }
    }
}
