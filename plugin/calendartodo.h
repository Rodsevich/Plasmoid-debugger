#ifndef CALENDARTODO_H
#define CALENDARTODO_H

#include "incidence.h"
#include <kcalcore/todo.h>
#include <QObject>
#include <QDateTime>

class FileCalendar;
class CalendarToDo : public Incidence{
    Q_OBJECT
    Q_PROPERTY(int percentCompleted READ percentCompleted WRITE setPercentCompleted NOTIFY percentCompletedChanged)
    Q_PROPERTY(bool overdue READ overdue NOTIFY overdueChanged)
    Q_PROPERTY(bool completed READ completed WRITE setCompleted NOTIFY completedChanged )
    Q_PROPERTY(QDateTime completedDate READ completedDate WRITE setCompletedDate NOTIFY completedDateChanged)

public:

    CalendarToDo(QObject *parent = 0 );
    CalendarToDo(QObject *parent, FileCalendar *calendar, KCalCore::Todo *todo);
    ~CalendarToDo();

    int percentCompleted();
    void setPercentCompleted(int percent);

    bool overdue();

    bool completed();
    void setCompleted(bool completed);

    QDateTime completedDate();
    void setCompletedDate(QDateTime completedDate);

    KCalCore::Todo *get_object();

Q_SIGNALS:
    void overdueChanged();
    void completedChanged();
    void completedDateChanged();
    void percentCompletedChanged();

private:

    KCalCore::Todo* _object;
};

#endif // CALENDARTODO_H
