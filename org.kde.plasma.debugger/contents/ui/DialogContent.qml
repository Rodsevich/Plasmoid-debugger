import QtQuick 2.5

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.kquickcontrolsaddons 2.0 as KQuickControlsAddons

// DialogContent

PlasmaComponents.Dialog {

    id: dialog

    property string titleText: "Title"
    property string messageText: "Message"

    property string closeButton: ""
    property string rejectButton: ""
    property string acceptButton: "Ok"

    property int widthSize: 300

    signal closed

    location: PlasmaCore.Types.Desktop

    title: Item {
        width: widthSize
        height: widthSize * 0.2
        PlasmaExtras.Title {
            text: dialog.titleText
        }
    }

    content: Item {
        width: widthSize
        height: widthSize * 0.5
        PlasmaExtras.Heading {
            level: 2
            text: dialog.messageText
        }
    }

    buttons: PlasmaComponents.ButtonRow {
        PlasmaComponents.Button {
            text: dialog.closeButton;
            visible: dialog.closeButton != ''
            iconSource: "dialog-close"
            onClicked: {
                dialog.closed()
                dialog.close();
            }
        }
        PlasmaComponents.Button {
            text: dialog.rejectButton;
            visible: dialog.rejectButton != ''
            iconSource: "dialog-cancel"
            onClicked: {
                dialog.reject();
            }
        }
        PlasmaComponents.Button {
            text: dialog.acceptButton;
            visible: dialog.acceptButton != ''
            iconSource: "dialog-ok"
            onClicked: {
                dialog.accept();
            }
        }
    }
}
