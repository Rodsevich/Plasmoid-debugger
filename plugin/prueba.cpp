#include <QObject>
#include <QFile>
#include <QTextStream>
#include "prueba.h"
#include <QDebug>
#include "debug_p.h"
#include "debug_prueba.h"

Prueba::Prueba(QObject* parent)
    : QObject(parent)
    , _calendar( new KCalCore::MemoryCalendar( KDateTime::UTC )){
//    QObject::connect(&this->_watcher, SIGNAL(fileChanged(QString)), this, SIGNAL(fileChanged()));
    QObject::connect(&this->_watcher, SIGNAL(fileChanged(QString)), this, SLOT(fileChangedSlot(QString)));
    m_customColorScheme["sorpi"] = 3;
    m_customColorScheme.insert("longa", "poronga");
//    CalendarToDo* c;
//    QString* sumario;
//    for(int i = 0; i < 3; i++){
//        c = new CalendarToDo(parent);
//        sumario = new QString();
//        *sumario = QString("sarasa %1").arg(i);
//        c->setSummary(*sumario);
//        c->setPriority(i * 2);
//        c->setPercentCompleted(i * 20);
//        this->listToDos.append(c);
//    }
//    QString* pp = new QString("VARIABLE SIN CAMBIAR");
//    this->setUri(*pp);
//    this->setLonga(*pp);
//    QString longa
//    this->setLonga(c->summary());
}

Prueba::~Prueba()
{
}

QString Prueba::uri(){
    return _uri;
}

void Prueba::append_todo(QQmlListProperty<CalendarToDo> *list, CalendarToDo* todo)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString* pp = new QString("uri cambiada, eh");
    calendar->setUri(*pp);
    QString debug = QString("intenté agregar un todo con sumario: ").append(todo->summary());
    calendar->setLonga(debug);
    if(calendar){
        todo->setParent(calendar);
        calendar->listToDos.append(todo);
    }
}

CalendarToDo *Prueba::at_todo(QQmlListProperty<CalendarToDo> *list, int index)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString debug = QString("intenté buscar el todo ").append(index);
    calendar->setLonga(debug);
    if (!calendar || index >= calendar->listToDos.count() || index < 0) {
        return 0;
    }  else {
        return calendar->listToDos.at(index);
    }
}

int Prueba::count_todo(QQmlListProperty<CalendarToDo> *list)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString *debug = new QString("intenté contar los todos");
    calendar->setLonga(*debug);
    if (calendar) {
        return calendar->listToDos.count();
    } else {
        return 0;
    }
}

void Prueba::clear_todo(QQmlListProperty<CalendarToDo> *list)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString* debug = new QString("intenté borrar los todos");
    calendar->setLonga(*debug);
    if(!calendar)
        return;
    calendar->listToDos.clear();
}

void Prueba::append_event(QQmlListProperty<CalendarEvent> *list, CalendarEvent* event)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString* pp = new QString("uri cambiada, eh");
    calendar->setUri(*pp);
    QString debug = QString("intenté agregar un Event con sumario: ").append(event->summary());
    calendar->setLonga(debug);
    if(calendar){
        event->setParent(calendar);
        calendar->listEvents.append(event);
    }
}

CalendarEvent *Prueba::at_event(QQmlListProperty<CalendarEvent> *list, int index)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString debug = QString("intenté buscar el Event ").append(index);
    calendar->setLonga(debug);
    if (!calendar || index >= calendar->listEvents.count() || index < 0) {
        return 0;
    }  else {
        return calendar->listEvents.at(index);
    }
}

int Prueba::count_event(QQmlListProperty<CalendarEvent> *list)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString *debug = new QString("intenté contar los Events");
    calendar->setLonga(*debug);
    if (calendar) {
        return calendar->listEvents.count();
    } else {
        return 0;
    }
}

void Prueba::clear_event(QQmlListProperty<CalendarEvent> *list)
{
    Prueba* calendar = qobject_cast<Prueba *>(list->object);
    QString* debug = new QString("intenté borrar los Events");
    calendar->setLonga(*debug);
    if(!calendar)
        return;
    calendar->listEvents.clear();
}

QQmlListProperty<CalendarToDo> Prueba::todos()
{
    return QQmlListProperty<CalendarToDo>(this, 0,
                                          &Prueba::append_todo,
                                          &Prueba::count_todo,
                                          &Prueba::at_todo,
                                          &Prueba::clear_todo);
}

QQmlListProperty<CalendarEvent> Prueba::events()
{
    return QQmlListProperty<CalendarEvent>(this, 0,
                                          &Prueba::append_event,
                                          &Prueba::count_event,
                                          &Prueba::at_event,
                                          &Prueba::clear_event);
}

void Prueba::setUri(QString &uri){
    if (_uri != uri){
        _watcher.removePath(_uri);
        _watcher.addPath(uri);
        _uri = uri;
        emit uriChanged();
    }
}

QString Prueba::longa(){
    return _longa;
}

