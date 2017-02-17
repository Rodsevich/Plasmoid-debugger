import QtQuick 2.5
import QtQuick.Controls 1.4
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.private.filecalendarplugin 0.1

import "Components"

PlasmaComponents.Page {

//    property alias evento: ev

    function _nn_agregarEvent(from, to){
        var ev = Qt.createComponent("eventCreator.qml").createObject(null);
        ev.startDateTime = addSeconds(new Date(), from);
        ev.endDateTime = addSeconds(new Date(), to);
//        root.debug("agregando evento: " + ev.startDateTime + " " + ev.endDateTime);
        cal.addEvent(ev);
        return ev;
    }

    function _debugerHorariosEvts(){
        var evts_pasados = [];
        var evts_por_pasar = [];
        var ahora = Date.now();
        root.debug("Ahora: " + ahora);
        for(var i in cal.events){
            var ev = cal.events[i];
            if(ev.startDateTime < ahora)
                evts_pasados.push(ev);
            else
                evts_por_pasar.push(ev);
        }
        root.debug(evts_pasados.length + " - " + evts_por_pasar.length);
        evts_pasados.sort(function (a,b){ return (a.startDateTime < b.startDateTime) ? -1 : 1;});
        evts_por_pasar.sort(function (a,b){ return (a.startDateTime < b.startDateTime) ? -1 : 1;});
        for(i=0;i<evts_pasados.length;i++){
            var e = evts_pasados[i];
            root.debug('<font color="red">' + e.summary + ": " + e.startDateTime + " - " + e.endDateTime + "</font>");
        }
        for(i=0;i<evts_por_pasar.length;i++){
            e = evts_por_pasar[i];
            root.debug('<font color="green">' + e.summary + ": " + e.startDateTime + " - " + e.endDateTime + "</font>");
        }
    }

    function addMinutes(date, minutes){
        date.setMinutes(date.getMinutes() + minutes);
        return date;
    }

    function addSeconds(date, seconds){
        date.setSeconds(date.getSeconds() + seconds);
        return date;
    }

    //Para pasar a TomaToDoING
    CalendarEvent{
        id: ev1
        summary: "sumario1"
        description: "descripcion1"
        startDateTime: addSeconds(new Date(), 1)
        endDateTime: addSeconds(new Date(), 2)
    }
    CalendarEvent{
        id: ev2
        summary: "sumario2"
        description: "descripcion2"
        startDateTime: addSeconds(new Date(), 9)
        endDateTime: addSeconds(new Date(), 12)
    }
    CalendarEvent{
        id: ev3
        summary: "sumario3"
        description: "descripcion3"
        startDateTime: addSeconds(new Date(), 14)
        endDateTime: addSeconds(new Date(), 24)
    }
    property QtObject cal: FileCalendar{
        id: cal
        uri: "/home/nico/.local/share/TomaToDoING/data.ics"
        events: {
            var r = [ev1,ev2,ev3];
            for(var i in events)
                r.push(events[i]);
            return r;
        }
    }

    function _arrancar(){
        ceui.contador.start();
    }

    function _rearrancar(){
        ceui.contador.restart();
    }

    function _estado(){
        return ceui.contador.running;
    }

    function _runTrue(){
        ceui.contador.running = true;
        return ceui.contador.running;
    }

    function _runFalse(){
        ceui.contador.running = false;
        return ceui.contador.running;
    }

    property alias ceui: cartelito
    CalendarEventUI{
        id: cartelito
        y: 50
        width: parent.width * 0.85
        calendar: cal
        onEndedEvent: root.debug(event.summary + " ended.")
        onStartedEvent: root.debug(event.summary + " started.")
        onChangedEvent: root.debug(event.summary + " changed.")
    }

//    PlasmaComponents.TextField{
//        id: sorp
//        anchors.top: .bottom
//        width: fondoTexto.width
//        text: "SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI SORPISORPI "
//    }

//    Component.onCompleted: {
//        _nn_agregarEvent(1,3);
//        _nn_agregarEvent(33,61);
//        _nn_agregarEvent(8,16);
//    }

}
