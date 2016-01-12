import QtQuick 2.5
import QtWebKit 3.0
import QtWebKit.experimental 1.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.private.filecalendarplugin 0.1
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {

    property alias ordndr: ordenador
    property alias base: reloj_base
//    property alias slts: selectores
    property alias wvw: webviewer

    /*
      current.chsr.wvw.url
    */

    Item{
        id: ordenador
        height: parent.height
        width: 200//parent.width
        PlasmaCore.SvgItem{
            id: reloj_base
            anchors.fill: parent
            svg: PlasmaCore.Svg{
                imagePath: plasmoid.file("images", "clock.svg")
                customColorScheme: {
                    "#fondo":"rgb(0,250,30)"
                }
            }
        }
        WebView{
            id: webviewer
            width: parent.width
            height: parent.height
            url: plasmoid.file("images", "selector.svg")
            experimental.transparentBackground: true
        }

//        AnimatedImage{
//            id: selectores
//            source: "file:///home/nico/.local/share/plasma/plasmoids/org.kde.pruebas/contents/images/w.svg"
//            playing: true
//        }

        PlasmaCore.SvgItem{
            id: reloj_brillo
            anchors.fill: parent
            svg: PlasmaCore.Svg{
                imagePath: plasmoid.file("images", "reflejo-clock.svg")
            }
        }

    }
//    ListView {
//        anchors {
//            top: parent.top
//            bottom: parent.bottom
//        }
//        id: lista
//        model: 100
//        cacheBuffer: 50
//        width: 300

//        delegate: Rectangle {
//            id: itemDelegatee
//            Component.onCompleted: showAnim.start();
//            transform: Rotation {
//                id:rt
//                origin.x: width
//                origin.y: height
//                axis {
//                    x: 0.3
//                    y: 1
//                    z: 0
//                }
//                angle: 0
//            }
//            width: parent.width
//            height: theme.mSize(theme.defaultFont).height * 2
//            color: index % 2 === 0 ? theme.buttonBackgroundColor : Qt.darker(theme.buttonBackgroundColor)
//            SequentialAnimation {
//                id: showAnim
//                running: false
//                RotationAnimation { target: rt; from: 180; to: 0; duration: 800; easing.type: Easing.OutBack; property: "angle" }
//            }
//        }
//    }
}
