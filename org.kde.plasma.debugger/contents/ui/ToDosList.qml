//Highly "inspired" (if not copied) from org.kde.plasma.clipboard

import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Controls 1.0 as QtControls
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.private.filecalendarplugin 0.1
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {

    id: todoList
    //Pa debug nomas:
    property alias lista: todosView
    property alias modelo: toDoNavModel
    property alias fc: calendario
    property alias ed: editDialog

    property string calendarFileUri: "/home/nico/.local/share/TomaToDoING/data.ics"

    FileCalendar{
        id: calendario
        uri: calendarFileUri
        onFileChanged: {main.debug("File changed");loadCalendar()}
        onTodosChanged: toDoNavModel.reload()
    }

    ListModel {
        id: toDoNavModel
        property string currentParentUid: ""
        property var todos: todosComponents()

        onCurrentParentUidChanged: {
            clear();
            var uid = currentParentUid;
            while(uid !== ""){
                var comp = calendario.componentByUid(uid);
                append({"uid":comp.uid, summary: comp.summary});
                uid = comp.parentUid;
            }
//            //Reverse the elements
            for(var i = 0; i < count - 1; i++)
                move(count - 1, i, 1);
            todos = todosComponents();
        }

        function resetTo(index){
            var cant = count - 1 - index;
            remove(index + 1, cant);
            var lastElem = get(count - 1);
            currentParentUid = lastElem ? lastElem.uid : "";
        }

        function reload(){
            todos = [];
            todos = todosComponents();
        }

        function todosComponents(){
            var filtered = [];
            for (var i in calendario.todos){
                if(calendario.todos[i].parentUid === currentParentUid){
                    if(calendario.todos[i].completed)
                        if(calendario.todos[i].completedDate.toDateString() !== new Date().toDateString())
                            continue; //If not completed today, better filter it out
                    filtered.push(calendario.todos[i]);
                }
            }
            return filtered;
        }
    }

    //current.tdl.lista.children[0].children[4].children[2]
    //current.tdl.fc.todos[3].parentUid
    //current.tdl.fc.componentByUid("d6d1e38b-9b0b-4621-b0e0-a712c127b9d4")
    //current.tdl.fc.componentByUid("86d0a32d-de2d-4a8c-bb82-b3f29aac701f").padre

    Column {
        anchors {
            fill: parent;
        }
        Flickable {
            id: explorerMenuScrollArea
            anchors {
                left: parent.left
                right: parent.right
            }
            height: todoRow.height
            Item {
                width: Math.max(parent.width, todoRow.width)
                height: todoRow.height

                Row {
                    id: todoRow
                    PlasmaComponents.ButtonRow {
                        spacing: 0
                        exclusive: true

                        PlasmaComponents.ToolButton {
                            flat:false;
                            iconSource: "go-previous"
                            visible: toDoNavModel.count > 0
                            onClicked: toDoNavModel.resetTo(toDoNavModel.count - 2)
                        }

                        PlasmaComponents.ToolButton {
                            flat:false
                            iconSource: "edit-delete"
                            visible: toDoNavModel.count > 0
                            onClicked: toDoNavModel.currentParentUid = ""
                        }

                        PlasmaComponents.ToolButton {
                            flat:false;
                            iconSource: "view-refresh"
                            onClicked: toDoNavModel.reload()
                        }

                        Repeater{
                            model: toDoNavModel
                            delegate: PlasmaComponents.ToolButton {
                                flat:false
                                text: model.summary
                                width: text.length * theme.mSize(theme.defaultFont).width
                                onClicked: toDoNavModel.resetTo(index)
                            }
                        }
                    }
                }
            }
        }

        ListView{

            id: todosView
            focus: true
            boundsBehavior: Flickable.StopAtBounds
            interactive: contentHeight > height
            highlight: PlasmaComponents.Highlight { }
            highlightMoveDuration: 0
            highlightResizeDuration: 0
            currentIndex: -1
            cacheBuffer: 4
            model: toDoNavModel.todos
            width: parent.width
            height: Math.min(contentHeight, parent.height)
            delegate: ToDoListViewDelegate {
                width: parent.width
            }
        }
        Row {
            id: newTodoRow
            property bool withFocus: newToDoPriority.focus || newToDoSummary.focus
            width: parent.width
            height: Math.max(newToDoPriority.implicitHeight, newToDoSummary.implicitHeight)
            PlasmaComponents.TextField{
                id: newToDoSummary
                height: parent.height
                anchors{
                    top: parent.top
                    right: newToDoPriority.left
                    left: parent.left
                }
                text: todoComponent.summary
                placeholderText: "New ToDo"
            }
            QtControls.SpinBox{
                id: newToDoPriority
                width: 55
                height: parent.height
                anchors{
                    top: parent.top
                    right: parent.right
                }
                minimumValue: 1
                maximumValue: 9
                value: todoComponent.priority || 5
            }
        }
    }

    //ToDo Editor

    TodoEditDialog{
        id: editDialog
        onAccepted: calendario.saveCalendar();
    }


    //ToDo Creator:

    property CalendarToDo todoComponent

    Component{
        id: toDoFactory

        CalendarToDo{
            description: "Fast-created with TomaToDoING"
        }
    }

    function makeNewToDo(){
        todoComponent = toDoFactory.createObject(todoList);
    }

    Component.onCompleted: {
        makeNewToDo();
    }

    focus: true
    Keys.onPressed: {
        switch(event.key) {
            case Qt.Key_Up: {
                todosView.decrementCurrentIndex();
                event.accepted = true;
                break;
            }
            case Qt.Key_Down: {
                todosView.incrementCurrentIndex();
                event.accepted = true;
                break;
            }
            case Qt.Key_Enter:
            case Qt.Key_Return: {
                event.accepted = true;
                if( newTodoRow.withFocus && newToDoSummary !== ""){
                    main.debug("condiciones dadas");
                    if(toDoNavModel.currentParentUid !== "")
                        todoComponent.parentUid = toDoNavModel.currentParentUid;
                    todoComponent.summary = newToDoSummary.text
                    todoComponent.priority = newToDoPriority.value
                    todoComponent.description = todoComponent.description + new Date()
                    calendario.addToDo(todoComponent);
                    calendario.saveCalendar();
                    makeNewToDo();
                    toDoNavModel.reload();
                }
            }
        }
    }
}
//            todos: [
//                CalendarToDo{
//                    priority: 1
//                    summary: "sorpi1"
//                    percentCompleted: 100
//                },
//                CalendarToDo{
//                    priority: 2
//                    summary: "sorpi2"
//                    percentCompleted: 46
//                },
//                CalendarToDo{
//                    priority: 3
//                    summary: "sorpi3"
//                    percentCompleted: 100
//                },
//                CalendarToDo{
//                    priority: 4
//                    summary: "sorpi"
//                    percentCompleted: 86
//                },
//                CalendarToDo{
//                    priority: 5
//                    summary: "sorpi5"
//                    percentCompleted: 100
//                },
//                CalendarToDo{
//                    priority: 6
//                    summary: "sorpi6"
//                    percentCompleted: 66
//                },
//                CalendarToDo{
//                    priority: 7
//                    summary: "sorpi7"
//                    percentCompleted: 23
//                },
//                CalendarToDo{
//                    priority: 8
//                    summary: "sorpi8"
//                    percentCompleted: 0
//                },
//                CalendarToDo{
//                    priority: 9
//                    summary: "sorpi9"
//                    percentCompleted: 6
//                }
//            ]
