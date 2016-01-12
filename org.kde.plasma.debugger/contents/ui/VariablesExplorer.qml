import QtQuick 2.5
import QtQuick.Controls 1.4
import org.kde.plasma.extras 2.0 as PlasmaExtras
import QtQuick.Controls.Styles.Plasma 2.0 as Styles
import org.kde.plasma.components 2.0 as PlasmaComponents

import "../code/logic.js" as Logic

Item {

    function exploreObject(obj){
        explorerModel.clear();
        var members = Logic.stringify(obj).split(',');
        for (var i in members){
            members[i] = members[i].replace(/["} {]/g, "");
            var data = {
                propertyName: members[i],
                type: typeof obj[members[i]]
            }
            data.value = data.type === "object" ? "{}" : obj[members[i]]+""; //convert to string

            explorerModel.append(data);
        }
    }

    function explorerResetTo(index){
        if(explorerMenuModel.remove(index + 1, explorerMenuModel.count - index - 1))
            exploreObject(explorerMenuModel.object);
    }

    function exploreSubElements(text){
        var subElem, type, components = text.split(".");
        for( var i in components){
            subElem = components[i];
            type = explorerMenuModel.test(subElem);
            if ( !type )
                return subElem;
            explorerMenuModel.append({"objectName": subElem, "tipo": ""+type}); //Apparently ListElement forces all object to be of the same type, so must tranform any possibly boolean value to string in order to have strings later
        }
        exploreObject(explorerMenuModel.object);
        return ""
    }

    function explorerRequest(text){
        explorerInput.text = exploreSubElements(text);
        if(explorerInput.text == "")
            explorerInput.textColor = explorerInput.defaultTextColor;
        else
            explorerInput.textColor = "red";
    }

    function explorerDebugVariable(name, type){
        if (type == "raw")
            commandLine.text = explorerMenuModel.objectName;
        else if (type == "function")
            commandLine.text = explorerMenuModel.objectName+"."+name+"()";
        else
            switch(explorerMenuModel.test(name)){
                case "subIndex":
                    commandLine.text = explorerMenuModel.objectName+"["+name+"]";
                break;
                case "subObject":
                    commandLine.text = explorerMenuModel.objectName+"."+name;
            }
        commandLine.forceActiveFocus();
    }


    property var explorerIcons: {
        "object":"debug-step-into",
        "number":"depth16to8",
        "function":"im-facebook",
        "boolean":"format-text-bold",
        "string":"TeX_logo",
        "undefined":"edit-delete"
    }

    property var explorerColors: {
        "object":"#FAA",
        "number":"#C9D",
        "function":"#DD3",
        "boolean":"#AEB",
        "string":"#BCE",
        "undefined":"#AAA"
    }

    ListModel {
        id: explorerModel
    }
    ListModel {
        id: explorerMenuModel

        property string objectName: ""
        property var object: eval(objectName)

        function test(str){
            var ret = false;
            try {
                if(count == 0)
                    ret = eval(str) !== undefined;
                else
                    ret = (eval(objectName+"."+str) !== undefined) ? "subObject" : false;
            }catch(e){
                ret = (object[str] !== undefined) ? "subIndex" : false;
            }finally{
                return ret;
            }
        }

        function updateAll(){
            rebuildName();
            exploreObject(object);
            explorerObjectTypeOutput.text = "["+object.toString()+"] "+objectName;
        }

        function rebuildName(){
            if (count > 0){
                objectName = get(0).objectName;
                for (var i = 1; i < count; i++)
                    objectName += get(i).tipo == "subObject" ?
                                    "."+get(i).objectName
                                  : get(i).tipo == "subIndex" ?
                                        "["+get(i).objectName+"]"
                                      : "";
            }else
                objectName = "";
        }

        onRowsInserted: updateAll()
        onRowsRemoved: updateAll()
    }

    Column {

        id:explorer
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        width: parent.width

//        PlasmaExtras.ScrollArea {
//        Item {
        Flickable {
            id: explorerMenuScrollArea
            anchors {
                left: parent.left
                right: parent.right
            }
            width: main.width / 3
            height: explorerMenu.height

                contentWidth: explorerMenuRow.width
                contentHeight: explorerMenuRow.height
                clip: true

                Item {
                    width: Math.max(parent.width, explorerMenuRow.width)
                    height: explorerMenuRow.height
                    id: explorerMenu


                    Row {
                        id: explorerMenuRow
                        PlasmaComponents.ButtonRow {
                            spacing: 0
                            exclusive: true

                            PlasmaComponents.ToolButton {
                                flat:false;
                                iconSource: "go-previous"
                                visible: explorerMenuModel.count > 0
                                onClicked: explorerResetTo(explorerMenuModel.count - 2)
                            }

                            PlasmaComponents.ToolButton {
                                flat:false
                                iconSource: "edit-delete"
                                visible: explorerMenuModel.count > 0
                                onClicked: explorerMenuModel.clear()
                            }

                            PlasmaComponents.ToolButton {
                                flat:false;
                                iconSource: "view-refresh"
                                visible: explorerMenuModel.count > 0
                                onClicked: exploreObject(explorerMenuModel.object)
                            }

                            Repeater{
                                model: explorerMenuModel
                                delegate: PlasmaComponents.ToolButton {
                                    flat:false
                                    text: {
                                        var ret = "<b>";// "<sup>"+index+"</sup><b>";
                                        if(model.tipo == "subObject")
                                              ret += "."+model.objectName+"</b>";
                                        else{
                                            if(model.tipo == "subIndex")
                                                ret += "["+model.objectName+"]</b>"
                                            else
                                                ret += model.objectName+"</b>";
                                        }
                                        return ret;
                                    }
                                    width: text.length * theme.mSize(theme.defaultFont).width
                                    onClicked: explorerResetTo(index)
                                }
                            }

                        }

                        PlasmaComponents.TextField{
                            id: explorerInput
                            property color defaultTextColor: theme.viewTextColor
                            property int letterWidth: theme.mSize(theme.defaultFont).width - 3 //will work well in most cases where the user doesn't likes to call all his variables "mmmmmmm"
                            placeholderText: "Explore variable contents"
                            width: Math.max(
                                       text.length * letterWidth,
                                       placeholderText.length * letterWidth
                                       )
                            Keys.onPressed: {
                                if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                                    event.accepted = true;
                                    explorerRequest(text);
                                } else if (textColor == "#ff0000"){
                                    textColor = defaultTextColor;
                                    text = "";
                                }
                            }
                        }
                    }
                }
            }
//        }

        PlasmaExtras.ScrollArea {

            anchors {
                left: parent.left
                right: parent.right
            }
            height: parent.height

            Flickable {
                id: flickable
                contentWidth: column.width
                contentHeight: column.height + explorerObjectTypeOutput.height
                clip: true

                Item {
                    width: Math.max(flickable.width, column.width)
                    height: column.height
                    Column {
                        id: column
                        PlasmaComponents.ButtonColumn {
                            id: buttons
                            exclusive: true
                            spacing: 0
                            width: main.width / 3

                            PlasmaComponents.ToolButton {
                                id: explorerObjectTypeOutput
                                text: "[null] - null"
                                onClicked: explorerDebugVariable("", "raw")
                            }

                            Repeater{
                                model: explorerModel
                                delegate: PlasmaComponents.ToolButton {
                                    flat:false
                                    text: "<font color='"+explorerColors[model.type]+"'><b>"+model.propertyName+"</b></font>: <i>"+model.value+"</i>"
                                    iconSource: explorerIcons[model.type]
                                    onClicked: model.type == "object" ?
                                                   exploreSubElements(model.propertyName)
                                                 : explorerDebugVariable(model.propertyName, model.type);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
