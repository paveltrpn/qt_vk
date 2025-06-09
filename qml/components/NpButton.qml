import QtQuick 2.15
import QtQuick.Templates 2.15 as QuickTemplates
import QtQuick.Controls.impl 2.15

import Tire 1.0

QuickTemplates.Button {
    id: control

    property alias backgroundRadius: backgroundItem.radius

    property bool isBorderEnabled: false

    property color backgroundColor: Theme.colors["background_additional_20"]

    states: [
        State {
            when: control.down
            PropertyChanges {
                target: control
                backgroundColor: Theme.colors["accent_focus_80"]
            }
        },
        State {
            when: control.hovered
            PropertyChanges {
                target: control
                backgroundColor: Theme.colors["background_additional_40"]
            }
        }
    ]

    property color textColor: Theme.colors["main_contrast"]

    // icon.color: textColor
    // implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
    // implicitContentWidth + leftPadding + rightPadding)
    // implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
    // implicitContentHeight + topPadding + bottomPadding)
    width: 40
    height: 40

    topPadding: 4
    bottomPadding: 4
    leftPadding: 4
    rightPadding: 4

    // spacing: 8
    background: Rectangle {
        id: backgroundItem
        anchors.fill: parent
        color: control.backgroundColor
        opacity: control.enabled ? 1.0 : 0.3
        radius: 8
    }

    contentItem: IconLabel {
        spacing: control.spacing
        mirrored: control.mirrored
        display: control.display
        icon: control.icon
        text: control.text
        font: control.font
        color: control.textColor
        opacity: control.enabled ? 1.0 : 0.3
    }
}
