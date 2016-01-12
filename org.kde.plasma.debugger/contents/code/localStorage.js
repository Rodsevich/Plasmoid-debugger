.import QtQuick.LocalStorage 2.0 as Sql;

var DB_version = "1.0"

function getDBConnection(){
    var db = Sql.LocalStorage.openDatabaseSync("TomaToDoING", DB_version, "DB for storing Pomodoros done, tasks, etc...", 10000, createDBIfNotExists);
    return db
}

function createDBIfNotExists(db){
    debug("Creating DB...");
    var tables = [
        "CREATE TABLE Tasks(\
            `id`                INTEGER PRIMARY KEY,
            `name`              TEXT NOT NULL UNIQUE,
            `creationTime`      TEXT NOT NULL,
            `progress`          INTEGER DEFAULT 0,
            `finished_on`       TEXT DEFAULT 0
        )",
        "CREATE TABLE Registry(\
            `id`                INTEGER PRIMARY KEY,
            `day`               TEXT NOT NULL DEFAULT (date('now', 'localtime')),
            `type`              INTEGER NOT NULL,
            `start_time`        TEXT NOT NULL DEFAULT (time('now', 'localtime')),
            `end_time`          TEXT,
            `task`              INTEGER,
            FOREIGN KEY(`task`) REFERENCES Tasks ( id )
        )"
    ];
    db.transaction( function(tx) {
        for( var i in tables){
            debug(tx.executeSql(tables[i]));
        }
    });
    db.changeVersion("", DB_version);
}

function insertTask(name){
    debug("metiendo.. " + name);
    var ret;
    var db = getDBConnection();
    var query = "INSERT INTO Tasks (creationTime, name) VALUES (datetime('now','localtime'), ?)";
    db.transaction( function(tx) {
        ret = tx.executeSql(query, name);
    });
    debug(ret);
    return ret.insertId;
}

function getTasks(){
    var ret = [];
    var db = getDBConnection();
    var query = "SELECT * FROM Tasks";
    db.transaction( function(tx) {
        var aux = tx.executeSql(query);
        for( var i = 0; i < aux.rows.length; i++){
            ret[aux.rows.item(i).id] = aux.rows.item(i);
        }
    });
    debug(ret);
    return ret;
}

function getTask(name){
    var ret;
    var db = getDBConnection();
    var query = "SELECT * FROM Tasks WHERE name = ?";
    db.transaction( function(tx) {
        ret = tx.executeSql(query, name);
    });
    debug(ret.rows.item(0));
    return ret.rows.item(0);
}

function getUnfinishedRegistryRow(){
    var ret;
    var db = getDBConnection();
    var query = "SELECT * FROM Registry WHERE end_time IS NULL";
    db.transaction( function(tx) {
        ret = tx.executeSql(query);
    });
    if(ret.rows.length > 1)
        throw "DB inconsistency: More than one null row in Registry"
    else if(ret.rows.length === 0){
        debug("none registry unfinished");
        return null;
    }
    debug(ret.rows.item(0));
    return ret.rows.item(0);
}

function finishCurrentRegistry(id){
    if( id === undefined ){
        var aux = getUnfinishedRegistryRow();
        if( aux === null )
            return false;
        else
            id = aux.id;
    }
    var ret;
    var db = getDBConnection();
    var query = "UPDATE Registry SET end_time = time('now', 'localtime') WHERE id = " + id;
    debug("Query: " + query);
    db.transaction( function(tx) {
        ret = tx.executeSql(query);
    });
    debug(ret.rowsAffected);
    if(ret.rowsAffected === 1)
        return true;
    else
        return false;
}

var insertIntoRegistry = function(type, task){ //[type] 0 = donePomo, 1 = break, 2 = pause
    var ret;
    var db = getDBConnection();
    debug(task);
    var query = "INSERT INTO Registry(type, task) VALUES (?, ?)";
    db.transaction( function(tx) {
        ret = tx.executeSql(query, [type, task.id]);
    });
    debug(ret);
    return ret.insertId;
}

function getPastDayRegistryRow(day){
    var ret = [];
    var db = getDBConnection();
//    var query = "SELECT datetime('now','localtime','start of day','-"+ day + " days') AS desde,\
//                 datetime('now','localtime','start of day','-"+ (day - 1) +" days') AS hasta,\
//                 count(*) AS totalInDay FROM donePomos\
//                 WHERE finishedTime BETWEEN\
//                 datetime('now','localtime','start of day','-"+ day +" days') AND\
//                 datetime('now','localtime','start of day','-"+ (day - 1) +" days')";
    var query = "SELECT * FROM Registry WHERE day = date('now','localtime','-"+ day +" days')";

    db.transaction( function(tx) {
        var results = tx.executeSql(query);
        for( var i = 0; i < results.rows.length; i++)
            ret.push(results.rows.item(i));
    });
    debug(ret);
    return ret;
}

function startPomodoro(task){
    if(typeof task == "string")
        task = getTask(task);
    var currentRow = getUnfinishedRegistryRow();
    if(currentRow !== null)
        finishCurrentRegistry(currentRow.id);
    insertIntoRegistry(0, task.id);
}

function startBreak(task){
    //If starting break, it's becoase a pomodoro has been started before
    finishCurrentRegistry();
    insertIntoRegistry(1, getTask(task));
}

function startPause(task){
    //If starting pause, it's becoase something else has been started before
    finishCurrentRegistry();
    insertIntoRegistry(2, getTask(task));
}
