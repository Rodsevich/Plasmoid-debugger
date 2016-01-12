import QtQuick 2.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

// DialogContent

PlasmaComponents.Dialog {

    id: dialogAlert

    property string messageText: "Message"
    property int widthSize: 200

    location: PlasmaCore.Types.Desktop

    title: Item {
        width: widthSize
        height: widthSize * 0.2
        PlasmaExtras.Title {
            text: "Alert!"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    content: Item {
        width: widthSize
        height: widthSize * 0.5
        PlasmaExtras.Heading {
            level: 2
            text: dialog.messageText
            horizontalAlignment: Text.AlignHCenter
        }
    }

    buttons: PlasmaComponents.ButtonRow {
        PlasmaComponents.Button {
            text: "Ok"
            iconSource: "dialog-ok"
            onClicked: {
                dialog.accept();
            }
        }
    }
}
