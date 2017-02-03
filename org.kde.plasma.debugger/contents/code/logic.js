//Debug functions

var root,plasmoid;

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
    tty = typeof tty !== "undefined" ? tty : true;
    if (tty){
        print(str);
        console.trace();
    }
//    if(plasmoid){
//        plasmoid.rootItem.debugOutput.text += str + "    ---- funcando con plasmoid.rootItem\n";
//    }else
    if(/org\.kde\.plasma\.debugger/.test(plasmoid.file('')))//this is the debugger
        root.debugOutput.text = str + "\n" + root.debugOutput.text;
}

