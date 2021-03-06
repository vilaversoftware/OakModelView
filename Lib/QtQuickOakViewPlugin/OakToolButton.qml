import QtQuick 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0

ToolButton {
    property alias toolTip: toolTipId.text
    property alias imageSource: imageId.source

    Image {
        id: imageId
        anchors.fill: parent
        anchors.margins: parent.height/7
        fillMode: Image.PreserveAspectFit
        layer.enabled: !parent.enabled
        layer.effect: Desaturate {
            desaturation: 1
            Behavior on desaturation { NumberAnimation { easing.type: Easing.InOutQuad } }
        }
    }
    hoverEnabled: true
    highlighted: hovered
    ToolTip {
        id: toolTipId
        visible: parent.hovered
    }
}
