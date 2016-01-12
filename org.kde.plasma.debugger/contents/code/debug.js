// jsconsole.js
.pragma library

function call(msg) {
    var exp = msg.toString();
    console.log(exp)
    var data = {
        expression : msg
    }
    try {
        var fun = new Function('return (' + exp + ');');
        data.result = JSON.stringify(fun.call(main), null, 2)
        console.log('scope: ' + JSON.stringify(main, null, 2) + 'result: ' + result)
    } catch(e) {
        console.log(e.toString())
        data.error = e.toString();
    }
    return data;
}
