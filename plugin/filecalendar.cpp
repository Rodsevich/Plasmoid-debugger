#include "filecalendar.h"

void FileCalendar::addToDo(CalendarToDo *todo)
{
    listToDos.append(todo);
    KCalCore::Todo::Ptr td(todo->get_object());
    _calendar->addTodo(td);
    emit todosChanged();
}

void FileCalendar::addEvent(CalendarEvent *event)
{
    listEvents.append(event);
    KCalCore::Event::Ptr evt(event->get_object());
    _calendar->addEvent(evt);
    emit eventsChanged();
}

bool FileCalendar::loadCalendar()
{
    if ( ! _uri.endsWith(".ics"))
        return false;

//    _calendar.create(KDateTime::UTC);//(new KCalCore::MemoryCalendar( KDateTime::UTC ));
////    _storage(new KCalCore::FileStorage( _calendar, _uri, new KCalCore::ICalFormat()));
////    _storage.create( _calendar, _uri, new KCalCore::ICalFormat());
//    _storage = new KCalCore::FileStorage( _calendar, _uri, new KCalCore::ICalFormat());//.create(;

    _calendar->close();
    if( _storage->load() ){
        reloadEvents();
        reloadTodos();
        return true;
    }else
        return false;
}

void FileCalendar::reloadEvents()
{
    KCalCore::Event::List events(_calendar->rawEventsForDate(QDate::currentDate()));
    CalendarEvent *evt;
    listEvents.clear();
    for(int i = 0; i < events.count(); i++){
        evt = new CalendarEvent(this->parent(), this, &*events[i]);
        listEvents.append(evt);
    }
    emit eventsChanged();
}

void FileCalendar::reloadTodos()
{
    listToDos.clear();
    KCalCore::Todo::List todos(_calendar->rawTodos(KCalCore::TodoSortPriority, KCalCore::SortDirectionAscending));
    CalendarToDo *ctd;
    for(int i = 0; i < todos.count(); i++){
        ctd = new CalendarToDo(this->parent(), this, &*todos[i]);
        listToDos.append(ctd);
    }
    emit todosChanged();
}

bool FileCalendar::saveCalendar()
{
    return _storage->save();
}

QObject* FileCalendar::componentByUid(QString uid)
{
    for (int i = 0; i < listToDos.size(); i++) {
        if (listToDos.at(i)->uid() == uid){
           return listToDos.at(i);
        }
    }
    return nullptr;
}

bool FileCalendar::deleteIncidenceByUid(QString uid)
{
    KCalCore::Incidence::Ptr incidence(_calendar->incidence(uid));
    if(_calendar->deleteIncidence(incidence)){
        switch(incidence->type()){
            case KCalCore::Incidence::TypeTodo:
                delete componentByUid(uid);
                reloadTodos();
            break;
            case KCalCore::Incidence::TypeEvent:
                reloadEvents();
        }
        return true;
    }else
        return false;
}

QString FileCalendar::uri()
{
    return _uri;
}

void FileCalendar::setUri(QString &uri)
{
    if (_uri != uri){
        _watcher.removePath(_uri);
        _watcher.addPath(uri);
        _uri = uri;
        _storage->setFileName(uri);
        emit uriChanged();
        loadCalendar();
    }
}

QQmlListProperty<CalendarToDo> FileCalendar::todos()
{
    return QQmlListProperty<CalendarToDo>(this, 0,
					  &FileCalendar::append_todo,
					  &FileCalendar::count_todo,
					  &FileCalendar::at_todo,
					  &FileCalendar::clear_todo);
}

QQmlListProperty<CalendarEvent> FileCalendar::events()
{
    return QQmlListProperty<CalendarEvent>(this, 0,
					  &FileCalendar::append_event,
					  &FileCalendar::count_event,
					  &FileCalendar::at_event,
					  &FileCalendar::clear_event);
}

FileCalendar::FileCalendar(QObject* parent)
    : QObject(parent)
    , _calendar( new KCalCore::MemoryCalendar( KDateTime::UTC ) )
    , _storage(new KCalCore::FileStorage( _calendar, _uri, new KCalCore::ICalFormat()))
{
    QObject::connect(&this->_watcher, SIGNAL(fileChanged(QString)),
                     this, SLOT(fileChangedSlot()));
}

FileCalendar::~FileCalendar(){

}

void FileCalendar::fileChangedSlot()
{
    _watcher.removePath(_uri);
    _watcher.addPath(_uri);
    emit fileChanged();
}

void FileCalendar::append_todo(QQmlListProperty<CalendarToDo> *list, CalendarToDo *todo)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if(calendar){
        todo->setParent(calendar);
        calendar->listToDos.append(todo);
    }
}

CalendarToDo *FileCalendar::at_todo(QQmlListProperty<CalendarToDo> *list, int index)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if (!calendar || index >= calendar->listToDos.count() || index < 0) {
        return 0;
    }  else {
        return calendar->listToDos.at(index);
    }
}

int FileCalendar::count_todo(QQmlListProperty<CalendarToDo> *list)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if (calendar) {
        return calendar->listToDos.count();
    } else {
        return 0;
    }
}

void FileCalendar::clear_todo(QQmlListProperty<CalendarToDo> *list)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if(!calendar)
        return;
    calendar->listToDos.clear();
}

void FileCalendar::append_event(QQmlListProperty<CalendarEvent> *list, CalendarEvent *event)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if(calendar){
        event->setParent(calendar);
        calendar->listEvents.append(event);
    }
}

CalendarEvent *FileCalendar::at_event(QQmlListProperty<CalendarEvent> *list, int index)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if (!calendar || index >= calendar->listEvents.count() || index < 0) {
        return 0;
    }  else {
        return calendar->listEvents.at(index);
    }
}

int FileCalendar::count_event(QQmlListProperty<CalendarEvent> *list)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if (calendar) {
        return calendar->listEvents.count();
    } else {
        return 0;
    }
}

void FileCalendar::clear_event(QQmlListProperty<CalendarEvent> *list)
{
    FileCalendar* calendar = qobject_cast<FileCalendar *>(list->object);
    if(!calendar)
        return;
    calendar->listEvents.clear();
}
