#include "calendarevent.h"

CalendarEvent::CalendarEvent(QObject *parent)
    : Incidence(parent){
    _object = new KCalCore::Event();
}

CalendarEvent::CalendarEvent(QObject *parent, KCalCore::Event *event)
    : Incidence(parent){
    _object = event;
}

KCalCore::Event *CalendarEvent::get_object(){
    return this->_object;
}

CalendarEvent::~CalendarEvent(){
    delete get_object();
}

QDateTime CalendarEvent::startDateTime(){
    KDateTime ret = get_object()->dtStart();
    return ret.dateTime();
}

void CalendarEvent::setStartDateTime(QDateTime startDT){
    KDateTime *KstartDT = new KDateTime(startDT);
    if( get_object()->dtStart() != *KstartDT){
        get_object()->setDtStart(*KstartDT);
        emit secondsDurationChanged();
        emit startDateTimeChanged();
    }
}

QDateTime CalendarEvent::endDateTime(){
    KDateTime ret =  get_object()->dtEnd();
    return ret.dateTime();
}

void CalendarEvent::setEndDateTime(QDateTime endDT){
    KDateTime *KendDT = new KDateTime(endDT);
    if( get_object()->dtEnd() != *KendDT){
        get_object()->setDtEnd(*KendDT);
        emit secondsDurationChanged();
        emit endDateTimeChanged();
    }
}

int CalendarEvent::secondsDuration(){
    return get_object()->duration().asSeconds();
}
