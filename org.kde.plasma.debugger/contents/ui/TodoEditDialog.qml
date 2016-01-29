import QtQuick 2.0
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Controls.Styles.Plasma 2.0 as Styles
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.private.filecalendarplugin 0.1
import org.kde.plasma.components 2.0 as PlasmaComponents

// DialogContent

PlasmaComponents.Dialog {

    id: editDialog

    property CalendarToDo todoComponent
    property int widthSize: 350
    property string action: "Edit"
    property var initialValues: {
        "summary":null,
        "description":null,
        "priority":null
    }

    location: PlasmaCore.Types.Desktop

    title: Item {
        width: widthSize
        height: widthSize * 0.2
        PlasmaExtras.Title {
            text: action + " ToDo"
            horizontalAlignment: Text.AlignHCenter
        }
    }

    content: Item {
        width: widthSize
        height: widthSize * 0.43

        QtControls.TextField {
            id: summaryIpt
            style: Styles.TextFieldStyle {}
            anchors{
                top: parent.top
                left: parent.left
            }
            text: todoComponent.summary
            placeholderText: "Name"
            onTextChanged: todoComponent.summary = text
        }
        QtControls.SpinBox{
            id: priorityIpt
            width: 55
            anchors{
                top: parent.top
                right: parent.right
                left: summaryIpt.right
            }
            minimumValue: 1
            maximumValue: 9
            value: todoComponent.priority || 5
            onValueChanged: todoComponent.priority = value
        }
        PlasmaComponents.TextArea {
            anchors{
                top: summaryIpt.bottom
                left: parent.left
                bottom: parent.bottom
                right: parent.right
            }
            text: todoComponent.description
            placeholderText: "Description"
            onTextChanged: todoComponent.description = text
        }
    }

    buttons: PlasmaComponents.ButtonRow {
        PlasmaComponents.Button {
            text: "Cancel"
            iconSource: "edit-delete"
            onClicked: {
                reject();
            }
        }
        PlasmaComponents.Button {
            text: editDialog.action
            iconSource: "dialog-ok"
            onClicked: {
                accept();
            }
        }
    }

    function edit(todoComp){
        main.debug("Llamada a editar con action: "+action,true)
        todoComponent = todoComp;
        initialValues.summary = todoComp.summary;
        initialValues.priority = todoComp.priority;
        initialValues.description = todoComp.description;
        this.open();
    }

    onTodoComponentChanged: {
        if (todoComponent.summary.length == 0)
            action = "Create"
    }

    onRejected: {
        todoComponent.summary = initialValues.summary;
        todoComponent.priority = initialValues.priority;
        todoComponent.description = initialValues.description;
    }
}
