#ifndef CALENDARTODO_H
#define CALENDARTODO_H

#include "incidence.h"
#include <kcalcore/todo.h>
#include <QObject>

class CalendarToDo : public Incidence
{
    Q_OBJECT
    Q_PROPERTY(int percentCompleted READ percentCompleted WRITE setPercentCompleted NOTIFY percentCompletedChanged)
    Q_PROPERTY(bool overdue READ overdue)

public:

    Q_INVOKABLE QString escribirLongaToDo();

    CalendarToDo(QObject *parent = 0 );
    CalendarToDo(QObject *parent, KCalCore::Todo *todo);
    ~CalendarToDo();

    int percentCompleted();
    void setPercentCompleted(int percent);

    bool overdue();

    KCalCore::Todo *get_object();

Q_SIGNALS:
    void percentCompletedChanged();

private:

    KCalCore::Todo* _object;
    //    QString* longa;
};

#endif // CALENDARTODO_H