void Prueba::setLonga(QString &longa){
    if (_longa != longa){
        _longa = longa;
    }
    m_arreglo.insert(longa, longa);
    emit arregloChanged(m_arreglo);
    emit longaChanged();
}

void Prueba::fileChangedSlot(QString file){
    _longa = file;
    emit fileChanged();
}

QString Prueba::leer(){
    QFile file(_uri);
    if(!file.open(QIODevice::ReadOnly))
        return "Uri mal??";

    QTextStream in(&file);
    QString ret;

    while(!in.atEnd())
        ret.append(in.readLine());

    file.close();

    return ret;
}

void Prueba::addToDo(CalendarToDo *todo)
{
    listToDos.append(todo);
    KCalCore::Todo::Ptr td(todo->get_object());
    _calendar->addTodo(td);
    emit todosChanged();
}

void Prueba::addEvent(CalendarEvent *event)
{
    listEvents.append(event);
    KCalCore::Event::Ptr evt(event->get_object());
    _calendar->addEvent(evt);
    emit eventsChanged();
}

bool Prueba::loadCalendar()
{
    if ( ! (_uri.endsWith(".ics") || _uri.endsWith(".vcs")))
        return false;
    if(!_storage)
        _storage = new KCalCore::FileStorage( _calendar, _uri );
    else
        _storage->setFileName(_uri);
    bool ret = _storage->load();

    if( ret){
        KCalCore::Todo::List todos(_calendar->todos());
        KCalCore::Event::List events(_calendar->events());

        int i;
        CalendarToDo *ctd;
        CalendarEvent *evt;
        for(i = 0; i < todos.count(); i++){
            ctd = new CalendarToDo(this->parent(), &*todos[i]);
            listToDos.append(ctd);
        }
        for(i = 0; i < events.count(); i++){
            evt = new CalendarEvent(this->parent(), &*events[i]);
            listEvents.append(evt);
        }

        return true;

    }else
        return false;
//    _calendar.clear();
//    if (_storage)
//        delete _storage;
//    KCalCore::MemoryCalendar::Ptr _calendar( new KCalCore::MemoryCalendar( KDateTime::UTC ));
    //    _storage = new KCalCore::FileStorage( _calendar, _uri );
}

bool Prueba::customColorRule(QString rule, QString color)
{
    qDebug() << "llamado a customColorRule( " << rule << " , " << color << " )";
    qCWarning(LOG_PLASMA) << "llamado a customColorRule( " << rule << " , " << color << " )";
    qCWarning(LOG_PRUEBA) << "llamado a customColorRule( " << rule << " , " << color << " )";
    bool already_existent;
    if( already_existent = m_customColorScheme.contains(rule))
        if(m_customColorScheme[rule] == color)
            return false;
    qCWarning(LOG_PRUEBA) << "existia antes: " << already_existent;
    qCWarning(LOG_PLASMA) << "existia antes: " << already_existent;
    m_customColorScheme.insert(rule, color);
    qCWarning(LOG_PRUEBA) << "Ahora tengo nº de reglas: " << m_customColorScheme.size();
    qCWarning(LOG_PLASMA) << "Ahora tengo nº de reglas: " << m_customColorScheme.size();
    emit customColorSchemeChanged();
    return !already_existent;
}

bool Prueba::saveCalendar(){

    //TODO: cuando arreglen el bug este, descomentar esto

//    _calendar->deleteAllTodos();
//    *_calendar->deleteAllEvents();

//    int i;
//    for(i = 0; i < listToDos.length(); i++){
//        *_calendar->addTodo(&*listToDos[i]->get_object());
//    }
//    for(i = 0; i < listEvents.length(); i++){
//        *_calendar->addEvent(&*listEvents[i]->get_object());
//    }
    return _storage->save();
}

QString Prueba::retTrue()
{
    QVariantMap pp;
    qCWarning(LOG_PRUEBA) << "Ahora tengo nº de reglas: " << m_customColorScheme.size();
    qCWarning(LOG_PLASMA) << "Ahora tengo nº de reglas: " << m_customColorScheme.size();
    pp.insert("sorp","longa");
    setArreglo(pp);
    return "cambiado";
}

QVariantMap Prueba::agregarArreglo(QString &key, QVariant &value)
{
    m_arreglo.insert(key, value);
    emit arregloChanged(m_arreglo);
    return m_arreglo;
}


//KCalCore::MemoryCalendar::Ptr cal(new KCalCore::MemoryCalendar(KDateTime::UTC));
//KCalCore::FileStorage store(cal, _uri); //El nombre del archivo como primer parametro

//store.open();
//store.load();

//    QDate* hoy = new QDate::Ptr(QDate::currentDate());
//QDate hoy = QDate::currentDate();
//KCalCore::Incidence::List incidencias(cal->incidences(hoy));
//KCalCore::Todo::List todos(cal->todos());

//int i,j;
//for(i = 0; i < todos.count(); i++){
//std::cout << todos[i]->uid().toStdString() << std::endl;
//}
