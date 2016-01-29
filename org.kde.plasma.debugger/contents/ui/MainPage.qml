import QtQuick 2.5
import QtWebKit 3.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.kquickcontrols 2.0 as KQuickControls
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.kde.plasma.private.filecalendarplugin 0.1

PlasmaComponents.Page {

//    property alias cldr: filecal
//    property alias td: todo
//    property alias evt: evento
//    property alias tdvw: todosView
//    property alias evw: eventosView
//    property alias tmt: tomate
//    property alias chsr: todochooser

//    PlasmaCore.ColorScope{
//        id: colorScope
//    }

    property alias test: svg_prueba
    property alias tdl: todosList
    property alias lab: labe
    property alias tx: texto

    function _loadCalendar(){
        return todosList.fc.loadCalendar();
    }

    function _abrirDialog(){
        current.tdl.children[1].open();
    }

    function _n_editarToDo(num){
        current.tdl.children[1].edit(current.tdl.modelo.todos[num]);
    }

    function _funca(){
        return [12, 5, 8, 130, 44].filter(function(q){return q < 9});
    }

    function _s_cambiarUid(uid){
        tdl.modelo.currentParentUid = uid;
        return "puesto el UID de "+tdl.fc.componentByUid(uid).summary;
    }

    function _todosUIDs(){
        var pp = tdl.fc.todos;
        var ret = "";
        for(var i in pp){
            ret += pp[i].summary+": "+pp[i].uid+"\n";
        }
        return ret;
    }
    function _todosLenght(){
        return tdl.fc.todos.length;
    }
    function _Mtodos(){
        return tdl.modelo.todos;
    }
    function _MtodosLenght(){
        return tdl.modelo.todos.length;
    }

    RowLayout{
        anchors {
            top: parent.top
            bottom: parent.bottom
        }

        PlasmaCore.SvgItem{
            id: svg_prueba
            svg: PlasmaCore.Svg{
                imagePath: plasmoid.file("images", "w.svg")
            }
        }

        Column{
            PlasmaComponents.Label {
                id: labe
                width: 200
                height: 30
                text: texto.text
                textFormat: Text.StyledText
            }
            PlasmaComponents.TextField{
                id: texto
                width: 300
                height: 30
            }
            KQuickControls.ColorButton{
                id: elegidorDeColor
                dialogTitle: "elegi un color"
                showAlphaChannel: true
                color: "#0000FF"
            }
            PlasmaComponents.Label{
                text: "Mirá ma, me están cambiando!"
                color: elegidorDeColor.currentColor
            }
        }
//        ToDoChooser{
//            id: todochooser
//            width: 500
//            height: 400
//        }

//        Tomate{
//            width: 200
//            height: 400
//            id: tomate
//        }

//        ListView{
//            id: todosView
//            cacheBuffer: 4
//            model: 80
//            delegate: PlasmaComponents.ListItem{
//                enabled: true
//                height: 30
//                width: parent.width
//                PlasmaComponents.Label {
//                    text: "Sorpi"
//                    textFormat: Text.StyledText
//                }
//            }
//        }

        ToDosList{
            id: todosList
            width: 250
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
        }

//        ListView{
//            id: eventosView
//            width: 150
//            height: parent.height
//            model: filecal.events
//            delegate: PlasmaComponents.ListItem{
//                enabled: true
//                content: model.modelData.summary
//            }
//        }
    }

//    PlasmaComponents.ButtonColumn {
//        anchors.right: parent.right
//        spacing: 2
//        width: 140
//        height: 200
//        PlasmaComponents.Button { text: "Top" }
//        PlasmaComponents.Button { text: "Bottom" }
//    }

//    ListView {
//        id: list
//        model: 5
//        spacing: 3
//        width: 100
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom

//        delegate: Rectangle {
//            id: delegado
//            width: parent.width
//            height: 50
//            border.color: theme.buttonTextColor
//            color: theme.buttonBackgroundColor

//            Label{
//                anchors.centerIn: parent
//                text: "Cuadrado "+ index
//                color: theme.buttonTextColor
//                fontSizeMode: Text.HorizontalFit
//            }
//        }
//    }

}
