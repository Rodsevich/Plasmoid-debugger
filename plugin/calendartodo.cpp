#include "calendartodo.h"

CalendarToDo::CalendarToDo(QObject *parent)
    : Incidence(parent){
    _object = new KCalCore::Todo();
    longa = new QString("ANDO!! (ToDo)");
//    QString ponga = new QString("ANDO!! (ToDo)");
//    this->longa = &ponga;
}

CalendarToDo::CalendarToDo(QObject *parent, KCalCore::Todo *todo)
    : Incidence(parent){
    _object = todo;
}

CalendarToDo::~CalendarToDo(){
    delete get_object();
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

QString CalendarToDo::escribirLongaToDo(){
    return *this->longa;
}

KCalCore::Todo *CalendarToDo::get_object()
{
    return this->_object;
}
