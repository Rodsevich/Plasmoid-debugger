//import QtQuick 2.5
//import QtQuick.Layouts 1.1
//import QtQuick.Controls 1.4
//import org.kde.plasma.components 2.0 as PlasmaComponents

//import org.kde.plasma.plasmoid 2.0
//import "../code/logic.js" as Logic
//import org.kde.plasma.private.filecalendarplugin 0.1

//PlasmaComponents.Page {

//    property var arrg: {
//        "pp": 1,
//        "qq": "longa"
//    }

//    onArrgChanged: prueba.arreglo = arrg

//    Prueba{
//        id: prueba
//        uri: "/home/nico/src/calendario/build/dummy.ics"
//        arreglo: arrg

//        onCustomColorSchemeChanged: {
//            main.debug(customColorScheme.length);
//        }

//        onFileChanged: {
//            main.debug("Archivo cambiado:" + leer());
//        }
//        onUriChanged: {
//            main.debug("Archivo cambiado:" + leer());
//        }
//        onLongaChanged: main.debug("debug info: "+longa);
////        longa: "DEBUG OUTPUT NUNCA CAMBIADO"
////        todos: [
////            CalendarToDo{
////                priority: 1
////                summary: "sorpi"
////                percentCompleted: 96
////            },
////            CalendarToDo{
////                priority: 7
////                summary: "znork"
////                percentCompleted: 6
////            },
////            CalendarToDo{
////                priority: 8
////                summary: "ponga"
////                percentCompleted: 0
////            }
////        ]
//    }

//    CalendarToDo{
//        id: todo
//        priority: 3
//        summary: "sarasa"
//        percentCompleted: 66
//    }

//    CalendarEvent{
//        id: event
//        summary: "eventito"
//        startDateTime: new Date();
//    }

//    function escribirLongaToDo(){
//        return todo.escribirLongaToDo();
//    }

//    function escribirLongaIncidence(){
//        return todo.escribirLongaIncidence();
//    }

//    function getTodo(){
//        return todo;
//    }

//    function agregarToDo(){
//        prueba.addToDo(todo);
//        return "intenté agregar dos, el currentCount es: " + prueba.todos.length;
//    }

//    function agregarEvent(){
//        prueba.addEvent(event);
//        return "intenté agregar dos, el currentCount es: " + prueba.todos.length;
//    }

//    function getPrueba(){
//        return prueba;
//    }

//    function queDiceLonga(){
//        return prueba.longa;
//    }

//    function queDiceUri(){
//        return prueba.uri;
//    }

//    function setearUri(){
//        prueba.uri = "/home/nico/src/calendario/build/dummy.ics";
//        return prueba.uri;
//    }

//    function cargarCalendar(){
//        return prueba.loadCalendar();
//    }

//    function guardarCalendar(){
//        return prueba.saveCalendar();
//    }

//    function retTrue(){
//        return prueba.retTrue();
//    }

//    property alias td: todo
//    property alias prb: prueba
//    property alias evt: event

//    function agregarArreglo(s,v){
//        return prueba.agregarArreglo(s,v);
//    }

//    function customColorRule(regla, color){
//        return prueba.customColorRule(regla, color);
//    }

//    function agregaNomas(){
//        return prueba.customColorRule(5, 5);
//    }

//    function getSummary(){
//        return todo.summary;
//    }

//    function getPriority(){
//        return todo.priority;
//    }

//    function getPercentCompleted(){
//        return todo.percentCompleted;
//    }

//    function setSummary(s){
//        todo.summary = s;
//    }

//    function setPriority(p){
//        todo.priority = p;
//    }

//    function setPercentCompleted(p){
//        todo.percentCompleted = p;
//    }

//    property alias uril: prueba.uri

//    function cambiarUri(uri){
//        prueba.uri = uri;
//    }

//    RowLayout{

//        ColumnLayout{
//            PlasmaComponents.Button {
//                text: "escribir en output"
//                onClicked: main.debug(prueba.leer());
//            }
//            PlasmaComponents.Label{
//                text: "ToDos"
//            }

//            Repeater{
//                model: prueba.todos
//                delegate: PlasmaComponents.ToolButton {
//                    flat:false
//    //                text: {
//    //                    var ret = "<b>";// "<sup>"+index+"</sup><b>";
//    //                    if(model.tipo == "subObject")
//    //                          ret += "."+model.objectName+"</b>";
//    //                    else{
//    //                        if(model.tipo == "subIndex")
//    //                            ret += "["+model.objectName+"]</b>"
//    //                        else
//    //                            ret += model.objectName+"</b>";
//    //                    }
//    //                    return ret;
//    //                }
//                    text: "("+model.modelData.percentCompleted+"%) "+ model.modelData.summary
//                    width: text.length * theme.mSize(theme.defaultFont)
//                    onClicked: main.debug("ToDo " + index + ": " + Object.keys(model.modelData));
//                }
//            }
//        }

//        ColumnLayout{
//            PlasmaComponents.Label{
//                text: "Eventos"
//            }

//            Repeater{
//                model: prueba.events
//                delegate: PlasmaComponents.ToolButton {
//                    flat:false
//                    //                text: {
//                    //                    var ret = "<b>";// "<sup>"+index+"</sup><b>";
//                    //                    if(model.tipo == "subObject")
//                    //                          ret += "."+model.objectName+"</b>";
//                    //                    else{
//                    //                        if(model.tipo == "subIndex")
//                    //                            ret += "["+model.objectName+"]</b>"
//                    //                        else
//                    //                            ret += model.objectName+"</b>";
//                    //                    }
//                    //                    return ret;
//                    //                }
//                    text: "("+model.modelData.startDateTime.getTime()+") "+ model.modelData.summary+" ("+model.modelData.endDateTime.getTime()+")"
//                    width: text.length * theme.mSize(theme.defaultFont)
//                    onClicked: main.debug("ToDo " + index + ": " + Object.keys(model.modelData));
//                }
//            }
//        }
//    }
//}
