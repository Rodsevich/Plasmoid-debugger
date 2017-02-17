import QtQuick 2.5
import QtQuick.Controls 1.4
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.Page {

    property alias rectangle: rectangulo
    property alias rect_text: texto

    function _nn_setSizes(h,w){
        rectangulo.height = h;
        rectangulo.width = w;
        return "Rectangle now has " + h + "x" + w;
    }

    function _c_setColor(col){
        rectangulo.color = col;
        return "Rectangle now is " + col;
    }

    Rectangle{
        id: rectangulo
        y: 100
        color: "yellow"
        width: 300
        height: 200
    }

    Label{
        id: texto
        anchors.centerIn: rectangulo
        text: "LABEL"
        color: "blue"
    }

    Label{
        anchors.bottom: rectangulo.bottom
        color: black
        text: "Access this rectangle with current.rectangle\n and it's label with current.rect_text"
    }
}
