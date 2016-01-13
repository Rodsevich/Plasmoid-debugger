import QtQuick 2.5

import "../code/logic.js" as Logic

Item {

    id: funcTriggers
    property var page
    property var functions: []
    property var components: []

    implicitHeight: debugTriggers.implicitHeight

    function updateFunctionName(index){
        var str = components[index].funcion + "( ";
        for(var i in components[index].arguments){
            str += components[index].arguments[i].text;
            if(i == components[index].arguments.length - 1)
                str += " )";
            else
                str += ", ";
        }
        components[index].button.text = str;
    }

    function triggerButton(index){
        main.debug(eval("main.current."+components[index].prefix+components[index].button.text));
//        components[index].output.text = components[index].button.text;
    }

    Column {
        id: debugTriggers
        width: parent.width
    }

    function destroyTriggers(){
        for(var i in debugTriggers.children)
            debugTriggers.children[i].destroy();
    }

    function renderTriggers(){
        var imports = "import QtQuick 2.5; import org.kde.plasma.components 2.0 as PlasmaComponents;"
        var argCounter, index = 0;
        for(var func in functions){
            components[index] = {
                "prefix": /^.*_/.exec(func)[0],
                "funcion": /[^_]+$/.exec(func)[0],
                "button": null,
                "arguments": [],
                "output": null
            };
            var row = Qt.createQmlObject(imports + "Row {}", debugTriggers);
            components[index].button = Qt.createQmlObject(imports + "PlasmaComponents.Button {
                    text: '" + components[index].funcion + "( )';
                    onClicked: {
                        triggerButton("+ index +")
                    }
                    Keys.onReturnPressed: {
                            if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                                triggerButton("+ index +")
                                event.accepted = true;
                            }
                    }
                }", row);
            if( functions[func] !== ""){
                var arguments = functions[func].split(',');
                components[index].arguments = functions[func].split(',');

                argCounter = 0;
                for (var arg in arguments){
                    components[index].arguments[argCounter] = Qt.createQmlObject(imports + "PlasmaComponents.TextField{
                        placeholderText: '" + arguments[arg] + "';
                        onTextChanged: {
                            updateFunctionName(" + index + ");
                        }
                    }", row);
                    argCounter++;
                }
            }
//            function's execution status label
//            components[index].output = Qt.createQmlObject(imports + "PlasmaComponents.Label {
//                text: 'not executed yet'
//            }", row);

            index++;
        }
    }

    function setFunctionNames(){
        //Can't do much becoase of bug https://bugreports.qt.io/browse/QTBUG-46122
        var func,name,paramsStr,arguments,
                members = Logic.stringify(page).split(','),
                displayableFunction = /^_/,
                param, re = /[nsc_]/; //RegExp used to match implicit parameters additions
        for (var i in members){
            //Clean rubbish
            name = members[i].replace(/["} {]/g, "");
            //Skip functions that doesn't starts with _
            if( ! displayableFunction.test(name) )
                continue;
            if (typeof page[name] == "function"){
                //If it has implicit parameters
                paramsStr = "";
                if(/^_.+_/.test(name)){
                    func = name.replace(/^_/,""); //name of function without _
                    var continuar = true;
                    while(continuar){
                        param = re.exec(func);
                        switch(param[0]){
                            case "s":
                                paramsStr += "string,";
                            break;
                            case "n":
                                paramsStr += "number,";
                            break;
                            case "c":
                                paramsStr += "color,";
                            break;
                            case "_":
                                paramsStr = paramsStr.replace(/,$/,"");
                            case null:
                                continuar = false;
                        }
                        func = func.replace(re,"");
                    }
                }
                functions[name] = paramsStr;
//                fnStr = page[members[i]].toString();//Doesn't woks becoase of bug
//                //Took from here http://stackoverflow.com/questions/1007981/how-to-get-function-parameter-names-values-dynamically-from-javascript
//                arguments = fnStr.slice(fnStr.indexOf('(')+1, fnStr.indexOf(')')).match(/([^\s,]+)/g);
//                if(arguments === null)
//                    value = null;

            }
        }
    }

    onPageChanged: {
        functions = [];
        destroyTriggers();
        setFunctionNames();
        renderTriggers();
    }

    Component.onCompleted: {
        setFunctionNames();
        renderTriggers();
    }
}
