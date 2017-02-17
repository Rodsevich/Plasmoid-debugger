import QtQuick 2.5
import QtQuick.Controls 1.4
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.private.filecalendarplugin 0.1

import "Components"

PlasmaComponents.Page {

    //boilerplate debugger

    property alias evento: ev

    property date start: addMinutes(new Date(), 3)
    property date end: addMinutes(new Date(), 23)

    CalendarEvent{
        id: ev
        summary: "sumario"
        description: "descripcion"
        startDateTime: start
        endDateTime: end
    }

    function addMinutes(date, minutes){
        date.setMinutes(date.getMinutes() + minutes);
        return date;
    }

    //Para pasar a TomaToDoING

    property QtObject cal: FileCalendar{
        id: cal
        uri: "/home/nico/.local/share/TomaToDoING/data.ics"
    }

    function _arrancar(){
        cartel.contador.start();
    }

    function _rearrancar(){
        cartel.contador.restart();
    }

    function _estado(){
        return cartel.contador.running;
    }

    function _runTrue(){
        cartel.contador.running = true;
        return cartel.contador.running;
    }

    function _runFalse(){
        cartel.contador.running = false;
        return cartel.contador.running;
    }

    property alias cartel: cartelito
    CalendarEventUI{
        id: cartelito
        y: 50
        width: parent.width * 0.85
        calendar: cal
    }

//    PlasmaComponents.TextField{
//        id: sorp
//        anchors.top: .bottom
//        width: fondoTexto.width
//        text: "SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI "
//    }

    Component.onCompleted: cal.addEvent(ev)
}
