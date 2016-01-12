import QtQuick 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
//import org.kde.plasma.private.filecalendarplugin 2.0

Item{

    /*
    current.tmt.main.customColorScheme
    current.tmt.main.customColorRule("circulo","red")
    */

    property alias tmt: tomate
    property alias main: main_svg

    PlasmaCore.Svg{
        id: main_svg
        imagePath: plasmoid.file("images", "clock.svg")
        customColorScheme: {
//            "#espiral":"green",
//            ".simple":"blue",
//            "#cuadrado":"red",
//            "#circulo":"yellow",
//            "#Gdr-start":"blue",
//            "#Gdr-end":"orange"
        }
    }
    PlasmaCore.SvgItem{
        id: tomate
        anchors.fill: parent
        svg: main_svg
    }

    function changeColor(color){

    }

    function intercambiarSVG(){
        tomate.svg = (tomate.svg === custom_svg) ? main_svg : custom_svg;
        var sorp = (tomate.svg === custom_svg) ? "main_svg" : "custom_svg";
        main.debug(sorp);
    }

}
