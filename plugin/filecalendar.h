#ifndef CALENDAR_H
#define CALENDAR_H

#include <QFile>
#include <QObject>
#include <QTextStream>
#include <QQmlListProperty>
#include <QFileSystemWatcher>
#include <kcalcore/calendar.h>
#include <kcalcore/filestorage.h>
#include <kcalcore/memorycalendar.h>
#include "calendartodo.h"
#include "calendarevent.h"

class FileCalendar : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QQmlListProperty<CalendarToDo> todos READ todos NOTIFY todosChanged())
    Q_PROPERTY(QQmlListProperty<CalendarEvent> events READ events NOTIFY eventsChanged())

public:
    Q_INVOKABLE void addToDo(CalendarToDo* todo);
    Q_INVOKABLE void addEvent(CalendarEvent* event);
    Q_INVOKABLE bool loadCalendar();
    Q_INVOKABLE bool saveCalendar();

    QString uri();
    void setUri(QString &uri);

    QQmlListProperty<CalendarToDo> todos();
    QQmlListProperty<CalendarEvent> events();

    FileCalendar(QObject* parent = 0);
    ~FileCalendar();
//    QString leer();

public slots:
    void fileChangedSlot(QString file);

Q_SIGNALS:
    void uriChanged();
    void fileChanged();
    void todosChanged();
    void eventsChanged();

private:
    QString _uri;
    KCalCore::MemoryCalendar::Ptr _calendar;
    KCalCore::FileStorage* _storage = 0;
    QFileSystemWatcher _watcher;
    QList<CalendarToDo*> listToDos;
    QList<CalendarEvent*> listEvents;

    void reloadEvents();
    void reloadTodos();

    static void append_todo(QQmlListProperty<CalendarToDo> *list, CalendarToDo *todo);
    static CalendarToDo *at_todo(QQmlListProperty<CalendarToDo> *list, int index);
    static int count_todo(QQmlListProperty<CalendarToDo> *list);
    static void clear_todo(QQmlListProperty<CalendarToDo> *list);

    static void append_event(QQmlListProperty<CalendarEvent> *list, CalendarEvent *event);
    static CalendarEvent *at_event(QQmlListProperty<CalendarEvent> *list, int index);
    static int count_event(QQmlListProperty<CalendarEvent> *list);
    static void clear_event(QQmlListProperty<CalendarEvent> *list);
};

#endif // CALENDAR_H
