import QtQuick 2.5
import QtQuick.Controls 1.4
import org.kde.plasma.components 2.0 as PlasmaComponents


PlasmaComponents.Page {

    property alias rectangle: rectangulo
    property alias rect_text: texto

    function _n_moveUp(num){
        rectangulo.y -= (num == null) ? 10 : num;
        return "up";
    }

    function _n_moveDown(num){
        rectangulo.y += (num == null) ? 10 : num;
        return "down";
    }

    function _ss_changeText(text,txt){
        rect_text.text = text + txt;
    }

    Rectangle{
        id: rectangulo
        y: 100
        color: "blue"
        width: 300
        height: 200
    }

    Label{
        id: texto
        anchors.centerIn: rectangulo
        text: "LABEL"
        color: "white"
    }
}
