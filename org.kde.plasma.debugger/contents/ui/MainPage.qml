import QtQuick 2.5
import QtWebKit 3.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.extras 2.0 as PlasmaExtras
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

    property alias prb: svg_prueba

    function _sorp(){
        return "sorp";
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

//        ToDosList{
//            id: todosList
//        }

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

////    ListView {
////        id: list
////        model: 5
////        spacing: 3
////        width: 100
////        anchors.top: parent.top
////        anchors.bottom: parent.bottom

////        delegate: Rectangle {
////            id: delegado
////            width: parent.width
////            height: 50
////            border.color: theme.buttonTextColor
////            color: theme.buttonBackgroundColor

////            Label{
////                anchors.centerIn: parent
////                text: "Cuadrado "+ index
////                color: theme.buttonTextColor
////                fontSizeMode: Text.HorizontalFit
////            }
////        }
////    }

//    CalendarEvent{
//        id: evento
//        startDateTime: new Date()
//        endDateTime: new Date().setHours(new Date().getHours() + 1)
//        summary: "Evento Longa"
//    }

//    FileCalendar{
//        id: filecal
//        uri: "/home/nico/src/calendario/build/dummy.ics"
//    }

//    CalendarToDo{
//        id: todo
//        summary: "Hacer un Longa"
//        percentCompleted: 10
//    }

//    function intercambiarSVG(){
//        tomate.intercambiarSVG();
//    }

//    function cambiarElemento(elem){
//        tomate.tmt.elementId = elem;
//    }

    function recargarSVG(){
        tomate.main.customColorRule("#circulo","orange");
        tomate.main.customColorRule("#cuadrado","yellow");
        var ip = tomate.main.imagePath;
        tomate.main.imagePath = "sorp";
        tomate.main.imagePath = ip;
        return JSON.stringify(tomate.main.customColorScheme);
    }

    function agregarColor(regla, color){
        return tomate.main.customColorRule(regla,color);
    }

    function cambiarColorGroup(num){
        return tomate.main.colorGroup = num;
    }

    function cambiarCuadrado(color){
        return tomate.main.customColorRule("#cuadrado",color);
    }

    function setearClock(){
        setearSVG("clock");
    }

    function setearSVG(src){
        tmt.main.imagePath = "/home/nico/.local/share/plasma/plasmoids/org.kde.pruebas/contents/images/" + src + ".svg";
        return tmt.main.isValid();
    }

//    function _retSorp(){
//        return "sorp";
//    }

//    function _s_rorp(){
//        return "sorp";
//    }

//    function _cn_etSorp(){
//        return "sorp";
//    }

//    function __orp(){
//        return "sorp";
//    }

//    function _sc_colorChange(rule, color){
//        return current.chsr.base.svg.customColorRule("#fondo","blue");
//    }

//    function guardarCalendario(){
//        return filecal.saveCalendar();
//    }

//    function agregarToDo(){
//        return filecal.addTodo(todo);
//    }
//    function agregarevento(){
//        return filecal.addEvent(evento);
//    }


}
