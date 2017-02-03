#ifndef CALENDAR_H
#define CALENDAR_H

#include <QFile>
#include <QObject>
#include <QDebug>
#include <QTextStream>
#include <QQmlListProperty>
#include <QFileSystemWatcher>
#include <kcalcore/calendar.h>
#include <kcalcore/filestorage.h>
#include <kcalcore/memorycalendar.h>
#include <kcalcore/icalformat.h>
#include "calendartodo.h"
#include "calendarevent.h"

class Incidence;
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
    Q_INVOKABLE QObject* componentByUid(QString uid);
    Q_INVOKABLE bool deleteIncidenceByUid(QString uid);

    QString uri();
    void setUri(QString &uri);

    QQmlListProperty<CalendarToDo> todos();
    QQmlListProperty<CalendarEvent> events();

    FileCalendar(QObject* parent = 0);
    ~FileCalendar();

public slots:
    void fileChangedSlot();

Q_SIGNALS:
    void uriChanged();
    void fileChanged();
    void todosChanged();
    void eventsChanged();
    void todoSortFieldChanged();
    void sortDirectionChanged();

private:
    QString _uri;
    KCalCore::MemoryCalendar::Ptr _calendar;
    KCalCore::FileStorage* _storage = 0;
//    KCalCore::FileStorage::Ptr _storage;
    QList<CalendarEvent*> listEvents;
    QList<CalendarToDo*> listToDos;
    QFileSystemWatcher _watcher;

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
