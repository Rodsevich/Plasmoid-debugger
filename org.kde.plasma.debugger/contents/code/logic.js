
function replacer(key, value) {
  if (value === null)
      return "null";
  if (typeof value === "object") {
    return "{" + Object.keys(value).join(", ") + "}";//"{}";//objKeys(value);
  }
  return value;
}

function objKeys(obj){
    var ownPNs = Object.getOwnPropertyNames(obj).sort();
    var keys = Object.keys(obj);
    var ret = "{ ";
    for(var i = 0, j = 0; i < keys.length; i++){
        while(ownPNs[i] !== keys[j])
            ret += keys[j++] + ", ";
        ret += '+' + keys[j++] + ", ";
    }
    return ret.replace(/, $/, " }");
}

function stringify(obj){
//    return objKeys(obj)//JSON.stringify(obj, replacer, 4);
    return JSON.stringify(obj, replacer, 4);
}

function debug(str, tty){
    tty = typeof tty !== undefined ? tty : false;
    if (tty){
        print(str);
        console.trace();
    }
//    if(plasmoid){
//        plasmoid.rootItem.debugOutput.text += str + "    ---- funcando con plasmoid.rootItem\n";
//    }else
        debugOutput.text = str + "\n" + debugOutput.text;
}


//function dialogAlerta(msg) {
//    dialogLoader.active = true;
//    dialogLoader.setSource("DialogContent.qml");
//    dialogLoader.item.titleText = "Alert!";
//    dialogLoader.item.messageText = msg;
//    dialogLoader.item.rejectButton = "";
//    dialogLoader.item.closeButton = "cerrar";
//    dialogLoader.item.acceptButton = "Understood";
//    dialogLoader.item.closed.connect(function(){ dialogLoader.active = false; });
//    enlazarBorrados();
//    dialogLoader.item.open();
//}

//function cerrarTodo(){
//    ponga.text = "deberia cerrar";
//    dialogLoader.active = false;
////    dialogLoader.source = "";
//}

//function enlazarBorrados(){
//    dialogLoader.item.closed.connect(cerrarTodo);
//    dialogLoader.item.accepted.connect(cerrarTodo);
//    dialogLoader.item.rejected.connect(cerrarTodo);
//}

//function dialogEleccion(msg) {
//    dialogLoader.active = true;
//    dialogLoader.setSource("DialogContent.qml",{
//                               "titleText":"Dialoguito",
//                               "messageText":msg,
//                               "rejectButton":"rechazar",
//                               "closeButton":"cerrame",
//                           });
//    dialogLoader.item.closed.connect(function(){ dialogLoader.active = false; });
//    dialogLoader.item.open();
//}

//function p(obj){
//    print(obj);
////    plasmoid.fullRepresentation.name.text = obj;
//}

//function dialogAlert(msg) {
////    print(Plasmoid);
////    print(plasmoid);
////    print(Plasmoid.fullRepresentation);
////    print(plasmoid.fullRepresentation);
////    p(dialogLoader);
//    print(dialogLoader);
//    var dialogLoader = Qt.createQmlObject('import QtQuick 2.5; Loader {}',
//        plasmoid.fullRepresentation, "dynamicLoader");
//    dialogLoader.onLoaded.connect(function(){
//        dialogLoader.item.onStatusChanged.connect(function unloadDialog(){
//            if (dialogLoader.item.status == PlasmaComponents.DialogStatus.Closed)
//                dialogLoader.source = "";
//        });
//        dialogLoader.item.open();
//    });
//    dialogLoader.setSource("Dialogs/Alert.qml", {"messageText":msg});
//    cosoFull.name.text = dialogLoader.status;
//}
