#ifndef CALENDAREVENT_H
#define CALENDAREVENT_H

#include "incidence.h"
#include <kcalcore/event.h>
#include <QObject>

class FileCalendar;
class CalendarEvent : public Incidence
{
    Q_OBJECT
    Q_PROPERTY(QDateTime startDateTime READ startDateTime WRITE setStartDateTime NOTIFY startDateTimeChanged)
    Q_PROPERTY(QDateTime endDateTime READ endDateTime WRITE setEndDateTime NOTIFY endDateTimeChanged)
    Q_PROPERTY(int secondsDuration READ secondsDuration NOTIFY secondsDurationChanged)

public:

    CalendarEvent(QObject *parent = 0 );
    CalendarEvent(QObject *parent, FileCalendar *calendar, KCalCore::Event *event);
    ~CalendarEvent();

    QDateTime startDateTime();
    void setStartDateTime(QDateTime startDT);

    QDateTime endDateTime();
    void setEndDateTime(QDateTime endDT);

    int secondsDuration();

    KCalCore::Event *get_object();

Q_SIGNALS:
    void startDateTimeChanged();
    void endDateTimeChanged();
    void secondsDurationChanged();

private:

    KCalCore::Event* _object;

};

#endif // CALENDAREVENT_H
