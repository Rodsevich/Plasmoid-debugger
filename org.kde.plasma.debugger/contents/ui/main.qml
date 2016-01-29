import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles.Plasma 2.0 as Styles

import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.extras 2.0 as PlasmaExtras
import org.kde.plasma.components 2.0 as PlasmaComponents

/*
        ¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡¡WARNING!!!!!!!!!!!!!!!!!!!!!

        Don't live-edit this!!!
        Instead, edit MainPage.qml
*/

import "../code/logic.js" as Logic
import "../code/localStorage.js" as DB

Item {
    id: main
    property alias exp: explorer

    PlasmaComponents.TabBar {
        id: tabBar

        height: main.height / 2;
        anchors {
//            left: main.left
            right: main.right
            top: main.top
        }
        tabPosition: Qt.RightEdge

        currentTab: pluginPage

        PlasmaComponents.TabButton { text: "Main workspace"; tab: mainPage; iconSource: "zoom-select-fit"}
//        PlasmaComponents.TabButton { text: "Custom plugin"; tab: pluginPage; iconSource: "plugins"}
        PlasmaComponents.TabButton { text: "Styles"; tab: stylePage; iconSource: "edit-paste-style"}
    }

    PlasmaComponents.TabGroup {
        id: tabGroup
        anchors {
            top: main.top
            left: main.left
            right: tabBar.left
            bottom: tabBar.bottom
        }

        MainPage {
            id: mainPage
        }

//        PluginPage {
//            id: pluginPage
//        }

        StylePage {
            id: stylePage
        }

    }

    property alias current: tabGroup.currentTab

    function debug(longa){
        Logic.debug(longa);
    }

    VariablesExplorer{
        id: explorer
        anchors {
            left: main.left
            top: tabGroup.bottom
            bottom: main.bottom
        }
        width: main.width / 3
    }

    PlasmaComponents.TextField{
        id: commandLine
        anchors {
            left: explorer.right
            right: main.right
            top: tabGroup.bottom
        }
        horizontalAlignment: TextInput.AlignHCenter
        placeholderText: "This will be debugged"
        Keys.onPressed: {
                if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                    Logic.debug(eval(this.text));
                    selectAll();
                    event.accepted = true;
                }
        }
    }

    FunctionTriggers {
        id: functionTriggers
        anchors {
            left: explorer.right
            right: debugOutput.left
            top: commandLine.bottom
            bottom: main.bottom
        }
        width: main.width / 3
        page: current
    }

    TextArea {
        id: debugOutput
        anchors {
            right: main.right
            top: commandLine.bottom
            bottom: main.bottom
        }
        readOnly: true
        width: main.width / 3

        Button{
            text: "Borrar todo"
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            onClicked: {
                debugOutput.text = '';
            }
        }
    }

//    Component.onCompleted: Logic.debug("LONGA");
//    DataEngine que explore el teclado apra ejecutar:
//    F6 --> borrar output
}
