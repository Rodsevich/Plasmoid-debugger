import QtQuick 2.5
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4

import "../code/logic.js" as Logic
import "../code/localStorage.js" as DB

Item {
    id: generalPage
    width: childrenRect.width
    height: childrenRect.height

    signal configurationChanged

    property alias cfg_bol: longa.checked
    property int cfg_ent

    ColumnLayout{
        CheckBox{
            id: longa
        }

        SpinBox{
            stepSize: 1.0
            value: cfg_ent
            onValueChanged: cfg_ent = value
        }
    }

}
