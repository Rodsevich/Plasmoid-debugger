import QtQuick 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {



    ListView{
        id: todosView
        width: 150
        anchors {
            top: parent.top
            bottom: parent.bottom
        }
        cacheBuffer: 4
        model: 80
        delegate: PlasmaComponents.ListItem{
            enabled: true
            height: lbl.height
            anchors {
                left: parent.left
                right: parent.right
            }
            PlasmaComponents.Label {
                id: lbl
                text: "Textito "+index
                textFormat: Text.StyledText
            }
        }
    }
}
