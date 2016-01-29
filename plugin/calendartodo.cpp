#include "calendartodo.h"

QDateTime CalendarToDo::completedDate()
{
    return get_object()->completed().dateTime();
}

CalendarToDo::CalendarToDo(QObject *parent)
    : Incidence(parent){
    _object = new KCalCore::Todo();
}

CalendarToDo::CalendarToDo(QObject *parent, FileCalendar *calendar, KCalCore::Todo *todo)
    : Incidence(parent, calendar){
    _object = todo;
}

CalendarToDo::~CalendarToDo(){
//    delete _object;
}

int CalendarToDo::percentCompleted(){
    return get_object()->percentComplete();
}

void CalendarToDo::setPercentCompleted(int percent){
    get_object()->setPercentComplete(percent);
}

bool CalendarToDo::overdue(){
    return get_object()->isOverdue();
}

bool CalendarToDo::completed(){
    return get_object()->isCompleted();
}

void CalendarToDo::setCompleted(bool completed)
{
    get_object()->setCompleted(completed);
    emit completedChanged();
}

void CalendarToDo::setCompletedDate(QDateTime completedDate)
{
    get_object()->setCompleted(new KDateTime(completedDate));
    emit completedDateChanged();
}

KCalCore::Todo *CalendarToDo::get_object()
{
    return this->_object;
}
