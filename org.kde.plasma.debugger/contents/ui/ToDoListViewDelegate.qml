//Highly "inspired" (if not copied) from org.kde.plasma.clipboard

import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Controls 1.0 as QtControls
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.private.filecalendarplugin 0.1
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem{

    id: todoItem

	height: texto.implicitHeight
    enabled: true
//	width: parent.width
	readonly property real gradientThreshold: (width - newTodoRow.width) / width

    onContainsMouseChanged: {
        if (containsMouse) {
            ListView.view.currentIndex = index
        } else {
            todosView.currentIndex = -1
        }
    }

    // this stuff here is used so we can fade out the text behind the tool buttons
    Item {
        id: labelMaskSource
        anchors.fill: parent
        visible: false

        Rectangle {
        anchors.centerIn: parent
        rotation: -90 // you cannot even rotate gradients without QtGraphicalEffects
        width: parent.height
        height: parent.width

        gradient: Gradient {
            GradientStop { position: 0.0; color: "white" }
            GradientStop { position: gradientThreshold - 0.25; color: "white"}
            GradientStop { position: gradientThreshold; color: "transparent"}
            GradientStop { position: 1; color: "transparent"}
        }
        }
    }
    OpacityMask {
        id: labelMask
        anchors.fill: texto
        cached: true
        maskSource: labelMaskSource
        visible: !!source && todoItem.ListView.isCurrentItem
    }

	QtControls.ProgressBar{
	    anchors.fill: parent
	    visible: !(model.modelData.completed || model.modelData.childUri !== "")
	    value: model.modelData.percentCompleted / 100
	    style: ProgressBarStyle{
            background: Rectangle{
                color: todoItem.ListView.isCurrentItem ?
                       theme.viewBackgroundColor
                     : theme.complementaryBackgroundColor
            }
            progress: Rectangle {
                color: todoItem.ListView.isCurrentItem ?
                       theme.visitedLinkColor
                     : Qt.rgba(theme.visitedLinkColor.r,
                       theme.visitedLinkColor.g,
                       theme.visitedLinkColor.b,
                       0.3)
            }
	    }
	}

	PlasmaComponents.Label {
	    id: texto
	    anchors.fill: parent
	    property string header: {
            if(!model.modelData.priority)
                return "h4"
            if(model.modelData.priority <= 2)
                return "h1"
            if(model.modelData.priority >= 8)
                return "h6"
            if(model.modelData.priority >= 6)
                return "h5"
            return 'h' + (model.modelData.priority - 1)
	    }
	    text: '<'+header+'>'+ model.modelData.summary +'</'+header+'>'
	    textFormat: Text.StyledText
	    font.underline: {
            var prioridades = [1,6,8];
            for(var i in prioridades)
                if(model.modelData.priority === prioridades[i])
                return true;
            return false;
	    }
	    font.strikeout: model.modelData.completed
	    color: {
            if(model.modelData.completed)
                return "gray"
            if(model.modelData.overdue)
                return "crimson"
            return theme.textColor
	    }
	}


	PlasmaCore.ToolTipArea{
	    anchors.fill: parent
	    mainText: model.modelData.summary
	    subText: model.modelData.description
	}

	Row {
	    id: toolButtonsLayout
	    anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
	    }
	    height: parent.height
        visible: todoItem.containsMouse
	    spacing: 0

	    PlasmaComponents.ToolButton {
            iconSource: "document-edit"
            tooltip: i18n("Edit ToDo")
            onClicked: editDialog.edit(model.modelData)
	    }
	    PlasmaComponents.ToolButton {
            iconSource: "edit-delete"
            tooltip: i18n("Delete ToDo")
            onClicked: main.debug("implementar borrado del ToDo")
	    }
	    PlasmaComponents.ToolButton {
            iconSource: "layer-lower"
            tooltip: i18n("Subdivide ToDo")
            onClicked: toDoNavModel.currentParentUid = model.modelData.uid;
	    }
	    PlasmaComponents.ToolButton {
            iconSource: "dialog-ok"
            visible: !model.modelData.completed
            tooltip: i18n("Mark ToDo as completed")
            onClicked: {
                model.modelData.completed = true;
                calendario.saveCalendar();
            }
	    }
	}

//	MouseArea{
//	    id: mArea
//	    anchors.fill: parent
//	    hoverEnabled: true
//	    cursorShape: model.modelData.completed ?
//			     Qt.ForbiddenCursor
//			   : Qt.ArrowCursor
//	}

}
